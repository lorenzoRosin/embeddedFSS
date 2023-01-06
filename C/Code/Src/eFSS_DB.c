/**
 * @file       eFSS_DB.c
 *
 * @brief      Database module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_DB.h"
#include "eFSS_UTILSHLPRV.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_DB_IsStatusStillCoherent(const t_eFSS_DB_Ctx* p_ptCtx);
static e_eFSS_DB_RES eFSS_DB_HLtoDBRes(const e_eFSS_UTILSHLPRV_RES p_eHLRes);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_DB_RES eFSS_DB_InitCtx(t_eFSS_DB_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx* const p_ptCtxCb,
                                  const uint32_t p_uPageToUse, const uint32_t p_uPageSize, uint8_t* const p_puBuff,
                                  uint32_t p_uBuffL, t_eFSS_DB_DbStruct p_tDBS)
{
    e_eFSS_DB_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ptCtxCb ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_DB_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_ptCtxCb->ptCtxErase ) || ( NULL == p_ptCtxCb->fErase ) ||
            ( NULL == p_ptCtxCb->ptCtxWrite ) || ( NULL == p_ptCtxCb->fWrite ) ||
            ( NULL == p_ptCtxCb->ptCtxRead  ) || ( NULL == p_ptCtxCb->fRead  ) ||
            ( NULL == p_ptCtxCb->ptCtxCrc32 ) || ( NULL == p_ptCtxCb->fCrc32 ) )
        {
            l_eRes = e_eFSS_DB_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( ( p_uPageToUse <= 2u ) || ( p_uPageSize <= EFSS_PAGEMETASIZE ) || ( ( p_uPageSize * 2u ) != p_uBuffL ) )
            {
                l_eRes = e_eFSS_DB_RES_BADPARAM;
            }
            else
            {
                /* Fill context */
                p_ptCtx->bIsInit = true;
                p_ptCtx->ptCtxCb = p_ptCtxCb;
                p_ptCtx->puBuff1 = p_puBuff;
                p_ptCtx->uBuff1L = p_uBuffL / 2u;
                p_ptCtx->puBuff2 = &p_puBuff[p_ptCtx->uBuff1L];
                p_ptCtx->uBuff2L = p_uBuffL / 2u;
                p_ptCtx->uNPage = p_uPageToUse;
                p_ptCtx->uPageSize = p_uPageSize;
            }
        }
    }

    return l_eRes;
}


e_eFSS_DB_RES eFSS_DB_IsInit(t_eFSS_DB_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eFSS_DB_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSS_DB_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInit;
        l_eRes = e_eFSS_DB_RES_OK;
	}

	return l_eRes;
}

e_eFSS_DB_RES eFSS_DB_GetStorageStatus(t_eFSS_DB_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx* const p_peStatus)
{

}


e_eFSS_DB_RES eFSS_DB_Format(t_eFSS_DB_Ctx* const p_ptCtx)
{


}


e_eFSS_DB_RES eFSS_DB_SaveElemen(t_eFSS_DB_Ctx* const p_ptCtx, uint32_t p_uPos, uint8_t* p_puBuff, uint32_t p_uBuffL)
{


}


e_eFSS_DB_RES eFSS_DB_GetElement(t_eFSS_DB_Ctx* const p_ptCtx, uint32_t p_uPos, uint8_t* p_puBuff, uint8_t* p_puBuffL)
{


}


/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eFSS_DB_IsStatusStillCoherent(const t_eFSS_DB_Ctx* p_ptCtx)
{
    bool_t l_eRes;

	/* Check context validity */
	if( (  NULL == p_ptCtx->ptCtxCb ) || ( NULL == p_ptCtx->puBuff1 ) )
	{
		l_eRes = false;
	}
	else
	{
        l_eRes = true;
	}

    return l_eRes;
}

static e_eFSS_DB_RES eFSS_BLOB_HLtoBLOBRes(const e_eFSS_UTILSHLPRV_RES p_eHLRes)
{
    e_eFSS_DB_RES l_eRes;

    switch(p_eHLRes)
    {
        case e_eFSS_UTILSHLPRV_RES_OK:
        {
            l_eRes = e_eFSS_DB_RES_OK;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_NOTVALIDPAGE:
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_OK_BKP_RCVRD:
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_BADPARAM:
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_BADPOINTER:
        {
            l_eRes = e_eFSS_DB_RES_BADPOINTER;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKERASEERR:
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKWRITEERR:
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKREADERR:
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKCRCERR:
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_WRITENOMATCHREAD:
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CORRUPT:
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
            break;
        }

        default:
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
            break;
        }
    }

    return l_eRes;
}