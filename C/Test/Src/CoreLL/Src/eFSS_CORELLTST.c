/**
 * @file       eFSS_CORELLTST.c
 *
 * @brief      Low level core module test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_CORELLTST.h"
#include "eFSS_CORELL.h"
#include <stdio.h>



/***********************************************************************************************************************
 *   PRIVATE TEST STRUCT DECLARATION
 **********************************************************************************************************************/
struct t_eFSS_TYPE_EraseCtxUser
{
    e_eFSS_CORELL_RES eLastEr;
};

struct t_eFSS_TYPE_WriteCtxUser
{
    e_eFSS_CORELL_RES eLastEr;
};

struct t_eFSS_TYPE_ReadCtxUser
{
    e_eFSS_CORELL_RES eLastEr;
};

struct t_eFSS_TYPE_CrcCtxUser
{
    e_eFSS_CORELL_RES eLastEr;
};



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_CORELLTST_EraseAdapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase);

static bool_t eFSS_CORELLTST_WriteAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                        const uint32_t p_uPageToWrite, const uint8_t* p_puDataToWrite,
                                        const uint32_t p_uDataToWriteL );

static bool_t eFSS_CORELLTST_ReadAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                       const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                       const uint32_t p_uReadBufferL );

static bool_t eFSS_CORELLTST_CrcAdapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
                                      const uint8_t* p_puData, const uint32_t p_uDataL,
                                      uint32_t* const p_puCrc32Val );

/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void eFSS_CORELLTST_BadPointer(void);


#if 0
static void eFSP_MSGDTST_BadInit(void);
static void eFSP_MSGDTST_BadParamEntr(void);
static void eFSP_MSGDTST_CorruptedCtx(void);
static void eFSP_MSGDTST_BadClBck(void);
static void eFSP_MSGDTST_MsgEnd(void);
static void eFSP_MSGDTST_FrameRestart(void);
static void eFSP_MSGDTST_BadFrame(void);
static void eFSP_MSGDTST_OutOfMem();
static void eFSP_MSGDTST_General(void);
static void eFSP_MSGDTST_General2(void);
static void eFSP_MSGDTST_CorernerMulti(void);
static void eFSP_MSGDTST_ErrorAndContinue(void);
static void eFSP_MSGDTST_ErrorAndContinueEx(void);
static void eFSP_MSGDTST_ErrorShortFrame(void);
static void eFSP_MSGDTST_ErrorBadStuff(void);
static void eFSP_MSGDTST_Corner(void);
#endif


/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eFSS_CORELLTST_ExeTest(void)
{
	(void)printf("\n\nCORE LOW LEVEL TEST START \n\n");

    eFSS_CORELLTST_BadPointer();

    (void)printf("\n\nCORE LOW LEVEL TEST END \n\n");
}


/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_CORELLTST_EraseAdapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase)
{

}

static bool_t eFSS_CORELLTST_WriteAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                        const uint32_t p_uPageToWrite, const uint8_t* p_puDataToWrite,
                                        const uint32_t p_uDataToWriteL )
{

}

static bool_t eFSS_CORELLTST_ReadAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                       const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                       const uint32_t p_uReadBufferL )
{

}

static bool_t eFSS_CORELLTST_CrcAdapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
                                      const uint8_t* p_puData, const uint32_t p_uDataL,
                                      uint32_t* const p_puCrc32Val )
{

}



/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void eFSS_CORELLTST_BadPointer(void)
{
    /* Local variable */
    t_eFSS_CORELL_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_uStorType;
    uint8_t l_auStor[24u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tGetStorSet;
    t_eFSS_CORELL_StorBuf l_ltUseBuff1;
    t_eFSS_CORELL_StorBuf l_ltUseBuff2;
    uint32_t l_uCrcGetted;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_CORELLTST_EraseAdapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_CORELLTST_WriteAdapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_CORELLTST_ReadAdapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_CORELLTST_CrcAdapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 1u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 2u;
    l_tStorSet.uPageVersion = 1u;

    l_uStorType = 1u;

    /* Function */
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_InitCtx(NULL, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, NULL, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 2  -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.ptCtxErase = NULL;
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 3  -- OK \n");
        l_tCtxCb.ptCtxErase = &l_tCtxErase;
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 3  -- FAIL \n");
        l_tCtxCb.ptCtxErase = &l_tCtxErase;
    }

    /* Function */
    l_tCtxCb.fErase = NULL;
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 4  -- OK \n");
        l_tCtxCb.fErase = &eFSS_CORELLTST_EraseAdapt;
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 4  -- FAIL \n");
        l_tCtxCb.fErase = &eFSS_CORELLTST_EraseAdapt;
    }

    /* Function */
    l_tCtxCb.ptCtxWrite = NULL;
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 5  -- OK \n");
        l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 5  -- FAIL \n");
        l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    }

    /* Function */
    l_tCtxCb.fWrite = NULL;
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 6  -- OK \n");
        l_tCtxCb.fWrite = &eFSS_CORELLTST_WriteAdapt;
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 6  -- FAIL \n");
        l_tCtxCb.fWrite = &eFSS_CORELLTST_WriteAdapt;
    }

    /* Function */
    l_tCtxCb.ptCtxRead = NULL;
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 7  -- OK \n");
        l_tCtxCb.ptCtxRead = &l_tCtxRead;
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 7  -- FAIL \n");
        l_tCtxCb.ptCtxRead = &l_tCtxRead;
    }

    /* Function */
    l_tCtxCb.fRead = NULL;
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 8  -- OK \n");
        l_tCtxCb.fRead = &eFSS_CORELLTST_ReadAdapt;
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 8  -- FAIL \n");
        l_tCtxCb.fRead = &eFSS_CORELLTST_ReadAdapt;
    }

    /* Function */
    l_tCtxCb.ptCtxCrc32 = NULL;
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 9  -- OK \n");
        l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 9  -- FAIL \n");
        l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    }

    /* Function */
    l_tCtxCb.fCrc32 = NULL;
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 10 -- OK \n");
        l_tCtxCb.fCrc32 = &eFSS_CORELLTST_CrcAdapt;
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 10 -- FAIL \n");
        l_tCtxCb.fCrc32 = &eFSS_CORELLTST_CrcAdapt;
    }

    /* Function */
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_IsInit(NULL, &l_bIsInit) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 11 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_IsInit(&l_tCtx, NULL) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 12 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_GetStorSett(NULL, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 13 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_GetStorSett(&l_tCtx, NULL) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 14 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_GetBuff(NULL, &l_ltUseBuff1, &l_ltUseBuff2) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 15 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_GetBuff(&l_tCtx, NULL, &l_ltUseBuff2) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 16 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_GetBuff(&l_tCtx, &l_ltUseBuff1, NULL) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 17 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_GetBuffNStor(NULL, &l_ltUseBuff1, &l_ltUseBuff2, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 18 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_GetBuffNStor(&l_tCtx, NULL, &l_ltUseBuff2, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 19 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_GetBuffNStor(&l_tCtx, &l_ltUseBuff1, NULL, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 20 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_GetBuffNStor(&l_tCtx, &l_ltUseBuff1, &l_ltUseBuff2, NULL) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 21 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_LoadPageInBuff(NULL, e_eFSS_CORELL_BUFFTYPE_1, 0u) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 22 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_FlushBuffInPage(NULL, e_eFSS_CORELL_BUFFTYPE_1, 0u) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 23 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_CalcCrcInBuff(NULL, e_eFSS_CORELL_BUFFTYPE_1, 0u, 1u, &l_uCrcGetted) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 24 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_BADPOINTER == eFSS_CORELL_CalcCrcInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u, 1u, NULL) )
    {
        (void)printf("eFSS_CORELLTST_BadPointer 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadPointer 25 -- FAIL \n");
    }
}

#if 0

void eFSP_MSGDTST_BadInit(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx l_tCtx;
    uint8_t  l_auMemArea[10u];
    uint32_t l_uVar32;
    uint8_t* l_puData;
    bool_t l_bIsMsgDec;
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    l_tCtxAdapterCrc.eLastEr = e_eCU_CRC_RES_OK;
    bool_t l_bIsInit;

    /* Function */
    l_tCtx.tBUNSTFCtx.bIsInit = false;
    l_tCtx.ptCrcCtx = &l_tCtxAdapterCrc;
    l_tCtx.fCrc = l_fCrcPTest;

    if( e_eFSP_MSGD_RES_NOINITLIB == eFSP_MSGD_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGDTST_BadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadInit 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_NOINITLIB == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puData, &l_uVar32) )
    {
        (void)printf("eFSP_MSGDTST_BadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadInit 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_NOINITLIB == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uVar32) )
    {
        (void)printf("eFSP_MSGDTST_BadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadInit 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_NOINITLIB == eFSP_MSGD_IsWaitingSof(&l_tCtx, &l_bIsMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_BadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadInit 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_NOINITLIB == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_BadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadInit 5  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_NOINITLIB == eFSP_MSGD_IsFrameBad(&l_tCtx, &l_bIsMsgDec) )
    {
        if( e_eCU_CRC_RES_OK == l_tCtxAdapterCrc.eLastEr )
        {
            (void)printf("eFSP_MSGDTST_BadInit 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_BadInit 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadInit 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_NOINITLIB == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uVar32) )
    {
        (void)printf("eFSP_MSGDTST_BadInit 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadInit 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_NOINITLIB == eFSP_MSGD_InsEncChunk(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), &l_uVar32) )
    {
        (void)printf("eFSP_MSGDTST_BadInit 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadInit 8  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsInit( &l_tCtx, &l_bIsInit ) )
    {
        if( false == l_bIsInit )
        {
            (void)printf("eFSP_MSGDTST_BadInit 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_BadInit 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadInit 9  -- FAIL \n");
    }
}

void eFSP_MSGDTST_BadParamEntr(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx l_tCtx;
    uint8_t  l_auMemArea[10u];
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    uint32_t l_uVar32;
    bool_t l_bIsInit;

    /* Function */
    if( e_eFSP_MSGD_RES_BADPARAM == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, 8u, l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_BadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadParamEntr 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, 9u, l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_BadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadParamEntr 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_BADPARAM == eFSP_MSGD_InsEncChunk(&l_tCtx, l_auMemArea, 0u, &l_uVar32) )
    {
        (void)printf("eFSP_MSGDTST_BadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadParamEntr 3  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsInit( &l_tCtx, &l_bIsInit ) )
    {
        if( true == l_bIsInit )
        {
            (void)printf("eFSP_MSGDTST_BadParamEntr 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_BadParamEntr 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadParamEntr 4  -- FAIL \n");
    }
}

void eFSP_MSGDTST_CorruptedCtx(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx l_tCtx;
    uint8_t  l_auMemArea[10u];
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    uint32_t l_uVar32;
    uint8_t* l_puData;
    bool_t l_bIsMsgDec;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, 9u, l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 1  -- FAIL \n");
    }

    l_tCtx.fCrc = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, 9u, l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 3  -- FAIL \n");
    }

    l_tCtx.ptCrcCtx = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, 9u, l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 5  -- FAIL \n");
    }

    l_tCtx.ptCrcCtx = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puData, &l_uVar32) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, 9u, l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 7  -- FAIL \n");
    }

    l_tCtx.ptCrcCtx = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_GetDecodedLen(&l_tCtx,&l_uVar32) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 8  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, 9u, l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 9  -- FAIL \n");
    }

    l_tCtx.ptCrcCtx = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_IsWaitingSof(&l_tCtx, &l_bIsMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 10 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, 9u, l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 11 -- FAIL \n");
    }

    l_tCtx.ptCrcCtx = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 12 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, 9u, l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 13 -- FAIL \n");
    }

    l_tCtx.ptCrcCtx = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_IsFrameBad(&l_tCtx, &l_bIsMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 14 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, 9u, l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 15 -- FAIL \n");
    }

    l_tCtx.ptCrcCtx = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uVar32) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 16 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, 9u, l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 17 -- FAIL \n");
    }

    l_tCtx.ptCrcCtx = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_InsEncChunk(&l_tCtx, l_auMemArea, 10u, &l_uVar32) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 18 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, 9u, l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 19 -- FAIL \n");
    }

    l_tCtx.tBUNSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 20 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, 9u, l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 21 -- FAIL \n");
    }

    l_tCtx.tBUNSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puData, &l_uVar32) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 22 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, 9u, l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 23 -- FAIL \n");
    }

    l_tCtx.tBUNSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uVar32) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 24 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, 9u, l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 25 -- FAIL \n");
    }

    l_tCtx.tBUNSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_IsWaitingSof(&l_tCtx, &l_bIsMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 26 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, 9u, l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 27 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 27 -- FAIL \n");
    }

    l_tCtx.tBUNSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 28 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 28 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, 9u, l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 29 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 29 -- FAIL \n");
    }

    l_tCtx.tBUNSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_IsFrameBad(&l_tCtx, &l_bIsMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 30 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 30 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, 9u, l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 31 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 31 -- FAIL \n");
    }

    l_tCtx.tBUNSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uVar32) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 32 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 32 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, 9u, l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 33 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 33 -- FAIL \n");
    }

    l_tCtx.tBUNSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_InsEncChunk(&l_tCtx, l_auMemArea, 10u, &l_uVar32) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 34 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 34 -- FAIL \n");
    }
}

void eFSP_MSGDTST_BadClBck(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx l_tCtx;
    uint8_t  l_auMemArea[10u];
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGDTST_c32SAdaptEr;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    uint32_t l_uVar32;
    bool_t l_bIsMsgDec;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_BadClBck 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadClBck 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGDTST_BadClBck 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadClBck 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0x00u, 0x00u, 0x00u, 0x00u, 0x01, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( e_eFSP_MSGD_RES_CRCCLBKERROR == eFSP_MSGD_InsEncChunk(&l_tCtx, testData, sizeof(testData), &l_uVar32) )
    {
        if( e_eCU_CRC_RES_BADPOINTER == l_tCtxAdapterCrc.eLastEr )
        {
            (void)printf("eFSP_MSGDTST_BadClBck 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_BadClBck 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadClBck 3  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_CRCCLBKERROR == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_BadClBck 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadClBck 4  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_CRCCLBKERROR == eFSP_MSGD_IsFrameBad(&l_tCtx, &l_bIsMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_BadClBck 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadClBck 5  -- FAIL \n");
    }
}

void eFSP_MSGDTST_MsgEnd(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx l_tCtx;
    uint8_t  l_auMemArea[10u];
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    uint32_t l_uVar32;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_MsgEnd 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_MsgEnd 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGDTST_MsgEnd 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_MsgEnd 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0xA6u, 0xC1u, 0xDCu, 0x0Au, 0x01, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( e_eFSP_MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&l_tCtx, testData, sizeof(testData), &l_uVar32) )
    {
        (void)printf("eFSP_MSGDTST_MsgEnd 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_MsgEnd 3  -- FAIL \n");
    }
}

void eFSP_MSGDTST_OutOfMem(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx l_tCtx;
    uint8_t  l_auMemArea[9u];
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    uint32_t l_uVar32;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_OutOfMem 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_OutOfMem 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGDTST_OutOfMem 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_OutOfMem 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0xE9u, 0x7Au, 0xF2u, 0xDAu, 0x02, 0x00u, 0x00u, 0x00u, 0x01, 0x01, ECU_EOF};

    if( e_eFSP_MSGD_RES_OUTOFMEM == eFSP_MSGD_InsEncChunk(&l_tCtx, testData, sizeof(testData), &l_uVar32) )
    {
        (void)printf("eFSP_MSGDTST_OutOfMem 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_OutOfMem 3  -- FAIL \n");
    }
}

void eFSP_MSGDTST_BadFrame(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx l_tCtx;
    uint8_t  l_auMemArea[10u];
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    uint32_t l_uVar32;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_BadFrame 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadFrame 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGDTST_BadFrame 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadFrame 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0x00u, 0x00u, 0x00u, 0x00u, 0x01, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&l_tCtx, testData, sizeof(testData), &l_uVar32) )
    {
        (void)printf("eFSP_MSGDTST_BadFrame 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadFrame 3  -- FAIL \n");
    }
}

void eFSP_MSGDTST_FrameRestart(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx l_tCtx;
    uint8_t  l_auMemArea[10u];
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    uint32_t l_uVar32;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_FrameRestart 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_FrameRestart 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGDTST_FrameRestart 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_FrameRestart 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0x00u, ECU_SOF, 0x00u, 0x00u, 0x01, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( e_eFSP_MSGD_RES_FRAMERESTART == eFSP_MSGD_InsEncChunk(&l_tCtx, testData, sizeof(testData), &l_uVar32) )
    {
        (void)printf("eFSP_MSGDTST_FrameRestart 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_FrameRestart 3  -- FAIL \n");
    }
}

void eFSP_MSGDTST_General(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx l_tCtx;
    uint8_t  l_auMemArea[40u];
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    uint32_t l_uConsumed;
    uint32_t l_uMostEfficient;
    uint32_t l_uPayLoadLen;
    uint8_t* l_puPayLoadLoc;
    bool_t l_bIsMsgDec;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_General 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGDTST_General 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 2  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 3  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 4  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&l_tCtx, &l_bIsMsgDec) )
    {
        if( true == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 5  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 6  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 7  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 9u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_General 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 8  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = {ECU_SOF, 0x80u, 0xE4u, 0xB1u, 0x84u, 0x02, 0x00u, 0x00u, 0x00u, ECU_ESC, (uint8_t)(~ECU_SOF),
                          ECU_ESC, (uint8_t)(~ECU_EOF), ECU_EOF};

    if( e_eFSP_MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&l_tCtx, testData, sizeof(testData), &l_uConsumed) )
    {
        if( 14u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_General 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 9  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 0u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_General 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 10 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 11 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( true == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 12 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 13 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x02u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 14 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x02u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 15 -- FAIL \n");
    }

    if( ( ECU_SOF == l_puPayLoadLoc[0u] ) && ( ECU_EOF == l_puPayLoadLoc[1u] ) )
    {
        (void)printf("eFSP_MSGDTST_General 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 16 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&l_tCtx, testData, sizeof(testData), &l_uConsumed) )
    {
        if( 0u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_General 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 17 -- FAIL \n");
    }
}

void eFSP_MSGDTST_General2(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx l_tCtx;
    uint8_t  l_auMemArea[40u];
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    uint32_t l_uConsumed;
    uint32_t l_uMostEfficient;
    uint32_t l_uPayLoadLen;
    uint8_t* l_puPayLoadLoc;
    bool_t l_bIsMsgDec;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_General2 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGDTST_General2 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 2  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 3  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 4  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&l_tCtx, &l_bIsMsgDec) )
    {
        if( true == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 5  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 6  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 7  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 9u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_General2 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 8  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = {ECU_SOF, 0x80u, 0xE4u, 0xB1u, 0x84u, 0x02, 0x00u, 0x00u, 0x00u, ECU_ESC, (uint8_t)(~ECU_SOF),
                          ECU_ESC, (uint8_t)(~ECU_EOF), ECU_EOF};

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&l_tCtx, testData, 1u, &l_uConsumed) )
    {
        if( 1u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_General2 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 9  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 8u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_General2 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 10 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 11 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 12 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 13 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 14 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 15 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[1], 4u, &l_uConsumed) )
    {
        if( 4u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_General2 16 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 16 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 16 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 4u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_General2 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 17 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 18 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 18 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 19 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 20 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 20 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 21 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 21 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 22 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[5], 4u, &l_uConsumed) )
    {
        if( 4u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_General2 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 23 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 3u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_General2 24 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 24 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 24 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 25 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 25 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 26 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 26 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 26 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 27 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 27 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 27 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 28 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 28 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 28 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 29 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 29 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 29 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[9], 3u, &l_uConsumed) )
    {
        if( 3u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_General2 30 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 30 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 30 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 2u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_General2 31 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 31 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 31 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 32 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 32 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 32 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 33 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 33 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 33 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 34 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 34 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 34 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x01u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 35 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 35 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 35 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x01u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 36 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 36 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 36 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[12], 2u, &l_uConsumed) )
    {
        if( 2u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_General2 37 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 37 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 37 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 0u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_General2 38 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 38 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 38 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 39 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 39 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 39 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( true == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 40 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 40 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 40 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 41 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 41 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 41 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x02u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 42 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 42 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 42 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x02u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 43 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 43 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 43 -- FAIL \n");
    }
}

void eFSP_MSGDTST_CorernerMulti(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx l_tCtx;
    uint8_t  l_auMemArea[40u];
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    uint32_t l_uConsumed;
    uint32_t l_uMostEfficient;
    uint32_t l_uPayLoadLen;
    uint8_t* l_puPayLoadLoc;
    bool_t l_bIsMsgDec;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 2  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 3  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 4  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 5  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 9u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = {ECU_SOF, 0x73u, 0x9Fu, 0x52u, 0xD9u, 0x07, 0x00u, 0x00u, 0x00u, ECU_ESC, (uint8_t)(~ECU_SOF),
                          ECU_ESC, (uint8_t)(~ECU_EOF), 0x01, 0x02, 0x03, 0x04, 0x05, ECU_EOF};

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&l_tCtx, testData, 4u, &l_uConsumed) )
    {
        if( 4u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 7  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 5u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 8  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 9  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 10 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 11 -- FAIL \n");
    }

    l_uConsumed = 0u;
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[4u], 4u, &l_uConsumed) )
    {
        if( 4u == l_uConsumed)
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 12 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 1u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 13 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 14 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 15 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 16 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 16 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 16 -- FAIL \n");
    }

    l_uConsumed = 0u;
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[8u], 2u, &l_uConsumed) )
    {
        if( 2u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 17 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 8u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 18 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 18 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 19 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 20 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 20 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 21 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 21 -- FAIL \n");
    }

    l_uConsumed = 0u;
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[10u], 4u, &l_uConsumed) )
    {
        if( 4u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 22 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 5u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 23 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 24 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 24 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 24 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x03u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 25 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 25 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x03u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 26 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 26 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 26 -- FAIL \n");
    }

    l_uConsumed = 0u;
    if( e_eFSP_MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[14u], 10u, &l_uConsumed) )
    {
        if( 5u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 27 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 27 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 27 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 0u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 28 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 28 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 28 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( true == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 29 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 29 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 29 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x07u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 30 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 30 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 30 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x07u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 31 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 31 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 31 -- FAIL \n");
    }

    if( ( ECU_SOF == l_puPayLoadLoc[0u] ) && ( ECU_EOF == l_puPayLoadLoc[1u] ) && ( 0x01u == l_puPayLoadLoc[2u] ) &&
        ( 0x02u == l_puPayLoadLoc[3u] ) && ( 0x03u == l_puPayLoadLoc[4u] ) && ( 0x04u == l_puPayLoadLoc[5u] ) &&
        ( 0x05u == l_puPayLoadLoc[6u] ) )
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 32 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 32 -- FAIL \n");
    }
}

void eFSP_MSGDTST_ErrorAndContinue(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx l_tCtx;
    uint8_t  l_auMemArea[40u];
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    uint32_t l_uConsumed;
    uint32_t l_uMostEfficient;
    uint32_t l_uPayLoadLen;
    uint8_t* l_puPayLoadLoc;
    bool_t l_bIsMsgDec;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 2  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 3  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 4  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 5  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 9u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = { ECU_SOF, ECU_SOF, 0xA0u, 0xE6u, 0xDCu, 0x0Du, 0x02, 0x00u, 0x00u, 0x00u, ECU_ESC,
                           (uint8_t)(~ECU_SOF), ECU_ESC, (uint8_t)(~ECU_EOF), 0x01, ECU_EOF};

    if( e_eFSP_MSGD_RES_FRAMERESTART == eFSP_MSGD_InsEncChunk(&l_tCtx, testData, sizeof(testData), &l_uConsumed) )
    {
        if( 2u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 7  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[2u], 12u, &l_uConsumed) )
    {
        if( 12u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 8  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 1u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 9  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 10 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 11 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 12 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x02u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 13 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x02u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 14 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[14u], 1u, &l_uConsumed) )
    {
        if( 1u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 15 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 0u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 17 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 18 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 18 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 19 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&l_tCtx, &l_bIsMsgDec) )
    {
        if( true == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 20 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 20 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x03u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 21 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 21 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x03u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 22 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[14u], 1u, &l_uConsumed) )
    {
        if( 0u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 23 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 24 -- FAIL \n");
    }

    /* Remove */
    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[1u], sizeof(testData) - 1u, &l_uConsumed) )
    {
        if( (sizeof(testData) - 1u) == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 25 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 25 -- FAIL \n");
    }
}

void eFSP_MSGDTST_ErrorAndContinueEx(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx l_tCtx;
    uint8_t  l_auMemArea[40u];
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    uint32_t l_uConsumed;
    uint32_t l_uMostEfficient;
    uint32_t l_uPayLoadLen;
    uint8_t* l_puPayLoadLoc;
    bool_t l_bIsMsgDec;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 2  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 3  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 4  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 5  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 9u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = { ECU_SOF, ECU_SOF, 0xAAu, 0xAAu, 0xAAu, 0xAAu, 0x02, 0x00u, 0x00u, 0x00u, ECU_ESC,
                           (uint8_t)(~ECU_SOF), ECU_ESC, (uint8_t)(~ECU_EOF), ECU_EOF};

    if( e_eFSP_MSGD_RES_FRAMERESTART == eFSP_MSGD_InsEncChunk(&l_tCtx, testData, sizeof(testData), &l_uConsumed) )
    {
        if( 2u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 7  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[2u], 12u, &l_uConsumed) )
    {
        if( 12u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 8  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 1u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 9  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 10 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 11 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 12 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x02u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 13 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x02u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 14 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[14u], 1u, &l_uConsumed) )
    {
        if( 1u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 15 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 0u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 17 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 18 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 18 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 19 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&l_tCtx, &l_bIsMsgDec) )
    {
        if( true == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 20 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 20 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x02u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 21 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 21 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x02u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 22 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[14u], 1u, &l_uConsumed) )
    {
        if( 0u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 23 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 24 -- FAIL \n");
    }

    /* Remove */
    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[1u], sizeof(testData) - 1u, &l_uConsumed) )
    {
        if( (sizeof(testData) - 1u) == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 25 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 25 -- FAIL \n");
    }
}

void eFSP_MSGDTST_ErrorShortFrame(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx l_tCtx;
    uint8_t  l_auMemArea[40u];
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    uint32_t l_uConsumed;
    uint32_t l_uMostEfficient;
    uint32_t l_uPayLoadLen;
    uint8_t* l_puPayLoadLoc;
    bool_t l_bIsMsgDec;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 2  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 3  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 4  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 5  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 9u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = { ECU_SOF, ECU_SOF, 0xAAu, 0xAAu, 0xAAu, 0xAAu, 0x04, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( e_eFSP_MSGD_RES_FRAMERESTART == eFSP_MSGD_InsEncChunk(&l_tCtx, testData, sizeof(testData), &l_uConsumed) )
    {
        if( 2u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 7  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[2u], 9u, &l_uConsumed) )
    {
        if( 9u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 8  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 4u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 9  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 10 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 11 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 12 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x01u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 13 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x01u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 14 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[11u], 1u, &l_uConsumed) )
    {
        if( 1u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 15 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 0u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 17 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 18 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 18 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 19 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&l_tCtx, &l_bIsMsgDec) )
    {
        if( true == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 20 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 20 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x01u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 21 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 21 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x01u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 22 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[11u], 1u, &l_uConsumed) )
    {
        if( 0u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 23 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 24 -- FAIL \n");
    }

    /* Remove */
    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[1u], sizeof(testData) - 1u, &l_uConsumed) )
    {
        if( (sizeof(testData) - 1u) == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 25 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 25 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 26 -- FAIL \n");
    }

    uint8_t testData2[] = { ECU_SOF, 0xAAu, 0xAAu, ECU_EOF};

    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&l_tCtx, testData2, sizeof(testData2), &l_uConsumed) )
    {
        if( 4u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 27 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 27 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 27 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&l_tCtx, testData2, sizeof(testData2), &l_uConsumed) )
    {
        if( 0u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 28 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 28 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 28 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 0u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 29 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 29 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 29 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 30 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 30 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 30 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 31 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 31 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 31 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&l_tCtx, &l_bIsMsgDec) )
    {
        if( true == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 32 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 32 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 32 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 33 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 33 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 33 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 34 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 34 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 34 -- FAIL \n");
    }
}

void eFSP_MSGDTST_ErrorBadStuff(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx l_tCtx;
    uint8_t  l_auMemArea[40u];
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    uint32_t l_uConsumed;
    uint32_t l_uMostEfficient;
    uint32_t l_uPayLoadLen;
    uint8_t* l_puPayLoadLoc;
    bool_t l_bIsMsgDec;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 2  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 3  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x00u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 4  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 5  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 9u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = { ECU_SOF, ECU_SOF, 0xAAu, 0xAAu, 0xAAu, 0xAAu, 0x04, 0x00u, 0x00u, 0x00u, 0x01, ECU_ESC,
                           0x02, 0x03, ECU_EOF};

    if( e_eFSP_MSGD_RES_FRAMERESTART == eFSP_MSGD_InsEncChunk(&l_tCtx, testData, sizeof(testData), &l_uConsumed) )
    {
        if( 2u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 7  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[2u], 9u, &l_uConsumed) )
    {
        if( 9u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 8  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 4u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 9  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 10 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 11 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 12 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x01u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 13 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x01u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 14 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[11u], 2u, &l_uConsumed) )
    {
        if( 2u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 15 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtx, &l_uMostEfficient) )
    {
        if( 0u == l_uMostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 17 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 18 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 18 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&l_tCtx, &l_bIsMsgDec) )
    {
        if( false == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 19 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&l_tCtx, &l_bIsMsgDec) )
    {
        if( true == l_bIsMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 20 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 20 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&l_tCtx, &l_uPayLoadLen) )
    {
        if( 0x01u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 21 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 21 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&l_tCtx, &l_puPayLoadLoc, &l_uPayLoadLen) )
    {
        if( 0x01u == l_uPayLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 22 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[11u], 2u, &l_uConsumed) )
    {
        if( 0u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 23 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 24 -- FAIL \n");
    }

    /* Remove */
    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&l_tCtx, &testData[1u], sizeof(testData) - 1u, &l_uConsumed) )
    {
        if( 12u == l_uConsumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 25 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 25 -- FAIL \n");
    }
}

void eFSP_MSGDTST_Corner(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx l_tCtx;
    uint8_t  l_auMemArea[10u];
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    uint32_t l_uVar32;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcPTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_Corner 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_Corner 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGDTST_Corner 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_Corner 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0xA6u, 0xC1u, 0xDCu, 0x0Au, 0x01, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( e_eFSP_MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&l_tCtx, testData, sizeof(testData) + 10u, &l_uVar32) )
    {
        if( sizeof(testData) == l_uVar32 )
        {
            (void)printf("eFSP_MSGDTST_Corner 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_Corner 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_Corner 3  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&l_tCtx, testData, sizeof(testData) + 10u, &l_uVar32) )
    {
        if( 0u == l_uVar32 )
        {
            (void)printf("eFSP_MSGDTST_Corner 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_Corner 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_Corner 4  -- FAIL \n");
    }
}

#endif