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
#include "eFSS_BLOBC.h"



/***********************************************************************************************************************
 *      PUBLIC TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eFSS_BLOB_RES_OK = 0,
    e_eFSS_BLOB_RES_NOINITLIB,
    e_eFSS_BLOB_RES_BADPARAM,
    e_eFSS_BLOB_RES_BADPOINTER,
    e_eFSS_BLOB_RES_CORRUPTCTX,
    e_eFSS_BLOB_RES_CLBCKERASEERR,
    e_eFSS_BLOB_RES_CLBCKWRITEERR,
    e_eFSS_BLOB_RES_CLBCKREADERR,
    e_eFSS_BLOB_RES_CLBCKCRCERR,
    e_eFSS_BLOB_RES_NOTVALIDBLOB,
    e_eFSS_BLOB_RES_NEWVERSIONFOUND,
    e_eFSS_BLOB_RES_WRITENOMATCHREAD,
    e_eFSS_BLOB_RES_OK_BKP_RCVRD,
    e_eFSS_BLOB_RES_BLOBNOTCHECKED,
    e_eFSS_BLOB_RES_WRITEONGOING,
    e_eFSS_BLOB_RES_WRITENOSTARTED
}e_eFSS_BLOB_RES;

typedef struct
{
    t_eFSS_BLOBC_Ctx tBLOBCCtx;
    bool_t bIsBlobCheked;
    bool_t bIsWriteOngoing;
    uint32_t uDataWritten;
    uint32_t uCrcOfDataWritten;
    uint32_t uCurrentSeqN;
}t_eFSS_BLOB_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the blob module context
 *
 * @param[in]   p_ptCtx        - Blob context
 * @param[in]   p_tCtxCb       - All callback collection context
 * @param[in]   p_tStorSet     - Storage settings
 * @param[in]   p_puBuff       - Pointer to a buffer used by the modules to make calc, must be pageSize * 2
 * @param[in]   p_uBuffL       - Size of p_puBuff
 *
 * @return      e_eFSS_BLOB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_BLOB_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_BLOB_RES_OK            - Operation ended correctly
 */
e_eFSS_BLOB_RES eFSS_BLOB_InitCtx(t_eFSS_BLOB_Ctx* const p_ptCtx, const t_eFSS_TYPE_CbStorCtx p_tCtxCb,
                                  const t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff,
                                  const uint32_t p_uBuffL);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Blob context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_BLOB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_BLOB_RES_OK            - Operation ended correctly
 */
e_eFSS_BLOB_RES eFSS_BLOB_IsInit(t_eFSS_BLOB_Ctx* const p_ptCtx, bool_t* const p_pbIsInit);

/**
 * @brief       Get the status of the storage. This need to be the first function to call when using the blob, just to
 *              be sure that something usefull and not corrupted is avaiable for the next operation.
 *              If the blob is corrupted the only function that we can call to ripristinate the situation is the
 *              eFSS_BLOB_Format function.
 *
 * @param[in]   p_ptCtx        - Blob context
 *
 * @return      e_eFSS_BLOB_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_BLOB_RES_OK                 - Operation ended correctly
 *              e_eFSS_BLOB_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_BLOB_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_BLOB_RES_NEWVERSIONBLOB     - New version of the blob requested
 *              e_eFSS_BLOB_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_BLOB_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_BLOB_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_BLOB_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_BLOB_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_BLOB_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_BLOB_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_BLOB_RES eFSS_BLOB_GetStorageStatus(t_eFSS_BLOB_Ctx* const p_ptCtx);

/**
 * @brief       Get info about the stored blob.
 *
 * @param[in]   p_ptCtx      - Blob context
 * @param[out]  p_puBlobSize - Pointer to a uint32_t that will be filled with the size of the blob
 *
 * @return      e_eFSS_BLOB_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_BLOB_RES_OK                 - Operation ended correctly
 *              e_eFSS_BLOB_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_BLOB_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_BLOB_RES_NEWVERSIONBLOB     - New version of the blob requested
 *              e_eFSS_BLOB_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_BLOB_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_BLOB_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_BLOB_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_BLOB_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_BLOB_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_BLOB_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_BLOB_RES eFSS_BLOB_GetInfo(t_eFSS_BLOB_Ctx* const p_ptCtx, uint32_t* const p_puBlobSize);

/**
 * @brief       Format the memory used for the blob and all data present. This is the only function that we can call
 *              when the blob is corrupted and no valid data can be readed.
 *
 * @param[in]   p_ptCtx      - Blob context
 *
 * @return      e_eFSS_BLOB_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_BLOB_RES_OK                 - Operation ended correctly
 *              e_eFSS_BLOB_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_BLOB_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_BLOB_RES_NEWVERSIONBLOB     - New version of the blob requested
 *              e_eFSS_BLOB_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_BLOB_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_BLOB_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_BLOB_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_BLOB_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_BLOB_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_BLOB_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_BLOB_RES eFSS_BLOB_Format(t_eFSS_BLOB_Ctx* const p_ptCtx);

/**
 * @brief       Read the blob already presente in storage.
 *
 * @param[in]   p_ptCtx    - Blob context
 * @param[in]   p_uOffset  - Offset where to start read the blob
 * @param[in]   p_uBuffL   - Size of p_puBuff and of the chunk we are going to read
 * @param[out]  p_puBuff   - Pointer to a buffer where to copy readed data
 *
 * @return      e_eFSS_BLOB_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_BLOB_RES_OK                 - Operation ended correctly
 *              e_eFSS_BLOB_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_BLOB_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_BLOB_RES_NEWVERSIONBLOB     - New version of the blob requested
 *              e_eFSS_BLOB_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_BLOB_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_BLOB_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_BLOB_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_BLOB_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_BLOB_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_BLOB_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_BLOB_RES eFSS_BLOB_ReadBlob(t_eFSS_BLOB_Ctx* const p_ptCtx, const uint32_t p_uOffset, const uint32_t p_uBuffL,
                                   uint8_t* const p_puBuff);

/**
 * @brief       Start to write the blob
 *
 * @param[in]   p_ptCtx      - Blob context
 *
 * @return      e_eFSS_BLOB_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_BLOB_RES_OK                 - Operation ended correctly
 *              e_eFSS_BLOB_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_BLOB_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_BLOB_RES_NEWVERSIONBLOB     - New version of the blob requested
 *              e_eFSS_BLOB_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_BLOB_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_BLOB_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_BLOB_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_BLOB_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_BLOB_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_BLOB_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_BLOB_RES eFSS_BLOB_StartWrite(t_eFSS_BLOB_Ctx* const p_ptCtx);

/**
 * @brief       Append some data to the blob that we have started to write
 *
 * @param[in]   p_ptCtx    - Blob context
 * @param[in]   p_puBuff   - Pointer to a buffer that must be write in the storage area
 * @param[in]   p_uBuffL   - Size of p_puBuff
 *
 * @return      e_eFSS_BLOB_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_BLOB_RES_OK                 - Operation ended correctly
 *              e_eFSS_BLOB_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_BLOB_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_BLOB_RES_NEWVERSIONBLOB     - New version of the blob requested
 *              e_eFSS_BLOB_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_BLOB_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_BLOB_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_BLOB_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_BLOB_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_BLOB_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_BLOB_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_BLOB_RES eFSS_BLOB_AppendData(t_eFSS_BLOB_Ctx* const p_ptCtx, uint8_t* const p_puBuff, const uint32_t p_uBuffL);

/**
 * @brief       Finalize and validate all data written till now.
 *
 * @param[in]   p_ptCtx      - Blob context
 *
 * @return      e_eFSS_BLOB_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_BLOB_RES_OK                 - Operation ended correctly
 *              e_eFSS_BLOB_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_BLOB_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_BLOB_RES_NEWVERSIONBLOB     - New version of the blob requested
 *              e_eFSS_BLOB_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_BLOB_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_BLOB_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_BLOB_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_BLOB_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_BLOB_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_BLOB_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_BLOB_RES eFSS_BLOB_EndWrite(t_eFSS_BLOB_Ctx* const p_ptCtx);

/**
 * @brief       Abort current write and ripristinate old file
 *
 * @param[in]   p_ptCtx      - Blob context
 *
 * @return      e_eFSS_BLOB_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_BLOB_RES_OK                 - Operation ended correctly
 *              e_eFSS_BLOB_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_BLOB_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_BLOB_RES_NEWVERSIONBLOB     - New version of the blob requested
 *              e_eFSS_BLOB_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_BLOB_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_BLOB_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_BLOB_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_BLOB_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_BLOB_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_BLOB_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_BLOB_RES eFSS_BLOB_AbortWrite(t_eFSS_BLOB_Ctx* const p_ptCtx);


#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_BLOB_H */