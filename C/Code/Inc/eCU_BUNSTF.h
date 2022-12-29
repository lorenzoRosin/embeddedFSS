/**
 * @file       eCU_BUNSTF.h
 *
 * @brief      Byte unstuffer utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef ECU_BUNSTF_H
#define ECU_BUNSTF_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_TYPE.h"
#include "eCU_BUNSTFPRV.h"



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eCU_BUNSTF_RES_OK = 0,
    e_eCU_BUNSTF_RES_BADPARAM,
    e_eCU_BUNSTF_RES_BADPOINTER,
	e_eCU_BUNSTF_RES_CORRUPTCTX,
    e_eCU_BUNSTF_RES_OUTOFMEM,
    e_eCU_BUNSTF_RES_BADFRAME,
	e_eCU_BUNSTF_RES_FRAMEENDED,
    e_eCU_BUNSTF_RES_FRAMERESTART,
    e_eCU_BUNSTF_RES_NOINITLIB,
}e_eCU_BUNSTF_RES;

typedef struct
{
    bool_t   bIsInit;
	uint8_t* puBuff;
	uint32_t uBuffL;
	uint32_t uFrameCtr;
	e_eCU_BUNSTFPRV_SM eSM;
}t_eCU_BUNSTF_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the byte unStuffer context
 *
 * @param[in]   p_ptCtx       - Byte unStuffer context
 * @param[in]   p_puBuff      - Pointer to a memory area that we will use to store the unstuffed data
 * @param[in]   p_uBuffL      - Dimension in byte of the memory area
 *
 * @return      e_eCU_BUNSTF_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eCU_BUNSTF_RES_BADPARAM     - In case of an invalid parameter passed to the function
 *              e_eCU_BUNSTF_RES_OK           - Operation ended correctly
 */
e_eCU_BUNSTF_RES eCU_BUNSTF_InitCtx(t_eCU_BUNSTF_Ctx* const p_ptCtx, uint8_t* p_puBuff, const uint32_t p_uBuffL);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Byte unStuffer context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eCU_BUNSTF_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eCU_BUNSTF_RES_OK            - Operation ended correctly
 */
e_eCU_BUNSTF_RES eCU_BUNSTF_IsInit(t_eCU_BUNSTF_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Start receiving a new frame, loosing the previous stored unstuffed frame
 *
 * @param[in]   p_ptCtx         - Byte unStuffer context
 *
 * @return      e_eCU_BUNSTF_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eCU_BUNSTF_RES_NOINITLIB    - Need to init context before taking some action
 *		        e_eCU_BUNSTF_RES_CORRUPTCTX   - In case of an corrupted context
 *              e_eCU_BUNSTF_RES_OK           - Operation ended correctly
 */
e_eCU_BUNSTF_RES eCU_BUNSTF_NewFrame(t_eCU_BUNSTF_Ctx* const p_ptCtx);

/**
 * @brief       Retrive the pointer to the stored unstuffed data, and the data size of the frame. Keep in mind that
 *              the frame parsing could be ongoing, and that value could change.
 *
 * @param[in]   p_ptCtx       - Byte unStuffer context
 * @param[out]  p_ppuData     - Pointer to a Pointer pointing to the unstuffed data frame
 * @param[out]  p_puL         - Pointer to a uint32_t variable where the size of the unstuffed data will be placed
 *
 * @return      e_eCU_BUNSTF_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eCU_BUNSTF_RES_NOINITLIB    - Need to init context before taking some action
 *		        e_eCU_BUNSTF_RES_CORRUPTCTX   - In case of an corrupted context
 *              e_eCU_BUNSTF_RES_OK           - Operation ended correctly
 */
e_eCU_BUNSTF_RES eCU_BUNSTF_GetUnstufData(t_eCU_BUNSTF_Ctx* const p_ptCtx, uint8_t** p_ppuData, uint32_t* const p_puL);

/**
 * @brief       Retrive the current numbers of unstuffed data received. Keep in mind that the frame parsing could be
 *              ongoing, and that value could change.
 *
 * @param[in]   p_ptCtx       - Byte unStuffer context
 * @param[out]  p_puL         - Pointer to a uint32_t variable where the size of the unstuffed data will be placed
 *
 * @return      e_eCU_BUNSTF_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eCU_BUNSTF_RES_NOINITLIB    - Need to init context before taking some action
 *		        e_eCU_BUNSTF_RES_CORRUPTCTX   - In case of an corrupted context
 *              e_eCU_BUNSTF_RES_OK           - Operation ended correctly
 */
e_eCU_BUNSTF_RES eCU_BUNSTF_GetUnstufLen(t_eCU_BUNSTF_Ctx* const p_ptCtx, uint32_t* const p_puL);

/**
 * @brief       Retrive if the Unstuffer is currently waiting for the Start of frame in order to start saving data.
 *
 * @param[in]   p_ptCtx       - Byte unStuffer context
 * @param[out]  p_pbIsWaitSof - Pointer to a bool_t variable that will be filled with true if we are waiting SOF
 *
 * @return      e_eCU_BUNSTF_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eCU_BUNSTF_RES_NOINITLIB    - Need to init context before taking some action
 *		        e_eCU_BUNSTF_RES_CORRUPTCTX   - In case of an corrupted context
 *              e_eCU_BUNSTF_RES_OK           - Operation ended correctly
 */
e_eCU_BUNSTF_RES eCU_BUNSTF_IsWaitingSof(const t_eCU_BUNSTF_Ctx* p_ptCtx, bool_t* const p_pbIsWaitSof);

/**
 * @brief       Check if the current frame is finished or if we need to unstuff some more data to have the full frame.
 *              If a frame is received with a bad format this function return that the frame is not fully unstuffed.
 *
 * @param[in]   p_ptCtx            - Byte unStuffer context
 * @param[out]  p_pbIsFrameUnstuff - Pointer to a bool_t variable where we will store if the frame parsing is ongoing
 *
 * @return      e_eCU_BUNSTF_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eCU_BUNSTF_RES_NOINITLIB    - Need to init context before taking some action
 *		        e_eCU_BUNSTF_RES_CORRUPTCTX   - In case of an corrupted context
 *              e_eCU_BUNSTF_RES_OK           - Operation ended correctly
 */
e_eCU_BUNSTF_RES eCU_BUNSTF_IsAFullFrameUnstuff(const t_eCU_BUNSTF_Ctx* p_ptCtx, bool_t* const p_pbIsFrameUnstuff);

/**
 * @brief       Check if the current received data compose a bad frame. If a bad frame is detected we can only
 *              call eCU_BUNSTF_NewFrame before parsing new data.
 *
 * @param[in]   p_ptCtx            - Byte unStuffer context
 * @param[out]  p_pbIsFrameBad     - Pointer to a bool_t variable where we will store if the frame is bad formed
 *
 * @return      e_eCU_BUNSTF_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eCU_BUNSTF_RES_NOINITLIB    - Need to init context before taking some action
 *		        e_eCU_BUNSTF_RES_CORRUPTCTX   - In case of an corrupted context
 *              e_eCU_BUNSTF_RES_OK           - Operation ended correctly
 */
e_eCU_BUNSTF_RES eCU_BUNSTF_IsFrameBad(const t_eCU_BUNSTF_Ctx* p_ptCtx, bool_t* const p_pbIsFrameBad);

/**
 * @brief       Insert the stuffed data chunk that the alg will unstuff byte per byte
 *
 * @param[in]   p_ptCtx           - Byte unStuffer context
 * @param[in]   p_puStuffBuf      - Pointer to the stuffed Data that we will unstuff
 * @param[in]   p_uStuffBufL      - Size of the p_puStuffBuf
 * @param[out]  p_puConsumedBufL  - Pointer to an uint32_t were we will store how many stuffed data byte has been
 *                                  analized. Keep in mind that unalized data were not unstuffed and they will need
 *                                  to be reparsed. Un parsed data happens when the frame ended earlier
 *                                  ( e_eCU_BUNSTF_RES_FRAMEENDED, e_eCU_BUNSTF_RES_BADFRAME or
 *                                  e_eCU_BUNSTF_RES_FRAMERESTART is returned ) or when some other error is returned.
 *                                  When the function return e_eCU_BUNSTF_RES_OK p_puConsumedBufL will always be
 *                                  returned has p_uStuffBufL.
 *
 * @return      e_eCU_BUNSTF_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eCU_BUNSTF_RES_NOINITLIB    - Need to init context before taking some action
 *		        e_eCU_BUNSTF_RES_BADPARAM     - In case of an invalid parameter passed to the function
 *		        e_eCU_BUNSTF_RES_CORRUPTCTX   - In case of an corrupted context
 *              e_eCU_BUNSTF_RES_OUTOFMEM     - Can not unstuff data, initial mem pointer was too small. The only way to
 *                                              resolve the issue is increasing the size of the memory area passed to
 *                                              init.
 *		        e_eCU_BUNSTF_RES_FRAMEENDED   - Frame ended, restart context in order to parse a new frame. Every other
 *                                              call to this function will not have effect until we call
 *                                              eCU_BUNSTF_NewFrame. In this situation bear in mind that some data
 *                                              could be left out the parsing, and so we need to reparse that data after
 *                                              calling eCU_BUNSTF_NewFrame.
 *              e_eCU_BUNSTF_RES_BADFRAME     - Found an error while parsing, the frame passed is invalid.
 *                                              Restart context in order to parse a new frame. Every other call
 *                                              to this function will not have effect until we call
 *                                              eCU_BUNSTF_NewFrame. In this situation bear in mind that some data
 *                                              could be left out the parsing, and so we need to reparse that data after
 *                                              calling eCU_BUNSTF_NewFrame.
 *              e_eCU_BUNSTF_RES_FRAMERESTART - During frame receiving another start of frame is received. In this
 *                                              situation clear old data and restart the frame, witouth the need to call
 *                                              any other function. In this situation bear in mind that some data could
 *                                              be left out the parsing and so we need to reparse that data with another
 *                                              call of BUNSTF_InsStufChunk.
 *              e_eCU_BUNSTF_RES_OK           - Operation ended correctly. The chunk is parsed correclty but the frame
 *                                              is not finished yet. In this situation p_puConsumedBufL is always
 *                                              reported with a value equals to p_uStuffBufL.
 */
e_eCU_BUNSTF_RES eCU_BUNSTF_InsStufChunk(t_eCU_BUNSTF_Ctx* const p_ptCtx, const uint8_t* p_puStuffBuf,
                                         const uint32_t p_uStuffBufL, uint32_t* const p_puConsumedBufL);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* ECU_BUNSTF_H */