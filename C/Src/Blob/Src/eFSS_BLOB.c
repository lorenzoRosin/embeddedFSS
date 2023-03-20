/**
 * @file       eFSS_BLOB.c
 *
 * @brief      Blob large file module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/* In this module we need to keep in mind a few things:
 * 1 - The file in order to be valid must have the same sequntial number on every pages.
 * 2 - We can call write function multiple time, but we can stored the value of file size and CRC only when the writing
 *     is DONE. So during the write process we could have and invalid file on the originals page, but we will have
 *     have a valid file on the backup pages.
 * 3 - File CRC and SIZE are store on the last 8 byte ot the last page, in this order: FILE LENGTH, FILE CRC
 * 4 - During the write process we are not able to perform any additcional operation
 * 5 - Buffer passed during init as support buffer can be used for other operation when we are not calling a module
 *     function.
 * 6 - Generate backup pages only when the write process is ended.
 */

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_BLOB.h"
#include "eFSS_Utils.h"



/***********************************************************************************************************************
 *      PRIVATE DEFINE
 **********************************************************************************************************************/
#define EFSS_BLOB_MINPAGESIZE                                                                    ( ( uint32_t )  0x09u )
#define EFSS_BLOB_LENOFF                                                                         ( ( uint32_t )  0x08u )
#define EFSS_BLOB_CRCOFF                                                                         ( ( uint32_t )  0x04u )



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_BLOB_IsStatusStillCoherent(t_eFSS_BLOB_Ctx* const p_ptCtx);
static e_eFSS_BLOB_RES eFSS_BLOB_BlobCtoBLOBRes(const e_eFSS_BLOBC_RES p_eCRes);



/***********************************************************************************************************************
 *  PRIVATE UTILS STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static e_eFSS_BLOB_RES eFSS_BLOB_OriginBackupAligner(t_eFSS_BLOB_Ctx* const p_ptCtx);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_BLOB_RES eFSS_BLOB_InitCtx(t_eFSS_BLOB_Ctx* const p_ptCtx, const t_eFSS_TYPE_CbStorCtx p_tCtxCb,
                                  const t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff,
                                  const uint32_t p_uBuffL)
{
    /* Return local var */
    e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOBC_RES l_eResC;

    /* Local storage variable */
    t_eFSS_BLOBC_StorBuf l_tBuff;
    uint32_t l_uUsePages;

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
            /* Get usable pages and buffer length so we can check blob default value validity */
            l_uUsePages = 0u;
            l_eResC = eFSS_BLOBC_GetBuffNUsable(&p_ptCtx->tBLOBCCtx, &l_tBuff, &l_uUsePages);
            l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

            if( e_eFSS_BLOB_RES_OK == l_eRes )
            {
                /* Check if page length is OK */
                if( l_tBuff.uBufL < EFSS_BLOB_MINPAGESIZE )
                {
                    /* We need more space for the DB */
                    l_eRes = e_eFSS_BLOB_RES_BADPARAM;

                    /* De init BLOBC */
                    (void)memset(&p_ptCtx->tBLOBCCtx, 0, sizeof(t_eFSS_BLOBC_Ctx));
                }
                else
                {
                    /* All ok, fill context */
                    p_ptCtx->bIsWriteOngoing = false;
                    p_ptCtx->uDataWritten = 0u;
                    p_ptCtx->uCrcOfDataWritten = 0u;
                    p_ptCtx->uCurrentSeqN = 0u;

                    /* Set to false fo the first operation will trigger a stored BLOB check */
                    p_ptCtx->bIsBlobCheked = false;
                }
            }
            else
            {
                /* De init BLOBC */
                (void)memset(&p_ptCtx->tBLOBCCtx, 0, sizeof(t_eFSS_BLOBC_Ctx));
            }
        }
    }

    return l_eRes;
}

e_eFSS_BLOB_RES eFSS_BLOB_IsInit(t_eFSS_BLOB_Ctx* const p_ptCtx, bool_t* const p_pbIsInit)
{
	/* Local variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOBC_RES l_eResC;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
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
        l_bIsInit = false;
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

                        if( ( e_eFSS_BLOB_RES_OK == l_eRes ) || ( e_eFSS_BLOB_RES_OK_BKP_RCVRD == l_eRes ) )
                        {
                            /* Signal it has checked so we dont need to redo this ckeck every time */
                            p_ptCtx->bIsBlobCheked = true;
                        }
                        else
                        {
                            /* Be sure to redo the check */
                            p_ptCtx->bIsBlobCheked = false;
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOB_RES eFSS_BLOB_GetInfo(t_eFSS_BLOB_Ctx* const p_ptCtx, uint32_t* const p_puBlobSize)
{
	/* Local return variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOBC_RES l_eResC;

    /* Local storage variable */
    t_eFSS_BLOBC_StorBuf l_tBuff;
    uint32_t l_uUsePages;
    uint32_t l_uSeqNumb;

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
        l_bIsInit = false;
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
                        if( false == p_ptCtx->bIsBlobCheked )
                        {
                            l_eRes = eFSS_BLOB_OriginBackupAligner(p_ptCtx);

                            if( ( e_eFSS_BLOB_RES_OK == l_eRes ) || ( e_eFSS_BLOB_RES_OK_BKP_RCVRD == l_eRes ) )
                            {
                                /* Signal it has checked so we dont need to redo this ckeck every time */
                                p_ptCtx->bIsBlobCheked = true;
                            }
                            else
                            {
                                /* Be sure to redo the check */
                                p_ptCtx->bIsBlobCheked = false;
                            }
                        }

                        if( ( e_eFSS_BLOB_RES_OK == l_eRes ) || ( e_eFSS_BLOB_RES_OK_BKP_RCVRD == l_eRes ) )
                        {
                            /* Retrive parameter from the last page, we are sure that the data is aligned because
                               we know that we have already called eFSS_BLOB_OriginBackupAligner */
                            l_uUsePages = 0u;
                            l_eResC = eFSS_BLOBC_GetBuffNUsable(&p_ptCtx->tBLOBCCtx, &l_tBuff, &l_uUsePages);
                            l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                            if( e_eFSS_BLOB_RES_OK == l_eRes )
                            {
                                /* Load the last page */
                                l_eResC = eFSS_BLOBC_LoadBufferFromPage(&p_ptCtx->tBLOBCCtx, true,
                                                                        ( l_uUsePages - 1u ), &l_uSeqNumb);
                                l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);
                                if( e_eFSS_BLOB_RES_OK == l_eRes )
                                {
                                    /* Extract data from the last page */
                                    if( true != eFSS_Utils_RetriveU32(&l_tBuff.puBuf[l_tBuff.uBufL - EFSS_BLOB_LENOFF],
                                                                      p_puBlobSize ) )
                                    {
                                        l_eRes = e_eFSS_BLOB_RES_CORRUPTCTX;
                                    }
                                    else
                                    {
                                        /* Is retrived size coherent? */
                                        if( *p_puBlobSize > ( ( l_uUsePages * l_tBuff.uBufL ) - EFSS_BLOB_LENOFF ) )
                                        {
                                            l_eRes = e_eFSS_BLOB_RES_NOTVALIDBLOB;
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

e_eFSS_BLOB_RES eFSS_BLOB_Format(t_eFSS_BLOB_Ctx* const p_ptCtx)
{
	/* Local return variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOBC_RES l_eResC;

    /* Local variable for calculation */
    bool_t l_bIsInit;
    t_eFSS_BLOBC_StorBuf l_tBuff;
    uint32_t l_uUsableP;
    uint32_t l_uUsePages;
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
        l_bIsInit = false;
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
                        l_uUsePages = 0u;
                        l_eResC = eFSS_BLOBC_GetBuffNUsable(&p_ptCtx->tBLOBCCtx, &l_tBuff, &l_uUsePages);
                        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                        if( e_eFSS_BLOB_RES_OK == l_eRes )
                        {
                            /* Erase internal status data */
                            p_ptCtx->bIsBlobCheked = true;
                            p_ptCtx->bIsWriteOngoing = false;
                            p_ptCtx->uDataWritten = 0u;
                            p_ptCtx->uCrcOfDataWritten = 0u;
                            p_ptCtx->uCurrentSeqN = 0u;

                            /* Init crc with default seed */
                            l_uBlobCrc = 0xFFFFFFFFu;
                            l_uCurrPage = 0u;

                            /* Write at zero every pages of the origin area, after that update backup one  */
                            while( ( l_uCurrPage < l_uUsableP ) && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                            {
                                memset(l_tBuff.puBuf, 0u, l_tBuff.uBufL);

                                if( ( l_uUsableP - 1u ) == l_uCurrPage )
                                {
                                    /* last page, calculate the CRC but exlude the CRC itself, and after that insert
                                       it inside the last page */
                                    l_eResC = eFSS_BLOBC_GetCrcFromTheBuffer(&p_ptCtx->tBLOBCCtx, l_uBlobCrc, &l_uBlobCrc);
                                    l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                                    {
                                        if( true != eFSS_Utils_InsertU32(&l_tBuff.puBuf[l_tBuff.uBufL - EFSS_BLOB_LENOFF],
                                                                         l_uBlobCrc ) )
                                        {
                                            l_eRes = e_eFSS_BLOB_RES_CORRUPTCTX;
                                        }
                                    }
                                }
                                else
                                {
                                    l_eResC = eFSS_BLOBC_GetCrcFromTheBuffer(&p_ptCtx->tBLOBCCtx, l_uBlobCrc, &l_uBlobCrc);
                                    l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);
                                }

                                if( e_eFSS_BLOB_RES_OK == l_eRes )
                                {
                                    l_eResC = eFSS_BLOBC_FlushBufferInPage(&p_ptCtx->tBLOBCCtx, true, l_uCurrPage, 0u );
                                    l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);
                                }

                                l_uCurrPage++;
                            }

                            if( e_eFSS_BLOB_RES_OK == l_eRes )
                            {
                                /* Original pages are OK, need to clone backup */
                                l_uCurrPage = 0u;

                                while( ( l_uCurrPage < l_uUsableP ) && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                                {
                                    l_eResC = eFSS_BLOBC_CopyOriInBkpIfNotEquals(&p_ptCtx->tBLOBCCtx, l_uCurrPage);
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

e_eFSS_BLOB_RES eFSS_BLOB_ReadBlob(t_eFSS_BLOB_Ctx* const p_ptCtx, const uint32_t p_uOffset, const uint32_t p_uBuffL,
                                   uint8_t* const p_puBuff)
{
	/* Local return variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOBC_RES l_eResC;

    /* Local variable for calculation */
    bool_t l_bIsInit;
    t_eFSS_BLOBC_StorBuf l_tBuff;
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
        l_bIsInit = false;
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
        l_bIsInit = false;
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

e_eFSS_BLOB_RES eFSS_BLOB_AppendData(t_eFSS_BLOB_Ctx* const p_ptCtx, uint8_t* const p_puBuff, const uint32_t p_uBuffL)
{
	/* Local return variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOBC_RES l_eResC;

    /* Local variable for calculation */
    bool_t l_bIsInit;
    t_eFSS_BLOBC_StorBuf l_tBuff;
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
        l_bIsInit = false;
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
        l_bIsInit = false;
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
                        l_eResC = eFSS_BLOBC_GenerateBkup(&p_ptCtx->tBLOBCCtx);
                        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_BLOB_RES eFSS_BLOB_AbortWrite(t_eFSS_BLOB_Ctx* const p_ptCtx)
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
        l_bIsInit = false;
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
                        l_eResC = eFSS_BLOBC_GenerateBkup(&p_ptCtx->tBLOBCCtx);
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
static bool_t eFSS_BLOB_IsStatusStillCoherent(t_eFSS_BLOB_Ctx* const p_ptCtx)
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

static e_eFSS_BLOB_RES eFSS_BLOB_OriginBackupAligner(t_eFSS_BLOB_Ctx* const p_ptCtx)
{
	/* Local return variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOBC_RES l_eResC;

    /* Local variable for calculation */
    bool_t l_bIsInit;
    t_eFSS_BLOBC_StorBuf l_tBuff;
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
                    l_eResC = eFSS_BLOBC_GetCrcFromTheBuffer(&p_ptCtx->tBLOBCCtx, l_uBlobCrc, &l_uBlobCrc);
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
                        l_eResC = eFSS_BLOBC_GetCrcFromTheBuffer(&p_ptCtx->tBLOBCCtx, l_uBlobCrc, &l_uBlobCrc);
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