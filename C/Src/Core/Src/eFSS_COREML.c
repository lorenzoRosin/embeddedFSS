/**
 * @file       eFSS_COREML.h
 *
 * @brief      Medium level core module
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
static bool_t eFSS_COREML_IsStatusStillCoherent(const t_eFSS_COREML_Ctx* p_ptCtx);
static e_eFSS_COREML_RES eFSS_COREML_LLtoMLRes(const e_eFSS_CORELL_RES p_eLLRes);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_COREML_RES eFSS_COREML_InitCtx(t_eFSS_COREML_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx const p_tCtxCb,
									  t_eFSS_CORELL_StorSet p_tStorSetLL, t_eFSS_COREML_StorSet p_tStorSetML,
                                      uint8_t* const p_puBuff, uint32_t p_uBuffL)
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
        /* Check data validity */
        if( p_tStorSet.uPagesLen <= 11u )
        {
            l_eRes = e_eFSS_COREML_RES_BADPARAM;
        }
        else
        {
            /* Init LL context */
            l_eResLL = eFSS_CORELL_InitCtx(p_ptCtx->tCORELLCtx, p_tCtxCb, p_tStorSet, p_puBuff, p_uBuffL);
            l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);

            if( e_eFSS_COREML_RES_OK == l_eRes )
            {
                p_ptCtx->tStorSett = p_tStorSetML;
            }
        }
    }

    return l_eRes;
}

e_eFSS_COREML_RES eFSS_COREML_IsInit(t_eFSS_COREML_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Return local var */
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

e_eFSS_COREML_RES eFSS_COREML_GetStorSett(t_eFSS_COREML_Ctx* const p_ptCtx, uint32_t* p_puDataL, uint32_t* p_puNPage,
                                          uint8_t* p_puPageType, uint16_t* p_puPageVersion)
{
	/* Return local var */
	e_eFSS_CORELL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    uint32_t l_uDataLLL;
    uint32_t l_uNPageLL;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puDataL ) || ( NULL == p_puNPage ) || ( NULL == p_puPageType ) || 
        ( NULL == p_puPageVersion ) )
	{
		l_eRes = e_eFSS_COREML_RES_BADPOINTER;
	}
	else
	{
        /* Check if initialized */
        l_eResLL = eFSS_CORELL_IsInit(p_ptCtx->tCORELLCtx, &l_bIsInit);
        l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);

        if( e_eFSS_COREML_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_COREML_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_COREML_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_COREML_RES_CORRUPTCTX;
                }
                else
                {
                    /* Return data */
                    l_eResLL = eFSS_CORELL_GetStorSett(p_ptCtx->tCORELLCtx, &l_uDataLLL, &l_uNPageLL);
                    l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);

                    if( e_eFSS_COREML_RES_OK == l_eRes )
                    {
                        *p_puDataL = l_uDataLLL - 11u;
                        *p_puNPage = l_uNPageLL;
                        *p_puPageType = p_ptCtx->tStorSett.uPageType;
                        *p_puPageVersion = p_ptCtx->tStorSett.uPageVersion;
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_COREML_RES eFSS_COREML_GetBuff(t_eFSS_COREML_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_eBuffType,
								      uint8_t** p_ppuBuff, uint32_t* p_puBuffL)
{
	/* Return local var */
	e_eFSS_CORELL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    uint8_t* l_puBuffLL;
    uint32_t l_uBuffLLL;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ppuBuff ) || ( NULL == p_puBuffL ) )
	{
		l_eRes = e_eFSS_COREML_RES_BADPOINTER;
	}
	else
	{
        /* Check if initialized */
        l_eResLL = eFSS_CORELL_IsInit(p_ptCtx->tCORELLCtx, &l_bIsInit);
        l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);

        if( e_eFSS_COREML_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_COREML_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_COREML_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_COREML_RES_CORRUPTCTX;
                }
                else
                {
                    /* Return data */
                    l_eResLL = eFSS_COREML_GetBuff(p_ptCtx->tCORELLCtx, &l_puBuffLL, &l_uBuffLLL);
                    l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);

                    if( e_eFSS_COREML_RES_OK == l_eRes )
                    {
                        *p_puBuffL = l_uBuffLLL - 11u;
                        *p_ppuBuff = l_puBuffLL;
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_COREML_RES eFSS_COREML_LoadPageInBuffNChkVal(t_eFSS_COREML_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_eBuffType,
								                    const uint32_t p_uPageIndx)
{
	/* Return local var */
	e_eFSS_CORELL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    uint8_t* l_puBuffLL;
    uint32_t l_uBuffLLL;
    uint32_t l_uComulIndx;
    uint32_t l_uTemp;
    uint8_t  l_uPageType;
    uint16_t l_uPageVersion;
    uint32_t l_uMagicNumber;
    uint32_t l_uPageCrc;
    uint32_t l_uPageCrcCalc;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_COREML_RES_BADPOINTER;
	}
	else
	{
        /* Check if initialized */
        l_eResLL = eFSS_CORELL_IsInit(p_ptCtx->tCORELLCtx, &l_bIsInit);
        l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);

        if( e_eFSS_COREML_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_COREML_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_COREML_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_COREML_RES_CORRUPTCTX;
                }
                else
                {
                    /* Load pae in internal buffer */
                    l_eResLL = eFSS_CORELL_LoadPageInBuff(&p_ptCtx->tCORELLCtx, p_eBuffType, p_uPageIndx);
                    l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);

                    if( e_eFSS_COREML_RES_OK == l_eRes )
                    {
                        /* Get internal buffer */
                        l_eResLL = eFSS_COREML_GetBuff(p_ptCtx->tCORELLCtx, &l_puBuffLL, &l_uBuffLLL);
                        l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);

                        if( e_eFSS_COREML_RES_OK == l_eRes )
                        {
                            /* Initialize internal status */
                            l_uMagicNumber = 0u;
                            l_uPageCrc = 0u;
                            l_uComulIndx = l_uBuffLLL - 11u;

                            /* --- Copy data Little endian -- PAGE TYPE */
                            l_uPageType = l_puBuffLL[l_uComulIndx];
                            l_uComulIndx++;

                            /* --- Copy data Little endian -- PAGE VERSION */
                            l_uTemp = (uint16_t) l_puBuffLL[l_uComulIndx];
                            l_uPageVersion |= ( l_uTemp & 0x00FFu );
                            l_uComulIndx++;

                            l_uTemp =  (uint16_t) ( ( (uint16_t) l_puBuffLL[l_uComulIndx] ) << 8u  );
                            l_uPageVersion |= ( l_uTemp & 0xFF00u );
                            l_uComulIndx++;

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
                                        if( p_ptCtx->tStorSett.uPageVersion != uPageVersion )
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

e_eFSS_COREML_RES eFSS_COREML_FlushBuffWUpdValInPage(t_eFSS_COREML_Ctx* const p_ptCtx,
                                                     e_eFSS_CORELL_BUFTYPE p_eBuffType, const uint32_t p_uPageIndx)
{
	/* Return local var */
	e_eFSS_CORELL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    uint8_t* l_puBuffLL;
    uint32_t l_uBuffLLL;
    uint32_t l_uComulIndx;
    uint32_t l_uTemp;
    uint8_t  l_uPageType;
    uint16_t l_uPageVersion;
    uint32_t l_uMagicNumber;
    uint32_t l_uPageCrcCalc;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_COREML_RES_BADPOINTER;
	}
	else
	{
        /* Check if initialized */
        l_eResLL = eFSS_CORELL_IsInit(p_ptCtx->tCORELLCtx, &l_bIsInit);
        l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);

        if( e_eFSS_COREML_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_COREML_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_COREML_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_COREML_RES_CORRUPTCTX;
                }
                else
                {
                    /* Get internal buffer */
                    l_eResLL = eFSS_COREML_GetBuff(&p_ptCtx->tCORELLCtx, &l_puBuffLL, &l_uBuffLLL);
                    l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);

                    if( e_eFSS_COREML_RES_OK == l_eRes )
                    {
                        /* Initialize internal status */
                        l_uPageType = p_ptCtx->tStorSett.uPageType;
                        l_uPageVersion = p_ptCtx->tStorSett.uPageVersion;
                        l_uMagicNumber = EFSS_PAGEMAGICNUMBER;
                        l_uComulIndx = l_uBuffLLL - 11u;

                        /* --- Copy data Little endian -- PAGE TYPE */
                        l_puBuffLL[l_uComulIndx] = (uint8_t) ( ( l_uPageType        ) & 0xFFu );
                        l_uComulIndx++;

                        /* --- Copy data Little endian -- PAGE VERSION */
                        l_puBuffLL[l_uComulIndx] = (uint8_t) ( ( l_uPageVersion        ) & 0x00FFu );
                        l_uComulIndx++;

                        l_puBuffLL[l_uComulIndx] = (uint8_t) ( ( l_uPageVersion >> 8u  ) & 0x00FFu );
                        l_uComulIndx++;

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
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_COREML_RES eFSS_COREML_CalcCrcInBuff(t_eFSS_COREML_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_eBuffType,
								            uint32_t p_uCrcSeed, uint32_t p_uLenCalc, uint32_t* p_puCrc)
{
	/* Return local var */
	e_eFSS_CORELL_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    uint8_t* l_puBuffLL;
    uint32_t l_uBuffLLL;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puCrc ) )
	{
		l_eRes = e_eFSS_COREML_RES_BADPOINTER;
	}
	else
	{
        /* Check if initialized */
        l_eResLL = eFSS_CORELL_IsInit(p_ptCtx->tCORELLCtx, &l_bIsInit);
        l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);

        if( e_eFSS_COREML_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_COREML_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_COREML_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_COREML_RES_CORRUPTCTX;
                }
                else
                {
                    /* Return data */
                    l_eResLL = eFSS_COREML_GetBuff(p_ptCtx->tCORELLCtx, &l_puBuffLL, &l_uBuffLLL);
                    l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);

                    if( e_eFSS_COREML_RES_OK == l_eRes )
                    {
                        /* Check data validity */
                        if( p_uLenCalc > l_uBuffLLL - 11u)
                        {
                            l_eRes = e_eFSS_COREML_RES_BADPARAM;
                        }
                        else
                        {
                            /* Calculate CRC */
                            l_eResLL = eFSS_CORELL_CalcCrcInBuff(&p_ptCtx->tCORELLCtx, p_eBuffType, p_uCrcSeed,
                                                                 p_uLenCalc, p_puCrc);
                            l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);
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
static bool_t eFSS_COREML_IsStatusStillCoherent(const t_eFSS_COREML_Ctx* p_ptCtx)
{
    /* Return local var */
    bool_t l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

    /* Local var used for calculation */
    uint32_t l_uPageL;
    uint32_t l_uNPage;

    l_eResLL = eFSS_CORELL_GetStorSett(&p_ptCtx->tCORELLCtx, &l_uPageL, &l_uNPage);
    if( e_eFSS_CORELL_RES_OK != l_eResLL )
    {
        l_eRes = false;
    }
    else
    {
        /* Check data validity */
        if( l_uPageL <= 11u )
        {
            l_eRes = false;
        }
        else
        {
            l_eRes = true;
        }
    }

    return l_eRes;
}

static e_eFSS_COREML_RES eFSS_COREML_LLtoMLRes(const e_eFSS_CORELL_RES p_eLLRes)
{
    e_eFSS_COREML_RES l_eRes;

    switch(p_eLLRes)
    {
        case e_eFSS_CORELL_RES_OK:
        {
            l_eRes = e_eFSS_COREML_RES_OK;
            break;
        }

        case e_eFSS_CORELL_RES_NOINITLIB:
        {
            l_eRes = e_eFSS_COREML_RES_NOINITLIB;
            break;
        }

        case e_eFSS_CORELL_RES_BADPARAM:
        {
            l_eRes = e_eFSS_COREML_RES_BADPARAM;
            break;
        }

        case e_eFSS_CORELL_RES_BADPOINTER:
        {
            l_eRes = e_eFSS_COREML_RES_BADPOINTER;
            break;
        }

        case e_eFSS_CORELL_RES_CORRUPTCTX:
        {
            l_eRes = e_eFSS_COREML_RES_CORRUPTCTX;
            break;
        }

        case e_eFSS_CORELL_RES_CLBCKERASEERR:
        {
            l_eRes = e_eFSS_COREML_RES_CLBCKERASEERR;
            break;
        }

        case e_eFSS_CORELL_RES_CLBCKWRITEERR:
        {
            l_eRes = e_eFSS_COREML_RES_CLBCKWRITEERR;
            break;
        }

        case e_eFSS_CORELL_RES_CLBCKREADERR:
        {
            l_eRes = e_eFSS_COREML_RES_CLBCKREADERR;
            break;
        }

        case e_eFSS_CORELL_RES_CLBCKCRCERR:
        {
            l_eRes = e_eFSS_COREML_RES_CLBCKCRCERR;
            break;
        }

        case e_eFSS_CORELL_RES_WRITENOMATCHREAD:
        {
            l_eRes = e_eFSS_COREML_RES_WRITENOMATCHREAD;
            break;
        }

        default:
        {
            l_eRes = e_eFSS_COREML_RES_CORRUPTCTX;
            break;
        }
    }

    return l_eRes;
}