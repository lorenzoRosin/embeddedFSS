/**
 * @file       eFSS_LOGCPRV.c
 *
 * @brief      LOG Core private module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_LOGCPRV.h"
#include "eFSS_UTILSHLPRV.h"



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
static bool_t eFSS_LOGCPRV_IsStatusStillCoherent(const t_eFSS_LOGC_Ctx* p_ptCtx)
{
    bool_t l_bRes;

    /* Check pointer validity */
    if( ( NULL == p_ptCtx->tCtxCb.ptCtxErase ) || ( NULL == p_ptCtx->tCtxCb.fErase ) ||
        ( NULL == p_ptCtx->tCtxCb.ptCtxWrite ) || ( NULL == p_ptCtx->tCtxCb.fWrite ) ||
        ( NULL == p_ptCtx->tCtxCb.ptCtxRead  ) || ( NULL == p_ptCtx->tCtxCb.fRead  ) ||
        ( NULL == p_ptCtx->tCtxCb.ptCtxCrc32 ) || ( NULL == p_ptCtx->tCtxCb.fCrc32 ) )
    {
        l_bRes = false;
    }
    else
    {
        /* Check data validity */
        if( p_ptCtx->puBuf != ( 2u * p_ptCtx->tStorSett.uPagesLen ) )
        {
            l_bRes = false;
        }
        else
        {
            /* Check data validity */
            if( ( 0u != ( p_tStorSet.uTotPages % 2u ) ) || 
                ( ( false == p_bFullBckup ) && ( false == p_bFlashCache ) && ( p_tStorSet.uTotPages < 4u  ) ) || 
                ( ( false == p_bFullBckup ) && ( true  == p_bFlashCache ) && ( p_tStorSet.uTotPages < 6u  ) ) || 
                ( ( true  == p_bFullBckup ) && ( false == p_bFlashCache ) && ( p_tStorSet.uTotPages < 8u  ) ) || 
                ( ( true  == p_bFullBckup ) && ( true  == p_bFlashCache ) && ( p_tStorSet.uTotPages < 10u ) ) )
            {
                l_bRes = false;
            }
            else
            {
                /* Check data validity */
                if( ( p_tStorSet.uPagesLen <= EFSS_PAGEMETASIZE ) || ( 0u != ( p_tStorSet.uPagesLen % 2u ) ) )
                {
                    l_bRes = false;
                }
                else
                {
                    /* Check data validity */
                    if( p_tStorSet.uRWERetry <= 0u )
                    {
                        l_bRes = false;
                    }
                    else
                    {
                        l_bRes = true;
                    }
                }
            }
        }
    }
    
    return l_bRes;
}

e_eFSS_LOGC_RES eFSS_LOGCPRV_HLtoLogRes(const e_eFSS_UTILSHLPRV_RES p_eHLRes)
{
    e_eFSS_LOGC_RES l_eRes;

    switch(p_eHLRes)
    {
        case e_eFSS_UTILSHLPRV_RES_OK:
        {
            l_eRes = e_eFSS_LOGC_RES_OK;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_NOTVALIDPAGE:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_OK_BKP_RCVRD:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_BADPARAM:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_BADPOINTER:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKERASEERR:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKWRITEERR:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKREADERR:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKCRCERR:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_WRITENOMATCHREAD:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CORRUPT:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
            break;
        }

        default:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
            break;
        }
    }

    return l_eRes;
}
