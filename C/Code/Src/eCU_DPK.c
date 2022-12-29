/**
 * @file       eCU_DPK.c
 *
 * @brief      Data pack utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eCU_DPK.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eCU_DPK_IsStatusStillCoherent(const t_eCU_DPK_Ctx* p_ptCtx);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eCU_DPK_RES eCU_DPK_InitCtx(t_eCU_DPK_Ctx* const p_ptCtx, uint8_t* p_puMemPK, const uint32_t p_uMemPKL,
                              const bool_t p_bIsLEnd)
{
	/* Local variable */
	e_eCU_DPK_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL ==  p_puMemPK ) )
	{
		l_eRes = e_eCU_DPK_RES_BADPOINTER;
	}
	else
	{
		/* Check data validity */
		if( p_uMemPKL <= 0u )
		{
			l_eRes = e_eCU_DPK_RES_BADPARAM;
		}
		else
		{
            p_ptCtx->bIsInit = true;
            p_ptCtx->bIsLE = p_bIsLEnd;
            p_ptCtx->puMemPK = p_puMemPK;
            p_ptCtx->uMemPKL = p_uMemPKL;
            p_ptCtx->uMemPKCtr = 0u;

            l_eRes = e_eCU_DPK_RES_OK;
		}
    }

	return l_eRes;
}

e_eCU_DPK_RES eCU_DPK_IsInit(t_eCU_DPK_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eCU_DPK_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eCU_DPK_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInit;
        l_eRes = e_eCU_DPK_RES_OK;
	}

	return l_eRes;
}

e_eCU_DPK_RES eCU_DPK_StartNewPack(t_eCU_DPK_Ctx* const p_ptCtx)
{
	/* Local variable */
	e_eCU_DPK_RES l_eRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eCU_DPK_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_DPK_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_DPK_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_DPK_RES_CORRUPTCTX;
            }
            else
            {
                /* Update index */
                p_ptCtx->uMemPKCtr = 0u;
                l_eRes = e_eCU_DPK_RES_OK;
            }
		}
    }

	return l_eRes;
}

e_eCU_DPK_RES eCU_DPK_GetDataReference(t_eCU_DPK_Ctx* const p_ptCtx, uint8_t** p_ppuData, uint32_t* const p_puDataL)
{
	/* Local variable */
	e_eCU_DPK_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ppuData ) || ( NULL == p_puDataL ) )
	{
		l_eRes = e_eCU_DPK_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_DPK_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_DPK_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_DPK_RES_CORRUPTCTX;
            }
            else
            {
                *p_ppuData = p_ptCtx->puMemPK;
                *p_puDataL = p_ptCtx->uMemPKCtr;
                l_eRes = e_eCU_DPK_RES_OK;
            }
		}
    }

	return l_eRes;
}

e_eCU_DPK_RES eCU_DPK_GetNPushed(t_eCU_DPK_Ctx* const p_ptCtx, uint32_t* const p_puDataL)
{
	/* Local variable */
	e_eCU_DPK_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puDataL ) )
	{
		l_eRes = e_eCU_DPK_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_DPK_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_DPK_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_DPK_RES_CORRUPTCTX;
            }
            else
            {
                *p_puDataL = p_ptCtx->uMemPKCtr;
                l_eRes = e_eCU_DPK_RES_OK;
            }
		}
    }

	return l_eRes;
}

e_eCU_DPK_RES eCU_DPK_PushArray(t_eCU_DPK_Ctx* const p_ptCtx, uint8_t* p_puData, const uint32_t p_uDataL)
{
	/* Local variable */
	e_eCU_DPK_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puData ) )
	{
		l_eRes = e_eCU_DPK_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_DPK_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_DPK_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_DPK_RES_CORRUPTCTX;
            }
			else
			{
                /* Check data validity */
                if( p_uDataL <= 0u )
                {
                    l_eRes = e_eCU_DPK_RES_BADPARAM;
                }
                else
                {
                    /* Check if we have memory for this */
                    if( ( p_ptCtx->uMemPKCtr + p_uDataL ) > p_ptCtx->uMemPKL )
                    {
                        l_eRes = e_eCU_DPK_RES_OUTOFMEM;
                    }
                    else
                    {
                        /* Copy data */
                        (void)memcpy(&p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr], p_puData, p_uDataL);

                        /* Update index */
                        p_ptCtx->uMemPKCtr += p_uDataL;

                        l_eRes = e_eCU_DPK_RES_OK;
                    }
                }
			}
		}
    }

	return l_eRes;
}

e_eCU_DPK_RES eCU_DPK_PushU8(t_eCU_DPK_Ctx* const p_ptCtx, const uint8_t p_uData)
{
	/* Local variable */
	e_eCU_DPK_RES l_eRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eCU_DPK_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_DPK_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_DPK_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_DPK_RES_CORRUPTCTX;
            }
            else
            {
                /* Check if we have memory for this */
                if( ( p_ptCtx->uMemPKCtr + sizeof(uint8_t) ) > p_ptCtx->uMemPKL )
                {
                    /* no free memory */
                    l_eRes = e_eCU_DPK_RES_OUTOFMEM;
                }
                else
                {
                    /* Copy data */
                    p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = p_uData;

                    /* Update index */
                    p_ptCtx->uMemPKCtr++;

                    l_eRes = e_eCU_DPK_RES_OK;
                }
            }
		}
    }

	return l_eRes;
}

e_eCU_DPK_RES eCU_DPK_PushU16(t_eCU_DPK_Ctx* const p_ptCtx, const uint16_t p_uData)
{
	/* Local variable */
	e_eCU_DPK_RES l_eRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eCU_DPK_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_DPK_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_DPK_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_DPK_RES_CORRUPTCTX;
            }
            else
            {
                /* Check if we have memory for this */
                if( ( p_ptCtx->uMemPKCtr + sizeof(uint16_t) ) > p_ptCtx->uMemPKL )
                {
                    /* no free memory */
                    l_eRes = e_eCU_DPK_RES_OUTOFMEM;
                }
                else
                {
                    if( true == p_ptCtx->bIsLE)
                    {
                        /* Copy data Little endian */
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData        ) & 0x00FFu );
                        p_ptCtx->uMemPKCtr++;
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData >> 8u  ) & 0x00FFu );
                        p_ptCtx->uMemPKCtr++;
                    }
                    else
                    {
                        /* Copy data big endian */
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData >> 8u  ) & 0x00FFu );
                        p_ptCtx->uMemPKCtr++;
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData        ) & 0x00FFu );
                        p_ptCtx->uMemPKCtr++;
                    }

                    l_eRes = e_eCU_DPK_RES_OK;
                }
            }
		}
    }

	return l_eRes;
}

e_eCU_DPK_RES eCU_DPK_PushU32(t_eCU_DPK_Ctx* const p_ptCtx, const uint32_t p_uData)
{
	/* Local variable */
	e_eCU_DPK_RES l_eRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eCU_DPK_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_DPK_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_DPK_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_DPK_RES_CORRUPTCTX;
            }
            else
            {
                /* Check if we have memory for this */
                if( ( p_ptCtx->uMemPKCtr + sizeof(uint32_t) ) > p_ptCtx->uMemPKL )
                {
                    /* no free memory */
                    l_eRes = e_eCU_DPK_RES_OUTOFMEM;
                }
                else
                {
                    if( true == p_ptCtx->bIsLE)
                    {
                        /* Copy data Little endian */
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData        ) & 0x000000FFu );
                        p_ptCtx->uMemPKCtr++;
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData >> 8u  ) & 0x000000FFu );
                        p_ptCtx->uMemPKCtr++;
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData >> 16u ) & 0x000000FFu );
                        p_ptCtx->uMemPKCtr++;
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData >> 24u ) & 0x000000FFu );
                        p_ptCtx->uMemPKCtr++;
                    }
                    else
                    {
                        /* Copy data big endian */
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData >> 24u ) & 0x000000FFu );
                        p_ptCtx->uMemPKCtr++;
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData >> 16u ) & 0x000000FFu );
                        p_ptCtx->uMemPKCtr++;
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData >> 8u  ) & 0x000000FFu );
                        p_ptCtx->uMemPKCtr++;
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData        ) & 0x000000FFu );
                        p_ptCtx->uMemPKCtr++;
                    }

                    l_eRes = e_eCU_DPK_RES_OK;
                }
            }
		}
    }

	return l_eRes;
}

e_eCU_DPK_RES eCU_DPK_PushU64(t_eCU_DPK_Ctx* const p_ptCtx, const uint64_t p_uData)
{
	/* Local variable */
	e_eCU_DPK_RES l_eRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eCU_DPK_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_DPK_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_DPK_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_DPK_RES_CORRUPTCTX;
            }
            else
            {
                /* Check if we have memory for this */
                if( ( p_ptCtx->uMemPKCtr + sizeof(uint64_t) ) > p_ptCtx->uMemPKL )
                {
                    /* no free memory */
                    l_eRes = e_eCU_DPK_RES_OUTOFMEM;
                }
                else
                {
                    if( true == p_ptCtx->bIsLE)
                    {
                        /* Copy data Little endian */
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData        ) & 0x00000000000000FFu );
                        p_ptCtx->uMemPKCtr++;
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData >> 8u  ) & 0x00000000000000FFu );
                        p_ptCtx->uMemPKCtr++;
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData >> 16u ) & 0x00000000000000FFu );
                        p_ptCtx->uMemPKCtr++;
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData >> 24u ) & 0x00000000000000FFu );
                        p_ptCtx->uMemPKCtr++;
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData >> 32u ) & 0x00000000000000FFu );
                        p_ptCtx->uMemPKCtr++;
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData >> 40u ) & 0x00000000000000FFu );
                        p_ptCtx->uMemPKCtr++;
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData >> 48u ) & 0x00000000000000FFu );
                        p_ptCtx->uMemPKCtr++;
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData >> 56u ) & 0x00000000000000FFu );
                        p_ptCtx->uMemPKCtr++;
                    }
                    else
                    {
                        /* Copy data big endian */
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData >> 56u ) & 0x00000000000000FFu );
                        p_ptCtx->uMemPKCtr++;
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData >> 48u ) & 0x00000000000000FFu );
                        p_ptCtx->uMemPKCtr++;
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData >> 40u ) & 0x00000000000000FFu );
                        p_ptCtx->uMemPKCtr++;
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData >> 32u ) & 0x00000000000000FFu );
                        p_ptCtx->uMemPKCtr++;
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData >> 24u ) & 0x00000000000000FFu );
                        p_ptCtx->uMemPKCtr++;
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData >> 16u ) & 0x00000000000000FFu );
                        p_ptCtx->uMemPKCtr++;
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData >> 8u  ) & 0x00000000000000FFu );
                        p_ptCtx->uMemPKCtr++;
                        p_ptCtx->puMemPK[p_ptCtx->uMemPKCtr] = (uint8_t) ( ( p_uData        ) & 0x00000000000000FFu );
                        p_ptCtx->uMemPKCtr++;
                    }

                    l_eRes = e_eCU_DPK_RES_OK;
                }
            }
		}
    }

	return l_eRes;
}



/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eCU_DPK_IsStatusStillCoherent(const t_eCU_DPK_Ctx* p_ptCtx)
{
    bool_t l_eRes;

	/* Check context validity */
	if( ( p_ptCtx->uMemPKL <= 0u ) || ( NULL == p_ptCtx->puMemPK ) )
	{
		l_eRes = false;
	}
	else
	{
		/* Check queue data coherence */
		if( p_ptCtx->uMemPKCtr > p_ptCtx->uMemPKL )
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