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
 *      PRIVATE DEFINE
 **********************************************************************************************************************/
#define EFSS_COREHL_PAGEMIN_L                                                                      ( ( uint32_t )   1u )



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static e_eFSS_COREHL_RES eFSS_COREHL_LLtoHLRes(const e_eFSS_CORELL_RES p_eLLRes);
static bool_t eFSS_COREHL_IsStatusStillCoherent(t_eFSS_COREHL_Ctx* const p_ptCtx);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_COREHL_RES eFSS_COREHL_InitCtx(t_eFSS_COREHL_Ctx* const p_ptCtx, const t_eFSS_TYPE_CbStorCtx p_tCtxCb,
									  const t_eFSS_TYPE_StorSet p_tStorSet, const uint8_t p_uStorType,
                                      uint8_t* const p_puBuff, const uint32_t p_uBuffL)
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
        l_eResLL = eFSS_CORELL_InitCtx(&p_ptCtx->tCORELLCtx, p_tCtxCb, p_tStorSet, p_uStorType, p_puBuff, p_uBuffL);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

        if( e_eFSS_COREHL_RES_OK == l_eRes )
        {
            l_eResLL = eFSS_CORELL_GetBuff(&p_ptCtx->tCORELLCtx, &l_tBuff1, &l_tBuff2);
            l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

            if( e_eFSS_COREHL_RES_OK == l_eRes )
            {
                if( l_tBuff1.uBufL <= EFSS_COREHL_PAGEMIN_L )
                {
                    l_eRes = e_eFSS_COREHL_RES_BADPARAM;
                }
            }
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
    bool_t l_bIsInit;

    if( NULL == p_ptCtx )
    {
        l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
    }
    else
    {
		/* Check Init */
        l_bIsInit = false;
        l_eResLL = eFSS_CORELL_IsInit(&p_ptCtx->tCORELLCtx, &l_bIsInit);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

        if( e_eFSS_COREHL_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_COREHL_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_COREHL_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_COREHL_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResLL = eFSS_CORELL_GetStorSett(&p_ptCtx->tCORELLCtx, p_ptStorSet);
                    l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);
                }
            }
        }
    }

    return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_GetBuff(t_eFSS_COREHL_Ctx* const p_ptCtx, t_eFSS_COREHL_StorBuf* const p_ptBuff)
{
    /* Return local var */
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;
    bool_t l_bIsInit;

    /* Calc local variable */
    t_eFSS_CORELL_StorBuf l_tBuff1;
    t_eFSS_CORELL_StorBuf l_tBuff2;

    if( ( NULL == p_ptCtx ) || ( NULL == p_ptBuff ) )
    {
        l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
    }
    else
    {
		/* Check Init */
        l_bIsInit = false;
        l_eResLL = eFSS_CORELL_IsInit(&p_ptCtx->tCORELLCtx, &l_bIsInit);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

        if( e_eFSS_COREHL_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_COREHL_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_COREHL_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_COREHL_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResLL = eFSS_CORELL_GetBuff(&p_ptCtx->tCORELLCtx, &l_tBuff1, &l_tBuff2);
                    l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                    if( e_eFSS_COREHL_RES_OK == l_eRes )
                    {
                        p_ptBuff->puBuf = l_tBuff1.puBuf;
                        p_ptBuff->uBufL = l_tBuff1.uBufL - EFSS_COREHL_PAGEMIN_L;
                    }
                }
            }
        }
    }

    return l_eRes;
}
e_eFSS_COREHL_RES eFSS_COREHL_GetBuffNStor(t_eFSS_COREHL_Ctx* const p_ptCtx, t_eFSS_COREHL_StorBuf* const p_ptBuff,
                                           t_eFSS_TYPE_StorSet* const p_ptStorSet)
{
    /* Return local var */
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;
    bool_t l_bIsInit;

    /* Calc local variable */
    t_eFSS_CORELL_StorBuf l_tBuff1;
    t_eFSS_CORELL_StorBuf l_tBuff2;

    if( ( NULL == p_ptCtx ) || ( NULL == p_ptBuff ) )
    {
        l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
    }
    else
    {
		/* Check Init */
        l_bIsInit = false;
        l_eResLL = eFSS_CORELL_IsInit(&p_ptCtx->tCORELLCtx, &l_bIsInit);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

        if( e_eFSS_COREHL_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_COREHL_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_COREHL_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_COREHL_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResLL = eFSS_CORELL_GetBuffNStor(&p_ptCtx->tCORELLCtx, &l_tBuff1, &l_tBuff2, p_ptStorSet);
                    l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                    if( e_eFSS_COREHL_RES_OK == l_eRes )
                    {
                        p_ptBuff->puBuf = l_tBuff1.puBuf;
                        p_ptBuff->uBufL = l_tBuff1.uBufL - EFSS_COREHL_PAGEMIN_L;
                    }

                }
            }
        }
    }

    return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_LoadPageInBuff(t_eFSS_COREHL_Ctx* const p_ptCtx, const uint32_t p_uPIdx,
                                             uint8_t* const p_puSubTypeRead)
{
    /* Return local var */
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;
    bool_t l_bIsInit;

    /* Calc local variable */
    t_eFSS_CORELL_StorBuf l_tBuff1;
    t_eFSS_CORELL_StorBuf l_tBuff2;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puSubTypeRead ) )
    {
        l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
    }
    else
    {
		/* Check Init */
        l_bIsInit = false;
        l_eResLL = eFSS_CORELL_IsInit(&p_ptCtx->tCORELLCtx, &l_bIsInit);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

        if( e_eFSS_COREHL_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_COREHL_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_COREHL_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_COREHL_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResLL = eFSS_CORELL_GetBuff(&p_ptCtx->tCORELLCtx, &l_tBuff1, &l_tBuff2);
                    l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                    if( e_eFSS_COREHL_RES_OK == l_eRes )
                    {
                        l_eResLL = eFSS_CORELL_LoadPageInBuff(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_1, p_uPIdx);
                        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                        if( e_eFSS_COREHL_RES_OK == l_eRes )
                        {
                            *p_puSubTypeRead = l_tBuff1.puBuf[ l_tBuff1.uBufL - EFSS_COREHL_PAGEMIN_L ];
                        }
                    }
                }
            }
        }
    }

    return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_FlushBuffInPage(t_eFSS_COREHL_Ctx* const p_ptCtx, const uint32_t p_uPIdx,
                                              const uint8_t p_uSubTypeToWrite)
{
    /* Return local var */
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;
    bool_t l_bIsInit;

    /* Calc local variable */
    t_eFSS_CORELL_StorBuf l_tBuff1;
    t_eFSS_CORELL_StorBuf l_tBuff2;

    if( NULL == p_ptCtx )
    {
        l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
    }
    else
    {
		/* Check Init */
        l_bIsInit = false;
        l_eResLL = eFSS_CORELL_IsInit(&p_ptCtx->tCORELLCtx, &l_bIsInit);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

        if( e_eFSS_COREHL_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_COREHL_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_COREHL_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_COREHL_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResLL = eFSS_CORELL_GetBuff(&p_ptCtx->tCORELLCtx, &l_tBuff1, &l_tBuff2);
                    l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                    if( e_eFSS_COREHL_RES_OK == l_eRes )
                    {
                        /* Setup sub page type */
                        l_tBuff1.puBuf[ l_tBuff1.uBufL - EFSS_COREHL_PAGEMIN_L ] = p_uSubTypeToWrite;
                        (void)l_tBuff1.uBufL;

                        /* Flush */
                        l_eResLL = eFSS_CORELL_FlushBuffInPage(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_1, p_uPIdx);
                        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);
                    }
                }
            }
        }
    }

    return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_CalcCrcInBuff(t_eFSS_COREHL_Ctx* const p_ptCtx, uint32_t p_uCrcSeed, uint32_t p_uLenCalc,
                                            uint32_t* p_puCrc)
{
    /* Return local var */
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;
    bool_t l_bIsInit;

    /* Calc local variable */
    t_eFSS_CORELL_StorBuf l_tBuff1;
    t_eFSS_CORELL_StorBuf l_tBuff2;

    if( NULL == p_ptCtx )
    {
        l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
    }
    else
    {
		/* Check Init */
        l_bIsInit = false;
        l_eResLL = eFSS_CORELL_IsInit(&p_ptCtx->tCORELLCtx, &l_bIsInit);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

        if( e_eFSS_COREHL_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_COREHL_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_COREHL_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_COREHL_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResLL = eFSS_CORELL_GetBuff(&p_ptCtx->tCORELLCtx, &l_tBuff1, &l_tBuff2);
                    l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                    if( e_eFSS_COREHL_RES_OK == l_eRes )
                    {
                        if( p_uLenCalc > ( l_tBuff1.uBufL - EFSS_COREHL_PAGEMIN_L ) )
                        {
                            l_eRes = e_eFSS_COREHL_RES_BADPARAM;
                        }
                        else
                        {
                            l_eResLL = eFSS_CORELL_CalcCrcInBuff(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_1,
                                                                 p_uCrcSeed, p_uLenCalc, p_puCrc);
                            l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);
                        }
                    }
                }
            }
        }
    }

    return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_FlushBuffInPageNBkp(t_eFSS_COREHL_Ctx* const p_ptCtx,
								                  const uint32_t p_uOriIdx, const uint32_t p_uBkpIdx,
                                                  const uint8_t p_uOriSubT, const uint8_t p_uBkpSubT)
{
    /* Return local var */
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;
    bool_t l_bIsInit;

    /* Calc local variable */
    t_eFSS_CORELL_StorBuf l_tBuff1;
    t_eFSS_CORELL_StorBuf l_tBuff2;

    if( NULL == p_ptCtx )
    {
        l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
    }
    else
    {
		/* Check Init */
        l_bIsInit = false;
        l_eResLL = eFSS_CORELL_IsInit(&p_ptCtx->tCORELLCtx, &l_bIsInit);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

        if( e_eFSS_COREHL_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_COREHL_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_COREHL_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_COREHL_RES_CORRUPTCTX;
                }
                else
                {
                    if( p_uBkpIdx == p_uOriIdx )
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
                            l_tBuff1.puBuf[ l_tBuff1.uBufL - EFSS_COREHL_PAGEMIN_L ] = p_uOriSubT;

                            /* Flush */
                            l_eResLL = eFSS_CORELL_FlushBuffInPage(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_1, p_uOriIdx);
                            l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                            if( e_eFSS_COREHL_RES_OK == l_eRes)
                            {
                                /* Update subtype of the backup page */
                                l_tBuff1.puBuf[ l_tBuff1.uBufL - EFSS_COREHL_PAGEMIN_L ] = p_uBkpSubT;

                                /* Flush backup page */
                                l_eResLL = eFSS_CORELL_FlushBuffInPage(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_1,
                                                                       p_uBkpIdx);
                                l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                                if( e_eFSS_COREHL_RES_OK == l_eRes)
                                {
                                    /* Ripristinate original page */
                                    l_tBuff1.puBuf[ l_tBuff1.uBufL - EFSS_COREHL_PAGEMIN_L ] = p_uOriSubT;
                                    (void)l_tBuff1.uBufL;
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

e_eFSS_COREHL_RES eFSS_COREHL_LoadPageInBuffNRipBkp(t_eFSS_COREHL_Ctx* const p_ptCtx,
                                                    const uint32_t p_uOriIdx, const uint32_t p_uBkpIdx,
                                                    const uint8_t p_uOriSubT, const uint8_t p_uBkpSubT)
{
    /* Return local var */
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;
    bool_t l_bIsInit;

    /* Calc local variable */
    t_eFSS_CORELL_StorBuf l_tBuff1;
    t_eFSS_CORELL_StorBuf l_tBuff2;

    /* Local var used for calculation */
    bool_t l_bIsOriVal;
    bool_t l_bIsBkpVal;

    if( NULL == p_ptCtx )
    {
        l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
    }
    else
    {
		/* Check Init */
        l_bIsInit = false;
        l_eResLL = eFSS_CORELL_IsInit(&p_ptCtx->tCORELLCtx, &l_bIsInit);
        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

        if( e_eFSS_COREHL_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_COREHL_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_COREHL_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_COREHL_RES_CORRUPTCTX;
                }
                else
                {
                    if( p_uBkpIdx == p_uOriIdx )
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
                            l_eResLL = eFSS_CORELL_LoadPageInBuff(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_1, p_uOriIdx);
                            l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                            if( ( e_eFSS_COREHL_RES_OK == l_eRes ) || ( e_eFSS_COREHL_RES_NOTVALIDPAGE == l_eRes ) ||
                                ( e_eFSS_COREHL_RES_NEWVERSIONFOUND == l_eRes ) )
                            {
                                /* Page readed, is valid? */
                                if( ( e_eFSS_COREHL_RES_OK == l_eRes ) &&
                                    ( p_uOriSubT == l_tBuff1.puBuf[ l_tBuff1.uBufL - EFSS_COREHL_PAGEMIN_L ] ) )
                                {
                                    l_bIsOriVal = true;
                                }
                                else
                                {
                                    l_bIsOriVal = false;
                                }

                                /* Load backup page in internal buffer */
                                l_eResLL = eFSS_CORELL_LoadPageInBuff(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_2, p_uBkpIdx);
                                l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                                if( ( e_eFSS_COREHL_RES_OK == l_eRes ) || ( e_eFSS_COREHL_RES_NOTVALIDPAGE == l_eRes ) ||
                                    ( e_eFSS_COREHL_RES_NEWVERSIONFOUND == l_eRes ) )
                                {
                                    /* Page readed, is valid? */
                                    if( ( e_eFSS_COREHL_RES_OK == l_eRes ) &&
                                        ( p_uBkpSubT == l_tBuff2.puBuf[ l_tBuff2.uBufL - EFSS_COREHL_PAGEMIN_L ] ) )
                                    {
                                        l_bIsBkpVal = true;
                                    }
                                    else
                                    {
                                        l_bIsBkpVal = false;
                                    }

                                    /* We have all the data needed to make a decision */
                                    if( ( true == l_bIsOriVal ) && ( true == l_bIsBkpVal ) )
                                    {
                                        /* Both page are valid, are they identical (except the subtype of course )? */
                                        if( 0 == memcmp(l_tBuff1.puBuf, l_tBuff2.puBuf, ( l_tBuff2.uBufL - EFSS_COREHL_PAGEMIN_L ) ) )
                                        {
                                            /* Page are equals */
                                            l_eRes = e_eFSS_COREHL_RES_OK;
                                        }
                                        else
                                        {
                                            /* Page are not equals, copy origin in backup */
                                            l_tBuff1.puBuf[ l_tBuff1.uBufL - EFSS_COREHL_PAGEMIN_L ] = p_uBkpSubT;
                                            l_eResLL = eFSS_CORELL_FlushBuffInPage(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_1,
                                                                                   p_uBkpIdx);
                                            l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                                            /* Ripristinate original one */
                                            l_tBuff1.puBuf[ l_tBuff1.uBufL - EFSS_COREHL_PAGEMIN_L ] = p_uOriSubT;
                                            (void)l_tBuff1.uBufL;

                                            /* If recovered return it */
                                            if( e_eFSS_COREHL_RES_OK == l_eRes )
                                            {
                                                l_eRes = e_eFSS_COREHL_RES_OK_BKP_RCVRD;
                                            }
                                        }
                                    }
                                    else if( ( false == l_bIsOriVal ) && ( true == l_bIsBkpVal ) )
                                    {
                                        /* Original page is not valid, ripristinate it from the backup one */
                                        l_tBuff2.puBuf[ l_tBuff2.uBufL - EFSS_COREHL_PAGEMIN_L ] = p_uOriSubT;

                                        l_eResLL = eFSS_CORELL_FlushBuffInPage(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_2,
                                                                               p_uOriIdx);
                                        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                                        /* Ripristinate backup one */
                                        l_tBuff2.puBuf[ l_tBuff2.uBufL - EFSS_COREHL_PAGEMIN_L ] = p_uBkpSubT;

                                        /* If recovered return it */
                                        if( e_eFSS_COREHL_RES_OK == l_eRes )
                                        {
                                            /* Buffer 1 must contains the original page */
                                            (void)memcpy(l_tBuff1.puBuf, l_tBuff2.puBuf, l_tBuff2.uBufL);
                                            l_tBuff1.puBuf[ l_tBuff1.uBufL - EFSS_COREHL_PAGEMIN_L ] = p_uOriSubT;
                                            (void)l_tBuff1.uBufL;

                                            l_eRes = e_eFSS_COREHL_RES_OK_BKP_RCVRD;
                                        }
                                    }
                                    else if( ( true == l_bIsOriVal ) && ( false == l_bIsBkpVal ) )
                                    {
                                        /* Backup is not valid, ripristinate it from the origin one */
                                        l_tBuff1.puBuf[ l_tBuff1.uBufL - EFSS_COREHL_PAGEMIN_L ] = p_uBkpSubT;

                                        l_eResLL = eFSS_CORELL_FlushBuffInPage(&p_ptCtx->tCORELLCtx, e_eFSS_CORELL_BUFFTYPE_1,
                                                                               p_uBkpIdx);
                                        l_eRes = eFSS_COREHL_LLtoHLRes(l_eResLL);

                                        /* Ripristinate original one */
                                        l_tBuff1.puBuf[ l_tBuff1.uBufL - EFSS_COREHL_PAGEMIN_L ] = p_uOriSubT;
                                        (void)l_tBuff1.uBufL;

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

static bool_t eFSS_COREHL_IsStatusStillCoherent(t_eFSS_COREHL_Ctx* const p_ptCtx)
{
    /* Return local var */
    bool_t l_bRes;
    e_eFSS_CORELL_RES l_eResLL;

    /* Calc local variable */
    t_eFSS_CORELL_StorBuf l_tBuff1;
    t_eFSS_CORELL_StorBuf l_tBuff2;

    l_eResLL = eFSS_CORELL_GetBuff(&p_ptCtx->tCORELLCtx, &l_tBuff1, &l_tBuff2);

    if( e_eFSS_CORELL_RES_OK != l_eResLL )
    {
        l_bRes = false;
    }
    else
    {
        if( l_tBuff1.uBufL <= EFSS_COREHL_PAGEMIN_L )
        {
            l_bRes = false;
        }
        else
        {
            l_bRes = true;
        }
    }

    return l_bRes;
}