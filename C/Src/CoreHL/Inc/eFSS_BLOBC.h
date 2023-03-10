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
 *      PUBLIC TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eFSS_BLOBC_RES_OK = 0,
    e_eFSS_BLOBC_RES_NOINITLIB,
    e_eFSS_BLOBC_RES_BADPARAM,
    e_eFSS_BLOBC_RES_BADPOINTER,
    e_eFSS_BLOBC_RES_CORRUPTCTX,
    e_eFSS_BLOBC_RES_CLBCKERASEERR,
    e_eFSS_BLOBC_RES_CLBCKWRITEERR,
    e_eFSS_BLOBC_RES_CLBCKREADERR,
    e_eFSS_BLOBC_RES_CLBCKCRCERR,
    e_eFSS_BLOBC_RES_NOTVALIDBLOB,
    e_eFSS_BLOBC_RES_NEWVERSIONBLOB,
    e_eFSS_BLOBC_RES_WRITENOMATCHREAD,
    e_eFSS_BLOBC_RES_OK_BKP_RCVRD,
}e_eFSS_BLOBC_RES;

typedef struct
{
    uint8_t* puBuf;
    uint32_t uBufL;
}t_eFSS_BLOBC_StorBuf;

typedef struct
{
    t_eFSS_COREHL_Ctx tCOREHLCtx;
}t_eFSS_BLOBC_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the Blob core module context
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
e_eFSS_BLOBC_RES eFSS_BLOBC_InitCtx(t_eFSS_BLOBC_Ctx* const p_ptCtx, const t_eFSS_TYPE_CbStorCtx p_tCtxCb,
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
 * @brief       Get the numbers of usable page and the storage buffer
 *
 * @param[in]   p_ptCtx       - Blob Core context
 * @param[out]  p_ptBuff      - Pointer to a storage struct that will be filled with info about internal buffer
 * @param[out]  p_puUsePages  - Pointer to an uint32_t that will be filled with the numbers of usable pages
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_BLOBC_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_BLOBC_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_BLOBC_RES_OK            - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_BLOBC_GetBuffNUsable(t_eFSS_BLOBC_Ctx* const p_ptCtx, t_eFSS_BLOBC_StorBuf* const p_ptBuff,
                                           uint32_t* const p_puUsePages);

/**
 * @brief       Load the buffer from a page at p_uIdx position, and read it's p_uSeqN number. We can read from
 *              the original area or from the backup one.
 *
 * @param[in]   p_ptCtx       - Blob Core context
 * @param[in]   p_bInOrigin   - Choose if read page from origin or backup area
 * @param[in]   p_uIdx        - Index of the page we want to read from
 * @param[out]  p_puSeqN      - An uint32_t value that will be filled with the sequence number from the page
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
e_eFSS_BLOBC_RES eFSS_BLOBC_LoadBufferFromPage(t_eFSS_BLOBC_Ctx* const p_ptCtx, const bool_t p_bInOrigin,
                                               const uint32_t p_uIdx, uint32_t* const p_puSeqN);

/**
 * @brief       Flush the buffer in a page at p_uIdx position, and with p_uSeqN as sequence number. We can write in
 *              to the original area or in to the backup one.
 *
 * @param[in]   p_ptCtx       - Blob Core context
 * @param[in]   p_bInOrigin   - Choose if write page in origin or backup area
 * @param[in]   p_uIdx        - Index of the page we want to write in to
 * @param[in]   p_uSeqN       - An uint32_t value that rappresent the sequential number that we want to store
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
e_eFSS_BLOBC_RES eFSS_BLOBC_FlushBufferInPage(t_eFSS_BLOBC_Ctx* const p_ptCtx, const bool_t p_bInOrigin,
                                              const uint32_t p_uIdx, const uint32_t p_uSeqN);

/**
 * @brief       Retrive the CRC value from the page present in the buffer getted using eFSS_BLOBC_GetBuffNUsable.
 *              The value of the getted CRC does not comprend the sequential number of the page, but only the
 *              raw data present in the buffer.
 *
 * @param[in]   p_ptCtx     - Blob Core context
 * @param[in]   p_uSeed     - Seed to use during the calculation
 * @param[out]  p_puCrc     - An uint32_t value that will be filled with the calculated CRC
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
e_eFSS_BLOBC_RES eFSS_BLOBC_GetCrcFromTheBuffer(t_eFSS_BLOBC_Ctx* const p_ptCtx, const uint32_t p_uSeed,
                                                uint32_t* const p_puCrc);

/**
 * @brief       If the original page is not equals to the backup pages, copy the original page in to the backup area.
 *              If they are equals do nothing
 *
 * @param[in]   p_ptCtx      - Blob Core context
 * @param[in]   p_uIdx       - Index of the original and backup pages where to do this check.
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_BLOBC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_BLOBC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_BLOBC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_BLOBC_RES_CLBCKCRCERR       - The crc callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKERASEERR     - The erase callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKWRITEERR     - The write callback reported an error
 *		        e_eFSS_BLOBC_RES_CLBCKREADERR      - The read callback reported an errorp_uIdx
 *		        e_eFSS_BLOBC_RES_WRITENOMATCHREAD  - Writen data dosent match what requested
 *              e_eFSS_BLOBC_RES_OK                - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_BLOBC_CopyOriInBkpIfNotEquals(t_eFSS_BLOBC_Ctx* const p_ptCtx, const uint32_t p_uIdx);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_BLOBC_H */