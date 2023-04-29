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
    e_eFSS_BLOBC_RES_NEWVERSIONFOUND,
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
 * @param[in]   p_ptCtx          - Blob Core context
 * @param[in]   p_tCtxCb         - All callback collection context
 * @param[in]   p_tStorSet       - Storage settings
 * @param[in]   p_puBuff         - Pointer to a buffer used by the modules to make calc, must be pageSize * 2
 * @param[in]   p_uBuffL         - Size of p_puBuff
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
e_eFSS_BLOBC_RES eFSS_BLOBC_IsInit(const t_eFSS_BLOBC_Ctx* p_ptCtx, bool_t* const p_pbIsInit);

/**
 * @brief       Get the numbers of usable page for writing or reading the blob and the buffer that we can use to read
 *              and write data. The size of the buffer refers only to the user avaiable data, others private metadata
 *              are not avaiable in this stage.
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
 * @brief       Load a page from the storage area in to the internal buffer. We can choose if load the buffer from
 *              the original pages area or from the backup ones.
 *
 * @param[in]   p_ptCtx       - Blob Core context
 * @param[in]   p_bInOrigin   - Choose if read page from origin or backup area
 * @param[in]   p_uIdx        - Index of the page we want to read from. Must be a a value that can vary from 0 to
 *                              p_puUsePages-1 (returned from eFSS_BLOBC_GetBuffNUsable).
 * @param[out]  p_puSeqN      - An uint32_t value that will be filled with the sequence number from the page
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER      - In case of bad pointer passed to the function
 *		        e_eFSS_BLOBC_RES_BADPARAM        - In case of an invalid parameter passed to the function
 *		        e_eFSS_BLOBC_RES_CORRUPTCTX      - Context is corrupted
 *		        e_eFSS_BLOBC_RES_NOINITLIB       - Need to init lib before calling function
 *		        e_eFSS_BLOBC_RES_CLBCKREADERR    - The read callback reported an error
 *              e_eFSS_BLOBC_RES_CLBCKCRCERR     - The crc callback reported an error
 *              e_eFSS_BLOBC_RES_NOTVALIDBLOB    - The readed page is invalid
 *              e_eFSS_BLOBC_RES_NEWVERSIONFOUND - The readed page has a new version
 *              e_eFSS_BLOBC_RES_OK              - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_BLOBC_LoadPageInBuff(t_eFSS_BLOBC_Ctx* const p_ptCtx, const bool_t p_bInOrigin,
                                           const uint32_t p_uIdx, uint32_t* const p_puSeqN);

/**
 * @brief       Flush the buffer in a page at p_uIdx position, and with p_uSeqN as sequence number. We can write in
 *              to the original area or in to the backup one. After this operation the used buffer will contain
 *              the same values as before, except for the private metadata
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
 * @brief       Calculate the Crc of the data present in the buffer. It's not necessary to calculate the CRC
 *              value of the whole buffer, we can choose to calculate the CRC of a portion of the buffer. In this
 *              calculation private metadata and sequence number is not included. And so the max Length is the
 *              same reported by the eFSS_BLOBC_GetBuffNUsable function
 *
 * @param[in]   p_ptCtx     - Blob Core context
 * @param[in]   p_uSeed     - Seed to use during the calculation
 * @param[in]   p_uCrcL     - Length that we want to use for the calculation
 * @param[out]  p_puCrc     - An uint32_t value that will be filled with the calculated CRC
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER        - In case of bad pointer passed to the function
 *		        e_eFSS_BLOBC_RES_BADPARAM          - In case of an invalid parameter passed to the function
 *		        e_eFSS_BLOBC_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_BLOBC_RES_NOINITLIB         - Need to init lib before calling function
 *              e_eFSS_BLOBC_RES_CLBCKCRCERR       - The crc callback reported an error
 *              e_eFSS_BLOBC_RES_OK                - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_BLOBC_CalcCrcInBuff(t_eFSS_BLOBC_Ctx* const p_ptCtx, const uint32_t p_uSeed,
                                          const uint32_t p_uCrcL, uint32_t* const p_puCrc);

/**
 * @brief       If the original page is not equals to the backup pages, copy the original page in to the backup area.
 *              If they are equals do nothing. In this case the comparsion comprend the sequential number.
 *              We will do this operation for every page of the original area
 *
 * @param[in]   p_ptCtx      - Blob Core context
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
 *              e_eFSS_BLOBC_RES_NOTVALIDBLOB      - The readed page is invalid
 *              e_eFSS_BLOBC_RES_NEWVERSIONFOUND    - The readed page has a new version
 *              e_eFSS_BLOBC_RES_OK                - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_BLOBC_CloneOriAreaInBkpIfNotEq(t_eFSS_BLOBC_Ctx* const p_ptCtx);

/**
 * @brief       Clone an area of the storage in the other one. So we can clone the original area in to the backup one
 *              or viceversa
 *
 * @param[in]   p_ptCtx      - Blob Core context
 * @param[in]   p_bStartOri  - bool_t parameter, if setted to true we will clone the original area in to the backup one
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
 *              e_eFSS_BLOBC_RES_NOTVALIDBLOB      - The readed page is invalid
 *              e_eFSS_BLOBC_RES_NEWVERSIONFOUND    - The readed page has a new version
 *              e_eFSS_BLOBC_RES_OK                - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_BLOBC_CloneArea(t_eFSS_BLOBC_Ctx* const p_ptCtx, const bool_t p_bStartOri);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_BLOBC_H */