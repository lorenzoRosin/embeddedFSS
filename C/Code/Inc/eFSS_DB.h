/**
 * @file       eFSS_DB.h
 *
 * @brief      High level utils for fail safe storage
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSS_DB_H
#define EFSS_DB_H



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
    e_eFSS_DB_RES_OK = 0,
    e_eFSS_DB_RES_BADPARAM,
    e_eFSS_DB_RES_BADPOINTER,
    e_eFSS_DB_RES_CLBCKREPORTERROR,
    e_eFSS_DB_RES_NOTVALIDPAGE
}e_eFSS_DB_RES;

typedef struct
{
    bool_t   bIsInitCtx;
    bool_t   bIsInitFlash;
    t_eFSS_TYPE_CbCtx* p_ptCtxCb;
	uint8_t* puBuff;
	uint32_t uBuffL;
    uint32_t uPageSize;
    t_eFSS_DB_DbStruct tDBS;
}t_eFSS_DB_Ctx;

typedef struct
{
    uint16_t uID;
	uint16_t uLen;
	uint16_t uVer;
    uint8_t* puDefVal;
}t_eFSS_DB_DbElement;

typedef struct
{
    uint32_t uNumberOfElement;
    t_eFSS_DB_DbElement* ptElementList;
}t_eFSS_DB_DbStruct;

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
e_eCU_BSTF_RES eFSS_DB_InitCtx(t_eFSS_DB_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx* const p_ptCtxCb, uint32_t p_uPageToUse,
                               uint32_t p_uPageSize, uint32_t p_uBuffL, uint8_t p_puBuff, t_eFSS_DB_DbStruct p_tDS);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Byte stuffer context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eCU_BSTF_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eCU_BSTF_RES_OK            - Operation ended correctly
 */
e_eCU_BSTF_RES eFSS_DB_IsInit(t_eFSS_DB_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

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
e_eCU_BSTF_RES eFSS_DB_InitFlash(t_eFSS_DB_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx* const p_ptCtxCb, uint32_t p_uPageToUse,
                               uint32_t p_uPageSize, uint32_t p_uBuffL, uint8_t p_puBuff, t_eFSS_DB_DbStruct p_tDS);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Byte stuffer context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eCU_BSTF_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eCU_BSTF_RES_OK            - Operation ended correctly
 */
e_eCU_BSTF_RES eFSS_DB_IsFlashInit(t_eFSS_DB_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Byte stuffer context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eCU_BSTF_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eCU_BSTF_RES_OK            - Operation ended correctly
 */
e_eCU_BSTF_RES eFSS_DB_SaveElemen(t_eFSS_DB_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Byte stuffer context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eCU_BSTF_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eCU_BSTF_RES_OK            - Operation ended correctly
 */
e_eCU_BSTF_RES eFSS_DB_GetElement(t_eFSS_DB_Ctx* const p_ptCtx, bool_t* p_pbIsInit);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_DB_H */
