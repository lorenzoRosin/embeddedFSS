/**
 * @file       eFSS_COREHL.h
 *
 * @brief      High level core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSS_COREHL_H
#define EFSS_COREHL_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_CORELL.h"



/***********************************************************************************************************************
 *      PUBLIC TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eFSS_COREHL_RES_OK = 0,
    e_eFSS_COREHL_RES_NOINITLIB,
    e_eFSS_COREHL_RES_BADPARAM,
    e_eFSS_COREHL_RES_BADPOINTER,
    e_eFSS_COREHL_RES_CORRUPTCTX,
    e_eFSS_COREHL_RES_CLBCKERASEERR,
    e_eFSS_COREHL_RES_CLBCKWRITEERR,
    e_eFSS_COREHL_RES_CLBCKREADERR,
    e_eFSS_COREHL_RES_CLBCKCRCERR,
    e_eFSS_COREHL_RES_NOTVALIDPAGE,
    e_eFSS_COREHL_RES_NEWVERSIONFOUND,
    e_eFSS_COREHL_RES_WRITENOMATCHREAD,
    e_eFSS_COREHL_RES_OK_BKP_RCVRD
}e_eFSS_COREHL_RES;

typedef struct
{
    uint8_t*  puBuf;
    uint32_t  uBufL;
}t_eFSS_COREHL_StorBuf;

typedef struct
{
    t_eFSS_CORELL_Ctx tCORELLCtx;
}t_eFSS_COREHL_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the High Level Core Module context
 *
 * @param[in]   p_ptCtx          - High Level Core context
 * @param[in]   p_tCtxCb         - All callback collection context
 * @param[in]   p_tStorSet       - Storage settings
 * @param[in]   p_uStorType      - Storage type
 * @param[in]   p_puBuff         - Pointer to a buffer used by the modules to make calc, must be pageSize * 2
 * @param[in]   p_uBuffL         - Size of p_puBuff
 *
 * @return      e_eFSS_COREHL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_COREHL_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_COREHL_RES_OK            - Operation ended correctly
 */
e_eFSS_COREHL_RES eFSS_COREHL_InitCtx(t_eFSS_COREHL_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx const p_tCtxCb,
									  const t_eFSS_TYPE_StorSet p_tStorSet, const uint8_t p_uStorType,
                                      uint8_t* const p_puBuff, const uint32_t p_uBuffL);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - High Level Core context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_COREHL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_COREHL_RES_OK            - Operation ended correctly
 */
e_eFSS_COREHL_RES eFSS_COREHL_IsInit(t_eFSS_COREHL_Ctx* const p_ptCtx, bool_t* const p_pbIsInit);

/**
 * @brief       Get storage settings
 *
 * @param[in]   p_ptCtx       - High Level Core context
 * @param[out]  p_ptStorSet   - Pointer to a storage settings that will be filled with data used during init
 *
 * @return      e_eFSS_COREHL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_COREHL_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_COREHL_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_COREHL_RES_OK            - Operation ended correctly
 */
e_eFSS_COREHL_RES eFSS_COREHL_GetStorSett(t_eFSS_COREHL_Ctx* const p_ptCtx, t_eFSS_TYPE_StorSet* const p_ptStorSet);

/**
 * @brief       Get the reference of buffer that we can use to read or write data from storage
 *
 * @param[in]   p_ptCtx       - High Level Core context
 * @param[out]  p_ptBuff      - Pointer to a struct that will be filled with info about buffer
 *
 * @return      e_eFSS_COREHL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_COREHL_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_COREHL_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_COREHL_RES_OK            - Operation ended correctly
 */
e_eFSS_COREHL_RES eFSS_COREHL_GetBuff(t_eFSS_COREHL_Ctx* const p_ptCtx, t_eFSS_COREHL_StorBuf* const p_ptBuff);

/**
 * @brief       Get storage settings and buffer all in one
 *
 * @param[in]   p_ptCtx       - High Level Core context
 * @param[out]  p_ptBuff      - Pointer to a storage collection struct that will be filled with info about internal buf
 * @param[out]  p_ptStorSet   - Pointer to a storage settings
 *
 * @return      e_eFSS_COREHL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_COREHL_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_COREHL_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_COREHL_RES_OK            - Operation ended correctly
 */
e_eFSS_COREHL_RES eFSS_COREHL_GetBuffNStor(t_eFSS_COREHL_Ctx* const p_ptCtx, t_eFSS_COREHL_StorBuf* const p_ptBuff,
                                           t_eFSS_TYPE_StorSet* const  p_ptStorSet);

/**
 * @brief       Load a page from the storage area in to the internal buffer
 *
 * @param[in]   p_ptCtx         - High Level Core context
 * @param[in]   p_uPageIndx     - uint32_t index rappresenting the page that we want to load from storage
 * @param[out]  p_puSubTypeRead - pointer to a uint8_t where we will store the subtype readed from the loaded page
 *
 * @return      e_eFSS_COREHL_RES_BADPOINTER      - In case of bad pointer passed to the function
 *		        e_eFSS_COREHL_RES_BADPARAM        - In case of an invalid parameter passed to the function
 *		        e_eFSS_COREHL_RES_CORRUPTCTX      - Context is corrupted
 *		        e_eFSS_COREHL_RES_NOINITLIB       - Need to init lib before calling function
 *		        e_eFSS_COREHL_RES_CLBCKREADERR    - The read callback reported an error
 *              e_eFSS_COREHL_RES_CLBCKCRCERR     - The crc callback reported an error
 *              e_eFSS_COREHL_RES_NOTVALIDPAGE    - The readed page is invalid
 *              e_eFSS_COREHL_RES_NEWVERSIONFOUND - The readed page has a new version
 *              e_eFSS_COREHL_RES_OK              - Operation ended correctly
 */
e_eFSS_COREHL_RES eFSS_COREHL_LoadPageInBuff(t_eFSS_COREHL_Ctx* const p_ptCtx, const uint32_t p_uPageIndx,
                                             uint8_t* const p_puSubTypeRead);

/**
 * @brief       Flush the internal buffer in to the storage area.
 *
 * @param[in]   p_ptCtx           - High Level Core context
 * @param[in]   p_uPageIndx       - uint32_t index rappresenting the page that we want to flush in storage
 * @param[in]   p_uSubTypeToWrite - uint8_t value that will the written in the page as subtype
 *
 * @return      e_eFSS_COREHL_RES_BADPOINTER       - In case of bad pointer passed to the function
 *		        e_eFSS_COREHL_RES_BADPARAM         - In case of an invalid parameter passed to the function
 *		        e_eFSS_COREHL_RES_CORRUPTCTX       - Context is corrupted
 *		        e_eFSS_COREHL_RES_NOINITLIB        - Need to init lib before calling function
 *              e_eFSS_COREHL_RES_CLBCKCRCERR      - The crc callback reported an error
 *		        e_eFSS_COREHL_RES_CLBCKERASEERR    - The erase callback reported an error
 *		        e_eFSS_COREHL_RES_CLBCKWRITEERR    - The write callback reported an error
 *		        e_eFSS_COREHL_RES_CLBCKREADERR     - The read callback reported an error
 *		        e_eFSS_COREHL_RES_WRITENOMATCHREAD - Writen data dosent match what requested
 *              e_eFSS_COREHL_RES_OK               - Operation ended correctly
 */
e_eFSS_COREHL_RES eFSS_COREHL_FlushBuffInPage(t_eFSS_COREHL_Ctx* const p_ptCtx, const uint32_t p_uPageIndx,
                                              uint8_t const p_uSubTypeToWrite);

/**
 * @brief       Calculate the Crc of the data present in the internal buffer. Can also select to calculate the crc of
 *              a given numbers of bytes. The subtype of the page is not included in to the calculation
 *
 * @param[in]   p_ptCtx       - High Level Core context
 * @param[in]   p_uCrcSeed    - uint32_t rappresenting the seed we want to use in the calc
 * @param[in]   p_uLenCalc    - uint32_t rappresenting the lenght we want to calc
 * @param[out]  p_puCrc       - Pointer to a uint32_t variable where the CRC calculated will be placed
 *
 * @return      e_eFSS_COREHL_RES_BADPOINTER       - In case of bad pointer passed to the function
 *		        e_eFSS_COREHL_RES_BADPARAM         - In case of an invalid parameter passed to the function
 *		        e_eFSS_COREHL_RES_CORRUPTCTX       - Context is corrupted
 *		        e_eFSS_COREHL_RES_NOINITLIB        - Need to init lib before calling function
 *		        e_eFSS_COREHL_RES_CLBCKCRCERR      - The CRC callback reported an error
 *              e_eFSS_COREHL_RES_OK               - Operation ended correctly
 */
e_eFSS_COREHL_RES eFSS_COREHL_CalcCrcInBuff(t_eFSS_COREHL_Ctx* const p_ptCtx, const uint32_t p_uCrcSeed,
                                            const uint32_t p_uLenCalc, uint32_t* const p_puCrc);

/**
 * @brief       Flush the internal buffer in to the storage area and generate a backup copy.
 *
 * @param[in]   p_ptCtx         - High Level Core context
 * @param[in]   p_uOrigIndx     - Page index of the original data
 * @param[in]   p_uBackupIndx   - Page index of the backup data
 * @param[in]   p_uOriSubType   - Sub type of the original page
 * @param[in]   p_uBckUpSubType - Sub type of the backup pages
 *
 * @return      e_eFSS_COREHL_RES_BADPOINTER       - In case of bad pointer passed to the function
 *		        e_eFSS_COREHL_RES_BADPARAM         - In case of an invalid parameter passed to the function
 *		        e_eFSS_COREHL_RES_CORRUPTCTX       - Context is corrupted
 *		        e_eFSS_COREHL_RES_NOINITLIB        - Need to init lib before calling function
 *		        e_eFSS_COREHL_RES_CLBCKREADERR     - The read callback reported an error
 *		        e_eFSS_COREHL_RES_CLBCKERASEERR    - The erase callback reported an error
 *		        e_eFSS_COREHL_RES_CLBCKWRITEERR    - The write callback reported an error
 *		        e_eFSS_COREHL_RES_CLBCKREADERR     - The read callback reported an error
 *		        e_eFSS_COREHL_RES_WRITENOMATCHREAD - Writen data dosent match what requested
 *              e_eFSS_COREHL_RES_OK               - Operation ended correctly
 */
e_eFSS_COREHL_RES eFSS_COREHL_FlushBuffInPageNBkp(t_eFSS_COREHL_Ctx* const p_ptCtx,
								                  const uint32_t p_uOrigIndx, const uint32_t p_uBackupIndx,
                                                  const uint8_t p_uOriSubType, const uint8_t p_uBckUpSubType);

/**
 * @brief       Verify the validity of the page present in p_uOrigIndx and of it's backup present in p_uBackupIndx.
 *              If everithing goes well the original page is loaded in the internal buffer, and any error is fixed.
 *              This function use this decision maps in order to load original page and verify it's backup:
 *              1 - If p_uOrigIndx and p_uBackupIndx are valid, verify if they are equals. If not copy p_uOrigIndx
 *                  in p_uBackupIndx
 *              2 - If p_uOrigIndx is not valid copy p_uBackupIndx in p_uOrigIndx
 *              3 - If p_uBackupIndx is not valid copy p_uOrigIndx in p_uBackupIndx
 *              4 - If p_uOrigIndx and p_uBackupIndx are not valid we cann not do nothing
 *              Note: page validity is checkd against subtype also.
 *
 * @param[in]   p_ptCtx         - Pointer to all callback context
 * @param[in]   p_uOrigIndx     - Page index of the original data
 * @param[in]   p_uBackupIndx   - Page index of the backup data
 * @param[in]   p_uOriSubType   - Sub type of the original page
 * @param[in]   p_uBckUpSubType - Sub type of the backup pages
 *
 * @return      e_eFSS_COREHL_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_COREHL_RES_BADPARAM          - In case of bad parameter passed to the function
 *		        e_eFSS_COREHL_RES_CORRUPTCTX        - Context is corrupted
 *		        e_eFSS_COREHL_RES_NOINITLIB         - Need to init lib before calling function
 *		        e_eFSS_COREHL_RES_CLBCKREADERR      - The read callback reported an error
 *              e_eFSS_COREHL_RES_CLBCKCRCERR       - The crc callback reported an error
 *              e_eFSS_COREHL_RES_NOTVALIDPAGE      - both origin and backup pages are corrupted
 *              e_eFSS_COREHL_RES_CLBCKERASEERR     - Error reported from the callback
 *              e_eFSS_COREHL_RES_CLBCKWRITEERR     - Error reported from the callback
 *              e_eFSS_COREHL_RES_WRITENOMATCHREAD  - For some unknow reason data write dosent match data readed
 *              e_eFSS_COREHL_RES_OK_BKP_RCVRD      - operation ended successfully recovering a backup or an origin
 *                                                    page
 *              e_eFSS_COREHL_RES_OK                - Operation ended successfully, page are correct
 */
e_eFSS_COREHL_RES eFSS_COREHL_LoadPageInBuffNRipBkp(t_eFSS_COREHL_Ctx* const p_ptCtx,
                                                    const uint32_t p_uOrigIndx, const uint32_t p_uBackupIndx,
                                                    const uint8_t p_uOriSubType, const uint8_t p_uBckUpSubType);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_COREHL_H */