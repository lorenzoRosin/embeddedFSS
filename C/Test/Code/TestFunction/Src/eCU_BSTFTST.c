/**
 * @file       eCU_BSTFTST.c
 *
 * @brief      Byte stuffer test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eCU_BSTFTST.h"
#include "eCU_BSTF.h"
#include <stdio.h>

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "CERT-STR32-C"
    /* Suppressed for code clarity in test execution*/
#endif



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void eCU_BSTFTST_BadPointer(void);
static void eCU_BSTFTST_BadInit(void);
static void eCU_BSTFTST_BadInitFrame(void);
static void eCU_BSTFTST_BadParamEntr(void);
static void eCU_BSTFTST_CorrupterContext(void);
static void eCU_BSTFTST_OutOfMem(void);
static void eCU_BSTFTST_StartRestart(void);
static void eCU_BSTFTST_GetRemainings(void);
static void eCU_BSTFTST_General(void);
static void eCU_BSTFTST_GeneralAnother(void);
static void eCU_BSTFTST_Correcteness(void);

/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eCU_BSTFTST_ExeTest(void)
{
	(void)printf("\n\nBYTE STUFFER TEST START \n\n");

    eCU_BSTFTST_BadPointer();
    eCU_BSTFTST_BadInit();
    eCU_BSTFTST_BadInitFrame();
    eCU_BSTFTST_BadParamEntr();
    eCU_BSTFTST_CorrupterContext();
    eCU_BSTFTST_OutOfMem();
    eCU_BSTFTST_StartRestart();
    eCU_BSTFTST_GetRemainings();
    eCU_BSTFTST_General();
    eCU_BSTFTST_GeneralAnother();
    eCU_BSTFTST_Correcteness();

    (void)printf("\n\nBYTE STUFFER TEST END \n\n");
}



/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
static void eCU_BSTFTST_BadPointer(void)
{
    /* Local variable */
    t_eCU_BSTF_Ctx l_tCtx;
    uint8_t  l_auMemArea[5u];
    uint32_t l_uVarTemp32;
    uint8_t* l_puPointer;
    bool_t l_bIsInit;


    /* Function */
    if( e_eCU_BSTF_RES_BADPOINTER == eCU_BSTF_InitCtx(NULL, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_BadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadPointer 1  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_BADPOINTER == eCU_BSTF_InitCtx(&l_tCtx, NULL, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_BadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadPointer 2  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_BADPOINTER == eCU_BSTF_NewFrame(NULL, 2u) )
    {
        (void)printf("eCU_BSTFTST_BadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadPointer 3  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_BADPOINTER == eCU_BSTF_GetWherePutData(NULL, &l_puPointer, &l_uVarTemp32) )
    {
        (void)printf("eCU_BSTFTST_BadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadPointer 4  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_BADPOINTER == eCU_BSTF_GetWherePutData(&l_tCtx, NULL, &l_uVarTemp32) )
    {
        (void)printf("eCU_BSTFTST_BadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadPointer 5  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_BADPOINTER == eCU_BSTF_GetWherePutData(&l_tCtx, &l_puPointer, NULL) )
    {
        (void)printf("eCU_BSTFTST_BadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadPointer 6  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_BADPOINTER == eCU_BSTF_RestartFrame(NULL) )
    {
        (void)printf("eCU_BSTFTST_BadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadPointer 7  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_BADPOINTER == eCU_BSTF_GetRemByteToGet(NULL, &l_uVarTemp32) )
    {
        (void)printf("eCU_BSTFTST_BadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadPointer 8  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_BADPOINTER == eCU_BSTF_GetRemByteToGet(&l_tCtx, NULL) )
    {
        (void)printf("eCU_BSTFTST_BadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadPointer 9  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_BADPOINTER == eCU_BSTF_GetStufChunk(NULL, l_auMemArea, sizeof(l_auMemArea), &l_uVarTemp32 ) )
    {
        (void)printf("eCU_BSTFTST_BadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadPointer 10 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_BADPOINTER == eCU_BSTF_GetStufChunk(&l_tCtx, NULL, sizeof(l_auMemArea), &l_uVarTemp32) )
    {
        (void)printf("eCU_BSTFTST_BadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadPointer 11 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_BADPOINTER == eCU_BSTF_GetStufChunk(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), NULL) )
    {
        (void)printf("eCU_BSTFTST_BadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadPointer 12 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_BADPOINTER == eCU_BSTF_IsInit( NULL, &l_bIsInit ) )
    {
        (void)printf("eCU_BSTFTST_BadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadPointer 13 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_BADPOINTER == eCU_BSTF_IsInit( &l_tCtx, NULL ) )
    {
        (void)printf("eCU_BSTFTST_BadPointer 14 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadPointer 14 -- FAIL \n");
    }
}

static void eCU_BSTFTST_BadInit(void)
{
    /* Local variable */
    t_eCU_BSTF_Ctx l_tCtx;
    uint8_t  l_auMemArea[5u];
    uint32_t l_uVarTemp32;
    uint8_t* l_puPointer;
    bool_t l_bIsInit;

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_BSTF_RES_NOINITLIB == eCU_BSTF_NewFrame(&l_tCtx, 2u) )
    {
        (void)printf("eCU_BSTFTST_BadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadInit 1  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_NOINITLIB == eCU_BSTF_GetWherePutData(&l_tCtx, &l_puPointer, &l_uVarTemp32) )
    {
        (void)printf("eCU_BSTFTST_BadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadInit 2  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_NOINITLIB == eCU_BSTF_RestartFrame(&l_tCtx) )
    {
        (void)printf("eCU_BSTFTST_BadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadInit 3  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_NOINITLIB == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        (void)printf("eCU_BSTFTST_BadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadInit 4  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_NOINITLIB == eCU_BSTF_GetStufChunk(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), &l_uVarTemp32) )
    {
        (void)printf("eCU_BSTFTST_BadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadInit 5  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_IsInit( &l_tCtx, &l_bIsInit ) )
    {
        if( false == l_bIsInit )
        {
            (void)printf("eCU_BSTFTST_BadInit 6  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_BadInit 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadInit 6  -- FAIL \n");
    }

}

static void eCU_BSTFTST_BadInitFrame(void)
{
    /* Local variable */
    t_eCU_BSTF_Ctx l_tCtx;
    uint8_t  l_auMemArea[5u];
    uint32_t l_uVarTemp32;
    bool_t l_bIsInit;

    /* Init variable */
    l_tCtx.bIsInit = false;
    (void)memset(l_auMemArea, 0, sizeof(l_auMemArea));

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_BadInitFrame 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadInitFrame 1  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_NOINITFRAME == eCU_BSTF_RestartFrame(&l_tCtx) )
    {
        (void)printf("eCU_BSTFTST_BadInitFrame 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadInitFrame 2  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_NOINITFRAME == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        (void)printf("eCU_BSTFTST_BadInitFrame 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadInitFrame 3  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_NOINITFRAME == eCU_BSTF_GetStufChunk(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), &l_uVarTemp32) )
    {
        (void)printf("eCU_BSTFTST_BadInitFrame 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadInitFrame 4  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, 2u) )
    {
        (void)printf("eCU_BSTFTST_BadInitFrame 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadInitFrame 5  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_RestartFrame(&l_tCtx) )
    {
        (void)printf("eCU_BSTFTST_BadInitFrame 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadInitFrame 6  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_IsInit( &l_tCtx, &l_bIsInit ) )
    {
        if( true == l_bIsInit )
        {
            (void)printf("eCU_BSTFTST_BadInitFrame 7  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_BadInitFrame 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadInitFrame 7  -- FAIL \n");
    }
}

static void eCU_BSTFTST_BadParamEntr(void)
{
    /* Local variable */
    t_eCU_BSTF_Ctx l_tCtx;
    uint8_t  l_auMemArea[5u];
    uint32_t l_uVarTemp32;

    /* Init variable */
    l_tCtx.bIsInit = false;
    (void)memset(l_auMemArea, 0, sizeof(l_auMemArea));

    /* Function */
    if( e_eCU_BSTF_RES_BADPARAM == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, 0u) )
    {
        (void)printf("eCU_BSTFTST_BadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadParamEntr 1  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_BadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadParamEntr 2  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_BADPARAM == eCU_BSTF_NewFrame(&l_tCtx, 0u) )
    {
        (void)printf("eCU_BSTFTST_BadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadParamEntr 3  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_BADPARAM == eCU_BSTF_NewFrame(&l_tCtx, (sizeof(l_auMemArea) +1u) ) )
    {
        (void)printf("eCU_BSTFTST_BadParamEntr 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadParamEntr 4  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_BadParamEntr 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadParamEntr 5  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_BADPARAM == eCU_BSTF_GetStufChunk(&l_tCtx, l_auMemArea, 0u, &l_uVarTemp32) )
    {
        (void)printf("eCU_BSTFTST_BadParamEntr 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_BadParamEntr 6  -- FAIL \n");
    }
}

static void eCU_BSTFTST_CorrupterContext(void)
{
    /* Local variable */
    t_eCU_BSTF_Ctx l_tCtx;
    uint8_t  l_auMemArea[5u];
    uint32_t l_uVarTemp32;
    uint8_t* l_puPointerP;

    /* Init variable */
    l_tCtx.bIsInit = false;

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 1  -- FAIL \n");
    }

    l_tCtx.uBuffL = 0u;
    if( e_eCU_BSTF_RES_CORRUPTCTX == eCU_BSTF_NewFrame(&l_tCtx, 4u) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 2  -- FAIL \n");
    }


    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 3  -- FAIL \n");
    }

    l_tCtx.puBuff = NULL;
    if( e_eCU_BSTF_RES_CORRUPTCTX == eCU_BSTF_NewFrame(&l_tCtx, 4u) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 4  -- FAIL \n");
    }

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 5  -- FAIL \n");
    }

    l_tCtx.uFrameL = 2u;
    l_tCtx.uFrameCtr = l_tCtx.uFrameL + 1u;
    if( e_eCU_BSTF_RES_CORRUPTCTX == eCU_BSTF_NewFrame(&l_tCtx, 4u) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 6  -- FAIL \n");
    }

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 7  -- FAIL \n");
    }

    l_tCtx.uFrameL = l_tCtx.uBuffL + 1u;
    if( e_eCU_BSTF_RES_CORRUPTCTX == eCU_BSTF_NewFrame(&l_tCtx, 4u) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 8  -- FAIL \n");
    }

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 9  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 9  -- FAIL \n");
    }

    l_tCtx.uFrameL = 2u;
    l_tCtx.uFrameCtr = 1u;
    if( e_eCU_BSTF_RES_CORRUPTCTX == eCU_BSTF_NewFrame(&l_tCtx, 4u) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 10 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 10 -- FAIL \n");
    }

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 11 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 11 -- FAIL \n");
    }

    l_tCtx.uFrameCtr = 0u;
    l_tCtx.eSM = e_eCU_BSTFPRV_SM_NEEDNEGATEPRECDATA;
    if( e_eCU_BSTF_RES_CORRUPTCTX == eCU_BSTF_NewFrame(&l_tCtx, 4u) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 12 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 12 -- FAIL \n");
    }

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 13 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 13 -- FAIL \n");
    }

    l_tCtx.uFrameCtr = 0u;
    l_tCtx.eSM = e_eCU_BSTFPRV_SM_NEEDEOF;
    if( e_eCU_BSTF_RES_CORRUPTCTX == eCU_BSTF_NewFrame(&l_tCtx, 4u) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 14 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 14 -- FAIL \n");
    }

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 15 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 15 -- FAIL \n");
    }

    l_tCtx.uFrameCtr = 0u;
    l_tCtx.eSM = e_eCU_BSTFPRV_SM_STUFFEND;
    if( e_eCU_BSTF_RES_CORRUPTCTX == eCU_BSTF_NewFrame(&l_tCtx, 4u) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 16 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 16 -- FAIL \n");
    }

     /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 17 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 17 -- FAIL \n");
    }

    l_tCtx.eSM = e_eCU_BSTFPRV_SM_STUFFEND;
    l_tCtx.uFrameCtr = 1u;
    l_tCtx.uFrameL = 2u;
    l_tCtx.uBuffL = 3u;
    if( e_eCU_BSTF_RES_CORRUPTCTX == eCU_BSTF_NewFrame(&l_tCtx, 4u) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 18 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 18 -- FAIL \n");
    }

     /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 19 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 19 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, 4u) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 20 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 20 -- FAIL \n");
    }

    l_tCtx.eSM = e_eCU_BSTFPRV_SM_NEEDNEGATEPRECDATA;
    l_tCtx.uFrameCtr = 1u;
    l_auMemArea[l_tCtx.uFrameCtr - 1u] = 0xFFu;
    if( e_eCU_BSTF_RES_CORRUPTCTX == eCU_BSTF_NewFrame(&l_tCtx, 4u) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 21 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 21 -- FAIL \n");
    }

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 22 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 22 -- FAIL \n");
    }

    l_tCtx.uBuffL = 0u;
    if( e_eCU_BSTF_RES_CORRUPTCTX == eCU_BSTF_GetWherePutData(&l_tCtx, &l_puPointerP, &l_uVarTemp32) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 23 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 23 -- FAIL \n");
    }


    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 24 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 24 -- FAIL \n");
    }

    l_tCtx.uBuffL = 0u;
    l_tCtx.uFrameL = 1u;
    if( e_eCU_BSTF_RES_CORRUPTCTX == eCU_BSTF_RestartFrame(&l_tCtx) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 25 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 25 -- FAIL \n");
    }

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 26 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 26 -- FAIL \n");
    }

    l_tCtx.uBuffL = 0u;
    l_tCtx.uFrameL = 1u;
    if( e_eCU_BSTF_RES_CORRUPTCTX == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 27 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 27 -- FAIL \n");
    }

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 28 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 28 -- FAIL \n");
    }

    l_tCtx.uBuffL = 0u;
    l_tCtx.uFrameL = 1u;
    if( e_eCU_BSTF_RES_CORRUPTCTX == eCU_BSTF_GetStufChunk(&l_tCtx, l_auMemArea, 1u, &l_uVarTemp32) )
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 29 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_CorrupterContext 29 -- FAIL \n");
    }
}

static void eCU_BSTFTST_OutOfMem(void)
{
    /* Local variable */
    t_eCU_BSTF_Ctx l_tCtx;
    uint8_t  l_auMemArea[5u];
    uint8_t  l_auMemAreaFinalChunk[50u];
    uint32_t l_uVarTemp32;
    uint32_t l_uReman;
    uint8_t* l_puPointer;

    /* Init variable */
    l_auMemArea[0u] = ECU_SOF;
    l_auMemArea[1u] = ECU_EOF;
    l_auMemArea[2u] = ECU_ESC;
    l_auMemArea[3u] = 0x12u;
    l_auMemArea[4u] = 0xFFu;

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_OutOfMem 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_OutOfMem 1  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_NOINITFRAME == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uReman) )
    {
        (void)printf("eCU_BSTFTST_OutOfMem 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_OutOfMem 2  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetWherePutData(&l_tCtx, &l_puPointer, &l_uVarTemp32) )
    {
        if( l_auMemArea == l_puPointer )
        {
            if( sizeof(l_auMemArea) == l_uVarTemp32 )
            {
                (void)printf("eCU_BSTFTST_OutOfMem 3  -- OK \n");
            }
            else
            {
                (void)printf("eCU_BSTFTST_OutOfMem 3  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_BSTFTST_OutOfMem 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_OutOfMem 3  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_OutOfMem 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_OutOfMem 4  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uReman) )
    {
        if( 10u == l_uReman )
        {
            (void)printf("eCU_BSTFTST_OutOfMem 5  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_OutOfMem 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_OutOfMem 5  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetWherePutData(&l_tCtx, &l_puPointer, &l_uVarTemp32) )
    {
        if( l_auMemArea == l_puPointer )
        {
            if( sizeof(l_auMemArea) == l_uVarTemp32 )
            {
                (void)printf("eCU_BSTFTST_OutOfMem 6  -- OK \n");
            }
            else
            {
                (void)printf("eCU_BSTFTST_OutOfMem 6  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_BSTFTST_OutOfMem 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_OutOfMem 6  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[0], 1u, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_OutOfMem 7  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_OutOfMem 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_OutOfMem 7  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[1], 1u, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_OutOfMem 8  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_OutOfMem 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_OutOfMem 8  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uReman) )
    {
        if( 8u == l_uReman )
        {
            (void)printf("eCU_BSTFTST_OutOfMem 9  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_OutOfMem 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_OutOfMem 9  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetWherePutData(&l_tCtx, &l_puPointer, &l_uVarTemp32) )
    {
        if( l_auMemArea == l_puPointer )
        {
            if( sizeof(l_auMemArea) == l_uVarTemp32 )
            {
                (void)printf("eCU_BSTFTST_OutOfMem 10 -- OK \n");
            }
            else
            {
                (void)printf("eCU_BSTFTST_OutOfMem 10 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_BSTFTST_OutOfMem 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_OutOfMem 10 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[2], 1u, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_OutOfMem 11 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_OutOfMem 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_OutOfMem 11 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_FRAMEENDED == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[3], 7u, &l_uVarTemp32) )
    {
        if( 7u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_OutOfMem 12 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_OutOfMem 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_OutOfMem 12 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_FRAMEENDED == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[10], 1u, &l_uVarTemp32) )
    {
        if( 0u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_OutOfMem 13 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_OutOfMem 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_OutOfMem 13 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uReman) )
    {
        if( 0u == l_uReman )
        {
            (void)printf("eCU_BSTFTST_OutOfMem 14 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_OutOfMem 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_OutOfMem 14 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetWherePutData(&l_tCtx, &l_puPointer, &l_uVarTemp32) )
    {
        if( l_auMemArea == l_puPointer )
        {
            if( sizeof(l_auMemArea) == l_uVarTemp32 )
            {
                (void)printf("eCU_BSTFTST_OutOfMem 15 -- OK \n");
            }
            else
            {
                (void)printf("eCU_BSTFTST_OutOfMem 15 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_BSTFTST_OutOfMem 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_OutOfMem 15 -- FAIL \n");
    }
}

static void eCU_BSTFTST_StartRestart(void)
{
    /* Local variable */
    t_eCU_BSTF_Ctx l_tCtx;
    uint8_t  l_auMemArea[5u];
    uint8_t  l_auMemAreaFinalChunk[50u];
    uint32_t l_uVarTemp32;
    uint32_t l_uReman;
    uint8_t* l_puPointer;

    /* Init variable */
    l_auMemArea[0u] = ECU_SOF;
    l_auMemArea[1u] = ECU_EOF;
    l_auMemArea[2u] = ECU_ESC;
    l_auMemArea[3u] = 0x12u;
    l_auMemArea[4u] = 0xFFu;

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_StartRestart 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_StartRestart 1  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_NOINITFRAME == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uReman) )
    {
        (void)printf("eCU_BSTFTST_StartRestart 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_StartRestart 2  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetWherePutData(&l_tCtx, &l_puPointer, &l_uVarTemp32) )
    {
        if( l_auMemArea == l_puPointer )
        {
            if( sizeof(l_auMemArea) == l_uVarTemp32 )
            {
                (void)printf("eCU_BSTFTST_StartRestart 3  -- OK \n");
            }
            else
            {
                (void)printf("eCU_BSTFTST_StartRestart 3  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_BSTFTST_StartRestart 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_StartRestart 3  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_StartRestart 4  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_StartRestart 4  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uReman) )
    {
        if( 10u == l_uReman )
        {
            (void)printf("eCU_BSTFTST_StartRestart 5  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_StartRestart 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_StartRestart 5  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetWherePutData(&l_tCtx, &l_puPointer, &l_uVarTemp32) )
    {
        if( l_auMemArea == l_puPointer )
        {
            if( sizeof(l_auMemArea) == l_uVarTemp32 )
            {
                (void)printf("eCU_BSTFTST_StartRestart 6  -- OK \n");
            }
            else
            {
                (void)printf("eCU_BSTFTST_StartRestart 6  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_BSTFTST_StartRestart 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_StartRestart 6  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[0], 1u, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_StartRestart 7  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_StartRestart 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_StartRestart 7  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_RestartFrame(&l_tCtx) )
    {
        (void)printf("eCU_BSTFTST_StartRestart 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_StartRestart 8  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uReman) )
    {
        if( 10u == l_uReman )
        {
            (void)printf("eCU_BSTFTST_StartRestart 9  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_StartRestart 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_StartRestart 9  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetWherePutData(&l_tCtx, &l_puPointer, &l_uVarTemp32) )
    {
        if( l_auMemArea == l_puPointer )
        {
            if( sizeof(l_auMemArea) == l_uVarTemp32 )
            {
                (void)printf("eCU_BSTFTST_StartRestart 10 -- OK \n");
            }
            else
            {
                (void)printf("eCU_BSTFTST_StartRestart 10 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_BSTFTST_StartRestart 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_StartRestart 10 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[0], 1u, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_StartRestart 11 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_StartRestart 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_StartRestart 11 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[1], 1u, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_StartRestart 12 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_StartRestart 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_StartRestart 12 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uReman) )
    {
        if( 8u == l_uReman )
        {
            (void)printf("eCU_BSTFTST_StartRestart 13 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_StartRestart 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_StartRestart 13 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetWherePutData(&l_tCtx, &l_puPointer, &l_uVarTemp32) )
    {
        if( l_auMemArea == l_puPointer )
        {
            if( sizeof(l_auMemArea) == l_uVarTemp32 )
            {
                (void)printf("eCU_BSTFTST_StartRestart 14 -- OK \n");
            }
            else
            {
                (void)printf("eCU_BSTFTST_StartRestart 14 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_BSTFTST_StartRestart 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_StartRestart 14 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[2], 1u, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_StartRestart 15 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_StartRestart 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_StartRestart 15 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_FRAMEENDED == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[3], 7u, &l_uVarTemp32) )
    {
        if( 7u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_StartRestart 16 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_StartRestart 16 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_StartRestart 16 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_FRAMEENDED == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[10], 1u, &l_uVarTemp32) )
    {
        if( 0u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_StartRestart 17 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_StartRestart 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_StartRestart 17 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uReman) )
    {
        if( 0u == l_uReman )
        {
            (void)printf("eCU_BSTFTST_StartRestart 18 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_StartRestart 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_StartRestart 18 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetWherePutData(&l_tCtx, &l_puPointer, &l_uVarTemp32) )
    {
        if( l_auMemArea == l_puPointer )
        {
            if( sizeof(l_auMemArea) == l_uVarTemp32 )
            {
                (void)printf("eCU_BSTFTST_StartRestart 19 -- OK \n");
            }
            else
            {
                (void)printf("eCU_BSTFTST_StartRestart 19 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_BSTFTST_StartRestart 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_StartRestart 19 -- FAIL \n");
    }
}

static void eCU_BSTFTST_GetRemainings(void)
{
    /* Local variable */
    t_eCU_BSTF_Ctx l_tCtx;
    uint8_t  l_auMemArea[5u];
    uint8_t  l_auMemAreaFinalChunk[50u];
    uint32_t l_uVarTemp32;
    uint8_t* l_puPointer;

    /* Init variable */
    l_auMemArea[0u] = ECU_SOF;
    l_auMemArea[1u] = ECU_EOF;
    l_auMemArea[2u] = ECU_ESC;
    l_auMemArea[3u] = 0x12u;
    l_auMemArea[4u] = 0xFFu;

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_GetRemainings 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_GetRemainings 1  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_GetRemainings 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_GetRemainings 2  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        if( 10u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GetRemainings 3  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GetRemainings 3  -- FAIL \n");
        }
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[0], 1u, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GetRemainings 4  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GetRemainings 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GetRemainings 4  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        if( 9u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GetRemainings 5  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GetRemainings 5  -- FAIL \n");
        }
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[1], 1u, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GetRemainings 6  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GetRemainings 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GetRemainings 6  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[2], 1u, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GetRemainings 7  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GetRemainings 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GetRemainings 7  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        if( 7u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GetRemainings 8  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GetRemainings 8  -- FAIL \n");
        }
    }

    if( e_eCU_BSTF_RES_FRAMEENDED == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[3], 7u, &l_uVarTemp32) )
    {
        if( 7u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GetRemainings 9  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GetRemainings 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GetRemainings 9  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        if( 0u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GetRemainings 10 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GetRemainings 10 -- FAIL \n");
        }
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetWherePutData(&l_tCtx, &l_puPointer, &l_uVarTemp32) )
    {
        if( ( sizeof(l_auMemArea) == l_uVarTemp32 ) && ( l_auMemArea == l_puPointer))
        {
            (void)printf("eCU_BSTFTST_GetRemainings 11 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GetRemainings 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GetRemainings 11 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_RestartFrame(&l_tCtx) )
    {
        (void)printf("eCU_BSTFTST_GetRemainings 12 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_GetRemainings 12 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        if( 10u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GetRemainings 13 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GetRemainings 3 -- FAIL \n");
        }
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[0], 1u, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GetRemainings 14 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GetRemainings 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GetRemainings 14 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        if( 9u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GetRemainings 15 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GetRemainings 15 -- FAIL \n");
        }
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[1], 1u, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GetRemainings 16 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GetRemainings 16 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GetRemainings 16 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[2], 1u, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GetRemainings 17 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GetRemainings 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GetRemainings 17 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        if( 7u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GetRemainings 18 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GetRemainings 18 -- FAIL \n");
        }
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[3], 6u, &l_uVarTemp32) )
    {
        if( 6u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GetRemainings 19 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GetRemainings 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GetRemainings 19 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GetRemainings 20 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GetRemainings 20 -- FAIL \n");
        }
    }

    if( e_eCU_BSTF_RES_FRAMEENDED == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[9], 1u, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GetRemainings 21 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GetRemainings 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GetRemainings 21 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        if( 0u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GetRemainings 22 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GetRemainings 22 -- FAIL \n");
        }
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetWherePutData(&l_tCtx, &l_puPointer, &l_uVarTemp32) )
    {
        if( ( sizeof(l_auMemArea) == l_uVarTemp32 ) && ( l_auMemArea == l_puPointer))
        {
            (void)printf("eCU_BSTFTST_GetRemainings 23 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GetRemainings 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GetRemainings 23 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, 1u) )
    {
        (void)printf("eCU_BSTFTST_GetRemainings 24 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_GetRemainings 24 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        if( 4u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GetRemainings 25 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GetRemainings 25 -- FAIL \n");
        }
    }
}

static void eCU_BSTFTST_General(void)
{
    /* Local variable */
    t_eCU_BSTF_Ctx l_tCtx;
    uint8_t  l_auMemArea[4u];
    uint8_t  l_auMemAreaFinalChunk[50u];
    uint8_t  l_auMemAreaExpected[9u];
    uint32_t l_uVarTemp32;
    uint32_t l_uCounter;
    uint32_t l_uRemaining;
    e_eCU_BSTF_RES l_eRes = e_eCU_BSTF_RES_OK;
    uint8_t* l_puPointer;

    /* Init variable */
    l_auMemAreaExpected[0u] = ECU_SOF;
    l_auMemAreaExpected[1u] = ECU_ESC;
    l_auMemAreaExpected[2u] = (uint8_t)~ECU_SOF;
    l_auMemAreaExpected[3u] = ECU_ESC;
    l_auMemAreaExpected[4u] = (uint8_t)~ECU_EOF;
    l_auMemAreaExpected[5u] = 0x12u;
    l_auMemAreaExpected[6u] = ECU_ESC;
    l_auMemAreaExpected[7u] = (uint8_t)~ECU_ESC;
    l_auMemAreaExpected[8u] = ECU_EOF;

    /* Init variable */
    (void)memset(l_auMemAreaFinalChunk, 0, sizeof(l_auMemAreaFinalChunk));
    l_auMemArea[0u] = ECU_SOF;
    l_auMemArea[1u] = ECU_EOF;
    l_auMemArea[2u] = 0x12u;
    l_auMemArea[3u] = ECU_ESC;

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_General 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 1  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_General 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 2  -- FAIL \n");
    }

    l_uVarTemp32 = 0u;
    l_uCounter = 0u;

    while( e_eCU_BSTF_RES_OK == l_eRes )
    {
        l_eRes = eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[l_uCounter], 1u, &l_uVarTemp32);
        if( ( e_eCU_BSTF_RES_FRAMEENDED == l_eRes) || ( e_eCU_BSTF_RES_OK == l_eRes) )
        {
            if(l_uVarTemp32 <= sizeof(l_auMemAreaFinalChunk))
            {
                l_uCounter += l_uVarTemp32;
            }
            else
            {
                l_eRes = e_eCU_BSTF_RES_BADPARAM;
            }
        }
    }

    if( e_eCU_BSTF_RES_FRAMEENDED == l_eRes )
    {
        (void)printf("eCU_BSTFTST_General 3  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 3  -- FAIL \n");
    }

    if( 0 == memcmp(l_auMemAreaExpected, l_auMemAreaFinalChunk, l_uCounter) )
    {
        if( sizeof(l_auMemAreaExpected) == l_uCounter )
        {
            (void)printf("eCU_BSTFTST_General 4  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_General 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 4  -- FAIL \n");
    }

    /* Init variable */
    (void)memset(l_auMemAreaFinalChunk, 0, sizeof(l_auMemAreaFinalChunk));
    l_auMemArea[0u] = ECU_SOF;
    l_auMemArea[1u] = ECU_EOF;
    l_auMemArea[2u] = 0x12u;
    l_auMemArea[3u] = ECU_ESC;

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_General 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 5  -- FAIL \n");
    }

    l_eRes = e_eCU_BSTF_RES_OK;
    l_uVarTemp32 = 0u;
    l_uCounter = 0u;

    while( e_eCU_BSTF_RES_OK == l_eRes )
    {
        l_eRes = eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[l_uCounter], 2u, &l_uVarTemp32);
        if( ( e_eCU_BSTF_RES_FRAMEENDED == l_eRes) || ( e_eCU_BSTF_RES_OK == l_eRes) )
        {
            l_uCounter += l_uVarTemp32;
        }
    }

    if( e_eCU_BSTF_RES_FRAMEENDED == l_eRes )
    {
        (void)printf("eCU_BSTFTST_General 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 6  -- FAIL \n");
    }

    if( 0 == memcmp(l_auMemAreaExpected, l_auMemAreaFinalChunk, l_uCounter) )
    {
        if( sizeof(l_auMemAreaExpected) == l_uCounter )
        {
            (void)printf("eCU_BSTFTST_General 7  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_General 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 7  -- FAIL \n");
    }

    /* Init variable */
    (void)memset(l_auMemAreaFinalChunk, 0, sizeof(l_auMemAreaFinalChunk));
    l_auMemArea[0u] = ECU_SOF;
    l_auMemArea[1u] = ECU_EOF;
    l_auMemArea[2u] = 0x12u;
    l_auMemArea[3u] = ECU_ESC;

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_General 8  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 8  -- FAIL \n");
    }

    l_eRes = e_eCU_BSTF_RES_OK;
    l_uVarTemp32 = 0u;
    l_uCounter = 0u;

    while( e_eCU_BSTF_RES_OK == l_eRes )
    {
        l_eRes = eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[l_uCounter], 3u, &l_uVarTemp32);
        if( ( e_eCU_BSTF_RES_FRAMEENDED == l_eRes) || ( e_eCU_BSTF_RES_OK == l_eRes) )
        {
            l_uCounter += l_uVarTemp32;
        }
    }

    if( e_eCU_BSTF_RES_FRAMEENDED == l_eRes )
    {
        (void)printf("eCU_BSTFTST_General 9  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 9  -- FAIL \n");
    }

    if( 0 == memcmp(l_auMemAreaExpected, l_auMemAreaFinalChunk, l_uCounter) )
    {
        if( sizeof(l_auMemAreaExpected) == l_uCounter )
        {
            (void)printf("eCU_BSTFTST_General 10 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_General 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 10 -- FAIL \n");
    }


    /* Init variable */
    (void)memset(l_auMemAreaFinalChunk, 0, sizeof(l_auMemAreaFinalChunk));
    l_auMemArea[0u] = ECU_SOF;
    l_auMemArea[1u] = ECU_EOF;
    l_auMemArea[2u] = 0x12u;
    l_auMemArea[3u] = ECU_ESC;

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_General 11 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 11 -- FAIL \n");
    }

    l_eRes = e_eCU_BSTF_RES_OK;
    l_uVarTemp32 = 0u;
    l_uCounter = 0u;

    while( e_eCU_BSTF_RES_OK == l_eRes )
    {
        l_eRes = eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[l_uCounter], 4u, &l_uVarTemp32);
        if( ( e_eCU_BSTF_RES_FRAMEENDED == l_eRes) || ( e_eCU_BSTF_RES_OK == l_eRes) )
        {
            l_uCounter += l_uVarTemp32;
        }
    }

    if( e_eCU_BSTF_RES_FRAMEENDED == l_eRes )
    {
        (void)printf("eCU_BSTFTST_General 12 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 12 -- FAIL \n");
    }

    if( 0 == memcmp(l_auMemAreaExpected, l_auMemAreaFinalChunk, l_uCounter) )
    {
        if( sizeof(l_auMemAreaExpected) == l_uCounter )
        {
            (void)printf("eCU_BSTFTST_General 13 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_General 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 13 -- FAIL \n");
    }



    /* Init variable */
    (void)memset(l_auMemAreaFinalChunk, 0, sizeof(l_auMemAreaFinalChunk));
    l_auMemArea[0u] = ECU_SOF;
    l_auMemArea[1u] = ECU_EOF;
    l_auMemArea[2u] = 0x12u;
    l_auMemArea[3u] = ECU_ESC;

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_General 14 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 14 -- FAIL \n");
    }

    l_eRes = e_eCU_BSTF_RES_OK;
    l_uVarTemp32 = 0u;
    l_uCounter = 0u;

    while( e_eCU_BSTF_RES_OK == l_eRes )
    {
        l_eRes = eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[l_uCounter], 5u, &l_uVarTemp32);
        if( ( e_eCU_BSTF_RES_FRAMEENDED == l_eRes) || ( e_eCU_BSTF_RES_OK == l_eRes) )
        {
            l_uCounter += l_uVarTemp32;
        }
    }

    if( e_eCU_BSTF_RES_FRAMEENDED == l_eRes )
    {
        (void)printf("eCU_BSTFTST_General 15 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 15 -- FAIL \n");
    }

    if( 0 == memcmp(l_auMemAreaExpected, l_auMemAreaFinalChunk, l_uCounter) )
    {
        if( sizeof(l_auMemAreaExpected) == l_uCounter )
        {
            (void)printf("eCU_BSTFTST_General 16 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_General 16 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 16 -- FAIL \n");
    }


    /* Init variable */
    (void)memset(l_auMemAreaFinalChunk, 0, sizeof(l_auMemAreaFinalChunk));
    l_auMemArea[0u] = ECU_SOF;
    l_auMemArea[1u] = ECU_EOF;
    l_auMemArea[2u] = 0x12u;
    l_auMemArea[3u] = ECU_ESC;

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_General 17 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 17 -- FAIL \n");
    }

    l_eRes = e_eCU_BSTF_RES_OK;
    l_uVarTemp32 = 0u;
    l_uCounter = 0u;

    while( e_eCU_BSTF_RES_OK == l_eRes )
    {
        l_eRes = eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[l_uCounter], 6u, &l_uVarTemp32);
        if( ( e_eCU_BSTF_RES_FRAMEENDED == l_eRes) || ( e_eCU_BSTF_RES_OK == l_eRes) )
        {
            l_uCounter += l_uVarTemp32;
        }
    }

    if( e_eCU_BSTF_RES_FRAMEENDED == l_eRes )
    {
        (void)printf("eCU_BSTFTST_General 18 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 18 -- FAIL \n");
    }

    if( 0 == memcmp(l_auMemAreaExpected, l_auMemAreaFinalChunk, l_uCounter) )
    {
        if( sizeof(l_auMemAreaExpected) == l_uCounter )
        {
            (void)printf("eCU_BSTFTST_General 19 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_General 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 19 -- FAIL \n");
    }



    /* Init variable */
    (void)memset(l_auMemAreaFinalChunk, 0, sizeof(l_auMemAreaFinalChunk));
    l_auMemArea[0u] = ECU_SOF;
    l_auMemArea[1u] = ECU_EOF;
    l_auMemArea[2u] = 0x12u;
    l_auMemArea[3u] = ECU_ESC;

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_General 20 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 20 -- FAIL \n");
    }

    l_eRes = e_eCU_BSTF_RES_OK;
    l_uVarTemp32 = 0u;
    l_uCounter = 0u;

    while( e_eCU_BSTF_RES_OK == l_eRes )
    {
        l_eRes = eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[l_uCounter], 7u, &l_uVarTemp32);
        if( ( e_eCU_BSTF_RES_FRAMEENDED == l_eRes) || ( e_eCU_BSTF_RES_OK == l_eRes) )
        {
            l_uCounter += l_uVarTemp32;
        }
    }

    if( e_eCU_BSTF_RES_FRAMEENDED == l_eRes )
    {
        (void)printf("eCU_BSTFTST_General 21 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 21 -- FAIL \n");
    }

    if( 0 == memcmp(l_auMemAreaExpected, l_auMemAreaFinalChunk, l_uCounter) )
    {
        if( sizeof(l_auMemAreaExpected) == l_uCounter )
        {
            (void)printf("eCU_BSTFTST_General 22 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_General 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 22 -- FAIL \n");
    }




    /* Init variable */
    (void)memset(l_auMemAreaFinalChunk, 0, sizeof(l_auMemAreaFinalChunk));
    l_auMemArea[0u] = ECU_SOF;
    l_auMemArea[1u] = ECU_EOF;
    l_auMemArea[2u] = 0x12u;
    l_auMemArea[3u] = ECU_ESC;

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_General 23 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 23 -- FAIL \n");
    }

    l_eRes = e_eCU_BSTF_RES_OK;
    l_uVarTemp32 = 0u;
    l_uCounter = 0u;

    while( e_eCU_BSTF_RES_OK == l_eRes )
    {
        l_eRes = eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[l_uCounter], 8u, &l_uVarTemp32);
        if( ( e_eCU_BSTF_RES_FRAMEENDED == l_eRes) || ( e_eCU_BSTF_RES_OK == l_eRes) )
        {
            l_uCounter += l_uVarTemp32;
        }
    }

    if( e_eCU_BSTF_RES_FRAMEENDED == l_eRes )
    {
        (void)printf("eCU_BSTFTST_General 24 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 24 -- FAIL \n");
    }

    if( 0 == memcmp(l_auMemAreaExpected, l_auMemAreaFinalChunk, l_uCounter) )
    {
        if( sizeof(l_auMemAreaExpected) == l_uCounter )
        {
            (void)printf("eCU_BSTFTST_General 25 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_General 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 25 -- FAIL \n");
    }


    /* Init variable */
    (void)memset(l_auMemAreaFinalChunk, 0, sizeof(l_auMemAreaFinalChunk));
    l_auMemArea[0u] = ECU_SOF;
    l_auMemArea[1u] = ECU_EOF;
    l_auMemArea[2u] = 0x12u;
    l_auMemArea[3u] = ECU_ESC;

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_General 26 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 26 -- FAIL \n");
    }

    l_eRes = e_eCU_BSTF_RES_OK;
    l_uVarTemp32 = 0u;
    l_uCounter = 0u;

    while( e_eCU_BSTF_RES_OK == l_eRes )
    {
        l_eRes = eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[l_uCounter], 9u, &l_uVarTemp32);
        if( ( e_eCU_BSTF_RES_FRAMEENDED == l_eRes) || ( e_eCU_BSTF_RES_OK == l_eRes) )
        {
            l_uCounter += l_uVarTemp32;
        }
    }

    if( e_eCU_BSTF_RES_FRAMEENDED == l_eRes )
    {
        (void)printf("eCU_BSTFTST_General 27 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 27 -- FAIL \n");
    }

    if( 0 == memcmp(l_auMemAreaExpected, l_auMemAreaFinalChunk, l_uCounter) )
    {
        if( sizeof(l_auMemAreaExpected) == l_uCounter )
        {
            (void)printf("eCU_BSTFTST_General 28 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_General 28 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 28 -- FAIL \n");
    }



    /* Init variable */
    (void)memset(l_auMemAreaFinalChunk, 0, sizeof(l_auMemAreaFinalChunk));
    l_auMemArea[0u] = ECU_SOF;
    l_auMemArea[1u] = ECU_EOF;
    l_auMemArea[2u] = 0x12u;
    l_auMemArea[3u] = ECU_ESC;

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_General 29 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 29 -- FAIL \n");
    }

    l_uVarTemp32 = 0u;
    l_uCounter = 0u;
    l_uRemaining = 0u;

    l_eRes = eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uRemaining);
    if( e_eCU_BSTF_RES_OK == l_eRes )
    {
        if( 9u == l_uRemaining )
        {
            (void)printf("eCU_BSTFTST_General 30 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_General 30 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 30 -- FAIL \n");
    }



    while( ( e_eCU_BSTF_RES_OK == l_eRes ) && ( 0u != l_uRemaining ) )
    {
        l_eRes = eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[l_uCounter], 10u, &l_uVarTemp32);
        if( ( e_eCU_BSTF_RES_FRAMEENDED == l_eRes) || ( e_eCU_BSTF_RES_OK == l_eRes) )
        {
            l_uCounter += l_uVarTemp32;
        }

        l_eRes = eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uRemaining);
    }

    if( e_eCU_BSTF_RES_OK == l_eRes )
    {
        (void)printf("eCU_BSTFTST_General 31 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 31 -- FAIL \n");
    }

    if( 0 == memcmp(l_auMemAreaExpected, l_auMemAreaFinalChunk, l_uCounter) )
    {
        if( sizeof(l_auMemAreaExpected) == l_uCounter )
        {
            (void)printf("eCU_BSTFTST_General 32 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_General 32 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 32 -- FAIL \n");
    }

    /* Init variable */
    (void)memset(l_auMemAreaFinalChunk, 0, sizeof(l_auMemAreaFinalChunk));
    l_auMemArea[0u] = ECU_SOF;
    l_auMemArea[1u] = ECU_ESC;
    l_auMemArea[2u] = 0x12u;
    l_auMemArea[3u] = ECU_ESC;

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_General 33 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 33 -- FAIL \n");
    }

    l_uVarTemp32 = 0u;
    l_uCounter = 0u;
    l_uRemaining = 0u;


    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[l_uCounter], 2u, &l_uVarTemp32) )
    {
        (void)printf("eCU_BSTFTST_General 34 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 34 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uRemaining) )
    {
        if( 7u == l_uRemaining )
        {
            (void)printf("eCU_BSTFTST_General 35 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_General 35 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 35 -- FAIL \n");
    }


    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, 1u) )
    {
        (void)printf("eCU_BSTFTST_General 36 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 36 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetWherePutData(&l_tCtx, &l_puPointer, &l_uVarTemp32) )
    {
        if( ( sizeof(l_auMemArea) == l_uVarTemp32 ) && ( l_auMemArea == l_puPointer))
        {
            (void)printf("eCU_BSTFTST_General 37 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_General 37 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_General 37 -- FAIL \n");
    }
}

static void eCU_BSTFTST_GeneralAnother(void)
{
    /* Local variable */
    t_eCU_BSTF_Ctx l_tCtx;
    uint8_t  l_auMemArea[4u];
    uint8_t  l_auMemAreaFinalChunk[50u];
    uint8_t  l_auMemAreaExpected[9u];
    uint32_t l_uVarTemp32;
    uint32_t l_uCounter;

    /* Init variable */
    l_auMemAreaExpected[0u] = ECU_SOF;
    l_auMemAreaExpected[1u] = ECU_ESC;
    l_auMemAreaExpected[2u] = (uint8_t)~ECU_SOF;
    l_auMemAreaExpected[3u] = ECU_ESC;
    l_auMemAreaExpected[4u] = (uint8_t)~ECU_EOF;
    l_auMemAreaExpected[5u] = 0x12u;
    l_auMemAreaExpected[6u] = ECU_ESC;
    l_auMemAreaExpected[7u] = (uint8_t)~ECU_ESC;
    l_auMemAreaExpected[8u] = ECU_EOF;

    /* Init variable */
    l_uCounter = 0u;
    (void)memset(l_auMemAreaFinalChunk, 0, sizeof(l_auMemAreaFinalChunk));
    l_auMemArea[0u] = ECU_SOF;
    l_auMemArea[1u] = ECU_EOF;
    l_auMemArea[2u] = 0x12u;
    l_auMemArea[3u] = ECU_ESC;

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 1  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 2  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        if( 9u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 3  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 3  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[l_uCounter], 1u, &l_uVarTemp32) )
    {
        l_uCounter += l_uVarTemp32;
        if( 1u == l_uVarTemp32 )
        {
            if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
            {
                if( 8u == l_uVarTemp32 )
                {
                    (void)printf("eCU_BSTFTST_GeneralAnother 4  -- OK \n");
                }
                else
                {
                    (void)printf("eCU_BSTFTST_GeneralAnother 4  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BSTFTST_GeneralAnother 4  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 4  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[l_uCounter], 7u, &l_uVarTemp32) )
    {
        l_uCounter += l_uVarTemp32;
        if( 7u == l_uVarTemp32 )
        {
            if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
            {
                if( 1u == l_uVarTemp32 )
                {
                    (void)printf("eCU_BSTFTST_GeneralAnother 3  -- OK \n");
                }
                else
                {
                    (void)printf("eCU_BSTFTST_GeneralAnother 3  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BSTFTST_GeneralAnother 3  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 3  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_FRAMEENDED == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[l_uCounter], 7u, &l_uVarTemp32) )
    {
        l_uCounter += l_uVarTemp32;
        if( 1u == l_uVarTemp32 )
        {
            if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
            {
                if( 0u == l_uVarTemp32 )
                {
                    (void)printf("eCU_BSTFTST_GeneralAnother 4  -- OK \n");
                }
                else
                {
                    (void)printf("eCU_BSTFTST_GeneralAnother 4  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eCU_BSTFTST_GeneralAnother 4  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 4  -- FAIL \n");
    }

    if( 0 == memcmp(l_auMemAreaExpected, l_auMemAreaFinalChunk, l_uCounter) )
    {
        if( sizeof(l_auMemAreaExpected) == l_uCounter )
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 5  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother   -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 5  -- FAIL \n");
    }

    /* Init variable */
    (void)memset(l_auMemAreaFinalChunk, 0, sizeof(l_auMemAreaFinalChunk));
    l_auMemArea[0u] = ECU_SOF;


    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 6  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, 1u) )
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 7  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        if( 4u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 8  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 8  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[0u], 1u, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            if( ECU_SOF == l_auMemAreaFinalChunk[0u] )
            {
                (void)printf("eCU_BSTFTST_GeneralAnother 9  -- OK \n");
            }
            else
            {
                (void)printf("eCU_BSTFTST_GeneralAnother 9  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 9  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        if( 3u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 10 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 10 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[0u], 1u, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            if( ECU_ESC == l_auMemAreaFinalChunk[0u] )
            {
                (void)printf("eCU_BSTFTST_GeneralAnother 11 -- OK \n");
            }
            else
            {
                (void)printf("eCU_BSTFTST_GeneralAnother 11 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 11 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        if( 2u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 12 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 12 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[0u], 1u, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            if( ((uint8_t)~ECU_SOF) == l_auMemAreaFinalChunk[0u] )
            {
                (void)printf("eCU_BSTFTST_GeneralAnother 13 -- OK \n");
            }
            else
            {
                (void)printf("eCU_BSTFTST_GeneralAnother 13 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 13 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 14 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 14 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_FRAMEENDED == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[0u], 1u, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            if( ECU_EOF == l_auMemAreaFinalChunk[0u] )
            {
                (void)printf("eCU_BSTFTST_GeneralAnother 15 -- OK \n");
            }
            else
            {
                (void)printf("eCU_BSTFTST_GeneralAnother 15 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 15 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        if( 0u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 16 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 16 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 16 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_FRAMEENDED == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[0u], 1u, &l_uVarTemp32) )
    {
        if( 0u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 17 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 17 -- FAIL \n");
    }

    /* Init variable */
    (void)memset(l_auMemAreaFinalChunk, 0, sizeof(l_auMemAreaFinalChunk));
    l_auMemArea[0u] = 0x12u;


    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 18 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 18 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, 1u) )
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 19 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 19 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        if( 3u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 20 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 20 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[0u], 1u, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            if( ECU_SOF == l_auMemAreaFinalChunk[0u] )
            {
                (void)printf("eCU_BSTFTST_GeneralAnother 21 -- OK \n");
            }
            else
            {
                (void)printf("eCU_BSTFTST_GeneralAnother 21 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 21 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        if( 2u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 22 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 22 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[0u], 1u, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            if( 0x12u == l_auMemAreaFinalChunk[0u] )
            {
                (void)printf("eCU_BSTFTST_GeneralAnother 23 -- OK \n");
            }
            else
            {
                (void)printf("eCU_BSTFTST_GeneralAnother 23 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 23 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 24 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 24 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 24 -- FAIL \n");
    }


    if( e_eCU_BSTF_RES_FRAMEENDED == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[0u], 1u, &l_uVarTemp32) )
    {
        if( 1u == l_uVarTemp32 )
        {
            if( ECU_EOF == l_auMemAreaFinalChunk[0u] )
            {
                (void)printf("eCU_BSTFTST_GeneralAnother 25 -- OK \n");
            }
            else
            {
                (void)printf("eCU_BSTFTST_GeneralAnother 25 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 25 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_GetRemByteToGet(&l_tCtx, &l_uVarTemp32) )
    {
        if( 0u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 26 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 26 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 26 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_FRAMEENDED == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[0u], 1u, &l_uVarTemp32) )
    {
        if( 0u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 27 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_GeneralAnother 27 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_GeneralAnother 27 -- FAIL \n");
    }
}

static void eCU_BSTFTST_Correcteness(void)
{
    /* Local variable */
    t_eCU_BSTF_Ctx l_tCtx;
    uint8_t  l_auMemArea[4u];
    uint8_t  l_auMemAreaFinalChunk[50u];
    uint8_t  l_auMemAreaExpected[9u];
    uint32_t l_uVarTemp32;

    /* Init variable */
    l_auMemAreaExpected[0u] = ECU_SOF;
    l_auMemAreaExpected[1u] = ECU_ESC;
    l_auMemAreaExpected[2u] = (uint8_t)~ECU_SOF;
    l_auMemAreaExpected[3u] = ECU_ESC;
    l_auMemAreaExpected[4u] = (uint8_t)~ECU_EOF;
    l_auMemAreaExpected[5u] = 0x12u;
    l_auMemAreaExpected[6u] = ECU_ESC;
    l_auMemAreaExpected[7u] = (uint8_t)~ECU_ESC;
    l_auMemAreaExpected[8u] = ECU_EOF;

    /* Init variable */
    (void)memset(l_auMemAreaFinalChunk, 0, sizeof(l_auMemAreaFinalChunk));
    l_auMemArea[0u] = ECU_SOF;
    l_auMemArea[1u] = ECU_EOF;
    l_auMemArea[2u] = 0x12u;
    l_auMemArea[3u] = ECU_ESC;

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_Correcteness 1  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_Correcteness 1  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_Correcteness 2  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_Correcteness 2  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_FRAMEENDED == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[0], sizeof(l_auMemAreaFinalChunk), &l_uVarTemp32) )
    {
        if( 9u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_Correcteness 3  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_Correcteness 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_Correcteness 3  -- FAIL \n");
    }

    if( 0 == memcmp(l_auMemAreaExpected, l_auMemAreaFinalChunk, 9u) )
    {
        (void)printf("eCU_BSTFTST_Correcteness 5  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_Correcteness 5  -- FAIL \n");
    }

    /* Init variable */
    l_auMemAreaExpected[0u] = ECU_SOF;
    l_auMemAreaExpected[1u] = 0xFFu;
    l_auMemAreaExpected[2u] = 0xFFu;
    l_auMemAreaExpected[3u] = 0xFFu;
    l_auMemAreaExpected[4u] = 0xFFu;
    l_auMemAreaExpected[5u] = ECU_EOF;

    /* Init variable */
    (void)memset(l_auMemAreaFinalChunk, 0, sizeof(l_auMemAreaFinalChunk));
    l_auMemArea[0u] = 0xFFu;
    l_auMemArea[1u] = 0xFFu;
    l_auMemArea[2u] = 0xFFu;
    l_auMemArea[3u] = 0xFFu;

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_Correcteness 6  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_Correcteness 6  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_Correcteness 7  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_Correcteness 7  -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_FRAMEENDED == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[0], sizeof(l_auMemAreaFinalChunk), &l_uVarTemp32) )
    {
        if( 6u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_Correcteness 8  -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_Correcteness 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_Correcteness 8  -- FAIL \n");
    }

    if( 0 == memcmp(l_auMemAreaExpected, l_auMemAreaFinalChunk, 6u) )
    {
        (void)printf("eCU_BSTFTST_Correcteness 9  -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_Correcteness 9  -- FAIL \n");
    }

    /* Init variable */
    l_auMemAreaExpected[0u] = ECU_SOF;
    l_auMemAreaExpected[1u] = 0xFFu;
    l_auMemAreaExpected[2u] = 0xFFu;
    l_auMemAreaExpected[3u] = 0xFFu;
    l_auMemAreaExpected[4u] = ECU_ESC;
    l_auMemAreaExpected[5u] = (uint8_t)~ECU_SOF;;
    l_auMemAreaExpected[6u] = ECU_EOF;

    /* Init variable */
    (void)memset(l_auMemAreaFinalChunk, 0, sizeof(l_auMemAreaFinalChunk));
    l_auMemArea[0u] = 0xFFu;
    l_auMemArea[1u] = 0xFFu;
    l_auMemArea[2u] = 0xFFu;
    l_auMemArea[3u] = ECU_SOF;

    /* Function */
    if( e_eCU_BSTF_RES_OK == eCU_BSTF_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_Correcteness 10 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_Correcteness 10 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_OK == eCU_BSTF_NewFrame(&l_tCtx, sizeof(l_auMemArea)) )
    {
        (void)printf("eCU_BSTFTST_Correcteness 11 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_Correcteness 11 -- FAIL \n");
    }

    if( e_eCU_BSTF_RES_FRAMEENDED == eCU_BSTF_GetStufChunk(&l_tCtx, &l_auMemAreaFinalChunk[0], sizeof(l_auMemAreaFinalChunk), &l_uVarTemp32) )
    {
        if( 7u == l_uVarTemp32 )
        {
            (void)printf("eCU_BSTFTST_Correcteness 12 -- OK \n");
        }
        else
        {
            (void)printf("eCU_BSTFTST_Correcteness 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eCU_BSTFTST_Correcteness 12 -- FAIL \n");
    }

    if( 0 == memcmp(l_auMemAreaExpected, l_auMemAreaFinalChunk, 7u) )
    {
        (void)printf("eCU_BSTFTST_Correcteness 13 -- OK \n");
    }
    else
    {
        (void)printf("eCU_BSTFTST_Correcteness 13 -- FAIL \n");
    }


}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "CERT-STR32-C"
#endif