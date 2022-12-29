/**
 * @file       eCU_CRC.h
 *
 * @brief      Cal crc32 (CRC-32/MPEG-2) utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef ECU_CRC_H
#define ECU_CRC_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eCU_TYPE.h"



/***********************************************************************************************************************
 *      DEFINES
 **********************************************************************************************************************/
#define eCU_CRC_BASE_SEED                                                                    ( (uint32_t ) 0xFFFFFFFFu )



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eCU_CRC_RES_OK = 0,
    e_eCU_CRC_RES_BADPOINTER
}e_eCU_CRC_RES;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Calculate the CRC 32 (CRC-32/MPEG-2) of a passed buffer using as seed the default value of 0xFFFFFFFFu
 *
 * @param[in]   p_puData      - Pointer to the data buffer used to calculate CRC 32
 * @param[in]   p_uDataL      - How many byte will be used to calculate the CRC 32
 * @param[out]  p_puCrc       - Pointer to an uint32_t were we will store the calculated CRC 32
 *
 * @return      e_eCU_CRC_RES_BADPOINTER      - In case of bad pointer passed to the function
 *              e_eCU_CRC_RES_OK              - Crc 32 calculated successfully
 */
e_eCU_CRC_RES eCU_CRC_32(const uint8_t* p_puData, const uint32_t p_uDataL, uint32_t* const p_puCrc);


/**
 * @brief       Calculate the CRC 32 (CRC-32/MPEG-2) of a passed buffer using a custom p_uSeed
 *
 * @param[in]   p_uSeed     - Seed that will be used to calculate the CRC 32
 * @param[in]   p_puData    - Pointer to the data buffer used to calculate CRC 32
 * @param[in]   p_uDataL    - How many byte will be used to calculate the CRC 32
 * @param[out]  p_puCrc     - Pointer to an uint32_t were we will store the calculated CRC 32
 *
 * @return      e_eCU_CRC_RES_BADPOINTER      - In case of bad pointer passed to the function
 *              e_eCU_CRC_RES_OK              - Crc 32 calculated successfully
 */
e_eCU_CRC_RES eCU_CRC_32Seed(const uint32_t p_uSeed, const uint8_t* p_puData, const uint32_t p_uDataL,
                             uint32_t* const p_puCrc);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* ECU_CRC_H */