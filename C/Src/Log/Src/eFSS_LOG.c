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
 * Keep in mind that the last 4 byte in the newest page are not counted in the filed called: byte in page used by 
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
static e_eFSS_LOG_RES eFSS_LOGC_LoadIndexNRepair(t_eFSS_LOG_Ctx* const p_ptCtx);
static e_eFSS_LOG_RES eFSS_LOGC_LoadIndexFromCache(t_eFSS_LOG_Ctx* const p_ptCtx);
static e_eFSS_LOG_RES eFSS_LOGC_LoadIndxBySearch(t_eFSS_LOG_Ctx* const p_ptCtx);



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
                    l_eRes = e_eFSS_LOGC_RES_BADPARAM;

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
                    l_eRes = eFSS_LOGC_LoadIndexNRepair(p_ptCtx);
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
                    l_eRes = eFSS_LOGC_LoadIndexNRepair(p_ptCtx);
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
    uint32_t l_uNextIndex;
    
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
                        /* Load index */
                        l_eRes = eFSS_LOGC_LoadIndexNRepair(p_ptCtx);

                        if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) )
                        {
                            /* Flash cache, so we find a valid newest page pointed by cache.
                               If the newest page is at index zero just format that page at index zero, withouth
                               modifyng the caches, otherwise invalidate
                               the current page (during a power outage the state of the storage will be invalid)
                               write newest page on index zero, and finaly update the flash cache (even
                               if a power outage occours, and the state of the storage will be invalid, no
                               problem: we were formatting the storage anyway.. ) */

                            /* No flash cache, so we find a valid newest page.
                               If the newest page is at index zero just format that page otherwise invalidate
                               the current page and just after write newest on index zero */

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
                        }

                        /* Start the procedure */
                        if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) ||
                            ( e_eFSS_LOG_RES_NOTVALIDLOG == l_eRes ) )
                        {
                            /* Invalidate the current pages */
                            if( true == l_bInvalidateCurrent )
                            {
                                /* Clear buffer  */
                                memset(l_tBuff.puBuf, 0u, l_tBuff.uBufL);
                                memset(&l_tBuff.ptMeta, 0u, sizeof(t_eFSS_TYPE_PageMeta));

                                l_eResC = eFSS_LOGC_FlushBufferAsLog(&p_ptCtx->tLOGCCtx, p_ptCtx->uNewPagIdx);
                                l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

                                if( e_eFSS_LOG_RES_OK == l_eRes )
                                {
                                    /* Invalidate newest backup also */
                                    eFSS_LOGC_GetNextIndex(&p_ptCtx->tLOGCCtx, p_ptCtx->uNewPagIdx, &l_uNextIndex);
                                    l_eResC = eFSS_LOGC_FlushBufferAsLog(&p_ptCtx->tLOGCCtx, l_uNextIndex);
                                    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);
                                }
                            }

                            if( e_eFSS_LOG_RES_OK == l_eRes )
                            {
                                /* Write newest in first index */
                                memset(l_tBuff.puBuf, 0u, l_tBuff.uBufL);
                                memset(&l_tBuff.ptMeta, 0u, sizeof(t_eFSS_TYPE_PageMeta));

                                l_eResC = eFSS_LOGC_FlushBufferAsNewestNBkpPage(&p_ptCtx->tLOGCCtx, 0u);
                                l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

                                if( e_eFSS_LOG_RES_OK == l_eRes )
                                {
                                    /* write cache */
                                    eFSS_LOGC_IsFlashCacheUsed(&p_ptCtx->tLOGCCtx, &l_bIsFlashCacheUsed);
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
    uint32_t l_uPages;

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
                        if( (p_uElemL <= 0u ) || ( (2u + p_uElemL) > l_tBuff.uBufL ) )
                        {
                            l_eRes = e_eFSS_LOG_RES_BADPARAM;
                        }
                        else
                        {
                            l_eRes = eFSS_LOGC_LoadIndexNRepair(p_ptCtx);
                            if( e_eFSS_LOG_RES_OK == l_eRes )
                            {
                                /* Read current write page:
                                 *    PAGE FULL: Go next, and save log in next page
                                 *    PAGE EMPTY: Save log in the current page and we are done
                                 */
                                l_eResC = eFSS_LOGC_LoadBufferAsNewestNBkpPage(&p_ptCtx->tLOGCCtx, p_ptCtx->uNewPagIdx);
                                l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

                                if( e_eFSS_LOG_RES_OK == l_eRes )
                                {
                                    if( ( l_tBuff.uBufL - l_tBuff.ptMeta->uPageUseSpec3 ) >= (2u + p_uElemL) )
                                    {
                                        /* We have some free space, add data in this page */
                                        l_tBuff.puBuf[l_tBuff.ptMeta->uPageUseSpec3] = p_uElemL & 0xFF;
                                        l_tBuff.ptMeta->uPageUseSpec3++;

                                        l_tBuff.puBuf[l_tBuff.ptMeta->uPageUseSpec3] = p_uElemL & 0xFF;
                                        l_tBuff.ptMeta->uPageUseSpec3++;

                                        memcpy(&l_tBuff.puBuf[l_tBuff.ptMeta->uPageUseSpec3], p_puRawVal, p_uElemL);
                                        l_tBuff.ptMeta->uPageUseSpec3+= p_uElemL;

                                        /* Write page */
                                        l_eResC = eFSS_LOGC_FlushBufferAsNewestNBkpPage(&p_ptCtx->tLOGCCtx, p_ptCtx->uNewPagIdx);
                                        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);
                                    }
                                    else
                                    {
                                        /* We dont' have space to append log, we need to increase the page.
                                        *    1 - Transform original page in normal log page
                                        *    2 - Add the new bkup pages after the current bkup page
                                        *    3 - If cache is present update cache with the new index
                                        *    4 - Trasform the backup page in the original pages
                                        */
                                        /* Step 1 - finalize original page */
                                        l_eResC = eFSS_LOGC_FlushBufferAsLog(&p_ptCtx->tLOGCCtx, p_ptCtx->uNewPagIdx);
                                        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

                                        if( e_eFSS_LOG_RES_OK == l_eRes )
                                        {
                                            /* Step 2 - add the new bkup page after the current bkup page */
                                            eFSS_LOGC_GetNextIndex(&p_ptCtx->tLOGCCtx, p_ptCtx->uFullFilledP, &l_uPages);
                                            eFSS_LOGC_GetNextIndex(&p_ptCtx->tLOGCCtx, l_uPages, &l_uPages);
                                            l_eResC = eFSS_LOGC_FlushBufferAsNewestBkupOnly(&p_ptCtx->tLOGCCtx, l_uPages);
                                            l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

                                            if( e_eFSS_LOG_RES_OK == l_eRes )
                                            {
                                                /* Step 3 - If cache is present update cache with the new index */
                                                eFSS_LOGC_GetNextIndex(&p_ptCtx->tLOGCCtx, p_ptCtx->uFullFilledP, &l_uPages);
                                                l_eResC = eFSS_LOGC_WriteCache(&p_ptCtx->tLOGCCtx, l_uPages, p_ptCtx->uFullFilledP + 1u);
                                                l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

                                                if( e_eFSS_LOG_RES_OK == l_eRes )
                                                {
                                                    /* Step 4 - Trasform the backup page in the original pages */
                                                    eFSS_LOGC_GetNextIndex(&p_ptCtx->tLOGCCtx, p_ptCtx->uFullFilledP, &l_uPages);
                                                    l_eResC = eFSS_LOGC_FlushBufferAsNewestOnly(&p_ptCtx->tLOGCCtx, l_uPages);
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
                    l_uUsePages = 0u;
                    l_eResC = eFSS_LOGC_GetBuffNUsable(&p_ptCtx->tLOGCCtx, &l_tBuff, &l_uUsePages);
                    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

                    if( e_eFSS_LOG_RES_OK == l_eRes )
                    {
                        if( 0u == p_uBufMaxL )
                        {
                            l_eRes = e_eFSS_LOG_RES_BADPARAM;
                        }
                        else
                        {
                            /* Repair and load index */
                            l_eRes = eFSS_LOGC_LoadIndexNRepair(p_ptCtx);
                            if( e_eFSS_LOG_RES_OK == l_eRes )
                            {
                                /* Can now retrive data */
                                if( p_uindx == p_ptCtx->uNewPagIdx )
                                {
                                    l_eResC =  eFSS_LOGC_LoadBufferAsNewestNBkpPage(&p_ptCtx->tLOGCCtx, p_uindx);
                                    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);
                                    if( e_eFSS_LOG_RES_OK == l_eRes )
                                    {
                                        if( p_uBufMaxL >= l_tBuff.ptMeta->uPageUseSpec3 )
                                        {
                                            memcpy(p_puBuf, l_tBuff.puBuf, l_tBuff.ptMeta->uPageUseSpec3);
                                        }
                                        else
                                        {
                                            memcpy(p_puBuf, l_tBuff.puBuf, p_uBufMaxL);
                                        }
                                    }
                                }
                                else
                                {
                                    /* Check index validity TODO */
                                    l_eResC =  eFSS_LOGC_LoadBufferAsLog(&p_ptCtx->tLOGCCtx, p_uindx);
                                    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);
                                    if( e_eFSS_LOG_RES_OK == l_eRes )
                                    {
                                        if( p_uBufMaxL >= l_tBuff.ptMeta->uPageUseSpec3 )
                                        {
                                            memcpy(p_puBuf, l_tBuff.puBuf, l_tBuff.ptMeta->uPageUseSpec3);
                                        }
                                        else
                                        {
                                            memcpy(p_puBuf, l_tBuff.puBuf, p_uBufMaxL);
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

static e_eFSS_LOG_RES eFSS_LOGC_LoadIndexNRepair(t_eFSS_LOG_Ctx* const p_ptCtx)
{
    e_eFSS_LOG_RES l_eRes;
    bool_t l_bIsFlashCacheUsed;

    /* Search for index in the flash cache if present */
    eFSS_LOGC_IsFlashCacheUsed(&p_ptCtx->tLOGCCtx, &l_bIsFlashCacheUsed);
    if( true == l_bIsFlashCacheUsed )
    {
        /* Load index from cache */
        l_eRes = eFSS_LOGC_LoadIndexFromCache(p_ptCtx);
    }
    else
    {
        /* Search for index */
        l_eRes = eFSS_LOGC_LoadIndxBySearch(p_ptCtx);
    }

    return l_eRes;
}

static e_eFSS_LOG_RES eFSS_LOGC_LoadIndexFromCache(t_eFSS_LOG_Ctx* const p_ptCtx)
{
	/* Local return variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES l_eResC;

    /* Local variable used for calculation */
    uint32_t l_uIdxN;
    uint32_t l_uIFlP;
    t_eFSS_LOGC_StorBuf l_tBuff;

    l_eResC = eFSS_LOGC_GetBuff(&p_ptCtx->tLOGCCtx, &l_tBuff);
    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

    if( e_eFSS_LOG_RES_OK == l_eRes )
    {
        /* Read cache */
        l_eResC = eFSS_LOGC_ReadCache(&p_ptCtx->tLOGCCtx, &l_uIdxN, &l_uIFlP);
        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

        if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) )
        {
            /* Ok, index are readed, verify if pointed page is rely the newest page */
            l_eResC = eFSS_LOGC_LoadBufferAsNewestNBkpPage(&p_ptCtx->tLOGCCtx, l_uIdxN);
            l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

            if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) )
            {
                /* Correct pages! Check value */
                if( ( l_tBuff.ptMeta->uPageUseSpec1 != l_uIdxN ) || ( l_tBuff.ptMeta->uPageUseSpec2 != l_uIFlP ) )
                {
                    l_eRes = e_eFSS_LOG_RES_NOTVALIDLOG;
                }
                else
                {
                    /* Update cache */
                    p_ptCtx->uFullFilledP = l_uIFlP;
                    p_ptCtx->uNewPagIdx = l_uIdxN;
                }
            }
        }
    }

    return l_eRes;
}

static e_eFSS_LOG_RES eFSS_LOGC_LoadIndxBySearch(t_eFSS_LOG_Ctx* const p_ptCtx)
{
	/* Local return variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES l_eResC;

    /* Local var used for settings */
    t_eFSS_LOGC_StorBuf l_tBuff;

    /* Local var used for search */
    uint32_t l_uNSearched;
    uint32_t l_uIdxSearch;
    uint32_t l_uUsablePage;

    l_eResC = eFSS_LOGC_GetBuff(&p_ptCtx->tLOGCCtx, &l_tBuff);
    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

    if( e_eFSS_LOG_RES_OK == l_eRes )
    {
        eFSS_LOGC_GetUsablePage(&p_ptCtx->tLOGCCtx, &l_uUsablePage);

        /* Init variable */
        l_uNSearched = 0u;

        /* Start searching from the cached page */
        l_uIdxSearch = p_ptCtx->uNewPagIdx;
        l_eRes = e_eFSS_LOG_RES_NOTVALIDLOG;

        while( ( l_uNSearched < l_uUsablePage ) && ( e_eFSS_LOG_RES_NOTVALIDLOG == l_eRes )  )
        {
            l_eResC = eFSS_LOGC_LoadBufferAsNewestNBkpPage(&p_ptCtx->tLOGCCtx, p_ptCtx->uNewPagIdx);
            l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eResC);

            if( e_eFSS_LOG_RES_OK == l_eRes )
            {
                /* Founded, super!! */
                p_ptCtx->uNewPagIdx   = l_uIdxSearch;
                p_ptCtx->uFullFilledP = l_tBuff.ptMeta->uPageUseSpec2;
            }
            else if( e_eFSS_LOG_RES_NOTVALIDLOG == l_eRes )
            {
                /* Not this one */
                eFSS_LOGC_GetNextIndex( &p_ptCtx->tLOGCCtx, l_uIdxSearch, &l_uIdxSearch );
                l_uNSearched++;
            }
            else
            {
                /* Unreparable error */
            }
        }
    }

	return l_eRes;
}

/**********************************************************************************************************************/
#if 0u
e_eFSS_LOGC_RES eFSS_LOGC_FlushBufferAsNewestNBkpPage(t_eFSS_LOGC_Ctx* const p_ptCtx, const uint32_t p_uIdx,
                                                      const uint32_t p_uFillInPage)
{
	/* Local variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint32_t l_uNPageU;
    uint32_t l_uBkupPage;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOGC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOGC_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResHL =  eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
                    l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                    {
                        /* Calculate n page */
                        l_uNPageU = eFSS_LOGC_GetMaxPage(p_ptCtx, l_tStorSet);

                        /* Save original newest page */
                        l_eRes = eFSS_LOGC_FlushBuffInPageNBkp(p_ptCtx, p_ptCtx->bFullBckup, p_uIdx, (l_uNPageU + p_uIdx),
                                                                EFSS_PAGESUBTYPE_LOGNEWESTORI, EFSS_PAGESUBTYPE_LOGNEWESTBKP);

                        if( e_eFSS_LOGC_RES_OK == l_eRes )
                        {
                            /* Flush */
                            l_uBkupPage = eFSS_LOGCPRV_GetNextIndex(p_ptCtx, l_tStorSet, p_uIdx);

                            /* Save newest backup page */
                            l_eRes = eFSS_LOGC_FlushBuffInPageNBkp(p_ptCtx, p_ptCtx->bFullBckup, l_uBkupPage,
                                                                    (l_uNPageU + l_uBkupPage), EFSS_PAGESUBTYPE_LOGNEWESTBKPORI,
                                                                    EFSS_PAGESUBTYPE_LOGNEWESTBKPBKP);
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGC_LoadBufferAsNewestNBkpPage(t_eFSS_LOGC_Ctx* const p_ptCtx, const uint32_t p_uIdx,
                                                     uint32_t* const p_puFillInPage)
{
	/* Local variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint32_t l_uNPageU;
	uint32_t l_uOriPageIdx;
    uint32_t l_uBkupPageIdx;

    /* page status local var */
    bool_t l_bIsOrigValid;
    bool_t l_bIsBkupValid;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOGC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOGC_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResHL =  eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
                    l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                    {
                        /* Calculate n page */
                        l_uNPageU = eFSS_LOGC_GetMaxPage(p_ptCtx, l_tStorSet);

                        /* Setup index */
                        l_uOriPageIdx = p_uIdx;
                        l_uBkupPageIdx = l_uNPageU + l_uOriPageIdx;

                        /* Read newest page first */
                        l_eRes =  eFSS_LOGC_LoadPageInBuffNRipBkp(p_ptCtx, p_ptCtx->bFullBckup, l_uOriPageIdx, l_uBkupPageIdx,
                                                                    EFSS_PAGESUBTYPE_LOGNEWESTORI, EFSS_PAGESUBTYPE_LOGNEWESTBKP);

                        if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) ||
                            ( e_eFSS_LOGC_RES_NOTVALIDLOG == l_eRes ) )
                        {
                            /* Page readed, is valid? */
                            if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) )
                            {
                                l_bIsOrigValid = true;
                            }
                            else
                            {
                                l_bIsOrigValid = false;
                            }

                            /* Setup index */
                            l_uOriPageIdx = eFSS_LOGCPRV_GetNextIndex(p_ptCtx, l_tStorSet, p_uIdx);
                            l_uBkupPageIdx = l_uNPageU + l_uOriPageIdx;

                            /* Read backup pages now */
                            l_eRes =  eFSS_LOGC_LoadPageInBuffNRipBkp(p_ptCtx, p_ptCtx->bFullBckup, l_uOriPageIdx, l_uBkupPageIdx,
                                                                        EFSS_PAGESUBTYPE_LOGNEWESTBKPORI, EFSS_PAGESUBTYPE_LOGNEWESTBKPBKP);

                            if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) ||
                                ( e_eFSS_LOGC_RES_NOTVALIDLOG == l_eRes ) )
                            {
                                /* Page readed, is valid? */
                                if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) )
                                {
                                    l_bIsBkupValid = true;
                                }
                                else
                                {
                                    l_bIsBkupValid = false;
                                }

                                /* We have all the data needed to make a decision */
                                if( ( false == l_bIsOrigValid ) && ( false == l_bIsBkupValid ) )
                                {
                                    /* No a single valid pages found */
                                    l_eRes = e_eFSS_LOGC_RES_NOTVALIDLOG;
                                }
                                else
                                {
                                    /* Need to cross do verifycation now */
                                    /* Setup index */
                                    l_uOriPageIdx  = p_uIdx;
                                    l_uBkupPageIdx = eFSS_LOGCPRV_GetNextIndex(p_ptCtx, l_tStorSet, l_uOriPageIdx);;

                                    /* Read newest page first */
                                    l_eRes =  eFSS_LOGC_LoadPageInBuffNRipBkp(p_ptCtx, true, l_uOriPageIdx, l_uBkupPageIdx,
                                                                                EFSS_PAGESUBTYPE_LOGNEWESTORI, EFSS_PAGESUBTYPE_LOGNEWESTBKPORI);

                                    if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) )
                                    {
                                        if( true == p_ptCtx->bFullBckup )
                                        {
                                            /* Need to cross do verifycation now */
                                            /* Setup index */
                                            l_uOriPageIdx  = p_uIdx + l_uNPageU;
                                            l_uBkupPageIdx = eFSS_LOGCPRV_GetNextIndex(p_ptCtx, l_tStorSet, l_uOriPageIdx);;

                                            /* Read newest page first */
                                            l_eRes =  eFSS_LOGC_LoadPageInBuffNRipBkp(p_ptCtx, true, l_uOriPageIdx, l_uBkupPageIdx,
                                                                                        EFSS_PAGESUBTYPE_LOGNEWESTBKP, EFSS_PAGESUBTYPE_LOGNEWESTBKPBKP);
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

/**
 * @brief       Get the value of the next index given a passed one.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_uIdx           - Index that we want to search for next. Usable value: from zero to the value returned
 *                                 by eFSS_DBC_GetBuffNUsable
 * @param[out]  p_puNextIdx      - Pointer where the value of the next index will be copied
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_LOGC_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_LOGC_RES_OK            - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGC_GetNextIndex(t_eFSS_LOGC_Ctx* const p_ptCtx, const uint32_t p_uIdx,
                                       uint32_t* const p_puNextIdx);

/**
 * @brief       Get the value of the previous index.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_uIdx           - Index that we want to search for previous. Usable value: from zero to the value
 *                                 returned by eFSS_DBC_GetBuffNUsable
 * @param[out]  p_puPrevIdx      - Pointer where the value of the previous index will be copied
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_LOGC_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_LOGC_RES_OK            - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGC_GetPrevIndex(t_eFSS_LOGC_Ctx* const p_ptCtx, const uint32_t p_uIdx,
                                       uint32_t* const p_puPrevIdx);

e_eFSS_LOGC_RES eFSS_LOGC_GetNextIndex(t_eFSS_LOGC_Ctx* const p_ptCtx, const uint32_t p_uIdx,
                                       uint32_t* const p_puNextIdx)
{
	/* Local variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
	uint32_t l_uNPageU;
    uint32_t l_uNextIdx;
    t_eFSS_TYPE_StorSet l_tStorSet;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puNextIdx ) )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOGC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOGC_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
                    l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                    {
                        /* Get the total numbers of page */
                        l_uNPageU = eFSS_LOGC_GetMaxPage(p_ptCtx->bFullBckup, p_ptCtx->bFlashCache, l_tStorSet.uTotPages);

                        if( p_uIdx >= ( l_uNPageU - 1u ) )
                        {
                            l_uNextIdx = 0u;
                        }
                        else
                        {
                            l_uNextIdx = p_uIdx + 1u;
                        }

                        *p_puNextIdx = l_uNextIdx;
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGC_GetPrevIndex(t_eFSS_LOGC_Ctx* const p_ptCtx, const uint32_t p_uIdx,
                                       uint32_t* const p_puPrevIdx)
{
	/* Local variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
	uint32_t l_uNPageU;
    uint32_t l_uPrevIdx;
    t_eFSS_TYPE_StorSet l_tStorSet;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puPrevIdx ) )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOGC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOGC_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
                    l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                    {
                        /* Get the total numbers of page */
                        l_uNPageU = eFSS_LOGC_GetMaxPage(p_ptCtx->bFullBckup, p_ptCtx->bFlashCache, l_tStorSet.uTotPages);

                        if( p_uIdx <= 0u )
                        {
                            l_uPrevIdx = l_uNPageU - 1u;
                        }
                        else
                        {
                            l_uPrevIdx = p_uIdx - 1u;
                        }

                        *p_puPrevIdx = l_uPrevIdx;
                    }
                }
            }
        }
	}

	return l_eRes;
}

#endif