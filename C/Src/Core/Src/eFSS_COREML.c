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
	e_eFSS_COREML_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

	uint8_t* l_puBuff;
	uint32_t l_uBuffL;
    uint32_t l_uPageL;
    uint32_t l_uNPage;
    uint32_t l_uComulIndx;
    uint32_t l_uTemp;
    uint32_t l_uMagicNumber;
    uint32_t l_uPageCrc;
    uint32_t l_uPageCrcStored;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_COREML_RES_BADPOINTER;
	}
	else
	{
        l_eResLL = eFSS_CORELL_LoadPageInBuff(&p_ptCtx->tCORELLCtx, p_tBuffType, p_uPageIndx);
        l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);

        if( e_eFSS_COREML_RES_OK == l_eRes )
        {
            /* Now can get page buffer and verify CRC + Magic number */
            l_eResLL = eFSS_CORELL_GetBuff(&p_ptCtx->tCORELLCtx, p_tBuffType, &l_puBuff, &l_uBuffL);
            l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);

            if( e_eFSS_COREML_RES_OK == l_eRes )
            {
                /* Initialize internal status */
                l_uMagicNumber = 0u;
                l_uPageCrc = 0u;
                l_uComulIndx = p_uPageL - 8u;

                /* Extract CRC and Page magic number  */
                /* --- Copy data Little endian */
                l_uTemp = (uint32_t) p_puPageBuf[l_uComulIndx];
                l_uMagicNumber |= ( l_uTemp & 0x000000FFu );
                l_uComulIndx++;

                l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 8u  );
                l_uMagicNumber |= ( l_uTemp & 0x0000FF00u );
                l_uComulIndx++;

                l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 16u  );
                l_uMagicNumber |= ( l_uTemp & 0x00FF0000u );
                l_uComulIndx++;

                l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 24u  );
                l_uMagicNumber |= ( l_uTemp & 0xFF000000u );
                l_uComulIndx++;

                /* --- Copy data Little endian */
                l_uTemp = (uint32_t) p_puPageBuf[l_uComulIndx];
                l_uPageCrc |= ( l_uTemp & 0x000000FFu );
                l_uComulIndx++;

                l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 8u  );
                l_uPageCrc |= ( l_uTemp & 0x0000FF00u );
                l_uComulIndx++;

                l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 16u  );
                l_uPageCrc |= ( l_uTemp & 0x00FF0000u );
                l_uComulIndx++;

                l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 24u  );
                l_uPageCrc |= ( l_uTemp & 0xFF000000u );
                l_uComulIndx++;

                if( EFSS_PAGEMAGICNUMBER != l_uMagicNumber )
                {
                    l_eRes = e_eFSS_COREML_RES_NOTVALIDLOG;
                }
                else
                {
                    l_eResLL = eFSS_CORELL_GetStorSett(&p_ptCtx->tCORELLCtx, &l_uPageL, &l_uNPage)
                    l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);

                    if( e_eFSS_COREML_RES_OK == l_eRes )
                    {
                        if( l_uPageL <= EFSS_PAGEMETASIZE )
                        {
                            l_eRes = e_eFSS_COREML_RES_CORRUPTCTX;
                        }
                        else
                        {
                            /* Calculate CRC */
                            l_eResLL = eFSS_CalcCrcInBuff(&p_ptCtx->tCORELLCtx, p_tBuffType, MAX_UINT32VAL, l_uPageL - 4u, &l_uPageCrcStored);
                            l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);
                            if( e_eFSS_COREML_RES_OK == l_eRes )
                            {
                                if( l_uPageCrcStored != l_uPageCrc )
                                {
                                    l_eRes = e_eFSS_COREML_RES_NOTVALIDLOG;
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

e_eFSS_COREML_RES eFSS_COREML_FlushBuffWUpdCrcInPage(t_eFSS_COREML_Ctx* const p_ptCtx,
                                                     e_eFSS_CORELL_BUFTYPE p_tBuffType, const uint32_t p_uPageIndx)
{
	/* Local variable */
	e_eFSS_COREML_RES l_eRes;
    e_eFSS_CORELL_RES l_eResLL;

	uint8_t* l_puBuff;
	uint32_t l_uBuffL;
    uint32_t l_uPageL;
    uint32_t l_uNPage;
    uint32_t l_uComulIndx;
    uint32_t l_uTemp;
    uint32_t l_uMagicNumber;
    uint32_t l_uPageCrc;
    uint32_t l_uPageCrcStored;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_COREML_RES_BADPOINTER;
	}
	else
	{
        l_eResLL = eFSS_CORELL_LoadPageInBuff(&p_ptCtx->tCORELLCtx, p_tBuffType, p_uPageIndx);
        l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);

        if( e_eFSS_COREML_RES_OK == l_eRes )
        {
            /* Now can get page buffer and verify CRC + Magic number */
            l_eResLL = eFSS_CORELL_GetBuff(&p_ptCtx->tCORELLCtx, p_tBuffType, &l_puBuff, &l_uBuffL);
            l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);

            if( e_eFSS_COREML_RES_OK == l_eRes )
            {
                /* Initialize internal status */
                l_uMagicNumber = 0u;
                l_uPageCrc = 0u;
                l_uComulIndx = p_uPageL - 8u;

                /* Extract CRC and Page magic number  */
                /* --- Copy data Little endian */
                l_uTemp = (uint32_t) p_puPageBuf[l_uComulIndx];
                l_uMagicNumber |= ( l_uTemp & 0x000000FFu );
                l_uComulIndx++;

                l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 8u  );
                l_uMagicNumber |= ( l_uTemp & 0x0000FF00u );
                l_uComulIndx++;

                l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 16u  );
                l_uMagicNumber |= ( l_uTemp & 0x00FF0000u );
                l_uComulIndx++;

                l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 24u  );
                l_uMagicNumber |= ( l_uTemp & 0xFF000000u );
                l_uComulIndx++;

                /* --- Copy data Little endian */
                l_uTemp = (uint32_t) p_puPageBuf[l_uComulIndx];
                l_uPageCrc |= ( l_uTemp & 0x000000FFu );
                l_uComulIndx++;

                l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 8u  );
                l_uPageCrc |= ( l_uTemp & 0x0000FF00u );
                l_uComulIndx++;

                l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 16u  );
                l_uPageCrc |= ( l_uTemp & 0x00FF0000u );
                l_uComulIndx++;

                l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 24u  );
                l_uPageCrc |= ( l_uTemp & 0xFF000000u );
                l_uComulIndx++;

                if( EFSS_PAGEMAGICNUMBER != l_uMagicNumber )
                {
                    l_eRes = e_eFSS_COREML_RES_NOTVALIDLOG;
                }
                else
                {
                    l_eResLL = eFSS_CORELL_GetStorSett(&p_ptCtx->tCORELLCtx, &l_uPageL, &l_uNPage)
                    l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);

                    if( e_eFSS_COREML_RES_OK == l_eRes )
                    {
                        if( l_uPageL <= EFSS_PAGEMETASIZE )
                        {
                            l_eRes = e_eFSS_COREML_RES_CORRUPTCTX;
                        }
                        else
                        {
                            /* Calculate CRC */
                            l_eResLL = eFSS_CalcCrcInBuff(&p_ptCtx->tCORELLCtx, p_tBuffType, MAX_UINT32VAL, l_uPageL - 4u, &l_uPageCrcStored);
                            l_eRes = eFSS_COREML_LLtoMLRes(l_eResLL);
                            if( e_eFSS_COREML_RES_OK == l_eRes )
                            {
                                if( l_uPageCrcStored != l_uPageCrc )
                                {
                                    l_eRes = e_eFSS_COREML_RES_NOTVALIDLOG;
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
