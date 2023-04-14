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
    uint32_t uTimeUsed;
};

struct t_eFSS_TYPE_WriteCtxUser
{
    e_eFSS_CORELL_RES eLastEr;
    uint32_t uTimeUsed;
};

struct t_eFSS_TYPE_ReadCtxUser
{
    e_eFSS_CORELL_RES eLastEr;
    uint32_t uTimeUsed;
};

struct t_eFSS_TYPE_CrcCtxUser
{
    e_eFSS_CORELL_RES eLastEr;
    uint32_t uTimeUsed;
};



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_CORELLTST_EraseAdapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase);

static bool_t eFSS_CORELLTST_EraseErrAdapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase);

static bool_t eFSS_CORELLTST_EraseTst1Adapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase);


static bool_t eFSS_CORELLTST_WriteAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                        const uint32_t p_uPageToWrite, const uint8_t* p_puDataToWrite,
                                        const uint32_t p_uDataToWriteL );

static bool_t eFSS_CORELLTST_WriteErrAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                           const uint32_t p_uPageToWrite, const uint8_t* p_puDataToWrite,
                                           const uint32_t p_uDataToWriteL );

static bool_t eFSS_CORELLTST_WriteTst1Adapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                        const uint32_t p_uPageToWrite, const uint8_t* p_puDataToWrite,
                                        const uint32_t p_uDataToWriteL );

static bool_t eFSS_CORELLTST_ReadAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                       const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                       const uint32_t p_uReadBufferL );

static bool_t eFSS_CORELLTST_ReadErrAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                       const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                       const uint32_t p_uReadBufferL );

static bool_t eFSS_CORELLTST_ReadTst1Adapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                       const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                       const uint32_t p_uReadBufferL );

static bool_t eFSS_CORELLTST_ReadTst2Adapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                       const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                       const uint32_t p_uReadBufferL );

static bool_t eFSS_CORELLTST_CrcAdapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
                                      const uint8_t* p_puData, const uint32_t p_uDataL,
                                      uint32_t* const p_puCrc32Val );

static bool_t eFSS_CORELLTST_CrcErrAdapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
                                         const uint8_t* p_puData, const uint32_t p_uDataL,
                                         uint32_t* const p_puCrc32Val );

static bool_t eFSS_CORELLTST_CrcTst1Adapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
                                          const uint8_t* p_puData, const uint32_t p_uDataL,
                                          uint32_t* const p_puCrc32Val );

/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void eFSS_CORELLTST_BadPointer(void);
static void eFSS_CORELLTST_BadInit(void);
static void eFSS_CORELLTST_BadParamEntr(void);
static void eFSS_CORELLTST_CorruptedCtx(void);
static void eFSS_CORELLTST_Basic(void);
static void eFSS_CORELLTST_BadClBckNRetry(void);
static void eFSS_CORELLTST_CrcTest(void);
static void eFSS_CORELLTST_LoadTest(void);
static void eFSS_CORELLTST_FlushTest(void);
static void eFSS_CORELLTST_GenTest(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eFSS_CORELLTST_ExeTest(void)
{
	(void)printf("\n\nCORE LOW LEVEL TEST START \n\n");

    eFSS_CORELLTST_BadPointer();
    eFSS_CORELLTST_BadInit();
    eFSS_CORELLTST_BadParamEntr();
    eFSS_CORELLTST_CorruptedCtx();
    eFSS_CORELLTST_Basic();
    eFSS_CORELLTST_BadClBckNRetry();
    eFSS_CORELLTST_CrcTest();
    eFSS_CORELLTST_LoadTest();
    eFSS_CORELLTST_FlushTest();
    eFSS_CORELLTST_GenTest();

    (void)printf("\n\nCORE LOW LEVEL TEST END \n\n");
}


/***********************************************************************************************************************
 *   PRIVATE MODULES VARIABLE DECLARATION
 **********************************************************************************************************************/
static bool_t  m_bIsErased1 = false;
static bool_t  m_bIsErased2 = false;
static uint8_t m_auStorArea1[24u];
static uint8_t m_auStorArea2[24u];


/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-8.13"
    /* Suppressed for code clarity in test execution*/
#endif

static bool_t eFSS_CORELLTST_EraseAdapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase)
{
    bool_t l_bRes;
    (void)p_uPageToErase;

    if( NULL == p_ptCtx )
    {
        l_bRes = false;
    }
    else
    {
        p_ptCtx->uTimeUsed++;
        l_bRes = true;
        p_ptCtx->eLastEr = e_eFSS_CORELL_RES_OK;
    }

    return l_bRes;
}

static bool_t eFSS_CORELLTST_EraseErrAdapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase)
{
    bool_t l_bRes;
    (void)p_uPageToErase;

    if( NULL == p_ptCtx )
    {
        l_bRes = false;
    }
    else
    {
        p_ptCtx->uTimeUsed++;
        l_bRes = false;
        p_ptCtx->eLastEr = e_eFSS_CORELL_RES_BADPOINTER;
    }

    return l_bRes;
}

static bool_t eFSS_CORELLTST_EraseTst1Adapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase)
{
    bool_t l_bRes;

    if( NULL == p_ptCtx )
    {
        l_bRes = false;
    }
    else
    {
        if( p_uPageToErase >= 2u )
        {
            l_bRes = false;
            p_ptCtx->eLastEr = e_eFSS_CORELL_RES_BADPARAM;
        }
        else
        {
            p_ptCtx->uTimeUsed++;

            if( 0u == p_uPageToErase )
            {
                l_bRes = true;
                m_bIsErased1 = true;
                (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
            }
            else
            {
                l_bRes = true;
                m_bIsErased2 = true;
                (void)memset(m_auStorArea2, 0, sizeof(m_auStorArea2));
            }
        }
    }

    return l_bRes;
}

static bool_t eFSS_CORELLTST_WriteAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                        const uint32_t p_uPageToWrite, const uint8_t* p_puDataToWrite,
                                        const uint32_t p_uDataToWriteL )
{
    bool_t l_bRes;
    (void)p_uPageToWrite;
    (void)p_uDataToWriteL;

    if( NULL == p_ptCtx )
    {
        l_bRes = false;
    }
    else
    {
        p_ptCtx->uTimeUsed++;

        if( NULL == p_puDataToWrite )
        {
            l_bRes = false;
            p_ptCtx->eLastEr = e_eFSS_CORELL_RES_BADPOINTER;
        }
        else
        {
            l_bRes = true;
            p_ptCtx->eLastEr = e_eFSS_CORELL_RES_OK;
        }
    }

    return l_bRes;
}

static bool_t eFSS_CORELLTST_WriteErrAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                           const uint32_t p_uPageToWrite, const uint8_t* p_puDataToWrite,
                                           const uint32_t p_uDataToWriteL )
{
    bool_t l_bRes;
    (void)p_uDataToWriteL;
    (void)p_uPageToWrite;

    if( NULL == p_ptCtx )
    {
        l_bRes = false;
    }
    else
    {
        p_ptCtx->uTimeUsed++;

        if( NULL == p_puDataToWrite )
        {
            l_bRes = false;
            p_ptCtx->eLastEr = e_eFSS_CORELL_RES_BADPOINTER;
        }
        else
        {
            l_bRes = false;
            p_ptCtx->eLastEr = e_eFSS_CORELL_RES_BADPOINTER;
        }
    }

    return l_bRes;
}

static bool_t eFSS_CORELLTST_WriteTst1Adapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                            const uint32_t p_uPageToWrite, const uint8_t* p_puDataToWrite,
                                            const uint32_t p_uDataToWriteL )
{
    bool_t l_bRes;

    if( NULL == p_ptCtx )
    {
        l_bRes = false;
    }
    else
    {
        if( NULL == p_puDataToWrite )
        {
            l_bRes = false;
            p_ptCtx->eLastEr = e_eFSS_CORELL_RES_BADPOINTER;
        }
        else
        {
            if( p_uPageToWrite >= 2u )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_CORELL_RES_BADPARAM;
            }
            else
            {
                if( 24u != p_uDataToWriteL )
                {
                    l_bRes = false;
                    p_ptCtx->eLastEr = e_eFSS_CORELL_RES_BADPARAM;
                }
                else
                {
                    p_ptCtx->uTimeUsed++;

                    if( 0u == p_uPageToWrite )
                    {
                        if( false == m_bIsErased1 )
                        {
                            l_bRes = false;
                            p_ptCtx->eLastEr = e_eFSS_CORELL_RES_CORRUPTCTX;
                        }
                        else
                        {
                            l_bRes = true;
                            p_ptCtx->eLastEr = e_eFSS_CORELL_RES_OK;
                            m_bIsErased1 = false;
                            (void)memcpy(m_auStorArea1, p_puDataToWrite, p_uDataToWriteL);
                        }
                    }
                    else
                    {
                        if( false == m_bIsErased2 )
                        {
                            l_bRes = false;
                            p_ptCtx->eLastEr = e_eFSS_CORELL_RES_CORRUPTCTX;
                        }
                        else
                        {
                            l_bRes = true;
                            p_ptCtx->eLastEr = e_eFSS_CORELL_RES_OK;
                            m_bIsErased2 = false;
                            (void)memcpy(m_auStorArea2, p_puDataToWrite, p_uDataToWriteL);
                        }
                    }

                }
            }
        }
    }

    return l_bRes;
}

static bool_t eFSS_CORELLTST_ReadAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                       const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                       const uint32_t p_uReadBufferL )
{
    bool_t l_bRes;
    (void)p_uReadBufferL;
    (void)p_uPageToRead;

    if( NULL == p_ptCtx )
    {
        l_bRes = false;
    }
    else
    {
        p_ptCtx->uTimeUsed++;

        if( NULL == p_puReadBuffer )
        {
            l_bRes = false;
            p_ptCtx->eLastEr = e_eFSS_CORELL_RES_BADPOINTER;
        }
        else
        {
            l_bRes = true;
            p_ptCtx->eLastEr = e_eFSS_CORELL_RES_OK;
        }
    }

    return l_bRes;
}

static bool_t eFSS_CORELLTST_ReadErrAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                       const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                       const uint32_t p_uReadBufferL )
{
    bool_t l_bRes;
    (void)p_uReadBufferL;
    (void)p_uPageToRead;

    if( NULL == p_ptCtx )
    {
        l_bRes = false;
    }
    else
    {
        p_ptCtx->uTimeUsed++;

        if( NULL == p_puReadBuffer )
        {
            l_bRes = false;
            p_ptCtx->eLastEr = e_eFSS_CORELL_RES_BADPOINTER;
        }
        else
        {
            l_bRes = false;
            p_ptCtx->eLastEr = e_eFSS_CORELL_RES_BADPOINTER;
        }
    }

    return l_bRes;
}

static bool_t eFSS_CORELLTST_ReadTst1Adapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                       const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                       const uint32_t p_uReadBufferL )
{
    bool_t l_bRes;

    if( NULL == p_ptCtx )
    {
        l_bRes = false;
    }
    else
    {
        if( NULL == p_puReadBuffer )
        {
            l_bRes = false;
            p_ptCtx->eLastEr = e_eFSS_CORELL_RES_BADPOINTER;
        }
        else
        {
            if( p_uPageToRead >= 2u )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_CORELL_RES_BADPARAM;
            }
            else
            {
                if( 24u != p_uReadBufferL )
                {
                    l_bRes = false;
                    p_ptCtx->eLastEr = e_eFSS_CORELL_RES_BADPARAM;
                }
                else
                {
                    p_ptCtx->uTimeUsed++;

                    if( 0u == p_uPageToRead )
                    {
                        l_bRes = true;
                        p_ptCtx->eLastEr = e_eFSS_CORELL_RES_OK;
                        (void)memcpy(p_puReadBuffer, m_auStorArea1, p_uReadBufferL);
                    }
                    else
                    {
                        l_bRes = true;
                        p_ptCtx->eLastEr = e_eFSS_CORELL_RES_OK;
                        (void)memcpy(p_puReadBuffer, m_auStorArea2, p_uReadBufferL);
                    }
                }
            }
        }
    }

    return l_bRes;
}

static bool_t eFSS_CORELLTST_ReadTst2Adapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                       const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                       const uint32_t p_uReadBufferL )
{
    bool_t l_bRes;

    if( NULL == p_ptCtx )
    {
        l_bRes = false;
    }
    else
    {
        if( NULL == p_puReadBuffer )
        {
            l_bRes = false;
            p_ptCtx->eLastEr = e_eFSS_CORELL_RES_BADPOINTER;
        }
        else
        {
            if( p_uPageToRead >= 2u )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_CORELL_RES_BADPARAM;
            }
            else
            {
                if( 24u != p_uReadBufferL )
                {
                    l_bRes = false;
                    p_ptCtx->eLastEr = e_eFSS_CORELL_RES_BADPARAM;
                }
                else
                {
                    p_ptCtx->uTimeUsed++;

                    if( 0u == p_uPageToRead )
                    {
                        l_bRes = true;
                        p_ptCtx->eLastEr = e_eFSS_CORELL_RES_OK;
                        (void)memcpy(p_puReadBuffer, m_auStorArea1, p_uReadBufferL);
                        p_puReadBuffer[0] = p_puReadBuffer[0] + 2u;
                    }
                    else
                    {
                        l_bRes = true;
                        p_ptCtx->eLastEr = e_eFSS_CORELL_RES_OK;
                        (void)memcpy(p_puReadBuffer, m_auStorArea2, p_uReadBufferL);
                        p_puReadBuffer[0] = p_puReadBuffer[0] + 2u;
                    }
                }
            }
        }
    }

    return l_bRes;
}

static bool_t eFSS_CORELLTST_CrcAdapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
                                      const uint8_t* p_puData, const uint32_t p_uDataL,
                                      uint32_t* const p_puCrc32Val )
{
    bool_t l_bRes;
    (void)p_uUseed;
    (void)p_uDataL;

    if( NULL == p_ptCtx )
    {
        l_bRes = false;
    }
    else
    {
        p_ptCtx->uTimeUsed++;

        if( ( NULL == p_puData ) || ( NULL == p_puCrc32Val ) )
        {
            l_bRes = false;
            p_ptCtx->eLastEr = e_eFSS_CORELL_RES_BADPOINTER;
        }
        else
        {
            l_bRes = true;
            *p_puCrc32Val = 0u;
            p_ptCtx->eLastEr = e_eFSS_CORELL_RES_OK;
        }
    }

    return l_bRes;
}

static bool_t eFSS_CORELLTST_CrcErrAdapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
                                         const uint8_t* p_puData, const uint32_t p_uDataL,
                                         uint32_t* const p_puCrc32Val )
{
    bool_t l_bRes;
    (void)p_uUseed;
    (void)p_uDataL;

    if( NULL == p_ptCtx )
    {
        l_bRes = false;
    }
    else
    {
        p_ptCtx->uTimeUsed++;

        if( ( NULL == p_puData ) || ( NULL == p_puCrc32Val ) )
        {
            l_bRes = false;
            p_ptCtx->eLastEr = e_eFSS_CORELL_RES_BADPOINTER;
        }
        else
        {
            l_bRes = false;
            *p_puCrc32Val = 0u;
            p_ptCtx->eLastEr = e_eFSS_CORELL_RES_BADPOINTER;
        }
    }

    return l_bRes;
}

static bool_t eFSS_CORELLTST_CrcTst1Adapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
                                          const uint8_t* p_puData, const uint32_t p_uDataL,
                                          uint32_t* const p_puCrc32Val )
{
    bool_t l_bRes;
    uint32_t l_uCnt;

    if( NULL == p_ptCtx )
    {
        l_bRes = false;
    }
    else
    {
        p_ptCtx->uTimeUsed++;

        if( ( NULL == p_puData ) || ( NULL == p_puCrc32Val ) )
        {
            l_bRes = false;
            p_ptCtx->eLastEr = e_eFSS_CORELL_RES_BADPOINTER;
        }
        else
        {
            if( 0u == p_uDataL )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_CORELL_RES_BADPARAM;
            }
            else
            {
                l_bRes = true;
                p_ptCtx->eLastEr = e_eFSS_CORELL_RES_OK;
                *p_puCrc32Val = p_uUseed;

                for(l_uCnt = 0u; l_uCnt < p_uDataL; l_uCnt++ )
                {
                    *p_puCrc32Val = ( (*p_puCrc32Val) + (p_puData[l_uCnt]) );
                }
            }
        }
    }

    return l_bRes;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-8.13"
#endif

/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-2.2_b"
    /* Suppressed for code clarity in test execution*/
#endif

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

    /* Misra complaiant */
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxCrc32.uTimeUsed = 0u;

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

    /* Misra complaiant */
    (void)l_tCtxErase.eLastEr;
    (void)l_tCtxErase.uTimeUsed;
    (void)l_tCtxWrite.eLastEr;
    (void)l_tCtxWrite.uTimeUsed;
    (void)l_tCtxRead.eLastEr;
    (void)l_tCtxRead.uTimeUsed;
    (void)l_tCtxCrc32.eLastEr;
    (void)l_tCtxCrc32.uTimeUsed;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-2.2_b"
#endif

void eFSS_CORELLTST_BadInit(void)
{
    /* Local variable */
    t_eFSS_CORELL_Ctx l_tCtx;
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tGetStorSet;
    t_eFSS_CORELL_StorBuf l_ltUseBuff1;
    t_eFSS_CORELL_StorBuf l_ltUseBuff2;
    uint32_t l_uCrcGetted;

    /* Init the remainings var */
    (void)memset(&l_tCtx, 0, sizeof(l_tCtx));

    /* Function */
    if( e_eFSS_CORELL_RES_NOINITLIB == eFSS_CORELL_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_BadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadInit 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_NOINITLIB == eFSS_CORELL_GetBuff(&l_tCtx, &l_ltUseBuff1, &l_ltUseBuff2) )
    {
        (void)printf("eFSS_CORELLTST_BadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadInit 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_NOINITLIB == eFSS_CORELL_GetBuffNStor(&l_tCtx, &l_ltUseBuff1, &l_ltUseBuff2, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_BadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadInit 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_NOINITLIB == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u) )
    {
        (void)printf("eFSS_CORELLTST_BadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadInit 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_NOINITLIB == eFSS_CORELL_FlushBuffInPage(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u) )
    {
        (void)printf("eFSS_CORELLTST_BadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadInit 5  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_NOINITLIB == eFSS_CORELL_CalcCrcInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u, 1u, &l_uCrcGetted) )
    {
        (void)printf("eFSS_CORELLTST_BadInit 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadInit 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_IsInit(&l_tCtx, &l_bIsInit) )
    {
        if( false == l_bIsInit )
        {
            (void)printf("eFSS_CORELLTST_BadInit 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_BadInit 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadInit 7  -- FAIL \n");
    }
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-10.5"
    /* Suppressed for code clarity in test execution*/
#endif

void eFSS_CORELLTST_BadParamEntr(void)
{
    /* Local variable */
    t_eFSS_CORELL_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_uStorType;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
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

    /* Misra complaiant */
    (void)l_tCtxErase.eLastEr;
    (void)l_tCtxErase.uTimeUsed;
    (void)l_tCtxWrite.eLastEr;
    (void)l_tCtxWrite.uTimeUsed;
    (void)l_tCtxRead.eLastEr;
    (void)l_tCtxRead.uTimeUsed;
    (void)l_tCtxCrc32.eLastEr;
    (void)l_tCtxCrc32.uTimeUsed;

    /* Init storage settings */
    l_tStorSet.uTotPages = 1u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 2u;
    l_tStorSet.uPageVersion = 1u;

    l_uStorType = 1u;

    /* Function */
    l_tStorSet.uPagesLen = 25u;
    if( e_eFSS_CORELL_RES_BADPARAM == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 1  -- OK \n");
        l_tStorSet.uPagesLen = 24u;
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 1  -- FAIL \n");
        l_tStorSet.uPagesLen = 24u;
    }

    /* Function */
    l_tStorSet.uPagesLen = 23u;
    if( e_eFSS_CORELL_RES_BADPARAM == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 2  -- OK \n");
        l_tStorSet.uPagesLen = 24u;
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 2  -- FAIL \n");
        l_tStorSet.uPagesLen = 24u;
    }

    /* Function */
    l_tStorSet.uTotPages = 0u;
    if( e_eFSS_CORELL_RES_BADPARAM == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 3  -- OK \n");
        l_tStorSet.uTotPages = 1u;
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 3  -- FAIL \n");
        l_tStorSet.uTotPages = 1u;
    }

    /* Function */
    l_tStorSet.uPagesLen = 19u;
    if( e_eFSS_CORELL_RES_BADPARAM == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, l_tStorSet.uPagesLen * 2u ) )
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 4  -- OK \n");
        l_tStorSet.uPagesLen = 24u;
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 4  -- FAIL \n");
        l_tStorSet.uPagesLen = 24u;
    }

    /* Function */
    l_tStorSet.uPagesLen = 18u;
    if( e_eFSS_CORELL_RES_BADPARAM == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, l_tStorSet.uPagesLen * 2u ) )
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 5  -- OK \n");
        l_tStorSet.uPagesLen = 24u;
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 5  -- FAIL \n");
        l_tStorSet.uPagesLen = 24u;
    }

    /* Function */
    l_tStorSet.uRWERetry = 0u;
    if( e_eFSS_CORELL_RES_BADPARAM == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 6  -- OK \n");
        l_tStorSet.uRWERetry = 1u;
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 6  -- FAIL \n");
        l_tStorSet.uRWERetry = 1u;
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_BADPARAM == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 1u) )
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 8  -- OK \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_BADPARAM == eFSS_CORELL_LoadPageInBuff(&l_tCtx, (e_eFSS_CORELL_BUFFTYPE)2u, 0u) )
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 9  -- OK \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_BADPARAM == eFSS_CORELL_FlushBuffInPage(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 1u) )
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 10 -- OK \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_BADPARAM == eFSS_CORELL_FlushBuffInPage(&l_tCtx, (e_eFSS_CORELL_BUFFTYPE)2u, 0u) )
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 11 -- OK \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_BADPARAM == eFSS_CORELL_CalcCrcInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u, 0u, &l_uCrcGetted) )
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 12 -- OK \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_BADPARAM == eFSS_CORELL_CalcCrcInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u, 6u, &l_uCrcGetted) )
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 12 -- OK \n");
    }
    /* Function */
    if( e_eFSS_CORELL_RES_BADPARAM == eFSS_CORELL_CalcCrcInBuff(&l_tCtx, (e_eFSS_CORELL_BUFFTYPE)2u, 0u, 5u, &l_uCrcGetted) )
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadParamEntr 12 -- OK \n");
    }
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.5"
#endif

void eFSS_CORELLTST_CorruptedCtx(void)
{
    /* Local variable */
    t_eFSS_CORELL_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_uStorType;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
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
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 2  -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCtxCb.ptCtxErase = NULL;
    if( e_eFSS_CORELL_RES_CORRUPTCTX == eFSS_CORELL_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 3  -- OK \n");
        l_tCtx.tCtxCb.ptCtxErase = &l_tCtxErase;
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 3  -- FAIL \n");
        l_tCtx.tCtxCb.ptCtxErase = &l_tCtxErase;
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 4  -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCtxCb.fErase = NULL;
    if( e_eFSS_CORELL_RES_CORRUPTCTX == eFSS_CORELL_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 5  -- OK \n");
        l_tCtx.tCtxCb.fErase = &eFSS_CORELLTST_EraseAdapt;
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 5  -- FAIL \n");
        l_tCtx.tCtxCb.fErase = &eFSS_CORELLTST_EraseAdapt;
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 6  -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCtxCb.ptCtxWrite = NULL;
    if( e_eFSS_CORELL_RES_CORRUPTCTX == eFSS_CORELL_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 7  -- OK \n");
        l_tCtx.tCtxCb.ptCtxWrite = &l_tCtxWrite;
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 7  -- FAIL \n");
        l_tCtx.tCtxCb.ptCtxWrite = &l_tCtxWrite;
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 8  -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCtxCb.fWrite = NULL;
    if( e_eFSS_CORELL_RES_CORRUPTCTX == eFSS_CORELL_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 9  -- OK \n");
        l_tCtx.tCtxCb.fWrite = &eFSS_CORELLTST_WriteAdapt;
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 9  -- FAIL \n");
        l_tCtx.tCtxCb.fWrite = &eFSS_CORELLTST_WriteAdapt;
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 10 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCtxCb.ptCtxRead = NULL;
    if( e_eFSS_CORELL_RES_CORRUPTCTX == eFSS_CORELL_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 11 -- OK \n");
        l_tCtx.tCtxCb.ptCtxRead = &l_tCtxRead;
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 11 -- FAIL \n");
        l_tCtx.tCtxCb.ptCtxRead = &l_tCtxRead;
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 12 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCtxCb.fRead = NULL;
    if( e_eFSS_CORELL_RES_CORRUPTCTX == eFSS_CORELL_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 13 -- OK \n");
        l_tCtx.tCtxCb.fRead = &eFSS_CORELLTST_ReadAdapt;
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 13 -- FAIL \n");
        l_tCtx.tCtxCb.fRead = &eFSS_CORELLTST_ReadAdapt;
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 14 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCtxCb.ptCtxCrc32 = NULL;
    if( e_eFSS_CORELL_RES_CORRUPTCTX == eFSS_CORELL_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 15 -- OK \n");
        l_tCtx.tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 15 -- FAIL \n");
        l_tCtx.tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 16 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCtxCb.fCrc32 = NULL;
    if( e_eFSS_CORELL_RES_CORRUPTCTX == eFSS_CORELL_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 17 -- OK \n");
        l_tCtx.tCtxCb.fCrc32 = &eFSS_CORELLTST_CrcAdapt;
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 17 -- FAIL \n");
        l_tCtx.tCtxCb.fCrc32 = &eFSS_CORELLTST_CrcAdapt;
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 18 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tBuff1.puBuf = NULL;
    if( e_eFSS_CORELL_RES_CORRUPTCTX == eFSS_CORELL_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 19 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 20 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tBuff2.puBuf = NULL;
    if( e_eFSS_CORELL_RES_CORRUPTCTX == eFSS_CORELL_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 21 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 22 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tStorSett.uTotPages = 0u;
    if( e_eFSS_CORELL_RES_CORRUPTCTX == eFSS_CORELL_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 23 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 24 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tStorSett.uPagesLen = 19u;
    if( e_eFSS_CORELL_RES_CORRUPTCTX == eFSS_CORELL_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 25 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 26 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tStorSett.uRWERetry = 0u;
    if( e_eFSS_CORELL_RES_CORRUPTCTX == eFSS_CORELL_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 27 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 27 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 28 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 28 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tBuff1.uBufL = l_tCtx.tBuff2.uBufL + 1u;
    if( e_eFSS_CORELL_RES_CORRUPTCTX == eFSS_CORELL_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 29 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 29 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 30 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 30 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tStorSett.uPagesLen = l_tCtx.tBuff1.uBufL + 1u;
    if( e_eFSS_CORELL_RES_CORRUPTCTX == eFSS_CORELL_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 31 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 31 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 32 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 32 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tStorSett.uPagesLen = l_tCtx.tBuff1.uBufL + 1u;
    if( e_eFSS_CORELL_RES_CORRUPTCTX == eFSS_CORELL_GetBuff(&l_tCtx, &l_ltUseBuff1, &l_ltUseBuff2) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 33 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 33 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 34 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 34 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tStorSett.uPagesLen = l_tCtx.tBuff1.uBufL + 1u;
    if( e_eFSS_CORELL_RES_CORRUPTCTX == eFSS_CORELL_GetBuffNStor(&l_tCtx, &l_ltUseBuff1, &l_ltUseBuff2, &l_tGetStorSet) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 35 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 35 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 36 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 36 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tStorSett.uPagesLen = l_tCtx.tBuff1.uBufL + 1u;
    if( e_eFSS_CORELL_RES_CORRUPTCTX == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 37 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 37 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 38 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 38 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tStorSett.uPagesLen = l_tCtx.tBuff1.uBufL + 1u;
    if( e_eFSS_CORELL_RES_CORRUPTCTX == eFSS_CORELL_FlushBuffInPage(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 39 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 39 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 40 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 40 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tStorSett.uPagesLen = l_tCtx.tBuff1.uBufL + 1u;
    if( e_eFSS_CORELL_RES_CORRUPTCTX == eFSS_CORELL_CalcCrcInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u, 5u, &l_uCrcGetted) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 41 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 41 -- FAIL \n");
    }


    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 42 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 42 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tBuff1 = l_tCtx.tBuff2;
    if( e_eFSS_CORELL_RES_CORRUPTCTX == eFSS_CORELL_CalcCrcInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u, 5u, &l_uCrcGetted) )
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 43 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CorruptedCtx 43 -- FAIL \n");
    }

    /* Misra complaiant */
    (void)l_tCtxErase.eLastEr;
    (void)l_tCtxErase.uTimeUsed;
    (void)l_tCtxWrite.eLastEr;
    (void)l_tCtxWrite.uTimeUsed;
    (void)l_tCtxRead.eLastEr;
    (void)l_tCtxRead.uTimeUsed;
    (void)l_tCtxCrc32.eLastEr;
    (void)l_tCtxCrc32.uTimeUsed;
}

void eFSS_CORELLTST_Basic(void)
{
    /* Local variable */
    t_eFSS_CORELL_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_uStorType;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tGetStorSet;
    t_eFSS_CORELL_StorBuf l_ltUseBuff1;
    t_eFSS_CORELL_StorBuf l_ltUseBuff2;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_CORELLTST_EraseAdapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_CORELLTST_WriteAdapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_CORELLTST_ReadErrAdapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_CORELLTST_CrcAdapt;

    /* Misra complaiant */
    (void)l_tCtxErase.eLastEr;
    (void)l_tCtxErase.uTimeUsed;
    (void)l_tCtxWrite.eLastEr;
    (void)l_tCtxWrite.uTimeUsed;
    (void)l_tCtxRead.eLastEr;
    (void)l_tCtxRead.uTimeUsed;
    (void)l_tCtxCrc32.eLastEr;
    (void)l_tCtxCrc32.uTimeUsed;

    /* Init storage settings */
    l_tStorSet.uTotPages = 1u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;
    l_uStorType = 1u;

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_Basic 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_Basic 1  -- FAIL \n");
    }

    /* Verify buffer validity */
    if( l_auStor == l_tCtx.tBuff1.puBuf )
    {
        (void)printf("eFSS_CORELLTST_Basic 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_Basic 2  -- FAIL \n");
    }

    /* Verify buffer validity */
    if( &l_auStor[24u] == l_tCtx.tBuff2.puBuf )
    {
        (void)printf("eFSS_CORELLTST_Basic 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_Basic 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_IsInit(&l_tCtx, &l_bIsInit ) )
    {
        if( true == l_bIsInit )
        {
            (void)printf("eFSS_CORELLTST_Basic 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_Basic 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_Basic 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_GetBuff(&l_tCtx, &l_ltUseBuff1, &l_ltUseBuff2) )
    {
        if( ( l_auStor == l_ltUseBuff1.puBuf ) && ( &l_auStor[24u] == l_ltUseBuff2.puBuf ) &&
            ( (24u-19u) == l_ltUseBuff1.uBufL ) && ( (24u-19u) == l_ltUseBuff2.uBufL ) )
        {
            (void)printf("eFSS_CORELLTST_Basic 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_Basic 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_Basic 5  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_GetBuffNStor(&l_tCtx, &l_ltUseBuff1, &l_ltUseBuff2, &l_tGetStorSet) )
    {
        if( ( l_auStor == l_ltUseBuff1.puBuf ) && ( &l_auStor[24u] == l_ltUseBuff2.puBuf ) &&
            ( (24u-19u) == l_ltUseBuff1.uBufL ) && ( (24u-19u) == l_ltUseBuff2.uBufL ) )
        {
            (void)printf("eFSS_CORELLTST_Basic 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_Basic 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_Basic 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_GetBuffNStor(&l_tCtx, &l_ltUseBuff1, &l_ltUseBuff2, &l_tGetStorSet) )
    {
        if( ( 1u == l_tGetStorSet.uTotPages ) && ( 24u == l_tGetStorSet.uPagesLen ) &&
            ( 3u == l_tGetStorSet.uRWERetry ) && ( 1u == l_tGetStorSet.uPageVersion ) )
        {
            (void)printf("eFSS_CORELLTST_Basic 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_Basic 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_Basic 7  -- FAIL \n");
    }


    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        if( ( 1u == l_tGetStorSet.uTotPages ) && ( 24u == l_tGetStorSet.uPagesLen ) &&
            ( 3u == l_tGetStorSet.uRWERetry ) && ( 1u == l_tGetStorSet.uPageVersion ) )
        {
            (void)printf("eFSS_CORELLTST_Basic 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_Basic 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_Basic 8  -- FAIL \n");
    }
}

void eFSS_CORELLTST_BadClBckNRetry(void)
{
    /* Local variable */
    t_eFSS_CORELL_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_uStorType;
    uint8_t l_auStor[48u];
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
    l_tCtxCb.fRead = &eFSS_CORELLTST_ReadErrAdapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_CORELLTST_CrcAdapt;

    /* Misra complaiant */
    (void)l_tCtxErase.eLastEr;
    (void)l_tCtxErase.uTimeUsed;
    (void)l_tCtxWrite.eLastEr;
    (void)l_tCtxWrite.uTimeUsed;
    (void)l_tCtxRead.eLastEr;
    (void)l_tCtxRead.uTimeUsed;
    (void)l_tCtxCrc32.eLastEr;
    (void)l_tCtxCrc32.uTimeUsed;

    /* Init storage settings */
    l_tStorSet.uTotPages = 1u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;
    l_uStorType = 1u;

    /* ------------------------------------------------------------------------------------------ TEST READ CALL BACK */
    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_IsInit(&l_tCtx, &l_bIsInit ) )
    {
        if( true == l_bIsInit )
        {
            (void)printf("eFSS_CORELLTST_BadClBckNRetry 2  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_BadClBckNRetry 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_GetBuffNStor(&l_tCtx, &l_ltUseBuff1, &l_ltUseBuff2, &l_tGetStorSet) )
    {
        if( ( l_auStor == l_ltUseBuff1.puBuf ) && ( &l_auStor[24u] == l_ltUseBuff2.puBuf ) &&
            ( (24u-19u) == l_ltUseBuff1.uBufL ) && ( (24u-19u) == l_ltUseBuff2.uBufL ) )
        {
            (void)printf("eFSS_CORELLTST_BadClBckNRetry 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_BadClBckNRetry 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_IsInit(&l_tCtx, &l_bIsInit ) )
    {
        if( true == l_bIsInit )
        {
            (void)printf("eFSS_CORELLTST_BadClBckNRetry 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_BadClBckNRetry 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 4  -- FAIL \n");
    }

    /* Test LOAD */
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;
    if( e_eFSS_CORELL_RES_CLBCKREADERR == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u) )
    {
        if( ( l_tStorSet.uRWERetry == l_tCtxRead.uTimeUsed ) && ( e_eFSS_CORELL_RES_BADPOINTER == l_tCtxRead.eLastEr ) &&
            ( 0u == l_tCtxCrc32.uTimeUsed ) )
        {
            (void)printf("eFSS_CORELLTST_BadClBckNRetry 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_BadClBckNRetry 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 5  -- FAIL \n");
    }

    /* Test LOAD */
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;
    if( e_eFSS_CORELL_RES_CLBCKREADERR == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 0u) )
    {
        if( ( l_tStorSet.uRWERetry == l_tCtxRead.uTimeUsed ) && ( e_eFSS_CORELL_RES_BADPOINTER == l_tCtxRead.eLastEr ) &&
            ( 0u == l_tCtxCrc32.uTimeUsed ) )
        {
            (void)printf("eFSS_CORELLTST_BadClBckNRetry 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_BadClBckNRetry 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 6  -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fRead = &eFSS_CORELLTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_CORELLTST_CrcErrAdapt;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_GetBuff(&l_tCtx, &l_ltUseBuff1, &l_ltUseBuff2) )
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 8  -- FAIL \n");
    }

    /* Test LOAD */
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;

    if( e_eFSS_CORELL_RES_CLBCKCRCERR == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u) )
    {
        if( ( 1u == l_tCtxRead.uTimeUsed ) && ( e_eFSS_CORELL_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_CORELL_RES_BADPOINTER == l_tCtxCrc32.eLastEr ) )
            {
                (void)printf("eFSS_CORELLTST_BadClBckNRetry 9  -- OK \n");
            }
            else
            {
                (void)printf("eFSS_CORELLTST_BadClBckNRetry 9  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_CORELLTST_BadClBckNRetry 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 9  -- FAIL \n");
    }

    /* Test LOAD */
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;

    if( e_eFSS_CORELL_RES_CLBCKCRCERR == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 0u) )
    {
        if( ( 1u == l_tCtxRead.uTimeUsed ) && ( e_eFSS_CORELL_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_CORELL_RES_BADPOINTER == l_tCtxCrc32.eLastEr ) )
            {
                (void)printf("eFSS_CORELLTST_BadClBckNRetry 10 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_CORELLTST_BadClBckNRetry 10 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_CORELLTST_BadClBckNRetry 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 10 -- FAIL \n");
    }

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fRead = &eFSS_CORELLTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_CORELLTST_CrcErrAdapt;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 11 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_GetBuff(&l_tCtx, &l_ltUseBuff1, &l_ltUseBuff2) )
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 12 -- FAIL \n");
    }

    /* Test LOAD */
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;

    if( e_eFSS_CORELL_RES_CLBCKCRCERR == eFSS_CORELL_CalcCrcInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u, 5u, &l_uCrcGetted) )
    {
        if( ( 0u == l_tCtxRead.uTimeUsed ) && ( e_eFSS_CORELL_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_CORELL_RES_BADPOINTER == l_tCtxCrc32.eLastEr ) )
            {
                (void)printf("eFSS_CORELLTST_BadClBckNRetry 13 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_CORELLTST_BadClBckNRetry 13 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_CORELLTST_BadClBckNRetry 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 13 -- FAIL \n");
    }

    /* ----------------------------------------------------------------------------------------- TEST WRITE CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_CORELLTST_EraseAdapt;
    l_tCtxCb.fWrite = &eFSS_CORELLTST_WriteAdapt;
    l_tCtxCb.fRead = &eFSS_CORELLTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_CORELLTST_CrcErrAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;

    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 14 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_GetBuff(&l_tCtx, &l_ltUseBuff1, &l_ltUseBuff2) )
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 15 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;

    if( e_eFSS_CORELL_RES_CLBCKCRCERR == eFSS_CORELL_FlushBuffInPage(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u) )
    {
        if( ( 0u == l_tCtxErase.uTimeUsed ) && ( e_eFSS_CORELL_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 0u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_CORELL_RES_OK == l_tCtxWrite.eLastEr ) &&
            ( 0u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_CORELL_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_CORELL_RES_BADPOINTER == l_tCtxCrc32.eLastEr ) )
            {
                (void)printf("eFSS_CORELLTST_BadClBckNRetry 16 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_CORELLTST_BadClBckNRetry 16 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_CORELLTST_BadClBckNRetry 16 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 16 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;

    if( e_eFSS_CORELL_RES_CLBCKCRCERR == eFSS_CORELL_FlushBuffInPage(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 0u) )
    {
        if( ( 0u == l_tCtxErase.uTimeUsed ) && ( e_eFSS_CORELL_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 0u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_CORELL_RES_OK == l_tCtxWrite.eLastEr ) &&
            ( 0u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_CORELL_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_CORELL_RES_BADPOINTER == l_tCtxCrc32.eLastEr ) )
            {
                (void)printf("eFSS_CORELLTST_BadClBckNRetry 17 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_CORELLTST_BadClBckNRetry 17 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_CORELLTST_BadClBckNRetry 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 17 -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_CORELLTST_EraseErrAdapt;
    l_tCtxCb.fWrite = &eFSS_CORELLTST_WriteAdapt;
    l_tCtxCb.fRead = &eFSS_CORELLTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_CORELLTST_CrcAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;

    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 18 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_GetBuff(&l_tCtx, &l_ltUseBuff1, &l_ltUseBuff2) )
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 19 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;

    if( e_eFSS_CORELL_RES_CLBCKERASEERR == eFSS_CORELL_FlushBuffInPage(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_CORELL_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 0u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_CORELL_RES_OK == l_tCtxWrite.eLastEr ) &&
            ( 0u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_CORELL_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 3u == l_tCtxErase.uTimeUsed ) && ( e_eFSS_CORELL_RES_BADPOINTER == l_tCtxErase.eLastEr ) )
            {
                (void)printf("eFSS_CORELLTST_BadClBckNRetry 20 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_CORELLTST_BadClBckNRetry 20 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_CORELLTST_BadClBckNRetry 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 20 -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_CORELLTST_EraseAdapt;
    l_tCtxCb.fWrite = &eFSS_CORELLTST_WriteErrAdapt;
    l_tCtxCb.fRead = &eFSS_CORELLTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_CORELLTST_CrcAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;

    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 21 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_GetBuff(&l_tCtx, &l_ltUseBuff1, &l_ltUseBuff2) )
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 22 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;

    if( e_eFSS_CORELL_RES_CLBCKWRITEERR == eFSS_CORELL_FlushBuffInPage(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_CORELL_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 3u == l_tCtxErase.uTimeUsed ) && ( e_eFSS_CORELL_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 0u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_CORELL_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 3u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_CORELL_RES_BADPOINTER == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_CORELLTST_BadClBckNRetry 23 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_CORELLTST_BadClBckNRetry 23 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_CORELLTST_BadClBckNRetry 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 23 -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_CORELLTST_EraseAdapt;
    l_tCtxCb.fWrite = &eFSS_CORELLTST_WriteAdapt;
    l_tCtxCb.fRead = &eFSS_CORELLTST_ReadErrAdapt;
    l_tCtxCb.fCrc32 = &eFSS_CORELLTST_CrcAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;

    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 24 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_GetBuff(&l_tCtx, &l_ltUseBuff1, &l_ltUseBuff2) )
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 25 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;

    if( e_eFSS_CORELL_RES_CLBCKREADERR == eFSS_CORELL_FlushBuffInPage(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 0u) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_CORELL_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 3u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_CORELL_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 3u == l_tCtxWrite.uTimeUsed )  && ( e_eFSS_CORELL_RES_OK == l_tCtxWrite.eLastEr ) )
        {
            if( ( 3u == l_tCtxRead.uTimeUsed ) && ( e_eFSS_CORELL_RES_BADPOINTER == l_tCtxRead.eLastEr ) )
            {
                (void)printf("eFSS_CORELLTST_BadClBckNRetry 26 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_CORELLTST_BadClBckNRetry 26 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_CORELLTST_BadClBckNRetry 26 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_BadClBckNRetry 26 -- FAIL \n");
    }
}

static void eFSS_CORELLTST_CrcTest(void)
{
    /* Local variable */
    t_eFSS_CORELL_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_uStorType;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_CORELL_StorBuf l_ltUseBuff1;
    t_eFSS_CORELL_StorBuf l_ltUseBuff2;
    uint32_t l_uCrcGetted;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_CORELLTST_EraseAdapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_CORELLTST_WriteAdapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_CORELLTST_ReadErrAdapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_CORELLTST_CrcAdapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 1u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;
    l_uStorType = 1u;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_CORELLTST_EraseAdapt;
    l_tCtxCb.fWrite = &eFSS_CORELLTST_WriteAdapt;
    l_tCtxCb.fRead = &eFSS_CORELLTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_CORELLTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;

    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_CrcTest 1 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CrcTest 1 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_GetBuff(&l_tCtx, &l_ltUseBuff1, &l_ltUseBuff2) )
    {
        (void)printf("eFSS_CORELLTST_CrcTest 2 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CrcTest 2 -- FAIL \n");
    }

    /* Setup buffer */
    l_ltUseBuff1.puBuf[0u] = 0x01u;
    l_ltUseBuff1.puBuf[1u] = 0x00u;
    l_ltUseBuff1.puBuf[2u] = 0x00u;
    l_ltUseBuff1.puBuf[3u] = 0x00u;
    l_ltUseBuff1.puBuf[4u] = 0x00u;

    l_ltUseBuff2.puBuf[0u] = 0x02u;
    l_ltUseBuff2.puBuf[1u] = 0x00u;
    l_ltUseBuff2.puBuf[2u] = 0x00u;
    l_ltUseBuff2.puBuf[3u] = 0x00u;
    l_ltUseBuff2.puBuf[4u] = 0x00u;

    /* Function */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_CalcCrcInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u, l_ltUseBuff1.uBufL, &l_uCrcGetted) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_CORELL_RES_OK == l_tCtxCrc32.eLastEr ) )
        {
            if( 5u == l_ltUseBuff1.uBufL )
            {
                if( 1u == l_uCrcGetted )
                {
                    (void)printf("eFSS_CORELLTST_CrcTest 3 -- OK \n");
                }
                else
                {
                    (void)printf("eFSS_CORELLTST_CrcTest 3 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eFSS_CORELLTST_CrcTest 3 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_CORELLTST_CrcTest 3 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CrcTest 3 -- FAIL \n");
    }

    /* Function */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;

    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_CalcCrcInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 0u, l_ltUseBuff1.uBufL, &l_uCrcGetted) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_CORELL_RES_OK == l_tCtxCrc32.eLastEr ) )
        {
            if( 5u == l_ltUseBuff1.uBufL )
            {
                if( 2u == l_uCrcGetted )
                {
                    (void)printf("eFSS_CORELLTST_CrcTest 4 -- OK \n");
                }
                else
                {
                    (void)printf("eFSS_CORELLTST_CrcTest 4 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eFSS_CORELLTST_CrcTest 4 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_CORELLTST_CrcTest 4 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CrcTest 4 -- FAIL \n");
    }

    /* Function */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_CalcCrcInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0x10u, l_ltUseBuff1.uBufL, &l_uCrcGetted) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_CORELL_RES_OK == l_tCtxCrc32.eLastEr ) )
        {
            if( 5u == l_ltUseBuff1.uBufL )
            {
                if( 0x11u == l_uCrcGetted )
                {
                    (void)printf("eFSS_CORELLTST_CrcTest 5 -- OK \n");
                }
                else
                {
                    (void)printf("eFSS_CORELLTST_CrcTest 5 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eFSS_CORELLTST_CrcTest 5 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_CORELLTST_CrcTest 5 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CrcTest 5 -- FAIL \n");
    }

    /* Function */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;

    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_CalcCrcInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 0x10u, l_ltUseBuff1.uBufL, &l_uCrcGetted) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_CORELL_RES_OK == l_tCtxCrc32.eLastEr ) )
        {
            if( 5u == l_ltUseBuff1.uBufL )
            {
                if( 0x12u == l_uCrcGetted )
                {
                    (void)printf("eFSS_CORELLTST_CrcTest 6 -- OK \n");
                }
                else
                {
                    (void)printf("eFSS_CORELLTST_CrcTest 6 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eFSS_CORELLTST_CrcTest 6 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_CORELLTST_CrcTest 6 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CrcTest 6 -- FAIL \n");
    }

    /* Setup buffer */
    l_ltUseBuff1.puBuf[0u] = 0x01u;
    l_ltUseBuff1.puBuf[1u] = 0x02u;
    l_ltUseBuff1.puBuf[2u] = 0x03u;
    l_ltUseBuff1.puBuf[3u] = 0x04u;
    l_ltUseBuff1.puBuf[4u] = 0x05u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    /* Function */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_CalcCrcInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u, l_ltUseBuff1.uBufL, &l_uCrcGetted) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_CORELL_RES_OK == l_tCtxCrc32.eLastEr ) )
        {
            if( 5u == l_ltUseBuff1.uBufL )
            {
                if( 0x0Fu == l_uCrcGetted )
                {
                    (void)printf("eFSS_CORELLTST_CrcTest 7 -- OK \n");
                }
                else
                {
                    (void)printf("eFSS_CORELLTST_CrcTest 7 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eFSS_CORELLTST_CrcTest 7 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_CORELLTST_CrcTest 7 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CrcTest 7 -- FAIL \n");
    }

    /* Function */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;

    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_CalcCrcInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 0u, l_ltUseBuff1.uBufL, &l_uCrcGetted) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_CORELL_RES_OK == l_tCtxCrc32.eLastEr ) )
        {
            if( 5u == l_ltUseBuff1.uBufL )
            {
                if( 0x5Fu == l_uCrcGetted )
                {
                    (void)printf("eFSS_CORELLTST_CrcTest 8 -- OK \n");
                }
                else
                {
                    (void)printf("eFSS_CORELLTST_CrcTest 8 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eFSS_CORELLTST_CrcTest 8 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_CORELLTST_CrcTest 8 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CrcTest 8 -- FAIL \n");
    }

    /* Setup buffer */
    l_ltUseBuff1.puBuf[0u] = 0x01u;
    l_ltUseBuff1.puBuf[1u] = 0x02u;
    l_ltUseBuff1.puBuf[2u] = 0x03u;
    l_ltUseBuff1.puBuf[3u] = 0x04u;
    l_ltUseBuff1.puBuf[4u] = 0x05u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    /* Function */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_CalcCrcInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u, 2u, &l_uCrcGetted) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_CORELL_RES_OK == l_tCtxCrc32.eLastEr ) )
        {
            if( 5u == l_ltUseBuff1.uBufL )
            {
                if( 0x03u == l_uCrcGetted )
                {
                    (void)printf("eFSS_CORELLTST_CrcTest 9 -- OK \n");
                }
                else
                {
                    (void)printf("eFSS_CORELLTST_CrcTest 9 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eFSS_CORELLTST_CrcTest 9 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_CORELLTST_CrcTest 9 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CrcTest 9 -- FAIL \n");
    }

    /* Setup buffer */
    l_ltUseBuff1.puBuf[0u] = 0x01u;
    l_ltUseBuff1.puBuf[1u] = 0x02u;
    l_ltUseBuff1.puBuf[2u] = 0x03u;
    l_ltUseBuff1.puBuf[3u] = 0x04u;
    l_ltUseBuff1.puBuf[4u] = 0x05u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    /* Function */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_CalcCrcInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u, 1u, &l_uCrcGetted) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_CORELL_RES_OK == l_tCtxCrc32.eLastEr ) )
        {
            if( 5u == l_ltUseBuff1.uBufL )
            {
                if( 0x01u == l_uCrcGetted )
                {
                    (void)printf("eFSS_CORELLTST_CrcTest 10-- OK \n");
                }
                else
                {
                    (void)printf("eFSS_CORELLTST_CrcTest 10-- FAIL \n");
                }
            }
            else
            {
                (void)printf("eFSS_CORELLTST_CrcTest 10-- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_CORELLTST_CrcTest 10-- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CrcTest 10-- FAIL \n");
    }

    if( e_eFSS_CORELL_RES_BADPARAM == eFSS_CORELL_CalcCrcInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u, 6u, &l_uCrcGetted) )
    {
        (void)printf("eFSS_CORELLTST_CrcTest 11-- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_CrcTest 11-- FAIL \n");
    }

    /* Misra complaiant */
    (void)l_tCtxErase.eLastEr;
    (void)l_tCtxErase.uTimeUsed;
    (void)l_tCtxWrite.eLastEr;
    (void)l_tCtxWrite.uTimeUsed;
    (void)l_tCtxRead.eLastEr;
    (void)l_tCtxRead.uTimeUsed;
    (void)l_tCtxCrc32.eLastEr;
    (void)l_tCtxCrc32.uTimeUsed;
    (void)l_ltUseBuff2.puBuf;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "CERT-INT31-C_c"
    /* Suppressed for code clarity in test execution*/
#endif

static void eFSS_CORELLTST_LoadTest(void)
{
    /* Local variable */
    t_eFSS_CORELL_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_uStorType;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_CORELL_StorBuf l_ltUseBuff1;
    t_eFSS_CORELL_StorBuf l_ltUseBuff2;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_CORELLTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_CORELLTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_CORELLTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_CORELLTST_CrcTst1Adapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 2u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;
    l_uStorType = 1u;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_CORELLTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_CORELLTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_CORELLTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_CORELLTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;

    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_LoadTest 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_LoadTest 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_GetBuff(&l_tCtx, &l_ltUseBuff1, &l_ltUseBuff2) )
    {
        (void)printf("eFSS_CORELLTST_LoadTest 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_LoadTest 2  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0xFF, sizeof(m_auStorArea2));

    /* Setup buffer */
    l_ltUseBuff1.puBuf[0u] = 0x01u;
    l_ltUseBuff1.puBuf[1u] = 0x02u;
    l_ltUseBuff1.puBuf[2u] = 0x03u;
    l_ltUseBuff1.puBuf[3u] = 0x04u;
    l_ltUseBuff1.puBuf[4u] = 0x05u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_CORELL_RES_NOTVALIDPAGE == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u) )
    {
        if( ( 0u == l_ltUseBuff1.puBuf[0u] ) && ( 0u == l_ltUseBuff1.puBuf[1u] ) && ( 0u == l_ltUseBuff1.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff1.puBuf[3u] ) && ( 0u == l_ltUseBuff1.puBuf[4u] ))
        {
            (void)printf("eFSS_CORELLTST_LoadTest 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_LoadTest 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_LoadTest 3  -- FAIL \n");
    }

    if( e_eFSS_CORELL_RES_NOTVALIDPAGE == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 0u) )
    {
        if( ( 0u == l_ltUseBuff2.puBuf[0u] ) && ( 0u == l_ltUseBuff2.puBuf[1u] ) && ( 0u == l_ltUseBuff2.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff2.puBuf[3u] ) && ( 0u == l_ltUseBuff2.puBuf[4u] ))
        {
            (void)printf("eFSS_CORELLTST_LoadTest 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_LoadTest 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_LoadTest 4  -- FAIL \n");
    }

    if( e_eFSS_CORELL_RES_NOTVALIDPAGE == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 1u) )
    {
        if( ( 0xFFu == l_ltUseBuff1.puBuf[0u] ) && ( 0xFFu == l_ltUseBuff1.puBuf[1u] ) && ( 0xFFu == l_ltUseBuff1.puBuf[2u] ) &&
             ( 0xFFu == l_ltUseBuff1.puBuf[3u] ) && ( 0xFFu == l_ltUseBuff1.puBuf[4u] ))
        {
            (void)printf("eFSS_CORELLTST_LoadTest 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_LoadTest 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_LoadTest 5  -- FAIL \n");
    }

    if( e_eFSS_CORELL_RES_NOTVALIDPAGE == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 1u) )
    {
        if( ( 0xFFu == l_ltUseBuff2.puBuf[0u] ) && ( 0xFFu == l_ltUseBuff2.puBuf[1u] ) && ( 0xFFu == l_ltUseBuff2.puBuf[2u] ) &&
             ( 0xFFu == l_ltUseBuff2.puBuf[3u] ) && ( 0xFFu == l_ltUseBuff2.puBuf[4u] ))
        {
            (void)printf("eFSS_CORELLTST_LoadTest 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_LoadTest 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_LoadTest 6  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[20u] = 0xFFu;
    m_auStorArea1[21u] = 0xFFu;
    m_auStorArea1[22u] = 0xFFu;
    m_auStorArea1[23u] = 0xFFu;

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[20u] = 0x3Fu;
    m_auStorArea2[21u] = 0x01u;
    m_auStorArea2[22u] = 0x00u;
    m_auStorArea2[23u] = 0x00u;

    /* Setup buffer */
    l_ltUseBuff1.puBuf[0u] = 0x01u;
    l_ltUseBuff1.puBuf[1u] = 0x02u;
    l_ltUseBuff1.puBuf[2u] = 0x03u;
    l_ltUseBuff1.puBuf[3u] = 0x04u;
    l_ltUseBuff1.puBuf[4u] = 0x05u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_CORELL_RES_NOTVALIDPAGE == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u) )
    {
        if( ( 0u == l_ltUseBuff1.puBuf[0u] ) && ( 0u == l_ltUseBuff1.puBuf[1u] ) && ( 0u == l_ltUseBuff1.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff1.puBuf[3u] ) && ( 0u == l_ltUseBuff1.puBuf[4u] ))
        {
            (void)printf("eFSS_CORELLTST_LoadTest 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_LoadTest 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_LoadTest 7  -- FAIL \n");
    }

    if( e_eFSS_CORELL_RES_NOTVALIDPAGE == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 0u) )
    {
        if( ( 0u == l_ltUseBuff2.puBuf[0u] ) && ( 0u == l_ltUseBuff2.puBuf[1u] ) && ( 0u == l_ltUseBuff2.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff2.puBuf[3u] ) && ( 0u == l_ltUseBuff2.puBuf[4u] ))
        {
            (void)printf("eFSS_CORELLTST_LoadTest 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_LoadTest 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_LoadTest 8  -- FAIL \n");
    }

    if( e_eFSS_CORELL_RES_NOTVALIDPAGE == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 1u) )
    {
        if( ( 0x10u == l_ltUseBuff1.puBuf[0u] ) && ( 0x10u == l_ltUseBuff1.puBuf[1u] ) && ( 0x10u == l_ltUseBuff1.puBuf[2u] ) &&
             ( 0x10u == l_ltUseBuff1.puBuf[3u] ) && ( 0x10u == l_ltUseBuff1.puBuf[4u] ))
        {
            (void)printf("eFSS_CORELLTST_LoadTest 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_LoadTest 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_LoadTest 9  -- FAIL \n");
    }

    if( e_eFSS_CORELL_RES_NOTVALIDPAGE == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 1u) )
    {
        if( ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
             ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x10u == l_ltUseBuff2.puBuf[4u] ))
        {
            (void)printf("eFSS_CORELLTST_LoadTest 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_LoadTest 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_LoadTest 10 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[16u] = 0xA5u;
    m_auStorArea1[17u] = 0xA5u;
    m_auStorArea1[18u] = 0xA5u;
    m_auStorArea1[19u] = 0xA5u;
    m_auStorArea1[20u] = 0x93u;
    m_auStorArea1[21u] = 0x02u;
    m_auStorArea1[22u] = 0x00u;
    m_auStorArea1[23u] = 0x00u;

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[16u] = 0xA5u;
    m_auStorArea2[17u] = 0xA5u;
    m_auStorArea2[18u] = 0xA5u;
    m_auStorArea2[19u] = 0xA5u;
    m_auStorArea2[20u] = 0x93u;
    m_auStorArea2[21u] = 0x03u;
    m_auStorArea2[22u] = 0x00u;
    m_auStorArea2[23u] = 0x00u;

    /* Setup buffer */
    l_ltUseBuff1.puBuf[0u] = 0x01u;
    l_ltUseBuff1.puBuf[1u] = 0x02u;
    l_ltUseBuff1.puBuf[2u] = 0x03u;
    l_ltUseBuff1.puBuf[3u] = 0x04u;
    l_ltUseBuff1.puBuf[4u] = 0x05u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_CORELL_RES_NOTVALIDPAGE == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u) )
    {
        if( ( 0u == l_ltUseBuff1.puBuf[0u] ) && ( 0u == l_ltUseBuff1.puBuf[1u] ) && ( 0u == l_ltUseBuff1.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff1.puBuf[3u] ) && ( 0u == l_ltUseBuff1.puBuf[4u] ))
        {
            (void)printf("eFSS_CORELLTST_LoadTest 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_LoadTest 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_LoadTest 11 -- FAIL \n");
    }

    if( e_eFSS_CORELL_RES_NOTVALIDPAGE == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 1u) )
    {
        if( ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
             ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x10u == l_ltUseBuff2.puBuf[4u] ))
        {
            (void)printf("eFSS_CORELLTST_LoadTest 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_LoadTest 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_LoadTest 12 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[12u] = 0x02u;
    m_auStorArea1[13u] = 0x00u;
    m_auStorArea1[14u] = 0x00u;
    m_auStorArea1[15u] = 0x00u;
    m_auStorArea1[16u] = 0xA5u;
    m_auStorArea1[17u] = 0xA5u;
    m_auStorArea1[18u] = 0xA5u;
    m_auStorArea1[19u] = 0xA5u;
    m_auStorArea1[20u] = 0x95u;
    m_auStorArea1[21u] = 0x02u;
    m_auStorArea1[22u] = 0x00u;
    m_auStorArea1[23u] = 0x00u;

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[12u] = 0x02u;
    m_auStorArea2[13u] = 0x00u;
    m_auStorArea2[14u] = 0x00u;
    m_auStorArea2[15u] = 0x00u;
    m_auStorArea2[16u] = 0xA5u;
    m_auStorArea2[17u] = 0xA5u;
    m_auStorArea2[18u] = 0xA5u;
    m_auStorArea2[19u] = 0xA5u;
    m_auStorArea2[20u] = 0x55u;
    m_auStorArea2[21u] = 0x03u;
    m_auStorArea2[22u] = 0x00u;
    m_auStorArea2[23u] = 0x00u;

    /* Setup buffer */
    l_ltUseBuff1.puBuf[0u] = 0x01u;
    l_ltUseBuff1.puBuf[1u] = 0x02u;
    l_ltUseBuff1.puBuf[2u] = 0x03u;
    l_ltUseBuff1.puBuf[3u] = 0x04u;
    l_ltUseBuff1.puBuf[4u] = 0x05u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_CORELL_RES_NOTVALIDPAGE == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u) )
    {
        if( ( 0u == l_ltUseBuff1.puBuf[0u] ) && ( 0u == l_ltUseBuff1.puBuf[1u] ) && ( 0u == l_ltUseBuff1.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff1.puBuf[3u] ) && ( 0u == l_ltUseBuff1.puBuf[4u] ))
        {
            (void)printf("eFSS_CORELLTST_LoadTest 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_LoadTest 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_LoadTest 13 -- FAIL \n");
    }

    if( e_eFSS_CORELL_RES_NOTVALIDPAGE == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 1u) )
    {
        if( ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
             ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x10u == l_ltUseBuff2.puBuf[4u] ))
        {
            (void)printf("eFSS_CORELLTST_LoadTest 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_LoadTest 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_LoadTest 14 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[10u] = 0x01u;
    m_auStorArea1[11u] = 0x00u;
    m_auStorArea1[12u] = 0x02u;
    m_auStorArea1[13u] = 0x00u;
    m_auStorArea1[14u] = 0x00u;
    m_auStorArea1[15u] = 0x00u;
    m_auStorArea1[16u] = 0xA5u;
    m_auStorArea1[17u] = 0xA5u;
    m_auStorArea1[18u] = 0xA5u;
    m_auStorArea1[19u] = 0xA5u;
    m_auStorArea1[20u] = 0x96u;
    m_auStorArea1[21u] = 0x02u;
    m_auStorArea1[22u] = 0x00u;
    m_auStorArea1[23u] = 0x00u;

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[10u] = 0x01u;
    m_auStorArea2[11u] = 0x00u;
    m_auStorArea2[12u] = 0x02u;
    m_auStorArea2[13u] = 0x00u;
    m_auStorArea2[14u] = 0x00u;
    m_auStorArea2[15u] = 0x00u;
    m_auStorArea2[16u] = 0xA5u;
    m_auStorArea2[17u] = 0xA5u;
    m_auStorArea2[18u] = 0xA5u;
    m_auStorArea2[19u] = 0xA5u;
    m_auStorArea2[20u] = 0x36u;
    m_auStorArea2[21u] = 0x03u;
    m_auStorArea2[22u] = 0x00u;
    m_auStorArea2[23u] = 0x00u;

    /* Setup buffer */
    l_ltUseBuff1.puBuf[0u] = 0x01u;
    l_ltUseBuff1.puBuf[1u] = 0x02u;
    l_ltUseBuff1.puBuf[2u] = 0x03u;
    l_ltUseBuff1.puBuf[3u] = 0x04u;
    l_ltUseBuff1.puBuf[4u] = 0x05u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_CORELL_RES_NOTVALIDPAGE == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u) )
    {
        if( ( 0u == l_ltUseBuff1.puBuf[0u] ) && ( 0u == l_ltUseBuff1.puBuf[1u] ) && ( 0u == l_ltUseBuff1.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff1.puBuf[3u] ) && ( 0u == l_ltUseBuff1.puBuf[4u] ))
        {
            (void)printf("eFSS_CORELLTST_LoadTest 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_LoadTest 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_LoadTest 15 -- FAIL \n");
    }

    if( e_eFSS_CORELL_RES_NOTVALIDPAGE == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 1u) )
    {
        if( ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
             ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x10u == l_ltUseBuff2.puBuf[4u] ))
        {
            (void)printf("eFSS_CORELLTST_LoadTest 16 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_LoadTest 16 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_LoadTest 16 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[9u] = 0x01u;
    m_auStorArea1[10u] = 0x01u;
    m_auStorArea1[11u] = 0x00u;
    m_auStorArea1[12u] = 0x02u;
    m_auStorArea1[13u] = 0x00u;
    m_auStorArea1[14u] = 0x00u;
    m_auStorArea1[15u] = 0x00u;
    m_auStorArea1[16u] = 0xA5u;
    m_auStorArea1[17u] = 0xA5u;
    m_auStorArea1[18u] = 0xA5u;
    m_auStorArea1[19u] = 0xA5u;
    m_auStorArea1[20u] = 0x97u;
    m_auStorArea1[21u] = 0x02u;
    m_auStorArea1[22u] = 0x00u;
    m_auStorArea1[23u] = 0x00u;

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[9u] = 0x01u;
    m_auStorArea2[10u] = 0x01u;
    m_auStorArea2[11u] = 0x00u;
    m_auStorArea2[12u] = 0x02u;
    m_auStorArea2[13u] = 0x00u;
    m_auStorArea2[14u] = 0x00u;
    m_auStorArea2[15u] = 0x00u;
    m_auStorArea2[16u] = 0xA5u;
    m_auStorArea2[17u] = 0xA5u;
    m_auStorArea2[18u] = 0xA5u;
    m_auStorArea2[19u] = 0xA5u;
    m_auStorArea2[20u] = 0x27u;
    m_auStorArea2[21u] = 0x03u;
    m_auStorArea2[22u] = 0x00u;
    m_auStorArea2[23u] = 0x00u;

    /* Setup buffer */
    l_ltUseBuff1.puBuf[0u] = 0x01u;
    l_ltUseBuff1.puBuf[1u] = 0x02u;
    l_ltUseBuff1.puBuf[2u] = 0x03u;
    l_ltUseBuff1.puBuf[3u] = 0x04u;
    l_ltUseBuff1.puBuf[4u] = 0x05u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u) )
    {
        if( ( 0u == l_ltUseBuff1.puBuf[0u] ) && ( 0u == l_ltUseBuff1.puBuf[1u] ) && ( 0u == l_ltUseBuff1.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff1.puBuf[3u] ) && ( 0u == l_ltUseBuff1.puBuf[4u] ))
        {
            (void)printf("eFSS_CORELLTST_LoadTest 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_LoadTest 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_LoadTest 17 -- FAIL \n");
    }

    if( e_eFSS_CORELL_RES_NOTVALIDPAGE == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 1u) )
    {
        if( ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
             ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x10u == l_ltUseBuff2.puBuf[4u] ))
        {
            (void)printf("eFSS_CORELLTST_LoadTest 18 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_LoadTest 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_LoadTest 18 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[5u] = 0x01u;
    m_auStorArea1[6u] = 0x00u;
    m_auStorArea1[7u] = 0x00u;
    m_auStorArea1[8u] = 0x00u;
    m_auStorArea1[9u] = 0x01u;
    m_auStorArea1[10u] = 0x01u;
    m_auStorArea1[11u] = 0x00u;
    m_auStorArea1[12u] = 0x02u;
    m_auStorArea1[13u] = 0x00u;
    m_auStorArea1[14u] = 0x00u;
    m_auStorArea1[15u] = 0x00u;
    m_auStorArea1[16u] = 0xA5u;
    m_auStorArea1[17u] = 0xA5u;
    m_auStorArea1[18u] = 0xA5u;
    m_auStorArea1[19u] = 0xA5u;
    m_auStorArea1[20u] = 0x98u;
    m_auStorArea1[21u] = 0x02u;
    m_auStorArea1[22u] = 0x00u;
    m_auStorArea1[23u] = 0x00u;

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));

    m_auStorArea2[5u] = 0x01u;
    m_auStorArea2[6u] = 0x00u;
    m_auStorArea2[7u] = 0x00u;
    m_auStorArea2[8u] = 0x00u;
    m_auStorArea2[9u] = 0x01u;
    m_auStorArea2[10u] = 0x01u;
    m_auStorArea2[11u] = 0x00u;
    m_auStorArea2[12u] = 0x02u;
    m_auStorArea2[13u] = 0x00u;
    m_auStorArea2[14u] = 0x00u;
    m_auStorArea2[15u] = 0x00u;
    m_auStorArea2[16u] = 0xA5u;
    m_auStorArea2[17u] = 0xA5u;
    m_auStorArea2[18u] = 0xA5u;
    m_auStorArea2[19u] = 0xA5u;
    m_auStorArea2[20u] = 0xE8u;
    m_auStorArea2[21u] = 0x02u;
    m_auStorArea2[22u] = 0x00u;
    m_auStorArea2[23u] = 0x00u;

    /* Setup buffer */
    l_ltUseBuff1.puBuf[0u] = 0x01u;
    l_ltUseBuff1.puBuf[1u] = 0x02u;
    l_ltUseBuff1.puBuf[2u] = 0x03u;
    l_ltUseBuff1.puBuf[3u] = 0x04u;
    l_ltUseBuff1.puBuf[4u] = 0x05u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_CORELL_RES_NOTVALIDPAGE == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u) )
    {
        if( ( 0u == l_ltUseBuff1.puBuf[0u] ) && ( 0u == l_ltUseBuff1.puBuf[1u] ) && ( 0u == l_ltUseBuff1.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff1.puBuf[3u] ) && ( 0u == l_ltUseBuff1.puBuf[4u] ))
        {
            (void)printf("eFSS_CORELLTST_LoadTest 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_LoadTest 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_LoadTest 19 -- FAIL \n");
    }

    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 1u) )
    {
        if( ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
             ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x10u == l_ltUseBuff2.puBuf[4u] ))
        {
            (void)printf("eFSS_CORELLTST_LoadTest 20 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_LoadTest 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_LoadTest 20 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[5u] = 0x00u;
    m_auStorArea1[6u] = 0x00u;
    m_auStorArea1[7u] = 0x00u;
    m_auStorArea1[8u] = 0x00u;
    m_auStorArea1[9u] = 0x01u;
    m_auStorArea1[10u] = 0x00u;
    m_auStorArea1[11u] = 0x00u;
    m_auStorArea1[12u] = 0x02u;
    m_auStorArea1[13u] = 0x00u;
    m_auStorArea1[14u] = 0x00u;
    m_auStorArea1[15u] = 0x00u;
    m_auStorArea1[16u] = 0xA5u;
    m_auStorArea1[17u] = 0xA5u;
    m_auStorArea1[18u] = 0xA5u;
    m_auStorArea1[19u] = 0xA5u;
    m_auStorArea1[20u] = 0x96u;
    m_auStorArea1[21u] = 0x02u;
    m_auStorArea1[22u] = 0x00u;
    m_auStorArea1[23u] = 0x00u;

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));

    m_auStorArea2[5u] = 0x01u;
    m_auStorArea2[6u] = 0x00u;
    m_auStorArea2[7u] = 0x00u;
    m_auStorArea2[8u] = 0x00u;
    m_auStorArea2[9u] = 0x01u;
    m_auStorArea2[10u] = 0x02u;
    m_auStorArea2[11u] = 0x00u;
    m_auStorArea2[12u] = 0x02u;
    m_auStorArea2[13u] = 0x00u;
    m_auStorArea2[14u] = 0x00u;
    m_auStorArea2[15u] = 0x00u;
    m_auStorArea2[16u] = 0xA5u;
    m_auStorArea2[17u] = 0xA5u;
    m_auStorArea2[18u] = 0xA5u;
    m_auStorArea2[19u] = 0xA5u;
    m_auStorArea2[20u] = 0xE9u;
    m_auStorArea2[21u] = 0x02u;
    m_auStorArea2[22u] = 0x00u;
    m_auStorArea2[23u] = 0x00u;

    /* Setup buffer */
    l_ltUseBuff1.puBuf[0u] = 0x01u;
    l_ltUseBuff1.puBuf[1u] = 0x02u;
    l_ltUseBuff1.puBuf[2u] = 0x03u;
    l_ltUseBuff1.puBuf[3u] = 0x04u;
    l_ltUseBuff1.puBuf[4u] = 0x05u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_CORELL_RES_NEWVERSIONFOUND == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u) )
    {
        if( ( 0u == l_ltUseBuff1.puBuf[0u] ) && ( 0u == l_ltUseBuff1.puBuf[1u] ) && ( 0u == l_ltUseBuff1.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff1.puBuf[3u] ) && ( 0u == l_ltUseBuff1.puBuf[4u] ))
        {
            (void)printf("eFSS_CORELLTST_LoadTest 21 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_LoadTest 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_LoadTest 21 -- FAIL \n");
    }

    if( e_eFSS_CORELL_RES_NEWVERSIONFOUND == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 1u) )
    {
        if( ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
             ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x10u == l_ltUseBuff2.puBuf[4u] ))
        {
            (void)printf("eFSS_CORELLTST_LoadTest 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_LoadTest 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_LoadTest 22 -- FAIL \n");
    }

    /* Misra complaiant */
    (void)l_tCtxErase.eLastEr;
    (void)l_tCtxErase.uTimeUsed;
    (void)l_tCtxWrite.eLastEr;
    (void)l_tCtxWrite.uTimeUsed;
    (void)l_tCtxRead.eLastEr;
    (void)l_tCtxRead.uTimeUsed;
    (void)l_tCtxCrc32.eLastEr;
    (void)l_tCtxCrc32.uTimeUsed;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "CERT-INT31-C_c"
#endif

static void eFSS_CORELLTST_FlushTest(void)
{
    /* Local variable */
    t_eFSS_CORELL_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_uStorType;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_CORELL_StorBuf l_ltUseBuff1;
    t_eFSS_CORELL_StorBuf l_ltUseBuff2;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_CORELLTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_CORELLTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_CORELLTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_CORELLTST_CrcTst1Adapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 2u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;
    l_uStorType = 1u;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_CORELLTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_CORELLTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_CORELLTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_CORELLTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;

    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_FlushTest 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_FlushTest 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_GetBuff(&l_tCtx, &l_ltUseBuff1, &l_ltUseBuff2) )
    {
        (void)printf("eFSS_CORELLTST_FlushTest 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_FlushTest 2  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0xFF, sizeof(m_auStorArea2));

    /* Setup buffer */
    l_ltUseBuff1.puBuf[0u] = 0x01u;
    l_ltUseBuff1.puBuf[1u] = 0x02u;
    l_ltUseBuff1.puBuf[2u] = 0x03u;
    l_ltUseBuff1.puBuf[3u] = 0x04u;
    l_ltUseBuff1.puBuf[4u] = 0x05u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_FlushBuffInPage(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u) )
    {
        if( ( 0x01u== m_auStorArea1[0u] )  && ( 0x02u== m_auStorArea1[1u] )  && ( 0x03u== m_auStorArea1[2u] ) &&
            ( 0x04u== m_auStorArea1[3u] )  && ( 0x05u== m_auStorArea1[4u] )  &&
            ( 0x00u== m_auStorArea1[5u] )  && ( 0x00u== m_auStorArea1[6u] )  && ( 0x00u== m_auStorArea1[7u] ) &&
            ( 0x00u== m_auStorArea1[8u] )  && ( 0x01u== m_auStorArea1[9u] )  && ( 0x01u== m_auStorArea1[10u] ) &&
            ( 0x00u== m_auStorArea1[11u] ) && ( 0x02u== m_auStorArea1[12u] ) && ( 0x00u== m_auStorArea1[13u] ) &&
            ( 0x00u== m_auStorArea1[14u] ) && ( 0x00u== m_auStorArea1[15u] ) && ( 0xA5u== m_auStorArea1[16u] ) &&
            ( 0xA5u== m_auStorArea1[17u] ) && ( 0xA5u== m_auStorArea1[18u] ) && ( 0xA5u== m_auStorArea1[19u] ) &&
            ( 0xA6u== m_auStorArea1[20u] ) && ( 0x02u== m_auStorArea1[21u] ) && ( 0x00u== m_auStorArea1[22u] ) &&
            ( 0x00u== m_auStorArea1[23u] ) &&
            ( 0xFFu== m_auStorArea2[0u] )  && ( 0xFFu== m_auStorArea2[1u] )  && ( 0xFFu== m_auStorArea2[2u] ) &&
            ( 0xFFu== m_auStorArea2[3u] )  && ( 0xFFu== m_auStorArea2[4u] )  &&
            ( 0xFFu== m_auStorArea2[5u] )  && ( 0xFFu== m_auStorArea2[6u] )  && ( 0xFFu== m_auStorArea2[7u] ) &&
            ( 0xFFu== m_auStorArea2[8u] )  && ( 0xFFu== m_auStorArea2[9u] )  && ( 0xFFu== m_auStorArea2[10u] ) &&
            ( 0xFFu== m_auStorArea2[11u] ) && ( 0xFFu== m_auStorArea2[12u] ) && ( 0xFFu== m_auStorArea2[13u] ) &&
            ( 0xFFu== m_auStorArea2[14u] ) && ( 0xFFu== m_auStorArea2[15u] ) && ( 0xFFu== m_auStorArea2[16u] ) &&
            ( 0xFFu== m_auStorArea2[17u] ) && ( 0xFFu== m_auStorArea2[18u] ) && ( 0xFFu== m_auStorArea2[19u] ) &&
            ( 0xFFu== m_auStorArea2[20u] ) && ( 0xFFu== m_auStorArea2[21u] ) && ( 0xFFu== m_auStorArea2[22u] ) &&
            ( 0xFFu== m_auStorArea2[23u] ) )
        {
            if( ( 0x01u== l_ltUseBuff1.puBuf[0u] )  && ( 0x02u== l_ltUseBuff1.puBuf[1u] )  && ( 0x03u== l_ltUseBuff1.puBuf[2u] ) &&
                ( 0x04u== l_ltUseBuff1.puBuf[3u] )  && ( 0x05u== l_ltUseBuff1.puBuf[4u] )  &&
                ( 0x01u== l_ltUseBuff2.puBuf[0u] )  && ( 0x02u== l_ltUseBuff2.puBuf[1u] )  && ( 0x03u== l_ltUseBuff2.puBuf[2u] ) &&
                ( 0x04u== l_ltUseBuff2.puBuf[3u] )  && ( 0x05u== l_ltUseBuff2.puBuf[4u] ) )
            {
                (void)printf("eFSS_CORELLTST_FlushTest 3  -- OK \n");
            }
            else
            {
                (void)printf("eFSS_CORELLTST_FlushTest 3  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_CORELLTST_FlushTest 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_FlushTest 3  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0xFF, sizeof(m_auStorArea2));

    /* Setup buffer */
    l_ltUseBuff1.puBuf[0u] = 0x01u;
    l_ltUseBuff1.puBuf[1u] = 0x02u;
    l_ltUseBuff1.puBuf[2u] = 0x03u;
    l_ltUseBuff1.puBuf[3u] = 0x04u;
    l_ltUseBuff1.puBuf[4u] = 0x05u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_FlushBuffInPage(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 1u) )
    {
        if( ( 0x01u== m_auStorArea2[0u] )  && ( 0x02u== m_auStorArea2[1u] )  && ( 0x03u== m_auStorArea2[2u] ) &&
            ( 0x04u== m_auStorArea2[3u] )  && ( 0x05u== m_auStorArea2[4u] )  &&
            ( 0x01u== m_auStorArea2[5u] )  && ( 0x00u== m_auStorArea2[6u] )  && ( 0x00u== m_auStorArea2[7u] ) &&
            ( 0x00u== m_auStorArea2[8u] )  && ( 0x01u== m_auStorArea2[9u] )  && ( 0x01u== m_auStorArea2[10u] ) &&
            ( 0x00u== m_auStorArea2[11u] ) && ( 0x02u== m_auStorArea2[12u] ) && ( 0x00u== m_auStorArea2[13u] ) &&
            ( 0x00u== m_auStorArea2[14u] ) && ( 0x00u== m_auStorArea2[15u] ) && ( 0xA5u== m_auStorArea2[16u] ) &&
            ( 0xA5u== m_auStorArea2[17u] ) && ( 0xA5u== m_auStorArea2[18u] ) && ( 0xA5u== m_auStorArea2[19u] ) &&
            ( 0xA7u== m_auStorArea2[20u] ) && ( 0x02u== m_auStorArea2[21u] ) && ( 0x00u== m_auStorArea2[22u] ) &&
            ( 0x00u== m_auStorArea2[23u] ) &&
            ( 0x00u== m_auStorArea1[0u] )  && ( 0x00u== m_auStorArea1[1u] )  && ( 0x00u== m_auStorArea1[2u] ) &&
            ( 0x00u== m_auStorArea1[3u] )  && ( 0x00u== m_auStorArea1[4u] )  &&
            ( 0x00u== m_auStorArea1[5u] )  && ( 0x00u== m_auStorArea1[6u] )  && ( 0x00u== m_auStorArea1[7u] ) &&
            ( 0x00u== m_auStorArea1[8u] )  && ( 0x00u== m_auStorArea1[9u] )  && ( 0x00u== m_auStorArea1[10u] ) &&
            ( 0x00u== m_auStorArea1[11u] ) && ( 0x00u== m_auStorArea1[12u] ) && ( 0x00u== m_auStorArea1[13u] ) &&
            ( 0x00u== m_auStorArea1[14u] ) && ( 0x00u== m_auStorArea1[15u] ) && ( 0x00u== m_auStorArea1[16u] ) &&
            ( 0x00u== m_auStorArea1[17u] ) && ( 0x00u== m_auStorArea1[18u] ) && ( 0x00u== m_auStorArea1[19u] ) &&
            ( 0x00u== m_auStorArea1[20u] ) && ( 0x00u== m_auStorArea1[21u] ) && ( 0x00u== m_auStorArea1[22u] ) &&
            ( 0x00u== m_auStorArea1[23u] ) )
        {
            if( ( 0x01u== l_ltUseBuff1.puBuf[0u] )  && ( 0x02u== l_ltUseBuff1.puBuf[1u] )  && ( 0x03u== l_ltUseBuff1.puBuf[2u] ) &&
                ( 0x04u== l_ltUseBuff1.puBuf[3u] )  && ( 0x05u== l_ltUseBuff1.puBuf[4u] )  &&
                ( 0x01u== l_ltUseBuff2.puBuf[0u] )  && ( 0x02u== l_ltUseBuff2.puBuf[1u] )  && ( 0x03u== l_ltUseBuff2.puBuf[2u] ) &&
                ( 0x04u== l_ltUseBuff2.puBuf[3u] )  && ( 0x05u== l_ltUseBuff2.puBuf[4u] ) )
            {
                (void)printf("eFSS_CORELLTST_FlushTest 4  -- OK \n");
            }
            else
            {
                (void)printf("eFSS_CORELLTST_FlushTest 4  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_CORELLTST_FlushTest 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_FlushTest 4  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0xFF, sizeof(m_auStorArea2));

    /* Setup buffer */
    l_ltUseBuff1.puBuf[0u] = 0x11u;
    l_ltUseBuff1.puBuf[1u] = 0x12u;
    l_ltUseBuff1.puBuf[2u] = 0x13u;
    l_ltUseBuff1.puBuf[3u] = 0x14u;
    l_ltUseBuff1.puBuf[4u] = 0x15u;

    l_ltUseBuff2.puBuf[0u] = 0x01u;
    l_ltUseBuff2.puBuf[1u] = 0x02u;
    l_ltUseBuff2.puBuf[2u] = 0x03u;
    l_ltUseBuff2.puBuf[3u] = 0x04u;
    l_ltUseBuff2.puBuf[4u] = 0x05u;

    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_FlushBuffInPage(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 0u) )
    {
        if( ( 0x01u== m_auStorArea1[0u] )  && ( 0x02u== m_auStorArea1[1u] )  && ( 0x03u== m_auStorArea1[2u] ) &&
            ( 0x04u== m_auStorArea1[3u] )  && ( 0x05u== m_auStorArea1[4u] )  &&
            ( 0x00u== m_auStorArea1[5u] )  && ( 0x00u== m_auStorArea1[6u] )  && ( 0x00u== m_auStorArea1[7u] ) &&
            ( 0x00u== m_auStorArea1[8u] )  && ( 0x01u== m_auStorArea1[9u] )  && ( 0x01u== m_auStorArea1[10u] ) &&
            ( 0x00u== m_auStorArea1[11u] ) && ( 0x02u== m_auStorArea1[12u] ) && ( 0x00u== m_auStorArea1[13u] ) &&
            ( 0x00u== m_auStorArea1[14u] ) && ( 0x00u== m_auStorArea1[15u] ) && ( 0xA5u== m_auStorArea1[16u] ) &&
            ( 0xA5u== m_auStorArea1[17u] ) && ( 0xA5u== m_auStorArea1[18u] ) && ( 0xA5u== m_auStorArea1[19u] ) &&
            ( 0xA6u== m_auStorArea1[20u] ) && ( 0x02u== m_auStorArea1[21u] ) && ( 0x00u== m_auStorArea1[22u] ) &&
            ( 0x00u== m_auStorArea1[23u] ) &&
            ( 0xFFu== m_auStorArea2[0u] )  && ( 0xFFu== m_auStorArea2[1u] )  && ( 0xFFu== m_auStorArea2[2u] ) &&
            ( 0xFFu== m_auStorArea2[3u] )  && ( 0xFFu== m_auStorArea2[4u] )  &&
            ( 0xFFu== m_auStorArea2[5u] )  && ( 0xFFu== m_auStorArea2[6u] )  && ( 0xFFu== m_auStorArea2[7u] ) &&
            ( 0xFFu== m_auStorArea2[8u] )  && ( 0xFFu== m_auStorArea2[9u] )  && ( 0xFFu== m_auStorArea2[10u] ) &&
            ( 0xFFu== m_auStorArea2[11u] ) && ( 0xFFu== m_auStorArea2[12u] ) && ( 0xFFu== m_auStorArea2[13u] ) &&
            ( 0xFFu== m_auStorArea2[14u] ) && ( 0xFFu== m_auStorArea2[15u] ) && ( 0xFFu== m_auStorArea2[16u] ) &&
            ( 0xFFu== m_auStorArea2[17u] ) && ( 0xFFu== m_auStorArea2[18u] ) && ( 0xFFu== m_auStorArea2[19u] ) &&
            ( 0xFFu== m_auStorArea2[20u] ) && ( 0xFFu== m_auStorArea2[21u] ) && ( 0xFFu== m_auStorArea2[22u] ) &&
            ( 0xFFu== m_auStorArea2[23u] ) )
        {
            if( ( 0x01u== l_ltUseBuff1.puBuf[0u] )  && ( 0x02u== l_ltUseBuff1.puBuf[1u] )  && ( 0x03u== l_ltUseBuff1.puBuf[2u] ) &&
                ( 0x04u== l_ltUseBuff1.puBuf[3u] )  && ( 0x05u== l_ltUseBuff1.puBuf[4u] )  &&
                ( 0x01u== l_ltUseBuff2.puBuf[0u] )  && ( 0x02u== l_ltUseBuff2.puBuf[1u] )  && ( 0x03u== l_ltUseBuff2.puBuf[2u] ) &&
                ( 0x04u== l_ltUseBuff2.puBuf[3u] )  && ( 0x05u== l_ltUseBuff2.puBuf[4u] ) )
            {
                (void)printf("eFSS_CORELLTST_FlushTest 5  -- OK \n");
            }
            else
            {
                (void)printf("eFSS_CORELLTST_FlushTest 5  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_CORELLTST_FlushTest 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_FlushTest 5  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0xFF, sizeof(m_auStorArea2));

    /* Setup buffer */
    l_ltUseBuff1.puBuf[0u] = 0x11u;
    l_ltUseBuff1.puBuf[1u] = 0x12u;
    l_ltUseBuff1.puBuf[2u] = 0x13u;
    l_ltUseBuff1.puBuf[3u] = 0x14u;
    l_ltUseBuff1.puBuf[4u] = 0x15u;

    l_ltUseBuff2.puBuf[0u] = 0x01u;
    l_ltUseBuff2.puBuf[1u] = 0x02u;
    l_ltUseBuff2.puBuf[2u] = 0x03u;
    l_ltUseBuff2.puBuf[3u] = 0x04u;
    l_ltUseBuff2.puBuf[4u] = 0x05u;

    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_FlushBuffInPage(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 1u) )
    {
        if( ( 0x01u== m_auStorArea2[0u] )  && ( 0x02u== m_auStorArea2[1u] )  && ( 0x03u== m_auStorArea2[2u] ) &&
            ( 0x04u== m_auStorArea2[3u] )  && ( 0x05u== m_auStorArea2[4u] )  &&
            ( 0x01u== m_auStorArea2[5u] )  && ( 0x00u== m_auStorArea2[6u] )  && ( 0x00u== m_auStorArea2[7u] ) &&
            ( 0x00u== m_auStorArea2[8u] )  && ( 0x01u== m_auStorArea2[9u] )  && ( 0x01u== m_auStorArea2[10u] ) &&
            ( 0x00u== m_auStorArea2[11u] ) && ( 0x02u== m_auStorArea2[12u] ) && ( 0x00u== m_auStorArea2[13u] ) &&
            ( 0x00u== m_auStorArea2[14u] ) && ( 0x00u== m_auStorArea2[15u] ) && ( 0xA5u== m_auStorArea2[16u] ) &&
            ( 0xA5u== m_auStorArea2[17u] ) && ( 0xA5u== m_auStorArea2[18u] ) && ( 0xA5u== m_auStorArea2[19u] ) &&
            ( 0xA7u== m_auStorArea2[20u] ) && ( 0x02u== m_auStorArea2[21u] ) && ( 0x00u== m_auStorArea2[22u] ) &&
            ( 0x00u== m_auStorArea2[23u] ) &&
            ( 0x00u== m_auStorArea1[0u] )  && ( 0x00u== m_auStorArea1[1u] )  && ( 0x00u== m_auStorArea1[2u] ) &&
            ( 0x00u== m_auStorArea1[3u] )  && ( 0x00u== m_auStorArea1[4u] )  &&
            ( 0x00u== m_auStorArea1[5u] )  && ( 0x00u== m_auStorArea1[6u] )  && ( 0x00u== m_auStorArea1[7u] ) &&
            ( 0x00u== m_auStorArea1[8u] )  && ( 0x00u== m_auStorArea1[9u] )  && ( 0x00u== m_auStorArea1[10u] ) &&
            ( 0x00u== m_auStorArea1[11u] ) && ( 0x00u== m_auStorArea1[12u] ) && ( 0x00u== m_auStorArea1[13u] ) &&
            ( 0x00u== m_auStorArea1[14u] ) && ( 0x00u== m_auStorArea1[15u] ) && ( 0x00u== m_auStorArea1[16u] ) &&
            ( 0x00u== m_auStorArea1[17u] ) && ( 0x00u== m_auStorArea1[18u] ) && ( 0x00u== m_auStorArea1[19u] ) &&
            ( 0x00u== m_auStorArea1[20u] ) && ( 0x00u== m_auStorArea1[21u] ) && ( 0x00u== m_auStorArea1[22u] ) &&
            ( 0x00u== m_auStorArea1[23u] ) )
        {
            if( ( 0x01u== l_ltUseBuff1.puBuf[0u] )  && ( 0x02u== l_ltUseBuff1.puBuf[1u] )  && ( 0x03u== l_ltUseBuff1.puBuf[2u] ) &&
                ( 0x04u== l_ltUseBuff1.puBuf[3u] )  && ( 0x05u== l_ltUseBuff1.puBuf[4u] )  &&
                ( 0x01u== l_ltUseBuff2.puBuf[0u] )  && ( 0x02u== l_ltUseBuff2.puBuf[1u] )  && ( 0x03u== l_ltUseBuff2.puBuf[2u] ) &&
                ( 0x04u== l_ltUseBuff2.puBuf[3u] )  && ( 0x05u== l_ltUseBuff2.puBuf[4u] ) )
            {
                (void)printf("eFSS_CORELLTST_FlushTest 6  -- OK \n");
            }
            else
            {
                (void)printf("eFSS_CORELLTST_FlushTest 6  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_CORELLTST_FlushTest 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_FlushTest 6  -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_CORELLTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_CORELLTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_CORELLTST_ReadTst2Adapt;
    l_tCtxCb.fCrc32 = &eFSS_CORELLTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;

    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_FlushTest 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_FlushTest 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_GetBuff(&l_tCtx, &l_ltUseBuff1, &l_ltUseBuff2) )
    {
        (void)printf("eFSS_CORELLTST_FlushTest 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_FlushTest 8  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0xFF, sizeof(m_auStorArea2));

    /* Setup buffer */
    l_ltUseBuff1.puBuf[0u] = 0x01u;
    l_ltUseBuff1.puBuf[1u] = 0x02u;
    l_ltUseBuff1.puBuf[2u] = 0x03u;
    l_ltUseBuff1.puBuf[3u] = 0x04u;
    l_ltUseBuff1.puBuf[4u] = 0x05u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_CORELL_RES_WRITENOMATCHREAD == eFSS_CORELL_FlushBuffInPage(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u) )
    {
        (void)printf("eFSS_CORELLTST_FlushTest 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_FlushTest 9  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0xFF, sizeof(m_auStorArea2));

    /* Setup buffer */
    l_ltUseBuff1.puBuf[0u] = 0x01u;
    l_ltUseBuff1.puBuf[1u] = 0x02u;
    l_ltUseBuff1.puBuf[2u] = 0x03u;
    l_ltUseBuff1.puBuf[3u] = 0x04u;
    l_ltUseBuff1.puBuf[4u] = 0x05u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_CORELL_RES_WRITENOMATCHREAD == eFSS_CORELL_FlushBuffInPage(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 1u) )
    {
        (void)printf("eFSS_CORELLTST_FlushTest 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_FlushTest 10 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0xFF, sizeof(m_auStorArea2));

    /* Setup buffer */
    l_ltUseBuff1.puBuf[0u] = 0x01u;
    l_ltUseBuff1.puBuf[1u] = 0x02u;
    l_ltUseBuff1.puBuf[2u] = 0x03u;
    l_ltUseBuff1.puBuf[3u] = 0x04u;
    l_ltUseBuff1.puBuf[4u] = 0x05u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_CORELL_RES_WRITENOMATCHREAD == eFSS_CORELL_FlushBuffInPage(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 0u) )
    {
        (void)printf("eFSS_CORELLTST_FlushTest 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_FlushTest 11 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0xFF, sizeof(m_auStorArea2));

    /* Setup buffer */
    l_ltUseBuff1.puBuf[0u] = 0x01u;
    l_ltUseBuff1.puBuf[1u] = 0x02u;
    l_ltUseBuff1.puBuf[2u] = 0x03u;
    l_ltUseBuff1.puBuf[3u] = 0x04u;
    l_ltUseBuff1.puBuf[4u] = 0x05u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_CORELL_RES_WRITENOMATCHREAD == eFSS_CORELL_FlushBuffInPage(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 1u) )
    {
        (void)printf("eFSS_CORELLTST_FlushTest 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_FlushTest 12 -- FAIL \n");
    }

    /* Misra complaiant */
    (void)l_tCtxErase.eLastEr;
    (void)l_tCtxErase.uTimeUsed;
    (void)l_tCtxWrite.eLastEr;
    (void)l_tCtxWrite.uTimeUsed;
    (void)l_tCtxRead.eLastEr;
    (void)l_tCtxRead.uTimeUsed;
    (void)l_tCtxCrc32.eLastEr;
    (void)l_tCtxCrc32.uTimeUsed;
    (void)l_ltUseBuff1.puBuf;
    (void)l_ltUseBuff2.puBuf;
}

static void eFSS_CORELLTST_GenTest(void)
{
    /* Local variable */
    t_eFSS_CORELL_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_uStorType;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_CORELL_StorBuf l_ltUseBuff1;
    t_eFSS_CORELL_StorBuf l_ltUseBuff2;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_CORELLTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_CORELLTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_CORELLTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_CORELLTST_CrcTst1Adapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 2u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;
    l_uStorType = 1u;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_CORELLTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_CORELLTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_CORELLTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_CORELLTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_CORELL_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_CORELL_RES_OK;

    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_CORELLTST_GenTest 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_GenTest 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_GetBuff(&l_tCtx, &l_ltUseBuff1, &l_ltUseBuff2) )
    {
        (void)printf("eFSS_CORELLTST_GenTest 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_GenTest 2  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0xFF, sizeof(m_auStorArea2));

    /* Setup buffer */
    l_ltUseBuff1.puBuf[0u] = 0x01u;
    l_ltUseBuff1.puBuf[1u] = 0x02u;
    l_ltUseBuff1.puBuf[2u] = 0x03u;
    l_ltUseBuff1.puBuf[3u] = 0x04u;
    l_ltUseBuff1.puBuf[4u] = 0x05u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_FlushBuffInPage(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 0u) )
    {
        (void)printf("eFSS_CORELLTST_GenTest 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_GenTest 3  -- FAIL \n");
    }

    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 0u) )
    {
        if( ( 0x01u== l_ltUseBuff2.puBuf[0u] ) && ( 0x02u== l_ltUseBuff2.puBuf[1u] ) && ( 0x03u== l_ltUseBuff2.puBuf[2u] ) &&
             ( 0x04u== l_ltUseBuff2.puBuf[3u] ) && ( 0x05u== l_ltUseBuff2.puBuf[4u] ))
        {
            (void)printf("eFSS_CORELLTST_GenTest 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_GenTest 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_GenTest 4  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0xFF, sizeof(m_auStorArea2));

    /* Setup buffer */
    l_ltUseBuff1.puBuf[0u] = 0x01u;
    l_ltUseBuff1.puBuf[1u] = 0x02u;
    l_ltUseBuff1.puBuf[2u] = 0x03u;
    l_ltUseBuff1.puBuf[3u] = 0x04u;
    l_ltUseBuff1.puBuf[4u] = 0x05u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_FlushBuffInPage(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_2, 1u) )
    {
        (void)printf("eFSS_CORELLTST_GenTest 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_CORELLTST_GenTest 5  -- FAIL \n");
    }

    if( e_eFSS_CORELL_RES_OK == eFSS_CORELL_LoadPageInBuff(&l_tCtx, e_eFSS_CORELL_BUFFTYPE_1, 1u) )
    {
        if( ( 0x11u== l_ltUseBuff1.puBuf[0u] ) && ( 0x12u== l_ltUseBuff1.puBuf[1u] ) && ( 0x13u== l_ltUseBuff1.puBuf[2u] ) &&
            ( 0x14u== l_ltUseBuff1.puBuf[3u] ) && ( 0x15u== l_ltUseBuff1.puBuf[4u] ))
        {
            (void)printf("eFSS_CORELLTST_GenTest 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_CORELLTST_GenTest 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_CORELLTST_GenTest 6  -- FAIL \n");
    }

    /* Misra complaiant */
    (void)l_tCtxErase.eLastEr;
    (void)l_tCtxErase.uTimeUsed;
    (void)l_tCtxWrite.eLastEr;
    (void)l_tCtxWrite.uTimeUsed;
    (void)l_tCtxRead.eLastEr;
    (void)l_tCtxRead.uTimeUsed;
    (void)l_tCtxCrc32.eLastEr;
    (void)l_tCtxCrc32.uTimeUsed;
    (void)l_ltUseBuff2.puBuf;
}