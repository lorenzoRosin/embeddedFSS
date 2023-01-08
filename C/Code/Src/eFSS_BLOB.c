/**
 * @file       eFSS_BLOB.c
 *
 * @brief      Blob large file module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/



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
static e_eFSS_BLOB_RES eFSS_BLOB_CrcBlobFull(const t_eFSS_BLOB_Ctx* p_ptCtx, uint8_t* p_puBlob, uint32_t p_uBlobL,
                                             uint32_t* p_puBlobCrc);
static e_eFSS_BLOB_RES eFSS_BLOB_OriginBackupAligner(const t_eFSS_BLOB_Ctx* p_ptCtx);
static e_eFSS_BLOB_RES eFSS_BLOB_ArePagesValid(const t_eFSS_BLOB_Ctx* p_ptCtx, uint32_t p_uStartIdx, uint32_t p_uEndIdx,
                                               bool_t* p_pbAreValid, t_eFSS_TYPE_PageMeta* p_ptMeta);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_BLOB_RES eFSS_BLOB_InitCtx(t_eFSS_BLOB_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx* const p_ptCtxCb,
                                  const uint32_t p_uPageToUse, const uint32_t p_uPageSize, uint8_t* const p_puBuff,
                                  uint32_t p_uBuffL, uint16_t p_uBlobVersion, uint32_t p_uRetry)
{
    e_eFSS_BLOB_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ptCtxCb ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_ptCtxCb->ptCtxErase ) || ( NULL == p_ptCtxCb->fErase ) ||
            ( NULL == p_ptCtxCb->ptCtxWrite ) || ( NULL == p_ptCtxCb->fWrite ) ||
            ( NULL == p_ptCtxCb->ptCtxRead  ) || ( NULL == p_ptCtxCb->fRead  ) ||
            ( NULL == p_ptCtxCb->ptCtxCrc32 ) || ( NULL == p_ptCtxCb->fCrc32 ) )
        {
            l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( ( p_uPageToUse < 2u ) || ( 0u != ( p_uPageToUse % 2u ) ) )
            {
                l_eRes = e_eFSS_BLOB_RES_BADPARAM;
            }
            else
            {
                /* Check data validity */
                if( ( p_uPageSize <= EFSS_PAGEMETASIZE ) || ( 0u != ( p_uPageSize % 2u ) ) )
                {
                    l_eRes = e_eFSS_BLOB_RES_BADPARAM;
                }
                else
                {
                    /* Check data validity */
                    if( p_uBuffL != ( 2u * p_uPageSize ) )
                    {
                        l_eRes = e_eFSS_BLOB_RES_BADPARAM;
                    }
                    else
                    {
                        /* Check data validity */
                        if( p_uRetry <= 0u )
                        {
                            l_eRes = e_eFSS_BLOB_RES_BADPARAM;
                        }
                        else
                        {
                            /* Fill context */
                            p_ptCtx->bIsInit = true;
                            p_ptCtx->ptCtxCb = p_ptCtxCb;
                            p_ptCtx->puBuff1 = p_puBuff;
                            p_ptCtx->uBuff1L = p_uBuffL / 2u;
                            p_ptCtx->puBuff2 = &p_puBuff[p_ptCtx->uBuff1L];
                            p_ptCtx->uBuff2L = p_uBuffL / 2u;
                            p_ptCtx->uNPage = p_uPageToUse;
                            p_ptCtx->uPageSize = p_uPageSize;
                            p_ptCtx->uReTry = p_uRetry;
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

e_eFSS_BLOB_RES eFSS_BLOB_Format(t_eFSS_BLOB_Ctx* const p_ptCtx)
{
	/* Local variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;
    uint32_t l_uCrcZeroBlob;
    uint32_t l_i;
    t_eFSS_TYPE_PageMeta l_tPagePrmToWrite;

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
                /* Calculate the CRC of the zero data blob */
                memset(p_ptCtx->puBuff1, 0u, p_ptCtx->uBuff1L);
                l_uCrcZeroBlob = 0u;
                for(l_i = 0u; l_i < p_ptCtx->uNPage / 2u; l_i++)
                {
                    if( 0u == l_i )
                    {
                        l_eHLRes = eFSS_UTILSHLPRV_CrcDigest(p_ptCtx->ptCtxCb, MAX_UINT32VAL, p_ptCtx->puBuff1 , p_ptCtx->uBuff1L - EFSS_PAGEMETASIZE, &l_uCrcZeroBlob);
                    }
                    else
                    {
                        l_eHLRes = eFSS_UTILSHLPRV_CrcDigest(p_ptCtx->ptCtxCb, l_uCrcZeroBlob, p_ptCtx->puBuff1 , p_ptCtx->uBuff1L - EFSS_PAGEMETASIZE, &l_uCrcZeroBlob);
                    }

                    l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
                }

                /* Format to zero everything */

                /* First format the original pages, update blob size and CRC value  */
                l_eRes = e_eFSS_BLOB_RES_OK;
                l_i = 0u;

                while( (l_i < p_ptCtx->uNPage / 2u)  && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                {
                    memset(p_ptCtx->puBuff1, 0u, p_ptCtx->uBuff1L);
                    l_tPagePrmToWrite.uPageType = 0u;
                    l_tPagePrmToWrite.uPageSubType = 0u;
                    l_tPagePrmToWrite.uPageVersion = 0u;
                    l_tPagePrmToWrite.uPageUseSpecific1 = 0u;
                    l_tPagePrmToWrite.uPageUseSpecific2 = l_uCrcZeroBlob;
                    l_tPagePrmToWrite.uPageSequentialN = 0u;
                    l_tPagePrmToWrite.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
                    l_tPagePrmToWrite.uPageCrc = 0u;

                    l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->ptCtxCb, l_i,
                                                         p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                         p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                         &l_tPagePrmToWrite, p_ptCtx->uReTry);
                    l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);

                }

                /* Now format the backup pages, update blob size and CRC value  */
                if( e_eFSS_BLOB_RES_OK == l_eRes )
                {
                    l_i = p_ptCtx->uNPage / 2u;

                    while( (l_i < p_ptCtx->uNPage)  && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                    {
                        memset(p_ptCtx->puBuff1, 0u, p_ptCtx->uBuff1L);
                        l_tPagePrmToWrite.uPageType = 0u;
                        l_tPagePrmToWrite.uPageSubType = 0u;
                        l_tPagePrmToWrite.uPageVersion = 0u;
                        l_tPagePrmToWrite.uPageUseSpecific1 = 0u;
                        l_tPagePrmToWrite.uPageUseSpecific2 = l_uCrcZeroBlob;
                        l_tPagePrmToWrite.uPageSequentialN = 0u;
                        l_tPagePrmToWrite.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
                        l_tPagePrmToWrite.uPageCrc = 0u;

                        l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->ptCtxCb, l_i,
                                                             p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                             p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                             &l_tPagePrmToWrite, p_ptCtx->uReTry);
                        l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);

                    }
                }
            }
		}
	}

	return l_eRes;
}

e_eFSS_BLOB_RES eFSS_BLOB_GetInfo(t_eFSS_BLOB_Ctx* const p_ptCtx, bool_t* p_puBlobSize)
{
	/* Local variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;
    t_eFSS_TYPE_PageMeta p_ptPagePrm;

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
                    l_eHLRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, 0u,  p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                            &p_ptPagePrm, p_ptCtx->uReTry);
                    l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
                    if( e_eFSS_BLOB_RES_OK != l_eRes )
                    {
                        *p_puBlobSize = p_ptPagePrm.uPageUseSpecific1;
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
	/* Local variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;
    t_eFSS_TYPE_PageMeta p_ptPagePrm;
    uint32_t l_i;
    uint32_t l_uReaded;

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
                        /* First format the original pages, update blob size and CRC value  */
                        l_i = 0u;
                        l_uReaded = 0u;

                        /* Read all pages */
                        while( (l_i < p_ptCtx->uNPage / 2u)  && ( e_eFSS_BLOB_RES_OK == l_eRes ) && ( l_uReaded < p_uBuffL) && ( l_uReaded < p_uBuffL) )
                        {
                            l_eHLRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, 0u,  p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                                    &p_ptPagePrm, p_ptCtx->uReTry);
                            l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
                            if( e_eFSS_BLOB_RES_OK != l_eRes )
                            {
                                if( p_ptPagePrm.uPageUseSpecific1 > l_uReaded )
                                {
                                    memcpy(&p_puBuff[l_uReaded], p_ptCtx->puBuff1, p_ptCtx->uPageSize - EFSS_PAGEMETASIZE );
                                    l_uReaded += p_ptCtx->uPageSize - EFSS_PAGEMETASIZE;
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

e_eFSS_BLOB_RES eFSS_BLOB_WriteAllBlob(t_eFSS_BLOB_Ctx* const p_ptCtx, uint8_t* p_puBuff, uint32_t p_uBuffL)
{
	/* Local variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;
    uint32_t l_i;
    uint32_t l_uWritten;
    t_eFSS_TYPE_PageMeta p_ptPagePrm;
    uint32_t l_uCrcZeroBlob;

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
                        /* Calculate the CRC of the data blob */
                        l_eRes = eFSS_BLOB_CrcBlobFull(p_ptCtx, p_puBuff, p_uBuffL, &l_uCrcZeroBlob);

                        /* Read a page in order to get the seqeuntial number */
                        l_eHLRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, 0u,  p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                                &p_ptPagePrm, p_ptCtx->uReTry);
                        l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);

                        /* First format the original pages, update blob size and CRC value  */
                        l_i = 0u;
                        l_uWritten = 0u;

                        /* Read all pages */
                        while( (l_i < p_ptCtx->uNPage / 2u)  && ( e_eFSS_BLOB_RES_OK == l_eRes ) && ( l_uWritten < p_uBuffL) && ( l_uWritten < p_uBuffL) )
                        {
                            memcpy(&p_puBuff[l_uWritten], p_ptCtx->puBuff1, p_ptCtx->uPageSize - EFSS_PAGEMETASIZE );
                            p_ptPagePrm.uPageType = EFSS_PAGETYPE_BLOB;
                            p_ptPagePrm.uPageSubType = EFSS_PAGESUBTYPE_BLOB;
                            p_ptPagePrm.uPageVersion = p_ptCtx->uVersion;
                            p_ptPagePrm.uPageUseSpecific1 = p_uBuffL;
                            p_ptPagePrm.uPageUseSpecific2 = l_uCrcZeroBlob;
                            p_ptPagePrm.uPageSequentialN = p_ptPagePrm.uPageSequentialN +1u;
                            p_ptPagePrm.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
                            p_ptPagePrm.uPageCrc = 0u;

                            l_eHLRes = eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->ptCtxCb, l_i,  p_ptCtx->puBuff1, p_ptCtx->uBuff1L, p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                                       &p_ptPagePrm, p_ptCtx->uReTry);
                            l_uWritten += p_ptCtx->uPageSize - EFSS_PAGEMETASIZE;
                            l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
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
    if( ( NULL == p_ptCtx->ptCtxCb->ptCtxErase ) || ( NULL == p_ptCtx->ptCtxCb->fErase ) ||
        ( NULL == p_ptCtx->ptCtxCb->ptCtxWrite ) || ( NULL == p_ptCtx->ptCtxCb->fWrite ) ||
        ( NULL == p_ptCtx->ptCtxCb->ptCtxRead  ) || ( NULL == p_ptCtx->ptCtxCb->fRead  ) ||
        ( NULL == p_ptCtx->ptCtxCb->ptCtxCrc32 ) || ( NULL == p_ptCtx->ptCtxCb->fCrc32 ) )
    {
        l_eRes = false;
    }
    else
    {
        /* Check data validity */
        if( ( p_ptCtx->uNPage < 2u ) || ( 0u != ( p_ptCtx->uNPage % 2u ) ) )
        {
            l_eRes = false;
        }
        else
        {
            /* Check data validity */
            if( ( p_ptCtx->uPageSize <= EFSS_PAGEMETASIZE ) || ( 0u != ( p_ptCtx->uPageSize % 2u ) ) )
            {
                l_eRes = false;
            }
            else
            {
                if( ( p_ptCtx->uPageSize != p_ptCtx->uBuff1L ) || ( p_ptCtx->uPageSize != p_ptCtx->uBuff2L ) )
                {
                    l_eRes = false;
                }
                else
                {
                    /* Check data validity */
                    if( p_ptCtx->uReTry <= 0u )
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

static e_eFSS_BLOB_RES eFSS_BLOB_CrcBlobFull(const t_eFSS_BLOB_Ctx* p_ptCtx, uint8_t* p_puBlob, uint32_t p_uBlobL,
                                             uint32_t* p_puBlobCrc)
{
	/* Local variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;
    uint32_t l_uUnusedByte;
    uint32_t l_uCrcDigested;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBlob ) || ( NULL == p_puBlobCrc ) )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
        l_uUnusedByte = ( p_ptCtx->uNPage / 2u ) * ( p_ptCtx->uPageSize - EFSS_PAGEMETASIZE ) - p_uBlobL;
        l_uCrcDigested = MAX_UINT32VAL;

        if( 0u != p_uBlobL )
        {
            l_eHLRes = eFSS_UTILSHLPRV_CrcDigest(p_ptCtx->ptCtxCb, l_uCrcDigested, p_puBlob , p_uBlobL, &l_uCrcDigested);
            l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
        }

        if( e_eFSS_BLOB_RES_OK == l_eRes )
        {
            if( 0u != l_uUnusedByte )
            {
                /* Calculate the remainings using zero as data */
                memset(p_ptCtx->puBuff1, 0u, p_ptCtx->uBuff1L);

                while( ( l_uUnusedByte > 0u) && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                {
                    if( l_uUnusedByte >= p_ptCtx->uBuff1L )
                    {
                        l_eHLRes = eFSS_UTILSHLPRV_CrcDigest(p_ptCtx->ptCtxCb, l_uCrcDigested, p_ptCtx->puBuff1 , p_ptCtx->uBuff1L, &l_uCrcDigested);
                        l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
                        if( e_eFSS_BLOB_RES_OK == l_eRes )
                        {
                            l_uUnusedByte -= p_ptCtx->uBuff1L;
                        }
                    }
                    else
                    {
                        l_eHLRes = eFSS_UTILSHLPRV_CrcDigest(p_ptCtx->ptCtxCb, l_uCrcDigested, p_ptCtx->puBuff1 , l_uUnusedByte, &l_uCrcDigested);
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

static e_eFSS_BLOB_RES eFSS_BLOB_OriginBackupAligner(const t_eFSS_BLOB_Ctx* p_ptCtx)
{
	/* Local variable */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;

    bool_t l_bIsOriginValid;
    t_eFSS_TYPE_PageMeta l_tMetaOrigin;
    bool_t l_bIsBackupValid;
    t_eFSS_TYPE_PageMeta l_tMetaBackUp;
    uint32_t l_i;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
        /* Need to check if the storage is valid or not */
        l_eRes = eFSS_BLOB_ArePagesValid(p_ptCtx, 0u, ( (p_ptCtx->uNPage/2u) - 1u), &l_bIsOriginValid, &l_tMetaOrigin);

        if( e_eFSS_BLOB_RES_OK == l_eRes )
        {
            l_eRes = eFSS_BLOB_ArePagesValid(p_ptCtx, ( p_ptCtx->uNPage/2u), (p_ptCtx->uNPage- 1u), &l_bIsBackupValid, &l_tMetaBackUp);

            if( e_eFSS_BLOB_RES_OK == l_eRes )
            {
                if( ( false == l_bIsOriginValid ) && ( false == l_bIsBackupValid ) )
                {
                    /* No valid data */
                    l_eRes = e_eFSS_BLOB_RES_NOTVALIDBLOB;
                }
                else if( ( true == l_bIsOriginValid ) && ( false == l_bIsBackupValid ) )
                {
                    /* Need to recover backup */
                    l_i = 0u;

                    while( (l_i <= ( (p_ptCtx->uNPage/2u) - 1u) )  && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                    {
                        l_eHLRes = eFSS_UTILSHLPRV_ClonePage(p_ptCtx->ptCtxCb, p_ptCtx->uReTry,
                                                 p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                 p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                 l_i, ( l_i + (p_ptCtx->uNPage/2u) ) );
                        l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
                    }

                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                    {
                        /* Backup recovered */
                        l_eRes = e_eFSS_BLOB_RES_OK_BKP_RCVRD;
                    }
                }
                else if( ( false == l_bIsOriginValid ) && ( true == l_bIsBackupValid ) )
                {
                    /* Need to recover original data */
                    l_i = 0u;

                    while( (l_i <= ( (p_ptCtx->uNPage/2u) - 1u) )  && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                    {
                        l_eHLRes = eFSS_UTILSHLPRV_ClonePage(p_ptCtx->ptCtxCb, p_ptCtx->uReTry,
                                                 p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                 p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                 ( l_i + (p_ptCtx->uNPage/2u) ), l_i);
                        l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
                    }

                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                    {
                        /* Original recovered */
                        l_eRes = e_eFSS_BLOB_RES_OK_BKP_RCVRD;
                    }
                }
                else
                {
                    /* Both backup and origin data are valid, verify if they are equals */
                    l_i = 0u;

                    while( (l_i <= ( (p_ptCtx->uNPage/2u) - 1u) )  && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                    {
                        l_eHLRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, l_i,  p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                                &l_tMetaOrigin, p_ptCtx->uReTry);
                        l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);

                        if( e_eFSS_BLOB_RES_OK == l_eRes )
                        {
                            l_eHLRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, l_i,  p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                                    &l_tMetaBackUp, p_ptCtx->uReTry);
                            l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
                            if( e_eFSS_BLOB_RES_OK == l_eRes )
                            {
                                if( 0u != memcmp(p_ptCtx->puBuff1, p_ptCtx->puBuff2, p_ptCtx->uBuff1L) )
                                {
                                    l_eHLRes = eFSS_UTILSHLPRV_ClonePage(p_ptCtx->ptCtxCb, p_ptCtx->uReTry,
                                                                         p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                                         p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                                         ( l_i + (p_ptCtx->uNPage/2u) ), l_i);
                                    l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
                                }
                            }
                        }
                    }

                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                    {
                        /* Original recovered */
                        l_eRes = e_eFSS_BLOB_RES_OK_BKP_RCVRD;
                    }
                }
            }
        }
	}

	return l_eRes;
}

static e_eFSS_BLOB_RES eFSS_BLOB_ArePagesValid(const t_eFSS_BLOB_Ctx* p_ptCtx, uint32_t p_uStartIdx, uint32_t p_uEndIdx,
                                               bool_t* p_pbAreValid, t_eFSS_TYPE_PageMeta* p_ptMeta)
{
	/* Local variable return */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;

    /* Local variable Cycle data */
    uint32_t l_i;
    t_eFSS_TYPE_PageMeta l_tPagePrm;
    t_eFSS_TYPE_PageMeta l_tPagePrmFirst;
    bool_t l_bIsValidPage;
    bool_t l_bIsValidSequence;
    uint32_t l_uDigestCrc32;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbAreValid ) || ( NULL == p_ptMeta ) )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity */
        if( p_uStartIdx > p_uEndIdx )
        {
            l_eRes = e_eFSS_BLOB_RES_BADPARAM;
        }
        else
        {
            /* Check data validity */
            if( ( p_uEndIdx - p_uStartIdx ) > p_ptCtx->uNPage )
            {
                l_eRes = e_eFSS_BLOB_RES_BADPARAM;
            }
            else
            {
                /* Init variable */
                l_eRes = e_eFSS_BLOB_RES_OK;
                l_uDigestCrc32 = 0u;
                l_bIsValidSequence = true;
                l_i = p_uStartIdx;

                while( (l_i <= p_uEndIdx )  && ( true == l_bIsValidSequence ) && ( e_eFSS_BLOB_RES_OK == l_eRes ) )
                {
                    /* Read a page, verify if it's valid and digest the CRC of the related BLOB */
                    l_eHLRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, l_i,  p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                            &l_tPagePrm, p_ptCtx->uReTry);
                    l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                    {
                        /* Verify if page is correct */
                        l_eHLRes = eFSS_UTILSHLPRV_IsValidPageInBuff(p_ptCtx->ptCtxCb, p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                                     &l_bIsValidPage);
                        l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
                        if( e_eFSS_BLOB_RES_OK == l_eRes )
                        {
                            if( true == l_bIsValidPage)
                            {
                                if( p_uStartIdx == l_i )
                                {
                                    /* First run, save seq number  */
                                    l_tPagePrmFirst = l_tPagePrm;

                                    /* Calc CRC of whole data starting from here */
                                    l_eHLRes = eFSS_UTILSHLPRV_CrcDigest(p_ptCtx->ptCtxCb, MAX_UINT32VAL, p_ptCtx->puBuff1 , p_ptCtx->uBuff1L - EFSS_PAGEMETASIZE, &l_uDigestCrc32);
                                    l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
                                }
                                else
                                {
                                    if( ( l_tPagePrm.uPageSequentialN != l_tPagePrmFirst.uPageSequentialN ) ||
                                        ( l_tPagePrm.uPageUseSpecific1 != l_tPagePrmFirst.uPageUseSpecific1 ) ||
                                        ( l_tPagePrm.uPageUseSpecific2 != l_tPagePrmFirst.uPageUseSpecific2 ) )
                                    {
                                        /* Not valid */
                                        l_bIsValidSequence = false;
                                    }
                                    else
                                    {
                                        /* Calculate the whole CRC */
                                        l_eHLRes = eFSS_UTILSHLPRV_CrcDigest(p_ptCtx->ptCtxCb, l_uDigestCrc32, p_ptCtx->puBuff1 , p_ptCtx->uBuff1L - EFSS_PAGEMETASIZE, &l_uDigestCrc32);
                                        l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
                                    }
                                }
                            }
                            else
                            {
                                l_bIsValidSequence = false;
                            }
                        }
                    }

                    l_i++;
                }

                if( e_eFSS_BLOB_RES_OK == l_eRes )
                {
                    /* No error found */
                    if( true == l_bIsValidSequence )
                    {
                        /* Verify if the stored crc of the blob is equals to the calculated one */
                        if( l_tPagePrmFirst.uPageUseSpecific2 == l_uDigestCrc32 )
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
            }
        }
	}

	return l_eRes;

}






























