/**
 * @file       eFSS_COREHL.c
 *
 * @brief      High level core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/* The written page in storage will have this struct:
 * ------------------------------------------------------------------ User data
 * - [uint8_t] -                    -> N byte of user data           |
 * ------------------------------------------------------------------ HL Metadata  (1 byte)
 * - uint8_t   - uPageSubType       -> Page subtype                  |
 * ------------------------------------------------------------------ Under we have LL metadata
 * - LOW LEVEL METADATA                                              |
 * ------------------------------------------------------------------ End of Page
 */



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
e_eFSS_COREHL_RES eFSS_COREHL_InitCtx(t_eFSS_COREHL_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx const p_tCtxCb,
									  const t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff,
                                      const uint32_t p_uBuffL)
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

        if( e_eFSS_COREHL_RES_OK == l_eRes )
        {
            p_ptCtx->uPageSubType = 0u;
        }
    }

    return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_IsInit(t_eFSS_COREHL_Ctx* const p_ptCtx, bool_t* const p_pbIsInit)
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

e_eFSS_COREHL_RES eFSS_COREHL_GetStorSett(t_eFSS_COREHL_Ctx* const p_ptCtx, t_eFSS_TYPE_StorSet* const p_ptStorSet)
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

e_eFSS_COREHL_RES eFSS_COREHL_GetBuff(t_eFSS_COREHL_Ctx* const p_ptCtx, t_eFSS_COREHL_StorBuf* const p_ptBuff)
{
    /* Return local var */
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

    /* Calc local variable */
    t_eFSS_CORELL_StorBuf l_tBuff1;
    t_eFSS_CORELL_StorBuf l_tBuff2;

    if( NULL == p_ptCtx )
    {
        l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
    }
    else
    {
        l_eResLL = eFSS_CORELL_GetBuff(&p_ptCtx->tCORELLCtx, &l_tBuff1, &l_tBuff2);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

        if( e_eFSS_COREHL_RES_OK == l_eRes )
        {
            p_ptBuff->puBuf = l_tBuff1.puBuf;
            p_ptBuff->uBufL = l_tBuff1.uBufL - 1u;
            p_ptBuff->puPageSubType = &p_ptCtx->uPageSubType;
        }
    }

    return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_GetBuffNStor(t_eFSS_COREHL_Ctx* const p_ptCtx, t_eFSS_COREHL_StorBuf* const p_ptBuff,
                                           t_eFSS_TYPE_StorSet* const  p_ptStorSet)
{
    /* Return local var */
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

    /* Calc local variable */
    t_eFSS_CORELL_StorBuf l_tBuff1;
    t_eFSS_CORELL_StorBuf l_tBuff2;

    if( NULL == p_ptCtx )
    {
        l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
    }
    else
    {
        l_eResLL = eFSS_CORELL_GetBuffNStor(&p_ptCtx->tCORELLCtx, &l_tBuff1, &l_tBuff2, p_ptStorSet);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

        if( e_eFSS_COREHL_RES_OK == l_eRes )
        {
            p_ptBuff->puBuf = l_tBuff1.puBuf;
            p_ptBuff->uBufL = l_tBuff1.uBufL - 1u;
            p_ptBuff->puPageSubType = &p_ptCtx->uPageSubType;
        }
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
    t_eFSS_TYPE_StorBuf l_tBuff1;
    t_eFSS_TYPE_StorBuf l_tBuff2;

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
            /* Get reference to the buffer */
            l_eResLL = eFSS_CORELL_GetBuff(&p_ptCtx->tCORELLCtx, &l_tBuff1, &l_tBuff2);
            l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

            if( e_eFSS_COREHL_RES_OK == l_eRes)
            {
                /* Update subtype */
                l_tBuff1.ptMeta->uPageSubType = p_uOriSubType;

                /* Flush */
                l_eResLL = eFSS_CORELL_FlushBuffInPage(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_1, p_uOrigIndx);
                l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                if( e_eFSS_COREHL_RES_OK == l_eRes)
                {
                    /* Update subtype of the backup page */
                    l_tBuff1.ptMeta->uPageSubType = p_uBckUpSubType;

                    /* Flush backup page */
                    l_eResLL = eFSS_CORELL_FlushBuffInPage(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_1,
                                                           p_uBackupIndx);
                    l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                    if( e_eFSS_COREHL_RES_OK == l_eRes)
                    {
                        /* Ripristinate original page */
                        l_tBuff1.ptMeta->uPageSubType = p_uOriSubType;
                    }
                }
            }
        }
    }

    return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_LoadPageInBuffNRipBkp(t_eFSS_COREHL_Ctx* const p_ptCtx,
                                                    const uint32_t p_uOrigIndx, const uint32_t p_uBackupIndx,
                                                    const uint32_t p_uOriSubType, const uint32_t p_uBckUpSubType)
{
     /* Return local var */
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

    /* Local var used for storage  */
    t_eFSS_TYPE_StorBuf l_tBuff1;
    t_eFSS_TYPE_StorBuf l_tBuff2;

    /* Local var used for calculation */
    bool_t l_bIsOrigValid;
    bool_t l_bIsBkupValid;

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
            /* Load storage buffer */
            l_eResLL = eFSS_CORELL_GetBuff(&p_ptCtx->tCORELLCtx, &l_tBuff1, &l_tBuff2);
            l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

            if( e_eFSS_COREHL_RES_OK == l_eRes)
            {
                /* Load original page in internal buffer */
                l_eResLL = eFSS_CORELL_LoadPageInBuff(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_1, p_uOrigIndx);
                l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                if( ( e_eFSS_COREHL_RES_OK == l_eRes ) || ( e_eFSS_COREHL_RES_NOTVALIDPAGE == l_eRes ) ||
                    ( e_eFSS_COREHL_RES_NEWVERSIONFOUND == l_eRes ) )
                {
                    /* Page readed, is valid? */
                    if( ( e_eFSS_COREHL_RES_OK == l_eRes ) && ( p_uOriSubType == l_tBuff1.ptMeta->uPageSubType ) )
                    {
                        l_bIsOrigValid = true;
                    }
                    else
                    {
                        l_bIsOrigValid = false;
                    }

                    /* Load backup page in internal buffer */
                    l_eResLL = eFSS_CORELL_LoadPageInBuff(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_2, p_uBackupIndx);
                    l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                    if( ( e_eFSS_COREHL_RES_OK == l_eRes ) || ( e_eFSS_COREHL_RES_NOTVALIDPAGE == l_eRes ) ||
                        ( e_eFSS_COREHL_RES_NEWVERSIONFOUND == l_eRes ) )
                    {
                        /* Page readed, is valid? */
                        if( ( e_eFSS_COREHL_RES_OK == l_eRes ) && ( p_uBckUpSubType == l_tBuff2.ptMeta->uPageSubType ) )
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
                            /* Both page are valid, are they identical? */
                            if( ( 0 == memcmp(l_tBuff1.puBuf, l_tBuff2.puBuf, l_tBuff2.uBufL ) ) &&
                                ( l_tBuff1.ptMeta->uPageUseSpec1 == l_tBuff2.ptMeta->uPageUseSpec1 ) &&
                                ( l_tBuff1.ptMeta->uPageUseSpec2 == l_tBuff2.ptMeta->uPageUseSpec2 ) &&
                                ( l_tBuff1.ptMeta->uPageUseSpec3 == l_tBuff2.ptMeta->uPageUseSpec3 ) &&
                                ( l_tBuff1.ptMeta->uPageUseSpec4 == l_tBuff2.ptMeta->uPageUseSpec4 ) )
                            {
                                /* Page are equals */
                                l_eRes = e_eFSS_COREHL_RES_OK;
                            }
                            else
                            {
                                /* Page are not equals, copy origin in backup */
                                l_tBuff1.ptMeta->uPageSubType = p_uBckUpSubType;
                                l_eResLL = eFSS_CORELL_FlushBuffInPage(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_1,
                                                                       p_uBackupIndx);
                                l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                                /* Ripristinate original one */
                                l_tBuff1.ptMeta->uPageSubType = p_uOriSubType;

                                /* If recovered return it */
                                if( e_eFSS_COREHL_RES_OK == l_eRes )
                                {
                                    l_eRes = e_eFSS_COREHL_RES_OK_BKP_RCVRD;
                                }
                            }
                        }
                        else if( ( false == l_bIsOrigValid ) && ( true == l_bIsBkupValid ) )
                        {
                            /* Original page is not valid, ripristinate it from the backup one */
                            l_tBuff2.ptMeta->uPageSubType = p_uOriSubType;
                            l_eResLL = eFSS_CORELL_FlushBuffInPage(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_2,
                                                                   p_uOrigIndx);
                            l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                            /* Ripristinate backup one */
                            l_tBuff2.ptMeta->uPageSubType = p_uBckUpSubType;

                            /* If recovered return it */
                            if( e_eFSS_COREHL_RES_OK == l_eRes )
                            {
                                /* Buffer 1 must contains the original page */
                                memcpy(l_tBuff1.puBuf, l_tBuff2.puBuf, l_tBuff2.uBufL);
                                memcpy(l_tBuff1.ptMeta, l_tBuff2.ptMeta, sizeof(t_eFSS_TYPE_PageMeta) );
                                l_tBuff1.ptMeta->uPageSubType = p_uOriSubType;

                                l_eRes = e_eFSS_COREHL_RES_OK_BKP_RCVRD;
                            }
                        }
                        else if( ( true == l_bIsOrigValid ) && ( false == l_bIsBkupValid ) )
                        {
                            /* Backup is not valid, ripristinate it from the origin one */
                            l_tBuff1.ptMeta->uPageSubType = p_uBckUpSubType;
                            l_eResLL = eFSS_CORELL_FlushBuffInPage(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_1,
                                                                   p_uBackupIndx);
                            l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                            /* Ripristinate original one */
                            l_tBuff1.ptMeta->uPageSubType = p_uOriSubType;

                            /* If recovered return it */
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

        case e_eFSS_CORELL_RES_NOTVALIDPAGE:
        {
            l_eRes = e_eFSS_COREHL_RES_NOTVALIDPAGE;
            break;
        }

        case e_eFSS_CORELL_RES_NEWVERSIONFOUND:
        {
            l_eRes = e_eFSS_COREHL_RES_NEWVERSIONFOUND;
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