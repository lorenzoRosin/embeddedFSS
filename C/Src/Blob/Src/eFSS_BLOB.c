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
 * 2 - We can call write function multiple time, but we can stored the value of file size and CRC only when the write
 *     process is DONE. So during the write process we could have and invalid file on the originals page, but we will
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
#define EFSS_BLOB_MINPAGESIZE                                                             ( ( uint32_t )         0x09u )
#define EFSS_BLOB_LENOFF                                                                  ( ( uint32_t )         0x08u )
#define EFSS_BLOB_CRCOFF                                                                  ( ( uint32_t )         0x04u )
#define EFSS_BLOB_SEED                                                                    ( ( uint32_t )  0x0FFFFFFFFu )



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_BLOB_IsStatusStillCoherent(t_eFSS_BLOB_Ctx* const p_ptCtx);
static e_eFSS_BLOB_RES eFSS_BLOB_BlobCtoBLOBRes(const e_eFSS_BLOBC_RES p_eCRes);



/***********************************************************************************************************************
 *  PRIVATE UTILS STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static e_eFSS_BLOB_RES eFSS_BLOB_OriginBackupAligner(t_eFSS_BLOB_Ctx* const p_ptCtx, const bool_t p_bForce);
static e_eFSS_BLOB_RES eFSS_BLOB_IsAreaValid(t_eFSS_BLOB_Ctx* const p_ptCtx, const bool_t p_bIsOri,
                                             bool_t* const p_pbIsVal);



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
	if( NULL == p_ptCtx )
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
                    /* We need more space for the BLOB */
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

                    /* Set to false fo the first operation, this will trigger a stored BLOB check */
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
                        /* Align if possible corrupted area, forcing the process */
                        l_eRes = eFSS_BLOB_OriginBackupAligner(p_ptCtx, true);
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
    uint32_t l_uMaxBlobSize;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBlobSize ) )
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
                        l_eRes = eFSS_BLOB_OriginBackupAligner(p_ptCtx, false);

                        if( ( e_eFSS_BLOB_RES_OK == l_eRes ) || ( e_eFSS_BLOB_RES_OK_BKP_RCVRD == l_eRes ) )
                        {
                            /* Retrive parameter from the last page, we are sure that some correct data will be
                               retrived because eFSS_BLOB_OriginBackupAligner is called before this operation */
                            l_uUsePages = 0u;
                            l_eResC = eFSS_BLOBC_GetBuffNUsable(&p_ptCtx->tBLOBCCtx, &l_tBuff, &l_uUsePages);
                            l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                            if( e_eFSS_BLOB_RES_OK == l_eRes )
                            {
                                /* Load the last page, where blob length is stored */
                                l_eResC = eFSS_BLOBC_LoadPageInBuff(&p_ptCtx->tBLOBCCtx, true,
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
                                        /* Is retrived size coherent? (do it just to be sure, even if not needed) */
                                        l_uMaxBlobSize = ( l_uUsePages * l_tBuff.uBufL ) - EFSS_BLOB_LENOFF;
                                        if( *p_puBlobSize > l_uMaxBlobSize )
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
    uint32_t l_uUsePages;
    uint32_t l_uCurrPage;
    uint32_t l_uBlobCrc;
    uint32_t l_uCrcOffset;

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
                            l_uBlobCrc = EFSS_BLOB_SEED;

                            /* Init local variable */
                            l_uCurrPage = 0u;

                            /* Write at zero every pages of the origin area, after that update backup one  */
                            while( ( l_uCurrPage < l_uUsePages ) && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                            {
                                /* Clear buffer */
                                (void)memset(l_tBuff.puBuf, 0, l_tBuff.uBufL);

                                if( ( l_uUsePages - 1u ) == l_uCurrPage )
                                {
                                    /* Last page, calculate the CRC but exlude the CRC itself, and after that insert
                                       it inside the last page */
                                    l_uCrcOffset = ( l_tBuff.uBufL - EFSS_BLOB_CRCOFF );
                                    l_eResC = eFSS_BLOBC_CalcCrcInBuff(&p_ptCtx->tBLOBCCtx, l_uBlobCrc,
                                                                             l_uCrcOffset, &l_uBlobCrc);
                                    l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                                    {
                                        /* Insert the CRC inside the buffer */
                                        if( true != eFSS_Utils_InsertU32(&l_tBuff.puBuf[l_uCrcOffset],
                                                                         l_uBlobCrc ) )
                                        {
                                            l_eRes = e_eFSS_BLOB_RES_CORRUPTCTX;
                                        }
                                    }
                                }
                                else
                                {
                                    /* This is not the last page, can calculate the CRC of the whole buffer */
                                    l_eResC = eFSS_BLOBC_CalcCrcInBuff(&p_ptCtx->tBLOBCCtx, l_uBlobCrc,
                                                                             l_tBuff.uBufL, &l_uBlobCrc);
                                    l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);
                                }

                                /* If all ok flush the page in storage */
                                if( e_eFSS_BLOB_RES_OK == l_eRes )
                                {
                                    l_eResC = eFSS_BLOBC_FlushBufferInPage(&p_ptCtx->tBLOBCCtx, true, l_uCurrPage, 0u );
                                    l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);
                                }

                                /* Erase next page */
                                l_uCurrPage++;
                            }

                            if( e_eFSS_BLOB_RES_OK == l_eRes )
                            {
                                /* Original pages are OK, need to clone backup */
                                l_uCurrPage = 0u;

                                while( ( l_uCurrPage < l_uUsePages ) && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                                {
                                    l_eResC = eFSS_BLOBC_CloneArea(&p_ptCtx->tBLOBCCtx, true);
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
    bool_t l_bIsInit;

    /* Local variable used for storage */
    t_eFSS_BLOBC_StorBuf l_tBuff;
    uint32_t l_uUsePages;

    /* Local variable for general calculation */
    uint32_t l_uBlobSize;
    uint32_t l_uSeqN;
    uint32_t l_uMaxBlobSize;

    /* Local variable used for page tracking */
    uint32_t l_uCurrPage;
    uint32_t l_uCurPageOff;
    uint32_t l_uRemToRead;
    uint32_t l_uReadedByte;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
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
                        /* Fix any memory problem */
                        l_eRes = eFSS_BLOB_OriginBackupAligner(p_ptCtx, false);

                        if( ( e_eFSS_BLOB_RES_OK == l_eRes ) || ( e_eFSS_BLOB_RES_OK_BKP_RCVRD == l_eRes ) )
                        {
                            /* Retrive buffer and the numbers of page so we can check requested param */
                            l_uUsePages = 0u;
                            l_eResC = eFSS_BLOBC_GetBuffNUsable(&p_ptCtx->tBLOBCCtx, &l_tBuff, &l_uUsePages);
                            l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                            if( e_eFSS_BLOB_RES_OK == l_eRes )
                            {
                                /* Get Blob info, load last page */
                                l_eResC = eFSS_BLOBC_LoadPageInBuff(&p_ptCtx->tBLOBCCtx, true,
                                                                        ( l_uUsePages - 1u ), &l_uSeqN);
                                l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);
                                if( e_eFSS_BLOB_RES_OK == l_eRes )
                                {
                                    /* Extract data from the last page */
                                    if( true != eFSS_Utils_RetriveU32(&l_tBuff.puBuf[l_tBuff.uBufL - EFSS_BLOB_LENOFF],
                                                                      &l_uBlobSize ) )
                                    {
                                        l_eRes = e_eFSS_BLOB_RES_CORRUPTCTX;
                                    }
                                    else
                                    {
                                        /* Is retrived size coherent? */
                                        l_uMaxBlobSize = ( l_uUsePages * l_tBuff.uBufL ) - EFSS_BLOB_LENOFF;
                                        if( l_uBlobSize > l_uMaxBlobSize )
                                        {
                                            l_eRes = e_eFSS_BLOB_RES_NOTVALIDBLOB;
                                        }
                                        else
                                        {
                                            /* Verify data validity */
                                            if( ( p_uBuffL <= 0u ) || ( p_uBuffL > l_uBlobSize ) ||
                                                ( p_uOffset >= l_uBlobSize ) ||
                                                ( ( p_uOffset + p_uBuffL ) > l_uBlobSize ) )
                                            {
                                                l_eRes = e_eFSS_BLOB_RES_BADPARAM;
                                            }
                                            else
                                            {
                                                /* Now we are sure page are aligned, and that passed data is valid */
                                                /* Init read counter */
                                                l_uRemToRead = p_uBuffL;

                                                /* Find starting page Find starting page */
                                                l_uCurrPage   = (uint32_t)(p_uOffset / l_tBuff.uBufL);
                                                l_uCurPageOff = (uint32_t)(p_uOffset % l_tBuff.uBufL);

                                                while( ( l_uRemToRead > 0u ) && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                                                {
                                                    /* Read the current buffer */
                                                    l_eResC = eFSS_BLOBC_LoadPageInBuff(&p_ptCtx->tBLOBCCtx, true,
                                                                                            l_uCurrPage, &l_uSeqN);
                                                    l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                                                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                                                    {
                                                        /* Init variable */
                                                        l_uReadedByte = p_uBuffL - l_uRemToRead;

                                                        /* Can copy data to the user buffer */
                                                        if( l_uRemToRead > ( l_tBuff.uBufL - l_uCurPageOff ) )
                                                        {
                                                            (void)memcpy(&p_puBuff[l_uReadedByte],
                                                                         &l_tBuff.puBuf[l_uCurPageOff],
                                                                         (l_tBuff.uBufL - l_uCurPageOff) );
                                                            l_uCurPageOff = 0u;
                                                            l_uRemToRead -= (l_tBuff.uBufL - l_uCurPageOff);
                                                        }
                                                        else
                                                        {
                                                            (void)memcpy(&p_puBuff[l_uReadedByte],
                                                                         &l_tBuff.puBuf[l_uCurPageOff],
                                                                         l_uRemToRead);
                                                            l_uCurPageOff = 0u;
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
                    if( true == p_ptCtx->bIsWriteOngoing )
                    {
                        l_eRes = e_eFSS_BLOB_RES_WRITEONGOING;
                    }
                    else
                    {
                        /* Fix any memory problem */
                        l_eRes = eFSS_BLOB_OriginBackupAligner(p_ptCtx, false);

                        if( ( e_eFSS_BLOB_RES_OK == l_eRes ) || ( e_eFSS_BLOB_RES_OK_BKP_RCVRD == l_eRes ) )
                        {
                            /* Load a page just to read sequential number and increase it */
                            l_uSeqN = 0u;
                            l_eResC = eFSS_BLOBC_LoadPageInBuff(&p_ptCtx->tBLOBCCtx, true,
                                                                    0u, &l_uSeqN);
                            l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                            if( e_eFSS_BLOB_RES_OK == l_eRes )
                            {
                                /* Now we can write data */
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
	}

	return l_eRes;
}

e_eFSS_BLOB_RES eFSS_BLOB_AppendData(t_eFSS_BLOB_Ctx* const p_ptCtx, uint8_t* const p_puBuff, const uint32_t p_uBuffL)
{
	/* Local return variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOBC_RES l_eResC;
    bool_t l_bIsInit;

    /* Local variable for storage */
    t_eFSS_BLOBC_StorBuf l_tBuff;
    uint32_t l_uUsePages;

    /* Local variable for calculation utils */
    uint32_t l_uMaxBlobSize;

    /* Local variable for calculation */
    uint32_t l_uCurrPage;
    uint32_t l_uCurPageOff;
    uint32_t l_uRemToWrite;
    uint32_t l_uWrittenByte;
    uint32_t l_uSeqN;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
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
                    if( false == p_ptCtx->bIsWriteOngoing )
                    {
                        l_eRes = e_eFSS_BLOB_RES_WRITENOSTARTED;
                    }
                    else
                    {
                        /* Get basic data */
                        l_uUsePages = 0u;
                        l_eResC = eFSS_BLOBC_GetBuffNUsable(&p_ptCtx->tBLOBCCtx, &l_tBuff, &l_uUsePages);
                        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                        if( e_eFSS_BLOB_RES_OK == l_eRes )
                        {
                            /* Calculat max blob len */
                            l_uMaxBlobSize = ( ( l_uUsePages * l_tBuff.uBufL ) - EFSS_BLOB_LENOFF );

                            /* Verify parameter validity */
                            if( ( p_uBuffL > l_uMaxBlobSize ) || ( p_uBuffL <= 0u ) ||
                                ( ( p_ptCtx->uDataWritten + p_uBuffL ) > l_uMaxBlobSize ) )
                            {
                                l_eRes = e_eFSS_BLOB_RES_BADPARAM;
                            }
                            else
                            {
                                /* Init remaining byte to write variable */
                                l_uRemToWrite = p_uBuffL;

                                /* Find starting page */
                                l_uCurrPage = (uint32_t)(p_ptCtx->uDataWritten / l_tBuff.uBufL);
                                l_uCurPageOff = (uint32_t)(p_ptCtx->uDataWritten % l_tBuff.uBufL);

                                while( ( l_uRemToWrite > 0u ) && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                                {
                                    /* First load the page that we want to write, because we could have some
                                       already appended data */
                                    l_eResC = eFSS_BLOBC_LoadPageInBuff(&p_ptCtx->tBLOBCCtx, true, l_uCurrPage,
                                                                            &l_uSeqN);
                                    l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                                    {
                                        /* Init variable */
                                        l_uWrittenByte = p_uBuffL - l_uRemToWrite;

                                        /* Are we able to fill a full page? If yes, we can calculate the CRC of the
                                           whole page also, if not, we insert data and fill the remaining space with
                                           zero. We will calculate the CRC of the inserted data on the next round */
                                        if( l_uRemToWrite >= ( l_tBuff.uBufL - l_uCurPageOff ) )
                                        {
                                            /* full fill the page with data */
                                            (void)memcpy(&l_tBuff.puBuf[l_uCurPageOff], &p_puBuff[l_uWrittenByte],
                                                         ( l_tBuff.uBufL - l_uCurPageOff ) );

                                            l_uCurPageOff = 0u;
                                            l_uRemToWrite -= ( l_tBuff.uBufL - l_uCurPageOff );

                                            /* if it's not the last page we can calculate the CRC of the buffer.
                                               The CRC of the last page is calculated at the end, when we are able to
                                               append the BLOB size also */
                                            if( ( l_uUsePages - 1u ) != l_uCurrPage )
                                            {
                                                l_eResC =  eFSS_BLOBC_CalcCrcInBuff(&p_ptCtx->tBLOBCCtx,
                                                                                          p_ptCtx->uCrcOfDataWritten,
                                                                                          l_tBuff.uBufL,
                                                                                          &p_ptCtx->uCrcOfDataWritten);
                                                l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);
                                            }
                                        }
                                        else
                                        {
                                            /* Set to zero the unused data, and do no calculate the CRC. */
                                            (void)memset(&l_tBuff.puBuf[l_uCurPageOff], 0,
                                                         ( l_tBuff.uBufL - l_uCurPageOff ) );

                                            /* Copy remaining data */
                                            (void)memcpy(&l_tBuff.puBuf[l_uCurPageOff], &p_puBuff[l_uWrittenByte],
                                                         l_uRemToWrite);
                                            l_uCurPageOff = 0u;
                                            l_uRemToWrite = 0u;
                                        }

                                        /* If previous operation are OK we can flush the updated page */
                                        if( e_eFSS_BLOB_RES_OK == l_eRes )
                                        {
                                            /* Buffer flush */
                                            l_eResC = eFSS_BLOBC_FlushBufferInPage(&p_ptCtx->tBLOBCCtx, true,
                                                                                   l_uCurrPage, p_ptCtx->uCurrentSeqN);
                                            l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);
                                        }

                                        /* Increase the page so we can write on the next one */
                                        l_uCurrPage++;
                                    }
                                }

                                /* If all ok increase context counter */
                                if( e_eFSS_BLOB_RES_OK == l_eRes )
                                {
                                    p_ptCtx->uDataWritten += p_uBuffL;
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
    t_eFSS_BLOBC_StorBuf l_tBuff;
    uint32_t l_uUsePages;
    uint32_t l_uCurrPage;
    uint32_t l_uCurPageOff;
    uint32_t l_uRemToWrite;
    uint32_t l_uSeqN;
    uint32_t l_uMaxBlobSize;
    uint32_t l_uLenCrcOff;

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
                    /* Cannot end a write if it's not even started */
                    if( false == p_ptCtx->bIsWriteOngoing )
                    {
                        l_eRes = e_eFSS_BLOB_RES_WRITENOSTARTED;
                    }
                    else
                    {
                        /* Get basic data */
                        l_uUsePages = 0u;
                        l_eResC = eFSS_BLOBC_GetBuffNUsable(&p_ptCtx->tBLOBCCtx, &l_tBuff, &l_uUsePages);
                        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                        if( e_eFSS_BLOB_RES_OK == l_eRes )
                        {
                            /* Calculat max blob len */
                            l_uMaxBlobSize = ( ( l_uUsePages * l_tBuff.uBufL ) - EFSS_BLOB_LENOFF );

                            /* Ok we need to complete the write process writing to zero any remaining pages and
                               updating CRC and BLOB size field */
                            l_uRemToWrite = ( l_uMaxBlobSize + EFSS_BLOB_LENOFF ) - p_ptCtx->uDataWritten;

                            /* Find offset where to start zeeting to zero data */
                            l_uCurrPage   = (uint32_t)(p_ptCtx->uDataWritten / l_tBuff.uBufL);
                            l_uCurPageOff = (uint32_t)(p_ptCtx->uDataWritten % l_tBuff.uBufL);

                            while( ( l_uRemToWrite > 0u ) && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                            {
                                /* First load the page that we want to write, because we could have some
                                   already appended data */
                                l_eResC = eFSS_BLOBC_LoadPageInBuff(&p_ptCtx->tBLOBCCtx, true, l_uCurrPage,
                                                                        &l_uSeqN);
                                l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                                if( e_eFSS_BLOB_RES_OK == l_eRes )
                                {
                                    /* Now, if we need to set to zero more data that present in the buffer we are
                                       sure we didnt reached the last page. */
                                    if( l_uRemToWrite > ( l_tBuff.uBufL - l_uCurPageOff ) )
                                    {
                                        /* full fill the page with data setted to zero */
                                        (void)memset(&l_tBuff.puBuf[l_uCurPageOff], 0,
                                                     ( l_tBuff.uBufL - l_uCurPageOff ) );
                                        l_uCurPageOff = 0u;
                                        l_uRemToWrite -= ( l_tBuff.uBufL - l_uCurPageOff );

                                        /* Not the last page, update the CRC also */
                                        l_eResC =  eFSS_BLOBC_CalcCrcInBuff(&p_ptCtx->tBLOBCCtx,
                                                                                  p_ptCtx->uCrcOfDataWritten,
                                                                                  l_tBuff.uBufL,
                                                                                  &p_ptCtx->uCrcOfDataWritten);
                                        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                                    }
                                    else
                                    {
                                        /* Ok so this must be the last page. Set to zero the unused data */
                                        (void)memset(&l_tBuff.puBuf[l_uCurPageOff], 0,
                                                     ( l_tBuff.uBufL - l_uCurPageOff ) );

                                        /* Update counter */
                                        l_uCurPageOff = 0u;
                                        l_uRemToWrite = 0u;

                                        /* We must update blob length and it's CRC  */
                                        l_uLenCrcOff = l_tBuff.uBufL - EFSS_BLOB_LENOFF;
                                        if( true != eFSS_Utils_InsertU32(&l_tBuff.puBuf[l_uLenCrcOff],
                                                                         p_ptCtx->uDataWritten ) )
                                        {
                                            l_eRes = e_eFSS_BLOB_RES_CORRUPTCTX;
                                        }
                                        else
                                        {
                                            /* Can now calculate the CRC of the last page, excluding the CRC itself */
                                            l_eResC =  eFSS_BLOBC_CalcCrcInBuff(&p_ptCtx->tBLOBCCtx,
                                                                                      p_ptCtx->uCrcOfDataWritten,
                                                                                      l_tBuff.uBufL - EFSS_BLOB_CRCOFF,
                                                                                      &p_ptCtx->uCrcOfDataWritten);
                                            l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                                            /* If all ok insert the CRC */
                                            if( e_eFSS_BLOB_RES_OK == l_eRes )
                                            {
                                                l_uLenCrcOff = l_tBuff.uBufL - EFSS_BLOB_CRCOFF;
                                                if( true != eFSS_Utils_InsertU32(&l_tBuff.puBuf[l_uLenCrcOff],
                                                                                 p_ptCtx->uCrcOfDataWritten ) )
                                                {
                                                    l_eRes = e_eFSS_BLOB_RES_CORRUPTCTX;
                                                }
                                            }
                                        }
                                    }

                                    /* if all ok can flush the page in the storage area */
                                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                                    {
                                        /* Buffer flush */
                                        l_eResC = eFSS_BLOBC_FlushBufferInPage(&p_ptCtx->tBLOBCCtx, true, l_uCurrPage,
                                                                               p_ptCtx->uCurrentSeqN);
                                        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);
                                    }

                                    /* Continue with the next one */
                                    l_uCurrPage++;
                                }
                            }

                            /* If all ok we are done, ripristinate the non writing situation */
                            if( e_eFSS_BLOB_RES_OK == l_eRes )
                            {
                                p_ptCtx->bIsWriteOngoing = false;
                                p_ptCtx->uDataWritten = 0u;
                                p_ptCtx->uCrcOfDataWritten = 0u;
                                p_ptCtx->uCurrentSeqN = 0u;
                            }
                        }
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
                    /* Cannot abort write if not started */
                    if( false == p_ptCtx->bIsWriteOngoing )
                    {
                        l_eRes = e_eFSS_BLOB_RES_WRITENOSTARTED;
                    }
                    else
                    {
                        /* In order to abort a write we just need to ripristinate backup pages */
                        l_eResC = eFSS_BLOBC_CloneArea(&p_ptCtx->tBLOBCCtx, false);
                        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                        if( e_eFSS_BLOB_RES_OK == l_eRes )
                        {
                            /* area cloned! Ripristinate the context */
                            p_ptCtx->bIsWriteOngoing = false;
                            p_ptCtx->uDataWritten = 0u;
                            p_ptCtx->uCrcOfDataWritten = 0u;
                            p_ptCtx->uCurrentSeqN = 0u;
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
static bool_t eFSS_BLOB_IsStatusStillCoherent(t_eFSS_BLOB_Ctx* const p_ptCtx)
{
    /* Local variable for return */
    bool_t l_bRes;
    e_eFSS_BLOBC_RES l_eResC;

    /* Local storage variable */
    t_eFSS_BLOBC_StorBuf l_tBuff;
    uint32_t l_uUsePages;

    /* Local variable for calculation utils */
    uint32_t l_uMaxBlobSize;

    /* Get usable pages and buffer length so we can check blob default value validity */
    l_uUsePages = 0u;
    l_eResC = eFSS_BLOBC_GetBuffNUsable(&p_ptCtx->tBLOBCCtx, &l_tBuff, &l_uUsePages);

    if( e_eFSS_BLOBC_RES_OK != l_eResC )
    {
        l_bRes = false;
    }
    else
    {
        /* Check if page length is OK */
        if( ( l_tBuff.uBufL < EFSS_BLOB_MINPAGESIZE ) || ( 0u == l_uUsePages ) )
        {
            /* We need more space for the BLOB */
            l_bRes = false;
        }
        else
        {
            /* If we are not doing some write in flash, we must have parameter setted to zero */
            if( true == p_ptCtx->bIsWriteOngoing )
            {
                /* Calculat max blob len */
                l_uMaxBlobSize = ( ( l_uUsePages * l_tBuff.uBufL ) - EFSS_BLOB_LENOFF );

                if( p_ptCtx->uDataWritten > l_uMaxBlobSize )
                {
                    /* wrong */
                    l_bRes = false;
                }
                else
                {
                    /* All OK */
                    l_bRes = true;
                }
            }
            else
            {
                if( ( 0u != p_ptCtx->uDataWritten ) || ( 0u != p_ptCtx->uCrcOfDataWritten ) ||
                    ( 0u != p_ptCtx->uCurrentSeqN ) )
                {
                    /* wrong */
                    l_bRes = false;
                }
                else
                {
                    /* All ok */
                    l_bRes = true;
                }
            }
        }
    }

    return l_bRes;
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

        case e_eFSS_BLOBC_RES_NOTVALIDBLOB:
        {
            l_eRes = e_eFSS_BLOB_RES_NOTVALIDBLOB;
            break;
        }

        case e_eFSS_BLOBC_RES_NEWVERSIONFOUND:
        {
            l_eRes = e_eFSS_BLOB_RES_NEWVERSIONFOUND;
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

static e_eFSS_BLOB_RES eFSS_BLOB_OriginBackupAligner(t_eFSS_BLOB_Ctx* const p_ptCtx, const bool_t p_bForce)
{
	/* Local return variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOBC_RES l_eResC;

    /* Local variable for calculation */
    t_eFSS_BLOBC_StorBuf l_tBuff;
    uint32_t l_uUsableP;
    bool_t l_bIsValidOrig;
    bool_t l_bIsValidBkup;

    /* Init return value */
    l_eRes = e_eFSS_BLOB_RES_OK;

    /* Check if alignement is needed */
    if( ( true == p_bForce ) || ( false == p_ptCtx->bIsBlobCheked ) )
    {
        /* Get needed data structures */
        l_eResC = eFSS_BLOBC_GetBuffNUsable(&p_ptCtx->tBLOBCCtx, &l_tBuff, &l_uUsableP);
        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

        if( e_eFSS_BLOB_RES_OK == l_eRes )
        {
            /* Check is needed, how can we do it?
            * 1 - Verify if original partition contain a valid blob.
            * 2 - If original partition is valid, clone, if needed, original partition in backup one using
            *     eFSS_BLOBC_CloneOriAreaInBkpIfNotEq
            * 3 - If original is not valid, verify if backup partition has a valid blob
            * 4 - If backup partition is valid, clone backup partition in original one using eFSS_BLOBC_CloneArea
            * 5 - If no partition is valid, return blob invalid */

            /* Verify Original */
            l_bIsValidOrig = false;
            l_eRes = eFSS_BLOB_IsAreaValid(p_ptCtx, true, &l_bIsValidOrig);

            if( e_eFSS_BLOB_RES_OK == l_eRes )
            {
                if( true == l_bIsValidOrig )
                {
                    /* Original area valid, verify that backup pages is aligned */
                    l_eResC = eFSS_BLOBC_CloneOriAreaInBkpIfNotEq(&p_ptCtx->tBLOBCCtx);
                    l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);
                }
                else
                {
                    /* Original area corrupted, need to verify the backup one */
                    l_bIsValidBkup = false;
                    l_eRes = eFSS_BLOB_IsAreaValid(p_ptCtx, false, &l_bIsValidBkup);

                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                    {
                        if( true == l_bIsValidBkup )
                        {
                            /* Backup area valid, Clone it in to the original one */
                            l_eResC = eFSS_BLOBC_CloneArea(&p_ptCtx->tBLOBCCtx, false);
                            l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);
                        }
                        else
                        {
                            /* Neiother original or backup area is valid, not valid blob stored */
                            l_eRes = e_eFSS_BLOB_RES_NOTVALIDBLOB;
                        }
                    }
                }
            }
        }
    }

    /* Set checked flag */
    if( ( e_eFSS_BLOB_RES_OK == l_eRes ) || ( e_eFSS_BLOB_RES_OK_BKP_RCVRD == l_eRes ) )
    {
        /* Blob checked and no error found */
        p_ptCtx->bIsBlobCheked = true;
    }
    else
    {
        /* Be sure to redo the check */
        p_ptCtx->bIsBlobCheked = false;
    }

	return l_eRes;
}

static e_eFSS_BLOB_RES eFSS_BLOB_IsAreaValid(t_eFSS_BLOB_Ctx* const p_ptCtx, const bool_t p_bIsOri,
                                             bool_t* const p_pbIsVal)
{
	/* Local return variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOBC_RES l_eResC;

    /* Local variable for storage */
    t_eFSS_BLOBC_StorBuf l_tBuff;
    uint32_t l_uUsableP;

    /* Local variable readed from Blob */
    uint32_t l_uReadedCRC;
    uint32_t l_uReadedLen;
    uint32_t l_uReadedSeqN;

    /* Local variable for calculation */
    uint32_t l_uTempSeqN;
    uint32_t l_uCurrPage;
    uint32_t l_uBlobCrc;
    uint32_t l_uRemByteToNoCheck;
    uint32_t l_uZeroToCheck;
    uint32_t l_uMaxBlobSize;

    /* Check data validity */
    if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsVal ) )
    {
        l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
    }
    else
    {
        /* Get needed basic data structures */
        l_uUsableP = 0u;
        l_eResC = eFSS_BLOBC_GetBuffNUsable(&p_ptCtx->tBLOBCCtx, &l_tBuff, &l_uUsableP);
        l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

        if( e_eFSS_BLOB_RES_OK == l_eRes )
        {
            /* Check this area, how can we do it?
            * 1 - Verify CRC validity
            * 2 - Verify that data outside blob len is setted to zero
            * 3 - Verify that every page has the same sequential number of the other one
            * 4 - Verify that data length is coherent
            */

            /* Start reading only tyhe last page in order to retrive basic data */
            l_eResC = eFSS_BLOBC_LoadPageInBuff(&p_ptCtx->tBLOBCCtx, p_bIsOri, ( l_uUsableP - 1u ), &l_uReadedSeqN);
            l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

            if( e_eFSS_BLOB_RES_OK == l_eRes )
            {
                /* estrapolate BLOB size and BLOB len */
                if( true != eFSS_Utils_RetriveU32(&l_tBuff.puBuf[l_tBuff.uBufL - EFSS_BLOB_LENOFF],
                                                  &l_uReadedLen ) )
                {
                    l_eRes = e_eFSS_BLOB_RES_CORRUPTCTX;
                }
                else
                {
                    if( true != eFSS_Utils_RetriveU32(&l_tBuff.puBuf[l_tBuff.uBufL - EFSS_BLOB_CRCOFF],
                                                      &l_uReadedCRC ) )
                    {
                        l_eRes = e_eFSS_BLOB_RES_CORRUPTCTX;
                    }
                    else
                    {
                        /* Calculat max blob len */
                        l_uMaxBlobSize = ( ( l_uUsableP * l_tBuff.uBufL ) - EFSS_BLOB_LENOFF );

                        /* Verify just readed data validity */
                        if( l_uReadedLen > l_uMaxBlobSize )
                        {
                            l_eRes = e_eFSS_BLOB_RES_NOTVALIDBLOB;
                        }
                    }
                }
            }

            /* If we retrived corretly the basic data continue by checking everything */
            if( e_eFSS_BLOB_RES_OK == l_eRes )
            {
                /* Read all original pages, verify sequential number and blob CRC */
                l_uCurrPage = 0u;
                l_uTempSeqN = 0u;
                l_uBlobCrc = EFSS_BLOB_SEED;
                l_uRemByteToNoCheck = l_uReadedLen;

                while( ( l_uCurrPage < l_uUsableP ) && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                {
                    l_eResC = eFSS_BLOBC_LoadPageInBuff(&p_ptCtx->tBLOBCCtx, true, l_uCurrPage, &l_uTempSeqN);
                    l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                    {
                        /* Check sequential numbr */
                        if( l_uTempSeqN != l_uReadedSeqN )
                        {
                            /* Need same seq number */
                            l_eRes = e_eFSS_BLOB_RES_NOTVALIDBLOB;
                        }
                        else
                        {
                            /* ------------ Check that unsued data is zero */
                            if( l_uRemByteToNoCheck < l_tBuff.uBufL )
                            {
                                /* How many zero to check? */
                                l_uZeroToCheck = l_tBuff.uBufL - l_uRemByteToNoCheck;

                                /* Check */
                                while( ( l_uZeroToCheck > 0u ) && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                                {
                                    if( 0u != l_tBuff.puBuf[ l_tBuff.uBufL - l_uZeroToCheck ] )
                                    {
                                        /* Not valid blob */
                                        l_eRes = e_eFSS_BLOB_RES_NOTVALIDBLOB;
                                    }

                                    l_uZeroToCheck--;
                                }

                                /* No remaining byte to check */
                                l_uRemByteToNoCheck = 0u;
                            }
                            else
                            {
                                l_uRemByteToNoCheck -= l_tBuff.uBufL;
                            }

                            /* ------------ Calculate the CRC */
                            if( e_eFSS_BLOB_RES_OK == l_eRes )
                            {
                                if( ( l_uUsableP - 1u ) != l_uCurrPage )
                                {
                                    /* Not the last page, check CRC of the whole */
                                    l_eResC = eFSS_BLOBC_CalcCrcInBuff(&p_ptCtx->tBLOBCCtx, l_uBlobCrc,
                                                                             l_tBuff.uBufL, &l_uBlobCrc);
                                    l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);
                                }
                                else
                                {
                                    /* This is the last page, exclude from calculation the stored CRC itself */
                                    l_eResC = eFSS_BLOBC_CalcCrcInBuff(&p_ptCtx->tBLOBCCtx, l_uBlobCrc,
                                                                             ( l_tBuff.uBufL - EFSS_BLOB_CRCOFF ),
                                                                             &l_uBlobCrc);
                                    l_eRes = eFSS_BLOB_BlobCtoBLOBRes(l_eResC);

                                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                                    {
                                        /* Last page, can check if CRC is correct also */
                                        if( l_uReadedCRC != l_uBlobCrc)
                                        {
                                            /* This area is not correct */
                                            l_eRes = e_eFSS_BLOB_RES_NOTVALIDBLOB;
                                        }
                                    }
                                }

                                /* Check next page */
                                l_uCurrPage++;
                            }
                        }
                    }
                }
            }
        }
    }

	return l_eRes;
}