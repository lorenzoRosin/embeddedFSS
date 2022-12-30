/**
 * @file       eFSS_LLUTILSPRV.h
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
/* Define a generic crc callback context that must be implemented by the user */
typedef struct t_eFSS_LLUTILSPRV_EraseCtxUser t_eFSS_LLUTILSPRV_EraseCtx;

/* Call back of a function that will calculate the CRC for this modules.
 * the p_ptCtx parameter is a custom pointer that can be used by the creator of this CRC callback, and will not be used
 * by the CRCdigest module */
typedef bool_t (*f_eFSS_LLUTILSPRV_EraseCb) ( t_eFSS_LLUTILSPRV_EraseCtx* const p_ptCtx,
                                              const uint32_t p_uPageToErase );

/* Define a generic crc callback context that must be implemented by the user */
typedef struct t_eFSS_LLUTILSPRV_WriteCtxUser t_eFSS_LLUTILSPRV_WriteCtx;

/* Call back of a function that will calculate the CRC for this modules.
 * the p_ptCtx parameter is a custom pointer that can be used by the creator of this CRC callback, and will not be used
 * by the CRCdigest module */
typedef bool_t (*f_eFSS_LLUTILSPRV_WriteCb) ( t_eFSS_LLUTILSPRV_WriteCtx* const p_ptCtx,
                                              const uint32_t p_uPageToWrite, uint32_t* const p_puDataToWrite
                                              const uint32_t p_uDataToWriteL);

/* Define a generic crc callback context that must be implemented by the user */
typedef struct t_eFSS_LLUTILSPRV_ReadCtxUser t_eFSS_LLUTILSPRV_ReadCtx;

/* Call back of a function that will calculate the CRC for this modules.
 * the p_ptCtx parameter is a custom pointer that can be used by the creator of this CRC callback, and will not be used
 * by the CRCdigest module */
typedef bool_t (*f_eFSS_LLUTILSPRV_ReadCb) ( t_eFSS_LLUTILSPRV_ReadCtx* const p_ptCtx,
                                              const uint32_t p_uPageToRead, uint32_t* const p_puReadBuffer
                                              const uint32_t p_uReadBufferL);

/* Define a generic crc callback context that must be implemented by the user */
typedef struct t_eFSS_LLUTILSPRV_CrcCtxUser t_eFSS_LLUTILSPRV_CrcCtx;

/* Call back of a function that will calculate the CRC for this modules.
 * the p_ptCtx parameter is a custom pointer that can be used by the creator of this CRC callback, and will not be used
 * by the CRCdigest module */
typedef bool_t (*f_eFSS_LLUTILSPRV_CrcCb) ( t_eFSS_LLUTILSPRV_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
                                            const uint8_t* p_puData, const uint32_t p_uDataL,
                                            uint32_t* const p_puCrc32Val );

typedef enum
{
    e_eFSS_LLUTILSPRV_RES_OK = 0,
    e_eFSS_LLUTILSPRV_RES_BADPARAM,
    e_eFSS_LLUTILSPRV_RES_BADPOINTER,
    e_eFSS_LLUTILSPRV_RES_CLBCKREPORTERROR
}e_eFSS_LLUTILSPRV_RES;



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
 * @param[in]   p_fErase      - Erase function pointer
 * @param[in]   p_uPageIndx   - Page index we want to erase
 *
 * @return      e_eFSS_LLUTILSPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_LLUTILSPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_LLUTILSPRV_RES_CLBCKREPORTERROR  - Error reported from the callback
 */
e_eFSS_LLUTILSPRV_RES eFSS_LLUTILSPRV_ErasePage( t_eFSS_LLUTILSPRV_EraseCtx* const uint32_t p_ptCtx,
                                                 f_eFSS_LLUTILSPRV_EraseCb p_fErase, const uint32_t p_uPageIndx );

/**
 * @brief       Write a specified page and verify the operation by reading the data back
 *
 * @param[in]   p_ptCtx       - Erase function context
 * @param[in]   p_fErase      - Erase function pointer
 * @param[in]   p_uPageIndx   - Page index we want to erase
 *
 * @return      e_eFSS_LLUTILSPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_LLUTILSPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_LLUTILSPRV_RES_BADPARAM          - In case of bad param passed to the function
 *              e_eFSS_LLUTILSPRV_RES_CLBCKREPORTERROR  - Error reported from the callback
 */
e_eFSS_LLUTILSPRV_RES eFSS_LLUTILSPRV_WritePage( t_eFSS_LLUTILSPRV_WriteCtx* const uint32_t p_ptCtxW,
                                                 f_eFSS_LLUTILSPRV_WriteCb p_fWrite,
                                                 t_eFSS_LLUTILSPRV_ReadCtx* const uint32_t p_ptCtxR,
                                                 f_eFSS_LLUTILSPRV_ReadCb p_fRead,
                                                 const uint32_t p_uPageIndx,
                                                 uint8_t* const p_puDataW, const uint32_t p_uDataWriteLen,
                                                 uint8_t* const p_puDataR, const uint32_t p_uDataReadLen );
/**
 * @brief       Read a specified page and verify the operation by reading the data back
 *
 * @param[in]   p_ptCtx       - Erase function context
 * @param[in]   p_fErase      - Erase function pointer
 * @param[in]   p_uPageIndx   - Page index we want to erase
 *
 * @return      e_eFSS_LLUTILSPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_LLUTILSPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_LLUTILSPRV_RES_BADPARAM          - In case of bad param passed to the function
 *              e_eFSS_LLUTILSPRV_RES_CLBCKREPORTERROR  - Error reported from the callback
 */
e_eFSS_LLUTILSPRV_RES eFSS_LLUTILSPRV_ReadPage( t_eFSS_LLUTILSPRV_ReadCtx* const uint32_t p_ptCtx,
                                                f_eFSS_LLUTILSPRV_ReadCb p_fRead,
                                                const uint32_t p_uPageIndx,
                                                uint8_t* const p_puDataR, const uint32_t p_uDataReadLen );

/**
 * @brief       Calculate the CRC 32 of a passed buffer using 0xFFFFFFFF as seed
 *
 * @param[in]   p_ptCtx       - Erase function context
 * @param[in]   p_fErase      - Erase function pointer
 * @param[in]   p_uPageIndx   - Page index we want to erase
 *
 * @return      e_eFSS_LLUTILSPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_LLUTILSPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_LLUTILSPRV_RES_BADPARAM          - In case of bad param passed to the function
 *              e_eFSS_LLUTILSPRV_RES_CLBCKREPORTERROR  - Error reported from the callback
 */
e_eFSS_LLUTILSPRV_RES eFSS_LLUTILSPRV_CalcCrc32( t_eFSS_LLUTILSPRV_CrcCtx* const uint32_t p_ptCtx,
                                                 f_eFSS_LLUTILSPRV_CrcCb p_fCrc32,
                                                 uint8_t* const p_puData, const uint32_t p_uDataLen,
                                                 uint32_t* const p_puCrcCalculated );



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_LLUTILSPRV_H */