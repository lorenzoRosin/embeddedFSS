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
#include "eFSS_Utils.h"



/***********************************************************************************************************************
 *      PRIVATE DEFINE
 **********************************************************************************************************************/
#define EFSS_DB_MINPAGESIZE                                                                      ( ( uint32_t )  0x05u )
#define EFSS_DB_VEROFF                                                                           ( ( uint32_t )  0x00u )
#define EFSS_DB_LENOFF                                                                           ( ( uint32_t )  0x02u )
#define EFSS_DB_RAWOFF                                                                           ( ( uint32_t )  0x04u )



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
static e_eFSS_DB_RES eFSS_DB_GetEleRawInBuffer(uint8_t* const p_puBuff, const uint32_t p_uMaxL, const uint32_t p_uPageL,
                                               t_eFSS_DB_DbElement* const p_ptElemToGet, uint32_t* const p_puUsedL);
static e_eFSS_DB_RES eFSS_DB_SetEleRawInBuffer(uint8_t* const p_puBuff, const uint32_t p_uMaxL,
                                               const t_eFSS_DB_DbElement p_tEleToSet, uint32_t* const p_puUsedL);
static e_eFSS_DB_RES eFSS_DB_FindElePageAndPos(const uint32_t p_uPageL, const t_eFSS_DB_DbStruct p_tDbStruct,
                                               const uint32_t p_uEleIdx, uint32_t* const p_puPFound,
                                               uint32_t* const p_puOffSetFound);



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
                                                                                   l_tBuff.uBufL,
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
                                l_eRes = eFSS_DB_SetEleRawInBuffer(&l_tBuff.puBuf[l_uByteInPageDone],
                                                                      (l_tBuff.uBufL - l_uByteInPageDone),
                                                                      p_ptCtx->tDB.ptDefEle[l_uCheckedElem],
                                                                      &l_uUsedByte);

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
                    if( ( p_uPos > p_ptCtx->tDB.uNEle ) || ( p_uElemL != p_ptCtx->tDB.ptDefEle[p_uPos].uEleL ) )
                    {
                        l_eRes = e_eFSS_DB_RES_BADPARAM;
                    }
                    else
                    {
                        /* First time calling a function we need to check for the whole stored integrity */
                        if( false == p_ptCtx->bIsDbCheked )
                        {
                            /* Check status */
                            l_eRes = eFSS_DB_GetDBStatus(p_ptCtx);
                        }

                        /* If internal stored database seems ok continue */
                        if( ( e_eFSS_DB_RES_OK == l_eRes ) || ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) ||
                            ( e_eFSS_DB_RES_PARAM_DEF_RESET == l_eRes ) )
                        {
                            /* Get storage info */
                            l_eDBCRes = eFSS_DBC_GetBuffNUsable(&p_ptCtx->tDbcCtx, &l_tBuff, &l_uUsePages);
                            l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                            if( e_eFSS_DB_RES_OK == l_eRes )
                            {
                                /* Find the page and page index where to save the data */
                                l_eRes = eFSS_DB_FindElePageAndPos(l_tBuff.uBufL, p_ptCtx->tDB, p_uPos,
                                                                   &p_puPageIdx, &p_puPagePos);

                                if( e_eFSS_DB_RES_OK == l_eRes )
                                {
                                    /* Load the page where we can find the needed element */
                                    l_eDBCRes = eFSS_DBC_LoadPageInBuff(&p_ptCtx->tDbcCtx, p_puPageIdx);
                                    l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                                    if( ( e_eFSS_DB_RES_OK == l_eRes ) || ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) )
                                    {
                                        /* Verify if the already stroed element is correct  */
                                        l_eRes = eFSS_DB_GetEleRawInBuffer(&l_tBuff.puBuf[p_puPagePos],
                                                                           ( l_tBuff.uBufL - p_puPagePos ) ,
                                                                           l_tBuff.uBufL,
                                                                           &l_tCurEle, &l_uUsedByte);

                                        if( e_eFSS_DB_RES_OK == l_eRes )
                                        {
                                            /* Check if previous param has correct version and length */
                                            if( ( l_tCurEle.uEleV != p_ptCtx->tDB.ptDefEle[p_uPos].uEleV ) ||
                                                ( l_tCurEle.uEleL != p_ptCtx->tDB.ptDefEle[p_uPos].uEleL ) )
                                            {
                                                /* The database is incoherent */
                                                l_eRes = e_eFSS_DB_RES_NOTVALIDDB;
                                            }
                                            else
                                            {
                                                /* Ok, the previously saved element seems ok, save it */
                                                l_eRes = eFSS_DB_SetEleRawInBuffer(&l_tBuff.puBuf[p_puPagePos],
                                                                                   (l_tBuff.uBufL - p_puPagePos),
                                                                                   l_tCurEle, &l_uUsedByte);

                                                if( e_eFSS_DB_RES_OK == l_eRes )
                                                {
                                                    /* Flush the page */
                                                    l_eDBCRes = eFSS_DBC_FlushBuffInPage(&p_ptCtx->tDbcCtx,
                                                                                         p_puPagePos);
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
                    if( ( p_uPos > p_ptCtx->tDB.uNEle ) || ( p_uElemL != p_ptCtx->tDB.ptDefEle[p_uPos].uEleL ) )
                    {
                        l_eRes = e_eFSS_DB_RES_BADPARAM;
                    }
                    else
                    {
                        /* First time calling a function we need to check for the whole stored integrity */
                        if( false == p_ptCtx->bIsDbCheked )
                        {
                            /* Check status */
                            l_eRes = eFSS_DB_GetDBStatus(p_ptCtx);
                        }

                        /* If internal stored database seems ok continue */
                        if( ( e_eFSS_DB_RES_OK == l_eRes ) || ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) ||
                            ( e_eFSS_DB_RES_PARAM_DEF_RESET == l_eRes ) )
                        {
                            /* Get storage info */
                            l_eDBCRes = eFSS_DBC_GetBuffNUsable(&p_ptCtx->tDbcCtx, &l_tBuff, &l_uUsePages);
                            l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                            if( e_eFSS_DB_RES_OK == l_eRes )
                            {
                                /* Find the page and page index where to get the data */
                                l_eRes = eFSS_DB_FindElePageAndPos(l_tBuff.uBufL, p_ptCtx->tDB, p_uPos,
                                                                   &p_puPageIdx, &p_puPagePos);

                                if( e_eFSS_DB_RES_OK == l_eRes )
                                {
                                    /* Load the page where we can find the needed element */
                                    l_eDBCRes = eFSS_DBC_LoadPageInBuff(&p_ptCtx->tDbcCtx, p_puPageIdx);
                                    l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                                    if( ( e_eFSS_DB_RES_OK == l_eRes ) || ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) )
                                    {
                                        /* Verify if the already stroed element is correct  */
                                        l_eRes = eFSS_DB_GetEleRawInBuffer(&l_tBuff.puBuf[p_puPagePos],
                                                                           ( l_tBuff.uBufL - p_puPagePos ) ,
                                                                           l_tBuff.uBufL,
                                                                           &l_tCurEle, &l_uUsedByte);

                                        if( e_eFSS_DB_RES_OK == l_eRes )
                                        {
                                            /* Check if previous param has correct version and length */
                                            if( ( l_tCurEle.uEleV != p_ptCtx->tDB.ptDefEle[p_uPos].uEleV ) ||
                                                ( l_tCurEle.uEleL != p_ptCtx->tDB.ptDefEle[p_uPos].uEleL ) )
                                            {
                                                /* The database is incoherent */
                                                l_eRes = e_eFSS_DB_RES_NOTVALIDDB;
                                            }
                                            else
                                            {
                                                /* Can copy the element */
                                                (void)memcpy(p_puRawVal, l_tCurEle.puEleRaw, p_uElemL);
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


/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eFSS_DB_IsStatusStillCoherent(t_eFSS_DB_Ctx* const p_ptCtx)
{
    /* Local return variable */
    bool_t l_eRes;
    e_eFSS_DBC_RES  l_eDBCRes;

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
        /* Check data validity */
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
                /* Check data validity */
                if( ( l_uUsePages <= 0u ) || ( l_tBuff.uBufL < EFSS_DB_MINPAGESIZE ) )
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

        case e_eFSS_DBC_RES_NOINITLIB:
        {
            l_eRes = e_eFSS_DB_RES_NOINITLIB;
            break;
        }

        case e_eFSS_DBC_RES_BADPARAM:
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
            break;
        }

        case e_eFSS_DBC_RES_BADPOINTER:
        {
            l_eRes = e_eFSS_DB_RES_BADPOINTER;
            break;
        }

        case e_eFSS_DBC_RES_CORRUPTCTX:
        {
            l_eRes = e_eFSS_DB_RES_CORRUPTCTX;
            break;
        }

        case e_eFSS_DBC_RES_CLBCKERASEERR:
        {
            l_eRes = e_eFSS_DB_RES_CLBCKERASEERR;
            break;
        }

        case e_eFSS_DBC_RES_CLBCKWRITEERR:
        {
            l_eRes = e_eFSS_DB_RES_CLBCKWRITEERR;
            break;
        }

        case e_eFSS_DBC_RES_CLBCKREADERR:
        {
            l_eRes = e_eFSS_DB_RES_CLBCKREADERR;
            break;
        }

        case e_eFSS_DBC_RES_CLBCKCRCERR:
        {
            l_eRes = e_eFSS_DB_RES_CLBCKCRCERR;
            break;
        }

        case e_eFSS_DBC_RES_NOTVALIDDB:
        {
            l_eRes = e_eFSS_DB_RES_NOTVALIDDB;
            break;
        }

        case e_eFSS_DBC_RES_NEWVERSIONDB:
        {
            l_eRes = e_eFSS_DB_RES_NEWVERSIONDB;
            break;
        }

        case e_eFSS_DBC_RES_WRITENOMATCHREAD:
        {
            l_eRes = e_eFSS_DB_RES_WRITENOMATCHREAD;
            break;
        }

        case e_eFSS_DBC_RES_OK_BKP_RCVRD:
        {
            l_eRes = e_eFSS_DB_RES_OK_BKP_RCVRD;
            break;
        }

        default:
        {
            l_eRes = e_eFSS_DB_RES_CORRUPTCTX;
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
    /* Check db validity:
       1- Each element version and len need to be different from zero
       2- Element raw data must be different from NULL
       3- An element length cannot be greater than page length
       4- All element must be able to be stored in database
       5- Numbers of element need to be different from zero
     */

    /* Local variable for return */
    bool_t l_bRes;

    /* Local variable for calculation */
    uint32_t l_uCurIndex;
    t_eFSS_DB_DbElement l_tCurEle;
    uint32_t l_uCurPage;
    uint32_t l_uCurPageUsed;

    /* Check pointer validity */
    if( NULL == p_tDefaultDb.ptDefEle )
    {
        l_bRes = false;
    }
    else
    {
        if( 0u == p_tDefaultDb.uNEle )
        {
            l_bRes = false;
        }
        else
        {
            /* Init local */
            l_uCurIndex = 0u;
            l_uCurPage = 0u;
            l_uCurPageUsed = 0u;
            l_bRes = true;

            /* Check every parameter till an error is found */
            while( ( true == l_bRes ) && ( l_uCurIndex < p_tDefaultDb.uNEle ) )
            {
                /* Get curr ele */
                l_tCurEle = p_tDefaultDb.ptDefEle[l_uCurIndex];

                /* Element check */
                if( ( 0u == l_tCurEle.uEleV ) || ( 0u == l_tCurEle.uEleL ) || ( NULL == l_tCurEle.puEleRaw ) ||
                    ( ( l_tCurEle.uEleL + EFSS_DB_RAWOFF ) > p_uPageL ) )
                {
                    /* Cannot be */
                    l_bRes = false;
                }
                else
                {
                    /* Check if can be placed in the current "page" */
                    if( ( l_tCurEle.uEleL + EFSS_DB_RAWOFF ) > ( p_uPageL - l_uCurPageUsed ) )
                    {
                        /* Cannot be placed in this page */
                        l_uCurPage++;
                        l_uCurPageUsed = ( l_tCurEle.uEleL + EFSS_DB_RAWOFF );
                    }
                    else
                    {
                        /* can be placed in this "page " */
                        l_uCurPageUsed += ( l_tCurEle.uEleL + EFSS_DB_RAWOFF );
                    }

                    /* Check if all ok */
                    if( l_uCurPageUsed > p_uNPage )
                    {
                        /* Cannot be */
                        l_bRes = false;
                    }
                    else
                    {
                        /* Increase index */
                        l_uCurIndex++;
                    }
                }
            }
        }
    }

    return l_bRes;
}

static e_eFSS_DB_RES eFSS_DB_GetEleRawInBuffer(uint8_t* const p_puBuff, const uint32_t p_uMaxL, const uint32_t p_uPageL,
                                               t_eFSS_DB_DbElement* const p_ptElemToGet, uint32_t* const p_puUsedL)
{
    /* Local variable for result */
    e_eFSS_DB_RES l_eRes;

    /* Check null pointer */
    if( ( NULL == p_puBuff ) || ( NULL == p_puUsedL ) || ( NULL == p_ptElemToGet ) )
    {
        l_eRes = e_eFSS_DB_RES_BADPOINTER;
    }
    else
    {
        /* Check parameter */
        if( ( p_uMaxL <= EFSS_DB_RAWOFF ) || ( p_uPageL < EFSS_DB_MINPAGESIZE ) )
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
        }
        else
        {
            /* can be retrived, for now */
            if( true != eFSS_Utils_RetriveU16(&p_puBuff[EFSS_DB_VEROFF], &p_ptElemToGet->uEleV) )
            {
                l_eRes = e_eFSS_DB_RES_CORRUPTCTX;
            }
            else
            {
                if( true != eFSS_Utils_RetriveU16(&p_puBuff[EFSS_DB_LENOFF], &p_ptElemToGet->uEleL) )
                {
                    l_eRes = e_eFSS_DB_RES_CORRUPTCTX;
                }
                else
                {
                    /* Check that the element has valid element */
                    if( ( 0u == p_ptElemToGet->uEleV ) || ( 0u == p_ptElemToGet->uEleL ) )
                    {
                        l_eRes = e_eFSS_DB_RES_NOTVALIDDB;
                    }
                    else
                    {
                        /* Check if element length seems valid */
                        if( ( p_ptElemToGet->uEleL + EFSS_DB_RAWOFF ) > p_uPageL )
                        {
                            l_eRes = e_eFSS_DB_RES_NOTVALIDDB;
                        }
                        else
                        {
                            /* Now that we have all the needed data check if it can be retrived */
                            if( ( p_ptElemToGet->uEleL + EFSS_DB_RAWOFF ) > p_uMaxL )
                            {
                                l_eRes = e_eFSS_DB_RES_NOTVALIDDB;
                            }
                            else
                            {
                                /* Can be retrived, copy the raw data */
                                (void)memcpy(p_ptElemToGet->puEleRaw, &p_puBuff[EFSS_DB_RAWOFF], p_ptElemToGet->uEleL);

                                /* Update used byte */
                                *p_puUsedL = ( p_ptElemToGet->uEleL + EFSS_DB_RAWOFF );

                                /* All ok */
                                l_eRes = e_eFSS_DB_RES_OK;
                            }
                        }
                    }
                }
            }
        }
    }

    return l_eRes;
}

static e_eFSS_DB_RES eFSS_DB_SetEleRawInBuffer(uint8_t* const p_puBuff, const uint32_t p_uMaxL,
                                               const t_eFSS_DB_DbElement p_tEleToSet, uint32_t* const p_puUsedL)
{
    /* Local variable for result */
    e_eFSS_DB_RES l_eRes;

    /* Check null pointer */
    if( ( NULL == p_puBuff ) || ( NULL == p_puUsedL ) || ( NULL == p_tEleToSet.puEleRaw ) )
    {
        l_eRes = e_eFSS_DB_RES_BADPOINTER;
    }
    else
    {
        /* Check parameter */
        if( ( 0 == p_uMaxL ) || ( 0u == p_tEleToSet.uEleV ) || ( 0u == p_tEleToSet.uEleL ) )
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
        }
        else
        {
            /* Init local variable */
            l_eRes = e_eFSS_DB_RES_OK;

            /* First, check if we have space for this parameter */
            if( ( p_tEleToSet.uEleL + EFSS_DB_RAWOFF ) > p_uMaxL )
            {
                /* Cannot be placed */
                *p_puUsedL = 0u;
            }
            else
            {
                /* can be placed, place */
                if( true != eFSS_Utils_InsertU16(&p_puBuff[EFSS_DB_VEROFF], p_tEleToSet.uEleV) )
                {
                    l_eRes = e_eFSS_DB_RES_CORRUPTCTX;
                }
                else
                {
                    if( true != eFSS_Utils_InsertU16(&p_puBuff[EFSS_DB_LENOFF], p_tEleToSet.uEleL) )
                    {
                        l_eRes = e_eFSS_DB_RES_CORRUPTCTX;
                    }
                    else
                    {
                        /* Copy the raw data */
                        (void)memcpy(&p_puBuff[EFSS_DB_RAWOFF], p_tEleToSet.puEleRaw, p_tEleToSet.uEleL);

                        /* Update used byte */
                        *p_puUsedL = ( p_tEleToSet.uEleL + EFSS_DB_RAWOFF );
                    }
                }
            }
        }
    }

    return l_eRes;
}

static e_eFSS_DB_RES eFSS_DB_FindElePageAndPos(const uint32_t p_uPageL, const t_eFSS_DB_DbStruct p_tDbStruct,
                                               const uint32_t p_uEleIdx, uint32_t* const p_puPFound,
                                               uint32_t* const p_puOffSetFound)
{
    /* Local variable for result */
    e_eFSS_DB_RES l_eRes;

    /* Local variable used for calculation */
    uint32_t l_uCurPage;
    uint32_t l_uCurPageUsed;
    uint32_t l_uCurIndex;
    t_eFSS_DB_DbElement l_tCurEle;

    /* Check null pointer */
    if( ( NULL == p_puPFound ) || ( NULL == p_puOffSetFound ) || ( NULL == p_tDbStruct.ptDefEle ) )
    {
        l_eRes = e_eFSS_DB_RES_BADPOINTER;
    }
    else
    {
        /* Check element validity */
        if( ( 0u == p_tDbStruct.uNEle ) || ( p_uPageL < EFSS_DB_MINPAGESIZE ) || ( p_uEleIdx > p_tDbStruct.uNEle ) )
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
        }
        else
        {
            /* Init local variable */
            l_eRes = e_eFSS_DB_RES_OK;

            /* Init local variable */
            l_uCurPage = 0u;
            l_uCurPageUsed = 0u;
            l_uCurIndex = 0u;

            /* Sum all other parameter of the DB till we reach the wanted one */
            while( ( l_uCurIndex < p_uEleIdx ) && ( e_eFSS_DB_RES_OK == l_eRes ) )
            {
                /* Get current element */
                l_tCurEle = p_tDbStruct.ptDefEle[l_uCurIndex];

                /* Just check element validty, not needed but misra check */
                if( ( l_tCurEle.uEleL + EFSS_DB_RAWOFF ) > p_uPageL )
                {
                    /* This DB struct seems wrong */
                    l_eRes = e_eFSS_DB_RES_BADPARAM;
                }
                else
                {
                    /* Check if can be placed in the current "page" */
                    if( ( l_tCurEle.uEleL + EFSS_DB_RAWOFF ) > ( p_uPageL - l_uCurPageUsed ) )
                    {
                        /* Cannot be placed in this page */
                        l_uCurPage++;
                        l_uCurPageUsed = ( l_tCurEle.uEleL + EFSS_DB_RAWOFF );
                    }
                    else
                    {
                        /* can be placed in this "page " */
                        l_uCurPageUsed += ( l_tCurEle.uEleL + EFSS_DB_RAWOFF );
                    }

                    /* Increase index */
                    l_uCurIndex++;
                }
            }

            if( e_eFSS_DB_RES_OK == l_eRes )
            {
                /* Now just check if the current element can be present in this page */
                /* Get current element */
                l_tCurEle = p_tDbStruct.ptDefEle[l_uCurIndex];

                /* Check if can be placed in the current "page" */
                if( ( l_tCurEle.uEleL + EFSS_DB_RAWOFF ) > ( p_uPageL - l_uCurPageUsed ) )
                {
                    /* Cannot be placed in this page */
                    l_uCurPage++;
                    l_uCurPageUsed = 0u;
                }

                /* Valorize ret value */
                *p_puPFound = l_uCurPage;
                *p_puOffSetFound = l_uCurPageUsed;
            }
        }
    }

    return l_eRes;
}