/**
 * @file       eFSS_COREHL.c
 *
 * @brief      High level core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_COREHL.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static e_eFSS_COREHL_RES eFSS_COREHL_LLtoHLRes(const e_eFSS_CORELL_RES p_eLLRes);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_COREHL_RES eFSS_COREHL_InitCtx(t_eFSS_COREHL_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx const p_tCtxCb,
									  t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL)
{
    /* Return local var */
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

    if( NULL == p_ptCtx )
    {
        l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
    }
    else
    {
        l_eResLL = eFSS_CORELL_InitCtx(&p_ptCtx->tCORELLCtx, p_tCtxCb, p_tStorSet, p_puBuff, p_uBuffL);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);
    }

    return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_IsInit(t_eFSS_COREHL_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
    /* Return local var */
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

    if( NULL == p_ptCtx )
    {
        l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
    }
    else
    {
        l_eResLL = eFSS_CORELL_IsInit(&p_ptCtx->tCORELLCtx, p_pbIsInit);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);
    }

    return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_GetStorSett(t_eFSS_COREHL_Ctx* const p_ptCtx, t_eFSS_TYPE_StorSet* p_ptStorSet)
{
    /* Return local var */
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

    if( NULL == p_ptCtx )
    {
        l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
    }
    else
    {
        l_eResLL = eFSS_CORELL_GetStorSett(&p_ptCtx->tCORELLCtx, p_ptStorSet);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);
    }

    return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_GetBuff(t_eFSS_COREHL_Ctx* const p_ptCtx, t_eFSS_TYPE_StorBuf* p_ptBuff1,
                                      t_eFSS_TYPE_StorBuf* p_ptBuff2)
{
    /* Return local var */
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

    if( NULL == p_ptCtx )
    {
        l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
    }
    else
    {
        l_eResLL = eFSS_CORELL_GetBuff(&p_ptCtx->tCORELLCtx, p_ptBuff1, p_ptBuff2);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);
    }

    return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_LoadPageInBuff(t_eFSS_COREHL_Ctx* const p_ptCtx, e_eFSS_TYPE_BUFFTYPE p_eBuffType,
								             const uint32_t p_uPageIndx)
{
    /* Return local var */
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

    if( NULL == p_ptCtx )
    {
        l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
    }
    else
    {
        l_eResLL = eFSS_CORELL_LoadPageInBuff(&p_ptCtx->tCORELLCtx, p_eBuffType, p_uPageIndx);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);
    }

    return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_FlushBuffInPage(t_eFSS_COREHL_Ctx* const p_ptCtx, e_eFSS_TYPE_BUFFTYPE p_eBuffType,
								              const uint32_t p_uPageIndx)
{
    /* Return local var */
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

    if( NULL == p_ptCtx )
    {
        l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
    }
    else
    {
        l_eResLL = eFSS_CORELL_FlushBuffInPage(&p_ptCtx->tCORELLCtx, p_eBuffType, p_uPageIndx);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);
    }

    return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_CalcCrcInBuff(t_eFSS_COREHL_Ctx* const p_ptCtx, e_eFSS_TYPE_BUFFTYPE p_eBuffType,
								            uint32_t p_uCrcSeed, uint32_t p_uLenCalc, uint32_t* p_puCrc)
{
    /* Return local var */
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

    if( NULL == p_ptCtx )
    {
        l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
    }
    else
    {
        l_eResLL = eFSS_CORELL_CalcCrcInBuff(&p_ptCtx->tCORELLCtx, p_eBuffType, p_uCrcSeed, p_uLenCalc, p_puCrc);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);
    }

    return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_VerifyNRipristBkup(t_eFSS_COREHL_Ctx* const p_ptCtx, const uint32_t p_uOrigIndx,
                                                 const uint32_t p_uBackupIndx, const uint32_t p_uOriSubType,
                                                 const uint32_t p_uBckUpSubType )
{
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;
    t_eFSS_TYPE_StorBuf* buff1;
    t_eFSS_TYPE_StorBuf* buff2;
    bool_t isOriginValid;
    bool_t isBackupValid;

    if( NULL == p_ptCtx )
    {
        l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
    }
    else
    {
        if( p_uBackupIndx == p_uOrigIndx )
        {
            l_eRes = e_eFSS_COREHL_RES_BADPARAM;
        }
        else
        {
            l_eResLL = eFSS_CORELL_GetBuff(&p_ptCtx->tCORELLCtx, &buff1, &buff2);
            l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

            if( e_eFSS_CORELL_RES_OK == l_eRes)
            {
                l_eResLL = eFSS_CORELL_LoadPageInBuff(&p_ptCtx->tCORELLCtx, e_eFSS_TYPE_BUFFTYPE_1, p_uOrigIndx);
                l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                if( e_eFSS_CORELL_RES_OK == l_eRes)
                {
                    l_eResLL = eFSS_CORELL_LoadPageInBuff(&p_ptCtx->tCORELLCtx, e_eFSS_TYPE_BUFFTYPE_2, p_uBackupIndx);
                    l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);



                }
            }










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

/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static e_eFSS_COREHL_RES eFSS_COREHL_LLtoHLRes(const e_eFSS_CORELL_RES p_eLLRes)
{
    e_eFSS_CORELL_RES l_eRes;

    switch(p_eLLRes)
    {
        case e_eFSS_CORELL_RES_OK:
        {
            l_eRes = e_eFSS_COREHL_RES_OK;
            break;
        }

        case e_eFSS_CORELL_RES_NOINITLIB:
        {
            l_eRes = e_eFSS_COREHL_RES_NOINITLIB;
            break;
        }

        case e_eFSS_CORELL_RES_BADPARAM:
        {
            l_eRes = e_eFSS_COREHL_RES_BADPARAM;
            break;
        }

        case e_eFSS_CORELL_RES_BADPOINTER:
        {
            l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
            break;
        }

        case e_eFSS_CORELL_RES_CORRUPTCTX:
        {
            l_eRes = e_eFSS_COREHL_RES_CORRUPTCTX;
            break;
        }

        case e_eFSS_CORELL_RES_CLBCKERASEERR:
        {
            l_eRes = e_eFSS_COREHL_RES_CLBCKERASEERR;
            break;
        }

        case e_eFSS_CORELL_RES_CLBCKWRITEERR:
        {
            l_eRes = e_eFSS_COREHL_RES_CLBCKWRITEERR;
            break;
        }

        case e_eFSS_CORELL_RES_CLBCKREADERR:
        {
            l_eRes = e_eFSS_COREHL_RES_CLBCKREADERR;
            break;
        }

        case e_eFSS_CORELL_RES_CLBCKCRCERR:
        {
            l_eRes = e_eFSS_COREHL_RES_CLBCKCRCERR;
            break;
        }

        case e_eFSS_CORELL_RES_WRITENOMATCHREAD:
        {
            l_eRes = e_eFSS_COREHL_RES_WRITENOMATCHREAD;
            break;
        }

        default:
        {
            l_eRes = e_eFSS_COREHL_RES_CORRUPTCTX;
            break;
        }
    }

    return l_eRes;
}