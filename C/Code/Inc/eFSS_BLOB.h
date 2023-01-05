/**
 * @file       eFSS_BLOB.h
 *
 * @brief      Blob large file module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSS_BLOB_H
#define EFSS_BLOB_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_TYPE.h"



/***********************************************************************************************************************
 *      PRIVATE TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eFSS_BLOB_RES_OK = 0,
    e_eFSS_BLOB_RES_OK_BKP_RCVRD,
    e_eFSS_BLOB_RES_BADPARAM,
    e_eFSS_BLOB_RES_BADPOINTER,
    e_eFSS_BLOB_RES_CLBCKREPORTERROR,
    e_eFSS_BLOB_RES_NOTVALIDBLOB,
    e_eFSS_BLOB_RES_CORRUPTCTX,
    e_eFSS_BLOB_RES_NOINITLIB,
}e_eFSS_BLOB_RES;

typedef struct
{
    bool_t   bIsInit;
    t_eFSS_TYPE_CbCtx* ptCtxCb;
	uint8_t* puBuff1;
	uint32_t uBuff1L;
	uint8_t* puBuff2;
	uint32_t uBuff2L;
    uint32_t uNPage;
    uint32_t uPageSize;
    uint32_t uReTry;
}t_eFSS_BLOB_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the blob module context
 *
 * @param[in]   p_ptCtx       - Blob context
 * @param[in]   p_ptCtxCb     - All callback collection context
 * @param[in]   p_uPageToUse  - How many page use for the blob module
 * @param[in]   p_uPageSize   - Size of the used pages
 * @param[in]   p_puBuff      - Pointer to a buffer used by the modules to make calc
 * @param[in]   p_uBuffL      - Size of p_puBuff
 *
 * @return      e_eFSS_BLOB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_BLOB_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_BLOB_RES_OK            - Operation ended correctly
 */
e_eFSS_BLOB_RES eFSS_BLOB_InitCtx(t_eFSS_BLOB_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx* const p_ptCtxCb,
                                  const uint32_t p_uPageToUse, const uint32_t p_uPageSize, uint8_t* const p_puBuff,
                                  uint32_t p_uBuffL);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Blob context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_BLOB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_BLOB_RES_OK            - Operation ended correctly
 */
e_eFSS_BLOB_RES eFSS_BLOB_IsInit(t_eFSS_BLOB_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Initialize the byte stuffer context
 *
 * @param[in]   p_ptCtx    - Blob context
 *
 * @return      e_eFSS_BLOB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_BLOB_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_BLOB_RES_OK            - Operation ended correctly
 */
e_eFSS_BLOB_RES eFSS_BLOB_GetStorageStatus(t_eFSS_BLOB_Ctx* const p_ptCtx);

/**
 * @brief       Format the memory used for the blob and all data
 *
 * @param[in]   p_ptCtx    - Blob context
 *
 * @return      e_eFSS_BLOB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_BLOB_RES_OK            - Operation ended correctly
 */
e_eFSS_BLOB_RES eFSS_BLOB_Format(t_eFSS_BLOB_Ctx* const p_ptCtx);

/**
 * @brief       Get info about the stored blob
 *
 * @param[in]   p_ptCtx      - Blob context
 * @param[out]  p_puBlobSize - Pointer to a uint32_t that will be filled with the size of the blob
 *
 * @return      e_eFSS_BLOB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_BLOB_RES_OK            - Operation ended correctly
 */
e_eFSS_BLOB_RES eFSS_BLOB_GetInfo(t_eFSS_BLOB_Ctx* const p_ptCtx, bool_t* p_puBlobSize);

/**
 * @brief       Read the whole blob
 *
 * @param[in]   p_ptCtx    - Blob context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_BLOB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_BLOB_RES_OK            - Operation ended correctly
 */
e_eFSS_BLOB_RES eFSS_BLOB_ReadAllBlob(t_eFSS_BLOB_Ctx* const p_ptCtx, uint8_t* p_puBuff, uint32_t p_uBuffSize,
                                      uint32_t* p_puReaded);

/**
 * @brief       Write the whole blob
 *
 * @param[in]   p_ptCtx    - Blob context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_BLOB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_BLOB_RES_OK            - Operation ended correctly
 */
e_eFSS_BLOB_RES eFSS_BLOB_WriteAllBlob(t_eFSS_BLOB_Ctx* const p_ptCtx, uint8_t* p_puBuff, uint32_t p_uBuffSize);







#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_BLOB_H */