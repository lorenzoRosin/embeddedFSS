/**
 * @file       eFSS_BLOB.c
 *
 * @brief      Blob large file module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_BLOB.h"
#include "eFSS_UTILSHLPRV.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_BLOB_IsStatusStillCoherent(const t_eFSS_BLOB_Ctx* p_ptCtx);
static e_eFSS_BLOB_RES eFSS_BLOB_AreOriValid(const t_eFSS_BLOB_Ctx* p_ptCtx, bool_t* p_pbAreValid, uint32_t* p_puCtr);
static e_eFSS_BLOB_RES eFSS_BLOB_AreBckUpValid(const t_eFSS_BLOB_Ctx* p_ptCtx, bool_t* p_pbAreValid, uint32_t* p_puCtr);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_BLOB_RES eFSS_BLOB_InitCtx(t_eFSS_BLOB_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx* const p_ptCtxCb,
                                  const uint32_t p_uPageToUse, const uint32_t p_uPageSize, uint8_t* const p_puBuff,
                                  uint32_t p_uBuffL)
{
    e_eFSS_BLOB_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ptCtxCb ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_ptCtxCb->ptCtxErase ) || ( NULL == p_ptCtxCb->fErase ) ||
            ( NULL == p_ptCtxCb->ptCtxWrite ) || ( NULL == p_ptCtxCb->fWrite ) ||
            ( NULL == p_ptCtxCb->ptCtxRead  ) || ( NULL == p_ptCtxCb->fRead  ) ||
            ( NULL == p_ptCtxCb->ptCtxCrc32 ) || ( NULL == p_ptCtxCb->fCrc32 ) )
        {
            l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( ( p_uPageToUse <= 2u ) || ( p_uPageSize <= EFSS_PAGEMETASIZE ) || ( p_uPageSize != p_uBuffL ) )
            {
                l_eRes = e_eFSS_BLOB_RES_BADPARAM;
            }
            else
            {
                /* Fill context */
                p_ptCtx->bIsInit = true;
                p_ptCtx->ptCtxCb = p_ptCtxCb;
                p_ptCtx->puBuff = p_puBuff;
                p_ptCtx->uBuffL = p_uBuffL;
                p_ptCtx->uNPage = p_uPageToUse;
                p_ptCtx->uPageSize = p_uPageSize;
            }
        }
    }

    return l_eRes;
}


e_eFSS_BLOB_RES eFSS_BLOB_IsInit(t_eFSS_BLOB_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eFSS_BLOB_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInit;
        l_eRes = e_eFSS_BLOB_RES_OK;
	}

	return l_eRes;
}

/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eFSS_BLOB_IsStatusStillCoherent(const t_eFSS_BLOB_Ctx* p_ptCtx)
{
    bool_t l_eRes;

	/* Check context validity */
	if( (  NULL == p_ptCtx->ptCtxCb ) || ( NULL == p_ptCtx->puBuff ) )
	{
		l_eRes = false;
	}
	else
	{
        l_eRes = true;
	}

    return l_eRes;
}


static e_eFSS_BLOB_RES eFSS_BLOB_AreOriValid(const t_eFSS_BLOB_Ctx* p_ptCtx, bool_t* p_pbAreValid, uint32_t* p_puCtr)
{
	/* Local variable return */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHlRes;

    /* Local variable Cycle data */
    uint32_t l_i;
    t_eFSS_TYPE_PageMeta p_ptPagePrm;
    bool_t l_pbIsValidPage;

    /* Local variable return data */
    uint32_t l_uValidCtr;
    uint32_t l_uLastReadInc;



	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbAreValid ) || ( NULL == p_puCtr ) )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
        l_eRes = e_eFSS_BLOB_RES_OK;

        for( l_i = 0u; l_i < p_ptCtx->uNPage / 2u; l_i++)
        {
            /* Read a page */
            l_eHlRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, l_i,  p_ptCtx->puBuff, p_ptCtx->uBuffL,
                                                    &p_ptPagePrm, p_ptCtx->uReTry);
            if( e_eFSS_UTILSHLPRV_RES_OK == l_eHlRes )
            {
                /* Verify if correct */
                l_eHlRes = eFSS_UTILSHLPRV_IsValidPageInBuff(p_ptCtx->ptCtxCb, p_ptCtx->puBuff, p_ptCtx->uBuffL,
                                                             &l_pbIsValidPage);
                if( e_eFSS_UTILSHLPRV_RES_OK == l_eHlRes )
                {
                    if( true == l_pbIsValidPage)
                    {
                        l_uValidCtr++;

                        if( 0u == l_i )
                        {
                            /* First run, save seq number  */
                            l_uLastReadInc = p_ptPagePrm.uPageSequentialN;
                        }
                        else
                        {
                            if( p_ptPagePrm.uPageSequentialN != l_uLastReadInc)
                            {
                                /* Not valid */
                            }
                        }
                    }
                }
            }
        }

        if( e_eFSS_BLOB_RES_OK == l_eRes )
        {
            if()
            {
                *p_pbAreValid = true;
                *p_puCtr = l_uLastReadInc;
            }
        }
	}

	return l_eRes;
}


static e_eFSS_BLOB_RES eFSS_BLOB_AreBckUpValid(const t_eFSS_BLOB_Ctx* p_ptCtx, bool_t* p_pbAreValid, uint32_t* p_puCtr)
{

}