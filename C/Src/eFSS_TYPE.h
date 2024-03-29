/**
 * @file       eFSS_TYPE.h
 *
 * @brief      eFss lib type header
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSS_TYPE_H
#define EFSS_TYPE_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_STANDARDTYPE.h"



/***********************************************************************************************************************
 *      TYPEDEFS CALLBACK
 **********************************************************************************************************************/

/* Define a generic storage erase function callback context that must be implemented by the user */
typedef struct t_eFSS_TYPE_EraseCtxUser t_eFSS_TYPE_EraseCtx;

/* Call back of a function that will erase a specified page of the storage area
 * the p_ptCtx parameter is a custom context pointer that can be used by the creator of this ERASE callback,
 * and will not be used by the LIB */
typedef bool_t (*f_eFSS_TYPE_EraseCb) ( t_eFSS_TYPE_EraseCtx* const p_ptCtx,
                                        const uint32_t p_uPageToErase );

/* Define a generic storage write function callback context that must be implemented by the user */
typedef struct t_eFSS_TYPE_WriteCtxUser t_eFSS_TYPE_WriteCtx;

/* Call back of a function that will write a specified page of the storage area
 * the p_ptCtx parameter is a custom context pointer that can be used by the creator of this WRITE callback,
 * and will not be used by the LIB */
typedef bool_t (*f_eFSS_TYPE_WriteCb) ( t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                        const uint32_t p_uPageToWrite, const uint8_t* p_puDataToWrite,
                                        const uint32_t p_uDataToWriteL );

/* Define a generic storage read function callback context that must be implemented by the user */
typedef struct t_eFSS_TYPE_ReadCtxUser t_eFSS_TYPE_ReadCtx;

/* Call back of a function that will read a specified page of the storage area
 * the p_ptCtx parameter is a custom context pointer that can be used by the creator of this READ callback,
 * and will not be used by the LIB */
typedef bool_t (*f_eFSS_TYPE_ReadCb) ( t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                       const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                       const uint32_t p_uReadBufferL );

/* Define a generic crc callback context that must be implemented by the user */
typedef struct t_eFSS_TYPE_CrcCtxUser t_eFSS_TYPE_CrcCtx;

/* Call back of a function that will calculate the CRC for this modules.
 * the p_ptCtx parameter is a custom pointer that can be used by the creator of this CRC callback, and will not be used
 * by the LIB */
typedef bool_t (*f_eFSS_TYPE_CrcCb) ( t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
                                      const uint8_t* p_puData, const uint32_t p_uDataL,
                                      uint32_t* const p_puCrc32Val );



/***********************************************************************************************************************
 *      TYPEDEFS STRUCT
 **********************************************************************************************************************/

typedef struct
{
    t_eFSS_TYPE_EraseCtx* ptCtxErase;
    f_eFSS_TYPE_EraseCb   fErase;
	t_eFSS_TYPE_WriteCtx* ptCtxWrite;
    f_eFSS_TYPE_WriteCb   fWrite;
	t_eFSS_TYPE_ReadCtx*  ptCtxRead;
    f_eFSS_TYPE_ReadCb    fRead;
	t_eFSS_TYPE_CrcCtx*   ptCtxCrc32;
    f_eFSS_TYPE_CrcCb     fCrc32;
}t_eFSS_TYPE_CbStorCtx;

typedef struct
{
    uint32_t    uTotPages;
    uint32_t    uPagesLen;
    uint32_t    uRWERetry;
    uint16_t    uPageVersion;
}t_eFSS_TYPE_StorSet;



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_TYPE_H */