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
 * ------------------------------------------------------------------ Under we have LL/HL metadata (20byte)
 * - LOW LEVEL / HIGH LEVEL METADATA                                 |
 * ------------------------------------------------------------------ End of Page
 *
 * In this module the storage is organizated as follow :
 *
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
#define EFSS_PAGESUBTYPE_DBORI                                                                   ( ( uint8_t )   0x01u )
#define EFSS_PAGESUBTYPE_DBBKP                                                                   ( ( uint8_t )   0x02u )
#define EFSS_DBC_NPAGEMIN                                                                        ( ( uint32_t )     2u )



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_DBC_IsStatusStillCoherent(t_eFSS_DBC_Ctx* const p_ptCtx);
static e_eFSS_DBC_RES eFSS_DBC_HLtoDBCRes(const e_eFSS_COREHL_RES p_eHLRes);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_DBC_RES eFSS_DBC_InitCtx(t_eFSS_DBC_Ctx* const p_ptCtx, const t_eFSS_TYPE_CbStorCtx p_tCtxCb,
								const t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff,
                                const uint32_t p_uBuffL)
{
    /* Return local var */
    e_eFSS_DBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    uint32_t l_uNPage;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_DBC_RES_BADPOINTER;
	}
	else
	{
        /* The database needs at least 2 pages, one for original and one for backup */
        l_uNPage = p_tStorSet.uTotPages;

        /* Check numbers of page validity */
        if( ( l_uNPage < EFSS_DBC_NPAGEMIN ) || ( 0u != ( l_uNPage % EFSS_DBC_NPAGEMIN ) ) )
        {
            l_eRes = e_eFSS_DBC_RES_BADPARAM;
        }
        else
        {
            /* Can now init low level context */
            l_eResHL = eFSS_COREHL_InitCtx(&p_ptCtx->tCOREHLCtx, p_tCtxCb, p_tStorSet, EFSS_PAGETYPE_DB, p_puBuff,
                                           p_uBuffL);
            l_eRes = eFSS_DBC_HLtoDBCRes(l_eResHL);
        }
    }

    return l_eRes;
}

e_eFSS_DBC_RES eFSS_DBC_IsInit(const t_eFSS_DBC_Ctx* p_ptCtx, bool_t* const p_pbIsInit)
{
	/* Return local var */
	e_eFSS_DBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_DBC_RES_BADPOINTER;
	}
	else
	{
        l_eResHL = eFSS_COREHL_IsInit(&p_ptCtx->tCOREHLCtx, p_pbIsInit);
        l_eRes = eFSS_DBC_HLtoDBCRes(l_eResHL);
	}

	return l_eRes;
}

e_eFSS_DBC_RES eFSS_DBC_GetBuffNUsable(t_eFSS_DBC_Ctx* const p_ptCtx, t_eFSS_DBC_StorBuf* const p_ptBuff,
                                       uint32_t* const p_puUsePages)
{
	/* Return local var */
	e_eFSS_DBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var for init */
    bool_t l_bIsInit;

    /* Local variable for storage */
    t_eFSS_COREHL_StorBuf l_tBuff;

    /* Local var used for calculation */
    t_eFSS_TYPE_StorSet   l_tStorSet;


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
        l_eRes = eFSS_DBC_HLtoDBCRes(l_eResHL);

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
                    l_eRes = eFSS_DBC_HLtoDBCRes(l_eResHL);

                    if( e_eFSS_DBC_RES_OK == l_eRes )
                    {
                        p_ptBuff->puBuf = l_tBuff.puBuf;
                        p_ptBuff->uBufL = l_tBuff.uBufL;
                        *p_puUsePages = (uint32_t)( l_tStorSet.uTotPages / EFSS_DBC_NPAGEMIN );
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_DBC_RES eFSS_DBC_LoadPageInBuff(t_eFSS_DBC_Ctx* const p_ptCtx, const uint32_t p_uPageIndx)
{
	/* Return local var */
	e_eFSS_DBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var for init */
    bool_t l_bIsInit;

    /* Local var used for storage */
    t_eFSS_TYPE_StorSet l_tStorSet;

    /* Local var used for calculation */
    uint32_t l_uTotPages;
    uint32_t l_uBkpIdx;

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
        l_eRes = eFSS_DBC_HLtoDBCRes(l_eResHL);

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
                    /* Need to check page index validity, get the HL stor settings and verify */
                    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
                    l_eRes = eFSS_DBC_HLtoDBCRes(l_eResHL);

                    if( e_eFSS_DBC_RES_OK == l_eRes )
                    {
                        l_uTotPages = l_tStorSet.uTotPages;
                        if( p_uPageIndx >= ( l_uTotPages / EFSS_DBC_NPAGEMIN ) )
                        {
                            l_eRes = e_eFSS_DBC_RES_BADPARAM;
                        }
                        else
                        {
                            l_uBkpIdx = p_uPageIndx + ( l_uTotPages / EFSS_DBC_NPAGEMIN );
                            l_eResHL = eFSS_COREHL_LoadPageInBuffNRipBkp(&p_ptCtx->tCOREHLCtx, p_uPageIndx, l_uBkpIdx,
                                                                         EFSS_PAGESUBTYPE_DBORI,
                                                                         EFSS_PAGESUBTYPE_DBBKP);
                            l_eRes = eFSS_DBC_HLtoDBCRes(l_eResHL);
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
	/* Return local var */
	e_eFSS_DBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var for init */
    bool_t l_bIsInit;

    /* Local var used for storage */
    t_eFSS_TYPE_StorSet l_tStorSet;

    /* Local var used for calculation */
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
        l_eRes = eFSS_DBC_HLtoDBCRes(l_eResHL);

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
                    /* Need to check page index validity, get the HL stor settings and verify */
                    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, &l_tStorSet);
                    l_eRes = eFSS_DBC_HLtoDBCRes(l_eResHL);

                    if( e_eFSS_DBC_RES_OK == l_eRes )
                    {
                        l_uTotPages = l_tStorSet.uTotPages;
                        if( p_uPageIndx >= ( l_uTotPages / EFSS_DBC_NPAGEMIN ) )
                        {
                            l_eRes = e_eFSS_DBC_RES_BADPARAM;
                        }
                        else
                        {
                            l_uBkpIndex = p_uPageIndx + ( l_uTotPages / EFSS_DBC_NPAGEMIN );

                            l_eResHL = eFSS_COREHL_FlushBuffInPageNBkp(&p_ptCtx->tCOREHLCtx, p_uPageIndx, l_uBkpIndex,
                                                                       EFSS_PAGESUBTYPE_DBORI, EFSS_PAGESUBTYPE_DBBKP);
                            l_eRes = eFSS_DBC_HLtoDBCRes(l_eResHL);
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
static bool_t eFSS_DBC_IsStatusStillCoherent(t_eFSS_DBC_Ctx* const p_ptCtx)
{
    /* Return local var */
    bool_t l_bRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for storage */
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
        if( ( l_uNPage < EFSS_DBC_NPAGEMIN ) || ( 0u != ( l_uNPage % EFSS_DBC_NPAGEMIN ) ) )
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

static e_eFSS_DBC_RES eFSS_DBC_HLtoDBCRes(const e_eFSS_COREHL_RES p_eHLRes)
{
    /* Return local var */
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
            l_eRes = e_eFSS_DBC_RES_NEWVERSIONFOUND;
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