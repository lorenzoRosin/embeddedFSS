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
    uint8_t*  puBuf;
    uint32_t  uBufL;
}t_eFSS_BLOBC_StorBuf;

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
 * @param[in]   p_puBuff       - Pointer to a buffer used by the modules to make calc, must be pageSize * 2
 * @param[in]   p_uBuffL       - Size of p_puBuff
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_BLOBC_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_BLOBC_RES_OK            - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_BLOBC_InitCtx(t_eFSS_BLOBC_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx const p_tCtxCb,
                                    const t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff,
                                    const uint32_t p_uBuffL);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Blob Core context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_BLOBC_RES_OK            - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_BLOBC_IsInit(t_eFSS_BLOBC_Ctx* const p_ptCtx, bool_t* const p_pbIsInit);

/**
 * @brief       Get the reference of buffer that we can use to read or write data from storage
 *
 * @param[in]   p_ptCtx       - Blob Core context
 * @param[out]  p_ptBuff      - Pointer to a struct that will be filled with info about buffer
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_BLOBC_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_BLOBC_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_BLOBC_RES_OK            - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_BLOBC_GetBuff(t_eFSS_BLOBC_Ctx* p_ptCtx, t_eFSS_TYPE_StorBuf* p_ptBuff);

/**
 * @brief       Get the value of usable page to save a blob.
 *
 * @param[in]   p_ptCtx          - Blob Core context
 * @param[out]  p_puUsableP      - Pointer where the number of usable pages will be copied
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_BLOBC_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_BLOBC_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_BLOBC_RES_OK            - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_BLOBC_GetUsablePage(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t* p_puUsableP);

/**
 * @brief       Retrive the stored value of blob crc and blob length. If the blob is invalid the returned value
 *              is not valid.
 *
 * @param[in]   p_ptCtx          - Blob Core context
 * @param[in]   p_puBlobLen      - Pointer where the readed size of blob will be stored
 * @param[in]   p_puBlobCrc      - Pointer where the readed CRC of blob will be stored
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_BLOBC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_BLOBC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_BLOBC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_BLOBC_RES_CLBCKCRCERR       - The crc callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKREADERR      - The read callback reported an error
 *		        e_eFSS_BLOBC_RES_WRITENOMATCHREAD  - Writen data dosent match what requested
 *              e_eFSS_BLOBC_RES_OK                - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_LOGC_GetOriginalPageMeta(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t* p_puBlobLen, uint32_t* p_puBlobCrc);

/**
 * @brief       Save the value of blob crc and blob length. This function only work on page that are already valid and
 *              dosent modify any value already present on the page itself.
 *
 * @param[in]   p_ptCtx          - Blob Core context
 * @param[in]   p_uBlobLen       - Pointer where the readed size of blob will be stored
 * @param[in]   p_uBlobCrc       - Pointer where the readed CRC of blob will be stored
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_BLOBC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_BLOBC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_BLOBC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_BLOBC_RES_CLBCKCRCERR       - The crc callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKREADERR      - The read callback reported an error
 *		        e_eFSS_BLOBC_RES_WRITENOMATCHREAD  - Writen data dosent match what requested
 *              e_eFSS_BLOBC_RES_OK                - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_LOGC_SetOriginalPageMeta(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t p_uBlobLen, uint32_t p_uBlobCrc);

/**
 * @brief       Retrive the stored value of blob crc and blob length. If the blob is invalid the returned value
 *              is not valid.
 *
 * @param[in]   p_ptCtx          - Blob Core context
 * @param[in]   p_puBlobLen      - Pointer where the readed size of blob will be stored
 * @param[in]   p_puBlobCrc      - Pointer where the readed CRC of blob will be stored
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_BLOBC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_BLOBC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_BLOBC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_BLOBC_RES_CLBCKCRCERR       - The crc callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKREADERR      - The read callback reported an error
 *		        e_eFSS_BLOBC_RES_WRITENOMATCHREAD  - Writen data dosent match what requested
 *              e_eFSS_BLOBC_RES_OK                - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_LOGC_GetBckupPageMeta(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t* p_puBlobLen, uint32_t* p_puBlobCrc);

/**
 * @brief       Flush the buffer in a page at p_uIdx position. Do not pass to this function NULL value
 *              or invalid index value. This function will take care of any support page.
 *              The buffer is managed with subtype related to log only.
 *
 * @param[in]   p_ptCtx          - Blob Core context
 * @param[in]   p_uIdx           - Index of the log page we want to write
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_BLOBC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_BLOBC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_BLOBC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_BLOBC_RES_CLBCKCRCERR       - The crc callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKREADERR      - The read callback reported an error
 *		        e_eFSS_BLOBC_RES_WRITENOMATCHREAD  - Writen data dosent match what requested
 *              e_eFSS_BLOBC_RES_OK                - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_LOGC_FlushBufferInNewPage(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t p_uSeqN, uint32_t p_uIdx);

/**
 * @brief       Flush the buffer in a page at p_uIdx position. Do not pass to this function NULL value
 *              or invalid index value. This function will take care of any support page.
 *              The buffer is managed with subtype related to log only.
 *
 * @param[in]   p_ptCtx          - Blob Core context
 * @param[in]   p_uIdx           - Index of the log page we want to write
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_BLOBC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_BLOBC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_BLOBC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_BLOBC_RES_CLBCKCRCERR       - The crc callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKREADERR      - The read callback reported an error
 *		        e_eFSS_BLOBC_RES_WRITENOMATCHREAD  - Writen data dosent match what requested
 *              e_eFSS_BLOBC_RES_OK                - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_LOGC_LoadBufferFromNewPage(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t* p_puSeqN, uint32_t p_uIdx);

/**
 * @brief       Flush the buffer in a page at p_uIdx position. Do not pass to this function NULL value
 *              or invalid index value. This function will take care of any support page.
 *              The buffer is managed with subtype related to log only.
 *
 * @param[in]   p_ptCtx          - Blob Core context
 * @param[in]   p_uIdx           - Index of the log page we want to write
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_BLOBC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_BLOBC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_BLOBC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_BLOBC_RES_CLBCKCRCERR       - The crc callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKREADERR      - The read callback reported an error
 *		        e_eFSS_BLOBC_RES_WRITENOMATCHREAD  - Writen data dosent match what requested
 *              e_eFSS_BLOBC_RES_OK                - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_LOGC_FlushBufferInBkupPage(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t p_uSeqN, uint32_t p_uIdx);

/**
 * @brief       Flush the buffer in a page at p_uIdx position. Do not pass to this function NULL value
 *              or invalid index value. This function will take care of any support page.
 *              The buffer is managed with subtype related to log only.
 *
 * @param[in]   p_ptCtx          - Blob Core context
 * @param[in]   p_uIdx           - Index of the log page we want to write
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_BLOBC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_BLOBC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_BLOBC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_BLOBC_RES_CLBCKCRCERR       - The crc callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKREADERR      - The read callback reported an error
 *		        e_eFSS_BLOBC_RES_WRITENOMATCHREAD  - Writen data dosent match what requested
 *              e_eFSS_BLOBC_RES_OK                - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_LOGC_LoadBufferFromBkupPage(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t* p_puSeqN, uint32_t p_uIdx);

/**
 * @brief       Flush the buffer in a page at p_uIdx position. Do not pass to this function NULL value
 *              or invalid index value. This function will take care of any support page.
 *              The buffer is managed with subtype related to log only.
 *
 * @param[in]   p_ptCtx          - Blob Core context
 * @param[in]   p_uIdx           - Index of the log page we want to write
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_BLOBC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_BLOBC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_BLOBC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_BLOBC_RES_CLBCKCRCERR       - The crc callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKREADERR      - The read callback reported an error
 *		        e_eFSS_BLOBC_RES_WRITENOMATCHREAD  - Writen data dosent match what requested
 *              e_eFSS_BLOBC_RES_OK                - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_LOGC_GetCrcFromTheBuffer(t_eFSS_BLOBC_Ctx* p_ptCtx, uint32_t p_uSeed, uint32_t* p_puCrc);

/**
 * @brief       Flush the buffer in a page at p_uIdx position. Do not pass to this function NULL value
 *              or invalid index value. This function will take care of any support page.
 *              The buffer is managed with subtype related to log only.
 *
 * @param[in]   p_ptCtx          - Blob Core context
 * @param[in]   p_uIdx           - Index of the log page we want to write
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_BLOBC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_BLOBC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_BLOBC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_BLOBC_RES_CLBCKCRCERR       - The crc callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKREADERR      - The read callback reported an error
 *		        e_eFSS_BLOBC_RES_WRITENOMATCHREAD  - Writen data dosent match what requested
 *              e_eFSS_BLOBC_RES_OK                - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_LOGC_GenerateBkup(t_eFSS_BLOBC_Ctx* p_ptCtx);

/**
 * @brief       Flush the buffer in a page at p_uIdx position. Do not pass to this function NULL value
 *              or invalid index value. This function will take care of any support page.
 *              The buffer is managed with subtype related to log only.
 *
 * @param[in]   p_ptCtx          - Blob Core context
 * @param[in]   p_uIdx           - Index of the log page we want to write
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_BLOBC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_BLOBC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_BLOBC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_BLOBC_RES_CLBCKCRCERR       - The crc callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKREADERR      - The read callback reported an error
 *		        e_eFSS_BLOBC_RES_WRITENOMATCHREAD  - Writen data dosent match what requested
 *              e_eFSS_BLOBC_RES_OK                - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_LOGC_RipristinateOriginal(t_eFSS_BLOBC_Ctx* p_ptCtx);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_BLOBC_H */