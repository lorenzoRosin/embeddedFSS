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
 *      PUBLIC TYPEDEFS
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
    e_eFSS_LOGC_RES_NEWVERSIONFOUND,
    e_eFSS_LOGC_RES_WRITENOMATCHREAD,
    e_eFSS_LOGC_RES_OK_BKP_RCVRD
}e_eFSS_LOGC_RES;

typedef enum
{
    e_eFSS_LOGC_PAGETYPE_LOG = 0,
    e_eFSS_LOGC_PAGETYPE_NEWEST,
    e_eFSS_LOGC_PAGETYPE_NEWEST_BKUP
}e_eFSS_LOGC_PAGETYPE;

typedef struct
{
    uint8_t* puBuf;
    uint32_t uBufL;
}t_eFSS_LOGC_StorBuf;

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
 * @param[in]   p_puBuff         - Pointer to a buffer used by the modules to make calc, must be pageSize * 2
 * @param[in]   p_uBuffL         - Size of p_puBuff
 * @param[in]   p_bFlashCache    - Use flash as cache for storing and resuming index
 * @param[in]   p_bFullBckup     - Save every log data in a backup pages
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER    - In case of bad pointer passed to the functione
 *		        e_eFSS_LOGC_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_LOGC_RES_OK            - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGC_InitCtx(t_eFSS_LOGC_Ctx* const p_ptCtx, const t_eFSS_TYPE_CbStorCtx p_tCtxCb,
                                  const t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff,
                                  const uint32_t p_uBuffL, const bool_t p_bFlashCache, const bool_t p_bFullBckup);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Log Core context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_LOGC_RES_OK            - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGC_IsInit(t_eFSS_LOGC_Ctx* const p_ptCtx, bool_t* const p_pbIsInit);

/**
 * @brief       Check if the flash cache is used
 *
 * @param[in]   p_ptCtx               - Log Core context
 * @param[out]  p_pbIsFlashCacheUsed  - Pointer to a bool_t variable that will be filled with true if the flash cache
 *                                      is used
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_LOGC_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_LOGC_RES_OK            - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGC_IsFlashCacheUsed(t_eFSS_LOGC_Ctx* const p_ptCtx, bool_t* const p_pbIsFlashCacheUsed);

/**
 * @brief       Get the numbers of usable page and the internal buffer
 *
 * @param[in]   p_ptCtx       - Log Core context
 * @param[out]  p_ptBuff      - Pointer to a storage struct that will be filled with info about internal buf
 * @param[out]  p_puUsePages  - Pointer to a uint32_t variable that will be filled with the numbers of usable pages
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_LOGC_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_LOGC_RES_OK            - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGC_GetBuffNUsable(t_eFSS_LOGC_Ctx* const p_ptCtx, t_eFSS_LOGC_StorBuf* const p_ptBuff,
                                         uint32_t* const p_puUsePages);

/**
 * @brief       Write in cache the value of the new index location and the numbers of filled pages.
 *              The new index location refers to the current index that we are using to store data, the number of
 *              filled pages indicate the number of completly filled page (from this calculation the newest index is
 *              excluded because it's where we are still writing or reading log).
 *              This function take care of the backup pages. Use this function only if flash cache is enabled.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_uIdxN          - Index of the new log page that we want to save in cache
 * @param[in]   p_uFilP          - Number of filled pages that we want to save
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_LOGC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_LOGC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_LOGC_RES_CLBCKCRCERR       - The crc callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKREADERR      - The read callback reported an error
 *		        e_eFSS_LOGC_RES_WRITENOMATCHREAD  - For some unknow reason data write dosent match data readed
 *              e_eFSS_LOGC_RES_OK                - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGC_WriteCache(t_eFSS_LOGC_Ctx* const p_ptCtx, const uint32_t p_uIdxN, const uint32_t p_uFilP);

/**
 * @brief       Read from cache the value of new index location and the numbers of filled pages.
 *              The new index location refers to the current index that we are using to store data, the number of
 *              filled pages indicate the number of completly filled page (from this calculation the newest index is
 *              excluded because it's where we are still writing or reading log).
 *              This function take care of the backup pages. Use this function only if flash cache is enabled.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[out]  p_puIdxN         - Pointer to an uint32_t that will be filled with the value of the index of the new
 *                                 logs pages that we want to read from cache
 * @param[out]  p_puFilP         - Pointer to an uint32_t that will be filled with the value of the number of filled
 *                                 pages that we want to read
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_LOGC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_LOGC_RES_NOINITLIB         - Need to init lib before calling function
 *		        e_eFSS_LOGC_RES_CLBCKREADERR      - The read callback reported an error
 *              e_eFSS_LOGC_RES_CLBCKCRCERR       - The crc callback reported an error
 *              e_eFSS_LOGC_RES_NOTVALIDLOG       - both origin and backup pages are corrupted
 *              e_eFSS_LOGC_RES_NEWVERSIONFOUND   - The readed page has a new version
 *		        e_eFSS_LOGC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_LOGC_RES_WRITENOMATCHREAD  - For some unknow reason data write dosent match data readed
 *              e_eFSS_LOGC_RES_OK_BKP_RCVRD      - Operation ended successfully recovering a backup or an origin
 *                                                  page
 *              e_eFSS_LOGC_RES_OK                - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGC_ReadCache(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t* const p_puIdxN, uint32_t* const p_puFilP);

/**
 * @brief       Flush the buffer in a page at p_uIdx position using p_ePageType as subtype and with a number of
 *              valorized byte equals to p_uFillInPage. This function will take care of any support page when the
 *              fullbackup option is enabled. Keep in mind that this function cannot overide flash cache pages.
 *              After this operation the internal buffer will mantains it's original values, except for the private
 *              metadata.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_ePageType      - SubType of the page we are flushing
 * @param[in]   p_uIdx           - Index of the log page we want to write
 * @param[in]   p_uFillInPage    - Number of filled byte in page, this value dosent comprend the space needed to store
 *                                 this parameter. So the max value can be the size of the buffer returned by
 *                                 eFSS_LOGC_GetBuffNUsable minus the sizeof(uint32_t)
 *
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_LOGC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_LOGC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_LOGC_RES_CLBCKCRCERR       - The crc callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKREADERR      - The read callback reported an error
 *		        e_eFSS_LOGC_RES_WRITENOMATCHREAD  - For some unknow reason data write dosent match data readed
 *              e_eFSS_LOGC_RES_OK                - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGC_FlushBufferAs(t_eFSS_LOGC_Ctx* const p_ptCtx, const e_eFSS_LOGC_PAGETYPE p_ePageType,
                                        const uint32_t p_uIdx, const uint32_t p_uFillInPage);

/**
 * @brief       Read a page of data at p_uIdx position with an expected subtype equals to p_ePageType. Load in
 *              p_puFillInPage the numbers os previously saved valorized byte in page. This function
 *              will take care of any support page when the fullbackup option is enabled and can so recover,
 *              only in this case, corrupted pages. Keep in mind that this function cannot read flash cache pages
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_ePageType      - Subtype of the page we are reading. If pagesubtype dosent match readed subytype the
 *                                 page will be considerated invalid
 * @param[in]   p_uIdx           - Index of the log page we want to read
 * @param[out]  p_puFillInPage   - Pointer to an uint32_t where the number of filled byte in page will be copied.
 *                                 This value dosent comprend the space needed to store this parameter.
 *                                 So the max value can be the size of the buffer returned by
 *                                 eFSS_LOGC_GetBuffNUsable minus the sizeof(uint32_t)
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_LOGC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_LOGC_RES_NOINITLIB         - Need to init lib before calling function
 *		        e_eFSS_LOGC_RES_CLBCKREADERR      - The read callback reported an error
 *              e_eFSS_LOGC_RES_CLBCKCRCERR       - The crc callback reported an error
 *              e_eFSS_LOGC_RES_NOTVALIDLOG       - Both origin and backup pages are corrupted
 *              e_eFSS_LOGC_RES_NEWVERSIONFOUND   - The readed page has a new version
 *		        e_eFSS_LOGC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_LOGC_RES_WRITENOMATCHREAD  - For some unknow reason data write dosent match data readed
 *              e_eFSS_LOGC_RES_OK_BKP_RCVRD      - Operation ended successfully recovering a backup or an origin
 *                                                  page
 *              e_eFSS_LOGC_RES_OK                - Operation ended correctly
 */
e_eFSS_LOGC_RES eFSS_LOGC_LoadBufferAs(t_eFSS_LOGC_Ctx* const p_ptCtx, const e_eFSS_LOGC_PAGETYPE p_ePageType,
                                       const uint32_t p_uIdx, uint32_t* const p_puFillInPage);

/**
 * @brief       Load in to the internal buffer the value of a page pointed by the index p_uIdx. The loaded page will
 *              be considered valid only if the subtype of the page itself is e_eFSS_LOGC_PAGETYPE_NEWEST or
 *              e_eFSS_LOGC_PAGETYPE_NEWEST_BKUP. When a valid page is found e_eFSS_LOGC_RES_OK is returned and
 *              the field p_pbIsNewest will be valorized with true if the founded subtype is
 *              e_eFSS_LOGC_PAGETYPE_NEWEST.
 *              When a new or new backup page is found and fullbackup is enable this function will take care of
 *              backup pages.
 *              If the founded page is new or newbackp the internal buffer will contain valid data
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_uIdx           - Index of the log page we want to read
 * @param[out]  p_pbIsNewest     - When e_eFSS_LOGC_RES_OK is returned this boolean is valorize with true if the
 *                                 page subtype is e_eFSS_LOGC_PAGETYPE_NEWEST, and with false if the page subtype is
 *                                 e_eFSS_LOGC_PAGETYPE_NEWEST_BKUP.
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_LOGC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_LOGC_RES_NOINITLIB         - Need to init lib before calling function
 *		        e_eFSS_LOGC_RES_CLBCKREADERR      - The read callback reported an error
 *              e_eFSS_LOGC_RES_CLBCKCRCERR       - The crc callback reported an error
 *              e_eFSS_LOGC_RES_NOTVALIDLOG       - Both origin and backup pages are not what we are looking for.
 *                                                  Page can be corrupted or just not having the needed subtype
 *              e_eFSS_LOGC_RES_NEWVERSIONFOUND   - The readed page has a new version
 *		        e_eFSS_LOGC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_LOGC_RES_WRITENOMATCHREAD  - For some unknow reason data write dosent match data readed
 *              e_eFSS_LOGC_RES_OK_BKP_RCVRD      - Operation ended successfully recovering a backup or an origin
 *                                                  page. The subtype of the pages are matching newest or newestbkup
 *              e_eFSS_LOGC_RES_OK                - Operation ended correctly. The subtype of the pages are matching
 *                                                  newest or newestbkup
 */
e_eFSS_LOGC_RES eFSS_LOGC_IsPageNewOrBkup(t_eFSS_LOGC_Ctx* const p_ptCtx, const uint32_t p_uIdx,
                                          bool_t* const p_pbIsNewest);

/**
 * @brief       Check if the data present in to the internal buffer is equals to an another page pointed by the
 *              index p_uIdx, if the data are not equals flush the internal buffer data in to the pointed page.
 *              If pages are equals do nothing.
 *              If page pointed by p_uIdx is invalid or is valid but the subtype of the page itself is different
 *              from p_eTypeFlush, flush anyway the data present in to the internal buffer.
 *              Keep in mind that subtype are not compared, the comparsion is only done using raw data (so we
 *              are comparing numbersofbyteinpage also).
 *              Th previusly present data in buffer is not modified after this call.
 *              If fullbackup is enabled this page will take care of backup pages.
 *
 * @param[in]   p_ptCtx         - Log Core context
 * @param[in]   p_uIdx          - uint32_t index rappresenting the page that we want to compare and eventualy flush
 * @param[in]   p_eTypeFlush    - Subtype that the pointe page need to have to be considered a valid page. If page
 *                                is valid but subtype dosent page the flush operation will be performed even if
 *                                data present in buffer match the one present in to the page
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_BADPARAM          - In case of an invalid parameter passed to the functionsB
 *		        e_eFSS_LOGC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_LOGC_RES_NOINITLIB         - Need to init lib before calling function
 *		        e_eFSS_LOGC_RES_CLBCKREADERR      - The read callback reported an error
 *              e_eFSS_LOGC_RES_CLBCKCRCERR       - The crc callback reported an error
 *              e_eFSS_LOGC_RES_NEWVERSIONFOUND   - The readed page has a new version
 *		        e_eFSS_LOGC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_LOGC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_LOGC_RES_WRITENOMATCHREAD  - For some unknow reason data write dosent match data readed
 *              e_eFSS_LOGC_RES_OK_BKP_RCVRD      - Operation ended successfully recovering a backup or an origin
 *                                                  page, because they were different from the buffer contenent
 *              e_eFSS_LOGC_RES_OK                - Operation ended correctly, no flush operation performed
 */
e_eFSS_LOGC_RES eFSS_LOGC_FlushBuffIfNotEquals(t_eFSS_LOGC_Ctx* const p_ptCtx, const uint32_t p_uIdx,
                                               const e_eFSS_LOGC_PAGETYPE p_eTypeFlush);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_LOGC_H */