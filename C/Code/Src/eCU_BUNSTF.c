/**
 * @file       eCU_BUNSTF.c
 *
 * @brief      Byte unstuffer utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eCU_BUNSTF.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eCU_BUNSTF_IsStatusStillCoherent(const t_eCU_BUNSTF_Ctx* p_ptCtx);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eCU_BUNSTF_RES eCU_BUNSTF_InitCtx(t_eCU_BUNSTF_Ctx* const p_ptCtx, uint8_t* p_puBuff, const uint32_t p_uBuffL)
{
	/* Local variable */
	e_eCU_BUNSTF_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eCU_BUNSTF_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity */
        if( p_uBuffL <= 0u )
        {
            l_eRes = e_eCU_BUNSTF_RES_BADPARAM;
        }
        else
        {
            /* Initialize internal status */
            p_ptCtx->bIsInit = true;
            p_ptCtx->puBuff = p_puBuff;
            p_ptCtx->uBuffL = p_uBuffL;
            p_ptCtx->uFrameCtr = 0u;
            p_ptCtx->eSM = e_eCU_BUNSTFPRV_SM_NEEDSOF;
            l_eRes = e_eCU_BUNSTF_RES_OK;
        }
	}

	return l_eRes;
}

e_eCU_BUNSTF_RES eCU_BUNSTF_IsInit(t_eCU_BUNSTF_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eCU_BUNSTF_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eCU_BUNSTF_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInit;
        l_eRes = e_eCU_BUNSTF_RES_OK;
	}

	return l_eRes;
}

e_eCU_BUNSTF_RES eCU_BUNSTF_NewFrame(t_eCU_BUNSTF_Ctx* const p_ptCtx)
{
	/* Local variable */
	e_eCU_BUNSTF_RES l_eRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eCU_BUNSTF_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_BUNSTF_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_BUNSTF_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_BUNSTF_RES_CORRUPTCTX;
            }
            else
            {
                /* Update index */
                p_ptCtx->uFrameCtr = 0u;
				p_ptCtx->eSM = e_eCU_BUNSTFPRV_SM_NEEDSOF;
                l_eRes = e_eCU_BUNSTF_RES_OK;
            }
		}
	}

	return l_eRes;
}

e_eCU_BUNSTF_RES eCU_BUNSTF_GetUnstufData(t_eCU_BUNSTF_Ctx* const p_ptCtx, uint8_t** p_ppuData, uint32_t* const p_puL)
{
	/* Local variable */
	e_eCU_BUNSTF_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ppuData ) || ( NULL == p_puL ) )
	{
		l_eRes = e_eCU_BUNSTF_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_BUNSTF_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_BUNSTF_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_BUNSTF_RES_CORRUPTCTX;
            }
            else
            {
                *p_ppuData = p_ptCtx->puBuff;
                *p_puL =  p_ptCtx->uFrameCtr;
                l_eRes = e_eCU_BUNSTF_RES_OK;
            }
		}
	}

	return l_eRes;
}

e_eCU_BUNSTF_RES eCU_BUNSTF_GetUnstufLen(t_eCU_BUNSTF_Ctx* const p_ptCtx, uint32_t* const p_puL)
{
	/* Local variable */
	e_eCU_BUNSTF_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puL ) )
	{
		l_eRes = e_eCU_BUNSTF_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_BUNSTF_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_BUNSTF_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_BUNSTF_RES_CORRUPTCTX;
            }
            else
            {
                *p_puL =  p_ptCtx->uFrameCtr;
                l_eRes = e_eCU_BUNSTF_RES_OK;
            }
		}
	}

	return l_eRes;
}

e_eCU_BUNSTF_RES eCU_BUNSTF_IsWaitingSof(const t_eCU_BUNSTF_Ctx* p_ptCtx, bool_t* const p_pbIsWaitSof)
{
	/* Local variable */
	e_eCU_BUNSTF_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsWaitSof ) )
	{
		l_eRes = e_eCU_BUNSTF_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_BUNSTF_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_BUNSTF_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_BUNSTF_RES_CORRUPTCTX;
            }
            else
            {
                if( e_eCU_BUNSTFPRV_SM_NEEDSOF == p_ptCtx->eSM )
                {
                    *p_pbIsWaitSof = true;
                }
                else
                {
                    *p_pbIsWaitSof = false;
                }

                l_eRes = e_eCU_BUNSTF_RES_OK;
            }
		}
	}

	return l_eRes;
}

e_eCU_BUNSTF_RES eCU_BUNSTF_IsAFullFrameUnstuff(const t_eCU_BUNSTF_Ctx* p_ptCtx, bool_t* const p_pbIsFrameUnstuff)
{
	/* Local variable */
	e_eCU_BUNSTF_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsFrameUnstuff ) )
	{
		l_eRes = e_eCU_BUNSTF_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_BUNSTF_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_BUNSTF_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_BUNSTF_RES_CORRUPTCTX;
            }
            else
            {
                if( e_eCU_BUNSTFPRV_SM_UNSTUFFEND == p_ptCtx->eSM )
                {
                    *p_pbIsFrameUnstuff = true;
                }
                else
                {
                    *p_pbIsFrameUnstuff = false;
                }
                l_eRes = e_eCU_BUNSTF_RES_OK;
            }
		}
	}

	return l_eRes;
}

e_eCU_BUNSTF_RES eCU_BUNSTF_IsFrameBad(const t_eCU_BUNSTF_Ctx* p_ptCtx, bool_t* const p_pbIsFrameBad)
{
	/* Local variable */
	e_eCU_BUNSTF_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsFrameBad ) )
	{
		l_eRes = e_eCU_BUNSTF_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_BUNSTF_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_BUNSTF_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_BUNSTF_RES_CORRUPTCTX;
            }
            else
            {
                if( e_eCU_BUNSTFPRV_SM_UNSTUFFFAIL == p_ptCtx->eSM )
                {
                    *p_pbIsFrameBad = true;
                }
                else
                {
                    *p_pbIsFrameBad = false;
                }
                l_eRes = e_eCU_BUNSTF_RES_OK;
            }
		}
	}

	return l_eRes;
}

e_eCU_BUNSTF_RES eCU_BUNSTF_InsStufChunk(t_eCU_BUNSTF_Ctx* const p_ptCtx, const uint8_t* p_puStuffBuf,
                                         const uint32_t p_uStuffBufL, uint32_t* const p_puConsumedBufL)
{
	/* Local variable */
	e_eCU_BUNSTF_RES l_eRes;
    uint32_t l_uNExamByte;
    uint8_t l_uCurByte;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puStuffBuf ) || ( NULL == p_puConsumedBufL ) )
	{
		l_eRes = e_eCU_BUNSTF_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_BUNSTF_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_BUNSTF_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_BUNSTF_RES_CORRUPTCTX;
            }
            else
            {
                /* Check param */
                if( p_uStuffBufL <= 0u )
                {
                    l_eRes = e_eCU_BUNSTF_RES_BADPARAM;
                }
                else
                {
                    /* Init counter */
                    l_uNExamByte = 0u;

                    /* Init l_eRes */
                    l_eRes = e_eCU_BUNSTF_RES_OK;

                    /* Elab all data */
                    while( ( l_uNExamByte < p_uStuffBufL ) && ( e_eCU_BUNSTF_RES_OK == l_eRes ) &&
					       ( e_eCU_BUNSTFPRV_SM_UNSTUFFEND != p_ptCtx->eSM ) &&
                           ( e_eCU_BUNSTFPRV_SM_UNSTUFFFAIL != p_ptCtx->eSM ) )
                    {
                        /* Read current byte */
                        l_uCurByte = p_puStuffBuf[l_uNExamByte];

                        /* Decide what to do */
						switch( p_ptCtx->eSM )
						{
							case e_eCU_BUNSTFPRV_SM_NEEDSOF:
							{
								/* Wait SOF, discharge others */
								if( ECU_SOF == l_uCurByte )
								{
									/* Found start */
									p_ptCtx->uFrameCtr = 0u;
									p_ptCtx->eSM = e_eCU_BUNSTFPRV_SM_NEEDRAWDATA;
								}
								else
								{
									/* Waiting for start, no other bytes */
                                    p_ptCtx->uFrameCtr = 0u;
                                    p_ptCtx->eSM = e_eCU_BUNSTFPRV_SM_UNSTUFFFAIL;
								}
								l_uNExamByte++;
								break;
							}

							case e_eCU_BUNSTFPRV_SM_NEEDRAWDATA:
							{
								if( ECU_SOF == l_uCurByte )
								{
									/* Found start, but wasn't expected */
									p_ptCtx->uFrameCtr = 0u;
                                    p_ptCtx->eSM = e_eCU_BUNSTFPRV_SM_NEEDRAWDATA;
                                    l_eRes = e_eCU_BUNSTF_RES_FRAMERESTART;
									l_uNExamByte++;
								}
								else if( ECU_EOF == l_uCurByte )
								{
									if( p_ptCtx->uFrameCtr <= 0u )
									{
										/* Found end, but no data received..  */
                                        p_ptCtx->eSM = e_eCU_BUNSTFPRV_SM_UNSTUFFFAIL;
									}
									else
									{
										/* Can close the frame, yey */
										p_ptCtx->eSM = e_eCU_BUNSTFPRV_SM_UNSTUFFEND;
									}

									l_uNExamByte++;
								}
								else if( ECU_ESC == l_uCurByte )
								{
									/* Next data will be negated data */
									p_ptCtx->eSM = e_eCU_BUNSTFPRV_SM_NEEDNEGATEDATA;
									l_uNExamByte++;
								}
								else
								{
									/* Received good raw data */
									if( p_ptCtx->uFrameCtr >= p_ptCtx->uBuffL )
									{
										/* No more data avaiable to save that thing */
										l_eRes = e_eCU_BUNSTF_RES_OUTOFMEM;
									}
									else
									{
										/* Only raw data */
										p_ptCtx->puBuff[p_ptCtx->uFrameCtr] = l_uCurByte;
										p_ptCtx->uFrameCtr++;
										l_uNExamByte++;
									}
								}
								break;
							}

							case e_eCU_BUNSTFPRV_SM_NEEDNEGATEDATA:
							{
								if( ECU_SOF == l_uCurByte )
								{
									/* Found start, but wasn't expected */
									p_ptCtx->uFrameCtr = 0u;
                                    p_ptCtx->eSM = e_eCU_BUNSTFPRV_SM_NEEDRAWDATA;
                                    l_eRes = e_eCU_BUNSTF_RES_FRAMERESTART;
									l_uNExamByte++;
								}
								else if( ( ECU_EOF == l_uCurByte ) ||
								         ( ECU_ESC == l_uCurByte ) )
								{
									/* Found and error, we were expecting raw negated data here.  */
                                    p_ptCtx->eSM = e_eCU_BUNSTFPRV_SM_UNSTUFFFAIL;
									l_uNExamByte++;
								}
								else
								{
									/* Received negated data */
									if( p_ptCtx->uFrameCtr >= p_ptCtx->uBuffL )
									{
										/* No more data avaiable to save that thing */
										l_eRes = e_eCU_BUNSTF_RES_OUTOFMEM;
									}
									else
									{
										/* Is it true that negate data is present ? */
										if( ( ECU_SOF == ( ( uint8_t ) ~l_uCurByte ) ) ||
											( ECU_EOF == ( ( uint8_t ) ~l_uCurByte ) ) ||
											( ECU_ESC == ( ( uint8_t ) ~l_uCurByte ) ) )
										{
											/* current data is neg */
											p_ptCtx->puBuff[p_ptCtx->uFrameCtr] = ( uint8_t ) ( ~l_uCurByte );
											p_ptCtx->eSM = e_eCU_BUNSTFPRV_SM_NEEDRAWDATA;
											p_ptCtx->uFrameCtr++;
											l_uNExamByte++;
										}
										else
										{
											/* Impossible receive a data after esc that is not SOF EOF or ESC neg */
                                            p_ptCtx->eSM = e_eCU_BUNSTFPRV_SM_UNSTUFFFAIL;
											l_uNExamByte++;
										}
									}
								}
								break;
							}

							default:
							{
								/* Impossible end here, and if so something horrible happened ( memory corruption ) */
								l_eRes = e_eCU_BUNSTF_RES_CORRUPTCTX;
								break;
							}
						}
                    }

					/* Save the l_eRes */
					*p_puConsumedBufL = l_uNExamByte;

					if( e_eCU_BUNSTF_RES_OK == l_eRes )
					{
						if( e_eCU_BUNSTFPRV_SM_UNSTUFFEND == p_ptCtx->eSM )
						{
							l_eRes = e_eCU_BUNSTF_RES_FRAMEENDED;
						}
                        else if( e_eCU_BUNSTFPRV_SM_UNSTUFFFAIL == p_ptCtx->eSM )
                        {
                           l_eRes = e_eCU_BUNSTF_RES_BADFRAME;
                        }
                        else
                        {
                            /* No error but have other data to unstuff */
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
static bool_t eCU_BUNSTF_IsStatusStillCoherent(const t_eCU_BUNSTF_Ctx* p_ptCtx)
{
    bool_t l_eRes;

	/* Check basic context validity */
	if( ( p_ptCtx->uBuffL <= 0u ) || ( NULL == p_ptCtx->puBuff ) )
	{
		l_eRes = false;
	}
	else
	{
        /* Check size validity */
        if( p_ptCtx->uFrameCtr > p_ptCtx->uBuffL )
        {
            l_eRes = false;
        }
        else
        {
            /* Check status coherence */
            if( ( e_eCU_BUNSTFPRV_SM_NEEDSOF == p_ptCtx->eSM ) && ( 0u != p_ptCtx->uFrameCtr ) )
            {
                l_eRes = false;
            }
            else
            {
                /* Check status coherence */
                if( ( e_eCU_BUNSTFPRV_SM_UNSTUFFEND == p_ptCtx->eSM ) && ( p_ptCtx->uFrameCtr <= 0u ) )
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

    return l_eRes;
}