/**
 * @file       eFSS_UTILSHLPRV.h
 *
 * @brief      High level utils for fail safe storage
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSS_UTILSHLPRV_H
#define EFSS_UTILSHLPRV_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_TYPE.h"



/***********************************************************************************************************************
 *      PRIVATE TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eFSS_UTILSHLPRV_RES_OK = 0,
    e_eFSS_UTILSHLPRV_RES_BADPARAM,
    e_eFSS_UTILSHLPRV_RES_BADPOINTER,
    e_eFSS_UTILSHLPRV_RES_CLBCKERASEERR,
    e_eFSS_UTILSHLPRV_RES_CLBCKWRITEERR,
    e_eFSS_UTILSHLPRV_RES_CLBCKREADERR,
    e_eFSS_UTILSHLPRV_RES_CLBCKCRCERR,
    e_eFSS_UTILSHLPRV_RES_WRITENOMATCHREAD,
    e_eFSS_UTILSHLPRV_RES_CORRUPT
}e_eFSS_UTILSHLPRV_RES;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Get metadata from a page already loaded in a buffer
 *
 * @param[in]   p_puPageBuf   - Pointer to a buffer page
 * @param[in]   p_uPageL      - Size of the p_puPageBuf buffer
 * @param[out]  p_ptPagePrm   - Pointer to the page metadata that need to be filled
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSHLPRV_RES_BADPARAM          - In case of bad parameter passed to the function
 */
e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_GetMetaFromBuff(const uint8_t* p_puPageBuf, const uint32_t p_uPageL,
                                                      t_eFSS_TYPE_PageMeta* const p_ptPagePrm);

/**
 * @brief       Set the metadata of a page inside a buffer
 *
 * @param[in]   p_puPageBuf   - Pointer to a buffer page
 * @param[in]   p_uPageL      - Size of the p_puPageBuf buffer
 * @param[in]   p_ptPagePrm   - Pointer to the page metadata that need to be inserted
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSHLPRV_RES_BADPARAM          - In case of bad parameter passed to the function
 */
e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_SetMetaInBuff(uint8_t* const p_puPageBuf, const uint32_t p_uPageL,
                                                    t_eFSS_TYPE_PageMeta* const p_ptPagePrm);

/**
 * @brief       Set the crc field of metadata inside a passed buffer
 *
 * @param[in]   p_puPageBuf   - Pointer to a buffer page
 * @param[in]   p_uPageL      - Size of the p_puPageBuf buffer
 * @param[in]   p_uCrcToSet   - Crc to set in the page metadata
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSHLPRV_RES_BADPARAM          - In case of bad parameter passed to the function
 */
e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_SetCrcMetaInBuff(uint8_t* const p_puPageBuf, const uint32_t p_uPageL,
                                                       const uint32_t p_uCrcToSet);

/**
 * @brief       Calculate the CRC value that must be saved toghether with the page metadata from a buffer of the page
 *
 * @param[in]   p_ptCbCtx     - Pointer to all callback context
 * @param[in]   p_puPageBuf   - Pointer to a buffer page
 * @param[in]   p_uPageL      - Size of the p_puPageBuf buffer
 * @param[out]  p_puCrcCalc   - Pointer where the calculated CRC will be placed
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSHLPRV_RES_BADPARAM          - In case of bad parameter passed to the function
 *              e_eFSS_UTILSHLPRV_RES_CLBCKCRCERR       - Error reported from the callback
 */
e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_CalcMetaCrcInBuff(t_eFSS_TYPE_CbCtx* const p_ptCbCtx, uint8_t* const p_puPageBuf,
                                                        const uint32_t p_uPageL, uint32_t* const p_puCrcCalc);



























/**
 * @brief       Erase a specified page
 *
 * @param[in]   p_ptCbCtx       - Erase function context
 * @param[in]   p_uPageIndx   - Page index we want to erase
 * @param[in]   p_uReTry      - Erase function pointer
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSLLPRV_RES_CLBCKREPORTERROR  - Error reported from the callback
 */
e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_SetPageMetaInBuffAndUpdtCrc(uint8_t* const pageBuff, const uint32_t p_pageL,
                                                      t_eFSS_TYPE_PageMeta* const pagePrm,
                                                            const uint32_t p_pageL, uint32_t* const p_puCrcCalc);

/**
 * @brief       Erase a specified page
 *
 * @param[in]   p_ptCbCtx       - Erase function context
 * @param[in]   p_uPageIndx   - Page index we want to erase
 * @param[in]   p_uReTry      - Erase function pointer
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSLLPRV_RES_CLBCKREPORTERROR  - Error reported from the callback
 */
e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_IsValidPageInBuff(t_eFSS_TYPE_CbCtx* const p_ptCbCtx, uint8_t* const pageBuff,
                                                        const uint32_t p_pageL );

































/**
 * @brief       Erase a specified page
 *
 * @param[in]   p_ptCbCtx       - Erase function context
 * @param[in]   p_uPageIndx   - Page index we want to erase
 * @param[in]   p_uReTry      - Erase function pointer
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSLLPRV_RES_CLBCKREPORTERROR  - Error reported from the callback
 */
e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_IsValidPage(t_eFSS_TYPE_CbCtx* const p_ptCbCtx, const uint32_t p_uPageIdx,
                                                  uint8_t* const pageBuff, const uint32_t p_pageL, const uint32_t p_uReTry );

/**
 * Erase and write a page with the param passed, and automatically update the CRC of the page it self
 * @param pginfo Information about memory and pages
 * @param cbHld Struct containing all callback reference
 * @param pageBuff Pointer to a buffer containing the data that we want to write in the page
 * @param suppBuff Support buffer to do the calculation, must be atleast a page size
 * @param pageIndex Index of the page to write
 * @param prmPage Param that we want to write in the page
 * @return EFSS_RES_BADPOINTER in case of bad pointer
 *         EFSS_RES_BADPARAM in case of a wrong param passed
 *         EFSS_RES_NOTVALIDPAGE the page is not valid, crc or somethings else dosent return
 *         EFSS_RES_ERRORWRITE error reported from write callback
 *         EFSS_RES_ERRORERASE error reported from erase callback
 *         EFSS_RES_OK operation ended successfully
 */
e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_WritePagePrmNUpdateCrc(t_eFSS_TYPE_CbCtx* const p_ptCbCtx, const uint32_t p_uPageIdx, uint8_t* const pageBuff,
                                   uint8_t* const p_pageL, const s_prv_pagePrm* prmPage, uint8_t* const pageBuffS, uint8_t* const p_pageLS, const uint32_t p_uReTry);

/**
 * Read a page and load it in the buffer, and copy data param in pagePrm also
 * @param pginfo Information about memory and pages
 * @param cbHld Struct containing all callback reference
 * @param pageBuff Pointer to a buffer that will be filled with data from the page
 * @param pageIndex Index of the page to read
 * @param prmPage Param that we fill with the information readed in the page
 * @return EFSS_RES_BADPOINTER in case of bad pointer
 *         EFSS_RES_BADPARAM in case of a wrong param passed
 *         EFSS_RES_NOTVALIDPAGE the page is not valid, crc or somethings else dosent return
 *         EFSS_RES_ERRORWRITE error reported from write callback
 *         EFSS_RES_ERRORERASE error reported from erase callback
 *         EFSS_RES_OK operation ended successfully
 */
e_eFSS_Res readPageNPrm(const t_eFSS_TYPE_PageMeta pginfo, const s_eFSS_Cb cbHld, uint8_t* const pageBuff,
                        const uint32_t pageIndx, s_prv_pagePrm* const pagePrm);

/**
 * Verify the validity of the page in origIndx and backupIndx.
 * 1 - If origIndx and backupIndx are valid, verify if they are equals. If not copy origIndx in backupIndx
 * 2 - If origIndx is not valid copy backupIndx in origIndx
 * 3 - If backupIndx is not valid copy origIndx in backupIndx
 * 4 - If origIndx and backupIndx are not valid we cann not do nothing
 * @param pginfo Information about memory and pages
 * @param cbHld Struct containing all callback reference
 * @param pageOrig Support buffer to do the calculation, must be atleast a page size
 * @param pageBkup Support buffer to do the calculation, must be atleast a page size
 * @param origIndx Index of the original page
 * @param backupIndx Index of the backup page
 * @return EFSS_RES_BADPOINTER in case of bad pointer
 *         EFSS_RES_BADPARAM in case of a wrong param passed
 *         EFSS_RES_NOTVALIDPAGE both origin and backup pages are corrupted
 *         EFSS_RES_ERRORWRITE error reported from write callback
 *         EFSS_RES_ERRORERASE error reported from erase callback
 *         EFSS_RES_ERRORREAD error reported from read callback
 *         EFSS_RES_OK_BKP_RCVRD operation ended successfully recovering a backup or an origin page
 *         EFSS_RES_OK operation ended successfully
 */
e_eFSS_Res verifyAndRipristinateBkup(const t_eFSS_TYPE_PageMeta pginfo, const s_eFSS_Cb cbHld, uint8_t* const pageOrig,
                                     uint8_t* const pageBkup, const uint32_t origIndx, const uint32_t backupIndx);

/**
 * Clone a page from origIndx to destIndx
 * @param pginfo Information about memory and pages
 * @param cbHld Struct containing all callback reference
 * @param pageBuff Support buffer to do the calculation, must be atleast a page size
 * @param suppBuff Support buffer to do the calculation, must be atleast a page size
 * @param origIndx Index of the original page
 * @param destIndx Index of the destination page
 * @return EFSS_RES_BADPOINTER in case of bad pointer
 *         EFSS_RES_BADPARAM in case of a wrong param passed
 *         EFSS_RES_ERRORWRITE error reported from write callback
 *         EFSS_RES_ERRORERASE error reported from erase callback
 *         EFSS_RES_ERRORREAD error reported from read callback
 *         EFSS_RES_OK operation ended successfully
 */
e_eFSS_Res cloneAPage(const t_eFSS_TYPE_PageMeta pginfo, const s_eFSS_Cb cbHld, uint8_t* const pageBuff,
                      uint8_t* const suppBuff, const uint32_t origIndx, const uint32_t destIndx);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_UTILSHLPRV_H */
