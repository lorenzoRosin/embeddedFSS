/**
 * @file       eCU_DUNPK.h
 *
 * @brief      Data pack utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef ECU_DUNPK_H
#define ECU_DUNPK_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_TYPE.h"



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eCU_DUNPK_RES_OK = 0,
    e_eCU_DUNPK_RES_BADPARAM,
    e_eCU_DUNPK_RES_BADPOINTER,
	e_eCU_DUNPK_RES_CORRUPTCTX,
    e_eCU_DUNPK_RES_NODATA,
    e_eCU_DUNPK_RES_NOINITLIB,
    e_eCU_DUNPK_RES_NOINITFRAME,
}e_eCU_DUNPK_RES;

typedef struct
{
    bool_t bIsInit;
	bool_t bIsLE;
	uint8_t* puMemUPK;
	uint32_t uMemUPKL;
    uint32_t uMemUPKFrameL;
	uint32_t uMemUPKCtr;
}t_eCU_DUNPK_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the data unpacker context
 *
 * @param[in]   p_ptCtx        - Data Unpacker context
 * @param[in]   p_puMemUPK     - Pointer to a memory area that we will use to store data that needs to be unpacked
 * @param[in]   p_uMemUPKL     - Dimension in byte of the data unpacker area
 * @param[in]   p_bIsLEnd      - Select if data unpacker must work in Little Endian or Big Endian
 *
 * @return      e_eCU_DUNPK_RES_BADPOINTER - In case of bad pointer passed to the function
 *		        e_eCU_DUNPK_RES_BADPARAM   - In case of an invalid parameter passed to the function
 *              e_eCU_DUNPK_RES_OK         - Operation ended correctly
 */
e_eCU_DUNPK_RES eCU_DUNPK_InitCtx(t_eCU_DUNPK_Ctx* const p_ptCtx, uint8_t* p_puMemUPK, const uint32_t p_uMemUPKL,
                                  const bool_t p_bIsLEnd);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Data Unpacker context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eCU_DUNPK_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eCU_DUNPK_RES_OK            - Operation ended correctly
 */
e_eCU_DUNPK_RES eCU_DUNPK_IsInit(t_eCU_DUNPK_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Retrive the pointer of the buffer that the user can use to insert data payload that need to be unpacked
 *
 * @param[in]   p_ptCtx       - Data Unpacker context
 * @param[out]  p_ppuData     - Pointer to a Pointer where the raw data needs to be copied before starting a session
 * @param[out]  p_puMaxDataL  - Pointer to a uint32_t variable where the max number of data that can be copied in
 *                              p_ppuData will be placed
 *
 * @return      e_eCU_DUNPK_RES_BADPOINTER - In case of bad pointer passed to the function
 *		        e_eCU_DUNPK_RES_NOINITLIB  - Need to init the data unpacker before taking some action
 *		        e_eCU_DUNPK_RES_CORRUPTCTX - In case of a corrupted context
 *              e_eCU_DUNPK_RES_OK         - Operation ended correctly
 */
e_eCU_DUNPK_RES eCU_DUNPK_GetUPkDataLocat(t_eCU_DUNPK_Ctx* const p_ptCtx, uint8_t** p_ppuData,
                                          uint32_t* const p_puMaxDataL);

/**
 * @brief       Start to unpack a new frame given the dimension of raw payload it self. This function suppouse that
 *              data payload that need to be unpocked were already copied in memory.( see eCU_DUNPK_GetUPkDataLocat
 *              in order to know how get the data pointer )
 *
 * @param[in]   p_ptCtx         - Data Unpacker context
 * @param[in]   p_uFrameL       - lenght of the raw data present in the frame that we need to unpack
 *
 * @return      e_eCU_DUNPK_RES_BADPOINTER - In case of bad pointer passed to the function
 *		        e_eCU_DUNPK_RES_BADPARAM   - In case of an invalid parameter passed to the function
 *		        e_eCU_DUNPK_RES_NOINITLIB  - Need to init the data unpacker before taking some action
 *		        e_eCU_DUNPK_RES_CORRUPTCTX - In case of a corrupted context
 *              e_eCU_DUNPK_RES_OK         - Operation ended correctly
 */
e_eCU_DUNPK_RES eCU_DUNPK_StartNewFrame(t_eCU_DUNPK_Ctx* const p_ptCtx, const uint32_t p_uFrameL);

/**
 * @brief       Reset data unpacker status and restart from start un packing. This will not delete or start a new
 *              session. This function will simply restart the packing process of already loaded data.
 *
 * @param[in]   p_ptCtx         - Data Unpacker context
 *
 * @return      e_eCU_DUNPK_RES_BADPOINTER  - In case of bad pointer passed to the function
 *		        e_eCU_DUNPK_RES_NOINITLIB   - Need to init the data unpacker before taking some action
 *		        e_eCU_DUNPK_RES_NOINITFRAME - Need to start a frame before restarting the current frame
 *		        e_eCU_DUNPK_RES_CORRUPTCTX  - In case of a corrupted context
 *              e_eCU_DUNPK_RES_OK          - Operation ended correctly
 */
e_eCU_DUNPK_RES eCU_DUNPK_RestartCurrentUnpack(t_eCU_DUNPK_Ctx* const p_ptCtx);

/**
 * @brief       Retrive how many byte we can still pop
 *
 * @param[in]   p_ptCtx       - Data Unpacker context
 * @param[out]  p_puGettedL   - Pointer to a memory area were we will store the size of the remaining poppable data
 *
 * @return      e_eCU_DUNPK_RES_BADPOINTER  - In case of bad pointer passed to the function
 *		        e_eCU_DUNPK_RES_NOINITLIB   - Need to init the data unpacker before taking some action
 *		        e_eCU_DUNPK_RES_NOINITFRAME - Need to start a frame before restarting the current frame
 *		        e_eCU_DUNPK_RES_CORRUPTCTX  - In case of a corrupted context
 *              e_eCU_DUNPK_RES_OK          - Operation ended correctly
 */
e_eCU_DUNPK_RES eCU_DUNPK_GetRemToPop(t_eCU_DUNPK_Ctx* const p_ptCtx, uint32_t* const p_puGettedL);

/**
 * @brief       Pop some raw data
 *
 * @param[in]   p_ptCtx     - Data Unpacker context
 * @param[out]  p_puData    - Pointer to a memory area where popped data will be copied
 * @param[in]   p_puGettedL - The amount of data that need to be copied in p_puData
 *
 * @return      e_eCU_DUNPK_RES_BADPOINTER  - In case of bad pointer passed to the function
 *		        e_eCU_DUNPK_RES_NOINITLIB   - Need to init the data unpacker before taking some action
 *		        e_eCU_DUNPK_RES_BADPARAM    - In case of an invalid parameter passed to the function
 *		        e_eCU_DUNPK_RES_NOINITFRAME - Need to start a frame before restarting the current frame
 *		        e_eCU_DUNPK_RES_CORRUPTCTX  - In case of a corrupted context
 *              e_eCU_DUNPK_RES_NODATA      - Not so much data to pop
 *              e_eCU_DUNPK_RES_OK          - Operation ended correctly
 */
e_eCU_DUNPK_RES eCU_DUNPK_PopArray(t_eCU_DUNPK_Ctx* const p_ptCtx, uint8_t* p_puData, uint32_t const p_uToGetL);

/**
 * @brief       Pop one byte from data passed to session
 *
 * @param[in]   p_ptCtx       - Data Unpacker context
 * @param[out]  p_puData      - Pointer to a variable that will contain the popped data
 *
 * @return      e_eCU_DUNPK_RES_BADPOINTER  - In case of bad pointer passed to the function
 *		        e_eCU_DUNPK_RES_NOINITLIB   - Need to init the data unpacker before taking some action
 *		        e_eCU_DUNPK_RES_NOINITFRAME - Need to start a frame before restarting the current frame
 *		        e_eCU_DUNPK_RES_CORRUPTCTX  - In case of a corrupted context
 *              e_eCU_DUNPK_RES_NODATA      - Not so much data to pop
 *              e_eCU_DUNPK_RES_OK          - Operation ended correctly
 */
e_eCU_DUNPK_RES eCU_DUNPK_PopU8(t_eCU_DUNPK_Ctx* const p_ptCtx, uint8_t* p_puData);

/**
 * @brief       Pop 2 byte from data passed to session
 *
 * @param[in]   p_ptCtx        - Data Unpacker context
 * @param[out]  p_puData       - Pointer to a variable that will contain the popped data
 *
 * @return      e_eCU_DUNPK_RES_BADPOINTER  - In case of bad pointer passed to the function
 *		        e_eCU_DUNPK_RES_NOINITLIB   - Need to init the data unpacker before taking some action
 *		        e_eCU_DUNPK_RES_NOINITFRAME - Need to start a frame before restarting the current frame
 *		        e_eCU_DUNPK_RES_CORRUPTCTX  - In case of a corrupted context
 *              e_eCU_DUNPK_RES_NODATA      - Not so much data to pop
 *              e_eCU_DUNPK_RES_OK          - Operation ended correctly
 */
e_eCU_DUNPK_RES eCU_DUNPK_PopU16(t_eCU_DUNPK_Ctx* const p_ptCtx, uint16_t* p_puData);

/**
 * @brief       Pop 4 byte from data passed to session
 *
 * @param[in]   p_ptCtx        - Data Unpacker context
 * @param[out]  p_puData       - Pointer to a variable that will contain the popped data
 *
 * @return      e_eCU_DUNPK_RES_BADPOINTER  - In case of bad pointer passed to the function
 *		        e_eCU_DUNPK_RES_NOINITLIB   - Need to init the data unpacker before taking some action
 *		        e_eCU_DUNPK_RES_NOINITFRAME - Need to start a frame before restarting the current frame
 *		        e_eCU_DUNPK_RES_CORRUPTCTX  - In case of a corrupted context
 *              e_eCU_DUNPK_RES_NODATA      - Not so much data to pop
 *              e_eCU_DUNPK_RES_OK          - Operation ended correctly
 */
e_eCU_DUNPK_RES eCU_DUNPK_PopU32(t_eCU_DUNPK_Ctx* const p_ptCtx, uint32_t* p_puData);

/**
 * @brief       Pop 8 byte from data passed to session
 *
 * @param[in]   p_ptCtx        - Data Unpacker context
 * @param[out]  p_puData       - Pointer to a variable that will contain the popped data
 *
 * @return      e_eCU_DUNPK_RES_BADPOINTER  - In case of bad pointer passed to the function
 *		        e_eCU_DUNPK_RES_NOINITLIB   - Need to init the data unpacker before taking some action
 *		        e_eCU_DUNPK_RES_NOINITFRAME - Need to start a frame before restarting the current frame
 *		        e_eCU_DUNPK_RES_CORRUPTCTX  - In case of a corrupted context
 *              e_eCU_DUNPK_RES_NODATA      - Not so much data to pop
 *              e_eCU_DUNPK_RES_OK          - Operation ended correctly
 */
e_eCU_DUNPK_RES eCU_DUNPK_PopU64(t_eCU_DUNPK_Ctx* const p_ptCtx, uint64_t* p_puData);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* ECU_DUNPK_H */