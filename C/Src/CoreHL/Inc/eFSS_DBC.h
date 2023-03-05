/**
 * @file       eFSS_DBC.h
 *
 * @brief      Database Core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSS_DBC_H
#define EFSS_DBC_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_COREHL.h"



/***********************************************************************************************************************
 *      PUBLIC TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eFSS_DBC_RES_OK = 0,
    e_eFSS_DBC_RES_NOINITLIB,
    e_eFSS_DBC_RES_BADPARAM,
    e_eFSS_DBC_RES_BADPOINTER,
    e_eFSS_DBC_RES_CORRUPTCTX,
    e_eFSS_DBC_RES_CLBCKERASEERR,
    e_eFSS_DBC_RES_CLBCKWRITEERR,
    e_eFSS_DBC_RES_CLBCKREADERR,
    e_eFSS_DBC_RES_CLBCKCRCERR,
    e_eFSS_DBC_RES_NOTVALIDDB,
    e_eFSS_DBC_RES_NEWVERSIONDB,
    e_eFSS_DBC_RES_WRITENOMATCHREAD,
    e_eFSS_DBC_RES_OK_BKP_RCVRD
}e_eFSS_DBC_RES;

typedef struct
{
    uint8_t*  puBuf;
    uint32_t  uBufL;
}t_eFSS_DBC_StorBuf;

typedef struct
{
    t_eFSS_COREHL_Ctx tCOREHLCtx;
}t_eFSS_DBC_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the Database Core module context
 *
 * @param[in]   p_ptCtx          - Database Core context
 * @param[in]   p_tCtxCb         - All callback collection context
 * @param[in]   p_tStorSet       - Storage settings
 * @param[in]   p_puBuff         - Pointer to a buffer used by the modules to make calc, must be pageSize * 2
 * @param[in]   p_uBuffL         - Size of p_puBuff
 *
 * @return      e_eFSS_DBC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DBC_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_DBC_RES_OK            - Operation ended correctly
 */
e_eFSS_DBC_RES eFSS_DBC_InitCtx(t_eFSS_DBC_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx const p_tCtxCb,
								const t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff,
                                const uint32_t p_uBuffL);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx          - Database Core context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_DBC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_DBC_RES_OK            - Operation ended correctly
 */
e_eFSS_DBC_RES eFSS_DBC_IsInit(t_eFSS_DBC_Ctx* const p_ptCtx, bool_t* const p_pbIsInit);

/**
 * @brief       Get the numbers of usable page and the buffer all in one
 *
 * @param[in]   p_ptCtx       - Database Core context
 * @param[out]  p_ptBuff      - Pointer to a storage collection struct that will be filled with info about internal buf
 * @param[out]  p_puUsePages  - Pointer to a storage settings
 *
 * @return      e_eFSS_DBC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DBC_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_DBC_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_DBC_RES_OK            - Operation ended correctly
 */
e_eFSS_DBC_RES eFSS_DBC_GetBuffNUsable(t_eFSS_DBC_Ctx* const p_ptCtx, t_eFSS_DBC_StorBuf* const p_ptBuff,
                                       uint32_t* const p_puUsePages);

/**
 * @brief       Load a page from the storage area in to the internal buffer
 *
 * @param[in]   p_ptCtx         - Database Core context
 * @param[in]   p_uPageIndx     - uint32_t index rappresenting the page that we want to load from storage
 *
 * @return      e_eFSS_DBC_RES_BADPOINTER      - In case of bad pointer passed to the function
 *		        e_eFSS_DBC_RES_BADPARAM        - In case of an invalid parameter passed to the function
 *		        e_eFSS_DBC_RES_CORRUPTCTX      - Context is corrupted
 *		        e_eFSS_DBC_RES_NOINITLIB       - Need to init lib before calling function
 *		        e_eFSS_DBC_RES_CLBCKREADERR    - The read callback reported an error
 *              e_eFSS_DBC_RES_CLBCKCRCERR     - The crc callback reported an error
 *              e_eFSS_DBC_RES_NOTVALIDPAGE    - The readed page is invalid
 *              e_eFSS_DBC_RES_NEWVERSIONFOUND - The readed page has a new version
 *              e_eFSS_DBC_RES_OK_BKP_RCVRD    - operation ended successfully recovering a backup or an origin page
 *              e_eFSS_DBC_RES_OK              - Operation ended correctly
 */
e_eFSS_DBC_RES eFSS_DBC_LoadPageInBuff(t_eFSS_DBC_Ctx* const p_ptCtx, const uint32_t p_uPageIndx);

/**
 * @brief       Flush the internal buffer in to the storage area.
 *
 * @param[in]   p_ptCtx             - Database Core context
 * @param[in]   p_uPageIndx         - uint32_t index rappresenting the page that we want to flush in storage
 *
 * @return      e_eFSS_DBC_RES_BADPOINTER       - In case of bad pointer passed to the function
 *		        e_eFSS_DBC_RES_BADPARAM         - In case of an invalid parameter passed to the function
 *		        e_eFSS_DBC_RES_CORRUPTCTX       - Context is corrupted
 *		        e_eFSS_DBC_RES_NOINITLIB        - Need to init lib before calling function
 *              e_eFSS_DBC_RES_CLBCKCRCERR      - The crc callback reported an error
 *		        e_eFSS_DBC_RES_CLBCKERASEERR    - The erase callback reported an error
 *		        e_eFSS_DBC_RES_CLBCKWRITEERR    - The write callback reported an error
 *		        e_eFSS_DBC_RES_CLBCKREADERR     - The read callback reported an error
 *		        e_eFSS_DBC_RES_WRITENOMATCHREAD - Writen data dosent match what requested
 *              e_eFSS_DBC_RES_OK               - Operation ended correctly
 */
e_eFSS_DBC_RES eFSS_DBC_FlushBuffInPage(t_eFSS_DBC_Ctx* const p_ptCtx, const uint32_t p_uPageIndx);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_DBC_H */
