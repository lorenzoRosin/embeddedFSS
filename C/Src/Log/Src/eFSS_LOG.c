/**
 * @file       eFSS_LOG.h
 *
 * @brief      LOG module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_LOG.h"
#include "eFSS_Utils.h"

/* In this module the page field has the following meaning:
 *
 *  --> NEWEST PAGE OR NEWEST BACKUP PAGES
 * ------------------------------------------------------------------ User data
 * - [uint8_t] -                    -> N byte of user data           |
 * ------------------------------------------------------------------ Metadata  (4 byte)|
 * - uint32_t  - AlreadyFilled P    -> Already filled pages          |
 * ------------------------------------------------------------------ Under we have LL/HL/LOGC metadata
 * - LOW LEVEL / HIGH LEVEL / LOGC METADATA                          |
 * ------------------------------------------------------------------ End of Page
 *
 *  --> LOG PAGES
 * ------------------------------------------------------------------ User data
 * - [uint8_t] -                    -> N byte of user data           |
 * ------------------------------------------------------------------ Under we have LL/HL/LOGC metadata
 * - LOW LEVEL / HIGH LEVEL / LOGC METADATA                           |
 * ------------------------------------------------------------------ End of Page
 *
 * This module is used to store LOGS. Remeber to check where the last log is stored using the flash cache if enabled.
 * If flash cache is not enabled the last log pages must be searched inside all log.
 * After a newest page is finalized and becames a log pages we must remove the last 4 byte used in the newest pages.
 * Keep in mind that the last 4 byte in the newest page are not counted in the field called: byte in page used by
 * LOGC module.
 * An unused page is always left after newer page and backup index.
 * Newest and oldest page index can be identical only when the log storage is empty. When the storage is full an empty
 * page is always present between new and old log index.
 * Es of full :
 * LOG | NEWEST PAGE | NEWEST PAGE BLUP | FREE PAGE | OLDEST LOG | ...
 *
 * uPageUseSpec2 (Number of Filled index page, or p_ptCtx->uFullFilledP) is calculated without counting the newest page
 * ES:
 * LOG | NEWEST PAGE | NEWEST PAGE BLUP | FREE PAGE | OLDEST LOG | LOG
 * uPageUseSpec2 = 3
 */



/***********************************************************************************************************************
 *      PRIVATE DEFINE
 **********************************************************************************************************************/
#define EFSS_LOG_MINPAGESIZE                                                              ( ( uint32_t )         0x05u )
#define EFSS_LOG_FILLPOFF                                                                 ( ( uint32_t )         0x04u )
#define EFSS_LOG_NEWBKPEMPY_P                                                             ( ( uint32_t )         0x03u )



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_LOG_IsStatusStillCoherent(t_eFSS_LOG_Ctx* const p_ptCtx);
static e_eFSS_LOG_RES eFSS_LOG_LOGCtoLOGRes(const e_eFSS_LOGC_RES p_eLOGCRes);



/***********************************************************************************************************************
 *  PRIVATE UTILS STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static e_eFSS_LOG_RES eFSS_LOG_LoadIndexNRepair(t_eFSS_LOG_Ctx* const p_ptCtx);
static e_eFSS_LOG_RES eFSS_LOG_LoadIndexFromCache(t_eFSS_LOG_Ctx* const p_ptCtx);
static e_eFSS_LOG_RES eFSS_LOG_LoadIndxBySearch(t_eFSS_LOG_Ctx* const p_ptCtx);
static e_eFSS_LOG_RES eFSS_LOG_GetNextIndex(t_eFSS_LOG_Ctx* const p_ptCtx, const uint32_t p_uCurIdx,
                                            uint32_t* const p_uNextIdx);
static e_eFSS_LOG_RES eFSS_LOG_GetPrevIndex(t_eFSS_LOG_Ctx* const p_ptCtx, const uint32_t p_uCurIdx,
                                            uint32_t* const p_puPrevIdx);



/***********************************************************************************************************************
 *  PRIVATE UTILS FOR BKUP PAGE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static e_eFSS_LOG_RES eFSS_LOG_FlushBufferAsNewestNBkpPage(t_eFSS_LOG_Ctx* const p_ptCtx, const uint32_t p_uIdx,
                                                           const uint32_t p_uByteInPage);
static e_eFSS_LOG_RES eFSS_LOG_LoadBufferAsNewestNBkpPage(t_eFSS_LOG_Ctx* const p_ptCtx, const uint32_t p_uIdx,
                                                          uint32_t* const p_puByteInPage);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_LOG_RES eFSS_LOG_InitCtx(t_eFSS_LOG_Ctx* const p_ptCtx, const t_eFSS_TYPE_CbStorCtx p_tCtxCb,
                                const t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff,
                                const uint32_t p_uBuffL, const bool_t p_bFlashCache, const bool_t p_bFullBckup)
{
    /* return local variable */
    e_eFSS_LOG_RES  l_eRes;
    e_eFSS_LOGC_RES l_eResC;

    /* Local storage variable */
    t_eFSS_LOGC_StorBuf l_tBuff;
    uint32_t l_uUsePages;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
        /* Can init low level context */
        l_eResC = eFSS_LOGC_InitCtx(&p_ptCtx->tLOGCCtx, p_tCtxCb, p_tStorSet, p_puBuff, p_uBuffL,
                                    p_bFlashCache, p_bFullBckup);
        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            /* Get usable pages and buffer length so we can check blob default value validity */
            l_uUsePages = 0u;
            l_eResC = eFSS_LOGC_GetBuffNUsable(&p_ptCtx->tLOGCCtx, &l_tBuff, &l_uUsePages);
            l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

            if( e_eFSS_LOG_RES_OK == l_eRes )
            {
                /* Check if page length is OK */
                if( l_tBuff.uBufL < EFSS_LOG_MINPAGESIZE )
                {
                    /* We need more space for the BLOB */
                    l_eRes = e_eFSS_LOG_RES_BADPARAM;

                    /* De init LOGC */
                    (void)memset(&p_ptCtx->tLOGCCtx, 0, sizeof(t_eFSS_LOGC_Ctx));
                }
                else
                {
                    /* All ok, fill context */
                    p_ptCtx->uNewPagIdx = 0;
                    p_ptCtx->uFullFilledP = 0;
                }
            }
            else
            {
                /* De init LOGC */
                (void)memset(&p_ptCtx->tLOGCCtx, 0, sizeof(t_eFSS_LOGC_Ctx));
            }
        }
    }

    return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_IsInit(t_eFSS_LOG_Ctx* const p_ptCtx, bool_t* const p_pbIsInit)
{
	/* Local variable */
    e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES l_eResC;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
        l_eResC = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, p_pbIsInit);
        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);
	}

	return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_GetLogStatus(t_eFSS_LOG_Ctx* const p_ptCtx)
{
	/* Local return variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES l_eResC;

    /* Local var used for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eResC = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, &l_bIsInit);
        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOG_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOG_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOG_RES_CORRUPTCTX;
                }
                else
                {
                    /* How to know the status of the storage? Load storage index, if they are not found the storage
                       is corrupted or non initialized */
                    l_eRes = eFSS_LOG_LoadIndexNRepair(p_ptCtx);
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_GetLogInfo(t_eFSS_LOG_Ctx* const p_ptCtx, uint32_t* const p_puNewLogI,
                                   uint32_t* const p_puOldLogI, uint32_t* const p_puNpageUsed,
                                   uint32_t* const p_puNpageTot)
{
	/* Local return variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES l_eResC;

    /* Local storage variable */
    t_eFSS_LOGC_StorBuf l_tBuff;
    uint32_t l_uUsePages;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    uint32_t l_uRemain;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puNewLogI ) || ( NULL == p_puOldLogI ) || ( NULL == p_puNpageUsed ) ||
        ( NULL == p_puNpageTot ) )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eResC = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, &l_bIsInit);
        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOG_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOG_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOG_RES_CORRUPTCTX;
                }
                else
                {
                    /* Verify storage integrity and load in the context the log index */
                    l_eRes = eFSS_LOG_LoadIndexNRepair(p_ptCtx);
                    if( e_eFSS_LOG_RES_OK == l_eRes )
                    {
                        /* Calculate n page */
                        l_uUsePages = 0u;
                        l_eResC = eFSS_LOGC_GetBuffNUsable(&p_ptCtx->tLOGCCtx, &l_tBuff, &l_uUsePages);
                        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

                        if( e_eFSS_LOG_RES_OK == l_eRes )
                        {
                            /* New log index fill */
                            *p_puNewLogI = p_ptCtx->uNewPagIdx;

                            /* Oldest log index fill */
                            if( p_ptCtx->uNewPagIdx < p_ptCtx->uFullFilledP )
                            {
                                l_uRemain = p_ptCtx->uFullFilledP - p_ptCtx->uNewPagIdx;
                                *p_puOldLogI = l_uUsePages - l_uRemain;
                            }
                            else
                            {
                                *p_puOldLogI = p_ptCtx->uNewPagIdx - p_ptCtx->uFullFilledP;
                            }

                            /* Numbers of filled page fill */
                            *p_puNpageUsed = p_ptCtx->uFullFilledP;

                            /* Total pages fill */
                            *p_puNpageTot = l_uUsePages;
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_Format(t_eFSS_LOG_Ctx* const p_ptCtx)
{
	/* Local return variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES l_eResC;

    /* Local storage variable */
    t_eFSS_LOGC_StorBuf l_tBuff;
    uint32_t l_uUsePages;

    /* Local var used for calculation */
    bool_t l_bIsInit;

    /* Local variable for decision making */
    bool_t l_bInvalidateCurrent;
    bool_t l_bIsFlashCacheUsed;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eResC = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, &l_bIsInit);
        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOG_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOG_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOG_RES_CORRUPTCTX;
                }
                else
                {
                    l_uUsePages = 0u;
                    l_eResC = eFSS_LOGC_GetBuffNUsable(&p_ptCtx->tLOGCCtx, &l_tBuff, &l_uUsePages);
                    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

                    if( e_eFSS_LOG_RES_OK == l_eRes )
                    {
                        /* Load index if possible, why? Because if we start formatting and a power outage occour
                           we need that the log must remain recoverable or must be totaly formatted (or corrupted
                           also). We dont want that a power outage could leave the area in state where wrong data
                           could be readed */
                        l_eRes = eFSS_LOG_LoadIndexNRepair(p_ptCtx);

                        if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) )
                        {
                            /* Flash cache, so we find a valid newest page pointed by cache.
                               1 - If the newest page is at index zero just format that page at index zero, withouth
                                   modifyng the caches
                               2 - If the newest page is not at index zero invalidate the current page (during a power
                                   outage the state of the storage will be invalid) write newest page on index zero,
                                   and finaly update the flash cache (even if a power outage occours, and the state of
                                   the storage will be invalid, no problem: we were formatting the storage anyway.. ) */

                            /* No flash cache, so we find a valid newest page.
                               1 - If the newest page is at index zero just format that page otherwise invalidate
                               2 - If the newest page is not at index zero invalidate the current page and just after
                                   write newest on index zero */

                            if( 0u != p_ptCtx->uNewPagIdx )
                            {
                                l_bInvalidateCurrent = true;
                            }
                            else
                            {
                                l_bInvalidateCurrent = false;
                            }
                        }
                        else if( e_eFSS_LOG_RES_NOTVALIDLOG == l_eRes )
                        {
                            /* Flash cache: We could be in the cases where chache has bad index.
                               first write newest page in index zero and after update caches */

                            /* No flash cache: no newest found. just write newest page in index 0 */

                            l_bInvalidateCurrent = false;
                        }
                        else
                        {
                            /* Found some strange error, cannot continue */
                            l_bInvalidateCurrent = true;
                        }

                        /* Start the procedure */
                        if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) ||
                            ( e_eFSS_LOG_RES_NOTVALIDLOG == l_eRes ) )
                        {
                            /* Invalidate the current pages if needed */
                            if( true == l_bInvalidateCurrent )
                            {
                                /* Clear buffer  */
                                memset(l_tBuff.puBuf, 0u, l_tBuff.uBufL);

                                /* Flush */
                                l_eRes = eFSS_LOG_FlushBufferAsNewestNBkpPage(p_ptCtx, p_ptCtx->uNewPagIdx, 0u);
                            }

                            if( e_eFSS_LOG_RES_OK == l_eRes )
                            {
                                /* Write newest in first index */
                                p_ptCtx->uNewPagIdx = 0u;
                                p_ptCtx->uFullFilledP = 0u;

                                /* Clear buffer  */
                                memset(l_tBuff.puBuf, 0u, l_tBuff.uBufL);

                                /* Flush */
                                l_eRes = eFSS_LOG_FlushBufferAsNewestNBkpPage(p_ptCtx, p_ptCtx->uNewPagIdx, 0u);

                                if( e_eFSS_LOG_RES_OK == l_eRes )
                                {
                                    /* write cache, if needed */
                                    l_eResC =  eFSS_LOGC_IsFlashCacheUsed(&p_ptCtx->tLOGCCtx, &l_bIsFlashCacheUsed);
                                    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);
                                    if( e_eFSS_LOG_RES_OK == l_eRes )
                                    {
                                        if( true == l_bIsFlashCacheUsed )
                                        {
                                            l_eResC = eFSS_LOGC_WriteCache(&p_ptCtx->tLOGCCtx, 0u, 0u);
                                            l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_AddLog(t_eFSS_LOG_Ctx* const p_ptCtx, uint8_t* const p_puRawVal, const uint32_t p_uElemL)
{
	/* Local return variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES l_eResC;

    /* Local storage variable */
    t_eFSS_LOGC_StorBuf l_tBuff;
    uint32_t l_uUsePages;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    bool_t l_bIsFlashC;
    uint32_t l_uByteInPage;
    uint32_t l_uNextIdx;
    uint32_t l_uNextNextIdx;

    /* Decision making flag */
    bool_t l_bNextBeforeSave;
    bool_t l_bNextAfterSave;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puRawVal ) )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eResC = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, &l_bIsInit);
        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOG_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOG_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOG_RES_CORRUPTCTX;
                }
                else
                {
                    l_uUsePages = 0u;
                    l_eResC = eFSS_LOGC_GetBuffNUsable(&p_ptCtx->tLOGCCtx, &l_tBuff, &l_uUsePages);
                    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

                    if( e_eFSS_LOG_RES_OK == l_eRes )
                    {
                        /* Check data validity */
                        if( (p_uElemL <= 0u ) || ( p_uElemL > l_tBuff.uBufL ) )
                        {
                            l_eRes = e_eFSS_LOG_RES_BADPARAM;
                        }
                        else
                        {
                            /* Need to load latest index in order to do this */
                            l_eRes = eFSS_LOG_LoadIndexNRepair(p_ptCtx);
                            if( e_eFSS_LOG_RES_OK == l_eRes )
                            {
                                /* Read current newest page:
                                 *    We have no space: Go next, and save log in next page
                                 *    We have space: Save log in the current page, and if the avaiable space after
                                 *                   saving is less than the size of filled page go next page
                                 */
                                l_eRes = eFSS_LOG_LoadBufferAsNewestNBkpPage(p_ptCtx, p_ptCtx->uNewPagIdx,
                                                                             &l_uByteInPage);

                                if( e_eFSS_LOG_RES_OK == l_eRes )
                                {
                                    /* Choose what we have to do */
                                    if( p_uElemL > ( l_tBuff.uBufL - l_uByteInPage ) )
                                    {
                                        /* Need to save the element on the next page */
                                        l_bNextBeforeSave = true;

                                        /* Need to skip even after the saving? */
                                        if( p_uElemL > ( l_tBuff.uBufL - EFSS_LOG_FILLPOFF ) )
                                        {
                                            /* Next page even after the save */
                                            l_bNextAfterSave = true;
                                        }
                                        else
                                        {
                                            l_bNextAfterSave = false;
                                        }
                                    }
                                    else
                                    {
                                        /* can save the element on the current page, need to choose if we need to go
                                           next page after saving */
                                           l_bNextBeforeSave = false;

                                           if( ( l_tBuff.uBufL - l_uByteInPage - p_uElemL ) <= EFSS_LOG_FILLPOFF )
                                           {
                                                l_bNextAfterSave = true;
                                           }
                                           else
                                           {
                                                l_bNextAfterSave = false;
                                           }
                                    }

                                    /* Ok, so now we have al cases and we can proceed with reading and writing pages */
                                    if( ( true == l_bNextBeforeSave ) && ( true == l_bNextAfterSave ) )
                                    {
                                        /* No space detected */
                                    }
                                    else if( ( true == l_bNextBeforeSave ) && ( false == l_bNextAfterSave ) )
                                    {
                                        /* No space detected */
                                    }
                                    /* Ok, so now we have al cases and we can proceed with reading and writing pages */
                                    else if( ( false == l_bNextBeforeSave ) && ( true == l_bNextAfterSave ) )
                                    {
                                        /* We have space to append the log but just after we need to increase log
                                         * page:
                                         *   1 - Append the log and flush the page as LOG page
                                         *   2 - Add a Newest backup page after the current newest bkp page ( so in the
                                         *       currently unused page )
                                         *   3 - If cache is present update the newest index and the filled page number
                                         *   4 - Transform the old newest page bkup in to the newest page only
                                         */

                                        memcpy(&l_tBuff.puBuf[l_uByteInPage], p_puRawVal, p_uElemL);
                                        l_uByteInPage += p_uElemL;

                                        l_eResC = eFSS_LOGC_FlushBufferAs(&p_ptCtx->tLOGCCtx, e_eFSS_LOGC_PAGETYPE_LOG,
                                                                          p_ptCtx->uNewPagIdx, l_uByteInPage);
                                        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

                                        if( e_eFSS_LOG_RES_OK == l_eRes )
                                        {
                                            /* Step 2 - add the new bkup page after the current bkup page */
                                            if( e_eFSS_LOG_RES_OK == eFSS_LOG_GetNextIndex(p_ptCtx, p_ptCtx->uNewPagIdx,
                                                                                           &l_uNextIdx) )
                                            {
                                                if( e_eFSS_LOG_RES_OK == eFSS_LOG_GetNextIndex(p_ptCtx, l_uNextIdx,
                                                                                               &l_uNextNextIdx) )
                                                {
                                                    memset(l_tBuff.puBuf, 0u, l_tBuff.uBufL);
                                                    l_uByteInPage = 0u;

                                                    if( true != eFSS_Utils_InsertU32(&l_tBuff.puBuf[l_tBuff.uBufL - EFSS_LOG_FILLPOFF], p_ptCtx->uFullFilledP) )
                                                    {
                                                        l_eRes = e_eFSS_LOG_RES_CORRUPTCTX;
                                                    }
                                                    else
                                                    {
                                                        l_eResC = eFSS_LOGC_FlushBufferAs(&p_ptCtx->tLOGCCtx, e_eFSS_LOGC_PAGETYPE_NEWEST_BKUP,
                                                                                          l_uNextNextIdx, l_uByteInPage);
                                                        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

                                                        if( e_eFSS_LOG_RES_OK == l_eRes )
                                                        {
                                                            l_eResC = eFSS_LOGC_IsFlashCacheUsed(&p_ptCtx->tLOGCCtx, &l_bIsFlashC);
                                                            l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

                                                            if( e_eFSS_LOG_RES_OK == l_eRes )
                                                            {
                                                                /* Step 3 - If cache is present update cache with the new index */
                                                                if( true == l_bIsFlashC )
                                                                {
                                                                    l_eResC = eFSS_LOGC_WriteCache(&p_ptCtx->tLOGCCtx, l_uNextIdx, p_ptCtx->uFullFilledP + 1u);
                                                                    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);
                                                                }

                                                                if( e_eFSS_LOG_RES_OK == l_eRes )
                                                                {
                                                                    /* Step 4 - Trasform the backup page in the original pages */
                                                                    l_eResC = eFSS_LOGC_FlushBufferAs(&p_ptCtx->tLOGCCtx, e_eFSS_LOGC_PAGETYPE_NEWEST,
                                                                                                      l_uNextIdx, l_uByteInPage);
                                                                    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        /* Simpliest cases, no new page before adding log, and no new page after
                                           saving the log, not even needed to update the flash cache */

                                        /* Add log in to the buffer */
                                        memcpy(&l_tBuff.puBuf[l_uByteInPage], p_puRawVal, p_uElemL);

                                        /* Increase the byte in page counter */
                                        l_uByteInPage += p_uElemL;

                                        /* Save */
                                        l_eRes = eFSS_LOG_FlushBufferAsNewestNBkpPage(p_ptCtx, p_ptCtx->uNewPagIdx,
                                                                                      l_uByteInPage);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_GetLogOfAPage(t_eFSS_LOG_Ctx* const p_ptCtx, const uint32_t p_uindx, uint8_t* const p_puBuf,
                                      const uint32_t p_uBufL, uint32_t* const p_puValorByte)
{
	/* Local return variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES l_eResC;

    /* Local storage variable */
    t_eFSS_LOGC_StorBuf l_tBuff;
    uint32_t l_uUsePages;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    uint32_t l_uByteInPages;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuf ) || ( NULL == p_puValorByte ) )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eResC = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, &l_bIsInit);
        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOG_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOG_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOG_RES_CORRUPTCTX;
                }
                else
                {
                    /* First get basic data in order to verify passed param validity */
                    l_uUsePages = 0u;
                    l_eResC = eFSS_LOGC_GetBuffNUsable(&p_ptCtx->tLOGCCtx, &l_tBuff, &l_uUsePages);
                    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

                    if( e_eFSS_LOG_RES_OK == l_eRes )
                    {
                        if( ( p_uindx >= l_uUsePages ) || ( p_uBufL < l_tBuff.uBufL ) )
                        {
                            l_eRes = e_eFSS_LOG_RES_BADPARAM;
                        }
                        else
                        {
                            /* Repair and load index */
                            l_eRes = eFSS_LOG_LoadIndexNRepair(p_ptCtx);
                            if( e_eFSS_LOG_RES_OK == l_eRes )
                            {
                                /* Can now retrive data, before doing this check if it's the newest */
                                if( p_uindx == p_ptCtx->uNewPagIdx )
                                {
                                    /* Newest page, load using proper function */
                                    l_eRes =  eFSS_LOG_LoadBufferAsNewestNBkpPage(p_ptCtx, p_uindx, &l_uByteInPages);

                                    if( e_eFSS_LOG_RES_OK == l_eRes )
                                    {
                                        if( l_uByteInPages > p_uBufL )
                                        {
                                            /* Not possible */
                                            l_eRes = e_eFSS_LOG_RES_BADPARAM;
                                        }
                                        else
                                        {
                                            memcpy(p_puBuf, l_tBuff.puBuf, l_uByteInPages);
                                        }
                                    }
                                }
                                else
                                {
                                    /* Check index validity, cannot read an invalid or actualy an un used page */

                                    /* Read the log page */
                                    l_eResC =  eFSS_LOGC_LoadBufferAs(&p_ptCtx->tLOGCCtx, e_eFSS_LOGC_PAGETYPE_LOG,
                                                                      p_uindx, &l_uByteInPages);
                                    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);
                                    if( e_eFSS_LOG_RES_OK == l_eRes )
                                    {
                                        if( l_uByteInPages > p_uBufL )
                                        {
                                            l_eRes = e_eFSS_LOG_RES_BADPARAM;
                                        }
                                        else
                                        {
                                            memcpy(p_puBuf, l_tBuff.puBuf, l_uByteInPages);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}



/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eFSS_LOG_IsStatusStillCoherent(t_eFSS_LOG_Ctx* const p_ptCtx)
{
    bool_t l_eRes;

	/* Check context validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = false;
	}
	else
	{
        l_eRes = true;
//            /* Che New index validity */
//            if( ( ( false == p_ptCtx->bFullBckup ) && ( false == p_ptCtx->bFlashCache ) &&
//                  ( p_ptCtx->uNewPagIdx >= l_uNPage ) ) ||
//                ( ( false == p_ptCtx->bFullBckup ) && ( true  == p_ptCtx->bFlashCache ) &&
//                  ( p_ptCtx->uNewPagIdx >= ( l_uNPage - 2u ) ) ) ||
//                ( ( true  == p_ptCtx->bFullBckup ) && ( false == p_ptCtx->bFlashCache ) &&
//                  ( p_ptCtx->uNewPagIdx >= ( l_uNPage / 2u ) ) ) ||
//                ( ( true  == p_ptCtx->bFullBckup ) && ( true  == p_ptCtx->bFlashCache ) &&
//                  ( p_ptCtx->uNewPagIdx >= ( ( l_uNPage - 2u ) / 2u ) ) ) )
//            {
//                l_bRes = false;
//            }
//            else
//            {
//                /* Check N filled page validity */
//                if( ( ( false == p_ptCtx->bFullBckup ) && ( false == p_ptCtx->bFlashCache ) &&
//                      ( p_ptCtx->uFullFilledP >= l_uNPage - 3u ) ) ||
//                    ( ( false == p_ptCtx->bFullBckup ) && ( true  == p_ptCtx->bFlashCache ) &&
//                      ( p_ptCtx->uFullFilledP >= ( l_uNPage - 2u - 3u ) ) ) ||
//                    ( ( true  == p_ptCtx->bFullBckup ) && ( false == p_ptCtx->bFlashCache ) &&
//                      ( p_ptCtx->uFullFilledP >= ( ( l_uNPage / 2u ) - 3u ) ) ) ||
//                    ( ( true  == p_ptCtx->bFullBckup ) && ( true  == p_ptCtx->bFlashCache ) &&
//                      ( p_ptCtx->uFullFilledP >= ( ( ( l_uNPage - 2u ) / 2u ) ) -3u ) ) )
//                {
//                    l_bRes = false;
//                }
//                else
//                {
//                    l_bRes = true;
//                }
//            }
	}

    return l_eRes;
}

static e_eFSS_LOG_RES eFSS_LOG_LOGCtoLOGRes(const e_eFSS_LOGC_RES p_eLOGCRes)
{
    e_eFSS_LOG_RES l_eRes;

    switch(p_eLOGCRes)
    {
        case e_eFSS_LOG_RES_OK:
        {
            l_eRes = e_eFSS_LOG_RES_OK;
            break;
        }

        case e_eFSS_LOG_RES_BADPARAM:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }

        default:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }
    }

    return l_eRes;
}



/***********************************************************************************************************************
 *  PRIVATE UTILS STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static e_eFSS_LOG_RES eFSS_LOG_LoadIndexNRepair(t_eFSS_LOG_Ctx* const p_ptCtx)
{
    /* Local variable for return */
    e_eFSS_LOG_RES  l_eRes;
    e_eFSS_LOGC_RES l_eResC;

    /* Local variable used for support */
    bool_t l_bIsFlashCacheUsed;

    /* Search for index in the flash cache if present */
    l_eResC = eFSS_LOGC_IsFlashCacheUsed(&p_ptCtx->tLOGCCtx, &l_bIsFlashCacheUsed);
    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

    if( e_eFSS_LOG_RES_OK == l_eRes )
    {
        if( true == l_bIsFlashCacheUsed )
        {
            /* Load index from cache, and verify index validity of course */
            l_eRes = eFSS_LOG_LoadIndexFromCache(p_ptCtx);
        }
        else
        {
            /* Search for index reading all logs */
            l_eRes = eFSS_LOG_LoadIndxBySearch(p_ptCtx);
        }
    }

    return l_eRes;
}

static e_eFSS_LOG_RES eFSS_LOG_LoadIndexFromCache(t_eFSS_LOG_Ctx* const p_ptCtx)
{
	/* Local return variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES l_eResC;

    /* Local storage variable */
    t_eFSS_LOGC_StorBuf l_tBuff;
    uint32_t l_uUsePages;

    /* Local variable used for calculation */
    uint32_t l_uIdxN;
    uint32_t l_uIFlP;
    uint32_t l_uByteInPage;

    /* Load index from cache, first load the basic data, read cache and verify cache validity */
    l_uUsePages = 0u;
    l_eResC = eFSS_LOGC_GetBuffNUsable(&p_ptCtx->tLOGCCtx, &l_tBuff, &l_uUsePages);
    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

    if( e_eFSS_LOG_RES_OK == l_eRes )
    {
        /* Read cache */
        l_eResC = eFSS_LOGC_ReadCache(&p_ptCtx->tLOGCCtx, &l_uIdxN, &l_uIFlP);
        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

        if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) )
        {
            /* Ok, index are readed, verify just readed data coherence */
            if( ( l_uIdxN >= l_uUsePages ) || ( l_uIFlP >= ( l_uUsePages - EFSS_LOG_NEWBKPEMPY_P ) ) )
            {
                l_eRes = e_eFSS_LOG_RES_NOTVALIDLOG;
            }
            else
            {
                /* Can now read the specified page and verify if it's the newest or not */
                l_eRes = eFSS_LOG_LoadBufferAsNewestNBkpPage(p_ptCtx, l_uIdxN, &l_uByteInPage);

                if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) )
                {
                    /* Seems fine, check data validity  */
                    if( ( l_uByteInPage > l_tBuff.uBufL ) || ( ( l_tBuff.uBufL - l_uByteInPage ) < EFSS_LOG_NEWBKPEMPY_P ) )
                    {
                        l_eRes = e_eFSS_LOG_RES_NOTVALIDLOG;
                    }
                    else
                    {
                        /* All ok, update index */
                        p_ptCtx->uNewPagIdx = l_uIdxN;
                        p_ptCtx->uFullFilledP = l_uIFlP;
                    }
                }
            }
        }
    }

    return l_eRes;
}

static e_eFSS_LOG_RES eFSS_LOG_LoadIndxBySearch(t_eFSS_LOG_Ctx* const p_ptCtx)
{
	/* Local return variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES l_eResC;


    /* Local storage variable */
    t_eFSS_LOGC_StorBuf l_tBuff;
    uint32_t l_uUsePages;

    /* Local var used for search */
    uint32_t l_uNSearched;
    uint32_t l_uIdxSearch;
    uint32_t l_uNewstIdx;
    uint32_t l_uByteInPage;
    uint32_t l_uFilled;
    bool_t   l_bIsNewest;

    /* Load page index by searching. We will start searching from the last cached index in order to speed up
       operation */
    l_uUsePages = 0u;
    l_eResC = eFSS_LOGC_GetBuffNUsable(&p_ptCtx->tLOGCCtx, &l_tBuff, &l_uUsePages);
    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

    if( e_eFSS_LOG_RES_OK == l_eRes )
    {
        /* Verify cached index validity */
        if( ( p_ptCtx->uNewPagIdx >= l_uUsePages ) || ( p_ptCtx->uFullFilledP >= ( l_uUsePages - EFSS_LOG_NEWBKPEMPY_P ) ) )
        {
            /* Very very strange, and quite impossible */
            l_eRes = e_eFSS_LOG_RES_CORRUPTCTX;
        }
        else
        {
            /* Init variable */
            l_uNSearched = 0u;
            l_eRes = e_eFSS_LOG_RES_NOTVALIDLOG;

            /* Start searching from the cached page */
            l_uIdxSearch = p_ptCtx->uNewPagIdx;


            while( ( l_uNSearched < l_uUsePages ) && ( e_eFSS_LOG_RES_NOTVALIDLOG == l_eRes )  )
            {
                /* Is the current page the newest or the newest backup? */
                l_eResC = eFSS_LOGC_IsPageNewOrBkup(&p_ptCtx->tLOGCCtx, l_uIdxSearch, &l_bIsNewest);
                l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

                if( e_eFSS_LOG_RES_OK == l_eRes )
                {
                    /* Founded the newest or the newest backup */
                    if( true == l_bIsNewest )
                    {
                        l_uNewstIdx = l_uIdxSearch;
                    }
                    else
                    {
                        /* Find newest index also */
                        l_eRes = eFSS_LOG_GetPrevIndex(p_ptCtx, l_uIdxSearch, &l_uNewstIdx);
                    }

                    if( e_eFSS_LOG_RES_OK == l_eRes )
                    {
                        /* All index founded, verify all the two pages validity */
                        l_eRes = eFSS_LOG_LoadBufferAsNewestNBkpPage(p_ptCtx, l_uNewstIdx, &l_uByteInPage);

                        if( e_eFSS_LOG_RES_OK == l_eRes )
                        {
                            if( true != eFSS_Utils_RetriveU32(&l_tBuff.puBuf[l_tBuff.uBufL - EFSS_LOG_FILLPOFF], &l_uFilled) )
                            {
                                l_eRes = e_eFSS_LOG_RES_CORRUPTCTX;
                            }
                            else
                            {
                                /* Need to verify parameter before confirm the validity of the page */
                                if( ( l_uByteInPage > l_tBuff.uBufL ) || ( ( l_tBuff.uBufL - l_uByteInPage ) < EFSS_LOG_NEWBKPEMPY_P ) )
                                {
                                    l_eRes = e_eFSS_LOG_RES_NOTVALIDLOG;
                                }
                                else
                                {
                                    /* All ok, update index */
                                    p_ptCtx->uNewPagIdx = l_uNewstIdx;
                                    p_ptCtx->uFullFilledP = l_uFilled;
                                }
                            }
                        }
                    }
                }
                else if( e_eFSS_LOG_RES_NOTVALIDLOG == l_eRes )
                {
                    /* Not this one, go to the next index */
                    l_eRes = eFSS_LOG_GetNextIndex(p_ptCtx, l_uIdxSearch, &l_uIdxSearch);

                    if( e_eFSS_LOG_RES_OK == l_eRes )
                    {
                        l_uNSearched++;
                    }
                }
                else
                {
                    /* Unreparable error */
                }
            }
        }
    }

	return l_eRes;
}

static e_eFSS_LOG_RES eFSS_LOG_GetNextIndex(t_eFSS_LOG_Ctx* const p_ptCtx, const uint32_t p_uCurIdx,
                                            uint32_t* const p_uNextIdx)
{
	/* Local variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES l_eResC;

    /* Local storage variable */
    t_eFSS_LOGC_StorBuf l_tBuff;
    uint32_t l_uUsePages;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_uNextIdx ) )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
        /* Get basic storage info in order to verify passed parameter */
        l_uUsePages = 0u;
        l_eResC = eFSS_LOGC_GetBuffNUsable(&p_ptCtx->tLOGCCtx, &l_tBuff, &l_uUsePages);
        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            /* Verify parameter */
            if( p_uCurIdx >= l_uUsePages )
            {
                l_eRes = e_eFSS_LOG_RES_BADPARAM;
            }
            else
            {
                if( p_uCurIdx >= ( l_uUsePages - 1u ) )
                {
                    *p_uNextIdx = 0u;
                }
                else
                {
                    *p_uNextIdx = p_uCurIdx + 1u;
                }
            }
        }
    }

	return l_eRes;
}

static e_eFSS_LOG_RES eFSS_LOG_GetPrevIndex(t_eFSS_LOG_Ctx* const p_ptCtx, const uint32_t p_uCurIdx,
                                            uint32_t* const p_puPrevIdx)
{
	/* Local variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES l_eResC;

    /* Local storage variable */
    t_eFSS_LOGC_StorBuf l_tBuff;
    uint32_t l_uUsePages;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puPrevIdx ) )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
        /* Get basic storage info in order to verify passed parameter */
        l_uUsePages = 0u;
        l_eResC = eFSS_LOGC_GetBuffNUsable(&p_ptCtx->tLOGCCtx, &l_tBuff, &l_uUsePages);
        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            /* Verify parameter */
            if( p_uCurIdx >= l_uUsePages )
            {
                l_eRes = e_eFSS_LOG_RES_BADPARAM;
            }
            else
            {
                if( p_uCurIdx <= 0u )
                {
                    *p_puPrevIdx = l_uUsePages - 1u;
                }
                else
                {
                    *p_puPrevIdx = p_uCurIdx - 1u;
                }
            }
        }
    }

	return l_eRes;
}



/***********************************************************************************************************************
 *  PRIVATE UTILS FOR BKUP PAGE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static e_eFSS_LOG_RES eFSS_LOG_FlushBufferAsNewestNBkpPage(t_eFSS_LOG_Ctx* const p_ptCtx, const uint32_t p_uIdx,
                                                           const uint32_t p_uByteInPage)
{
	/* Local variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES l_eResC;

    /* Local storage variable */
    t_eFSS_LOGC_StorBuf l_tBuff;
    uint32_t l_uUsePages;

    /* Local var used for calculation */
    uint32_t l_uNextIdx;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
        /* Get basic storage info in order to verify passed parameter */
        l_uUsePages = 0u;
        l_eResC = eFSS_LOGC_GetBuffNUsable(&p_ptCtx->tLOGCCtx, &l_tBuff, &l_uUsePages);
        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            /* Verify parameter */
            if( ( p_uIdx >= l_uUsePages ) || ( p_uByteInPage > l_tBuff.uBufL ) )
            {
                l_eRes = e_eFSS_LOG_RES_BADPARAM;
            }
            else
            {
                /* First, flush the first page as newest one */
                l_eResC = eFSS_LOGC_FlushBufferAs(&p_ptCtx->tLOGCCtx, e_eFSS_LOGC_PAGETYPE_NEWEST, p_uIdx,
                                                  p_uByteInPage);
                l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

                if( e_eFSS_LOG_RES_OK == l_eRes )
                {
                    /* Search for the next page that will be used as */
                    l_uNextIdx = 0u;
                    l_eRes = eFSS_LOG_GetNextIndex(p_ptCtx, p_uIdx, &l_uNextIdx);

                    if( e_eFSS_LOG_RES_OK == l_eRes )
                    {
                        /* Flush the backup page */
                        l_eResC = eFSS_LOGC_FlushBufferAs(&p_ptCtx->tLOGCCtx, e_eFSS_LOGC_PAGETYPE_NEWEST_BKUP,
                                                          l_uNextIdx, p_uByteInPage);
                        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);
                    }
                }
            }
        }
	}

	return l_eRes;
}

static e_eFSS_LOG_RES eFSS_LOG_LoadBufferAsNewestNBkpPage(t_eFSS_LOG_Ctx* const p_ptCtx, const uint32_t p_uIdx,
                                                          uint32_t* const p_puByteInPage)
{
	/* Local variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES l_eResC;

    /* Local storage variable */
    t_eFSS_LOGC_StorBuf l_tBuff;
    uint32_t l_uUsePages;

    /* Local var used for calculation */
    uint32_t l_uBkupPageIdx;

    /* page status local var */
    bool_t l_bIsOrigValid;
    bool_t l_bIsBkupValid;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puByteInPage ) )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
        /* Get basic storage info in order to verify passed parameter */
        l_uUsePages = 0u;
        l_eResC = eFSS_LOGC_GetBuffNUsable(&p_ptCtx->tLOGCCtx, &l_tBuff, &l_uUsePages);
        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            /* Verify parameter */
            if( p_uIdx >= l_uUsePages )
            {
                l_eRes = e_eFSS_LOG_RES_BADPARAM;
            }
            else
            {
                /* Now we can try to load the newest page */
                l_eResC =  eFSS_LOGC_LoadBufferAs(&p_ptCtx->tLOGCCtx, e_eFSS_LOGC_PAGETYPE_NEWEST, p_uIdx,
                                                  p_puByteInPage);
                l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

                if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) ||
                    ( e_eFSS_LOG_RES_NOTVALIDLOG == l_eRes ) )
                {
                    /* Page readed, is valid? */
                    if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) )
                    {
                        l_bIsOrigValid = true;
                    }
                    else
                    {
                        l_bIsOrigValid = false;
                    }

                    /* Find the next index */
                    l_uBkupPageIdx = 0u;
                    l_eRes = eFSS_LOG_GetNextIndex(p_ptCtx, p_uIdx, &l_uBkupPageIdx);

                    if( e_eFSS_LOG_RES_OK == l_eRes )
                    {
                        /* Now we can try to load the newest nackup page */
                        l_eResC =  eFSS_LOGC_LoadBufferAs(&p_ptCtx->tLOGCCtx, e_eFSS_LOGC_PAGETYPE_NEWEST_BKUP,
                                                          l_uBkupPageIdx, p_puByteInPage);
                        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

                        if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) ||
                            ( e_eFSS_LOG_RES_NOTVALIDLOG == l_eRes ) )
                        {
                            /* Page readed, is valid? */
                            if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) )
                            {
                                l_bIsBkupValid = true;
                            }
                            else
                            {
                                l_bIsBkupValid = false;
                            }

                            /* We have all the data needed to make a decision */
                            if( ( true == l_bIsOrigValid ) && ( true == l_bIsBkupValid ) )
                            {
                                /* No a single valid pages found */
                                l_eRes = e_eFSS_LOG_RES_NOTVALIDLOG;
                            }
                            else if( ( true == l_bIsOrigValid ) && ( false == l_bIsBkupValid ) )
                            {

                            }
                            else if( ( false == l_bIsOrigValid ) && ( true == l_bIsBkupValid ) )
                            {

                            }
                            else
                            {
                                /* No a single valid pages found */
                                l_eRes = e_eFSS_LOG_RES_NOTVALIDLOG;
                            }
                        }
                    }
                }
            }
        }
	}

	return l_eRes;	/* Local variable */
}
