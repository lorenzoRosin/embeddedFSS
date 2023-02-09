/**
 * @file       eFSS_LOGC.c
 *
 * @brief      LOG Core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/* In this module the page field has the following meaning:
 * - uPageUseSpecific1 -> New page Index
 * - uPageUseSpecific2 -> Number of Filled index page
 * - uPageUseSpecific3 -> Valorized byte in page
 * - uPageUseSpecific4 -> N page used
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
 * An unused page is always left after newer page abckup index.
 * Newest and ondest page index can be identical only when the log storage is empty. When the storage is full an empty
 * page is always present between new and old log index.
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
        if( ( ( false == p_ptCtx->bFullBckup ) && ( false == p_ptCtx->bFlashCache ) && ( l_uNPage < 4u  ) ) ||
            ( ( false == p_ptCtx->bFullBckup ) && ( true  == p_ptCtx->bFlashCache ) && ( l_uNPage < 6u  ) ) ||
            ( ( true  == p_ptCtx->bFullBckup ) && ( false == p_ptCtx->bFlashCache ) && ( l_uNPage < 8u  ) ) ||
            ( ( true  == p_ptCtx->bFullBckup ) && ( true  == p_ptCtx->bFlashCache ) && ( l_uNPage < 10u ) ) )
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
        }
        else
        {
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
                    l_eRes = eFSS_LOGC_LoadIndexNRepair(p_ptCtx);
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGC_GetLogInfo(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t *p_puNewLogI, uint32_t *p_puNpageUsed,
                                     uint32_t *p_puNpageTot)
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
	if( ( NULL == p_ptCtx ) || ( NULL == p_puNewLogI ) || ( NULL == p_puNpageUsed ) || ( NULL == p_puNpageTot ) )
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
                        l_eRes = eFSS_LOGCPRV_GetBuffer(p_ptCtx, &l_tBuff);
                        if( e_eFSS_LOGC_RES_OK == l_eRes )
                        {
                            l_eRes = eFSS_LOGC_LoadIndexNRepair(p_ptCtx);
                            if( e_eFSS_LOGC_RES_OK == l_eRes )
                            {
                                l_eRes = eFSS_LOGCPRV_LoadBufferAsNewestPage(p_ptCtx, p_ptCtx->uNewPagIdx);
                                if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) )
                                {
                                    /* Calculate n page */
                                    l_uNPageU = eFSS_LOGCPRV_GetUsablePage(p_ptCtx, l_tStorSet);

                                    /* Copy result */
                                    *p_puNewLogI = l_tBuff.ptMeta->uPageUseSpec1;
                                    *p_puNpageUsed = l_tBuff.ptMeta->uPageUseSpec2;
                                    *p_puNpageTot = l_uNPageU;
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
                        l_eRes = eFSS_LOGCPRV_GetBuffer(p_ptCtx, &l_tBuff);
                        if( e_eFSS_LOGC_RES_OK == l_eRes )
                        {
                            /* In case of many page a full erase of the area could take too much times.
                               1 - First erase the newest page and it's backup. In this ways even if a power loss occours we have
                                   completly invalidated the whole memory.
                               2 - If present set the index cache to point to the first page of the area. In this ways even if a
                                   power outage occours we have that cache is not pointing to valid page anymore.
                               3 - Setup first page as original newest page.
                               4 - Set second page as backup of first page.
                            */
                            l_eRes = eFSS_LOGC_LoadIndexNRepair(p_ptCtx);

                            if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) )
                            {
                                /* Valid index found, invalidate the pages pointed by index */
                            }
                            else if( e_eFSS_LOGC_RES_NOTVALIDLOG == l_eRes )
                            {
                                /* Set first page as indexed one */
                                p_ptCtx->uNewPagIdx = 0u;
                                p_ptCtx->uFullFilledP = 0u;
                            }
                            else
                            {
                                /* Found some strange error, cannot continue */
                            }

                            /* Start the procedure */
                            if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) ||
                                ( e_eFSS_LOGC_RES_NEWVERSIONLOG == l_eRes ) || ( e_eFSS_LOGC_RES_NOTVALIDLOG == l_eRes ) )
                            {
                                /* Clear buffer  */
                                memset(l_tBuff.puBuf, 0u, l_tBuff.uBufL);
                                memset(&l_tBuff.ptMeta, 0u, sizeof(t_eFSS_TYPE_PageMeta));

                                /* Step 1 - invalidate orginal */

                                /* Write */
                                l_eRes =  eFSS_LOGCPRV_FlushBufferAsLog(p_ptCtx, p_ptCtx->uNewPagIdx);

                                if( e_eFSS_LOGC_RES_OK == l_eRes )
                                {
                                    /* Step 1 - invalidate orginal backup */
                                    /* Clear buffer  */
                                    memset(l_tBuff.puBuf, 0u, l_tBuff.uBufL);
                                    memset(&l_tBuff.ptMeta, 0u, sizeof(t_eFSS_TYPE_PageMeta));

                                    /* Write */
                                    l_uNextIndex = eFSS_LOGCPRV_GetNextIndex(p_ptCtx, l_tStorSet, p_ptCtx->uNewPagIdx);

                                    /* Write */
                                    l_eRes =  eFSS_LOGCPRV_FlushBufferAsLog(p_ptCtx, l_uNextIndex);

                                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                                    {
                                        /* Step 2 - if cache is present make it to point first page */
                                        if( true == p_ptCtx->bFlashCache )
                                        {
                                            l_eRes = eFSS_LOGCPRV_WriteCache(p_ptCtx, 0u, 0u);
                                        }

                                        if( e_eFSS_LOGC_RES_OK == l_eRes )
                                        {
                                            /* Clear buffer  */
                                            memset(l_tBuff.puBuf, 0u, l_tBuff.uBufL);
                                            memset(&l_tBuff.ptMeta, 0u, sizeof(t_eFSS_TYPE_PageMeta));

                                            /* Step 3 - Write first page as first page and bkup page */
                                            l_eRes = eFSS_LOGCPRV_FlushBufferAsNewestPage(p_ptCtx, 0u);
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

e_eFSS_LOGC_RES eFSS_LOGC_GetLogOfASpecificPage(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t p_uindx, uint8_t** p_ppuBuf,
                                                uint32_t* p_uBufL)
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
	if( ( NULL == p_ptCtx ) || ( NULL == p_ppuBuf ) || ( NULL == p_uBufL ) )
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
                        l_eRes = eFSS_LOGCPRV_GetBuffer(p_ptCtx, &l_tBuff);
                        if( e_eFSS_LOGC_RES_OK == l_eRes )
                        {
                            l_eRes = eFSS_LOGC_LoadIndexNRepair(p_ptCtx);
                            if( e_eFSS_LOGC_RES_OK == l_eRes )
                            {
                                if( p_uindx == p_ptCtx->uNewPagIdx )
                                {
                                    l_eRes =  eFSS_LOGCPRV_LoadBufferAsNewestPage(p_ptCtx, p_uindx);
                                }
                                else
                                {
                                    l_eRes =  eFSS_LOGCPRV_LoadBufferAsLog(p_ptCtx, p_uindx);
                                }

                                if( e_eFSS_LOGC_RES_OK == l_eRes )
                                {
                                    /* Copy readed data */
                                    *p_ppuBuf = l_tBuff.puBuf;
                                    *p_uBufL = l_tBuff.uBufL;
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
                        l_eRes = eFSS_LOGCPRV_GetBuffer(p_ptCtx, &l_tBuff);
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

    l_eRes = eFSS_LOGCPRV_GetBuffer(p_ptCtx, &l_tBuff);
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
        l_eRes = eFSS_LOGCPRV_GetBuffer(p_ptCtx, &l_tBuff);
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