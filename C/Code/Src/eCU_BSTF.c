/**
 * @file       eCU_BSTF.c
 *
 * @brief      Byte stuffer utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eCU_BSTF.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eCU_BSTF_IsStatusStillCoherent(const t_eCU_BSTF_Ctx* p_ptCtx);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eCU_BSTF_RES eCU_BSTF_InitCtx(t_eCU_BSTF_Ctx* const p_ptCtx, uint8_t* p_puBuff, const uint32_t p_uBuffL)
{
	/* Local variable */
	e_eCU_BSTF_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eCU_BSTF_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity */
        if( p_uBuffL <= 0u )
        {
            l_eRes = e_eCU_BSTF_RES_BADPARAM;
        }
        else
        {
            /* Initialize internal status */
            p_ptCtx->bIsInit = true;
            p_ptCtx->puBuff = p_puBuff;
            p_ptCtx->uBuffL = p_uBuffL;
            p_ptCtx->uFrameL = 0u;
            p_ptCtx->uFrameCtr = 0u;
            p_ptCtx->eSM = e_eCU_BSTFPRV_SM_NEEDSOF;

            l_eRes = e_eCU_BSTF_RES_OK;
        }
	}

	return l_eRes;
}

e_eCU_BSTF_RES eCU_BSTF_IsInit(t_eCU_BSTF_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eCU_BSTF_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eCU_BSTF_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInit;
        l_eRes = e_eCU_BSTF_RES_OK;
	}

	return l_eRes;
}

e_eCU_BSTF_RES eCU_BSTF_GetWherePutData(t_eCU_BSTF_Ctx* const p_ptCtx, uint8_t** p_ppuData, uint32_t* const p_puMaxL)
{
	/* Local variable */
	e_eCU_BSTF_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ppuData ) || ( NULL == p_puMaxL ) )
	{
		l_eRes = e_eCU_BSTF_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_BSTF_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_BSTF_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_BSTF_RES_CORRUPTCTX;
            }
            else
            {
                /* return data */
                *p_ppuData = p_ptCtx->puBuff;
                *p_puMaxL = p_ptCtx->uBuffL;
                l_eRes = e_eCU_BSTF_RES_OK;
            }
		}
	}

	return l_eRes;
}

e_eCU_BSTF_RES eCU_BSTF_NewFrame(t_eCU_BSTF_Ctx* const p_ptCtx, const uint32_t p_uFrameL)
{
	/* Local variable */
	e_eCU_BSTF_RES l_eRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eCU_BSTF_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_BSTF_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_BSTF_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_BSTF_RES_CORRUPTCTX;
            }
            else
            {
                /* Check param validity */
                if( ( p_uFrameL <= 0u ) || ( p_uFrameL > p_ptCtx->uBuffL ) )
                {
                    l_eRes = e_eCU_BSTF_RES_BADPARAM;
                }
                else
                {
                    /* Update data */
                    p_ptCtx->uFrameL = p_uFrameL;
                    p_ptCtx->uFrameCtr = 0u;
                    p_ptCtx->eSM = e_eCU_BSTFPRV_SM_NEEDSOF;

                    l_eRes = e_eCU_BSTF_RES_OK;
                }
            }
		}
	}

	return l_eRes;
}

e_eCU_BSTF_RES eCU_BSTF_RestartFrame(t_eCU_BSTF_Ctx* const p_ptCtx)
{
	/* Local variable */
	e_eCU_BSTF_RES l_eRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eCU_BSTF_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_BSTF_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_BSTF_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_BSTF_RES_CORRUPTCTX;
            }
            else
            {
                /* Param */
                if( p_ptCtx->uFrameL <= 0u )
                {
                    l_eRes = e_eCU_BSTF_RES_NOINITFRAME;
                }
                else
                {
                    /* Update index */
                    p_ptCtx->uFrameCtr = 0u;
                    p_ptCtx->eSM = e_eCU_BSTFPRV_SM_NEEDSOF;

                    l_eRes = e_eCU_BSTF_RES_OK;
                }
            }
		}
	}

	return l_eRes;
}

e_eCU_BSTF_RES eCU_BSTF_GetRemByteToGet(t_eCU_BSTF_Ctx* const p_ptCtx, uint32_t* const p_puGettedL)
{
	/* Local variable */
	e_eCU_BSTF_RES l_eRes;
    uint32_t l_uCalLen;
	uint32_t l_uIndx;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puGettedL ) )
	{
		l_eRes = e_eCU_BSTF_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_BSTF_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_BSTF_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_BSTF_RES_CORRUPTCTX;
            }
            else
            {
                /* Check param */
                if( p_ptCtx->uFrameL <= 0u )
                {
                    l_eRes = e_eCU_BSTF_RES_NOINITFRAME;
                }
                else
                {
                    /* Analyze the current state of the state machine */
                    if( e_eCU_BSTFPRV_SM_NEEDSOF == p_ptCtx->eSM )
                    {
                        /* SOF + Data + EOF */
                        l_uCalLen = 2u;
                    }
                    else if( e_eCU_BSTFPRV_SM_NEEDNEGATEPRECDATA == p_ptCtx->eSM )
                    {
                        /* If a precedent byte of the payload was an SOF, EOF or ESC character, this means that the
                        * ESC char is already inserted in the unstuffed data, but that we need to add the negation of
                        * the payload + the EOF */
                        l_uCalLen = 2u;
                    }
                    else if( e_eCU_BSTFPRV_SM_STUFFEND == p_ptCtx->eSM )
                    {
                        /* Stuffend ended, no data to wait */
                        l_uCalLen = 0u;
                    }
                    else
                    {
                        /* data + EOF. With the current state machine is not possible to have a single call that
                         * leave the system in the state: e_eCU_BSTFPRV_SM_NEEDEOF */
                        l_uCalLen = 1u;
                    }

                    /* Calculate the remaining byte from the current counter of course */
					l_uIndx = p_ptCtx->uFrameCtr;
                    while( ( l_uIndx < p_ptCtx->uFrameL ) && ( l_uCalLen < 0xFFFFFFFFu ) )
                    {
                        if( ECU_SOF == p_ptCtx->puBuff[l_uIndx] )
                        {
							/* Try to avoid overflow. Resonable limit for HW */
							if( l_uCalLen <= 0xFFFFFFFDu )
							{
								/* Stuff with escape */
								l_uCalLen += 2u;
							}
							else
							{
								l_uCalLen = 0xFFFFFFFFu;
							}
                        }
                        else if( ECU_EOF == p_ptCtx->puBuff[l_uIndx] )
                        {
							/* Try to avoid overflow. Resonable limit for HW */
							if( l_uCalLen <= 0xFFFFFFFDu )
							{
								/* Stuff with escape */
								l_uCalLen += 2u;
							}
							else
							{
								l_uCalLen = 0xFFFFFFFFu;
							}
                        }
                        else if( ECU_ESC == p_ptCtx->puBuff[l_uIndx] )
                        {
							/* Try to avoid overflow. Resonable limit for HW */
							if( l_uCalLen <= 0xFFFFFFFDu )
							{
								/* Stuff with escape */
								l_uCalLen += 2u;
							}
							else
							{
								l_uCalLen = 0xFFFFFFFFu;
							}
                        }
                        else
                        {
                            /* Stuff with escape */
                            l_uCalLen += 1u;
                        }

						l_uIndx++;
                    }

                    /* Copy calc value */
                    *p_puGettedL = l_uCalLen;

                    l_eRes = e_eCU_BSTF_RES_OK;
                }
            }
		}
	}

	return l_eRes;
}

e_eCU_BSTF_RES eCU_BSTF_GetStufChunk(t_eCU_BSTF_Ctx* const p_ptCtx, uint8_t* p_puStuffedBuf, const uint32_t p_uMaxBufL,
                                     uint32_t* const p_puGettedL)
{
	/* Local variable */
	e_eCU_BSTF_RES l_eRes;
    uint32_t l_uNFillB;
    uint8_t l_uPrecB;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puStuffedBuf ) || ( NULL == p_puGettedL ) )
	{
		l_eRes = e_eCU_BSTF_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_BSTF_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eCU_BSTF_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_BSTF_RES_CORRUPTCTX;
            }
            else
            {
                /* Check param */
                if( p_ptCtx->uFrameL <= 0u )
                {
                    l_eRes = e_eCU_BSTF_RES_NOINITFRAME;
                }
                else
                {
                    /* Check param */
                    if( p_uMaxBufL <= 0u )
                    {
                        l_eRes = e_eCU_BSTF_RES_BADPARAM;
                    }
                    else
                    {
                        /* Init counter */
                        l_uNFillB = 0u;
						l_eRes = e_eCU_BSTF_RES_OK;

                        /* Execute parsing cycle */
                        while( ( l_uNFillB < p_uMaxBufL ) &&
						       ( e_eCU_BSTFPRV_SM_STUFFEND != p_ptCtx->eSM ) &&
							   ( e_eCU_BSTF_RES_OK == l_eRes ) )
                        {
                            switch( p_ptCtx->eSM )
                            {
                                case e_eCU_BSTFPRV_SM_NEEDSOF :
                                {
                                    /* Start of frame */
                                    p_puStuffedBuf[l_uNFillB] = ECU_SOF;
                                    l_uNFillB++;
                                    p_ptCtx->eSM = e_eCU_BSTFPRV_SM_NEEDRAWDATA;

                                    break;
                                }

                                case e_eCU_BSTFPRV_SM_NEEDRAWDATA :
                                {
                                    if( p_ptCtx->uFrameCtr >= p_ptCtx->uFrameL )
                                    {
                                        /* End of frame needed */
                                        p_ptCtx->eSM = e_eCU_BSTFPRV_SM_NEEDEOF;
                                    }
                                    else
                                    {
                                        /* Parse data from the frame now */
                                        if( ECU_SOF == p_ptCtx->puBuff[p_ptCtx->uFrameCtr] )
                                        {
                                            /* Stuff with escape */
                                            p_puStuffedBuf[l_uNFillB] = ECU_ESC;
                                            p_ptCtx->eSM = e_eCU_BSTFPRV_SM_NEEDNEGATEPRECDATA;
                                            l_uNFillB++;
                                            p_ptCtx->uFrameCtr++;
                                        }
                                        else if( ECU_EOF == p_ptCtx->puBuff[p_ptCtx->uFrameCtr] )
                                        {
                                            /* Stuff with escape */
                                            p_puStuffedBuf[l_uNFillB] = ECU_ESC;
                                            p_ptCtx->eSM = e_eCU_BSTFPRV_SM_NEEDNEGATEPRECDATA;
                                            l_uNFillB++;
                                            p_ptCtx->uFrameCtr++;
                                        }
                                        else if( ECU_ESC == p_ptCtx->puBuff[p_ptCtx->uFrameCtr] )
                                        {
                                            /* Stuff with escape */
                                            p_puStuffedBuf[l_uNFillB] = ECU_ESC;
                                            p_ptCtx->eSM = e_eCU_BSTFPRV_SM_NEEDNEGATEPRECDATA;
                                            l_uNFillB++;
                                            p_ptCtx->uFrameCtr++;
                                        }
                                        else
                                        {
                                            /* Can insert data and continue parsing other raw data */
                                            p_puStuffedBuf[l_uNFillB] = p_ptCtx->puBuff[p_ptCtx->uFrameCtr];
                                            l_uNFillB++;
                                            p_ptCtx->uFrameCtr++;
                                        }
                                    }

                                    break;
                                }

                                case e_eCU_BSTFPRV_SM_NEEDNEGATEPRECDATA :
                                {
                                    /* Something from an old iteration  */
                                    l_uPrecB = p_ptCtx->puBuff[p_ptCtx->uFrameCtr - 1u];
                                    p_puStuffedBuf[l_uNFillB] = ( (uint8_t) ~( l_uPrecB ) );
                                    l_uNFillB++;

                                    /* After this we can continue parsing raw data */
                                    p_ptCtx->eSM = e_eCU_BSTFPRV_SM_NEEDRAWDATA;

                                    break;
                                }

                                case e_eCU_BSTFPRV_SM_NEEDEOF :
                                {
                                    /* End of frame */
                                    p_puStuffedBuf[l_uNFillB] = ECU_EOF;
                                    p_ptCtx->eSM = e_eCU_BSTFPRV_SM_STUFFEND;
                                    l_uNFillB++;

                                    break;
                                }

                               default:
                               {
                                   /* Impossible end here, and if so something horrible happened (memory corruption) */
									l_eRes = e_eCU_BSTF_RES_CORRUPTCTX;
                                   break;
                               }
                            }
                        }

						/* Save counter */
						*p_puGettedL = l_uNFillB;

						if( e_eCU_BSTF_RES_OK == l_eRes )
						{
							/* l_eRes? */
							if( e_eCU_BSTFPRV_SM_STUFFEND == p_ptCtx->eSM )
							{
								/* Nothing more */
								l_eRes = e_eCU_BSTF_RES_FRAMEENDED;
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
static bool_t eCU_BSTF_IsStatusStillCoherent(const t_eCU_BSTF_Ctx* p_ptCtx)
{
    bool_t l_eRes;
    uint8_t l_uPrecB;

	/* Check basic context validity */
	if( ( p_ptCtx->uBuffL <= 0u ) || ( NULL == p_ptCtx->puBuff ) )
	{
		l_eRes = false;
	}
	else
	{
        /* Check context limit validity */
        if( ( p_ptCtx->uFrameL > p_ptCtx->uBuffL ) || ( p_ptCtx->uFrameCtr > p_ptCtx->uFrameL ) )
        {
            l_eRes = false;
        }
        else
        {
            /* Check data coherence on SOF */
            if( ( e_eCU_BSTFPRV_SM_NEEDSOF  == p_ptCtx->eSM ) && ( 0u != p_ptCtx->uFrameCtr ) )
            {
                l_eRes = false;
            }
            else
            {
                /* Check data coherence on SOF */
                if( ( 0u == p_ptCtx->uFrameCtr ) && ( e_eCU_BSTFPRV_SM_NEEDSOF != p_ptCtx->eSM ) &&
                    ( e_eCU_BSTFPRV_SM_NEEDRAWDATA != p_ptCtx->eSM ) )
                {
                    l_eRes = false;
                }
                else
                {
                    /* Check data coherence on EOF */
                    if( ( e_eCU_BSTFPRV_SM_STUFFEND == p_ptCtx->eSM ) && ( p_ptCtx->uFrameCtr != p_ptCtx->uFrameL ) )
                    {
                        l_eRes = false;
                    }
                    else
                    {
                        /* Check data coherence on precedent data */
                        if( e_eCU_BSTFPRV_SM_NEEDNEGATEPRECDATA == p_ptCtx->eSM )
                        {
                            l_uPrecB = p_ptCtx->puBuff[p_ptCtx->uFrameCtr - 1u];
                            if( ( ECU_ESC != l_uPrecB ) && ( ECU_EOF != l_uPrecB ) && ( ECU_SOF != l_uPrecB ) )
                            {
                                l_eRes = false;
                            }
                            else
                            {
                                l_eRes = true;
                            }
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