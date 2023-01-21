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
 *      DEFINES
 **********************************************************************************************************************/
#define ECU_SOF                                                                                    ( ( uint8_t ) 0xA1u )
#define ECU_EOF                                                                                    ( ( uint8_t ) 0xA2u )
#define ECU_ESC                                                                                    ( ( uint8_t ) 0xA3u )

/* Pages Metadata values */
#define EFSS_PAGEMETASIZE                                                                          ( ( uint32_t )  28u )
#define EFSS_PAGEMAGICNUMBER                                                               ( ( uint32_t )  0xA5A5A5A5u )

/* Pages type */
#define EFSS_PAGETYPE_BLOB                                                                       ( ( uint32_t )  0x01u )
#define EFSS_PAGETYPE_LOG                                                                        ( ( uint32_t )  0x02u )
#define EFSS_PAGETYPE_DB                                                                         ( ( uint32_t )  0x03u )

/* Page BLOB subtypes */
#define EFSS_PAGESUBTYPE_BLOBORI                                                                 ( ( uint32_t )  0x01u )
#define EFSS_PAGESUBTYPE_BLOBBKP                                                                 ( ( uint32_t )  0x02u )

/* Page LOG subtypes */
#define EFSS_PAGESUBTYPE_LOG                                                                     ( ( uint32_t )  0x01u )
#define EFSS_PAGESUBTYPE_LOGNEWEST                                                               ( ( uint32_t )  0x02u )
#define EFSS_PAGESUBTYPE_LOGNEWESTBKP                                                            ( ( uint32_t )  0x03u )
#define EFSS_PAGESUBTYPE_LOGCACHEORI                                                             ( ( uint32_t )  0x04u )
#define EFSS_PAGESUBTYPE_LOGCACHEBKP                                                             ( ( uint32_t )  0x05u )

/* Page DB subtypes */
#define EFSS_PAGESUBTYPE_DB                                                                      ( ( uint32_t )  0x01u )





/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/

/* Define a generic crc callback context that must be implemented by the user */
typedef struct t_eFSS_TYPE_EraseCtxUser t_eFSS_TYPE_EraseCtx;

/* Call back of a function that will calculate the CRC for this modules.
 * the p_ptCtx parameter is a custom pointer that can be used by the creator of this CRC callback, and will not be used
 * by the CRCdigest module */
typedef bool_t (*f_eFSS_TYPE_EraseCb) ( t_eFSS_TYPE_EraseCtx* const p_ptCtx,
                                              const uint32_t p_uPageToErase );

/* Define a generic crc callback context that must be implemented by the user */
typedef struct t_eFSS_TYPE_WriteCtxUser t_eFSS_TYPE_WriteCtx;

/* Call back of a function that will calculate the CRC for this modules.
 * the p_ptCtx parameter is a custom pointer that can be used by the creator of this CRC callback, and will not be used
 * by the CRCdigest module */
typedef bool_t (*f_eFSS_TYPE_WriteCb) ( t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                              const uint32_t p_uPageToWrite, uint8_t* const p_puDataToWrite,
                                              const uint32_t p_uDataToWriteL);

/* Define a generic crc callback context that must be implemented by the user */
typedef struct t_eFSS_TYPE_ReadCtxUser t_eFSS_TYPE_ReadCtx;

/* Call back of a function that will calculate the CRC for this modules.
 * the p_ptCtx parameter is a custom pointer that can be used by the creator of this CRC callback, and will not be used
 * by the CRCdigest module */
typedef bool_t (*f_eFSS_TYPE_ReadCb) ( t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                              const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                              const uint32_t p_uReadBufferL);

/* Define a generic crc callback context that must be implemented by the user */
typedef struct t_eFSS_TYPE_CrcCtxUser t_eFSS_TYPE_CrcCtx;

/* Call back of a function that will calculate the CRC for this modules.
 * the p_ptCtx parameter is a custom pointer that can be used by the creator of this CRC callback, and will not be used
 * by the CRCdigest module */
typedef bool_t (*f_eFSS_TYPE_CrcCb) ( t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
                                            const uint8_t* p_puData, const uint32_t p_uDataL,
                                            uint32_t* const p_puCrc32Val );

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
}t_eFSS_TYPE_CbCtx;

typedef struct
{
    uint8_t     uPageType;
    uint8_t     uPageSubType;
    uint16_t    uPageVersion;
    uint32_t    uPageUseSpecific1;
    uint32_t    uPageUseSpecific2;
    uint32_t    uPageUseSpecific3;
    uint32_t    uPageUseSpecific4;
    uint32_t    uPageMagicNumber;
    uint32_t    uPageCrc;
}t_eFSS_TYPE_PageMeta;

typedef struct
{
    uint32_t    uTotPages;
    uint32_t    uPagesLen;
    uint32_t    uStorageVer;
    uint32_t    uRWERetry;
}t_eFSS_TYPE_StorageSettings;


#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_TYPE_H */