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
}e_eFSS_COREHL_RES;

typedef struct
{
    t_eFSS_CORELL_Ctx tCORELLCtx;
}t_eFSS_COREHL_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the Low Level Core Module context
 *
 * @param[in]   p_ptCtx          - Low Level Log Core context
 * @param[in]   p_tCtxCb         - All callback collection context
 * @param[in]   p_tStorSet       - Storage settings
 * @param[in]   p_puBuff         - Pointer to a buffer used by the modules to make calc
 * @param[in]   p_uBuffL         - Size of p_puBuff
 *
 * @return      e_eFSS_COREHL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_COREHL_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_COREHL_RES_OK            - Operation ended correctly
 */
e_eFSS_COREHL_RES eFSS_COREHL_InitCtx(t_eFSS_COREHL_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx const p_tCtxCb,
									  t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Low Level Log Core context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_COREHL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_COREHL_RES_OK            - Operation ended correctly
 */
e_eFSS_COREHL_RES eFSS_COREHL_IsInit(t_eFSS_COREHL_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Get storage settings
 *
 * @param[in]   p_ptCtx       - Low Level Log Core context
 * @param[out]  p_ptStorSet   - Pointer to a storage settings
 *
 * @return      e_eFSS_COREHL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_COREHL_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_COREHL_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_COREHL_RES_OK            - Operation ended correctly
 */
e_eFSS_COREHL_RES eFSS_COREHL_GetStorSett(t_eFSS_COREHL_Ctx* const p_ptCtx, t_eFSS_TYPE_StorSet* p_ptStorSet);

/**
 * @brief       Get reference to one of the two internal buffer
 *
 * @param[in]   p_ptCtx       - Low Level Log Core context
 * @param[in]   p_eBuffType   - Enum used to select wich buffer we want to select
 * @param[out]  p_ppuBuff     - Pointer to a Pointer pointing to the p_eBuffType buffer
 * @param[out]  p_puBuffL     - Pointer to a uint32_t variable where the size of p_ppuBuff buffer will be placed
 * @param[out]  p_pptMetaB    - Pointer to a meta struct variable where the pointer to the page metasize will be placed
 *
 * @return      e_eFSS_COREHL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_COREHL_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *		        e_eFSS_COREHL_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_COREHL_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_COREHL_RES_OK            - Operation ended correctly
 */
e_eFSS_COREHL_RES eFSS_COREHL_GetBuff(t_eFSS_COREHL_Ctx* const p_ptCtx, e_eFSS_TYPE_BUFFTYPE p_eBuffType,
								      uint8_t** p_ppuBuff, uint32_t* p_puBuffL, t_eFSS_TYPE_PageMeta** p_pptMetaB);

/**
 * @brief       Load a page from the storage area in one of he two internal buffer
 *
 * @param[in]   p_ptCtx       - Low Level Log Core context
 * @param[in]   p_eBuffType   - Enum used to select wich buffer we want to select
 * @param[in]   p_uPageIndx   - uint32_t index rappresenting the page that we want to load from storage
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
e_eFSS_COREHL_RES eFSS_COREHL_LoadPageInBuff(t_eFSS_COREHL_Ctx* const p_ptCtx, e_eFSS_TYPE_BUFFTYPE p_eBuffType,
								             const uint32_t p_uPageIndx);

/**
 * @brief       Flush one of the two buffer in the storage are. Keep in mine that the other buffer well be used
 *              to check if the data was flushed corretly, and after this operation will contains different value.
 *
 * @param[in]   p_ptCtx       - Low Level Log Core context
 * @param[in]   p_eBuffType   - Enum used to select wich buffer we want to select
 * @param[in]   p_uPageIndx   - uint32_t index rappresenting the page that we want to flush in storage
 *
 * @return      e_eFSS_COREHL_RES_BADPOINTER       - In case of bad pointer passed to the function
 *		        e_eFSS_COREHL_RES_BADPARAM         - In case of an invalid parameter passed to the function
 *		        e_eFSS_COREHL_RES_CORRUPTCTX       - Context is corrupted
 *		        e_eFSS_COREHL_RES_NOINITLIB        - Need to init lib before calling function
 *		        e_eFSS_COREHL_RES_CLBCKREADERR     - The read callback reported an error
 *		        e_eFSS_COREHL_RES_CLBCKERASEERR    - The erase callback reported an error
 *		        e_eFSS_COREHL_RES_CLBCKWRITEERR    - The write callback reported an error
 *		        e_eFSS_COREHL_RES_WRITENOMATCHREAD - Writen data dosent match what requested
 *              e_eFSS_COREHL_RES_OK               - Operation ended correctly
 */
e_eFSS_COREHL_RES eFSS_COREHL_FlushBuffInPage(t_eFSS_COREHL_Ctx* const p_ptCtx, e_eFSS_TYPE_BUFFTYPE p_eBuffType,
								              const uint32_t p_uPageIndx);

/**
 * @brief       Calculate the Crc of the data present in the choosen buffer. Can also select to calculate the crc of
 *              a given numbers of bytes.
 *
 * @param[in]   p_ptCtx       - Low Level Log Core context
 * @param[in]   p_eBuffType   - Enum used to select wich buffer we want to select
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
e_eFSS_COREHL_RES eFSS_COREHL_CalcCrcInBuff(t_eFSS_COREHL_Ctx* const p_ptCtx, e_eFSS_TYPE_BUFFTYPE p_eBuffType,
								            uint32_t p_uCrcSeed, uint32_t p_uLenCalc, uint32_t* p_puCrc);

/**
 * @brief       Verify the validity of the page in p_uOrigIndx and p_uBackupIndx. If everithing goes well at the end
 *              we can also retrive the original page from the buffer 1.
 *              1 - If p_uOrigIndx and p_uBackupIndx are valid, verify if they are equals. If not copy p_uOrigIndx
 *                  in p_uBackupIndx
 *              2 - If p_uOrigIndx is not valid copy p_uBackupIndx in p_uOrigIndx
 *              3 - If p_uBackupIndx is not valid copy p_uOrigIndx in p_uBackupIndx
 *              4 - If p_uOrigIndx and p_uBackupIndx are not valid we cann not do nothing
 *
 * @param[in]   p_ptCtx       - Pointer to all callback context
 * @param[in]   p_uOrigIndx   - Page index of the original data
 * @param[in]   p_uBackupIndx - Page index of the backup data
 *
 * @return      e_eFSS_COREHL_RES_OK                - Operation ended successfully, page are correct
 *              e_eFSS_COREHL_RES_NOTVALIDPAGE      - both origin and backup pages are corrupted
 *              e_eFSS_COREHL_RES_OK_BKP_RCVRD      - operation ended successfully recovering a backup or an origin
 *                                                    page
 *              e_eFSS_COREHL_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_COREHL_RES_BADPARAM          - In case of bad parameter passed to the function
 *              e_eFSS_COREHL_RES_CLBCKCRCERR       - Error reported from the callback
 *              e_eFSS_COREHL_RES_CLBCKERASEERR     - Error reported from the callback
 *              e_eFSS_COREHL_RES_CLBCKWRITEERR     - Error reported from the callback
 *              e_eFSS_COREHL_RES_CLBCKREADERR      - Error reported from the callback
 *              e_eFSS_COREHL_RES_WRITENOMATCHREAD  - For some unknow reason data write dosent match data readed
 */
e_eFSS_COREHL_RES eFSS_COREHL_VerifyNRipristBkup(t_eFSS_COREHL_Ctx* const p_ptCtx, const uint32_t p_uOrigIndx, 
                                                 const uint32_t p_uBackupIndx, const uint32_t p_uOriSubType, 
                                                 const uint32_t p_uBckUpSubType );

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_COREHL_H */