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
static e_eFSS_BLOB_RES eFSS_BLOB_OriginBackupAligner(t_eFSS_BLOB_Ctx*  p_ptCtx);




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
            p_ptCtx->bIsWriteOngoing = false;
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
                    if( true == p_ptCtx->bIsWriteOngoing )
                    {
                        l_eRes = e_eFSS_BLOB_RES_WRITEONGOING;
                    }
                    else
                    {
                        l_eRes = eFSS_BLOB_OriginBackupAligner(p_ptCtx);
                    }
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
    uint32_t l_uBlobCrc;

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
                    if( true == p_ptCtx->bIsWriteOngoing )
                    {
                        l_eRes = e_eFSS_BLOB_RES_WRITEONGOING;
                    }
                    else
                    {
                        /* Fix any memory problem */
                        l_eRes = eFSS_BLOB_OriginBackupAligner(p_ptCtx);

                        if( ( e_eFSS_BLOB_RES_OK == l_eRes ) || ( e_eFSS_BLOB_RES_OK_BKP_RCVRD == l_eRes ) )
                        {
                            /* Now we are sure page are aligned */
                            l_eResC = eFSS_LOGC_GetOriginalPageMeta(&p_ptCtx->tBLOBCCtx, p_puBlobSize, &l_uBlobCrc);
                            l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);
                        }
                    }
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
    t_eFSS_TYPE_StorBuf l_tBuff;
    uint32_t l_uUsableP;
    uint32_t l_uCurrPage;
    uint32_t l_uBlobCrc;

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
                    l_eResC = eFSS_BLOBC_GetBuff(&p_ptCtx->tBLOBCCtx, &l_tBuff);
                    l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                    {
                        l_eResC = eFSS_BLOBC_GetUsablePage(&p_ptCtx->tBLOBCCtx, &l_uUsableP);
                        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                        if( e_eFSS_BLOB_RES_OK == l_eRes )
                        {
                            /* An eventual pending write is aborted */
                            p_ptCtx->bIsWriteOngoing = false;
                            p_ptCtx->uDataWritten = 0u;
                            p_ptCtx->uCrcOfDataWritten = 0u;
                            p_ptCtx->uCurrentSeqN = 0u;

                            /* Init crc with default seed */
                            l_uBlobCrc = 0xFFFFFFFFu;

                            /* write at zero every page and bakup pages, do no use the backuped pages function,
                            first write all original pages, and only after the backup one */
                            while( ( l_uCurrPage < l_uUsableP ) && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                            {
                                memset(l_tBuff.ptMeta, 0u, sizeof(t_eFSS_TYPE_PageMeta) );
                                memset(l_tBuff.puBuf,  0u, l_tBuff.uBufL);
                                l_eResC = eFSS_LOGC_GetCrcFromTheBuffer(&p_ptCtx->tBLOBCCtx, l_uBlobCrc, &l_uBlobCrc);
                                l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                                if( e_eFSS_BLOB_RES_OK == l_eRes )
                                {
                                    l_eResC = eFSS_LOGC_FlushBufferInNewPage(&p_ptCtx->tBLOBCCtx, 0u, l_uCurrPage);
                                    l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);
                                }

                                l_uCurrPage++;
                            }

                            if( e_eFSS_BLOB_RES_OK == l_eRes )
                            {
                                /* Can finalize the blob writing it's size and CRC */
                                l_eResC = eFSS_LOGC_SetOriginalPageMeta(&p_ptCtx->tBLOBCCtx, 0u, l_uBlobCrc);
                                l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                                if( e_eFSS_BLOB_RES_OK == l_eRes )
                                {
                                    /* Can finalize the blob writing it's size and CRC */
                                    l_eResC = eFSS_LOGC_GenerateBkup(&p_ptCtx->tBLOBCCtx);
                                    l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);
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

e_eFSS_BLOB_RES eFSS_BLOB_ReadBlob(t_eFSS_BLOB_Ctx* const p_ptCtx, uint32_t p_uOffset, uint8_t* p_puBuff,
                                   uint32_t p_uBuffL)
{
	/* Local return variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOBC_RES l_eResC;

    /* Local variable for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorBuf l_tBuff;
    uint32_t l_uUsableP;
    uint32_t l_uBlobCrc;
    uint32_t l_uBlobSize;
    uint32_t l_uCurrPage;
    uint32_t l_uCurrPageR;
    uint32_t l_uRemToRead;
    uint32_t l_uSeqN;

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
                    l_eResC = eFSS_BLOBC_GetBuff(&p_ptCtx->tBLOBCCtx, &l_tBuff);
                    l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                    {
                        l_eResC = eFSS_BLOBC_GetUsablePage(&p_ptCtx->tBLOBCCtx, &l_uUsableP);
                        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                        if( e_eFSS_BLOB_RES_OK == l_eRes )
                        {
                            /* Fix any memory problem */
                            l_eRes = eFSS_BLOB_OriginBackupAligner(p_ptCtx);

                            if( ( e_eFSS_BLOB_RES_OK == l_eRes ) || ( e_eFSS_BLOB_RES_OK_BKP_RCVRD == l_eRes ) )
                            {
                                /* Now we are sure page are aligned */
                                l_eResC = eFSS_LOGC_GetOriginalPageMeta(&p_ptCtx->tBLOBCCtx, &l_uBlobSize, &l_uBlobCrc);
                                l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);
                                if( e_eFSS_BLOB_RES_OK == l_eRes )
                                {
                                    /* possible to read? */
                                    if( ( p_uOffset + p_uBuffL ) > l_uBlobSize)
                                    {
                                        l_eRes = e_eFSS_BLOB_RES_BADPARAM;
                                    }
                                    else
                                    {
                                        /* Find starting page */
                                        l_uRemToRead = p_uBuffL;
                                        l_uCurrPage = (uint32_t)(p_uOffset / l_tBuff.uBufL);
                                        l_uCurrPageR = (uint32_t)(p_uOffset % l_tBuff.uBufL);

                                        while( ( l_uRemToRead > 0u) && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                                        {
                                            l_eResC = eFSS_LOGC_LoadBufferFromNewPage(&p_ptCtx->tBLOBCCtx, &l_uSeqN, l_uCurrPage);
                                            l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                                            if( e_eFSS_BLOB_RES_OK == l_eRes )
                                            {

                                                if( l_uRemToRead > (l_tBuff.uBufL - l_uCurrPageR) )
                                                {
                                                    memcpy(p_puBuff, &l_tBuff.puBuf[l_uCurrPageR], (l_tBuff.uBufL - l_uCurrPageR) );
                                                    l_uCurrPageR = 0u;
                                                    l_uRemToRead -= (l_tBuff.uBufL - l_uCurrPageR);
                                                }
                                                else
                                                {
                                                    memcpy(p_puBuff, &l_tBuff.puBuf[l_uCurrPageR], l_uRemToRead);
                                                    l_uCurrPageR = 0u;
                                                    l_uRemToRead = 0u;
                                                }
                                                l_uCurrPage++;
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

e_eFSS_BLOB_RES eFSS_BLOB_StartWrite(t_eFSS_BLOB_Ctx* const p_ptCtx)
{
	/* Local return variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOBC_RES l_eResC;

    /* Local variable for calculation */
    bool_t l_bIsInit;
    uint32_t l_uBlobCrc;
    uint32_t l_uBlobSize;
    uint32_t l_uSeqN;

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
                    /* Fix any memory problem before start writing */
                    l_eRes = eFSS_BLOB_OriginBackupAligner(p_ptCtx);

                    if( ( e_eFSS_BLOB_RES_OK == l_eRes ) || ( e_eFSS_BLOB_RES_OK_BKP_RCVRD == l_eRes ) )
                    {
                        l_eResC = eFSS_LOGC_LoadBufferFromNewPage(&p_ptCtx->tBLOBCCtx, &l_uSeqN, 0u);
                        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                        if( e_eFSS_BLOB_RES_OK == l_eRes )
                        {
                            /* Can return requested data if offset and size are coherent */
                            p_ptCtx->bIsWriteOngoing = true;
                            p_ptCtx->uDataWritten = 0u;
                            p_ptCtx->uCrcOfDataWritten = 0u;
                            p_ptCtx->uCurrentSeqN = l_uSeqN + 1u;
                        }
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
    t_eFSS_TYPE_StorBuf l_tBuff;
    uint32_t l_uUsableP;
    uint32_t l_uBlobCrc;
    uint32_t l_uBlobSize;
    uint32_t l_uCurrPage;
    uint32_t l_uCurrPageR;
    uint32_t l_uRemToWrite;
    uint32_t l_uSeqN;

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
                    l_eResC = eFSS_BLOBC_GetBuff(&p_ptCtx->tBLOBCCtx, &l_tBuff);
                    l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                    {
                        l_eResC = eFSS_BLOBC_GetUsablePage(&p_ptCtx->tBLOBCCtx, &l_uUsableP);
                        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                        if( e_eFSS_BLOB_RES_OK == l_eRes )
                        {
                            /* possible to write? */
                            if( ( p_uBuffL + p_ptCtx->uDataWritten ) > ( l_uUsableP * l_tBuff.uBufL ) )
                            {
                                l_eRes = e_eFSS_BLOB_RES_BADPARAM;
                            }
                            else
                            {
                                /* Find starting page */
                                l_uRemToWrite = p_uBuffL;
                                l_uCurrPage = (uint32_t)(p_ptCtx->uDataWritten / l_tBuff.uBufL);
                                l_uCurrPageR = (uint32_t)(p_ptCtx->uDataWritten % l_tBuff.uBufL);

                                while( ( l_uRemToWrite > 0u) && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                                {
                                    /* First load the page that we want to write */
                                    l_eResC = eFSS_LOGC_LoadBufferFromNewPage(&p_ptCtx->tBLOBCCtx, &l_uSeqN, l_uCurrPage);
                                    l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                                    {
                                        if( l_uRemToWrite > (l_tBuff.uBufL - l_uCurrPageR) )
                                        {
                                            memcpy(p_puBuff, &l_tBuff.puBuf[l_uCurrPageR], (l_tBuff.uBufL - l_uCurrPageR) );
                                            l_uCurrPageR = 0u;
                                            l_uRemToWrite -= (l_tBuff.uBufL - l_uCurrPageR);
                                        }
                                        else
                                        {
                                            /* Set to zero the unused data */
                                            memset(&l_tBuff.puBuf, 0u, l_tBuff.uBufL);
                                            memcpy(p_puBuff, &l_tBuff.puBuf[l_uCurrPageR], l_uRemToWrite);
                                            l_uCurrPageR = 0u;
                                            l_uRemToWrite = 0u;
                                        }

                                        /* Buffer flush */
                                        l_eResC = eFSS_LOGC_FlushBufferInNewPage(&p_ptCtx->tBLOBCCtx, p_ptCtx->uCurrentSeqN, l_uCurrPage);
                                        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                                        l_uCurrPage++;
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
                    l_eResC = eFSS_LOGC_SetOriginalPageMeta(&p_ptCtx->tBLOBCCtx, p_ptCtx->uDataWritten, p_ptCtx->uCrcOfDataWritten);
                    l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                    {
                        l_eResC = eFSS_LOGC_GenerateBkup(&p_ptCtx->tBLOBCCtx);
                        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);
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

static e_eFSS_BLOB_RES eFSS_BLOB_OriginBackupAligner(t_eFSS_BLOB_Ctx*  p_ptCtx)
{
	/* Local return variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOBC_RES l_eResC;

    /* Local variable for calculation */
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorBuf l_tBuff;
    uint32_t l_uUsableP;
    uint32_t l_uBlobCrc;
    uint32_t l_uBlobSize;
    uint32_t l_uCurrPage;
    uint32_t l_uCurrPageR;
    uint32_t l_uRemToWrite;
    uint32_t l_uSeqN;
    bool_t l_bIsValidOrig;
    bool_t l_bIsValidBkup;

    l_eResC = eFSS_BLOBC_GetBuff(&p_ptCtx->tBLOBCCtx, &l_tBuff);
    l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

    if( e_eFSS_BLOB_RES_OK == l_eRes )
    {
        l_eResC = eFSS_BLOBC_GetUsablePage(&p_ptCtx->tBLOBCCtx, &l_uUsableP);
        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

        if( e_eFSS_BLOB_RES_OK == l_eRes )
        {
            /* Read all original pages, generate the page crc, verify sequential number and verify metadata validity */
            l_uCurrPage = 0u;

            while( ( l_uCurrPage < l_uUsableP ) && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
            {
                l_eResC = eFSS_LOGC_LoadBufferFromNewPage(&p_ptCtx->tBLOBCCtx, &l_uSeqN, l_uCurrPage);
                l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                if( e_eFSS_BLOB_RES_OK == l_eRes )
                {
                    l_eResC = eFSS_LOGC_GetCrcFromTheBuffer(&p_ptCtx->tBLOBCCtx, l_uBlobCrc, &l_uBlobCrc);
                    l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);
                }

                l_uCurrPage++;
            }

            if( e_eFSS_BLOB_RES_OK == l_eRes )
            {
                while( ( l_uCurrPage < l_uUsableP ) && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                {
                    l_eResC = eFSS_LOGC_LoadBufferFromNewPage(&p_ptCtx->tBLOBCCtx, &l_uSeqN, l_uCurrPage);
                    l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                    {
                        l_eResC = eFSS_LOGC_GetCrcFromTheBuffer(&p_ptCtx->tBLOBCCtx, l_uBlobCrc, &l_uBlobCrc);
                        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);
                    }

                    l_uCurrPage++;
                }

                if( e_eFSS_BLOB_RES_OK == l_eRes )
                {
                    if( ( true == l_bIsValidOrig ) && ( true == l_bIsValidBkup ) )
                    {

                    }
                    else if( ( true == l_bIsValidOrig ) && ( true == l_bIsValidBkup ) )
                    {

                    }
                    else if( ( true == l_bIsValidOrig ) && ( true == l_bIsValidBkup ) )
                    {

                    }
                    else
                    {
                        l_eRes = e_eFSS_BLOB_RES_NOTVALIDBLOB;
                    }
                }
            }
        }
    }

	return l_eRes;
}