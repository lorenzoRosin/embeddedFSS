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
#include "eFSS_LOGC.h"
#include "eFSS_LOGCPRV.h"
#include "eFSS_UTILSHLPRV.h"


/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_CORELL_IsStatusStillCoherent(const t_eFSS_LOGC_Ctx* p_ptCtx);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_CORELL_RES eFSS_CORELL_InitCtx(t_eFSS_CORELL_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx const p_tCtxCb,
									  t_eFSS_CORELL_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL)
{
    e_eFSS_CORELL_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_CORELL_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_tCtxCb.ptCtxErase ) || ( NULL == p_tCtxCb.fErase ) ||
            ( NULL == p_tCtxCb.ptCtxWrite ) || ( NULL == p_tCtxCb.fWrite ) ||
            ( NULL == p_tCtxCb.ptCtxRead  ) || ( NULL == p_tCtxCb.fRead  ) ||
            ( NULL == p_tCtxCb.ptCtxCrc32 ) || ( NULL == p_tCtxCb.fCrc32 ) )
        {
            l_eRes = e_eFSS_CORELL_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( p_uBuffL != ( 2u * p_tStorSet.uPagesLen ) )
            {
                l_eRes = e_eFSS_CORELL_RES_BADPARAM;
            }
            else
            {
                /* Check data validity */
                if( p_tStorSet.uTotPages <= 0u )
                {
                    l_eRes = e_eFSS_CORELL_RES_BADPARAM;
                }
                else
                {
                    /* Check data validity */
                    if( ( p_tStorSet.uPagesLen <= EFSS_PAGEMETASIZE ) || ( 0u != ( p_tStorSet.uPagesLen % 2u ) ) )
                    {
                        l_eRes = e_eFSS_CORELL_RES_BADPARAM;
                    }
                    else
                    {
                        /* Check data validity */
                        if( p_tStorSet.uRWERetry <= 0u )
                        {
                            l_eRes = e_eFSS_CORELL_RES_BADPARAM;
                        }
                        else
                        {
                            /* Fill context */
                            p_ptCtx->bIsInit 	= true;
                            p_ptCtx->tCtxCb 	= p_tCtxCb;
                            p_ptCtx->tStorSett 	= p_tStorSet;
                            p_ptCtx->puBuf1 	= p_puBuff;
                            p_ptCtx->uBuf1L 	= p_uBuffL \ 2u;
                            p_ptCtx->puBuf2 	= &p_puBuff[p_ptCtx->uBuf1L];
                            p_ptCtx->uBuf2L 	= p_ptCtx->uBuf1L;
                        }
                    }
                }
            }
        }
    }

    return l_eRes;
}

e_eFSS_CORELL_RES eFSS_CORELL_IsInit(t_eFSS_CORELL_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eFSS_CORELL_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSS_CORELL_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInit;
        l_eRes = e_eFSS_CORELL_RES_OK;
	}

	return l_eRes;
}


e_eFSS_CORELL_RES eFSS_CORELL_GetStorSett(t_eFSS_CORELL_Ctx* const p_ptCtx, uint32_t* p_puPageL, uint32_t* p_puNPage)
{
	/* Local variable */
	e_eFSS_CORELL_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ppuBuff ) || ( NULL == p_puBuffL ) )
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
				switch(p_tBuffType)
				{
					case e_eFSS_CORELL_BUFTYPE_1:
					{
						*p_ppuBuff = p_ptCtx->puBuf1;
						*p_puBuffL = p_ptCtx->uBuf1L;
						l_eRes = e_eFSS_LOGC_RES_OK;
						break;
					}

					case e_eFSS_CORELL_BUFTYPE_2:
					{
						*p_ppuBuff = p_ptCtx->puBuf2;
						*p_puBuffL = p_ptCtx->uBuf2L;
						l_eRes = e_eFSS_LOGC_RES_OK;
						break;
					}

					default:
					{
						l_eRes = e_eFSS_CORELL_RES_BADPARAM;
						break;
					}
				}
            }
		}
	}

	return l_eRes;   
}

e_eFSS_CORELL_RES eFSS_CORELL_GetBuff(t_eFSS_CORELL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_tBuffType,
								      uint8_t** p_ppuBuff, uint32_t* p_puBuffL)
{
	/* Local variable */
	e_eFSS_CORELL_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ppuBuff ) || ( NULL == p_puBuffL ) )
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
				switch(p_tBuffType)
				{
					case e_eFSS_CORELL_BUFTYPE_1:
					{
						*p_ppuBuff = p_ptCtx->puBuf1;
						*p_puBuffL = p_ptCtx->uBuf1L;
						l_eRes = e_eFSS_LOGC_RES_OK;
						break;
					}

					case e_eFSS_CORELL_BUFTYPE_2:
					{
						*p_ppuBuff = p_ptCtx->puBuf2;
						*p_puBuffL = p_ptCtx->uBuf2L;
						l_eRes = e_eFSS_LOGC_RES_OK;
						break;
					}

					default:
					{
						l_eRes = e_eFSS_CORELL_RES_BADPARAM;
						break;
					}
				}
            }
		}
	}

	return l_eRes;
}

e_eFSS_CORELL_RES eFSS_CORELL_LoadPageInBuff(t_eFSS_CORELL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_tBuffType,
								             const uint32_t p_uPageIndx)
{
	/* Local variable */
	e_eFSS_CORELL_RES l_eRes;
    uint32_t l_uTryPerformed;
    bool_t l_bCbRes;

	uint8_t* l_puBuff;
	uint32_t l_uBuffL;

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

e_eFSS_CORELL_RES eFSS_CORELL_FlushBuffInPage(t_eFSS_CORELL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_tBuffType,
								              const uint32_t p_uPageIndx)
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

e_eFSS_CORELL_RES eFSS_CalcCrcInBuff(t_eFSS_CORELL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_tBuffType,
								     uint32_t p_uCrcSeed, uint32_t p_uLenCalc, uint32_t* p_puCrc)
{

}


/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eFSS_CORELL_IsStatusStillCoherent(const t_eFSS_BLOBC_Ctx* p_ptCtx)
{
    bool_t l_eRes;

	/* Check context validity */
    if( ( NULL == p_ptCtx->tCtxCb.ptCtxErase ) || ( NULL == p_ptCtx->tCtxCb.fErase ) ||
        ( NULL == p_ptCtx->tCtxCb.ptCtxWrite ) || ( NULL == p_ptCtx->tCtxCb.fWrite ) ||
        ( NULL == p_ptCtx->tCtxCb.ptCtxRead  ) || ( NULL == p_ptCtx->tCtxCb.fRead  ) ||
        ( NULL == p_ptCtx->tCtxCb.ptCtxCrc32 ) || ( NULL == p_ptCtx->tCtxCb.fCrc32 ) ||
        ( NULL == p_ptCtx->puBuf ) )
    {
        l_eRes = false;
    }
    else
    {
        /* Check data validity */
        if( ( p_ptCtx->tStorSett.uTotPages < 2u ) || ( 0u != ( p_ptCtx->tStorSett.uTotPages % 2u ) ) )
        {
            l_eRes = false;
        }
        else
        {
            /* Check data validity */
            if( ( p_ptCtx->tStorSett.uPagesLen <= EFSS_PAGEMETASIZE ) ||
                ( 0u != ( p_ptCtx->tStorSett.uPagesLen % 2u ) ) )
            {
                l_eRes = false;
            }
            else
            {
                if( p_ptCtx->tStorSett.uRWERetry <= 0u )
                {
                        l_eRes = false;
                }
                else
                {
                    /* Check data validity */
                    if( ( 2u * p_ptCtx->tStorSett.uPagesLen ) != p_ptCtx->uBufL )
                    {
                        l_eRes = false;
                    }
                    else
                    {
                        l_eRes = true;
                    }
                }
            }
        }
    }

    return l_eRes;
}
