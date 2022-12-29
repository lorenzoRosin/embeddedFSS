/**
 * @file       eCU_DUNPKTST.c
 *
 * @brief      Data unpack test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eCU_DUNPKTST.h"
#include "eCU_DUNPK.h"
#include <stdio.h>



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void eCU_DUNPKTST_BadPointer(void);
static void eCU_DUNPKTST_BadInit(void);
static void eCU_DUNPKTST_BadInitSession(void);
static void eCU_DUNPKTST_BadParamEntr(void);
static void eCU_DUNPKTST_CorruptContext(void);
static void eCU_DUNPKTST_OutOfMem(void);
static void eCU_DUNPKTST_EndianLe(void);
static void eCU_DUNPKTST_EndianBe(void);
static void eCU_DUNPKTST_Cycle(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eCU_DUNPKTST_ExeTest(void)
{
	(void)printf("\n\nDATA UNPACK TEST START \n\n");

    eCU_DUNPKTST_BadPointer();
    eCU_DUNPKTST_BadInit();
    eCU_DUNPKTST_BadInitSession();
    eCU_DUNPKTST_BadParamEntr();
    eCU_DUNPKTST_CorruptContext();
    eCU_DUNPKTST_OutOfMem();
    eCU_DUNPKTST_EndianLe();
    eCU_DUNPKTST_EndianBe();
    eCU_DUNPKTST_Cycle();

    (void)printf("\n\nDATA UNPACK END \n\n");
}



/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
static void eCU_DUNPKTST_BadPointer(void)
{
    /* Local variable */
    t_eCU_DUNPK_Ctx l_tCtx;
    uint8_t  l_auBadPointerMempool[5u];
    uint8_t* l_puTempPo;
    uint64_t l_uVarTemp64;
    uint32_t l_uVarTemp32;
    uint16_t l_uVarTemp16;
    uint8_t  l_uVarTemp8;
    bool_t l_bIsInit;

    /* Function */
    if( e_eCU_DUNPK_RES_BADPOINTER == eCU_DUNPK_InitCtx(NULL, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DUNPKTST_BadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadPointer 1  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPOINTER == eCU_DUNPK_InitCtx(&l_tCtx, NULL, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DUNPKTST_BadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadPointer 2  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPOINTER == eCU_DUNPK_StartNewFrame(NULL, 3u) )
    {
        (void)printf("eCU_DUNPKTST_BadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadPointer 3  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPOINTER == eCU_DUNPK_GetUPkDataLocat( NULL, &l_puTempPo, &l_uVarTemp32) )
    {
        (void)printf("eCU_DUNPKTST_BadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadPointer 4  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPOINTER == eCU_DUNPK_GetUPkDataLocat( &l_tCtx, NULL, &l_uVarTemp32) )
    {
        (void)printf("eCU_DUNPKTST_BadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadPointer 5  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPOINTER == eCU_DUNPK_GetUPkDataLocat( &l_tCtx, &l_puTempPo, NULL) )
    {
        (void)printf("eCU_DUNPKTST_BadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadPointer 6  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPOINTER == eCU_DUNPK_RestartCurrentUnpack( NULL ) )
    {
        (void)printf("eCU_DUNPKTST_BadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadPointer 7  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPOINTER == eCU_DUNPK_GetRemToPop( NULL, &l_uVarTemp32 ) )
    {
        (void)printf("eCU_DUNPKTST_BadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadPointer 8  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPOINTER == eCU_DUNPK_GetRemToPop( &l_tCtx, NULL ) )
    {
        (void)printf("eCU_DUNPKTST_BadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadPointer 9  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPOINTER == eCU_DUNPK_PopArray( NULL, l_auBadPointerMempool, sizeof(l_auBadPointerMempool) ) )
    {
        (void)printf("eCU_DUNPKTST_BadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadPointer 10 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPOINTER == eCU_DUNPK_PopArray( &l_tCtx, NULL, sizeof(l_auBadPointerMempool) ) )
    {
        (void)printf("eCU_DUNPKTST_BadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadPointer 11 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPOINTER == eCU_DUNPK_PopU8( NULL, &l_uVarTemp8 ) )
    {
        (void)printf("eCU_DUNPKTST_BadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadPointer 12 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPOINTER == eCU_DUNPK_PopU8( &l_tCtx, NULL ) )
    {
        (void)printf("eCU_DUNPKTST_BadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadPointer 13 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPOINTER == eCU_DUNPK_PopU16( NULL, &l_uVarTemp16 ) )
    {
        (void)printf("eCU_DUNPKTST_BadPointer 14 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadPointer 14 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPOINTER == eCU_DUNPK_PopU16( &l_tCtx, NULL ) )
    {
        (void)printf("eCU_DUNPKTST_BadPointer 15 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadPointer 15 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPOINTER == eCU_DUNPK_PopU32( NULL, &l_uVarTemp32 ) )
    {
        (void)printf("eCU_DUNPKTST_BadPointer 16 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadPointer 16 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPOINTER == eCU_DUNPK_PopU32( &l_tCtx, NULL ) )
    {
        (void)printf("eCU_DUNPKTST_BadPointer 17 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadPointer 17 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPOINTER == eCU_DUNPK_PopU64( NULL, &l_uVarTemp64 ) )
    {
        (void)printf("eCU_DUNPKTST_BadPointer 18 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadPointer 18 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPOINTER == eCU_DUNPK_PopU64( &l_tCtx, NULL ) )
    {
        (void)printf("eCU_DUNPKTST_BadPointer 19 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadPointer 19 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPOINTER == eCU_DUNPK_IsInit( NULL, &l_bIsInit ) )
    {
        (void)printf("eCU_DUNPKTST_BadPointer 20 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadPointer 20 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPOINTER == eCU_DUNPK_IsInit( &l_tCtx, NULL ) )
    {
        (void)printf("eCU_DUNPKTST_BadPointer 21 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadPointer 21 -- FAIL \n");
    }
}

static void eCU_DUNPKTST_BadInit(void)
{
    /* Local variable */
    t_eCU_DUNPK_Ctx l_tCtx;
    uint8_t  l_auBadPointerMempool[5u];
    uint8_t* l_puTempPo;
    uint64_t l_uVarTemp64;
    uint32_t l_uVarTemp32;
    uint16_t l_uVarTemp16;
    uint8_t  l_uVarTemp8;
    bool_t l_bIsInit;

    /* Init variable */
    l_tCtx.bIsInit = true;

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DUNPKTST_BadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadInit 1  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = false;

    if( e_eCU_DUNPK_RES_NOINITLIB == eCU_DUNPK_StartNewFrame( &l_tCtx, 1u ) )
    {
        (void)printf("eCU_DUNPKTST_BadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadInit 2  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NOINITLIB == eCU_DUNPK_GetUPkDataLocat( &l_tCtx, &l_puTempPo, &l_uVarTemp32 ) )
    {
        (void)printf("eCU_DUNPKTST_BadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadInit 3  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NOINITLIB == eCU_DUNPK_RestartCurrentUnpack( &l_tCtx ) )
    {
        (void)printf("eCU_DUNPKTST_BadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadInit 4  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NOINITLIB == eCU_DUNPK_GetRemToPop( &l_tCtx, &l_uVarTemp32 ) )
    {
        (void)printf("eCU_DUNPKTST_BadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadInit 5  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NOINITLIB == eCU_DUNPK_PopArray( &l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool) ) )
    {
        (void)printf("eCU_DUNPKTST_BadInit 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadInit 6  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NOINITLIB == eCU_DUNPK_PopU8( &l_tCtx, &l_uVarTemp8 ) )
    {
        (void)printf("eCU_DUNPKTST_BadInit 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadInit 7  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NOINITLIB == eCU_DUNPK_PopU16( &l_tCtx, &l_uVarTemp16 ) )
    {
        (void)printf("eCU_DUNPKTST_BadInit 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadInit 8  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NOINITLIB == eCU_DUNPK_PopU32( &l_tCtx, &l_uVarTemp32 ) )
    {
        (void)printf("eCU_DUNPKTST_BadInit 9  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadInit 9  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NOINITLIB == eCU_DUNPK_PopU64( &l_tCtx, &l_uVarTemp64 ) )
    {
        (void)printf("eCU_DUNPKTST_BadInit 10 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadInit 10 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_IsInit( &l_tCtx, &l_bIsInit ) )
    {
        if( false == l_bIsInit )
        {
            (void)printf("eCU_DUNPKTST_BadInit 11 -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_BadInit 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadInit 11 -- FAIL \n");
    }
}

static void eCU_DUNPKTST_BadInitSession(void)
{
    /* Local variable */
    t_eCU_DUNPK_Ctx l_tCtx;
    uint8_t  l_auBadPointerMempool[5u];
    uint64_t l_uVarTemp64;
    uint32_t l_uVarTemp32;
    uint16_t l_uVarTemp16;
    uint8_t  l_uVarTemp8;
    bool_t l_bIsInit;

    /* Init variable */
    l_tCtx.bIsInit = true;

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DUNPKTST_BadInitSession 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadInitSession 1  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NOINITFRAME == eCU_DUNPK_RestartCurrentUnpack( &l_tCtx  ) )
    {
        (void)printf("eCU_DUNPKTST_BadInitSession 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadInitSession 2  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NOINITFRAME == eCU_DUNPK_GetRemToPop( &l_tCtx, &l_uVarTemp32  ) )
    {
        (void)printf("eCU_DUNPKTST_BadInitSession 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadInitSession 3  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NOINITFRAME == eCU_DUNPK_PopArray( &l_tCtx, l_auBadPointerMempool, 2u  ) )
    {
        (void)printf("eCU_DUNPKTST_BadInitSession 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadInitSession 4  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NOINITFRAME == eCU_DUNPK_PopU8( &l_tCtx, &l_uVarTemp8  ) )
    {
        (void)printf("eCU_DUNPKTST_BadInitSession 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadInitSession 5  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NOINITFRAME == eCU_DUNPK_PopU16( &l_tCtx, &l_uVarTemp16  ) )
    {
        (void)printf("eCU_DUNPKTST_BadInitSession 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadInitSession 6  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NOINITFRAME == eCU_DUNPK_PopU32( &l_tCtx, &l_uVarTemp32  ) )
    {
        (void)printf("eCU_DUNPKTST_BadInitSession 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadInitSession 7  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NOINITFRAME == eCU_DUNPK_PopU64( &l_tCtx, &l_uVarTemp64  ) )
    {
        (void)printf("eCU_DUNPKTST_BadInitSession 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadInitSession 8  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_IsInit( &l_tCtx, &l_bIsInit ) )
    {
        if( true == l_bIsInit )
        {
            (void)printf("eCU_DUNPKTST_BadInitSession 9  -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_BadInitSession 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadInitSession 9  -- FAIL \n");
    }
}

static void eCU_DUNPKTST_BadParamEntr(void)
{
    /* Local variable */
    t_eCU_DUNPK_Ctx l_tCtx;
    uint8_t  l_auBadPointerMempool[5u];

    /* Init variable */
    l_tCtx.bIsInit = false;
    (void)memset(l_auBadPointerMempool, 0, sizeof(l_auBadPointerMempool));

    /* Function */
    if( e_eCU_DUNPK_RES_BADPARAM == eCU_DUNPK_InitCtx(&l_tCtx, l_auBadPointerMempool, 0u, true) )
    {
        (void)printf("eCU_DUNPKTST_BadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadParamEntr 1  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DUNPKTST_BadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadParamEntr 2  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPARAM == eCU_DUNPK_StartNewFrame( &l_tCtx, 0u ) )
    {
        (void)printf("eCU_DUNPKTST_BadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadParamEntr 3  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPARAM == eCU_DUNPK_StartNewFrame( &l_tCtx, ( sizeof(l_auBadPointerMempool) + 1u ) ) )
    {
        (void)printf("eCU_DUNPKTST_BadParamEntr 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadParamEntr 4  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_StartNewFrame( &l_tCtx, 2u ) )
    {
        (void)printf("eCU_DUNPKTST_BadParamEntr 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadParamEntr 5  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_BADPARAM == eCU_DUNPK_PopArray( &l_tCtx, l_auBadPointerMempool, 0u ) )
    {
        (void)printf("eCU_DUNPKTST_BadParamEntr 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_BadParamEntr 6  -- FAIL \n");
    }
}

static void eCU_DUNPKTST_CorruptContext(void)
{
    /* Local variable */
    t_eCU_DUNPK_Ctx l_tCtx;
    uint8_t  l_auBadPointerMempool[5u];
    uint8_t* l_puTempPo;
    uint32_t l_uVarTemp;
    uint8_t  l_uVarTemp8;
    uint16_t l_uVarTemp16;
    uint32_t l_uVarTemp32;
    uint64_t l_uVarTemp64;

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 1  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.uMemUPKL = 0u;

    if( e_eCU_DUNPK_RES_CORRUPTCTX == eCU_DUNPK_StartNewFrame(&l_tCtx, 4u) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 2  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 3  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.puMemUPK = NULL;

    if( e_eCU_DUNPK_RES_CORRUPTCTX == eCU_DUNPK_StartNewFrame(&l_tCtx, 4u) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 4  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 5  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.uMemUPKFrameL = l_tCtx.uMemUPKL + 1u;

    if( e_eCU_DUNPK_RES_CORRUPTCTX == eCU_DUNPK_StartNewFrame(&l_tCtx, 4u) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 6  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 7  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.uMemUPKFrameL = l_tCtx.uMemUPKL - 2u;
    l_tCtx.uMemUPKCtr = l_tCtx.uMemUPKFrameL + 1u;

    if( e_eCU_DUNPK_RES_CORRUPTCTX == eCU_DUNPK_StartNewFrame(&l_tCtx, 4u) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 8  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 9  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 9  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.uMemUPKFrameL = 1u;
    l_tCtx.uMemUPKL = 0u;

    if( e_eCU_DUNPK_RES_CORRUPTCTX == eCU_DUNPK_GetUPkDataLocat( &l_tCtx, &l_puTempPo, &l_uVarTemp32 ) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 10 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 10 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 11 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 11 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.uMemUPKFrameL = 1u;
    l_tCtx.uMemUPKL = 0u;

    if( e_eCU_DUNPK_RES_CORRUPTCTX == eCU_DUNPK_RestartCurrentUnpack(&l_tCtx) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 12 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 12 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 13 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 13 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.uMemUPKFrameL = 1u;
    l_tCtx.uMemUPKL = 0u;

    if( e_eCU_DUNPK_RES_CORRUPTCTX == eCU_DUNPK_GetRemToPop(&l_tCtx, &l_uVarTemp) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 14 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 14 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 15 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 15 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.uMemUPKFrameL = 1u;
    l_tCtx.uMemUPKL = 0u;

    if( e_eCU_DUNPK_RES_CORRUPTCTX == eCU_DUNPK_PopArray(&l_tCtx, l_auBadPointerMempool, 1u) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 16 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 16 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 17 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 17 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.uMemUPKFrameL = 1u;
    l_tCtx.uMemUPKL = 0u;

    if( e_eCU_DUNPK_RES_CORRUPTCTX == eCU_DUNPK_PopU8(&l_tCtx, &l_uVarTemp8) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 18 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 18 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 19 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 19 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.uMemUPKFrameL = 1u;
    l_tCtx.uMemUPKL = 0u;

    if( e_eCU_DUNPK_RES_CORRUPTCTX == eCU_DUNPK_PopU16(&l_tCtx, &l_uVarTemp16) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 20 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 20 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 21 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 21 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.uMemUPKFrameL = 1u;
    l_tCtx.uMemUPKL = 0u;

    if( e_eCU_DUNPK_RES_CORRUPTCTX == eCU_DUNPK_PopU32(&l_tCtx, &l_uVarTemp32) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 22 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 22 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 23 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 23 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.uMemUPKFrameL = 1u;
    l_tCtx.uMemUPKL = 0u;

    if( e_eCU_DUNPK_RES_CORRUPTCTX == eCU_DUNPK_PopU64(&l_tCtx, &l_uVarTemp64) )
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 24 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_CorruptContext 24 -- FAIL \n");
    }
}

static void eCU_DUNPKTST_OutOfMem(void)
{
    /* Local variable */
    t_eCU_DUNPK_Ctx l_tCtx;
    uint8_t  l_auBadPointerMempool[9u];
    uint8_t  l_auPushPool[5u];
    uint64_t l_uVarTemp64;
    uint32_t l_uVarTemp32;
    uint16_t l_uVarTemp16;
    uint8_t  l_uVarTemp8;
    uint8_t* l_puTempPo;

    /* Init variable */
    l_tCtx.bIsInit = false;
    (void)memset(l_auBadPointerMempool, 0, sizeof(l_auBadPointerMempool));

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 1  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_StartNewFrame(&l_tCtx, 9u) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 2  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_GetUPkDataLocat(&l_tCtx, &l_puTempPo, &l_uVarTemp32) )
    {
        if( ( l_auBadPointerMempool == l_puTempPo ) && ( sizeof(l_auBadPointerMempool) == l_uVarTemp32 ) )
        {
            (void)printf("eCU_DUNPKTST_OutOfMem 3  -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_OutOfMem 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 3  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_GetRemToPop(&l_tCtx, &l_uVarTemp32) )
    {
        if( sizeof(l_auBadPointerMempool) == l_uVarTemp32 )
        {
            (void)printf("eCU_DUNPKTST_OutOfMem 4  -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_OutOfMem 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 4  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_PopU64( &l_tCtx, &l_uVarTemp64 ) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 5  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_GetUPkDataLocat(&l_tCtx, &l_puTempPo, &l_uVarTemp32) )
    {
        if( ( l_auBadPointerMempool == l_puTempPo ) && ( sizeof(l_auBadPointerMempool) == l_uVarTemp32 ) )
        {
            (void)printf("eCU_DUNPKTST_OutOfMem 6  -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_OutOfMem 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 6  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_GetRemToPop(&l_tCtx, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            (void)printf("eCU_DUNPKTST_OutOfMem 7  -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_OutOfMem 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 7  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NODATA == eCU_DUNPK_PopU64( &l_tCtx, &l_uVarTemp64 ) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 8  -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_StartNewFrame(&l_tCtx, 7u) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 9  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 9  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_GetUPkDataLocat(&l_tCtx, &l_puTempPo, &l_uVarTemp32) )
    {
        if( ( l_auBadPointerMempool == l_puTempPo ) && ( sizeof(l_auBadPointerMempool) == l_uVarTemp32 ) )
        {
            (void)printf("eCU_DUNPKTST_OutOfMem 10 -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_OutOfMem 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 10 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_GetRemToPop(&l_tCtx, &l_uVarTemp32) )
    {
        if( 7u == l_uVarTemp32 )
        {
            (void)printf("eCU_DUNPKTST_OutOfMem 11 -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_OutOfMem 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 11 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NODATA == eCU_DUNPK_PopU64( &l_tCtx, &l_uVarTemp64 ) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 12 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 12 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_GetUPkDataLocat(&l_tCtx, &l_puTempPo, &l_uVarTemp32) )
    {
        if( ( l_auBadPointerMempool == l_puTempPo ) && ( sizeof(l_auBadPointerMempool) == l_uVarTemp32 ) )
        {
            (void)printf("eCU_DUNPKTST_OutOfMem 13 -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_OutOfMem 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 13 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_GetRemToPop(&l_tCtx, &l_uVarTemp32) )
    {
        if( 7u == l_uVarTemp32 )
        {
            (void)printf("eCU_DUNPKTST_OutOfMem 14 -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_OutOfMem 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 14 -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_StartNewFrame(&l_tCtx, 5u) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 15 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 15 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_PopU32( &l_tCtx, &l_uVarTemp32 ) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 16 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 16 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NODATA == eCU_DUNPK_PopU32( &l_tCtx, &l_uVarTemp32 ) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 17 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 17 -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_StartNewFrame(&l_tCtx, 3u) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 18 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 18 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NODATA == eCU_DUNPK_PopU32( &l_tCtx, &l_uVarTemp32 ) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 19 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 19 -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_StartNewFrame(&l_tCtx, 3u) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 20 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 20 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_PopU16( &l_tCtx, &l_uVarTemp16 ) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 21 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 21 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NODATA == eCU_DUNPK_PopU16( &l_tCtx, &l_uVarTemp16 ) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 22 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 22 -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_StartNewFrame(&l_tCtx, 1u) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 23 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 23 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NODATA == eCU_DUNPK_PopU16( &l_tCtx, &l_uVarTemp16 ) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 24 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 24 -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_StartNewFrame(&l_tCtx, 1u) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 25 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 25 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_PopU8( &l_tCtx, &l_uVarTemp8 ) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 26 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 26 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NODATA == eCU_DUNPK_PopU8( &l_tCtx, &l_uVarTemp8 ) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 27 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 27 -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_StartNewFrame(&l_tCtx, 6u) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 28 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 28 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_PopArray( &l_tCtx, l_auPushPool, 5u ) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 29 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 29 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NODATA == eCU_DUNPK_PopArray( &l_tCtx, l_auPushPool, 5u ) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 30 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 30 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_PopU8( &l_tCtx, &l_uVarTemp8 ) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 31 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 31 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NODATA == eCU_DUNPK_PopU8( &l_tCtx, &l_uVarTemp8 ) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 32 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 32 -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_StartNewFrame(&l_tCtx, 1u) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 33 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 33 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_NODATA == eCU_DUNPK_PopArray( &l_tCtx, l_auPushPool, 2u ) )
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 34 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_OutOfMem 34 -- FAIL \n");
    }
}

static void eCU_DUNPKTST_EndianLe(void)
{
    /* Local variable */
    t_eCU_DUNPK_Ctx l_tCtx;
    uint8_t  l_auBadPointerMempool[20u];
    uint8_t  l_auPushPool[20u];
    uint32_t l_uRemtoPop;
    uint8_t* l_puTempPo;
    uint64_t l_uVarTemp64;
    uint32_t l_uVarTemp32;
    uint16_t l_uVarTemp16;
    uint8_t  l_uVarTemp8;

    /* Init variable */
    l_tCtx.bIsInit = false;
    (void)memset(l_auBadPointerMempool, 0, sizeof(l_auBadPointerMempool));

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DUNPKTST_EndianLe 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianLe 1  -- FAIL \n");
    }

    l_auBadPointerMempool[0u] = 0x12u;
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_StartNewFrame(&l_tCtx, 1u) )
    {
        (void)printf("eCU_DUNPKTST_EndianLe 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianLe 2  -- FAIL \n");
    }

    l_auBadPointerMempool[0u] = 0xF1u;
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_PopArray( &l_tCtx, l_auPushPool, 1u ) )
    {
        if( 0xF1u == l_auPushPool[0u] )
        {
            (void)printf("eCU_DUNPKTST_EndianLe 3  -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_EndianLe 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianLe 3  -- FAIL \n");
    }

    l_auBadPointerMempool[0u] = 0x12u;
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_RestartCurrentUnpack( &l_tCtx  ) )
    {
        (void)printf("eCU_DUNPKTST_EndianLe 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianLe 4  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_PopU8( &l_tCtx, &l_uVarTemp8 ) )
    {
        if( 0x12u == l_uVarTemp8 )
        {
            (void)printf("eCU_DUNPKTST_EndianLe 5  -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_EndianLe 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianLe 5  -- FAIL \n");
    }

    l_auBadPointerMempool[0u] = 0x34u;
    l_auBadPointerMempool[1u] = 0x12u;
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_StartNewFrame(&l_tCtx, 4u) )
    {
        (void)printf("eCU_DUNPKTST_EndianLe 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianLe 6  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_PopU16( &l_tCtx, &l_uVarTemp16 ) )
    {
        if( 0x1234u == l_uVarTemp16 )
        {
            (void)printf("eCU_DUNPKTST_EndianLe 7  -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_EndianLe 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianLe 7  -- FAIL \n");
    }

    l_auBadPointerMempool[0u] = 0x78u;
    l_auBadPointerMempool[1u] = 0x56u;
    l_auBadPointerMempool[2u] = 0x34u;
    l_auBadPointerMempool[3u] = 0x12u;
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_RestartCurrentUnpack( &l_tCtx ) )
    {
        (void)printf("eCU_DUNPKTST_EndianLe 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianLe 8  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_PopU32( &l_tCtx, &l_uVarTemp32 ) )
    {
        if( 0x12345678u == l_uVarTemp32 )
        {
            (void)printf("eCU_DUNPKTST_EndianLe 9  -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_EndianLe 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianLe 9  -- FAIL \n");
    }

    l_auBadPointerMempool[0u] = 0xF0u;
    l_auBadPointerMempool[1u] = 0xDEu;
    l_auBadPointerMempool[2u] = 0xBCu;
    l_auBadPointerMempool[3u] = 0x9Au;
    l_auBadPointerMempool[4u] = 0x78u;
    l_auBadPointerMempool[5u] = 0x56u;
    l_auBadPointerMempool[6u] = 0x34u;
    l_auBadPointerMempool[7u] = 0x12u;
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_StartNewFrame(&l_tCtx, 8u) )
    {
        (void)printf("eCU_DUNPKTST_EndianLe 10 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianLe 10 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_GetRemToPop(&l_tCtx, &l_uRemtoPop) )
    {
        if( 0x08u == l_uRemtoPop )
        {
            if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_GetUPkDataLocat(&l_tCtx, &l_puTempPo, &l_uRemtoPop) )
            {
                if( ( sizeof(l_auBadPointerMempool) == l_uRemtoPop ) && ( l_auBadPointerMempool == l_puTempPo ) )
                {
                    (void)printf("eCU_DUNPKTST_EndianLe 11 -- OK \n");
                }
                else
                {
                    (void)printf("eCU_DUNPKTST_EndianLe 11 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_DUNPKTST_EndianLe 11 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_DUNPKTST_EndianLe 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianLe 11 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_PopU64( &l_tCtx, &l_uVarTemp64 ) )
    {
        if( 0x123456789ABCDEF0UL == l_uVarTemp64 )
        {
            (void)printf("eCU_DUNPKTST_EndianLe 12 -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_EndianLe 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianLe 12 -- FAIL \n");
    }
}

static void eCU_DUNPKTST_EndianBe(void)
{
    /* Local variable */
    t_eCU_DUNPK_Ctx l_tCtx;
    uint8_t  l_auBadPointerMempool[20u];
    uint8_t  l_auPushPool[20u];
    uint32_t l_uRemtoPop;
    uint8_t* l_puTempPo;
    uint64_t l_uVarTemp64;
    uint32_t l_uVarTemp32;
    uint16_t l_uVarTemp16;
    uint8_t  l_uVarTemp8;

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), false) )
    {
        (void)printf("eCU_DUNPKTST_EndianBe 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianBe 1  -- FAIL \n");
    }

    l_auBadPointerMempool[0u] = 0x12u;
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_StartNewFrame(&l_tCtx, 20u) )
    {
        (void)printf("eCU_DUNPKTST_EndianBe 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianBe 2  -- FAIL \n");
    }

    l_auBadPointerMempool[0u] = 0xF1u;
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_PopArray( &l_tCtx, l_auPushPool, 1u ) )
    {
        if( 0xF1u == l_auPushPool[0u] )
        {
            (void)printf("eCU_DUNPKTST_EndianBe 3  -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_EndianBe 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianBe 3  -- FAIL \n");
    }

    l_auBadPointerMempool[0u] = 0x12u;
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_RestartCurrentUnpack( &l_tCtx ) )
    {
        (void)printf("eCU_DUNPKTST_EndianBe 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianBe 4  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_PopU8( &l_tCtx, &l_uVarTemp8 ) )
    {
        if( 0x12u == l_uVarTemp8 )
        {
            (void)printf("eCU_DUNPKTST_EndianBe 5  -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_EndianBe 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianBe 5  -- FAIL \n");
    }

    l_auBadPointerMempool[0u] = 0x12u;
    l_auBadPointerMempool[1u] = 0x34u;
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_RestartCurrentUnpack( &l_tCtx ) )
    {
        (void)printf("eCU_DUNPKTST_EndianBe 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianBe 6  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_PopU16( &l_tCtx, &l_uVarTemp16 ) )
    {
        if( 0x1234u == l_uVarTemp16 )
        {
            (void)printf("eCU_DUNPKTST_EndianBe 7  -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_EndianBe 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianBe 7  -- FAIL \n");
    }

    l_auBadPointerMempool[0u] = 0x12u;
    l_auBadPointerMempool[1u] = 0x34u;
    l_auBadPointerMempool[2u] = 0x56u;
    l_auBadPointerMempool[3u] = 0x78u;
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_RestartCurrentUnpack( &l_tCtx ) )
    {
        (void)printf("eCU_DUNPKTST_EndianBe 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianBe 8  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_PopU32( &l_tCtx, &l_uVarTemp32 ) )
    {
        if( 0x12345678u == l_uVarTemp32 )
        {
            (void)printf("eCU_DUNPKTST_EndianBe 9  -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_EndianBe 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianBe 9  -- FAIL \n");
    }

    l_auBadPointerMempool[0u] = 0x12u;
    l_auBadPointerMempool[1u] = 0x34u;
    l_auBadPointerMempool[2u] = 0x56u;
    l_auBadPointerMempool[3u] = 0x78u;
    l_auBadPointerMempool[4u] = 0x9Au;
    l_auBadPointerMempool[5u] = 0xBCu;
    l_auBadPointerMempool[6u] = 0xDEu;
    l_auBadPointerMempool[7u] = 0xF0u;

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_RestartCurrentUnpack( &l_tCtx ) )
    {
        (void)printf("eCU_DUNPKTST_EndianBe 10 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianBe 10 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_GetRemToPop(&l_tCtx, &l_uRemtoPop) )
    {
        if( 0x14u == l_uRemtoPop )
        {
            if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_GetUPkDataLocat(&l_tCtx, &l_puTempPo, &l_uRemtoPop) )
            {
                if( ( 0x14u == l_uRemtoPop ) && ( l_auBadPointerMempool == l_puTempPo ) )
                {
                    (void)printf("eCU_DUNPKTST_EndianBe 11 -- OK \n");
                }
                else
                {
                    (void)printf("eCU_DUNPKTST_EndianBe 11 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_DUNPKTST_EndianBe 11 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_DUNPKTST_EndianBe 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianBe 11 -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_PopU64( &l_tCtx, &l_uVarTemp64 ) )
    {
        if( 0x123456789ABCDEF0UL == l_uVarTemp64 )
        {
            (void)printf("eCU_DUNPKTST_EndianBe 12 -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_EndianBe 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_EndianBe 12 -- FAIL \n");
    }
}

static void eCU_DUNPKTST_Cycle(void)
{
    /* Local variable */
    t_eCU_DUNPK_Ctx l_tCtx;
    uint8_t  l_auBadPointerMempool[20u];
    uint16_t l_uVarTemp16;
    uint32_t l_uRemtoPop;
    uint8_t* l_puTempPo;

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DUNPKTST_Cycle 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_Cycle 1  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_StartNewFrame(&l_tCtx, 20u) )
    {
        (void)printf("eCU_DUNPKTST_Cycle 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_Cycle 2  -- FAIL \n");
    }

    l_auBadPointerMempool[0u] = 0x78u;
    l_auBadPointerMempool[1u] = 0x56u;
    l_auBadPointerMempool[2u] = 0x34u;
    l_auBadPointerMempool[3u] = 0x12u;
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_RestartCurrentUnpack( &l_tCtx ) )
    {
        (void)printf("eCU_DUNPKTST_Cycle 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_Cycle 3  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_PopU16( &l_tCtx, &l_uVarTemp16 ) )
    {
        if( 0x5678u == l_uVarTemp16 )
        {
            if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_GetRemToPop(&l_tCtx, &l_uRemtoPop) )
            {
                if( 0x12u == l_uRemtoPop )
                {
                    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_GetUPkDataLocat(&l_tCtx, &l_puTempPo, &l_uRemtoPop) )
                    {
                        if( ( sizeof(l_auBadPointerMempool) == l_uRemtoPop ) && ( l_auBadPointerMempool == l_puTempPo ) )
                        {
                            (void)printf("eCU_DUNPKTST_Cycle 4  -- OK \n");
                        }
                        else
                        {
                            (void)printf("eCU_DUNPKTST_Cycle 4  -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_DUNPKTST_Cycle 4  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_DUNPKTST_Cycle 4  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_DUNPKTST_Cycle 4  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_DUNPKTST_Cycle 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_Cycle 4  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_PopU16( &l_tCtx, &l_uVarTemp16 ) )
    {
        if( 0x1234u == l_uVarTemp16 )
        {
            (void)printf("eCU_DUNPKTST_Cycle 5  -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_Cycle 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_Cycle 5  -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_RestartCurrentUnpack( &l_tCtx ) )
    {
        (void)printf("eCU_DUNPKTST_Cycle 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_Cycle 6  -- FAIL \n");
    }

    l_auBadPointerMempool[0u] = 0x78u;
    l_auBadPointerMempool[1u] = 0x56u;
    l_auBadPointerMempool[2u] = 0x34u;
    l_auBadPointerMempool[3u] = 0x12u;
    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_RestartCurrentUnpack( &l_tCtx ) )
    {
        (void)printf("eCU_DUNPKTST_Cycle 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DUNPKTST_Cycle 7  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_PopU16( &l_tCtx, &l_uVarTemp16 ) )
    {
        if( 0x5678u == l_uVarTemp16 )
        {
            (void)printf("eCU_DUNPKTST_Cycle 8  -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_Cycle 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_Cycle 8  -- FAIL \n");
    }

    if( e_eCU_DUNPK_RES_OK == eCU_DUNPK_PopU16( &l_tCtx, &l_uVarTemp16 ) )
    {
        if( 0x1234u == l_uVarTemp16 )
        {
            (void)printf("eCU_DUNPKTST_Cycle 9  -- OK \n");
        }
        else
        {
            (void)printf("eCU_DUNPKTST_Cycle 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DUNPKTST_Cycle 9  -- FAIL \n");
    }
}