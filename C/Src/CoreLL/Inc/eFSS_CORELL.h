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
    e_eFSS_CORELL_RES_OK = 0,
    e_eFSS_CORELL_RES_NOINITLIB,
    e_eFSS_CORELL_RES_BADPARAM,
    e_eFSS_CORELL_RES_BADPOINTER,
    e_eFSS_CORELL_RES_CORRUPTCTX,
    e_eFSS_CORELL_RES_CLBCKERASEERR,
    e_eFSS_CORELL_RES_CLBCKWRITEERR,
    e_eFSS_CORELL_RES_CLBCKREADERR,
    e_eFSS_CORELL_RES_CLBCKCRCERR,
    e_eFSS_CORELL_RES_NOTVALIDPAGE,
    e_eFSS_CORELL_RES_NEWVERSIONFOUND,
    e_eFSS_CORELL_RES_WRITENOMATCHREAD,
}e_eFSS_CORELL_RES;

typedef enum
{
    e_eFSS_CORELL_BUFFTYPE_1 = 0,
    e_eFSS_CORELL_BUFFTYPE_2
}e_eFSS_CORELL_BUFFTYPE;

typedef struct
{
    uint8_t*  puBuf;
    uint32_t  uBufL;
}t_eFSS_CORELL_StorBufPrv;

typedef struct
{
    uint8_t*  puBuf;
    uint32_t  uBufL;
}t_eFSS_CORELL_StorBuf;

typedef struct
{
    bool_t   bIsInit;
    t_eFSS_TYPE_CbStorCtx tCtxCb;
    t_eFSS_TYPE_StorSet tStorSett;
    uint8_t uStorType;
    t_eFSS_CORELL_StorBufPrv tBuff1;
    t_eFSS_CORELL_StorBufPrv tBuff2;
}t_eFSS_CORELL_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the Low Level Core Module context
 *
 * @param[in]   p_ptCtx          - Low Level Core context
 * @param[in]   p_tCtxCb         - All callback collection context
 * @param[in]   p_tStorSet       - Storage settings
 * @param[in]   p_uStorType      - Storage type
 * @param[in]   p_puBuff         - Pointer to a buffer used by the modules to make calc, must be pageSize * 2
 * @param[in]   p_uBuffL         - Size of p_puBuff
 *
 * @return      e_eFSS_CORELL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_CORELL_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eFSS_CORELL_RES_OK            - Operation ended correctly
 */
e_eFSS_CORELL_RES eFSS_CORELL_InitCtx(t_eFSS_CORELL_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx const p_tCtxCb,
									  const t_eFSS_TYPE_StorSet p_tStorSet, const uint8_t p_uStorType,
                                      uint8_t* const p_puBuff, const uint32_t p_uBuffL);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Low Level Core context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSS_CORELL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSS_CORELL_RES_OK            - Operation ended correctly
 */
e_eFSS_CORELL_RES eFSS_CORELL_IsInit(t_eFSS_CORELL_Ctx* const p_ptCtx, bool_t* const p_pbIsInit);

/**
 * @brief       Get storage settings
 *
 * @param[in]   p_ptCtx       - Low Level Core context
 * @param[out]  p_ptStorSet   - Pointer to a storage settings that will be filled with data used during init
 *
 * @return      e_eFSS_CORELL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_CORELL_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_CORELL_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_CORELL_RES_OK            - Operation ended correctly
 */
e_eFSS_CORELL_RES eFSS_CORELL_GetStorSett(t_eFSS_CORELL_Ctx* const p_ptCtx, t_eFSS_TYPE_StorSet* const p_ptStorSet);

/**
 * @brief       Get the reference of the two buffer used to read and write the storage area
 *
 * @param[in]   p_ptCtx       - Low Level Core context
 * @param[out]  p_ptBuff1     - Pointer to a pointer struct that will be filled with info about buffer 1
 * @param[out]  p_ptBuff2     - Pointer to a pointer struct that will be filled with info about buffer 2
 *
 * @return      e_eFSS_CORELL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_CORELL_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_CORELL_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_CORELL_RES_OK            - Operation ended correctly
 */
e_eFSS_CORELL_RES eFSS_CORELL_GetBuff(t_eFSS_CORELL_Ctx* const p_ptCtx, t_eFSS_CORELL_StorBuf* const p_ptBuff1,
                                      t_eFSS_CORELL_StorBuf* const p_ptBuff2);

/**
 * @brief       Get the reference of the two buffer used to read and write the storage area and get reference of storage
 *              settings.
 *
 * @param[in]   p_ptCtx       - Low Level Core context
 * @param[out]  p_ptBuff1     - Pointer to a pointer struct that will be filled with info about buffer 1
 * @param[out]  p_ptBuff2     - Pointer to a pointer struct that will be filled with info about buffer 2
 * @param[out]  p_ptStorSet   - Pointer to a storage settings that will be filled with data used during init
 *
 * @return      e_eFSS_CORELL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eFSS_CORELL_RES_CORRUPTCTX    - Context is corrupted
 *		        e_eFSS_CORELL_RES_NOINITLIB     - Need to init lib before calling function
 *              e_eFSS_CORELL_RES_OK            - Operation ended correctly
 */
e_eFSS_CORELL_RES eFSS_CORELL_GetBuffNStor(t_eFSS_CORELL_Ctx* const p_ptCtx, t_eFSS_CORELL_StorBuf* const p_ptBuff1,
                                           t_eFSS_CORELL_StorBuf* const p_ptBuff2,
                                           t_eFSS_TYPE_StorSet* const p_ptStorSet);

/**
 * @brief       Load a page from the storage area in one of the two internal buffer
 *
 * @param[in]   p_ptCtx       - Low Level Core context
 * @param[in]   p_eBuffType   - Enum used to select wich buffer we want to select
 * @param[in]   p_uPageIndx   - uint32_t index rappresenting the page that we want to load from storage
 *
 * @return      e_eFSS_CORELL_RES_BADPOINTER      - In case of bad pointer passed to the function
 *		        e_eFSS_CORELL_RES_BADPARAM        - In case of an invalid parameter passed to the function
 *		        e_eFSS_CORELL_RES_CORRUPTCTX      - Context is corrupted
 *		        e_eFSS_CORELL_RES_NOINITLIB       - Need to init lib before calling function
 *		        e_eFSS_CORELL_RES_CLBCKREADERR    - The read callback reported an error
 *              e_eFSS_CORELL_RES_CLBCKCRCERR     - The crc callback reported an error
 *              e_eFSS_CORELL_RES_NOTVALIDPAGE    - The readed page is invalid
 *              e_eFSS_CORELL_RES_NEWVERSIONFOUND - The readed page has a new version
 *              e_eFSS_CORELL_RES_OK              - Operation ended correctly
 */
e_eFSS_CORELL_RES eFSS_CORELL_LoadPageInBuff(t_eFSS_CORELL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFFTYPE p_eBuffType,
								             const uint32_t p_uPageIndx);

/**
 * @brief       Flush one of the two buffer in the storage area. Keep in mind that the other buffer will be used
 *              to check if the data was flushed corretly, and so after this operation it will contains different value
 *              from the one stored before. Only the buffer of the flushed area will be valid after this operation.
 *
 * @param[in]   p_ptCtx       - Low Level Core context
 * @param[in]   p_eBuffType   - Enum used to select wich buffer we want to select
 * @param[in]   p_uPageIndx   - uint32_t index rappresenting the page that we want to flush in storage
 *
 * @return      e_eFSS_CORELL_RES_BADPOINTER       - In case of bad pointer passed to the function
 *		        e_eFSS_CORELL_RES_BADPARAM         - In case of an invalid parameter passed to the function
 *		        e_eFSS_CORELL_RES_CORRUPTCTX       - Context is corrupted
 *		        e_eFSS_CORELL_RES_NOINITLIB        - Need to init lib before calling function
 *              e_eFSS_CORELL_RES_CLBCKCRCERR      - The crc callback reported an error
 *		        e_eFSS_CORELL_RES_CLBCKERASEERR    - The erase callback reported an error
 *		        e_eFSS_CORELL_RES_CLBCKWRITEERR    - The write callback reported an error
 *		        e_eFSS_CORELL_RES_CLBCKREADERR     - The read callback reported an error
 *		        e_eFSS_CORELL_RES_WRITENOMATCHREAD - Writen data dosent match what requested
 *              e_eFSS_CORELL_RES_OK               - Operation ended correctly
 */
e_eFSS_CORELL_RES eFSS_CORELL_FlushBuffInPage(t_eFSS_CORELL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFFTYPE p_eBuffType,
								              const uint32_t p_uPageIndx);

/**
 * @brief       Calculate the Crc of the data present in the choosen buffer. We can also choose to calculate the crc of
 *              a given numbers of bytes.
 *
 * @param[in]   p_ptCtx       - Low Level Core context
 * @param[in]   p_eBuffType   - Enum used to select wich buffer we want to select
 * @param[in]   p_uCrcSeed    - uint32_t rappresenting the seed we want to use in the calc
 * @param[in]   p_uLenCalc    - uint32_t rappresenting the lenght we want to calc
 * @param[out]  p_puCrc       - Pointer to a uint32_t variable where the CRC calculated will be placed
 *
 * @return      e_eFSS_CORELL_RES_BADPOINTER       - In case of bad pointer passed to the function
 *		        e_eFSS_CORELL_RES_BADPARAM         - In case of an invalid parameter passed to the function
 *		        e_eFSS_CORELL_RES_CORRUPTCTX       - Context is corrupted
 *		        e_eFSS_CORELL_RES_NOINITLIB        - Need to init lib before calling function
 *		        e_eFSS_CORELL_RES_CLBCKCRCERR      - The CRC callback reported an error
 *              e_eFSS_CORELL_RES_OK               - Operation ended correctly
 */
e_eFSS_CORELL_RES eFSS_CORELL_CalcCrcInBuff(t_eFSS_CORELL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFFTYPE p_eBuffType,
								            const uint32_t p_uCrcSeed, const uint32_t p_uLenCalc,
                                            uint32_t* const p_puCrc);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_CORELL_H */