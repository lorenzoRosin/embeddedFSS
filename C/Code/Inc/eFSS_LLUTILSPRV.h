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
typedef struct t_eFSS_LLUTILSPRV_CrcCtxUser t_eFSS_LLUTILSPRV_CrcCtx;

/* Call back of a function that will calculate the CRC for this modules.
 * the p_ptCtx parameter is a custom pointer that can be used by the creator of this CRC callback, and will not be used
 * by the CRCdigest module */
typedef bool_t (*f_eFSS_LLUTILSPRV_CrcCb) ( t_eFSS_LLUTILSPRV_CrcCtx* const p_ptCtx, const uint32_t p_uUseed, const uint8_t* p_puData,
                                     const uint32_t p_uDataL, uint32_t* const p_puCrc32Val );

typedef enum
{
    e_eFSS_LLUTILSPRV_RES_OK = 0,
    e_eFSS_LLUTILSPRV_RES_BADPARAM,
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
 * Erase a memory page
 * @param pginfo Information about memory and pages
 * @param cb Information about callbacks and callbacks pointer
 * @param pageIndx page index to erase
 * @return EFSS_RES_BADPOINTER in case of bad pointer
 *         EFSS_RES_BADPARAM in case of a wrong param passed
 *         EFSS_RES_ERRORERASE erase function callback failed
 *         EFSS_RES_OK page erased successfully
 */
e_eFSS_Res erasePageLL( const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cb, const uint32_t pageIndx );

/**
 * Write a page in memory
 * @param pginfo Information about memory and pages
 * @param cb Information about callbacks and callbacks pointer
 * @param pageIndx page index to write
 * @param dataW data to write
 * @param supportMemory pointer to a memory area used to support the write operation
 * @return EFSS_RES_BADPOINTER in case of bad pointer
 *         EFSS_RES_BADPARAM in case of a wrong param passed
 *         EFSS_RES_ERRORWRITE write function callback failed
 *         EFSS_RES_OK page writed successfully
 */
e_eFSS_Res writePageLL( const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cb, const uint32_t pageIndx, const uint8_t* dataW,
                        uint8_t* const supportMemory );

/**
 * Read a page in memory
 * @param pginfo Information about memory and pages
 * @param cb Information about callbacks and callbacks pointer
 * @param pageIndx page index to write
 * @param dataR pointer to a buffer where the data readed will be copied
 * @param supportMemory pointer to a memory area used to support the write operation
 * @return EFSS_RES_BADPOINTER in case of bad pointer
 *         EFSS_RES_BADPARAM in case of a wrong param passed
 *         EFSS_RES_ERRORREAD read function callback failed
 *         EFSS_RES_OK page readed successfully
 */
e_eFSS_Res readPageLL( const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cb, const uint32_t pageIndx, uint8_t* const dataR );

/**
 * Calculate the CRC of a passed buffer
 * @param cb Information about callbacks and callbacks pointer
 * @param crc pointer to the memory area where the calculated crc will be putted
 * @param data pointer to the buffer where the alg will start calculating the CRC
 * @param dataLen how many byte will be used to calculate the CRC
 * @return EFSS_RES_BADPOINTER in case of bad pointer
 *         EFSS_RES_BADPARAM crc function callback failed
 *         EFSS_RES_OK crc calculated successfully
 */
e_eFSS_Res calcCrcLL(const s_eFSS_Cb cb, uint32_t* const crc, const uint8_t* data, const uint32_t dataLen);

/**
 * Calculate the CRC of a passed buffer using a specific SEED
 * @param cb Information about callbacks and callbacks pointer
 * @param crc pointer to the memory area where the calculated crc will be putted
 * @param data pointer to the buffer where the alg will start calculating the CRC
 * @param dataLen how many byte will be used to calculate the CRC
 * @param seed Seed to use in the calculation
 * @return EFSS_RES_BADPOINTER in case of bad pointer
 *         EFSS_RES_BADPARAM crc function callback failed
 *         EFSS_RES_OK crc calculated successfully
 */
e_eFSS_Res calcCrcSeedLL(const s_eFSS_Cb cb, uint32_t* const crc, const uint8_t* data, const uint32_t dataLen,
                         const uint32_t seed );

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_LLUTILSPRV_H */
