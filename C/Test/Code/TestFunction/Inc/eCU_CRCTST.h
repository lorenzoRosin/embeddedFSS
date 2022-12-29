/**
 * @file       eCU_CRCTST.h
 *
 * @brief      CRC test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef ECU_CRCTST_H
#define ECU_CRCTST_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_TYPE.h"



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Test the crc module
 *
 * @param[in]   none
 *
 * @return      none
 */
void eCU_CRCTST_ExeTest(void);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* ECU_CRCTST_H */