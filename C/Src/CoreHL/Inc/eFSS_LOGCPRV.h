/**
 * @file       eFSS_LOGCPRV.h
 *
 * @brief      LOG Core private module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSS_LOGCPRV_H
#define EFSS_LOGCPRV_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_LOGC.h"



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/
typedef struct
{
    uint32_t uOriSubType;
    uint32_t uBckUpSubType;
    uint32_t uNewPageIndex;
    uint32_t uFilledPageIndex;
    uint32_t uByteInPage;
}t_eFSS_LOGCPRV_WriteMeta;

typedef struct
{
    uint32_t uNewPageIndex;
    uint32_t uFilledPageIndex;
    uint32_t uByteInPage;
}t_eFSS_LOGCPRV_WriteCurNewMeta;

typedef struct
{
    uint32_t uNewPageIndex;
    uint32_t uFilledPageIndex;
    uint32_t uByteInPage;
}t_eFSS_LOGCPRV_ReadMeta;

typedef struct
{
    uint32_t uOriSubType;
    uint32_t uBckUpSubType;
}t_eFSS_LOGCPRV_ReadExpectedMeta;

/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Verify if the log core context is valid. Be sure to no pass null to this function
 *
 * @param[in]   p_ptCtx          - Log Core context
 *
 * @return      true    - The t_eFSS_LOGC_Ctx context is valid
 *		        false   - Detected some corruption in the t_eFSS_LOGC_Ctx context
 */
bool_t eFSS_LOGCPRV_IsStatusStillCoherent(const t_eFSS_LOGC_Ctx* p_ptCtx);

/**
 * @brief       Convert enum from HL utils to LOGC module enum
 *
 * @param[in]   p_eHLRes      - HL utils enum value
 *
 * @return      Return the corrisponding value from HL utils operation to LOGC enum code
 */
e_eFSS_LOGC_RES eFSS_LOGCPRV_HLtoLogRes(const e_eFSS_COREHL_RES p_eHLRes);

/**
 * @brief       Get the value of the next index. Be sure to not insert any NULL value and
 *              call eFSS_LOGCPRV_IsStatusStillCoherent before. Do not insert non valid index.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_uIdx           - Index that we want to search for next
 *
 * @return      Return the next index of p_uIdx
 */
uint32_t eFSS_LOGCPRV_GetNextIndex(const t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t p_uIdx);

/**
 * @brief       Get the value of the previous index. Be sure to not insert any NULL value and
 *              call eFSS_LOGCPRV_IsStatusStillCoherent before. Do not insert non valid index.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_uIdx           - Index that we want to search for previous
 *
 * @return      Return the previous index of p_uIdx
 */
uint32_t eFSS_LOGCPRV_GetPrevIndex(const t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t p_uIdx);

/**
 * @brief       Write in cache the value of new and old index. This function take care of the backup pages.
 *              Do not pass to function NULL pointer or invalid index, they are not checked.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_uIdxN          - Index of the new log page  that we want to save in cache
 * @param[in]   p_uIFlP          - Number of filled pages
 *
 * @return      Return error related to read write erase function
 */
e_eFSS_LOGC_RES eFSS_LOGCPRV_WriteCache(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t p_uIdxN, uint32_t p_uIFlP);

/**
 * @brief       Read from cache the value of new and old index. This function take care of the backup pages.
 *              Do not pass to function NULL pointer or invalid index, they are not checked.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_puIdxN         - Index of the new log page that we want to read from cache
 * @param[in]   p_puIFlP         - Number of filled pages
 *
 * @return      Return error related to read write erase function, even invalid page if found.
 */
e_eFSS_LOGC_RES eFSS_LOGCPRV_ReadCache(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t* p_puIdxN, uint32_t* p_puIFlP);

/**
 * @brief       Write a page of data at p_uIdx position. Do not pass to this function NULL value
 *              or invalid index value. This function will take care of any support page.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_uIdx           - Index of the new log page that we want to read from cache
 * @param[in]   p_puBuf          - Pointer to the data that we want to write in storage
 * @param[in]   p_uBufL          - Size of p_puBuf
 * @param[in]   p_puBufS         - Pointer to a buffer that we will use as support buffer
 * @param[in]   p_uBufSL         - Size of p_puBufS
 * @param[in]   p_tParam         - Param data we want to add to the p_puBuf
 *
 * @return      Return error related to read write erase function, even invalid page if found.
 */
e_eFSS_LOGC_RES eFSS_LOGCPRV_WritePage(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t p_uIdx,
                                       uint8_t* p_puBuf, uint32_t p_uBufL,
                                       uint8_t* p_puBufS, uint32_t p_uBufSL,
                                       t_eFSS_LOGCPRV_WriteMeta p_tWriteMEta);

/**
 * @brief       Read a page of data at p_uIdx position. Do not pass to this function NULL value
 *              or invalid index value. This function will take care of any support page.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_uIdx           - Index of the new log page that we want to read from cache
 * @param[in]   p_puBuf          - Pointer to the data that we want to write in storage
 * @param[in]   p_uBufL          - Size of p_puBuf
 * @param[in]   p_puBufS         - Pointer to a buffer that we will use as support buffer
 * @param[in]   p_uBufSL         - Size of p_puBufS
 * @param[in]   p_ptParam        - Param data we want to read from the p_puBuf
 *
 * @return      Return error related to read write erase function, even invalid page if found.
 */
e_eFSS_LOGC_RES eFSS_LOGCPRV_ReadPage(t_eFSS_LOGC_Ctx* const p_ptCtx, uint32_t p_uIdx,
                                      uint8_t* p_puBuf, uint32_t p_uBufL,
                                      uint8_t* p_puBufS, uint32_t p_uBufSL,
                                      t_eFSS_LOGCPRV_ReadExpectedMeta p_tExpectedMeta,
                                      t_eFSS_LOGCPRV_ReadMeta* p_ptReadMeta);

/**
 * @brief       Write a page of data at p_uIdx position. Do not pass to this function NULL value
 *              or invalid index value. This function will take care of any support page.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_uIdx           - Index of the new log page that we want to read from cache
 * @param[in]   p_puBuf          - Pointer to the data that we want to write in storage
 * @param[in]   p_uBufL          - Size of p_puBuf
 * @param[in]   p_puBufS         - Pointer to a buffer that we will use as support buffer
 * @param[in]   p_uBufSL         - Size of p_puBufS
 * @param[in]   p_tParam         - Param data we want to add to the p_puBuf
 *
 * @return      Return error related to read write erase function, even invalid page if found.
 */
e_eFSS_LOGC_RES eFSS_LOGCPRV_WriteCurrNewPageAndbkup(const t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t p_uIdx,
												     uint8_t* p_puBuf, uint32_t p_uBufL,
												     uint8_t* p_puBufS, uint32_t p_uBufSL,
												     t_eFSS_LOGCPRV_WriteCurNewMeta p_tMetaWrite);

/**
 * @brief       Read a page of data at p_uIdx position. Do not pass to this function NULL value
 *              or invalid index value. This function will take care of any support page.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_uIdx           - Index of the new log page that we want to read from cache
 * @param[in]   p_puBuf          - Pointer to the data that we want to write in storage
 * @param[in]   p_uBufL          - Size of p_puBuf
 * @param[in]   p_puBufS         - Pointer to a buffer that we will use as support buffer
 * @param[in]   p_uBufSL         - Size of p_puBufS
 * @param[in]   p_ptParam        - Param data we want to read from the p_puBuf
 *
 * @return      Return error related to read write erase function, even invalid page if found.
 */
e_eFSS_LOGC_RES eFSS_LOGCPRV_ReadCurrNewPageAndbkup(const t_eFSS_LOGC_Ctx* p_ptCtx,uint32_t p_uIdx,
                                                    uint8_t* p_puBuf, uint32_t p_uBufL,
                                                    uint8_t* p_puBufS, uint32_t p_uBufSL,
                                                    t_eFSS_TYPE_PageMeta* p_ptParam);










#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_LOGCPRV_H */