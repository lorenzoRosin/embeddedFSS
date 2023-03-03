/**
 * @file       eFSS_BLOB.c
 *
 * @brief      Blob Core - large file core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_BLOB.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_BLOB_IsStatusStillCoherent(const t_eFSS_BLOB_Ctx* p_ptCtx);
static e_eFSS_BLOB_RES eFSS_BLOB_BlobCtoBLOBRes(const e_eFSS_BLOBC_RES p_eCRes);




/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_BLOB_RES eFSS_BLOB_InitCtx(t_eFSS_BLOB_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx const p_tCtxCb,
                                  t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL)
{
    /* Return local var */
    e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOBC_RES l_eResC;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
        /* Can init low level context */
        l_eResC = eFSS_BLOBC_InitCtx(&p_ptCtx->tBLOBCCtx, p_tCtxCb, p_tStorSet, p_puBuff, p_uBuffL);
        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

        if( e_eFSS_BLOB_RES_OK == l_eRes )
        {
            /* Init internal context */
            p_ptCtx->uDataWritten = 0u;
            p_ptCtx->uCrcOfDataWritten = 0u;
            p_ptCtx->uCurrentSeqN = 0u;
        }
    }

    return l_eRes;
}

e_eFSS_BLOB_RES eFSS_BLOB_IsInit(t_eFSS_BLOB_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOBC_RES l_eResC;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
        l_eResC = eFSS_BLOBC_IsInit(&p_ptCtx->tBLOBCCtx, p_pbIsInit);
        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);
	}

	return l_eRes;
}

e_eFSS_BLOB_RES eFSS_BLOB_GetStorageStatus(t_eFSS_BLOB_Ctx* const p_ptCtx)
{
	/* Local return variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOBC_RES l_eResC;

    /* Local variable for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResC = eFSS_BLOBC_IsInit(&p_ptCtx->tBLOBCCtx, &l_bIsInit);
        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

        if( e_eFSS_BLOB_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOB_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_BLOB_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOB_RES_CORRUPTCTX;
                }
                else
                {
                    // l_eRes = eFSS_BLOB_OriginBackupAligner(p_ptCtx);
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOB_RES eFSS_BLOB_GetInfo(t_eFSS_BLOB_Ctx* const p_ptCtx, uint32_t* p_puBlobSize)
{
	/* Local return variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOBC_RES l_eResC;

    /* Local variable for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResC = eFSS_BLOBC_IsInit(&p_ptCtx->tBLOBCCtx, &l_bIsInit);
        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

        if( e_eFSS_BLOB_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOB_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_BLOB_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOB_RES_CORRUPTCTX;
                }
                else
                {
                    /* Fix any memory problem */
                    // l_eRes = eFSS_BLOB_OriginBackupAligner(p_ptCtx);
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOB_RES eFSS_BLOB_Format(t_eFSS_BLOB_Ctx* const p_ptCtx)
{
	/* Local return variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOBC_RES l_eResC;

    /* Local variable for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResC = eFSS_BLOBC_IsInit(&p_ptCtx->tBLOBCCtx, &l_bIsInit);
        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

        if( e_eFSS_BLOB_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOB_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_BLOB_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOB_RES_CORRUPTCTX;
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

e_eFSS_BLOB_RES eFSS_BLOB_ReadBlob(t_eFSS_BLOB_Ctx* const p_ptCtx, uint32_t p_uOffset, uint8_t* p_puBuff,
                                   uint32_t p_uBuffL)
{
	/* Local return variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOBC_RES l_eResC;

    /* Local variable for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResC = eFSS_BLOBC_IsInit(&p_ptCtx->tBLOBCCtx, &l_bIsInit);
        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

        if( e_eFSS_BLOB_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOB_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_BLOB_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOB_RES_CORRUPTCTX;
                }
                else
                {

                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOB_RES eFSS_BLOB_StartWrite(t_eFSS_BLOB_Ctx* const p_ptCtx)
{
	/* Local return variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOBC_RES l_eResC;

    /* Local variable for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResC = eFSS_BLOBC_IsInit(&p_ptCtx->tBLOBCCtx, &l_bIsInit);
        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

        if( e_eFSS_BLOB_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOB_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_BLOB_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOB_RES_CORRUPTCTX;
                }
                else
                {
                    /* Fix any memory problem */
                    l_eRes = eFSS_BLOB_OriginBackupAligner(p_ptCtx);

                    if( ( e_eFSS_BLOB_RES_OK == l_eRes ) || ( e_eFSS_BLOB_RES_OK_BKP_RCVRD == l_eRes ) )
                    {
                        /* Can return requested data if offset and size are coherent */
                        p_ptCtx->uDataWritten = 0u;
                        p_ptCtx->uCrcOfDataWritten = 0u;
                        p_ptCtx->uCurrentSeqN = 0u;
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOB_RES eFSS_BLOB_AppendData(t_eFSS_BLOB_Ctx* const p_ptCtx, uint8_t* p_puBuff, uint32_t p_uBuffL)
{
	/* Local return variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOBC_RES l_eResC;

    /* Local variable for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResC = eFSS_BLOBC_IsInit(&p_ptCtx->tBLOBCCtx, &l_bIsInit);
        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

        if( e_eFSS_BLOB_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOB_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_BLOB_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOB_RES_CORRUPTCTX;
                }
                else
                {
                    /* Fix any memory problem */
                    l_eRes = eFSS_BLOB_OriginBackupAligner(p_ptCtx);

                    if( ( e_eFSS_BLOB_RES_OK == l_eRes ) || ( e_eFSS_BLOB_RES_OK_BKP_RCVRD == l_eRes ) )
                    {
                        /* Load the needed page */
                        /* Write data needed (only of the original pages) */
                        /* DO NO GENERATE the backup pages */
                        /* Clear context */

                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOB_RES eFSS_BLOB_EndWrite(t_eFSS_BLOB_Ctx* const p_ptCtx)
{
	/* Local return variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOBC_RES l_eResC;

    /* Local variable for calculation */
    bool_t l_bIsInit;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eResC = eFSS_BLOBC_IsInit(&p_ptCtx->tBLOBCCtx, &l_bIsInit);
        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

        if( e_eFSS_BLOB_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_BLOB_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_BLOB_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_BLOB_RES_CORRUPTCTX;
                }
                else
                {
                    /* Fix any memory problem */
                    l_eRes = eFSS_BLOB_OriginBackupAligner(p_ptCtx);

                    if( ( e_eFSS_BLOB_RES_OK == l_eRes ) || ( e_eFSS_BLOB_RES_OK_BKP_RCVRD == l_eRes ) )
                    {
                        /* Calculate the CRC of the written data */
                        /* Save on the last page CRC and size of the data written (only of the original pages) */
                        /* Generate the backup pages */
                        /* Clear context */

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
static bool_t eFSS_BLOB_IsStatusStillCoherent(const t_eFSS_BLOB_Ctx* p_ptCtx)
{
    bool_t l_eRes;
    return l_eRes;
}

static e_eFSS_BLOB_RES eFSS_BLOB_BlobCtoBLOBRes(const e_eFSS_BLOBC_RES p_eCRes)
{
    e_eFSS_BLOB_RES l_eRes;

    switch(p_eCRes)
    {
        case e_eFSS_BLOBC_RES_OK:
        {
            l_eRes = e_eFSS_BLOB_RES_OK;
            break;
        }

        case e_eFSS_BLOBC_RES_NOINITLIB:
        {
            l_eRes = e_eFSS_BLOB_RES_NOINITLIB;
            break;
        }

        case e_eFSS_BLOBC_RES_BADPARAM:
        {
            l_eRes = e_eFSS_BLOB_RES_BADPARAM;
            break;
        }

        case e_eFSS_BLOBC_RES_BADPOINTER:
        {
            l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
            break;
        }

        case e_eFSS_BLOBC_RES_CORRUPTCTX:
        {
            l_eRes = e_eFSS_BLOB_RES_CORRUPTCTX;
            break;
        }

        case e_eFSS_BLOBC_RES_CLBCKERASEERR:
        {
            l_eRes = e_eFSS_BLOB_RES_CLBCKERASEERR;
            break;
        }

        case e_eFSS_BLOBC_RES_CLBCKWRITEERR:
        {
            l_eRes = e_eFSS_BLOB_RES_CLBCKWRITEERR;
            break;
        }

        case e_eFSS_BLOBC_RES_CLBCKREADERR:
        {
            l_eRes = e_eFSS_BLOB_RES_CLBCKREADERR;
            break;
        }

        case e_eFSS_BLOBC_RES_CLBCKCRCERR:
        {
            l_eRes = e_eFSS_BLOB_RES_CLBCKCRCERR;
            break;
        }

        case e_eFSS_BLOBC_RES_NEWVERSIONBLOB:
        {
            l_eRes = e_eFSS_BLOB_RES_NEWVERSIONBLOB;
            break;
        }

        case e_eFSS_BLOBC_RES_NOTVALIDBLOB:
        {
            l_eRes = e_eFSS_BLOB_RES_NOTVALIDBLOB;
            break;
        }

        case e_eFSS_BLOBC_RES_WRITENOMATCHREAD:
        {
            l_eRes = e_eFSS_BLOB_RES_WRITENOMATCHREAD;
            break;
        }

        case e_eFSS_BLOBC_RES_OK_BKP_RCVRD:
        {
            l_eRes = e_eFSS_BLOB_RES_OK_BKP_RCVRD;
            break;
        }

        default:
        {
            l_eRes = e_eFSS_BLOB_RES_CORRUPTCTX;
            break;
        }
    }

    return l_eRes;
}

