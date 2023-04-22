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
    e_eFSS_DBC_RES_NEWVERSIONFOUND,
    e_eFSS_DBC_RES_WRITENOMATCHREAD,
    e_eFSS_DBC_RES_OK_BKP_RCVRD
}e_eFSS_DBC_RES;

typedef struct
{
    uint8_t* puBuf;
    uint32_t uBufL;
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
e_eFSS_DBC_RES eFSS_DBC_InitCtx(t_eFSS_DBC_Ctx* const p_ptCtx, const t_eFSS_TYPE_CbStorCtx p_tCtxCb,
								const t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff,
                                const uint32_t p_uBuffL);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Database Core context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_DBC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_DBC_RES_OK            - Operation ended correctly
 */
e_eFSS_DBC_RES eFSS_DBC_IsInit(const t_eFSS_DBC_Ctx* p_ptCtx, bool_t* const p_pbIsInit);

/**
 * @brief       Get the numbers of usable page for writing or reading logs and the buffer that we can use to read and
 *              write data. The size of the buffer refers only to the user avaiable data, others private metadata
 *              are not avaiable in this stage.
 *
 * @param[in]   p_ptCtx       - Database Core context
 * @param[out]  p_ptBuff      - Pointer to a struct that will be filled with info about internal buffer
 * @param[out]  p_puUsePages  - Pointer to an uint32_t that will be filled with the numbers of usable pages
 *
 * @return      e_eFSS_DBC_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_DBC_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_DBC_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_DBC_RES_OK            - Operation ended correctly
 */
e_eFSS_DBC_RES eFSS_DBC_GetBuffNUsable(t_eFSS_DBC_Ctx* const p_ptCtx, t_eFSS_DBC_StorBuf* const p_ptBuff,
                                       uint32_t* const p_puUsePages);

/**
 * @brief       Load a page from the storage area in to the internal buffer. Backup pages area managed automatically
 *
 * @param[in]   p_ptCtx         - Database Core context
 * @param[in]   p_uPageIndx     - uint32_t index rappresenting the page that we want to load from storage. Must be a
 *                                a value that can vary from 0 to p_puUsePages-1 (returned from
 *                                eFSS_DBC_GetBuffNUsable).
 *
 * @return      e_eFSS_DBC_RES_BADPOINTER       - In case of bad pointer passed to the function
 *		        e_eFSS_DBC_RES_BADPARAM         - In case of an invalid parameter passed to the function
 *		        e_eFSS_DBC_RES_CORRUPTCTX       - Context is corrupted
 *		        e_eFSS_DBC_RES_NOINITLIB        - Need to init lib before calling function
 *		        e_eFSS_DBC_RES_CLBCKREADERR     - The read callback reported an error
 *              e_eFSS_DBC_RES_CLBCKCRCERR      - The crc callback reported an error
 *              e_eFSS_DBC_RES_NOTVALIDDB       - The readed page is invalid
 *              e_eFSS_DBC_RES_NEWVERSIONDB     - The readed page has a new version
 *              e_eFSS_DBC_RES_CLBCKERASEERR    - Error reported from the callback
 *              e_eFSS_DBC_RES_CLBCKWRITEERR    - Error reported from the callback
 *              e_eFSS_DBC_RES_WRITENOMATCHREAD - For some unknow reason data write dosent match data readed
 *              e_eFSS_DBC_RES_OK_BKP_RCVRD     - operation ended successfully recovering a backup or an origin page
 *              e_eFSS_DBC_RES_OK               - Operation ended correctly
 */
e_eFSS_DBC_RES eFSS_DBC_LoadPageInBuff(t_eFSS_DBC_Ctx* const p_ptCtx, const uint32_t p_uPageIndx);

/**
 * @brief       Flush the internal buffer in to the storage area. Backup pages area managed automatically
 *
 * @param[in]   p_ptCtx             - Database Core context
 * @param[in]   p_uPageIndx         - uint32_t index rappresenting the page that we want to flush in storage. Must be a
 *                                    a value that can vary from 0 to p_puUsePages-1 (returned from
 *                                    eFSS_DBC_GetBuffNUsable).
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