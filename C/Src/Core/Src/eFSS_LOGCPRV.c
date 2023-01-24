/**
 * @file       eFSS_LOGCPRV.c
 *
 * @brief      LOG Core private module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_LOGCPRV.h"
#include "eFSS_UTILSHLPRV.h"



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
bool_t eFSS_LOGCPRV_IsStatusStillCoherent(const t_eFSS_LOGC_Ctx* p_ptCtx)
{
    bool_t l_bRes;

    /* Check pointer validity */
    if( ( NULL == p_ptCtx->tCtxCb.ptCtxErase ) || ( NULL == p_ptCtx->tCtxCb.fErase ) ||
        ( NULL == p_ptCtx->tCtxCb.ptCtxWrite ) || ( NULL == p_ptCtx->tCtxCb.fWrite ) ||
        ( NULL == p_ptCtx->tCtxCb.ptCtxRead  ) || ( NULL == p_ptCtx->tCtxCb.fRead  ) ||
        ( NULL == p_ptCtx->tCtxCb.ptCtxCrc32 ) || ( NULL == p_ptCtx->tCtxCb.fCrc32 ) )
    {
        l_bRes = false;
    }
    else
    {
        /* Check data validity */
        if( p_ptCtx->uBufL != ( 2u * p_ptCtx->tStorSett.uPagesLen ) )
        {
            l_bRes = false;
        }
        else
        {
            /* Check data validity */
            if( ( 0u != ( p_ptCtx->tStorSett.uTotPages % 2u ) ) ||
                ( ( false == p_ptCtx->bFullBckup ) && ( false == p_ptCtx->bFlashCache ) && ( p_ptCtx->tStorSett.uTotPages < 4u  ) ) ||
                ( ( false == p_ptCtx->bFullBckup ) && ( true  == p_ptCtx->bFlashCache ) && ( p_ptCtx->tStorSett.uTotPages < 6u  ) ) ||
                ( ( true  == p_ptCtx->bFullBckup ) && ( false == p_ptCtx->bFlashCache ) && ( p_ptCtx->tStorSett.uTotPages < 8u  ) ) ||
                ( ( true  == p_ptCtx->bFullBckup ) && ( true  == p_ptCtx->bFlashCache ) && ( p_ptCtx->tStorSett.uTotPages < 10u ) ) )
            {
                l_bRes = false;
            }
            else
            {
                /* Check data validity */
                if( ( p_ptCtx->tStorSett.uPagesLen <= EFSS_PAGEMETASIZE ) || ( 0u != ( p_ptCtx->tStorSett.uPagesLen % 2u ) ) )
                {
                    l_bRes = false;
                }
                else
                {
                    /* Check data validity */
                    if( p_ptCtx->tStorSett.uRWERetry <= 0u )
                    {
                        l_bRes = false;
                    }
                    else
                    {
                        /* Che index validity */
                        if( ( ( false == p_ptCtx->bFullBckup ) && ( false == p_ptCtx->bFlashCache ) && 
                              ( p_ptCtx->uNewPagIdx >= p_ptCtx->tStorSett.uTotPages ) ) ||
                            ( ( false == p_ptCtx->bFullBckup ) && ( true  == p_ptCtx->bFlashCache ) && 
                              ( p_ptCtx->uNewPagIdx >= ( p_ptCtx->tStorSett.uTotPages -2u ) ) ) ||
                            ( ( true  == p_ptCtx->bFullBckup ) && ( false == p_ptCtx->bFlashCache ) && 
                              ( p_ptCtx->uNewPagIdx >= ( p_ptCtx->tStorSett.uTotPages / 2u ) ) ) ||
                            ( ( true  == p_ptCtx->bFullBckup ) && ( true  == p_ptCtx->bFlashCache ) && 
                              ( p_ptCtx->uNewPagIdx >= ( ( p_ptCtx->tStorSett.uTotPages -2u ) / 2u ) ) ) )
                        {
                            l_bRes = false;
                        }
                        else
                        {
                            /* Che filled log validity */
                            if( ( ( false == p_ptCtx->bFullBckup ) && ( false == p_ptCtx->bFlashCache ) && 
                                  ( p_ptCtx->uFullFilledP >= p_ptCtx->tStorSett.uTotPages -3u ) ) ||
                                ( ( false == p_ptCtx->bFullBckup ) && ( true  == p_ptCtx->bFlashCache ) && 
                                  ( p_ptCtx->uFullFilledP >= ( p_ptCtx->tStorSett.uTotPages -2u -3u ) ) ) ||
                                ( ( true  == p_ptCtx->bFullBckup ) && ( false == p_ptCtx->bFlashCache ) && 
                                  ( p_ptCtx->uFullFilledP >= ( ( p_ptCtx->tStorSett.uTotPages / 2u ) -3u ) ) ) ||
                                ( ( true  == p_ptCtx->bFullBckup ) && ( true  == p_ptCtx->bFlashCache ) && 
                                  ( p_ptCtx->uFullFilledP >= ( ( ( p_ptCtx->tStorSett.uTotPages -2u ) / 2u ) ) -3u ) ) )
                            {
                                l_bRes = false;
                            }
                            else
                            {
                                l_bRes = true;
                            }
                        }
                    }
                }
            }
        }
    }

    return l_bRes;
}

e_eFSS_LOGC_RES eFSS_LOGCPRV_HLtoLogRes(const e_eFSS_UTILSHLPRV_RES p_eHLRes)
{
    e_eFSS_LOGC_RES l_eRes;

    switch(p_eHLRes)
    {
        case e_eFSS_UTILSHLPRV_RES_OK:
        {
            l_eRes = e_eFSS_LOGC_RES_OK;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_NOTVALIDPAGE:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_OK_BKP_RCVRD:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_BADPARAM:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_BADPOINTER:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKERASEERR:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKWRITEERR:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKREADERR:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKCRCERR:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_WRITENOMATCHREAD:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CORRUPT:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
            break;
        }

        default:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
            break;
        }
    }

    return l_eRes;
}

uint32_t eFSS_LOGCPRV_GetNextIndex(const t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t p_uIdx)
{
    uint32_t l_uNextIdx;
	uint32_t l_uNPageU;

    /* Calculate n page */
    l_uNPageU = p_ptCtx->tStorSett.uTotPages;

	/* Flash cache will use two flash pages */
    if( true == p_ptCtx->bFlashCache )
    {
        l_uNPageU -= 2u;
    }

	/* Flash full bkup will use twice as pages as normal log */
    if( true == p_ptCtx->bFullBckup )
    {
        l_uNPageU = (uint32_t)( l_uNPageU / 2u );
    }

    if( p_uIdx >= ( l_uNPageU - 1u ) )
    {
        l_uNextIdx = 0u;
    }
    else
    {
        l_uNextIdx = p_uIdx + 1u;
    }

    return l_uNextIdx;
}

uint32_t eFSS_LOGCPRV_GetPrevIndex(const t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t p_uIdx)
{
    uint32_t l_uPrevIdx;
	uint32_t l_uNPageU;

    /* Calculate n page */
    l_uNPageU = p_ptCtx->tStorSett.uTotPages;

	/* Flash cache will use two flash pages */
    if( true == p_ptCtx->bFlashCache )
    {
        l_uNPageU -= 2u;
    }

	/* Flash full bkup will use twice as pages as normal log */
    if( true == p_ptCtx->bFullBckup )
    {
        l_uNPageU = (uint32_t)( l_uNPageU / 2u );
    }

    if( p_uIdx <= 0u )
    {
        l_uPrevIdx = l_uNPageU - 1u;
    }
    else
    {
        l_uPrevIdx = p_uIdx - 1u;
    }

    return l_uPrevIdx;
}

e_eFSS_LOGC_RES eFSS_LOGCPRV_WriteCache(const t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t p_uIdxN, uint32_t p_uIdxO)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;

    /* Local calc variable */
    t_eFSS_TYPE_PageMeta l_tPagePrm;
	uint32_t l_uCachePageIdx;
    uint8_t* l_puBuF1;
    uint32_t l_uBuF1L;
    uint8_t* l_puBuF2;
    uint32_t l_uBuF2L;

    /* Get buffer for calculation */
    l_puBuF1 = p_ptCtx->puBuf;
    l_uBuF1L = p_ptCtx->uBufL / 2u ;
    l_puBuF2 = &p_ptCtx->puBuf[l_uBuF1L];
    l_uBuF2L = p_ptCtx->uBufL / 2u ;

    /* Calculate cache page index */
    l_uCachePageIdx = p_ptCtx->tStorSett.uTotPages - 2u;

    /* Clear buffer */
    memset(l_puBuF1, 0u, l_uBuF1L);

    /* Setup metadata */
    l_tPagePrm.uPageType = EFSS_PAGETYPE_LOG;
    l_tPagePrm.uPageSubType = EFSS_PAGESUBTYPE_LOGCACHEORI;
    l_tPagePrm.uPageVersion = p_ptCtx->tStorSett.uStorageVer;
    l_tPagePrm.uPageUseSpecific1 = p_uIdxN;
    l_tPagePrm.uPageUseSpecific2 = p_uIdxO;
    l_tPagePrm.uPageUseSpecific3 = 0u;
    l_tPagePrm.uPageUseSpecific4 = p_ptCtx->tStorSett.uTotPages;
    l_tPagePrm.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
    l_tPagePrm.uPageCrc = 0u;

    /* Write */
    l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->tCtxCb, l_uCachePageIdx, l_puBuF1, l_uBuF1L,
                                                   l_puBuF2, l_uBuF2L, &l_tPagePrm, p_ptCtx->tStorSett.uRWERetry);

    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eHLRes);

    if( e_eFSS_LOGC_RES_OK == l_eRes )
    {
        /* Clear buffer */
        memset(l_puBuF1, 0u, l_uBuF1L);

        /* Setup metadata */
        l_tPagePrm.uPageType = EFSS_PAGETYPE_LOG;
        l_tPagePrm.uPageSubType = EFSS_PAGESUBTYPE_LOGCACHEBKP;
		l_tPagePrm.uPageVersion = p_ptCtx->tStorSett.uStorageVer;
		l_tPagePrm.uPageUseSpecific1 = p_uIdxN;
		l_tPagePrm.uPageUseSpecific2 = p_uIdxO;
		l_tPagePrm.uPageUseSpecific3 = 0u;
		l_tPagePrm.uPageUseSpecific4 = p_ptCtx->tStorSett.uTotPages;
		l_tPagePrm.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
		l_tPagePrm.uPageCrc = 0u;

        /* Write */
        l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->tCtxCb, (l_uCachePageIdx + 1u), l_puBuF1, l_uBuF1L,
                                                       l_puBuF2, l_uBuF2L, &l_tPagePrm, p_ptCtx->tStorSett.uRWERetry);

        l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eHLRes);
    }

    return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGCPRV_ReadCache(const t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t* p_puIdxN, uint32_t* p_puIdxO)
{
    /* Local variable */
	e_eFSS_UTILSHLPRV_RES l_eRes;
    e_eFSS_UTILSLLPRV_RES l_eResLL;

    bool_t isOriginValid;
    bool_t isBackupValid;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puIdxN ) || ( NULL == p_puIdxO ) )
	{
		l_eRes = e_eFSS_UTILSHLPRV_RES_BADPOINTER;
	}
    else
    {
        /* Check data validity */
        if( ( p_uDataWLen <= EFSS_PAGEMETASIZE ) || ( p_uDataRLen <= EFSS_PAGEMETASIZE ) ||
            ( p_uDataRLen != p_uDataWLen ) || ( p_uReTry <= 0u ) || ( p_uOrigIndx == p_uBackupIndx) )
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_BADPARAM;
        }
        else
        {
            /* Read origin page */
            l_eResLL = eFSS_UTILSLLPRV_ReadPage(p_ptCbCtx, p_uOrigIndx, p_puDataW, p_uDataWLen, p_uReTry);
            l_eRes = eFSS_UTILSHLPRV_LLtoHLRes(l_eResLL);

            if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes)
            {
                /* Read backup page */
                l_eResLL = eFSS_UTILSLLPRV_ReadPage(p_ptCbCtx, p_uBackupIndx, p_puDataR, p_uDataRLen, p_uReTry);
                l_eRes = eFSS_UTILSHLPRV_LLtoHLRes(l_eResLL);

                if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes )
                {
                    /* Verify origin integrity */
                    l_eRes = eFSS_UTILSHLPRV_IsValidPage(p_ptCbCtx, p_uOrigIndx, p_puDataW, p_uDataWLen, p_uReTry,
                                                         &isOriginValid);
                    if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes )
                    {
                        /* Verify backup integrity */
                        l_eRes = eFSS_UTILSHLPRV_IsValidPage(p_ptCbCtx, p_uBackupIndx, p_puDataR, p_uDataRLen, p_uReTry,
                                                            &isBackupValid);

                        if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes )
                        {
                            if( ( true == isOriginValid ) && ( true == isBackupValid ) )
                            {
                                /* Both page are valid, are they identical? */
                                if( 0 == memcmp(p_puDataW, p_puDataR, p_uDataRLen ) )
                                {
                                    /* Page are equals*/
                                    l_eRes = e_eFSS_UTILSHLPRV_RES_OK;
                                }
                                else
                                {
                                    /* Page are not equals, copy origin in backup */
                                    l_eResLL = eFSS_UTILSLLPRV_WritePage(p_ptCbCtx, p_puDataW, p_uDataWLen, p_puDataR,
                                                                        p_uDataRLen, p_uBackupIndx, p_uReTry);
                                    l_eRes = eFSS_UTILSHLPRV_LLtoHLRes(l_eResLL);

                                    if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes )
                                    {
                                        l_eRes = e_eFSS_UTILSHLPRV_RES_OK_BKP_RCVRD;
                                    }
                                }
                            }
                            else if( ( false == isOriginValid ) && ( true == isBackupValid ) )
                            {
                                /* Origin is not valid, repristinate from backup */
                                l_eResLL = eFSS_UTILSLLPRV_WritePage(p_ptCbCtx, p_puDataR, p_uDataRLen, p_puDataW,
                                                                     p_uDataWLen, p_uOrigIndx, p_uReTry);
                                l_eRes = eFSS_UTILSHLPRV_LLtoHLRes(l_eResLL);

                                if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes )
                                {
                                    l_eRes = e_eFSS_UTILSHLPRV_RES_OK_BKP_RCVRD;
                                }
                            }
                            else if( ( true == isOriginValid ) && ( false == isBackupValid ) )
                            {
                                /* Backup is not valid, repristinate from origin */
                                l_eResLL = eFSS_UTILSLLPRV_WritePage(p_ptCbCtx, p_puDataW, p_uDataWLen, p_puDataR,
                                                                     p_uDataRLen, p_uBackupIndx, p_uReTry);
                                l_eRes = eFSS_UTILSHLPRV_LLtoHLRes(l_eResLL);

                                if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes )
                                {
                                    l_eRes = e_eFSS_UTILSHLPRV_RES_OK_BKP_RCVRD;
                                }
                            }
                            else
                            {
                                /* No a single valid pages found */
                                l_eRes = e_eFSS_UTILSHLPRV_RES_NOTVALIDPAGE;
                            }
                        }
                    }
                }
            }
        }
    }

    return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGCPRV_WritePage(const t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t p_uIdx,
                                       uint8_t* p_puBuf, uint32_t p_uBufL,
                                       uint8_t* p_puBufS, uint32_t p_uBufSL,
                                       t_eFSS_TYPE_PageMeta p_tParam)
{
	/* Local variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;
	uint32_t l_uCachePageIdx;
    uint8_t* l_puBuF1;
    uint32_t l_uBuF1L;
    uint8_t* l_puBuF2;
    uint32_t l_uBuF2L;

    /* Get buffer for calculation */
    l_puBuF1 = p_ptCtx->puBuf;
    l_uBuF1L = p_ptCtx->uBufL / 2u ;
    l_puBuF2 = &p_ptCtx->puBuf[l_uBuF1L];
    l_uBuF2L = p_ptCtx->uBufL / 2u ;

    /* Calculate cache page index */
    l_uCachePageIdx = p_ptCtx->tStorSett.uTotPages - 2u;

    /* Clear buffer */
    memset(l_puBuF1, 0u, l_uBuF1L);

    /* Write */
    l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->tCtxCb, l_uCachePageIdx,
                                                l_puBuF1, l_uBuF1L,
                                                l_puBuF2, l_uBuF2L,
                                                &p_tParam, p_ptCtx->tStorSett.uRWERetry);

    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eHLRes);

    if( e_eFSS_LOGC_RES_OK == l_eRes )
    {
		if( true == p_ptCtx->bFullBckup )
		{
			/* Write the bkup page */
			/* Clear buffer */
			memset(l_puBuF1, 0u, l_uBuF1L);

			/* Write */
			l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->tCtxCb, (l_uCachePageIdx + 1u),
														l_puBuF1, l_uBuF1L,
														l_puBuF2, l_uBuF2L,
														&p_tParam, p_ptCtx->tStorSett.uRWERetry);

			l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eHLRes);

		}
    }
}

e_eFSS_LOGC_RES eFSS_LOGCPRV_ReadPage(const t_eFSS_LOGC_Ctx* p_ptCtx,uint32_t p_uIdx,
                                      uint8_t* p_puBuf, uint32_t p_uBufL,
                                      uint8_t* p_puBufS, uint32_t p_uBufSL,
                                      t_eFSS_TYPE_PageMeta* p_ptParam)
{

}

e_eFSS_LOGC_RES eFSS_LOGCPRV_WriteCurrNewPageAndbkup(const t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t p_uIdx,
												  uint8_t* p_puBuf, uint32_t p_uBufL,
												  uint8_t* p_puBufS, uint32_t p_uBufSL,
												  t_eFSS_TYPE_PageMeta p_tParam)
{

}

e_eFSS_LOGC_RES eFSS_LOGCPRV_ReadCurrNewPageAndbkup(const t_eFSS_LOGC_Ctx* p_ptCtx,uint32_t p_uIdx,
                                                 uint8_t* p_puBuf, uint32_t p_uBufL,
                                                 uint8_t* p_puBufS, uint32_t p_uBufSL,
                                                 t_eFSS_TYPE_PageMeta* p_ptParam)
{

}