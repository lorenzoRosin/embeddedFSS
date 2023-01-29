/**
 * @file       eFSS_COREHL.h
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
static bool_t eFSS_COREHL_IsStatusStillCoherent(const t_eFSS_COREHL_Ctx* p_ptCtx);
static e_eFSS_COREHL_RES eFSS_COREHL_MLtoHLRes(const e_eFSS_COREML_RES p_eLLRes);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_COREHL_RES eFSS_COREHL_InitCtx(t_eFSS_COREHL_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx const p_tCtxCb,
									  t_eFSS_CORELL_StorSet p_tStorSetLL, t_eFSS_COREML_StorSet p_tStorSetML,
                                      uint8_t* const p_puBuff, uint32_t p_uBuffL)
{
    e_eFSS_COREHL_RES l_eRes;
    e_eFSS_COREML_RES l_eResML;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity */
        if( p_tStorSet.uPagesLen <= 28u )
        {
            l_eRes = e_eFSS_COREHL_RES_BADPARAM;
        }
        else
        {
            /* Init ML context */
            l_eResML = eFSS_COREML_InitCtx(&p_ptCtx->tCOREMLCtx, p_tCtxCb, p_tStorSetLL,p_tStorSetML, p_puBuff, 
                                           p_uBuffL);
            l_eRes = eFSS_COREHL_MLtoHLRes(l_eResML);

            if( e_eFSS_COREHL_RES_OK == l_eRes )
            {
                memset( &p_ptCtx->tPageMeta, 0u, sizeof(t_eFSS_COREHL_PageMeta) );
            }
        }
    }

    return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_IsInit(t_eFSS_COREHL_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Return local var */
	e_eFSS_COREHL_RES l_eRes;
    e_eFSS_COREML_RES l_eResML;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
	}
	else
	{
        l_eResML = eFSS_COREML_IsInit(p_ptCtx->tCOREMLCtx, p_pbIsInit);
        l_eRes = eFSS_COREHL_MLtoHLRes(l_eResML);
	}

	return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_GetStorSett(t_eFSS_COREHL_Ctx* const p_ptCtx, uint32_t* p_puDataL, uint32_t* p_puNPage,
                                          uint8_t* p_puPageType, uint16_t* p_puPageVersion)
{
	/* Return local var */
	e_eFSS_COREHL_RES l_eRes;
    e_eFSS_COREML_RES l_eResML;

    /* Local var used for calculation */
    bool_t   l_bIsInit;
    uint32_t l_uDataLLL;
    uint32_t l_uNPageLL;
    uint8_t  l_uPageType;
    uint16_t l_uPageVersion;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puDataL ) || ( NULL == p_puNPage ) || ( NULL == p_puPageType ) || 
        ( NULL == p_puPageVersion ) )
	{
		l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
	}
	else
	{
        /* Check if initialized */
        l_eResML = eFSS_COREML_IsInit(p_ptCtx->tCOREMLCtx, &l_bIsInit);
        l_eRes = eFSS_COREHL_MLtoHLRes(l_eResML);

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
                    /* Return data */
                    l_eResML = eFSS_CORELL_GetStorSett(p_ptCtx->tCOREMLCtx, &l_uDataLLL, &l_uNPageLL, &l_uPageType,
                                                       &l_uPageVersion);
                    l_eRes = eFSS_COREHL_MLtoHLRes(l_eResML);

                    if( e_eFSS_COREHL_RES_OK == l_eRes )
                    {
                        *p_puDataL = l_uDataLLL - 17u;
                        *p_puNPage = l_uNPageLL;
                        *p_puPageType = l_uPageType;
                        *p_puPageVersion = l_uPageVersion;
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_GetBuff(t_eFSS_COREHL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_eBuffType,
								      uint8_t** p_ppuBuff, uint32_t* p_puBuffL, t_eFSS_COREHL_PageMeta** p_pptMeta)
{
	/* Return local var */
	e_eFSS_COREHL_RES l_eRes;
    e_eFSS_COREML_RES l_eResML;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    uint8_t* l_puBuffML;
    uint32_t l_uBuffLML;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ppuBuff ) || ( NULL == p_puBuffL ) || ( NULL == p_pptMeta ) )
	{
		l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
	}
	else
	{
        /* Check if initialized */
        l_eResML = eFSS_COREML_IsInit(p_ptCtx->tCOREMLCtx, &l_bIsInit);
        l_eRes = eFSS_COREHL_MLtoHLRes(l_eResML);

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
                    /* Return data */
                    l_eResML = eFSS_COREML_GetBuff(p_ptCtx->tCOREMLCtx, &l_puBuffML, &l_uBuffLML);
                    l_eRes = eFSS_COREHL_MLtoHLRes(l_eResML);

                    if( e_eFSS_COREHL_RES_OK == l_eRes )
                    {
                        *p_puBuffL = l_uBuffLML - 17u;
                        *p_ppuBuff = l_puBuffML;
                        *p_pptMeta = &p_ptCtx->tPageMeta;
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_LoadPageInBuffNChkVal(t_eFSS_COREHL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_eBuffType,
								                    const uint32_t p_uPageIndx)
{
	/* Return local var */
	e_eFSS_COREHL_RES l_eRes;
    e_eFSS_COREML_RES l_eResML;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    uint8_t* l_puBuffML;
    uint32_t l_uBuffLML;
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
		l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
	}
	else
	{
        /* Check if initialized */
        l_eResML = eFSS_COREML_IsInit(p_ptCtx->tCOREMLCtx, &l_bIsInit);
        l_eRes = eFSS_COREHL_MLtoHLRes(l_eResML);

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
                    /* Load pae in internal buffer */
                    l_eResML = eFSS_COREML_LoadPageInBuff(&p_ptCtx->tCOREMLCtx, p_eBuffType, p_uPageIndx);
                    l_eRes = eFSS_COREHL_MLtoHLRes(l_eResML);

                    if( e_eFSS_COREHL_RES_OK == l_eRes )
                    {
                        /* Get internal buffer */
                        l_eResML = eFSS_COREML_GetBuff(p_ptCtx->tCOREMLCtx, &l_puBuffML, &l_uBuffLML);
                        l_eRes = eFSS_COREHL_MLtoHLRes(l_eResML);

                        if( e_eFSS_COREHL_RES_OK == l_eRes )
                        {
                            /* Initialize internal status */
                            tPageMeta.uPageUseSpecific1 = 0u;
                            tPageMeta.uPageUseSpecific2 = 0u;
                            tPageMeta.uPageUseSpecific3 = 0u;
                            tPageMeta.uPageUseSpecific4 = 0u;
                            tPageMeta.uPageSubType = 0u;
                            l_uComulIndx = l_uBuffLML - 17u;

                            /* --- Copy data Little endian -- uPageUseSpecific1 */
                            l_uTemp = (uint32_t) l_puBuffML[l_uComulIndx];
                            tPageMeta.uPageUseSpecific1 |= ( l_uTemp & 0x000000FFu );
                            l_uComulIndx++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) l_puBuffML[l_uComulIndx] ) << 8u  );
                            tPageMeta.uPageUseSpecific1 |= ( l_uTemp & 0x0000FF00u );
                            l_uComulIndx++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) l_puBuffML[l_uComulIndx] ) << 16u  );
                            tPageMeta.uPageUseSpecific1 |= ( l_uTemp & 0x00FF0000u );
                            l_uComulIndx++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) l_puBuffML[l_uComulIndx] ) << 24u  );
                            tPageMeta.uPageUseSpecific1 |= ( l_uTemp & 0xFF000000u );
                            l_uComulIndx++;

                            /* --- Copy data Little endian -- uPageUseSpecific1 */
                            l_uTemp = (uint32_t) l_puBuffML[l_uComulIndx];
                            tPageMeta.uPageUseSpecific1 |= ( l_uTemp & 0x000000FFu );
                            l_uComulIndx++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) l_puBuffML[l_uComulIndx] ) << 8u  );
                            tPageMeta.uPageUseSpecific1 |= ( l_uTemp & 0x0000FF00u );
                            l_uComulIndx++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) l_puBuffML[l_uComulIndx] ) << 16u  );
                            tPageMeta.uPageUseSpecific1 |= ( l_uTemp & 0x00FF0000u );
                            l_uComulIndx++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) l_puBuffML[l_uComulIndx] ) << 24u  );
                            tPageMeta.uPageUseSpecific1 |= ( l_uTemp & 0xFF000000u );
                            l_uComulIndx++;

                            /* --- Copy data Little endian -- uPageUseSpecific1 */
                            l_uTemp = (uint32_t) l_puBuffML[l_uComulIndx];
                            tPageMeta.uPageUseSpecific1 |= ( l_uTemp & 0x000000FFu );
                            l_uComulIndx++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) l_puBuffML[l_uComulIndx] ) << 8u  );
                            tPageMeta.uPageUseSpecific1 |= ( l_uTemp & 0x0000FF00u );
                            l_uComulIndx++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) l_puBuffML[l_uComulIndx] ) << 16u  );
                            tPageMeta.uPageUseSpecific1 |= ( l_uTemp & 0x00FF0000u );
                            l_uComulIndx++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) l_puBuffML[l_uComulIndx] ) << 24u  );
                            tPageMeta.uPageUseSpecific1 |= ( l_uTemp & 0xFF000000u );
                            l_uComulIndx++;

                            /* --- Copy data Little endian -- uPageUseSpecific1 */
                            l_uTemp = (uint32_t) l_puBuffML[l_uComulIndx];
                            tPageMeta.uPageUseSpecific1 |= ( l_uTemp & 0x000000FFu );
                            l_uComulIndx++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) l_puBuffML[l_uComulIndx] ) << 8u  );
                            tPageMeta.uPageUseSpecific1 |= ( l_uTemp & 0x0000FF00u );
                            l_uComulIndx++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) l_puBuffML[l_uComulIndx] ) << 16u  );
                            tPageMeta.uPageUseSpecific1 |= ( l_uTemp & 0x00FF0000u );
                            l_uComulIndx++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) l_puBuffML[l_uComulIndx] ) << 24u  );
                            tPageMeta.uPageUseSpecific1 |= ( l_uTemp & 0xFF000000u );
                            l_uComulIndx++;

                            /* --- Copy data Little endian -- PAGE SUBTYPE */
                            tPageMeta.uPageSubType = l_puBuffML[l_uComulIndx];
                            l_uComulIndx++;
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_FlushBuffWUpdValInPage(t_eFSS_COREHL_Ctx* const p_ptCtx,
                                                     e_eFSS_CORELL_BUFTYPE p_eBuffType, const uint32_t p_uPageIndx)
{
	/* Return local var */
	e_eFSS_COREHL_RES l_eRes;
    e_eFSS_COREML_RES l_eResML;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    uint8_t* l_puBuffML;
    uint32_t l_uBuffLML;
    uint32_t l_uComulIndx;
    uint32_t l_uTemp;
    uint8_t  l_uPageType;
    uint16_t l_uPageVersion;
    uint32_t l_uMagicNumber;
    uint32_t l_uPageCrcCalc;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
	}
	else
	{
        /* Check if initialized */
        l_eResML = eFSS_COREML_IsInit(p_ptCtx->tCOREMLCtx, &l_bIsInit);
        l_eRes = eFSS_COREHL_MLtoHLRes(l_eResML);

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
                    /* Get internal buffer */
                    l_eResML = eFSS_COREML_GetBuff(&p_ptCtx->tCOREMLCtx, &l_puBuffML, &l_uBuffLML);
                    l_eRes = eFSS_COREHL_MLtoHLRes(l_eResML);

                    if( e_eFSS_COREHL_RES_OK == l_eRes )
                    {
                        /* Initialize internal status */
                        l_uComulIndx = l_uBuffLML - 17u;

                        /* --- Copy data Little endian -- uPageUseSpecific1 */
                        l_puBuffML[l_uComulIndx] = (uint8_t) ( ( tPageMeta.uPageUseSpecific1        ) & 0x000000FFu );
                        l_uComulIndx++;

                        l_puBuffML[l_uComulIndx] = (uint8_t) ( ( tPageMeta.uPageUseSpecific1 >> 8u  ) & 0x000000FFu );
                        l_uComulIndx++;

                        l_puBuffML[l_uComulIndx] = (uint8_t) ( ( l_uMagitPageMeta.uPageUseSpecific1cNumber >> 16u ) & 0x000000FFu );
                        l_uComulIndx++;

                        l_puBuffML[l_uComulIndx] = (uint8_t) ( ( l_uMagitPageMeta.uPageUseSpecific1cNumber >> 24u ) & 0x000000FFu );
                        l_uComulIndx++;

                        /* --- Copy data Little endian -- uPageUseSpecific1 */
                        l_puBuffML[l_uComulIndx] = (uint8_t) ( ( tPageMeta.uPageUseSpecific1        ) & 0x000000FFu );
                        l_uComulIndx++;

                        l_puBuffML[l_uComulIndx] = (uint8_t) ( ( tPageMeta.uPageUseSpecific1 >> 8u  ) & 0x000000FFu );
                        l_uComulIndx++;

                        l_puBuffML[l_uComulIndx] = (uint8_t) ( ( l_uMagitPageMeta.uPageUseSpecific1cNumber >> 16u ) & 0x000000FFu );
                        l_uComulIndx++;

                        l_puBuffML[l_uComulIndx] = (uint8_t) ( ( l_uMagitPageMeta.uPageUseSpecific1cNumber >> 24u ) & 0x000000FFu );
                        l_uComulIndx++;

                        /* --- Copy data Little endian -- uPageUseSpecific1 */
                        l_puBuffML[l_uComulIndx] = (uint8_t) ( ( tPageMeta.uPageUseSpecific1        ) & 0x000000FFu );
                        l_uComulIndx++;

                        l_puBuffML[l_uComulIndx] = (uint8_t) ( ( tPageMeta.uPageUseSpecific1 >> 8u  ) & 0x000000FFu );
                        l_uComulIndx++;

                        l_puBuffML[l_uComulIndx] = (uint8_t) ( ( l_uMagitPageMeta.uPageUseSpecific1cNumber >> 16u ) & 0x000000FFu );
                        l_uComulIndx++;

                        l_puBuffML[l_uComulIndx] = (uint8_t) ( ( l_uMagitPageMeta.uPageUseSpecific1cNumber >> 24u ) & 0x000000FFu );
                        l_uComulIndx++;

                        /* --- Copy data Little endian -- uPageUseSpecific1 */
                        l_puBuffML[l_uComulIndx] = (uint8_t) ( ( tPageMeta.uPageUseSpecific1        ) & 0x000000FFu );
                        l_uComulIndx++;

                        l_puBuffML[l_uComulIndx] = (uint8_t) ( ( tPageMeta.uPageUseSpecific1 >> 8u  ) & 0x000000FFu );
                        l_uComulIndx++;

                        l_puBuffML[l_uComulIndx] = (uint8_t) ( ( l_uMagitPageMeta.uPageUseSpecific1cNumber >> 16u ) & 0x000000FFu );
                        l_uComulIndx++;

                        l_puBuffML[l_uComulIndx] = (uint8_t) ( ( l_uMagitPageMeta.uPageUseSpecific1cNumber >> 24u ) & 0x000000FFu );
                        l_uComulIndx++;

                        /* --- Copy data Little endian -- PAGE TYPE */
                        l_puBuffML[l_uComulIndx] = (uint8_t) ( ( l_uPageType        ) & 0xFFu );
                        l_uComulIndx++;

                        /* Flush buffer */
                        l_eResML = eFSS_COREML_FlushBuffWUpdValInPage(&p_ptCtx->tCOREMLCtx, p_eBuffType, p_uPageIndx);
                        l_eRes = eFSS_COREHL_MLtoHLRes(l_eResML);
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_COREHL_RES eFSS_COREHL_CalcCrcInBuff(t_eFSS_COREHL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_eBuffType,
								            uint32_t p_uCrcSeed, uint32_t p_uLenCalc, uint32_t* p_puCrc)
{
	/* Return local var */
	e_eFSS_COREHL_RES l_eRes;
    e_eFSS_COREML_RES l_eResML;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    uint8_t* l_puBuffML;
    uint32_t l_uBuffLML;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puCrc ) )
	{
		l_eRes = e_eFSS_COREHL_RES_BADPOINTER;
	}
	else
	{
        /* Check if initialized */
        l_eResML = eFSS_COREML_IsInit(p_ptCtx->tCOREMLCtx, &l_bIsInit);
        l_eRes = eFSS_COREHL_MLtoHLRes(l_eResML);

        if( e_eFSS_COREHL_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_COREHL_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_COREML_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_COREHL_RES_CORRUPTCTX;
                }
                else
                {
                    /* Return data */
                    l_eResML = eFSS_COREML_GetBuff(p_ptCtx->tCOREMLCtx, &l_puBuffML, &l_uBuffLML);
                    l_eRes = eFSS_COREHL_MLtoHLRes(l_eResML);

                    if( e_eFSS_COREHL_RES_OK == l_eRes )
                    {
                        /* Check data validity */
                        if( p_uLenCalc > l_uBuffLML - 17u)
                        {
                            l_eRes = e_eFSS_COREHL_RES_BADPARAM;
                        }
                        else
                        {
                            /* Calculate CRC */
                            l_eResML = eFSS_COREML_CalcCrcInBuff(&p_ptCtx->tCOREMLCtx, p_eBuffType, p_uCrcSeed,
                                                                 p_uLenCalc, p_puCrc);
                            l_eRes = eFSS_COREHL_MLtoHLRes(l_eResML);
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
static bool_t eFSS_COREHL_IsStatusStillCoherent(const t_eFSS_COREHL_Ctx* p_ptCtx)
{
    /* Return local var */
    bool_t l_eRes;
    e_eFSS_CORELL_RES l_eResML;

    /* Local var used for calculation */
    uint32_t l_uPageL;
    uint32_t l_uNPage;

    l_eResML = eFSS_CORELL_GetStorSett(&p_ptCtx->tCOREMLCtx, &l_uPageL, &l_uNPage);
    if( e_eFSS_CORELL_RES_OK != l_eResML )
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

static e_eFSS_COREHL_RES eFSS_COREHL_MLtoHLRes(const e_eFSS_COREML_RES p_eLLRes)
{
    e_eFSS_COREML_RES l_eRes;

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