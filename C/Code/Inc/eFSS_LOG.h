/**
 * @file       eFSS_LOG.h
 *
 * @brief      LOG module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSS_LOG_H
#define EFSS_LOG_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_UTILSLLPRV.h"



/***********************************************************************************************************************
 *      PRIVATE TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eFSS_LOG_RES_OK = 0,
    e_eFSS_LOG_RES_OK_BKP_RCVRD,
    e_eFSS_LOG_RES_BADPARAM,
    e_eFSS_LOG_RES_BADPOINTER,
    e_eFSS_LOG_RES_CLBCKREPORTERROR,
    e_eFSS_LOG_RES_NOTVALIDBLOB,
    e_eFSS_LOG_RES_CORRUPTCTX,
    e_eFSS_LOG_RES_NOINITLIB,
}e_eFSS_LOG_RES;

typedef struct
{
    bool_t   bIsInit;
    t_eFSS_TYPE_CbCtx* ptCtxCb;
	uint8_t* puBuff1;
	uint32_t uBuff1L;
	uint8_t* puBuff2;
	uint32_t uBuff2L;
    uint32_t uNPage;
    uint32_t uPageSize;
    uint32_t uReTry;
    uint32_t uNewestPage;
    uint32_t uOldestPage;
    uint32_t uLogVersion;
    bool_t bUseIndexingFlashCache;
}t_eFSS_LOG_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the byte stuffer context
 *
 * @param[in]   p_ptCtx    - Byte stuffer context
 * @param[in]   p_puBuff   - Pointer to a memory area that we will use to store data that needs to be stuffed
 * @param[in]   p_uBuffL   - Dimension in byte of the memory area
 *
 * @return      e_eFSS_LOG_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_LOG_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_LOG_RES_OK            - Operation ended correctly
 */
e_eFSS_LOG_RES eFSS_LOG_InitCtx(t_eFSS_LOG_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx* const p_ptCtxCb,
                                const uint32_t p_uPageToUse, const uint32_t p_uPageSize, uint8_t* const p_puBuff,
                                uint32_t p_uBuffL);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Byte stuffer context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_LOG_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_LOG_RES_OK            - Operation ended correctly
 */
e_eFSS_LOG_RES eFSS_LOG_IsInit(t_eFSS_LOG_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Initialize the byte stuffer context
 *
 * @param[in]   p_ptCtx    - Byte stuffer context
 * @param[in]   p_puBuff   - Pointer to a memory area that we will use to store data that needs to be stuffed
 * @param[in]   p_uBuffL   - Dimension in byte of the memory area
 *
 * @return      e_eFSS_LOG_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_LOG_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_LOG_RES_OK            - Operation ended correctly
 */
e_eFSS_LOG_RES eFSS_LOG_GetStorageStatus(t_eFSS_LOG_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx* const p_peStatus);

/**
 * @brief       Initialize the byte stuffer context
 *
 * @param[in]   p_ptCtx    - Byte stuffer context
 * @param[in]   p_puBuff   - Pointer to a memory area that we will use to store data that needs to be stuffed
 * @param[in]   p_uBuffL   - Dimension in byte of the memory area
 *
 * @return      e_eFSS_LOG_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_LOG_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_LOG_RES_OK            - Operation ended correctly
 */
e_eFSS_LOG_RES eFSS_LOG_Format(t_eFSS_LOG_Ctx* const p_ptCtx);


/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Byte stuffer context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_LOG_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_LOG_RES_OK            - Operation ended correctly
 */
e_eFSS_LOG_RES eFSS_LOG_GetLogInfo(t_eFSS_LOG_Ctx* const p_ptCtx, uint32_t *NewlogIndex, uint32_t *OldlogIndex,
                                   uint32_t *NpageValorized, uint32_t *NpageUtilizable, uint32_t *LogVersion);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Byte stuffer context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_LOG_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_LOG_RES_OK            - Operation ended correctly
 */
e_eFSS_LOG_RES eFSS_LOG_AddLog(t_eFSS_LOG_Ctx* const p_ptCtx, uint8_t* p_puLogToSave, uint32_t p_uLogL);


/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Byte stuffer context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_LOG_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_LOG_RES_OK            - Operation ended correctly
 */
e_eFSS_LOG_RES eFSS_LOG_GetLogOfASpecificPage(t_eFSS_LOG_Ctx* const p_ptCtx, uint8_t* p_puLogBuff, uint32_t* p_puLogBuffL,
                                              uint32_t *p_puNLogGetted);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_LOG_H */











