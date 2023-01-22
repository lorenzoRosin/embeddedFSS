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
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_eHLRes      - HL utils enum value
 *
 * @return      Return the corrisponding value from HL utils operation to LOGC enum code
 */
e_eFSS_LOGC_RES eFSS_LOGCPRV_HLtoLogRes(const e_eFSS_UTILSHLPRV_RES p_eHLRes);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_LOGCPRV_H */