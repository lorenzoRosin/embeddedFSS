/**
 * @file       eFSS_LOGC.h
 *
 * @brief      LOG Core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSS_LOGC_H
#define EFSS_LOGC_H



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
    e_eFSS_LOGC_RES_OK = 0,
    e_eFSS_LOGC_RES_NOINITLIB,
    e_eFSS_LOGC_RES_BADPARAM,
    e_eFSS_LOGC_RES_BADPOINTER,
    e_eFSS_LOGC_RES_CORRUPTCTX,
    e_eFSS_LOGC_RES_CLBCKERASEERR,
    e_eFSS_LOGC_RES_CLBCKWRITEERR,
    e_eFSS_LOGC_RES_CLBCKREADERR,
    e_eFSS_LOGC_RES_CLBCKCRCERR,
    e_eFSS_LOGC_RES_NOTVALIDLOG,
    e_eFSS_LOGC_RES_NEWVERSIONLOG,
    e_eFSS_LOGC_RES_WRITENOMATCHREAD,
    e_eFSS_LOGC_RES_OK_BKP_RCVRD,
}e_eFSS_LOGC_RES;

typedef struct
{
    t_eFSS_COREHL_Ctx tCOREHLCtx;
    bool_t bFlashCache;
    bool_t bFullBckup;
}t_eFSS_LOGC_Ctx;



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
 * @return      e_eFSS_LOGC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_LOGC_RES_OK            - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGC_InitCtx(t_eFSS_LOGC_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx const p_tCtxCb,
                                  t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL,
                                  bool_t p_bFlashCache, bool_t p_bFullBckup);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Log Core context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_LOGC_RES_OK            - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGC_IsInit(t_eFSS_LOGC_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Get the reference of buffer that we can use to read or write data from storage
 *
 * @param[in]   p_ptCtx       - Log Core context
 * @param[out]  p_ptBuff      - Pointer to a struct that will be filled with info about buffer
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_LOGC_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_LOGC_RES_OK            - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGC_GetBuff(t_eFSS_LOGC_Ctx* p_ptCtx, t_eFSS_TYPE_StorBuf* p_ptBuff);

/**
 * @brief       Get the value of usable page to save a log.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[out]  p_puUsableP      - Pointer where the number of usable pages will be copied
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_LOGC_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_LOGC_RES_OK            - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGC_GetUsablePage(t_eFSS_LOGC_Ctx* p_ptCtx, uint8_t* p_puUsableP);

/**
 * @brief       Get the value of the next index given a passed one.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_uIdx           - Index that we want to search for next. Usable value: from zero to the value returned
 *                                 by eFSS_LOGC_GetUsablePage
 * @param[out]  p_puNextIdx      - Pointer where the value of the next index will be copied
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_LOGC_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_LOGC_RES_OK            - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGC_GetNextIndex(t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t p_uIdx, uint32_t* p_puNextIdx);

/**
 * @brief       Get the value of the previous index.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_uIdx           - Index that we want to search for previous. Usable value: from zero to the value
 *                                 returned by eFSS_LOGC_GetUsablePage
 * @param[out]  p_puPrevIdx      - Pointer where the value of the previous index will be copied
 *
 * @return      Return the previous index of p_uIdx
 */
e_eFSS_LOGC_RES eFSS_LOGC_GetPrevIndex(t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t p_uIdx, uint32_t* p_puPrevIdx);

/**
 * @brief       Write in cache the value of the new index location and the numbers of filled pages.
 *              Use this function only if flash cache is enabled.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_uIdxN          - Index of the new log page that we want to save in cache
 * @param[in]   p_uIFlP          - Number of filled pages that we want to save
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_LOGC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_LOGC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_LOGC_RES_CLBCKCRCERR       - The crc callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKREADERR      - The read callback reported an error
 *		        e_eFSS_LOGC_RES_WRITENOMATCHREAD  - Writen data dosent match what requested
 *              e_eFSS_LOGC_RES_OK                - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGC_WriteCache(t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t p_uIdxN, uint32_t p_uIFlP);

/**
 * @brief       Read from cache the value of new index location and the numbers of filled pages. This function take
 *              care of the backup pages.
 *              Use this function only if flash cache is enabled.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[out]  p_puIdxN         - Index of the new log page that we want to read from cache
 * @param[out]  p_puIFlP         - Number of filled pages that we want to read
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_LOGC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_LOGC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_LOGC_RES_CLBCKCRCERR       - The crc callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKREADERR      - The read callback reported an error
 *		        e_eFSS_LOGC_RES_WRITENOMATCHREAD  - Writen data dosent match what requested
 *              e_eFSS_LOGC_RES_NOTVALIDPAGE      - both origin and backup pages are corrupted
 *              e_eFSS_LOGC_RES_OK_BKP_RCVRD      - operation ended successfully recovering a backup or an origin
 *                                                  page
 *              e_eFSS_LOGC_RES_OK                - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGC_ReadCache(t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t* p_puIdxN, uint32_t* p_puIFlP);

/**
 * @brief       Flush the buffer in a page at p_uIdx position. Do not pass to this function NULL value
 *              or invalid index value. This function will take care of any support page.
 *              The buffer is managed with subtype related to log only.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_uIdx           - Index of the log page we want to write
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_LOGC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_LOGC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_LOGC_RES_CLBCKCRCERR       - The crc callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKREADERR      - The read callback reported an error
 *		        e_eFSS_LOGC_RES_WRITENOMATCHREAD  - Writen data dosent match what requested
 *              e_eFSS_LOGC_RES_OK                - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGC_FlushBufferAsLog(t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t p_uIdx);

/**
 * @brief       Read a page of data at p_uIdx position. Do not pass to this function NULL value
 *              or invalid index value. This function will take care of any support page.
 *              Do not use this function on Flash cache pages.
 *              The buffer is managed with subtype related to log only.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_uIdx           - Index of the log page we want to read
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_LOGC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_LOGC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_LOGC_RES_CLBCKCRCERR       - The crc callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKREADERR      - The read callback reported an error
 *		        e_eFSS_LOGC_RES_WRITENOMATCHREAD  - Writen data dosent match what requested
 *              e_eFSS_LOGC_RES_NOTVALIDPAGE      - both origin and backup pages are corrupted
 *              e_eFSS_LOGC_RES_OK_BKP_RCVRD      - operation ended successfully recovering a backup or an origin
 *                                                  page
 *              e_eFSS_LOGC_RES_OK                - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGC_LoadBufferAsLog(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t p_uIdx);

/**
 * @brief       Flush the buffer in a page at p_uIdx position as newest only. Do not pass to this function NULL value
 *              or invalid index value. This function will take care of any support page.
 *              Make sure eFSS_LOGC_IsStatusStillCoherent is called before calling this function.
 *              Do not use this function on Flash cache pages.
 *              The buffer is managed with subtype related to newest page only.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_uIdx           - Index of the newest page we want to write
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_LOGC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_LOGC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_LOGC_RES_CLBCKCRCERR       - The crc callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKREADERR      - The read callback reported an error
 *		        e_eFSS_LOGC_RES_WRITENOMATCHREAD  - Writen data dosent match what requested
 *              e_eFSS_LOGC_RES_OK                - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGC_FlushBufferAsNewestOnly(t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t p_uIdx);

































/**
 * @brief       Flush the buffer in a page at p_uIdx position as newest bkup only. Do not pass to this function NULL
 *              value or invalid index value. This function will take care of any support page.
 *              Make sure eFSS_LOGC_IsStatusStillCoherent is called before calling this function.
 *              Do not use this function on Flash cache pages.
 *              The buffer is managed with subtype related to newest page only.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_uIdx           - Index of the newest page we want to write
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_LOGC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_LOGC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_LOGC_RES_CLBCKCRCERR       - The crc callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKREADERR      - The read callback reported an error
 *		        e_eFSS_LOGC_RES_WRITENOMATCHREAD  - Writen data dosent match what requested
 *              e_eFSS_LOGC_RES_OK                - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGCPRV_FlushBufferAsNewestBkupOnly(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t p_uIdx);

/**
 * @brief       Write a page of data at p_uIdx position as newest and bkup. Do not pass to this function NULL value
 *              or invalid index value. This function will take care of any support page.
 *              Make sure eFSS_LOGC_IsStatusStillCoherent is called before calling this function.
 *              Do not use this function on Flash cache pages.
 *              The buffer is managed with subtype related to newest page only.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_uIdx           - Index of the new newest log page that we want to write
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_LOGC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_LOGC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_LOGC_RES_CLBCKCRCERR       - The crc callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKREADERR      - The read callback reported an error
 *		        e_eFSS_LOGC_RES_WRITENOMATCHREAD  - Writen data dosent match what requested
 *              e_eFSS_LOGC_RES_OK                - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGCPRV_FlushBufferAsNewestNBkpPage(t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t p_uIdx);

/**
 * @brief       Read a page of data at p_uIdx position. Do not pass to this function NULL value
 *              or invalid index value. This function will take care of any support page.
 *              Make sure eFSS_LOGC_IsStatusStillCoherent is called before calling this function.
 *              Do not use this function on Flash cache pages.
 *              The buffer is managed with subtype related to newest page and newest backup only.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_uIdx           - Index of the new log page that we want to read from cache
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_LOGC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_LOGC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_LOGC_RES_CLBCKCRCERR       - The crc callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKREADERR      - The read callback reported an error
 *		        e_eFSS_LOGC_RES_WRITENOMATCHREAD  - Writen data dosent match what requested
 *              e_eFSS_LOGC_RES_OK                - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGCPRV_LoadBufferAsNewestNBkpPage(t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t p_uIdx);












#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_LOGC_H */