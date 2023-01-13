/**
 * @file       eFSS_LOG.c
 *
 * @brief      LOG module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/* In this module the page field has the following meaning:
 * - uPageUseSpecific1 -> New page Index
 * - uPageUseSpecific2 -> Old page Index
 * - uPageUseSpecific3 -> Valorized byte in page
 * - uPageUseSpecific4 -> N page used
 */

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_LOG.h"
#include "eFSS_UTILSHLPRV.h"


/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_LOG_IsStatusStillCoherent(const t_eFSS_LOG_Ctx* p_ptCtx);
static e_eFSS_LOG_RES eFSS_LOG_HLtoLogRes(const e_eFSS_UTILSHLPRV_RES p_eHLRes);
static e_eFSS_LOG_RES eFSS_LOG_LoadIndexNRepair(t_eFSS_LOG_Ctx* const p_ptCtx);
static e_eFSS_LOG_RES eFSS_LOG_LoadIndexFromCache(t_eFSS_LOG_Ctx* const p_ptCtx);
static e_eFSS_LOG_RES eFSS_LOG_LoadIndxBySearch(t_eFSS_LOG_Ctx* const p_ptCtx);
static e_eFSS_LOG_RES eFSS_LOG_UpdateFlashCache(t_eFSS_LOG_Ctx* const p_ptCtx, t_eFSS_TYPE_PageMeta* p_ptMeta);
static e_eFSS_LOG_RES eFSS_LOG_WriteCurrNewPageAndbkup(t_eFSS_LOG_Ctx* const p_ptCtx, uint8_t* p_puNewPage, t_eFSS_TYPE_PageMeta* p_ptMeta);
static e_eFSS_LOG_RES eFSS_LOG_IncrNewPageWriteAndbkup(t_eFSS_LOG_Ctx* const p_ptCtx, uint8_t* p_puNewPage, t_eFSS_TYPE_PageMeta* p_ptMeta);
static uint32_t eFSS_LOG_GetNextIndex(uint32_t p_uCurrIdx, uint32_t p_uTotPage);
static uint32_t eFSS_LOG_GetPrevIndex(uint32_t p_uCurrIdx, uint32_t p_uTotPage);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_LOG_RES eFSS_LOG_InitCtx(t_eFSS_LOG_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx* const p_ptCtxCb,
                                const uint32_t p_uPageToUse, const uint32_t p_uPageSize, uint8_t* const p_puBuff,
                                uint32_t p_uBuffL, uint16_t p_uLogVersion, uint32_t p_uRetry,
                                bool_t p_bUseFlashCache, bool_t p_bUseFullBckup)
{
    e_eFSS_LOG_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ptCtxCb ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_ptCtxCb->ptCtxErase ) || ( NULL == p_ptCtxCb->fErase ) ||
            ( NULL == p_ptCtxCb->ptCtxWrite ) || ( NULL == p_ptCtxCb->fWrite ) ||
            ( NULL == p_ptCtxCb->ptCtxRead  ) || ( NULL == p_ptCtxCb->fRead  ) ||
            ( NULL == p_ptCtxCb->ptCtxCrc32 ) || ( NULL == p_ptCtxCb->fCrc32 ) )
        {
            l_eRes = e_eFSS_LOG_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( ( p_uPageToUse <= 2u ) || ( p_uPageSize <= EFSS_PAGEMETASIZE ) || ( ( p_uPageSize * 2u ) != p_uBuffL ) )
            {
                l_eRes = e_eFSS_LOG_RES_BADPARAM;
            }
            else
            {
                /* Fill context */
                p_ptCtx->bIsInit = true;
                p_ptCtx->ptCtxCb = p_ptCtxCb;
                p_ptCtx->puBuff1 = p_puBuff;
                p_ptCtx->uBuff1L = p_uBuffL / 2u;
                p_ptCtx->puBuff2 = &p_puBuff[p_ptCtx->uBuff1L];
                p_ptCtx->uBuff2L = p_uBuffL / 2u;
                p_ptCtx->uNPage = p_uPageToUse;
                p_ptCtx->uPageSize = p_uPageSize;
            }
        }
    }

    return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_IsInit(t_eFSS_LOG_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eFSS_LOG_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInit;
        l_eRes = e_eFSS_LOG_RES_OK;
	}

	return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_GetStorageStatus(t_eFSS_LOG_Ctx* const p_ptCtx)
{
	/* Local variable */
	e_eFSS_LOG_RES l_eRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
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
                l_eRes = eFSS_LOG_LoadIndexNRepair(p_ptCtx);
            }
		}
	}

	return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_GetLogInfo(t_eFSS_LOG_Ctx* const p_ptCtx, uint32_t *p_puNewLogI, uint32_t *p_puOldLogI,
                                   uint32_t *p_puNpageUsed, uint32_t *p_puLogVer)
{
	/* Local variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;
    t_eFSS_TYPE_PageMeta l_tPagePrm;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puNewLogI ) )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
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
                /* Fix any memory problem and load index */
                l_eRes = eFSS_LOG_LoadIndexNRepair(p_ptCtx);

                if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) )
                {
                    /* Retrive info from the first page */
                    l_eHLRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, p_ptCtx->uNPagIdxFound,
                                                            p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                            &l_tPagePrm, p_ptCtx->uReTry);

                    if( e_eFSS_UTILSHLPRV_RES_OK != l_eHLRes )
                    {
                        l_eRes = eFSS_LOG_HLtoLogRes(l_eHLRes);
                    }
                    else
                    {
                        *p_puNewLogI = l_tPagePrm.uPageUseSpecific1;
                        *p_puOldLogI = l_tPagePrm.uPageUseSpecific2;
                        *p_puNpageUsed = l_tPagePrm.uPageUseSpecific4;
                        *p_puLogVer = l_tPagePrm.uPageVersion;
                    }
                }
            }
		}
	}

	return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_Format(t_eFSS_LOG_Ctx* const p_ptCtx)
{
	/* Local variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;
    t_eFSS_TYPE_PageMeta l_tPagePrm;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
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
                /* In case of many page a full erase of the area could take too times, simply format index, and
                   the new pages */
                /* First erase the newest page and it's backup, after, only if they are present, the flash caches
                   pages */
                /* Clear buffer */
                memset(p_ptCtx->puBuff1, 0u, p_ptCtx->uBuff1L);

                /* Setup metadata */
                l_tPagePrm.uPageType = EFSS_PAGETYPE_LOG;
                l_tPagePrm.uPageSubType = EFSS_PAGESUBTYPE_LOG;
                l_tPagePrm.uPageVersion = p_ptCtx->uVersion;
                l_tPagePrm.uPageUseSpecific1 = 0u;
                l_tPagePrm.uPageUseSpecific2 = 0u;
                l_tPagePrm.uPageUseSpecific3 = 0u;
                l_tPagePrm.uPageUseSpecific4 = 0u;
                l_tPagePrm.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
                l_tPagePrm.uPageCrc = 0u;

                /* Write */
                l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->ptCtxCb, 0u,
                                                               p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                               p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                               &l_tPagePrm, p_ptCtx->uReTry);
                l_eRes = eFSS_LOG_HLtoLogRes(l_eHLRes);

                if( e_eFSS_LOG_RES_OK == l_eRes )
                {
                    /* Clear buffer */
                    memset(p_ptCtx->puBuff1, 0u, p_ptCtx->uBuff1L);

                    /* Setup metadata */
                    l_tPagePrm.uPageType = EFSS_PAGETYPE_LOG;
                    l_tPagePrm.uPageSubType = EFSS_PAGESUBTYPE_LOG;
                    l_tPagePrm.uPageVersion = p_ptCtx->uVersion;
                    l_tPagePrm.uPageUseSpecific1 = 0u;
                    l_tPagePrm.uPageUseSpecific2 = 0u;
                    l_tPagePrm.uPageUseSpecific3 = 0u;
                    l_tPagePrm.uPageUseSpecific4 = 0u;
                    l_tPagePrm.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
                    l_tPagePrm.uPageCrc = 0u;

                    /* Write */
                    l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->ptCtxCb, 1u,
                                                                p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                                p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                                &l_tPagePrm, p_ptCtx->uReTry);
                    l_eRes = eFSS_LOG_HLtoLogRes(l_eHLRes);

                    if( e_eFSS_LOG_RES_OK == l_eRes )
                    {
                        /* Now we can clear the first two pages*/
                        memset(p_ptCtx->puBuff1, 0u, p_ptCtx->uBuff1L);

                        /* Setup metadata */
                        l_tPagePrm.uPageType = EFSS_PAGETYPE_LOG;
                        l_tPagePrm.uPageSubType = EFSS_PAGESUBTYPE_LOGNEWEST;
                        l_tPagePrm.uPageVersion = p_ptCtx->uVersion;
                        l_tPagePrm.uPageUseSpecific1 = 0u;
                        l_tPagePrm.uPageUseSpecific2 = 0u;
                        l_tPagePrm.uPageUseSpecific3 = 0u;
                        l_tPagePrm.uPageUseSpecific4 = 0u;
                        l_tPagePrm.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
                        l_tPagePrm.uPageCrc = 0u;

                        /* Write */
                        l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->ptCtxCb, 0u,
                                                                       p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                                       p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                                       &l_tPagePrm, p_ptCtx->uReTry);
                        l_eRes = eFSS_LOG_HLtoLogRes(l_eHLRes);

                        if( e_eFSS_LOG_RES_OK == l_eRes )
                        {
                            /* Clear buffer */
                            memset(p_ptCtx->puBuff1, 0u, p_ptCtx->uBuff1L);

                            /* Setup metadata */
                            l_tPagePrm.uPageType = EFSS_PAGETYPE_LOG;
                            l_tPagePrm.uPageSubType = EFSS_PAGESUBTYPE_LOGNEWESTBKP;
                            l_tPagePrm.uPageVersion = p_ptCtx->uVersion;
                            l_tPagePrm.uPageUseSpecific1 = 0u;
                            l_tPagePrm.uPageUseSpecific2 = 0u;
                            l_tPagePrm.uPageUseSpecific3 = 0u;
                            l_tPagePrm.uPageUseSpecific4 = 0u;
                            l_tPagePrm.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
                            l_tPagePrm.uPageCrc = 0u;

                            /* Write */
                            l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->ptCtxCb, 1u,
                                                                        p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                                        p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                                        &l_tPagePrm, p_ptCtx->uReTry);
                            l_eRes = eFSS_LOG_HLtoLogRes(l_eHLRes);

                            if( e_eFSS_LOG_RES_OK == l_eRes )
                            {
                                /* Clear flash cache if present */
                                if( true == p_ptCtx->bUseFlashCache )
                                {
                                    /* Setup metadata */
                                    l_tPagePrm.uPageType = EFSS_PAGETYPE_LOG;
                                    l_tPagePrm.uPageSubType = EFSS_PAGESUBTYPE_LOGNEWESTBKP;
                                    l_tPagePrm.uPageVersion = p_ptCtx->uVersion;
                                    l_tPagePrm.uPageUseSpecific1 = 0u;
                                    l_tPagePrm.uPageUseSpecific2 = 0u;
                                    l_tPagePrm.uPageUseSpecific3 = 0u;
                                    l_tPagePrm.uPageUseSpecific4 = 0u;
                                    l_tPagePrm.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
                                    l_tPagePrm.uPageCrc = 0u;

                                    l_eRes = eFSS_LOG_UpdateFlashCache(p_ptCtx, &l_tPagePrm);
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

e_eFSS_LOG_RES eFSS_LOG_AddLog(t_eFSS_LOG_Ctx* const p_ptCtx, uint8_t* p_puLogToSave, uint32_t p_uLogL)
{
    /* Read current write page */

    /* If page full go next */

    /* Update page and index */

    /* Return */
	/* Local variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;
    t_eFSS_TYPE_PageMeta l_tPagePrm;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puLogToSave ) )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
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
                /* Check params validity */
                if( p_uLogL >= 1000u  )
                {
                    l_eRes = e_eFSS_LOG_RES_BADPARAM;
                }
                else
                {
                    /* Fix any memory problem and load index */
                    l_eRes = eFSS_LOG_LoadIndexNRepair(p_ptCtx);

                    if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) )
                    {
                        l_eHLRes =  eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, p_ptCtx->uNPagIdxFound,
                                                                 p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                                 &l_tPagePrm, p_ptCtx->uReTry);
                        l_eRes = eFSS_LOG_HLtoLogRes(l_eHLRes);

                        if( e_eFSS_LOG_RES_OK == l_eRes )
                        {
                            if( (l_tPagePrm.uPageUseSpecific3 - 1000u) >= p_uLogL )
                            {
                                /* Rol the page and write */
                                eFSS_LOG_IncrNewPageWriteAndbkup(p_ptCtx, p_ptCtx->puBuff1, &l_tPagePrm);
                            }
                            else
                            {
                                /* We have some free space, add data in this page */
                                memcpy(&p_ptCtx->puBuff1[l_tPagePrm.uPageUseSpecific3], p_puLogToSave, p_uLogL);
                                l_tPagePrm.uPageUseSpecific3 += p_uLogL;

                                /* Write page */
                                eFSS_LOG_WriteCurrNewPageAndbkup(p_ptCtx, p_ptCtx->puBuff1, &l_tPagePrm);
                            }
                        }
                    }
                }
            }
		}
	}

	return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_GetLogOfASpecificPage(t_eFSS_LOG_Ctx* const p_ptCtx, uint8_t* p_puLogBuf, uint32_t* p_uLogBufL)
{
	/* Local variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;
    t_eFSS_TYPE_PageMeta l_tPagePrm;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
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
                /* Fix any memory problem and load index */
                l_eRes = eFSS_LOG_LoadIndexNRepair(p_ptCtx);

                if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) )
                {
                    l_eHLRes =  eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, p_ptCtx->uNPagIdxFound,
                                                            p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                            &l_tPagePrm, p_ptCtx->uReTry);
                    l_eRes = eFSS_LOG_HLtoLogRes(l_eHLRes);

                    if( e_eFSS_LOG_RES_OK == l_eRes )
                    {
                        /* Copy readed data */
                        memcpy(p_puLogBuf, p_ptCtx->puBuff1, p_ptCtx->uBuff1L);
                    }
                    else
                    {
                        *p_uLogBufL = 0u;
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
	if( (  NULL == p_ptCtx->ptCtxCb ) || ( NULL == p_ptCtx->puBuff1 ) )
	{
		l_eRes = false;
	}
	else
	{
        l_eRes = true;
	}

    return l_eRes;
}

static e_eFSS_LOG_RES eFSS_LOG_HLtoLogRes(const e_eFSS_UTILSHLPRV_RES p_eHLRes)
{
    e_eFSS_LOG_RES l_eRes;

    switch(p_eHLRes)
    {
        case e_eFSS_UTILSHLPRV_RES_OK:
        {
            l_eRes = e_eFSS_LOG_RES_OK;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_NOTVALIDPAGE:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_OK_BKP_RCVRD:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_BADPARAM:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_BADPOINTER:
        {
            l_eRes = e_eFSS_LOG_RES_BADPOINTER;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKERASEERR:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKWRITEERR:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKREADERR:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKCRCERR:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_WRITENOMATCHREAD:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CORRUPT:
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


static e_eFSS_LOG_RES eFSS_LOG_LoadIndexNRepair(t_eFSS_LOG_Ctx* const p_ptCtx)
{
    e_eFSS_LOG_RES l_eRes;

    /* Search for index in the flash cache if present */
    if( true == p_ptCtx->bUseFlashCache )
    {
        /* Load index from cache */
        l_eRes = eFSS_LOG_LoadIndexFromCache(p_ptCtx);
    }
    else
    {
        /* Search for index */
        l_eRes = eFSS_LOG_LoadIndxBySearch(p_ptCtx);
    }

    return l_eRes;
}

static e_eFSS_LOG_RES eFSS_LOG_LoadIndexFromCache(t_eFSS_LOG_Ctx* const p_ptCtx)
{
    /* Index are present in the first page and it's backup */
    e_eFSS_UTILSHLPRV_RES l_eIntegrityRes;
    e_eFSS_LOG_RES l_eRes;
    t_eFSS_TYPE_PageMeta l_tPagePrm;

    /* Verify and create page index integrity */
    l_eIntegrityRes =  eFSS_UTILSHLPRV_VerifyNRipristBkup(p_ptCtx->ptCtxCb, p_ptCtx->uReTry,
                                                          p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                          p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                          0u, 1u);
    l_eRes = eFSS_LOG_HLtoLogRes(l_eIntegrityRes);

    if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) )
    {
        /* Index page is OK, we can retrive info from that */
        l_eIntegrityRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, 0u,
                                                       p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                       &l_tPagePrm, p_ptCtx->uReTry);
        l_eRes = eFSS_LOG_HLtoLogRes(l_eIntegrityRes);
        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            /* can load finded index in the context */
            p_ptCtx->uNPagIdxFound = l_tPagePrm.uPageUseSpecific1;
            p_ptCtx->uOPagIdxFound = l_tPagePrm.uPageUseSpecific2;

            /* Verify log version */
            if( p_ptCtx->uVersion != l_tPagePrm.uPageVersion )
            {
                /* New version of log found */
                l_eRes = e_eFSS_LOG_RES_NEWVERSIONLOG;
            }
            else
            {
                /* Verify page type, if not coherent we were formatting the area before */
            }
        }
    }

    return l_eRes;
}

static e_eFSS_LOG_RES eFSS_LOG_LoadIndxBySearch(t_eFSS_LOG_Ctx* const p_ptCtx)
{
    /* Verify if index already present are valid, if not search for them */
    e_eFSS_UTILSHLPRV_RES l_eIntegrityRes;
    e_eFSS_LOG_RES l_eRes;
    bool_t l_bIsValid;
    t_eFSS_TYPE_PageMeta p_ptPagePrm;
    bool_t l_bIsIndexFound;
    uint32_t l_i;

    /* Init parameter */
    l_bIsIndexFound = false;

    /* Verify integrity first */
    l_eIntegrityRes = eFSS_UTILSHLPRV_IsValidPage(p_ptCtx->ptCtxCb, p_ptCtx->uNPagIdxFound,
                                                  p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                  p_ptCtx->uReTry,  &l_bIsValid );
    l_eRes = eFSS_LOG_HLtoLogRes(l_eIntegrityRes);

    if( e_eFSS_LOG_RES_OK == l_eRes )
    {
        if( true == l_bIsValid )
        {
            /* The page pointed by uNPagIdxFound is ok, need to check if it'is the newest page */
            l_eIntegrityRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, p_ptCtx->uNPagIdxFound,
                                                           p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                           &p_ptPagePrm, p_ptCtx->uReTry);
            l_eRes = eFSS_LOG_HLtoLogRes(l_eIntegrityRes);
            if( e_eFSS_LOG_RES_OK == l_eRes )
            {
                if( ( EFSS_PAGETYPE_LOG == p_ptPagePrm.uPageType ) &&
                    ( EFSS_PAGESUBTYPE_LOGNEWEST == p_ptPagePrm.uPageSubType ) &&
                    ( p_ptCtx->uVersion == p_ptPagePrm.uPageVersion ) &&
                    ( EFSS_PAGEMAGICNUMBER == p_ptPagePrm.uPageMagicNumber ) )
                {
                    /* Very good, this is the newwest page avaiable  */
                    l_bIsIndexFound = true;
                }
            }
        }

        if( false == l_bIsIndexFound )
        {
            /* Read every page and search for the newest page type */
            l_i = 0u;

            while( ( false == l_bIsIndexFound ) && ( l_i < p_ptCtx->uNPage )  )
            {
                l_eIntegrityRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, l_i,
                                                               p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                               &p_ptPagePrm, p_ptCtx->uReTry);
                l_eRes = eFSS_LOG_HLtoLogRes(l_eIntegrityRes);
                if( e_eFSS_LOG_RES_OK == l_eRes )
                {
                    /* Is the page valid? */
                    l_eIntegrityRes = eFSS_UTILSHLPRV_IsValidPageInBuff(p_ptCtx->ptCtxCb,
                                                                        p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                                        &l_bIsValid);
                    l_eRes = eFSS_LOG_HLtoLogRes(l_eIntegrityRes);

                    if( e_eFSS_LOG_RES_OK == l_eRes )
                    {
                        if( true == l_bIsValid )
                        {
                            /* Valid page, is the one we where searching? */
                            if( ( EFSS_PAGETYPE_LOG == p_ptPagePrm.uPageType ) &&
                                ( EFSS_PAGESUBTYPE_LOGNEWEST == p_ptPagePrm.uPageSubType ) &&
                                ( p_ptCtx->uVersion == p_ptPagePrm.uPageVersion ) &&
                                ( EFSS_PAGEMAGICNUMBER == p_ptPagePrm.uPageMagicNumber ) )
                            {
                                /* Super top, this is the page, nothing more to do  */
                                l_bIsIndexFound = true;
                                p_ptCtx->uNPagIdxFound = l_i;
                                p_ptCtx->uOPagIdxFound = p_ptPagePrm.uPageUseSpecific1;
                            }
                            else if( ( EFSS_PAGETYPE_LOG == p_ptPagePrm.uPageType ) &&
                                ( EFSS_PAGESUBTYPE_LOGNEWESTBKP == p_ptPagePrm.uPageSubType ) &&
                                ( p_ptCtx->uVersion == p_ptPagePrm.uPageVersion ) &&
                                ( EFSS_PAGEMAGICNUMBER == p_ptPagePrm.uPageMagicNumber ) )
                            {
                                /* Super top, this is the page, nothing more to do  */
                                l_bIsIndexFound = true;
                                p_ptCtx->uNPagIdxFound = eFSS_LOG_GetPrevIndex( l_i, p_ptCtx->uNPage );
                                p_ptCtx->uOPagIdxFound = p_ptPagePrm.uPageUseSpecific1;
                            }
                        }
                    }
                }
            }
        }
    }

    return l_eRes;
}

static e_eFSS_LOG_RES eFSS_LOG_UpdateFlashCache(t_eFSS_LOG_Ctx* const p_ptCtx, t_eFSS_TYPE_PageMeta* p_ptMeta)
{
	/* Local variable */
	e_eFSS_LOG_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;
    t_eFSS_TYPE_PageMeta l_tPagePrm;

    /* Clear buffer */
    memset(p_ptCtx->puBuff1, 0u, p_ptCtx->uBuff1L);

    /* Setup metadata */
    l_tPagePrm.uPageType = EFSS_PAGETYPE_LOG;
    l_tPagePrm.uPageSubType = EFSS_PAGESUBTYPE_LOGCACHEORI;
    l_tPagePrm.uPageVersion = p_ptCtx->uVersion;
    l_tPagePrm.uPageUseSpecific1 = 0u;
    l_tPagePrm.uPageUseSpecific2 = 0u;
    l_tPagePrm.uPageUseSpecific3 = 0u;
    l_tPagePrm.uPageUseSpecific4 = 0u;
    l_tPagePrm.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
    l_tPagePrm.uPageCrc = 0u;

    /* Write */
    l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->ptCtxCb, 0u,
                                                p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                &l_tPagePrm, p_ptCtx->uReTry);

    l_eRes = eFSS_LOG_HLtoLogRes(l_eHLRes);

    if( e_eFSS_LOG_RES_OK == l_eRes )
    {
        /* Clear buffer */
        memset(p_ptCtx->puBuff1, 0u, p_ptCtx->uBuff1L);

        /* Setup metadata */
        l_tPagePrm.uPageType = EFSS_PAGETYPE_LOG;
        l_tPagePrm.uPageSubType = EFSS_PAGESUBTYPE_LOGCACHEBKP;
        l_tPagePrm.uPageVersion = p_ptCtx->uVersion;
        l_tPagePrm.uPageUseSpecific1 = 0u;
        l_tPagePrm.uPageUseSpecific2 = 0u;
        l_tPagePrm.uPageUseSpecific3 = 0u;
        l_tPagePrm.uPageUseSpecific4 = 0u;
        l_tPagePrm.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
        l_tPagePrm.uPageCrc = 0u;

        /* Write */
        l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->ptCtxCb, 1u,
                                                    p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                    p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                    &l_tPagePrm, p_ptCtx->uReTry);

        l_eRes = eFSS_LOG_HLtoLogRes(l_eHLRes);
    }
}

static e_eFSS_LOG_RES eFSS_LOG_WriteCurrNewPageAndbkup(t_eFSS_LOG_Ctx* const p_ptCtx, uint8_t* p_puNewPage, t_eFSS_TYPE_PageMeta* p_ptMeta)
{


}

static e_eFSS_LOG_RES eFSS_LOG_IncrNewPageWriteAndbkup(t_eFSS_LOG_Ctx* const p_ptCtx, uint8_t* p_puNewPage, t_eFSS_TYPE_PageMeta* p_ptMeta)
{


}

static uint32_t eFSS_LOG_GetNextIndex(uint32_t p_uCurrIdx, uint32_t p_uTotPage)
{
    uint32_t l_uNextIdx;

    if( p_uCurrIdx >= ( p_uTotPage - 1u ) )
    {
        l_uNextIdx = 0u;
    }
    else
    {
        l_uNextIdx = p_uCurrIdx + 1u;
    }
    return l_uNextIdx;
}

static uint32_t eFSS_LOG_GetPrevIndex(uint32_t p_uCurrIdx, uint32_t p_uTotPage)
{
    uint32_t l_uPrevIdx;

    if( p_uCurrIdx <= 0u )
    {
        p_uCurrIdx = p_uTotPage - 1u;
    }
    else
    {
        l_uPrevIdx = p_uCurrIdx - 1u;
    }
    return l_uPrevIdx;
}