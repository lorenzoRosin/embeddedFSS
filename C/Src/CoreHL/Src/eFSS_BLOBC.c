/**
 * @file       eFSS_BLOBC.c
 *
 * @brief      Blob Core - large file core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/* In this module the page field has the following meaning:
 * ------------------------------------------------------------------ User data
 * - [uint8_t] -                    -> N byte of user data           |
 * ------------------------------------------------------------------ Metadata  (4 byte)
 * - uint32_t  - Page Index         -> Page Seq Number               |
 * ------------------------------------------------------------------ Under we have LL/HL metadata
 * - LOW LEVEL / HIGH LEVEL METADATA                                 |
 * ------------------------------------------------------------------ End of Page
 *
 * In this module the storage is organizated as follow :
 *
 * - [ 0                            -    ( uTotPages / 2 ) - 1 ]  -> Original pages
 * - [ ( uTotPages / 2 )            -    uTotPages - 1         ]  -> Backup pages
 *
 */



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_BLOBC.h"
#include "eFSS_Utils.h"



/***********************************************************************************************************************
 *      PRIVATE DEFINE
 **********************************************************************************************************************/
#define EFSS_PAGETYPE_BLOB                                                                       ( ( uint8_t )   0x01u )
#define EFSS_BLOBC_PAGEMIN_L                                                                     ( ( uint32_t )     4u )
#define EFSS_PAGESUBTYPE_BLOBORI                                                                 ( ( uint8_t )   0x01u )
#define EFSS_PAGESUBTYPE_BLOBBKP                                                                 ( ( uint8_t )   0x02u )



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_BLOBC_IsStatusStillCoherent(t_eFSS_BLOBC_Ctx*const p_ptCtx);
static e_eFSS_BLOBC_RES eFSS_BLOBC_HLtoBLOBCRes(const e_eFSS_COREHL_RES p_eHLRes);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_BLOBC_RES eFSS_BLOBC_InitCtx(t_eFSS_BLOBC_Ctx* const p_ptCtx, const t_eFSS_TYPE_CbStorCtx p_tCtxCb,
                                    const t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff,
                                    const uint32_t p_uBuffL)
{
    /* Return local var */
    e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    uint32_t l_uNPage;
    t_eFSS_COREHL_StorBuf l_tBuff;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
        /* Blobs need both original page and backup pages */
        l_uNPage = p_tStorSet.uTotPages;

        /* Check numbers of page validity */
        if( ( l_uNPage < 2u ) || ( 0u != ( l_uNPage % 2u ) ) )
        {
            l_eRes = e_eFSS_BLOBC_RES_BADPARAM;
        }
        else
        {
            /* Can init low level context */
            l_eResHL = eFSS_COREHL_InitCtx(&p_ptCtx->tCOREHLCtx, p_tCtxCb, p_tStorSet, EFSS_PAGETYPE_BLOB, p_puBuff,
                                           p_uBuffL);
            l_eRes = eFSS_BLOBC_HLtoBLOBCRes(l_eResHL);

            if( e_eFSS_BLOBC_RES_OK == l_eRes )
            {
                /* Check if we have enogh space for the page */
                l_eResHL = eFSS_COREHL_GetBuff(&p_ptCtx->tCOREHLCtx, &l_tBuff);
                l_eRes = eFSS_BLOBC_HLtoBLOBCRes(l_eResHL);
                if( e_eFSS_BLOBC_RES_OK == l_eRes )
                {
                    if( l_tBuff.uBufL <= EFSS_BLOBC_PAGEMIN_L )
                    {
                        l_eRes = e_eFSS_BLOBC_RES_BADPARAM;

                        /* De init HL */
                        (void)memset(&p_ptCtx->tCOREHLCtx, 0, sizeof(t_eFSS_COREHL_Ctx));
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

e_eFSS_BLOBC_RES eFSS_BLOBC_IsInit(t_eFSS_BLOBC_Ctx* const p_ptCtx, bool_t* const p_pbIsInit)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, p_pbIsInit);
        l_eRes = eFSS_BLOBC_HLtoBLOBCRes(l_eResHL);
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_BLOBC_GetBuffNUsable(t_eFSS_BLOBC_Ctx* const p_ptCtx, t_eFSS_BLOBC_StorBuf* const p_ptBuff,
                                           uint32_t* const p_puUsePages)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    t_eFSS_COREHL_StorBuf l_tBuff;
    t_eFSS_TYPE_StorSet   l_tStorSet;
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ptBuff ) || ( NULL == p_puUsePages ) )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_BLOBC_HLtoBLOBCRes(l_eResHL);

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
                    l_eResHL = eFSS_COREHL_GetBuffNStor(&p_ptCtx->tCOREHLCtx, &l_tBuff, &l_tStorSet);
                    l_eRes = eFSS_BLOBC_HLtoBLOBCRes(l_eResHL);

                    if( e_eFSS_BLOBC_RES_OK == l_eRes )
                    {
                        p_ptBuff->puBuf = l_tBuff.puBuf;
                        p_ptBuff->uBufL = l_tBuff.uBufL - EFSS_BLOBC_PAGEMIN_L;
                        *p_puUsePages = (uint32_t)( l_tStorSet.uTotPages / 2u );
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_BLOBC_LoadBufferFromPage(t_eFSS_BLOBC_Ctx* const p_ptCtx, const bool_t p_bInOrigin,
                                               const uint32_t p_uIdx, uint32_t* const p_puSeqN)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for storage */
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_COREHL_StorBuf l_tBuff;

    /* Local var used for calculation */
    bool_t l_bIsInit;
	uint32_t l_uLastPageIdx;
    uint32_t l_uCurrPageConv;
    uint8_t l_uSubTypeToCheck;
    uint8_t l_uSubTypeReaded;
    uint32_t l_uSeqOff;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puSeqN ) )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_BLOBC_HLtoBLOBCRes(l_eResHL);

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
                    /* Need to check index validity */
                    l_eResHL = eFSS_COREHL_GetBuffNStor(&p_ptCtx->tCOREHLCtx, &l_tBuff, &l_tStorSet);
                    l_eRes = eFSS_BLOBC_HLtoBLOBCRes(l_eResHL);

                    if( e_eFSS_BLOBC_RES_OK == l_eRes )
                    {
                        l_uLastPageIdx = ( l_tStorSet.uTotPages / 2u );

                        if( p_uIdx >= l_uLastPageIdx )
                        {
                            l_eRes = e_eFSS_BLOBC_RES_BADPARAM;
                        }
                        else
                        {
                            /* align page */
                            if( true == p_bInOrigin )
                            {
                                l_uSubTypeToCheck = EFSS_PAGESUBTYPE_BLOBORI;
                                l_uCurrPageConv = p_uIdx;
                            }
                            else
                            {
                                l_uSubTypeToCheck = EFSS_PAGESUBTYPE_BLOBBKP;
                                l_uCurrPageConv = p_uIdx + l_uLastPageIdx;
                            }

                            /* Load the page in to the internal buffer */
                            l_uSubTypeReaded = EFSS_PAGESUBTYPE_BLOBORI;
                            l_eResHL = eFSS_COREHL_LoadPageInBuff(&p_ptCtx->tCOREHLCtx, l_uCurrPageConv,
                                                                  &l_uSubTypeReaded);
                            l_eRes = eFSS_BLOBC_HLtoBLOBCRes(l_eResHL);

                            if( e_eFSS_BLOBC_RES_OK == l_eRes )
                            {
                                /* Verify basic data */
                                if( l_uSubTypeToCheck != l_uSubTypeReaded )
                                {
                                    l_eRes = e_eFSS_BLOBC_RES_NOTVALIDBLOB;
                                }
                                else
                                {
                                    /* Retrive Seq Number */
                                    l_uSeqOff = l_tBuff.uBufL - EFSS_BLOBC_PAGEMIN_L;
                                    if( true != eFSS_Utils_RetriveU32(&l_tBuff.puBuf[l_uSeqOff], p_puSeqN) )
                                    {
                                        l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
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

e_eFSS_BLOBC_RES eFSS_BLOBC_FlushBufferInPage(t_eFSS_BLOBC_Ctx* const p_ptCtx, const bool_t p_bInOrigin,
                                              const uint32_t p_uIdx, const uint32_t p_uSeqN)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for storage */
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_COREHL_StorBuf l_tBuff;

    /* Local var used for calculation */
    bool_t l_bIsInit;
	uint32_t l_uLastPageIdx;
    uint32_t l_uCurrPageConv;
    uint8_t l_uSubTypeWrite;
    uint32_t l_uSeqOff;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_BLOBC_HLtoBLOBCRes(l_eResHL);

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
                    /* Need to check index validity */
                    l_eResHL = eFSS_COREHL_GetBuffNStor(&p_ptCtx->tCOREHLCtx, &l_tBuff, &l_tStorSet);
                    l_eRes = eFSS_BLOBC_HLtoBLOBCRes(l_eResHL);

                    if( e_eFSS_BLOBC_RES_OK == l_eRes )
                    {
                        l_uLastPageIdx = ( l_tStorSet.uTotPages / 2u );

                        if( p_uIdx >= l_uLastPageIdx )
                        {
                            l_eRes = e_eFSS_BLOBC_RES_BADPARAM;
                        }
                        else
                        {
                            /* align page */
                            if( true == p_bInOrigin )
                            {
                                l_uSubTypeWrite = EFSS_PAGESUBTYPE_BLOBORI;
                                l_uCurrPageConv = p_uIdx;
                            }
                            else
                            {
                                l_uSubTypeWrite = EFSS_PAGESUBTYPE_BLOBBKP;
                                l_uCurrPageConv = p_uIdx + l_uLastPageIdx;
                            }

                            /* Insert Seq Number */
                            l_uSeqOff = l_tBuff.uBufL - EFSS_BLOBC_PAGEMIN_L;

                            if( true != eFSS_Utils_InsertU32(&l_tBuff.puBuf[l_uSeqOff], p_uSeqN) )
                            {
                                l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
                            }
                            else
                            {
                                l_eResHL = eFSS_COREHL_FlushBuffInPage(&p_ptCtx->tCOREHLCtx, l_uCurrPageConv,
                                                                       l_uSubTypeWrite);
                                l_eRes = eFSS_BLOBC_HLtoBLOBCRes(l_eResHL);
                            }
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_BLOBC_GetCrcFromTheBuffer(t_eFSS_BLOBC_Ctx* const p_ptCtx, const uint32_t p_uSeed,
                                                uint32_t* const p_puCrc)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    t_eFSS_COREHL_StorBuf l_tBuff;
    uint32_t l_uSeqOff;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_BLOBC_HLtoBLOBCRes(l_eResHL);

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
                    l_eResHL = eFSS_COREHL_GetBuff(&p_ptCtx->tCOREHLCtx, &l_tBuff);
                    l_eRes = eFSS_BLOBC_HLtoBLOBCRes(l_eResHL);

                    if( e_eFSS_BLOBC_RES_OK == l_eRes )
                    {
                        /* remove Seq Number from CRC calculation  */
                        l_uSeqOff = l_tBuff.uBufL - EFSS_BLOBC_PAGEMIN_L;

                        l_eResHL = eFSS_COREHL_CalcCrcInBuff(&p_ptCtx->tCOREHLCtx, p_uSeed, l_uSeqOff, p_puCrc);
                        l_eRes = eFSS_BLOBC_HLtoBLOBCRes(l_eResHL);
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_BLOBC_CopyOriInBkpIfNotEquals(t_eFSS_BLOBC_Ctx* const p_ptCtx, const uint32_t p_uIdx)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    bool_t l_bIsEquals;
    t_eFSS_TYPE_StorSet l_tStorSet;
	uint32_t l_uLastPageIdx;
    uint8_t l_uSubTypeReaded;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_BLOBC_HLtoBLOBCRes(l_eResHL);

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
                    /* Verify index */
                    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
                    l_eRes = eFSS_BLOBC_HLtoBLOBCRes(l_eResHL);

                    if( e_eFSS_BLOBC_RES_OK == l_eRes )
                    {
                        /* Get last page */
                        l_uLastPageIdx = l_tStorSet.uTotPages / 2u ;

                        if( p_uIdx >= l_uLastPageIdx )
                        {
                            l_eRes = e_eFSS_BLOBC_RES_BADPARAM;
                        }
                        else
                        {
                            /* Load the page in to the internal buffer */
                            l_uSubTypeReaded = EFSS_PAGESUBTYPE_BLOBORI;
                            l_eResHL = eFSS_COREHL_LoadPageInBuff(&p_ptCtx->tCOREHLCtx, p_uIdx, &l_uSubTypeReaded);
                            l_eRes = eFSS_BLOBC_HLtoBLOBCRes(l_eResHL);

                            if( e_eFSS_BLOBC_RES_OK == l_eRes )
                            {
                                /* Verify basic data */
                                if( EFSS_PAGESUBTYPE_BLOBORI != l_uSubTypeReaded )
                                {
                                    l_eRes = e_eFSS_BLOBC_RES_NOTVALIDBLOB;
                                }
                                else
                                {
                                    /* Verify if orginal page match the backup one */
                                    l_bIsEquals = false;
                                    l_eResHL = eFSS_COREHL_IsBuffEqualToPage(&p_ptCtx->tCOREHLCtx,
                                                                             (l_uLastPageIdx + p_uIdx),
                                                                             &l_bIsEquals);
                                    l_eRes = eFSS_BLOBC_HLtoBLOBCRes(l_eResHL);

                                    if( e_eFSS_BLOBC_RES_OK == l_eRes )
                                    {
                                        if( false == l_bIsEquals )
                                        {
                                            /* Not equal! Flush ori in backup */
                                            l_eResHL = eFSS_COREHL_FlushBuffInPage(&p_ptCtx->tCOREHLCtx,
                                                                                   (l_uLastPageIdx + p_uIdx),
                                                                                   EFSS_PAGESUBTYPE_BLOBBKP);
                                            l_eRes = eFSS_BLOBC_HLtoBLOBCRes(l_eResHL);
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
static bool_t eFSS_BLOBC_IsStatusStillCoherent(t_eFSS_BLOBC_Ctx* const p_ptCtx)
{
    /* Return local var */
    bool_t l_bRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_COREHL_StorBuf l_tBuff;
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
        if( ( l_uNPage < 2u ) || ( 0u != ( l_uNPage % 2u ) ) )
        {
            l_bRes = false;
        }
        else
        {
            if( l_tBuff.uBufL <= EFSS_BLOBC_PAGEMIN_L )
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

static e_eFSS_BLOBC_RES eFSS_BLOBC_HLtoBLOBCRes(const e_eFSS_COREHL_RES p_eHLRes)
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
            l_eRes = e_eFSS_BLOBC_RES_NOTVALIDBLOB;
            break;
        }

        case e_eFSS_COREHL_RES_NEWVERSIONFOUND:
        {
            l_eRes = e_eFSS_BLOBC_RES_NEWVERSIONBLOB;
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