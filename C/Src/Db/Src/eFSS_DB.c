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
static bool_t eFSS_DB_IsStatusStillCoherent(t_eFSS_DB_Ctx* const p_ptCtx);
static e_eFSS_DB_RES eFSS_DB_DBCtoDBRes(const e_eFSS_DBC_RES p_eDBCRes);



/***********************************************************************************************************************
 *  PRIVATE UTILS STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_DB_IsDbDefStructValid(const t_eFSS_DB_DbStruct p_tDefaultDb, const uint32_t p_uNPage,
                                         const uint32_t p_uPageL);
static e_eFSS_DB_RES eFSS_DB_GetEleRawInBuffer(uint8_t* const p_puBuff, const uint32_t p_uMaxL,
                                               t_eFSS_DB_DbElement* const p_ptElemToGet, uint32_t* const p_puUsedL);
static e_eFSS_DB_RES eFSS_DB_SetEleRawInBuffer(uint8_t* const p_puBuff, const uint32_t p_uMaxL,
                                               const t_eFSS_DB_DbElement p_tEleToSet, uint32_t* const p_puUsedL);
static e_eFSS_DB_RES eFSS_DB_GetPageAndPagePosition(const uint32_t p_uPageL, const t_eFSS_DB_DbStruct p_tDbStruct,
                                                    const uint32_t p_uIndex, uint32_t* const p_puPageIdx,
                                                    uint32_t* const p_puLogPos);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_DB_RES eFSS_DB_InitCtx(t_eFSS_DB_Ctx* const p_ptCtx, const t_eFSS_TYPE_CbStorCtx p_tCtxCb,
                              const t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff, const uint32_t p_uBuffL,
                              const t_eFSS_DB_DbStruct p_tDbStruct)
{
    /* Local return variable */
    e_eFSS_DB_RES l_eRes;
    e_eFSS_DBC_RES  l_eDBCRes;
    bool_t l_bIsDbStructValid;

    /* Local storage variable */
    t_eFSS_DBC_StorBuf l_tBuff;
    uint32_t l_uUsePages;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_DB_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( NULL == p_tDbStruct.ptDefEle )
        {
            l_eRes = e_eFSS_DB_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( p_tDbStruct.uNEle <= 0u )
            {
                l_eRes = e_eFSS_DB_RES_BADPARAM;
            }
            else
            {
                /* Can init low level context, and after get the used space and check database validity  */
                l_eDBCRes = eFSS_DBC_InitCtx(&p_ptCtx->tDbcCtx, p_tCtxCb, p_tStorSet, p_puBuff, p_uBuffL);
                l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                if( e_eFSS_DB_RES_OK == l_eRes )
                {
                    /* Get usable pages and buffer length so we can check database default value validity */
                    l_eDBCRes = eFSS_DBC_GetBuffNUsable(&p_ptCtx->tDbcCtx, &l_tBuff, &l_uUsePages);
                    l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                    if( e_eFSS_DB_RES_OK == l_eRes )
                    {
                        /* Check validity of the passed db struct */
                        l_bIsDbStructValid = eFSS_DB_IsDbDefStructValid(p_tDbStruct, l_uUsePages, l_tBuff.uBufL);

                        if( false == l_bIsDbStructValid )
                        {
                            /* De init DBC */
                            (void)memset(&p_ptCtx->tDbcCtx, 0, sizeof(t_eFSS_DBC_Ctx));

                            l_eRes = e_eFSS_DB_RES_BADPARAM;
                        }
                        else
                        {
                            /* Fill context */
                            p_ptCtx->tDB = p_tDbStruct;
                            p_ptCtx->bIsDbCheked = false;
                        }
                    }
                    else
                    {
                        /* De init DBC */
                        (void)memset(&p_ptCtx->tDbcCtx, 0, sizeof(t_eFSS_DBC_Ctx));
                    }
                }
            }
        }
    }

    return l_eRes;
}

e_eFSS_DB_RES eFSS_DB_IsInit(t_eFSS_DB_Ctx* const p_ptCtx, bool_t* const p_pbIsInit)
{
	/* Local variable */
    e_eFSS_DB_RES l_eRes;
    e_eFSS_DBC_RES  l_eDBCRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_DB_RES_BADPOINTER;
	}
	else
	{
        l_eDBCRes = eFSS_DBC_IsInit(&p_ptCtx->tDbcCtx, p_pbIsInit);
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
    t_eFSS_DBC_StorBuf l_tBuff;
    uint32_t l_uUsePages;

    /* Local variable for calculation */
    uint32_t l_uCurrPage;
    uint32_t l_uCheckedElem;
    uint32_t l_uByteInPageDone;
    bool_t l_bIsPageMod;
    bool_t l_bNewParAdd;
    t_eFSS_DB_DbElement l_tCurEle;
    uint32_t l_uUsedByte;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_DB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eDBCRes = eFSS_DBC_IsInit(&p_ptCtx->tDbcCtx, &l_bIsInit);
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
                    l_eDBCRes = eFSS_DBC_GetBuffNUsable(&p_ptCtx->tDbcCtx, &l_tBuff, &l_uUsePages);
                    l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                    if( e_eFSS_DB_RES_OK == l_eRes )
                    {
                        /* Now that we have every needed data and that the default database struct seems correct
                           we need to check that the database actualy stored in storage is correct. Keep in mind
                           that unused storage area must be set to zero, in this way if anothers entry is added
                           to the default struct we can set the default value of the entry that are actualy stored
                           with length of zero byte.
                           So:
                           1 - If we need to check some parameter from def database read a storage page.
                           2 - For every readed storage page check that every entry has the same length of the default
                               database. After that check the parameter version, and if they differ update parameter
                               default value and version.
                           3 - If some parameter is updated with a new default value flush the page in to the storage.
                           4 - When checking entry length keep in mind that if the stored entry has length zero it
                               could be possible that a new entry was added to the default database. In that case
                               add the new parameter and continue. If the next stored parameter has a length different
                               from zero the database was corrupted.
                            Keep in mind that the pourpose of this function is to: update new parameter version,
                            update newly added parameter and check if parameter are correct. If the database is
                            corrupted, we can ripristinate it only calling eFSS_DB_FormatToDefault */

                        /* Init variable */
                        l_uCurrPage = 0u;
                        l_uCheckedElem = 0u;
                        l_bNewParAdd = false;

                        /* Read and check every page of the storage area */
                        while( ( ( e_eFSS_DB_RES_OK == l_eRes ) ||
                                 ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) ||
                                 ( e_eFSS_DB_RES_PARAM_DEF_RESET == l_eRes ) ) &&
                               ( l_uCurrPage < l_uUsePages ) )
                        {
                            /* Init page check, so we know if the page need to be stored after a modification */
                            l_bIsPageMod = false;

                            /* Load the current page */
                            l_eDBCRes = eFSS_DBC_LoadPageInBuff(&p_ptCtx->tDbcCtx, l_uCurrPage);
                            l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                            if( ( e_eFSS_DB_RES_OK == l_eRes ) || ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) )
                            {
                                /* Whole page just read, check the page against the default value if any, or check
                                   if the unused data is actualy zero */

                                /* Init variable that keep count of the checked numbers of byte */
                                l_uByteInPageDone = 0u;

                                while( ( ( e_eFSS_DB_RES_OK == l_eRes ) ||
                                         ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) ||
                                         ( e_eFSS_DB_RES_PARAM_DEF_RESET == l_eRes ) ) &&
                                       ( l_uByteInPageDone < l_tBuff.uBufL ) )
                                {
                                    /* Two cases: 1) we have some element to check
                                     *            2) we need to check that unused elements are set to zero */
                                    if( l_uCheckedElem < p_ptCtx->tDB.uNEle )
                                    {
                                        /* Wee need to check elements value  */
                                        /* Do we have space in this page for another element? */
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
                                            /* Get current element, if any */
                                            if( true == eFSS_DB_GetEleRawInBuffer( &l_tBuff.puBuf[l_uByteInPageDone],
                                                                                   (l_tBuff.uBufL - l_uByteInPageDone),
                                                                                   &l_tCurEle, &l_uUsedByte ) )
                                            {
                                                /* We have getted a valid element, check it! */
                                                if( l_tCurEle.uEleL != p_ptCtx->tDB.ptDefEle[l_uCheckedElem].uEleL )
                                                {
                                                    /* Oks, this could lead to an invalid database. First check if,
                                                       maybe, this is a new added element */
                                                    if( 0u == l_tCurEle.uEleL )
                                                    {
                                                        /* New parameter found! */
                                                        l_bNewParAdd = true;

                                                        /* Try to add it to the page */
                                                        if( true == eFSS_DB_SetEleRawInBuffer( &l_tBuff.puBuf[l_uByteInPageDone],
                                                                                            (l_tBuff.uBufL - l_uByteInPageDone),
                                                                                            p_ptCtx->tDB.ptDefEle[l_uCheckedElem], &l_uUsedByte ) )
                                                        {
                                                            /* Setted */
                                                            l_uByteInPageDone += ( 2u + 2u + l_tCurEle.uEleL );
                                                            l_uCheckedElem++;
                                                            l_bIsPageMod = true;
                                                        }
                                                        else
                                                        {
                                                            /* No space, check zero data */
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
                                                    else
                                                    {
                                                        /* Invalid database */
                                                        l_eRes = e_eFSS_DB_RES_NOTVALIDDB;
                                                    }
                                                }
                                                else if( true == l_bNewParAdd )
                                                {
                                                    /* We have already added a new param, cannot stand that we have
                                                       another parameter stored with a valid length */
                                                    l_eRes = e_eFSS_DB_RES_NOTVALIDDB;
                                                }
                                                else if( l_tCurEle.uEleV != p_ptCtx->tDB.ptDefEle[l_uCheckedElem].uEleV )
                                                {
                                                    /* Valid element but need to update version, ripristinate default */
                                                    if( true == eFSS_DB_SetEleRawInBuffer( &l_tBuff.puBuf[l_uByteInPageDone],
                                                                                           (l_tBuff.uBufL - l_uByteInPageDone),
                                                                                           p_ptCtx->tDB.ptDefEle[l_uCheckedElem],
                                                                                           &l_uUsedByte ) )
                                                    {
                                                        /* Setted */
                                                        l_uByteInPageDone += ( 2u + 2u + l_tCurEle.uEleL );
                                                        l_uCheckedElem++;
                                                        l_bIsPageMod = true;
                                                    }
                                                    else
                                                    {
                                                        /* No space, check zero data */
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
                                                else
                                                {
                                                    /* Element is already OK */
                                                    l_uByteInPageDone += ( 2u + 2u + l_tCurEle.uEleL );
                                                    l_uCheckedElem++;
                                                }
                                            }
                                            else
                                            {
                                                /* No element found, check zero data */
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
                                    }
                                    else
                                    {
                                        /* No more element to check, check that unused data is set to zero */
                                        if( 0u == l_tBuff.puBuf[l_uByteInPageDone] )
                                        {
                                            l_uByteInPageDone++;
                                        }
                                        else
                                        {
                                            /* unused memory must be set to zero */
                                            l_eRes = e_eFSS_DB_RES_NOTVALIDDB;
                                        }
                                    }
                                }

                                /* If page is modified we need to store the new value */
                                if( true == l_bIsPageMod )
                                {
                                    l_eDBCRes = eFSS_DBC_FlushBuffInPage(&p_ptCtx->tDbcCtx, l_uCurrPage);
                                    l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);
                                }

                                l_uCurrPage++;
                            }
                        }

                        if( ( e_eFSS_DB_RES_OK == l_eRes ) ||
                            ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) ||
                            ( e_eFSS_DB_RES_PARAM_DEF_RESET == l_eRes ) )
                        {
                            /* Whole database checked */
                            p_ptCtx->bIsDbCheked = true;
                        }
                        else
                        {
                            /* Database is corrupted, signal il as not cheked so we will not allow any other
                             * modification to this situation */
                            p_ptCtx->bIsDbCheked = true;
                        }
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
    t_eFSS_DBC_StorBuf l_tBuff;
    uint32_t l_uUsePages;

    /* Local variable for calculation */
    uint32_t l_uCurrPage;
    uint32_t l_uCheckedElem;
    uint32_t l_uByteInPageDone;
    uint32_t l_uUsedByte;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_DB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eDBCRes = eFSS_DBC_IsInit(&p_ptCtx->tDbcCtx, &l_bIsInit);
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
                    l_eDBCRes = eFSS_DBC_GetBuffNUsable(&p_ptCtx->tDbcCtx, &l_tBuff, &l_uUsePages);
                    l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                    if( e_eFSS_DB_RES_OK == l_eRes )
                    {
                        /* In order to format to default the DB we can just set to zero the buffer to flush
                           and copy inside it only the needed parameter. If no parameter are needed, we can
                           just flush an zero filled buffer */
                        l_uCurrPage = 0u;
                        l_uCheckedElem = 0u;

                        /* Continue till we have setted all pages or an error occours */
                        while( ( e_eFSS_DB_RES_OK == l_eRes ) || ( l_uCurrPage < l_uUsePages ) )
                        {
                            /* Memset the current page */
                            memset(&l_tBuff.puBuf,  0u, l_tBuff.uBufL);

                            /* Set to zero the numbers of byte used */
                            l_uByteInPageDone = 0u;

                            /* Continue till the page is full, parameter are avaiable, or an error occours */
                            while( ( e_eFSS_DB_RES_OK == l_eRes ) ||
                                   ( l_uCheckedElem < p_ptCtx->tDB.uNEle ) ||
                                   ( l_uByteInPageDone < l_tBuff.uBufL ) )
                            {
                                /* Try to set the element */
                                l_uUsedByte = 0u;
                                l_eDBCRes = eFSS_DB_SetEleRawInBuffer(&l_tBuff.puBuf[l_uByteInPageDone],
                                                                      (l_tBuff.uBufL - l_uByteInPageDone),
                                                                      p_ptCtx->tDB.ptDefEle[l_uCheckedElem],
                                                                      &l_uUsedByte);
                                l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                                if( e_eFSS_DB_RES_OK == l_eRes )
                                {
                                    /* Check if there was enough space to insert element */
                                    if( 0u != l_uUsedByte )
                                    {
                                        /* Element added */
                                        l_uByteInPageDone += l_uUsedByte;
                                        l_uCheckedElem++;
                                    }
                                    else
                                    {
                                        /* No space to insert the element, flush the page */
                                        l_uByteInPageDone = l_tBuff.uBufL;
                                    }
                                }
                            }

                            if( e_eFSS_DB_RES_OK == l_eRes )
                            {
                                l_eDBCRes = eFSS_DBC_FlushBuffInPage(&p_ptCtx->tDbcCtx, l_uCurrPage);
                                l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                                if( e_eFSS_DB_RES_OK == l_eRes )
                                {
                                    l_uCurrPage++;
                                }
                            }
                        }

                        p_ptCtx->bIsDbCheked = true;
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_DB_RES eFSS_DB_SaveElemen(t_eFSS_DB_Ctx* const p_ptCtx, const uint32_t p_uPos, const uint16_t p_uElemL,
                                 uint8_t* const p_puRawVal)
{
	/* Local return variable */
    e_eFSS_DB_RES l_eRes;
    e_eFSS_DBC_RES  l_eDBCRes;
    bool_t l_bIsInit;

    /* Local variable for storage */
    t_eFSS_DBC_StorBuf l_tBuff;
    uint32_t l_uUsePages;

    /* Local variable for calculation */
    uint32_t p_puPageIdx;
    uint32_t p_puPagePos;
    t_eFSS_DB_DbElement l_tCurEle;
    uint32_t l_uUsedByte;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puRawVal ) )
	{
		l_eRes = e_eFSS_DB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_bIsInit = false;
        l_eDBCRes = eFSS_DBC_IsInit(&p_ptCtx->tDbcCtx, &l_bIsInit);
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
                    /* Verify if parameter is ok checked against the DB */
                    if( (p_uPos > p_ptCtx->tDB.uNEle ) || ( p_uElemL != p_ptCtx->tDB.ptDefEle[p_uPos].uEleL ) )
                    {
                        l_eRes = e_eFSS_DB_RES_BADPARAM;
                    }
                    else
                    {
                        /* If not done till now check the stored database validity */
                        if( false == p_ptCtx->bIsDbCheked )
                        {
                            /* Check status */
                            l_eRes = eFSS_DB_GetDBStatus(p_ptCtx);
                        }

                        if( ( e_eFSS_DB_RES_OK == l_eRes ) || ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) ||
                            ( e_eFSS_DB_RES_PARAM_DEF_RESET == l_eRes ) )
                        {
                            /* Get storage info */
                            l_eDBCRes = eFSS_DBC_GetBuffNUsable(&p_ptCtx->tDbcCtx, &l_tBuff, &l_uUsePages);
                            l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                            if( e_eFSS_DB_RES_OK == l_eRes )
                            {
                                /* Find the page and page index where to save the data */
                                eFSS_DB_GetPageAndPagePosition(l_tBuff.uBufL, p_ptCtx->tDB, p_uPos, &p_puPageIdx,
                                                               &p_puPagePos);

                                /* Load the page where we can find the needed element */
                                l_eDBCRes = eFSS_DBC_LoadPageInBuff(&p_ptCtx->tDbcCtx, p_puPageIdx);
                                l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                                if( ( e_eFSS_DB_RES_OK == l_eRes ) || ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) )
                                {
                                    /* Verify if the already old stored element is correct */
                                    eFSS_DB_GetEleRawInBuffer(&l_tBuff.puBuf[p_puPagePos],
                                                              ( l_tBuff.uBufL - p_puPagePos ) ,
                                                              &l_tCurEle, &l_uUsedByte);

                                    if( ( l_tCurEle.uEleV != p_ptCtx->tDB.ptDefEle[p_uPos].uEleV ) ||
                                        ( l_tCurEle.uEleL != p_ptCtx->tDB.ptDefEle[p_uPos].uEleL ) )
                                    {
                                        /* The database is incoherent */
                                        l_eRes = e_eFSS_DB_RES_NOTVALIDDB;
                                    }
                                    else
                                    {
                                        /* Set element in the buffer */
                                        l_eDBCRes = eFSS_DB_SetEleRawInBuffer(&l_tBuff.puBuf[p_puPagePos],
                                                                             (l_tBuff.uBufL - p_puPagePos),
                                                                              l_tCurEle, &l_uUsedByte);
                                        l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                                        /* Flush the page */
                                        l_eDBCRes = eFSS_DBC_FlushBuffInPage(&p_ptCtx->tDbcCtx, p_puPagePos);
                                        l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);
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

e_eFSS_DB_RES eFSS_DB_GetElement(t_eFSS_DB_Ctx* const p_ptCtx, const uint32_t p_uPos, const uint16_t p_uElemL,
                                 uint8_t* const p_puRawVal)
{
	/* Local return variable */
    e_eFSS_DB_RES l_eRes;
    e_eFSS_DBC_RES  l_eDBCRes;
    bool_t l_bIsInit;

    /* Local variable for storage */
    t_eFSS_DBC_StorBuf l_tBuff;
    uint32_t l_uUsePages;

    /* Local variable for calculation */
    uint32_t p_puPageIdx;
    uint32_t p_puPagePos;
    t_eFSS_DB_DbElement l_tCurEle;
    uint32_t l_uUsedByte;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_DB_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eDBCRes = eFSS_DBC_IsInit(&p_ptCtx->tDbcCtx, &l_bIsInit);
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
                    /* Verify if parameter is ok checked against the DB */
                    if( p_uPos > p_ptCtx->tDB.uNEle )
                    {
                        l_eRes = e_eFSS_DB_RES_BADPARAM;
                    }
                    else
                    {
                        /* Verify if parameter is ok checked against the DB */
                        if( p_uElemL != p_ptCtx->tDB.ptDefEle[p_uPos].uEleL )
                        {
                            l_eRes = e_eFSS_DB_RES_BADPARAM;
                        }
                        else
                        {
                            if( false == p_ptCtx->bIsDbCheked )
                            {
                                /* Check status */
                                l_eRes = eFSS_DB_GetDBStatus(p_ptCtx);
                            }

                            if( ( e_eFSS_DB_RES_OK == l_eRes ) || ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) )
                            {
                                /* Get storage info */
                                l_eDBCRes = eFSS_DBC_GetBuffNUsable(&p_ptCtx->tDbcCtx, &l_tBuff, &l_uUsePages);
                                l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                                if( e_eFSS_DB_RES_OK == l_eRes )
                                {
                                    /* Find the page and page index where to save the data */
                                    eFSS_DB_GetPageAndPagePosition(l_tBuff.uBufL, p_ptCtx->tDB, p_uPos, &p_puPageIdx, &p_puPagePos);

                                    /* Load the page where we can find the needed element */
                                    l_eDBCRes = eFSS_DBC_LoadPageInBuff(&p_ptCtx->tDbcCtx, p_puPageIdx);
                                    l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                                    if( ( e_eFSS_DB_RES_OK == l_eRes ) || ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) )
                                    {
                                        /* Verify if the old element is already correct */
                                        eFSS_DB_GetEleRawInBuffer(&l_tBuff.puBuf[p_puPagePos],
                                                                  ( l_tBuff.uBufL - p_puPagePos ) ,
                                                                  &l_tCurEle, &l_uUsedByte);

                                        if( ( l_tCurEle.uEleV != p_ptCtx->tDB.ptDefEle[p_uPos].uEleV ) ||
                                            ( l_tCurEle.uEleL != p_ptCtx->tDB.ptDefEle[p_uPos].uEleL ) )
                                        {
                                            /* The database is incoherent */
                                            l_eRes = e_eFSS_DB_RES_NOTVALIDDB;
                                        }
                                        else
                                        {
                                            /* Can copy the element */
                                            memcpy(p_puRawVal, l_tCurEle.puEleRaw, p_uElemL);
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


/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eFSS_DB_IsStatusStillCoherent(t_eFSS_DB_Ctx* const p_ptCtx)
{
    /* Local return variable */
    bool_t l_eRes;
    e_eFSS_DBC_RES  l_eDBCRes;
    bool_t l_bIsDbStructValid;

    /* Local storage variable */
    t_eFSS_DBC_StorBuf l_tBuff;
    uint32_t l_uUsePages;

	/* Check pointer validity */
	if( NULL == p_ptCtx->tDB.ptDefEle )
	{
		l_eRes = false;
	}
	else
	{
        /* Check pointer validity */
        if( p_ptCtx->tDB.uNEle <= 0u )
        {
            l_eRes = false;
        }
        else
        {
            /* Get usable pages and buffer length so we can check database default value validity */
            l_eDBCRes = eFSS_DBC_GetBuffNUsable(&p_ptCtx->tDbcCtx, &l_tBuff, &l_uUsePages);
            l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

            if( e_eFSS_DB_RES_OK != l_eRes )
            {
                l_eRes = false;
            }
            else
            {
                /* Check validity of the passed db struct */
                l_eRes = eFSS_DB_IsDbDefStructValid(p_ptCtx->tDB, l_uUsePages, l_tBuff.uBufL);
            }
        }
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



/***********************************************************************************************************************
 *  PRIVATE UTILS STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_DB_IsDbDefStructValid(const t_eFSS_DB_DbStruct p_tDefaultDb, const uint32_t p_uNPage,
                                         const uint32_t p_uPageL)
{
    /* Check db validity, each element need to be different from zero and ver must not be equals to zero  */
    return true;
}

static e_eFSS_DB_RES eFSS_DB_GetEleRawInBuffer(uint8_t* const p_puBuff, const uint32_t p_uMaxL,
                                               t_eFSS_DB_DbElement* const p_ptElemToGet, uint32_t* const p_puUsedL)
{
    /* Local variable */
    // uint32_t l_uElemPos;
    // uint32_t l_uCurroffset;
    // uint32_t l_uTemp;

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

static e_eFSS_DB_RES eFSS_DB_SetEleRawInBuffer(uint8_t* const p_puBuff, const uint32_t p_uMaxL,
                                               const t_eFSS_DB_DbElement p_tEleToSet, uint32_t* const p_puUsedL)
{
    /* Local variable */
     // uint32_t l_uElemPos;
     // uint32_t l_uCurroffset;

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

static e_eFSS_DB_RES eFSS_DB_GetPageAndPagePosition(const uint32_t p_uPageL, const t_eFSS_DB_DbStruct p_tDbStruct,
                                                    const uint32_t p_uIndex, uint32_t* const p_puPageIdx,
                                                    uint32_t* const p_puLogPos)
{
    // /* Local variable */
    // uint32_t l_uLogPerPage;
    // uint32_t l_uPagePosition;
    // uint32_t l_uLogPositionInPage;
//
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