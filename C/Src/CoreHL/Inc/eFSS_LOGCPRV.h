/**
 * @file       eFSS_LOGCPRV.h
 *
 * @brief      LOG Core private module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSS_LOGCPRV_H
#define EFSS_LOGCPRV_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_LOGC.h"



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Verify if the log core context is valid. Be sure to no pass null to this function
 *
 * @param[in]   p_ptCtx          - Log Core context
 *
 * @return      true    - The t_eFSS_LOGC_Ctx context is valid
 *		        false   - Detected some corruption in the t_eFSS_LOGC_Ctx context
 */
bool_t eFSS_LOGCPRV_IsStatusStillCoherent(t_eFSS_LOGC_Ctx* p_ptCtx);

/**
 * @brief       Convert enum from HL utils to LOGC module enum
 *
 * @param[in]   p_eHLRes      - HL utils enum value
 *
 * @return      Return the corrisponding value from HL utils operation to LOGC enum code
 */
e_eFSS_LOGC_RES eFSS_LOGCPRV_HLtoLogRes(const e_eFSS_COREHL_RES p_eHLRes);

/**
 * @brief       Get the value of usable page to save a log. Be sure to not insert any NULL value and
 *              call eFSS_LOGCPRV_IsStatusStillCoherent before.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_tStorSet       - Storage settings
 *
 * @return      Return the numbers of usable page to save a log, excluding flash cache and backup pages
 */
uint32_t eFSS_LOGCPRV_GetUsablePage(const t_eFSS_LOGC_Ctx* p_ptCtx, t_eFSS_TYPE_StorSet p_tStorSet);

/**
 * @brief       Get the value of the next index given a passed one. Be sure to not insert any NULL value and
 *              call eFSS_LOGCPRV_IsStatusStillCoherent before. Do not insert non valid index.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_tStorSet       - Storage settings
 * @param[in]   p_uIdx           - Index that we want to search for next. Usable value: from zero to the value returned
 *                                 by eFSS_LOGCPRV_GetUsablePage
 *
 * @return      Return the next index of p_uIdx
 */
uint32_t eFSS_LOGCPRV_GetNextIndex(const t_eFSS_LOGC_Ctx* p_ptCtx, t_eFSS_TYPE_StorSet p_tStorSet, uint32_t p_uIdx);

/**
 * @brief       Get the value of the previous index. Be sure to not insert any NULL value and
 *              call eFSS_LOGCPRV_IsStatusStillCoherent before. Do not insert non valid index.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_tStorSet       - Storage settings
 * @param[in]   p_uIdx           - Index that we want to search for previous. Usable value: from zero to the value
 *                                 returned by eFSS_LOGCPRV_GetUsablePage
 *
 * @return      Return the previous index of p_uIdx
 */
uint32_t eFSS_LOGCPRV_GetPrevIndex(const t_eFSS_LOGC_Ctx* p_ptCtx, t_eFSS_TYPE_StorSet p_tStorSet, uint32_t p_uIdx);

/**
 * @brief       Write in cache the value of the new index location and the numbers of filled pages.
 *              This function take care of the backup pages. Do not pass to this function NULL pointer or invalid
 *              index, they are not checked. Make sure eFSS_LOGCPRV_IsStatusStillCoherent is called before calling this
 *              function. Use this function only if flash cache is enabled.
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
e_eFSS_LOGC_RES eFSS_LOGCPRV_WriteCache(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t p_uIdxN, uint32_t p_uIFlP);

/**
 * @brief       Read from cache the value of new index location and the numbers of filled pages. This function take
 *              care of the backup pages. Do not pass to this function NULL pointer or invalid index, they are
 *              not checked. Make sure eFSS_LOGCPRV_IsStatusStillCoherent is called before calling this function.
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
e_eFSS_LOGC_RES eFSS_LOGCPRV_ReadCache(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t* p_puIdxN, uint32_t* p_puIFlP);

/**
 * @brief       Get read and write buffer. Do not pass to this function NULL value.
 *              Make sure eFSS_LOGCPRV_IsStatusStillCoherent is called before calling this function.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[out]  p_ptBuff         - Pointer to a struct that will be filled with info about buffer
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_LOGC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_COREHL_RES_OK              - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGCPRV_GetBuffer(t_eFSS_LOGC_Ctx* const p_ptCtx, t_eFSS_TYPE_StorBuf* p_ptBuff);

/**
 * @brief       Flush the buffer in a page at p_uIdx position. Do not pass to this function NULL value
 *              or invalid index value. This function will take care of any support page.
 *              Make sure eFSS_LOGCPRV_IsStatusStillCoherent is called before calling this function.
 *              Do not use this function on Flash cache pages.
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
e_eFSS_LOGC_RES eFSS_LOGCPRV_FlushBufferAsLog(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t p_uIdx);

/**
 * @brief       Read a page of data at p_uIdx position. Do not pass to this function NULL value
 *              or invalid index value. This function will take care of any support page.
 *              Make sure eFSS_LOGCPRV_IsStatusStillCoherent is called before calling this function.
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
e_eFSS_LOGC_RES eFSS_LOGCPRV_LoadBufferAsLog(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t p_uIdx);

/**
 * @brief       Flush the buffer in a page at p_uIdx position as newest only. Do not pass to this function NULL value
 *              or invalid index value. This function will take care of any support page.
 *              Make sure eFSS_LOGCPRV_IsStatusStillCoherent is called before calling this function.
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
e_eFSS_LOGC_RES eFSS_LOGCPRV_FlushBufferAsNewestOnly(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t p_uIdx);

/**
 * @brief       Flush the buffer in a page at p_uIdx position as newest bkup only. Do not pass to this function NULL
 *              value or invalid index value. This function will take care of any support page.
 *              Make sure eFSS_LOGCPRV_IsStatusStillCoherent is called before calling this function.
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
 *              Make sure eFSS_LOGCPRV_IsStatusStillCoherent is called before calling this function.
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
 *              Make sure eFSS_LOGCPRV_IsStatusStillCoherent is called before calling this function.
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



#endif /* EFSS_LOGCPRV_H */