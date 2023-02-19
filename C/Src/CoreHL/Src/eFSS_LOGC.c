/**
 * @file       eFSS_LOGC.c
 *
 * @brief      LOG Core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/* In this module the page field has the following meaning:
 * ------------------------------------------------------------------ User data
 * - [uint8_t] -                    -> N byte of user data           |
 * ------------------------------------------------------------------ Metadata  (17 byte)
 * - uint32_t  - uPageUseSpec1      -> New page Index                |
 * - uint32_t  - uPageUseSpec2      -> Number of Filled index page   |
 * - uint32_t  - uPageUseSpec3      -> Valorized byte in page        |
 * - uint32_t  - uPageUseSpec4      -> No value defined              |
 * - uint8_t   - uPageSubType       -> Page subtype                  |
 * ------------------------------------------------------------------+
 *
 * In this module the storage is organizated as follow :
 *
 *   bFullBckup = true, bFlashCache = true
 * - [ 0                            -    ( ( uTotPages - 2 ) / 2 ) - 1 ]  -> Original pages
 * - [ ( ( uTotPages - 2 ) / 2 )    -    uTotPages - 1 -2              ]  -> Backup pages
 * - [ uTotPages - 1 -1             -    uTotPages - 1 -1              ]  -> Cache original
 * - [ uTotPages - 1                -    uTotPages - 1                 ]  -> Cache backup
 *
 * First write original pages and after the backup pages
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
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_LOGC.h"
#include "eFSS_LOGCPRV.h"


/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static e_eFSS_LOGC_RES eFSS_LOGC_LoadIndexNRepair(t_eFSS_LOGC_Ctx* const p_ptCtx);
static e_eFSS_LOGC_RES eFSS_LOGC_LoadIndexFromCache(t_eFSS_LOGC_Ctx* const p_ptCtx);
static e_eFSS_LOGC_RES eFSS_LOGC_LoadIndxBySearch(t_eFSS_LOGC_Ctx* const p_ptCtx);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_LOGC_RES eFSS_LOGC_InitCtx(t_eFSS_LOGC_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx const p_tCtxCb,
                                  t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL,
                                  bool_t p_bFlashCache, bool_t p_bFullBckup)
{
    /* Return local var */
    e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    uint32_t l_uNPage;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
        l_uNPage = p_tStorSet.uTotPages;

        /* Check numbers of page validity */
        if( ( ( false == p_bFullBckup ) && ( false == p_bFlashCache ) && ( l_uNPage < 4u  ) ) ||
            ( ( false == p_bFullBckup ) && ( true  == p_bFlashCache ) && ( l_uNPage < 6u  ) ) ||
            ( ( true  == p_bFullBckup ) && ( false == p_bFlashCache ) && ( l_uNPage < 8u  ) ) ||
            ( ( true  == p_bFullBckup ) && ( true  == p_bFlashCache ) && ( l_uNPage < 10u ) ) ||
            ( ( true  == p_bFullBckup ) && ( 0u != ( l_uNPage % 2u ) ) ) )
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
        }
        else
        {
            /* Can init low level context */
            l_eResHL = eFSS_COREHL_InitCtx(&p_ptCtx->tCOREHLCtx, p_tCtxCb, p_tStorSet, p_puBuff, p_uBuffL);
            l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

            if( e_eFSS_LOGC_RES_OK == l_eRes )
            {
                /* Fill context */
                p_ptCtx->uNewPagIdx = 0;
                p_ptCtx->uFullFilledP = 0;
                p_ptCtx->bFlashCache = p_bFlashCache;
                p_ptCtx->bFullBckup = p_bFullBckup;
            }
        }
    }

    return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGC_IsInit(t_eFSS_LOGC_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, p_pbIsInit);
        l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);
	}

	return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGC_GetStorageStatus(t_eFSS_LOGC_Ctx* const p_ptCtx)
{
	/* Local variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOGC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOGCPRV_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
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

e_eFSS_LOGC_RES eFSS_LOGC_GetLogInfo(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t *p_puNewLogI, uint32_t *p_puOldLogI,
                                     uint32_t *p_puNpageUsed, uint32_t *p_puNpageTot)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint32_t l_uNPageU;
    uint32_t l_uRemain;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puNewLogI ) || ( NULL == p_puOldLogI ) || ( NULL == p_puNpageUsed ) ||
        ( NULL == p_puNpageTot ) )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOGC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOGCPRV_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
                }
                else
                {
                    /* Retrive storage settings */
                    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
                    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                    {
                        /* Verify storage integrity and load in the context the log index */
                        l_eRes = eFSS_LOGC_LoadIndexNRepair(p_ptCtx);
                        if( e_eFSS_LOGC_RES_OK == l_eRes )
                        {
                            /* Calculate n page */
                            l_uNPageU = eFSS_LOGCPRV_GetUsablePage(p_ptCtx, l_tStorSet);

                            /* Copy result */
                            *p_puNewLogI = p_ptCtx->uNewPagIdx;
                            *p_puNpageUsed = p_ptCtx->uFullFilledP;
                            if( p_ptCtx->uNewPagIdx < p_ptCtx->uFullFilledP )
                            {
                                l_uRemain = p_ptCtx->uFullFilledP - p_ptCtx->uNewPagIdx;
                                *p_puOldLogI = l_uNPageU - l_uRemain;
                            }
                            else
                            {
                                *p_puOldLogI = p_ptCtx->uNewPagIdx - p_ptCtx->uFullFilledP;
                            }

                            *p_puNpageTot = l_uNPageU;
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGCPRV_GetBuffer(t_eFSS_LOGC_Ctx* const p_ptCtx, t_eFSS_TYPE_StorBuf* p_ptBuff)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Get storage area, we need to return only the first buffer */
    l_eResHL = eFSS_COREHL_GetBuff(&p_ptCtx->tCOREHLCtx, p_ptBuff);
    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

    return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGC_Format(t_eFSS_LOGC_Ctx* const p_ptCtx)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_TYPE_StorBuf l_tBuff;
    uint32_t l_uNextIndex;

    /* Local variable for decision making */
    bool_t l_bInvalidateCurrent;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOGC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOGCPRV_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
                    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                    {
                        l_eResHL = eFSS_COREHL_GetBuff(&p_ptCtx->tCOREHLCtx, &l_tBuff);
                        l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);
                        if( e_eFSS_LOGC_RES_OK == l_eRes )
                        {
                            /* Load index */
                            l_eRes = eFSS_LOGC_LoadIndexNRepair(p_ptCtx);

                            if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) )
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
                            else if( e_eFSS_LOGC_RES_NOTVALIDLOG == l_eRes )
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
                            if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) ||
                                ( e_eFSS_LOGC_RES_NOTVALIDLOG == l_eRes ) )
                            {
                                /* Invalidate the current pages */
                                if( true == l_bInvalidateCurrent )
                                {
                                    /* Clear buffer  */
                                    memset(l_tBuff.puBuf, 0u, l_tBuff.uBufL);
                                    memset(&l_tBuff.ptMeta, 0u, sizeof(t_eFSS_TYPE_PageMeta));

                                    l_eRes = eFSS_LOGCPRV_FlushBufferAsLog(p_ptCtx, p_ptCtx->uNewPagIdx);

                                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                                    {
                                        /* Invalidate newest backup also */
                                        l_uNextIndex = eFSS_LOGCPRV_GetNextIndex(p_ptCtx, l_tStorSet, p_ptCtx->uNewPagIdx);
                                        l_eRes = eFSS_LOGCPRV_FlushBufferAsLog(p_ptCtx, l_uNextIndex);
                                    }
                                }

                                if( e_eFSS_LOGC_RES_OK == l_eRes )
                                {
                                    /* Write newest in first index */
                                    memset(l_tBuff.puBuf, 0u, l_tBuff.uBufL);
                                    memset(&l_tBuff.ptMeta, 0u, sizeof(t_eFSS_TYPE_PageMeta));
                                    l_eRes = eFSS_LOGCPRV_FlushBufferAsNewestNBkpPage(p_ptCtx, 0u);

                                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                                    {
                                        /* write cache */
                                        if( true == p_ptCtx->bFlashCache )
                                        {
                                            l_eRes = eFSS_LOGCPRV_WriteCache(p_ptCtx, 0u, 0u);
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

e_eFSS_LOGC_RES eFSS_LOGC_GetLogOfASpecificPage(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t p_uindx, uint8_t* p_puBuf,
                                                uint32_t* p_puBufL, uint32_t p_uBufMaxL)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_TYPE_StorBuf l_tBuff;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuf ) || ( NULL == p_puBufL ) )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOGC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOGCPRV_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
                    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                    {
                        if( 0u == p_uBufMaxL )
                        {
                            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
                        }
                        else
                        {
                            /* Get working buffer */
                            l_eResHL = eFSS_COREHL_GetBuff(&p_ptCtx->tCOREHLCtx, &l_tBuff);
                            l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);
                            if( e_eFSS_LOGC_RES_OK == l_eRes )
                            {
                                /* Repair and load index */
                                l_eRes = eFSS_LOGC_LoadIndexNRepair(p_ptCtx);
                                if( e_eFSS_LOGC_RES_OK == l_eRes )
                                {
                                    /* Can now retrive data */
                                    if( p_uindx == p_ptCtx->uNewPagIdx )
                                    {
                                        l_eRes =  eFSS_LOGCPRV_LoadBufferAsNewestNBkpPage(p_ptCtx, p_uindx);
                                        if( e_eFSS_LOGC_RES_OK == l_eRes )
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
                                        l_eRes =  eFSS_LOGCPRV_LoadBufferAsLog(p_ptCtx, p_uindx);
                                        if( e_eFSS_LOGC_RES_OK == l_eRes )
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
	}

	return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGC_AddLogAndHeader(t_eFSS_LOGC_Ctx* const p_ptCtx, uint8_t* p_puLogToSaveH, uint32_t p_uLogLH,
                                          uint8_t* p_puLogToSave, uint32_t p_uLogL)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_TYPE_StorBuf l_tBuff;
    uint32_t l_uNPageU;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puLogToSave ) )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOGC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOGCPRV_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
                    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                    {
                        l_eResHL = eFSS_COREHL_GetBuff(&p_ptCtx->tCOREHLCtx, &l_tBuff);
                        l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);
                        if( e_eFSS_LOGC_RES_OK == l_eRes )
                        {
                            l_eRes = eFSS_LOGC_LoadIndexNRepair(p_ptCtx);
                            if( e_eFSS_LOGC_RES_OK == l_eRes )
                            {
                                /* Read current write page */
                                /* If page full go next */
                                /* Update page and index */
                                /* Get buffer for calculation */
                                l_eRes =  eFSS_LOGCPRV_LoadBufferAsNewestPage(p_ptCtx, p_ptCtx->uNewPagIdx);

                                if( e_eFSS_LOGC_RES_OK == l_eRes )
                                {
                                    if( ( l_tBuff.uBufL - l_tBuff.ptMeta->uPageUseSpec3 ) > p_uLogL )
                                    {
                                        /* We dont' have space to append log, we need to increase the page.
                                        *    1 - Transform original page in normal log page
                                        *    2 - Add the new bkup pages after the current bkup page
                                        *    3 - If cache is present update cache with the new index
                                        *    4 - trasform the backup page in the original pages
                                        */
                                        /* Step 1 - finalize original page */
                                        l_eRes = eFSS_LOGCPRV_FlushBufferAsLog(p_ptCtx, p_ptCtx->uNewPagIdx);

                                        if( e_eFSS_LOGC_RES_OK == l_eRes )
                                        {
                                            /* Step 2 - add the new bkup page after the current bkup page */
                                            l_eRes = eFSS_LOGCPRV_WritePage(p_ptCtx, p_ptCtx->uNPagIdxFound, l_puBuF1, l_uBuF1L,
                                                                            l_puBuF2, l_uBuF2L, l_tPagePrm);
                                        }
                                    }
                                    else
                                    {
                                        /* We have some free space, add data in this page */
                                        memcpy(&l_tBuff.puBuf[l_tBuff.ptMeta->uPageUseSpec3], p_puLogToSave, p_uLogL);
                                        l_tBuff.ptMeta->uPageUseSpec3+= p_uLogL;

                                        /* Write page */
                                        eFSS_LOGCPRV_FlushBufferAsNewestPage(p_ptCtx, p_ptCtx->uNewPagIdx);
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

e_eFSS_LOGC_RES eFSS_LOGC_AddLog(t_eFSS_LOGC_Ctx* const p_ptCtx, uint8_t* p_puLogToSave, uint32_t p_uLogL)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_TYPE_StorBuf l_tBuff;
    uint32_t l_uNPageU;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puLogToSave ) )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOGC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOGCPRV_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
                    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                    {
                        l_eResHL = eFSS_COREHL_GetBuff(&p_ptCtx->tCOREHLCtx, &l_tBuff);
                        l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);
                        if( e_eFSS_LOGC_RES_OK == l_eRes )
                        {
                            l_eRes = eFSS_LOGC_LoadIndexNRepair(p_ptCtx);
                            if( e_eFSS_LOGC_RES_OK == l_eRes )
                            {
                                /* Read current write page */
                                /* If page full go next */
                                /* Update page and index */
                                /* Get buffer for calculation */
                                l_eRes =  eFSS_LOGCPRV_LoadBufferAsNewestPage(p_ptCtx, p_ptCtx->uNewPagIdx);

                                if( e_eFSS_LOGC_RES_OK == l_eRes )
                                {
                                    if( ( l_tBuff.uBufL - l_tBuff.ptMeta->uPageUseSpec3 ) > p_uLogL )
                                    {
                                        /* We dont' have space to append log, we need to increase the page.
                                        *    1 - Transform original page in normal log page
                                        *    2 - Add the new bkup pages after the current bkup page
                                        *    3 - If cache is present update cache with the new index
                                        *    4 - trasform the backup page in the original pages
                                        */
                                        /* Step 1 - finalize original page */
                                        l_eRes = eFSS_LOGCPRV_FlushBufferAsLog(p_ptCtx, p_ptCtx->uNewPagIdx);

                                        if( e_eFSS_LOGC_RES_OK == l_eRes )
                                        {
                                            /* Step 2 - add the new bkup page after the current bkup page */
                                            l_eRes = eFSS_LOGCPRV_WritePage(p_ptCtx, p_ptCtx->uNPagIdxFound, l_puBuF1, l_uBuF1L,
                                                                            l_puBuF2, l_uBuF2L, l_tPagePrm);
                                        }
                                    }
                                    else
                                    {
                                        /* We have some free space, add data in this page */
                                        memcpy(&l_tBuff.puBuf[l_tBuff.ptMeta->uPageUseSpec3], p_puLogToSave, p_uLogL);
                                        l_tBuff.ptMeta->uPageUseSpec3+= p_uLogL;

                                        /* Write page */
                                        eFSS_LOGCPRV_FlushBufferAsNewestPage(p_ptCtx, p_ptCtx->uNewPagIdx);
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
static e_eFSS_LOGC_RES eFSS_LOGC_LoadIndexNRepair(t_eFSS_LOGC_Ctx* const p_ptCtx)
{
    e_eFSS_LOGC_RES l_eRes;

    /* Search for index in the flash cache if present */
    if( true == p_ptCtx->bFlashCache )
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

static e_eFSS_LOGC_RES eFSS_LOGC_LoadIndexFromCache(t_eFSS_LOGC_Ctx* const p_ptCtx)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    uint32_t l_uIdxN;
    uint32_t l_uIFlP;
    t_eFSS_TYPE_StorBuf l_tBuff;
    e_eFSS_COREHL_RES l_eResHL;

    l_eResHL = eFSS_COREHL_GetBuff(&p_ptCtx->tCOREHLCtx, &l_tBuff);
    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

    if( e_eFSS_LOGC_RES_OK == l_eRes )
    {
        /* Read cache */
        l_eRes = eFSS_LOGCPRV_ReadCache(p_ptCtx, &l_uIdxN, &l_uIFlP);

        if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) )
        {
            /* Ok, index are readed, verify if pointed page is relay the newest page */
            l_eRes = eFSS_LOGCPRV_LoadBufferAsNewestPage(p_ptCtx, l_uIdxN);

            if( e_eFSS_LOGC_RES_OK == l_eRes )
            {
                if( ( l_tBuff.ptMeta->uPageUseSpec1 != l_uIdxN ) || ( l_tBuff.ptMeta->uPageUseSpec2 != l_uIFlP ) )
                {
                    l_eRes = e_eFSS_LOGC_RES_NOTVALIDLOG;
                }
            }
        }
    }

    return l_eRes;
}

static e_eFSS_LOGC_RES eFSS_LOGC_LoadIndxBySearch(t_eFSS_LOGC_Ctx* const p_ptCtx)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_TYPE_StorBuf l_tBuff;
    bool_t l_bIsIndexFound;
    uint32_t l_unSearched;
    uint32_t l_uIdxSearch;

    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

    if( e_eFSS_LOGC_RES_OK == l_eRes )
    {
        l_eResHL = eFSS_COREHL_GetBuff(&p_ptCtx->tCOREHLCtx, &l_tBuff);
        l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            /* First check if pages pointed by caches index are already OK */
            l_eRes = eFSS_LOGCPRV_LoadBufferAsNewestPage(p_ptCtx, p_ptCtx->uNewPagIdx);

            if( e_eFSS_LOGC_RES_OK == l_eRes )
            {
                if( ( l_tBuff.ptMeta->uPageUseSpec1 != p_ptCtx->uNewPagIdx ) ||
                    ( l_tBuff.ptMeta->uPageUseSpec2 != p_ptCtx->uFullFilledP ) )
                {
                    l_bIsIndexFound = false;
                }
                else
                {
                    l_bIsIndexFound = true;
                }
            }
            else if( e_eFSS_LOGC_RES_NOTVALIDLOG == l_eRes )
            {
                l_bIsIndexFound = false;
            }

            if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_NOTVALIDLOG == l_eRes ) )
            {
                l_unSearched = 0u;
                l_uIdxSearch = p_ptCtx->uNewPagIdx;

                while( ( false == l_bIsIndexFound ) && ( l_unSearched < totalPageSearch ) )
                {
                    l_eRes = eFSS_LOGCPRV_LoadBufferAsNewestPage(p_ptCtx, l_uIdxSearch);

                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                    {
                        /* Founded, super!! */
                        l_bIsIndexFound = true;
                        p_ptCtx->uNewPagIdx    = l_uIdxSearch;
                        p_ptCtx->uFullFilledP = l_tBuff.ptMeta->uPageUseSpec2;
                    }
                    else if( e_eFSS_LOGC_RES_NOTVALIDLOG == l_eRes )
                    {
                        /* Not this one */
                        l_bIsIndexFound = false;
                        l_uIdxSearch = eFSS_LOGCPRV_GetNextIndex( p_ptCtx, l_tStorSet, l_uIdxSearch );
                        l_unSearched++;
                    }
                }
            }
        }
    }

	return l_eRes;
}