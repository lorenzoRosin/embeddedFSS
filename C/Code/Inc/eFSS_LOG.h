/**
 * @file       eFSS_LOG.h
 *
 * @brief      High level utils for fail safe storage
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
    e_eFSS_LOG_RES_BADPARAM,
    e_eFSS_LOG_RES_BADPOINTER,
    e_eFSS_LOG_RES_CLBCKREPORTERROR,
    e_eFSS_LOG_RES_NOTVALIDPAGE
}e_eFSS_LOG_RES;

typedef struct
{
    bool_t   bIsInitCtx;
    bool_t   bIsInitFlash;
    t_eFSS_TYPE_CbCtx* p_ptCtxCb;
	uint8_t* puBuff;
	uint32_t uBuffL;
    uint32_t uPageSize;
    t_eFSS_LOG_DbStruct tDBS;
}t_eFSS_LOG_Ctx;

typedef struct
{
    uint16_t uID;
	uint16_t uLen;
	uint16_t uVer;
    uint8_t* puDefVal;
}t_eFSS_LOG_DbElement;

typedef struct
{
    uint32_t uNumberOfElement;
    t_eFSS_LOG_DbElement* ptElementList;
}t_eFSS_LOG_DbStruct;

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
 * @return      e_eCU_BSTF_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eCU_BSTF_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eCU_BSTF_RES_OK            - Operation ended correctly
 */
e_eCU_BSTF_RES eFSS_LOG_InitCtx(t_eFSS_LOG_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx* const p_ptCtxCb, uint32_t p_uPageToUse,
                               uint32_t p_uPageSize, uint32_t p_uBuffL, uint8_t p_puBuff, t_eFSS_LOG_DbStruct p_tDS);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Byte stuffer context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eCU_BSTF_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eCU_BSTF_RES_OK            - Operation ended correctly
 */
e_eCU_BSTF_RES eFSS_LOG_IsInit(t_eFSS_LOG_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Initialize the byte stuffer context
 *
 * @param[in]   p_ptCtx    - Byte stuffer context
 * @param[in]   p_puBuff   - Pointer to a memory area that we will use to store data that needs to be stuffed
 * @param[in]   p_uBuffL   - Dimension in byte of the memory area
 *
 * @return      e_eCU_BSTF_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eCU_BSTF_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eCU_BSTF_RES_OK            - Operation ended correctly
 */
e_eCU_BSTF_RES eFSS_LOG_GetStorageStatus(t_eFSS_LOG_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx* const p_peStatus);

/**
 * @brief       Initialize the byte stuffer context
 *
 * @param[in]   p_ptCtx    - Byte stuffer context
 * @param[in]   p_puBuff   - Pointer to a memory area that we will use to store data that needs to be stuffed
 * @param[in]   p_uBuffL   - Dimension in byte of the memory area
 *
 * @return      e_eCU_BSTF_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eCU_BSTF_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eCU_BSTF_RES_OK            - Operation ended correctly
 */
e_eCU_BSTF_RES eFSS_LOG_Format(t_eFSS_BLOB_Ctx* const p_ptCtx);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Byte stuffer context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eCU_BSTF_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eCU_BSTF_RES_OK            - Operation ended correctly
 */
e_eCU_BSTF_RES eFSS_LOG_AddLog(t_eFSS_LOG_Ctx* const p_ptCtx, uint8_t* p_puLogToSave, uint32_t p_uLogL);


e_eCU_BSTF_RES eFSS_LOG_GetCurrentLogPageIndex(uint32_t *logIndex);


e_eCU_BSTF_RES eFSS_LOG_GetOldestLogPageIndex(uint32_t *logIndex);


e_eCU_BSTF_RES eFSS_LOG_GetNumberOfLogPageValorized(uint32_t *num);


e_eCU_BSTF_RES eFSS_LOG_GetLogVersion(uint32_t *logVersion);


e_eCU_BSTF_RES eFSS_LOG_GetNumOfTotalLogPage(uint32_t *maxLogPages);

e_eCU_BSTF_RES eFSS_LOG_LogOfASpecificPage(uint32_t *maxLogPages);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_LOG_H */











