/**
 * @file       eCU_CIRQ.h
 *
 * @brief      Circular Queue utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef ECU_CIRQ_H
#define ECU_CIRQ_H



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
    e_eCU_CIRQ_RES_OK = 0,
    e_eCU_CIRQ_RES_BADPARAM,
    e_eCU_CIRQ_RES_BADPOINTER,
	e_eCU_CIRQ_RES_CORRUPTCTX,
	e_eCU_CIRQ_RES_FULL,
    e_eCU_CIRQ_RES_EMPTY,
    e_eCU_CIRQ_RES_NOINITLIB,
}e_eCU_CIRQ_RES;

typedef struct
{
    bool_t   bIsInit;
    uint8_t* puBuff;
    uint32_t uBuffL;
    uint32_t uBuffUsedL;
    uint32_t uBuffFreeIdx;
}t_eCU_CIRQ_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the circular queue context
 *
 * @param[in]   p_ptCtx   - Circular queue context
 * @param[in]   p_puBuff  - Pointer to a memory buffer that we will use to manage the circular queue
 * @param[in]   p_uBuffL  - Dimension in byte of the memory buffer
 *
 * @return      e_eCU_CIRQ_RES_BADPOINTER   - In case of bad pointer passed to the function
 *              e_eCU_CIRQ_RES_BADPARAM     - In case of bad parameter passed to the function
 *              e_eCU_CIRQ_RES_OK           - Circular queue initialized successfully
 */
e_eCU_CIRQ_RES eCU_CIRQ_InitCtx(t_eCU_CIRQ_Ctx* const p_ptCtx, uint8_t* p_puBuff, const uint32_t p_uBuffL);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx         - Circular queue context
 * @param[out]  p_pbIsInit      - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eCU_CIRQ_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eCU_CIRQ_RES_OK            - Operation ended correctly
 */
e_eCU_CIRQ_RES eCU_CIRQ_IsInit(t_eCU_CIRQ_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Reset the state of the circular queue and discharge all saved data
 *
 * @param[in]   p_ptCtx         - Circular queue context
 *
 * @return      e_eCU_CIRQ_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eCU_CIRQ_RES_NOINITLIB    - Need to init the queue before taking some action
 *		        e_eCU_CIRQ_RES_CORRUPTCTX   - In case of an corrupted context
 *              e_eCU_CIRQ_RES_OK           - Circular queue resetted successfully
 */
e_eCU_CIRQ_RES eCU_CIRQ_Reset(t_eCU_CIRQ_Ctx* const p_ptCtx);

/**
 * @brief       Get free space present in the queue
 *
 * @param[in]   p_ptCtx           - Circular queue context
 * @param[out]  p_puFreeSpace     - Pointer to variable where free space present in queue will be stored
 *
 * @return      e_eCU_CIRQ_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eCU_CIRQ_RES_NOINITLIB    - Need to init the queue before taking some action
 *		        e_eCU_CIRQ_RES_CORRUPTCTX   - In case of an corrupted context
 *              e_eCU_CIRQ_RES_OK           - Operation ended successfully
 */
e_eCU_CIRQ_RES eCU_CIRQ_GetFreeSapce(t_eCU_CIRQ_Ctx* const p_ptCtx, uint32_t* const p_puFreeSpace);

/**
 * @brief       Get used space present in the queue
 *
 * @param[in]   p_ptCtx         - Circular queue context
 * @param[out]  p_puUsedSpace   - Pointer to variable where used space present in queue will be stored
 *
 * @return      e_eCU_CIRQ_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eCU_CIRQ_RES_NOINITLIB    - Need to init the queue before taking some action
 *		        e_eCU_CIRQ_RES_CORRUPTCTX   - In case of an corrupted context
 *              e_eCU_CIRQ_RES_OK           - Operation ended successfully
 */
e_eCU_CIRQ_RES eCU_CIRQ_GetOccupiedSapce(t_eCU_CIRQ_Ctx* const p_ptCtx, uint32_t* const p_puUsedSpace);

/**
 * @brief       Insert data in the queue if free space is avaiable
 *
 * @param[in]   p_ptCtx     - Circular queue context
 * @param[in]   p_puData    - Pointer to the data that we want to insert in the queue
 * @param[in]   p_uDataL    - Lenght of the data present in the pointer passed
 *
 * @return      e_eCU_CIRQ_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eCU_CIRQ_RES_NOINITLIB    - Need to init the queue before taking some action
 *              e_eCU_CIRQ_RES_BADPARAM     - In case of bad parameter passed to the function
 *		        e_eCU_CIRQ_RES_FULL         - Queue full, can't push this amount of data
 *		        e_eCU_CIRQ_RES_CORRUPTCTX   - In case of an corrupted context
 *              e_eCU_CIRQ_RES_OK           - Operation ended successfully
 */
e_eCU_CIRQ_RES eCU_CIRQ_InsertData(t_eCU_CIRQ_Ctx* const p_ptCtx, const uint8_t* p_puData, const uint32_t p_uDataL);

/**
 * @brief       Retrive data from the queue if avaiable
 *
 * @param[in]   p_ptCtx        - Circular queue context
 * @param[out]  p_puData       - Pointer to the buffer that will contain retrived data from queue
 * @param[in]   p_uDataL       - Lenght of the data that we want to retrive
 *
 * @return      e_eCU_CIRQ_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eCU_CIRQ_RES_NOINITLIB    - Need to init the queue before taking some action
 *              e_eCU_CIRQ_RES_BADPARAM     - In case of bad parameter passed to the function
 *		        e_eCU_CIRQ_RES_EMPTY        - Not so many byte present in queue
 *		        e_eCU_CIRQ_RES_CORRUPTCTX   - In case of an corrupted context
 *              e_eCU_CIRQ_RES_OK           - Operation ended successfully
 */
e_eCU_CIRQ_RES eCU_CIRQ_RetriveData(t_eCU_CIRQ_Ctx* const p_ptCtx, uint8_t* p_puData, const uint32_t p_uDataL);

/**
 * @brief       Peek data from the queue if avaiable
 *
 * @param[in]   p_ptCtx       - Circular queue context
 * @param[out]  p_puData      - Pointer to the buffer that will contain retrived data from queue
 * @param[in]   p_uDataL      - Lenght of the data that we want to peek
 *
 * @return      e_eCU_CIRQ_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eCU_CIRQ_RES_NOINITLIB    - Need to init the queue before taking some action
 *              e_eCU_CIRQ_RES_BADPARAM     - In case of bad parameter passed to the function
 *		        e_eCU_CIRQ_RES_EMPTY        - Not so many byte present in queue
 *		        e_eCU_CIRQ_RES_CORRUPTCTX   - In case of an corrupted context
 *              e_eCU_CIRQ_RES_OK           - Operation ended successfully
 */
e_eCU_CIRQ_RES eCU_CIRQ_PeekData(t_eCU_CIRQ_Ctx* const p_ptCtx, uint8_t* p_puData, const uint32_t p_uDataL);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* ECU_CIRQ_H */