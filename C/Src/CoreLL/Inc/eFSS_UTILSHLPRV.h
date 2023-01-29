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
    e_eFSS_UTILSHLPRV_RES_NOTVALIDPAGE,
    e_eFSS_UTILSHLPRV_RES_OK_BKP_RCVRD,
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
 * @brief       Set the metadata of a page inside a buffer and update the CRC of the metadata
 *
 * @param[in]   p_puPageBuf   - Pointer to a buffer page
 * @param[in]   p_uPageL      - Size of the p_puPageBuf buffer
 * @param[in]   p_ptPagePrm   - Pointer to the page metadata that need to be inserted and CRC updated
 * @param[in]   p_ptCbCtx     - Pointer to all callback context
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSHLPRV_RES_BADPARAM          - In case of bad parameter passed to the function
 *              e_eFSS_UTILSHLPRV_RES_CLBCKCRCERR       - Error reported from the callback
 */
e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_SetMetaInBuffNUpdCrc(uint8_t* const p_puPageBuf, const uint32_t p_uPageL,
                                                           t_eFSS_TYPE_PageMeta* const p_ptPagePrm,
                                                           t_eFSS_TYPE_CbCtx* const p_ptCbCtx);

/**
 * @brief       Is the page present in the passed buffer valid? ( Crc check and magic number )
 *
 * @param[in]   p_ptCbCtx     - Pointer to all callback context
 * @param[in]   p_puPageBuf   - Pointer to a buffer page
 * @param[in]   p_uPageL      - Size of the p_puPageBuf buffer
 * @param[in]   p_pbIsValid   - Pointer to a bool_t where the result will be placed
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSHLPRV_RES_BADPARAM          - In case of bad parameter passed to the function
 *              e_eFSS_UTILSHLPRV_RES_CLBCKCRCERR       - Error reported from the callback
 */
e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_IsValidPageInBuff(t_eFSS_TYPE_CbCtx* const p_ptCbCtx, uint8_t* const p_puPageBuf,
                                                        const uint32_t p_uPageL, bool_t* const p_pbIsValid);

/**
 * @brief       Is the page present in the passed buffer valid? ( Crc check and magic number )
 *
 * @param[in]   p_ptCbCtx     - Pointer to all callback context
 * @param[in]   p_uPageIndx   - Page index we want to validate
 * @param[in]   p_puPageBuf   - Pointer to a buffer used to read the indicated page
 * @param[in]   p_uPageL      - Size of the p_puPageBuf buffer
 * @param[in]   p_uReTry      - How many times we can retry if some error happens
 * @param[in]   p_pbIsValid   - Pointer to a bool_t where the result will be placed
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSHLPRV_RES_BADPARAM          - In case of bad parameter passed to the function
 *              e_eFSS_UTILSHLPRV_RES_CLBCKCRCERR       - Error reported from the callback
 */
e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_IsValidPage(t_eFSS_TYPE_CbCtx* const p_ptCbCtx, const uint32_t p_uPageIndx,
                                                  uint8_t* const p_puPageBuf, const uint32_t p_uPageL,
                                                  const uint32_t p_uReTry, bool_t* const p_pbIsValid);

/**
 * @brief       Erase, write and verify a page with the passed params, and automatically update the CRC of the page
 *
 * @param[in]   p_ptCbCtx     - Pointer to all callback context
 * @param[in]   p_uPageIndx   - Page index we want to write
 * @param[in]   p_puDataW     - Pointer to the data we want to write
 * @param[in]   p_uDataWLen   - size of the p_puDataW buffer
 * @param[in]   p_puDataR     - Pointer to support buffer used to verify the data just written
 * @param[in]   p_uDataRLen   - size of the p_puDataR buffer
 * @param[in]   p_ptPagePrm   - Pointer to the page metadata that need to be inserted
 * @param[in]   p_uReTry      - How many times we can retry if some error happens
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSHLPRV_RES_BADPARAM          - In case of bad parameter passed to the function
 *              e_eFSS_UTILSHLPRV_RES_CLBCKCRCERR       - Error reported from the callback
 *              e_eFSS_UTILSHLPRV_RES_CLBCKERASEERR     - Error reported from the callback
 *              e_eFSS_UTILSHLPRV_RES_CLBCKWRITEERR     - Error reported from the callback
 *              e_eFSS_UTILSHLPRV_RES_CLBCKREADERR      - Error reported from the callback
 *              e_eFSS_UTILSHLPRV_RES_WRITENOMATCHREAD  - For some unknow reason data write dosent match data readed
 */
e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_WritePagePrmNUpCrc(t_eFSS_TYPE_CbCtx* const p_ptCbCtx, const uint32_t p_uPageIndx,
                                                         uint8_t* const p_puDataW, const uint32_t p_uDataWLen,
                                                         uint8_t* const p_puDataR, const uint32_t p_uDataRLen,
                                                         t_eFSS_TYPE_PageMeta* const p_ptPagePrm,
                                                         const uint32_t p_uReTry);

/**
 * @brief       Erase, write and verify a page and automatically update the CRC of the page
 *
 * @param[in]   p_ptCbCtx     - Pointer to all callback context
 * @param[in]   p_uPageIndx   - Page index we want to write
 * @param[in]   p_puDataW     - Pointer to the data we want to write
 * @param[in]   p_uDataWLen   - size of the p_puDataW buffer
 * @param[in]   p_puDataR     - Pointer to support buffer used to verify the data just written
 * @param[in]   p_uDataRLen   - size of the p_puDataR buffer
 * @param[in]   p_uReTry      - How many times we can retry if some error happens
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSHLPRV_RES_BADPARAM          - In case of bad parameter passed to the function
 *              e_eFSS_UTILSHLPRV_RES_CLBCKCRCERR       - Error reported from the callback
 *              e_eFSS_UTILSHLPRV_RES_CLBCKERASEERR     - Error reported from the callback
 *              e_eFSS_UTILSHLPRV_RES_CLBCKWRITEERR     - Error reported from the callback
 *              e_eFSS_UTILSHLPRV_RES_CLBCKREADERR      - Error reported from the callback
 *              e_eFSS_UTILSHLPRV_RES_WRITENOMATCHREAD  - For some unknow reason data write dosent match data readed
 */
e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_WritePageNUpCrc(t_eFSS_TYPE_CbCtx* const p_ptCbCtx, const uint32_t p_uPageIndx,
                                                         uint8_t* const p_puDataW, const uint32_t p_uDataWLen,
                                                         uint8_t* const p_puDataR, const uint32_t p_uDataRLen,
                                                         const uint32_t p_uReTry);

/**
 * @brief       Read a page and load it in the passed buffer, and copy data param in pagePrm also
 *
 * @param[in]   p_ptCbCtx     - Pointer to all callback context
 * @param[in]   p_uPageIndx   - Page index we want to read
 * @param[in]   p_puDataR     - Pointer to the buffer where we want to copy data
 * @param[in]   p_uDataRLen   - size of the p_puDataR buffer
 * @param[in]   p_ptPagePrm   - Pointer to the page metadata that need to be filled
 * @param[in]   p_uReTry      - How many times we can retry if some error happens
 *
 * @return      e_eFSS_UTILSHLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSHLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSHLPRV_RES_BADPARAM          - In case of bad parameter passed to the function
 *              e_eFSS_UTILSHLPRV_RES_CLBCKREADERR      - Error reported from the callback
 */
e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_ReadPageNPrm(t_eFSS_TYPE_CbCtx* const p_ptCbCtx, const uint32_t p_uPageIndx,
                                                   uint8_t* const p_puDataR, const uint32_t p_uDataRLen,
                                                   t_eFSS_TYPE_PageMeta* const p_ptPagePrm, const uint32_t p_uReTry);

/**
 * @brief       Clone a page from p_uOrigIndx to p_uDestIndx
 *
 * @param[in]   p_ptCbCtx     - Pointer to all callback context
 * @param[in]   p_uReTry      - How many times we can retry if some error happens
 * @param[in]   p_puDataW     - Buffer used for write operation
 * @param[in]   p_uDataWLen   - size of the p_puDataW buffer
 * @param[in]   p_puDataR     - Buffer used for read operation
 * @param[in]   p_uDataRLen   - size of the p_puDataR buffer
 * @param[in]   p_uOrigIndx   - Page index we want to clone
 * @param[in]   p_uDestIndx   - Destination indexof the cloned page
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSHLPRV_RES_BADPARAM          - In case of bad parameter passed to the function
 *              e_eFSS_UTILSHLPRV_RES_CLBCKERASEERR     - Error reported from the callback
 *              e_eFSS_UTILSHLPRV_RES_CLBCKWRITEERR     - Error reported from the callback
 *              e_eFSS_UTILSHLPRV_RES_CLBCKREADERR      - Error reported from the callback
 *              e_eFSS_UTILSHLPRV_RES_WRITENOMATCHREAD  - For some unknow reason data write dosent match data readed
 */
e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_ClonePage( t_eFSS_TYPE_CbCtx* const p_ptCbCtx, const uint32_t p_uReTry,
                                                 uint8_t* const p_puDataW, const uint32_t p_uDataWLen,
                                                 uint8_t* const p_puDataR, const uint32_t p_uDataRLen,
                                                 const uint32_t p_uOrigIndx, const uint32_t p_uDestIndx );
/**
 * @brief       Clone a page from p_uOrigIndx to p_uDestIndx but use a new subtype
 *
 * @param[in]   p_ptCbCtx     - Pointer to all callback context
 * @param[in]   p_uReTry      - How many times we can retry if some error happens
 * @param[in]   p_puDataW     - Buffer used for write operation
 * @param[in]   p_uDataWLen   - size of the p_puDataW buffer
 * @param[in]   p_puDataR     - Buffer used for read operation
 * @param[in]   p_uDataRLen   - size of the p_puDataR buffer
 * @param[in]   p_uOrigIndx   - Page index we want to clone
 * @param[in]   p_uDestIndx   - Destination indexof the cloned page
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSHLPRV_RES_BADPARAM          - In case of bad parameter passed to the function
 *              e_eFSS_UTILSHLPRV_RES_CLBCKERASEERR     - Error reported from the callback
 *              e_eFSS_UTILSHLPRV_RES_CLBCKWRITEERR     - Error reported from the callback
 *              e_eFSS_UTILSHLPRV_RES_CLBCKREADERR      - Error reported from the callback
 *              e_eFSS_UTILSHLPRV_RES_WRITENOMATCHREAD  - For some unknow reason data write dosent match data readed
 */
e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_ClonePageNSbT( t_eFSS_TYPE_CbCtx* const p_ptCbCtx, const uint32_t p_uReTry,
                                                     uint8_t* const p_puDataW, const uint32_t p_uDataWLen,
                                                     uint8_t* const p_puDataR, const uint32_t p_uDataRLen,
                                                     const uint32_t p_uOrigIndx, const uint32_t p_uDestIndx,
                                                     const uint8_t p_uNewSubType );

/**
 * @brief       Verify the validity of the page in p_uOrigIndx and p_uBackupIndx.
 *              1 - If p_uOrigIndx and p_uBackupIndx are valid, verify if they are equals. If not copy p_uOrigIndx
 *                  in p_uBackupIndx
 *              2 - If p_uOrigIndx is not valid copy p_uBackupIndx in p_uOrigIndx
 *              3 - If p_uBackupIndx is not valid copy p_uOrigIndx in p_uBackupIndx
 *              4 - If p_uOrigIndx and p_uBackupIndx are not valid we cann not do nothing
 *
 * @param[in]   p_ptCbCtx     - Pointer to all callback context
 * @param[in]   p_uReTry      - How many times we can retry if some error happens
 * @param[in]   p_puDataW     - Buffer used for write operation
 * @param[in]   p_uDataWLen   - size of the p_puDataW buffer
 * @param[in]   p_puDataR     - Buffer used for read operation
 * @param[in]   p_uDataRLen   - size of the p_puDataR buffer
 * @param[in]   p_uOrigIndx   - Page index of the original data
 * @param[in]   p_uBackupIndx - Page index of the backup data
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully, page are correct
 *              e_eFSS_UTILSLLPRV_RES_NOTVALIDPAGE      - both origin and backup pages are corrupted
 *              e_eFSS_UTILSLLPRV_RES_OK_BKP_RCVRD      - operation ended successfully recovering a backup or an origin
 *                                                        page
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSHLPRV_RES_BADPARAM          - In case of bad parameter passed to the function
 *              e_eFSS_UTILSHLPRV_RES_CLBCKCRCERR       - Error reported from the callback
 *              e_eFSS_UTILSHLPRV_RES_CLBCKERASEERR     - Error reported from the callback
 *              e_eFSS_UTILSHLPRV_RES_CLBCKWRITEERR     - Error reported from the callback
 *              e_eFSS_UTILSHLPRV_RES_CLBCKREADERR      - Error reported from the callback
 *              e_eFSS_UTILSHLPRV_RES_WRITENOMATCHREAD  - For some unknow reason data write dosent match data readed
 */
e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_VerifyNRipristBkup( t_eFSS_TYPE_CbCtx* const p_ptCbCtx, const uint32_t p_uReTry,
                                                          uint8_t* const p_puDataW, const uint32_t p_uDataWLen,
                                                          uint8_t* const p_puDataR, const uint32_t p_uDataRLen,
                                                          const uint32_t p_uOrigIndx, const uint32_t p_uBackupIndx,
                                                          const uint32_t p_uOriSubType, const uint32_t p_uBckUpSubType,
                                                          t_eFSS_TYPE_PageMeta* p_ptPagePrm );

/**
 * @brief       Digest a crc using a seed (first calc) or the previous Crc (digest)
 *
 * @param[in]   p_ptCbCtx     - Pointer to all callback context
 * @param[in]   p_uSeedOrCrc  - Seed or previous CRC calcualted
 * @param[in]   p_puData      - Data to calc crc
 * @param[in]   p_uDataLen    - size of the p_puData buffer
 * @param[in]   p_puCrcCal    - Returned CRC
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully, page are correct
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSHLPRV_RES_BADPARAM          - In case of bad parameter passed to the function
 *              e_eFSS_UTILSHLPRV_RES_CLBCKCRCERR       - Error reported from the callback
 */
e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_CrcDigest( t_eFSS_TYPE_CbCtx* const p_ptCbCtx, const uint32_t p_uSeedOrCrc,
                                                 uint8_t* const p_puData, const uint32_t p_uDataLen,
                                                 uint32_t* const p_puCrcCal );

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_UTILSHLPRV_H */