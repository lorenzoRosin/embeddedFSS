/**
 * @file       eFSS_UTILSLLPRV.c
 *
 * @brief      Low level utils for fail safe storage
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_UTILSLLPRV.h"



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_UTILSLLPRV_RES eFSS_UTILSLLPRV_ErasePage( t_eFSS_TYPE_CbCtx* const p_ptCbCtx, const uint32_t p_uPageIndx,
                                                 const uint32_t p_uReTry)
{
	/* Local variable */
	e_eFSS_UTILSLLPRV_RES l_eRes;
    uint32_t l_uTryPerformed;
    bool_t l_bCbRes;

	/* Check pointer validity */
	if( NULL == p_ptCbCtx )
	{
		l_eRes = e_eFSS_UTILSLLPRV_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_ptCbCtx->ptCtxErase ) || ( NULL == p_ptCbCtx->fErase ) ||
            ( NULL == p_ptCbCtx->ptCtxWrite ) || ( NULL == p_ptCbCtx->fWrite ) ||
            ( NULL == p_ptCbCtx->ptCtxRead  ) || ( NULL == p_ptCbCtx->fRead  ) ||
            ( NULL == p_ptCbCtx->ptCtxCrc32 ) || ( NULL == p_ptCbCtx->fCrc32 ) )
        {
            l_eRes = e_eFSS_UTILSLLPRV_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( p_uReTry <= 0u )
            {
                l_eRes = e_eFSS_UTILSLLPRV_RES_BADPARAM;
            }
            else
            {
                /* Initialize internal status */
                l_eRes = e_eFSS_UTILSLLPRV_RES_CLBCKERASEERR;
                l_uTryPerformed = 0u;
                l_bCbRes = false;

                /* Do operation */
                while( ( false == l_bCbRes ) && ( l_uTryPerformed < p_uReTry ) )
                {
                    l_bCbRes = (*(p_ptCbCtx->fErase))(p_ptCbCtx->ptCtxErase, p_uPageIndx);
                    l_uTryPerformed++;

                    if( false == l_bCbRes )
                    {
                        l_eRes = e_eFSS_UTILSLLPRV_RES_CLBCKERASEERR;
                    }
                    else
                    {
                        l_eRes = e_eFSS_UTILSLLPRV_RES_OK;
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_UTILSLLPRV_RES eFSS_UTILSLLPRV_WritePage( t_eFSS_TYPE_CbCtx* const p_ptCbCtx,
                                                 uint8_t* const p_puDataW, const uint32_t p_uDataWLen,
                                                 uint8_t* const p_puDataR, const uint32_t p_uDataRLen,
                                                 const uint32_t p_uPageIndx, const uint32_t p_uReTry)
{
	/* Local variable */
	e_eFSS_UTILSLLPRV_RES l_eRes;
    uint32_t l_uTryPerformed;
    bool_t l_bCbRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCbCtx ) || ( NULL == p_puDataW ) || ( NULL == p_puDataR ) )
	{
		l_eRes = e_eFSS_UTILSLLPRV_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_ptCbCtx->ptCtxErase ) || ( NULL == p_ptCbCtx->fErase ) ||
            ( NULL == p_ptCbCtx->ptCtxWrite ) || ( NULL == p_ptCbCtx->fWrite ) ||
            ( NULL == p_ptCbCtx->ptCtxRead  ) || ( NULL == p_ptCbCtx->fRead  ) ||
            ( NULL == p_ptCbCtx->ptCtxCrc32 ) || ( NULL == p_ptCbCtx->fCrc32 ) )
        {
            l_eRes = e_eFSS_UTILSLLPRV_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( ( p_uReTry <= 0u ) || ( p_uDataWLen <= 0u ) || ( p_uDataRLen <= 0u ) ||
                ( p_uDataWLen != p_uDataRLen ) )
            {
                l_eRes = e_eFSS_UTILSLLPRV_RES_BADPARAM;
            }
            else
            {
                /* Initialize internal status */
                l_eRes = e_eFSS_UTILSLLPRV_RES_CLBCKWRITEERR;
                l_uTryPerformed = 0u;
                l_bCbRes = false;

                /* Do operation */
                while( ( false == l_bCbRes ) && ( l_uTryPerformed < p_uReTry ) )
                {
                    /* Erase */
                    l_bCbRes = (*(p_ptCbCtx->fErase))(p_ptCbCtx->ptCtxErase, p_uPageIndx);

                    if( true == l_bCbRes )
                    {
                        /* Write */
                        l_bCbRes = (*(p_ptCbCtx->fWrite))(p_ptCbCtx->ptCtxWrite, p_uPageIndx, p_puDataW, p_uDataWLen);

                        if( false == l_bCbRes )
                        {
                            /* Write error  */
                            l_eRes = e_eFSS_UTILSLLPRV_RES_CLBCKWRITEERR;
                        }
                    }
                    else
                    {
                        l_eRes = e_eFSS_UTILSLLPRV_RES_CLBCKERASEERR;
                    }

                    if( true == l_bCbRes )
                    {
                        /* Read */
                        l_bCbRes = (*(p_ptCbCtx->fRead))(p_ptCbCtx->ptCtxRead, p_uPageIndx, p_puDataR, p_uDataRLen);

                        if( false == l_bCbRes )
                        {
                            /* Write error  */
                            l_eRes = e_eFSS_UTILSLLPRV_RES_CLBCKREADERR;
                        }
                    }

                    if( true == l_bCbRes )
                    {
                        /* Compare */
                        if( 0 == memcmp(p_puDataW, p_puDataR, p_uDataWLen) )
                        {
                            l_bCbRes = true;
                        }
                        else
                        {
                            l_bCbRes = false;
                            l_eRes = e_eFSS_UTILSLLPRV_RES_WRITENOMATCHREAD;
                        }
                    }

                    l_uTryPerformed++;
                }

                if( true == l_bCbRes )
                {
                    l_eRes = e_eFSS_UTILSLLPRV_RES_OK;
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_UTILSLLPRV_RES eFSS_UTILSLLPRV_ReadPage( t_eFSS_TYPE_CbCtx* const p_ptCbCtx, const uint32_t p_uPageIndx,
                                                uint8_t* const p_puDataR, const uint32_t p_uDataRLen,
                                                const uint32_t p_uReTry )
{
	/* Local variable */
	e_eFSS_UTILSLLPRV_RES l_eRes;
    uint32_t l_uTryPerformed;
    bool_t l_bCbRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCbCtx ) || ( NULL == p_puDataR ) )
	{
		l_eRes = e_eFSS_UTILSLLPRV_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_ptCbCtx->ptCtxErase ) || ( NULL == p_ptCbCtx->fErase ) ||
            ( NULL == p_ptCbCtx->ptCtxWrite ) || ( NULL == p_ptCbCtx->fWrite ) ||
            ( NULL == p_ptCbCtx->ptCtxRead  ) || ( NULL == p_ptCbCtx->fRead  ) ||
            ( NULL == p_ptCbCtx->ptCtxCrc32 ) || ( NULL == p_ptCbCtx->fCrc32 ) )
        {
            l_eRes = e_eFSS_UTILSLLPRV_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( ( p_uReTry <= 0u ) || ( p_uDataRLen <= 0u ) )
            {
                l_eRes = e_eFSS_UTILSLLPRV_RES_BADPARAM;
            }
            else
            {
                /* Initialize internal status */
                l_eRes = e_eFSS_UTILSLLPRV_RES_CLBCKREADERR;
                l_uTryPerformed = 0u;
                l_bCbRes = false;

                /* Do operation */
                while( ( false == l_bCbRes ) && ( l_uTryPerformed < p_uReTry ) )
                {
                    /* Read */
                    l_bCbRes = (*(p_ptCbCtx->fRead))(p_ptCbCtx->ptCtxRead, p_uPageIndx, p_puDataR, p_uDataRLen);
                    l_uTryPerformed++;

                    if( false == l_bCbRes )
                    {
                        l_eRes = e_eFSS_UTILSLLPRV_RES_CLBCKREADERR;
                    }
                    else
                    {
                        l_eRes = e_eFSS_UTILSLLPRV_RES_OK;
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_UTILSLLPRV_RES eFSS_UTILSLLPRV_CalcCrc32( t_eFSS_TYPE_CbCtx* const p_ptCbCtx,
                                                 uint8_t* const p_puData, const uint32_t p_uDataLen,
                                                 uint32_t* const p_puCrcCal )
{
	/* Local variable */
	e_eFSS_UTILSLLPRV_RES l_eRes;
    bool_t l_bCbRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCbCtx ) || ( NULL == p_puData ) || ( NULL == p_puCrcCal ) )
	{
		l_eRes = e_eFSS_UTILSLLPRV_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_ptCbCtx->ptCtxErase ) || ( NULL == p_ptCbCtx->fErase ) ||
            ( NULL == p_ptCbCtx->ptCtxWrite ) || ( NULL == p_ptCbCtx->fWrite ) ||
            ( NULL == p_ptCbCtx->ptCtxRead  ) || ( NULL == p_ptCbCtx->fRead  ) ||
            ( NULL == p_ptCbCtx->ptCtxCrc32 ) || ( NULL == p_ptCbCtx->fCrc32 ) )
        {
            l_eRes = e_eFSS_UTILSLLPRV_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( p_uDataLen <= 0u )
            {
                l_eRes = e_eFSS_UTILSLLPRV_RES_BADPARAM;
            }
            else
            {
                /* Read */
                l_bCbRes = (*(p_ptCbCtx->fCrc32))(p_ptCbCtx->ptCtxCrc32, MAX_UINT32VAL, p_puData, p_uDataLen,
                                                  p_puCrcCal);

                if( false == l_bCbRes )
                {
                    l_eRes = e_eFSS_UTILSLLPRV_RES_CLBCKCRCERR;
                }
                else
                {
                    l_eRes = e_eFSS_UTILSLLPRV_RES_OK;
                }
            }
        }
	}

	return l_eRes;
}


