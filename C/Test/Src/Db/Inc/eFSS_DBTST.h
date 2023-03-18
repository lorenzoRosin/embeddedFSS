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
 * database is checked will delete the previously stored value and will save it's default value instead.
 * During the live of the database it's not possible to remove an entry or change an entry size. It's possible
 * to add new entry instead. */
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
    e_eFSS_DB_RES_NEWVERSIONDB,
    e_eFSS_DB_RES_WRITENOMATCHREAD,
    e_eFSS_DB_RES_OK_BKP_RCVRD,
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
 * @brief       Check the whole database status. This function should be called before doing anything else with the
 *              the database. In anycase this function is called automatically on the firt action against the db.
 *              This function is reading all the entry in to the database, so i can takes some times to execute.
 *
 * @param[in]   p_ptCtx          - Database context
 *
 * @return      e_eFSS_DB_DBFL_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DB_DBFL_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_DB_DBFL_OK            - Operation ended correctly
 */
e_eFSS_DB_RES eFSS_DB_GetDBStatus(t_eFSS_DB_Ctx* const p_ptCtx);

/**
 * @brief       Erase all the data present in the DB and restore default value. This function is the only function
 *              that is able to recover a corrupted database.
 *
 * @param[in]   p_ptCtx    - Database context
 *
 * @return      e_eFSS_DB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DB_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_DB_RES_OK            - Operation ended correctly
 */
e_eFSS_DB_RES eFSS_DB_FormatToDefault(t_eFSS_DB_Ctx* const p_ptCtx);

/**
 * @brief       Save an element in to the database
 *
 * @param[in]   p_ptCtx       - Database context
 * @param[in]   p_uPos        - Position of the element we want to save in the database
 * @param[in]   p_uElemL      - Length of the element
 * @param[in]   p_puRawVal    - Raw value of the element we want to save
 *
 * @return      e_eFSS_DB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DB_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_DB_RES_OK            - Operation ended correctly
 */
e_eFSS_DB_RES eFSS_DB_SaveElemen(t_eFSS_DB_Ctx* const p_ptCtx, const uint32_t p_uPos, const uint16_t p_uElemL,
                                 uint8_t* const p_puRawVal);

/**
 * @brief       Get an element stored from the database
 *
 * @param[in]   p_ptCtx       - Database context
 * @param[in]   p_uPos        - Position of the element we want to read from the database
 * @param[in]   p_uElemL      - Length of the element
 * @param[out]  p_puRawVal    - Storage are of size p_uElemL were we will save the element
 *
 * @return      e_eFSS_DB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DB_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_DB_RES_OK            - Operation ended correctly
 */
e_eFSS_DB_RES eFSS_DB_GetElement(t_eFSS_DB_Ctx* const p_ptCtx, const uint32_t p_uPos, const uint16_t p_uElemL,
                                 uint8_t* const p_puRawVal);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_DB_H */