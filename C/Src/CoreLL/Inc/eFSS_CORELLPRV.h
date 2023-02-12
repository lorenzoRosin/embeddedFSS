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
 *      PUBLIC TYPEDEFS
 **********************************************************************************************************************/
typedef struct
{
    uint8_t     uPageType;
    uint16_t    uPageVersion;
    uint32_t    uPageTot;
    uint32_t    uPageMagicNumber;
    uint32_t    uPageCrc;
}t_eFSS_CORELLPRV_PrvMeta;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Extract all metadata from the passed buffer. Keep in mind that not a single controll of any parameter
 *              is executed by this function, so do not pass NULL pointer of invalid buffer. The correct buffer must
 *              be EFSS_PAGEMETALLSIZE long.
 *
 * @param[in]   p_puBuf       - Buffer of EFSS_PAGEMETALLSIZE length which will be used to extract needed parameter
 * @param[out]  p_ptPMeta   - Pointer to a private metadata struct that will be filled with data present in p_puBuf
 * @param[out]  p_ptMeta      - Pointer to a metadata struct that will be filled with data present in p_puBuf
 *
 * @return      Nothing will be returned
 */
void eFSS_CORELLPRV_ExtractData(uint8_t* p_puBuf, t_eFSS_CORELLPRV_PrvMeta* p_ptPMeta, t_eFSS_TYPE_PageMeta* p_ptMeta);

/**
 * @brief       Insert data taken from p_ptPMeta and p_ptMeta inside the passed buffer. Keep in mind that not a
 *              single controll of any parameter is executed by this function, so do not pass NULL pointer of invalid
 *              buffer. The correct buffer must be EFSS_PAGEMETALLSIZE long.
 *
 * @param[out]  p_puBuf       - Buffer of EFSS_PAGEMETALLSIZE length which will be filled with passed parameter
 * @param[in]   p_ptPMeta   - Pointer to a private metadata struct that will be copied inside p_puBuf
 * @param[in]   p_ptMeta      - Pointer to a metadata struct that will be copied inside p_puBuf
 *
 * @return      Nothing will be returned
 */
void eFSS_CORELLPRV_InsertData(uint8_t* p_puBuf, t_eFSS_CORELLPRV_PrvMeta* p_ptPMeta, t_eFSS_TYPE_PageMeta* p_ptMeta);

/**
 * @brief       Insert a CRC32 value inside the passed buffer. Keep in mind that not a single controll of any parameter
 *              is executed by this function, so do not pass NULL pointer of invalid buffer. The correct buffer must
 *              be sizeof(uint32_t) long.
 *
 * @param[out]  p_puBuf       - Buffer of sizeof(uint32_t) length which will be filled with crc value
 * @param[in]   p_uCrc        - Crc to insert
 *
 * @return      Nothing will be returned
 */
void eFSS_CORELLPRV_InsertCrc(uint8_t* p_puBuf, uint32_t p_uCrc);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_CORELLPRV_H */