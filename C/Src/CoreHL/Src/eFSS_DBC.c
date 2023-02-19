/**
 * @file       eFSS_DB.c
 *
 * @brief      Database module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_DBC.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_DB_IsStatusStillCoherent(const t_eFSS_DB_Ctx* p_ptCtx);
static e_eFSS_DB_RES eFSS_DB_HLtoDBRes(const e_eFSS_UTILSHLPRV_RES p_eHLRes);
static e_eFSS_DB_RES eFSS_DB_RetrivePageAndOffset(const t_eFSS_DB_Ctx* p_ptCtx, uint32_t p_Index, uint32_t* p_puPage,
                                                  uint32_t* p_puOffset);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_DB_RES eFSS_DB_InitCtx(t_eFSS_DB_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx* const p_ptCtxCb,
                                  const uint32_t p_uPageToUse, const uint32_t p_uPageSize, uint8_t* const p_puBuff,
                                  uint32_t p_uBuffL, t_eFSS_DB_DbStruct p_tDBS)
{
    e_eFSS_DB_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ptCtxCb ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_DB_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_ptCtxCb->ptCtxErase ) || ( NULL == p_ptCtxCb->fErase ) ||
            ( NULL == p_ptCtxCb->ptCtxWrite ) || ( NULL == p_ptCtxCb->fWrite ) ||
            ( NULL == p_ptCtxCb->ptCtxRead  ) || ( NULL == p_ptCtxCb->fRead  ) ||
            ( NULL == p_ptCtxCb->ptCtxCrc32 ) || ( NULL == p_ptCtxCb->fCrc32 ) )
        {
            l_eRes = e_eFSS_DB_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( ( p_uPageToUse <= 2u ) || ( p_uPageSize <= EFSS_PAGEMETASIZE ) || ( ( p_uPageSize * 2u ) != p_uBuffL ) )
            {
                l_eRes = e_eFSS_DB_RES_BADPARAM;
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
            }
        }
    }

    return l_eRes;
}


e_eFSS_DB_RES eFSS_DB_IsInit(t_eFSS_DB_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eFSS_DB_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSS_DB_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInit;
        l_eRes = e_eFSS_DB_RES_OK;
	}

	return l_eRes;
}

e_eFSS_DB_RES eFSS_DB_GetStorageStatus(t_eFSS_DB_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx* const p_peStatus)
{
	/* Local variable */
	e_eFSS_DB_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;
    uint32_t l_i;
    t_eFSS_TYPE_PageMeta p_ptPagePrm;
    uint32_t l_uCntStr;
    uint32_t l_uPgCntr;
    uint16_t l_uCurrElLen;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_DB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eFSS_DB_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eFSS_DB_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eFSS_DB_RES_CORRUPTCTX;
            }
            else
            {
                /* Verify integrity of every page and every backup */
                l_eRes = e_eFSS_DB_RES_OK;
                l_i = 0u;

                while( (l_i < p_ptCtx->uNPage / 2u)  && ( (e_eFSS_DB_RES_OK == l_eRes) || (e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes)  ) )
                {
                    l_eHLRes = eFSS_UTILSHLPRV_VerifyNRipristBkup( p_ptCtx->ptCtxCb, p_ptCtx->uReTry,
                                                                   p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                                   p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                                   l_i, l_i +  (p_ptCtx->uNPage / 2u) );
                    l_eRes = eFSS_DB_HLtoDBRes(l_eHLRes);
                }

                /* Al page backup checked */
                if( e_eFSS_DB_RES_OK == l_eRes )
                {
                    /* Now we need to check every parameter length */
                    l_i = 0u;
                    l_uCntStr = 0u;
                    l_uPgCntr = 0u;

                    while( (l_i < p_ptCtx->uNPage / 2u)  && ( (e_eFSS_DB_RES_OK == l_eRes) ) )
                    {
                        l_eHLRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, l_i,  p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                            &p_ptPagePrm, p_ptCtx->uReTry);
                        l_eRes = eFSS_DB_HLtoDBRes(l_eHLRes);

                        if( e_eFSS_DB_RES_OK == l_eRes )
                        {
                            while( ( l_uCntStr < p_ptCtx->tDBS.uNumberOfElement ) && ( l_uPgCntr <= p_ptCtx->uPageSize - EFSS_PAGEMETASIZE ) )
                            {
                                l_uCurrElLen = p_ptCtx->puBuff1[l_uPgCntr];
                                if( l_uCurrElLen == p_ptCtx->tDBS.ptElementList[l_uCntStr].uLen  )
                                {
                                    /* Same element lenght top */
                                }
                                else if( 0u == l_uCurrElLen )
                                {
                                    /* Different because is new */

                                }
                                else
                                {
                                    /* Database modified need to format all */

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


e_eFSS_DB_RES eFSS_DB_Format(t_eFSS_DB_Ctx* const p_ptCtx)
{
	/* Local variable */
	e_eFSS_DB_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;
    uint32_t l_uCrcZeroBlob;
    uint32_t l_i;
    t_eFSS_TYPE_PageMeta l_tPagePrmToWrite;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_DB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eFSS_DB_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eFSS_DB_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eFSS_DB_RES_CORRUPTCTX;
            }
            else
            {
                /* Calculate the CRC of the zero data blob */
                l_uCrcZeroBlob = 10u;

                /* Format to zero everything */

                /* First format the original pages, update blob size and CRC value  */
                l_eRes = e_eFSS_DB_RES_OK;
                l_i = 0u;

                while( (l_i < p_ptCtx->uNPage / 2u)  && ( e_eFSS_DB_RES_OK == l_eRes ) )
                {
                    memset(p_ptCtx->puBuff1, 0u, p_ptCtx->uBuff1L);
                    l_tPagePrmToWrite.uPageType = 0u;
                    l_tPagePrmToWrite.uPageSubType = 0u;
                    l_tPagePrmToWrite.uPageVersion = 0u;
                    l_tPagePrmToWrite.uPageUseSpecific1 = 0u;
                    l_tPagePrmToWrite.uPageUseSpecific2 = l_uCrcZeroBlob;
                    l_tPagePrmToWrite.uPageUseSpecific3 = 0u;
                    l_tPagePrmToWrite.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
                    l_tPagePrmToWrite.uPageCrc = 0u;

                    l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->ptCtxCb, l_i,
                                                         p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                         p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                         &l_tPagePrmToWrite, p_ptCtx->uReTry);
                    l_eRes = eFSS_DB_HLtoDBRes(l_eHLRes);

                }

                /* Now format the backup pages, update blob size and CRC value  */
                if( e_eFSS_DB_RES_OK == l_eRes )
                {
                    l_i = p_ptCtx->uNPage / 2u;

                    while( (l_i < p_ptCtx->uNPage)  && ( e_eFSS_DB_RES_OK == l_eRes ) )
                    {
                        memset(p_ptCtx->puBuff1, 0u, p_ptCtx->uBuff1L);
                        l_tPagePrmToWrite.uPageType = 0u;
                        l_tPagePrmToWrite.uPageSubType = 0u;
                        l_tPagePrmToWrite.uPageVersion = 0u;
                        l_tPagePrmToWrite.uPageUseSpecific1 = 0u;
                        l_tPagePrmToWrite.uPageUseSpecific2 = l_uCrcZeroBlob;
                        l_tPagePrmToWrite.uPageUseSpecific3 = 0u;
                        l_tPagePrmToWrite.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
                        l_tPagePrmToWrite.uPageCrc = 0u;

                        l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->ptCtxCb, l_i,
                                                             p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                             p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                             &l_tPagePrmToWrite, p_ptCtx->uReTry);
                        l_eRes = eFSS_DB_HLtoDBRes(l_eHLRes);

                    }
                }
            }
		}
	}

	return l_eRes;
}


e_eFSS_DB_RES eFSS_DB_SaveElemen(t_eFSS_DB_Ctx* const p_ptCtx, uint32_t p_uPos, uint8_t* p_puBuff, uint32_t p_uBuffL)
{
	/* Local variable */
	e_eFSS_DB_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;
    uint32_t l_i;
    uint32_t l_uWritten;
    uint32_t l_uPageToCheck;
    uint32_t l_uOffsetToCheck;
    t_eFSS_TYPE_PageMeta p_ptPagePrm;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_DB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eFSS_DB_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eFSS_DB_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eFSS_DB_RES_CORRUPTCTX;
            }
            else
            {
                /* Check data validity */
                if( p_uBuffL <= 0u )
                {
                    l_eRes = e_eFSS_DB_RES_BADPARAM;
                }
                else
                {
                    /* Get element and offset */
                    l_eRes =  eFSS_DB_RetrivePageAndOffset(p_ptCtx, p_uPos, &l_uPageToCheck, &l_uOffsetToCheck);

                    if( e_eFSS_DB_RES_OK == l_eRes )
                    {
                        /* Ovviamente */
                        l_eHLRes = eFSS_UTILSHLPRV_VerifyNRipristBkup( p_ptCtx->ptCtxCb, p_ptCtx->uReTry,
                                                                    p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                                    p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                                    l_i, l_i +  (p_ptCtx->uNPage / 2u) );
                        l_eRes = eFSS_DB_HLtoDBRes(l_eHLRes);

                        if( ( e_eFSS_DB_RES_OK == l_eRes ) || ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) )
                        {
                            l_eHLRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, l_i,  p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                                &p_ptPagePrm, p_ptCtx->uReTry);
                            l_eRes = eFSS_DB_HLtoDBRes(l_eHLRes);

                            if( e_eFSS_DB_RES_OK == l_eRes )
                            {
                                memcpy(&p_ptCtx->puBuff1[l_uOffsetToCheck], p_puBuff, p_uBuffL);

                                /* Now write the page origin */
                                l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->ptCtxCb, l_i,
                                                                    p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                                    p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                                    &p_ptPagePrm, p_ptCtx->uReTry);
                                l_eRes = eFSS_DB_HLtoDBRes(l_eHLRes);

                                /* Now write the page backup */
                                l_eHLRes =  eFSS_UTILSHLPRV_WritePagePrmNUpCrc(p_ptCtx->ptCtxCb, l_i,
                                                                    p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                                    p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                                    &p_ptPagePrm, p_ptCtx->uReTry);
                                l_eRes = eFSS_DB_HLtoDBRes(l_eHLRes);
                            }
                        }
                    }
                }
            }
		}
	}

	return l_eRes;
}


e_eFSS_DB_RES eFSS_DB_GetElement(t_eFSS_DB_Ctx* const p_ptCtx, uint32_t p_uPos, uint8_t* p_puBuff, uint8_t* p_puBuffL)
{
	/* Local variable */
	e_eFSS_DB_RES l_eRes;
    e_eFSS_UTILSHLPRV_RES l_eHLRes;
    uint32_t l_i;
    uint32_t l_uWritten;
    uint32_t l_uPageToCheck;
    uint32_t l_uOffsetToCheck;
    t_eFSS_TYPE_PageMeta p_ptPagePrm;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_DB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eFSS_DB_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eFSS_DB_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eFSS_DB_RES_CORRUPTCTX;
            }
            else
            {
                /* Get element and offset */
                l_eRes =  eFSS_DB_RetrivePageAndOffset(p_ptCtx, p_uPos, &l_uPageToCheck, &l_uOffsetToCheck);

                if( e_eFSS_DB_RES_OK == l_eRes )
                {
                    /* Ovviamente */
                    l_eHLRes = eFSS_UTILSHLPRV_VerifyNRipristBkup( p_ptCtx->ptCtxCb, p_ptCtx->uReTry,
                                                                p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                                p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                                l_i, l_i +  (p_ptCtx->uNPage / 2u) );
                    l_eRes = eFSS_DB_HLtoDBRes(l_eHLRes);

                    if( ( e_eFSS_DB_RES_OK == l_eRes ) || ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) )
                    {
                        l_eHLRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->ptCtxCb, l_i,  p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                            &p_ptPagePrm, p_ptCtx->uReTry);
                        l_eRes = eFSS_DB_HLtoDBRes(l_eHLRes);

                        if( e_eFSS_DB_RES_OK == l_eRes )
                        {
                            memcpy(p_puBuff, &p_ptCtx->puBuff1[l_uOffsetToCheck], 10u);
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
static bool_t eFSS_DB_IsStatusStillCoherent(const t_eFSS_DB_Ctx* p_ptCtx)
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

static e_eFSS_DB_RES eFSS_DB_HLtoDBRes(const e_eFSS_UTILSHLPRV_RES p_eHLRes)
{
    e_eFSS_DB_RES l_eRes;

    switch(p_eHLRes)
    {
        case e_eFSS_UTILSHLPRV_RES_OK:
        {
            l_eRes = e_eFSS_DB_RES_OK;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_NOTVALIDPAGE:
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_OK_BKP_RCVRD:
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_BADPARAM:
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_BADPOINTER:
        {
            l_eRes = e_eFSS_DB_RES_BADPOINTER;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKERASEERR:
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKWRITEERR:
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKREADERR:
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CLBCKCRCERR:
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_WRITENOMATCHREAD:
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSHLPRV_RES_CORRUPT:
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
            break;
        }

        default:
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
            break;
        }
    }

    return l_eRes;
}

static e_eFSS_DB_RES eFSS_DB_RetrivePageAndOffset(const t_eFSS_DB_Ctx* p_ptCtx, uint32_t p_Index, uint32_t* p_puPage,
                                                  uint32_t* p_puOffset)
{


}