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
 *      PRIVATE DEFINE
 **********************************************************************************************************************/
typedef enum
{
    e_eFSS_DB_RES_CHECK_ALREADYADDED = 0,
    e_eFSS_DB_RES_CHECK_NODATA,
    e_eFSS_DB_RES_CHECK_NODATAANDAFTERNEWADDED,
    e_eFSS_DB_RES_CHECK_NEWADDED,
    e_eFSS_DB_RES_CHECK_FINISH
}e_eFSS_DB_PRVSMCHECK_RES;



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

static e_eFSS_DB_RES eFSS_DB_GetEleRawInBuffer(const uint16_t p_uExpEleL, uint8_t* const p_puBuff,
                                               t_eFSS_DB_DbElement* const p_ptEleToGet);

static e_eFSS_DB_RES eFSS_DB_SetEleRawInBuffer(const t_eFSS_DB_DbElement p_tEleToSet, uint8_t* const p_puBuff);

static e_eFSS_DB_RES eFSS_DB_FindElePageAndPos(const uint32_t p_uPageL, const t_eFSS_DB_DbStruct p_tDbDefault,
                                               const uint32_t p_uEleIdx, uint32_t* const p_puPageFound,
                                               uint32_t* const p_puOffSetFound);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_DB_RES eFSS_DB_InitCtx(t_eFSS_DB_Ctx* const p_ptCtx, const t_eFSS_TYPE_CbStorCtx p_tCtxCb,
                              const t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff, const uint32_t p_uBuffL,
                              const t_eFSS_DB_DbStruct p_tDbStruct)
{
    /* Return local var */
    e_eFSS_DB_RES l_eRes;
    e_eFSS_DBC_RES l_eDBCRes;

    /* Local variable for storage */
    t_eFSS_DBC_StorBuf l_tBuff;
    uint32_t l_uTotPage;

    /* Local var used for calculation */
    bool_t l_bIsDbStructValid;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_DB_RES_BADPOINTER;
	}
	else
	{
        /* Can init low level context, and after get the used space and check database validity */
        l_eDBCRes = eFSS_DBC_InitCtx(&p_ptCtx->tDbcCtx, p_tCtxCb, p_tStorSet, p_puBuff, p_uBuffL);
        l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

        if( e_eFSS_DB_RES_OK == l_eRes )
        {
            /* Get usable pages and buffer length so we can check database default value validity */
            l_uTotPage = 0u;
            l_eDBCRes = eFSS_DBC_GetBuffNUsable(&p_ptCtx->tDbcCtx, &l_tBuff, &l_uTotPage);
            l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

            if( e_eFSS_DB_RES_OK == l_eRes )
            {
                /* Check if page length is OK */
                if( ( l_uTotPage <= 0u ) || ( l_tBuff.uBufL < EFSS_DB_MINPAGESIZE ) )
                {
                    /* We need more space for the DB */
                    l_eRes = e_eFSS_DB_RES_BADPARAM;

                    /* De init DBC */
                    (void)memset(&p_ptCtx->tDbcCtx, 0, sizeof(t_eFSS_DBC_Ctx));
                    (void)memset(&p_ptCtx->tDB,     0, sizeof(t_eFSS_DB_DbStruct));
                    p_ptCtx->bIsDbCheked = false;
                }
                else
                {
                    /* Check validity of the passed db struct */
                    l_bIsDbStructValid = eFSS_DB_IsDbDefStructValid(p_tDbStruct, l_uTotPage, l_tBuff.uBufL);

                    if( false == l_bIsDbStructValid )
                    {
                        /* Not valid struct */
                        l_eRes = e_eFSS_DB_RES_BADPARAM;

                        /* De init DBC, we need a valid DB */
                        (void)memset(&p_ptCtx->tDbcCtx, 0, sizeof(t_eFSS_DBC_Ctx));
                        (void)memset(&p_ptCtx->tDB,     0, sizeof(t_eFSS_DB_DbStruct));
                        p_ptCtx->bIsDbCheked = false;
                    }
                    else
                    {
                        /* All ok, fill context */
                        p_ptCtx->tDB = p_tDbStruct;

                        /* Set to false for the first operation. The user must check the DB before using it in order
                           to controll that the stored DB is valid, that new version are updated and to set to default
                           value new parameters */
                        p_ptCtx->bIsDbCheked = false;
                    }
                }
            }
            else
            {
                /* De init DBC */
                (void)memset(&p_ptCtx->tDbcCtx, 0, sizeof(t_eFSS_DBC_Ctx));
                (void)memset(&p_ptCtx->tDB,     0, sizeof(t_eFSS_DB_DbStruct));
                p_ptCtx->bIsDbCheked = false;
            }
        }
    }

    return l_eRes;
}

e_eFSS_DB_RES eFSS_DB_IsInit(t_eFSS_DB_Ctx* const p_ptCtx, bool_t* const p_pbIsInit)
{
	/* Local variable */
    e_eFSS_DB_RES l_eRes;
    e_eFSS_DBC_RES l_eDBCRes;

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
	/* Return local var */
    e_eFSS_DB_RES l_eRes;
    e_eFSS_DBC_RES l_eDBCRes;

    /* Local var for init */
    bool_t l_bIsInit;

    /* Local variable for storage */
    t_eFSS_DBC_StorBuf l_tBuff;
    uint32_t l_uTotPage;

    /* Page and offset local variable */
    uint32_t l_uPageGlob;
    uint32_t l_uOffGlob;
    uint32_t l_uPageLoc;
    uint32_t l_uOffLoc;

    /* Local variable for element check */
    uint32_t l_uCheckedElem;

    /* DB element local variable */
    t_eFSS_DB_DbElement l_tCurEle;
    t_eFSS_DB_DbElement l_tGettedEle;

    /* Local variable to keep track of DB status */
    bool_t l_bIsPageModGlob;
    bool_t l_bIsPageModLoc;

    /* Local variable to keep track of recoveredPage */
    bool_t l_bIsPageRecFromBkup;

    /* Check status local variable */
    e_eFSS_DB_PRVSMCHECK_RES l_eCurStatus;

    /* Support buff */
    uint8_t* l_puSupBuf;

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
                    l_uTotPage = 0u;
                    l_eDBCRes = eFSS_DBC_GetBuffNUsable(&p_ptCtx->tDbcCtx, &l_tBuff, &l_uTotPage);
                    l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                    if( e_eFSS_DB_RES_OK == l_eRes )
                    {
                        /* Now that we have every needed data and that the default database struct seems correct
                           we need to check that the database actualy stored in storage is correct. Keep in mind
                           that unused storage area must be set to zero, in this way if anothers entry is added
                           to the default struct we can set the default value of the entry that are actualy stored
                           with length of zero byte.
                           So we must follow this steps:
                           1 - Read a storage page and check every entry present in the just readed area. For every
                               entry check that parameter legth match the default vaue stored in the database. If
                               legth dosent match go to point 2. If parameter legth match check parameter version.
                               If parameter version dosent match update the stored param with the default value
                               and save the modified storage page.
                           2 - Check that every remaining byte stored in all the remainings page are set to zero.
                               If they differ from zero the database is invalid. If they are all zero we can assume
                               that every parameter that we need to check are new parameter and they must be stored
                               in the storage area.
                            Keep in mind that the pourpose of this function is to: update new parameter version,
                            update newly added parameter and check if parameter are correct. If the database is
                            corrupted, we can ripristinate it only calling eFSS_DB_FormatToDefault */

                        /* Init variable */
                        l_uPageGlob = 0u;
                        l_uOffGlob = 0u;
                        l_uCheckedElem = 0u;
                        l_bIsPageModGlob = false;
                        l_bIsPageRecFromBkup = false;
                        l_eCurStatus = e_eFSS_DB_RES_CHECK_ALREADYADDED;

                        /* Execute the check till we have finished */
                        while ( e_eFSS_DB_RES_CHECK_FINISH != l_eCurStatus )
                        {
                            /* Do check using current status */
                            switch( l_eCurStatus )
                            {
                                case e_eFSS_DB_RES_CHECK_ALREADYADDED:
                                {
                                    /* Check every parameter in every page */
                                    while( ( l_uPageGlob < l_uTotPage ) &&
                                           ( e_eFSS_DB_RES_CHECK_ALREADYADDED == l_eCurStatus ) )
                                    {
                                        /* Init page check, so we know if the page need to be stored after a mod */
                                        l_bIsPageModLoc = false;

                                        /* Load the current page */
                                        l_eDBCRes = eFSS_DBC_LoadPageInBuff(&p_ptCtx->tDbcCtx, l_uPageGlob);
                                        l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                                        if( ( e_eFSS_DB_RES_OK == l_eRes ) || ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) )
                                        {
                                            if( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes )
                                            {
                                                l_bIsPageRecFromBkup = true;
                                            }

                                            /* Init variable */
                                            l_uOffGlob = 0u;

                                            /* Whole page just readed, check the page against the default value */
                                            while( ( l_uOffGlob < l_tBuff.uBufL ) &&
                                                   ( e_eFSS_DB_RES_CHECK_ALREADYADDED == l_eCurStatus ) )
                                            {
                                                if( l_uCheckedElem < p_ptCtx->tDB.uNEle )
                                                {
                                                    /* Get current parameter to check */
                                                    l_tCurEle = p_ptCtx->tDB.ptDefEle[l_uCheckedElem];

                                                    /* Check parameter */
                                                    /* Is the checked parameter present in this page? */
                                                    if( ( l_uOffGlob + l_tCurEle.uEleL + EFSS_DB_RAWOFF ) <=
                                                        l_tBuff.uBufL )
                                                    {
                                                        /* Can retrive the element */
                                                        l_eRes = eFSS_DB_GetEleRawInBuffer( l_tCurEle.uEleL,
                                                                                            &l_tBuff.puBuf[l_uOffGlob],
                                                                                            &l_tGettedEle);
                                                        if( e_eFSS_DB_RES_OK == l_eRes )
                                                        {
                                                            /* Check parameter */
                                                            if( l_tCurEle.uEleV != l_tGettedEle.uEleV )
                                                            {
                                                                /* Need to update this entry */
                                                                l_puSupBuf = &l_tBuff.puBuf[l_uOffGlob];
                                                                l_eRes = eFSS_DB_SetEleRawInBuffer( l_tCurEle,
                                                                                                    l_puSupBuf );
                                                                if( e_eFSS_DB_RES_OK == l_eRes )
                                                                {
                                                                    /* Updated */
                                                                    l_uCheckedElem++;
                                                                    l_uOffGlob += ( l_tCurEle.uEleL + EFSS_DB_RAWOFF );
                                                                    l_bIsPageModLoc = true;
                                                                    l_bIsPageModGlob = true;
                                                                }
                                                                else
                                                                {
                                                                    /* Irreparable error */
                                                                    l_eCurStatus = e_eFSS_DB_RES_CHECK_FINISH;
                                                                }
                                                            }
                                                            else
                                                            {
                                                                /* DB entry is ok, nothing to do */
                                                                l_uCheckedElem++;
                                                                l_uOffGlob += ( l_tCurEle.uEleL + EFSS_DB_RAWOFF );
                                                            }
                                                        }
                                                        else if( e_eFSS_DB_RES_NOTVALIDDB == l_eRes )
                                                        {
                                                            /* Current readed parameter has some error, maybe because
                                                               it is a new parameter? Check for zeros */
                                                            l_eCurStatus = e_eFSS_DB_RES_CHECK_NODATAANDAFTERNEWADDED;
                                                        }
                                                        else
                                                        {
                                                            /* Some other kind of error */
                                                            l_eCurStatus = e_eFSS_DB_RES_CHECK_FINISH;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        /* No more space in this page for other parameter, check zero */
                                                        while( l_uOffGlob < l_tBuff.uBufL )
                                                        {
                                                            if( 0u == l_tBuff.puBuf[l_uOffGlob] )
                                                            {
                                                                l_uOffGlob++;
                                                            }
                                                            else
                                                            {
                                                                /* unused memory must be set to zero */
                                                                l_eRes = e_eFSS_DB_RES_NOTVALIDDB;
                                                                l_eCurStatus = e_eFSS_DB_RES_CHECK_FINISH;
                                                            }
                                                        }
                                                    }
                                                }
                                                else
                                                {
                                                    /* No more parameter to check */
                                                    l_eCurStatus = e_eFSS_DB_RES_CHECK_NODATA;
                                                }
                                            }

                                            /* If no error found and some change where made, flush the page */
                                            if( ( e_eFSS_DB_RES_CHECK_ALREADYADDED == l_eCurStatus ) ||
                                                ( e_eFSS_DB_RES_CHECK_NODATA == l_eCurStatus ) ||
                                                ( e_eFSS_DB_RES_CHECK_NODATAANDAFTERNEWADDED == l_eCurStatus ) )
                                            {
                                                /* If page is modified we need to store the new value */
                                                if( true == l_bIsPageModLoc )
                                                {
                                                    l_eDBCRes = eFSS_DBC_FlushBuffInPage(&p_ptCtx->tDbcCtx,
                                                                                         l_uPageGlob);
                                                    l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                                                    if( e_eFSS_DB_RES_OK != l_eRes )
                                                    {
                                                        /* Irreparable error */
                                                        l_eCurStatus = e_eFSS_DB_RES_CHECK_FINISH;
                                                    }
                                                }

                                                if ( e_eFSS_DB_RES_CHECK_ALREADYADDED == l_eCurStatus )
                                                {
                                                    /* Increase current page only in this case*/
                                                    l_uPageGlob++;
                                                }
                                            }
                                        }
                                        else
                                        {
                                            /* Some error during page reading, return */
                                            l_eCurStatus = e_eFSS_DB_RES_CHECK_FINISH;
                                        }
                                    }

                                    if ( e_eFSS_DB_RES_CHECK_ALREADYADDED == l_eCurStatus )
                                    {
                                        /* This means that we have finished the check  */
                                        l_eCurStatus = e_eFSS_DB_RES_CHECK_FINISH;
                                    }

                                    break;
                                }

                                case e_eFSS_DB_RES_CHECK_NODATA:
                                case e_eFSS_DB_RES_CHECK_NODATAANDAFTERNEWADDED:
                                {
                                    /* check that every remaining byte is zero, first get global offset */
                                    l_uPageLoc = l_uPageGlob;
                                    l_uOffLoc = l_uOffGlob;

                                    while( ( ( e_eFSS_DB_RES_CHECK_NODATA == l_eCurStatus ) ||
                                             ( e_eFSS_DB_RES_CHECK_NODATAANDAFTERNEWADDED == l_eCurStatus ) ) &&
                                           ( l_uPageLoc < l_uTotPage ) )
                                    {
                                        /* Load the current page */
                                        l_eDBCRes = eFSS_DBC_LoadPageInBuff(&p_ptCtx->tDbcCtx, l_uPageLoc);
                                        l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                                        if( ( e_eFSS_DB_RES_OK == l_eRes ) || ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) )
                                        {
                                            if( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes )
                                            {
                                                l_bIsPageRecFromBkup = true;
                                            }

                                            /* Check every byte */
                                            while( ( ( e_eFSS_DB_RES_CHECK_NODATA == l_eCurStatus ) ||
                                                     ( e_eFSS_DB_RES_CHECK_NODATAANDAFTERNEWADDED == l_eCurStatus ) ) &&
                                                   ( l_uOffLoc < l_tBuff.uBufL ) )
                                            {
                                                if( 0u == l_tBuff.puBuf[l_uOffLoc] )
                                                {
                                                    /* Ok continue */
                                                    l_uOffLoc++;
                                                }
                                                else
                                                {
                                                    /* Non zero, corrupted database */
                                                    l_eCurStatus = e_eFSS_DB_RES_CHECK_FINISH;
                                                    l_eRes = e_eFSS_DB_RES_NOTVALIDDB;
                                                }
                                            }

                                            /* Reload offset if everything is ok and increase the page to search */
                                            if( ( e_eFSS_DB_RES_CHECK_NODATA == l_eCurStatus ) ||
                                                ( e_eFSS_DB_RES_CHECK_NODATAANDAFTERNEWADDED == l_eCurStatus ) )
                                            {
                                                l_uOffLoc = 0u;
                                                l_uPageLoc++;
                                            }
                                        }
                                        else
                                        {
                                            /* Some error during page reading, return */
                                            l_eCurStatus = e_eFSS_DB_RES_CHECK_FINISH;
                                        }
                                    }

                                    /* If everything went ok continue */
                                    if( e_eFSS_DB_RES_CHECK_NODATA == l_eCurStatus )
                                    {
                                        /* Done */
                                        l_eCurStatus = e_eFSS_DB_RES_CHECK_FINISH;
                                    }
                                    else if( e_eFSS_DB_RES_CHECK_NODATAANDAFTERNEWADDED == l_eCurStatus )
                                    {
                                        /* check for new parameter */
                                        l_eCurStatus = e_eFSS_DB_RES_CHECK_NEWADDED;
                                    }
                                    else
                                    {
                                        /* Operation ended with an error */
                                    }

                                    break;
                                }

                                case e_eFSS_DB_RES_CHECK_NEWADDED:
                                {
                                    /* ok now we have to insert in the storage some new parameters */
                                    while( ( l_uCheckedElem < p_ptCtx->tDB.uNEle ) &&
                                           ( e_eFSS_DB_RES_CHECK_NEWADDED == l_eCurStatus ) )
                                    {
                                        /* Init page check, so we know if the page need to be stored after a mod */
                                        l_bIsPageModLoc = false;

                                        /* Load the current page */
                                        l_eDBCRes = eFSS_DBC_LoadPageInBuff(&p_ptCtx->tDbcCtx, l_uPageGlob);
                                        l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                                        if( ( e_eFSS_DB_RES_OK == l_eRes ) || ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) )
                                        {
                                            if( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes )
                                            {
                                                l_bIsPageRecFromBkup = true;
                                            }

                                            /* Whole page just readed, check the page against the default value */
                                            while( ( l_uOffGlob < l_tBuff.uBufL ) &&
                                                   ( l_uCheckedElem < p_ptCtx->tDB.uNEle ) &&
                                                   ( e_eFSS_DB_RES_CHECK_NEWADDED == l_eCurStatus ) )
                                            {
                                                /* Get current parameter to check */
                                                l_tCurEle = p_ptCtx->tDB.ptDefEle[l_uCheckedElem];

                                                /* Check parameter */
                                                /* Is the checked parameter present in this page? */
                                                if( ( l_uOffGlob + l_tCurEle.uEleL + EFSS_DB_RAWOFF ) <=
                                                    l_tBuff.uBufL )
                                                {
                                                    /* Need to update this entry */
                                                    l_eRes = eFSS_DB_SetEleRawInBuffer( l_tCurEle,
                                                                                        &l_tBuff.puBuf[l_uOffGlob] );
                                                    if( e_eFSS_DB_RES_OK == l_eRes )
                                                    {
                                                        /* Updated */
                                                        l_uCheckedElem++;
                                                        l_uOffGlob += ( l_tCurEle.uEleL + EFSS_DB_RAWOFF );
                                                        l_bIsPageModLoc = true;
                                                        l_bIsPageModGlob = true;
                                                    }
                                                    else
                                                    {
                                                        /* Irreparable error */
                                                        l_eCurStatus = e_eFSS_DB_RES_CHECK_FINISH;
                                                    }
                                                }
                                                else
                                                {
                                                    /* No more space in this page for other parameter */
                                                    l_uOffGlob = l_tBuff.uBufL;
                                                }
                                            }

                                            /* If no error found and some change where made, flush the page */
                                            if( e_eFSS_DB_RES_CHECK_NEWADDED == l_eCurStatus )
                                            {
                                                /* If page is modified we need to store the new value */
                                                if( true == l_bIsPageModLoc )
                                                {
                                                    l_eDBCRes = eFSS_DBC_FlushBuffInPage(&p_ptCtx->tDbcCtx,
                                                                                         l_uPageGlob);
                                                    l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                                                    if( e_eFSS_DB_RES_OK != l_eRes )
                                                    {
                                                        /* Error flushing */
                                                        l_eCurStatus = e_eFSS_DB_RES_CHECK_FINISH;
                                                    }
                                                }

                                                /* current page is full, switch */
                                                /* Check requested from misra */
                                                if( l_uPageGlob < l_uTotPage )
                                                {
                                                    l_uPageGlob++;
                                                }
                                                else
                                                {
                                                    /* Some absurd error */
                                                    l_eCurStatus = e_eFSS_DB_RES_CHECK_FINISH;
                                                    l_eRes = e_eFSS_DB_RES_CORRUPTCTX;
                                                }

                                                l_uOffGlob = 0u;
                                            }
                                        }
                                        else
                                        {
                                            /* Some error during page reading, return */
                                            l_eCurStatus = e_eFSS_DB_RES_CHECK_FINISH;
                                        }
                                    }

                                    /* In any cases we have finished now */
                                    l_eCurStatus = e_eFSS_DB_RES_CHECK_FINISH;

                                    break;
                                }

                                default:
                                {
                                    /* Impossible end here */
                                    l_eCurStatus = e_eFSS_DB_RES_CHECK_FINISH;
                                    l_eRes = e_eFSS_DB_RES_CORRUPTCTX;
                                    break;
                                }
                            }
                        }

                        /* Finaly, if no error occoured update status */
                        if( ( e_eFSS_DB_RES_OK == l_eRes ) ||
                            ( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eRes ) )
                        {
                            /* Whole database checked and it is OK */
                            p_ptCtx->bIsDbCheked = true;

                            /* Specified the returned value */
                            if( true == l_bIsPageModGlob )
                            {
                                /* In this case we have updated some value */
                                l_eRes = e_eFSS_DB_RES_PARAM_DEF_RESET;
                            }
                            else if( true == l_bIsPageRecFromBkup )
                            {
                                /* Signal that a page was recovered from bkup */
                                l_eRes = e_eFSS_DB_RES_OK_BKP_RCVRD;
                            }
                            else
                            {
                                /* All ok perfect */
                                l_eRes = e_eFSS_DB_RES_OK;
                            }
                        }
                        else
                        {
                            /* Database is corrupted, signal it as not cheked so we will not allow any other
                             * modification to this situation */
                            p_ptCtx->bIsDbCheked = false;
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
	/* Return local var */
    e_eFSS_DB_RES l_eRes;
    e_eFSS_DBC_RES l_eDBCRes;

    /* Local var for init */
    bool_t l_bIsInit;

    /* Local variable for storage */
    t_eFSS_DBC_StorBuf l_tBuff;
    uint32_t l_uTotPage;

    /* Local variable for calculation */
    uint32_t l_uCurrPage;
    uint32_t l_uCurOff;
    uint32_t l_uCheckedElem;

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
                    /* no need to check if database was checked, we are formating anyway */

                    /* Get storage info */
                    l_uTotPage = 0u;
                    l_eDBCRes = eFSS_DBC_GetBuffNUsable(&p_ptCtx->tDbcCtx, &l_tBuff, &l_uTotPage);
                    l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                    if( e_eFSS_DB_RES_OK == l_eRes )
                    {
                        /* In order to format to default the DB we can just set to zero the buffer to flush
                           and copy inside it only the needed parameter. If no parameter are needed, we can
                           just flush a zero filled buffer. Flushyng zero as data will leave open the possibility
                           to add new parameter */
                        l_uCurrPage = 0u;
                        l_uCheckedElem = 0u;

                        /* Continue till we have setted all pages or an error occours */
                        while( ( l_uCurrPage < l_uTotPage ) && ( e_eFSS_DB_RES_OK == l_eRes ) )
                        {
                            /* Memset the current page to zero */
                            (void)memset(&l_tBuff.puBuf, 0, l_tBuff.uBufL);

                            /* Set to zero the numbers of byte used */
                            l_uCurOff = 0u;

                            /* Continue till the page is full, parameter are avaiable, or an error occours */
                            while( ( l_uCurOff < l_tBuff.uBufL )  &&
                                   ( l_uCheckedElem < p_ptCtx->tDB.uNEle ) &&
                                   ( e_eFSS_DB_RES_OK == l_eRes ) )
                            {
                                /* We have some element to set, can be placed here? */
                                if( ( l_uCurOff + p_ptCtx->tDB.ptDefEle[l_uCheckedElem].uEleL +
                                      EFSS_DB_RAWOFF ) > l_tBuff.uBufL )
                                {
                                    /* This element cannot be stored in this buffer. We will store it in the next
                                     * page */
                                    l_uCurOff = l_tBuff.uBufL;
                                }
                                else
                                {
                                    /* This element can be stored in this buffer */
                                    l_eRes = eFSS_DB_SetEleRawInBuffer(p_ptCtx->tDB.ptDefEle[l_uCheckedElem],
                                                                       &l_tBuff.puBuf[l_uCurOff] );
                                    if( e_eFSS_DB_RES_OK == l_eRes )
                                    {
                                        /* Element inserted, increase the offset and start elaborating the next
                                         * element */
                                        l_uCurOff += ( p_ptCtx->tDB.ptDefEle[l_uCheckedElem].uEleL +
                                                       EFSS_DB_RAWOFF );
                                        l_uCheckedElem++;
                                    }
                                }
                            }

                            /* Page is completed, we can flush */
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

                        if( e_eFSS_DB_RES_OK == l_eRes )
                        {
                            /* Setted to default every entry, and we are sure that DB is correct. No need to do
                               additional check */
                            p_ptCtx->bIsDbCheked = true;
                        }
                    }
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_DB_RES eFSS_DB_SaveElemen(t_eFSS_DB_Ctx* const p_ptCtx, const uint32_t p_uPos, const uint16_t p_uRawValL,
                                 uint8_t* const p_puRawVal)
{
	/* Return local var */
    e_eFSS_DB_RES l_eRes;
    e_eFSS_DB_RES l_eResLoad;
    e_eFSS_DBC_RES l_eDBCRes;

    /* Local var for init */
    bool_t l_bIsInit;

    /* Local variable for storage */
    t_eFSS_DBC_StorBuf l_tBuff;
    uint32_t l_uTotPage;

    /* Local variable for calculation */
    uint32_t l_uPageIdx;
    uint32_t l_uCurOff;
    t_eFSS_DB_DbElement l_tCurEle;

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
                    /* First time calling a function we need to check for the whole stored integrity.
                     * We need to do this check to be sure that the DB version is not increased, to be sure that
                     * parameter with updated version are setted to default value and that new parameter are
                     * initialized */
                    if( false == p_ptCtx->bIsDbCheked )
                    {
                        /* Check status */
                        l_eRes = e_eFSS_DB_RES_DBNOTCHECKED;
                    }
                    else
                    {
                        /* Verify if parameter is ok checked against the DB */
                        if( ( p_uPos >= p_ptCtx->tDB.uNEle ) || ( p_uRawValL != p_ptCtx->tDB.ptDefEle[p_uPos].uEleL ) )
                        {
                            l_eRes = e_eFSS_DB_RES_BADPARAM;
                        }
                        else
                        {
                            /* Get storage info */
                            l_eDBCRes = eFSS_DBC_GetBuffNUsable(&p_ptCtx->tDbcCtx, &l_tBuff, &l_uTotPage);
                            l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                            if( e_eFSS_DB_RES_OK == l_eRes )
                            {
                                /* Find the page and page index where to save the data */
                                l_eRes = eFSS_DB_FindElePageAndPos(l_tBuff.uBufL, p_ptCtx->tDB, p_uPos,
                                                                   &l_uPageIdx, &l_uCurOff);

                                if( e_eFSS_DB_RES_OK == l_eRes )
                                {
                                    /* Load the page where we can find the needed element */
                                    l_eDBCRes = eFSS_DBC_LoadPageInBuff(&p_ptCtx->tDbcCtx, l_uPageIdx);
                                    l_eResLoad = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                                    if( ( e_eFSS_DB_RES_OK != l_eResLoad ) &&
                                        ( e_eFSS_DB_RES_OK_BKP_RCVRD != l_eResLoad ) )
                                    {
                                        l_eRes = l_eResLoad;
                                    }
                                    else
                                    {
                                        /* Verify if the already stored element is correct */
                                        (void)memset(&l_tCurEle, 0, sizeof(t_eFSS_DB_DbElement) );

                                        l_eRes = eFSS_DB_GetEleRawInBuffer( p_ptCtx->tDB.ptDefEle[p_uPos].uEleL,
                                                                            &l_tBuff.puBuf[l_uCurOff],
                                                                            &l_tCurEle);

                                        if( e_eFSS_DB_RES_OK == l_eRes )
                                        {
                                            /* At this point the database should be already checked,
                                               but just to be sure check if previous stored param has correct
                                               version and length */
                                            if( l_tCurEle.uEleV != p_ptCtx->tDB.ptDefEle[p_uPos].uEleV )
                                            {
                                                /* The database is incoherent, the version should match because
                                                 * the function eFSS_DB_GetDBStatus was already called */
                                                l_eRes = e_eFSS_DB_RES_NOTVALIDDB;
                                            }
                                            else
                                            {
                                                /* Ok, the previously saved element seems ok, save it */
                                                l_tCurEle.uEleL = p_ptCtx->tDB.ptDefEle[p_uPos].uEleL;
                                                l_tCurEle.uEleV = p_ptCtx->tDB.ptDefEle[p_uPos].uEleV;
                                                l_tCurEle.puEleRaw = p_puRawVal;

                                                l_eRes = eFSS_DB_SetEleRawInBuffer(l_tCurEle,
                                                                                   &l_tBuff.puBuf[l_uCurOff] );

                                                if( e_eFSS_DB_RES_OK == l_eRes )
                                                {
                                                    /* Ok, so we have updated the loaded buffer with
                                                       the needed raw data that we want to save. Flush the page */
                                                    l_eDBCRes = eFSS_DBC_FlushBuffInPage(&p_ptCtx->tDbcCtx,
                                                                                         l_uCurOff);
                                                    l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                                                    if( e_eFSS_DB_RES_OK == l_eRes )
                                                    {
                                                        /* Check if we had some problem loading the buffer */
                                                        if( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eResLoad )
                                                        {
                                                            l_eRes = e_eFSS_DB_RES_OK_BKP_RCVRD;
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
        }
	}

	return l_eRes;
}

e_eFSS_DB_RES eFSS_DB_GetElement(t_eFSS_DB_Ctx* const p_ptCtx, const uint32_t p_uPos, const uint16_t p_uRawValL,
                                 uint8_t* const p_puRawVal)
{
	/* Return local var */
    e_eFSS_DB_RES l_eRes;
    e_eFSS_DB_RES l_eResLoad;
    e_eFSS_DBC_RES l_eDBCRes;

    /* Local var for init */
    bool_t l_bIsInit;

    /* Local variable for storage */
    t_eFSS_DBC_StorBuf l_tBuff;
    uint32_t l_uTotPage;

    /* Local variable for calculation */
    uint32_t l_uPageIdx;
    uint32_t l_uCurOff;
    t_eFSS_DB_DbElement l_tCurEle;

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
                    /* First time calling a function we need to check for the whole storage integrity.
                     * We need to do this check to be sure that the DB version is not increased, to be sure that
                     * parameter with updated version are setted to default value and that new parameter are
                     * initialized */
                    if( false == p_ptCtx->bIsDbCheked )
                    {
                        /* Check status before executing action */
                        l_eRes = e_eFSS_DB_RES_DBNOTCHECKED;
                    }
                    else
                    {
                        /* Verify if parameter is ok checked against the DB */
                        if( ( p_uPos >= p_ptCtx->tDB.uNEle ) || ( p_uRawValL != p_ptCtx->tDB.ptDefEle[p_uPos].uEleL ) )
                        {
                            l_eRes = e_eFSS_DB_RES_BADPARAM;
                        }
                        else
                        {
                            /* Get storage info */
                            l_eDBCRes = eFSS_DBC_GetBuffNUsable(&p_ptCtx->tDbcCtx, &l_tBuff, &l_uTotPage);
                            l_eRes = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                            if( e_eFSS_DB_RES_OK == l_eRes )
                            {
                                /* Find the page and page index where to get the data */
                                l_eRes = eFSS_DB_FindElePageAndPos(l_tBuff.uBufL, p_ptCtx->tDB, p_uPos,
                                                                   &l_uPageIdx, &l_uCurOff);

                                if( e_eFSS_DB_RES_OK == l_eRes )
                                {
                                    /* Load the page where we can find the needed element */
                                    l_eDBCRes = eFSS_DBC_LoadPageInBuff(&p_ptCtx->tDbcCtx, l_uPageIdx);
                                    l_eResLoad = eFSS_DB_DBCtoDBRes(l_eDBCRes);

                                    if( ( e_eFSS_DB_RES_OK != l_eResLoad ) &&
                                        ( e_eFSS_DB_RES_OK_BKP_RCVRD != l_eResLoad ) )
                                    {
                                        l_eRes = l_eResLoad;
                                    }
                                    else
                                    {
                                        /* Verify if the already stored element is correct */
                                        (void)memset(&l_tCurEle, 0, sizeof(t_eFSS_DB_DbElement) );

                                        /* Get element reference */
                                        l_eRes = eFSS_DB_GetEleRawInBuffer( p_ptCtx->tDB.ptDefEle[p_uPos].uEleL,
                                                                            &l_tBuff.puBuf[l_uCurOff],
                                                                            &l_tCurEle);

                                        if( e_eFSS_DB_RES_OK == l_eRes )
                                        {
                                            /* At this point the database should be already checked,
                                               but just to be sure check if previous stored param has correct
                                               version and length */
                                            if( l_tCurEle.uEleV != p_ptCtx->tDB.ptDefEle[p_uPos].uEleV )
                                            {
                                                /* The database is incoherent, the version should match because
                                                 * the function eFSS_DB_GetDBStatus was already called */
                                                l_eRes = e_eFSS_DB_RES_NOTVALIDDB;
                                            }
                                            else
                                            {
                                                /* Can copy the element */
                                                (void)memcpy(p_puRawVal, l_tCurEle.puEleRaw, (uint32_t)p_uRawValL);

                                                if( e_eFSS_DB_RES_OK_BKP_RCVRD == l_eResLoad )
                                                {
                                                    l_eRes = e_eFSS_DB_RES_OK_BKP_RCVRD;
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



/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eFSS_DB_IsStatusStillCoherent(t_eFSS_DB_Ctx* const p_ptCtx)
{
    /* Return local var */
    bool_t l_eRes;
    e_eFSS_DBC_RES l_eDBCRes;

    /* Local variable for storage */
    t_eFSS_DBC_StorBuf l_tBuff;
    uint32_t l_uTotPage;

    /* Get usable pages and buffer length so we can check database default value validity */
    l_uTotPage = 0u;
    l_eDBCRes = eFSS_DBC_GetBuffNUsable(&p_ptCtx->tDbcCtx, &l_tBuff, &l_uTotPage);

    if( e_eFSS_DBC_RES_OK != l_eDBCRes )
    {
        l_eRes = false;
    }
    else
    {
        /* Check data validity */
        if( ( l_uTotPage <= 0u ) || ( l_tBuff.uBufL < EFSS_DB_MINPAGESIZE ) )
        {
            l_eRes = false;
        }
        else
        {
            /* Check validity of the passed db struct */
            l_eRes = eFSS_DB_IsDbDefStructValid(p_ptCtx->tDB, l_uTotPage, l_tBuff.uBufL);
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

        case e_eFSS_DBC_RES_NEWVERSIONFOUND:
        {
            l_eRes = e_eFSS_DB_RES_NEWVERSIONFOUND;
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
       1- DB array must be valid and not NULL
       2- Numbers of element need to be different from zero
       3- Each element version and len need to be different from zero
       4- Element raw data must be different from NULL
       5- An element length cannot be greater than page length
       6- All element must be able to be stored in database
     */

    /* Local variable for return */
    bool_t l_bRes;

    /* Local variable used for index */
    uint32_t l_uCurPage;
    uint32_t l_uCurOff;

    /* Local variable for calculation */
    uint32_t l_uCurIndex;
    t_eFSS_DB_DbElement l_tCurEle;

    /* Check pointer validity */
    if( NULL == p_tDefaultDb.ptDefEle )
    {
        l_bRes = false;
    }
    else
    {
        if( p_tDefaultDb.uNEle <= 0u )
        {
            l_bRes = false;
        }
        else
        {
            /* Init local */
            l_uCurIndex = 0u;
            l_uCurPage = 0u;
            l_uCurOff = 0u;
            l_bRes = true;

            /* Check every parameter till an error is found */
            while( ( l_uCurIndex < p_tDefaultDb.uNEle ) && ( true == l_bRes ) )
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
                    if( ( l_uCurOff + l_tCurEle.uEleL + EFSS_DB_RAWOFF ) > p_uPageL )
                    {
                        /* Cannot be placed in this page */
                        l_uCurPage++;
                        l_uCurOff = ( l_tCurEle.uEleL + EFSS_DB_RAWOFF );
                    }
                    else
                    {
                        /* can be placed in this "page " */
                        l_uCurOff += ( l_tCurEle.uEleL + EFSS_DB_RAWOFF );
                    }

                    /* Check if all ok */
                    if( l_uCurPage >= p_uNPage )
                    {
                        /* Cannot be, we don't have space for storing all parameter */
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

static e_eFSS_DB_RES eFSS_DB_GetEleRawInBuffer(const uint16_t p_uExpEleL, uint8_t* const p_puBuff,
                                               t_eFSS_DB_DbElement* const p_ptEleToGet)
{
    /* Local variable for result */
    e_eFSS_DB_RES l_eRes;

    /* Check null pointer */
    if( ( NULL == p_puBuff ) || ( NULL == p_ptEleToGet ) )
    {
        l_eRes = e_eFSS_DB_RES_BADPOINTER;
    }
    else
    {
        /* Check parameter */
        if( 0u == p_uExpEleL )
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
        }
        else
        {
            /* can be retrived, for now */
            p_ptEleToGet->uEleV = 0u;
            if( true != eFSS_Utils_RetriveU16(&p_puBuff[EFSS_DB_VEROFF], &p_ptEleToGet->uEleV) )
            {
                l_eRes = e_eFSS_DB_RES_CORRUPTCTX;
            }
            else
            {
                p_ptEleToGet->uEleL = 0u;
                if( true != eFSS_Utils_RetriveU16(&p_puBuff[EFSS_DB_LENOFF], &p_ptEleToGet->uEleL ) )
                {
                    l_eRes = e_eFSS_DB_RES_CORRUPTCTX;
                }
                else
                {
                    /* Check that the element has valid parameter */
                    if( ( 0u == p_ptEleToGet->uEleV ) || ( p_uExpEleL != p_ptEleToGet->uEleL ) )
                    {
                        l_eRes = e_eFSS_DB_RES_NOTVALIDDB;
                    }
                    else
                    {
                        /* Can be retrived, copy the raw data */
                        p_ptEleToGet->puEleRaw = &p_puBuff[EFSS_DB_RAWOFF];

                        /* All ok */
                        l_eRes = e_eFSS_DB_RES_OK;
                    }
                }
            }
        }
    }

    return l_eRes;
}

static e_eFSS_DB_RES eFSS_DB_SetEleRawInBuffer(const t_eFSS_DB_DbElement p_tEleToSet, uint8_t* const p_puBuff)
{
    /* Local variable for result */
    e_eFSS_DB_RES l_eRes;

    /* Check null pointer */
    if( ( NULL == p_puBuff ) || ( NULL == p_tEleToSet.puEleRaw ) )
    {
        l_eRes = e_eFSS_DB_RES_BADPOINTER;
    }
    else
    {
        /* Check parameter */
        if( ( 0u == p_tEleToSet.uEleV ) || ( 0u == p_tEleToSet.uEleL ) )
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
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
                    (void)memcpy(&p_puBuff[EFSS_DB_RAWOFF], p_tEleToSet.puEleRaw, (uint32_t)p_tEleToSet.uEleL);

                    /* All OK */
                    l_eRes = e_eFSS_DB_RES_OK;
                }
            }
        }
    }

    return l_eRes;
}

static e_eFSS_DB_RES eFSS_DB_FindElePageAndPos(const uint32_t p_uPageL, const t_eFSS_DB_DbStruct p_tDbDefault,
                                               const uint32_t p_uEleIdx, uint32_t* const p_puPageFound,
                                               uint32_t* const p_puOffSetFound)
{
    /* Local variable for result */
    e_eFSS_DB_RES l_eRes;

    /* Local variable used for index */
    uint32_t l_uCurPage;
    uint32_t l_uCurOff;

    /* Local variable used for calculation */
    uint32_t l_uCurIndex;
    t_eFSS_DB_DbElement l_tCurEle;

    /* Check null pointer */
    if( ( NULL == p_puPageFound ) || ( NULL == p_puOffSetFound ) || ( NULL == p_tDbDefault.ptDefEle ) )
    {
        l_eRes = e_eFSS_DB_RES_BADPOINTER;
    }
    else
    {
        /* Check element validity */
        if( ( p_tDbDefault.uNEle <= 0u ) || ( p_uPageL < EFSS_DB_MINPAGESIZE ) || ( p_uEleIdx >= p_tDbDefault.uNEle ) )
        {
            l_eRes = e_eFSS_DB_RES_BADPARAM;
        }
        else
        {
            /* Init local variable */
            l_eRes = e_eFSS_DB_RES_OK;

            /* Init local variable */
            l_uCurPage = 0u;
            l_uCurOff = 0u;
            l_uCurIndex = 0u;

            /* Sum all other parameter of the DB till we reach the wanted one */
            while( ( l_uCurIndex < p_uEleIdx ) && ( e_eFSS_DB_RES_OK == l_eRes ) && ( l_uCurPage < MAX_UINT32VAL ) )
            {
                /* Get current element */
                l_tCurEle = p_tDbDefault.ptDefEle[l_uCurIndex];

                /* Just check element validty, not needed because we check context validity every time, but
                   who knows */
                if( ( l_tCurEle.uEleL + EFSS_DB_RAWOFF ) > p_uPageL )
                {
                    /* This DB struct seems invalid */
                    l_eRes = e_eFSS_DB_RES_BADPARAM;
                }
                else
                {
                    /* Check if the current element can be placed in the current "page" */
                    if( ( l_uCurOff + l_tCurEle.uEleL + EFSS_DB_RAWOFF ) > p_uPageL )
                    {
                        /* current element cannot be placed in this page */
                        l_uCurPage++;
                        l_uCurOff = ( l_tCurEle.uEleL + EFSS_DB_RAWOFF );
                    }
                    else
                    {
                        /* can be placed in this "page " */
                        l_uCurOff += ( l_tCurEle.uEleL + EFSS_DB_RAWOFF );
                    }

                    /* Increase index */
                    l_uCurIndex++;
                }
            }

            if( l_uCurPage >= MAX_UINT32VAL )
            {
                l_eRes = e_eFSS_DB_RES_BADPARAM;
            }
            else
            {
                if( e_eFSS_DB_RES_OK == l_eRes )
                {
                    /* Finded the page and the offset of the index we were looking for. We just need to check if
                       the findeded element can be present on the current pages, if not increase it */
                    l_tCurEle = p_tDbDefault.ptDefEle[l_uCurIndex];

                    /* Just check element validty, not needed because we check context validity every time, but
                       who knows */
                    if( ( l_tCurEle.uEleL + EFSS_DB_RAWOFF ) > p_uPageL )
                    {
                        /* This DB struct seems invalid */
                        l_eRes = e_eFSS_DB_RES_BADPARAM;
                    }
                    else
                    {
                        /* Check if the current element can be placed in the current "page" */
                        if( ( l_uCurOff + l_tCurEle.uEleL + EFSS_DB_RAWOFF ) > p_uPageL )
                        {
                            /* Cannot be placed in this page */
                            l_uCurPage++;
                            l_uCurOff = 0u;
                        }

                        /* Valorize ret value */
                        *p_puPageFound = l_uCurPage;
                        *p_puOffSetFound = l_uCurOff;
                    }
                }
            }
        }
    }

    return l_eRes;
}