/**
 * @file       eFSS_LOGCPRV.c
 *
 * @brief      LOG Core private module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/* In this module the page field has the following meaning:
 * ------------------------------------------------------------------ User data
 * - [uint8_t] -                    -> N byte of user data           |
 * ------------------------------------------------------------------ Metadata  (17 byte)
 * - uint32_t  - uPageUseSpec1      -> New page Index                |
 * - uint32_t  - uPageUseSpec2      -> Number of Filled index page   |
 * - uint32_t  - uPageUseSpec3      -> Valorized byte in page        |
 * - uint32_t  - uPageUseSpec4      -> No value defined              |
 * - uint8_t   - uPageSubType       -> Page subtype                  |
 * ------------------------------------------------------------------+
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
 * An unused page is always left after newer page and backup index.
 * Newest and oldest page index can be identical only when the log storage is empty. When the storage is full an empty
 * page is always present between new and old log index.
 */

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_LOGCPRV.h"
#include "eFSS_COREHL.h"


/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static e_eFSS_LOGC_RES eFSS_LOGCPRV_FlushBuffInPageNBkp(t_eFSS_LOGC_Ctx* const p_ptCtx, bool_t p_bIsBkpPresent,
								                        const uint32_t p_uOrigIndx, const uint32_t p_uBackupIndx,
                                                        const uint32_t p_uOriSubType, const uint32_t p_uBckUpSubType);

static e_eFSS_LOGC_RES eFSS_LOGCPRV_LoadPageInBuffNRipBkp(t_eFSS_LOGC_Ctx* const p_ptCtx, bool_t p_bIsBkpPresent,
                                                          const uint32_t p_uOrigIndx, const uint32_t p_uBackupIndx,
                                                          const uint32_t p_uOriSubType, const uint32_t p_uBckUpSubType);


/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
bool_t eFSS_LOGCPRV_IsStatusStillCoherent(t_eFSS_LOGC_Ctx* p_ptCtx)
{
    /* Return local var */
    bool_t l_bRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint32_t l_uNPage;

    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
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
            ( ( true  == p_ptCtx->bFullBckup ) && ( true  == p_ptCtx->bFlashCache ) && ( l_uNPage < 10u ) ) )
        {
            l_bRes = false;
        }
        else
        {
            /* Che New index validity */
            if( ( ( false == p_ptCtx->bFullBckup ) && ( false == p_ptCtx->bFlashCache ) &&
                  ( p_ptCtx->uNewPagIdx >= l_uNPage ) ) ||
                ( ( false == p_ptCtx->bFullBckup ) && ( true  == p_ptCtx->bFlashCache ) &&
                  ( p_ptCtx->uNewPagIdx >= ( l_uNPage - 2u ) ) ) ||
                ( ( true  == p_ptCtx->bFullBckup ) && ( false == p_ptCtx->bFlashCache ) &&
                  ( p_ptCtx->uNewPagIdx >= ( l_uNPage / 2u ) ) ) ||
                ( ( true  == p_ptCtx->bFullBckup ) && ( true  == p_ptCtx->bFlashCache ) &&
                  ( p_ptCtx->uNewPagIdx >= ( ( l_uNPage - 2u ) / 2u ) ) ) )
            {
                l_bRes = false;
            }
            else
            {
                /* Check N filled page validity */
                if( ( ( false == p_ptCtx->bFullBckup ) && ( false == p_ptCtx->bFlashCache ) &&
                      ( p_ptCtx->uFullFilledP >= l_uNPage - 3u ) ) ||
                    ( ( false == p_ptCtx->bFullBckup ) && ( true  == p_ptCtx->bFlashCache ) &&
                      ( p_ptCtx->uFullFilledP >= ( l_uNPage - 2u - 3u ) ) ) ||
                    ( ( true  == p_ptCtx->bFullBckup ) && ( false == p_ptCtx->bFlashCache ) &&
                      ( p_ptCtx->uFullFilledP >= ( ( l_uNPage / 2u ) - 3u ) ) ) ||
                    ( ( true  == p_ptCtx->bFullBckup ) && ( true  == p_ptCtx->bFlashCache ) &&
                      ( p_ptCtx->uFullFilledP >= ( ( ( l_uNPage - 2u ) / 2u ) ) -3u ) ) )
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

    return l_bRes;
}

e_eFSS_LOGC_RES eFSS_LOGCPRV_HLtoLogRes(const e_eFSS_COREHL_RES p_eHLRes)
{
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
            l_eRes = e_eFSS_LOGC_RES_NEWVERSIONLOG;
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

uint32_t eFSS_LOGCPRV_GetUsablePage(const t_eFSS_LOGC_Ctx* p_ptCtx, t_eFSS_TYPE_StorSet p_tStorSet)
{
    /* Local variable for return */
	uint32_t l_uNPageU;

    /* Get the total numbers of page */
    l_uNPageU = p_tStorSet.uTotPages;

	/* Flash cache will use two flash pages */
    if( true == p_ptCtx->bFlashCache )
    {
        l_uNPageU -= 2u;
    }

	/* Flash full bakup will use twice as pages as normal log */
    if( true == p_ptCtx->bFullBckup )
    {
        l_uNPageU = (uint32_t)( l_uNPageU / 2u );
    }

    return l_uNPageU;
}

uint32_t eFSS_LOGCPRV_GetNextIndex(const t_eFSS_LOGC_Ctx* p_ptCtx, t_eFSS_TYPE_StorSet p_tStorSet, uint32_t p_uIdx)
{
    /* Local variable for return */
    uint32_t l_uNextIdx;

    /* Local var for calc */
	uint32_t l_uNPageU;

    /* Calculate the numbers of usable page */
    l_uNPageU = eFSS_LOGCPRV_GetUsablePage(p_ptCtx, p_tStorSet);

    if( p_uIdx >= ( l_uNPageU - 1u ) )
    {
        l_uNextIdx = 0u;
    }
    else
    {
        l_uNextIdx = p_uIdx + 1u;
    }

    return l_uNextIdx;
}

uint32_t eFSS_LOGCPRV_GetPrevIndex(const t_eFSS_LOGC_Ctx* p_ptCtx, t_eFSS_TYPE_StorSet p_tStorSet, uint32_t p_uIdx)
{
    /* Local var for calc */
    uint32_t l_uPrevIdx;

    /* Local var for calc */
	uint32_t l_uNPageU;

    /* Calculate n page */
    l_uNPageU = eFSS_LOGCPRV_GetUsablePage(p_ptCtx, p_tStorSet);

    if( p_uIdx <= 0u )
    {
        l_uPrevIdx = l_uNPageU - 1u;
    }
    else
    {
        l_uPrevIdx = p_uIdx - 1u;
    }

    return l_uPrevIdx;
}

e_eFSS_LOGC_RES eFSS_LOGCPRV_WriteCache(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t p_uIdxN, uint32_t p_uIFlP)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_TYPE_StorBuf l_tBuff;

    /* Local calc variable */
	uint32_t l_uCachePageIdx;

    l_eResHL =  eFSS_COREHL_GetBuffNStor(&p_ptCtx->tCOREHLCtx, &l_tBuff, &l_tStorSet);
    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

    if( e_eFSS_LOGC_RES_OK == l_eRes )
    {
        /* Clear data */
        memset(l_tBuff.puBuf,  0u,  l_tBuff.uBufL);
        memset(l_tBuff.ptMeta, 0u,  sizeof(t_eFSS_TYPE_PageMeta));

        /* Set Metadata */
        l_tBuff.ptMeta->uPageUseSpec1 = p_uIdxN;
        l_tBuff.ptMeta->uPageUseSpec2 = p_uIFlP;
        l_tBuff.ptMeta->uPageUseSpec3 = 0u;
        l_tBuff.ptMeta->uPageUseSpec4 = 0u;

        /* Setup index */
        l_uCachePageIdx = l_tStorSet.uTotPages - 2u;

        /* Flush */
        l_eRes = eFSS_LOGCPRV_FlushBuffInPageNBkp(p_ptCtx, true, l_uCachePageIdx, (l_uCachePageIdx + 1u),
                                                  EFSS_PAGESUBTYPE_LOGCACHEORI, EFSS_PAGESUBTYPE_LOGCACHEBKP);
    }

    return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGCPRV_ReadCache(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t* p_puIdxN, uint32_t* p_puIFlP)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_TYPE_StorBuf l_tBuff;

    /* Local calc variable */
	uint32_t l_uOriPageIdx;
    uint32_t l_uBkupPageIdx;

    /* Get storage settings */
    l_eResHL =  eFSS_COREHL_GetBuffNStor(&p_ptCtx->tCOREHLCtx, &l_tBuff, &l_tStorSet);
    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

    if( e_eFSS_LOGC_RES_OK == l_eRes )
    {
        /* Setup index */
        l_uOriPageIdx = l_tStorSet.uTotPages  - 2u;
        l_uBkupPageIdx = l_tStorSet.uTotPages - 1u;

        /* Before reading fix any error in original and backup pages */
        l_eRes =  eFSS_LOGCPRV_LoadPageInBuffNRipBkp(p_ptCtx, true, l_uOriPageIdx, l_uBkupPageIdx,
                                                     EFSS_PAGESUBTYPE_LOGCACHEORI, EFSS_PAGESUBTYPE_LOGCACHEBKP);

        if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) )
        {
            /* Can read data */
            *p_puIdxN = l_tBuff.ptMeta->uPageUseSpec1;
            *p_puIFlP = l_tBuff.ptMeta->uPageUseSpec2;
        }
    }

    return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGCPRV_GetBuffer(t_eFSS_LOGC_Ctx* const p_ptCtx, t_eFSS_TYPE_StorBuf* p_ptBuff)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Get storage area, we need to return only the first buffer */
    l_eResHL = eFSS_COREHL_GetBuff(&p_ptCtx->tCOREHLCtx, p_ptBuff);
    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

    return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGCPRV_FlushBufferAsLog(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t p_uIdx)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for storage */
    t_eFSS_TYPE_StorSet l_tStorSet;

    /* Local calc variable */
	uint32_t l_uNPageU;

    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

    if( e_eFSS_LOGC_RES_OK == l_eRes )
    {
        /* Calculate n page */
        l_uNPageU = eFSS_LOGCPRV_GetUsablePage(p_ptCtx, l_tStorSet);

        l_eRes = eFSS_LOGCPRV_FlushBuffInPageNBkp(p_ptCtx, p_ptCtx->bFullBckup, p_uIdx, (l_uNPageU + p_uIdx),
                                                  EFSS_PAGESUBTYPE_LOGORI, EFSS_PAGESUBTYPE_LOGBKP);
    }

    return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGCPRV_LoadBufferAsLog(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t p_uIdx)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for storage */
    t_eFSS_TYPE_StorSet l_tStorSet;

    /* Local calc variable */
	uint32_t l_uOriPageIdx;
    uint32_t l_uBkupPageIdx;
    uint32_t l_uNPageU;

    /* Get storage settings */
    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

    if( e_eFSS_LOGC_RES_OK == l_eRes )
    {
        /* Calculate n page */
        l_uNPageU = eFSS_LOGCPRV_GetUsablePage(p_ptCtx, l_tStorSet);

        /* Setup index */
        l_uOriPageIdx = p_uIdx;
        l_uBkupPageIdx = l_uNPageU + p_uIdx;

        l_eRes =  eFSS_LOGCPRV_LoadPageInBuffNRipBkp(p_ptCtx, p_ptCtx->bFullBckup, l_uOriPageIdx, l_uBkupPageIdx,
                                                     EFSS_PAGESUBTYPE_LOGORI, EFSS_PAGESUBTYPE_LOGBKP);
    }

    return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGCPRV_FlushBufferAsNewestOnly(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t p_uIdx)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for storage */
    t_eFSS_TYPE_StorSet l_tStorSet;

    /* Local calc variable */
	uint32_t l_uNPageU;

    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

    if( e_eFSS_LOGC_RES_OK == l_eRes )
    {
        /* Calculate n page */
        l_uNPageU = eFSS_LOGCPRV_GetUsablePage(p_ptCtx, l_tStorSet);

        l_eRes = eFSS_LOGCPRV_FlushBuffInPageNBkp(p_ptCtx, p_ptCtx->bFullBckup, p_uIdx, (l_uNPageU + p_uIdx),
                                                  EFSS_PAGESUBTYPE_LOGNEWESTORI, EFSS_PAGESUBTYPE_LOGNEWESTBKP);
    }

    return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGCPRV_FlushBufferAsNewestBkupOnly(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t p_uIdx)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for storage */
    t_eFSS_TYPE_StorSet l_tStorSet;

    /* Local calc variable */
	uint32_t l_uNPageU;

    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

    if( e_eFSS_LOGC_RES_OK == l_eRes )
    {
        /* Calculate n page */
        l_uNPageU = eFSS_LOGCPRV_GetUsablePage(p_ptCtx, l_tStorSet);

        l_eRes = eFSS_LOGCPRV_FlushBuffInPageNBkp(p_ptCtx, p_ptCtx->bFullBckup, p_uIdx, (l_uNPageU + p_uIdx),
                                                  EFSS_PAGESUBTYPE_LOGNEWESTBKPORI, EFSS_PAGESUBTYPE_LOGNEWESTBKPBKP);
    }

    return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGCPRV_FlushBufferAsNewestNBkpPage(t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t p_uIdx)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for storage */
    t_eFSS_TYPE_StorSet l_tStorSet;

    /* Local calc variable */
	uint32_t l_uNPageU;
    uint32_t l_uBkupPage;

    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

    if( e_eFSS_LOGC_RES_OK == l_eRes )
    {
        /* Calculate n page */
        l_uNPageU = eFSS_LOGCPRV_GetUsablePage(p_ptCtx, l_tStorSet);

        /* Save original newest page */
        l_eRes = eFSS_LOGCPRV_FlushBuffInPageNBkp(p_ptCtx, p_ptCtx->bFullBckup, p_uIdx, (l_uNPageU + p_uIdx),
                                                  EFSS_PAGESUBTYPE_LOGNEWESTORI, EFSS_PAGESUBTYPE_LOGNEWESTBKP);

        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            /* Flush */
            l_uBkupPage = eFSS_LOGCPRV_GetNextIndex(p_ptCtx, l_tStorSet, p_uIdx);

            /* Save newest backup page */
            l_eRes = eFSS_LOGCPRV_FlushBuffInPageNBkp(p_ptCtx, p_ptCtx->bFullBckup, l_uBkupPage,
                                                      (l_uNPageU + l_uBkupPage), EFSS_PAGESUBTYPE_LOGNEWESTBKPORI,
                                                      EFSS_PAGESUBTYPE_LOGNEWESTBKPBKP);
        }
    }

    return l_eRes;
}

e_eFSS_LOGC_RES eFSS_LOGCPRV_LoadBufferAsNewestNBkpPage(t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t p_uIdx)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for storage */
    t_eFSS_TYPE_StorSet l_tStorSet;

    /* Local calc variable */
	uint32_t l_uOriPageIdx;
    uint32_t l_uBkupPageIdx;
    uint32_t l_uNPageU;

    /* page status local var */
    bool_t l_bIsOrigValid;
    bool_t l_bIsBkupValid;

    /* Get storage settings */
    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

    if( e_eFSS_LOGC_RES_OK == l_eRes )
    {
        /* Calculate n page */
        l_uNPageU = eFSS_LOGCPRV_GetUsablePage(p_ptCtx, l_tStorSet);

        /* Setup index */
        l_uOriPageIdx = p_uIdx;
        l_uBkupPageIdx = l_uNPageU + l_uOriPageIdx;

        /* Read newest page first */
        l_eRes =  eFSS_LOGCPRV_LoadPageInBuffNRipBkp(p_ptCtx, p_ptCtx->bFullBckup, l_uOriPageIdx, l_uBkupPageIdx,
                                                     EFSS_PAGESUBTYPE_LOGNEWESTORI, EFSS_PAGESUBTYPE_LOGNEWESTBKP);

        if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) ||
            ( e_eFSS_LOGC_RES_NOTVALIDLOG == l_eRes ) )
        {
            /* Page readed, is valid? */
            if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) )
            {
                l_bIsOrigValid = true;
            }
            else
            {
                l_bIsOrigValid = false;
            }

            /* Setup index */
            l_uOriPageIdx = eFSS_LOGCPRV_GetNextIndex(p_ptCtx, l_tStorSet, p_uIdx);
            l_uBkupPageIdx = l_uNPageU + l_uOriPageIdx;

            /* Read backup pages now */
            l_eRes =  eFSS_LOGCPRV_LoadPageInBuffNRipBkp(p_ptCtx, p_ptCtx->bFullBckup, l_uOriPageIdx, l_uBkupPageIdx,
                                                        EFSS_PAGESUBTYPE_LOGNEWESTBKPORI, EFSS_PAGESUBTYPE_LOGNEWESTBKPBKP);

            if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) ||
                ( e_eFSS_LOGC_RES_NOTVALIDLOG == l_eRes ) )
            {
                /* Page readed, is valid? */
                if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) )
                {
                    l_bIsBkupValid = true;
                }
                else
                {
                    l_bIsBkupValid = false;
                }

                /* We have all the data needed to make a decision */
                if( ( false == l_bIsOrigValid ) && ( false == l_bIsBkupValid ) )
                {
                    /* No a single valid pages found */
                    l_eRes = e_eFSS_LOGC_RES_NOTVALIDLOG;
                }
                else
                {
                    /* Need to cross do verifycation now */
                    /* Setup index */
                    l_uOriPageIdx  = p_uIdx;
                    l_uBkupPageIdx = eFSS_LOGCPRV_GetNextIndex(p_ptCtx, l_tStorSet, l_uOriPageIdx);;

                    /* Read newest page first */
                    l_eRes =  eFSS_LOGCPRV_LoadPageInBuffNRipBkp(p_ptCtx, true, l_uOriPageIdx, l_uBkupPageIdx,
                                                                EFSS_PAGESUBTYPE_LOGNEWESTORI, EFSS_PAGESUBTYPE_LOGNEWESTBKPORI);

                    if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) )
                    {
                        if( true == p_ptCtx->bFullBckup )
                        {
                            /* Need to cross do verifycation now */
                            /* Setup index */
                            l_uOriPageIdx  = p_uIdx + l_uNPageU;
                            l_uBkupPageIdx = eFSS_LOGCPRV_GetNextIndex(p_ptCtx, l_tStorSet, l_uOriPageIdx);;

                            /* Read newest page first */
                            l_eRes =  eFSS_LOGCPRV_LoadPageInBuffNRipBkp(p_ptCtx, true, l_uOriPageIdx, l_uBkupPageIdx,
                                                                        EFSS_PAGESUBTYPE_LOGNEWESTBKP, EFSS_PAGESUBTYPE_LOGNEWESTBKPBKP);
                        }
                    }
                }
            }
        }
    }

    return l_eRes;
}



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static e_eFSS_LOGC_RES eFSS_LOGCPRV_FlushBuffInPageNBkp(t_eFSS_LOGC_Ctx* const p_ptCtx, bool_t p_bIsBkpPresent,
								                        const uint32_t p_uOrigIndx, const uint32_t p_uBackupIndx,
                                                        const uint32_t p_uOriSubType, const uint32_t p_uBckUpSubType)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for storage */
    t_eFSS_TYPE_StorBuf l_tBuff;

    l_eResHL =  eFSS_COREHL_GetBuff(&p_ptCtx->tCOREHLCtx, &l_tBuff);
    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

    if( e_eFSS_LOGC_RES_OK == l_eRes )
    {
        if( true == p_bIsBkpPresent )
        {
            /* Flush */
            l_eResHL = eFSS_COREHL_FlushBuffInPageNBkp(&p_ptCtx->tCOREHLCtx, p_uOrigIndx, p_uBackupIndx,
                                                       p_uOriSubType, p_uBckUpSubType);

            l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);
        }
        else
        {
            /* Flush */
            l_tBuff.ptMeta->uPageSubType = p_uOriSubType;
            l_eResHL = eFSS_COREHL_FlushBuffInPage(&p_ptCtx->tCOREHLCtx, p_uOrigIndx);
            l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);
        }
    }

    return l_eRes;
}

static e_eFSS_LOGC_RES eFSS_LOGCPRV_LoadPageInBuffNRipBkp(t_eFSS_LOGC_Ctx* const p_ptCtx, bool_t p_bIsBkpPresent,
                                                          const uint32_t p_uOrigIndx, const uint32_t p_uBackupIndx,
                                                          const uint32_t p_uOriSubType, const uint32_t p_uBckUpSubType)
{
	/* Local return variable */
	e_eFSS_LOGC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for storage */
    t_eFSS_TYPE_StorBuf l_tBuff;

    /* Get storage settings */
    l_eResHL =  eFSS_COREHL_GetBuff(&p_ptCtx->tCOREHLCtx, &l_tBuff);
    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

    if( e_eFSS_LOGC_RES_OK == l_eRes )
    {
        if( true == p_ptCtx->bFullBckup )
        {
            /* Before reading fix any error in original and backup pages */
            l_eResHL = eFSS_COREHL_LoadPageInBuffNRipBkp(&p_ptCtx->tCOREHLCtx, p_uOrigIndx, p_uBackupIndx,
                                                        p_uOriSubType, p_uBckUpSubType );
            l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);
        }
        else
        {
            l_eResHL =  eFSS_COREHL_LoadPageInBuff(&p_ptCtx->tCOREHLCtx, p_uOrigIndx);
            l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eResHL);

            if( e_eFSS_LOGC_RES_OK == l_eRes )
            {
                /* Check subtype */
                if( p_uOriSubType != l_tBuff.ptMeta->uPageSubType )
                {
                    l_eRes = e_eFSS_LOGC_RES_NOTVALIDLOG;
                }
            }
        }
    }

    return l_eRes;
}