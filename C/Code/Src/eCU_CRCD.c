/**
 * @file       eCU_CRCD.c
 *
 * @brief      Cal CRC using custom CRC32 using digest approach
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eCU_CRCD.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eCU_CRCD_IsStatusStillCoherent(const t_eCU_CRCD_Ctx* p_ptCtx);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eCU_CRCD_RES eCU_CRCD_InitCtx(t_eCU_CRCD_Ctx* const p_ptCtx, f_eCU_CRCD_CrcCb p_fCrc,
                                t_eCU_CRCD_CrcCtx* const p_ptFctx)
{
	/* Local variable */
	e_eCU_CRCD_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx) || ( NULL ==  p_fCrc) || ( NULL ==  p_ptFctx) )
	{
		l_eRes = e_eCU_CRCD_RES_BADPOINTER;
	}
	else
	{
        /* Init context */
		p_ptCtx->bIsInit = true;
		p_ptCtx->uBaseSeed = eCU_CRC_BASE_SEED;
		p_ptCtx->uDigestedTimes = 0u;
		p_ptCtx->uLastDigVal = 0u;
		p_ptCtx->fCrc = p_fCrc;
        p_ptCtx->ptCrcCtx = p_ptFctx;

		l_eRes = e_eCU_CRCD_RES_OK;
    }

	return l_eRes;
}

e_eCU_CRCD_RES eCU_CRCD_SeedInitCtx(t_eCU_CRCD_Ctx* const p_ptCtx, const uint32_t p_uUseed, f_eCU_CRCD_CrcCb p_fCrc,
                                    t_eCU_CRCD_CrcCtx* const p_ptFctx)
{
	/* Local variable */
	e_eCU_CRCD_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx) || ( NULL ==  p_fCrc) || ( NULL ==  p_ptFctx) )
	{
		l_eRes = e_eCU_CRCD_RES_BADPOINTER;
	}
	else
	{
        /* Init context */
		p_ptCtx->bIsInit = true;
		p_ptCtx->uBaseSeed = p_uUseed;
		p_ptCtx->uDigestedTimes = 0u;
		p_ptCtx->uLastDigVal = 0u;
		p_ptCtx->fCrc = p_fCrc;
        p_ptCtx->ptCrcCtx = p_ptFctx;

		l_eRes = e_eCU_CRCD_RES_OK;
    }

	return l_eRes;
}

e_eCU_CRCD_RES eCU_CRCD_IsInit(t_eCU_CRCD_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eCU_CRCD_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eCU_CRCD_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInit;
        l_eRes = e_eCU_CRCD_RES_OK;
	}

	return l_eRes;
}

e_eCU_CRCD_RES eCU_CRCD_Restart(t_eCU_CRCD_Ctx* const p_ptCtx)
{
	/* Local variable */
	e_eCU_CRCD_RES l_eRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eCU_CRCD_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_CRCD_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_CRCD_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_CRCD_RES_CORRUPTCTX;
            }
            else
            {
                /* Init context */
                p_ptCtx->uDigestedTimes = 0u;
                p_ptCtx->uLastDigVal = 0u;

                l_eRes = e_eCU_CRCD_RES_OK;
            }
        }
    }

    return l_eRes;
}

e_eCU_CRCD_RES eCU_CRCD_Digest(t_eCU_CRCD_Ctx* const p_ptCtx, const uint8_t* p_puData, const uint32_t p_uDataL)
{
	/* Local variable */
	e_eCU_CRCD_RES l_eRes;
    uint32_t l_uC32;
    bool_t l_bRes;
    f_eCU_CRCD_CrcCb l_fCb;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puData ) )
	{
		l_eRes = e_eCU_CRCD_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_CRCD_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_CRCD_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_CRCD_RES_CORRUPTCTX;
            }
			else
			{
                /* Check data validity */
                if( p_uDataL <= 0u )
                {
                    l_eRes = e_eCU_CRCD_RES_BADPARAM;
                }
                else
                {
                    /* Check if we have memory for this */
                    if( p_ptCtx->uDigestedTimes >= MAX_UINT32VAL )
                    {
                        l_eRes = e_eCU_CRCD_RES_TOOMANYDIGEST;
                    }
                    else
                    {
                        /* First time? */
                        if( 0u >= p_ptCtx->uDigestedTimes )
                        {
                            /* Use base p_uUseed for the first time */
                            l_fCb = p_ptCtx->fCrc;
                            l_bRes = (*(l_fCb))( p_ptCtx->ptCrcCtx, p_ptCtx->uBaseSeed, p_puData, p_uDataL, &l_uC32 );

                            if( true == l_bRes )
                            {
                                p_ptCtx->uDigestedTimes++;
                                p_ptCtx->uLastDigVal = l_uC32;
								l_eRes = e_eCU_CRCD_RES_OK;
                            }
							else
							{
								l_eRes = e_eCU_CRCD_RES_CLBCKREPORTERROR;
							}
                        }
                        else
                        {
                            /* Continue calc using old digested value for p_uUseed */
                            l_fCb = p_ptCtx->fCrc;
                            l_bRes = (*(l_fCb))( p_ptCtx->ptCrcCtx, p_ptCtx->uLastDigVal, p_puData, p_uDataL, &l_uC32 );

                            if( true == l_bRes )
                            {
                                p_ptCtx->uDigestedTimes++;
                                p_ptCtx->uLastDigVal = l_uC32;
								l_eRes = e_eCU_CRCD_RES_OK;
                            }
							else
							{
								l_eRes = e_eCU_CRCD_RES_CLBCKREPORTERROR;
							}
                        }
                    }
                }
			}
		}
    }

	return l_eRes;
}

e_eCU_CRCD_RES eCU_CRCD_GetDigestVal(t_eCU_CRCD_Ctx* const p_ptCtx, uint32_t* const p_puCrcCalc)
{
	/* Local variable */
	e_eCU_CRCD_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puCrcCalc ) )
	{
		l_eRes = e_eCU_CRCD_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_CRCD_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_CRCD_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_CRCD_RES_CORRUPTCTX;
            }
			else
			{
                /* Check data validity */
                if( p_ptCtx->uDigestedTimes <= 0u )
                {
                    /* Cannot retrive undigested value */
                    l_eRes = e_eCU_CRCD_RES_NODIGESTDONE;
                }
                else
                {
                    /* Return digested value */
                    *p_puCrcCalc = p_ptCtx->uLastDigVal;

                    /* Restart */
                    p_ptCtx->uDigestedTimes = 0u;
                    p_ptCtx->uLastDigVal = 0u;

                    l_eRes = e_eCU_CRCD_RES_OK;
                }
			}
		}
    }

	return l_eRes;
}



/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eCU_CRCD_IsStatusStillCoherent(const t_eCU_CRCD_Ctx* p_ptCtx)
{
    bool_t l_eRes;

	/* Check context validity */
	if( ( NULL == p_ptCtx->fCrc ) || ( NULL == p_ptCtx->ptCrcCtx ) )
	{
		l_eRes = false;
	}
	else
	{
        l_eRes = true;
	}

    return l_eRes;
}