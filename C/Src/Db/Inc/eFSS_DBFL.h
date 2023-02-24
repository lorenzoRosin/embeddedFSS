/**
 * @file       eFSS_DBFL.h
 *
 * @brief      Database of fixed length data module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSS_DBFL_H
#define EFSS_DBFL_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_DBC.h"



/***********************************************************************************************************************
 *      PRIVATE TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eFSS_DBFL_RES_OK = 0,
    e_eFSS_DBFL_RES_NOINITLIB,
    e_eFSS_DBFL_RES_BADPARAM,
    e_eFSS_DBFL_RES_BADPOINTER,
    e_eFSS_DBFL_RES_CORRUPTCTX,
    e_eFSS_DBFL_RES_CLBCKERASEERR,
    e_eFSS_DBFL_RES_CLBCKWRITEERR,
    e_eFSS_DBFL_RES_CLBCKREADERR,
    e_eFSS_DBFL_RES_CLBCKCRCERR,
    e_eFSS_DBFL_RES_NOTVALIDDB,
    e_eFSS_DBFL_RES_NEWVERSIONDB,
    e_eFSS_DBFL_RES_WRITENOMATCHREAD,
    e_eFSS_DBFL_RES_OK_BKP_RCVRD,
}e_eFSS_DBFL_RES;

typedef struct
{
	uint16_t uVer;
    t_eFSS_TYPE_DbRawElement* ptDefVal;
}t_eFSS_DBFL_DbElement;

typedef struct
{
    uint32_t uNumberOfElement;
    uint32_t uRawElemL;
    t_eFSS_DBFL_DbElement* ptElementList;
}t_eFSS_DBFL_DbStruct;

typedef struct
{
    t_eFSS_DBC_Ctx         tDbCtx;
    t_eFSS_DBFL_DbStruct   tDBS;
    t_eFSS_TYPE_CbDbDeSerCtx tSeDeserCtx;
}t_eFSS_DBFL_Ctx;

/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the Log Fixed length module context
 *
 * @param[in]   p_ptCtx          - Database Fixed length context
 * @param[in]   p_tCtxCb         - All callback collection context
 * @param[in]   p_tStorSet       - Storage settings
 * @param[in]   p_puBuff         - Pointer to a buffer used by the modules to make calc, must ne pageSize * 2
 * @param[in]   p_uBuffL         - Size of p_puBuff
 * @param[in]   p_tSerDeseCb     - List of serializer deserializer function to store data in database
 * @param[in]   p_tDbStruct      - Struct containing the default struct of the database
 *
 * @return      e_eFSS_DBFL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DBFL_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_DBFL_RES_OK            - Operation ended correctly
 */
e_eFSS_DBFL_RES eFSS_DBFL_InitCtx(t_eFSS_DBFL_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx const p_tCtxCb,
                                  t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL,
                                  t_eFSS_TYPE_CbDbDeSerCtx const p_tSerDeseCb, t_eFSS_DBFL_DbStruct p_tDbStruct);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx         - Database Fixed length context
 * @param[out]  p_pbIsInit      - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_DBFL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_DBFL_RES_OK            - Operation ended correctly
 */
e_eFSS_DBFL_RES eFSS_DBFL_IsInit(t_eFSS_DBFL_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Check the whole database status. This function must be called before proceeding with other operation,
 *              so we are sure to run on a valid database.
 *
 * @param[in]   p_ptCtx    - Database Fixed length context
 *
 * @return      e_eFSS_DB_DBFL_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DB_DBFL_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_DB_DBFL_OK            - Operation ended correctly
 */
e_eFSS_DBFL_RES eFSS_DBFL_GetDBStatus(t_eFSS_DBFL_Ctx* const p_ptCtx);

/**
 * @brief       Erase all the data present in the DB and restore default value. This function is the only function
 *              that is able to recover a corrupted database.
 *
 * @param[in]   p_ptCtx    - Database Fixed length context
 * @param[in]   p_puBuff   - Pointer to a memory area that we will use to store data that needs to be stuffed
 * @param[in]   p_uBuffL   - Dimension in byte of the memory area
 *
 * @return      e_eFSS_DBFL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DBFL_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_DBFL_RES_OK            - Operation ended correctly
 */
e_eFSS_DBFL_RES eFSS_DBFL_Format(t_eFSS_DBFL_Ctx* const p_ptCtx);

/**
 * @brief       Save an element in to the database
 *
 * @param[in]   p_ptCtx       - Database Fixed length context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_DBFL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DBFL_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_DBFL_RES_OK            - Operation ended correctly
 */
e_eFSS_DBFL_RES eFSS_DBFL_SaveElemen(t_eFSS_DBFL_Ctx* const p_ptCtx, uint32_t p_uPos,
                                     t_eFSS_TYPE_DbRawElement* p_ptElem);

/**
 * @brief       Get an element stored in to the database
 *
 * @param[in]   p_ptCtx       - Database Fixed length context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_DBFL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DBFL_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_DBFL_RES_OK            - Operation ended correctly
 */
e_eFSS_DBFL_RES eFSS_DBFL_GetElement(t_eFSS_DBFL_Ctx* const p_ptCtx, uint32_t p_uPos,
                                     t_eFSS_TYPE_DbRawElement* p_ptElem);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_DBFL_H */
