/**
 * @file       eFSS_LOG.h
 *
 * @brief      LOG of fiexd length module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_LOG.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_LOG_IsStatusStillCoherent(const t_eFSS_LOG_Ctx* p_ptCtx);
static e_eFSS_LOG_RES eFSS_LOG_DBCtoDBFLRes(const e_eFSS_LOGC_RES p_eDBCRes);
static void eFSS_LOG_GetEleRawInBuffer(const uint32_t p_uEleL, const uint32_t p_uIndexInPage, uint8_t* p_puBuffer,
                                        uint16_t* p_uVer, uint8_t** p_ppuEleRaw);
static void eFSS_LOG_SetEleInBuffer(const uint32_t p_uEleL, const uint32_t p_uIndexInPage, uint8_t* p_puBuffer,
                                     f_eFSS_TYPE_DbSerrialzCb p_fSerial, t_eFSS_LOG_DbElement* p_tElem);
static void eFSS_LOG_GetPageAndPagePosition(const uint32_t p_uPageL, const uint32_t p_uEleL, const uint32_t p_uIndex,
                                             uint32_t* p_puPageIdx, uint32_t* p_puLogPos);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_LOG_RES eFSS_LOG_InitCtx(t_eFSS_LOG_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx const p_tCtxCb,
                                    t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL,
                                    bool_t p_bFlashCache, bool_t p_bFullBckup, uint32_t p_uLogL,
                                    t_eFSS_TYPE_CbLogDeSerCtx const p_tSerDeseCb)
{
    e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES  l_eDBCRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_tSerDeseCb.fSerial ) || ( NULL == p_tSerDeseCb.fDeserial ) )
        {
            l_eRes = e_eFSS_LOG_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( ( p_uLogL <= 0u ) || ( p_uLogL > p_tStorSet.uPagesLen ) )
            {
                l_eRes = e_eFSS_LOG_RES_BADPARAM;
            }
            else
            {
                /* Can init low level context */
                l_eDBCRes = eFSS_LOGC_InitCtx(&p_ptCtx->tLOGCCtx, p_tCtxCb, p_tStorSet, p_puBuff, p_uBuffL,
                                              p_bFlashCache, p_bFullBckup);
                l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);

                if( e_eFSS_LOG_RES_OK == l_eRes )
                {
                    /* Fill context */
                    p_ptCtx->uLogL = p_uLogL;
                    p_ptCtx->tSeDeserCtx = p_tSerDeseCb;
                }
            }
        }
    }

    return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_IsInit(t_eFSS_LOG_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
    e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES  l_eDBCRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
        l_eDBCRes = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, p_pbIsInit);
        l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);
	}

	return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_GetStorageStatus(t_eFSS_LOG_Ctx* const p_ptCtx)
{
	/* Local return variable */
    e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES  l_eDBCRes;

    /* Local variable for calculation */
    t_eFSS_TYPE_StorBuf l_tBuff;
    t_eFSS_TYPE_StorSet l_tStorSet;
    bool_t l_bIsInit;
    bool_t l_bIsPageMod;
    uint32_t l_uIndexPToCheck;
    uint32_t l_uMaxPIndex;
    uint32_t l_uMaxElemPageIndex;
    uint32_t l_uElemDone;
    uint32_t l_uElemInPageDone;
    uint16_t l_uCurrEleVer;
    uint8_t* l_puCurrEleData;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eDBCRes = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, &l_bIsInit);
        l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);

        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOG_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOG_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOG_RES_CORRUPTCTX;
                }
                else
                {
                    /* Get storage info */
                    l_eDBCRes = eFSS_LOGC_GetBuffNStor(&p_ptCtx->tLOGCCtx, &l_tBuff, &l_tStorSet);
                    l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);

                    if( e_eFSS_LOG_RES_OK == l_eRes )
                    {
                        /* Check every parameter against DB struct now */
                        l_uIndexPToCheck = 0u;
                        l_uElemDone = 0u;
                        l_uMaxPIndex = (uint32_t)( l_tStorSet.uTotPages / 2u ) ;
                        l_uMaxElemPageIndex = (uint32_t)( l_tBuff.uBufL / p_ptCtx->tDBS.uRawElemL ) ;

                        while( ( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) ) ||
                               ( l_uIndexPToCheck < l_uMaxPIndex ) || ( l_uElemDone < p_ptCtx->tDBS.uNumberOfElement ) )
                        {
                            l_bIsPageMod = false;
                            l_eDBCRes = eFSS_LOGC_LoadPageInBuff(&p_ptCtx->tLOGCCtx, l_uIndexPToCheck);
                            l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);

                            if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) )
                            {
                                /* Check against DB default */
                                l_uElemInPageDone = 0u;

                                while( l_uElemInPageDone < l_uMaxElemPageIndex )
                                {
                                    eFSS_LOG_GetEleRawInBuffer(p_ptCtx->tDBS.uRawElemL, l_uElemInPageDone,
                                                                 l_tBuff.puBuf, &l_uCurrEleVer, &l_puCurrEleData);

                                    /* Verify version */
                                    if( l_uCurrEleVer != p_ptCtx->tDBS.ptElementList[l_uElemDone].uVer )
                                    {
                                        /* Update version, ripristinate default */

                                        eFSS_LOG_SetEleInBuffer(p_ptCtx->tDBS.uRawElemL, l_uElemInPageDone,
                                                                 l_tBuff.puBuf, p_ptCtx->tSeDeserCtx.fSerial,
                                                                 &p_ptCtx->tDBS.ptElementList[l_uElemDone]);
                                        l_bIsPageMod = true;
                                    }

                                    l_uElemDone++;
                                    l_uElemInPageDone++;
                                }

                                if( true == l_bIsPageMod )
                                {
                                    l_eDBCRes = eFSS_LOGC_FlushBuffInPage(&p_ptCtx->tLOGCCtx, l_uIndexPToCheck);
                                    l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);
                                }
                            }

                            l_uIndexPToCheck++;
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_GetLogInfo(t_eFSS_LOG_Ctx* const p_ptCtx, uint32_t *p_puNewLogI, uint32_t *p_puOldLogI,
                                       uint32_t *p_puNpageUsed, uint32_t *p_puNpageTot)
{

}

e_eFSS_LOG_RES eFSS_LOG_Format(t_eFSS_LOG_Ctx* const p_ptCtx)
{
	/* Local return variable */
    e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES  l_eDBCRes;

    /* Local variable for calculation */
    t_eFSS_TYPE_StorBuf l_tBuff;
    t_eFSS_TYPE_StorSet l_tStorSet;
    bool_t l_bIsInit;
    uint32_t l_uIndexPToCheck;
    uint32_t l_uMaxPIndex;
    uint32_t l_uMaxElemPageIndex;
    uint32_t l_uElemDone;
    uint32_t l_uElemInPageDone;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eDBCRes = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, &l_bIsInit);
        l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);

        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOG_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOG_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOG_RES_CORRUPTCTX;
                }
                else
                {
                    /* Get storage info */
                    l_eDBCRes = eFSS_LOGC_GetBuffNStor(&p_ptCtx->tLOGCCtx, &l_tBuff, &l_tStorSet);
                    l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);

                    if( e_eFSS_LOG_RES_OK == l_eRes )
                    {
                        /* Check every parameter against DB struct now */
                        l_uIndexPToCheck = 0u;
                        l_uElemDone = 0u;
                        l_uMaxPIndex = (uint32_t)( l_tStorSet.uTotPages / 2u ) ;
                        l_uMaxElemPageIndex = (uint32_t)( l_tBuff.uBufL / p_ptCtx->tDBS.uRawElemL ) ;

                        while( ( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) ) ||
                               ( l_uIndexPToCheck < l_uMaxPIndex ) || ( l_uElemDone < p_ptCtx->tDBS.uNumberOfElement ) )
                        {
                            l_eDBCRes = eFSS_LOGC_LoadPageInBuff(&p_ptCtx->tLOGCCtx, l_uIndexPToCheck);
                            l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);

                            if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) )
                            {
                                /* Check against DB default */
                                l_uElemInPageDone = 0u;

                                while( l_uElemInPageDone < l_uMaxElemPageIndex )
                                {
                                    /* Update version, ripristinate default */
                                    eFSS_LOG_SetEleInBuffer(p_ptCtx->tDBS.uRawElemL, l_uElemInPageDone,
                                                             l_tBuff.puBuf, p_ptCtx->tSeDeserCtx.fSerial,
                                                             &p_ptCtx->tDBS.ptElementList[l_uElemDone]);

                                    l_uElemDone++;
                                    l_uElemInPageDone++;
                                }

                                l_eDBCRes = eFSS_LOGC_FlushBuffInPage(&p_ptCtx->tLOGCCtx, l_uIndexPToCheck);
                                l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);
                            }

                            l_uIndexPToCheck++;
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_GetLogOfASpecificPage(t_eFSS_LOG_Ctx* const p_ptCtx, uint32_t p_uindx, uint8_t* p_puBuf,
                                                  uint32_t* p_puBufL, uint32_t p_uBufMaxL)
{
	/* Local return variable */
    e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES  l_eDBCRes;

    /* Local variable for calculation */
    t_eFSS_TYPE_StorBuf l_tBuff;
    t_eFSS_TYPE_StorSet l_tStorSet;
    bool_t l_bIsInit;
    uint16_t l_uCurrEleVer;
    uint8_t* l_puCurrEleData;
    uint32_t p_puPageIdx;
    uint32_t p_puPagePos;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eDBCRes = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, &l_bIsInit);
        l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);

        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOG_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOG_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOG_RES_CORRUPTCTX;
                }
                else
                {
                    /* Get storage info */
                    l_eDBCRes = eFSS_LOGC_GetBuffNStor(&p_ptCtx->tLOGCCtx, &l_tBuff, &l_tStorSet);
                    l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);

                    if( e_eFSS_LOG_RES_OK == l_eRes )
                    {
                        /* Find the page and page index where to load the data */
                        eFSS_LOG_GetPageAndPagePosition(l_tStorSet.uPagesLen, p_ptCtx->tDBS.uRawElemL, p_uPos,
                                                         &p_puPageIdx, &p_puPagePos);

                        /* Load the page where we can find the needed element */
                        l_eDBCRes = eFSS_LOGC_LoadPageInBuff(&p_ptCtx->tLOGCCtx, p_puPageIdx);
                        l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);

                        if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) )
                        {
                            /* Get element from the buffer */
                            eFSS_LOG_GetEleRawInBuffer(p_ptCtx->tDBS.uRawElemL, p_puPageIdx,
                                                        l_tBuff.puBuf, &l_uCurrEleVer, &l_puCurrEleData);

                            /* Deserialize the needed element */
                            (*p_ptCtx->tSeDeserCtx.fDeserial)(p_uPos, p_ptCtx->tDBS.uRawElemL, p_ptElem, l_puCurrEleData);
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_AddLog(t_eFSS_LOG_Ctx* const p_ptCtx, t_eFSS_TYPE_LogRawElement* p_ptLog)
{
	/* Local return variable */
    e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES  l_eDBCRes;

    /* Local variable for calculation */
    t_eFSS_TYPE_StorBuf l_tBuff;
    t_eFSS_TYPE_StorSet l_tStorSet;
    bool_t l_bIsInit;
    uint32_t p_puPageIdx;
    uint32_t p_puPagePos;
    t_eFSS_LOG_DbElement l_tCurrElem;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eDBCRes = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, &l_bIsInit);
        l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);

        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOG_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOG_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOG_RES_CORRUPTCTX;
                }
                else
                {
                    /* Get storage info */
                    l_eDBCRes = eFSS_LOGC_GetBuffNStor(&p_ptCtx->tLOGCCtx, &l_tBuff, &l_tStorSet);
                    l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);

                    if( e_eFSS_LOG_RES_OK == l_eRes )
                    {
                        /* Find the page and page index where to save the data */
                        eFSS_LOG_GetPageAndPagePosition(l_tStorSet.uPagesLen, p_ptCtx->tDBS.uRawElemL, p_uPos,
                                                         &p_puPageIdx, &p_puPagePos);

                        /* Load the page where we can find the needed element */
                        l_eDBCRes = eFSS_LOGC_LoadPageInBuff(&p_ptCtx->tLOGCCtx, p_puPageIdx);
                        l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);

                        if( ( e_eFSS_LOG_RES_OK == l_eRes ) || ( e_eFSS_LOG_RES_OK_BKP_RCVRD == l_eRes ) )
                        {
                            /* Set element in the buffer */
                            l_tCurrElem.ptDefVal = p_ptElem;
                            l_tCurrElem.uVer =  p_ptCtx->tDBS.ptElementList[p_uPos].uVer;
                            eFSS_LOG_SetEleInBuffer(p_ptCtx->tDBS.uRawElemL, p_puPagePos, l_tBuff.puBuf,
                                                     p_ptCtx->tSeDeserCtx.fSerial, &l_tCurrElem);

                            /* Flush the page */
                            l_eDBCRes = eFSS_LOGC_FlushBuffInPage(&p_ptCtx->tLOGCCtx, p_puPagePos);
                            l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);
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
static bool_t eFSS_LOG_IsStatusStillCoherent(const t_eFSS_LOG_Ctx* p_ptCtx)
{
    bool_t l_eRes;

	/* Check context validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = false;
	}
	else
	{
        l_eRes = true;
	}

    return l_eRes;
}

static e_eFSS_LOG_RES eFSS_LOG_DBCtoDBFLRes(const e_eFSS_LOGC_RES p_eDBCRes)
{
    e_eFSS_LOG_RES l_eRes;

    switch(p_eDBCRes)
    {
        case e_eFSS_LOGC_RES_OK:
        {
            l_eRes = e_eFSS_LOG_RES_OK;
            break;
        }

        case e_eFSS_LOGC_RES_BADPARAM:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }

        default:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }
    }

    return l_eRes;
}

static void eFSS_LOG_GetEleRawInBuffer(const uint32_t p_uEleL, const uint32_t p_uIndexInPage, uint8_t* p_puBuffer,
                                        uint16_t* p_uVer, uint8_t** p_ppuEleRaw)
{
    /* Local variable */
    uint32_t l_uElemPos;
    uint32_t l_uCurroffset;
    uint32_t l_uTemp;

    /* Calc position */
    l_uElemPos = p_uEleL * p_uIndexInPage;

    /* First, copy the element version in Little endian */
    l_uCurroffset = 0u;
    *p_uVer = 0u;
    l_uTemp = (uint16_t) p_puBuffer[l_uElemPos+l_uCurroffset];
    *p_uVer |= ( l_uTemp & 0x00FFu );
    l_uCurroffset++;

    l_uTemp =  (uint16_t) ( ( (uint16_t) p_puBuffer[l_uElemPos+l_uCurroffset] ) << 8u  );
    *p_uVer |= ( l_uTemp & 0xFF00u );
    l_uCurroffset++;

    /* Now insert resialed data from the user */
    *p_ppuEleRaw = &p_puBuffer[l_uElemPos+l_uCurroffset];
}

static void eFSS_LOG_SetEleInBuffer(const uint32_t p_uEleL, const uint32_t p_uIndexInPage, uint8_t* p_puBuffer,
                                           f_eFSS_TYPE_DbSerrialzCb p_fSerial, t_eFSS_LOG_DbElement* p_tElem)
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

static void eFSS_LOG_GetPageAndPagePosition(const uint32_t p_uPageL, const uint32_t p_uEleL, const uint32_t p_uIndex,
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