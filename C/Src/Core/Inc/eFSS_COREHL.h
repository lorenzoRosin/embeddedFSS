/**
 * @file       eFSS_COREHL.h
 *
 * @brief      High level core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSS_COREHL_H
#define EFSS_COREHL_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_COREML.h"



/***********************************************************************************************************************
 *      PUBLIC TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eFSS_COREHL_RES_OK = 0,
    e_eFSS_COREHL_RES_NOTVALIDPAGE,
    e_eFSS_COREHL_RES_NEWVERSIONFOUND,
    e_eFSS_COREHL_RES_NOINITLIB,
    e_eFSS_COREHL_RES_BADPARAM,
    e_eFSS_COREHL_RES_BADPOINTER,
    e_eFSS_COREHL_RES_CORRUPTCTX,
    e_eFSS_COREHL_RES_CLBCKERASEERR,
    e_eFSS_COREHL_RES_CLBCKWRITEERR,
    e_eFSS_COREHL_RES_CLBCKREADERR,
    e_eFSS_COREHL_RES_CLBCKCRCERR,
    e_eFSS_COREHL_RES_WRITENOMATCHREAD,
}e_eFSS_COREHL_RES;

typedef struct
{
    uint32_t    uPageUseSpecific1;
    uint32_t    uPageUseSpecific2;
    uint32_t    uPageUseSpecific3;
    uint32_t    uPageUseSpecific4;
    uint8_t     uPageSubType;
}t_eFSS_COREHL_PageMeta;

typedef struct
{
    t_eFSS_COREML_Ctx tCOREMLCtx;
    t_eFSS_COREHL_PageMeta tPageMeta;
}t_eFSS_COREHL_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the High Level Core Module context
 *
 * @param[in]   p_ptCtx          - High Level Log Core context
 * @param[in]   p_tCtxCb         - All callback collection context
 * @param[in]   p_tStorSetLL     - Storage settings LL
 * @param[in]   p_tStorSetML     - Storage settings ML
 * @param[in]   p_puBuff         - Pointer to a buffer used by the modules to make calc
 * @param[in]   p_uBuffL         - Size of p_puBuff
 *
 * @return      e_eFSS_COREHL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_COREHL_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_COREHL_RES_OK            - Operation ended correctly
 */
e_eFSS_COREHL_RES eFSS_COREHL_InitCtx(t_eFSS_COREHL_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx const p_tCtxCb,
									  t_eFSS_CORELL_StorSet p_tStorSetLL, t_eFSS_COREML_StorSet p_tStorSetML,
                                      uint8_t* const p_puBuff, uint32_t p_uBuffL);
/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - High Level Log Core context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_COREHL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_COREHL_RES_OK            - Operation ended correctly
 */
e_eFSS_COREHL_RES eFSS_COREML_IsInit(t_eFSS_COREHL_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Get storage settings
 *
 * @param[in]   p_ptCtx       - High Level Log Core context
 * @param[out]  p_puDataL     - Pointer to a uint32_t variable that will be filled with the usable data lenght
 * @param[out]  p_puNPage     - Pointer to a uint32_t variable that will be filled with the numbers of page
 *
 * @return      e_eFSS_COREHL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_COREHL_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_COREHL_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_COREHL_RES_OK            - Operation ended correctly
 */
e_eFSS_COREHL_RES eFSS_COREML_GetStorSett(t_eFSS_COREHL_Ctx* const p_ptCtx, uint32_t* p_puDataL, uint32_t* p_puNPage,
                                          uint8_t* p_puPageType, uint16_t* p_puPageVersion);

/**
 * @brief       Get reference to one of the two internal buffer
 *
 * @param[in]   p_ptCtx       - High Level Log Core context
 * @param[in]   p_eBuffType   - Enum used to select wich buffer we want to select
 * @param[out]  p_ppuBuff     - Pointer to a Pointer pointing to the p_eBuffType buffer
 * @param[out]  p_puBuffL     - Pointer to a uint32_t variable where the size of p_ppuBuff buffer will be placed
 * @param[out]  p_ptMeta      - Pointer to a t_eFSS_COREHL_PageMeta variable where the metadata of the page will be 
 *                              placed
 *
 * @return      e_eFSS_COREML_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_COREML_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *		        e_eFSS_COREML_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_COREML_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_COREML_RES_OK            - Operation ended correctly
 */
e_eFSS_COREHL_RES eFSS_COREML_GetBuff(t_eFSS_COREHL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_eBuffType,
								      uint8_t** p_ppuBuff, uint32_t* p_puBuffL, t_eFSS_COREHL_PageMeta** p_pptMeta);

/**
 * @brief       Load a page from the storage area in one of he two internal buffer
 *
 * @param[in]   p_ptCtx       - High Level Log Core context
 * @param[in]   p_eBuffType   - Enum used to select wich buffer we want to select
 * @param[in]   p_uPageIndx   - uint32_t index rappresenting the page that we want to load from storage
 *
 * @return      e_eFSS_COREML_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_COREML_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *		        e_eFSS_COREML_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_COREML_RES_NOINITLIB     - Need to init lib before calling function
 *		        e_eFSS_COREML_RES_CLBCKREADERR  - The read callback reported an error
 *              e_eFSS_COREML_RES_OK            - Operation ended correctly
 */
e_eFSS_COREHL_RES eFSS_COREML_LoadPageInBuffNChkVal(t_eFSS_COREHL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_eBuffType,
								                    const uint32_t p_uPageIndx);

/**
 * @brief       Flush one of the two buffer in the storage are. Keep in mine that the other buffer well be used
 *              to check if the data was flushed corretly, and after this operation will contains different value.
 *
 * @param[in]   p_ptCtx       - High Level Log Core context
 * @param[in]   p_eBuffType   - Enum used to select wich buffer we want to select
 * @param[in]   p_uPageIndx   - uint32_t index rappresenting the page that we want to flush in storage
 *
 * @return      e_eFSS_COREML_RES_BADPOINTER       - In case of bad pointer passed to the function
 *		        e_eFSS_COREML_RES_BADPARAM         - In case of an invalid parameter passed to the function
 *		        e_eFSS_COREML_RES_CORRUPTCTX       - Context is corrupted
 *		        e_eFSS_COREML_RES_NOINITLIB        - Need to init lib before calling function
 *		        e_eFSS_COREML_RES_CLBCKREADERR     - The read callback reported an error
 *		        e_eFSS_COREML_RES_CLBCKERASEERR    - The erase callback reported an error
 *		        e_eFSS_COREML_RES_CLBCKWRITEERR    - The write callback reported an error
 *		        e_eFSS_COREML_RES_WRITENOMATCHREAD - Writen data dosent match what requested
 *              e_eFSS_COREML_RES_OK               - Operation ended correctly
 */
e_eFSS_COREHL_RES eFSS_COREML_FlushBuffWUpdValInPage(t_eFSS_COREHL_Ctx* const p_ptCtx,
                                                     e_eFSS_CORELL_BUFTYPE p_eBuffType, const uint32_t p_uPageIndx);

/**
 * @brief       Calculate the Crc of the data present in the choosen buffer. Can also select to calculate the crc of
 *              a given numbers of bytes.
 *
 * @param[in]   p_ptCtx       - High Level Log Core context
 * @param[in]   p_eBuffType   - Enum used to select wich buffer we want to select
 * @param[in]   p_uCrcSeed    - uint32_t rappresenting the seed we want to use in the calc
 * @param[in]   p_uLenCalc    - uint32_t rappresenting the lenght we want to calc
 * @param[out]  p_puCrc       - Pointer to a uint32_t variable where the CRC calculated will be placed
 *
 * @return      e_eFSS_COREML_RES_BADPOINTER       - In case of bad pointer passed to the function
 *		        e_eFSS_COREML_RES_BADPARAM         - In case of an invalid parameter passed to the function
 *		        e_eFSS_COREML_RES_CORRUPTCTX       - Context is corrupted
 *		        e_eFSS_COREML_RES_NOINITLIB        - Need to init lib before calling function
 *		        e_eFSS_COREML_RES_CLBCKCRCERR      - The CRC callback reported an error
 *              e_eFSS_COREML_RES_OK               - Operation ended correctly
 */
e_eFSS_COREHL_RES eFSS_COREML_CalcCrcInBuff(t_eFSS_COREHL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_eBuffType,
								            uint32_t p_uCrcSeed, uint32_t p_uLenCalc, uint32_t* p_puCrc);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_COREHL_H */