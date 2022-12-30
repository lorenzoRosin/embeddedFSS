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
    e_eFSS_UTILSLLPRV_RES_CLBCKREPORTERROR
}e_eFSS_UTILSLLPRV_RES;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/

/**
 * @brief       Erase a specified page
 *
 * @param[in]   p_ptCtx       - Erase function context
 * @param[in]   p_uPageIndx   - Page index we want to erase
 * @param[in]   p_uReTry      - Erase function pointer
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSLLPRV_RES_CLBCKREPORTERROR  - Error reported from the callback
 */
e_eFSS_UTILSLLPRV_RES eFSS_UTILSLLPRV_ErasePage( t_eFSS_TYPE_CbCtx* const p_ptCtx, const uint32_t p_uPageIndx,
                                                  const uint32_t p_uReTry);

/**
 * @brief       Write a specified page and verify the operation by reading the data back
 *
 * @param[in]   p_ptCtx       - Erase function context
 * @param[in]   p_fErase      - Erase function pointer
 * @param[in]   p_uPageIndx   - Page index we want to erase
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSLLPRV_RES_BADPARAM          - In case of bad param passed to the function
 *              e_eFSS_UTILSLLPRV_RES_CLBCKREPORTERROR  - Error reported from the callback
 */
e_eFSS_UTILSLLPRV_RES eFSS_UTILSLLPRV_WritePage( t_eFSS_TYPE_CbCtx* const p_ptCtx,
                                                 uint8_t* const p_puDataW, const uint32_t p_uDataWriteLen,
                                                 uint8_t* const p_puDataR, const uint32_t p_uDataReadLen,
                                                 const uint32_t p_uPageIndx, const uint32_t p_uReTry);
/**
 * @brief       Read a specified page and verify the operation by reading the data back
 *
 * @param[in]   p_ptCtx       - Erase function context
 * @param[in]   p_fErase      - Erase function pointer
 * @param[in]   p_uPageIndx   - Page index we want to erase
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSLLPRV_RES_BADPARAM          - In case of bad param passed to the function
 *              e_eFSS_UTILSLLPRV_RES_CLBCKREPORTERROR  - Error reported from the callback
 */
e_eFSS_UTILSLLPRV_RES eFSS_UTILSLLPRV_ReadPage( t_eFSS_TYPE_CbCtx* const p_ptCtx, const uint32_t p_uPageIndx,
                                                uint8_t* const p_puDataR, const uint32_t p_uDataReadLen,
                                                const uint32_t p_uReTry );

/**
 * @brief       Calculate the CRC 32 of a passed buffer using 0xFFFFFFFF as seed
 *
 * @param[in]   p_ptCtx       - Erase function context
 * @param[in]   p_fErase      - Erase function pointer
 * @param[in]   p_uPageIndx   - Page index we want to erase
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSLLPRV_RES_BADPARAM          - In case of bad param passed to the function
 *              e_eFSS_UTILSLLPRV_RES_CLBCKREPORTERROR  - Error reported from the callback
 */
e_eFSS_UTILSLLPRV_RES eFSS_UTILSLLPRV_CalcCrc32( t_eFSS_TYPE_CbCtx* const p_ptCtx,
                                                 uint8_t* const p_puData, const uint32_t p_uDataLen,
                                                 uint32_t* const p_puCrcCalculated );



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_LLUTILSPRV_H */