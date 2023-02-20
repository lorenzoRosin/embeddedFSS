/**
 * @file       eFSS_DBFL.h
 *
 * @brief      Database of fixed length data module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/


/* In this module the page field has the following meaning:
 * ------------------------------------------------------------------ User data
 * - [uint8_t] -                    -> N byte of user data           |
 * ------------------------------------------------------------------ Metadata  (17 byte)
 * - uint32_t  - uPageUseSpec1      -> Parameter length              |
 * - uint32_t  - uPageUseSpec2      -> No value defined              |
 * - uint32_t  - uPageUseSpec3      -> No value defined              |
 * ------------------------------------------------------------------+
 */

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_DBFL.h"


/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_DBFL_IsStatusStillCoherent(const t_eFSS_DBFL_Ctx* p_ptCtx);
static e_eFSS_DBFL_RES eFSS_DBFL_DBCtoDBFLRes(const e_eFSS_DBC_RES p_eDBCRes);
static void eFSS_DBFL_GetElementInBuffer(const uint32_t p_uEleL, const uint32_t p_uIndexInPage, uint8_t* p_puBuffer, 
                                        f_eFSS_TYPE_DeserialCb p_fDeSerial, t_eFSS_DBFL_DbElement* p_tElem);
static void eFSS_DBFL_SetElementInBuffer(const uint32_t p_uEleL, const uint32_t p_uIndexInPage, uint8_t* p_puBuffer, 
                                           f_eFSS_TYPE_SerrialzCb p_fSerial, t_eFSS_DBFL_DbElement* p_tElem);
static void eFSS_DBFL_GetPageAndPagePosition(const uint32_t p_uPageL, const uint32_t p_uEleL, const uint32_t p_uIndex, 
                                             uint32_t* p_puPageIdx, uint32_t* p_puLogPos);



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
                        /* Copy DB struct now */
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
                                    /* Update version, ripristinate default */
                                    if( false == eFSS_DBFL_SetElementInBuffer(l_uElemInPageDone, l_tBuff.puBuf, p_ptCtx->tDBS.ptElementList[l_uElemDone]) )
                                    {
                                        l_eRes = e_eFSS_DBFL_RES_BADPARAM;
                                    }

                                    l_uElemDone++;
                                    l_uElemInPageDone++;
                                }

                                l_eDBCRes = eFSS_DBC_FlushBuffInPage(&p_ptCtx->tDbCtx, l_uIndexToCheck);
                                l_eRes = eFSS_DBFL_DBCtoDBFLRes(l_eDBCRes);

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

e_eFSS_DBFL_RES eFSS_DBFL_SaveElemen(t_eFSS_DBFL_Ctx* const p_ptCtx, uint32_t p_uPos,
                                     t_eFSS_TYPE_SingleDbElement* p_ptElem)
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

    uint32_t p_puPageIdx;
    uint32_t p_puPagePos;

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
                        /* Where */
                        eFSS_DBFL_GetPageAndPagePosition(p_uPos, &p_puPageIdx, &p_puPagePos);

                        l_eDBCRes = eFSS_DBC_LoadPageInBuff(&p_ptCtx->tDbCtx, p_puPageIdx);
                        l_eRes = eFSS_DBFL_DBCtoDBFLRes(l_eDBCRes);

                        if( ( e_eFSS_DBFL_RES_OK == l_eRes ) || ( e_eFSS_DBFL_RES_OK_BKP_RCVRD == l_eRes ) )
                        {
                            if( false == eFSS_DBFL_SetElementInBuffer(p_puPagePos, l_tBuff.puBuf, p_ptElem) )
                            {
                                l_eRes = e_eFSS_DBFL_RES_BADPARAM;
                            }

                            l_eDBCRes = eFSS_DBC_FlushBuffInPage(&p_ptCtx->tDbCtx, l_uIndexToCheck);
                            l_eRes = eFSS_DBFL_DBCtoDBFLRes(l_eDBCRes);
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_DBFL_RES eFSS_DBFL_GetElement(t_eFSS_DBFL_Ctx* const p_ptCtx, uint32_t p_uPos,
                                     t_eFSS_TYPE_SingleDbElement* p_ptElem)
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

    uint32_t p_puPageIdx;
    uint32_t p_puPagePos;

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
                        /* Where */
                        eFSS_DBFL_GetPageAndPagePosition(p_uPos, &p_puPageIdx, &p_puPagePos);

                        l_eDBCRes = eFSS_DBC_LoadPageInBuff(&p_ptCtx->tDbCtx, p_puPageIdx);
                        l_eRes = eFSS_DBFL_DBCtoDBFLRes(l_eDBCRes);

                        if( ( e_eFSS_DBFL_RES_OK == l_eRes ) || ( e_eFSS_DBFL_RES_OK_BKP_RCVRD == l_eRes ) )
                        {
                            *p_ptElem = eFSS_DBFL_GetElementInBuffer(p_puPagePos, l_tBuff.puBuf, p_ptElem);

                            l_eDBCRes = eFSS_DBC_FlushBuffInPage(&p_ptCtx->tDbCtx, l_uIndexToCheck);
                            l_eRes = eFSS_DBFL_DBCtoDBFLRes(l_eDBCRes);
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
static bool_t eFSS_DB_IsStatusStillCoherent(const t_eFSS_DBFL_Ctx* p_ptCtx)
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

static e_eFSS_DBFL_RES eFSS_DBFL_DBCtoDBFLRes(const e_eFSS_DBC_RES p_eDBCRes)
{
    e_eFSS_DBFL_RES l_eRes;

    switch(p_eDBCRes)
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

static void eFSS_DBFL_GetElementInBuffer(const uint32_t p_uEleL, const uint32_t p_uIndexInPage, uint8_t* p_puBuffer, 
                                        f_eFSS_TYPE_DeserialCb p_fDeSerial, t_eFSS_DBFL_DbElement* p_tElem)
{
    /* Local variable */
    uint32_t l_uElemPos;
    uint32_t l_uCurroffset;
    uint32_t l_uTemp;

    /* Calc position */
    l_uElemPos = p_uEleL * p_uIndexInPage;

    /* First, copy the element version in Little endian */
    l_uCurroffset = 0u;
    p_tElem->uVer = 0u;
    l_uTemp = (uint16_t) p_puBuffer[l_uElemPos+l_uCurroffset];
    p_tElem->uVer |= ( l_uTemp & 0x00FFu );
    l_uCurroffset++;

    l_uTemp =  (uint16_t) ( ( (uint16_t) p_puBuffer[l_uElemPos+l_uCurroffset] ) << 8u  );
    p_tElem->uVer |= ( l_uTemp & 0xFF00u );
    l_uCurroffset++;

    /* Now insert resialed data from the user */
    (*p_fDeSerial)(l_uElemPos, p_uEleL, p_tElem->ptDefVal, &p_puBuffer[l_uElemPos+l_uCurroffset]);
}

static void eFSS_DBFL_SetElementInBuffer(const uint32_t p_uEleL, const uint32_t p_uIndexInPage, uint8_t* p_puBuffer, 
                                           f_eFSS_TYPE_SerrialzCb p_fSerial, t_eFSS_DBFL_DbElement* p_tElem)
{
    /* Local variable */
    uint32_t l_uElemPos;
    uint32_t l_uCurroffset;

    /* Calc position */
    l_uElemPos = p_uEleL * p_uIndexInPage;

    /* First, insert the element version in Little endian */
    l_uCurroffset = 0u;
    p_puBuffer[l_uElemPos+l_uCurroffset] = (uint8_t) ( ( p_tElem->uVer        ) & 0x00FFu );
    l_uCurroffset++;

    p_puBuffer[l_uElemPos+l_uCurroffset] = (uint8_t) ( ( p_tElem->uVer  >> 8u  ) & 0x00FFu );
    l_uCurroffset++;

    /* Now insert resialed data from the user */
    (*p_fSerial)(l_uElemPos, p_uEleL, p_tElem->ptDefVal, &p_puBuffer[l_uElemPos+l_uCurroffset]);
}

static void eFSS_DBFL_GetPageAndPagePosition(const uint32_t p_uPageL, const uint32_t p_uEleL, const uint32_t p_uIndex, 
                                             uint32_t* p_puPageIdx, uint32_t* p_puLogPos)
{
    /* Local variable */
    uint32_t l_uLogPerPage;
    uint32_t l_uPagePosition;
    uint32_t l_uLogPositionInPage;

    /* Calc how many log we cat store in a page */
    l_uLogPerPage = (uint32_t)( p_uPageL / p_uEleL );

    /* Calculate page offset */
    l_uPagePosition = (uint32_t)( p_uIndex / l_uLogPerPage );

    /* Calculate log offset in page */
    l_uLogPositionInPage = (uint32_t)( p_uIndex % l_uLogPerPage );

    /* Return */
    *p_puPageIdx = l_uPagePosition;
    *p_puLogPos  = l_uLogPositionInPage;

}