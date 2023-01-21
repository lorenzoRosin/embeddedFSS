/**
 * @file       eFSS_DB.h
 *
 * @brief      Database module
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

typedef enum
{
    e_eFSS_DB_RES_OK = 0,
    e_eFSS_DB_RES_OK_BKP_RCVRD,
    e_eFSS_DB_RES_BADPARAM,
    e_eFSS_DB_RES_BADPOINTER,
    e_eFSS_DB_RES_CLBCKREPORTERROR,
    e_eFSS_DB_RES_NOTVALIDBLOB,
    e_eFSS_DB_RES_CORRUPTCTX,
    e_eFSS_DB_RES_NOINITLIB,
}e_eFSS_DB_RES;

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
    t_eFSS_DB_DbStruct tDBS;
}t_eFSS_DB_Ctx;

/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the database context
 *
 * @param[in]   p_ptCtx       - Blob context
 * @param[in]   p_ptCtxCb     - All callback collection context
 * @param[in]   p_uPageToUse  - How many page use for the blob module
 * @param[in]   p_uPageSize   - Size of the used pages
 * @param[in]   p_puBuff      - Pointer to a buffer used by the modules to make calc
 * @param[in]   p_uBuffL      - Size of p_puBuff
 *
 * @return      e_eFSS_DB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DB_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_DB_RES_OK            - Operation ended correctly
 */
e_eFSS_DB_RES eFSS_DB_InitCtx(t_eFSS_DB_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx* const p_ptCtxCb,
                                  const uint32_t p_uPageToUse, const uint32_t p_uPageSize, uint8_t* const p_puBuff,
                                  uint32_t p_uBuffL, t_eFSS_DB_DbStruct p_tDBS);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Blob context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_DB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DB_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_DB_RES_OK            - Operation ended correctly
 */
e_eFSS_DB_RES eFSS_DB_IsInit(t_eFSS_DB_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Initialize the byte stuffer context
 *
 * @param[in]   p_ptCtx    - Byte stuffer context
 * @param[in]   p_puBuff   - Pointer to a memory area that we will use to store data that needs to be stuffed
 * @param[in]   p_uBuffL   - Dimension in byte of the memory area
 *
 * @return      e_eFSS_DB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DB_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_DB_RES_OK            - Operation ended correctly
 */
e_eFSS_DB_RES eFSS_DB_GetStorageStatus(t_eFSS_DB_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx* const p_peStatus);

/**
 * @brief       Initialize the byte stuffer context
 *
 * @param[in]   p_ptCtx    - Byte stuffer context
 * @param[in]   p_puBuff   - Pointer to a memory area that we will use to store data that needs to be stuffed
 * @param[in]   p_uBuffL   - Dimension in byte of the memory area
 *
 * @return      e_eFSS_DB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DB_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_DB_RES_OK            - Operation ended correctly
 */
e_eFSS_DB_RES eFSS_DB_Format(t_eFSS_DB_Ctx* const p_ptCtx);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Byte stuffer context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_DB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DB_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_DB_RES_OK            - Operation ended correctly
 */
e_eFSS_DB_RES eFSS_DB_SaveElemen(t_eFSS_DB_Ctx* const p_ptCtx, uint32_t p_uPos, uint8_t* p_puBuff, uint32_t p_uBuffL);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Byte stuffer context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_DB_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DB_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_DB_RES_OK            - Operation ended correctly
 */
e_eFSS_DB_RES eFSS_DB_GetElement(t_eFSS_DB_Ctx* const p_ptCtx, uint32_t p_uPos, uint8_t* p_puBuff, uint8_t* p_puBuffL);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_DB_H */
