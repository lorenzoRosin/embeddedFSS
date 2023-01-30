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
#include "eFSS_CORELL.h"


/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_CORELL_IsStatusStillCoherent(const t_eFSS_CORELL_Ctx* p_ptCtx);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_CORELL_RES eFSS_CORELL_InitCtx(t_eFSS_CORELL_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx const p_tCtxCb,
									  t_eFSS_CORELL_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL)
{
    /* Return local var */
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
                    if( p_tStorSet.uPagesLen <= 8u )
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
                            p_ptCtx->uBuf1L 	= p_uBuffL / 2u;
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
    /* Return local var */
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
	/* Return local var */
	e_eFSS_CORELL_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puPageL ) || ( NULL == p_puNPage ) )
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
                /* Copy data */
                *p_puPageL = p_ptCtx->tStorSett.uPagesLen;
                *p_puNPage = p_ptCtx->tStorSett.uTotPages;
                l_eRes = e_eFSS_CORELL_RES_OK;
            }
		}
	}

	return l_eRes;
}

e_eFSS_CORELL_RES eFSS_CORELL_GetBuff(t_eFSS_CORELL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_eBuffType,
								      uint8_t** p_ppuBuff, uint32_t* p_puBuffL)
{
	/* Return local var */
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
				switch(p_eBuffType)
				{
					case e_eFSS_CORELL_BUFTYPE_1:
					{
						*p_ppuBuff = p_ptCtx->puBuf1;
						*p_puBuffL = p_ptCtx->uBuf1L;
						l_eRes = e_eFSS_CORELL_RES_OK;
						break;
					}

					case e_eFSS_CORELL_BUFTYPE_2:
					{
						*p_ppuBuff = p_ptCtx->puBuf2;
						*p_puBuffL = p_ptCtx->uBuf2L;
						l_eRes = e_eFSS_CORELL_RES_OK;
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

e_eFSS_CORELL_RES eFSS_CORELL_LoadPageInBuff(t_eFSS_CORELL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_eBuffType,
								             const uint32_t p_uPageIndx)
{
	/* Return local var */
	e_eFSS_CORELL_RES l_eRes;

    /* Local var used for calculation */
    uint32_t l_uTryPerformed;
    bool_t l_bCbRes;
	uint8_t* l_puBuff;
	uint32_t l_uBuffL;
    uint32_t l_uMagicNumber;
    uint32_t l_uPageCrc;
    uint32_t l_uPageCrcCalc;
    uint32_t l_uComulIndx;
    uint32_t l_uTemp;

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
                    switch(p_eBuffType)
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

                        while( ( false == l_bCbRes ) && ( l_uTryPerformed < p_ptCtx->tStorSett.uRWERetry ) )
                        {
                            /* Read */
                            l_bCbRes = (*(p_ptCtx->tCtxCb.fRead))(p_ptCtx->tCtxCb.ptCtxRead, p_uPageIndx, l_puBuff, l_uBuffL);
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

                        if( e_eFSS_CORELL_RES_OK == l_eRes )
                        {
                            /* Can check validity CRC and Magic number */
                            /* Initialize internal status */
                            l_uMagicNumber = 0u;
                            l_uPageCrc = 0u;
                            l_uComulIndx = l_uBuffLLL - 8u;

                            /* --- Copy data Little endian -- MAGIC NUMBER */
                            l_uTemp = (uint32_t) l_puBuffLL[l_uComulIndx];
                            l_uMagicNumber |= ( l_uTemp & 0x000000FFu );
                            l_uComulIndx++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) l_puBuffLL[l_uComulIndx] ) << 8u  );
                            l_uMagicNumber |= ( l_uTemp & 0x0000FF00u );
                            l_uComulIndx++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) l_puBuffLL[l_uComulIndx] ) << 16u  );
                            l_uMagicNumber |= ( l_uTemp & 0x00FF0000u );
                            l_uComulIndx++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) l_puBuffLL[l_uComulIndx] ) << 24u  );
                            l_uMagicNumber |= ( l_uTemp & 0xFF000000u );
                            l_uComulIndx++;

                            /* --- Copy data Little endian -- CRC */
                            l_uTemp = (uint32_t) l_puBuffLL[l_uComulIndx];
                            l_uPageCrc |= ( l_uTemp & 0x000000FFu );
                            l_uComulIndx++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) l_puBuffLL[l_uComulIndx] ) << 8u  );
                            l_uPageCrc |= ( l_uTemp & 0x0000FF00u );
                            l_uComulIndx++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) l_puBuffLL[l_uComulIndx] ) << 16u  );
                            l_uPageCrc |= ( l_uTemp & 0x00FF0000u );
                            l_uComulIndx++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) l_puBuffLL[l_uComulIndx] ) << 24u  );
                            l_uPageCrc |= ( l_uTemp & 0xFF000000u );
                            l_uComulIndx++;

                            /* Calculate CRC */
                            l_eResLL = eFSS_CORELL_CalcCrcInBuff(&p_ptCtx->tCORELLCtx, p_eBuffType, MAX_UINT32VAL,
                                                                 l_uBuffLLL - 4u, &l_uPageCrcCalc);
                            l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);

                            if( e_eFSS_COREML_RES_OK == l_eRes )
                            {
                                if( l_uPageCrcCalc != l_uPageCrc )
                                {
                                    l_eRes = e_eFSS_COREML_RES_NOTVALIDPAGE;
                                }
                                else
                                {
                                    if( ( EFSS_PAGEMAGICNUMBER != l_uMagicNumber ) ||
                                        ( p_ptCtx->tStorSett.uPageType != l_uPageType ) )
                                    {
                                        l_eRes = e_eFSS_COREML_RES_NOTVALIDPAGE;
                                    }
                                    else
                                    {
                                        if( p_ptCtx->tStorSett.uPageVersion != l_uPageVersion )
                                        {
                                            l_eRes = e_eFSS_COREML_RES_NEWVERSIONFOUND;
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

e_eFSS_CORELL_RES eFSS_CORELL_FlushBuffInPage(t_eFSS_CORELL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_eBuffType,
								              const uint32_t p_uPageIndx)
{
	/* Return local var */
	e_eFSS_CORELL_RES l_eRes;

    /* Local var used for calculation */
    uint32_t l_uTryPerformed;
    bool_t l_bCbRes;
	uint8_t* l_puBuff;
	uint32_t l_uBuffL;
	uint8_t* l_puBuffS;
	uint32_t l_uBuffSL;
    uint32_t l_uComulIndx;
    uint32_t l_uMagicNumber;
    uint32_t l_uPageCrcCalc;

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
                    switch(p_eBuffType)
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
                            l_puBuff = NULL;
                            l_uBuffL = 0u;
                            l_eRes = e_eFSS_CORELL_RES_BADPARAM;
                            break;
                        }
                    }

                    /* Check validity */
                    if( e_eFSS_CORELL_RES_OK == l_eRes )
                    {
                        /* Calc and insert magic number and CRC */
                        /* Initialize internal status */
                        l_uPageType = p_ptCtx->tStorSett.uPageType;
                        l_uPageVersion = p_ptCtx->tStorSett.uPageVersion;
                        l_uMagicNumber = EFSS_PAGEMAGICNUMBER;
                        l_uComulIndx = l_uBuffLLL - 11u;

                        /* --- Copy data Little endian -- MAGIC NUMBER */
                        l_puBuffLL[l_uComulIndx] = (uint8_t) ( ( l_uMagicNumber        ) & 0x000000FFu );
                        l_uComulIndx++;

                        l_puBuffLL[l_uComulIndx] = (uint8_t) ( ( l_uMagicNumber >> 8u  ) & 0x000000FFu );
                        l_uComulIndx++;

                        l_puBuffLL[l_uComulIndx] = (uint8_t) ( ( l_uMagicNumber >> 16u ) & 0x000000FFu );
                        l_uComulIndx++;

                        l_puBuffLL[l_uComulIndx] = (uint8_t) ( ( l_uMagicNumber >> 24u ) & 0x000000FFu );
                        l_uComulIndx++;

                        /* Calculate CRC */
                        l_eResLL = eFSS_CORELL_CalcCrcInBuff(&p_ptCtx->tCORELLCtx, p_eBuffType, MAX_UINT32VAL,
                                                             l_uBuffLLL - 4u, &l_uPageCrcCalc);
                        l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);
                        if( e_eFSS_COREML_RES_OK == l_eRes )
                        {
                            /* --- Copy data Little endian -- CRC */
                            l_puBuffLL[l_uComulIndx] = (uint8_t) ( ( l_uPageCrcCalc        ) & 0x000000FFu );
                            l_uComulIndx++;

                            l_puBuffLL[l_uComulIndx] = (uint8_t) ( ( l_uPageCrcCalc >> 8u  ) & 0x000000FFu );
                            l_uComulIndx++;

                            l_puBuffLL[l_uComulIndx] = (uint8_t) ( ( l_uPageCrcCalc >> 16u ) & 0x000000FFu );
                            l_uComulIndx++;

                            l_puBuffLL[l_uComulIndx] = (uint8_t) ( ( l_uPageCrcCalc >> 24u ) & 0x000000FFu );
                            l_uComulIndx++;

                            /* Flush buffer */
                            l_eResLL = eFSS_CORELL_FlushBuffInPage(&p_ptCtx->tCORELLCtx, p_eBuffType, p_uPageIndx);
                            l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);
                        }

                        if( e_eFSS_COREML_RES_OK == l_eRes)
                        {
                            /* Init var */
                            l_eRes = e_eFSS_CORELL_RES_CLBCKWRITEERR;
                            l_uTryPerformed = 0u;
                            l_bCbRes = false;

                            while( ( false == l_bCbRes ) && ( l_uTryPerformed < p_ptCtx->tStorSett.uRWERetry ) )
                            {
                                /* Erase */
                                l_bCbRes = (*(p_ptCtx->tCtxCb.fErase))(p_ptCtx->tCtxCb.ptCtxErase, p_uPageIndx);

                                if( true == l_bCbRes )
                                {
                                    /* Write */
                                    l_bCbRes = (*(p_ptCtx->tCtxCb.fWrite))(p_ptCtx->tCtxCb.ptCtxWrite, p_uPageIndx, l_puBuff, l_uBuffL);

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
                                    l_bCbRes = (*(p_ptCtx->tCtxCb.fRead))(p_ptCtx->tCtxCb.ptCtxRead, p_uPageIndx, l_puBuffS, l_uBuffSL);

                                    if( false == l_bCbRes )
                                    {
                                        /* Write error  */
                                        l_eRes = e_eFSS_CORELL_RES_CLBCKREADERR;
                                    }
                                }

                                if( true == l_bCbRes )
                                {
                                    /* Compare buffer to write with the readed one */
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
	}

	return l_eRes;
}

e_eFSS_CORELL_RES eFSS_CORELL_CalcCrcInBuff(t_eFSS_CORELL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_eBuffType,
								            uint32_t p_uCrcSeed, uint32_t p_uLenCalc, uint32_t* p_puCrc)
{
	/* Return local var */
	e_eFSS_CORELL_RES l_eRes;

    /* Local var used for calculation */
    bool_t l_bCbRes;
	uint8_t* l_puBuff;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puCrc ) )
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
                if( ( p_uLenCalc <= 0u ) || ( p_uLenCalc > p_ptCtx->tStorSett.uPagesLen ) )
                {
                    l_eRes = e_eFSS_CORELL_RES_BADPARAM;
                }
                else
                {
                    switch(p_eBuffType)
                    {
                        case e_eFSS_CORELL_BUFTYPE_1:
                        {
                            l_puBuff = p_ptCtx->puBuf1;
                            l_eRes = e_eFSS_CORELL_RES_OK;
                            break;
                        }

                        case e_eFSS_CORELL_BUFTYPE_2:
                        {
                            l_puBuff = p_ptCtx->puBuf2;
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
                        /* Calc */
                        l_bCbRes = (*(p_ptCtx->tCtxCb.fCrc32))(p_ptCtx->tCtxCb.ptCtxCrc32, p_uCrcSeed, l_puBuff, p_uLenCalc,
                                                          p_puCrc );

                        if( true != l_bCbRes )
                        {
                            l_eRes = e_eFSS_CORELL_RES_CLBCKCRCERR;
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
static bool_t eFSS_CORELL_IsStatusStillCoherent(const t_eFSS_CORELL_Ctx* p_ptCtx)
{
    bool_t l_eRes;

	/* Check context validity */
    if( ( NULL == p_ptCtx->tCtxCb.ptCtxErase ) || ( NULL == p_ptCtx->tCtxCb.fErase ) ||
        ( NULL == p_ptCtx->tCtxCb.ptCtxWrite ) || ( NULL == p_ptCtx->tCtxCb.fWrite ) ||
        ( NULL == p_ptCtx->tCtxCb.ptCtxRead  ) || ( NULL == p_ptCtx->tCtxCb.fRead  ) ||
        ( NULL == p_ptCtx->tCtxCb.ptCtxCrc32 ) || ( NULL == p_ptCtx->tCtxCb.fCrc32 ) ||
        ( NULL == p_ptCtx->puBuf1 ) || ( NULL == p_ptCtx->puBuf2 ) )
    {
        l_eRes = false;
    }
    else
    {
        /* Check data validity */
        if( p_ptCtx->tStorSett.uTotPages <= 0u )
        {
            l_eRes = false;
        }
        else
        {
            /* Check data validity */
            if( p_ptCtx->tStorSett.uPagesLen <= 8u )
            {
                l_eRes = false;
            }
            else
            {
                /* Check data validity */
                if( p_ptCtx->tStorSett.uRWERetry <= 0u )
                {
                        l_eRes = false;
                }
                else
                {
                    /* Check data validity */
                    if( p_ptCtx->uBuf2L != p_ptCtx->uBuf1L )
                    {
                        l_eRes = false;
                    }
                    else
                    {
                        /* Check data validity */
                        if( p_ptCtx->tStorSett.uPagesLen != p_ptCtx->uBuf1L )
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
    }

    return l_eRes;
}
