/**
 * @file       eFSS_BLOBC.h
 *
 * @brief      Blob Core - large file core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSS_BLOBC_H
#define EFSS_BLOBC_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_COREHL.h"



/***********************************************************************************************************************
 *      PRIVATE TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eFSS_BLOBC_RES_OK = 0,
    e_eFSS_BLOBC_RES_OK_BKP_RCVRD,
    e_eFSS_BLOBC_RES_NOTVALIDBLOB,
    e_eFSS_BLOBC_RES_NEWVERSIONBLOB,
    e_eFSS_BLOBC_RES_NOINITLIB,
    e_eFSS_BLOBC_RES_BADPARAM,
    e_eFSS_BLOBC_RES_BADPOINTER,
    e_eFSS_BLOBC_RES_CORRUPTCTX,
    e_eFSS_BLOBC_RES_CLBCKERASEERR,
    e_eFSS_BLOBC_RES_CLBCKWRITEERR,
    e_eFSS_BLOBC_RES_CLBCKREADERR,
    e_eFSS_BLOBC_RES_CLBCKCRCERR,
    e_eFSS_BLOBC_RES_WRITENOMATCHREAD,
}e_eFSS_BLOBC_RES;

typedef struct
{
    t_eFSS_COREHL_Ctx tCOREHLCtx;
}t_eFSS_BLOBC_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the blob module context
 *
 * @param[in]   p_ptCtx        - Blob Core context
 * @param[in]   p_tCtxCb       - All callback collection context
 * @param[in]   p_tStorSet     - Storage settings
 * @param[in]   p_puBuff       - Pointer to a buffer used by the modules to make calc, must ne pageSize * 2
 * @param[in]   p_uBuffL       - Size of p_puBuff
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_BLOBC_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_BLOBC_RES_OK            - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_BLOBC_InitCtx(t_eFSS_BLOBC_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx const p_tCtxCb,
                                    t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Blob Core context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_BLOBC_RES_OK            - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_BLOBC_IsInit(t_eFSS_BLOBC_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Get the reference of buffer that we can use to read or write data from storage
 *
 * @param[in]   p_ptCtx       - Log Core context
 * @param[out]  p_ptBuff      - Pointer to a struct that will be filled with info about buffer
 *
 * @return      e_eFSS_BLOBC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_BLOBC_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_BLOBC_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_BLOBC_RES_OK            - Operation ended correctly
 */
e_eFSS_BLOBC_RES eFSS_BLOBC_GetBuff(t_eFSS_BLOBC_Ctx* p_ptCtx, t_eFSS_TYPE_StorBuf* p_ptBuff);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_BLOBC_H */