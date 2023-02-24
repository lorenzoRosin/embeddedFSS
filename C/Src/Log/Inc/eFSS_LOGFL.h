/**
 * @file       eFSS_LOGFL.h
 *
 * @brief      LOG of fiexd length module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSS_LOGFL_H
#define EFSS_LOGFL_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_LOGC.h"



/***********************************************************************************************************************
 *      PRIVATE TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eFSS_LOGFL_RES_OK = 0,
    e_eFSS_LOGFL_RES_NOINITLIB,
    e_eFSS_LOGFL_RES_BADPARAM,
    e_eFSS_LOGFL_RES_BADPOINTER,
    e_eFSS_LOGFL_RES_CORRUPTCTX,
    e_eFSS_LOGFL_RES_CLBCKERASEERR,
    e_eFSS_LOGFL_RES_CLBCKWRITEERR,
    e_eFSS_LOGFL_RES_CLBCKREADERR,
    e_eFSS_LOGFL_RES_CLBCKCRCERR,
    e_eFSS_LOGFL_RES_NOTVALIDLOG,
    e_eFSS_LOGFL_RES_NEWVERSIONLOG,
    e_eFSS_LOGFL_RES_WRITENOMATCHREAD,
    e_eFSS_LOGFL_RES_OK_BKP_RCVRD,
}e_eFSS_LOGFL_RES;

typedef struct
{
    t_eFSS_LOGC_Ctx tLOGCCtx;
    uint32_t uNewPagIdx;
    uint32_t uFullFilledP;
    bool_t bFlashCache;
    bool_t bFullBckup;
}t_eFSS_LOGFL_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the Log Core module context
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_tCtxCb         - All callback collection context
 * @param[in]   p_tStorSet       - Storage settings
 * @param[in]   p_puBuff         - Pointer to a buffer used by the modules to make calc, must ne pageSize * 2
 * @param[in]   p_uBuffL         - Size of p_puBuff
 * @param[in]   p_bFlashCache    - Use flash as cache for storing and resuming index
 * @param[in]   p_bFullBckup     - Save every log data in a backup pages
 *
 * @return      e_eFSS_LOGFL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_LOGFL_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_LOGFL_RES_OK            - Operation ended correctly
 */
e_eFSS_LOGFL_RES eFSS_LOGFL_InitCtx(t_eFSS_LOGFL_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx const p_tCtxCb,
                                  t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL,
                                  bool_t p_bFlashCache, bool_t p_bFullBckup);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Log Core context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_LOGFL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_LOGFL_RES_OK            - Operation ended correctly
 */
e_eFSS_LOGFL_RES eFSS_LOGFL_IsInit(t_eFSS_LOGFL_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Get the status of the storage. This operation will restore any corrupted information if possible.
 *              In the case it's not possible to restore corrupted contex e_eFSS_LOGFL_RES_NOTVALIDLOG will be returned.
 *              When e_eFSS_LOGFL_RES_NOTVALIDLOG is returned we need to call eFSS_LOGFL_Format in order to format the
 *              storage area because no other operations are possible.
 *
 * @param[in]   p_ptCtx       - Log context
 *
 * @return      e_eFSS_LOGFL_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_LOGFL_RES_OK                 - Operation ended correctly
 *              e_eFSS_LOGFL_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_LOGFL_RES_NOTVALIDLOG        - No valid log founded
 *              e_eFSS_LOGFL_RES_NEWVERSIONLOG      - New version of the blob requested
 *              e_eFSS_LOGFL_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_LOGFL_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_LOGFL_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_LOGFL_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_LOGFL_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_LOGFL_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_LOGFL_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_LOGFL_RES eFSS_LOGFL_GetStorageStatus(t_eFSS_LOGFL_Ctx* const p_ptCtx);

/**
 * @brief       Get info about the stored log
 *
 * @param[in]   p_ptCtx       - Log context
 * @param[out]  p_puNewLogI   - Pointer to a uint32_t that will be filled with the Newest log index
 * @param[out]  p_puOldLogI   - Pointer to a uint32_t that will be filled with the oldest log index
 * @param[out]  p_puNpageUsed - Pointer to a uint32_t that will be filled with the number of valorized page
 * @param[out]  p_puNpageTot  - Pointer to a uint32_t that will be filled with the number of redable page
 *
 * @return      e_eFSS_LOGFL_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_LOGFL_RES_OK                 - Operation ended correctly
 *              e_eFSS_LOGFL_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_LOGFL_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_LOGFL_RES_NEWVERSIONLOG      - New version of the blob requested
 *              e_eFSS_LOGFL_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_LOGFL_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_LOGFL_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_LOGFL_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_LOGFL_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_LOGFL_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_LOGFL_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_LOGFL_RES eFSS_LOGFL_GetLogInfo(t_eFSS_LOGFL_Ctx* const p_ptCtx, uint32_t *p_puNewLogI, uint32_t *p_puOldLogI,
                                     uint32_t *p_puNpageUsed, uint32_t *p_puNpageTot);

/**
 * @brief       Get read and write buffer. Do not pass to this function NULL value.
 *              Make sure eFSS_LOGFLPRV_IsStatusStillCoherent is called before calling this function.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[out]  p_ptBuff         - Pointer to a struct that will be filled with info about buffer
 *
 * @return      e_eFSS_LOGFL_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_LOGFL_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_LOGFL_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_LOGFL_RES_OK                - Operation ended correctly
 */
e_eFSS_LOGFL_RES eFSS_LOGFLPRV_GetBuffer(t_eFSS_LOGFL_Ctx* const p_ptCtx, t_eFSS_TYPE_StorBuf* p_ptBuff);

/**
 * @brief       Format the memory used for the log, previous data, if present, will be lost.
 *
 * @param[in]   p_ptCtx       - Log context
 *
 * @return      e_eFSS_LOGFL_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_LOGFL_RES_OK                 - Operation ended correctly
 *              e_eFSS_LOGFL_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_LOGFL_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_LOGFL_RES_NEWVERSIONLOG      - New version of the blob requested
 *              e_eFSS_LOGFL_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_LOGFL_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_LOGFL_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_LOGFL_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_LOGFL_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_LOGFL_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_LOGFL_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_LOGFL_RES eFSS_LOGFL_Format(t_eFSS_LOGFL_Ctx* const p_ptCtx);

/**
 * @brief       Get all the log present on a specific page.
 *
 * @param[in]   p_ptCtx        - Log context
 * @param[in]   p_uindx        - Index to get data from
 * @param[in]   p_puBuf        - Pointer to a buffer where founded log will be copied
 * @param[in]   p_puBufL       - Size of the copied log
 * @param[in]   p_uBufMaxL     - Max size of p_puBuf
 *
 * @return      e_eFSS_LOGFL_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_LOGFL_RES_OK                 - Operation ended correctly
 *              e_eFSS_LOGFL_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_LOGFL_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_LOGFL_RES_NEWVERSIONLOG      - New version of the blob requested
 *              e_eFSS_LOGFL_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_LOGFL_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_LOGFL_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_LOGFL_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_LOGFL_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_LOGFL_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_LOGFL_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_LOGFL_RES eFSS_LOGFL_GetLogOfASpecificPage(t_eFSS_LOGFL_Ctx* const p_ptCtx, uint32_t p_uindx, uint8_t* p_puBuf,
                                                uint32_t* p_puBufL, uint32_t p_uBufMaxL);

/**
 * @brief       Add a log with an header
 *
 * @param[in]   p_ptCtx        - Log context
 * @param[in]   p_puLogToSaveH - Pointer to the buffer containing the header of the log to store
 * @param[in]   p_uLogLH       - Dimension in byte of the log header to store
 * @param[in]   p_puLogToSave  - Pointer to the buffer containing the log to store
 * @param[in]   p_uLogL        - Dimension in byte of the log to store
 *
 * @return      e_eFSS_LOGFL_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_LOGFL_RES_OK                 - Operation ended correctly
 *              e_eFSS_LOGFL_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_LOGFL_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_LOGFL_RES_NEWVERSIONLOG      - New version of the blob requested
 *              e_eFSS_LOGFL_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_LOGFL_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_LOGFL_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_LOGFL_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_LOGFL_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_LOGFL_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_LOGFL_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_LOGFL_RES eFSS_LOGFL_AddLogAndHeader(t_eFSS_LOGFL_Ctx* const p_ptCtx, uint8_t* p_puLogToSaveH, uint32_t p_uLogLH,
                                          uint8_t* p_puLogToSave, uint32_t p_uLogL);

/**
 * @brief       Add a log
 *
 * @param[in]   p_ptCtx       - Log context
 * @param[in]   p_puLogToSave - Pointer to the buffer containing the log to store
 * @param[in]   p_uLogL       - Dimension in byte of the log to store
 *
 * @return      e_eFSS_LOGFL_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eFSS_LOGFL_RES_OK                 - Operation ended correctly
 *              e_eFSS_LOGFL_RES_OK_BKP_RCVRD       - All ok, but some page where recovered
 *              e_eFSS_LOGFL_RES_NOTVALIDBLOB       - No valid blob founded
 *              e_eFSS_LOGFL_RES_NEWVERSIONLOG      - New version of the blob requested
 *              e_eFSS_LOGFL_RES_NOINITLIB          - Need to init the lib before calling this function
 *              e_eFSS_LOGFL_RES_CORRUPTCTX         - Context is corrupted
 *              e_eFSS_LOGFL_RES_CLBCKERASEERR      - Erase callback returned error
 *              e_eFSS_LOGFL_RES_CLBCKWRITEERR      - Write callback returned error
 *              e_eFSS_LOGFL_RES_CLBCKREADERR       - Read callback returned error
 *              e_eFSS_LOGFL_RES_CLBCKCRCERR        - Crc callback returned error
 *              e_eFSS_LOGFL_RES_WRITENOMATCHREAD   - After Write operation the Read operation readed different data
 */
e_eFSS_LOGFL_RES eFSS_LOGFL_AddLog(t_eFSS_LOGFL_Ctx* const p_ptCtx, uint8_t* p_puLogToSave, uint32_t p_uLogL);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_LOGFL_H */