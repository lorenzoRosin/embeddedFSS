/**
 * @file       eFSS_LOG.h
 *
 * @brief      LOG module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSS_LOG_H
#define EFSS_LOG_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_UTILSLLPRV.h"



/***********************************************************************************************************************
 *      PRIVATE TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eFSS_LOG_RES_OK = 0,
    e_eFSS_LOG_RES_OK_BKP_RCVRD,
    e_eFSS_LOG_RES_NOTVALIDBLOB,
    e_eFSS_LOG_RES_NEWVERSIONLOG,
    e_eFSS_LOG_RES_NOINITLIB,
    e_eFSS_LOG_RES_BADPARAM,
    e_eFSS_LOG_RES_BADPOINTER,
    e_eFSS_LOG_RES_CORRUPTCTX,
    e_eFSS_LOG_RES_CLBCKERASEERR,
    e_eFSS_LOG_RES_CLBCKWRITEERR,
    e_eFSS_LOG_RES_CLBCKREADERR,
    e_eFSS_LOG_RES_CLBCKCRCERR,
    e_eFSS_LOG_RES_WRITENOMATCHREAD,
}e_eFSS_LOG_RES;

typedef struct
{
    bool_t   bIsInit;
    t_eFSS_TYPE_CbStorCtx* ptCtxCb;
	uint8_t* puBuff1;
	uint32_t uBuff1L;
	uint8_t* puBuff2;
	uint32_t uBuff2L;
    uint32_t uNPage;
    uint32_t uPageSize;
    uint32_t uReTry;
    uint32_t uVersion;
    bool_t bUseFlashCache;
    bool_t bUseFullBckup;
    uint32_t uNPagIdxFound;
    uint32_t uOPagIdxFound;
}t_eFSS_LOG_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the blob module context
 *
 * @param[in]   p_ptCtx          - Log context
 * @param[in]   p_ptCtxCb        - All callback collection context
 * @param[in]   p_uPageToUse     - How many page use for the log module
 * @param[in]   p_uPageSize      - Size of the used pages
 * @param[in]   p_puBuff         - Pointer to a buffer used by the modules to make calc
 * @param[in]   p_uBuffL         - Size of p_puBuff
 * @param[in]   p_uBlobVersion   - Version of the Log
 * @param[in]   p_uRetry         - How many time retry basic read write erase operation if error occour
 * @param[in]   p_bUseFlashCache - Use flash stored cache for resuming index
 * @param[in]   p_bUseFullBckup  - Save every log data in a backup pages
 *
 * @return      e_eFSS_BLOB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_BLOB_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_BLOB_RES_OK            - Operation ended correctly
 */
e_eFSS_LOG_RES eFSS_LOG_InitCtx(t_eFSS_LOG_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx* const p_ptCtxCb,
                                const uint32_t p_uPageToUse, const uint32_t p_uPageSize, uint8_t* const p_puBuff,
                                uint32_t p_uBuffL, uint16_t p_uLogVersion, uint32_t p_uRetry,
                                bool_t p_bUseFlashCache, bool_t p_bUseFullBckup);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Log context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_LOG_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_LOG_RES_OK            - Operation ended correctly
 */
e_eFSS_LOG_RES eFSS_LOG_IsInit(t_eFSS_LOG_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Get the status of the storage
 *
 * @param[in]   p_ptCtx       - Log context
 *
 * @return      e_eFSS_LOG_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_LOG_RES_OK                 - Operation ended correctly
 *              e_eFSS_LOG_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_LOG_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_LOG_RES_NEWVERSIONLOG      - New version of the blob requested
 *              e_eFSS_LOG_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_LOG_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_LOG_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_LOG_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_LOG_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_LOG_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_LOG_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_LOG_RES eFSS_LOG_GetStorageStatus(t_eFSS_LOG_Ctx* const p_ptCtx);

/**
 * @brief       Get info about the stored log
 *
 * @param[in]   p_ptCtx       - Log context
 * @param[out]  p_puNewLogI   - Pointer to a uint32_t that will be filled with the New log index
 * @param[out]  p_puOldLogI   - Pointer to a uint32_t that will be filled with the Old log index
 * @param[out]  p_puNpageUsed - Pointer to a uint32_t that will be filled with the number of valorized page
 * @param[out]  p_puLogVer    - Pointer to a uint32_t that will be filled with the version of the log
 *
 * @return      e_eFSS_LOG_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_LOG_RES_OK                 - Operation ended correctly
 *              e_eFSS_LOG_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_LOG_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_LOG_RES_NEWVERSIONLOG      - New version of the blob requested
 *              e_eFSS_LOG_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_LOG_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_LOG_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_LOG_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_LOG_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_LOG_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_LOG_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_LOG_RES eFSS_LOG_GetLogInfo(t_eFSS_LOG_Ctx* const p_ptCtx, uint32_t *p_puNewLogI, uint32_t *p_puOldLogI,
                                   uint32_t *p_puNpageUsed, uint32_t *p_puLogVer);

/**
 * @brief       Format the memory used for the log and all data
 *
 * @param[in]   p_ptCtx       - Log context
 *
 * @return      e_eFSS_LOG_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_LOG_RES_OK                 - Operation ended correctly
 *              e_eFSS_LOG_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_LOG_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_LOG_RES_NEWVERSIONLOG      - New version of the blob requested
 *              e_eFSS_LOG_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_LOG_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_LOG_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_LOG_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_LOG_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_LOG_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_LOG_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_LOG_RES eFSS_LOG_Format(t_eFSS_LOG_Ctx* const p_ptCtx);

/**
 * @brief       Add a log
 *
 * @param[in]   p_ptCtx       - Log context
 * @param[in]   p_puLogToSave - Pointer to the buffer containing the log to store
 * @param[in]   p_uLogL       - Dimension in byte of the log to store
 *
 * @return      e_eFSS_LOG_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_LOG_RES_OK                 - Operation ended correctly
 *              e_eFSS_LOG_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_LOG_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_LOG_RES_NEWVERSIONLOG      - New version of the blob requested
 *              e_eFSS_LOG_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_LOG_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_LOG_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_LOG_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_LOG_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_LOG_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_LOG_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_LOG_RES eFSS_LOG_AddLog(t_eFSS_LOG_Ctx* const p_ptCtx, uint8_t* p_puLogToSave, uint32_t p_uLogL);

/**
 * @brief       Get all the log present on a specific page
 *
 * @param[in]   p_ptCtx        - Log context
 * @param[in]   p_puLogBuf     - Pointer to the buffer where founded log will be stored
 * @param[in]   p_uLogBufL     - Size fo the p_puLogBuf buffer
 * @param[in]   p_puNLogGetted - Pointer to an uint32_t where the numbers of retrived log will be saved
 *
 * @return      e_eFSS_LOG_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_LOG_RES_OK                 - Operation ended correctly
 *              e_eFSS_LOG_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_LOG_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_LOG_RES_NEWVERSIONLOG      - New version of the blob requested
 *              e_eFSS_LOG_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_LOG_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_LOG_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_LOG_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_LOG_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_LOG_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_LOG_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_LOG_RES eFSS_LOG_GetLogOfASpecificPage(t_eFSS_LOG_Ctx* const p_ptCtx, uint8_t* p_puLogBuf, uint32_t* p_uLogBufL);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_LOG_H */











