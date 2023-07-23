/**
 * @file       eFSS_DB.h
 *
 * @brief      Database module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSS_DB_H
#define EFSS_DB_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_DBC.h"



/***********************************************************************************************************************
 *      DATABASE TYPEDEFS
 **********************************************************************************************************************/
/* Every element in the database has this struct. Keep in mind that a stored element cannot change it's size in
 * it's life but instead, can change it's version. When the version of an entry is changed the first time that the
 * database is checked we will delete the previously stored value and will save it's default value instead.
 * During the life time of the database it's not possible to remove an entry or change an entry size. It's possible
 * to add new entry instead (only if we have enough space to store the entry in the storage area).
 * We are saving element in a single storage page and we are not able to have an element splitted between two page.
 * If we need to remove element or change some element size the only safe way is to increase the version of the
 * storage (not of the element), so when we will check the DB status we will found an invalid database and we can
 * format the database to the default value, in this way all previously stored value will be lost, and default
 * value will be ripristinated */
typedef struct
{
	uint16_t uEleV;
    uint16_t uEleL;
    uint8_t* puEleRaw;
}t_eFSS_DB_DbElement;

typedef struct
{
    uint32_t uNEle;
    t_eFSS_DB_DbElement* ptDefEle;
}t_eFSS_DB_DbStruct;



/***********************************************************************************************************************
 *      PUBLIC TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eFSS_DB_RES_OK = 0,
    e_eFSS_DB_RES_NOINITLIB,
    e_eFSS_DB_RES_BADPARAM,
    e_eFSS_DB_RES_BADPOINTER,
    e_eFSS_DB_RES_CORRUPTCTX,
    e_eFSS_DB_RES_CLBCKERASEERR,
    e_eFSS_DB_RES_CLBCKWRITEERR,
    e_eFSS_DB_RES_CLBCKREADERR,
    e_eFSS_DB_RES_CLBCKCRCERR,
    e_eFSS_DB_RES_NOTVALIDDB,
    e_eFSS_DB_RES_NEWVERSIONFOUND,
    e_eFSS_DB_RES_WRITENOMATCHREAD,
    e_eFSS_DB_RES_OK_BKP_RCVRD,
    e_eFSS_DB_RES_DBNOTCHECKED,
    e_eFSS_DB_RES_PARAM_DEF_RESET
}e_eFSS_DB_RES;

typedef struct
{
    t_eFSS_DBC_Ctx           tDbcCtx;
    t_eFSS_DB_DbStruct       tDB;
    bool_t                   bIsDbCheked;
}t_eFSS_DB_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the Database module context
 *
 * @param[in]   p_ptCtx          - Database context
 * @param[in]   p_tCtxCb         - All callback collection context
 * @param[in]   p_tStorSet       - Storage settings
 * @param[in]   p_puBuff         - Pointer to a buffer used by the modules to make calc, must be pageSize * 2
 * @param[in]   p_uBuffL         - Size of p_puBuff
 * @param[in]   p_tDbStruct      - Struct containing the default struct of the database
 *
 * @return      e_eFSS_DB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DB_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_DB_RES_OK            - Operation ended correctly
 */
e_eFSS_DB_RES eFSS_DB_InitCtx(t_eFSS_DB_Ctx* const p_ptCtx, const t_eFSS_TYPE_CbStorCtx p_tCtxCb,
                              const t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff, const uint32_t p_uBuffL,
                              const t_eFSS_DB_DbStruct p_tDbStruct);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx         - Database context
 * @param[out]  p_pbIsInit      - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_DB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_DB_RES_OK            - Operation ended correctly
 */
e_eFSS_DB_RES eFSS_DB_IsInit(t_eFSS_DB_Ctx* const p_ptCtx, bool_t* const p_pbIsInit);

/**
 * @brief       Check the whole database status. This function must be called before doing anything else with the
 *              the database. This function will update new entry, will check that already stored entry are
 *              correct and not corrupted and will restored entry with a new version.
 *              This function will read all the entry of the database, so i can takes some times to execute.
 *              If e_eFSS_DB_RES_NOTVALIDDB or e_eFSS_DB_RES_NEWVERSIONFOUND are returned it's means that the database
 *              cannot be used untill we use the function called eFSS_DB_FormatToDefault.
 *
 * @param[in]   p_ptCtx          - Database context
 *
 * @return      e_eFSS_DB_RES_BADPOINTER       - In case of bad pointer passed to the function
 *		        e_eFSS_DB_RES_BADPARAM         - In case of an invalid parameter passed to the function
 *		        e_eFSS_DB_RES_CORRUPTCTX       - Context is corrupted
 *		        e_eFSS_DB_RES_NOINITLIB        - Need to init lib before calling function
 *		        e_eFSS_DB_RES_CLBCKREADERR     - The read callback reported an error
 *              e_eFSS_DB_RES_CLBCKCRCERR      - The crc callback reported an error
 *              e_eFSS_DB_RES_NOTVALIDDB       - Database is invalid
 *              e_eFSS_DB_RES_NEWVERSIONFOUND  - The database has a new version
 *              e_eFSS_DB_RES_CLBCKERASEERR    - Error reported from the callback
 *              e_eFSS_DB_RES_CLBCKWRITEERR    - Error reported from the callback
 *              e_eFSS_DB_RES_WRITENOMATCHREAD - For some unknow reason data write dosent match data readed
 *              e_eFSS_DB_RES_OK_BKP_RCVRD     - operation ended successfully recovering a backup or an origin page
 *              e_eFSS_DB_RES_PARAM_DEF_RESET  - Some of the database entry were updated to a new version, or a new
 *                                               entry is added
 *              e_eFSS_DB_RES_OK               - Operation ended correctly
 */
e_eFSS_DB_RES eFSS_DB_GetDBStatus(t_eFSS_DB_Ctx* const p_ptCtx);

/**
 * @brief       Erase all the data present in the DB and restore default value. This function is the only function
 *              that is able to recover a corrupted database or start a new version of the database itself.
 *
 * @param[in]   p_ptCtx    - Database context
 *
 * @return      e_eFSS_DB_RES_BADPOINTER       - In case of bad pointer passed to the function
 *		        e_eFSS_DB_RES_BADPARAM         - In case of an invalid parameter passed to the function
 *		        e_eFSS_DB_RES_CORRUPTCTX       - Context is corrupted
 *		        e_eFSS_DB_RES_NOINITLIB        - Need to init lib before calling function
 *		        e_eFSS_DB_RES_CLBCKREADERR     - The read callback reported an error
 *              e_eFSS_DB_RES_CLBCKCRCERR      - The crc callback reported an error
 *              e_eFSS_DB_RES_NOTVALIDDB       - Database is invalid
 *              e_eFSS_DB_RES_NEWVERSIONFOUND  - The database has a new version
 *              e_eFSS_DB_RES_CLBCKERASEERR    - Error reported from the callback
 *              e_eFSS_DB_RES_CLBCKWRITEERR    - Error reported from the callback
 *              e_eFSS_DB_RES_WRITENOMATCHREAD - For some unknow reason data write dosent match data readed
 *              e_eFSS_DB_RES_OK_BKP_RCVRD     - operation ended successfully recovering a backup or an origin page
 *              e_eFSS_DB_RES_DBNOTCHECKED     - The function eFSS_DB_GetDBStatus needs to be called before and
 *                                               the status of the database need to be ok to proceed
 *              e_eFSS_DB_RES_OK               - Operation ended correctly
 */
e_eFSS_DB_RES eFSS_DB_FormatToDefault(t_eFSS_DB_Ctx* const p_ptCtx);

/**
 * @brief       Save an element in to the database
 *
 * @param[in]   p_ptCtx       - Database context
 * @param[in]   p_uPos        - Position of the element we want to save in to the database
 * @param[in]   p_uRawValL    - Length of the element
 * @param[in]   p_puRawVal    - Raw value of the element we want to save that must be of size of p_uRawValL
 *
 * @return      e_eFSS_DB_RES_BADPOINTER       - In case of bad pointer passed to the function
 *		        e_eFSS_DB_RES_BADPARAM         - In case of an invalid parameter passed to the function
 *		        e_eFSS_DB_RES_CORRUPTCTX       - Context is corrupted
 *		        e_eFSS_DB_RES_NOINITLIB        - Need to init lib before calling function
 *		        e_eFSS_DB_RES_CLBCKREADERR     - The read callback reported an error
 *              e_eFSS_DB_RES_CLBCKCRCERR      - The crc callback reported an error
 *              e_eFSS_DB_RES_NOTVALIDDB       - Database is invalid
 *              e_eFSS_DB_RES_NEWVERSIONFOUND  - The database has a new version
 *              e_eFSS_DB_RES_CLBCKERASEERR    - Error reported from the callback
 *              e_eFSS_DB_RES_CLBCKWRITEERR    - Error reported from the callback
 *              e_eFSS_DB_RES_WRITENOMATCHREAD - For some unknow reason data write dosent match data readed
 *              e_eFSS_DB_RES_OK_BKP_RCVRD     - operation ended successfully recovering a backup or an origin page
 *              e_eFSS_DB_RES_DBNOTCHECKED     - The function eFSS_DB_GetDBStatus needs to be called before and
 *                                               the status of the database need to be ok to proceed
 *              e_eFSS_DB_RES_OK               - Operation ended correctly
 */
e_eFSS_DB_RES eFSS_DB_SaveElemen(t_eFSS_DB_Ctx* const p_ptCtx, const uint32_t p_uPos, const uint16_t p_uRawValL,
                                 uint8_t* const p_puRawVal);

/**
 * @brief       Get an element stored from the database
 *
 * @param[in]   p_ptCtx       - Database context
 * @param[in]   p_uPos        - Position of the element we want to read from the database
 * @param[in]   p_uRawValL    - Length of the element
 * @param[out]  p_puRawVal    - Storage are of size p_uRawValL were we will save the element
 *
 * @return      e_eFSS_DB_RES_BADPOINTER       - In case of bad pointer passed to the function
 *		        e_eFSS_DB_RES_BADPARAM         - In case of an invalid parameter passed to the function
 *		        e_eFSS_DB_RES_CORRUPTCTX       - Context is corrupted
 *		        e_eFSS_DB_RES_NOINITLIB        - Need to init lib before calling function
 *		        e_eFSS_DB_RES_CLBCKREADERR     - The read callback reported an error
 *              e_eFSS_DB_RES_CLBCKCRCERR      - The crc callback reported an error
 *              e_eFSS_DB_RES_NOTVALIDDB       - Database is invalid
 *              e_eFSS_DB_RES_NEWVERSIONFOUND  - The database has a new version
 *              e_eFSS_DB_RES_CLBCKERASEERR    - Error reported from the callback
 *              e_eFSS_DB_RES_CLBCKWRITEERR    - Error reported from the callback
 *              e_eFSS_DB_RES_WRITENOMATCHREAD - For some unknow reason data write dosent match data readed
 *              e_eFSS_DB_RES_OK_BKP_RCVRD     - operation ended successfully recovering a backup or an origin page
 *              e_eFSS_DB_RES_DBNOTCHECKED     - The function eFSS_DB_GetDBStatus needs to be called before and
 *                                               the status of the database need to be ok to proceed
 *              e_eFSS_DB_RES_OK               - Operation ended correctly
 */
e_eFSS_DB_RES eFSS_DB_GetElement(t_eFSS_DB_Ctx* const p_ptCtx, const uint32_t p_uPos, const uint16_t p_uRawValL,
                                 uint8_t* const p_puRawVal);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_DB_H */