/**
 * @file       eCU_CRCDTST.c
 *
 * @brief      CRC digest test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eCU_CRCDTST.h"
#include "eCU_CRCD.h"
#include "eCU_CRC.h"
#include <stdio.h>



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
struct t_eCU_CRCD_CrcCtxUser
{
    e_eCU_CRC_RES eLastError;
};

static bool_t eCU_CRCDTST_c32SAdapt(t_eCU_CRCD_CrcCtx* const p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val);
static bool_t eCU_CRCDTST_c32SAdaptEr(t_eCU_CRCD_CrcCtx* const p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val);



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void eCU_CRCDTST_BadPointer(void);
static void eCU_CRCDTST_BadInit(void);
static void eCU_CRCDTST_BadParamEntr(void);
static void eCU_CRCDTST_ContextStatus(void);
static void eCU_CRCDTST_ToManyOperation(void);
static void eCU_CRCDTST_NoOperation(void);
static void eCU_CRCDTST_ClbErr(void);
static void eCU_CRCDTST_Mono(void);
static void eCU_CRCDTST_Combined(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eCU_CRCDTST_ExeTest(void)
{
	(void)printf("\n\nCRC DIGEST TEST START \n\n");

    eCU_CRCDTST_BadPointer();
    eCU_CRCDTST_BadInit();
    eCU_CRCDTST_BadParamEntr();
    eCU_CRCDTST_ContextStatus();
    eCU_CRCDTST_ToManyOperation();
    eCU_CRCDTST_NoOperation();
    eCU_CRCDTST_ClbErr();
    eCU_CRCDTST_Mono();
    eCU_CRCDTST_Combined();

    (void)printf("\n\nCRC DIGEST END \n\n");
}



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eCU_CRCDTST_c32SAdapt(t_eCU_CRCD_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val)
{
    bool_t l_bRes;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puD ) || ( NULL == p_puC32Val ) )
    {
        l_bRes = false;
    }
    else
    {
        if( 0u == p_uDLen )
        {
            l_bRes = false;
        }
        else
        {
            p_ptCtx->eLastError = eCU_CRC_32Seed(p_uS, p_puD, p_uDLen, p_puC32Val);
            if( e_eCU_CRC_RES_OK == p_ptCtx->eLastError )
            {
                l_bRes = true;
            }
            else
            {
                l_bRes = false;
            }
        }
    }

    return l_bRes;
}

static bool_t eCU_CRCDTST_c32SAdaptEr(t_eCU_CRCD_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val)
{
    bool_t l_bRes;

    (void)p_uS;
    (void)p_puD;
    (void)p_uDLen;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puD ) || ( NULL == p_puC32Val ) )
    {
        l_bRes = false;
    }
    else
    {
        if( 0u == p_uDLen )
        {
            l_bRes = false;
        }
        else
        {
            p_ptCtx->eLastError = e_eCU_CRC_RES_BADPOINTER;
            l_bRes = false;
            *p_puC32Val = 0u;
        }
    }

    return l_bRes;
}



/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
static void eCU_CRCDTST_BadPointer(void)
{
    /* Local variable */
    t_eCU_CRCD_Ctx l_tCtx;
    f_eCU_CRCD_CrcCb l_fCrcTest = &eCU_CRCDTST_c32SAdapt;
    t_eCU_CRCD_CrcCtx l_tCtxAdapterCrc;

    uint8_t  l_auVarBuff[5u];
    uint32_t l_uVarTemp;
    bool_t l_bIsInit;

    /* Function */
    if( e_eCU_CRCD_RES_BADPOINTER == eCU_CRCD_InitCtx(NULL, l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eCU_CRCDTST_BadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_BadPointer 1  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_BADPOINTER == eCU_CRCD_InitCtx(&l_tCtx, NULL, &l_tCtxAdapterCrc) )
    {
        (void)printf("eCU_CRCDTST_BadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_BadPointer 2  -- FAIL \n");
    }


    if( e_eCU_CRCD_RES_BADPOINTER == eCU_CRCD_InitCtx(&l_tCtx, l_fCrcTest, NULL) )
    {
        (void)printf("eCU_CRCDTST_BadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_BadPointer 2  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_BADPOINTER == eCU_CRCD_SeedInitCtx(NULL, 0x123456u, l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eCU_CRCDTST_BadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_BadPointer 4  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_BADPOINTER == eCU_CRCD_SeedInitCtx(&l_tCtx, 0x123456u, NULL, &l_tCtxAdapterCrc) )
    {
        (void)printf("eCU_CRCDTST_BadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_BadPointer 5  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_BADPOINTER == eCU_CRCD_SeedInitCtx(&l_tCtx, 0x123456u, l_fCrcTest, NULL) )
    {
        (void)printf("eCU_CRCDTST_BadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_BadPointer 6  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_BADPOINTER == eCU_CRCD_Restart( NULL ) )
    {
        (void)printf("eCU_CRCDTST_BadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_BadPointer 7  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_BADPOINTER == eCU_CRCD_Digest( NULL, l_auVarBuff, sizeof(l_auVarBuff) ) )
    {
        (void)printf("eCU_CRCDTST_BadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_BadPointer 8  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_BADPOINTER == eCU_CRCD_Digest( &l_tCtx, NULL, sizeof(l_auVarBuff) ) )
    {
        (void)printf("eCU_CRCDTST_BadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_BadPointer 9  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_BADPOINTER == eCU_CRCD_GetDigestVal( NULL, &l_uVarTemp) )
    {
        (void)printf("eCU_CRCDTST_BadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_BadPointer 10 -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_BADPOINTER == eCU_CRCD_GetDigestVal( &l_tCtx, NULL) )
    {
        (void)printf("eCU_CRCDTST_BadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_BadPointer 11 -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_BADPOINTER == eCU_CRCD_IsInit( NULL, &l_bIsInit ) )
    {
        (void)printf("eCU_CRCDTST_BadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_BadPointer 12 -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_BADPOINTER == eCU_CRCD_IsInit( &l_tCtx, NULL ) )
    {
        (void)printf("eCU_CRCDTST_BadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_BadPointer 13 -- FAIL \n");
    }
}

static void eCU_CRCDTST_BadInit(void)
{
    /* Local variable */
    t_eCU_CRCD_Ctx l_tCtx;
    uint8_t  l_auVarBuff[5u];
    uint32_t l_uVarTemp;
    bool_t l_bIsInit;

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_CRCD_RES_NOINITLIB == eCU_CRCD_Restart( &l_tCtx ) )
    {
        (void)printf("eCU_CRCDTST_BadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_BadInit 1  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_NOINITLIB == eCU_CRCD_Digest( &l_tCtx, l_auVarBuff, sizeof(l_auVarBuff) ) )
    {
        (void)printf("eCU_CRCDTST_BadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_BadInit 2  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_NOINITLIB == eCU_CRCD_GetDigestVal( &l_tCtx, &l_uVarTemp) )
    {
        (void)printf("eCU_CRCDTST_BadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_BadInit 3  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_OK == eCU_CRCD_IsInit( &l_tCtx, &l_bIsInit ) )
    {
        if( false == l_bIsInit )
        {
            (void)printf("eCU_CRCDTST_BadInit 4  -- OK \n");
        }
        else
        {
            (void)printf("eCU_CRCDTST_BadInit 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_CRCDTST_BadInit 4  -- FAIL \n");
    }
}

static void eCU_CRCDTST_BadParamEntr(void)
{
    /* Local variable */
    t_eCU_CRCD_Ctx l_tCtx;
    f_eCU_CRCD_CrcCb l_fCrcTest = &eCU_CRCDTST_c32SAdapt;
    t_eCU_CRCD_CrcCtx l_tCtxAdapterCrc;
    bool_t l_bIsInit;
    uint8_t  l_auVarBuff[5u];
    l_auVarBuff[0] = 0u;

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_CRCD_RES_OK == eCU_CRCD_InitCtx(&l_tCtx, l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eCU_CRCDTST_BadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_BadParamEntr 1  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_BADPARAM == eCU_CRCD_Digest( &l_tCtx, l_auVarBuff, 0u ) )
    {
        (void)printf("eCU_CRCDTST_BadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_BadParamEntr 2  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_OK == eCU_CRCD_IsInit( &l_tCtx, &l_bIsInit ) )
    {
        if( true == l_bIsInit )
        {
            (void)printf("eCU_CRCDTST_BadParamEntr 3  -- OK \n");
        }
        else
        {
            (void)printf("eCU_CRCDTST_BadParamEntr 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_CRCDTST_BadParamEntr 3  -- FAIL \n");
    }
}

static void eCU_CRCDTST_ContextStatus(void)
{
    /* Local variable */
    t_eCU_CRCD_Ctx l_tCtx;
    f_eCU_CRCD_CrcCb l_fCrcTest = &eCU_CRCDTST_c32SAdapt;
    t_eCU_CRCD_CrcCtx l_tCtxAdapterCrc;
    uint8_t  l_auVarBuff[5u];
    uint32_t l_uVarTemp;

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_CRCD_RES_OK == eCU_CRCD_InitCtx(&l_tCtx, l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eCU_CRCDTST_ContextStatus 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_ContextStatus 1  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.fCrc = NULL;
    if( e_eCU_CRCD_RES_CORRUPTCTX == eCU_CRCD_Restart( &l_tCtx ) )
    {
        (void)printf("eCU_CRCDTST_ContextStatus 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_ContextStatus 2  -- FAIL \n");
    }


    /* Function */
    if( e_eCU_CRCD_RES_OK == eCU_CRCD_InitCtx(&l_tCtx, l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eCU_CRCDTST_ContextStatus 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_ContextStatus 3  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.fCrc = NULL;
    if( e_eCU_CRCD_RES_CORRUPTCTX == eCU_CRCD_Digest( &l_tCtx, l_auVarBuff, sizeof(l_auVarBuff) ) )
    {
        (void)printf("eCU_CRCDTST_ContextStatus 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_ContextStatus 4  -- FAIL \n");
    }

    /* Function */
    if( e_eCU_CRCD_RES_OK == eCU_CRCD_InitCtx(&l_tCtx, l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eCU_CRCDTST_ContextStatus 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_ContextStatus 5  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.ptCrcCtx = NULL;
    if( e_eCU_CRCD_RES_CORRUPTCTX == eCU_CRCD_GetDigestVal( &l_tCtx, &l_uVarTemp ) )
    {
        (void)printf("eCU_CRCDTST_ContextStatus 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_ContextStatus 6  -- FAIL \n");
    }
}

static void eCU_CRCDTST_ToManyOperation(void)
{
    /* Local variable */
    t_eCU_CRCD_Ctx l_tCtx;
    f_eCU_CRCD_CrcCb l_fCrcTest = &eCU_CRCDTST_c32SAdapt;
    t_eCU_CRCD_CrcCtx l_tCtxAdapterCrc;
    uint8_t  l_auVarBuff[5u];

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_CRCD_RES_OK == eCU_CRCD_InitCtx(&l_tCtx, l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eCU_CRCDTST_ToManyOperation 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_ToManyOperation 1  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.uDigestedTimes = 0xFFFFFFFFu;
    if( e_eCU_CRCD_RES_TOOMANYDIGEST == eCU_CRCD_Digest( &l_tCtx, l_auVarBuff, sizeof(l_auVarBuff) ) )
    {
        (void)printf("eCU_CRCDTST_ToManyOperation 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_ToManyOperation 2  -- FAIL \n");
    }
}

static void eCU_CRCDTST_NoOperation(void)
{
    /* Local variable */
    t_eCU_CRCD_Ctx l_tCtx;
    f_eCU_CRCD_CrcCb l_fCrcTest = &eCU_CRCDTST_c32SAdapt;
    t_eCU_CRCD_CrcCtx l_tCtxAdapterCrc;
    uint32_t  l_uVarCarc;

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_CRCD_RES_OK == eCU_CRCD_InitCtx(&l_tCtx, l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eCU_CRCDTST_NoOperation 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_NoOperation 1  -- FAIL \n");
    }

    /* Init variable */
    if( e_eCU_CRCD_RES_NODIGESTDONE == eCU_CRCD_GetDigestVal( &l_tCtx, &l_uVarCarc ) )
    {
        (void)printf("eCU_CRCDTST_NoOperation 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_NoOperation 2  -- FAIL \n");
    }
}

static void eCU_CRCDTST_ClbErr(void)
{
    /* Local variable */
    t_eCU_CRCD_Ctx l_tCtx;
    f_eCU_CRCD_CrcCb l_fCrcPTestErr = &eCU_CRCDTST_c32SAdaptEr;
    f_eCU_CRCD_CrcCb l_fCrcTest = &eCU_CRCDTST_c32SAdapt;
    t_eCU_CRCD_CrcCtx l_tCtxAdapterCrc;
    uint8_t  l_auVarBuff[5u];

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    l_tCtxAdapterCrc.eLastError = e_eCU_CRC_RES_OK;
    if( e_eCU_CRCD_RES_OK == eCU_CRCD_InitCtx(&l_tCtx, l_fCrcPTestErr, &l_tCtxAdapterCrc) )
    {
        (void)printf("eCU_CRCDTST_ClbErr 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_ClbErr 1  -- FAIL \n");
    }

    /* Init variable */
    if( e_eCU_CRCD_RES_CLBCKREPORTERROR == eCU_CRCD_Digest( &l_tCtx, l_auVarBuff, sizeof(l_auVarBuff) ) )
    {
        if( e_eCU_CRC_RES_BADPOINTER == l_tCtxAdapterCrc.eLastError )
        {
            (void)printf("eCU_CRCDTST_ClbErr 2  -- OK \n");
        }
        else
        {
            (void)printf("eCU_CRCDTST_ClbErr 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_CRCDTST_ClbErr 2  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    l_tCtxAdapterCrc.eLastError = e_eCU_CRC_RES_OK;
    if( e_eCU_CRCD_RES_OK == eCU_CRCD_InitCtx(&l_tCtx, l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eCU_CRCDTST_ClbErr 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_ClbErr 3  -- FAIL \n");
    }

    /* Init variable */
    if( e_eCU_CRCD_RES_OK== eCU_CRCD_Digest( &l_tCtx, l_auVarBuff, 1u ) )
    {
        (void)printf("eCU_CRCDTST_ClbErr 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_ClbErr 4  -- FAIL \n");
    }

    l_tCtx.fCrc = l_fCrcPTestErr;
    if( e_eCU_CRCD_RES_CLBCKREPORTERROR == eCU_CRCD_Digest( &l_tCtx, l_auVarBuff, sizeof(l_auVarBuff) ) )
    {
        if( e_eCU_CRC_RES_BADPOINTER == l_tCtxAdapterCrc.eLastError )
        {
            (void)printf("eCU_CRCDTST_ClbErr 5  -- OK \n");
        }
        else
        {
            (void)printf("eCU_CRCDTST_ClbErr 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_CRCDTST_ClbErr 5  -- FAIL \n");
    }
}

static void eCU_CRCDTST_Mono(void)
{
    /* Local variable */
    t_eCU_CRCD_Ctx l_tCtx;
    f_eCU_CRCD_CrcCb l_fCrcTest = &eCU_CRCDTST_c32SAdapt;
    t_eCU_CRCD_CrcCtx l_tCtxAdapterCrc;

    /* Test value */
    uint8_t l_auCrcTestData[] = {0xA1u, 0xB3u, 0xFFu, 0xFFu, 0x00u, 0xCFu, 0xD9u, 0x56u};
    uint32_t l_uCrcTestValRet;

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_CRCD_RES_OK == eCU_CRCD_InitCtx(&l_tCtx, l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eCU_CRCDTST_Mono 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_Mono 1  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_OK == eCU_CRCD_Digest( &l_tCtx, l_auCrcTestData, sizeof(l_auCrcTestData) ) )
    {
        (void)printf("eCU_CRCDTST_Mono 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_Mono 2  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_OK == eCU_CRCD_GetDigestVal( &l_tCtx, &l_uCrcTestValRet ) )
    {
        if( 0xBCF43C51u == l_uCrcTestValRet)
        {
            (void)printf("eCU_CRCDTST_Mono 3  -- OK \n");
        }
        else
        {
            (void)printf("eCU_CRCDTST_Mono 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_CRCDTST_Mono 3  -- FAIL \n");
    }

    /* Function */
    if( e_eCU_CRCD_RES_OK == eCU_CRCD_SeedInitCtx(&l_tCtx, 0x0u, l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eCU_CRCDTST_Mono 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_Mono 4  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_OK == eCU_CRCD_Digest( &l_tCtx, l_auCrcTestData, sizeof(l_auCrcTestData) ) )
    {
        (void)printf("eCU_CRCDTST_Mono 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_Mono 5  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_OK == eCU_CRCD_GetDigestVal( &l_tCtx, &l_uCrcTestValRet ) )
    {
        if( 0xD5F08708u == l_uCrcTestValRet)
        {
            (void)printf("eCU_CRCDTST_Mono 6  -- OK \n");
        }
        else
        {
            (void)printf("eCU_CRCDTST_Mono 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_CRCDTST_Mono 6  -- FAIL \n");
    }

    /* Function */
    if( e_eCU_CRCD_RES_OK == eCU_CRCD_Restart(&l_tCtx) )
    {
        (void)printf("eCU_CRCDTST_Mono 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_Mono 7  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_OK == eCU_CRCD_Digest( &l_tCtx, l_auCrcTestData, sizeof(l_auCrcTestData) ) )
    {
        (void)printf("eCU_CRCDTST_Mono 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_Mono 8  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_OK == eCU_CRCD_GetDigestVal( &l_tCtx, &l_uCrcTestValRet ) )
    {
        if( 0xD5F08708u == l_uCrcTestValRet)
        {
            (void)printf("eCU_CRCDTST_Mono 9  -- OK \n");
        }
        else
        {
            (void)printf("eCU_CRCDTST_Mono 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_CRCDTST_Mono 9  -- FAIL \n");
    }

    /* Function */
    if( e_eCU_CRCD_RES_OK == eCU_CRCD_Restart(&l_tCtx) )
    {
        (void)printf("eCU_CRCDTST_Mono 10 -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_Mono 10 -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_NODIGESTDONE == eCU_CRCD_GetDigestVal( &l_tCtx, &l_uCrcTestValRet ) )
    {
        (void)printf("eCU_CRCDTST_Mono 11 -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_Mono 11 -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_OK == eCU_CRCD_Digest( &l_tCtx, l_auCrcTestData, sizeof(l_auCrcTestData) ) )
    {
        (void)printf("eCU_CRCDTST_Mono 12 -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_Mono 12 -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_OK == eCU_CRCD_GetDigestVal( &l_tCtx, &l_uCrcTestValRet ) )
    {
        if( 0xD5F08708u == l_uCrcTestValRet)
        {
            (void)printf("eCU_CRCDTST_Mono 13 -- OK \n");
        }
        else
        {
            (void)printf("eCU_CRCDTST_Mono 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_CRCDTST_Mono 13 -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_NODIGESTDONE == eCU_CRCD_GetDigestVal( &l_tCtx, &l_uCrcTestValRet ) )
    {
        (void)printf("eCU_CRCDTST_Mono 14 -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_Mono 14 -- FAIL \n");
    }
}

static void eCU_CRCDTST_Combined(void)
{
    /* Local variable */
    t_eCU_CRCD_Ctx l_tCtx;
    f_eCU_CRCD_CrcCb l_fCrcTest = &eCU_CRCDTST_c32SAdapt;
    t_eCU_CRCD_CrcCtx l_tCtxAdapterCrc;

    /* Test value */
    uint8_t l_auCrcTestDataC[] = {0x00u, 0x01u, 0x02u, 0x03u, 0x04u, 0x05u, 0xA1u, 0xB3u, 0xFFu, 0xFFu, 0x00u, 0xCFu, 0xD9u,
                              0x56u, 0x00u, 0x01u, 0x02u, 0x03u, 0x04u, 0x05u, 0xA1u, 0xB3u, 0xFFu, 0xFFu, 0x00u, 0xCFu,
                              0xD9u, 0x56u };
    uint32_t l_uCrcTestValRetC;

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_CRCD_RES_OK == eCU_CRCD_InitCtx(&l_tCtx, l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eCU_CRCDTST_Combined 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_Combined 1  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_OK == eCU_CRCD_Digest( &l_tCtx, l_auCrcTestDataC, 0x02u ) )
    {
        (void)printf("eCU_CRCDTST_Combined 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_Combined 2  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_OK == eCU_CRCD_Digest( &l_tCtx, &l_auCrcTestDataC[2u], 0x04u ) )
    {
        (void)printf("eCU_CRCDTST_Combined 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_Combined 3  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_OK == eCU_CRCD_Digest( &l_tCtx, &l_auCrcTestDataC[6u], 0x04u ) )
    {
        (void)printf("eCU_CRCDTST_Combined 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_Combined 4  -- FAIL \n");
    }


    if( e_eCU_CRCD_RES_OK == eCU_CRCD_Digest( &l_tCtx, &l_auCrcTestDataC[10u], 0x12u ) )
    {
        (void)printf("eCU_CRCDTST_Combined 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_Combined 5  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_OK == eCU_CRCD_GetDigestVal( &l_tCtx, &l_uCrcTestValRetC ) )
    {
        if( 0x1CE847A8u == l_uCrcTestValRetC)
        {
            (void)printf("eCU_CRCDTST_Combined 6  -- OK \n");
        }
        else
        {
            (void)printf("eCU_CRCDTST_Combined 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_CRCDTST_Combined 6  -- FAIL \n");
    }

     /* Function */
    if( e_eCU_CRCD_RES_OK == eCU_CRCD_SeedInitCtx(&l_tCtx, 0x00u, l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eCU_CRCDTST_Combined 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_Combined 7  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_OK == eCU_CRCD_Digest( &l_tCtx, l_auCrcTestDataC, 0x02u ) )
    {
        (void)printf("eCU_CRCDTST_Combined 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_Combined 8  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_OK == eCU_CRCD_Digest( &l_tCtx, &l_auCrcTestDataC[2u], 0x04u ) )
    {
        (void)printf("eCU_CRCDTST_Combined 9  -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_Combined 9  -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_OK == eCU_CRCD_Digest( &l_tCtx, &l_auCrcTestDataC[6u], 0x04u ) )
    {
        (void)printf("eCU_CRCDTST_Combined 10 -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_Combined 10 -- FAIL \n");
    }


    if( e_eCU_CRCD_RES_OK == eCU_CRCD_Digest( &l_tCtx, &l_auCrcTestDataC[10u], 0x12u ) )
    {
        (void)printf("eCU_CRCDTST_Combined 11 -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_Combined 11 -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_OK == eCU_CRCD_GetDigestVal( &l_tCtx, &l_uCrcTestValRetC ) )
    {
        if( 0x74F9B656u == l_uCrcTestValRetC)
        {
            (void)printf("eCU_CRCDTST_Combined 12 -- OK \n");
        }
        else
        {
            (void)printf("eCU_CRCDTST_Combined 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_CRCDTST_Combined 12 -- FAIL \n");
    }

    if( e_eCU_CRCD_RES_NODIGESTDONE == eCU_CRCD_GetDigestVal( &l_tCtx, &l_uCrcTestValRetC ) )
    {
        (void)printf("eCU_CRCDTST_Combined 13 -- OK \n");
    }
    else
    {
        (void)printf("eCU_CRCDTST_Combined 13 -- FAIL \n");
    }
}