/**
 * @file       eFSS_DBC.h
 *
 * @brief      Database Core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/* In this module the page field has the following meaning:
 * ------------------------------------------------------------------ User data
 * - [uint8_t] -                    -> N byte of user data           |
 * ------------------------------------------------------------------ Metadata  (0 byte)
 * ------------------------------------------------------------------ Under we have LL/HL metadata
 * - LOW LEVEL / HIGH LEVEL METADATA                                 |
 * ------------------------------------------------------------------ End of Page
 *
 * In this module the storage is organizated as follow :
 *
 *   bFullBckup = true, bFlashCache = true
 * - [ 0                            -    ( uTotPages / 2 ) - 1    ]  -> Original pages
 * - [ ( uTotPages / 2 )            -    uTotPages - 1            ]  -> Backup pages
 *
 */

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_DBC.h"
#include "eFSS_Utils.h"



/***********************************************************************************************************************
 *      PRIVATE DEFINE
 **********************************************************************************************************************/
#define EFSS_PAGETYPE_DB                                                                         ( ( uint8_t )   0x03u )
#define EFSS_DBC_PAGEMIN_L                                                                       ( ( uint32_t )     0u )
#define EFSS_PAGESUBTYPE_DBORI                                                                   ( ( uint8_t )   0x01u )
#define EFSS_PAGESUBTYPE_DBBKP                                                                   ( ( uint8_t )   0x02u )


/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_DBC_IsStatusStillCoherent(t_eFSS_DBC_Ctx* p_ptCtx);
static e_eFSS_DBC_RES eFSS_DB_HLtoDBCRes(const e_eFSS_COREHL_RES p_eHLRes);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_DBC_RES eFSS_DBC_InitCtx(t_eFSS_DBC_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx const p_tCtxCb,
								const t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff,
                                const uint32_t p_uBuffL)
{
    /* Return local var */
    e_eFSS_DBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    uint32_t l_uNPage;
    t_eFSS_COREHL_StorBuf l_tBuff;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_DBC_RES_BADPOINTER;
	}
	else
	{
        l_uNPage = p_tStorSet.uTotPages;

        /* Check numbers of page validity */
        if( ( l_uNPage < 2u ) && ( 0u != ( l_uNPage % 2u ) ) )
        {
            l_eRes = e_eFSS_DBC_RES_BADPARAM;
        }
        else
        {
            /* Can init low level context */
            l_eResHL = eFSS_COREHL_InitCtx(&p_ptCtx->tCOREHLCtx, p_tCtxCb, p_tStorSet, EFSS_PAGETYPE_DB, p_puBuff, p_uBuffL);
            l_eRes = eFSS_DB_HLtoDBCRes(l_eResHL);

            if( e_eFSS_DBC_RES_OK == l_eRes )
            {
                /* Check if we have enogh space for the page */
                l_eResHL = eFSS_COREHL_GetBuff(&p_ptCtx->tCOREHLCtx, &l_tBuff);
                l_eRes = eFSS_DB_HLtoDBCRes(l_eResHL);
                if( e_eFSS_DBC_RES_OK == l_eRes )
                {
                    if( l_tBuff.uBufL <= EFSS_DBC_PAGEMIN_L)
                    {
                        l_eRes = e_eFSS_DBC_RES_BADPARAM;
                    }
                }
            }
        }
    }

    return l_eRes;
}

e_eFSS_DBC_RES eFSS_DBC_IsInit(t_eFSS_DBC_Ctx* const p_ptCtx, bool_t* const p_pbIsInit)
{
	/* Local variable */
	e_eFSS_DBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSS_DBC_RES_BADPOINTER;
	}
	else
	{
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, p_pbIsInit);
        l_eRes = eFSS_DB_HLtoDBCRes(l_eResHL);
	}

	return l_eRes;
}

e_eFSS_DBC_RES eFSS_DBC_GetBuffNUsable(t_eFSS_DBC_Ctx* const p_ptCtx, t_eFSS_DBC_StorBuf* const p_ptBuff,
                                       uint32_t* const p_puUsePages)
{
	/* Local variable */
	e_eFSS_DBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    t_eFSS_COREHL_StorBuf l_tBuff;
    t_eFSS_TYPE_StorSet   l_tStorSet;
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ptBuff ) || ( NULL == p_puUsePages ) )
	{
		l_eRes = e_eFSS_DBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_DB_HLtoDBCRes(l_eResHL);

        if( e_eFSS_DBC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_DBC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_DBC_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_DBC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResHL = eFSS_COREHL_GetBuffNStor(&p_ptCtx->tCOREHLCtx, &l_tBuff, &l_tStorSet);
                    l_eRes = eFSS_DB_HLtoDBCRes(l_eResHL);

                    if( e_eFSS_DBC_RES_OK == l_eRes )
                    {
                        p_ptBuff->puBuf = l_tBuff.puBuf;
                        p_ptBuff->uBufL = l_tBuff.uBufL - EFSS_DBC_PAGEMIN_L;
                        *p_puUsePages = (uint32_t)( l_tStorSet.uTotPages / 2u );
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_DBC_RES eFSS_DBC_LoadPageInBuff(t_eFSS_DBC_Ctx* const p_ptCtx, const uint32_t p_uPageIndx)
{
	/* Local variable */
	e_eFSS_DBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for storage */
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_COREHL_StorBuf l_tBuff;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    uint32_t l_uTotPages;
    uint32_t l_uBkpIndex;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_DBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_DB_HLtoDBCRes(l_eResHL);

        if( e_eFSS_DBC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_DBC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_DBC_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_DBC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResHL = eFSS_COREHL_GetBuffNStor(&p_ptCtx->tCOREHLCtx, &l_tBuff, &l_tStorSet);
                    l_eRes = eFSS_DB_HLtoDBCRes(l_eResHL);

                    if( e_eFSS_DBC_RES_OK == l_eRes )
                    {
                        l_uTotPages = l_tStorSet.uTotPages;
                        if( p_uPageIndx >= ( l_uTotPages / 2u ) )
                        {
                            l_eRes = e_eFSS_DBC_RES_BADPARAM;
                        }
                        else
                        {
                            l_uBkpIndex = p_uPageIndx + ( l_uTotPages / 2u );
                            l_eResHL = eFSS_COREHL_LoadPageInBuffNRipBkp(&p_ptCtx->tCOREHLCtx, p_uPageIndx, l_uBkpIndex,
                                                                         EFSS_PAGESUBTYPE_DBORI, EFSS_PAGESUBTYPE_DBBKP);
                            l_eRes = eFSS_DB_HLtoDBCRes(l_eResHL);
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_DBC_RES eFSS_DBC_FlushBuffInPage(t_eFSS_DBC_Ctx* const p_ptCtx, const uint32_t p_uPageIndx)
{
	/* Local variable */
	e_eFSS_DBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for storage */
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_COREHL_StorBuf l_tBuff;

    /* Local var used for calculation */
    bool_t l_bIsInit;
    uint32_t l_uTotPages;
    uint32_t l_uBkpIndex;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_DBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_DB_HLtoDBCRes(l_eResHL);

        if( e_eFSS_DBC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_DBC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_DBC_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_DBC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResHL = eFSS_COREHL_GetBuffNStor(&p_ptCtx->tCOREHLCtx, &l_tBuff, &l_tStorSet);
                    l_eRes = eFSS_DB_HLtoDBCRes(l_eResHL);

                    if( e_eFSS_DBC_RES_OK == l_eRes )
                    {
                        l_uTotPages = l_tStorSet.uTotPages;
                        if( p_uPageIndx >= ( l_uTotPages / 2u ) )
                        {
                            l_eRes = e_eFSS_DBC_RES_BADPARAM;
                        }
                        else
                        {
                            l_uBkpIndex = p_uPageIndx + ( l_uTotPages / 2u );

                            l_eResHL = eFSS_COREHL_FlushBuffInPageNBkp(&p_ptCtx->tCOREHLCtx, p_uPageIndx, l_uBkpIndex,
                                                                       EFSS_PAGESUBTYPE_DBORI, EFSS_PAGESUBTYPE_DBBKP);
                            l_eRes = eFSS_DB_HLtoDBCRes(l_eResHL);
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
static bool_t eFSS_DBC_IsStatusStillCoherent(t_eFSS_DBC_Ctx* p_ptCtx)
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
        if( ( l_uNPage < 2u ) && ( 0u != ( l_uNPage % 2u ) ) )
        {
            l_bRes = false;
        }
        else
        {
            if( l_tBuff.uBufL <= EFSS_DBC_PAGEMIN_L )
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

static e_eFSS_DBC_RES eFSS_DB_HLtoDBCRes(const e_eFSS_COREHL_RES p_eHLRes)
{
    e_eFSS_DBC_RES l_eRes;

    switch(p_eHLRes)
    {
        case e_eFSS_COREHL_RES_OK:
        {
            l_eRes = e_eFSS_DBC_RES_OK;
            break;
        }

        case e_eFSS_COREHL_RES_NOINITLIB:
        {
            l_eRes = e_eFSS_DBC_RES_NOINITLIB;
            break;
        }

        case e_eFSS_COREHL_RES_BADPARAM:
        {
            l_eRes = e_eFSS_DBC_RES_BADPARAM;
            break;
        }

        case e_eFSS_COREHL_RES_BADPOINTER:
        {
            l_eRes = e_eFSS_DBC_RES_BADPOINTER;
            break;
        }

        case e_eFSS_COREHL_RES_CORRUPTCTX:
        {
            l_eRes = e_eFSS_DBC_RES_CORRUPTCTX;
            break;
        }

        case e_eFSS_COREHL_RES_CLBCKERASEERR:
        {
            l_eRes = e_eFSS_DBC_RES_CLBCKERASEERR;
            break;
        }

        case e_eFSS_COREHL_RES_CLBCKWRITEERR:
        {
            l_eRes = e_eFSS_DBC_RES_CLBCKWRITEERR;
            break;
        }

        case e_eFSS_COREHL_RES_CLBCKREADERR:
        {
            l_eRes = e_eFSS_DBC_RES_CLBCKREADERR;
            break;
        }

        case e_eFSS_COREHL_RES_CLBCKCRCERR:
        {
            l_eRes = e_eFSS_DBC_RES_CLBCKCRCERR;
            break;
        }

        case e_eFSS_COREHL_RES_NOTVALIDPAGE:
        {
            l_eRes = e_eFSS_DBC_RES_NOTVALIDDB;
            break;
        }

        case e_eFSS_COREHL_RES_NEWVERSIONFOUND:
        {
            l_eRes = e_eFSS_DBC_RES_NEWVERSIONDB;
            break;
        }

        case e_eFSS_COREHL_RES_WRITENOMATCHREAD:
        {
            l_eRes = e_eFSS_DBC_RES_WRITENOMATCHREAD;
            break;
        }

        case e_eFSS_COREHL_RES_OK_BKP_RCVRD:
        {
            l_eRes = e_eFSS_DBC_RES_OK_BKP_RCVRD;
            break;
        }

        default:
        {
            l_eRes = e_eFSS_DBC_RES_CORRUPTCTX;
            break;
        }
    }

    return l_eRes;
}