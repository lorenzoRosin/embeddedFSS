/**
 * @file       eFSS_Utils.h
 *
 * @brief      Utils module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSS_UTILS_H
#define EFSS_UTILS_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_STANDARDTYPE.h"



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Insert a value inside the passed buffer. Keep in mind that the buffer must be atleast equals to the
 *				size of the data that we are inserting
 *
 * @param[in]   p_puBuf       - Pointer to a buffer where to insert an uint32_t
 * @param[in]   p_uToInsert   - uint32_t value that we need to insert in the p_puBuf
 *
 * @return      Return true if the buffer is not NULL, otherwise return false
 */
bool_t eFSS_Utils_InsertU32(uint8_t* const p_puBuf, const uint32_t p_uToInsert);

/**
 * @brief       Insert a value inside the passed buffer. Keep in mind that the buffer must be atleast equals to the
 *				size of the data that we are inserting
 *
 * @param[in]   p_puBuf       - Pointer to a buffer where to insert an uint16_t
 * @param[in]   p_uToInsert   - uint16_t value that we need to insert in the p_puBuf
 *
 * @return      Return true if the buffer is not NULL, otherwise return false
 */
bool_t eFSS_Utils_InsertU16(uint8_t* const p_puBuf, const uint16_t p_uToInsert);

/**
 * @brief       Insert a value inside the passed buffer. Keep in mind that the buffer must be atleast equals to the
 *				size of the data that we are inserting
 *
 * @param[in]   p_puBuf       - Pointer to a buffer where to insert an uint8_t
 * @param[in]   p_uToInsert   - uint8_t value that we need to insert in the p_puBuf
 *
 * @return      Return true if the buffer is not NULL, otherwise return false
 */
bool_t eFSS_Utils_InsertU8(uint8_t* const p_puBuf, const uint8_t p_uToInsert);

/**
 * @brief       Retrive a value from the passed buffer. Keep in mind that the buffer must be atleast equals to the
 *				size of the data that we are retriving
 *
 * @param[in]   p_puBuf       - Pointer to a buffer where retrive an uint32_t
 * @param[out]  p_puToRetrive - Pointer to a uint32_t value that we need to retrive from the p_puBuf
 *
 * @return      Return true if the buffer and the p_puToRetrive are not NULL, otherwise return false
 */
bool_t eFSS_Utils_RetriveU32(const uint8_t* p_puBuf, uint32_t* const p_puToRetrive);

/**
 * @brief       Retrive a value from the passed buffer. Keep in mind that the buffer must be atleast equals to the
 *				size of the data that we are retriving
 *
 * @param[in]   p_puBuf       - Pointer to a buffer where retrive an uint16_t
 * @param[out]  p_puToRetrive - Pointer to a uint16_t value that we need to retrive from the p_puBuf
 *
 * @return      Return true if the buffer and the p_puToRetrive are not NULL, otherwise return false
 */
bool_t eFSS_Utils_RetriveU16(const uint8_t* p_puBuf, uint16_t* const p_puToRetrive);

/**
 * @brief       Retrive a value from the passed buffer. Keep in mind that the buffer must be atleast equals to the
 *				size of the data that we are retriving
 *
 * @param[in]   p_puBuf       - Pointer to a buffer where retrive an uint8_t
 * @param[out]  p_puToRetrive - Pointer to a uint8_t value that we need to retrive from the p_puBuf
 *
 * @return      Return true if the buffer and the p_puToRetrive are not NULL, otherwise return false
 */
bool_t eFSS_Utils_RetriveU8(const uint8_t* p_puBuf, uint8_t* const p_puToRetrive);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_UTILS_H */