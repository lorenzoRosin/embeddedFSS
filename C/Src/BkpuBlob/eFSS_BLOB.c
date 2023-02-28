/**
 * @file       eFSS_BLOB.c
 *
 * @brief      Blob large file module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/* In this module the page field has the following meaning:
 * - uPageUseSpecific1 -> Size of the stored blob
 * - uPageUseSpecific2 -> Crc of the stored blob
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
#include "eFSS_BLOB.h"
#include "eFSS_UTILSHLPRV.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_BLOB_IsStatusStillCoherent(const t_eFSS_BLOB_Ctx* p_ptCtx);
static e_eFSS_BLOB_RES eFSS_BLOB_HLtoBLOBRes(const e_eFSS_UTILSHLPRV_RES p_eHLRes);
static e_eFSS_BLOB_RES eFSS_BLOB_CrcBlobFull(t_eFSS_BLOB_Ctx* const p_ptCtx, uint8_t* p_puBlob, uint32_t p_uBlobL,
                                             uint32_t* p_puBlobCrc);
static e_eFSS_BLOB_RES eFSS_BLOB_OriginBackupAligner(t_eFSS_BLOB_Ctx* const p_ptCtx);
static e_eFSS_BLOB_RES eFSS_BLOB_ArePagesValid(t_eFSS_BLOB_Ctx* const p_ptCtx, uint32_t p_uStartIdx, uint32_t p_uNPages,
                                               bool_t* p_pbAreValid, t_eFSS_TYPE_PageMeta* p_ptMeta, uint8_t p_uSubT);
static e_eFSS_BLOB_RES eFSS_BLOB_WriteBlobNPrm(t_eFSS_BLOB_Ctx* const p_ptCtx, uint8_t* p_puBuff, uint32_t p_uBuffL,
                                               uint32_t p_uStartId, uint32_t p_uNPages, t_eFSS_TYPE_PageMeta* p_ptMeta);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_BLOB_RES eFSS_BLOB_InitCtx(t_eFSS_BLOB_Ctx* const p_ptCtx, const t_eFSS_TYPE_CbStorCtx p_tCtxCb,
                                  uint8_t* const p_puBuff, uint32_t p_uBuffL, t_eFSS_TYPE_StorageSettings p_tStorSet)
{
    e_eFSS_BLOB_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_tCtxCb.ptCtxErase ) || ( NULL == p_tCtxCb.fErase ) ||
            ( NULL == p_tCtxCb.ptCtxWrite ) || ( NULL == p_tCtxCb.fWrite ) ||
            ( NULL == p_tCtxCb.ptCtxRead  ) || ( NULL == p_tCtxCb.fRead  ) ||
            ( NULL == p_tCtxCb.ptCtxCrc32 ) || ( NULL == p_tCtxCb.fCrc32 ) )
        {
            l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( p_uBuffL != ( 2u * p_tStorSet.uPagesLen ) )
            {
                l_eRes = e_eFSS_BLOB_RES_BADPARAM;
            }
            else
            {
                /* Check data validity */
                if( ( p_tStorSet.uTotPages < 2u ) || ( 0u != ( p_tStorSet.uTotPages % 2u ) ) )
                {
                    l_eRes = e_eFSS_BLOB_RES_BADPARAM;
                }
                else
                {
                    /* Check data validity */
                    if( ( p_tStorSet.uPagesLen <= EFSS_PAGEMETASIZE ) || ( 0u != ( p_tStorSet.uPagesLen % 2u ) ) )
                    {
                        l_eRes = e_eFSS_BLOB_RES_BADPARAM;
                    }
                    else
                    {
                        /* Check data validity */
                        if( p_tStorSet.uRWERetry <= 0u )
                        {
                            l_eRes = e_eFSS_BLOB_RES_BADPARAM;
                        }
                        else
                        {
                            /* Fill context */
                            p_ptCtx->bIsInit = true;
                            p_ptCtx->tCtxCb = p_tCtxCb;
                            p_ptCtx->tStorSett = p_tStorSet;
                            p_ptCtx->puBuf = p_puBuff;
                            p_ptCtx->uBufL = p_uBuffL;
                        }
                    }
                }
            }
        }
    }

    return l_eRes;
}

e_eFSS_BLOB_RES eFSS_BLOB_IsInit(t_eFSS_BLOB_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eFSS_BLOB_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInit;
        l_eRes = e_eFSS_BLOB_RES_OK;
	}

	return l_eRes;
}

e_eFSS_BLOB_RES eFSS_BLOB_GetStorageStatus(t_eFSS_BLOB_Ctx* const p_ptCtx)
{
	/* Local variable */
	e_eFSS_BLOB_RES l_eRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
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
                l_eRes = eFSS_BLOB_OriginBackupAligner(p_ptCtx);
            }
		}
	}

	return l_eRes;
}

e_eFSS_BLOB_RES eFSS_BLOB_GetInfo(t_eFSS_BLOB_Ctx* const p_ptCtx, uint32_t* p_puBlobSize)
{
	/* Local variable for result */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;

    /* Local calc variable */
    t_eFSS_TYPE_PageMeta l_tPagePrm;
    uint8_t* l_puBuF1;
    uint32_t l_uBuF1L;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBlobSize ) )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
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
                    /* Get buffer for calculation */
                    l_puBuF1 = p_ptCtx->puBuf;
                    l_uBuF1L = p_ptCtx->uBufL / 2u ;

                    /* Retrive info from the first page */
                    l_eHLRes = eFSS_UTILSHLPRV_ReadPageNPrm(&p_ptCtx->tCtxCb, 0u,  l_puBuF1, l_uBuF1L, &l_tPagePrm,
                                                            p_ptCtx->tStorSett.uRWERetry);

                    if( e_eFSS_UTILSHLPRV_RES_OK != l_eHLRes )
                    {
                        l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
                    }
                    else
                    {
                        *p_puBlobSize = l_tPagePrm.uPageUseSpecific1;
                    }
                }
            }
		}
	}

	return l_eRes;
}

e_eFSS_BLOB_RES eFSS_BLOB_Format(t_eFSS_BLOB_Ctx* const p_ptCtx)
{
	/* Local variable for result */
	e_eFSS_BLOB_RES l_eRes;

    /* Local calc variable */
    uint32_t l_uCrcZeroBlob;
    uint8_t l_uZero;
    t_eFSS_TYPE_PageMeta l_tPagePrm;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
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
                l_uZero = 0u;

                /* Calculate the CRC of the zero data blob */
                l_eRes = eFSS_BLOB_CrcBlobFull(p_ptCtx, &l_uZero, 0u, &l_uCrcZeroBlob);

                if( e_eFSS_BLOB_RES_OK == l_eRes )
                {
                    /* First format the original pages, update blob size and CRC value, and after do the same things
                     * to the backup pages */
                    l_tPagePrm.uPageType = EFSS_PAGETYPE_BLOB;
                    l_tPagePrm.uPageSubType = EFSS_PAGESUBTYPE_BLOBORI;
                    l_tPagePrm.uPageVersion = p_ptCtx->tStorSett.uStorageVer;
                    l_tPagePrm.uPageUseSpecific1 = 0;                               /* Blob size */
                    l_tPagePrm.uPageUseSpecific2 = l_uCrcZeroBlob;                  /* Blob CRC */
                    l_tPagePrm.uPageUseSpecific3 = p_ptCtx->tStorSett.uTotPages;    /* Total pages */
                    l_tPagePrm.uPageUseSpecific4 = 0u;                              /* Sequential number */
                    l_tPagePrm.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
                    l_tPagePrm.uPageCrc = 0u;

                    /* Write first the original pages */
                    l_eRes = eFSS_BLOB_WriteBlobNPrm(p_ptCtx, &l_uZero, 1u, 0u,
                                                    ( p_ptCtx->tStorSett.uTotPages / 2u ), &l_tPagePrm);

                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                    {
                        /* Now write backup pages */
                        l_tPagePrm.uPageSubType = EFSS_PAGESUBTYPE_BLOBBKP;
                        l_eRes = eFSS_BLOB_WriteBlobNPrm(p_ptCtx, &l_uZero, 1u,
                                                        ( p_ptCtx->tStorSett.uTotPages / 2u ),
                                                        ( p_ptCtx->tStorSett.uTotPages / 2u ), &l_tPagePrm);
                    }
                }
            }
		}
	}

	return l_eRes;
}

e_eFSS_BLOB_RES eFSS_BLOB_ReadAllBlob(t_eFSS_BLOB_Ctx* const p_ptCtx, uint8_t* p_puBuff, uint32_t p_uBuffL,
                                      uint32_t* p_puReaded)
{
	/* Local variable for result */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;

    /* Local calc variable */
    uint8_t* l_puBuF1;
    uint32_t l_uBuF1L;
    t_eFSS_TYPE_PageMeta l_tPagePrm;
    uint32_t l_uI;
    uint32_t l_uRemToRead;
    uint32_t l_uRemToReadCtr;
    uint32_t l_uBlobL;
    uint32_t l_uRawDataP;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) || ( NULL == p_puReaded ) )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
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
                /* Check data validity */
                if( p_uBuffL <= 0u )
                {
                    l_eRes = e_eFSS_BLOB_RES_BADPARAM;
                }
                else
                {
                    /* Fix any memory problem */
                    l_eRes = eFSS_BLOB_OriginBackupAligner(p_ptCtx);

                    if( ( e_eFSS_BLOB_RES_OK == l_eRes ) || ( e_eFSS_BLOB_RES_OK_BKP_RCVRD == l_eRes ) )
                    {
                        /* Get buffer for calculation */
                        l_puBuF1 = p_ptCtx->puBuf;
                        l_uBuF1L = p_ptCtx->uBufL / 2u ;

                        /* Read first pages of the blob to check it's size */
                        l_eHLRes = eFSS_UTILSHLPRV_ReadPageNPrm(&p_ptCtx->tCtxCb, 0u, l_puBuF1, l_uBuF1L, &l_tPagePrm,
                                                                p_ptCtx->tStorSett.uRWERetry);

                        if( e_eFSS_UTILSHLPRV_RES_OK != l_eHLRes )
                        {
                            l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
                        }
                        else
                        {
                            /* Calculate how many byte we have to read */
                            l_uBlobL = l_tPagePrm.uPageUseSpecific1;

                            if( l_uBlobL >= p_uBuffL )
                            {
                                l_uRemToRead = p_uBuffL;
                                l_uRemToReadCtr = p_uBuffL;
                            }
                            else
                            {
                                l_uRemToRead = l_uBlobL;
                                l_uRemToReadCtr = l_uBlobL;
                            }

                            /* Calculate the raw space for data in every page */
                            l_uRawDataP = p_ptCtx->tStorSett.uPagesLen - EFSS_PAGEMETASIZE;

                            /* Pages are ok, read till we have data or till the buffer is full */
                            l_uI = 0u;

                            /* Read all pages */
                            while( ( l_uI < ( p_ptCtx->tStorSett.uTotPages / 2u ) )  &&
                                   ( ( e_eFSS_BLOB_RES_OK == l_eRes ) || ( e_eFSS_BLOB_RES_OK_BKP_RCVRD == l_eRes ) ) &&
                                   ( l_uRemToReadCtr > 0 ) )
                            {
                                l_eHLRes = eFSS_UTILSHLPRV_ReadPageNPrm(&p_ptCtx->tCtxCb, l_uI, l_puBuF1, l_uBuF1L,
                                                                        &l_tPagePrm, p_ptCtx->tStorSett.uRWERetry);

                                if( e_eFSS_UTILSHLPRV_RES_OK != l_eHLRes )
                                {
                                    l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
                                }
                                else
                                {
                                    if( l_uRemToReadCtr >= l_uRawDataP )
                                    {
                                        memcpy(&p_puBuff[l_uRemToRead-l_uRemToReadCtr], l_puBuF1, l_uRawDataP );
                                        l_uRemToReadCtr -= l_uRawDataP;
                                    }
                                    else
                                    {
                                        memcpy(&p_puBuff[l_uRemToRead-l_uRemToReadCtr], l_puBuF1, l_uRemToReadCtr );
                                        l_uRemToReadCtr = 0u;
                                    }
                                }
                            }

                            if( ( e_eFSS_BLOB_RES_OK == l_eRes ) || ( e_eFSS_BLOB_RES_OK_BKP_RCVRD == l_eRes ) )
                            {
                                /* Read complete */
                                *p_puReaded = l_uRemToRead - l_uRemToReadCtr;
                            }
                            else
                            {
                                *p_puReaded = 0u;
                            }
                        }
                    }
                }
            }
		}
	}

	return l_eRes;
}

e_eFSS_BLOB_RES eFSS_BLOB_WriteAllBlob(t_eFSS_BLOB_Ctx* const p_ptCtx, uint8_t* p_puBuff, uint32_t p_uBuffL)
{
	/* Local variable for result */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_BLOB_RES l_eResSub;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;

    /* Local calc variable */
    uint8_t* l_puBuF1;
    uint32_t l_uBuF1L;
    t_eFSS_TYPE_PageMeta l_tPagePrm;
    uint32_t l_uCrcBlob;
    uint32_t l_uNweSeq;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
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
                    /* Get buffer for calculation */
                    l_puBuF1 = p_ptCtx->puBuf;
                    l_uBuF1L = p_ptCtx->uBufL / 2u ;

                    /* Calculate the CRC of the data blob */
                    l_eRes = eFSS_BLOB_CrcBlobFull(p_ptCtx, p_puBuff, p_uBuffL, &l_uCrcBlob);

                    /* Read a page in order to get the seqeuntial number */
                    l_eHLRes = eFSS_UTILSHLPRV_ReadPageNPrm(&p_ptCtx->tCtxCb, 0u,  l_puBuF1, l_uBuF1L, &l_tPagePrm,
                                                                p_ptCtx->tStorSett.uRWERetry);

                    if( e_eFSS_UTILSHLPRV_RES_OK != l_eHLRes )
                    {
                        l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
                    }
                    else
                    {
                        /* Increase sequence number at every blob write */
                        l_uNweSeq = l_tPagePrm.uPageUseSpecific4 + 1u;

                        /* Generate page params that we want */
                        l_tPagePrm.uPageType = EFSS_PAGETYPE_BLOB;
                        l_tPagePrm.uPageSubType = EFSS_PAGESUBTYPE_BLOBORI;
                        l_tPagePrm.uPageVersion = p_ptCtx->tStorSett.uStorageVer;
                        l_tPagePrm.uPageUseSpecific1 = p_uBuffL;                        /* Blob size */
                        l_tPagePrm.uPageUseSpecific2 = l_uCrcBlob;                      /* Blob CRC */
                        l_tPagePrm.uPageUseSpecific3 = p_ptCtx->tStorSett.uTotPages;    /* Total pages */
                        l_tPagePrm.uPageUseSpecific4 = l_uNweSeq;                       /* Sequential number */
                        l_tPagePrm.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
                        l_tPagePrm.uPageCrc = 0u;

                        /* Write first the original pages */
                        l_eResSub = eFSS_BLOB_WriteBlobNPrm(p_ptCtx, p_puBuff, p_uBuffL, 0u,
                                                           ( p_ptCtx->tStorSett.uTotPages / 2u ), &l_tPagePrm);

                        if( e_eFSS_BLOB_RES_OK != l_eResSub )
                        {
                             l_eRes = l_eResSub;
                        }
                        else
                        {
                            /* Now write backup pages */
                            l_tPagePrm.uPageSubType = EFSS_PAGESUBTYPE_BLOBBKP;
                            l_eResSub = eFSS_BLOB_WriteBlobNPrm( p_ptCtx, p_puBuff, p_uBuffL,
                                                                ( p_ptCtx->tStorSett.uTotPages / 2u ),
                                                                ( p_ptCtx->tStorSett.uTotPages / 2u ), &l_tPagePrm);

                            if( e_eFSS_BLOB_RES_OK != l_eResSub )
                            {
                                l_eRes = l_eResSub;
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
static bool_t eFSS_BLOB_IsStatusStillCoherent(const t_eFSS_BLOB_Ctx* p_ptCtx)
{
    bool_t l_eRes;

	/* Check context validity */
    if( ( NULL == p_ptCtx->tCtxCb.ptCtxErase ) || ( NULL == p_ptCtx->tCtxCb.fErase ) ||
        ( NULL == p_ptCtx->tCtxCb.ptCtxWrite ) || ( NULL == p_ptCtx->tCtxCb.fWrite ) ||
        ( NULL == p_ptCtx->tCtxCb.ptCtxRead  ) || ( NULL == p_ptCtx->tCtxCb.fRead  ) ||
        ( NULL == p_ptCtx->tCtxCb.ptCtxCrc32 ) || ( NULL == p_ptCtx->tCtxCb.fCrc32 ) ||
        ( NULL == p_ptCtx->puBuf ) )
    {
        l_eRes = false;
    }
    else
    {
        /* Check data validity */
        if( ( p_ptCtx->tStorSett.uTotPages < 2u ) || ( 0u != ( p_ptCtx->tStorSett.uTotPages % 2u ) ) )
        {
            l_eRes = false;
        }
        else
        {
            /* Check data validity */
            if( ( p_ptCtx->tStorSett.uPagesLen <= EFSS_PAGEMETASIZE ) ||
                ( 0u != ( p_ptCtx->tStorSett.uPagesLen % 2u ) ) )
            {
                l_eRes = false;
            }
            else
            {
                if( p_ptCtx->tStorSett.uRWERetry <= 0u )
                {
                        l_eRes = false;
                }
                else
                {
                    /* Check data validity */
                    if( ( 2u * p_ptCtx->tStorSett.uPagesLen ) != p_ptCtx->uBufL )
                    {
                        l_eRes = false;
                    }
                    else
                    {
                        l_eRes = true;
                    }
                }
            }
        }
    }

    return l_eRes;
}

static e_eFSS_BLOB_RES eFSS_BLOB_HLtoBLOBRes(const e_eFSS_UTILSHLPRV_RES p_eHLRes)
{
    e_eFSS_BLOB_RES l_eRes;

    switch(p_eHLRes)
    {
        case e_eFSS_UTILSHLPRV_RES_OK:
        {
            l_eRes = e_eFSS_BLOB_RES_OK;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_NOTVALIDPAGE:
        {
            l_eRes = e_eFSS_BLOB_RES_NOTVALIDBLOB;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_OK_BKP_RCVRD:
        {
            l_eRes = e_eFSS_BLOB_RES_OK_BKP_RCVRD;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_BADPARAM:
        {
            l_eRes = e_eFSS_BLOB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_BADPOINTER:
        {
            l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKERASEERR:
        {
            l_eRes = e_eFSS_BLOB_RES_CLBCKERASEERR;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKWRITEERR:
        {
            l_eRes = e_eFSS_BLOB_RES_CLBCKWRITEERR;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKREADERR:
        {
            l_eRes = e_eFSS_BLOB_RES_CLBCKREADERR;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKCRCERR:
        {
            l_eRes = e_eFSS_BLOB_RES_CLBCKCRCERR;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_WRITENOMATCHREAD:
        {
            l_eRes = e_eFSS_BLOB_RES_WRITENOMATCHREAD;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CORRUPT:
        {
            l_eRes = e_eFSS_BLOB_RES_CORRUPTCTX;
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

static e_eFSS_BLOB_RES eFSS_BLOB_CrcBlobFull(t_eFSS_BLOB_Ctx* const p_ptCtx, uint8_t* p_puBlob, uint32_t p_uBlobL,
                                             uint32_t* p_puBlobCrc)
{
	/* Local variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;
    uint32_t l_uRawDataP;
    uint32_t l_uRawDataWhole;
    uint32_t l_uUnusedByte;
    uint32_t l_uCrcD;
    uint8_t* l_puBuF1;
    uint32_t l_uBuF1L;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBlob ) || ( NULL == p_puBlobCrc ) )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
        /* Calculate the raw space for data in every page */
        l_uRawDataP = p_ptCtx->tStorSett.uPagesLen - EFSS_PAGEMETASIZE;
        l_uRawDataWhole = l_uRawDataP * ( p_ptCtx->tStorSett.uTotPages / 2u );

        if( l_uRawDataWhole < p_uBlobL )
        {
            l_eRes = e_eFSS_BLOB_RES_BADPARAM;
        }
        else
        {
            /* Calc unused byte  */
            l_uUnusedByte = l_uRawDataWhole - p_uBlobL;

            /* Set calc CRC to default seed (0xFFFFFFFF) */
            l_uCrcD = MAX_UINT32VAL;

            /* If blob has some size calculate the crc of the blob */
            if( 0u != p_uBlobL )
            {
                l_eHLRes = eFSS_UTILSHLPRV_CrcDigest(&p_ptCtx->tCtxCb, l_uCrcD, p_puBlob , p_uBlobL, &l_uCrcD);
                l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
            }
            else
            {
                l_eRes = e_eFSS_BLOB_RES_OK;
            }

            /* Add to the blob crc the crc of the blank space */
            if( e_eFSS_BLOB_RES_OK == l_eRes )
            {
                if( 0u != l_uUnusedByte )
                {
                    /* Get buffer for calculation */
                    l_puBuF1 = p_ptCtx->puBuf;
                    l_uBuF1L = p_ptCtx->uBufL / 2u ;

                    /* Calculate the remainings using zero as data */
                    memset(l_puBuF1, 0u, l_uBuF1L);

                    while( ( l_uUnusedByte > 0u ) && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                    {
                        if( l_uUnusedByte >= l_uBuF1L )
                        {
                            l_eHLRes = eFSS_UTILSHLPRV_CrcDigest(&p_ptCtx->tCtxCb, l_uCrcD, l_puBuF1,
                                                                 l_uBuF1L, &l_uCrcD);
                            l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);

                            if( e_eFSS_BLOB_RES_OK == l_eRes )
                            {
                                l_uUnusedByte -= l_uBuF1L;
                            }
                        }
                        else
                        {
                            l_eHLRes = eFSS_UTILSHLPRV_CrcDigest(&p_ptCtx->tCtxCb, l_uCrcD, l_puBuF1,
                                                                 l_uUnusedByte, &l_uCrcD);
                            l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);

                            if( e_eFSS_BLOB_RES_OK == l_eRes )
                            {
                                l_uUnusedByte = 0u;
                            }
                        }
                    }
                }
            }
        }
    }

    return l_eRes;
}

static e_eFSS_BLOB_RES eFSS_BLOB_OriginBackupAligner(t_eFSS_BLOB_Ctx* const p_ptCtx)
{
	/* Local variable return */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;

    /* Local variable Cycle data */
    uint8_t* l_puBuF1;
    uint32_t l_uBuF1L;
    uint8_t* l_puBuF2;
    uint32_t l_uBuF2L;
    bool_t l_bIsOriginValid;
    bool_t l_bIsBackupValid;
    t_eFSS_TYPE_PageMeta l_tMetaOri;
    t_eFSS_TYPE_PageMeta l_tMetaBkp;
    uint32_t l_uI;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
        /* Get buffer for calculation */
        l_puBuF1 = p_ptCtx->puBuf;
        l_uBuF1L = p_ptCtx->uBufL / 2u ;
        l_puBuF2 = &p_ptCtx->puBuf[l_uBuF1L];
        l_uBuF2L = p_ptCtx->uBufL / 2u ;

        /* Verify the validity of original sequence and backup sequence  */
        l_eRes = eFSS_BLOB_ArePagesValid(p_ptCtx, 0u, ( p_ptCtx->tStorSett.uTotPages/2u ), &l_bIsOriginValid, &l_tMetaOri, EFSS_PAGESUBTYPE_BLOBORI);

        if( e_eFSS_BLOB_RES_OK == l_eRes )
        {
            l_eRes = eFSS_BLOB_ArePagesValid(p_ptCtx, ( p_ptCtx->tStorSett.uTotPages/2u ), ( p_ptCtx->tStorSett.uTotPages/2u ),
                                             &l_bIsBackupValid, &l_tMetaBkp, EFSS_PAGESUBTYPE_BLOBBKP);

            if( e_eFSS_BLOB_RES_OK == l_eRes )
            {
                if( ( false == l_bIsOriginValid ) && ( false == l_bIsBackupValid ) )
                {
                    /* None of the sequence is valid, need to format the storage */
                    l_eRes = e_eFSS_BLOB_RES_NOTVALIDBLOB;
                }
                else if( ( true == l_bIsOriginValid ) && ( false == l_bIsBackupValid ) )
                {
                    /* Need to recover backup */
                    l_uI = 0u;

                    while( (l_uI <= ( (p_ptCtx->tStorSett.uTotPages/2u) - 1u) )  && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                    {
                        l_eHLRes = eFSS_UTILSHLPRV_ClonePage(&p_ptCtx->tCtxCb, p_ptCtx->tStorSett.uRWERetry,
                                                             l_puBuF1, l_uBuF1L,
                                                             l_puBuF2, l_uBuF2L,
                                                             l_uI, ( l_uI + (p_ptCtx->tStorSett.uTotPages/2u) ) );
                        l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
                    }

                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                    {
                        /* Backup recovered */
                        l_eRes = e_eFSS_BLOB_RES_OK_BKP_RCVRD;

                        if( p_ptCtx->tStorSett.uStorageVer != l_tMetaOri.uPageVersion )
                        {
                            /* New version found */
                            l_eRes = e_eFSS_BLOB_RES_NEWVERSIONBLOB;
                        }
                    }
                }
                else if( ( false == l_bIsOriginValid ) && ( true == l_bIsBackupValid ) )
                {
                    /* Need to recover original data */
                    l_uI = 0u;

                    while( (l_uI <= ( (p_ptCtx->tStorSett.uTotPages/2u) - 1u) )  && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                    {
                        l_eHLRes = eFSS_UTILSHLPRV_ClonePage(&p_ptCtx->tCtxCb, p_ptCtx->tStorSett.uRWERetry,
                                                             l_puBuF1, l_uBuF1L,
                                                             l_puBuF2, l_uBuF2L,
                                                             ( l_uI + (p_ptCtx->tStorSett.uTotPages/2u) ), l_uI);
                        l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
                    }

                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                    {
                        /* Original recovered */
                        l_eRes = e_eFSS_BLOB_RES_OK_BKP_RCVRD;

                        if( p_ptCtx->tStorSett.uStorageVer != l_tMetaBkp.uPageVersion )
                        {
                            /* New version found */
                            l_eRes = e_eFSS_BLOB_RES_NEWVERSIONBLOB;
                        }
                    }
                }
                else
                {
                    /* Both backup and origin data are valid, verify if they are equals */
                    if( ( l_tMetaBkp.uPageUseSpecific4 != l_tMetaOri.uPageUseSpecific4 ) ||
                        ( l_tMetaBkp.uPageUseSpecific1 != l_tMetaOri.uPageUseSpecific1 ) ||
                        ( l_tMetaBkp.uPageUseSpecific2 != l_tMetaOri.uPageUseSpecific2 ) )
                    {
                        /* They are different, copy from origin to backup */
                        l_uI = 0u;

                        while( (l_uI <= ( (p_ptCtx->tStorSett.uTotPages/2u) - 1u) )  && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                        {
                            l_eHLRes = eFSS_UTILSHLPRV_ClonePage(&p_ptCtx->tCtxCb, p_ptCtx->tStorSett.uRWERetry,
                                                                 l_puBuF1, l_uBuF1L,
                                                                 l_puBuF2, l_uBuF2L,
                                                                 l_uI, ( l_uI + (p_ptCtx->tStorSett.uTotPages/2u) ) );
                            l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
                        }

                        if( e_eFSS_BLOB_RES_OK == l_eRes )
                        {
                            /* Backup recovered */
                            l_eRes = e_eFSS_BLOB_RES_OK_BKP_RCVRD;

                            if( p_ptCtx->tStorSett.uStorageVer != l_tMetaOri.uPageVersion )
                            {
                                /* New version found */
                                l_eRes = e_eFSS_BLOB_RES_NEWVERSIONBLOB;
                            }
                        }
                    }
                    else
                    {
                        /* They are equals */
                        if( p_ptCtx->tStorSett.uStorageVer != l_tMetaOri.uPageVersion )
                        {
                            /* New version found */
                            l_eRes = e_eFSS_BLOB_RES_NEWVERSIONBLOB;
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}

static e_eFSS_BLOB_RES eFSS_BLOB_ArePagesValid(t_eFSS_BLOB_Ctx* const p_ptCtx, uint32_t p_uStartIdx, uint32_t p_uNPages,
                                               bool_t* p_pbAreValid, t_eFSS_TYPE_PageMeta* p_ptMeta, uint8_t p_uSubT)
{
	/* Local variable return */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLR;

    /* Local variable Cycle data */
    uint8_t* l_puBuF1;
    uint32_t l_uBuF1L;
    uint32_t l_uI;
    t_eFSS_TYPE_PageMeta l_tPagePrm;
    t_eFSS_TYPE_PageMeta l_tPagePrmFirst;
    bool_t l_bIsValidPage;
    bool_t l_bIsValidSequence;
    uint32_t l_uDCrc;
    uint32_t l_uRawDataP;
    uint32_t l_uRawDataWhole;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbAreValid ) || ( NULL == p_ptMeta ) )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity */
        if( ( p_uNPages > p_ptCtx->tStorSett.uTotPages ) || ( p_uNPages <= 0u )  )
        {
            l_eRes = e_eFSS_BLOB_RES_BADPARAM;
        }
        else
        {
            /* Calculate the raw space for data in every page */
            l_uRawDataP = p_ptCtx->tStorSett.uPagesLen - EFSS_PAGEMETASIZE;

            /* Init variable */
            l_uI = p_uStartIdx;
            l_bIsValidSequence = true;
            l_eRes = e_eFSS_BLOB_RES_OK;
            l_uDCrc = 0u;

            /* Get buffer for calculation */
            l_puBuF1 = p_ptCtx->puBuf;
            l_uBuF1L = p_ptCtx->uBufL / 2u ;

            while( ( l_uI < ( p_uNPages + p_uStartIdx ) ) && ( true == l_bIsValidSequence ) &&
                   ( e_eFSS_BLOB_RES_OK == l_eRes ) )
            {
                /* Read a page, verify if it's valid and digest the CRC of the related BLOB */
                l_eHLR = eFSS_UTILSHLPRV_ReadPageNPrm(&p_ptCtx->tCtxCb, l_uI,  l_puBuF1, l_uBuF1L,
                                                      &l_tPagePrm, p_ptCtx->tStorSett.uRWERetry);
                l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLR);
                if( e_eFSS_BLOB_RES_OK == l_eRes )
                {
                    /* Verify if page is correct */
                    l_eHLR = eFSS_UTILSHLPRV_IsValidPageInBuff(&p_ptCtx->tCtxCb, l_puBuF1, l_uBuF1L,
                                                               &l_bIsValidPage);
                    l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLR);
                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                    {
                        if( true == l_bIsValidPage)
                        {
                            if( p_uStartIdx == l_uI )
                            {
                                /* First page, save the related metadata in order to be able to verify metadata
                                 * of others page present in the sequence */
                                l_tPagePrmFirst = l_tPagePrm;

                                /* Validate data */
                                if( ( EFSS_PAGETYPE_BLOB != l_tPagePrmFirst.uPageType ) ||
                                    ( p_uSubT != l_tPagePrmFirst.uPageSubType ) ||
                                    ( EFSS_PAGEMAGICNUMBER != l_tPagePrmFirst.uPageMagicNumber ) ||
                                    ( p_ptCtx->tStorSett.uTotPages != l_tPagePrmFirst.uPageUseSpecific3 ) )
                                {
                                    /* Not valid */
                                    l_bIsValidSequence = false;
                                }
                                else
                                {
                                    /* Calc CRC of the data only starting from here ( No metadata ) */
                                    l_eHLR = eFSS_UTILSHLPRV_CrcDigest(&p_ptCtx->tCtxCb, MAX_UINT32VAL,
                                                                       l_puBuF1 , l_uRawDataP, &l_uDCrc);
                                    l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLR);
                                }
                            }
                            else
                            {
                                /* Validate the metada of the current page against the one of the first */
                                if( ( l_tPagePrm.uPageType != l_tPagePrmFirst.uPageType ) ||
                                    ( l_tPagePrm.uPageSubType != l_tPagePrmFirst.uPageSubType ) ||
                                    ( l_tPagePrm.uPageVersion != l_tPagePrmFirst.uPageVersion ) ||
                                    ( l_tPagePrm.uPageUseSpecific1 != l_tPagePrmFirst.uPageUseSpecific1 ) ||
                                    ( l_tPagePrm.uPageUseSpecific2 != l_tPagePrmFirst.uPageUseSpecific2 ) ||
                                    ( l_tPagePrm.uPageUseSpecific3 != l_tPagePrmFirst.uPageUseSpecific3 ) ||
                                    ( l_tPagePrm.uPageUseSpecific4 != l_tPagePrmFirst.uPageUseSpecific4 ) ||
                                    ( l_tPagePrm.uPageMagicNumber != l_tPagePrmFirst.uPageMagicNumber ) )
                                {
                                    /* Not valid */
                                    l_bIsValidSequence = false;
                                }
                                else
                                {
                                    /* Continue to digest CRC */
                                    l_eHLR = eFSS_UTILSHLPRV_CrcDigest(&p_ptCtx->tCtxCb, l_uDCrc, l_puBuF1,
                                                                           l_uRawDataP, &l_uDCrc);
                                    l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLR);
                                }
                            }
                        }
                        else
                        {
                            /* Every invalid page invalidate the whole sequence */
                            l_bIsValidSequence = false;
                        }
                    }
                }

                l_uI++;
            }

            if( e_eFSS_BLOB_RES_OK == l_eRes )
            {
                /* No error found */
                if( true == l_bIsValidSequence )
                {
                    /* Verify if the stored crc of the blob is equals to the calculated one */
                    if( l_tPagePrmFirst.uPageUseSpecific2 == l_uDCrc )
                    {
                        /* Calculate the raw space for data in every page */
                        l_uRawDataWhole = l_uRawDataP * ( p_ptCtx->tStorSett.uTotPages / 2u );

                        /* Verify coherence blob size */
                        if( l_tPagePrmFirst.uPageUseSpecific1 <= l_uRawDataWhole )
                        {
                            *p_pbAreValid = true;
                            *p_ptMeta = l_tPagePrmFirst;
                        }
                        else
                        {
                            *p_pbAreValid = false;
                         }
                    }
                    else
                    {
                        *p_pbAreValid = false;
                    }
                }
                else
                {
                    *p_pbAreValid = false;
                }
            }
        }
	}

	return l_eRes;
}

static e_eFSS_BLOB_RES eFSS_BLOB_WriteBlobNPrm(t_eFSS_BLOB_Ctx* const p_ptCtx, uint8_t* p_puBuff, uint32_t p_uBuffL,
                                               uint32_t p_uStartId, uint32_t p_uNPages, t_eFSS_TYPE_PageMeta* p_ptMeta)
{
	/* Local variable for result */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;

    /* Local calc variable */
    uint8_t* l_puBuF1;
    uint32_t l_uBuF1L;
    uint8_t* l_puBuF2;
    uint32_t l_uBuF2L;
    uint32_t l_uI;
    uint32_t l_uToWrite;
    uint32_t l_uCrcBlob;
    uint32_t l_uRawDataP;
    uint32_t l_uRawDataWhole;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) || (NULL == p_ptMeta ) )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
        /* Check param validity */
        if( p_uNPages <= 0u )
        {
            l_eRes = e_eFSS_BLOB_RES_BADPARAM;
        }
        else
        {
            /* Calculate the raw space for data in every page */
            l_uRawDataP = p_ptCtx->tStorSett.uPagesLen - EFSS_PAGEMETASIZE;
            l_uRawDataWhole = l_uRawDataP * p_uNPages;

            /* Check data validity */
            if( ( p_uBuffL <= 0u ) || ( p_uBuffL >= l_uRawDataWhole ) )
            {
                l_eRes = e_eFSS_BLOB_RES_BADPARAM;
            }
            else
            {
                /* Get buffer for calculation */
                l_puBuF1 = p_ptCtx->puBuf;
                l_uBuF1L = p_ptCtx->uBufL / 2u ;
                l_puBuF2 = &p_ptCtx->puBuf[l_uBuF1L];
                l_uBuF2L = p_ptCtx->uBufL / 2u ;

                /* Calculate the CRC of the data blob */
                l_eRes = eFSS_BLOB_CrcBlobFull(p_ptCtx, p_puBuff, p_uBuffL, &l_uCrcBlob);

                if( e_eFSS_BLOB_RES_OK == l_eRes )
                {
                    /* init cycle variable */
                    l_uI = 0u;
                    l_uToWrite = p_uBuffL;

                    /* write blob data in original area, and un used space is setted to zero. After
                        that write the same things in the backup area */
                    while( ( l_uI < p_uNPages )  &&
                           ( ( e_eFSS_BLOB_RES_OK == l_eRes ) || ( e_eFSS_BLOB_RES_OK_BKP_RCVRD == l_eRes ) ) )
                    {
                        memset(l_puBuF1, 0u, l_uBuF1L);

                        if( l_uToWrite > 0u )
                        {
                            if( l_uToWrite >= l_uRawDataP )
                            {
                                memcpy(&p_puBuff[ p_uBuffL - l_uToWrite ], l_puBuF1, l_uRawDataP );
                                l_uToWrite -= l_uRawDataP;
                            }
                            else
                            {
                                memcpy(&p_puBuff[ p_uBuffL - l_uToWrite ], l_puBuF1, l_uToWrite );
                                l_uToWrite = 0u;
                            }
                        }
                        else
                        {
                            /* No blob data related avaiable, just write the zeros */
                        }

                        l_eHLRes = eFSS_UTILSHLPRV_WritePagePrmNUpCrc(&p_ptCtx->tCtxCb, l_uI, l_puBuF1,
                                                                      l_uBuF1L, l_puBuF2, l_uBuF2L, p_ptMeta,
                                                                      p_ptCtx->tStorSett.uRWERetry);
                        if( e_eFSS_UTILSHLPRV_RES_OK != l_eHLRes )
                        {
                            l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}