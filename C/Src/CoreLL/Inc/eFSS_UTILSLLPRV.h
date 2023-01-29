/**
 * @file       eFSS_UTILSLLPRV.h
 *
 * @brief      Low level utils for fail safe storage
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSS_LLUTILSPRV_H
#define EFSS_LLUTILSPRV_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_TYPE.h"



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eFSS_UTILSLLPRV_RES_OK = 0,
    e_eFSS_UTILSLLPRV_RES_BADPARAM,
    e_eFSS_UTILSLLPRV_RES_BADPOINTER,
    e_eFSS_UTILSLLPRV_RES_CLBCKERASEERR,
    e_eFSS_UTILSLLPRV_RES_CLBCKWRITEERR,
    e_eFSS_UTILSLLPRV_RES_CLBCKREADERR,
    e_eFSS_UTILSLLPRV_RES_CLBCKCRCERR,
    e_eFSS_UTILSLLPRV_RES_WRITENOMATCHREAD
}e_eFSS_UTILSLLPRV_RES;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Erase then Write a specified page and verify the operation by reading the data back
 *
 * @param[in]   p_ptCbCtx       - Call back functions contexts
 * @param[in]   p_puDataW       - Pointer to the data we want to write
 * @param[in]   p_uDataWLen     - size of the p_puDataW buffer
 * @param[in]   p_puDataR       - Pointer to support buffer used to verify the data just written
 * @param[in]   p_uDataRLen     - size of the p_puDataR buffer
 * @param[in]   p_uPageIndx     - Page index we want to write
 * @param[in]   p_uReTry        - How many times we can retry if some error happens
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSLLPRV_RES_BADPARAM          - In case of bad param passed to the function
 *              e_eFSS_UTILSLLPRV_RES_CLBCKERASEERR     - Error reported from the callback
 *              e_eFSS_UTILSLLPRV_RES_CLBCKWRITEERR     - Error reported from the callback
 *              e_eFSS_UTILSLLPRV_RES_CLBCKREADERR      - Error reported from the callback
 *              e_eFSS_UTILSLLPRV_RES_WRITENOMATCHREAD  - For some unknow reason data write dosent match data readed
 */
e_eFSS_UTILSLLPRV_RES eFSS_UTILSLLPRV_WritePage( t_eFSS_TYPE_CbCtx* const p_ptCbCtx,
                                                 uint8_t* const p_puDataW, const uint32_t p_uDataWLen,
                                                 uint8_t* const p_puDataR, const uint32_t p_uDataRLen,
                                                 const uint32_t p_uPageIndx, const uint32_t p_uReTry);
/**
 * @brief       Read a specified page
 *
 * @param[in]   p_ptCbCtx     - Call back functions contexts
 * @param[in]   p_uPageIndx   - Page index we want to read
 * @param[in]   p_puDataR     - Pointer to buffer used to read data
 * @param[in]   p_uDataRLen   - size of the p_puDataR buffer
 * @param[in]   p_uReTry      - How many times we can retry if some error happens
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSLLPRV_RES_BADPARAM          - In case of bad param passed to the function
 *              e_eFSS_UTILSLLPRV_RES_CLBCKREADERR      - Error reported from the callback
 */
e_eFSS_UTILSLLPRV_RES eFSS_UTILSLLPRV_ReadPage( t_eFSS_TYPE_CbCtx* const p_ptCbCtx, const uint32_t p_uPageIndx,
                                                uint8_t* const p_puDataR, const uint32_t p_uDataRLen,
                                                const uint32_t p_uReTry );

/**
 * @brief       Calculate the CRC 32 of a passed buffer using custom seed
 *
 * @param[in]   p_ptCbCtx     - Call back functions contexts
 * @param[in]   p_puData      - Pointer to the data we want to calculate the CRC
 * @param[in]   p_uDataLen    - size of the p_puData buffer
 * @param[out]  p_puCrcCal    - Pointer where the calculated CRc result will be saved
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSLLPRV_RES_BADPARAM          - In case of bad param passed to the function
 *              e_eFSS_UTILSLLPRV_RES_CLBCKCRCERR       - Error reported from the callback
 */
e_eFSS_UTILSLLPRV_RES eFSS_UTILSLLPRV_CalcCrc32CS( t_eFSS_TYPE_CbCtx* const p_ptCbCtx, uint32_t p_uSeed,
                                                   uint8_t* const p_puData, const uint32_t p_uDataLen,
                                                   uint32_t* const p_puCrcCal );



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_LLUTILSPRV_H */