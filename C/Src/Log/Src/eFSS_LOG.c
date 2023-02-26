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



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_LOG_IsStatusStillCoherent(const t_eFSS_LOG_Ctx* p_ptCtx);
static e_eFSS_LOG_RES eFSS_LOG_LOGCtoLOGRes(const e_eFSS_LOGC_RES p_eLOGCRes);
static e_eFSS_LOG_RES eFSS_LOGC_LoadIndexNRepair(t_eFSS_LOG_Ctx* const p_ptCtx);
static e_eFSS_LOG_RES eFSS_LOGC_LoadIndexFromCache(t_eFSS_LOG_Ctx* const p_ptCtx);
static e_eFSS_LOG_RES eFSS_LOGC_LoadIndxBySearch(t_eFSS_LOG_Ctx* const p_ptCtx);


/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_LOG_RES eFSS_LOG_InitCtx(t_eFSS_LOG_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx const p_tCtxCb,
                                t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL,
                                bool_t p_bFlashCache, bool_t p_bFullBckup)
{
    e_eFSS_LOG_RES  l_eRes;
    e_eFSS_LOGC_RES l_eLOGCRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
        /* Can init low level context */
        l_eLOGCRes = eFSS_LOGC_InitCtx(&p_ptCtx->tLOGCCtx, p_tCtxCb, p_tStorSet, p_puBuff, p_uBuffL,
                                      p_bFlashCache, p_bFullBckup);
        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);

        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            p_ptCtx->uNewPagIdx = 0;
            p_ptCtx->uFullFilledP = 0;
        }
    }

    return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_IsInit(t_eFSS_LOG_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
    e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES l_eLOGCRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
        l_eLOGCRes = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, p_pbIsInit);
        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);
	}

	return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_GetLogStatus(t_eFSS_LOG_Ctx* const p_ptCtx)
{
	/* Local variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES l_eLOGCRes;

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
        l_eLOGCRes = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, &l_bIsInit);
        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);

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

e_eFSS_LOG_RES eFSS_LOG_GetLogInfo(t_eFSS_LOG_Ctx* const p_ptCtx, uint32_t *p_puNewLogI, uint32_t *p_puOldLogI,
                                   uint32_t *p_puNpageUsed, uint32_t *p_puNpageTot)
{
	/* Local return variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES l_eLOGCRes;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    uint32_t l_uNPageU;
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
        l_eLOGCRes = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, &l_bIsInit);
        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);

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
                        eFSS_LOGC_GetUsablePage(&p_ptCtx->tLOGCCtx, &l_uNPageU);

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

	return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_Format(t_eFSS_LOG_Ctx* const p_ptCtx)
{
	/* Local return variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES l_eLOGCRes;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorBuf l_tBuff;
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
        l_eLOGCRes = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, &l_bIsInit);
        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);

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
                    l_eLOGCRes = eFSS_LOGC_GetBuff(&p_ptCtx->tLOGCCtx, &l_tBuff);
                    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);

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

                                l_eLOGCRes = eFSS_LOGC_FlushBufferAsLog(&p_ptCtx->tLOGCCtx, p_ptCtx->uNewPagIdx);
                                l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);

                                if( e_eFSS_LOG_RES_OK == l_eRes )
                                {
                                    /* Invalidate newest backup also */
                                    eFSS_LOGC_GetNextIndex(&p_ptCtx->tLOGCCtx, p_ptCtx->uNewPagIdx, &l_uNextIndex);
                                    l_eLOGCRes = eFSS_LOGC_FlushBufferAsLog(&p_ptCtx->tLOGCCtx, l_uNextIndex);
                                    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);
                                }
                            }

                            if( e_eFSS_LOG_RES_OK == l_eRes )
                            {
                                /* Write newest in first index */
                                memset(l_tBuff.puBuf, 0u, l_tBuff.uBufL);
                                memset(&l_tBuff.ptMeta, 0u, sizeof(t_eFSS_TYPE_PageMeta));

                                l_eLOGCRes = eFSS_LOGC_FlushBufferAsNewestNBkpPage(&p_ptCtx->tLOGCCtx, 0u);
                                l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);

                                if( e_eFSS_LOG_RES_OK == l_eRes )
                                {
                                    /* write cache */
                                    eFSS_LOGC_IsFlashCacheUsed(&p_ptCtx->tLOGCCtx, &l_bIsFlashCacheUsed);
                                    if( true == l_bIsFlashCacheUsed )
                                    {
                                        l_eLOGCRes = eFSS_LOGC_WriteCache(&p_ptCtx->tLOGCCtx, 0u, 0u);
                                        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);
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

e_eFSS_LOG_RES eFSS_LOG_AddLog(t_eFSS_LOG_Ctx* const p_ptCtx, uint16_t p_uElemL, uint8_t* p_puRawVal)
{
	/* Local return variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES l_eLOGCRes;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorBuf l_tBuff;
    uint32_t l_uPages;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puRawVal ) )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eLOGCRes = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, &l_bIsInit);
        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);

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
                    l_eLOGCRes = eFSS_LOGC_GetBuff(&p_ptCtx->tLOGCCtx, &l_tBuff);
                    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);

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
                                l_eLOGCRes = eFSS_LOGC_LoadBufferAsNewestNBkpPage(&p_ptCtx->tLOGCCtx, p_ptCtx->uNewPagIdx);
                                l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);

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
                                        l_eLOGCRes = eFSS_LOGC_FlushBufferAsNewestNBkpPage(&p_ptCtx->tLOGCCtx, p_ptCtx->uNewPagIdx);
                                        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);
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
                                        l_eLOGCRes = eFSS_LOGC_FlushBufferAsLog(&p_ptCtx->tLOGCCtx, p_ptCtx->uNewPagIdx);
                                        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);

                                        if( e_eFSS_LOG_RES_OK == l_eRes )
                                        {
                                            /* Step 2 - add the new bkup page after the current bkup page */
                                            eFSS_LOGC_GetNextIndex(&p_ptCtx->tLOGCCtx, p_ptCtx->uFullFilledP, &l_uPages);
                                            eFSS_LOGC_GetNextIndex(&p_ptCtx->tLOGCCtx, l_uPages, &l_uPages);
                                            l_eLOGCRes = eFSS_LOGC_FlushBufferAsNewestBkupOnly(&p_ptCtx->tLOGCCtx, l_uPages);
                                            l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);

                                            if( e_eFSS_LOG_RES_OK == l_eRes )
                                            {
                                                /* Step 3 - If cache is present update cache with the new index */
                                                eFSS_LOGC_GetNextIndex(&p_ptCtx->tLOGCCtx, p_ptCtx->uFullFilledP, &l_uPages);
                                                l_eLOGCRes = eFSS_LOGC_WriteCache(&p_ptCtx->tLOGCCtx, l_uPages, p_ptCtx->uFullFilledP + 1u);
                                                l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);

                                                if( e_eFSS_LOG_RES_OK == l_eRes )
                                                {
                                                    /* Step 4 - Trasform the backup page in the original pages */
                                                    eFSS_LOGC_GetNextIndex(&p_ptCtx->tLOGCCtx, p_ptCtx->uFullFilledP, &l_uPages);
                                                    l_eLOGCRes = eFSS_LOGC_FlushBufferAsNewestOnly(&p_ptCtx->tLOGCCtx, l_uPages);
                                                    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);
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

e_eFSS_LOG_RES eFSS_LOG_GetLogOfAPage(t_eFSS_LOG_Ctx* const p_ptCtx, uint32_t p_uindx, uint8_t* p_puBuf,
                                      uint32_t* p_puBufL, uint32_t p_uBufMaxL)
{
	/* Local return variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES l_eLOGCRes;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorBuf l_tBuff;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuf ) || ( NULL == p_puBufL ) )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eLOGCRes = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, &l_bIsInit);
        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);

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
                    l_eLOGCRes = eFSS_LOGC_GetBuff(&p_ptCtx->tLOGCCtx, &l_tBuff);
                    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);

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
                                    l_eLOGCRes =  eFSS_LOGC_LoadBufferAsNewestNBkpPage(&p_ptCtx->tLOGCCtx, p_uindx);
                                    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);
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
                                    l_eLOGCRes =  eFSS_LOGC_LoadBufferAsLog(&p_ptCtx->tLOGCCtx, p_uindx);
                                    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);
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
static bool_t eFSS_LOG_IsStatusStillCoherent(const t_eFSS_LOG_Ctx* p_ptCtx)
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
    e_eFSS_LOGC_RES l_eLOGCRes;

    /* Local variable used for calculation */
    uint32_t l_uIdxN;
    uint32_t l_uIFlP;
    t_eFSS_TYPE_StorBuf l_tBuff;

    l_eLOGCRes = eFSS_LOGC_GetBuff(&p_ptCtx->tLOGCCtx, &l_tBuff);
    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);

    if( e_eFSS_LOG_RES_OK == l_eRes )
    {
        /* Read cache */
        l_eLOGCRes = eFSS_LOGC_ReadCache(&p_ptCtx->tLOGCCtx, &l_uIdxN, &l_uIFlP);
        l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);

        if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) )
        {
            /* Ok, index are readed, verify if pointed page is rely the newest page */
            l_eLOGCRes = eFSS_LOGC_LoadBufferAsNewestNBkpPage(&p_ptCtx->tLOGCCtx, l_uIdxN);
            l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);

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
    e_eFSS_LOGC_RES l_eLOGCRes;

    /* Local var used for settings */
    t_eFSS_TYPE_StorBuf l_tBuff;

    /* Local var used for search */
    uint32_t l_uNSearched;
    uint32_t l_uIdxSearch;
    uint32_t l_uUsablePage;

    l_eLOGCRes = eFSS_LOGC_GetBuff(&p_ptCtx->tLOGCCtx, &l_tBuff);
    l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);

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
            l_eLOGCRes = eFSS_LOGC_LoadBufferAsNewestNBkpPage(&p_ptCtx->tLOGCCtx, p_ptCtx->uNewPagIdx);
            l_eRes = eFSS_LOG_LOGCtoLOGRes(l_eLOGCRes);

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