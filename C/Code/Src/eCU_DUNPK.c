/**
 * @file       eCU_DUNPK.c
 *
 * @brief      Data pack utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eCU_DUNPK.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eCU_DUNPK_IsStatusStillCoherent(const t_eCU_DUNPK_Ctx* p_ptCtx);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eCU_DUNPK_RES eCU_DUNPK_InitCtx(t_eCU_DUNPK_Ctx* const p_ptCtx, uint8_t* p_puMemUPK, const uint32_t p_uMemUPKL,
                                  const bool_t p_bIsLEnd)
{
	/* Local variable */
	e_eCU_DUNPK_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL ==  p_puMemUPK ) )
	{
		l_eRes = e_eCU_DUNPK_RES_BADPOINTER;
	}
	else
	{
		/* Check data validity */
		if( p_uMemUPKL <= 0u )
		{
			l_eRes = e_eCU_DUNPK_RES_BADPARAM;
		}
		else
		{
            p_ptCtx->bIsInit = true;
            p_ptCtx->bIsLE = p_bIsLEnd;
            p_ptCtx->puMemUPK = p_puMemUPK;
            p_ptCtx->uMemUPKL = p_uMemUPKL;
            p_ptCtx->uMemUPKFrameL = 0u;
            p_ptCtx->uMemUPKCtr = 0u;

            l_eRes = e_eCU_DUNPK_RES_OK;
		}
    }

	return l_eRes;
}

e_eCU_DUNPK_RES eCU_DUNPK_IsInit(t_eCU_DUNPK_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eCU_DUNPK_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eCU_DUNPK_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInit;
        l_eRes = e_eCU_DUNPK_RES_OK;
	}

	return l_eRes;
}

e_eCU_DUNPK_RES eCU_DUNPK_GetUPkDataLocat(t_eCU_DUNPK_Ctx* const p_ptCtx, uint8_t** p_ppuData,
                                          uint32_t* const p_puMaxDataL)
{
	/* Local variable */
	e_eCU_DUNPK_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ppuData ) || ( NULL == p_puMaxDataL ) )
	{
		l_eRes = e_eCU_DUNPK_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_DUNPK_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_DUNPK_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_DUNPK_RES_CORRUPTCTX;
            }
            else
            {
                /* return data */
                *p_ppuData = p_ptCtx->puMemUPK;
                *p_puMaxDataL = p_ptCtx->uMemUPKL;
                l_eRes = e_eCU_DUNPK_RES_OK;
            }
		}
	}

	return l_eRes;
}

e_eCU_DUNPK_RES eCU_DUNPK_StartNewFrame(t_eCU_DUNPK_Ctx* const p_ptCtx, const uint32_t p_uFrameL)
{
	/* Local variable */
	e_eCU_DUNPK_RES l_eRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eCU_DUNPK_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_DUNPK_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_DUNPK_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_DUNPK_RES_CORRUPTCTX;
            }
            else
            {
                /* Check param validity */
                if( ( p_uFrameL <= 0u ) || ( p_uFrameL > p_ptCtx->uMemUPKL ) )
                {
                    l_eRes = e_eCU_DUNPK_RES_BADPARAM;
                }
                else
                {
                    /* Update data */
                    p_ptCtx->uMemUPKFrameL = p_uFrameL;
                    p_ptCtx->uMemUPKCtr = 0u;
                    l_eRes = e_eCU_DUNPK_RES_OK;
                }
            }
		}
	}

	return l_eRes;
}

e_eCU_DUNPK_RES eCU_DUNPK_RestartCurrentUnpack(t_eCU_DUNPK_Ctx* const p_ptCtx)
{
	/* Local variable */
	e_eCU_DUNPK_RES l_eRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eCU_DUNPK_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_DUNPK_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_DUNPK_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_DUNPK_RES_CORRUPTCTX;
            }
            else
            {
                /* Check Init */
                if( p_ptCtx->uMemUPKFrameL <= 0u )
                {
                    l_eRes = e_eCU_DUNPK_RES_NOINITFRAME;
                }
                else
                {
                    /* Update index */
                    p_ptCtx->uMemUPKCtr = 0u;
                    l_eRes = e_eCU_DUNPK_RES_OK;
                }
            }
		}
    }

	return l_eRes;
}

e_eCU_DUNPK_RES eCU_DUNPK_GetRemToPop(t_eCU_DUNPK_Ctx* const p_ptCtx, uint32_t* const p_puGettedL)
{
	/* Local variable */
	e_eCU_DUNPK_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puGettedL ) )
	{
		l_eRes = e_eCU_DUNPK_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_DUNPK_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_DUNPK_IsStatusStillCoherent(p_ptCtx) )
            {
                /* We have removed more data that we had */
                l_eRes = e_eCU_DUNPK_RES_CORRUPTCTX;
            }
            else
            {
                /* Check Init */
                if( p_ptCtx->uMemUPKFrameL <= 0u )
                {
                    l_eRes = e_eCU_DUNPK_RES_NOINITFRAME;
                }
                else
                {
                    *p_puGettedL = p_ptCtx->uMemUPKFrameL - p_ptCtx->uMemUPKCtr;
                    l_eRes = e_eCU_DUNPK_RES_OK;
                }
            }
		}
    }

	return l_eRes;
}

e_eCU_DUNPK_RES eCU_DUNPK_PopArray(t_eCU_DUNPK_Ctx* const p_ptCtx, uint8_t* p_puData, uint32_t const p_uToGetL)
{
	/* Local variable */
	e_eCU_DUNPK_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puData ) )
	{
		l_eRes = e_eCU_DUNPK_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_DUNPK_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_DUNPK_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_DUNPK_RES_CORRUPTCTX;
            }
            else
            {
                /* Check Init */
                if( p_ptCtx->uMemUPKFrameL <= 0u )
                {
                    l_eRes = e_eCU_DUNPK_RES_NOINITFRAME;
                }
                else
                {
                    /* Check data validity */
                    if( p_uToGetL <= 0u )
                    {
                        /* We have removed more data that we had */
                        l_eRes = e_eCU_DUNPK_RES_BADPARAM;
                    }
                    else
                    {
                        /* Check if we can pop that amount */
                        if( ( p_ptCtx->uMemUPKCtr + p_uToGetL ) > p_ptCtx->uMemUPKFrameL )
                        {
                            l_eRes = e_eCU_DUNPK_RES_NODATA;
                        }
                        else
                        {
                            /* Copy data */
                            (void)memcpy(p_puData, &p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr], p_uToGetL);

                            /* Update index */
                            p_ptCtx->uMemUPKCtr += p_uToGetL;
                            l_eRes = e_eCU_DUNPK_RES_OK;
                        }
                    }
                }
            }
		}
    }

	return l_eRes;
}

e_eCU_DUNPK_RES eCU_DUNPK_PopU8(t_eCU_DUNPK_Ctx* const p_ptCtx, uint8_t *p_puData)
{
	/* Local variable */
	e_eCU_DUNPK_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puData ) )
	{
		l_eRes = e_eCU_DUNPK_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_DUNPK_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_DUNPK_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_DUNPK_RES_CORRUPTCTX;
            }
            else
            {
                /* Check Init */
                if( p_ptCtx->uMemUPKFrameL <= 0u )
                {
                    l_eRes = e_eCU_DUNPK_RES_NOINITFRAME;
                }
                else
                {
                    /* Check if we can pop that amount */
                    if( ( p_ptCtx->uMemUPKCtr + sizeof(uint8_t) ) > p_ptCtx->uMemUPKFrameL )
                    {
                        l_eRes = e_eCU_DUNPK_RES_NODATA;
                    }
                    else
                    {
                        /* Copy data */
                        *p_puData = p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr];

                        /* Update index */
                        p_ptCtx->uMemUPKCtr++;
                        l_eRes = e_eCU_DUNPK_RES_OK;
                    }
                }
            }
		}
    }

	return l_eRes;
}

e_eCU_DUNPK_RES eCU_DUNPK_PopU16(t_eCU_DUNPK_Ctx* const p_ptCtx, uint16_t* p_puData)
{
	/* Local variable */
	e_eCU_DUNPK_RES l_eRes;
    uint16_t l_uTemp;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puData ) )
	{
		l_eRes = e_eCU_DUNPK_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_DUNPK_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_DUNPK_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_DUNPK_RES_CORRUPTCTX;
            }
            else
            {
                /* Check Init */
                if( p_ptCtx->uMemUPKFrameL <= 0u )
                {
                    l_eRes = e_eCU_DUNPK_RES_NOINITFRAME;
                }
                else
                {
                    /* Check if we can pop that amount */
                    if( ( p_ptCtx->uMemUPKCtr + sizeof(uint16_t) ) > p_ptCtx->uMemUPKFrameL )
                    {
                        l_eRes = e_eCU_DUNPK_RES_NODATA;
                    }
                    else
                    {
                        *p_puData = 0u;

                        if( true == p_ptCtx->bIsLE)
                        {
                            /* Copy data Little endian */
                            l_uTemp = (uint16_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr];
                            *p_puData |= ( l_uTemp & 0x00FFu );
                            p_ptCtx->uMemUPKCtr++;

                            l_uTemp =  (uint16_t) ( ( (uint16_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr] ) << 8u  );
                            *p_puData |= ( l_uTemp & 0xFF00u );
                            p_ptCtx->uMemUPKCtr++;
                        }
                        else
                        {
                            /* Copy data big endian */
                            l_uTemp =  (uint16_t) ( ( (uint16_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr] ) << 8u  );
                            *p_puData |= ( l_uTemp & 0xFF00u );
                            p_ptCtx->uMemUPKCtr++;

                            l_uTemp = (uint16_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr];
                            *p_puData |= ( l_uTemp & 0x00FFu );
                            p_ptCtx->uMemUPKCtr++;
                        }

                        l_eRes = e_eCU_DUNPK_RES_OK;
                    }
                }
            }
		}
    }

	return l_eRes;
}

e_eCU_DUNPK_RES eCU_DUNPK_PopU32(t_eCU_DUNPK_Ctx* const p_ptCtx, uint32_t* p_puData)
{
	/* Local variable */
	e_eCU_DUNPK_RES l_eRes;
    uint32_t l_uTemp;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puData ) )
	{
		l_eRes = e_eCU_DUNPK_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_DUNPK_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_DUNPK_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_DUNPK_RES_CORRUPTCTX;
            }
            else
            {
                /* Check Init */
                if( p_ptCtx->uMemUPKFrameL <= 0u )
                {
                    l_eRes = e_eCU_DUNPK_RES_NOINITFRAME;
                }
                else
                {
                    /* Check if we can pop that amount */
                    if( ( p_ptCtx->uMemUPKCtr + sizeof(uint32_t) ) > p_ptCtx->uMemUPKFrameL )
                    {
                        l_eRes = e_eCU_DUNPK_RES_NODATA;
                    }
                    else
                    {
                        *p_puData = 0u;

                        if( true == p_ptCtx->bIsLE)
                        {
                            /* Copy data Little endian */
                            l_uTemp = (uint32_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr];
                            *p_puData |= ( l_uTemp & 0x000000FFu );
                            p_ptCtx->uMemUPKCtr++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr] ) << 8u  );
                            *p_puData |= ( l_uTemp & 0x0000FF00u );
                            p_ptCtx->uMemUPKCtr++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr] ) << 16u  );
                            *p_puData |= ( l_uTemp & 0x00FF0000u );
                            p_ptCtx->uMemUPKCtr++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr] ) << 24u  );
                            *p_puData |= ( l_uTemp & 0xFF000000u );
                            p_ptCtx->uMemUPKCtr++;
                        }
                        else
                        {
                            /* Copy data big endian */
                            l_uTemp =  (uint32_t) ( ( (uint32_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr] ) << 24u  );
                            *p_puData |= ( l_uTemp & 0xFF000000u );
                            p_ptCtx->uMemUPKCtr++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr] ) << 16u  );
                            *p_puData |= ( l_uTemp & 0x00FF0000u );
                            p_ptCtx->uMemUPKCtr++;

                            l_uTemp =  (uint32_t) ( ( (uint32_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr] ) << 8u  );
                            *p_puData |= ( l_uTemp & 0x0000FF00u );
                            p_ptCtx->uMemUPKCtr++;

                            l_uTemp = (uint32_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr];
                            *p_puData |= ( l_uTemp & 0x000000FFu );
                            p_ptCtx->uMemUPKCtr++;
                        }

                        l_eRes = e_eCU_DUNPK_RES_OK;
                    }
                }
            }
		}
	}

	return l_eRes;
}

e_eCU_DUNPK_RES eCU_DUNPK_PopU64(t_eCU_DUNPK_Ctx* const p_ptCtx, uint64_t* p_puData)
{
	/* Local variable */
	e_eCU_DUNPK_RES l_eRes;
    uint64_t l_uTemp;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puData ) )
	{
		l_eRes = e_eCU_DUNPK_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_DUNPK_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_DUNPK_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_DUNPK_RES_CORRUPTCTX;
            }
            else
            {
                /* Check Init */
                if( p_ptCtx->uMemUPKFrameL <= 0u )
                {
                    l_eRes = e_eCU_DUNPK_RES_NOINITFRAME;
                }
                else
                {
                    /* Check if we can pop that amount */
                    if( ( p_ptCtx->uMemUPKCtr + sizeof(uint64_t) ) > p_ptCtx->uMemUPKFrameL )
                    {
                        l_eRes = e_eCU_DUNPK_RES_NODATA;
                    }
                    else
                    {
                        *p_puData = 0u;

                        if( true == p_ptCtx->bIsLE)
                        {
                            /* Copy data Little endian */
                            l_uTemp = (uint64_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr];
                            *p_puData |= ( l_uTemp & 0x00000000000000FFUL );
                            p_ptCtx->uMemUPKCtr++;

                            l_uTemp =  (uint64_t) ( ( (uint64_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr] ) << 8u  );
                            *p_puData |= ( l_uTemp & 0x000000000000FF00UL );
                            p_ptCtx->uMemUPKCtr++;

                            l_uTemp =  (uint64_t) ( ( (uint64_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr] ) << 16u  );
                            *p_puData |= ( l_uTemp & 0x0000000000FF0000UL );
                            p_ptCtx->uMemUPKCtr++;

                            l_uTemp =  (uint64_t) ( ( (uint64_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr] ) << 24u  );
                            *p_puData |= ( l_uTemp & 0x00000000FF000000UL );
                            p_ptCtx->uMemUPKCtr++;

                            l_uTemp =  (uint64_t) ( ( (uint64_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr] ) << 32u  );
                            *p_puData |= ( l_uTemp & 0x000000FF00000000UL );
                            p_ptCtx->uMemUPKCtr++;

                            l_uTemp =  (uint64_t) ( ( (uint64_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr] ) << 40u  );
                            *p_puData |= ( l_uTemp & 0x0000FF0000000000UL );
                            p_ptCtx->uMemUPKCtr++;

                            l_uTemp =  (uint64_t) ( ( (uint64_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr] ) << 48u  );
                            *p_puData |= ( l_uTemp & 0x00FF000000000000UL );
                            p_ptCtx->uMemUPKCtr++;

                            l_uTemp =  (uint64_t) ( ( (uint64_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr] ) << 56u  );
                            *p_puData |= ( l_uTemp & 0xFF00000000000000UL );
                            p_ptCtx->uMemUPKCtr++;
                        }
                        else
                        {
                            /* Copy data big endian */
                            l_uTemp =  (uint64_t) ( ( (uint64_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr] ) << 56u  );
                            *p_puData |= ( l_uTemp & 0xFF00000000000000UL );
                            p_ptCtx->uMemUPKCtr++;

                            l_uTemp =  (uint64_t) ( ( (uint64_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr] ) << 48u  );
                            *p_puData |= ( l_uTemp & 0x00FF000000000000UL );
                            p_ptCtx->uMemUPKCtr++;

                            l_uTemp =  (uint64_t) ( ( (uint64_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr] ) << 40u  );
                            *p_puData |= ( l_uTemp & 0x0000FF0000000000UL );
                            p_ptCtx->uMemUPKCtr++;

                            l_uTemp =  (uint64_t) ( ( (uint64_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr] ) << 32u  );
                            *p_puData |= ( l_uTemp & 0x000000FF00000000UL );
                            p_ptCtx->uMemUPKCtr++;

                            l_uTemp =  (uint64_t) ( ( (uint64_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr] ) << 24u  );
                            *p_puData |= ( l_uTemp & 0x00000000FF000000UL );
                            p_ptCtx->uMemUPKCtr++;

                            l_uTemp =  (uint64_t) ( ( (uint64_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr] ) << 16u  );
                            *p_puData |= ( l_uTemp & 0x0000000000FF0000UL );
                            p_ptCtx->uMemUPKCtr++;

                            l_uTemp =  (uint64_t) ( ( (uint64_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr] ) << 8u  );
                            *p_puData |= ( l_uTemp & 0x000000000000FF00UL );
                            p_ptCtx->uMemUPKCtr++;

                            l_uTemp = (uint64_t) p_ptCtx->puMemUPK[p_ptCtx->uMemUPKCtr];
                            *p_puData |= ( l_uTemp & 0x00000000000000FFUL );
                            p_ptCtx->uMemUPKCtr++;
                        }

                        l_eRes = e_eCU_DUNPK_RES_OK;
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
static bool_t eCU_DUNPK_IsStatusStillCoherent(const t_eCU_DUNPK_Ctx* p_ptCtx)
{
    bool_t l_eRes;

	/* Check context validity */
	if( ( p_ptCtx->uMemUPKL <= 0u ) || ( NULL == p_ptCtx->puMemUPK ) )
	{
		l_eRes = false;
	}
	else
	{
		/* Check queue data coherence */
		if( ( p_ptCtx->uMemUPKCtr > p_ptCtx->uMemUPKFrameL ) || ( p_ptCtx->uMemUPKFrameL > p_ptCtx->uMemUPKL ) )
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