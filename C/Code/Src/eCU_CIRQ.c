/**
 * @file       eCU_CIRQ.c
 *
 * @brief      Circular Queue utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eCU_CIRQ.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eCU_CIRQ_IsStatusStillCoherent(const t_eCU_CIRQ_Ctx* p_ptCtx);
static uint32_t eCU_CIRQ_GetoccupiedIndex(const t_eCU_CIRQ_Ctx* p_ptCtx);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eCU_CIRQ_RES eCU_CIRQ_InitCtx(t_eCU_CIRQ_Ctx* const p_ptCtx, uint8_t* p_puBuff, const uint32_t p_uBuffL)
{
	/* Local variable */
	e_eCU_CIRQ_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL ==  p_puBuff ) )
	{
		l_eRes = e_eCU_CIRQ_RES_BADPOINTER;
	}
	else
	{
		/* Check data validity */
		if( p_uBuffL <= 0u )
		{
			l_eRes = e_eCU_CIRQ_RES_BADPARAM;
		}
		else
		{
			/* Check Init */
			p_ptCtx->bIsInit = true;
			p_ptCtx->puBuff = p_puBuff;
			p_ptCtx->uBuffL = p_uBuffL;
			p_ptCtx->uBuffUsedL = 0u;
			p_ptCtx->uBuffFreeIdx = 0u;

			l_eRes = e_eCU_CIRQ_RES_OK;
		}
    }

	return l_eRes;
}

e_eCU_CIRQ_RES eCU_CIRQ_IsInit(t_eCU_CIRQ_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eCU_CIRQ_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eCU_CIRQ_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInit;
        l_eRes = e_eCU_CIRQ_RES_OK;
	}

	return l_eRes;
}

e_eCU_CIRQ_RES eCU_CIRQ_Reset(t_eCU_CIRQ_Ctx* const p_ptCtx)
{
	/* Local variable */
	e_eCU_CIRQ_RES l_eRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eCU_CIRQ_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_CIRQ_RES_NOINITLIB;
		}
		else
		{
            /* Check data coherence */
            if( false == eCU_CIRQ_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_CIRQ_RES_CORRUPTCTX;
            }
            else
            {
                /* Update index in order to discharge all saved data */
                p_ptCtx->uBuffUsedL = 0u;
                p_ptCtx->uBuffFreeIdx = 0u;

                l_eRes = e_eCU_CIRQ_RES_OK;
            }
		}
    }

	return l_eRes;
}

e_eCU_CIRQ_RES eCU_CIRQ_GetFreeSapce(t_eCU_CIRQ_Ctx* const p_ptCtx, uint32_t* const p_puFreeSpace)
{
	/* Local variable */
	e_eCU_CIRQ_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puFreeSpace ) )
	{
		l_eRes = e_eCU_CIRQ_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_CIRQ_RES_NOINITLIB;
		}
		else
		{
            /* Check data coherence */
            if( false == eCU_CIRQ_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_CIRQ_RES_CORRUPTCTX;
            }
			else
			{
				*p_puFreeSpace = p_ptCtx->uBuffL - p_ptCtx->uBuffUsedL;
				l_eRes = e_eCU_CIRQ_RES_OK;
			}
		}
    }

	return l_eRes;
}

e_eCU_CIRQ_RES eCU_CIRQ_GetOccupiedSapce(t_eCU_CIRQ_Ctx* const p_ptCtx, uint32_t* const p_puUsedSpace)
{
	/* Local variable */
	e_eCU_CIRQ_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puUsedSpace ) )
	{
		l_eRes = e_eCU_CIRQ_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_CIRQ_RES_NOINITLIB;
		}
		else
		{
            /* Check data coherence */
            if( false == eCU_CIRQ_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_CIRQ_RES_CORRUPTCTX;
            }
			else
			{
				*p_puUsedSpace = p_ptCtx->uBuffUsedL;
				l_eRes = e_eCU_CIRQ_RES_OK;
			}
		}
    }

	return l_eRes;
}

e_eCU_CIRQ_RES eCU_CIRQ_InsertData(t_eCU_CIRQ_Ctx* const p_ptCtx, const uint8_t* p_puData, const uint32_t p_uDataL)
{
	/* Local variable */
	e_eCU_CIRQ_RES l_eRes;
	uint32_t l_uFreeSpace;
	uint32_t l_uFirstLen;
	uint32_t l_uSecondLen;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puData ) )
	{
		l_eRes = e_eCU_CIRQ_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_CIRQ_RES_NOINITLIB;
		}
		else
		{
            /* Check data coherence */
            if( false == eCU_CIRQ_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_CIRQ_RES_CORRUPTCTX;
            }
			else
			{
                /* Check data validity */
                if( p_uDataL <= 0u )
                {
                    l_eRes = e_eCU_CIRQ_RES_BADPARAM;
                }
                else
                {
                    l_uFreeSpace = p_ptCtx->uBuffL - p_ptCtx->uBuffUsedL;
                    if( p_uDataL > l_uFreeSpace )
                    {
                        /* No memory avaiable */
                        l_eRes = e_eCU_CIRQ_RES_FULL;
                    }
                    else
                    {
                        /* Can insert data */
                        if( ( p_uDataL + p_ptCtx->uBuffFreeIdx ) <= p_ptCtx->uBuffL )
                        {
                            /* Direct copy */
                            (void)memcpy(&p_ptCtx->puBuff[p_ptCtx->uBuffFreeIdx], p_puData, p_uDataL);

                            /* Update free index */
                            p_ptCtx->uBuffFreeIdx += p_uDataL;
                            if( p_ptCtx->uBuffFreeIdx >= p_ptCtx->uBuffL )
                            {
                                p_ptCtx->uBuffFreeIdx = 0u;
                            }
                        }
                        else
                        {
                            /* Multicopy */

                            /* First round */
                            l_uFirstLen = p_ptCtx->uBuffL - p_ptCtx->uBuffFreeIdx;
                            (void)memcpy(&p_ptCtx->puBuff[p_ptCtx->uBuffFreeIdx], p_puData, l_uFirstLen);
                            p_ptCtx->uBuffFreeIdx = 0u;

                            /* Second round */
                            l_uSecondLen = p_uDataL - l_uFirstLen;
                            (void)memcpy(&p_ptCtx->puBuff[p_ptCtx->uBuffFreeIdx], &p_puData[l_uFirstLen], l_uSecondLen);
                            p_ptCtx->uBuffFreeIdx += l_uSecondLen;
                        }

                        p_ptCtx->uBuffUsedL += p_uDataL;
                        l_eRes = e_eCU_CIRQ_RES_OK;
                    }
                }
			}
		}
    }

	return l_eRes;
}

e_eCU_CIRQ_RES eCU_CIRQ_RetriveData(t_eCU_CIRQ_Ctx* const p_ptCtx, uint8_t* p_puData, const uint32_t p_uDataL)
{
	/* Local variable */
	e_eCU_CIRQ_RES l_eRes;
	uint32_t l_uFirstLen;
	uint32_t l_uSecondLen;
    uint32_t l_uMemPOccIdx;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puData ) )
	{
		l_eRes = e_eCU_CIRQ_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_CIRQ_RES_NOINITLIB;
		}
		else
		{
            /* Check data coherence */
            if( false == eCU_CIRQ_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_CIRQ_RES_CORRUPTCTX;
            }
			else
			{
                /* Check data validity an queue integrity */
                if( p_uDataL <= 0u )
                {
                    l_eRes = e_eCU_CIRQ_RES_BADPARAM;
                }
                else
                {
                    if( p_uDataL > p_ptCtx->uBuffUsedL )
                    {
                        /* No enoght data in the queue */
                        l_eRes = e_eCU_CIRQ_RES_EMPTY;
                    }
                    else
                    {
                        /* Retrive occupied index */
                        l_uMemPOccIdx = eCU_CIRQ_GetoccupiedIndex(p_ptCtx);

                        /* Can retrive data */
                        if( ( p_uDataL +  l_uMemPOccIdx ) <= p_ptCtx->uBuffL )
                        {
                            /* Direct copy */
                            (void)memcpy(p_puData, &p_ptCtx->puBuff[l_uMemPOccIdx], p_uDataL);
                        }
                        else
                        {
                            /* Multicopy */

                            /* First round */
                            l_uFirstLen = p_ptCtx->uBuffL - l_uMemPOccIdx;
                            (void)memcpy(p_puData, &p_ptCtx->puBuff[l_uMemPOccIdx], l_uFirstLen);
                            l_uMemPOccIdx = 0u;

                            /* Second round */
                            l_uSecondLen = p_uDataL - l_uFirstLen;
                            (void)memcpy(&p_puData[l_uFirstLen], &p_ptCtx->puBuff[l_uMemPOccIdx], l_uSecondLen);
                        }

                        p_ptCtx->uBuffUsedL -= p_uDataL;
                        l_eRes = e_eCU_CIRQ_RES_OK;
                    }
                }
			}
		}
    }

	return l_eRes;
}

e_eCU_CIRQ_RES eCU_CIRQ_PeekData(t_eCU_CIRQ_Ctx* const p_ptCtx, uint8_t* p_puData, const uint32_t p_uDataL)
{
	/* Local variable */
	e_eCU_CIRQ_RES l_eRes;
	uint32_t l_uFirstLen;
	uint32_t l_uSecondLen;
    uint32_t l_uMemPOccIdx;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puData ) )
	{
		l_eRes = e_eCU_CIRQ_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eCU_CIRQ_RES_NOINITLIB;
		}
		else
		{
            /* Check data coherence */
            if( false == eCU_CIRQ_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eCU_CIRQ_RES_CORRUPTCTX;
            }
			else
			{
                /* Check data validity an queue integrity */
                if( p_uDataL <= 0u )
                {
                    l_eRes = e_eCU_CIRQ_RES_BADPARAM;
                }
                else
                {
                    if( p_uDataL > p_ptCtx->uBuffUsedL )
                    {
                        /* No enoght data in the queue */
                        l_eRes = e_eCU_CIRQ_RES_EMPTY;
                    }
                    else
                    {
                        /* Retrive occupied index */
                        l_uMemPOccIdx = eCU_CIRQ_GetoccupiedIndex(p_ptCtx);

                        /* Can retrive data */
                        if( ( p_uDataL +  l_uMemPOccIdx ) <= p_ptCtx->uBuffL )
                        {
                            /* Direct copy */
                            (void)memcpy(p_puData, &p_ptCtx->puBuff[l_uMemPOccIdx], p_uDataL);
                        }
                        else
                        {
                            /* Multicopy */

                            /* First round */
                            l_uFirstLen = p_ptCtx->uBuffL - l_uMemPOccIdx;
                            (void)memcpy(p_puData, &p_ptCtx->puBuff[l_uMemPOccIdx], l_uFirstLen);
                            l_uMemPOccIdx = 0u;

                            /* Second round */
                            l_uSecondLen = p_uDataL - l_uFirstLen;
                            (void)memcpy(&p_puData[l_uFirstLen], &p_ptCtx->puBuff[l_uMemPOccIdx], l_uSecondLen);
                        }

                        l_eRes = e_eCU_CIRQ_RES_OK;
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
static bool_t eCU_CIRQ_IsStatusStillCoherent(const t_eCU_CIRQ_Ctx* p_ptCtx)
{
    bool_t l_eRes;

	/* Check context validity */
	if( ( p_ptCtx->uBuffL <= 0u ) || ( NULL == p_ptCtx->puBuff ) )
	{
		l_eRes = false;
	}
	else
	{
		/* we cannot have more data than data size */
		if( p_ptCtx->uBuffUsedL > p_ptCtx->uBuffL )
		{
			l_eRes = false;
		}
		else
		{
            /* Cannot go beyond limits */
            if( p_ptCtx->uBuffFreeIdx >= p_ptCtx->uBuffL )
            {
                l_eRes = false;
            }
            else
            {
                l_eRes = true;
            }
		}
	}

    return l_eRes;
}

static uint32_t eCU_CIRQ_GetoccupiedIndex(const t_eCU_CIRQ_Ctx* p_ptCtx)
{
    uint32_t l_uOccIndx;

    if( p_ptCtx->uBuffUsedL <= p_ptCtx->uBuffFreeIdx )
    {
        l_uOccIndx = p_ptCtx->uBuffFreeIdx - p_ptCtx->uBuffUsedL;
    }
    else
    {
        l_uOccIndx = p_ptCtx->uBuffL - ( p_ptCtx->uBuffUsedL - p_ptCtx->uBuffFreeIdx );
    }

    return l_uOccIndx;
}