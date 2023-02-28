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
 *   bFullBckup = true, bFlashCache = true
 * - [ 0                            -    ( uTotPages / 2 ) - 1 ]  -> Original pages
 * - [ ( uTotPages / 2 )            -    uTotPages - 1         ]  -> Backup pages
 *
 * First write original pages and after the backup pages
 * Only the backup pages have a valid size and and valid crc (because the crc and size calculation is updated at the
 * end).
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
static bool_t eFSS_BLOBC_IsStatusStillCoherent(const t_eFSS_BLOBC_Ctx* p_ptCtx);
static e_eFSS_BLOBC_RES eFSS_BLOBC_HLtoBLOBRes(const e_eFSS_COREHL_RES p_eHLRes);




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
            l_eRes = eFSS_DB_HLtoDBRes(l_eResHL);
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
        l_eRes = eFSS_DB_HLtoDBRes(l_eResHL);
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
                if( false == eFSS_BLOBC_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eResHL = eFSS_COREHL_GetBuff(&p_ptCtx->tCOREHLCtx, p_ptBuff);
                    l_eRes = eFSS_LOGC_HLtoLOGCRes(l_eResHL);
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_LOGC_GetOriginalPageMeta(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t p_uIdx)
{

}

e_eFSS_BLOBC_RES eFSS_LOGC_GetBckupPageMeta(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t p_uIdx)
{

}

e_eFSS_BLOBC_RES eFSS_LOGC_FlushBufferInNewPage(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t p_uIdx)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_LOGC_LoadBufferFromNewPage(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t p_uIdx)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_LOGC_FlushBufferInBkupPage(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t p_uIdx)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_LOGC_LoadBufferFromBkupPage(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t p_uIdx)
{
	/* Local variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_LOGC_GetCrcFromTheBuffer(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t p_uIdx)
{

}

e_eFSS_BLOBC_RES eFSS_LOGC_GenerateBkup(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t p_uIdx)
{

}

e_eFSS_BLOBC_RES eFSS_LOGC_RipristinateOriginal(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t p_uIdx)
{


}


/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eFSS_BLOBC_IsStatusStillCoherent(const t_eFSS_BLOBC_Ctx* p_ptCtx)
{
    bool_t l_eRes;



    return l_eRes;
}

static e_eFSS_BLOBC_RES eFSS_BLOBC_HLtoBLOBRes(const e_eFSS_COREHL_RES p_eHLRes)
{

}

