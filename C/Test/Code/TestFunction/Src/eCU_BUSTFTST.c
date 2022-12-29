/**
 * @file       eCU_BUSTFTST.c
 *
 * @brief      Byte unstuffer test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eCU_BUSTFTST.h"
#include "eCU_BSTF.h"
#include "eCU_BUNSTF.h"
#include <stdio.h>



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void eCU_BUSTFTST_BadPointer(void);
static void eCU_BUSTFTST_BadInit(void);
static void eCU_BUSTFTST_BadParamEntr(void);
static void eCU_BUSTFTST_CorrupterContext(void);
static void eCU_BUSTFTST_OutOfMem(void);
static void eCU_BUSTFTST_FrameEnd(void);
static void eCU_BUSTFTST_General(void);
static void eCU_BUSTFTST_CornerCase(void);
static void eCU_BUSTFTST_CornerCase2(void);
static void eCU_BUSTFTST_CodeCoverage(void);

/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eCU_BUSTFTST_ExeTest(void)
{
	(void)printf("\n\nBYTE UNSTUFFER TEST START \n\n");

    eCU_BUSTFTST_BadPointer();
    eCU_BUSTFTST_BadInit();
    eCU_BUSTFTST_BadParamEntr();
    eCU_BUSTFTST_CorrupterContext();
    eCU_BUSTFTST_OutOfMem();
    eCU_BUSTFTST_FrameEnd();
    eCU_BUSTFTST_General();
    eCU_BUSTFTST_CornerCase();
    eCU_BUSTFTST_CornerCase2();
    eCU_BUSTFTST_CodeCoverage();

    (void)printf("\n\nBYTE UNSTUFFER TEST END \n\n");
}





/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
static void eCU_BUSTFTST_BadPointer(void)
{
    /* Local variable */
    t_eCU_BUNSTF_Ctx l_tCtx;
    uint8_t  l_auMemArea[5u];
    uint32_t l_uVarTemp32;
    uint8_t *l_puPointerP;
    bool_t l_bFrameIsUnstuffed;
    bool_t l_bIsWaiting;
    bool_t l_bIsInit;

    /* Function */
    if( e_eCU_BUNSTF_RES_BADPOINTER == eCU_BUNSTF_InitCtx(NULL, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BUSTFTST_BadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadPointer 1  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADPOINTER == eCU_BUNSTF_InitCtx(&l_tCtx, NULL, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BUSTFTST_BadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadPointer 2  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADPOINTER == eCU_BUNSTF_NewFrame( NULL ) )
    {
        (void)printf("eCU_BUSTFTST_BadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadPointer 3  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADPOINTER == eCU_BUNSTF_GetUnstufData( NULL, &l_puPointerP, &l_uVarTemp32 ) )
    {
        (void)printf("eCU_BUSTFTST_BadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadPointer 4  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADPOINTER == eCU_BUNSTF_GetUnstufData( &l_tCtx, NULL, &l_uVarTemp32 ) )
    {
        (void)printf("eCU_BUSTFTST_BadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadPointer 5  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADPOINTER == eCU_BUNSTF_GetUnstufData( &l_tCtx, &l_puPointerP, NULL ) )
    {
        (void)printf("eCU_BUSTFTST_BadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadPointer 6  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADPOINTER == eCU_BUNSTF_GetUnstufLen( NULL, &l_uVarTemp32 ) )
    {
        (void)printf("eCU_BUSTFTST_BadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadPointer 7  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADPOINTER == eCU_BUNSTF_GetUnstufLen( &l_tCtx, NULL ) )
    {
        (void)printf("eCU_BUSTFTST_BadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadPointer 8  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADPOINTER == eCU_BUNSTF_IsWaitingSof( NULL, &l_bIsWaiting ) )
    {
        (void)printf("eCU_BUSTFTST_BadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadPointer 9  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADPOINTER == eCU_BUNSTF_IsWaitingSof( &l_tCtx, NULL ) )
    {
        (void)printf("eCU_BUSTFTST_BadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadPointer 10 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADPOINTER == eCU_BUNSTF_IsAFullFrameUnstuff( NULL, &l_bFrameIsUnstuffed ) )
    {
        (void)printf("eCU_BUSTFTST_BadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadPointer 11 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADPOINTER == eCU_BUNSTF_IsAFullFrameUnstuff( &l_tCtx, NULL ) )
    {
        (void)printf("eCU_BUSTFTST_BadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadPointer 12 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADPOINTER == eCU_BUNSTF_IsFrameBad( NULL, &l_bFrameIsUnstuffed ) )
    {
        (void)printf("eCU_BUSTFTST_BadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadPointer 13 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADPOINTER == eCU_BUNSTF_IsFrameBad( &l_tCtx, NULL ) )
    {
        (void)printf("eCU_BUSTFTST_BadPointer 14 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadPointer 14 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADPOINTER == eCU_BUNSTF_InsStufChunk( NULL, l_auMemArea, sizeof(l_auMemArea), &l_uVarTemp32 ) )
    {
        (void)printf("eCU_BUSTFTST_BadPointer 15 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadPointer 15 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADPOINTER == eCU_BUNSTF_InsStufChunk( &l_tCtx, NULL, sizeof(l_auMemArea), &l_uVarTemp32 ) )
    {
        (void)printf("eCU_BUSTFTST_BadPointer 16 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadPointer 16 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADPOINTER == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auMemArea, sizeof(l_auMemArea), NULL) )
    {
        (void)printf("eCU_BUSTFTST_BadPointer 17 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadPointer 17 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADPOINTER == eCU_BUNSTF_IsInit( NULL, &l_bIsInit ) )
    {
        (void)printf("eCU_BUSTFTST_BadPointer 18 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadPointer 18 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADPOINTER == eCU_BUNSTF_IsInit( &l_tCtx, NULL ) )
    {
        (void)printf("eCU_BUSTFTST_BadPointer 19 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadPointer 19 -- FAIL \n");
    }
}

static void eCU_BUSTFTST_BadInit(void)
{
    /* Local variable */
    t_eCU_BUNSTF_Ctx l_tCtx;
    uint8_t  l_auMemArea[5u];
    uint32_t l_uVarTemp32;
    uint8_t *l_puPointerP;
    bool_t l_bFrameIsUnstuffed;
    bool_t l_bIsWaiting;
    bool_t l_bIsInit;

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_BUNSTF_RES_NOINITLIB == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_BadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadInit 1  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_NOINITLIB == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
    {
        (void)printf("eCU_BUSTFTST_BadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadInit 2  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_NOINITLIB == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
    {
        (void)printf("eCU_BUSTFTST_BadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadInit 3  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_NOINITLIB == eCU_BUNSTF_IsWaitingSof(&l_tCtx, &l_bIsWaiting ) )
    {
        (void)printf("eCU_BUSTFTST_BadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadInit 4  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_NOINITLIB == eCU_BUNSTF_IsAFullFrameUnstuff(&l_tCtx, &l_bFrameIsUnstuffed) )
    {
        (void)printf("eCU_BUSTFTST_BadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadInit 5  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_NOINITLIB == eCU_BUNSTF_IsFrameBad(&l_tCtx, &l_bFrameIsUnstuffed) )
    {
        (void)printf("eCU_BUSTFTST_BadInit 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadInit 6  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_NOINITLIB == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auMemArea, sizeof(l_auMemArea), &l_uVarTemp32 ) )
    {
        (void)printf("eCU_BUSTFTST_BadInit 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadInit 7  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsInit( &l_tCtx, &l_bIsInit ) )
    {
        if( false == l_bIsInit )
        {
            (void)printf("eCU_BUSTFTST_BadInit 8  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BUSTFTST_BadInit 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadInit 8  -- FAIL \n");
    }
}

static void eCU_BUSTFTST_BadParamEntr(void)
{
    /* Local variable */
    t_eCU_BUNSTF_Ctx l_tCtx;
    uint8_t  l_auMemArea[5u];
    uint32_t l_uVarTemp32;
    bool_t l_bIsInit;

    /* Function */
    if( e_eCU_BUNSTF_RES_BADPARAM == eCU_BUNSTF_InitCtx(&l_tCtx, l_auMemArea, 0u) )
    {
        (void)printf("eCU_BUSTFTST_BadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadParamEntr 1  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BUSTFTST_BadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadParamEntr 2  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADPARAM == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auMemArea, 0u, &l_uVarTemp32 ) )
    {
        (void)printf("eCU_BUSTFTST_BadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadParamEntr 3  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsInit( &l_tCtx, &l_bIsInit ) )
    {
        if( true == l_bIsInit )
        {
            (void)printf("eCU_BUSTFTST_BadParamEntr 4  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BUSTFTST_BadParamEntr 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_BadParamEntr 4  -- FAIL \n");
    }
}

static void eCU_BUSTFTST_CorrupterContext(void)
{
    /* Local variable */
    t_eCU_BUNSTF_Ctx l_tCtx;
    uint8_t  l_auMemArea[5u];
    uint32_t l_uVarTemp32;
    uint8_t *l_puPointerP;
    bool_t l_bFrameIsUnstuffed;
    bool_t l_bIsWaiting;

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 1  -- FAIL \n");
    }

    l_tCtx.uBuffL = 0u;
    if( e_eCU_BUNSTF_RES_CORRUPTCTX == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 2  -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 3  -- FAIL \n");
    }

    l_tCtx.puBuff = NULL;
    if( e_eCU_BUNSTF_RES_CORRUPTCTX == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 4  -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 5  -- FAIL \n");
    }

    l_tCtx.uFrameCtr = l_tCtx.uBuffL + 1u;
    if( e_eCU_BUNSTF_RES_CORRUPTCTX == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 6  -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 7  -- FAIL \n");
    }

    l_tCtx.uFrameCtr = 1u;
    l_tCtx.eSM = e_eCU_BUNSTFPRV_SM_NEEDSOF;
    if( e_eCU_BUNSTF_RES_CORRUPTCTX == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 8  -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 9  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 9  -- FAIL \n");
    }

    l_tCtx.uFrameCtr = 0u;
    l_tCtx.eSM = e_eCU_BUNSTFPRV_SM_UNSTUFFEND;
    if( e_eCU_BUNSTF_RES_CORRUPTCTX == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 10 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 10 -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 11 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 11 -- FAIL \n");
    }

    l_tCtx.uBuffL = 0u;
    if( e_eCU_BUNSTF_RES_CORRUPTCTX == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 12 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 12 -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 13 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 13 -- FAIL \n");
    }

    l_tCtx.uBuffL = 0u;
    if( e_eCU_BUNSTF_RES_CORRUPTCTX == eCU_BUNSTF_GetUnstufData( &l_tCtx, &l_puPointerP, &l_uVarTemp32 ) )
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 14 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 14 -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 15 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 15 -- FAIL \n");
    }

    l_tCtx.uBuffL = 0u;
    if( e_eCU_BUNSTF_RES_CORRUPTCTX == eCU_BUNSTF_IsWaitingSof(&l_tCtx, &l_bIsWaiting) )
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 16 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 16 -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 17 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 17 -- FAIL \n");
    }

    l_tCtx.uBuffL = 0u;
    if( e_eCU_BUNSTF_RES_CORRUPTCTX == eCU_BUNSTF_IsAFullFrameUnstuff(&l_tCtx, &l_bFrameIsUnstuffed) )
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 18 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 18 -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 19 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 19 -- FAIL \n");
    }

    l_tCtx.uBuffL = 0u;
    if( e_eCU_BUNSTF_RES_CORRUPTCTX == eCU_BUNSTF_IsFrameBad(&l_tCtx, &l_bFrameIsUnstuffed) )
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 20 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 20 -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 21 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 21 -- FAIL \n");
    }

    l_tCtx.uBuffL = 0u;
    if( e_eCU_BUNSTF_RES_CORRUPTCTX == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auMemArea, 9u, &l_uVarTemp32 ) )
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 22 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CorrupterContext 22 -- FAIL \n");
    }
}

static void eCU_BUSTFTST_OutOfMem(void)
{
    /* Local variable */
    t_eCU_BUNSTF_Ctx l_tCtx;
    uint8_t  l_auMemArea[5u];
    uint8_t  l_auStuffed[10u];
    uint32_t l_uVarTemp32;
    uint8_t* l_puPointerP;
    bool_t l_bFrIsUnstuf;
    bool_t l_bIsWaiting;
    bool_t l_bIsBad;

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BUSTFTST_OutOfMem 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_OutOfMem 1  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsWaitingSof(&l_tCtx, &l_bIsWaiting) )
    {
        if( true == l_bIsWaiting )
        {
            (void)printf("eCU_BUSTFTST_OutOfMem 2  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BUSTFTST_OutOfMem 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_OutOfMem 2  -- FAIL \n");
    }

    l_auStuffed[0u] = ECU_SOF;
    l_auStuffed[1u] = 0x01u;
    l_auStuffed[2u] = 0x02u;
    l_auStuffed[3u] = 0x03u;
    l_auStuffed[4u] = 0x04u;
    l_auStuffed[5u] = 0x01u;
    l_auStuffed[6u] = ECU_ESC;
    l_auStuffed[7u] = 0x03u;
    l_auStuffed[8u] = ECU_EOF;

    if( e_eCU_BUNSTF_RES_OUTOFMEM == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, 9u, &l_uVarTemp32 ) )
    {
        if( 7u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_OutOfMem 3  -- FAIL \n");
        }
        else
        {
            if( ( 0x01u != l_auMemArea[0u] ) || ( 0x02u != l_auMemArea[1u] ) || ( 0x03u != l_auMemArea[2u] ) ||
                ( 0x04u != l_auMemArea[3u] ) || ( 0x01u != l_auMemArea[4u] )  )
            {
                (void)printf("eCU_BUSTFTST_OutOfMem 3  -- FAIL \n");
            }
            else
            {
                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
                {
                    if( 5u == l_uVarTemp32 )
                    {
                        if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                        {
                            if( ( 5u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                            {
                                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsAFullFrameUnstuff(&l_tCtx, &l_bFrIsUnstuf) )
                                {
                                    if( false == l_bFrIsUnstuf)
                                    {
                                        (void)printf("eCU_BUSTFTST_OutOfMem 3  -- OK \n");
                                    }
                                    else
                                    {
                                        (void)printf("eCU_BUSTFTST_OutOfMem 3  -- FAIL \n");
                                    }
                                }
                                else
                                {
                                    (void)printf("eCU_BUSTFTST_OutOfMem 3  -- FAIL \n");
                                }
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_OutOfMem 3  -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_OutOfMem 3  -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_OutOfMem 3  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_OutOfMem 3  -- FAIL \n");
                }
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_OutOfMem 3  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsWaitingSof(&l_tCtx, &l_bIsWaiting) )
    {
        if( false == l_bIsWaiting )
        {
            (void)printf("eCU_BUSTFTST_OutOfMem 4  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BUSTFTST_OutOfMem 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_OutOfMem 4  -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_OutOfMem 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_OutOfMem 5  -- FAIL \n");
    }

    l_auStuffed[0u] = ECU_SOF;
    l_auStuffed[1u] = 0x01u;
    l_auStuffed[2u] = 0x02u;
    l_auStuffed[3u] = 0x03u;
    l_auStuffed[4u] = 0x04u;
    l_auStuffed[5u] = 0x05u;
    l_auStuffed[6u] = 0x06u;
    l_auStuffed[7u] = ECU_EOF;

    if( e_eCU_BUNSTF_RES_OUTOFMEM == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, 7u, &l_uVarTemp32 ) )
    {
        if( 6u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_OutOfMem 6  -- FAIL \n");
        }
        else
        {
            if( ( 0x01u != l_auMemArea[0u] ) || ( 0x02u != l_auMemArea[1u] ) || ( 0x03u != l_auMemArea[2u] ) ||
                ( 0x04u != l_auMemArea[3u] ) || ( 0x05u != l_auMemArea[4u] )  )
            {
                (void)printf("eCU_BUSTFTST_OutOfMem 6  -- FAIL \n");
            }
            else
            {
                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
                {
                    if( 5u == l_uVarTemp32 )
                    {
                        if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                        {
                            if( ( 5u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                            {
                                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsAFullFrameUnstuff(&l_tCtx, &l_bFrIsUnstuf) )
                                {
                                    if( false == l_bFrIsUnstuf)
                                    {
                                        (void)printf("eCU_BUSTFTST_OutOfMem 6  -- OK \n");
                                    }
                                    else
                                    {
                                        (void)printf("eCU_BUSTFTST_OutOfMem 6  -- FAIL \n");
                                    }
                                }
                                else
                                {
                                    (void)printf("eCU_BUSTFTST_OutOfMem 6  -- FAIL \n");
                                }
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_OutOfMem 6  -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_OutOfMem 6  -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_OutOfMem 6  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_OutOfMem 6  -- FAIL \n");
                }
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_OutOfMem 6  -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_OutOfMem 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_OutOfMem 7  -- FAIL \n");
    }

    l_auStuffed[0u] = ECU_SOF;
    l_auStuffed[1u] = 0x01u;
    l_auStuffed[2u] = 0x02u;
    l_auStuffed[3u] = 0x03u;
    l_auStuffed[4u] = 0x04u;
    l_auStuffed[5u] = 0x01u;
    l_auStuffed[6u] = ECU_ESC;
    l_auStuffed[7u] = 0x03u;
    l_auStuffed[8u] = ECU_EOF;

    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, 3u, &l_uVarTemp32 ) )
    {
        if( 3u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_OutOfMem 8  -- FAIL \n");
        }
        else
        {
            if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
            {
                if( 2u == l_uVarTemp32 )
                {
                    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                    {
                        if( ( 2u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                        {
                            (void)printf("eCU_BUSTFTST_OutOfMem 8  -- OK \n");
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_OutOfMem 8  -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_OutOfMem 8  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_OutOfMem 8  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_OutOfMem 8  -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_OutOfMem 8  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InsStufChunk( &l_tCtx, &l_auStuffed[3u], 4u, &l_uVarTemp32 ) )
    {
        if( 4u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_OutOfMem 9  -- FAIL \n");
        }
        else
        {
            if( ( 0x01u != l_auMemArea[0u] ) || ( 0x02u != l_auMemArea[1u] ) || ( 0x03u != l_auMemArea[2u] ) ||
                ( 0x04u != l_auMemArea[3u] ) || ( 0x01u != l_auMemArea[4u] )  )
            {
                (void)printf("eCU_BUSTFTST_OutOfMem 9  -- FAIL \n");
            }
            else
            {
                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
                {
                    if( 5u == l_uVarTemp32 )
                    {
                        if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                        {
                            if( ( 5u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                            {
                                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsAFullFrameUnstuff(&l_tCtx, &l_bFrIsUnstuf) )
                                {
                                    if( false == l_bFrIsUnstuf)
                                    {
                                        (void)printf("eCU_BUSTFTST_OutOfMem 9  -- OK \n");
                                    }
                                    else
                                    {
                                        (void)printf("eCU_BUSTFTST_OutOfMem 9  -- FAIL \n");
                                    }
                                }
                                else
                                {
                                    (void)printf("eCU_BUSTFTST_OutOfMem 9  -- FAIL \n");
                                }
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_OutOfMem 9  -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_OutOfMem 9  -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_OutOfMem 9  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_OutOfMem 9  -- FAIL \n");
                }
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_OutOfMem 9  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_OUTOFMEM == eCU_BUNSTF_InsStufChunk( &l_tCtx, &l_auStuffed[7u], 1u, &l_uVarTemp32 ) )
    {
        if( 0u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_OutOfMem 10 -- FAIL \n");
        }
        else
        {
            if( ( 0x01u != l_auMemArea[0u] ) || ( 0x02u != l_auMemArea[1u] ) || ( 0x03u != l_auMemArea[2u] ) ||
                ( 0x04u != l_auMemArea[3u] ) || ( 0x01u != l_auMemArea[4u] )  )
            {
                (void)printf("eCU_BUSTFTST_OutOfMem 10 -- FAIL \n");
            }
            else
            {
                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
                {
                    if( 5u == l_uVarTemp32 )
                    {
                        if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                        {
                            if( ( 5u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                            {
                                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsAFullFrameUnstuff(&l_tCtx, &l_bFrIsUnstuf) )
                                {
                                    if( false == l_bFrIsUnstuf)
                                    {
                                        (void)printf("eCU_BUSTFTST_OutOfMem 10 -- OK \n");
                                    }
                                    else
                                    {
                                        (void)printf("eCU_BUSTFTST_OutOfMem 10 -- FAIL \n");
                                    }
                                }
                                else
                                {
                                    (void)printf("eCU_BUSTFTST_OutOfMem 10 -- FAIL \n");
                                }
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_OutOfMem 10 -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_OutOfMem 10 -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_OutOfMem 10 -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_OutOfMem 10 -- FAIL \n");
                }
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_OutOfMem 10 -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_OutOfMem 11 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_OutOfMem 11 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsFrameBad(&l_tCtx, &l_bIsBad) )
    {
        if( false == l_bIsBad )
        {
            (void)printf("eCU_BUSTFTST_OutOfMem 12 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BUSTFTST_OutOfMem 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_OutOfMem 12 -- FAIL \n");
    }

    l_auStuffed[0u] = ECU_SOF;
    l_auStuffed[1u] = 0x01u;
    l_auStuffed[2u] = 0x02u;
    l_auStuffed[3u] = 0x03u;
    l_auStuffed[4u] = 0x04u;
    l_auStuffed[5u] = 0x05u;
    l_auStuffed[6u] = 0x06u;
    l_auStuffed[7u] = ECU_EOF;

    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, 3u, &l_uVarTemp32 ) )
    {
        if( 3u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_OutOfMem 13 -- FAIL \n");
        }
        else
        {
            if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
            {
                if( 2u == l_uVarTemp32 )
                {
                    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                    {
                        if( ( 2u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                        {
                            if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsAFullFrameUnstuff(&l_tCtx, &l_bFrIsUnstuf) )
                            {
                                if( false == l_bFrIsUnstuf)
                                {
                                    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsFrameBad(&l_tCtx, &l_bIsBad) )
                                    {
                                        if( false == l_bIsBad)
                                        {
                                            (void)printf("eCU_BUSTFTST_OutOfMem 13 -- OK \n");
                                        }
                                        else
                                        {
                                            (void)printf("eCU_BUSTFTST_OutOfMem 13 -- FAIL \n");
                                        }
                                    }
                                    else
                                    {
                                        (void)printf("eCU_BUSTFTST_OutOfMem 13 -- FAIL \n");
                                    }
                                }
                                else
                                {
                                    (void)printf("eCU_BUSTFTST_OutOfMem 13 -- FAIL \n");
                                }
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_OutOfMem 13 -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_OutOfMem 13 -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_OutOfMem 13 -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_OutOfMem 13 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_OutOfMem 13 -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_OutOfMem 13 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_OUTOFMEM == eCU_BUNSTF_InsStufChunk( &l_tCtx, &l_auStuffed[3u], 4u, &l_uVarTemp32 ) )
    {
        if( 3u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_OutOfMem 14 -- FAIL \n");
        }
        else
        {
            if( ( 0x01u != l_auMemArea[0u] ) || ( 0x02u != l_auMemArea[1u] ) || ( 0x03u != l_auMemArea[2u] ) ||
                ( 0x04u != l_auMemArea[3u] ) || ( 0x05u != l_auMemArea[4u] )  )
            {
                (void)printf("eCU_BUSTFTST_OutOfMem 14 -- FAIL \n");
            }
            else
            {
                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
                {
                    if( 5u == l_uVarTemp32 )
                    {
                        if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                        {
                            if( ( 5u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                            {
                                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsAFullFrameUnstuff(&l_tCtx, &l_bFrIsUnstuf) )
                                {
                                    if( false == l_bFrIsUnstuf)
                                    {
                                        if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsFrameBad(&l_tCtx, &l_bIsBad) )
                                        {
                                            if( false == l_bIsBad)
                                            {
                                                (void)printf("eCU_BUSTFTST_OutOfMem 14 -- OK \n");
                                            }
                                            else
                                            {
                                                (void)printf("eCU_BUSTFTST_OutOfMem 14 -- FAIL \n");
                                            }
                                        }
                                        else
                                        {
                                            (void)printf("eCU_BUSTFTST_OutOfMem 14 -- FAIL \n");
                                        }
                                    }
                                    else
                                    {
                                        (void)printf("eCU_BUSTFTST_OutOfMem 14 -- FAIL \n");
                                    }
                                }
                                else
                                {
                                    (void)printf("eCU_BUSTFTST_OutOfMem 14 -- FAIL \n");
                                }
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_OutOfMem 14 -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_OutOfMem 14 -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_OutOfMem 14 -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_OutOfMem 14 -- FAIL \n");
                }
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_OutOfMem 14 -- FAIL \n");
    }
}

static void eCU_BUSTFTST_FrameEnd(void)
{
    /* Local variable */
    t_eCU_BUNSTF_Ctx l_tCtx;
    uint8_t  l_auMemArea[5u];
    uint8_t  l_auStuffed[10u];
    uint32_t l_uVarTemp32;
    uint8_t* l_puPointerP;
    bool_t l_bFrIsUnstuf;
    bool_t l_bIsBad;

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BUSTFTST_FrameEnd 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_FrameEnd 1  -- FAIL \n");
    }

    l_auStuffed[0u] = ECU_SOF;
    l_auStuffed[1u] = 0x01u;
    l_auStuffed[2u] = 0x02u;
    l_auStuffed[3u] = 0x03u;
    l_auStuffed[4u] = 0x04u;
    l_auStuffed[5u] = 0x01u;
    l_auStuffed[6u] = ECU_EOF;
    l_auStuffed[7u] = ECU_SOF;


    if( e_eCU_BUNSTF_RES_FRAMEENDED == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, sizeof(l_auStuffed), &l_uVarTemp32 ) )
    {
        if( 7u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_FrameEnd 2  -- FAIL \n");
        }
        else
        {
            if( ( 0x01u != l_auMemArea[0u] ) || ( 0x02u != l_auMemArea[1u] ) || ( 0x03u != l_auMemArea[2u] ) ||
                ( 0x04u != l_auMemArea[3u] ) || ( 0x01u != l_auMemArea[4u] )  )
            {
                (void)printf("eCU_BUSTFTST_FrameEnd 2  -- FAIL \n");
            }
            else
            {
                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
                {
                    if( 5u == l_uVarTemp32 )
                    {
                        if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                        {
                            if( ( 5u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                            {
                                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsAFullFrameUnstuff(&l_tCtx, &l_bFrIsUnstuf) )
                                {
                                    if( true == l_bFrIsUnstuf)
                                    {
                                        if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsFrameBad(&l_tCtx, &l_bIsBad) )
                                        {
                                            if( false == l_bIsBad)
                                            {
                                                (void)printf("eCU_BUSTFTST_FrameEnd 2  -- OK \n");
                                            }
                                            else
                                            {
                                                (void)printf("eCU_BUSTFTST_FrameEnd 2  -- FAIL \n");
                                            }
                                        }
                                        else
                                        {
                                            (void)printf("eCU_BUSTFTST_FrameEnd 2  -- FAIL \n");
                                        }
                                    }
                                    else
                                    {
                                        (void)printf("eCU_BUSTFTST_FrameEnd 2  -- FAIL \n");
                                    }
                                }
                                else
                                {
                                    (void)printf("eCU_BUSTFTST_FrameEnd 2  -- FAIL \n");
                                }
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_FrameEnd 2  -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_FrameEnd 2  -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_FrameEnd 2  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_FrameEnd 2  -- FAIL \n");
                }
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_FrameEnd 2  -- FAIL \n");
    }


    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_FrameEnd 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_FrameEnd 3  -- FAIL \n");
    }

    l_auStuffed[0u] = ECU_SOF;
    l_auStuffed[1u] = 0x01u;
    l_auStuffed[2u] = ECU_ESC;
    l_auStuffed[3u] = (uint8_t)(~ECU_SOF);
    l_auStuffed[4u] = 0x02u;
    l_auStuffed[5u] = ECU_EOF;

    if( e_eCU_BUNSTF_RES_FRAMEENDED == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, 6u, &l_uVarTemp32 ) )
    {
        if( 6u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_FrameEnd 4  -- FAIL \n");
        }
        else
        {
            if( ( 0x01u != l_auMemArea[0u] ) || ( ECU_SOF != l_auMemArea[1u] ) || ( 0x02u != l_auMemArea[2u] ) )
            {
                (void)printf("eCU_BUSTFTST_FrameEnd 4  -- FAIL \n");
            }
            else
            {
                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
                {
                    if( 3u == l_uVarTemp32 )
                    {
                        if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                        {
                            if( ( 3u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                            {
                                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsAFullFrameUnstuff(&l_tCtx, &l_bFrIsUnstuf) )
                                {
                                    if( true == l_bFrIsUnstuf)
                                    {
                                        (void)printf("eCU_BUSTFTST_FrameEnd 4  -- OK \n");
                                    }
                                    else
                                    {
                                        (void)printf("eCU_BUSTFTST_FrameEnd 4  -- FAIL \n");
                                    }
                                }
                                else
                                {
                                    (void)printf("eCU_BUSTFTST_FrameEnd 4  -- FAIL \n");
                                }
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_FrameEnd 4  -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_FrameEnd 4  -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_FrameEnd 4  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_FrameEnd 4  -- FAIL \n");
                }
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_FrameEnd 4  -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_FrameEnd 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_FrameEnd 5  -- FAIL \n");
    }

    l_auStuffed[0u] = ECU_SOF;
    l_auStuffed[1u] = 0x01u;
    l_auStuffed[2u] = 0x02u;
    l_auStuffed[3u] = 0x03u;
    l_auStuffed[4u] = 0x04u;
    l_auStuffed[5u] = 0x01u;
    l_auStuffed[6u] = ECU_EOF;
    l_auStuffed[7u] = ECU_SOF;

    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, 3u, &l_uVarTemp32 ) )
    {
        if( 3u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_FrameEnd 6  -- FAIL \n");
        }
        else
        {
            if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
            {
                if( 2u == l_uVarTemp32 )
                {
                    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                    {
                        if( ( 2u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                        {
                            if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsAFullFrameUnstuff(&l_tCtx, &l_bFrIsUnstuf) )
                            {
                                if( false == l_bFrIsUnstuf)
                                {
                                    (void)printf("eCU_BUSTFTST_FrameEnd 6  -- OK \n");
                                }
                                else
                                {
                                    (void)printf("eCU_BUSTFTST_FrameEnd 6  -- FAIL \n");
                                }
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_FrameEnd 6  -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_FrameEnd 6  -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_FrameEnd 6  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_FrameEnd 6  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_FrameEnd 6  -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_FrameEnd 6  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_FRAMEENDED == eCU_BUNSTF_InsStufChunk( &l_tCtx, &l_auStuffed[3u], 6u, &l_uVarTemp32 ) )
    {
        if( 4u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_FrameEnd 7  -- FAIL \n");
        }
        else
        {
            if( ( 0x01u != l_auMemArea[0u] ) || ( 0x02u != l_auMemArea[1u] ) || ( 0x03u != l_auMemArea[2u] ) ||
                ( 0x04u != l_auMemArea[3u] ) || ( 0x01u != l_auMemArea[4u] )  )
            {
                (void)printf("eCU_BUSTFTST_FrameEnd 7  -- FAIL \n");
            }
            else
            {
                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
                {
                    if( 5u == l_uVarTemp32 )
                    {
                        if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                        {
                            if( ( 5u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                            {
                                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsAFullFrameUnstuff(&l_tCtx, &l_bFrIsUnstuf) )
                                {
                                    if( true == l_bFrIsUnstuf)
                                    {
                                        (void)printf("eCU_BUSTFTST_FrameEnd 7  -- OK \n");
                                    }
                                    else
                                    {
                                        (void)printf("eCU_BUSTFTST_FrameEnd 7  -- FAIL \n");
                                    }
                                }
                                else
                                {
                                    (void)printf("eCU_BUSTFTST_FrameEnd 7  -- FAIL \n");
                                }
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_FrameEnd 7  -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_FrameEnd 7  -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_FrameEnd 7  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_FrameEnd 7  -- FAIL \n");
                }
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_FrameEnd 7  -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_FrameEnd 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_FrameEnd 8  -- FAIL \n");
    }

    l_auStuffed[0u] = ECU_SOF;
    l_auStuffed[1u] = 0x01u;
    l_auStuffed[2u] = ECU_ESC;
    l_auStuffed[3u] = (uint8_t)(~ECU_SOF);
    l_auStuffed[4u] = 0x02u;
    l_auStuffed[5u] = ECU_EOF;

    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, 3u, &l_uVarTemp32 ) )
    {
        if( 3u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_FrameEnd 9  -- FAIL \n");
        }
        else
        {
            if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
            {
                if( 1u == l_uVarTemp32 )
                {
                    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                    {
                        if( ( 1u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                        {
                            if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsAFullFrameUnstuff(&l_tCtx, &l_bFrIsUnstuf) )
                            {
                                if( false == l_bFrIsUnstuf)
                                {
                                    (void)printf("eCU_BUSTFTST_FrameEnd 9  -- OK \n");
                                }
                                else
                                {
                                    (void)printf("eCU_BUSTFTST_FrameEnd 9  -- FAIL \n");
                                }
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_FrameEnd 9  -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_FrameEnd 9  -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_FrameEnd 9  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_FrameEnd 9  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_FrameEnd 9  -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_FrameEnd 9  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_FRAMEENDED == eCU_BUNSTF_InsStufChunk( &l_tCtx, &l_auStuffed[3u], 3u, &l_uVarTemp32 ) )
    {
        if( 3u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_FrameEnd 10 -- FAIL \n");
        }
        else
        {
            if( ( 0x01u != l_auMemArea[0u] ) || ( ECU_SOF != l_auMemArea[1u] ) || ( 0x02u != l_auMemArea[2u] ) )
            {
                (void)printf("eCU_BUSTFTST_FrameEnd 10 -- FAIL \n");
            }
            else
            {
                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
                {
                    if( 3u == l_uVarTemp32 )
                    {
                        if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                        {
                            if( ( 3u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                            {
                                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsAFullFrameUnstuff(&l_tCtx, &l_bFrIsUnstuf) )
                                {
                                    if( true == l_bFrIsUnstuf)
                                    {
                                        (void)printf("eCU_BUSTFTST_FrameEnd 10 -- OK \n");
                                    }
                                    else
                                    {
                                        (void)printf("eCU_BUSTFTST_FrameEnd 10 -- FAIL \n");
                                    }
                                }
                                else
                                {
                                    (void)printf("eCU_BUSTFTST_FrameEnd 10 -- FAIL \n");
                                }
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_FrameEnd 10 -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_FrameEnd 10 -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_FrameEnd 10 -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_FrameEnd 10 -- FAIL \n");
                }
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_FrameEnd 10 -- FAIL \n");
    }
}

static void eCU_BUSTFTST_General(void)
{
    /* Local variable */
    t_eCU_BUNSTF_Ctx l_tCtx;
    uint8_t  l_auMemArea[50u];
    uint8_t  l_auStuffed[10u];
    uint32_t l_uVarTemp32;
    uint8_t* l_puPointerP;
    bool_t l_bIsBad;

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BUSTFTST_General 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_General 1  -- FAIL \n");
    }

    l_auStuffed[0u] = ECU_SOF;
    l_auStuffed[1u] = ECU_EOF;

    if( e_eCU_BUNSTF_RES_BADFRAME == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, 2u, &l_uVarTemp32 ) )
    {
        if( 2u != l_uVarTemp32  )
        {
            (void)printf("eCU_BUSTFTST_General 2  -- FAIL \n");
        }
        else
        {
            (void)printf("eCU_BUSTFTST_General 2  -- OK \n");
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_General 2  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADFRAME == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, 1u, &l_uVarTemp32 ) )
    {
        if( 0u != l_uVarTemp32  )
        {
            (void)printf("eCU_BUSTFTST_General 3  -- FAIL \n");
        }
        else
        {
            (void)printf("eCU_BUSTFTST_General 3  -- OK \n");
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_General 3  -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_General 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_General 4  -- FAIL \n");
    }

    l_auStuffed[0u] = ECU_SOF;
    l_auStuffed[1u] = ECU_SOF;
    l_auStuffed[2u] = ECU_ESC;
    l_auStuffed[3u] = (uint8_t)(~ECU_SOF);
    l_auStuffed[4u] = ECU_EOF;

    if( e_eCU_BUNSTF_RES_FRAMERESTART == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, 5u, &l_uVarTemp32 ) )
    {
        if( 2u !=  l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_General 5  -- FAIL \n");
        }
        else
        {
            if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
            {
                if( 0u == l_uVarTemp32 )
                {
                    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                    {
                        if( ( 0u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                        {
                            if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsFrameBad(&l_tCtx, &l_bIsBad) )
                            {
                                if( false == l_bIsBad)
                                {
                                    (void)printf("eCU_BUSTFTST_General 5  -- OK \n");
                                }
                                else
                                {
                                    (void)printf("eCU_BUSTFTST_General 5  -- FAIL \n");
                                }
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_General 5  -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_General 5  -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_General 5  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_General 5  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_General 5  -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_General 5  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_FRAMEENDED == eCU_BUNSTF_InsStufChunk( &l_tCtx, &l_auStuffed[2u], 3u, &l_uVarTemp32 ) )
    {
        if( 3u !=  l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_General 6  -- FAIL \n");
        }
        else
        {
            if( ECU_SOF == l_auMemArea[0u] )
            {
                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
                {
                    if( 1u == l_uVarTemp32 )
                    {
                        if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                        {
                            if( ( 1u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                            {
                                (void)printf("eCU_BUSTFTST_General 6  -- OK \n");
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_General 6  -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_General 6  -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_General 6  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_General 6  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_General 6  -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_General 6  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_FRAMEENDED == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, 3u, &l_uVarTemp32 ) )
    {
        if( 0u !=  l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_General 7  -- FAIL \n");
        }
        else
        {
            if( ECU_SOF == l_auMemArea[0u] )
            {
                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
                {
                    if( 1u == l_uVarTemp32 )
                    {
                        if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                        {
                            if( ( 1u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                            {
                                (void)printf("eCU_BUSTFTST_General 7  -- OK \n");
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_General 7  -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_General 7  -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_General 7  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_General 7  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_General 7  -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_General 7  -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_General 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_General 8  -- FAIL \n");
    }

    l_auStuffed[0u] = ECU_SOF;
    l_auStuffed[1u] = ECU_ESC;
    l_auStuffed[2u] = 0x02u;
    l_auStuffed[3u] = 0x02u;
    l_auStuffed[4u] = ECU_EOF;

    if( e_eCU_BUNSTF_RES_BADFRAME == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, 5u, &l_uVarTemp32 ) )
    {
        if( 3u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_General 9  -- FAIL \n");
        }
        else
        {
            if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
            {
                if( 0u == l_uVarTemp32 )
                {
                    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                    {
                        if( ( 0u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                        {
                            if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsFrameBad(&l_tCtx, &l_bIsBad) )
                            {
                                if( true == l_bIsBad)
                                {
                                    (void)printf("eCU_BUSTFTST_General 9  -- OK \n");
                                }
                                else
                                {
                                    (void)printf("eCU_BUSTFTST_General 9  -- FAIL \n");
                                }
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_General 9  -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_General 9  -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_General 9  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_General 9  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_General 9  -- FAIL \n");
            }

        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_General 9  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_General 10 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_General 10 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADFRAME == eCU_BUNSTF_InsStufChunk( &l_tCtx, &l_auStuffed[3u], 2u, &l_uVarTemp32 ) )
    {
        if( 1u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_General 11 -- FAIL \n");
        }
        else
        {
            if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
            {
                if( 0u == l_uVarTemp32 )
                {
                    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                    {
                        if( ( 0u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                        {
                            (void)printf("eCU_BUSTFTST_General 11 -- OK \n");
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_General 11 -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_General 11 -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_General 11 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_General 11 -- FAIL \n");
            }

        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_General 11 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_General 12 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_General 12 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsFrameBad(&l_tCtx, &l_bIsBad) )
    {
        if( false == l_bIsBad)
        {
            (void)printf("eCU_BUSTFTST_General 13 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BUSTFTST_General 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_General 13 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADFRAME == eCU_BUNSTF_InsStufChunk( &l_tCtx, &l_auStuffed[4u], 1u, &l_uVarTemp32 ) )
    {
        if( 1u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_General 14 -- FAIL \n");
        }
        else
        {
            if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
            {
                if( 0u == l_uVarTemp32 )
                {
                    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                    {
                        if( ( 0u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                        {
                            if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_IsFrameBad(&l_tCtx, &l_bIsBad) )
                            {
                                if( true == l_bIsBad)
                                {
                                    (void)printf("eCU_BUSTFTST_General 14 -- OK \n");
                                }
                                else
                                {
                                    (void)printf("eCU_BUSTFTST_General 14 -- FAIL \n");
                                }
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_General 14 -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_General 14 -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_General 14 -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_General 14 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_General 14 -- FAIL \n");
            }

        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_General 14 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame( &l_tCtx ) )
    {
        (void)printf("eCU_BUSTFTST_General 15 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_General 15 -- FAIL \n");
    }

}

static void eCU_BUSTFTST_CornerCase(void)
{
    /* Local variable */
    t_eCU_BUNSTF_Ctx l_tCtx;
    uint8_t  l_auMemArea[5u];
    uint8_t  l_auStuffed[10u];
    uint32_t l_uVarTemp32;
    uint8_t* l_puPointerP;

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BUSTFTST_CornerCase 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 1  -- FAIL \n");
    }

    l_auStuffed[0u] = 0x02u;
    l_auStuffed[1u] = ECU_SOF;
    l_auStuffed[2u] = 0x02u;
    l_auStuffed[3u] = 0x03u;
    l_auStuffed[4u] = 0x04u;
    l_auStuffed[5u] = 0x01u;
    l_auStuffed[6u] = ECU_EOF;

    if( e_eCU_BUNSTF_RES_BADFRAME == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, sizeof(l_auStuffed), &l_uVarTemp32 ) )
    {
        if( 1u !=  l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_CornerCase 2  -- FAIL \n");
        }
        else
        {
            if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
            {
                if( 0u == l_uVarTemp32 )
                {
                    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                    {
                        if( 0u == l_uVarTemp32 )
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase 2  -- OK \n");
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase 2  -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_CornerCase 2  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_CornerCase 2  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_CornerCase 2  -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 2  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BUSTFTST_CornerCase 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 3  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_FRAMEENDED == eCU_BUNSTF_InsStufChunk( &l_tCtx, &l_auStuffed[1], sizeof(l_auStuffed)-1u, &l_uVarTemp32 ) )
    {
        if( 6u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_CornerCase 4  -- FAIL \n");
        }
        else
        {
            if( ( 0x02u == l_auMemArea[0u] ) && ( 0x03u == l_auMemArea[1u] ) && ( 0x04u == l_auMemArea[2u] ) &&
                ( 0x01u == l_auMemArea[3u] ) )
            {
                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
                {
                    if( 4u == l_uVarTemp32 )
                    {
                        if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                        {
                            if( ( 4u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                            {
                                (void)printf("eCU_BUSTFTST_CornerCase 4  -- OK \n");
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_CornerCase 4  -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase 4  -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_CornerCase 4  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_CornerCase 4  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_CornerCase 4  -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 4  -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_CornerCase 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 5  -- FAIL \n");
    }

    l_auStuffed[0u] = ECU_SOF;
    l_auStuffed[1u] = ECU_SOF;
    l_auStuffed[2u] = 0x01u;
    l_auStuffed[3u] = 0x02u;
    l_auStuffed[4u] = 0x03u;
    l_auStuffed[5u] = 0x04u;
    l_auStuffed[6u] = ECU_EOF;

    if( e_eCU_BUNSTF_RES_FRAMERESTART == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, sizeof(l_auStuffed), &l_uVarTemp32 ) )
    {
        if( 2u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_CornerCase 6  -- FAIL \n");
        }
        else
        {
            if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
            {
                if( 0u == l_uVarTemp32 )
                {
                    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                    {
                        if( ( 0u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase 6  -- OK \n");
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase 6  -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_CornerCase 6  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_CornerCase 6  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_CornerCase 6  -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 6  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_FRAMEENDED == eCU_BUNSTF_InsStufChunk( &l_tCtx, &l_auStuffed[2u], sizeof(l_auStuffed)-1u, &l_uVarTemp32 ) )
    {
        if( 5u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_CornerCase 7  -- FAIL \n");
        }
        else
        {
            if( ( 0x01u == l_auMemArea[0u] ) && ( 0x02u == l_auMemArea[1u] ) && ( 0x03u == l_auMemArea[2u] ) &&
                ( 0x04u == l_auMemArea[3u] ) )
            {
                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
                {
                    if( 4u == l_uVarTemp32 )
                    {
                        if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                        {
                            if( ( 4u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                            {
                                (void)printf("eCU_BUSTFTST_CornerCase 7  -- OK \n");
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_CornerCase 7  -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase 7  -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_CornerCase 7  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_CornerCase 7  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_CornerCase 7  -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 7  -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_CornerCase 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 8  -- FAIL \n");
    }

    l_auStuffed[0u] = ECU_SOF;
    l_auStuffed[1u] = ECU_EOF;
    l_auStuffed[2u] = ECU_SOF;
    l_auStuffed[3u] = 0x02u;
    l_auStuffed[4u] = 0x03u;
    l_auStuffed[5u] = 0x04u;
    l_auStuffed[6u] = ECU_EOF;

    if( e_eCU_BUNSTF_RES_BADFRAME == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, sizeof(l_auStuffed), &l_uVarTemp32 ) )
    {
        if( 2u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_CornerCase 9  -- FAIL \n");
        }
        else
        {
            if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
            {
                if( 0u == l_uVarTemp32 )
                {
                    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                    {
                        if( ( 0u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase 9  -- OK \n");
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase 9  -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_CornerCase 9  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_CornerCase 9  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_CornerCase 9  -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 9  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_CornerCase 10 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 10 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_FRAMEENDED == eCU_BUNSTF_InsStufChunk( &l_tCtx, &l_auStuffed[2u], sizeof(l_auStuffed)-2u, &l_uVarTemp32 ) )
    {
        if( 5u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_CornerCase 11 -- FAIL \n");
        }
        else
        {
            if( ( 0x02u == l_auMemArea[0u] ) && ( 0x03u == l_auMemArea[1u] ) && ( 0x04u == l_auMemArea[2u] ) )
            {
                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
                {
                    if( 3u == l_uVarTemp32 )
                    {
                        if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                        {
                            if( ( 3u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                            {
                                (void)printf("eCU_BUSTFTST_CornerCase 11 -- OK \n");
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_CornerCase 11 -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase 11 -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_CornerCase 11 -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_CornerCase 11 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_CornerCase 11 -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 11 -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_CornerCase 12 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 12 -- FAIL \n");
    }

    l_auStuffed[0u] = ECU_SOF;
    l_auStuffed[1u] = 0x02u;
    l_auStuffed[2u] = ECU_ESC;
    l_auStuffed[3u] = ECU_EOF;
    l_auStuffed[4u] = ECU_SOF;
    l_auStuffed[5u] = 0x02u;
    l_auStuffed[6u] = ECU_EOF;

    if( e_eCU_BUNSTF_RES_BADFRAME == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, sizeof(l_auStuffed), &l_uVarTemp32 ) )
    {
        if( 4u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_CornerCase 13 -- FAIL \n");
        }
        else
        {
            if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
            {
                if( 1u == l_uVarTemp32 )
                {
                    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                    {
                        if( ( 1u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase 13 -- OK \n");
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase 13 -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_CornerCase 13 -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_CornerCase 13 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_CornerCase 13 -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 13 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_CornerCase 14 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 14 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_FRAMEENDED == eCU_BUNSTF_InsStufChunk( &l_tCtx, &l_auStuffed[4u], sizeof(l_auStuffed), &l_uVarTemp32 ) )
    {
        if( 3u !=  l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_CornerCase 15 -- FAIL \n");
        }
        else
        {
            if( 0x02u == l_auMemArea[0u] )
            {
                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
                {
                    if( 1u == l_uVarTemp32 )
                    {
                        if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                        {
                            if( ( 1u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                            {
                                (void)printf("eCU_BUSTFTST_CornerCase 15 -- OK \n");
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_CornerCase 15 -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase 15 -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_CornerCase 15 -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_CornerCase 15 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_CornerCase 15 -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 15 -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_CornerCase 16 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 16 -- FAIL \n");
    }

    l_auStuffed[0u] = ECU_SOF;
    l_auStuffed[1u] = ECU_ESC;
    l_auStuffed[2u] = ECU_ESC;
    l_auStuffed[3u] = ECU_SOF;
    l_auStuffed[4u] = 0x03u;
    l_auStuffed[5u] = 0x02u;
    l_auStuffed[6u] = ECU_EOF;

    if( e_eCU_BUNSTF_RES_BADFRAME == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, sizeof(l_auStuffed), &l_uVarTemp32 ) )
    {
        if( 3u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_CornerCase 17 -- FAIL \n");
        }
        else
        {
            if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
            {
                if( 0u == l_uVarTemp32 )
                {
                    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                    {
                        if( ( 0u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase 17 -- OK \n");
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase 17 -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_CornerCase 17 -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_CornerCase 17 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_CornerCase 17 -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 17 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_CornerCase 18 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 18 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_FRAMEENDED == eCU_BUNSTF_InsStufChunk( &l_tCtx, &l_auStuffed[3u], sizeof(l_auStuffed), &l_uVarTemp32 ) )
    {
        if( 4u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_CornerCase 19 -- FAIL \n");
        }
        else
        {
            if( ( 0x03u == l_auMemArea[0u] ) && ( 0x02u == l_auMemArea[1u] ) )
            {
                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
                {
                    if( 2u == l_uVarTemp32 )
                    {
                        if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                        {
                            if( ( 2u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                            {
                                (void)printf("eCU_BUSTFTST_CornerCase 19 -- OK \n");
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_CornerCase 19 -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase 19 -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_CornerCase 19 -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_CornerCase 19 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_CornerCase 19 -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 19 -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_CornerCase 20 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 20 -- FAIL \n");
    }

    l_auStuffed[0u] = ECU_SOF;
    l_auStuffed[1u] = ECU_ESC;
    l_auStuffed[2u] = 0x01u;
    l_auStuffed[3u] = ECU_EOF;
    l_auStuffed[4u] = ECU_SOF;
    l_auStuffed[5u] = 0x03u;
    l_auStuffed[6u] = ECU_EOF;

    if( e_eCU_BUNSTF_RES_BADFRAME == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, sizeof(l_auStuffed), &l_uVarTemp32 ) )
    {
        if( 3u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_CornerCase 21 -- FAIL \n");
        }
        else
        {
            if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
            {
                if( 0u == l_uVarTemp32 )
                {
                    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                    {
                        if( ( 0u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase 21 -- OK \n");
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase 21 -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_CornerCase 21 -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_CornerCase 21 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_CornerCase 21 -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 21 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_CornerCase 22 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 22 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_BADFRAME == eCU_BUNSTF_InsStufChunk( &l_tCtx, &l_auStuffed[3u], sizeof(l_auStuffed), &l_uVarTemp32 ) )
    {
        if( 1u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_CornerCase 23 -- FAIL \n");
        }
        else
        {
            if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
            {
                if( 0u == l_uVarTemp32 )
                {
                    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                    {
                        if( ( 0u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase 23 -- OK \n");
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase 23 -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_CornerCase 23 -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_CornerCase 23 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_CornerCase 23 -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 23 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_CornerCase 24 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 24 -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_FRAMEENDED == eCU_BUNSTF_InsStufChunk( &l_tCtx, &l_auStuffed[4u], sizeof(l_auStuffed), &l_uVarTemp32 ) )
    {
        if( 3u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_CornerCase 25 -- FAIL \n");
        }
        else
        {
            if( 0x03u == l_auMemArea[0u] )
            {
                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
                {
                    if( 1u == l_uVarTemp32 )
                    {
                        if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                        {
                            if( ( 1u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                            {
                                (void)printf("eCU_BUSTFTST_CornerCase 25 -- OK \n");
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_CornerCase 25 -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase 25 -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_CornerCase 25 -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_CornerCase 25 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_CornerCase 25 -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase 25 -- FAIL \n");
    }
}

static void eCU_BUSTFTST_CornerCase2(void)
{
    /* Local variable */
    t_eCU_BUNSTF_Ctx l_tCtx;
    uint8_t  l_auMemArea[5u];
    uint8_t  l_auStuffed[10u];
    uint32_t l_uVarTemp32;
    uint8_t* l_puPointerP;

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BUSTFTST_CornerCase2 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase2 1  -- FAIL \n");
    }

    l_auStuffed[0u] = ECU_SOF;
    l_auStuffed[1u] = 0x02u;
    l_auStuffed[2u] = ECU_ESC;
    l_auStuffed[3u] = ECU_SOF;
    l_auStuffed[4u] = 0x04u;
    l_auStuffed[5u] = 0x01u;
    l_auStuffed[6u] = ECU_EOF;

    if( e_eCU_BUNSTF_RES_FRAMERESTART == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, sizeof(l_auStuffed), &l_uVarTemp32 ) )
    {
        if( 4u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_CornerCase2 2  -- FAIL \n");
        }
        else
        {
            if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
            {
                if( 0u == l_uVarTemp32 )
                {
                    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                    {
                        if( ( 0u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase2 2  -- OK \n");
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase2 2  -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_CornerCase2 2  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_CornerCase2 2  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_CornerCase2 2  -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase2 2  -- FAIL \n");
    }

    if( e_eCU_BUNSTF_RES_FRAMEENDED == eCU_BUNSTF_InsStufChunk( &l_tCtx, &l_auStuffed[4u], sizeof(l_auStuffed), &l_uVarTemp32 ) )
    {
        if( 3u != l_uVarTemp32 )
        {
            (void)printf("eCU_BUSTFTST_CornerCase2 3  -- FAIL \n");
        }
        else
        {
            if( ( 0x04u == l_auMemArea[0u] ) && ( 0x01u == l_auMemArea[1u] ) )
            {
                if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufLen(&l_tCtx, &l_uVarTemp32) )
                {
                    if( 2u == l_uVarTemp32 )
                    {
                        if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_GetUnstufData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
                        {
                            if( ( 2u == l_uVarTemp32 ) && ( l_auMemArea == l_puPointerP) )
                            {
                                (void)printf("eCU_BUSTFTST_CornerCase2 3  -- OK \n");
                            }
                            else
                            {
                                (void)printf("eCU_BUSTFTST_CornerCase2 3  -- FAIL \n");
                            }
                        }
                        else
                        {
                            (void)printf("eCU_BUSTFTST_CornerCase2 3  -- FAIL \n");
                        }
                    }
                    else
                    {
                        (void)printf("eCU_BUSTFTST_CornerCase2 3  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eCU_BUSTFTST_CornerCase2 3  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BUSTFTST_CornerCase2 3  -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CornerCase2 3  -- FAIL \n");
    }

}

static void eCU_BUSTFTST_CodeCoverage(void)
{
    /* Local variable */
    t_eCU_BUNSTF_Ctx l_tCtx;
    uint8_t  l_auMemArea[50u];
    uint8_t  l_auStuffed[10u];
    uint32_t l_uVarTemp32;

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BUSTFTST_CodeCoverage 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CodeCoverage 1  -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_CodeCoverage 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CodeCoverage 2  -- FAIL \n");
    }

    l_auStuffed[0u] = ECU_SOF;
    l_auStuffed[1u] = ECU_ESC;
    l_auStuffed[2u] = (uint8_t)(~ECU_SOF);
    l_auStuffed[3u] = ECU_EOF;

    if( e_eCU_BUNSTF_RES_FRAMEENDED == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, sizeof(l_auStuffed), &l_uVarTemp32 ) )
    {
        if( ( (ECU_SOF == l_auMemArea[0u]) ) && ( 4u == l_uVarTemp32 ) )
        {
            (void)printf("eCU_BUSTFTST_CodeCoverage 3  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BUSTFTST_CodeCoverage 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CodeCoverage 3  -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_CodeCoverage 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CodeCoverage 4  -- FAIL \n");
    }

    l_auStuffed[0u] = ECU_SOF;
    l_auStuffed[1u] = ECU_ESC;
    l_auStuffed[2u] = (uint8_t)(~ECU_ESC);
    l_auStuffed[3u] = ECU_EOF;

    if( e_eCU_BUNSTF_RES_FRAMEENDED == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, sizeof(l_auStuffed), &l_uVarTemp32 ) )
    {
        if( ( (ECU_ESC == l_auMemArea[0u]) ) && ( 4u == l_uVarTemp32 ) )
        {
            (void)printf("eCU_BUSTFTST_CodeCoverage 5  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BUSTFTST_CodeCoverage 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CodeCoverage 5  -- FAIL \n");
    }

    /* Function  */
    if( e_eCU_BUNSTF_RES_OK == eCU_BUNSTF_NewFrame(&l_tCtx) )
    {
        (void)printf("eCU_BUSTFTST_CodeCoverage 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CodeCoverage 6  -- FAIL \n");
    }

    l_auStuffed[0u] = ECU_SOF;
    l_auStuffed[1u] = ECU_ESC;
    l_auStuffed[2u] = (uint8_t)(~ECU_EOF);
    l_auStuffed[3u] = ECU_EOF;

    if( e_eCU_BUNSTF_RES_FRAMEENDED == eCU_BUNSTF_InsStufChunk( &l_tCtx, l_auStuffed, sizeof(l_auStuffed), &l_uVarTemp32 ) )
    {
        if( ( (ECU_EOF == l_auMemArea[0u]) ) && ( 4u == l_uVarTemp32 ) )
        {
            (void)printf("eCU_BUSTFTST_CodeCoverage 7  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BUSTFTST_CodeCoverage 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BUSTFTST_CodeCoverage 7  -- FAIL \n");
    }
}