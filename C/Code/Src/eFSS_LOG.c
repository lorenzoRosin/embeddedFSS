/**
 * @file       eFSS_LOG.c
 *
 * @brief      LOG module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_LOG.h"
#include "eFSS_UTILSHLPRV.h"


/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_LOG_IsStatusStillCoherent(const t_eFSS_LOG_Ctx* p_ptCtx);
static e_eFSS_LOG_RES eFSS_LOG_HLtoLogRes(const e_eFSS_UTILSHLPRV_RES p_eHLRes);
static bool_t eFSS_LOG_LoadIndex(t_eFSS_LOG_Ctx* const p_ptCtx);
static bool_t eFSS_LOG_LoadIndexFromCache(t_eFSS_LOG_Ctx* const p_ptCtx);
static bool_t eFSS_LOG_LoadIndexBySearch(t_eFSS_LOG_Ctx* const p_ptCtx);
static bool_t eFSS_LOG_VerifyVersion(t_eFSS_LOG_Ctx* const p_ptCtx);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_LOG_RES eFSS_LOG_InitCtx(t_eFSS_LOG_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx* const p_ptCtxCb,
                                const uint32_t p_uPageToUse, const uint32_t p_uPageSize, uint8_t* const p_puBuff,
                                uint32_t p_uBuffL)
{
    e_eFSS_LOG_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ptCtxCb ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_ptCtxCb->ptCtxErase ) || ( NULL == p_ptCtxCb->fErase ) ||
            ( NULL == p_ptCtxCb->ptCtxWrite ) || ( NULL == p_ptCtxCb->fWrite ) ||
            ( NULL == p_ptCtxCb->ptCtxRead  ) || ( NULL == p_ptCtxCb->fRead  ) ||
            ( NULL == p_ptCtxCb->ptCtxCrc32 ) || ( NULL == p_ptCtxCb->fCrc32 ) )
        {
            l_eRes = e_eFSS_LOG_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( ( p_uPageToUse <= 2u ) || ( p_uPageSize <= EFSS_PAGEMETASIZE ) || ( ( p_uPageSize * 2u ) != p_uBuffL ) )
            {
                l_eRes = e_eFSS_LOG_RES_BADPARAM;
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

e_eFSS_LOG_RES eFSS_LOG_IsInit(t_eFSS_LOG_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eFSS_LOG_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInit;
        l_eRes = e_eFSS_LOG_RES_OK;
	}

	return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_GetStorageStatus(t_eFSS_LOG_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx* const p_peStatus)
{


}

e_eFSS_LOG_RES eFSS_LOG_Format(t_eFSS_LOG_Ctx* const p_ptCtx)
{


}

e_eFSS_LOG_RES eFSS_LOG_GetLogInfo(t_eFSS_LOG_Ctx* const p_ptCtx, uint32_t *NewlogIndex, uint32_t *OldlogIndex,
                                   uint32_t *NpageValorized, uint32_t *NpageUtilizable, uint32_t *LogVersion)
{



}

e_eFSS_LOG_RES eFSS_LOG_AddLog(t_eFSS_LOG_Ctx* const p_ptCtx, uint8_t* p_puLogToSave, uint32_t p_uLogL)
{
    /* Verify pointer */

    /* Verify parameter */

    /* Verify if init */

    /* Verify if coherent */

    /* Load index */

    /* Read current write page */

    /* If page full go next */

    /* Update page and index */

    /* Return */
}



e_eFSS_LOG_RES eFSS_LOG_GetLogOfASpecificPage(t_eFSS_LOG_Ctx* const p_ptCtx, uint8_t* p_puLogBuff, uint32_t* p_puLogBuffL,
                                              uint32_t *p_puNLogGetted)
{



}

/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eFSS_LOG_IsStatusStillCoherent(const t_eFSS_LOG_Ctx* p_ptCtx)
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

static e_eFSS_LOG_RES eFSS_LOG_HLtoLogRes(const e_eFSS_UTILSHLPRV_RES p_eHLRes)
{
    e_eFSS_LOG_RES l_eRes;

    switch(p_eHLRes)
    {
        case e_eFSS_UTILSHLPRV_RES_OK:
        {
            l_eRes = e_eFSS_LOG_RES_OK;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_NOTVALIDPAGE:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_OK_BKP_RCVRD:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_BADPARAM:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_BADPOINTER:
        {
            l_eRes = e_eFSS_LOG_RES_BADPOINTER;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKERASEERR:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKWRITEERR:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKREADERR:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKCRCERR:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_WRITENOMATCHREAD:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CORRUPT:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }

        default:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }
    }

    return l_eRes;
}


static bool_t eFSS_LOG_LoadIndex(t_eFSS_LOG_Ctx* const p_ptCtx)
{
    /* Search for index of oldest and newest page */
    if( true == p_ptCtx->bUseIndexingFlashCache )
    {
        /* Load index from cache */
        eFSS_LOG_LoadIndexFromCache(p_ptCtx);
    }
    else
    {
        /* verify if current index are ok, if not search them */
        eFSS_LOG_LoadIndexBySearch(p_ptCtx);
    }

    return true;
}

static bool_t eFSS_LOG_LoadIndexFromCache(t_eFSS_LOG_Ctx* const p_ptCtx)
{
    /* Index are present in the first page + backup */
    e_eFSS_UTILSHLPRV_RES l_eIntegrityRes;
    e_eFSS_LOG_RES l_eRes;
    t_eFSS_TYPE_PageMeta p_ptPagePrm;

    /* Verify index integrity */
    l_eIntegrityRes =  eFSS_UTILSHLPRV_VerifyNRipristBkup(p_ptCtx->ptCtxCb, p_ptCtx->uReTry, p_ptCtx->puBuff1, p_ptCtx->uBuff1L, p_ptCtx->puBuff2, p_ptCtx->uBuff2L, 0u, 1u);
    l_eRes = eFSS_LOG_HLtoLogRes(l_eIntegrityRes);
    if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) )
    {
        /* Index page is OK, can retrive */
        l_eIntegrityRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, 0u,  p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                &p_ptPagePrm, p_ptCtx->uReTry);
        l_eRes = eFSS_LOG_HLtoLogRes(l_eIntegrityRes);
        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            p_ptCtx->uNewestPage = p_ptPagePrm.uPageUseSpecific1;
            p_ptCtx->uOldestPage = p_ptPagePrm.uPageUseSpecific2;
        }
    }

    return true;
}

static bool_t eFSS_LOG_LoadIndexBySearch(t_eFSS_LOG_Ctx* const p_ptCtx)
{
    /* Verify if index already present are valid */
    e_eFSS_UTILSHLPRV_RES l_eIntegrityRes;
    e_eFSS_LOG_RES l_eRes;
    bool_t l_bIsValid;
    t_eFSS_TYPE_PageMeta p_ptPagePrm;
    bool_t l_bIsIndexFound;
    uint32_t l_i;

    /* Verify integrity first */
    l_eIntegrityRes = eFSS_UTILSHLPRV_IsValidPage(p_ptCtx->ptCtxCb, p_ptCtx->uNewestPage, p_ptCtx->puBuff1, p_ptCtx->uBuff1L, p_ptCtx->uReTry, &l_bIsValid );
    l_eRes = eFSS_LOG_HLtoLogRes(l_eIntegrityRes);

    if( e_eFSS_LOG_RES_OK == l_eRes )
    {
        if( true == l_bIsValid )
        {
            /* Verify page type and subtype */
            /* Index page is OK, can retrive */
            l_eIntegrityRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, 0u,  p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                    &p_ptPagePrm, p_ptCtx->uReTry);
            l_eRes = eFSS_LOG_HLtoLogRes(l_eIntegrityRes);
            if( e_eFSS_LOG_RES_OK == l_eRes )
            {
                if( ( 0u == p_ptPagePrm.uPageType ) && ( 1u == p_ptPagePrm.uPageSubType ) )
                {
                    /* Super top, this is the page, nothing more to do  */
                    l_bIsIndexFound = true;
                }
            }
        }

        if( false == l_bIsIndexFound )
        {
            /* Read every page and search for type */
            l_i = 0u;

            while( ( false == l_bIsIndexFound ) && (l_i < p_ptCtx->uNPage )  )
            {
                l_eIntegrityRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, 0u,  p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                        &p_ptPagePrm, p_ptCtx->uReTry);
                l_eRes = eFSS_LOG_HLtoLogRes(l_eIntegrityRes);
                if( e_eFSS_LOG_RES_OK == l_eRes )
                {
                    /* Is the page valid? */
                    l_eIntegrityRes = eFSS_UTILSHLPRV_IsValidPageInBuff(p_ptCtx->ptCtxCb, p_ptCtx->puBuff1, p_ptCtx->uBuff1L, &l_bIsValid);
                    l_eRes = eFSS_LOG_HLtoLogRes(l_eIntegrityRes);

                    if( e_eFSS_LOG_RES_OK == l_eRes )
                    {
                        if( true == l_bIsValid )
                        {
                            /* Valid page, is the one we where searching? */
                            if( ( 0u == p_ptPagePrm.uPageType ) && ( 1u == p_ptPagePrm.uPageSubType ) )
                            {
                                /* Super top, this is the page, nothing more to do  */
                                l_bIsIndexFound = true;
                                p_ptCtx->uNewestPage = p_ptPagePrm.uPageUseSpecific1;
                                p_ptCtx->uOldestPage = p_ptPagePrm.uPageUseSpecific2;
                            }
                        }
                    }
                }
            }
        }
    }
}

static bool_t eFSS_LOG_VerifyVersion(t_eFSS_LOG_Ctx* const p_ptCtx)
{
    /* Index are present in the first page + backup */
    e_eFSS_UTILSHLPRV_RES l_eIntegrityRes;
    e_eFSS_LOG_RES l_eRes;
    t_eFSS_TYPE_PageMeta p_ptPagePrm;

    /* Verify index integrity */
    l_eIntegrityRes =  eFSS_UTILSHLPRV_VerifyNRipristBkup(p_ptCtx->ptCtxCb, p_ptCtx->uReTry, p_ptCtx->puBuff1, p_ptCtx->uBuff1L, p_ptCtx->puBuff2, p_ptCtx->uBuff2L, 0u, 1u);
    l_eRes = eFSS_LOG_HLtoLogRes(l_eIntegrityRes);
    if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) )
    {
        /* Index page is OK, can retrive */
        l_eIntegrityRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, 0u,  p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                &p_ptPagePrm, p_ptCtx->uReTry);
        l_eRes = eFSS_LOG_HLtoLogRes(l_eIntegrityRes);
        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            p_ptCtx->uNewestPage = p_ptPagePrm.uPageUseSpecific1;
            p_ptCtx->uOldestPage = p_ptPagePrm.uPageUseSpecific2;
        }
    }

    return true;
}
