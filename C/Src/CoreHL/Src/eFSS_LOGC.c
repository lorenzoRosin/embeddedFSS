/**
 * @file       eFSS_LOGC.c
 *
 * @brief      LOG Core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/* In this module the page field has the following meaning:
 * ------------------------------------------------------------------ User data
 * - [uint8_t] -                    -> N byte of user data           |
 * ------------------------------------------------------------------ Metadata  (4 byte)
 * - uint32_t  - Byte in Page       -> Valorized byte in page        |
 * ------------------------------------------------------------------ Under we have LL/HL metadata (20byte)
 * - LOW LEVEL / HIGH LEVEL METADATA                                 |
 * ------------------------------------------------------------------ End of Page
 *
 * In this module the storage is organizated as follow :
 *
 *   bFullBckup = true, bFlashCache = true
 * - [ 0                            -    ( ( uTotPages - 2 ) / 2 ) - 1 ]  -> Original Log pages
 * - [ ( ( uTotPages - 2 ) / 2 )    -    uTotPages - 1 - 2             ]  -> Backup Log pages
 * - [ uTotPages - 1 - 1            -    uTotPages - 1 - 1             ]  -> Cache original
 * - [ uTotPages - 1                -    uTotPages - 1                 ]  -> Cache backup
 *
 */

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_LOGC.h"
#include "eFSS_Utils.h"



/***********************************************************************************************************************
 *      PRIVATE DEFINE
 **********************************************************************************************************************/
#define EFSS_PAGETYPE_LOG                                                                        ( ( uint8_t )   0x02u )
#define EFSS_PAGESUBTYPE_LOGORI                                                                  ( ( uint8_t )   0x01u )
#define EFSS_PAGESUBTYPE_LOGBKP                                                                  ( ( uint8_t )   0x02u )
#define EFSS_PAGESUBTYPE_LOGNEWESTORI                                                            ( ( uint8_t )   0x03u )
#define EFSS_PAGESUBTYPE_LOGNEWESTBKP                                                            ( ( uint8_t )   0x04u )
#define EFSS_PAGESUBTYPE_LOGNEWESTBKPORI                                                         ( ( uint8_t )   0x05u )
#define EFSS_PAGESUBTYPE_LOGNEWESTBKPBKP                                                         ( ( uint8_t )   0x06u )
#define EFSS_PAGESUBTYPE_LOGCACHEORI                                                             ( ( uint8_t )   0x07u )
#define EFSS_PAGESUBTYPE_LOGCACHEBKP                                                             ( ( uint8_t )   0x08u )
#define EFSS_LOGC_PAGEMIN_L                                                                      ( ( uint32_t )     4u )
#define EFSS_LOGC_CACHEMIN_L                                                                     ( ( uint32_t )    12u )



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_LOGC_IsStatusStillCoherent(t_eFSS_LOGC_Ctx* const p_ptCtx);
static e_eFSS_LOGC_RES eFSS_LOGC_HLtoLOGCRes(const e_eFSS_COREHL_RES p_eHLRes);



/***********************************************************************************************************************
 *  PRIVATE STATIC UTILS FUNCTION DECLARATION
 **********************************************************************************************************************/
static uint32_t eFSS_LOGC_GetMaxPage(const bool_t p_bIsFullBkup, const bool_t p_bIsFCache, const uint32_t p_uTotPages);

static e_eFSS_LOGC_RES eFSS_LOGC_FlushBuff(t_eFSS_LOGC_Ctx* const p_ptCtx,
                                           const bool_t p_bIsBkpP, const uint32_t p_uByteUse,
								           const uint32_t p_uOrigIdx, const uint32_t p_uBackupIdx,
                                           const uint8_t p_uOriSubT, const uint8_t p_uBckUpSubT);

static e_eFSS_LOGC_RES eFSS_LOGC_LoadBuff(t_eFSS_LOGC_Ctx* const p_ptCtx,
                                          const bool_t p_bIsBkpP, uint32_t* const p_puByteUse,
                                          const uint32_t p_uOrigIdx, const uint32_t p_uBackupIdx,
                                          const uint8_t p_uOriSubT, const uint8_t p_uBckUpSubT);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_LOGC_RES eFSS_LOGC_InitCtx(t_eFSS_LOGC_Ctx* const p_ptCtx, const t_eFSS_TYPE_CbStorCtx p_tCtxCb,
                                  const t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff,
                                  const uint32_t p_uBuffL, const bool_t p_bFlashCache, const bool_t p_bFullBckup)
{
    /* Local return variable */
    e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local storage variable */
    t_eFSS_COREHL_StorBuf l_tBuff;

    /* Local variable for calculation */
    uint32_t l_uNPage;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
        l_uNPage = p_tStorSet.uTotPages;

        /* Check numbers of page validity */
        if( ( ( false == p_bFullBckup ) && ( false == p_bFlashCache ) && ( l_uNPage < 4u  ) ) ||
            ( ( false == p_bFullBckup ) && ( true  == p_bFlashCache ) && ( l_uNPage < 6u  ) ) ||
            ( ( true  == p_bFullBckup ) && ( false == p_bFlashCache ) && ( l_uNPage < 8u  ) ) ||
            ( ( true  == p_bFullBckup ) && ( true  == p_bFlashCache ) && ( l_uNPage < 10u ) ) ||
            ( ( true  == p_bFullBckup ) && ( 0u != ( l_uNPage % 2u ) ) ) )
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
        }
        else
        {
            /* Can init low level context */
            l_eResHL = eFSS_COREHL_InitCtx(&p_ptCtx->tCOREHLCtx, p_tCtxCb, p_tStorSet, EFSS_PAGETYPE_LOG,
                                           p_puBuff, p_uBuffL);
            l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

            if( e_eFSS_LOGC_RES_OK == l_eRes )
            {
                /* HL level initialized, now check if we have enogh space for the page specific data */
                l_eResHL = eFSS_COREHL_GetBuff(&p_ptCtx->tCOREHLCtx, &l_tBuff);
                l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

                if( e_eFSS_LOGC_RES_OK == l_eRes )
                {
                    if( ( ( l_tBuff.uBufL <= EFSS_LOGC_PAGEMIN_L  ) && ( false == p_bFlashCache ) ) ||
                        ( ( l_tBuff.uBufL <= EFSS_LOGC_CACHEMIN_L ) && ( true == p_bFlashCache  ) ) )
                    {
                        /* De init HL, we dont' have enogh data avaible */
                        (void)memset(&p_ptCtx->tCOREHLCtx, 0, sizeof(t_eFSS_COREHL_Ctx));

                        l_eRes = e_eFSS_LOGC_RES_BADPARAM;
                    }
                    else
                    {
                        /* Fill context */
                        p_ptCtx->bFlashCache = p_bFlashCache;
                        p_ptCtx->bFullBckup = p_bFullBckup;
                    }
                }
                else
                {
                    /* De init HL */
                    (void)memset(&p_ptCtx->tCOREHLCtx, 0, sizeof(t_eFSS_COREHL_Ctx));
                }
            }
        }
    }

    return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGC_IsInit(t_eFSS_LOGC_Ctx* const p_ptCtx, bool_t* const p_pbIsInit)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, p_pbIsInit);
        l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);
	}

	return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGC_IsFlashCacheUsed(t_eFSS_LOGC_Ctx* const p_ptCtx, bool_t* const p_pbIsFlashCacheUsed)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local variable for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsFlashCacheUsed ) )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
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
                    *p_pbIsFlashCacheUsed = p_ptCtx->bFlashCache;
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGC_GetBuffNUsable(t_eFSS_LOGC_Ctx* const p_ptCtx, t_eFSS_LOGC_StorBuf* const p_ptBuff,
                                         uint32_t* const p_puUsePages)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local storage variable */
    t_eFSS_COREHL_StorBuf l_tBuff;
    t_eFSS_TYPE_StorSet   l_tStorSet;

    /* Local variable for calculation */
    bool_t l_bIsInit;


	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ptBuff ) || ( NULL == p_puUsePages ) )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
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
                    l_eResHL = eFSS_COREHL_GetBuffNStor(&p_ptCtx->tCOREHLCtx, &l_tBuff, &l_tStorSet);
                    l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                    {
                        p_ptBuff->puBuf = l_tBuff.puBuf;
                        p_ptBuff->uBufL = l_tBuff.uBufL - EFSS_LOGC_PAGEMIN_L;

                        *p_puUsePages = eFSS_LOGC_GetMaxPage(p_ptCtx->bFullBckup, p_ptCtx->bFlashCache,
                                                             l_tStorSet.uTotPages);
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGC_WriteCache(t_eFSS_LOGC_Ctx* const p_ptCtx, const uint32_t p_uIdxN, const uint32_t p_uFilP)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local storage variable */
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_COREHL_StorBuf l_tBuff;

    /* Local variable for calculation */
    bool_t l_bIsInit;
    uint32_t l_uCacheIdx;
    uint32_t l_uCacheIdxBkp;
    uint32_t l_uUsableP;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
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
                    /* In order to write the cache the cache itself must be enabled */
                    if( false == p_ptCtx->bFlashCache )
                    {
                        l_eRes = e_eFSS_LOGC_RES_BADPARAM;
                    }
                    else
                    {
                        /* Get pages and storage settings */
                        l_eResHL = eFSS_COREHL_GetBuffNStor(&p_ptCtx->tCOREHLCtx, &l_tBuff, &l_tStorSet);
                        l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

                        if( e_eFSS_LOGC_RES_OK == l_eRes )
                        {
                            /* Need to Verify passed page index and filled size */
                            l_uUsableP = eFSS_LOGC_GetMaxPage(p_ptCtx->bFullBckup, p_ptCtx->bFlashCache,
                                                              l_tStorSet.uTotPages);

                            /* Cannot save an invalid index and an overloaded numbers of filled page */
                            if( ( p_uIdxN >= l_uUsableP ) || ( ( p_uFilP + 3u ) > l_uUsableP ) )
                            {
                                l_eRes = e_eFSS_LOGC_RES_BADPARAM;
                            }
                            else
                            {
                                /* Clear internal buffer */
                                (void)memset(l_tBuff.puBuf, 0, l_tBuff.uBufL);

                                /* Insert data */
                                if( true != eFSS_Utils_InsertU32(&l_tBuff.puBuf[0u], p_uIdxN) )
                                {
                                    l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
                                }
                                else
                                {
                                    /* Insert data */
                                    if( true != eFSS_Utils_InsertU32(&l_tBuff.puBuf[4u], p_uFilP) )
                                    {
                                        l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
                                    }
                                    else
                                    {
                                        /* Setup index */
                                        l_uCacheIdx = l_tStorSet.uTotPages - 2u;
                                        l_uCacheIdxBkp = l_tStorSet.uTotPages - 1u;

                                        /* Flush */
                                        l_eRes = eFSS_LOGC_FlushBuff(p_ptCtx, true, 8u, l_uCacheIdx, l_uCacheIdxBkp,
                                                                     EFSS_PAGESUBTYPE_LOGCACHEORI,
                                                                     EFSS_PAGESUBTYPE_LOGCACHEBKP);
                                    }
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

e_eFSS_LOGC_RES eFSS_LOGC_ReadCache(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t* const p_puIdxN, uint32_t* const p_puFilP)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local storage variable */
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_COREHL_StorBuf l_tBuff;

    /* Local variable for calculation */
    bool_t l_bIsInit;
    uint32_t l_uCacheIdx;
    uint32_t l_uCacheIdxBkp;
    uint32_t l_uByteInP;
    uint32_t l_uUsableP;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puIdxN ) || ( NULL == p_puFilP ) )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
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
                    if( false == p_ptCtx->bFlashCache )
                    {
                        l_eRes = e_eFSS_LOGC_RES_BADPARAM;
                    }
                    else
                    {
                        l_eResHL = eFSS_COREHL_GetBuffNStor(&p_ptCtx->tCOREHLCtx, &l_tBuff, &l_tStorSet);
                        l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

                        if( e_eFSS_LOGC_RES_OK == l_eRes )
                        {
                            /* Setup index */
                            l_uCacheIdx = l_tStorSet.uTotPages - 2u;
                            l_uCacheIdxBkp = l_tStorSet.uTotPages - 1u;

                            /* Before reading fix any error in original and backup pages */
                            l_eRes = eFSS_LOGC_LoadBuff(p_ptCtx, true, &l_uByteInP, l_uCacheIdx, l_uCacheIdxBkp,
                                                        EFSS_PAGESUBTYPE_LOGCACHEORI, EFSS_PAGESUBTYPE_LOGCACHEBKP);

                            if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) )
                            {
                                /* Retrive parameter */
                                if( true != eFSS_Utils_RetriveU32(&l_tBuff.puBuf[0u], p_puIdxN) )
                                {
                                    l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
                                }
                                else
                                {
                                    /* Retrive parameter */
                                    if( true != eFSS_Utils_RetriveU32(&l_tBuff.puBuf[4u], p_puFilP) )
                                    {
                                        l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
                                    }
                                    else
                                    {
                                        /* Need to verify parameter before confirmg the validity of the page */
                                        if( 8u != l_uByteInP )
                                        {
                                            l_eRes = e_eFSS_LOGC_RES_NOTVALIDLOG;
                                        }
                                        else
                                        {
                                            /* Verify also parameter coherence */
                                            l_uUsableP = eFSS_LOGC_GetMaxPage(p_ptCtx->bFullBckup, p_ptCtx->bFlashCache,
                                                                              l_tStorSet.uTotPages);

                                            /* Verify index parameter coherence */
                                            if( ( *p_puIdxN >= l_uUsableP ) || ( ( *p_puFilP + 3u ) > l_uUsableP ) )
                                            {
                                                l_eRes = e_eFSS_LOGC_RES_NOTVALIDLOG;
                                            }
                                        }
                                    }
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

e_eFSS_LOGC_RES eFSS_LOGC_FlushBufferAs(t_eFSS_LOGC_Ctx* const p_ptCtx, const e_eFSS_LOGC_PAGETYPE p_ePageType,
                                        const uint32_t p_uIdx, const uint32_t p_uFillInPage)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local storage variable */
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint32_t l_uNPageU;

    /* Local variable for calculation */
    bool_t l_bIsInit;
    uint8_t l_uPagSubTOri;
    uint8_t l_uPagSubTBkp;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
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
                    /* Get the needed data from HL level */
                    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
                    l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                    {
                        /* Calculate n page */
                        l_uNPageU = eFSS_LOGC_GetMaxPage(p_ptCtx->bFullBckup, p_ptCtx->bFlashCache,
                                                         l_tStorSet.uTotPages);

                        /* Verify page coherence */
                        if( p_uIdx >= l_uNPageU )
                        {
                            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
                        }
                        else
                        {
                            switch(p_ePageType)
                            {
                                case e_eFSS_LOGC_PAGETYPE_LOG:
                                {
                                    l_uPagSubTOri = EFSS_PAGESUBTYPE_LOGORI;
                                    l_uPagSubTBkp = EFSS_PAGESUBTYPE_LOGBKP;
                                    break;
                                }

                                case e_eFSS_LOGC_PAGETYPE_NEWEST:
                                {
                                    l_uPagSubTOri = EFSS_PAGESUBTYPE_LOGNEWESTORI;
                                    l_uPagSubTBkp = EFSS_PAGESUBTYPE_LOGNEWESTBKP;
                                    break;
                                }

                                case e_eFSS_LOGC_PAGETYPE_NEWEST_BKUP:
                                {
                                    l_uPagSubTOri = EFSS_PAGESUBTYPE_LOGNEWESTBKPORI;
                                    l_uPagSubTBkp = EFSS_PAGESUBTYPE_LOGNEWESTBKPBKP;
                                    break;
                                }

                                default:
                                {
                                    l_uPagSubTOri = 0u;
                                    l_uPagSubTBkp = 0u;
                                    l_eRes = e_eFSS_LOGC_RES_BADPARAM;
                                    break;
                                }
                            }

                            if( e_eFSS_LOGC_RES_OK == l_eRes )
                            {
                                l_eRes = eFSS_LOGC_FlushBuff(p_ptCtx, p_ptCtx->bFullBckup, p_uFillInPage, p_uIdx,
                                                             ( l_uNPageU + p_uIdx ), l_uPagSubTOri, l_uPagSubTBkp);
                            }
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGC_LoadBufferAs(t_eFSS_LOGC_Ctx* const p_ptCtx, const e_eFSS_LOGC_PAGETYPE p_ePageType,
                                       const uint32_t p_uIdx, uint32_t* const p_puFillInPage)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local storage variable */
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint32_t l_uNPageU;

    /* Local variable for calculation */
    bool_t l_bIsInit;
    uint8_t l_uPagSubTOri;
    uint8_t l_uPagSubTBkp;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
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
                    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
                    l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                    {
                        /* Calculate n page */
                        l_uNPageU = eFSS_LOGC_GetMaxPage(p_ptCtx->bFullBckup, p_ptCtx->bFlashCache,
                                                         l_tStorSet.uTotPages);

                        /* Index mus be coherent */
                        if( p_uIdx >= l_uNPageU )
                        {
                            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
                        }
                        else
                        {
                            switch(p_ePageType)
                            {
                                case e_eFSS_LOGC_PAGETYPE_LOG:
                                {
                                    l_uPagSubTOri = EFSS_PAGESUBTYPE_LOGORI;
                                    l_uPagSubTBkp = EFSS_PAGESUBTYPE_LOGBKP;
                                    break;
                                }

                                case e_eFSS_LOGC_PAGETYPE_NEWEST:
                                {
                                    l_uPagSubTOri = EFSS_PAGESUBTYPE_LOGNEWESTORI;
                                    l_uPagSubTBkp = EFSS_PAGESUBTYPE_LOGNEWESTBKP;
                                    break;
                                }

                                case e_eFSS_LOGC_PAGETYPE_NEWEST_BKUP:
                                {
                                    l_uPagSubTOri = EFSS_PAGESUBTYPE_LOGNEWESTBKPORI;
                                    l_uPagSubTBkp = EFSS_PAGESUBTYPE_LOGNEWESTBKPBKP;
                                    break;
                                }

                                default:
                                {
                                    l_uPagSubTOri = 0u;
                                    l_uPagSubTBkp = 0u;
                                    l_eRes = e_eFSS_LOGC_RES_BADPARAM;
                                    break;
                                }
                            }

                            if( e_eFSS_LOGC_RES_OK == l_eRes )
                            {
                                l_eRes = eFSS_LOGC_LoadBuff(p_ptCtx, p_ptCtx->bFullBckup, p_puFillInPage, p_uIdx,
                                                            ( l_uNPageU + p_uIdx ), l_uPagSubTOri, l_uPagSubTBkp);
                            }
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGC_IsPageNewOrBkup(t_eFSS_LOGC_Ctx* const p_ptCtx, const uint32_t p_uIdx,
                                          bool_t* const p_pbIsNewest)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local storage variable */
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_COREHL_StorBuf l_tBuff;

    /* Local variable for calculation */
    bool_t l_bIsInit;
    uint32_t l_uByteUsed;
    uint32_t l_uByteUsedOff;
    uint8_t l_uPageSubTypeRed;
    uint32_t l_uNPageU;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsNewest ) )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
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
                    l_eResHL = eFSS_COREHL_GetBuffNStor(&p_ptCtx->tCOREHLCtx, &l_tBuff, &l_tStorSet);
                    l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                    {
                        /* Calculate n page and byte offset  */
                        l_uNPageU = eFSS_LOGC_GetMaxPage(p_ptCtx->bFullBckup, p_ptCtx->bFlashCache,
                                                         l_tStorSet.uTotPages);

                        if( p_uIdx >= l_uNPageU )
                        {
                            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
                        }
                        else
                        {
                            /* Calculate redByteOffset */
                            l_uByteUsedOff = l_tBuff.uBufL - EFSS_LOGC_PAGEMIN_L;

                            /* Start reading original page, if it's corrupted read the backup one */
                            l_uPageSubTypeRed = 0xFFu;
                            l_eResHL = eFSS_COREHL_LoadPageInBuff(&p_ptCtx->tCOREHLCtx, p_uIdx, &l_uPageSubTypeRed);
                            l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

                            if( e_eFSS_LOGC_RES_OK == l_eRes )
                            {
                                /* Check subtype */
                                if( ( EFSS_PAGESUBTYPE_LOGNEWESTORI    != l_uPageSubTypeRed ) &&
                                    ( EFSS_PAGESUBTYPE_LOGNEWESTBKPORI != l_uPageSubTypeRed ) )
                                {
                                    /* Not what we are searching */
                                    l_eRes = e_eFSS_LOGC_RES_NOTVALIDLOG;
                                }
                                else
                                {
                                    /* Retrive parameter */
                                    if( true != eFSS_Utils_RetriveU32(&l_tBuff.puBuf[l_uByteUsedOff], &l_uByteUsed) )
                                    {
                                        l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
                                    }
                                    else
                                    {
                                        /* Check validity */
                                        if( l_uByteUsed > ( l_tBuff.uBufL - EFSS_LOGC_PAGEMIN_L ) )
                                        {
                                            l_eRes = e_eFSS_LOGC_RES_NOTVALIDLOG;
                                        }
                                        else
                                        {
                                            if( EFSS_PAGESUBTYPE_LOGNEWESTORI == l_uPageSubTypeRed )
                                            {
                                                *p_pbIsNewest = true;
                                            }
                                            else
                                            {
                                                *p_pbIsNewest = false;
                                            }
                                        }
                                    }
                                }
                            }

                            /* Not valid? search on the packup pages */
                            if( ( ( e_eFSS_LOGC_RES_NOTVALIDLOG == l_eRes ) ||
                                  ( e_eFSS_LOGC_RES_NEWVERSIONFOUND == l_eRes ) ) &&
                                ( true == p_ptCtx->bFullBckup ) )
                            {
                                l_uPageSubTypeRed = 0xFFu;
                                l_eResHL = eFSS_COREHL_LoadPageInBuff(&p_ptCtx->tCOREHLCtx, ( l_uNPageU + p_uIdx ),
                                                                      &l_uPageSubTypeRed);
                                l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

                                if( e_eFSS_LOGC_RES_OK == l_eRes )
                                {
                                    /* Check subtype */
                                    if( ( EFSS_PAGESUBTYPE_LOGNEWESTBKP    != l_uPageSubTypeRed ) &&
                                        ( EFSS_PAGESUBTYPE_LOGNEWESTBKPBKP != l_uPageSubTypeRed ) )
                                    {
                                        /* Not what we are searching */
                                        l_eRes = e_eFSS_LOGC_RES_NOTVALIDLOG;
                                    }
                                    else
                                    {
                                        /* Retrive parameter */
                                        if( true != eFSS_Utils_RetriveU32(&l_tBuff.puBuf[l_uByteUsedOff],
                                                                          &l_uByteUsed) )
                                        {
                                            l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
                                        }
                                        else
                                        {
                                            /* Check validity */
                                            if( l_uByteUsed > ( l_tBuff.uBufL - EFSS_LOGC_PAGEMIN_L ) )
                                            {
                                                l_eRes = e_eFSS_LOGC_RES_NOTVALIDLOG;
                                            }
                                            else
                                            {
                                                if( EFSS_PAGESUBTYPE_LOGNEWESTBKP == l_uPageSubTypeRed )
                                                {
                                                    *p_pbIsNewest = true;
                                                }
                                                else
                                                {
                                                    *p_pbIsNewest = false;
                                                }
                                            }
                                        }
                                    }
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

e_eFSS_LOGC_RES eFSS_LOGC_FlushBuffIfNotEquals(t_eFSS_LOGC_Ctx* const p_ptCtx, const uint32_t p_uIdx,
                                               const e_eFSS_LOGC_PAGETYPE p_eTypeFlush)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local storage variable */
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_COREHL_StorBuf l_tBuff;
    uint32_t l_uNPageU;

   /* Local variable for calculation */
    bool_t l_bIsInit;
    bool_t l_bAreEquals;
    uint8_t l_uPagSubTOri;
    uint8_t l_uPagSubTBkp;
    uint32_t l_uByteInPage;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
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
                    l_eResHL = eFSS_COREHL_GetBuffNStor(&p_ptCtx->tCOREHLCtx, &l_tBuff, &l_tStorSet);
                    l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                    {
                        /* Calculate n page and byte offset  */
                        l_uNPageU = eFSS_LOGC_GetMaxPage(p_ptCtx->bFullBckup, p_ptCtx->bFlashCache,
                                                         l_tStorSet.uTotPages);

                        if( p_uIdx >= l_uNPageU )
                        {
                            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
                        }
                        else
                        {
                            switch(p_eTypeFlush)
                            {
                                case e_eFSS_LOGC_PAGETYPE_LOG:
                                {
                                    l_uPagSubTOri = EFSS_PAGESUBTYPE_LOGORI;
                                    l_uPagSubTBkp = EFSS_PAGESUBTYPE_LOGBKP;
                                    break;
                                }

                                case e_eFSS_LOGC_PAGETYPE_NEWEST:
                                {
                                    l_uPagSubTOri = EFSS_PAGESUBTYPE_LOGNEWESTORI;
                                    l_uPagSubTBkp = EFSS_PAGESUBTYPE_LOGNEWESTBKP;
                                    break;
                                }

                                case e_eFSS_LOGC_PAGETYPE_NEWEST_BKUP:
                                {
                                    l_uPagSubTOri = EFSS_PAGESUBTYPE_LOGNEWESTBKPORI;
                                    l_uPagSubTBkp = EFSS_PAGESUBTYPE_LOGNEWESTBKPBKP;
                                    break;
                                }

                                default:
                                {
                                    l_uPagSubTOri = 0u;
                                    l_uPagSubTBkp = 0u;
                                    l_eRes = e_eFSS_LOGC_RES_BADPARAM;
                                    break;
                                }
                            }

                            if( e_eFSS_LOGC_RES_OK == l_eRes )
                            {
                                l_bAreEquals = false;
                                l_eResHL = eFSS_COREHL_IsBuffEqualToPage(&p_ptCtx->tCOREHLCtx, p_uIdx, &l_bAreEquals,
                                                                         NULL);
                                l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

                                if( e_eFSS_LOGC_RES_OK == l_eRes )
                                {
                                    /* Retrive parameter */
                                    if( true != eFSS_Utils_RetriveU32(&l_tBuff.puBuf[4u], &l_uByteInPage) )
                                    {
                                        l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
                                    }
                                    else
                                    {
                                        if( false == l_bAreEquals )
                                        {
                                            /* Not equal! Flush buffer here */
                                            l_eRes = eFSS_LOGC_FlushBuff(p_ptCtx, p_ptCtx->bFullBckup, l_uByteInPage,
                                                                        p_uIdx, ( l_uNPageU + p_uIdx ), l_uPagSubTOri,
                                                                        l_uPagSubTBkp);
                                        }
                                    }
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



/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eFSS_LOGC_IsStatusStillCoherent(t_eFSS_LOGC_Ctx* const p_ptCtx)
{
    /* Local return variable */
    bool_t l_bRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local storage variable */
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_COREHL_StorBuf l_tBuff;

    /* Local variable for calculation */
    uint32_t l_uNPage;

    l_eResHL = eFSS_COREHL_GetBuffNStor(&p_ptCtx->tCOREHLCtx, &l_tBuff, &l_tStorSet);
    if( e_eFSS_COREHL_RES_OK != l_eResHL )
    {
        l_bRes = false;
    }
    else
    {
        l_uNPage = l_tStorSet.uTotPages;

        /* Check numbers of page validity */
        if( ( ( false == p_ptCtx->bFullBckup ) && ( false == p_ptCtx->bFlashCache ) && ( l_uNPage < 4u  ) ) ||
            ( ( false == p_ptCtx->bFullBckup ) && ( true  == p_ptCtx->bFlashCache ) && ( l_uNPage < 6u  ) ) ||
            ( ( true  == p_ptCtx->bFullBckup ) && ( false == p_ptCtx->bFlashCache ) && ( l_uNPage < 8u  ) ) ||
            ( ( true  == p_ptCtx->bFullBckup ) && ( true  == p_ptCtx->bFlashCache ) && ( l_uNPage < 10u ) ) ||
            ( ( true  == p_ptCtx->bFullBckup ) && ( 0u != ( l_uNPage % 2u ) ) ) )
        {
            l_bRes = false;
        }
        else
        {
            if( ( ( l_tBuff.uBufL <= EFSS_LOGC_PAGEMIN_L  ) && ( false == p_ptCtx->bFlashCache ) ) ||
                ( ( l_tBuff.uBufL <= EFSS_LOGC_CACHEMIN_L ) && ( true == p_ptCtx->bFlashCache  ) ) )
            {
                l_bRes = false;
            }
            else
            {
                l_bRes = true;
            }
        }
    }

    return l_bRes;
}

static e_eFSS_LOGC_RES eFSS_LOGC_HLtoLOGCRes(const e_eFSS_COREHL_RES p_eHLRes)
{
    /* Local return variable */
    e_eFSS_LOGC_RES l_eRes;

    switch(p_eHLRes)
    {
        case e_eFSS_COREHL_RES_OK:
        {
            l_eRes = e_eFSS_LOGC_RES_OK;
            break;
        }

        case e_eFSS_COREHL_RES_NOINITLIB:
        {
            l_eRes = e_eFSS_LOGC_RES_NOINITLIB;
            break;
        }

        case e_eFSS_COREHL_RES_BADPARAM:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
            break;
        }

        case e_eFSS_COREHL_RES_BADPOINTER:
        {
            l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
            break;
        }

        case e_eFSS_COREHL_RES_CORRUPTCTX:
        {
            l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
            break;
        }

        case e_eFSS_COREHL_RES_CLBCKERASEERR:
        {
            l_eRes = e_eFSS_LOGC_RES_CLBCKERASEERR;
            break;
        }

        case e_eFSS_COREHL_RES_CLBCKWRITEERR:
        {
            l_eRes = e_eFSS_LOGC_RES_CLBCKWRITEERR;
            break;
        }

        case e_eFSS_COREHL_RES_CLBCKREADERR:
        {
            l_eRes = e_eFSS_LOGC_RES_CLBCKREADERR;
            break;
        }

        case e_eFSS_COREHL_RES_CLBCKCRCERR:
        {
            l_eRes = e_eFSS_LOGC_RES_CLBCKCRCERR;
            break;
        }

        case e_eFSS_COREHL_RES_NOTVALIDPAGE:
        {
            l_eRes = e_eFSS_LOGC_RES_NOTVALIDLOG;
            break;
        }

        case e_eFSS_COREHL_RES_NEWVERSIONFOUND:
        {
            l_eRes = e_eFSS_LOGC_RES_NEWVERSIONFOUND;
            break;
        }

        case e_eFSS_COREHL_RES_WRITENOMATCHREAD:
        {
            l_eRes = e_eFSS_LOGC_RES_WRITENOMATCHREAD;
            break;
        }

        case e_eFSS_COREHL_RES_OK_BKP_RCVRD:
        {
            l_eRes = e_eFSS_LOGC_RES_OK_BKP_RCVRD;
            break;
        }

        default:
        {
            l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
            break;
        }
    }

    return l_eRes;
}



/***********************************************************************************************************************
 *  PRIVATE STATIC UTILS FUNCTION DECLARATION
 **********************************************************************************************************************/
static uint32_t eFSS_LOGC_GetMaxPage(const bool_t p_bIsFullBkup, const bool_t p_bIsFCache, const uint32_t p_uTotPages)
{
    /* Local return variable */
	uint32_t l_uNPageU;

    /* Get the total numbers of page */
    l_uNPageU = p_uTotPages;

    /* This function suppose that the context is coherent, so be sure to call eFSS_LOGC_IsStatusStillCoherent before */
	/* Flash cache will use two flash pages */
    if( true == p_bIsFCache )
    {
        if( l_uNPageU >= 2u )
        {
            l_uNPageU -= 2u;
        }
        else
        {
            /* Impossible end here, data is checkd before, just put zero */
            l_uNPageU = 0u;
        }
    }

	/* Flash full bakup will use twice as pages as normal log */
    if( true == p_bIsFullBkup )
    {
        l_uNPageU = (uint32_t)( l_uNPageU / 2u );
    }

    return l_uNPageU;
}

static e_eFSS_LOGC_RES eFSS_LOGC_FlushBuff(t_eFSS_LOGC_Ctx* const p_ptCtx,
                                           const bool_t p_bIsBkpP, const uint32_t p_uByteUse,
								           const uint32_t p_uOrigIdx, const uint32_t p_uBackupIdx,
                                           const uint8_t p_uOriSubT, const uint8_t p_uBckUpSubT)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for storage */
    t_eFSS_COREHL_StorBuf l_tBuff;

    /* This function suppose that the context is coherent, so be sure to call eFSS_LOGC_IsStatusStillCoherent
       before */
    l_eResHL = eFSS_COREHL_GetBuff(&p_ptCtx->tCOREHLCtx, &l_tBuff);
    l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

    if( e_eFSS_LOGC_RES_OK == l_eRes )
    {
        /* Check data validity */
        if( p_uByteUse > ( l_tBuff.uBufL - EFSS_LOGC_PAGEMIN_L ) )
        {
            /* Cannot fill more data than possible */
            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
        }
        else
        {
            /* Insert Meta */
            if( true != eFSS_Utils_InsertU32(&l_tBuff.puBuf[l_tBuff.uBufL - EFSS_LOGC_PAGEMIN_L], p_uByteUse) )
            {
                l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
            }
            else
            {
                if( true == p_bIsBkpP )
                {
                    /* Flush */
                    l_eResHL = eFSS_COREHL_FlushBuffInPageNBkp(&p_ptCtx->tCOREHLCtx, p_uOrigIdx, p_uBackupIdx,
                                                               p_uOriSubT, p_uBckUpSubT);

                    l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);
                }
                else
                {
                    /* Flush */
                    l_eResHL = eFSS_COREHL_FlushBuffInPage(&p_ptCtx->tCOREHLCtx, p_uOrigIdx, p_uOriSubT);
                    l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);
                }
            }
        }
    }

    return l_eRes;
}

static e_eFSS_LOGC_RES eFSS_LOGC_LoadBuff(t_eFSS_LOGC_Ctx* const p_ptCtx,
                                          const bool_t p_bIsBkpP, uint32_t* const p_puByteUse,
                                          const uint32_t p_uOrigIdx, const uint32_t p_uBackupIdx,
                                          const uint8_t p_uOriSubT, const uint8_t p_uBckUpSubT)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for storage */
    t_eFSS_COREHL_StorBuf l_tBuff;
    uint8_t l_uPageSubTypeRed;

    if( NULL == p_puByteUse )
    {
        l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
    }
    else
    {
        /* This function suppose that the context is coherent, so be sure to call eFSS_LOGC_IsStatusStillCoherent
           before */
        l_eResHL = eFSS_COREHL_GetBuff(&p_ptCtx->tCOREHLCtx, &l_tBuff);
        l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            if( true == p_bIsBkpP )
            {
                l_eResHL = eFSS_COREHL_LoadPageInBuffNRipBkp(&p_ptCtx->tCOREHLCtx, p_uOrigIdx, p_uBackupIdx,
                                                             p_uOriSubT, p_uBckUpSubT);
                l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);
            }
            else
            {
                l_uPageSubTypeRed = 0x00u;
                l_eResHL = eFSS_COREHL_LoadPageInBuff(&p_ptCtx->tCOREHLCtx, p_uOrigIdx, &l_uPageSubTypeRed);
                l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

                if( e_eFSS_LOGC_RES_OK == l_eRes )
                {
                    /* Check subtype */
                    if( p_uOriSubT != l_uPageSubTypeRed )
                    {
                        l_eRes = e_eFSS_LOGC_RES_NOTVALIDLOG;
                    }
                }
            }

            if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) )
            {
                /* Retrive parameter */
                if( true != eFSS_Utils_RetriveU32(&l_tBuff.puBuf[l_tBuff.uBufL - EFSS_LOGC_PAGEMIN_L], p_puByteUse) )
                {
                    l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
                }
                else
                {
                    /* Need to verify parameter before confirm the validity of the page */
                    if( *p_puByteUse > ( l_tBuff.uBufL - EFSS_LOGC_PAGEMIN_L ) )
                    {
                        l_eRes = e_eFSS_LOGC_RES_NOTVALIDLOG;
                    }
                }
            }
        }
    }

    return l_eRes;
}