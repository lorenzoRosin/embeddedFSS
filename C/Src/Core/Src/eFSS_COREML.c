/**
 * @file       eFSS_CORELL.h
 *
 * @brief      Low level core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_COREML.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static e_eFSS_COREML_RES eFSS_COREML_LLtoMLRes(const e_eFSS_CORELL_RES p_eLLRes);


/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_COREML_RES eFSS_COREML_InitCtx(t_eFSS_COREML_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx const p_tCtxCb,
									  t_eFSS_CORELL_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL)
{
    e_eFSS_COREML_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_COREML_RES_BADPOINTER;
	}
	else
	{
        /* Init LL context */
        l_eResLL = eFSS_CORELL_InitCtx(p_ptCtx->tCORELLCtx, p_tCtxCb, p_tStorSet, p_puBuff, p_uBuffL);
        l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);
    }

    return l_eRes;
}

e_eFSS_COREML_RES eFSS_COREML_IsInit(t_eFSS_COREML_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eFSS_CORELL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSS_COREML_RES_BADPOINTER;
	}
	else
	{
        l_eResLL = eFSS_CORELL_IsInit(p_ptCtx->tCORELLCtx, p_pbIsInit);
        l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);
	}

	return l_eRes;
}

e_eFSS_CORELL_RES eFSS_CORELL_GetBuff(t_eFSS_CORELL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_tBuffType,
								      uint8_t** p_ppuBuff, uint32_t* p_puBuffL)
{
	/* Local variable */
	e_eFSS_CORELL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ppuBuff ) || ( NULL == p_puBuffL ) )
	{
		l_eRes = e_eFSS_COREML_RES_BADPOINTER;
	}
	else
	{
        l_eResLL = eFSS_CORELL_GetBuff(&p_ptCtx->tCORELLCtx, p_tBuffType, p_ppuBuff, p_puBuffL);
        l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);
	}

	return l_eRes;
}

e_eFSS_COREML_RES eFSS_COREML_LoadPageInBuffNChkCrc(t_eFSS_COREML_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_tBuffType,
								                    const uint32_t p_uPageIndx)
{
	/* Local variable */
	e_eFSS_CORELL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;
    uint32_t l_uTryPerformed;
    bool_t l_bCbRes;

	uint8_t* l_puBuff;
	uint32_t l_uBuffL;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_COREML_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResLL = eFSS_CORELL_IsInit(p_ptCtx->tCORELLCtx, p_pbIsInit);
        l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);

		if( ( e_eFSS_COREML_RES_OK == l_eRes ) || ( true == p_pbIsInit ) )
		{
			l_eRes = e_eFSS_COREML_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eFSS_CORELL_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
            }
            else
            {
                /* Check param validity */
                if( p_uPageIndx >= p_ptCtx->tStorSett.uTotPages )
                {
                    l_eRes = e_eFSS_CORELL_RES_BADPARAM;
                }
                else
                {
                    switch(p_tBuffType)
                    {
                        case e_eFSS_CORELL_BUFTYPE_1:
                        {
                            l_puBuff = p_ptCtx->puBuf1;
                            l_uBuffL = p_ptCtx->uBuf1L;
                            l_eRes = e_eFSS_CORELL_RES_OK;
                            break;
                        }

                        case e_eFSS_CORELL_BUFTYPE_2:
                        {
                            l_puBuff = p_ptCtx->puBuf2;
                            l_uBuffL = p_ptCtx->uBuf2L;
                            l_eRes = e_eFSS_CORELL_RES_OK;
                            break;
                        }

                        default:
                        {
                            l_eRes = e_eFSS_CORELL_RES_BADPARAM;
                            break;
                        }
                    }

                    /* Check validity */
                    if( e_eFSS_CORELL_RES_OK == l_eRes )
                    {
                        /* Init var */
                        l_eRes = e_eFSS_CORELL_RES_CLBCKREADERR;
                        l_uTryPerformed = 0u;
                        l_bCbRes = false;

                        while( ( false == l_bCbRes ) && ( l_uTryPerformed < p_uReTry ) )
                        {
                            /* Read */
                            l_bCbRes = (*(p_ptCbCtx->fRead))(p_ptCbCtx->ptCtxRead, p_uPageIndx, l_puBuff, l_uBuffL);
                            l_uTryPerformed++;

                            if( false == l_bCbRes )
                            {
                                l_eRes = e_eFSS_CORELL_RES_CLBCKREADERR;
                            }
                            else
                            {
                                l_eRes = e_eFSS_CORELL_RES_OK;
                            }
                        }
                    }
                }
            }
		}
	}

	return l_eRes;
}

e_eFSS_COREML_RES eFSS_COREML_FlushBuffWUpdCrcInPage(t_eFSS_COREML_Ctx* const p_ptCtx,
                                                     e_eFSS_CORELL_BUFTYPE p_tBuffType, const uint32_t p_uPageIndx)
{
	/* Local variable */
	e_eFSS_CORELL_RES l_eRes;
    uint32_t l_uTryPerformed;
    bool_t l_bCbRes;

	uint8_t* l_puBuff;
	uint32_t l_uBuffL;
	uint8_t* l_puBuffS;
	uint32_t l_uBuffSL;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_CORELL_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eFSS_CORELL_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eFSS_CORELL_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
            }
            else
            {
                /* Check param validity */
                if( p_uPageIndx >= p_ptCtx->tStorSett.uTotPages )
                {
                    l_eRes = e_eFSS_CORELL_RES_BADPARAM;
                }
                else
                {
                    switch(p_tBuffType)
                    {
                        case e_eFSS_CORELL_BUFTYPE_1:
                        {
                            l_puBuff = p_ptCtx->puBuf1;
                            l_uBuffL = p_ptCtx->uBuf1L;
                            l_puBuffS = p_ptCtx->puBuf2;
                            l_uBuffSL = p_ptCtx->uBuf2L;
                            l_eRes = e_eFSS_CORELL_RES_OK;
                            break;
                        }

                        case e_eFSS_CORELL_BUFTYPE_2:
                        {
                            l_puBuff = p_ptCtx->puBuf2;
                            l_uBuffL = p_ptCtx->uBuf2L;
                            l_puBuffS = p_ptCtx->puBuf1;
                            l_uBuffSL = p_ptCtx->uBuf1L;
                            l_eRes = e_eFSS_CORELL_RES_OK;
                            break;
                        }

                        default:
                        {
                            l_eRes = e_eFSS_CORELL_RES_BADPARAM;
                            break;
                        }
                    }

                    /* Check validity */
                    if( e_eFSS_CORELL_RES_OK == l_eRes )
                    {
                        /* Init var */
                        l_eRes = e_eFSS_CORELL_RES_CLBCKWRITEERR;
                        l_uTryPerformed = 0u;
                        l_bCbRes = false;

                        while( ( false == l_bCbRes ) && ( l_uTryPerformed < p_uReTry ) )
                        {
                            /* Erase */
                            l_bCbRes = (*(p_ptCbCtx->fErase))(p_ptCbCtx->ptCtxErase, p_uPageIndx);

                            if( true == l_bCbRes )
                            {
                                /* Write */
                                l_bCbRes = (*(p_ptCbCtx->fWrite))(p_ptCbCtx->ptCtxWrite, p_uPageIndx, l_puBuff, l_uBuffL);

                                if( false == l_bCbRes )
                                {
                                    /* Write error  */
                                    l_eRes = e_eFSS_CORELL_RES_CLBCKWRITEERR;
                                }
                            }
                            else
                            {
                                l_eRes = e_eFSS_CORELL_RES_CLBCKERASEERR;
                            }

                            if( true == l_bCbRes )
                            {
                                /* Read */
                                l_bCbRes = (*(p_ptCbCtx->fRead))(p_ptCbCtx->ptCtxRead, p_uPageIndx, l_puBuffS, l_uBuffSL);

                                if( false == l_bCbRes )
                                {
                                    /* Write error  */
                                    l_eRes = e_eFSS_CORELL_RES_CLBCKREADERR;
                                }
                            }

                            if( true == l_bCbRes )
                            {
                                /* Compare */
                                if( 0 == memcmp(l_puBuff, l_puBuffS, l_uBuffL) )
                                {
                                    l_bCbRes = true;
                                }
                                else
                                {
                                    l_bCbRes = false;
                                    l_eRes = e_eFSS_CORELL_RES_WRITENOMATCHREAD;
                                }
                            }

                            l_uTryPerformed++;
                        }

                        if( true == l_bCbRes )
                        {
                            l_eRes = e_eFSS_CORELL_RES_OK;
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
