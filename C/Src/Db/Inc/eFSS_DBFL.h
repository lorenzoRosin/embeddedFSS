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
    e_eFSS_DBFL_RES_NOTVALIDLOG,
    e_eFSS_DBFL_RES_NEWVERSIONLOG,
    e_eFSS_DBFL_RES_WRITENOMATCHREAD,
    e_eFSS_DBFL_RES_OK_BKP_RCVRD,
}e_eFSS_DBFL_RES;

typedef struct
{
	uint16_t uVer;
    t_eFSS_DBFL_SingleDbElement* ptDefVal;
}t_eFSS_DB_DbElement;

typedef struct
{
    uint32_t uNumberOfElement;
    uint32_t uSerialEleSize;
    t_eFSS_DB_DbElement* ptElementList;
}t_eFSS_DB_DbStruct;


typedef struct
{
    t_eFSS_DBC_Ctx  tDbCtx;
    t_eFSS_DB_DbStruct tDBS;
}t_eFSS_DB_Ctx;

/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the database context
 *
 * @param[in]   p_ptCtx       - Blob context
 * @param[in]   p_ptCtxCb     - All callback collection context
 * @param[in]   p_uPageToUse  - How many page use for the blob module
 * @param[in]   p_uPageSize   - Size of the used pages
 * @param[in]   p_puBuff      - Pointer to a buffer used by the modules to make calc
 * @param[in]   p_uBuffL      - Size of p_puBuff
 *
 * @return      e_eFSS_DB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DB_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_DB_RES_OK            - Operation ended correctly
 */
e_eFSS_DB_RES eFSS_DB_InitCtx(t_eFSS_DB_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx* const p_ptCtxCb,
                                  const uint32_t p_uPageToUse, const uint32_t p_uPageSize, uint8_t* const p_puBuff,
                                  uint32_t p_uBuffL, t_eFSS_DB_DbStruct p_tDBS);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx          - Database Core context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_DBC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_DBC_RES_OK            - Operation ended correctly
 */
e_eFSS_DBC_RES eFSS_DBC_IsInit(t_eFSS_DBC_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Initialize the byte stuffer context
 *
 * @param[in]   p_ptCtx    - Byte stuffer context
 * @param[in]   p_puBuff   - Pointer to a memory area that we will use to store data that needs to be stuffed
 * @param[in]   p_uBuffL   - Dimension in byte of the memory area
 *
 * @return      e_eFSS_DB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DB_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_DB_RES_OK            - Operation ended correctly
 */
e_eFSS_DB_RES eFSS_DB_GetStorageStatus(t_eFSS_DB_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx* const p_peStatus);

/**
 * @brief       Initialize the byte stuffer context
 *
 * @param[in]   p_ptCtx    - Byte stuffer context
 * @param[in]   p_puBuff   - Pointer to a memory area that we will use to store data that needs to be stuffed
 * @param[in]   p_uBuffL   - Dimension in byte of the memory area
 *
 * @return      e_eFSS_DB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DB_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_DB_RES_OK            - Operation ended correctly
 */
e_eFSS_DB_RES eFSS_DB_Format(t_eFSS_DB_Ctx* const p_ptCtx);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Byte stuffer context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_DB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DB_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_DB_RES_OK            - Operation ended correctly
 */
e_eFSS_DB_RES eFSS_DB_SaveElemen(t_eFSS_DB_Ctx* const p_ptCtx, uint32_t p_uPos, uint8_t* p_puBuff, uint32_t p_uBuffL);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Byte stuffer context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_DB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DB_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_DB_RES_OK            - Operation ended correctly
 */
e_eFSS_DB_RES eFSS_DB_GetElement(t_eFSS_DB_Ctx* const p_ptCtx, uint32_t p_uPos, uint8_t* p_puBuff, uint8_t* p_puBuffL);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_DBFL_H */
