/**
 * @file       eFSS_CORELL.h
 *
 * @brief      Low level core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSS_CORELL_H
#define EFSS_CORELL_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_TYPE.h"



/***********************************************************************************************************************
 *      PUBLIC TYPEDEFS
 **********************************************************************************************************************/
 typedef enum
{
    e_eFSS_CORELL_BUFTYPE_1 = 0,
    e_eFSS_CORELL_BUFTYPE_2
}e_eFSS_CORELL_BUFTYPE;

typedef enum
{
    e_eFSS_CORELL_RES_OK = 0,
    e_eFSS_CORELL_RES_OK_BKP_RCVRD,
    e_eFSS_CORELL_RES_NOTVALIDLOG,
    e_eFSS_CORELL_RES_NEWVERSIONLOG,
    e_eFSS_CORELL_RES_NOINITLIB,
    e_eFSS_CORELL_RES_BADPARAM,
    e_eFSS_CORELL_RES_BADPOINTER,
    e_eFSS_CORELL_RES_CORRUPTCTX,
    e_eFSS_CORELL_RES_CLBCKERASEERR,
    e_eFSS_CORELL_RES_CLBCKWRITEERR,
    e_eFSS_CORELL_RES_CLBCKREADERR,
    e_eFSS_CORELL_RES_CLBCKCRCERR,
    e_eFSS_CORELL_RES_WRITENOMATCHREAD,
}e_eFSS_CORELL_RES;

typedef struct
{
    uint32_t    uTotPages;
    uint32_t    uPagesLen;
    uint32_t    uRWERetry;
}t_eFSS_CORELL_StorSet;

typedef struct
{
    bool_t   bIsInit;
    t_eFSS_TYPE_CbCtx tCtxCb;
    t_eFSS_CORELL_StorSet tStorSett;
	uint8_t* puBuf1;
	uint32_t uBuf1L;
	uint8_t* puBuf2;
	uint32_t uBuf2L;
}t_eFSS_CORELL_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the Log Core module context
 *
 * @param[in]   p_ptCtx          - Log Core context
 * @param[in]   p_tCtxCb         - All callback collection context
 * @param[in]   p_puBuff         - Pointer to a buffer used by the modules to make calc
 * @param[in]   p_uBuffL         - Size of p_puBuff
 * @param[in]   p_tStorSet       - Storage settings
 * @param[in]   p_bFlashCache    - Use flash as cache for storing and resuming index
 * @param[in]   p_bFullBckup     - Save every log data in a backup pages
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_LOGC_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_LOGC_RES_OK            - Operation ended correctly
 */
e_eFSS_CORELL_RES eFSS_CORELL_InitCtx(t_eFSS_CORELL_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx const p_tCtxCb,
									  t_eFSS_CORELL_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Log Core context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_LOGC_RES_OK            - Operation ended correctly
 */
e_eFSS_CORELL_RES eFSS_CORELL_IsInit(t_eFSS_CORELL_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Log Core context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_LOGC_RES_OK            - Operation ended correctly
 */
e_eFSS_CORELL_RES eFSS_CORELL_GetBuff(t_eFSS_CORELL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_tBuffType, 
								      uint8_t** p_ppuBuff, uint32_t* p_puBuffL);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Log Core context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_LOGC_RES_OK            - Operation ended correctly
 */
e_eFSS_CORELL_RES eFSS_CORELL_LoadPage(t_eFSS_CORELL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_tBuffType, 
								       const uint32_t p_uPageIndx);
									   
/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Log Core context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_LOGC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_LOGC_RES_OK            - Operation ended correctly
 */
e_eFSS_CORELL_RES eFSS_CORELL_FlushPage(t_eFSS_CORELL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_tBuffType, 
								        const uint32_t p_uPageIndx);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_CORELL_H */