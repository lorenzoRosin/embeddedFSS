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

e_eFSS_COREHL_RES eFSS_COREHL_GetStorSett(t_eFSS_COREHL_Ctx* p_ptCtx, t_eFSS_TYPE_StorSet* p_ptStorSet)
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

e_eFSS_COREHL_RES eFSS_COREHL_GetBuff(t_eFSS_COREHL_Ctx* p_ptCtx, t_eFSS_TYPE_StorBuf* p_ptBuff)
{
    /* Return local var */
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

    /* Calc local variable */
    t_eFSS_TYPE_StorBuf l_tBuff2;

    if( NULL == p_ptCtx )
    {
        l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
    }
    else
    {
        l_eResLL = eFSS_CORELL_GetBuff(&p_ptCtx->tCORELLCtx, p_ptBuff, &l_tBuff2);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);
    }

    return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_GetBuffNStor(t_eFSS_COREHL_Ctx* p_ptCtx, t_eFSS_TYPE_StorBuf* p_ptBuff,
                                           t_eFSS_TYPE_StorSet* p_ptStorSet)
{
    /* Return local var */
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

    /* Calc local variable */
    t_eFSS_TYPE_StorBuf l_tBuff2;

    if( NULL == p_ptCtx )
    {
        l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
    }
    else
    {
        l_eResLL = eFSS_CORELL_GetBuffNStor(&p_ptCtx->tCORELLCtx, p_ptBuff, &l_tBuff2, p_ptStorSet);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);
    }

    return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_LoadPageInBuff(t_eFSS_COREHL_Ctx* const p_ptCtx, const uint32_t p_uPageIndx)
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
        l_eResLL = eFSS_CORELL_LoadPageInBuff(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_1, p_uPageIndx);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);
    }

    return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_FlushBuffInPage(t_eFSS_COREHL_Ctx* const p_ptCtx, const uint32_t p_uPageIndx)
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
        l_eResLL = eFSS_CORELL_FlushBuffInPage(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_1, p_uPageIndx);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);
    }

    return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_CalcCrcInBuff(t_eFSS_COREHL_Ctx* const p_ptCtx, uint32_t p_uCrcSeed, uint32_t p_uLenCalc,
                                            uint32_t* p_puCrc)
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
        l_eResLL = eFSS_CORELL_CalcCrcInBuff(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_1, p_uCrcSeed, p_uLenCalc,
                                             p_puCrc);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);
    }

    return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_FlushBuffInPageNBkp(t_eFSS_COREHL_Ctx* const p_ptCtx,
								                  const uint32_t p_uOrigIndx, const uint32_t p_uBackupIndx,
                                                  const uint32_t p_uOriSubType, const uint32_t p_uBckUpSubType)
{
    /* Return local var */
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

    /* Local var used for storage  */
    t_eFSS_TYPE_StorBuf l_tBuffCol;

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
            l_eResLL = eFSS_CORELL_GetBuff(&p_ptCtx->tCORELLCtx, &l_tBuffCol);
            l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

            if( e_eFSS_COREHL_RES_OK == l_eRes)
            {
                l_tBuff1.ptMeta->uPageSubType = p_uOriSubType;

                l_eResLL = eFSS_CORELL_FlushBuffInPage(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_1, p_uOrigIndx);
                l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                if( e_eFSS_COREHL_RES_OK == l_eRes)
                {
                    l_tBuff1.ptMeta->uPageSubType = p_uBckUpSubType;

                    l_eResLL = eFSS_CORELL_FlushBuffInPage(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_1, p_uBckUpSubType);
                    l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);
                }
            }
        }
    }

    return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_LoadPageInBuffNRipBkp(t_eFSS_COREHL_Ctx* const p_ptCtx,
                                                    const uint32_t p_uOrigIndx, const uint32_t p_uBackupIndx,
                                                    const uint32_t p_uOriSubType, const uint32_t p_uBckUpSubType )
{
     /* Return local var */
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

    /* Local var used for storage  */
    t_eFSS_TYPE_StorBuf l_tBuff1;
    t_eFSS_TYPE_StorBuf l_tBuff2;

    /* Local var used for calculation */
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
            l_eResLL = eFSS_CORELL_GetBuff(&p_ptCtx->tCORELLCtx, &l_tBuff1, &l_tBuff2);
            l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

            if( e_eFSS_COREHL_RES_OK == l_eRes)
            {
                l_eResLL = eFSS_CORELL_LoadPageInBuff(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_1, p_uOrigIndx);
                l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                if( ( e_eFSS_COREHL_RES_OK == l_eRes ) || ( e_eFSS_COREHL_RES_NOTVALIDPAGE == l_eRes ) ||
                    ( e_eFSS_COREHL_RES_NEWVERSIONFOUND == l_eRes ) )
                {
                    if( ( e_eFSS_COREHL_RES_OK == l_eRes ) && ( p_uOriSubType == l_tBuff1.ptMeta->uPageSubType ) )
                    {
                        isOriginValid = true;
                    }
                    else
                    {
                        isOriginValid = false;
                    }

                    l_eResLL = eFSS_CORELL_LoadPageInBuff(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_2, p_uBackupIndx);
                    l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                    if( ( e_eFSS_COREHL_RES_OK == l_eRes ) || ( e_eFSS_COREHL_RES_NOTVALIDPAGE == l_eRes ) ||
                        ( e_eFSS_COREHL_RES_NEWVERSIONFOUND == l_eRes ) )
                    {
                        if( ( e_eFSS_COREHL_RES_OK == l_eRes ) && ( p_uBckUpSubType == l_tBuff2.ptMeta->uPageSubType ) )
                        {
                            isBackupValid = true;
                        }
                        else
                        {
                            isBackupValid = false;
                        }

                        if( ( true == isOriginValid ) && ( true == isBackupValid ) )
                        {
                            /* Both page are valid, are they identical? */
                            if( ( 0 == memcmp(l_tBuff1.puBuf, l_tBuff2.puBuf, l_tBuff2.uBufL ) ) &&
                                ( l_tBuff1.ptMeta->uPageUseSpec1 == l_tBuff2.ptMeta->uPageUseSpec1 ) &&
                                ( l_tBuff1.ptMeta->uPageUseSpec2 == l_tBuff2.ptMeta->uPageUseSpec2 ) &&
                                ( l_tBuff1.ptMeta->uPageUseSpec3 == l_tBuff2.ptMeta->uPageUseSpec3 ) &&
                                ( l_tBuff1.ptMeta->uPageUseSpec4 == l_tBuff2.ptMeta->uPageUseSpec4 ) )
                            {
                                /* Page are equals*/
                                l_eRes = e_eFSS_COREHL_RES_OK;
                            }
                            else
                            {
                                /* Page are not equals, copy origin in backup */
                                l_tBuff1.ptMeta->uPageSubType = p_uBckUpSubType;
                                l_eResLL = eFSS_CORELL_FlushBuffInPage(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_1,
                                                                       p_uBackupIndx);
                                l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                                l_tBuff1.ptMeta->uPageSubType = p_uOriSubType;
                                if( e_eFSS_COREHL_RES_OK == l_eRes )
                                {
                                    l_eRes = e_eFSS_COREHL_RES_OK_BKP_RCVRD;
                                }
                            }
                        }
                        else if( ( false == isOriginValid ) && ( true == isBackupValid ) )
                        {
                            /* Origin is not valid, repristinate from backup */
                            l_tBuff2.ptMeta->uPageSubType = p_uOriSubType;
                            l_eResLL = eFSS_CORELL_FlushBuffInPage(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_2,
                                                                   p_uOrigIndx);
                            l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);
                            l_tBuff2.ptMeta->uPageSubType = p_uBckUpSubType;
                            if( e_eFSS_COREHL_RES_OK == l_eRes )
                            {
                                memcpy(l_tBuff1.puBuf, l_tBuff2.puBuf, l_tBuff2.uBufL);
                                memcpy(l_tBuff1.ptMeta, l_tBuff2.ptMeta, sizeof(t_eFSS_TYPE_PageMeta) );
                                l_tBuff1.ptMeta->uPageSubType = p_uOriSubType;

                                l_eRes = e_eFSS_COREHL_RES_OK_BKP_RCVRD;
                            }
                        }
                        else if( ( true == isOriginValid ) && ( false == isBackupValid ) )
                        {
                            /* Backup is not valid, repristinate from origin */
                            l_tBuff1.ptMeta->uPageSubType = p_uBckUpSubType;
                            l_eResLL = eFSS_CORELL_FlushBuffInPage(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_1,
                                                                   p_uBackupIndx);
                            l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                            l_tBuff1.ptMeta->uPageSubType = p_uOriSubType;
                            if( e_eFSS_COREHL_RES_OK == l_eRes )
                            {
                                l_eRes = e_eFSS_COREHL_RES_OK_BKP_RCVRD;
                            }
                        }
                        else
                        {
                            /* No a single valid pages found */
                            l_eRes = e_eFSS_COREHL_RES_NOTVALIDPAGE;
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
    e_eFSS_COREHL_RES l_eRes;

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