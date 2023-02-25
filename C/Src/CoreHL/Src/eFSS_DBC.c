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
 * ------------------------------------------------------------------ Metadata  (17 byte)
 * - uint32_t  - uPageUseSpec1      -> No value defined              |
 * - uint32_t  - uPageUseSpec2      -> No value defined              |
 * - uint32_t  - uPageUseSpec3      -> No value defined              |
 * - uint32_t  - uPageUseSpec4      -> Page Index                    |
 * - uint8_t   - uPageSubType       -> Page subtype                  |
 * ------------------------------------------------------------------+
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



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_DBC_IsStatusStillCoherent(t_eFSS_DBC_Ctx* p_ptCtx);
static e_eFSS_DBC_RES eFSS_DB_HLtoDBCRes(const e_eFSS_COREHL_RES p_eHLRes);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_DBC_RES eFSS_DBC_InitCtx(t_eFSS_DBC_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx const p_tCtxCb,
                                t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL)
{
    /* Return local var */
    e_eFSS_DBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    uint32_t l_uNPage;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
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
            l_eResHL = eFSS_COREHL_InitCtx(&p_ptCtx->tCOREHLCtx, p_tCtxCb, p_tStorSet, p_puBuff, p_uBuffL);
            l_eRes = eFSS_DB_HLtoDBCRes(l_eResHL);
        }
    }

    return l_eRes;
}

e_eFSS_DBC_RES eFSS_DBC_IsInit(t_eFSS_DBC_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
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

e_eFSS_DBC_RES eFSS_DBC_GetStorSett(t_eFSS_DBC_Ctx* p_ptCtx, t_eFSS_TYPE_StorSet* p_ptStorSet)
{
	/* Local variable */
	e_eFSS_DBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ptStorSet ) )
	{
		l_eRes = e_eFSS_DBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
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
                    l_eResHL = eFSS_COREHL_GetStorSett(&p_ptCtx->tCOREHLCtx, p_ptStorSet);
                    l_eRes = eFSS_DB_HLtoDBCRes(l_eResHL);
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_DBC_RES eFSS_DBC_GetBuff(t_eFSS_DBC_Ctx* p_ptCtx, t_eFSS_TYPE_StorBuf* p_ptBuff)
{
	/* Local variable */
	e_eFSS_DBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ptBuff ) )
	{
		l_eRes = e_eFSS_DBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
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
                    l_eResHL = eFSS_COREHL_GetBuff(&p_ptCtx->tCOREHLCtx, p_ptBuff);
                    l_eRes = eFSS_DB_HLtoDBCRes(l_eResHL);
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_DBC_RES eFSS_DBC_GetBuffNStor(t_eFSS_DBC_Ctx* p_ptCtx, t_eFSS_TYPE_StorBuf* p_ptBuff,
                                     t_eFSS_TYPE_StorSet* p_ptStorSet)
{
	/* Local variable */
	e_eFSS_DBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ptBuff ) || ( NULL == p_ptStorSet ) )
	{
		l_eRes = e_eFSS_DBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
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
                    l_eResHL = eFSS_COREHL_GetBuffNStor(&p_ptCtx->tCOREHLCtx, p_ptBuff, p_ptStorSet);
                    l_eRes = eFSS_DB_HLtoDBCRes(l_eResHL);
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
    t_eFSS_TYPE_StorBuf p_ptBuff;

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
                    l_eResHL = eFSS_COREHL_GetBuffNStor(&p_ptCtx->tCOREHLCtx, &p_ptBuff, &l_tStorSet);
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

                            if( ( e_eFSS_DBC_RES_OK == l_eRes ) || ( e_eFSS_DBC_RES_OK_BKP_RCVRD == l_eRes ) )
                            {
                                /* check if the index is well formed */
                                if( p_uPageIndx != p_ptBuff.ptMeta->uPageUseSpec4 )
                                {
                                    /* The page of index p_uPageIndx is not marked with the same index */
                                    l_eRes = e_eFSS_DBC_RES_NOTVALIDDB;
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

e_eFSS_DBC_RES eFSS_DBC_FlushBuffInPage(t_eFSS_DBC_Ctx* const p_ptCtx, const uint32_t p_uPageIndx)
{
	/* Local variable */
	e_eFSS_DBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local var used for storage */
    t_eFSS_TYPE_StorSet l_tStorSet;
    t_eFSS_TYPE_StorBuf p_ptBuff;

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
                    l_eResHL = eFSS_COREHL_GetBuffNStor(&p_ptCtx->tCOREHLCtx,& p_ptBuff, &l_tStorSet);
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
                            p_ptBuff.ptMeta->uPageUseSpec4 = p_uPageIndx;
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