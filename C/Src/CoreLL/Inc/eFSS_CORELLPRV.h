/**
 * @file       eFSS_CORELLPRV.h
 *
 * @brief      Low level private core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSS_CORELLPRV_H
#define EFSS_CORELLPRV_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_CORELL.h"




/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Extract all metadata from the passed buffer
 *
 * @param[in]   p_puBuff      - Pointer to a buffer containing data that we need to extract
 * @param[out]  p_ptPrvMeta   - Enum used to select wich buffer we want to select
 * @param[out]  p_ptMetaB     - uint32_t index rappresenting the page that we want to flush in storage
 *
 * @return      Nothing will be returned
 */
void eFSS_CORELLPRV_ExtractData(uint8_t* p_puBuff, t_eFSS_CORELL_privMeta* p_ptPrvMeta,
                                t_eFSS_TYPE_PageMeta* p_ptMetaB);

/**
 * @brief       Insert all metadata in to the the passed buffer
 *
 * @param[in]   p_puBuff      - Pointer to a buffer containing data that we need to extract
 * @param[out]  p_ptPrvMeta   - Enum used to select wich buffer we want to select
 * @param[out]  p_ptMetaB     - uint32_t index rappresenting the page that we want to flush in storage
 *
 * @return      Nothing will be returned
 */
void eFSS_CORELLPRV_InsertData(uint8_t* p_puBuff, t_eFSS_CORELL_privMeta* p_ptPrvMeta,
                               t_eFSS_TYPE_PageMeta* p_ptMetaB);

/**
 * @brief       Insert CRC metadata in to the the passed buffer
 *
 * @param[in]   p_puBuff      - Pointer to a buffer containing data that we need to extract
 * @param[out]  p_uCrc        - Crc to insert
 *
 * @return      Nothing will be returned
 */
void eFSS_CORELLPRV_InsertCrc(uint8_t* p_puBuff, uint32_t p_uCrc);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_CORELLPRV_H */