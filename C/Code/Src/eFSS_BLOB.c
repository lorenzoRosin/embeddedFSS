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
static e_eFSS_BLOB_RES eFSS_BLOB_AreOriValid(const t_eFSS_BLOB_Ctx* p_ptCtx, bool_t* p_pbAreValid, t_eFSS_TYPE_PageMeta* p_ptMeta);
static e_eFSS_BLOB_RES eFSS_BLOB_AreBckUpValid(const t_eFSS_BLOB_Ctx* p_ptCtx, bool_t* p_pbAreValid, t_eFSS_TYPE_PageMeta* p_ptMeta);
static e_eFSS_BLOB_RES eFSS_BLOB_OriginBackupAligner(const t_eFSS_BLOB_Ctx* p_ptCtx);



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
                l_uCrcZeroBlob = 10u;

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
                        /* First format the original pages, update blob size and CRC value  */
                        l_i = 0u;
                        l_uWritten = 0u;

                        /* Read all pages */
                        while( (l_i < p_ptCtx->uNPage / 2u)  && ( e_eFSS_BLOB_RES_OK == l_eRes ) && ( l_uWritten < p_uBuffL) && ( l_uWritten < p_uBuffL) )
                        {
                            l_eHLRes = eFSS_UTILSHLPRV_WritePageNUpCrc(p_ptCtx->ptCtxCb, l_i,  p_ptCtx->puBuff1, p_ptCtx->uBuff1L, p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                                       p_ptCtx->uReTry);
                            l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHLRes);
                            if( e_eFSS_BLOB_RES_OK != l_eRes )
                            {
                                memcpy(&p_puBuff[l_uWritten], p_ptCtx->puBuff1, p_ptCtx->uPageSize - EFSS_PAGEMETASIZE );
                                l_uWritten += p_ptCtx->uPageSize - EFSS_PAGEMETASIZE;
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
	if( (  NULL == p_ptCtx->ptCtxCb ) || ( NULL == p_ptCtx->puBuff1 ) )
	{
		l_eRes = false;
	}
	else
	{
        l_eRes = true;
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
            l_eRes = e_eFSS_BLOB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_OK_BKP_RCVRD:
        {
            l_eRes = e_eFSS_BLOB_RES_BADPARAM;
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
            l_eRes = e_eFSS_BLOB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKWRITEERR:
        {
            l_eRes = e_eFSS_BLOB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKREADERR:
        {
            l_eRes = e_eFSS_BLOB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKCRCERR:
        {
            l_eRes = e_eFSS_BLOB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_WRITENOMATCHREAD:
        {
            l_eRes = e_eFSS_BLOB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CORRUPT:
        {
            l_eRes = e_eFSS_BLOB_RES_BADPARAM;
            break;
        }

        default:
        {
            l_eRes = e_eFSS_BLOB_RES_BADPARAM;
            break;
        }
    }

    return l_eRes;
}

static e_eFSS_BLOB_RES eFSS_BLOB_AreOriValid(const t_eFSS_BLOB_Ctx* p_ptCtx, bool_t* p_pbAreValid, t_eFSS_TYPE_PageMeta* p_ptMeta)
{
	/* Local variable return */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHlRes;

    /* Local variable Cycle data */
    uint32_t l_i;
    t_eFSS_TYPE_PageMeta p_ptPagePrm;
    t_eFSS_TYPE_PageMeta p_ptPagePrmFirst;
    bool_t l_pbIsValidPage;
    bool_t l_pbIsValidSequence;
    uint32_t l_uDigestCrc32;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbAreValid ) || ( NULL == p_ptMeta ) )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
        /* Init variable */
        l_eRes = e_eFSS_BLOB_RES_OK;
        l_pbIsValidSequence = true;
        l_i = 0u;

        while( (l_i < p_ptCtx->uNPage / 2u)  && ( true == l_pbIsValidSequence ) )
        {
            /* Read a page */
            l_eHlRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, l_i,  p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                    &p_ptPagePrm, p_ptCtx->uReTry);
            l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHlRes);
            if( e_eFSS_BLOB_RES_OK == l_eRes )
            {
                /* Verify if correct */
                l_eHlRes = eFSS_UTILSHLPRV_IsValidPageInBuff(p_ptCtx->ptCtxCb, p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                             &l_pbIsValidPage);
                l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHlRes);
                if( e_eFSS_BLOB_RES_OK == l_eRes )
                {
                    if( true == l_pbIsValidPage)
                    {
                        if( 0u == l_i )
                        {
                            /* First run, save seq number  */
                            p_ptPagePrmFirst = p_ptPagePrm;

                            /* Calc CRC of whole data starting from here */
                            l_eHlRes = eFSS_UTILSHLPRV_CrcDigest(p_ptCtx->ptCtxCb, MAX_UINT32VAL, p_ptCtx->puBuff1 , p_ptCtx->uBuff1L - EFSS_PAGEMETASIZE, &l_uDigestCrc32);
                            l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHlRes);
                            if( e_eFSS_BLOB_RES_OK != l_eRes )
                            {
                                l_pbIsValidSequence = false;
                            }
                        }
                        else
                        {
                            if( ( p_ptPagePrm.uPageSequentialN != p_ptPagePrmFirst.uPageSequentialN ) ||
                                ( p_ptPagePrm.uPageUseSpecific1 != p_ptPagePrmFirst.uPageUseSpecific1 ) ||
                                ( p_ptPagePrm.uPageUseSpecific2 != p_ptPagePrmFirst.uPageUseSpecific2 ) )
                            {
                                /* Not valid */
                                l_pbIsValidSequence = false;
                            }
                            else
                            {
                                /* Calculate the whole CRC */
                                l_eHlRes = eFSS_UTILSHLPRV_CrcDigest(p_ptCtx->ptCtxCb, l_uDigestCrc32, p_ptCtx->puBuff1 , p_ptCtx->uBuff1L - EFSS_PAGEMETASIZE, &l_uDigestCrc32);
                                l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHlRes);
                                if( e_eFSS_BLOB_RES_OK != l_eRes )
                                {
                                    l_pbIsValidSequence = false;
                                }
                            }
                        }
                    }
                    else
                    {
                        l_pbIsValidSequence = false;
                    }
                }
                else
                {
                    l_pbIsValidSequence = false;
                }
            }
            else
            {
                l_pbIsValidSequence = false;
            }

            l_i++;
        }

        if( true == l_pbIsValidSequence )
        {
            if( p_ptPagePrmFirst.uPageUseSpecific2 == l_uDigestCrc32 )
            {
                *p_pbAreValid = true;
                *p_ptMeta = p_ptPagePrmFirst;
            }
            else
            {
                *p_pbAreValid = false;
            }
        }
	}

	return l_eRes;
}


static e_eFSS_BLOB_RES eFSS_BLOB_AreBckUpValid(const t_eFSS_BLOB_Ctx* p_ptCtx, bool_t* p_pbAreValid, t_eFSS_TYPE_PageMeta* p_ptMeta)
{
	/* Local variable return */
	e_eFSS_BLOB_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHlRes;

    /* Local variable Cycle data */
    uint32_t l_i;
    t_eFSS_TYPE_PageMeta p_ptPagePrm;
    t_eFSS_TYPE_PageMeta p_ptPagePrmFirst;
    bool_t l_pbIsValidPage;
    bool_t l_pbIsValidSequence;
    uint32_t l_uDigestCrc32;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbAreValid ) || ( NULL == p_ptMeta ) )
	{
		l_eRes = e_eFSS_BLOB_RES_BADPOINTER;
	}
	else
	{
        /* Init variable */
        l_eRes = e_eFSS_BLOB_RES_OK;
        l_pbIsValidSequence = true;
        l_i = 0u;

        while( (l_i < p_ptCtx->uNPage / 2u)  && ( true == l_pbIsValidSequence ) )
        {
            /* Read a page */
            l_eHlRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, l_i,  p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                    &p_ptPagePrm, p_ptCtx->uReTry);
            l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHlRes);
            if( e_eFSS_BLOB_RES_OK == l_eRes )
            {
                /* Verify if correct */
                l_eHlRes = eFSS_UTILSHLPRV_IsValidPageInBuff(p_ptCtx->ptCtxCb, p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                             &l_pbIsValidPage);
                l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHlRes);
                if( e_eFSS_BLOB_RES_OK == l_eRes )
                {
                    if( true == l_pbIsValidPage)
                    {
                        if( 0u == l_i )
                        {
                            /* First run, save seq number  */
                            p_ptPagePrmFirst = p_ptPagePrm;

                            /* Calc CRC of whole data starting from here */
                            l_eHlRes = eFSS_UTILSHLPRV_CrcDigest(p_ptCtx->ptCtxCb, MAX_UINT32VAL, p_ptCtx->puBuff1 , p_ptCtx->uBuff1L - EFSS_PAGEMETASIZE, &l_uDigestCrc32);
                            l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHlRes);
                            if( e_eFSS_BLOB_RES_OK != l_eRes )
                            {
                                l_pbIsValidSequence = false;
                            }
                        }
                        else
                        {
                            if( ( p_ptPagePrm.uPageSequentialN != p_ptPagePrmFirst.uPageSequentialN ) ||
                                ( p_ptPagePrm.uPageUseSpecific1 != p_ptPagePrmFirst.uPageUseSpecific1 ) ||
                                ( p_ptPagePrm.uPageUseSpecific2 != p_ptPagePrmFirst.uPageUseSpecific2 ) )
                            {
                                /* Not valid */
                                l_pbIsValidSequence = false;
                            }
                            else
                            {
                                /* Calculate the whole CRC */
                                l_eHlRes = eFSS_UTILSHLPRV_CrcDigest(p_ptCtx->ptCtxCb, l_uDigestCrc32, p_ptCtx->puBuff1 , p_ptCtx->uBuff1L - EFSS_PAGEMETASIZE, &l_uDigestCrc32);
                                l_eRes = eFSS_BLOB_HLtoBLOBRes(l_eHlRes);
                                if( e_eFSS_BLOB_RES_OK != l_eRes )
                                {
                                    l_pbIsValidSequence = false;
                                }
                            }
                        }
                    }
                    else
                    {
                        l_pbIsValidSequence = false;
                    }
                }
                else
                {
                    l_pbIsValidSequence = false;
                }
            }
            else
            {
                l_pbIsValidSequence = false;
            }

            l_i++;
        }

        if( true == l_pbIsValidSequence )
        {
            if( p_ptPagePrmFirst.uPageUseSpecific2 == l_uDigestCrc32 )
            {
                *p_pbAreValid = true;
                *p_ptMeta = p_ptPagePrmFirst;
            }
            else
            {
                *p_pbAreValid = false;
            }
        }
	}

	return l_eRes;
}

static e_eFSS_BLOB_RES eFSS_BLOB_OriginBackupAligner(const t_eFSS_BLOB_Ctx* p_ptCtx)
{
	/* Local variable */
	e_eFSS_BLOB_RES l_eRes;
    bool_t l_bIsOriginValid;
    t_eFSS_TYPE_PageMeta l_tMetaOrigin;
    bool_t l_bIsBackupValid;
    t_eFSS_TYPE_PageMeta l_tMetaBackUp;

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
                /* Need to check if the storage is valid or not */
                l_eRes = eFSS_BLOB_AreOriValid(p_ptCtx, &l_bIsOriginValid, &l_tMetaOrigin);

                if( e_eFSS_BLOB_RES_OK == l_eRes )
                {
                    l_eRes = eFSS_BLOB_AreBckUpValid(p_ptCtx, &l_bIsBackupValid, &l_tMetaBackUp);

                    if( e_eFSS_BLOB_RES_OK == l_eRes )
                    {
                        if( ( false == l_bIsOriginValid ) && ( false == l_bIsBackupValid ) )
                        {
                            /* No valid data */
                        }
                    }
                }
            }
		}
	}

	return l_eRes;
}