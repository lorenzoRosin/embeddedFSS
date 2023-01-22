/**
 * @file       eFSS_LOGC.c
 *
 * @brief      LOG Core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/* In this module the page field has the following meaning:
 * - uPageUseSpecific1 -> New page Index
 * - uPageUseSpecific2 -> Old page Index
 * - uPageUseSpecific3 -> Valorized byte in page
 * - uPageUseSpecific4 -> N page used
 * 
 * In this module the storage is organizated as follow :
 * 
 *   bFullBckup = true, bFlashCache = true
 * - [ 0                            -    ( ( uTotPages - 2 ) / 2 ) - 1 ]  -> Original pages
 * - [ ( ( uTotPages - 2 ) / 2 )    -    uTotPages - 1 -2              ]  -> Backup pages
 * - [ uTotPages - 1 -1             -    uTotPages - 1 -1              ]  -> Cache original
 * - [ uTotPages - 1                -    uTotPages - 1                 ]  -> Cache backup
 * 
 * First write original pages and after the backup pages 
 * An unused page is always left after newer page abckup index.
 * Newest and ondest page index can be identical only when the log storage is empty. When the storage is full an empty
 * page is always present between new and old log index.
 */

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_LOGC.h"
#include "eFSS_UTILSHLPRV.h"


/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_LOGC_IsStatusStillCoherent(const t_eFSS_LOGC_Ctx* p_ptCtx);
static e_eFSS_LOGC_RES eFSS_LOGC_HLtoLogRes(const e_eFSS_UTILSHLPRV_RES p_eHLRes);
static e_eFSS_LOGC_RES eFSS_LOGC_LoadIndexNRepair(t_eFSS_LOGC_Ctx* const p_ptCtx);
static e_eFSS_LOGC_RES eFSS_LOGC_LoadIndexFromCache(t_eFSS_LOGC_Ctx* const p_ptCtx);
static e_eFSS_LOGC_RES eFSS_LOGC_LoadIndxBySearch(t_eFSS_LOGC_Ctx* const p_ptCtx);
static e_eFSS_LOGC_RES eFSS_LOGC_UpdateFlashCache(t_eFSS_LOGC_Ctx* const p_ptCtx, t_eFSS_TYPE_PageMeta* p_ptMeta);
static e_eFSS_LOGC_RES eFSS_LOGC_WriteCurrNewPageAndbkup(t_eFSS_LOGC_Ctx* const p_ptCtx, uint8_t* p_puNewPage, t_eFSS_TYPE_PageMeta* p_ptMeta);
static e_eFSS_LOGC_RES eFSS_LOGC_IncrNewPageWriteAndbkup(t_eFSS_LOGC_Ctx* const p_ptCtx, uint8_t* p_puNewPage, t_eFSS_TYPE_PageMeta* p_ptMeta);
static uint32_t eFSS_LOGC_GetNextIndex(uint32_t p_uCurrIdx, uint32_t p_uTotPage);
static uint32_t eFSS_LOGC_GetPrevIndex(uint32_t p_uCurrIdx, uint32_t p_uTotPage);



static e_eFSS_LOGC_RES eFSS_LOGC_WriteCache(const t_eFSS_LOGC_Ctx* p_ptCtx, t_eFSS_TYPE_PageMeta p_tCacheParam);
static e_eFSS_LOGC_RES eFSS_LOGC_ReadCache(const t_eFSS_LOGC_Ctx* p_ptCtx, t_eFSS_TYPE_PageMeta* p_ptCacheParam);
static e_eFSS_LOGC_RES eFSS_LOGC_WritePage(const t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t p_uIdx, 
                                           uint8_t* p_puBuf, uint32_t p_uBufL,
                                           uint8_t* p_puBufS, uint32_t p_uBufSL, t_eFSS_TYPE_PageMeta p_tParam);
static e_eFSS_LOGC_RES eFSS_LOGC_ReadPage(const t_eFSS_LOGC_Ctx* p_ptCtx,uint32_t p_uIdx,  
                                          uint8_t* p_puBuf, uint32_t p_uBufL,
                                          uint8_t* p_puBufS, uint32_t p_uBufSL, t_eFSS_TYPE_PageMeta* p_ptParam);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_LOGC_RES eFSS_LOGC_InitCtx(t_eFSS_LOGC_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx const p_tCtxCb,
                                  uint8_t* const p_puBuff, uint32_t p_uBuffL, t_eFSS_TYPE_StorageSettings p_tStorSet,
                                  bool_t p_bFlashCache, bool_t p_bFullBckup);
{
    e_eFSS_LOGC_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_tCtxCb.ptCtxErase ) || ( NULL == p_tCtxCb.fErase ) ||
            ( NULL == p_tCtxCb.ptCtxWrite ) || ( NULL == p_tCtxCb.fWrite ) ||
            ( NULL == p_tCtxCb.ptCtxRead  ) || ( NULL == p_tCtxCb.fRead  ) ||
            ( NULL == p_tCtxCb.ptCtxCrc32 ) || ( NULL == p_tCtxCb.fCrc32 ) )
        {
            l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( p_uBuffL != ( 2u * p_tStorSet.uPagesLen ) )
            {
                l_eRes = e_eFSS_BLOBC_RES_BADPARAM;
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
                    l_eRes = e_eFSS_BLOBC_RES_BADPARAM;
                }
                else
                {
                    /* Check data validity */
                    if( ( p_tStorSet.uPagesLen <= EFSS_PAGEMETASIZE ) || ( 0u != ( p_tStorSet.uPagesLen % 2u ) ) )
                    {
                        l_eRes = e_eFSS_BLOBC_RES_BADPARAM;
                    }
                    else
                    {
                        /* Check data validity */
                        if( p_tStorSet.uRWERetry <= 0u )
                        {
                            l_eRes = e_eFSS_BLOBC_RES_BADPARAM;
                        }
                        else
                        {
                            /* Fill context */
                            p_ptCtx->bIsInit = true;
                            p_ptCtx->tCtxCb = p_tCtxCb;
                            p_ptCtx->tStorSett = p_tStorSet;
                            p_ptCtx->puBuf = p_puBuff;
                            p_ptCtx->uBufL = p_uBuffL;
                            p_ptCtx->uNewPagIdx = 0;
                            p_ptCtx->uOldPagIdx = 0;
                            p_ptCtx->bFlashCache = p_bFlashCache;
                            p_ptCtx->bFullBckup = p_bFullBckup;
                        }
                    }
                }
            }
        }
    }

    return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGC_IsInit(t_eFSS_LOGC_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eFSS_LOGC_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInit;
        l_eRes = e_eFSS_LOGC_RES_OK;
	}

	return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGC_GetStorageStatus(t_eFSS_LOGC_Ctx* const p_ptCtx)
{
	/* Local variable */
	e_eFSS_LOGC_RES l_eRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eFSS_LOGC_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eFSS_LOGC_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
            }
            else
            {
                l_eRes = eFSS_LOGC_LoadIndexNRepair(p_ptCtx);
            }
		}
	}

	return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGC_GetLogInfo(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t *p_puNewLogI, uint32_t *p_puOldLogI,
                                     uint32_t *p_puNpageUsed, uint32_t *p_puLogVer)
{
	/* Local variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;
    t_eFSS_TYPE_PageMeta l_tPagePrm;
    uint8_t* l_puBuF1;
    uint32_t l_uBuF1L;
    uint32_t l_uNPageU;


	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puNewLogI ) || ( NULL == p_puOldLogI ) || ( NULL == p_puNpageUsed ) || 
        ( NULL == p_puLogVer ) )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eFSS_LOGC_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eFSS_LOGC_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
            }
            else
            {
                /* Fix any memory problem and load index */
                l_eRes = eFSS_LOGC_LoadIndexNRepair(p_ptCtx);

                if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) )
                {
                    /* Get buffer for calculation */
                    l_puBuF1 = p_ptCtx->puBuf;
                    l_uBuF1L = p_ptCtx->uBufL / 2u ;

                    /* Retrive info from the newest log index */
                    l_eHLRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, p_ptCtx->uNPagIdxFound,
                                                            l_puBuF1, l_uBuF1L, &l_tPagePrm, 
                                                            p_ptCtx->tStorSett.uRWERetry);

                    if( e_eFSS_UTILSHLPRV_RES_OK != l_eHLRes )
                    {
                        l_eRes = eFSS_LOGC_HLtoLogRes(l_eHLRes);
                    }
                    else
                    {
                        /* Calculate n page */
                        l_uNPageU = p_ptCtx->tStorSett.uTotPages;

                        if( true == p_ptCtx->bFlashCache )
                        {
                            l_uNPageU -= 2u; 
                        }

                        if( true == p_ptCtx->bFullBckup )
                        {
                            l_uNPageU = (uint32_t)( l_uNPageU / 2u ); 
                        }

                        /* Copy result */
                        *p_puNewLogI = l_tPagePrm.uPageUseSpecific1;
                        *p_puOldLogI = l_tPagePrm.uPageUseSpecific2;
                        *p_puNpageUsed = l_uNPageU;
                        *p_puLogVer = l_tPagePrm.uPageVersion;
                    }
                }
            }
		}
	}

	return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGC_Format(t_eFSS_LOGC_Ctx* const p_ptCtx)
{
	/* Local variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;
    t_eFSS_TYPE_PageMeta l_tPagePrm;
    uint8_t* l_puBuF1;
    uint32_t l_uBuF1L;
    uint8_t* l_puBuF2;
    uint32_t l_uBuF2L;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eFSS_LOGC_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eFSS_LOGC_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
            }
            else
            {
                /* In case of many page a full erase of the area could take too much times.
                   1 - First erase the newest page and it's backup. In this ways even if a power loss occours we have
                       completly invalidated the whole memory.
                   2 - If present set the index cache to point to the first page of the area. In this ways even if a 
                       power outage occours we have that cache is not pointing to valid page anymore.
                   3 - Setup first page as original newest page. 
                   4 - Set second page as backup of first page.
                */
                l_eRes = eFSS_LOGC_LoadIndexNRepair(p_ptCtx);

                if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) ||
                    ( e_eFSS_LOGC_RES_NEWVERSIONLOG == l_eRes ) )
                {
                    /* Valid index found, invalidate the pages pointed by index */
                }
                else if( e_eFSS_LOGC_RES_NOTVALIDLOG == l_eRes )
                {
                    /* Set first page as indexed one */
                    p_ptCtx->uNewPagIdx = 0u;
                    p_ptCtx->uOldPagIdx = 0u;
                }
                else
                {
                    /* Found some strange error, cannot continue */
                }

                /* Start the procedure */
                if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) ||
                    ( e_eFSS_LOGC_RES_NEWVERSIONLOG == l_eRes ) || ( e_eFSS_LOGC_RES_NOTVALIDLOG == l_eRes ) )
                {
                    /* Get buffer for calculation */
                    l_puBuF1 = p_ptCtx->puBuf;
                    l_uBuF1L = p_ptCtx->uBufL / 2u ;
                    l_puBuF2 = &p_ptCtx->puBuf[l_uBuF1L];
                    l_uBuF2L = p_ptCtx->uBufL / 2u ;

                    /* Clear buffer  */
                    memset(l_puBuF1, 0u, l_uBuF1L);
                    memset(l_puBuF2, 0u, l_uBuF2L);

                    /* Step 1 - invalidate orginal */
                    l_tPagePrm.uPageType = EFSS_PAGETYPE_LOG;
                    l_tPagePrm.uPageSubType = EFSS_PAGESUBTYPE_LOG;
                    l_tPagePrm.uPageVersion = p_ptCtx->uVersion;
                    l_tPagePrm.uPageUseSpecific1 = 0u;
                    l_tPagePrm.uPageUseSpecific2 = 0u;
                    l_tPagePrm.uPageUseSpecific3 = 0u;
                    l_tPagePrm.uPageUseSpecific4 = 0u;
                    l_tPagePrm.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
                    l_tPagePrm.uPageCrc = 0u;

                    /* Write */
                    l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->ptCtxCb, 0u, l_puBuF1, l_uBuF1L,
                                                                   l_puBuF2, l_uBuF2L, &l_tPagePrm, 
                                                                   p_ptCtx->tStorSett.uRWERetry);
                    l_eRes = eFSS_LOGC_HLtoLogRes(l_eHLRes);

                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                    {
                        /* Step 1 - invalidate orginal backup */
                        l_tPagePrm.uPageType = EFSS_PAGETYPE_LOG;
                        l_tPagePrm.uPageSubType = EFSS_PAGESUBTYPE_LOG;
                        l_tPagePrm.uPageVersion = p_ptCtx->uVersion;
                        l_tPagePrm.uPageUseSpecific1 = 0u;
                        l_tPagePrm.uPageUseSpecific2 = 0u;
                        l_tPagePrm.uPageUseSpecific3 = 0u;
                        l_tPagePrm.uPageUseSpecific4 = 0u;
                        l_tPagePrm.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
                        l_tPagePrm.uPageCrc = 0u;

                        /* Write */
                        l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->ptCtxCb, 0u, l_puBuF1, l_uBuF1L,
                                                                    l_puBuF2, l_uBuF2L, &l_tPagePrm, 
                                                                    p_ptCtx->tStorSett.uRWERetry);
                        l_eRes = eFSS_LOGC_HLtoLogRes(l_eHLRes);

                        if( e_eFSS_LOGC_RES_OK == l_eRes )
                        {
                            /* Step 2 - if cache is present make it to point first page */
                            if( true == p_ptCtx->bFlashCache )
                            {

                            }

                            if( e_eFSS_LOGC_RES_OK == l_eRes )
                            {
                                /* Step 3 */
                            }
                    }
                }















                /* Clear buffer */
                memset(p_ptCtx->puBuff1, 0u, p_ptCtx->uBuff1L);

                /* Setup metadata */
                l_tPagePrm.uPageType = EFSS_PAGETYPE_LOG;
                l_tPagePrm.uPageSubType = EFSS_PAGESUBTYPE_LOG;
                l_tPagePrm.uPageVersion = p_ptCtx->uVersion;
                l_tPagePrm.uPageUseSpecific1 = 0u;
                l_tPagePrm.uPageUseSpecific2 = 0u;
                l_tPagePrm.uPageUseSpecific3 = 0u;
                l_tPagePrm.uPageUseSpecific4 = 0u;
                l_tPagePrm.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
                l_tPagePrm.uPageCrc = 0u;

                /* Write */
                l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->ptCtxCb, 0u,
                                                               p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                               p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                               &l_tPagePrm, p_ptCtx->uReTry);
                l_eRes = eFSS_LOGC_HLtoLogRes(l_eHLRes);

                if( e_eFSS_LOGC_RES_OK == l_eRes )
                {
                    /* Clear buffer */
                    memset(p_ptCtx->puBuff1, 0u, p_ptCtx->uBuff1L);

                    /* Setup metadata */
                    l_tPagePrm.uPageType = EFSS_PAGETYPE_LOG;
                    l_tPagePrm.uPageSubType = EFSS_PAGESUBTYPE_LOG;
                    l_tPagePrm.uPageVersion = p_ptCtx->uVersion;
                    l_tPagePrm.uPageUseSpecific1 = 0u;
                    l_tPagePrm.uPageUseSpecific2 = 0u;
                    l_tPagePrm.uPageUseSpecific3 = 0u;
                    l_tPagePrm.uPageUseSpecific4 = 0u;
                    l_tPagePrm.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
                    l_tPagePrm.uPageCrc = 0u;

                    /* Write */
                    l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->ptCtxCb, 1u,
                                                                p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                                p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                                &l_tPagePrm, p_ptCtx->uReTry);
                    l_eRes = eFSS_LOGC_HLtoLogRes(l_eHLRes);

                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                    {
                        /* Now we can clear the first two pages*/
                        memset(p_ptCtx->puBuff1, 0u, p_ptCtx->uBuff1L);

                        /* Setup metadata */
                        l_tPagePrm.uPageType = EFSS_PAGETYPE_LOG;
                        l_tPagePrm.uPageSubType = EFSS_PAGESUBTYPE_LOGNEWEST;
                        l_tPagePrm.uPageVersion = p_ptCtx->uVersion;
                        l_tPagePrm.uPageUseSpecific1 = 0u;
                        l_tPagePrm.uPageUseSpecific2 = 0u;
                        l_tPagePrm.uPageUseSpecific3 = 0u;
                        l_tPagePrm.uPageUseSpecific4 = 0u;
                        l_tPagePrm.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
                        l_tPagePrm.uPageCrc = 0u;

                        /* Write */
                        l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->ptCtxCb, 0u,
                                                                       p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                                       p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                                       &l_tPagePrm, p_ptCtx->uReTry);
                        l_eRes = eFSS_LOGC_HLtoLogRes(l_eHLRes);

                        if( e_eFSS_LOGC_RES_OK == l_eRes )
                        {
                            /* Clear buffer */
                            memset(p_ptCtx->puBuff1, 0u, p_ptCtx->uBuff1L);

                            /* Setup metadata */
                            l_tPagePrm.uPageType = EFSS_PAGETYPE_LOG;
                            l_tPagePrm.uPageSubType = EFSS_PAGESUBTYPE_LOGNEWESTBKP;
                            l_tPagePrm.uPageVersion = p_ptCtx->uVersion;
                            l_tPagePrm.uPageUseSpecific1 = 0u;
                            l_tPagePrm.uPageUseSpecific2 = 0u;
                            l_tPagePrm.uPageUseSpecific3 = 0u;
                            l_tPagePrm.uPageUseSpecific4 = 0u;
                            l_tPagePrm.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
                            l_tPagePrm.uPageCrc = 0u;

                            /* Write */
                            l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->ptCtxCb, 1u,
                                                                        p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                                        p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                                        &l_tPagePrm, p_ptCtx->uReTry);
                            l_eRes = eFSS_LOGC_HLtoLogRes(l_eHLRes);

                            if( e_eFSS_LOGC_RES_OK == l_eRes )
                            {
                                /* Clear flash cache if present */
                                if( true == p_ptCtx->bUseFlashCache )
                                {
                                    /* Setup metadata */
                                    l_tPagePrm.uPageType = EFSS_PAGETYPE_LOG;
                                    l_tPagePrm.uPageSubType = EFSS_PAGESUBTYPE_LOGNEWESTBKP;
                                    l_tPagePrm.uPageVersion = p_ptCtx->uVersion;
                                    l_tPagePrm.uPageUseSpecific1 = 0u;
                                    l_tPagePrm.uPageUseSpecific2 = 0u;
                                    l_tPagePrm.uPageUseSpecific3 = 0u;
                                    l_tPagePrm.uPageUseSpecific4 = 0u;
                                    l_tPagePrm.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
                                    l_tPagePrm.uPageCrc = 0u;

                                    l_eRes = eFSS_LOGC_UpdateFlashCache(p_ptCtx, &l_tPagePrm);
                                }
                            }
                        }
                    }
                }
            }
		}
	}

	return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGC_GetLogOfASpecificPage(t_eFSS_LOGC_Ctx* const p_ptCtx, uint8_t* p_puLogBuf, uint32_t* p_uLogBufL)
{
	/* Local variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;
    t_eFSS_TYPE_PageMeta l_tPagePrm;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eFSS_LOGC_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eFSS_LOGC_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
            }
            else
            {
                /* Fix any memory problem and load index */
                l_eRes = eFSS_LOGC_LoadIndexNRepair(p_ptCtx);

                if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) )
                {
                    l_eHLRes =  eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, p_ptCtx->uNPagIdxFound,
                                                            p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                            &l_tPagePrm, p_ptCtx->uReTry);
                    l_eRes = eFSS_LOGC_HLtoLogRes(l_eHLRes);

                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                    {
                        /* Copy readed data */
                        memcpy(p_puLogBuf, p_ptCtx->puBuff1, p_ptCtx->uBuff1L);
                    }
                    else
                    {
                        *p_uLogBufL = 0u;
                    }
                }
            }
		}
	}

	return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGC_AddLog(t_eFSS_LOGC_Ctx* const p_ptCtx, uint8_t* p_puLogToSave, uint32_t p_uLogL)
{
    /* Read current write page */

    /* If page full go next */

    /* Update page and index */

    /* Return */
	/* Local variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;
    t_eFSS_TYPE_PageMeta l_tPagePrm;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puLogToSave ) )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eFSS_LOGC_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eFSS_LOGC_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
            }
            else
            {
                /* Check params validity */
                if( p_uLogL >= 1000u  )
                {
                    l_eRes = e_eFSS_LOGC_RES_BADPARAM;
                }
                else
                {
                    /* Fix any memory problem and load index */
                    l_eRes = eFSS_LOGC_LoadIndexNRepair(p_ptCtx);

                    if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) )
                    {
                        l_eHLRes =  eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, p_ptCtx->uNPagIdxFound,
                                                                 p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                                 &l_tPagePrm, p_ptCtx->uReTry);
                        l_eRes = eFSS_LOGC_HLtoLogRes(l_eHLRes);

                        if( e_eFSS_LOGC_RES_OK == l_eRes )
                        {
                            if( (l_tPagePrm.uPageUseSpecific3 - 1000u) >= p_uLogL )
                            {
                                /* Rol the page and write */
                                eFSS_LOGC_IncrNewPageWriteAndbkup(p_ptCtx, p_ptCtx->puBuff1, &l_tPagePrm);
                            }
                            else
                            {
                                /* We have some free space, add data in this page */
                                memcpy(&p_ptCtx->puBuff1[l_tPagePrm.uPageUseSpecific3], p_puLogToSave, p_uLogL);
                                l_tPagePrm.uPageUseSpecific3 += p_uLogL;

                                /* Write page */
                                eFSS_LOGC_WriteCurrNewPageAndbkup(p_ptCtx, p_ptCtx->puBuff1, &l_tPagePrm);
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
static bool_t eFSS_LOGC_IsStatusStillCoherent(const t_eFSS_LOGC_Ctx* p_ptCtx)
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

static e_eFSS_LOGC_RES eFSS_LOGC_HLtoLogReseFSS_LOGC_HLtoLogRes(const e_eFSS_UTILSHLPRV_RES p_eHLRes)
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


static e_eFSS_LOGC_RES eFSS_LOGC_LoadIndexNRepair(t_eFSS_LOGC_Ctx* const p_ptCtx)
{
    e_eFSS_LOGC_RES l_eRes;

    /* Search for index in the flash cache if present */
    if( true == p_ptCtx->bUseFlashCache )
    {
        /* Load index from cache */
        l_eRes = eFSS_LOGC_LoadIndexFromCache(p_ptCtx);
    }
    else
    {
        /* Search for index */
        l_eRes = eFSS_LOGC_LoadIndxBySearch(p_ptCtx);
    }

    return l_eRes;
}

static e_eFSS_LOGC_RES eFSS_LOGC_LoadIndexFromCache(t_eFSS_LOGC_Ctx* const p_ptCtx)
{
    /* Index are present in the first page and it's backup */
    e_eFSS_UTILSHLPRV_RES l_eIntegrityRes;
    e_eFSS_LOGC_RES l_eRes;
    t_eFSS_TYPE_PageMeta l_tPagePrm;

    /* Verify and create page index integrity */
    l_eIntegrityRes =  eFSS_UTILSHLPRV_VerifyNRipristBkup(p_ptCtx->ptCtxCb, p_ptCtx->uReTry,
                                                          p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                          p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                          0u, 1u);
    l_eRes = eFSS_LOGC_HLtoLogRes(l_eIntegrityRes);

    if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) )
    {
        /* Index page is OK, we can retrive info from that */
        l_eIntegrityRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, 0u,
                                                       p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                       &l_tPagePrm, p_ptCtx->uReTry);
        l_eRes = eFSS_LOGC_HLtoLogRes(l_eIntegrityRes);
        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            /* can load finded index in the context */
            p_ptCtx->uNPagIdxFound = l_tPagePrm.uPageUseSpecific1;
            p_ptCtx->uOPagIdxFound = l_tPagePrm.uPageUseSpecific2;

            /* Verify log version */
            if( p_ptCtx->uVersion != l_tPagePrm.uPageVersion )
            {
                /* New version of log found */
                l_eRes = e_eFSS_LOGC_RES_NEWVERSIONLOG;
            }
            else
            {
                /* Verify page type, if not coherent we were formatting the area before */
            }
        }
    }

    return l_eRes;
}

static e_eFSS_LOGC_RES eFSS_LOGC_LoadIndxBySearch(t_eFSS_LOGC_Ctx* const p_ptCtx)
{
    /* Verify if index already present are valid, if not search for them */
    e_eFSS_UTILSHLPRV_RES l_eIntegrityRes;
    e_eFSS_LOGC_RES l_eRes;
    bool_t l_bIsValid;
    t_eFSS_TYPE_PageMeta p_ptPagePrm;
    bool_t l_bIsIndexFound;
    uint32_t l_i;

    /* Init parameter */
    l_bIsIndexFound = false;

    /* Verify integrity first */
    l_eIntegrityRes = eFSS_UTILSHLPRV_IsValidPage(p_ptCtx->ptCtxCb, p_ptCtx->uNPagIdxFound,
                                                  p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                  p_ptCtx->uReTry,  &l_bIsValid );
    l_eRes = eFSS_LOGC_HLtoLogRes(l_eIntegrityRes);

    if( e_eFSS_LOGC_RES_OK == l_eRes )
    {
        if( true == l_bIsValid )
        {
            /* The page pointed by uNPagIdxFound is ok, need to check if it'is the newest page */
            l_eIntegrityRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, p_ptCtx->uNPagIdxFound,
                                                           p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                           &p_ptPagePrm, p_ptCtx->uReTry);
            l_eRes = eFSS_LOGC_HLtoLogRes(l_eIntegrityRes);
            if( e_eFSS_LOGC_RES_OK == l_eRes )
            {
                if( ( EFSS_PAGETYPE_LOG == p_ptPagePrm.uPageType ) &&
                    ( EFSS_PAGESUBTYPE_LOGNEWEST == p_ptPagePrm.uPageSubType ) &&
                    ( p_ptCtx->uVersion == p_ptPagePrm.uPageVersion ) &&
                    ( EFSS_PAGEMAGICNUMBER == p_ptPagePrm.uPageMagicNumber ) )
                {
                    /* Very good, this is the newwest page avaiable  */
                    l_bIsIndexFound = true;
                }
            }
        }

        if( false == l_bIsIndexFound )
        {
            /* Read every page and search for the newest page type */
            l_i = 0u;

            while( ( false == l_bIsIndexFound ) && ( l_i < p_ptCtx->uNPage )  )
            {
                l_eIntegrityRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, l_i,
                                                               p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                               &p_ptPagePrm, p_ptCtx->uReTry);
                l_eRes = eFSS_LOGC_HLtoLogRes(l_eIntegrityRes);
                if( e_eFSS_LOGC_RES_OK == l_eRes )
                {
                    /* Is the page valid? */
                    l_eIntegrityRes = eFSS_UTILSHLPRV_IsValidPageInBuff(p_ptCtx->ptCtxCb,
                                                                        p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                                        &l_bIsValid);
                    l_eRes = eFSS_LOGC_HLtoLogRes(l_eIntegrityRes);

                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                    {
                        if( true == l_bIsValid )
                        {
                            /* Valid page, is the one we where searching? */
                            if( ( EFSS_PAGETYPE_LOG == p_ptPagePrm.uPageType ) &&
                                ( EFSS_PAGESUBTYPE_LOGNEWEST == p_ptPagePrm.uPageSubType ) &&
                                ( p_ptCtx->uVersion == p_ptPagePrm.uPageVersion ) &&
                                ( EFSS_PAGEMAGICNUMBER == p_ptPagePrm.uPageMagicNumber ) )
                            {
                                /* Super top, this is the page, nothing more to do  */
                                l_bIsIndexFound = true;
                                p_ptCtx->uNPagIdxFound = l_i;
                                p_ptCtx->uOPagIdxFound = p_ptPagePrm.uPageUseSpecific1;
                            }
                            else if( ( EFSS_PAGETYPE_LOG == p_ptPagePrm.uPageType ) &&
                                ( EFSS_PAGESUBTYPE_LOGNEWESTBKP == p_ptPagePrm.uPageSubType ) &&
                                ( p_ptCtx->uVersion == p_ptPagePrm.uPageVersion ) &&
                                ( EFSS_PAGEMAGICNUMBER == p_ptPagePrm.uPageMagicNumber ) )
                            {
                                /* Super top, this is the page, nothing more to do  */
                                l_bIsIndexFound = true;
                                p_ptCtx->uNPagIdxFound = eFSS_LOGC_GetPrevIndex( l_i, p_ptCtx->uNPage );
                                p_ptCtx->uOPagIdxFound = p_ptPagePrm.uPageUseSpecific1;
                            }
                        }
                    }
                }
            }
        }
    }

    return l_eRes;
}

static e_eFSS_LOGC_RES eFSS_LOGC_UpdateFlashCacheeFSS_LOGC_UpdateFlashCache(t_eFSS_LOGC_Ctx* const p_ptCtx, t_eFSS_TYPE_PageMeta* p_ptMeta)
{
	/* Local variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;
    t_eFSS_TYPE_PageMeta l_tPagePrm;

    /* Clear buffer */
    memset(p_ptCtx->puBuff1, 0u, p_ptCtx->uBuff1L);

    /* Setup metadata */
    l_tPagePrm.uPageType = EFSS_PAGETYPE_LOG;
    l_tPagePrm.uPageSubType = EFSS_PAGESUBTYPE_LOGCACHEORI;
    l_tPagePrm.uPageVersion = p_ptCtx->uVersion;
    l_tPagePrm.uPageUseSpecific1 = 0u;
    l_tPagePrm.uPageUseSpecific2 = 0u;
    l_tPagePrm.uPageUseSpecific3 = 0u;
    l_tPagePrm.uPageUseSpecific4 = 0u;
    l_tPagePrm.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
    l_tPagePrm.uPageCrc = 0u;

    /* Write */
    l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->ptCtxCb, 0u,
                                                p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                &l_tPagePrm, p_ptCtx->uReTry);

    l_eRes = eFSS_LOGC_HLtoLogRes(l_eHLRes);

    if( e_eFSS_LOGC_RES_OK == l_eRes )
    {
        /* Clear buffer */
        memset(p_ptCtx->puBuff1, 0u, p_ptCtx->uBuff1L);

        /* Setup metadata */
        l_tPagePrm.uPageType = EFSS_PAGETYPE_LOG;
        l_tPagePrm.uPageSubType = EFSS_PAGESUBTYPE_LOGCACHEBKP;
        l_tPagePrm.uPageVersion = p_ptCtx->uVersion;
        l_tPagePrm.uPageUseSpecific1 = 0u;
        l_tPagePrm.uPageUseSpecific2 = 0u;
        l_tPagePrm.uPageUseSpecific3 = 0u;
        l_tPagePrm.uPageUseSpecific4 = 0u;
        l_tPagePrm.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
        l_tPagePrm.uPageCrc = 0u;

        /* Write */
        l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->ptCtxCb, 1u,
                                                    p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                    p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                    &l_tPagePrm, p_ptCtx->uReTry);

        l_eRes = eFSS_LOGC_HLtoLogRes(l_eHLRes);
    }
}

static e_eFSS_LOGC_RES eFSS_LOGC_WriteCurrNewPageAndbkup(t_eFSS_LOGC_Ctx* const p_ptCtx, uint8_t* p_puNewPage, t_eFSS_TYPE_PageMeta* p_ptMeta)
{


}

static e_eFSS_LOGC_RES eFSS_LOGC_IncrNewPageWriteAndbkup(t_eFSS_LOGC_Ctx* const p_ptCtx, uint8_t* p_puNewPage, t_eFSS_TYPE_PageMeta* p_ptMeta)
{


}

static uint32_t eFSS_LOGC_GetNextIndex(uint32_t p_uCurrIdx, uint32_t p_uTotPage)
{
    uint32_t l_uNextIdx;

    if( p_uCurrIdx >= ( p_uTotPage - 1u ) )
    {
        l_uNextIdx = 0u;
    }
    else
    {
        l_uNextIdx = p_uCurrIdx + 1u;
    }
    return l_uNextIdx;
}

static uint32_t eFSS_LOGC_GetPrevIndex(uint32_t p_uCurrIdx, uint32_t p_uTotPage)
{
    uint32_t l_uPrevIdx;

    if( p_uCurrIdx <= 0u )
    {
        p_uCurrIdx = p_uTotPage - 1u;
    }
    else
    {
        l_uPrevIdx = p_uCurrIdx - 1u;
    }
    return l_uPrevIdx;
}