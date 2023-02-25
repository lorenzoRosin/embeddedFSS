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
typedef struct
{
	uint16_t uElemVer;
    uint16_t uElemL;
    uint8_t* puRawVal;
}t_eFSS_DB_DbElement;

typedef struct
{
    uint32_t uNOfElem;
    t_eFSS_DB_DbElement* ptDefValElemList;
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
    e_eFSS_DB_RES_OK_BKP_RCVRD
}e_eFSS_DB_RES;

typedef struct
{
    t_eFSS_DBC_Ctx           tDbCtx;
    t_eFSS_DB_DbStruct       tDBS;
    bool_t                   bIsDbStatusChecked;
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
e_eFSS_DB_RES eFSS_DB_InitCtx(t_eFSS_DB_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx const p_tCtxCb,
                              t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL,
                              t_eFSS_DB_DbStruct p_tDbStruct);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx         - Database context
 * @param[out]  p_pbIsInit      - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_DB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_DB_RES_OK            - Operation ended correctly
 */
e_eFSS_DB_RES eFSS_DB_IsInit(t_eFSS_DB_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Check the whole database status. This function should be called before doing anything else with the
 *              the database. In anycase this function is called automatically on the firt action against the db.
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
e_eFSS_DB_RES eFSS_DB_SaveElemen(t_eFSS_DB_Ctx* const p_ptCtx, uint32_t p_uPos, uint16_t p_uElemL,
                                 uint8_t* p_puRawVal);

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
e_eFSS_DB_RES eFSS_DB_GetElement(t_eFSS_DB_Ctx* const p_ptCtx, uint32_t p_uPos, uint16_t p_uElemL,
                                 uint8_t* p_puRawVal);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_DB_H */
