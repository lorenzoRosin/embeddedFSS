/**
 * @file       eFSS_DBFL.h
 *
 * @brief      Database of fixed length data module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_DBFL.h"


/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_DBFL_IsStatusStillCoherent(const t_eFSS_DBFL_Ctx* p_ptCtx);
static e_eFSS_DBFL_RES eFSS_DBFL_DBCtoDBFLRes(const e_eFSS_DBC_RES p_eDBCRes);
static t_eFSS_DBFL_DbElement eFSS_DBFL_GetElementInBuffer(const uint32_t p_uIndex, uint8_t* p_puBuffer);
static bool_t eFSS_DBFL_SetElementInBuffer(const uint32_t p_uIndex, uint8_t* p_puBuffer, t_eFSS_DBFL_DbElement p_tElem);


/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_DBFL_RES eFSS_DBFL_InitCtx(t_eFSS_DBFL_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx const p_tCtxCb,
                                  t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL,
                                  t_eFSS_TYPE_CbDeSerCtx const p_tSerDeseCb, t_eFSS_DBFL_DbStruct p_tDbStruct)
{
    e_eFSS_DBFL_RES l_eRes;
    e_eFSS_DBC_RES  l_eDBCRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_DBFL_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_tSerDeseCb.fSerial ) || ( NULL == p_tSerDeseCb.fDeserial ) || 
            ( NULL == p_tDbStruct.ptElementList ) )
        {
            l_eRes = e_eFSS_DBFL_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( ( p_tDbStruct.uNumberOfElement <= 0u ) || ( p_tDbStruct.uSerialEleSize <= 0u ) )
            {
                l_eRes = e_eFSS_DBFL_RES_BADPARAM;
            }
            else
            {
                /* Can init low level context */
                l_eDBCRes = eFSS_DBC_InitCtx(&p_ptCtx->tDbCtx, p_tCtxCb, p_tStorSet, p_puBuff, p_uBuffL);
                l_eRes = eFSS_DBFL_DBCtoDBFLRes(l_eDBCRes);

                if( e_eFSS_DBFL_RES_OK == l_eRes )
                {
                    /* Fill context */
                    p_ptCtx->tDBS = p_tDbStruct;
                    p_ptCtx->tSeDeserCtx = p_tSerDeseCb;
                }
            }
        }
    }

    return l_eRes;
}

e_eFSS_DBFL_RES eFSS_DBFL_IsInit(t_eFSS_DBFL_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
    e_eFSS_DBFL_RES l_eRes;
    e_eFSS_DBC_RES  l_eDBCRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSS_DBC_RES_BADPOINTER;
	}
	else
	{
        l_eDBCRes = eFSS_DBC_IsInit(&p_ptCtx->tDbCtx, p_pbIsInit);
        l_eRes = eFSS_DBFL_DBCtoDBFLRes(l_eDBCRes);
	}

	return l_eRes;
}

e_eFSS_DBFL_RES eFSS_DBFL_GetDBStatus(t_eFSS_DBFL_Ctx* const p_ptCtx)
{
	/* Local return variable */
    e_eFSS_DBFL_RES l_eRes;
    e_eFSS_DBC_RES  l_eDBCRes;

    /* Local variable for calculation */
    t_eFSS_TYPE_StorBuf l_tBuff;
    t_eFSS_TYPE_StorSet l_tStorSet;
    bool_t l_bIsInit;
    uint32_t l_uIndexToCheck;
    uint32_t l_uMaxIndex;
    uint32_t l_uMaxElemPageIndex;
    uint32_t l_uElemDone;
    uint32_t l_uElemInPageDone;
    t_eFSS_DBFL_DbElement l_tCurrElem;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_DBFL_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eDBCRes = eFSS_DBC_IsInit(&p_ptCtx->tDbCtx, &l_bIsInit);
        l_eRes = eFSS_DBFL_DBCtoDBFLRes(l_eDBCRes);

        if( e_eFSS_DBFL_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_DBFL_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_DBFL_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_DBFL_RES_CORRUPTCTX;
                }
                else
                {
                    /* Get storage info */
                    l_eDBCRes = eFSS_DBC_GetBuffNStor(&p_ptCtx->tDbCtx, &l_tBuff, &l_tStorSet);
                    l_eRes = eFSS_DBFL_DBCtoDBFLRes(l_eDBCRes);

                    if( e_eFSS_DBFL_RES_OK == l_eRes )
                    {
                        /* Check every parameter against DB struct now */
                        l_uIndexToCheck = 0u;
                        l_uElemDone = 0u;
                        l_uMaxIndex = ( l_tStorSet.uTotPages / 2u ) ;
                        l_uMaxElemPageIndex = (uint32_t)( l_tBuff.uBufL / p_ptCtx->tDBS.uSerialEleSize ) ;

                        while( ( ( e_eFSS_DBFL_RES_OK == l_eRes ) || ( e_eFSS_DBFL_RES_OK_BKP_RCVRD == l_eRes ) ) || 
                               ( l_uIndexToCheck < l_uMaxIndex ) )
                        {
                            l_eDBCRes = eFSS_DBC_LoadPageInBuff(&p_ptCtx->tDbCtx, l_uIndexToCheck);
                            l_eRes = eFSS_DBFL_DBCtoDBFLRes(l_eDBCRes);

                            if( ( e_eFSS_DBFL_RES_OK == l_eRes ) || ( e_eFSS_DBFL_RES_OK_BKP_RCVRD == l_eRes ) )
                            {
                                /* Check against DB default */
                                l_uElemInPageDone = 0u;

                                while( ( l_uElemInPageDone < l_uMaxElemPageIndex ) )
                                {
                                    l_tCurrElem = eFSS_DBFL_GetElementInBuffer(l_uElemInPageDone, l_tBuff.puBuf);

                                    /* Verify version */
                                    if( l_tCurrElem.uVer != p_ptCtx->tDBS.ptElementList[l_uElemDone].uVer )
                                    {
                                        /* Update version, ripristinate default */
                                        if( false == eFSS_DBFL_SetElementInBuffer(l_uElemInPageDone, l_tBuff.puBuf, p_ptCtx->tDBS.ptElementList[l_uElemDone]) )
                                        {
                                            l_eRes = e_eFSS_DBFL_RES_BADPARAM;
                                        }
                                    }

                                    l_uElemDone++;
                                    l_uElemInPageDone++;
                                }

                                if(true == pageModified)
                                {
                                    eFSS_DBC_FlushBuffInPage(&p_ptCtx->tDbCtx, l_uIndexToCheck);
                                }

                            }

                            l_uIndexToCheck++;
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_DBFL_RES eFSS_DBFL_Format(t_eFSS_DBFL_Ctx* const p_ptCtx)
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

static t_eFSS_DBFL_DbElement eFSS_DBFL_GetElementInBuffer(const uint32_t p_uIndex, uint8_t* p_puBuffer)
{

}

static bool_t eFSS_DBFL_SetElementInBuffer(const uint32_t p_uIndex, uint8_t* p_puBuffer, t_eFSS_DBFL_DbElement p_tElem)
{

}