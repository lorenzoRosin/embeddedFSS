/**
 * @file       eFSS_BLOBC.h
 *
 * @brief      Blob Core - large file core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSS_BLOBC_H
#define EFSS_BLOBC_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_COREHL.h"



/***********************************************************************************************************************
 *      PRIVATE TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eFSS_BLOBC_RES_OK = 0,
    e_eFSS_BLOBC_RES_OK_BKP_RCVRD,
    e_eFSS_BLOBC_RES_NOTVALIDBLOB,
    e_eFSS_BLOBC_RES_NEWVERSIONBLOB,
    e_eFSS_BLOBC_RES_NOINITLIB,
    e_eFSS_BLOBC_RES_BADPARAM,
    e_eFSS_BLOBC_RES_BADPOINTER,
    e_eFSS_BLOBC_RES_CORRUPTCTX,
    e_eFSS_BLOBC_RES_CLBCKERASEERR,
    e_eFSS_BLOBC_RES_CLBCKWRITEERR,
    e_eFSS_BLOBC_RES_CLBCKREADERR,
    e_eFSS_BLOBC_RES_CLBCKCRCERR,
    e_eFSS_BLOBC_RES_WRITENOMATCHREAD,
}e_eFSS_BLOBC_RES;

typedef struct
{
    t_eFSS_COREHL_Ctx tCOREHLCtx;
}t_eFSS_BLOBC_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the blob module context
 *
 * @param[in]   p_ptCtx        - Blob Core context
 * @param[in]   p_tCtxCb       - All callback collection context
 * @param[in]   p_tStorSet     - Storage settings
 * @param[in]   p_puBuff       - Pointer to a buffer used by the modules to make calc, must ne pageSize * 2
 * @param[in]   p_uBuffL       - Size of p_puBuff
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_BLOBC_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_BLOBC_RES_OK            - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_BLOBC_InitCtx(t_eFSS_BLOBC_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx const p_tCtxCb,
                                    t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Blob Core context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_BLOBC_RES_OK            - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_BLOBC_IsInit(t_eFSS_BLOBC_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Get the status of the storage
 *
 * @param[in]   p_ptCtx    - Blob context
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_BLOBC_RES_OK                 - Operation ended correctly
 *              e_eFSS_BLOBC_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_BLOBC_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_BLOBC_RES_NEWVERSIONBLOB     - New version of the blob requested
 *              e_eFSS_BLOBC_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_BLOBC_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_BLOBC_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_BLOBC_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_BLOBC_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_BLOBC_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_BLOBC_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_BLOBC_RES eFSS_BLOBC_GetStorageStatus(t_eFSS_BLOBC_Ctx* const p_ptCtx);

/**
 * @brief       Get info about the stored blob
 *
 * @param[in]   p_ptCtx      - Blob context
 * @param[out]  p_puBlobSize - Pointer to a uint32_t that will be filled with the size of the blob
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_BLOBC_RES_OK                 - Operation ended correctly
 *              e_eFSS_BLOBC_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_BLOBC_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_BLOBC_RES_NEWVERSIONBLOB     - New version of the blob requested
 *              e_eFSS_BLOBC_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_BLOBC_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_BLOBC_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_BLOBC_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_BLOBC_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_BLOBC_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_BLOBC_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_BLOBC_RES eFSS_BLOBC_GetInfo(t_eFSS_BLOBC_Ctx* const p_ptCtx, uint32_t* p_puBlobSize);

/**
 * @brief       Format the memory used for the blob and all data
 *
 * @param[in]   p_ptCtx    - Blob context
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_BLOBC_RES_OK                 - Operation ended correctly
 *              e_eFSS_BLOBC_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_BLOBC_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_BLOBC_RES_NEWVERSIONBLOB     - New version of the blob requested
 *              e_eFSS_BLOBC_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_BLOBC_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_BLOBC_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_BLOBC_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_BLOBC_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_BLOBC_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_BLOBC_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_BLOBC_RES eFSS_BLOBC_Format(t_eFSS_BLOBC_Ctx* const p_ptCtx);

/**
 * @brief       Read the blob
 *
 * @param[in]   p_ptCtx    - Blob context
 * @param[in]   p_uOffset  - Offset where to start read the blob
 * @param[in]   p_puBuff   - Pointer to a buffer where to copy readed data
 * @param[in]   p_uBuffL   - Size of p_puBuff
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_BLOBC_RES_OK                 - Operation ended correctly
 *              e_eFSS_BLOBC_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_BLOBC_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_BLOBC_RES_NEWVERSIONBLOB     - New version of the blob requested
 *              e_eFSS_BLOBC_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_BLOBC_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_BLOBC_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_BLOBC_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_BLOBC_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_BLOBC_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_BLOBC_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_BLOBC_RES eFSS_BLOBC_ReadBlob(t_eFSS_BLOBC_Ctx* const p_ptCtx, uint32_t p_uOffset, uint8_t* p_puBuff,
                                     uint32_t p_uBuffL);

/**
 * @brief       Start to write the blob
 *
 * @param[in]   p_ptCtx    - Blob context
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_BLOBC_RES_OK                 - Operation ended correctly
 *              e_eFSS_BLOBC_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_BLOBC_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_BLOBC_RES_NEWVERSIONBLOB     - New version of the blob requested
 *              e_eFSS_BLOBC_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_BLOBC_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_BLOBC_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_BLOBC_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_BLOBC_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_BLOBC_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_BLOBC_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_BLOBC_RES eFSS_BLOBC_StartWrite(t_eFSS_BLOBC_Ctx* const p_ptCtx);

/**
 * @brief       Append some data to the blob that we have started to write
 *
 * @param[in]   p_ptCtx    - Blob context
 * @param[in]   p_puBuff   - Pointer to a buffer that must be write in the storage area
 * @param[in]   p_uBuffL   - Size of p_puBuff
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_BLOBC_RES_OK                 - Operation ended correctly
 *              e_eFSS_BLOBC_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_BLOBC_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_BLOBC_RES_NEWVERSIONBLOB     - New version of the blob requested
 *              e_eFSS_BLOBC_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_BLOBC_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_BLOBC_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_BLOBC_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_BLOBC_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_BLOBC_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_BLOBC_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_BLOBC_RES eFSS_BLOBC_AppendData(t_eFSS_BLOBC_Ctx* const p_ptCtx, uint8_t* p_puBuff, uint32_t p_uBuffL);

/**
 * @brief       Save all the data written till now
 *
 * @param[in]   p_ptCtx    - Blob context
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_BLOBC_RES_OK                 - Operation ended correctly
 *              e_eFSS_BLOBC_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_BLOBC_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_BLOBC_RES_NEWVERSIONBLOB     - New version of the blob requested
 *              e_eFSS_BLOBC_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_BLOBC_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_BLOBC_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_BLOBC_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_BLOBC_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_BLOBC_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_BLOBC_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_BLOBC_RES eFSS_BLOBC_EndWrite(t_eFSS_BLOBC_Ctx* const p_ptCtx);


#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_BLOBC_H */