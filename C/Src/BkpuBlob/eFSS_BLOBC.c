/**
 * @file       eFSS_BLOBC.c
 *
 * @brief      Blob Core - large file core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/* In this module the page field has the following meaning:
 * - uPageUseSpecific1 -> (Valid only on the last page) Size of the stored blob
 * - uPageUseSpecific2 -> (Valid only on the last page) Crc of the stored blob
 * - uPageUseSpecific3 -> Total page given during formatting
 * - uPageUseSpecific4 -> Sequential number of the page
 *
 * In this module the original pages are written before the backup pages
 *
 * In this module the storage is organizated as follow :
 * - [ 0                 -    ( uTotPages / 2 ) - 1 ]  -> Original pages
 * - [ ( uTotPages / 2 ) -    uTotPages - 1 ]          -> Backup pages
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

            if( e_eFSS_BLOBC_RES_OK == l_eRes )
            {
                /* Init internal context */
            }
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

e_eFSS_BLOBC_RES eFSS_BLOBC_GetStorageStatus(t_eFSS_BLOBC_Ctx* const p_ptCtx)
{
	/* Local return variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local variable for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_DBC_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_DB_DBCtoDBFLRes(l_eResHL);

        if( e_eFSS_BLOBC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOBC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_DB_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
                }
                else
                {
                    l_eRes = eFSS_BLOBC_OriginBackupAligner(p_ptCtx);
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_BLOBC_GetInfo(t_eFSS_BLOBC_Ctx* const p_ptCtx, uint32_t* p_puBlobSize)
{
	/* Local return variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local variable for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_DBC_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_DB_DBCtoDBFLRes(l_eResHL);

        if( e_eFSS_BLOBC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOBC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_DB_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
                }
                else
                {
                    /* Fix any memory problem */
                    l_eRes = eFSS_BLOBC_OriginBackupAligner(p_ptCtx);

                    if( ( e_eFSS_BLOBC_RES_OK == l_eRes ) || ( e_eFSS_BLOBC_RES_OK_BKP_RCVRD == l_eRes ) )
                    {
                        /* Get the last page of the memory area and read the parameter uPageUseSpecific1 */
                        /* Do no return if write is open */
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_BLOBC_Format(t_eFSS_BLOBC_Ctx* const p_ptCtx)
{
	/* Local return variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local variable for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_DBC_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_DB_DBCtoDBFLRes(l_eResHL);

        if( e_eFSS_BLOBC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOBC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_DB_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
                }
                else
                {
                    /* write at zero every page and bakup pages, do no use the backuped pages function,
                       first write all original pages, and only after the backup one */
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_BLOBC_ReadBlob(t_eFSS_BLOBC_Ctx* const p_ptCtx, uint32_t p_uOffset, uint8_t* p_puBuff,
                                     uint32_t p_uBuffL)
{
	/* Local return variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local variable for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_DBC_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_DB_DBCtoDBFLRes(l_eResHL);

        if( e_eFSS_BLOBC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOBC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_DB_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
                }
                else
                {
                    /* Fix any memory problem */
                    l_eRes = eFSS_BLOBC_OriginBackupAligner(p_ptCtx);

                    if( ( e_eFSS_BLOBC_RES_OK == l_eRes ) || ( e_eFSS_BLOBC_RES_OK_BKP_RCVRD == l_eRes ) )
                    {
                        /* Can return requested data if offset and size are coherent */
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_BLOBC_StartWrite(t_eFSS_BLOBC_Ctx* const p_ptCtx)
{
	/* Local return variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local variable for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_DBC_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_DB_DBCtoDBFLRes(l_eResHL);

        if( e_eFSS_BLOBC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOBC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_DB_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
                }
                else
                {
                    /* Fix any memory problem */
                    l_eRes = eFSS_BLOBC_OriginBackupAligner(p_ptCtx);

                    if( ( e_eFSS_BLOBC_RES_OK == l_eRes ) || ( e_eFSS_BLOBC_RES_OK_BKP_RCVRD == l_eRes ) )
                    {
                        /* Can return requested data if offset and size are coherent */
                        p_ptCtx->bIsWriteOnGoing = true;
                        p_ptCtx->uDataWritten = 0u;
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_BLOBC_AppendData(t_eFSS_BLOBC_Ctx* const p_ptCtx, uint8_t* p_puBuff, uint32_t p_uBuffL)
{
	/* Local return variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local variable for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_DBC_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_DB_DBCtoDBFLRes(l_eResHL);

        if( e_eFSS_BLOBC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOBC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_DB_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
                }
                else
                {
                    /* Fix any memory problem */
                    l_eRes = eFSS_BLOBC_OriginBackupAligner(p_ptCtx);

                    if( ( e_eFSS_BLOBC_RES_OK == l_eRes ) || ( e_eFSS_BLOBC_RES_OK_BKP_RCVRD == l_eRes ) )
                    {
                        /* Load the needed page */
                        /* Write data needed (only of the original pages) */
                        /* DO NO GENERATE the backup pages */
                        /* Clear context */
                        p_ptCtx->bIsWriteOnGoing = false;
                        p_ptCtx->uDataWritten += p_uBuffL;
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOBC_RES eFSS_BLOBC_EndWrite(t_eFSS_BLOBC_Ctx* const p_ptCtx)
{
	/* Local return variable */
	e_eFSS_BLOBC_RES l_eRes;
    e_eFSS_COREHL_RES l_eResHL;

    /* Local variable for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOBC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResHL = eFSS_DBC_IsInit(&p_ptCtx->tCOREHLCtx, &l_bIsInit);
        l_eRes = eFSS_DB_DBCtoDBFLRes(l_eResHL);

        if( e_eFSS_BLOBC_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOBC_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_DB_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOBC_RES_CORRUPTCTX;
                }
                else
                {
                    /* Fix any memory problem */
                    l_eRes = eFSS_BLOBC_OriginBackupAligner(p_ptCtx);

                    if( ( e_eFSS_BLOBC_RES_OK == l_eRes ) || ( e_eFSS_BLOBC_RES_OK_BKP_RCVRD == l_eRes ) )
                    {
                        /* Calculate the CRC of the written data */
                        /* Save on the last page CRC and size of the data written (only of the original pages) */
                        /* Generate the backup pages */
                        /* Clear context */
                        p_ptCtx->bIsWriteOnGoing = false;
                        p_ptCtx->uDataWritten = 0u;
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
static bool_t eFSS_BLOBC_IsStatusStillCoherent(const t_eFSS_BLOBC_Ctx* p_ptCtx)
{
    bool_t l_eRes;



    return l_eRes;
}

static e_eFSS_BLOBC_RES eFSS_BLOBC_HLtoBLOBRes(const e_eFSS_COREHL_RES p_eHLRes)
{

}

