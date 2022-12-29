/**
 * @file       eCU_DPKTST.c
 *
 * @brief      Data pack test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eCU_DPKTST.h"
#include "eCU_DPK.h"
#include <stdio.h>



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void eCU_DPKTST_BadPointer(void);
static void eCU_DPKTST_BadInit(void);
static void eCU_DPKTST_BadParamEntr(void);
static void eCU_DPKTST_CorruptContext(void);
static void eCU_DPKTST_OutOfMem(void);
static void eCU_DPKTST_EndianLe(void);
static void eCU_DPKTST_EndianBe(void);
static void eCU_DPKTST_Cycle(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eCU_DPKTST_ExeTest(void)
{
	(void)printf("\n\nDATA PACK TEST START \n\n");

    eCU_DPKTST_BadPointer();
    eCU_DPKTST_BadInit();
    eCU_DPKTST_BadParamEntr();
    eCU_DPKTST_CorruptContext();
    eCU_DPKTST_OutOfMem();
    eCU_DPKTST_EndianLe();
    eCU_DPKTST_EndianBe();
    eCU_DPKTST_Cycle();

    (void)printf("\n\nDATA PACK END \n\n");
}



/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
static void eCU_DPKTST_BadPointer(void)
{
    /* Local variable */
    t_eCU_DPK_Ctx l_tCtx;
    uint8_t  l_auBadPointerMempool[5u];
    uint32_t l_uVarTemp;
    uint8_t* l_puPointerP;
    bool_t l_bIsInit;

    /* Function */
    if( e_eCU_DPK_RES_BADPOINTER == eCU_DPK_InitCtx(NULL, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DPKTST_BadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadPointer 1  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_BADPOINTER == eCU_DPK_InitCtx(&l_tCtx, NULL, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DPKTST_BadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadPointer 2  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_BADPOINTER == eCU_DPK_StartNewPack( NULL ) )
    {
        (void)printf("eCU_DPKTST_BadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadPointer 3  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_BADPOINTER == eCU_DPK_GetDataReference( NULL, &l_puPointerP, &l_uVarTemp ) )
    {
        (void)printf("eCU_DPKTST_BadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadPointer 4  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_BADPOINTER == eCU_DPK_GetDataReference( &l_tCtx, NULL, &l_uVarTemp ) )
    {
        (void)printf("eCU_DPKTST_BadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadPointer 5  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_BADPOINTER == eCU_DPK_GetDataReference( &l_tCtx, &l_puPointerP, NULL ) )
    {
        (void)printf("eCU_DPKTST_BadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadPointer 6  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_BADPOINTER == eCU_DPK_GetNPushed( NULL, &l_uVarTemp ) )
    {
        (void)printf("eCU_DPKTST_BadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadPointer 7  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_BADPOINTER == eCU_DPK_GetNPushed( &l_tCtx, NULL ) )
    {
        (void)printf("eCU_DPKTST_BadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadPointer 8  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_BADPOINTER == eCU_DPK_PushArray( NULL, l_auBadPointerMempool, sizeof(l_auBadPointerMempool) ) )
    {
        (void)printf("eCU_DPKTST_BadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadPointer 9  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_BADPOINTER == eCU_DPK_PushArray( &l_tCtx, NULL, sizeof(l_auBadPointerMempool) ) )
    {
        (void)printf("eCU_DPKTST_BadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadPointer 10 -- FAIL \n");
    }

    if( e_eCU_DPK_RES_BADPOINTER == eCU_DPK_PushU8( NULL, 10u ) )
    {
        (void)printf("eCU_DPKTST_BadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadPointer 11 -- FAIL \n");
    }

    if( e_eCU_DPK_RES_BADPOINTER == eCU_DPK_PushU16( NULL, 10u ) )
    {
        (void)printf("eCU_DPKTST_BadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadPointer 12 -- FAIL \n");
    }

    if( e_eCU_DPK_RES_BADPOINTER == eCU_DPK_PushU32( NULL, 10u ) )
    {
        (void)printf("eCU_DPKTST_BadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadPointer 13 -- FAIL \n");
    }

    if( e_eCU_DPK_RES_BADPOINTER == eCU_DPK_PushU64( NULL, 10u ) )
    {
        (void)printf("eCU_DPKTST_BadPointer 14 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadPointer 14 -- FAIL \n");
    }

    if( e_eCU_DPK_RES_BADPOINTER == eCU_DPK_IsInit( NULL, &l_bIsInit ) )
    {
        (void)printf("eCU_DPKTST_BadPointer 15 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadPointer 15 -- FAIL \n");
    }

    if( e_eCU_DPK_RES_BADPOINTER == eCU_DPK_IsInit( &l_tCtx, NULL ) )
    {
        (void)printf("eCU_DPKTST_BadPointer 16 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadPointer 16 -- FAIL \n");
    }
}

static void eCU_DPKTST_BadInit(void)
{
    /* Local variable */
    t_eCU_DPK_Ctx l_tCtx;
    uint8_t  l_auBadPointerMempool[5u];
    uint32_t l_uVarTemp;
    uint8_t* l_puPointerP;
    bool_t l_bIsInit;

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DPKTST_BadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadInit 1  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = false;

    if( e_eCU_DPK_RES_NOINITLIB == eCU_DPK_StartNewPack( &l_tCtx ) )
    {
        (void)printf("eCU_DPKTST_BadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadInit 2  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_NOINITLIB == eCU_DPK_GetDataReference( &l_tCtx, &l_puPointerP, &l_uVarTemp ) )
    {
        (void)printf("eCU_DPKTST_BadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadInit 3  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_NOINITLIB == eCU_DPK_GetNPushed( &l_tCtx, &l_uVarTemp ) )
    {
        (void)printf("eCU_DPKTST_BadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadInit 4  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_NOINITLIB == eCU_DPK_PushArray( &l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool) ) )
    {
        (void)printf("eCU_DPKTST_BadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadInit 5  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_NOINITLIB == eCU_DPK_PushU8( &l_tCtx, 10u ) )
    {
        (void)printf("eCU_DPKTST_BadInit 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadInit 6  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_NOINITLIB == eCU_DPK_PushU16( &l_tCtx, 10u ) )
    {
        (void)printf("eCU_DPKTST_BadInit 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadInit 7  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_NOINITLIB == eCU_DPK_PushU32( &l_tCtx, 10u ) )
    {
        (void)printf("eCU_DPKTST_BadInit 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadInit 8  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_NOINITLIB == eCU_DPK_PushU64( &l_tCtx, 10u ) )
    {
        (void)printf("eCU_DPKTST_BadInit 9  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadInit 9  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_IsInit( &l_tCtx, &l_bIsInit ) )
    {
        if( false == l_bIsInit )
        {
            (void)printf("eCU_DPKTST_BadInit 10 -- OK \n");
        }
        else
        {
            (void)printf("eCU_DPKTST_BadInit 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DPKTST_BadInit 10 -- FAIL \n");
    }
}

static void eCU_DPKTST_BadParamEntr(void)
{
    /* Local variable */
    t_eCU_DPK_Ctx l_tCtx;
    uint8_t  l_auBadPointerMempool[5u];
    bool_t l_bIsInit;

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DPK_RES_BADPARAM == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, 0u, true) )
    {
        (void)printf("eCU_DPKTST_BadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadParamEntr 1  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DPK_RES_BADPARAM == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, 0u, false) )
    {
        (void)printf("eCU_DPKTST_BadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadParamEntr 2  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = true;

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DPKTST_BadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadParamEntr 3  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_BADPARAM == eCU_DPK_PushArray( &l_tCtx, l_auBadPointerMempool, 0u ) )
    {
        (void)printf("eCU_DPKTST_BadParamEntr 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_BadParamEntr 4  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_IsInit( &l_tCtx, &l_bIsInit ) )
    {
        if( true == l_bIsInit )
        {
            (void)printf("eCU_DPKTST_BadParamEntr 5  -- OK \n");
        }
        else
        {
            (void)printf("eCU_DPKTST_BadParamEntr 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DPKTST_BadParamEntr 5  -- FAIL \n");
    }
}

static void eCU_DPKTST_CorruptContext(void)
{
    /* Local variable */
    t_eCU_DPK_Ctx l_tCtx;
    uint8_t  l_auBadPointerMempool[5u];
    uint32_t l_uVarTemp;
    uint8_t* l_puPointerP;

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DPKTST_CorruptContext 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_CorruptContext 1  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.uMemPKL = 0u;

    if( e_eCU_DPK_RES_CORRUPTCTX == eCU_DPK_GetNPushed(&l_tCtx, &l_uVarTemp) )
    {
        (void)printf("eCU_DPKTST_CorruptContext 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_CorruptContext 2  -- FAIL \n");
    }



    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DPKTST_CorruptContext 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_CorruptContext 3  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.puMemPK = NULL;

    if( e_eCU_DPK_RES_CORRUPTCTX == eCU_DPK_GetNPushed(&l_tCtx, &l_uVarTemp) )
    {
        (void)printf("eCU_DPKTST_CorruptContext 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_CorruptContext 4  -- FAIL \n");
    }



    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DPKTST_CorruptContext 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_CorruptContext 5  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.uMemPKCtr = l_tCtx.uMemPKL + 1u;

    if( e_eCU_DPK_RES_CORRUPTCTX == eCU_DPK_GetNPushed(&l_tCtx, &l_uVarTemp) )
    {
        (void)printf("eCU_DPKTST_CorruptContext 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_CorruptContext 6  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DPKTST_CorruptContext 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_CorruptContext 7  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.uMemPKL = 0u;

    if( e_eCU_DPK_RES_CORRUPTCTX == eCU_DPK_StartNewPack( &l_tCtx ) )
    {
        (void)printf("eCU_DPKTST_CorruptContext 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_CorruptContext 8  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DPKTST_CorruptContext 9  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_CorruptContext 9  -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.uMemPKL = 0u;

    if( e_eCU_DPK_RES_CORRUPTCTX == eCU_DPK_GetDataReference( &l_tCtx, &l_puPointerP, &l_uVarTemp ) )
    {
        (void)printf("eCU_DPKTST_CorruptContext 10 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_CorruptContext 10 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DPKTST_CorruptContext 11 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_CorruptContext 11 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.uMemPKL = 0u;

    if( e_eCU_DPK_RES_CORRUPTCTX == eCU_DPK_PushArray(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool)) )
    {
        (void)printf("eCU_DPKTST_CorruptContext 12 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_CorruptContext 12 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DPKTST_CorruptContext 13 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_CorruptContext 13 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.uMemPKL = 0u;

    if( e_eCU_DPK_RES_CORRUPTCTX == eCU_DPK_PushU8(&l_tCtx, 10u) )
    {
        (void)printf("eCU_DPKTST_CorruptContext 14 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_CorruptContext 14 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DPKTST_CorruptContext 15 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_CorruptContext 15 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.uMemPKL = 0u;

    if( e_eCU_DPK_RES_CORRUPTCTX == eCU_DPK_PushU16(&l_tCtx, 10u) )
    {
        (void)printf("eCU_DPKTST_CorruptContext 16 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_CorruptContext 16 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DPKTST_CorruptContext 17 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_CorruptContext 17 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.uMemPKL = 0u;

    if( e_eCU_DPK_RES_CORRUPTCTX == eCU_DPK_PushU32(&l_tCtx, 10u) )
    {
        (void)printf("eCU_DPKTST_CorruptContext 18 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_CorruptContext 18 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DPKTST_CorruptContext 19 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_CorruptContext 19 -- FAIL \n");
    }

    /* Init variable */
    l_tCtx.uMemPKL = 0u;

    if( e_eCU_DPK_RES_CORRUPTCTX == eCU_DPK_PushU64(&l_tCtx, 10u) )
    {
        (void)printf("eCU_DPKTST_CorruptContext 20 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_CorruptContext 20 -- FAIL \n");
    }
}

static void eCU_DPKTST_OutOfMem(void)
{
    /* Local variable */
    t_eCU_DPK_Ctx l_tCtx;
    uint8_t  l_auBadPointerMempool[9u];
    uint8_t  l_auArrayTest[5u];

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 1  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_PushU64( &l_tCtx, 10u ) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 2  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OUTOFMEM == eCU_DPK_PushU64( &l_tCtx, 10u ) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 3  -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, 7u, true) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 4  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OUTOFMEM == eCU_DPK_PushU64( &l_tCtx, 10u ) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 5  -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, 5u, true) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 6  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_PushU32( &l_tCtx, 10u ) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 7  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OUTOFMEM == eCU_DPK_PushU32( &l_tCtx, 10u ) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 8  -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, 3u, true) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 9  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 9  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OUTOFMEM == eCU_DPK_PushU32( &l_tCtx, 10u ) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 10 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 10 -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, 3u, true) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 11 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 11 -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_PushU16( &l_tCtx, 10u ) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 12 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 12 -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OUTOFMEM == eCU_DPK_PushU16( &l_tCtx, 10u ) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 13 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 13 -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, 1u, true) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 14 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 14 -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OUTOFMEM == eCU_DPK_PushU16( &l_tCtx, 10u ) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 15 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 15 -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, 1u, true) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 16 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 16 -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_PushU8( &l_tCtx, 10u ) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 17 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 17 -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OUTOFMEM == eCU_DPK_PushU8( &l_tCtx, 10u ) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 18 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 18 -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 19 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 19 -- FAIL \n");
    }

    (void)memset(l_auArrayTest, 0, sizeof(l_auArrayTest));
    if( e_eCU_DPK_RES_OK == eCU_DPK_PushArray( &l_tCtx, l_auArrayTest, sizeof(l_auArrayTest) ) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 20 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 20 -- FAIL \n");
    }

    (void)memset(l_auArrayTest, 0, sizeof(l_auArrayTest));
    if( e_eCU_DPK_RES_OUTOFMEM == eCU_DPK_PushArray( &l_tCtx, l_auArrayTest, sizeof(l_auArrayTest) ) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 21 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 21 -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, 4u, true) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 22 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 22 -- FAIL \n");
    }

    (void)memset(l_auArrayTest, 0, sizeof(l_auArrayTest));
    if( e_eCU_DPK_RES_OUTOFMEM == eCU_DPK_PushArray( &l_tCtx, l_auArrayTest, sizeof(l_auArrayTest) ) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 23 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 23 -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 24 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 24 -- FAIL \n");
    }

    (void)memset(l_auArrayTest, 0, sizeof(l_auArrayTest));
    if( e_eCU_DPK_RES_OK == eCU_DPK_PushArray( &l_tCtx, l_auArrayTest, sizeof(l_auArrayTest) ) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 25 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 25 -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_PushU8( &l_tCtx, 10u ) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 26 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 26 -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_PushU16( &l_tCtx, 10u ) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 27 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 27 -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OUTOFMEM == eCU_DPK_PushU32( &l_tCtx, 10u ) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 28 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 28 -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OUTOFMEM == eCU_DPK_PushU64( &l_tCtx, 10u ) )
    {
        (void)printf("eCU_DPKTST_OutOfMem 29 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_OutOfMem 29 -- FAIL \n");
    }
}

static void eCU_DPKTST_EndianLe(void)
{
    /* Local variable */
    t_eCU_DPK_Ctx l_tCtx;
    uint8_t  l_auBadPointerMempool[20u];
    uint32_t l_uVarTemp;
    uint8_t* l_puPointerP;

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DPKTST_EndianLe 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianLe 1  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_PushU16( &l_tCtx, 0x1234u ) )
    {
        (void)printf("eCU_DPKTST_EndianLe 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianLe 2  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_GetNPushed( &l_tCtx, &l_uVarTemp ) )
    {
        (void)printf("eCU_DPKTST_EndianLe 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianLe 3  -- FAIL \n");
    }

    if( 0x02u == l_uVarTemp)
    {
        if( ( 0x34u == l_auBadPointerMempool[0u] ) && ( 0x12u == l_auBadPointerMempool[1u] ) )
        {
            if( e_eCU_DPK_RES_OK == eCU_DPK_GetDataReference( &l_tCtx, &l_puPointerP, &l_uVarTemp ) )
            {
                if( ( 0x02u == l_uVarTemp ) && ( l_auBadPointerMempool == l_puPointerP ) )
                {
                    (void)printf("eCU_DPKTST_EndianLe 4  -- OK \n");
                }
                else
                {
                    (void)printf("eCU_DPKTST_EndianLe 4  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_DPKTST_EndianLe 4  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_DPKTST_EndianLe 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianLe 4  -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_StartNewPack( &l_tCtx ) )
    {
        (void)printf("eCU_DPKTST_EndianLe 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianLe 5  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_PushU32( &l_tCtx, 0x12345678u ) )
    {
        (void)printf("eCU_DPKTST_EndianLe 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianLe 6  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_GetNPushed( &l_tCtx, &l_uVarTemp ) )
    {
        (void)printf("eCU_DPKTST_EndianLe 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianLe 7  -- FAIL \n");
    }

    if( 0x04u == l_uVarTemp)
    {
        if( ( 0x78u == l_auBadPointerMempool[0u] ) && ( 0x56u == l_auBadPointerMempool[1u] ) &&
            ( 0x34u == l_auBadPointerMempool[2u] ) && ( 0x12u == l_auBadPointerMempool[3u] ) )
        {
            if( e_eCU_DPK_RES_OK == eCU_DPK_GetDataReference( &l_tCtx, &l_puPointerP, &l_uVarTemp ) )
            {
                if( ( 0x04u == l_uVarTemp ) && ( l_auBadPointerMempool == l_puPointerP ) )
                {
                    (void)printf("eCU_DPKTST_EndianLe 8  -- OK \n");
                }
                else
                {
                    (void)printf("eCU_DPKTST_EndianLe 8  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_DPKTST_EndianLe 8  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_DPKTST_EndianLe 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianLe 8  -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_StartNewPack( &l_tCtx ) )
    {
        (void)printf("eCU_DPKTST_EndianLe 9  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianLe 9  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_PushU64( &l_tCtx, 0x123456789ABCDEF0u ) )
    {
        (void)printf("eCU_DPKTST_EndianLe 10 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianLe 10 -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_GetNPushed( &l_tCtx, &l_uVarTemp ) )
    {
        (void)printf("eCU_DPKTST_EndianLe 11 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianLe 11 -- FAIL \n");
    }

    if( 0x08u == l_uVarTemp)
    {
        if( ( 0xF0u == l_auBadPointerMempool[0u] ) && ( 0xDEu == l_auBadPointerMempool[1u] ) &&
            ( 0xBCu == l_auBadPointerMempool[2u] ) && ( 0x9Au == l_auBadPointerMempool[3u] ) &&
            ( 0x78u == l_auBadPointerMempool[4u] ) && ( 0x56u == l_auBadPointerMempool[5u] ) &&
            ( 0x34u == l_auBadPointerMempool[6u] ) && ( 0x12u == l_auBadPointerMempool[7u] ))
        {
            if( e_eCU_DPK_RES_OK == eCU_DPK_GetDataReference( &l_tCtx, &l_puPointerP, &l_uVarTemp ) )
            {
                if( ( 0x08u == l_uVarTemp ) && ( l_auBadPointerMempool == l_puPointerP ) )
                {
                    (void)printf("eCU_DPKTST_EndianLe 12 -- OK \n");
                }
                else
                {
                    (void)printf("eCU_DPKTST_EndianLe 12 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_DPKTST_EndianLe 12 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_DPKTST_EndianLe 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianLe 12 -- FAIL \n");
    }
}

static void eCU_DPKTST_EndianBe(void)
{
    /* Local variable */
    t_eCU_DPK_Ctx l_tCtx;
    uint8_t  l_auBadPointerMempool[20u];
    uint32_t l_uVarTemp;
    uint8_t* l_puPointerP;

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), false) )
    {
        (void)printf("eCU_DPKTST_EndianBe 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianBe 1  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_PushU16( &l_tCtx, 0x1234u ) )
    {
        (void)printf("eCU_DPKTST_EndianBe 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianBe 2  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_GetNPushed( &l_tCtx, &l_uVarTemp ) )
    {
        (void)printf("eCU_DPKTST_EndianBe 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianBe 3  -- FAIL \n");
    }

    if( 0x02u == l_uVarTemp)
    {
        if( ( 0x12u == l_auBadPointerMempool[0u] ) && ( 0x34u == l_auBadPointerMempool[1u] ) )
        {
            if( e_eCU_DPK_RES_OK == eCU_DPK_GetDataReference( &l_tCtx, &l_puPointerP, &l_uVarTemp ) )
            {
                if( ( 0x02u == l_uVarTemp ) && ( l_auBadPointerMempool == l_puPointerP ) )
                {
                    (void)printf("eCU_DPKTST_EndianBe 4  -- OK \n");
                }
                else
                {
                    (void)printf("eCU_DPKTST_EndianBe 4  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_DPKTST_EndianBe 4  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_DPKTST_EndianBe 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianBe 4  -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_StartNewPack( &l_tCtx ) )
    {
        (void)printf("eCU_DPKTST_EndianBe 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianBe 5  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_PushU32( &l_tCtx, 0x12345678u ) )
    {
        (void)printf("eCU_DPKTST_EndianBe 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianBe 6  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_GetNPushed( &l_tCtx, &l_uVarTemp ) )
    {
        (void)printf("eCU_DPKTST_EndianBe 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianBe 7  -- FAIL \n");
    }

    if( 0x04u == l_uVarTemp)
    {
        if( ( 0x12u == l_auBadPointerMempool[0u] ) && ( 0x34u == l_auBadPointerMempool[1u] ) &&
            ( 0x56u == l_auBadPointerMempool[2u] ) && ( 0x78u == l_auBadPointerMempool[3u] ) )
        {
            if( e_eCU_DPK_RES_OK == eCU_DPK_GetDataReference( &l_tCtx, &l_puPointerP, &l_uVarTemp ) )
            {
                if( ( 0x04u == l_uVarTemp ) && ( l_auBadPointerMempool == l_puPointerP ) )
                {
                    (void)printf("eCU_DPKTST_EndianBe 8  -- OK \n");
                }
                else
                {
                    (void)printf("eCU_DPKTST_EndianBe 8  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_DPKTST_EndianBe 8  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_DPKTST_EndianBe 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianBe 8  -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_StartNewPack( &l_tCtx ) )
    {
        (void)printf("eCU_DPKTST_EndianBe 9  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianBe 9  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_PushU64( &l_tCtx, 0x123456789ABCDEF0u ) )
    {
        (void)printf("eCU_DPKTST_EndianBe 10 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianBe 10 -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_GetNPushed( &l_tCtx, &l_uVarTemp ) )
    {
        (void)printf("eCU_DPKTST_EndianBe 11 -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianBe 11 -- FAIL \n");
    }

    if( 0x08u == l_uVarTemp)
    {
        if( ( 0x12u == l_auBadPointerMempool[0u] ) && ( 0x34u == l_auBadPointerMempool[1u] ) &&
            ( 0x56u == l_auBadPointerMempool[2u] ) && ( 0x78u == l_auBadPointerMempool[3u] ) &&
            ( 0x9Au == l_auBadPointerMempool[4u] ) && ( 0xBCu == l_auBadPointerMempool[5u] ) &&
            ( 0xDEu == l_auBadPointerMempool[6u] ) && ( 0xF0u == l_auBadPointerMempool[7u] ))
        {
            if( e_eCU_DPK_RES_OK == eCU_DPK_GetDataReference( &l_tCtx, &l_puPointerP, &l_uVarTemp ) )
            {
                if( ( 0x08u == l_uVarTemp ) && ( l_auBadPointerMempool == l_puPointerP ) )
                {
                    (void)printf("eCU_DPKTST_EndianBe 12 -- OK \n");
                }
                else
                {
                    (void)printf("eCU_DPKTST_EndianBe 12 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_DPKTST_EndianBe 12 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_DPKTST_EndianBe 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DPKTST_EndianBe 12 -- FAIL \n");
    }
}

static void eCU_DPKTST_Cycle(void)
{
    /* Local variable */
    t_eCU_DPK_Ctx l_tCtx;
    uint8_t  l_auBadPointerMempool[20u];
    uint32_t l_uVarTemp;
    uint8_t* l_puPointerP;

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_InitCtx(&l_tCtx, l_auBadPointerMempool, sizeof(l_auBadPointerMempool), true) )
    {
        (void)printf("eCU_DPKTST_Cycle 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_Cycle 1  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_PushU16( &l_tCtx, 0x1234u ) )
    {
        (void)printf("eCU_DPKTST_Cycle 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_Cycle 2  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_PushU16( &l_tCtx, 0x5678u ) )
    {
        (void)printf("eCU_DPKTST_Cycle 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_Cycle 3  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_GetNPushed( &l_tCtx, &l_uVarTemp ) )
    {
        (void)printf("eCU_DPKTST_Cycle 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_Cycle 4  -- FAIL \n");
    }

    if( 0x04u == l_uVarTemp)
    {
        if( ( 0x34u == l_auBadPointerMempool[0u] ) && ( 0x12u == l_auBadPointerMempool[1u] ) &&
            ( 0x78u == l_auBadPointerMempool[2u] ) && ( 0x56u == l_auBadPointerMempool[3u] )  )
        {
            if( e_eCU_DPK_RES_OK == eCU_DPK_GetDataReference( &l_tCtx, &l_puPointerP, &l_uVarTemp ) )
            {
                if( ( 0x04u == l_uVarTemp ) && ( l_auBadPointerMempool == l_puPointerP ) )
                {
                    (void)printf("eCU_DPKTST_Cycle 5  -- OK \n");
                }
                else
                {
                    (void)printf("eCU_DPKTST_Cycle 5  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_DPKTST_Cycle 5  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_DPKTST_Cycle 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DPKTST_Cycle 5  -- FAIL \n");
    }

    /* Function */
    if( e_eCU_DPK_RES_OK == eCU_DPK_StartNewPack( &l_tCtx ) )
    {
        (void)printf("eCU_DPKTST_Cycle 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_Cycle 6  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_PushU16( &l_tCtx, 0x1234u ) )
    {
        (void)printf("eCU_DPKTST_Cycle 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_Cycle 7  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_PushU16( &l_tCtx, 0x5678u ) )
    {
        (void)printf("eCU_DPKTST_Cycle 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_Cycle 8  -- FAIL \n");
    }

    if( e_eCU_DPK_RES_OK == eCU_DPK_GetNPushed( &l_tCtx, &l_uVarTemp) )
    {
        (void)printf("eCU_DPKTST_Cycle 9  -- OK \n");
    }
    else
    {
        (void)printf("eCU_DPKTST_Cycle 9  -- FAIL \n");
    }

    if( 0x04u == l_uVarTemp)
    {
        if( ( 0x34u == l_auBadPointerMempool[0u] ) && ( 0x12u == l_auBadPointerMempool[1u] ) &&
            ( 0x78u == l_auBadPointerMempool[2u] ) && ( 0x56u == l_auBadPointerMempool[3u] )  )
        {
            if( e_eCU_DPK_RES_OK == eCU_DPK_GetDataReference( &l_tCtx, &l_puPointerP, &l_uVarTemp ) )
            {
                if( ( 0x04u == l_uVarTemp ) && ( l_auBadPointerMempool == l_puPointerP ) )
                {
                    (void)printf("eCU_DPKTST_Cycle 10 -- OK \n");
                }
                else
                {
                    (void)printf("eCU_DPKTST_Cycle 10 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_DPKTST_Cycle 10 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_DPKTST_Cycle 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_DPKTST_Cycle 10 -- FAIL \n");
    }
}