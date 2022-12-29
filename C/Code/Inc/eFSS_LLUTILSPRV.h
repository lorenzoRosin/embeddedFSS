/**
 * @file Prv_eFSSUtilsLL.h
 *
 */

#ifndef PRVEFSSUTILSLL_H
#define PRVEFSSUTILSLL_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "Prv_eFSSUtils.h"



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/

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



#endif /* PRVEFSSUTILSLL_H */
