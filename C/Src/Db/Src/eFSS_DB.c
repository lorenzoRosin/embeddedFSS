/**
 * @file       eFSS_DB.h
 *
 * @brief      Database module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_DB.h"


/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_DB_IsStatusStillCoherent(const t_eFSS_DB_Ctx* p_ptCtx);
static e_eFSS_DB_RES eFSS_DB_DBCtoDBRes(const e_eFSS_DBC_RES p_eDBCRes);
static bool_t eFSS_DB_IsDbDefStructValid(t_eFSS_DB_DbStruct p_tDefaultDb);
static bool_t eFSS_DB_IsDbDefStructUsable(t_eFSS_DB_DbStruct p_tDefaultDb, uint32_t p_uNPage, uint32_t p_uPageL);
static void eFSS_DB_GetEleRawInBuffer(t_eFSS_DB_DbElement* p_gettedElem, uint8_t* p_puBuff);
static void eFSS_DB_SetEleRawInBuffer(t_eFSS_DB_DbElement p_gettedElem, uint8_t* p_puBuff);
static void eFSS_DB_GetPageAndPagePosition(const uint32_t p_uPageL, t_eFSS_DB_DbStruct p_tDbStruct,
                                           const uint32_t p_uIndex, uint32_t* p_puPageIdx, uint32_t* p_puLogPos);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_DB_RES eFSS_DB_InitCtx(t_eFSS_DB_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx const p_tCtxCb,
                              t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL,
                              t_eFSS_DB_DbStruct p_tDbStruct)
{
    /* Local return variable */
    e_eFSS_DB_RES l_eRes;
    e_eFSS_DBC_RES  l_eDBCRes;
    bool_t l_bIsDbStructValid;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_DB_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( NULL == p_tDbStruct.ptDefValElemList )
        {
            l_eRes = e_eFSS_DB_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( p_tDbStruct.uNOfElem <= 0u )
            {
                l_eRes = e_eFSS_DB_RES_BADPARAM;
            }
            else
            {
                if( p_tStorSet.uPagesLen < (100u) )
                {
                    l_eRes = e_eFSS_DB_RES_BADPARAM;
                }
                else
                {
                    /* Check validity of the passed db struct */
                    l_bIsDbStructValid = eFSS_DB_IsDbDefStructValid(p_tDbStruct);

                    if( false == l_bIsDbStructValid )
                    {
                        l_eRes = e_eFSS_DB_RES_BADPARAM;
                    }
                    else
                    {
                        /* Check validity of the passed db struct */
                        l_bIsDbStructValid = eFSS_DB_IsDbDefStructUsable(p_tDbStruct, 10u, 10u);

                        if( false == l_bIsDbStructValid )
                        {
                            l_eRes = e_eFSS_DB_RES_BADPARAM;
                        }
                        else
                        {
                            /* Can init low level context */
                            l_eDBCRes = eFSS_DBC_InitCtx(&p_ptCtx->tDbCtx, p_tCtxCb, p_tStorSet, p_puBuff, p_uBuffL);
                            l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                            if( e_eFSS_DB_RES_OK == l_eRes )
                            {
                                /* Fill context */
                                p_ptCtx->tDBS = p_tDbStruct;
                                p_ptCtx->bIsDbStatusChecked = false;
                            }
                        }
                    }
                }
            }
        }
    }

    return l_eRes;
}

e_eFSS_DB_RES eFSS_DB_IsInit(t_eFSS_DB_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
    e_eFSS_DB_RES l_eRes;
    e_eFSS_DBC_RES  l_eDBCRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSS_DB_RES_BADPOINTER;
	}
	else
	{
        l_eDBCRes = eFSS_DBC_IsInit(&p_ptCtx->tDbCtx, p_pbIsInit);
        l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);
	}

	return l_eRes;
}

e_eFSS_DB_RES eFSS_DB_GetDBStatus(t_eFSS_DB_Ctx* const p_ptCtx)
{
	/* Local return variable */
    e_eFSS_DB_RES  l_eRes;
    e_eFSS_DBC_RES l_eDBCRes;
    bool_t l_bIsInit;

    /* Local variable for storage */
    t_eFSS_TYPE_StorBuf l_tBuff;
    t_eFSS_TYPE_StorSet l_tStorSet;

    /* Local variable for calculation */
    uint32_t l_uCurrPage;
    uint32_t l_uMaxPage;
    uint32_t l_uCheckedElem;
    uint32_t l_uByteInPageDone;
    bool_t l_bIsPageMod;
    t_eFSS_DB_DbElement l_tCurrElem;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_DB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eDBCRes = eFSS_DBC_IsInit(&p_ptCtx->tDbCtx, &l_bIsInit);
        l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

        if( e_eFSS_DB_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
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
                    /* Get storage info */
                    l_eDBCRes = eFSS_DBC_GetBuffNStor(&p_ptCtx->tDbCtx, &l_tBuff, &l_tStorSet);
                    l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                    if( e_eFSS_DB_RES_OK == l_eRes )
                    {
                        /* Read every page, get parameter of every page and compare the parameter against the default
                         * DB struct. If some parameter has different version update them and if they differ signal
                         * to the user that the database is invalid */
                        l_uCurrPage = 0u;
                        l_uMaxPage = (uint32_t)( l_tStorSet.uTotPages / 2u );
                        l_uCheckedElem = 0u;

                        while( ( e_eFSS_DB_RES_OK == l_eRes ) || ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) ||
                               ( l_uCurrPage < l_uMaxPage ) )
                        {
                            /* Load the current page */
                            l_bIsPageMod = false;
                            l_eDBCRes = eFSS_DBC_LoadPageInBuff(&p_ptCtx->tDbCtx, l_uCurrPage);
                            l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                            if( ( e_eFSS_DB_RES_OK == l_eRes ) || ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) )
                            {
                                /* Check against DB default */
                                l_uByteInPageDone = 0u;

                                while( ( e_eFSS_DB_RES_OK == l_eRes ) || ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) ||
                                       ( l_uByteInPageDone < l_tBuff.uBufL ) )
                                {
                                    if( l_uCheckedElem < p_ptCtx->tDBS.uNOfElem )
                                    {
                                        /* Check element */
                                        if( ( l_uByteInPageDone + 2u + 2u + 1u ) > l_tBuff.uBufL )
                                        {
                                            /* cannot get other parameter, just check that data is zero */
                                            if( 0u == l_tBuff.puBuf[l_uByteInPageDone] )
                                            {
                                                l_uByteInPageDone++;
                                            }
                                            else
                                            {
                                                l_eRes = e_eFSS_DB_RES_NOTVALIDDB;
                                            }
                                        }
                                        else
                                        {
                                            /* Could be possible to have other parameter in page */
                                            /* Get current element */
                                            eFSS_DB_GetEleRawInBuffer(&l_tCurrElem, &l_tBuff.puBuf[l_uByteInPageDone]);

                                            /* Check the element */
                                            if( 0u == l_tCurrElem.uElemL )
                                            {
                                                /* No other element present */
                                                if( ( 0u == l_tCurrElem.uElemVer ) )
                                                {
                                                    l_uByteInPageDone += 2u + 2u;
                                                }
                                                else
                                                {
                                                    l_eRes = e_eFSS_DB_RES_NOTVALIDDB;
                                                }
                                            }
                                            else if( (l_uByteInPageDone + 2u + 2u + l_tCurrElem.uElemL) > l_tBuff.uBufL )
                                            {
                                                /* Getted more data than possible */
                                                l_eRes = e_eFSS_DB_RES_NOTVALIDDB;
                                            }
                                            else if( l_tCurrElem.uElemL != p_ptCtx->tDBS.ptDefValElemList[l_uCheckedElem].uElemL )
                                            {
                                                /* Invalid database */
                                                l_eRes = e_eFSS_DB_RES_NOTVALIDDB;
                                            }
                                            else if( l_tCurrElem.uElemVer != p_ptCtx->tDBS.ptDefValElemList[l_uCheckedElem].uElemVer )
                                            {
                                                /* Valid element but need to update version, ripristinate default */
                                                eFSS_DB_SetEleRawInBuffer(p_ptCtx->tDBS.ptDefValElemList[l_uCheckedElem],
                                                                          &l_tBuff.puBuf[l_uByteInPageDone]);
                                                l_uByteInPageDone += 2u + 2u + l_tCurrElem.uElemL;
                                                l_uCheckedElem++;
                                                l_bIsPageMod = true;
                                            }
                                            else
                                            {
                                                /* Element is already OK */
                                                l_uByteInPageDone += 2u + 2u + l_tCurrElem.uElemL;
                                                l_uCheckedElem++;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        /* Here we only need to check that non used byte are equals to zero */
                                        if( 0u == l_tBuff.puBuf[l_uByteInPageDone] )
                                        {
                                            l_uByteInPageDone++;
                                        }
                                        else
                                        {
                                            l_eRes = e_eFSS_DB_RES_NOTVALIDDB;
                                        }
                                    }
                                }

                                if( true == l_bIsPageMod )
                                {
                                    l_eDBCRes = eFSS_DBC_FlushBuffInPage(&p_ptCtx->tDbCtx, l_uCurrPage);
                                    l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);
                                }

                                l_uCurrPage++;
                            }
                        }

                        p_ptCtx->bIsDbStatusChecked = true;
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_DB_RES eFSS_DB_FormatToDefault(t_eFSS_DB_Ctx* const p_ptCtx)
{
	/* Local return variable */
    e_eFSS_DB_RES  l_eRes;
    e_eFSS_DBC_RES l_eDBCRes;
    bool_t l_bIsInit;

    /* Local variable for storage */
    t_eFSS_TYPE_StorBuf l_tBuff;
    t_eFSS_TYPE_StorSet l_tStorSet;

    /* Local variable for calculation */
    uint32_t l_uCurrPage;
    uint32_t l_uMaxPage;
    uint32_t l_uCheckedElem;
    uint32_t l_uByteInPageDone;
    t_eFSS_DB_DbElement l_tCurrElem;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_DB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eDBCRes = eFSS_DBC_IsInit(&p_ptCtx->tDbCtx, &l_bIsInit);
        l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

        if( e_eFSS_DB_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
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
                    /* Get storage info */
                    l_eDBCRes = eFSS_DBC_GetBuffNStor(&p_ptCtx->tDbCtx, &l_tBuff, &l_tStorSet);
                    l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                    if( e_eFSS_DB_RES_OK == l_eRes )
                    {
                        /* Set to zero the buffer of every page, copy default paramete and flush */
                        l_uCurrPage = 0u;
                        l_uMaxPage = (uint32_t)( l_tStorSet.uTotPages / 2u );
                        l_uCheckedElem = 0u;

                        while( ( e_eFSS_DB_RES_OK == l_eRes ) || ( l_uCurrPage < l_uMaxPage ) )
                        {
                            /* Memset the current page */
                            memset(&l_tBuff.ptMeta, 0u, sizeof(l_tBuff.ptMeta) );
                            memset(&l_tBuff.puBuf,  0u, l_tBuff.uBufL          );

                            /* Check against DB default */
                            l_uByteInPageDone = 0u;

                            while( ( e_eFSS_DB_RES_OK == l_eRes ) || ( l_uCheckedElem < p_ptCtx->tDBS.uNOfElem ) ||
                                   ( l_uByteInPageDone < l_tBuff.uBufL ) )
                            {
                                if( ( l_uByteInPageDone + 2u + 2u + p_ptCtx->tDBS.ptDefValElemList[l_uCheckedElem].uElemL ) <= l_tBuff.uBufL )
                                {
                                    eFSS_DB_SetEleRawInBuffer(p_ptCtx->tDBS.ptDefValElemList[l_uCheckedElem],
                                                              &l_tBuff.puBuf[l_uByteInPageDone]);
                                    l_uByteInPageDone += 2u + 2u + p_ptCtx->tDBS.ptDefValElemList[l_uCheckedElem].uElemL;
                                    l_uCheckedElem++;
                                }
                                else
                                {
                                    /* No more space in this page */
                                    l_uByteInPageDone = l_tBuff.uBufL;
                                }
                            }

                            l_eDBCRes = eFSS_DBC_FlushBuffInPage(&p_ptCtx->tDbCtx, l_uCurrPage);
                            l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                            l_uCurrPage++;
                        }

                        p_ptCtx->bIsDbStatusChecked = true;
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_DB_RES eFSS_DB_SaveElemen(t_eFSS_DB_Ctx* const p_ptCtx, uint32_t p_uPos, uint16_t p_uElemL,
                                 uint8_t* p_puRawVal)
{
	/* Local return variable */
    e_eFSS_DB_RES l_eRes;
    e_eFSS_DBC_RES  l_eDBCRes;

    /* Local variable for calculation */
    t_eFSS_TYPE_StorBuf l_tBuff;
    t_eFSS_TYPE_StorSet l_tStorSet;
    bool_t l_bIsInit;
    uint32_t p_puPageIdx;
    uint32_t p_puPagePos;
    t_eFSS_DB_DbElement l_tCurrElem;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_DB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eDBCRes = eFSS_DBC_IsInit(&p_ptCtx->tDbCtx, &l_bIsInit);
        l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

        if( e_eFSS_DB_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
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
                    /* Get storage info */
                    l_eDBCRes = eFSS_DBC_GetBuffNStor(&p_ptCtx->tDbCtx, &l_tBuff, &l_tStorSet);
                    l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                    if( e_eFSS_DB_RES_OK == l_eRes )
                    {
                        /* Find the page and page index where to save the data */
                        eFSS_DB_GetPageAndPagePosition(l_tStorSet.uPagesLen, p_ptCtx->tDBS, p_uPos, &p_puPageIdx, &p_puPagePos);

                        /* Load the page where we can find the needed element */
                        l_eDBCRes = eFSS_DBC_LoadPageInBuff(&p_ptCtx->tDbCtx, p_puPageIdx);
                        l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                        if( ( e_eFSS_DB_RES_OK == l_eRes ) || ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) )
                        {
                            /* Set element in the buffer */
                            l_tCurrElem.uElemVer = 0u;
                            l_tCurrElem.uElemL =  10u;
                            eFSS_DB_SetEleRawInBuffer(l_tCurrElem, &l_tBuff.puBuf[p_puPageIdx]);

                            /* Flush the page */
                            l_eDBCRes = eFSS_DBC_FlushBuffInPage(&p_ptCtx->tDbCtx, p_puPagePos);
                            l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_DB_RES eFSS_DB_GetElement(t_eFSS_DB_Ctx* const p_ptCtx, uint32_t p_uPos, uint16_t p_uElemL,
                                 uint8_t* p_puRawVal)
{
	/* Local return variable */
    e_eFSS_DB_RES l_eRes;
    e_eFSS_DBC_RES  l_eDBCRes;

    /* Local variable for calculation */
    t_eFSS_TYPE_StorBuf l_tBuff;
    t_eFSS_TYPE_StorSet l_tStorSet;
    bool_t l_bIsInit;
    uint16_t l_uCurrEleVer;
    uint8_t* l_puCurrEleData;
    uint32_t p_puPageIdx;
    uint32_t p_puPagePos;
    t_eFSS_DB_DbElement l_tCurrElem;
    uint32_t l_uByteInPageDone;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_DB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eDBCRes = eFSS_DBC_IsInit(&p_ptCtx->tDbCtx, &l_bIsInit);
        l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

        if( e_eFSS_DB_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
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
                    /* Get storage info */
                    l_eDBCRes = eFSS_DBC_GetBuffNStor(&p_ptCtx->tDbCtx, &l_tBuff, &l_tStorSet);
                    l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                    if( e_eFSS_DB_RES_OK == l_eRes )
                    {
                        /* Find the page and page index where to load the data */
                        eFSS_DB_GetPageAndPagePosition(l_tStorSet.uPagesLen, p_ptCtx->tDBS, p_uPos, &p_puPageIdx, &p_puPagePos);;

                        /* Load the page where we can find the needed element */
                        l_eDBCRes = eFSS_DBC_LoadPageInBuff(&p_ptCtx->tDbCtx, p_puPageIdx);
                        l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                        if( ( e_eFSS_DB_RES_OK == l_eRes ) || ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) )
                        {
                            /* Get element from the buffer */
                            eFSS_DB_GetEleRawInBuffer(&l_tCurrElem, &l_tBuff.puBuf[l_uByteInPageDone]);
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

static e_eFSS_DB_RES eFSS_DB_DBCtoDBRes(const e_eFSS_DBC_RES p_eDBCRes)
{
    e_eFSS_DB_RES l_eRes;

    switch(p_eDBCRes)
    {
        case e_eFSS_DBC_RES_OK:
        {
            l_eRes = e_eFSS_DB_RES_OK;
            break;
        }

        case e_eFSS_DBC_RES_BADPARAM:
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

static bool_t eFSS_DB_IsDbDefStructValid(t_eFSS_DB_DbStruct p_tDefaultDb)
{

}

static bool_t eFSS_DB_IsDbDefStructUsable(t_eFSS_DB_DbStruct p_tDefaultDb, uint32_t p_uNPage, uint32_t p_uPageL)
{

}

static void eFSS_DB_GetEleRawInBuffer(t_eFSS_DB_DbElement* p_gettedElem, uint8_t* p_puBuff)
{
    /* Local variable */
    uint32_t l_uElemPos;
    uint32_t l_uCurroffset;
    uint32_t l_uTemp;

    // /* Calc position */
    // l_uElemPos = p_uEleL * p_uIndexInPage;
//
    // /* First, copy the element version in Little endian */
    // l_uCurroffset = 0u;
    // *p_uVer = 0u;
    // l_uTemp = (uint16_t) p_puBuffer[l_uElemPos+l_uCurroffset];
    // *p_uVer |= ( l_uTemp & 0x00FFu );
    // l_uCurroffset++;
//
    // l_uTemp =  (uint16_t) ( ( (uint16_t) p_puBuffer[l_uElemPos+l_uCurroffset] ) << 8u  );
    // *p_uVer |= ( l_uTemp & 0xFF00u );
    // l_uCurroffset++;
//
    // /* Now insert resialed data from the user */
    // *p_ppuEleRaw = &p_puBuffer[l_uElemPos+l_uCurroffset];
}

static void eFSS_DB_SetEleRawInBuffer(t_eFSS_DB_DbElement p_gettedElem, uint8_t* p_puBuff)
{
    /* Local variable */
    uint32_t l_uElemPos;
    uint32_t l_uCurroffset;

    // /* Calc position */
    // l_uElemPos = p_uEleL * p_uIndexInPage;
//
    // /* First, insert the element version in Little endian */
    // l_uCurroffset = 0u;
    // p_puBuffer[l_uElemPos+l_uCurroffset] = (uint8_t) ( ( p_tElem->uVer        ) & 0x00FFu );
    // l_uCurroffset++;
//
    // p_puBuffer[l_uElemPos+l_uCurroffset] = (uint8_t) ( ( p_tElem->uVer  >> 8u  ) & 0x00FFu );
    // l_uCurroffset++;
//
    // /* Now insert resialed data from the user */
    // (*p_fSerial)(l_uElemPos, p_uEleL, p_tElem->ptDefVal, &p_puBuffer[l_uElemPos+l_uCurroffset]);
}

static void eFSS_DB_GetPageAndPagePosition(const uint32_t p_uPageL, t_eFSS_DB_DbStruct p_tDbStruct,
                                           const uint32_t p_uIndex, uint32_t* p_puPageIdx, uint32_t* p_puLogPos)
{
    /* Local variable */
    uint32_t l_uLogPerPage;
    uint32_t l_uPagePosition;
    uint32_t l_uLogPositionInPage;

    // /* Calc how many log we cat store in a page */
    // l_uLogPerPage = (uint32_t)( p_uPageL / p_uEleL );
//
    // /* Calculate page offset */
    // l_uPagePosition = (uint32_t)( p_uIndex / l_uLogPerPage );
//
    // /* Calculate log offset in page */
    // l_uLogPositionInPage = (uint32_t)( p_uIndex % l_uLogPerPage );
//
    // /* Return */
    // *p_puPageIdx = l_uPagePosition;
    // *p_puLogPos  = l_uLogPositionInPage;

}