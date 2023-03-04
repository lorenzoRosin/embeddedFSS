/**
 * @file       eFSS_BLOBC.c
 *
 * @brief      Blob Core - large file core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/* In this module the page field has the following meaning:
 * --------------------------------------------------------------------------------------------- User data
 * - [uint8_t] -                    -> N byte of user data                                      |
 * --------------------------------------------------------------------------------------------- Metadata  (17 byte)
 * - uint32_t  - uPageUseSpec1      -> (Valid only on the last page) Size of the stored blob    |
 * - uint32_t  - uPageUseSpec2      -> (Valid only on the last page) Crc of the stored blob     |
 * - uint32_t  - uPageUseSpec3      -> Sequential number of the page                            |
 * - uint32_t  - uPageUseSpec4      -> Page id                                                  |
 * - uint8_t   - uPageSubType       -> Page subtype                                             |
 * ---------------------------------------------------------------------------------------------+
 *
 * In this module the storage is organizated as follow :
 *
 * - [ 0                            -    ( uTotPages / 2 ) - 1 ]  -> Original pages
 * - [ ( uTotPages / 2 )            -    uTotPages - 1         ]  -> Backup pages
 *
 * First write original pages and after the backup pages
 * Only a valid blob last page has a valid size and and valid crc (because the crc and size calculation is updated at
 * the end).
 * The process of backup is done only when all the original pages are valid, so during a long file copy we can always
 * retrive a valid image.
 */



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_BLOBC.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_BLOBC_IsStatusStillCoherent(t_eFSS_BLOBC_Ctx* p_ptCtx);
static e_eFSS_BLOBC_RES eFSS_BLOBC_HLtoBLOBRes(const e_eFSS_COREHL_RES p_eHLRes);
static e_eFSS_BLOBC_RES eFSS_BLOBC_LoadBufferFromPage(t_eFSS_BLOBC_Ctx* p_ptCtx, t_eFSS_TYPE_StorBuf* p_ptBuff,
                                                      bool_t p_bIsOri, uint32_t p_uIdx);
static e_eFSS_BLOBC_RES eFSS_LOGC_FlushBufferInPage(t_eFSS_BLOBC_Ctx* p_ptCtx, bool_t p_bIsOri, uint32_t p_uBlobL,
                                                    uint32_t p_uCrc, uint32_t p_uSeqN, uint32_t p_uIdx);


/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_BLOBC_RES eFSS_BLOBC_InitCtx(t_eFSS_BLOBC_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx const p_tCtxCb,
                                    t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL)
{
    /* Return local var */
    e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    uint32_t l_uNPage;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
        l_uNPage = p_tStorSet.uTotPages;

        /* Check numbers of page validity */
        if( ( l_uNPage < 2u ) && ( 0u != ( l_uNPage % 2u ) ) )
        {
            l_eRes = e_eFSS_BLOBC_RES_BADPARAM;
        }
        else
        {
            /* Can init low level context */
            l_eResHL = eFSS_COREHL_InitCtx(&p_ptCtx->tCOREHLCtx, p_tCtxCb, p_tStorSet, p_puBuff, p_uBuffL);
            l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);
        }
    }

    return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_BLOBC_IsInit(t_eFSS_BLOBC_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, p_pbIsInit);
        l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_BLOBC_GetBuff(t_eFSS_BLOBC_Ctx* p_ptCtx, t_eFSS_TYPE_StorBuf* p_ptBuff)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ptBuff ) )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);

        if( e_eFSS_BLOBC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOBC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_BLOBC_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResHL = eFSS_COREHL_GetBuff(&p_ptCtx->tCOREHLCtx, p_ptBuff);
                    l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_BLOBC_GetUsablePage(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t* p_puUsableP)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
	uint32_t l_uNPageU;
    t_eFSS_TYPE_StorSet l_tStorSet;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puUsableP ) )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

        if( e_eFSS_BLOBC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOBC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOGC_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
                    l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);

                    if( e_eFSS_BLOBC_RES_OK == l_eRes )
                    {
                        /* Get the total numbers of page */
                        l_uNPageU = l_tStorSet.uTotPages;

                        l_uNPageU = (uint32_t)( l_uNPageU / 2u );
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_LOGC_GetOriginalPageMeta(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t* p_puBlobLen, uint32_t* p_puBlobCrc)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_TYPE_StorBuf l_tBuff;
	uint32_t l_uLastPageIdx;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBlobLen ) || ( NULL == p_puBlobCrc ) )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);

        if( e_eFSS_BLOBC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOBC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_BLOBC_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResHL =  eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
                    l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);

                    if( e_eFSS_BLOBC_RES_OK == l_eRes )
                    {
                        /* Get last page, where blob size and crc are store */
                        l_uLastPageIdx = ( l_tStorSet.uTotPages / 2u ) - 1u;

                        /* Before reading fix any error in original and backup pages */
                        l_eRes = eFSS_BLOBC_LoadBufferFromPage(p_ptCtx, &l_tBuff, true, l_uLastPageIdx);

                        if( e_eFSS_BLOBC_RES_OK == l_eRes )
                        {
                            *p_puBlobLen = l_tBuff.ptMeta->uPageUseSpec1;
                            *p_puBlobCrc = l_tBuff.ptMeta->uPageUseSpec2;
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_LOGC_SetOriginalPageMeta(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t p_uBlobLen, uint32_t p_uBlobCrc)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_TYPE_StorBuf l_tBuff;
	uint32_t l_uLastPageIdx;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);

        if( e_eFSS_BLOBC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOBC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_BLOBC_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResHL =  eFSS_COREHL_GetBuffNStor(&p_ptCtx->tCOREHLCtx, &l_tBuff, &l_tStorSet);
                    l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);

                    if( e_eFSS_BLOBC_RES_OK == l_eRes )
                    {
                        /* Get last page, where blob size and crc are store */
                        l_uLastPageIdx = ( l_tStorSet.uTotPages / 2u ) - 1u;

                        /* Before reading fix any error in original and backup pages */
                        l_eRes = eFSS_BLOBC_LoadBufferFromPage(p_ptCtx, &l_tBuff, true, l_uLastPageIdx);

                        if( e_eFSS_BLOBC_RES_OK == l_eRes )
                        {
                            l_eRes = eFSS_LOGC_FlushBufferInPage(p_ptCtx, true, p_uBlobLen, p_uBlobCrc,
                                                                 l_tBuff.ptMeta->uPageUseSpec3, l_uLastPageIdx);
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_LOGC_GetBckupPageMeta(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t* p_puBlobLen, uint32_t* p_puBlobCrc)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_TYPE_StorBuf l_tBuff;
	uint32_t l_uLastPageIdx;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBlobLen ) || ( NULL == p_puBlobCrc ) )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);

        if( e_eFSS_BLOBC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOBC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_BLOBC_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
                    l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);

                    if( e_eFSS_BLOBC_RES_OK == l_eRes )
                    {
                        /* Get last page, where blob size and crc are store */
                        l_uLastPageIdx = l_tStorSet.uTotPages - 1u;

                        l_eRes = eFSS_BLOBC_LoadBufferFromPage(p_ptCtx, &l_tBuff, false, l_uLastPageIdx);

                        if( e_eFSS_BLOBC_RES_OK == l_eRes )
                        {
                            *p_puBlobLen = l_tBuff.ptMeta->uPageUseSpec1;
                            *p_puBlobCrc = l_tBuff.ptMeta->uPageUseSpec2;
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_LOGC_FlushBufferInNewPage(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t p_uSeqN, uint32_t p_uIdx)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);

        if( e_eFSS_BLOBC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOBC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_BLOBC_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eRes = eFSS_LOGC_FlushBufferInPage(p_ptCtx, true, 0u, 0u, p_uSeqN, p_uIdx);
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_LOGC_LoadBufferFromNewPage(t_eFSS_BLOBC_Ctx* p_ptCtx,  uint32_t* p_puSeqN, uint32_t p_uIdx)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorBuf l_tBuff;
	uint32_t l_uLastPageIdx;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);

        if( e_eFSS_BLOBC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOBC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_BLOBC_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eRes = eFSS_BLOBC_LoadBufferFromPage(p_ptCtx, &l_tBuff, true, p_uIdx);

                    if( e_eFSS_BLOBC_RES_OK == l_eRes )
                    {
                        *p_puSeqN = l_tBuff.ptMeta->uPageUseSpec3;
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_LOGC_FlushBufferInBkupPage(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t p_uSeqN, uint32_t p_uIdx)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);

        if( e_eFSS_BLOBC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOBC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_BLOBC_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eRes = eFSS_LOGC_FlushBufferInPage(p_ptCtx, false, 0u, 0u, p_uSeqN, p_uIdx);
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_LOGC_LoadBufferFromBkupPage(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t* p_puSeqN, uint32_t p_uIdx)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorBuf l_tBuff;
	uint32_t l_uLastPageIdx;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);

        if( e_eFSS_BLOBC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOBC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_BLOBC_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eRes = eFSS_BLOBC_LoadBufferFromPage(p_ptCtx, &l_tBuff, false, p_uIdx);

                    if( e_eFSS_BLOBC_RES_OK == l_eRes )
                    {
                        *p_puSeqN = l_tBuff.ptMeta->uPageUseSpec3;
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_LOGC_GetCrcFromTheBuffer(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t p_uSeed, uint32_t* p_puCrc)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_TYPE_StorBuf l_tBuff;
	uint32_t l_uLastPageIdx;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puCrc ) )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);

        if( e_eFSS_BLOBC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOBC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_BLOBC_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResHL =  eFSS_COREHL_CalcCrcInBuff(&p_ptCtx->tCOREHLCtx, p_uSeed, l_tBuff.uBufL, p_puCrc);
                    l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_LOGC_GenerateBkup(t_eFSS_BLOBC_Ctx* p_ptCtx)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_TYPE_StorBuf l_tBuff;
	uint32_t l_uLastPageIdx;
    uint32_t l_currPageIndx;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);

        if( e_eFSS_BLOBC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOBC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_BLOBC_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResHL =  eFSS_COREHL_GetBuffNStor(&p_ptCtx->tCOREHLCtx, &l_tBuff, &l_tStorSet);
                    l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);

                    if( e_eFSS_BLOBC_RES_OK == l_eRes )
                    {
                        /* Get last page */
                        l_uLastPageIdx = l_tStorSet.uTotPages / 2u ;
                        l_currPageIndx = 0u;

                        while( ( l_currPageIndx < l_uLastPageIdx ) && ( e_eFSS_BLOBC_RES_OK== l_eRes ) )
                        {
                            /* Before reading fix any error in original and backup pages */
                            l_eRes =  eFSS_COREHL_LoadPageInBuff(&p_ptCtx->tCOREHLCtx, l_currPageIndx);
                            l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);

                            if( e_eFSS_BLOBC_RES_OK == l_eRes )
                            {
                                /* Flush */
                                l_eRes =  eFSS_COREHL_FlushBuffInPage(&p_ptCtx->tCOREHLCtx, ( l_uLastPageIdx + l_currPageIndx ) );
                                l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);
                            }

                            l_currPageIndx++;
                        }

                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_LOGC_RipristinateOriginal(t_eFSS_BLOBC_Ctx* p_ptCtx)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_TYPE_StorBuf l_tBuff;
	uint32_t l_uLastPageIdx;
    uint32_t l_currPageIndx;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);

        if( e_eFSS_BLOBC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOBC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_BLOBC_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResHL =  eFSS_COREHL_GetBuffNStor(&p_ptCtx->tCOREHLCtx, &l_tBuff, &l_tStorSet);
                    l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);

                    if( e_eFSS_BLOBC_RES_OK == l_eRes )
                    {
                        /* Get last page */
                        l_uLastPageIdx = l_tStorSet.uTotPages / 2u ;
                        l_currPageIndx = 0u;

                        while( ( l_currPageIndx < l_uLastPageIdx ) && ( e_eFSS_BLOBC_RES_OK== l_eRes ) )
                        {
                            /* Before reading fix any error in original and backup pages */
                            l_eRes =  eFSS_COREHL_LoadPageInBuff(&p_ptCtx->tCOREHLCtx, l_currPageIndx);
                            l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);

                            if( e_eFSS_BLOBC_RES_OK == l_eRes )
                            {
                                /* Flush */
                                l_eRes =  eFSS_COREHL_FlushBuffInPage(&p_ptCtx->tCOREHLCtx, ( l_uLastPageIdx + l_currPageIndx ) );
                                l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);
                            }

                            l_currPageIndx++;
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
static bool_t eFSS_BLOBC_IsStatusStillCoherent(t_eFSS_BLOBC_Ctx* p_ptCtx)
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
        if( ( l_uNPage < 2u ) && ( 0u != ( l_uNPage % 2u ) ) )
        {
            l_bRes = false;
        }
        else
        {
            l_bRes = true;
        }
    }

    return l_bRes;
}

static e_eFSS_BLOBC_RES eFSS_BLOBC_HLtoBLOBRes(const e_eFSS_COREHL_RES p_eHLRes)
{
    e_eFSS_BLOBC_RES l_eRes;

    switch(p_eHLRes)
    {
        case e_eFSS_COREHL_RES_OK:
        {
            l_eRes = e_eFSS_BLOBC_RES_OK;
            break;
        }

        case e_eFSS_COREHL_RES_NOINITLIB:
        {
            l_eRes = e_eFSS_BLOBC_RES_NOINITLIB;
            break;
        }

        case e_eFSS_COREHL_RES_BADPARAM:
        {
            l_eRes = e_eFSS_BLOBC_RES_BADPARAM;
            break;
        }

        case e_eFSS_COREHL_RES_BADPOINTER:
        {
            l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
            break;
        }

        case e_eFSS_COREHL_RES_CORRUPTCTX:
        {
            l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
            break;
        }

        case e_eFSS_COREHL_RES_CLBCKERASEERR:
        {
            l_eRes = e_eFSS_BLOBC_RES_CLBCKERASEERR;
            break;
        }

        case e_eFSS_COREHL_RES_CLBCKWRITEERR:
        {
            l_eRes = e_eFSS_BLOBC_RES_CLBCKWRITEERR;
            break;
        }

        case e_eFSS_COREHL_RES_CLBCKREADERR:
        {
            l_eRes = e_eFSS_BLOBC_RES_CLBCKREADERR;
            break;
        }

        case e_eFSS_COREHL_RES_CLBCKCRCERR:
        {
            l_eRes = e_eFSS_BLOBC_RES_CLBCKCRCERR;
            break;
        }

        case e_eFSS_COREHL_RES_NOTVALIDPAGE:
        {
            l_eRes = e_eFSS_BLOBC_RES_NEWVERSIONBLOB;
            break;
        }

        case e_eFSS_COREHL_RES_NEWVERSIONFOUND:
        {
            l_eRes = e_eFSS_BLOBC_RES_NOTVALIDBLOB;
            break;
        }

        case e_eFSS_COREHL_RES_WRITENOMATCHREAD:
        {
            l_eRes = e_eFSS_BLOBC_RES_WRITENOMATCHREAD;
            break;
        }

        case e_eFSS_COREHL_RES_OK_BKP_RCVRD:
        {
            l_eRes = e_eFSS_BLOBC_RES_OK_BKP_RCVRD;
            break;
        }

        default:
        {
            l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
            break;
        }
    }

    return l_eRes;
}

static e_eFSS_BLOBC_RES eFSS_BLOBC_LoadBufferFromPage(t_eFSS_BLOBC_Ctx* p_ptCtx, t_eFSS_TYPE_StorBuf* p_ptBuff,
                                                     bool_t p_bIsOri, uint32_t p_uIdx)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tStorSet;
	uint32_t l_uLastPageIdx;
    uint32_t l_uCurrPageConv;
    uint8_t subTypeToCheck;

    l_eResHL =  eFSS_COREHL_GetBuffNStor(&p_ptCtx->tCOREHLCtx, p_ptBuff, &l_tStorSet);
    l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);

    if( e_eFSS_BLOBC_RES_OK == l_eRes )
    {
        /* Get last page */
        l_uLastPageIdx = ( l_tStorSet.uTotPages / 2u );

        if( p_uIdx >= l_uLastPageIdx )
        {
            l_eRes = e_eFSS_BLOBC_RES_BADPARAM;
        }
        else
        {
            /* align page */
            if( true == p_bIsOri )
            {
                subTypeToCheck = EFSS_PAGESUBTYPE_BLOBORI;
                l_uCurrPageConv = p_uIdx;
            }
            else
            {
                subTypeToCheck = EFSS_PAGESUBTYPE_BLOBBKP;
                l_uCurrPageConv = p_uIdx + l_uLastPageIdx;
            }

            /* Before reading fix any error in original and backup pages */
            l_eRes =  eFSS_COREHL_LoadPageInBuff(&p_ptCtx->tCOREHLCtx, l_uCurrPageConv);
            l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);

            if( e_eFSS_BLOBC_RES_OK == l_eRes )
            {
                /* Verify basic data */
                if( ( subTypeToCheck != p_ptBuff->ptMeta->uPageSubType ) ||
                    ( p_uIdx != p_ptBuff->ptMeta->uPageUseSpec4 ) )
                {
                    l_eRes = e_eFSS_BLOBC_RES_NOTVALIDBLOB;
                }
            }
        }
    }


	return l_eRes;
}

static e_eFSS_BLOBC_RES eFSS_LOGC_FlushBufferInPage(t_eFSS_BLOBC_Ctx* p_ptCtx, bool_t p_bIsOri, uint32_t p_uBlobL,
                                                    uint32_t p_uCrc, uint32_t p_uSeqN, uint32_t p_uIdx)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_TYPE_StorBuf l_tBuff;
	uint32_t l_uLastPageIdx;
    uint32_t l_uCurrPageConv;
    uint8_t subTypeToCheck;

    l_eResHL =  eFSS_COREHL_GetBuffNStor(&p_ptCtx->tCOREHLCtx, &l_tBuff, &l_tStorSet);
    l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);

    if( e_eFSS_BLOBC_RES_OK == l_eRes )
    {
        /* Get last page */
        l_uLastPageIdx = ( l_tStorSet.uTotPages / 2u );

        if( p_uIdx >= l_uLastPageIdx )
        {
            l_eRes = e_eFSS_BLOBC_RES_BADPARAM;
        }
        else
        {
            /* align page */
            if( true == p_bIsOri )
            {
                subTypeToCheck = EFSS_PAGESUBTYPE_BLOBORI;
                l_uCurrPageConv = p_uIdx;
            }
            else
            {
                subTypeToCheck = EFSS_PAGESUBTYPE_BLOBBKP;
                l_uCurrPageConv = p_uIdx + l_uLastPageIdx;
            }

            /* Can read data */
            l_tBuff.ptMeta->uPageUseSpec1 = p_uBlobL;
            l_tBuff.ptMeta->uPageUseSpec2 = p_uCrc;
            l_tBuff.ptMeta->uPageUseSpec3 = p_uSeqN;
            l_tBuff.ptMeta->uPageUseSpec4 = l_uCurrPageConv;
            l_tBuff.ptMeta->uPageSubType = subTypeToCheck;

            /* Flush */
            l_eRes =  eFSS_COREHL_FlushBuffInPage(&p_ptCtx->tCOREHLCtx, l_uLastPageIdx);
            l_eRes = eFSS_BLOBC_HLtoBLOBRes(l_eResHL);
        }
    }

	return l_eRes;
}