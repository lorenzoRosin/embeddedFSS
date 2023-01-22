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
e_eFSS_LOGC_RES eFSS_LOGCPRV_HLtoLogRes(const e_eFSS_UTILSHLPRV_RES p_eHLRes);

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
 * @brief       Get the value of the previous index. Be sure to not insert any NULL value and
 *              call eFSS_LOGCPRV_IsStatusStillCoherent before. Do not insert non valid index.
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_uIdx           - Index that we want to search for previous
 *
 * @return      Return the previous index of p_uIdx
 */
e_eFSS_LOGC_RES eFSS_LOGCPRV_WriteCache(const t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t p_uIdxN, uint32_t p_uIdxO);

e_eFSS_LOGC_RES eFSS_LOGCPRV_ReadCache(const t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t* p_puIdxN, uint32_t* p_puIdxO);

e_eFSS_LOGC_RES eFSS_LOGCPRV_WritePage(const t_eFSS_LOGC_Ctx* p_ptCtx, uint32_t p_uIdx, 
                                       uint8_t* p_puBuf, uint32_t p_uBufL,
                                       uint8_t* p_puBufS, uint32_t p_uBufSL, 
                                       t_eFSS_TYPE_PageMeta p_tParam);

e_eFSS_LOGC_RES eFSS_LOGCPRV_ReadPage(const t_eFSS_LOGC_Ctx* p_ptCtx,uint32_t p_uIdx,  
                                      uint8_t* p_puBuf, uint32_t p_uBufL,
                                      uint8_t* p_puBufS, uint32_t p_uBufSL, 
                                      t_eFSS_TYPE_PageMeta* p_ptParam);

e_eFSS_LOGC_RES eFSS_LOGC_WriteCurrNewPageAndbkup(t_eFSS_LOGC_Ctx* const p_ptCtx, uint8_t* p_puNewPage, t_eFSS_TYPE_PageMeta* p_ptMeta);


#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_LOGCPRV_H */