/**
 * @file       eFSS_LLUTILSPRV.c
 *
 * @brief      Low level utils for fail safe storage
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_LLUTILSPRV.h"



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_LLUTILSPRV_RES eFSS_LLUTILSPRV_ErasePage( t_eFSS_TYPE_CbCtx* const p_ptCtx, const uint32_t p_uPageIndx,
                                                  const uint32_t p_uReTry)
{
	/* Local variable */
	e_eFSS_LLUTILSPRV_RES l_eRes;
    uint32_t l_uTryPerformed;
    bool_t l_bCbRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LLUTILSPRV_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_ptCtx->ptCtxErase ) || ( NULL == p_ptCtx->fErase ) ||
            ( NULL == p_ptCtx->ptCtxWrite ) || ( NULL == p_ptCtx->fWrite ) ||
            ( NULL == p_ptCtx->ptCtxRead  ) || ( NULL == p_ptCtx->fRead ) ||
            ( NULL == p_ptCtx->ptCtxCrc32 ) || ( NULL == p_ptCtx->fCrc32 ) )
        {
            l_eRes = e_eFSS_LLUTILSPRV_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( p_uReTry <= 0u )
            {
                l_eRes = e_eFSS_LLUTILSPRV_RES_BADPARAM;
            }
            else
            {
                /* Initialize internal status */
                l_uTryPerformed = 0u;
                l_bCbRes = false;

                /* Do operation */
                while( ( false == l_bCbRes ) && ( l_uTryPerformed < p_uReTry ) )
                {
                    l_bCbRes = (*(p_ptCtx->fErase))(p_ptCtx->ptCtxErase, p_uPageIndx);
                    l_uTryPerformed++;
                }

                if( false == l_bCbRes )
                {
                    l_eRes = e_eFSS_LLUTILSPRV_RES_CLBCKREPORTERROR;
                }
                else
                {
                    l_eRes = e_eFSS_LLUTILSPRV_RES_OK;
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_LLUTILSPRV_RES eFSS_LLUTILSPRV_WritePage( t_eFSS_TYPE_CbCtx* const p_ptCtx,
                                                 uint8_t* const p_puDataW, const uint32_t p_uDataWriteLen,
                                                 uint8_t* const p_puDataR, const uint32_t p_uDataReadLen,
                                                 const uint32_t p_uPageIndx, const uint32_t p_uReTry)
{
	/* Local variable */
	e_eFSS_LLUTILSPRV_RES l_eRes;
    uint32_t l_uTryPerformed;
    bool_t l_bCbRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puDataW ) || ( NULL == p_puDataR ) )
	{
		l_eRes = e_eFSS_LLUTILSPRV_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_ptCtx->ptCtxErase ) || ( NULL == p_ptCtx->fErase ) ||
            ( NULL == p_ptCtx->ptCtxWrite ) || ( NULL == p_ptCtx->fWrite ) ||
            ( NULL == p_ptCtx->ptCtxRead  ) || ( NULL == p_ptCtx->fRead ) ||
            ( NULL == p_ptCtx->ptCtxCrc32 ) || ( NULL == p_ptCtx->fCrc32 ) )
        {
            l_eRes = e_eFSS_LLUTILSPRV_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( ( p_uReTry <= 0u ) || ( p_uDataWriteLen <= 0u ) || ( p_uDataReadLen <= 0u ) )
            {
                l_eRes = e_eFSS_LLUTILSPRV_RES_BADPARAM;
            }
            else
            {
                /* Initialize internal status */
                l_uTryPerformed = 0u;
                l_bCbRes = false;

                /* Do operation */
                while( ( false == l_bCbRes ) && ( l_uTryPerformed < p_uReTry ) )
                {
                    /* Erase */
                    l_bCbRes = (*(p_ptCtx->fErase))(p_ptCtx->ptCtxErase, p_uPageIndx);

                    if( true == l_bCbRes )
                    {
                        /* Write */
                        l_bCbRes = (*(p_ptCtx->fWrite))(p_ptCtx->ptCtxWrite, p_uPageIndx, p_puDataW, p_uDataWriteLen);
                    }

                    if( true == l_bCbRes )
                    {
                        /* Read */
                        l_bCbRes = (*(p_ptCtx->fRead))(p_ptCtx->ptCtxRead, p_uPageIndx, p_puDataR, p_uDataReadLen);
                    }

                    if( true == l_bCbRes )
                    {
                        /* Compare */
                        if( 0 == memcmp(p_puDataW, p_puDataR, p_uDataWriteLen) )
                        {
                            l_bCbRes = true;
                        }
                        else
                        {
                            l_bCbRes = false;
                        }
                    }

                    l_uTryPerformed++;
                }

                if( false == l_bCbRes )
                {
                    l_eRes = e_eFSS_LLUTILSPRV_RES_CLBCKREPORTERROR;
                }
                else
                {
                    l_eRes = e_eFSS_LLUTILSPRV_RES_OK;
                }
            }
        }
	}

	return l_eRes;
}


e_eFSS_LLUTILSPRV_RES eFSS_LLUTILSPRV_ReadPage( t_eFSS_TYPE_CbCtx* const p_ptCtx, const uint32_t p_uPageIndx,
                                                uint8_t* const p_puDataR, const uint32_t p_uDataReadLen,
                                                const uint32_t p_uReTry )
{
	/* Local variable */
	e_eFSS_LLUTILSPRV_RES l_eRes;
    uint32_t l_uTryPerformed;
    bool_t l_bCbRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puDataR ) )
	{
		l_eRes = e_eFSS_LLUTILSPRV_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_ptCtx->ptCtxErase ) || ( NULL == p_ptCtx->fErase ) ||
            ( NULL == p_ptCtx->ptCtxWrite ) || ( NULL == p_ptCtx->fWrite ) ||
            ( NULL == p_ptCtx->ptCtxRead  ) || ( NULL == p_ptCtx->fRead ) ||
            ( NULL == p_ptCtx->ptCtxCrc32 ) || ( NULL == p_ptCtx->fCrc32 ) )
        {
            l_eRes = e_eFSS_LLUTILSPRV_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( ( p_uReTry <= 0u ) || ( p_uDataReadLen <= 0u ) )
            {
                l_eRes = e_eFSS_LLUTILSPRV_RES_BADPARAM;
            }
            else
            {
                /* Initialize internal status */
                l_uTryPerformed = 0u;
                l_bCbRes = false;

                /* Do operation */
                while( ( false == l_bCbRes ) && ( l_uTryPerformed < p_uReTry ) )
                {
                    /* Read */
                    l_bCbRes = (*(p_ptCtx->fRead))(p_ptCtx->ptCtxRead, p_uPageIndx, p_puDataR, p_uDataReadLen);

                    l_uTryPerformed++;
                }

                if( false == l_bCbRes )
                {
                    l_eRes = e_eFSS_LLUTILSPRV_RES_CLBCKREPORTERROR;
                }
                else
                {
                    l_eRes = e_eFSS_LLUTILSPRV_RES_OK;
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_LLUTILSPRV_RES eFSS_LLUTILSPRV_CalcCrc32( t_eFSS_TYPE_CbCtx* const p_ptCtx,
                                                 uint8_t* const p_puData, const uint32_t p_uDataLen,
                                                 uint32_t* const p_puCrcCalculated )
{
	/* Local variable */
	e_eFSS_LLUTILSPRV_RES l_eRes;
    bool_t l_bCbRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puData ) || ( NULL == p_puCrcCalculated ) )
	{
		l_eRes = e_eFSS_LLUTILSPRV_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_ptCtx->ptCtxErase ) || ( NULL == p_ptCtx->fErase ) ||
            ( NULL == p_ptCtx->ptCtxWrite ) || ( NULL == p_ptCtx->fWrite ) ||
            ( NULL == p_ptCtx->ptCtxRead  ) || ( NULL == p_ptCtx->fRead ) ||
            ( NULL == p_ptCtx->ptCtxCrc32 ) || ( NULL == p_ptCtx->fCrc32 ) )
        {
            l_eRes = e_eFSS_LLUTILSPRV_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( p_uDataLen <= 0u )
            {
                l_eRes = e_eFSS_LLUTILSPRV_RES_BADPARAM;
            }
            else
            {
                /* Initialize internal status */
                l_bCbRes = false;

                /* Read */
                l_bCbRes = (*(p_ptCtx->fCrc32))(p_ptCtx->ptCtxCrc32, MAX_UINT32VAL, p_puData, p_uDataLen,
                                                p_puCrcCalculated);

                if( false == l_bCbRes )
                {
                    l_eRes = e_eFSS_LLUTILSPRV_RES_CLBCKREPORTERROR;
                }
                else
                {
                    l_eRes = e_eFSS_LLUTILSPRV_RES_OK;
                }
            }
        }
	}

	return l_eRes;
}


