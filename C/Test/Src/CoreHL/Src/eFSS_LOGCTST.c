/**
 * @file       eFSS_LOGCTST.c
 *
 * @brief      LOG Core module test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_LOGCTST.h"
#include "eFSS_LOGC.h"
#include <stdio.h>



/***********************************************************************************************************************
 *   PRIVATE TEST STRUCT DECLARATION
 **********************************************************************************************************************/
struct t_eFSS_TYPE_EraseCtxUser
{
    e_eFSS_LOGC_RES eLastEr;
    uint32_t uTimeUsed;
};

struct t_eFSS_TYPE_WriteCtxUser
{
    e_eFSS_LOGC_RES eLastEr;
    uint32_t uTimeUsed;
};

struct t_eFSS_TYPE_ReadCtxUser
{
    e_eFSS_LOGC_RES eLastEr;
    uint32_t uTimeUsed;
};

struct t_eFSS_TYPE_CrcCtxUser
{
    e_eFSS_LOGC_RES eLastEr;
    uint32_t uTimeUsed;
};



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_LOGCTST_EraseAdapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase);

static bool_t eFSS_LOGCTST_EraseErrAdapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase);

static bool_t eFSS_LOGCTST_EraseTst1Adapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase);


static bool_t eFSS_LOGCTST_WriteAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                        const uint32_t p_uPageToWrite, const uint8_t* p_puDataToWrite,
                                        const uint32_t p_uDataToWriteL );

static bool_t eFSS_LOGCTST_WriteErrAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                           const uint32_t p_uPageToWrite, const uint8_t* p_puDataToWrite,
                                           const uint32_t p_uDataToWriteL );

static bool_t eFSS_LOGCTST_WriteErrSecAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                              const uint32_t p_uPageToWrite, const uint8_t* p_puDataToWrite,
                                              const uint32_t p_uDataToWriteL );

static bool_t eFSS_LOGCTST_WriteTst1Adapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                        const uint32_t p_uPageToWrite, const uint8_t* p_puDataToWrite,
                                        const uint32_t p_uDataToWriteL );

static bool_t eFSS_LOGCTST_ReadAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                       const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                       const uint32_t p_uReadBufferL );

static bool_t eFSS_LOGCTST_ReadErrAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                          const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                          const uint32_t p_uReadBufferL );

static bool_t eFSS_LOGCTST_ReadErrSecAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                             const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                             const uint32_t p_uReadBufferL );

static bool_t eFSS_LOGCTST_ReadTst1Adapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                       const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                       const uint32_t p_uReadBufferL );

static bool_t eFSS_LOGCTST_ReadTst2Adapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                       const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                       const uint32_t p_uReadBufferL );

static bool_t eFSS_LOGCTST_CrcAdapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
                                      const uint8_t* p_puData, const uint32_t p_uDataL,
                                      uint32_t* const p_puCrc32Val );

static bool_t eFSS_LOGCTST_CrcErrAdapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
                                         const uint8_t* p_puData, const uint32_t p_uDataL,
                                         uint32_t* const p_puCrc32Val );

static bool_t eFSS_LOGCTST_CrcTst1Adapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
                                          const uint8_t* p_puData, const uint32_t p_uDataL,
                                          uint32_t* const p_puCrc32Val );

/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void eFSS_LOGCTST_BadPointer(void);
static void eFSS_LOGCTST_BadInit(void);
static void eFSS_LOGCTST_BadParamEntr(void);
static void eFSS_LOGCTST_CorruptedCtx(void);
static void eFSS_LOGCTST_Basic(void);
static void eFSS_LOGCTST_BadClBckNRetry(void);
static void eFSS_LOGCTST_ReadCacheTest(void);
static void eFSS_LOGCTST_WriteCacheTest(void);

#if 0
static void eFSS_LOGCTST_LoadTest(void);
static void eFSS_LOGCTST_FlushTest(void);
static void eFSS_LOGCTST_Compare(void);
static void eFSS_LOGCTST_LoadBkupTest(void);
static void eFSS_LOGCTST_FlushBkupTest(void);
static void eFSS_LOGCTST_GenTest(void);
#endif


/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eFSS_LOGCTST_ExeTest(void)
{
	(void)printf("\n\nLOG CORE TEST START \n\n");

    // eFSS_LOGCTST_BadPointer();
    // eFSS_LOGCTST_BadInit();
    // eFSS_LOGCTST_BadParamEntr();
    // eFSS_LOGCTST_CorruptedCtx();
    // eFSS_LOGCTST_Basic();
    // eFSS_LOGCTST_BadClBckNRetry();
    // eFSS_LOGCTST_ReadCacheTest();
    eFSS_LOGCTST_WriteCacheTest();

#if 0

    eFSS_LOGCTST_LoadTest();
    eFSS_LOGCTST_FlushTest();
    eFSS_LOGCTST_Compare();
    eFSS_LOGCTST_LoadBkupTest();
    eFSS_LOGCTST_FlushBkupTest();
    eFSS_LOGCTST_GenTest();
#endif

    (void)printf("\n\nLOG CORE TEST END \n\n");
}

/***********************************************************************************************************************
 *   PRIVATE MODULES VARIABLE DECLARATION
 **********************************************************************************************************************/
static bool_t  m_bIsErased[14u] = false;
static uint8_t m_auStorArea[14][32u];



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-8.13"
    /* Suppressed for code clarity in test execution*/
#endif


static bool_t eFSS_LOGCTST_EraseAdapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase)
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
        p_ptCtx->eLastEr = e_eFSS_LOGC_RES_OK;
    }

    return l_bRes;
}

static bool_t eFSS_LOGCTST_EraseErrAdapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase)
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
        p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPOINTER;
    }

    return l_bRes;
}

static bool_t eFSS_LOGCTST_EraseTst1Adapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase)
{
    bool_t l_bRes;

    if( NULL == p_ptCtx )
    {
        l_bRes = false;
    }
    else
    {
        if( p_uPageToErase >= 14u )
        {
            l_bRes = false;
            p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPARAM;
        }
        else
        {
            p_ptCtx->uTimeUsed++;
            (void)memset(m_auStorArea[p_uPageToErase], 0, sizeof(m_auStorArea[p_uPageToErase]));
            l_bRes = true;
            m_bIsErased[p_uPageToErase] = true;

        }
    }

    return l_bRes;
}

static bool_t eFSS_LOGCTST_WriteAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPOINTER;
        }
        else
        {
            l_bRes = true;
            p_ptCtx->eLastEr = e_eFSS_LOGC_RES_OK;
        }
    }

    return l_bRes;
}

static bool_t eFSS_LOGCTST_WriteErrAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPOINTER;
        }
        else
        {
            l_bRes = false;
            p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPOINTER;
        }
    }

    return l_bRes;
}

static bool_t eFSS_LOGCTST_WriteErrSecAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPOINTER;
        }
        else
        {
            if( p_uPageToWrite >= 14u )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPARAM;
            }
            else
            {
                if( 32u != p_uDataToWriteL )
                {
                    l_bRes = false;
                    p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPARAM;
                }
                else
                {
                    if( 0u == p_ptCtx->uTimeUsed )
                    {
                        if( false == m_bIsErased[p_uPageToWrite] )
                        {
                            l_bRes = false;
                            p_ptCtx->eLastEr = e_eFSS_LOGC_RES_CORRUPTCTX;
                        }
                        else
                        {
                            l_bRes = true;
                            p_ptCtx->eLastEr = e_eFSS_LOGC_RES_OK;
                            m_bIsErased[p_uPageToWrite] = false;
                            (void)memcpy(m_auStorArea[p_uPageToWrite], p_puDataToWrite, p_uDataToWriteL);
                        }

                        p_ptCtx->uTimeUsed++;

                    }
                    else
                    {
                        l_bRes = false;
                        p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPOINTER;
                    }
                }
            }
        }
    }

    return l_bRes;
}

static bool_t eFSS_LOGCTST_WriteTst1Adapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPOINTER;
        }
        else
        {
            if( p_uPageToWrite >= 14u )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPARAM;
            }
            else
            {
                if( 32u != p_uDataToWriteL )
                {
                    l_bRes = false;
                    p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPARAM;
                }
                else
                {
                    p_ptCtx->uTimeUsed++;

                    if( false == m_bIsErased[p_uPageToWrite] )
                    {
                        l_bRes = false;
                        p_ptCtx->eLastEr = e_eFSS_LOGC_RES_CORRUPTCTX;
                    }
                    else
                    {
                        l_bRes = true;
                        p_ptCtx->eLastEr = e_eFSS_LOGC_RES_OK;
                        m_bIsErased[p_uPageToWrite] = false;
                        (void)memcpy(m_auStorArea[p_uPageToWrite], p_puDataToWrite, p_uDataToWriteL);
                    }
                }
            }
        }
    }

    return l_bRes;
}

static bool_t eFSS_LOGCTST_ReadAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPOINTER;
        }
        else
        {
            l_bRes = true;
            p_ptCtx->eLastEr = e_eFSS_LOGC_RES_OK;
        }
    }

    return l_bRes;
}

static bool_t eFSS_LOGCTST_ReadErrAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPOINTER;
        }
        else
        {
            l_bRes = false;
            p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPOINTER;
        }
    }

    return l_bRes;
}


static bool_t eFSS_LOGCTST_ReadErrSecAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPOINTER;
        }
        else
        {
            if( p_uPageToRead >= 14u )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPARAM;
            }
            else
            {
                if( 32u != p_uReadBufferL )
                {
                    l_bRes = false;
                    p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPARAM;
                }
                else
                {
                    if( 0u == p_ptCtx->uTimeUsed )
                    {
                        l_bRes = true;
                        p_ptCtx->eLastEr = e_eFSS_LOGC_RES_OK;
                        (void)memcpy(p_puReadBuffer, m_auStorArea[p_uPageToRead], p_uReadBufferL);

                        p_ptCtx->uTimeUsed++;
                    }
                    else
                    {
                        l_bRes = false;
                        p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPOINTER;
                    }
                }
            }
        }
    }

    return l_bRes;
}

static bool_t eFSS_LOGCTST_ReadTst1Adapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPOINTER;
        }
        else
        {
            if( p_uPageToRead >= 14u )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPARAM;
            }
            else
            {
                if( 32u != p_uReadBufferL )
                {
                    l_bRes = false;
                    p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPARAM;
                }
                else
                {
                    p_ptCtx->uTimeUsed++;

                    l_bRes = true;
                    p_ptCtx->eLastEr = e_eFSS_LOGC_RES_OK;
                    (void)memcpy(p_puReadBuffer, m_auStorArea[p_uPageToRead], p_uReadBufferL);
                }
            }
        }
    }

    return l_bRes;
}

static bool_t eFSS_LOGCTST_ReadTst2Adapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPOINTER;
        }
        else
        {
            if( p_uPageToRead >= 14u )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPARAM;
            }
            else
            {
                if( 32u != p_uReadBufferL )
                {
                    l_bRes = false;
                    p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPARAM;
                }
                else
                {
                    p_ptCtx->uTimeUsed++;

                    l_bRes = true;
                    p_ptCtx->eLastEr = e_eFSS_LOGC_RES_OK;
                    (void)memcpy(p_puReadBuffer, m_auStorArea[p_uPageToRead], p_uReadBufferL);
                    p_puReadBuffer[0] = p_puReadBuffer[0] + 2u;
                }
            }
        }
    }

    return l_bRes;
}

static bool_t eFSS_LOGCTST_CrcAdapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
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
            p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPOINTER;
        }
        else
        {
            l_bRes = true;
            *p_puCrc32Val = 0u;
            p_ptCtx->eLastEr = e_eFSS_LOGC_RES_OK;
        }
    }

    return l_bRes;
}

static bool_t eFSS_LOGCTST_CrcErrAdapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
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
            p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPOINTER;
        }
        else
        {
            l_bRes = false;
            *p_puCrc32Val = 0u;
            p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPOINTER;
        }
    }

    return l_bRes;
}

static bool_t eFSS_LOGCTST_CrcTst1Adapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
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
            p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPOINTER;
        }
        else
        {
            if( 0u == p_uDataL )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_LOGC_RES_BADPARAM;
            }
            else
            {
                l_bRes = true;
                p_ptCtx->eLastEr = e_eFSS_LOGC_RES_OK;
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
void eFSS_LOGCTST_BadPointer(void)
{
    /* Local variable */
    t_eFSS_LOGC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[56u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    bool_t l_bIsInit;
    t_eFSS_LOGC_StorBuf l_ltUseBuff;
    bool_t l_bIsNewest;
    uint32_t l_uByteInPage;
    uint32_t l_uPageUsable;
    uint32_t l_uNewPIx;
    uint32_t l_uFillPIdx;

    /* Misra complaiant */
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxCrc32.uTimeUsed = 0u;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseAdapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteAdapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadAdapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcAdapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 14;
    l_tStorSet.uPagesLen = 28u;
    l_tStorSet.uRWERetry = 2u;
    l_tStorSet.uPageVersion = 1u;

    l_bIsNewest = false;
    l_uByteInPage = 0u;
    l_uPageUsable = 0u;
    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;

    /* Function */
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_InitCtx(NULL, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, NULL, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 2  -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.ptCtxErase = NULL;
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 3  -- OK \n");
        l_tCtxCb.ptCtxErase = &l_tCtxErase;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 3  -- FAIL \n");
        l_tCtxCb.ptCtxErase = &l_tCtxErase;
    }

    /* Function */
    l_tCtxCb.fErase = NULL;
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 4  -- OK \n");
        l_tCtxCb.fErase = &eFSS_LOGCTST_EraseAdapt;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 4  -- FAIL \n");
        l_tCtxCb.fErase = &eFSS_LOGCTST_EraseAdapt;
    }

    /* Function */
    l_tCtxCb.ptCtxWrite = NULL;
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 5  -- OK \n");
        l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 5  -- FAIL \n");
        l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    }

    /* Function */
    l_tCtxCb.fWrite = NULL;
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 6  -- OK \n");
        l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteAdapt;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 6  -- FAIL \n");
        l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteAdapt;
    }

    /* Function */
    l_tCtxCb.ptCtxRead = NULL;
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 7  -- OK \n");
        l_tCtxCb.ptCtxRead = &l_tCtxRead;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 7  -- FAIL \n");
        l_tCtxCb.ptCtxRead = &l_tCtxRead;
    }

    /* Function */
    l_tCtxCb.fRead = NULL;
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 8  -- OK \n");
        l_tCtxCb.fRead = &eFSS_LOGCTST_ReadAdapt;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 8  -- FAIL \n");
        l_tCtxCb.fRead = &eFSS_LOGCTST_ReadAdapt;
    }

    /* Function */
    l_tCtxCb.ptCtxCrc32 = NULL;
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 9  -- OK \n");
        l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 9  -- FAIL \n");
        l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    }

    /* Function */
    l_tCtxCb.fCrc32 = NULL;
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 10 -- OK \n");
        l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcAdapt;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 10 -- FAIL \n");
        l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcAdapt;
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_IsInit(NULL, &l_bIsInit) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 11 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_IsInit(&l_tCtx, NULL) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 12 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_IsFlashCacheUsed(NULL, &l_bIsInit) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 13 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_IsFlashCacheUsed(&l_tCtx, NULL) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 14 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 15 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_GetBuffNUsable(NULL, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 16 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 17 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_GetBuffNUsable(&l_tCtx, NULL, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 18 -- OK \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 18 -- FAIL \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 19 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, NULL) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 20 -- OK \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 20 -- FAIL \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 21 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_WriteCache(NULL, 0u, 0u) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 22 -- OK \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 22 -- FAIL \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 23 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_ReadCache(NULL, &l_uNewPIx, &l_uFillPIdx) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 24 -- OK \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 24 -- FAIL \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 25 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_ReadCache(&l_tCtx, NULL, &l_uFillPIdx) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 26 -- OK \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 26 -- FAIL \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 27 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 27 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, NULL) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 28 -- OK \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 28 -- FAIL \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 29 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 29 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_FlushBufferAs(NULL, e_eFSS_LOGC_PAGETYPE_LOG, 0u, 0u) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 30 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 30 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_LoadBufferAs(NULL, e_eFSS_LOGC_PAGETYPE_LOG, 0u, &l_uByteInPage) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 31 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 31 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_LoadBufferAs(&l_tCtx, e_eFSS_LOGC_PAGETYPE_LOG, 0u, NULL) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 32 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 32 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_IsPageNewOrBkup(NULL, 0u, &l_bIsNewest) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 33 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 33 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_IsPageNewOrBkup(&l_tCtx, 0u, NULL) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 34 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 34 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPOINTER == eFSS_LOGC_FlushBuffIfNotEquals(NULL, 0u, e_eFSS_LOGC_PAGETYPE_LOG) )
    {
        (void)printf("eFSS_LOGCTST_BadPointer 35 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadPointer 35 -- FAIL \n");
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

void eFSS_LOGCTST_BadInit(void)
{
    /* Local variable */
    t_eFSS_LOGC_Ctx l_tCtx;
    bool_t l_bIsInit;
    t_eFSS_LOGC_StorBuf l_ltUseBuff;
    uint32_t l_uCrcGetted;
    uint8_t l_uSubTypeRead;
    uint8_t l_uSubTypeWrite;
    bool_t l_bIsNewest;
    bool_t l_bIsFlashCacheUsed;
    uint32_t l_uByteInPage;
    uint32_t l_uPageUsable;
    uint32_t l_uNewPIx;
    uint32_t l_uFillPIdx;

    l_uSubTypeRead = 0u;
    l_uSubTypeWrite = 0u;
    l_bIsNewest = false;
    l_bIsFlashCacheUsed = false;
    l_uByteInPage = 0u;
    l_uPageUsable = 0u;
    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;

    /* Init the remainings var */
    (void)memset(&l_tCtx, 0, sizeof(l_tCtx));

    /* Function */
    if( e_eFSS_LOGC_RES_NOINITLIB == eFSS_LOGC_IsFlashCacheUsed(&l_tCtx, &l_bIsFlashCacheUsed) )
    {
        (void)printf("eFSS_LOGCTST_BadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadInit 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_NOINITLIB == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadInit 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_NOINITLIB == eFSS_LOGC_WriteCache(&l_tCtx, 0u, 0u) )
    {
        (void)printf("eFSS_LOGCTST_BadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadInit 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_NOINITLIB == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx) )
    {
        (void)printf("eFSS_LOGCTST_BadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadInit 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_NOINITLIB == eFSS_LOGC_FlushBufferAs(&l_tCtx, e_eFSS_LOGC_PAGETYPE_LOG, 0u, 0u) )
    {
        (void)printf("eFSS_LOGCTST_BadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadInit 5  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_NOINITLIB == eFSS_LOGC_LoadBufferAs(&l_tCtx, e_eFSS_LOGC_PAGETYPE_LOG, 0u, &l_uByteInPage) )
    {
        (void)printf("eFSS_LOGCTST_BadInit 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadInit 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_IsInit(&l_tCtx, &l_bIsInit) )
    {
        if( false == l_bIsInit )
        {
            (void)printf("eFSS_LOGCTST_BadInit 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadInit 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadInit 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_NOINITLIB == eFSS_LOGC_IsPageNewOrBkup(&l_tCtx, 0u, &l_bIsNewest) )
    {
        (void)printf("eFSS_LOGCTST_BadInit 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadInit 8  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_NOINITLIB == eFSS_LOGC_FlushBuffIfNotEquals(&l_tCtx, 0u, e_eFSS_LOGC_PAGETYPE_LOG) )
    {
        (void)printf("eFSS_LOGCTST_BadInit 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadInit 9  -- FAIL \n");
    }
}

void eFSS_LOGCTST_BadParamEntr(void)
{
    /* Local variable */
    t_eFSS_LOGC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[56u];
    uint8_t l_auStorL[64u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    uint32_t l_uCrcGetted;
    uint8_t l_uSubTypeRead;
    uint8_t l_uSubTypeWrite;
    bool_t l_bIsNewest;
    uint32_t l_uByteInPage;
    uint32_t l_uPageUsable;
    uint32_t l_uNewPIx;
    uint32_t l_uFillPIdx;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

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
    l_tStorSet.uTotPages = 14;
    l_tStorSet.uPagesLen = 28u;
    l_tStorSet.uRWERetry = 2u;
    l_tStorSet.uPageVersion = 1u;

    l_uSubTypeRead = 0u;
    l_uSubTypeWrite = 0u;
    l_bIsNewest = false;
    l_uByteInPage = 0u;
    l_uPageUsable = 0u;
    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;

    /* Function */
    l_tStorSet.uPagesLen = 25u;
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 1  -- OK \n");
        l_tStorSet.uPagesLen = 28u;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 1  -- FAIL \n");
        l_tStorSet.uPagesLen = 28u;
    }

    /* Function */
    l_tStorSet.uPagesLen = 23u;
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 2  -- OK \n");
        l_tStorSet.uPagesLen = 28u;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 2  -- FAIL \n");
        l_tStorSet.uPagesLen = 28u;
    }

    /* Function */
    l_tStorSet.uTotPages = 0u;
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 3  -- OK \n");
        l_tStorSet.uTotPages = 14;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 3  -- FAIL \n");
        l_tStorSet.uTotPages = 14;
    }

    /* Function */
    l_tStorSet.uPagesLen = 19u;
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, false, false  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 4  -- OK \n");
        l_tStorSet.uPagesLen = 28u;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 4  -- FAIL \n");
        l_tStorSet.uPagesLen = 28u;
    }

    /* Function */
    l_tStorSet.uPagesLen = 18u;
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, false, false  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 5  -- OK \n");
        l_tStorSet.uPagesLen = 28u;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 5  -- FAIL \n");
        l_tStorSet.uPagesLen = 28u;
    }

    /* Function */
    l_tStorSet.uPagesLen = 24;
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, false, false  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 6  -- OK \n");
        l_tStorSet.uPagesLen = 28u;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 6  -- FAIL \n");
        l_tStorSet.uPagesLen = 28u;
    }

    /* Function */
    l_tStorSet.uPagesLen = 25;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, false, false  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 7  -- OK \n");
        l_tStorSet.uPagesLen = 28u;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 7  -- FAIL \n");
        l_tStorSet.uPagesLen = 28u;
    }

    /* Function */
    l_tStorSet.uPagesLen = 31;
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, true, false  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 8  -- OK \n");
        l_tStorSet.uPagesLen = 28u;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 8  -- FAIL \n");
        l_tStorSet.uPagesLen = 28u;
    }

    /* Function */
    l_tStorSet.uPagesLen = 32;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, true, false  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 8  -- OK \n");
        l_tStorSet.uPagesLen = 28u;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 8  -- FAIL \n");
        l_tStorSet.uPagesLen = 28u;
    }

    /* Function */
    l_tStorSet.uRWERetry = 0u;
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 9  -- OK \n");
        l_tStorSet.uRWERetry = 1u;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 9  -- FAIL \n");
        l_tStorSet.uRWERetry = 1u;
    }

    /* Function */
    l_tStorSet.uPagesLen = 20u;
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 10 -- OK \n");
        l_tStorSet.uPagesLen = 28u;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 10 -- FAIL \n");
        l_tStorSet.uPagesLen = 28u;
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 11 -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uPagesLen = 32;
    l_tStorSet.uTotPages = 3u;
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, false, false  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 12 -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uPagesLen = 32;
    l_tStorSet.uTotPages = 4u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, false, false  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 13 -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uPagesLen = 32;
    l_tStorSet.uTotPages = 5u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, false, false  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 14 -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uPagesLen = 32;
    l_tStorSet.uTotPages = 5u;
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, true, false  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 15 -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uPagesLen = 32;
    l_tStorSet.uTotPages = 6u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, true, false  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 16 -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uPagesLen = 32;
    l_tStorSet.uTotPages = 7u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, true, false  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 17 -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uPagesLen = 32;
    l_tStorSet.uTotPages = 7u;
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, false, true  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 18 -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uPagesLen = 32;
    l_tStorSet.uTotPages = 6u;
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, false, true  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 19 -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uPagesLen = 32;
    l_tStorSet.uTotPages = 11u;
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, false, true  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 20 -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uPagesLen = 32;
    l_tStorSet.uTotPages = 8u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, false, true  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 21 -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uPagesLen = 32;
    l_tStorSet.uTotPages = 10u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, false, true  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 22 -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uPagesLen = 32;
    l_tStorSet.uTotPages = 7u;
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, true, true  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 23 -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uPagesLen = 32;
    l_tStorSet.uTotPages = 6u;
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, true, true  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 24 -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uPagesLen = 32;
    l_tStorSet.uTotPages = 11u;
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, true, true  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 25 -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uPagesLen = 32;
    l_tStorSet.uTotPages = 10u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, true, true  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 26 -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uPagesLen = 32;
    l_tStorSet.uTotPages = 12u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, true, true  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 27 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 27 -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uPagesLen = 32;
    l_tStorSet.uTotPages = 5u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, false, false  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 28 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 28 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_WriteCache(&l_tCtx, 0u, 0u ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 29 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 29 -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uPagesLen = 32;
    l_tStorSet.uTotPages = 6u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, true, false  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 30 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 30 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_WriteCache(&l_tCtx, 4u, 0u ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 31 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 31 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_WriteCache(&l_tCtx, 0u, 2u ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 32 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 32 -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uPagesLen = 32;
    l_tStorSet.uTotPages = 12u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, true, true  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 33 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 33 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_WriteCache(&l_tCtx, 5u, 0u ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 31 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 31 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_WriteCache(&l_tCtx, 0u, 3u ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 32 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 32 -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uPagesLen = 32;
    l_tStorSet.uTotPages = 5u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, false, false  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 33 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 33 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 34 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 34 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_FlushBufferAs(&l_tCtx, e_eFSS_LOGC_PAGETYPE_NEWEST_BKUP, 5u, 0u ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 34 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 34 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_FlushBufferAs(&l_tCtx, (e_eFSS_LOGC_PAGETYPE)4u, 0u, 0u ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 35 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 35 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_FlushBufferAs(&l_tCtx, e_eFSS_LOGC_PAGETYPE_NEWEST_BKUP, 0u, 9u ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 36 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 36 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_LoadBufferAs(&l_tCtx, e_eFSS_LOGC_PAGETYPE_NEWEST_BKUP, 5u, &l_uByteInPage ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 37 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 37 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_LoadBufferAs(&l_tCtx, (e_eFSS_LOGC_PAGETYPE)4u, 0u, &l_uByteInPage ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 38 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 38 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_IsPageNewOrBkup(&l_tCtx, 5u, &l_bIsNewest ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 39 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 39 -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uPagesLen = 32;
    l_tStorSet.uTotPages = 12u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u, true, true  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 40 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 40 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_IsPageNewOrBkup(&l_tCtx, 5u, &l_bIsNewest ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 41 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 41 -- FAIL \n");
    }

    /* Function */
    (void)memset(l_auStorL, 0, sizeof(l_auStorL));
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_FlushBuffIfNotEquals(&l_tCtx, 5u, e_eFSS_LOGC_PAGETYPE_LOG ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 42 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 42 -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uPagesLen = 32u;
    l_tStorSet.uTotPages = 12u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStorL, sizeof(l_auStorL), true, true  ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 43 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 43 -- FAIL \n");
    }

    /* Function */
    (void)memset(l_auStorL, 0, sizeof(l_auStorL));
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_FlushBuffIfNotEquals(&l_tCtx, 5u, e_eFSS_LOGC_PAGETYPE_LOG ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 44 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 44 -- FAIL \n");
    }

    /* Function */
    (void)memset(l_auStorL, 0, sizeof(l_auStorL));
    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_FlushBuffIfNotEquals(&l_tCtx, 5u, (e_eFSS_LOGC_PAGETYPE)4u ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 45 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 45 -- FAIL \n");
    }

    /* Function */
    (void)memset(l_auStorL, 0, sizeof(l_auStorL));
    l_auStorL[8u] = 9u;

    if( e_eFSS_LOGC_RES_BADPARAM == eFSS_LOGC_FlushBuffIfNotEquals(&l_tCtx, 0u, e_eFSS_LOGC_PAGETYPE_LOG ) )
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 46 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadParamEntr 46 -- FAIL \n");
    }
}

void eFSS_LOGCTST_CorruptedCtx(void)
{
    /* Local variable */
    t_eFSS_LOGC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_uStorType;
    uint8_t l_auStor[64u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_LOGC_StorBuf l_ltUseBuff;
    uint32_t l_uCrcGetted;
    uint8_t l_uSubTypeRead;
    uint8_t l_uSubTypeWrite;
    bool_t l_bIsNewest;
    uint32_t l_uByteInPage;
    uint32_t l_uPageUsable;
    uint32_t l_uNewPIx;
    uint32_t l_uFillPIdx;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseAdapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteAdapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadAdapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcAdapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 14;
    l_tStorSet.uPagesLen = 32u;
    l_tStorSet.uRWERetry = 2u;
    l_tStorSet.uPageVersion = 1u;
    l_uStorType = 1u;
    l_uSubTypeRead = 0u;
    l_uSubTypeWrite = 0u;
    l_bIsNewest = false;
    l_uByteInPage = 0u;
    l_uPageUsable = 0u;
    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 2  -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxErase = NULL;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 3  -- OK \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxErase = &l_tCtxErase;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 3  -- FAIL \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxErase = &l_tCtxErase;
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 4  -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fErase = NULL;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 5  -- OK \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fErase = &eFSS_LOGCTST_EraseAdapt;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 5  -- FAIL \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fErase = &eFSS_LOGCTST_EraseAdapt;
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 6  -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxWrite = NULL;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 7  -- OK \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxWrite = &l_tCtxWrite;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 7  -- FAIL \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxWrite = &l_tCtxWrite;
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 8  -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fWrite = NULL;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 9  -- OK \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fWrite = &eFSS_LOGCTST_WriteAdapt;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 9  -- FAIL \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fWrite = &eFSS_LOGCTST_WriteAdapt;
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 10 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxRead = NULL;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 11 -- OK \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxRead = &l_tCtxRead;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 11 -- FAIL \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxRead = &l_tCtxRead;
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 12 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fRead = NULL;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 13 -- OK \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fRead = &eFSS_LOGCTST_ReadAdapt;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 13 -- FAIL \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fRead = &eFSS_LOGCTST_ReadAdapt;
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 14 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxCrc32 = NULL;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 15 -- OK \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 15 -- FAIL \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 16 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fCrc32 = NULL;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 17 -- OK \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcAdapt;
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 17 -- FAIL \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcAdapt;
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 18 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff1.puBuf = NULL;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 19 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 20 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff2.puBuf = NULL;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 21 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 22 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uTotPages = 0u;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 23 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 24 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uPagesLen = 19u;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 25 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 26 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uRWERetry = 0u;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 27 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 27 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 28 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 28 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff1.uBufL = l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff2.uBufL + 1u;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 29 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 29 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 30 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 30 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uPagesLen = l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff1.uBufL + 1u;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 31 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 31 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 32 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 32 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uPagesLen = 20u;
    l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff1.uBufL = 20u;
    l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff2.uBufL = 20u;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 33 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 33 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 34 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 34 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff2.puBuf = l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff1.puBuf;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 35 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 35 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 36 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 36 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uPagesLen = 24u;
    l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff1.uBufL = 24u;
    l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff2.uBufL = 24u;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 37 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 37 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), true, false ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 38 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 38 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uPagesLen = 31u;
    l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff1.uBufL = 31u;
    l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff2.uBufL = 31u;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 39 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 39 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 40 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 40 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uTotPages = 3u;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 41 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 41 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), true, false ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 42 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 42 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uTotPages = 5u;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 43 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 43 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, true ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 44 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 44 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uTotPages = 7u;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 45 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 45 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uTotPages = 11u;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 46 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 46 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), true, true ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 47 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 47 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uTotPages = 9u;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 48 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 48 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uTotPages = 17u;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 49 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 49 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), true, true ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 50 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 50 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uTotPages = 9u;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_IsFlashCacheUsed(&l_tCtx, &l_bIsNewest) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 51 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 51 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), true, true ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 52 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 52 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uTotPages = 9u;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_WriteCache(&l_tCtx, 0u, 0u) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 53 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 53 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), true, true ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 54 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 54 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uTotPages = 9u;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 55 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 55 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), true, true ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 56 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 56 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uTotPages = 9u;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_FlushBufferAs(&l_tCtx, e_eFSS_LOGC_PAGETYPE_LOG, 0u, 0u) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 57 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 57 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), true, true ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 58 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 58 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uTotPages = 9u;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_LoadBufferAs(&l_tCtx, e_eFSS_LOGC_PAGETYPE_LOG, 0u, &l_uFillPIdx) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 59 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 59 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), true, true ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 60 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 60 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uTotPages = 9u;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_IsPageNewOrBkup(&l_tCtx, 0u, &l_bIsNewest) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 61 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 61 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), true, true ) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 62 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 62 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uTotPages = 9u;
    if( e_eFSS_LOGC_RES_CORRUPTCTX == eFSS_LOGC_FlushBuffIfNotEquals(&l_tCtx, 0u, e_eFSS_LOGC_PAGETYPE_LOG) )
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 63 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_CorruptedCtx 63 -- FAIL \n");
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

void eFSS_LOGCTST_Basic(void)
{
    /* Local variable */
    t_eFSS_LOGC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_uStorType;
    uint8_t l_auStor[64u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    bool_t l_bIsInit;
    t_eFSS_LOGC_StorBuf l_ltUseBuff;
    uint32_t l_uByteInPage;
    uint32_t l_uPageUsable;
    uint32_t l_uNewPIx;
    uint32_t l_uFillPIdx;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseAdapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteAdapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadErrAdapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcAdapt;

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
    l_tStorSet.uTotPages = 14;
    l_tStorSet.uPagesLen = 32u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;
    l_uStorType = 1u;
    l_uByteInPage = 0u;
    l_uPageUsable = 0u;
    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_Basic 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Basic 1  -- FAIL \n");
    }

    /* Verify buffer validity */
    if( l_auStor == l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff1.puBuf )
    {
        (void)printf("eFSS_LOGCTST_Basic 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Basic 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_IsInit(&l_tCtx, &l_bIsInit ) )
    {
        if( true == l_bIsInit )
        {
            (void)printf("eFSS_LOGCTST_Basic 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_Basic 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Basic 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_IsFlashCacheUsed(&l_tCtx, &l_bIsInit ) )
    {
        if( false == l_bIsInit )
        {
            (void)printf("eFSS_LOGCTST_Basic 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_Basic 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Basic 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), true, false ) )
    {
        (void)printf("eFSS_LOGCTST_Basic 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Basic 5  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_IsFlashCacheUsed(&l_tCtx, &l_bIsInit ) )
    {
        if( true == l_bIsInit )
        {
            (void)printf("eFSS_LOGCTST_Basic 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_Basic 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Basic 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_Basic 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Basic 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        if( ( l_auStor == l_ltUseBuff.puBuf ) && ( (28u-20u) == l_ltUseBuff.uBufL ) &&
            ( (14u) == l_uPageUsable ))
        {
            (void)printf("eFSS_LOGCTST_Basic 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_Basic 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Basic 8  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), true, false ) )
    {
        (void)printf("eFSS_LOGCTST_Basic 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Basic 9  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        if( ( l_auStor == l_ltUseBuff.puBuf ) && ( (28u-20u) == l_ltUseBuff.uBufL ) &&
            ( (12u) == l_uPageUsable ))
        {
            (void)printf("eFSS_LOGCTST_Basic 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_Basic 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Basic 10 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, true ) )
    {
        (void)printf("eFSS_LOGCTST_Basic 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Basic 11 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        if( ( l_auStor == l_ltUseBuff.puBuf ) && ( (28u-20u) == l_ltUseBuff.uBufL ) &&
            ( (7u) == l_uPageUsable ))
        {
            (void)printf("eFSS_LOGCTST_Basic 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_Basic 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Basic 12 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), true, true ) )
    {
        (void)printf("eFSS_LOGCTST_Basic 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Basic 13 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        if( ( l_auStor == l_ltUseBuff.puBuf ) && ( (28u-20u) == l_ltUseBuff.uBufL ) &&
            ( (6u) == l_uPageUsable ))
        {
            (void)printf("eFSS_LOGCTST_Basic 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_Basic 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Basic 14 -- FAIL \n");
    }
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "CERT-INT31-C_c"
    /* Suppressed for code clarity in test execution*/
#endif

void eFSS_LOGCTST_BadClBckNRetry(void)
{
    /* Local variable */
    t_eFSS_LOGC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_uStorType;
    uint8_t l_auStor[64u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    bool_t l_bIsInit;
    t_eFSS_LOGC_StorBuf l_ltUseBuff;
    uint32_t l_uCrcGetted;
    uint8_t l_uSubTypeRead;
    uint8_t l_uSubTypeWrite;
    bool_t l_bIsNewest;
    uint32_t l_uByteInPage;
    uint32_t l_uPageUsable;
    uint32_t l_uNewPIx;
    uint32_t l_uFillPIdx;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseAdapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteAdapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadErrAdapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcAdapt;

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
    l_tStorSet.uTotPages = 14u;
    l_tStorSet.uPagesLen = 32u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;
    l_uStorType = 1u;
    l_uSubTypeRead = 0u;
    l_uSubTypeWrite = 0u;
    l_bIsNewest = false;
    l_uByteInPage = 0u;
    l_uPageUsable = 0u;
    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;

    /* ------------------------------------------------------------------------------------------ TEST READ CALL BACK */
    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), true, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_IsInit(&l_tCtx, &l_bIsInit ) )
    {
        if( true == l_bIsInit )
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 2  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        if( ( l_auStor == l_ltUseBuff.puBuf ) && ( (32u-24u) == l_ltUseBuff.uBufL ) && ( 12u == l_uPageUsable ) )
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_IsInit(&l_tCtx, &l_bIsInit ) )
    {
        if( true == l_bIsInit )
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_IsFlashCacheUsed(&l_tCtx, &l_bIsInit ) )
    {
        if( true == l_bIsInit )
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 5  -- FAIL \n");
    }

    /* ----------------------------------------------------------------------------------------- TEST LOAD CACHE CALL BACK */
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;
    if( e_eFSS_LOGC_RES_CLBCKREADERR == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx) )
    {
        if( ( l_tStorSet.uRWERetry == l_tCtxRead.uTimeUsed ) && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxRead.eLastEr ) &&
            ( 0u == l_tCtxCrc32.uTimeUsed ) )
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 6  -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcErrAdapt;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), true, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 8  -- FAIL \n");
    }

    /* Test LOAD */
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_CLBCKCRCERR == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx) )
    {
        if( ( 1u == l_tCtxRead.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxCrc32.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 9  -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 9  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 9  -- FAIL \n");
    }

    /* ----------------------------------------------------------------------------------------- TEST FLUSH CACHE CALL BACK */
    /* Function */
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadErrAdapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcAdapt;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), true, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 10 -- FAIL \n");
    }

    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;
    if( e_eFSS_LOGC_RES_CLBCKREADERR == eFSS_LOGC_WriteCache(&l_tCtx, 0u, 0u) )
    {
        if( ( l_tStorSet.uRWERetry == l_tCtxRead.uTimeUsed ) && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxRead.eLastEr ) &&
            ( 1u == l_tCtxCrc32.uTimeUsed ) )
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 11 -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcErrAdapt;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), true, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 12 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 13 -- FAIL \n");
    }

    /* Test LOAD */
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_CLBCKCRCERR == eFSS_LOGC_WriteCache(&l_tCtx, 0u, 0u) )
    {
        if( ( 0u == l_tCtxRead.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxCrc32.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 14 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 14 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 14 -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst2Adapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcAdapt;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), true, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 15 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 16 -- FAIL \n");
    }

    /* Test LOAD */
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_WRITENOMATCHREAD == eFSS_LOGC_WriteCache(&l_tCtx, 0u, 0u) )
    {
        if( ( 3u == l_tCtxRead.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxRead.eLastEr ) &&
            ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 3u == l_tCtxErase.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxErase.eLastEr ))
        {
            if( ( 3u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 17 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 17 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 17 -- FAIL \n");
    }

    /* ----------------------------------------------------------------------------------------- TEST FLUSH AS CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseAdapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteAdapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcErrAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 18 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 19 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_CLBCKCRCERR == eFSS_LOGC_FlushBufferAs(&l_tCtx, e_eFSS_LOGC_PAGETYPE_LOG, 0u, 0u) )
    {
        if( ( 0u == l_tCtxErase.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 0u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxWrite.eLastEr ) &&
            ( 0u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_LOGC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxCrc32.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 20 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 20 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 20 -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseErrAdapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteAdapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 21 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_CLBCKERASEERR == eFSS_LOGC_FlushBufferAs(&l_tCtx, e_eFSS_LOGC_PAGETYPE_LOG, 0u, 0u) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 0u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxWrite.eLastEr ) &&
            ( 0u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_LOGC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 3u == l_tCtxErase.uTimeUsed ) && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxErase.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 22 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 22 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 22 -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseAdapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteErrAdapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 23 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_CLBCKWRITEERR == eFSS_LOGC_FlushBufferAs(&l_tCtx, e_eFSS_LOGC_PAGETYPE_LOG, 0u, 0u) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 3u == l_tCtxErase.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 0u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_LOGC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 3u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 24 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 24 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 24 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 24 -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseAdapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteAdapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadErrAdapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 25 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 25 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_CLBCKREADERR == eFSS_LOGC_FlushBufferAs(&l_tCtx, e_eFSS_LOGC_PAGETYPE_LOG, 0u, 0u) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 3u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 3u == l_tCtxWrite.uTimeUsed )  && ( e_eFSS_LOGC_RES_OK == l_tCtxWrite.eLastEr ) )
        {
            if( ( 3u == l_tCtxRead.uTimeUsed ) && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxRead.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 26 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 26 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 26 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 26 -- FAIL \n");
    }

    /* ----------------------------------------------------------------------------------------- TEST FLUSH AS BKUP CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseAdapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteErrAdapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, true ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 27 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 27 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 28 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 28 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_CLBCKWRITEERR == eFSS_LOGC_FlushBufferAs(&l_tCtx, e_eFSS_LOGC_PAGETYPE_LOG, 0u, 0u) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 3u == l_tCtxErase.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 0u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_LOGC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 3u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 29 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 29 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 29 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 29 -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteErrSecAdapt;
    l_tCtxCb.fRead  = &eFSS_LOGCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, true ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 30 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 30 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 31 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 31 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_CLBCKWRITEERR == eFSS_LOGC_FlushBufferAs(&l_tCtx, e_eFSS_LOGC_PAGETYPE_LOG, 0u, 0u) )
    {
        if( ( 2u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 4u == l_tCtxErase.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 1u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_LOGC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 1u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 32 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 32 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 32 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 32 -- FAIL \n");
    }

    /* ----------------------------------------------------------------------------------------- TEST LOAD AS CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseAdapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteAdapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcErrAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 33 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 33 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 34 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 34 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_CLBCKCRCERR == eFSS_LOGC_LoadBufferAs(&l_tCtx, e_eFSS_LOGC_PAGETYPE_LOG, 0u, &l_uFillPIdx) )
    {
        if( ( 0u == l_tCtxErase.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 0u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxWrite.eLastEr ) &&
            ( 1u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_LOGC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxCrc32.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 35 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 35 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 35 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 35 -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseAdapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteAdapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadErrAdapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 36 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 36 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_CLBCKREADERR == eFSS_LOGC_LoadBufferAs(&l_tCtx, e_eFSS_LOGC_PAGETYPE_LOG, 0u, &l_uFillPIdx) )
    {
        if( ( 0u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 0u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxWrite.eLastEr ) &&
            ( 3u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxRead.eLastEr ) )
        {
            if( ( 0u == l_tCtxErase.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxErase.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 37 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 37 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 37 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 37 -- FAIL \n");
    }


    /* ----------------------------------------------------------------------------------------- TEST LOAD FULL BKP AS CALL BACK */

    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseAdapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteAdapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadErrAdapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, true ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 38 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 38 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 39 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 39 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_CLBCKREADERR == eFSS_LOGC_LoadBufferAs(&l_tCtx, e_eFSS_LOGC_PAGETYPE_LOG, 0u, &l_uFillPIdx) )
    {
        if( ( 0u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 0u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 0u == l_tCtxWrite.uTimeUsed )  && ( e_eFSS_LOGC_RES_OK == l_tCtxWrite.eLastEr ) )
        {
            if( ( 3u == l_tCtxRead.uTimeUsed ) && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxRead.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 40 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 40 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 40 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 40 -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadErrSecAdapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, true ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 41 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 41 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 42 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 42 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_CLBCKREADERR == eFSS_LOGC_LoadBufferAs(&l_tCtx, e_eFSS_LOGC_PAGETYPE_LOG, 0u, &l_uFillPIdx) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 0u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 0u == l_tCtxWrite.uTimeUsed )  && ( e_eFSS_LOGC_RES_OK == l_tCtxWrite.eLastEr ) )
        {
            if( ( 1u == l_tCtxRead.uTimeUsed ) && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxRead.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 43 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 43 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 43 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 43 -- FAIL \n");
    }

    /* Init */
    l_tStorSet.uPageVersion = 0u;

    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteErrAdapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, true ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 44 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 44 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 45 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 45 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[0u], 0, sizeof(m_auStorArea[0u]));
    (void)memset(m_auStorArea[7u], 0xFF, sizeof(m_auStorArea[7u]));

    /* Fill the two storage page */


    /* Setup storage area */
    (void)memset(m_auStorArea[0u], 0, sizeof(m_auStorArea[0u]));
    m_auStorArea[0u][12u] = 0x01u;  /* Page SUBTYPE */
    m_auStorArea[0u][13u] = 0x00u;  /* Page index */
    m_auStorArea[0u][14u] = 0x00u;  /* Page index */
    m_auStorArea[0u][15u] = 0x00u;  /* Page index */
    m_auStorArea[0u][16u] = 0x00u;  /* Page index */
    m_auStorArea[0u][17u] = 0x02u;  /* Page type */
    m_auStorArea[0u][18u] = 0x00u;  /* Page version */
    m_auStorArea[0u][19u] = 0x00u;  /* Page version */
    m_auStorArea[0u][20u] = 0x0Eu;  /* Total page */
    m_auStorArea[0u][21u] = 0x00u;  /* Total page */
    m_auStorArea[0u][22u] = 0x00u;  /* Total page */
    m_auStorArea[0u][23u] = 0x00u;  /* Total page */
    m_auStorArea[0u][24u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][25u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][26u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][27u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][28u] = 0xA4u;  /* CRC */
    m_auStorArea[0u][29u] = 0x02u;  /* CRC */
    m_auStorArea[0u][30u] = 0x00u;  /* CRC */
    m_auStorArea[0u][31u] = 0x00u;  /* CRC */

    (void)memset(m_auStorArea[1u], 0x10, sizeof(m_auStorArea[1u]));
    m_auStorArea[7u][12u] = 0x01u;  /* Page SUBTYPE */
    m_auStorArea[7u][13u] = 0x07u;  /* Page index */
    m_auStorArea[7u][14u] = 0x00u;  /* Page index */
    m_auStorArea[7u][15u] = 0x00u;  /* Page index */
    m_auStorArea[7u][16u] = 0x00u;  /* Page index */
    m_auStorArea[7u][17u] = 0x02u;  /* Page type */
    m_auStorArea[7u][18u] = 0x00u;  /* Page version */
    m_auStorArea[7u][19u] = 0x00u;  /* Page version */
    m_auStorArea[7u][20u] = 0x0eu;  /* Total page */
    m_auStorArea[7u][21u] = 0x00u;  /* Total page */
    m_auStorArea[7u][22u] = 0x00u;  /* Total page */
    m_auStorArea[7u][23u] = 0x00u;  /* Total page */
    m_auStorArea[7u][24u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][25u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][26u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][27u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][28u] = 0xF5u;  /* CRC */
    m_auStorArea[7u][29u] = 0x02u;  /* CRC */
    m_auStorArea[7u][30u] = 0x00u;  /* CRC */
    m_auStorArea[7u][31u] = 0x00u;  /* CRC */

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_CLBCKWRITEERR == eFSS_LOGC_LoadBufferAs(&l_tCtx, e_eFSS_LOGC_PAGETYPE_LOG, 0u, &l_uFillPIdx) )
    {
        if( ( 3u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 3u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 2u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_LOGC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 3u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 46 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 46 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 46 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 46 -- FAIL \n");
    }

    /* ----------------------------------------------------------------------------------------- TEST IS PAGE NEW OR BKP AS CALL BACK */
    /* Init */
    l_tStorSet.uPageVersion = 0u;

    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadErrAdapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, false ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 47 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 47 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 48 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 48 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[0u], 0, sizeof(m_auStorArea[0u]));
    (void)memset(m_auStorArea[7u], 0xFF, sizeof(m_auStorArea[7u]));

    /* Fill the two storage page */


    /* Setup storage area */
    (void)memset(m_auStorArea[0u], 0, sizeof(m_auStorArea[0u]));
    m_auStorArea[0u][12u] = 0x01u;  /* Page SUBTYPE */
    m_auStorArea[0u][13u] = 0x00u;  /* Page index */
    m_auStorArea[0u][14u] = 0x00u;  /* Page index */
    m_auStorArea[0u][15u] = 0x00u;  /* Page index */
    m_auStorArea[0u][16u] = 0x00u;  /* Page index */
    m_auStorArea[0u][17u] = 0x02u;  /* Page type */
    m_auStorArea[0u][18u] = 0x00u;  /* Page version */
    m_auStorArea[0u][19u] = 0x00u;  /* Page version */
    m_auStorArea[0u][20u] = 0x0Eu;  /* Total page */
    m_auStorArea[0u][21u] = 0x00u;  /* Total page */
    m_auStorArea[0u][22u] = 0x00u;  /* Total page */
    m_auStorArea[0u][23u] = 0x00u;  /* Total page */
    m_auStorArea[0u][24u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][25u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][26u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][27u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][28u] = 0xA4u;  /* CRC */
    m_auStorArea[0u][29u] = 0x02u;  /* CRC */
    m_auStorArea[0u][30u] = 0x00u;  /* CRC */
    m_auStorArea[0u][31u] = 0x00u;  /* CRC */

    (void)memset(m_auStorArea[1u], 0x10, sizeof(m_auStorArea[1u]));
    m_auStorArea[7u][12u] = 0x01u;  /* Page SUBTYPE */
    m_auStorArea[7u][13u] = 0x07u;  /* Page index */
    m_auStorArea[7u][14u] = 0x00u;  /* Page index */
    m_auStorArea[7u][15u] = 0x00u;  /* Page index */
    m_auStorArea[7u][16u] = 0x00u;  /* Page index */
    m_auStorArea[7u][17u] = 0x02u;  /* Page type */
    m_auStorArea[7u][18u] = 0x00u;  /* Page version */
    m_auStorArea[7u][19u] = 0x00u;  /* Page version */
    m_auStorArea[7u][20u] = 0x0eu;  /* Total page */
    m_auStorArea[7u][21u] = 0x00u;  /* Total page */
    m_auStorArea[7u][22u] = 0x00u;  /* Total page */
    m_auStorArea[7u][23u] = 0x00u;  /* Total page */
    m_auStorArea[7u][24u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][25u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][26u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][27u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][28u] = 0xF5u;  /* CRC */
    m_auStorArea[7u][29u] = 0x02u;  /* CRC */
    m_auStorArea[7u][30u] = 0x00u;  /* CRC */
    m_auStorArea[7u][31u] = 0x00u;  /* CRC */

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_CLBCKREADERR == eFSS_LOGC_IsPageNewOrBkup(&l_tCtx, 0u, &l_bIsNewest) )
    {
        if( ( 0u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 0u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 3u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxRead.eLastEr ) )
        {
            if( ( 0u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 49 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 49 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 49 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 49 -- FAIL \n");
    }


    /* ----------------------------------------------------------------------------------------- TEST IS PAGE NEW OR BKP AS CALL BACK */
    /* Init */
    l_tStorSet.uPageVersion = 0u;

    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadErrSecAdapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, true ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 50 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 50 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 51 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 51 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[0u], 0, sizeof(m_auStorArea[0u]));
    (void)memset(m_auStorArea[7u], 0xFF, sizeof(m_auStorArea[7u]));

    /* Fill the two storage page */


    /* Setup storage area */
    (void)memset(m_auStorArea[0u], 0, sizeof(m_auStorArea[0u]));
    m_auStorArea[0u][12u] = 0x03u;  /* Page SUBTYPE */
    m_auStorArea[0u][13u] = 0x00u;  /* Page index */
    m_auStorArea[0u][14u] = 0x00u;  /* Page index */
    m_auStorArea[0u][15u] = 0x00u;  /* Page index */
    m_auStorArea[0u][16u] = 0x00u;  /* Page index */
    m_auStorArea[0u][17u] = 0x02u;  /* Page type */
    m_auStorArea[0u][18u] = 0x00u;  /* Page version */
    m_auStorArea[0u][19u] = 0x00u;  /* Page version */
    m_auStorArea[0u][20u] = 0x0Eu;  /* Total page */
    m_auStorArea[0u][21u] = 0x00u;  /* Total page */
    m_auStorArea[0u][22u] = 0x00u;  /* Total page */
    m_auStorArea[0u][23u] = 0x00u;  /* Total page */
    m_auStorArea[0u][24u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][25u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][26u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][27u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][28u] = 0xA6u;  /* CRC */
    m_auStorArea[0u][29u] = 0x02u;  /* CRC */
    m_auStorArea[0u][30u] = 0x00u;  /* CRC */
    m_auStorArea[0u][31u] = 0x00u;  /* CRC */

    (void)memset(m_auStorArea[1u], 0x10, sizeof(m_auStorArea[1u]));
    m_auStorArea[7u][12u] = 0x01u;  /* Page SUBTYPE */
    m_auStorArea[7u][13u] = 0x07u;  /* Page index */
    m_auStorArea[7u][14u] = 0x00u;  /* Page index */
    m_auStorArea[7u][15u] = 0x00u;  /* Page index */
    m_auStorArea[7u][16u] = 0x00u;  /* Page index */
    m_auStorArea[7u][17u] = 0x02u;  /* Page type */
    m_auStorArea[7u][18u] = 0x00u;  /* Page version */
    m_auStorArea[7u][19u] = 0x00u;  /* Page version */
    m_auStorArea[7u][20u] = 0x0eu;  /* Total page */
    m_auStorArea[7u][21u] = 0x00u;  /* Total page */
    m_auStorArea[7u][22u] = 0x00u;  /* Total page */
    m_auStorArea[7u][23u] = 0x00u;  /* Total page */
    m_auStorArea[7u][24u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][25u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][26u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][27u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][28u] = 0xF5u;  /* CRC */
    m_auStorArea[7u][29u] = 0x02u;  /* CRC */
    m_auStorArea[7u][30u] = 0x00u;  /* CRC */
    m_auStorArea[7u][31u] = 0x00u;  /* CRC */

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_CLBCKREADERR == eFSS_LOGC_IsPageNewOrBkup(&l_tCtx, 0u, &l_bIsNewest) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 0u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 1u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxRead.eLastEr ) )
        {
            if( ( 0u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 52 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 52 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 52 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 52 -- FAIL \n");
    }

    /* ----------------------------------------------------------------------------------------- TEST IS PAGE NEW OR BKP AS CALL BACK */
    /* Init */
    l_tStorSet.uPageVersion = 0u;

    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteErrAdapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, true ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 53 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 53 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 54 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 54 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[0u], 0, sizeof(m_auStorArea[0u]));
    (void)memset(m_auStorArea[7u], 0xFF, sizeof(m_auStorArea[7u]));

    /* Fill the two storage page */


    /* Setup storage area */
    (void)memset(m_auStorArea[0u], 0, sizeof(m_auStorArea[0u]));
    m_auStorArea[0u][12u] = 0x03u;  /* Page SUBTYPE */
    m_auStorArea[0u][13u] = 0x00u;  /* Page index */
    m_auStorArea[0u][14u] = 0x00u;  /* Page index */
    m_auStorArea[0u][15u] = 0x00u;  /* Page index */
    m_auStorArea[0u][16u] = 0x00u;  /* Page index */
    m_auStorArea[0u][17u] = 0x02u;  /* Page type */
    m_auStorArea[0u][18u] = 0x00u;  /* Page version */
    m_auStorArea[0u][19u] = 0x00u;  /* Page version */
    m_auStorArea[0u][20u] = 0x0Eu;  /* Total page */
    m_auStorArea[0u][21u] = 0x00u;  /* Total page */
    m_auStorArea[0u][22u] = 0x00u;  /* Total page */
    m_auStorArea[0u][23u] = 0x00u;  /* Total page */
    m_auStorArea[0u][24u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][25u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][26u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][27u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][28u] = 0xA6u;  /* CRC */
    m_auStorArea[0u][29u] = 0x02u;  /* CRC */
    m_auStorArea[0u][30u] = 0x00u;  /* CRC */
    m_auStorArea[0u][31u] = 0x00u;  /* CRC */

    (void)memset(m_auStorArea[1u], 0x10, sizeof(m_auStorArea[1u]));
    m_auStorArea[7u][12u] = 0x01u;  /* Page SUBTYPE */
    m_auStorArea[7u][13u] = 0x07u;  /* Page index */
    m_auStorArea[7u][14u] = 0x00u;  /* Page index */
    m_auStorArea[7u][15u] = 0x00u;  /* Page index */
    m_auStorArea[7u][16u] = 0x00u;  /* Page index */
    m_auStorArea[7u][17u] = 0x02u;  /* Page type */
    m_auStorArea[7u][18u] = 0x00u;  /* Page version */
    m_auStorArea[7u][19u] = 0x00u;  /* Page version */
    m_auStorArea[7u][20u] = 0x0eu;  /* Total page */
    m_auStorArea[7u][21u] = 0x00u;  /* Total page */
    m_auStorArea[7u][22u] = 0x00u;  /* Total page */
    m_auStorArea[7u][23u] = 0x00u;  /* Total page */
    m_auStorArea[7u][24u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][25u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][26u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][27u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][28u] = 0xF5u;  /* CRC */
    m_auStorArea[7u][29u] = 0x02u;  /* CRC */
    m_auStorArea[7u][30u] = 0x00u;  /* CRC */
    m_auStorArea[7u][31u] = 0x00u;  /* CRC */

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_CLBCKWRITEERR == eFSS_LOGC_IsPageNewOrBkup(&l_tCtx, 0u, &l_bIsNewest) )
    {
        if( ( 3u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 3u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 2u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_LOGC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 3u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 55 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 55 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 55 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 55 -- FAIL \n");
    }

    /* ----------------------------------------------------------------------------------------- TEST IS PAGE NEW OR BKP AS CALL BACK */
    /* Init */
    l_tStorSet.uPageVersion = 0u;

    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteErrAdapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, true ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 56 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 56 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 57 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 57 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[0u], 0, sizeof(m_auStorArea[0u]));
    (void)memset(m_auStorArea[7u], 0xFF, sizeof(m_auStorArea[7u]));

    /* Fill the two storage page */


    /* Setup storage area */
    (void)memset(m_auStorArea[0u], 0, sizeof(m_auStorArea[0u]));
    m_auStorArea[0u][8u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][9u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][10u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][11u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][12u] = 0x04u;  /* Page SUBTYPE */
    m_auStorArea[0u][13u] = 0x00u;  /* Page index */
    m_auStorArea[0u][14u] = 0x00u;  /* Page index */
    m_auStorArea[0u][15u] = 0x00u;  /* Page index */
    m_auStorArea[0u][16u] = 0x00u;  /* Page index */
    m_auStorArea[0u][17u] = 0x02u;  /* Page type */
    m_auStorArea[0u][18u] = 0x00u;  /* Page version */
    m_auStorArea[0u][19u] = 0x00u;  /* Page version */
    m_auStorArea[0u][20u] = 0x0Eu;  /* Total page */
    m_auStorArea[0u][21u] = 0x00u;  /* Total page */
    m_auStorArea[0u][22u] = 0x00u;  /* Total page */
    m_auStorArea[0u][23u] = 0x00u;  /* Total page */
    m_auStorArea[0u][24u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][25u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][26u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][27u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][28u] = 0xA6u;  /* CRC */
    m_auStorArea[0u][29u] = 0x02u;  /* CRC */
    m_auStorArea[0u][30u] = 0x00u;  /* CRC */
    m_auStorArea[0u][31u] = 0xFFu;  /* CRC */

    (void)memset(m_auStorArea[1u], 0x10, sizeof(m_auStorArea[1u]));
    m_auStorArea[7u][8u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][9u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][10u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][11u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][12u] = 0x04u;  /* Page SUBTYPE */
    m_auStorArea[7u][13u] = 0x07u;  /* Page index */
    m_auStorArea[7u][14u] = 0x00u;  /* Page index */
    m_auStorArea[7u][15u] = 0x00u;  /* Page index */
    m_auStorArea[7u][16u] = 0x00u;  /* Page index */
    m_auStorArea[7u][17u] = 0x02u;  /* Page type */
    m_auStorArea[7u][18u] = 0x00u;  /* Page version */
    m_auStorArea[7u][19u] = 0x00u;  /* Page version */
    m_auStorArea[7u][20u] = 0x0eu;  /* Total page */
    m_auStorArea[7u][21u] = 0x00u;  /* Total page */
    m_auStorArea[7u][22u] = 0x00u;  /* Total page */
    m_auStorArea[7u][23u] = 0x00u;  /* Total page */
    m_auStorArea[7u][24u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][25u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][26u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][27u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][28u] = 0xA6u;  /* CRC */
    m_auStorArea[7u][29u] = 0x0Au;  /* CRC */
    m_auStorArea[7u][30u] = 0x00u;  /* CRC */
    m_auStorArea[7u][31u] = 0x00u;  /* CRC */

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_CLBCKWRITEERR == eFSS_LOGC_IsPageNewOrBkup(&l_tCtx, 0u, &l_bIsNewest) )
    {
        if( ( 3u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 3u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 2u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_LOGC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 3u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 58 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 58 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 58 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 58 -- FAIL \n");
    }

    /* Init */
    l_tStorSet.uPageVersion = 0u;

    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadErrSecAdapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, true ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 59 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 59 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 60 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 60 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[0u], 0, sizeof(m_auStorArea[0u]));
    (void)memset(m_auStorArea[7u], 0xFF, sizeof(m_auStorArea[7u]));

    /* Fill the two storage page */


    /* Setup storage area */
    (void)memset(m_auStorArea[0u], 0, sizeof(m_auStorArea[0u]));
    m_auStorArea[0u][8u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][9u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][10u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][11u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][12u] = 0x04u;  /* Page SUBTYPE */
    m_auStorArea[0u][13u] = 0x00u;  /* Page index */
    m_auStorArea[0u][14u] = 0x00u;  /* Page index */
    m_auStorArea[0u][15u] = 0x00u;  /* Page index */
    m_auStorArea[0u][16u] = 0x00u;  /* Page index */
    m_auStorArea[0u][17u] = 0x02u;  /* Page type */
    m_auStorArea[0u][18u] = 0x00u;  /* Page version */
    m_auStorArea[0u][19u] = 0x00u;  /* Page version */
    m_auStorArea[0u][20u] = 0x0Eu;  /* Total page */
    m_auStorArea[0u][21u] = 0x00u;  /* Total page */
    m_auStorArea[0u][22u] = 0x00u;  /* Total page */
    m_auStorArea[0u][23u] = 0x00u;  /* Total page */
    m_auStorArea[0u][24u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][25u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][26u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][27u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][28u] = 0xA6u;  /* CRC */
    m_auStorArea[0u][29u] = 0x02u;  /* CRC */
    m_auStorArea[0u][30u] = 0x00u;  /* CRC */
    m_auStorArea[0u][31u] = 0xFFu;  /* CRC */

    (void)memset(m_auStorArea[1u], 0x10, sizeof(m_auStorArea[1u]));
    m_auStorArea[7u][8u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][9u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][10u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][11u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][12u] = 0x04u;  /* Page SUBTYPE */
    m_auStorArea[7u][13u] = 0x07u;  /* Page index */
    m_auStorArea[7u][14u] = 0x00u;  /* Page index */
    m_auStorArea[7u][15u] = 0x00u;  /* Page index */
    m_auStorArea[7u][16u] = 0x00u;  /* Page index */
    m_auStorArea[7u][17u] = 0x02u;  /* Page type */
    m_auStorArea[7u][18u] = 0x00u;  /* Page version */
    m_auStorArea[7u][19u] = 0x00u;  /* Page version */
    m_auStorArea[7u][20u] = 0x0eu;  /* Total page */
    m_auStorArea[7u][21u] = 0x00u;  /* Total page */
    m_auStorArea[7u][22u] = 0x00u;  /* Total page */
    m_auStorArea[7u][23u] = 0x00u;  /* Total page */
    m_auStorArea[7u][24u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][25u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][26u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][27u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][28u] = 0xA6u;  /* CRC */
    m_auStorArea[7u][29u] = 0x0Au;  /* CRC */
    m_auStorArea[7u][30u] = 0x00u;  /* CRC */
    m_auStorArea[7u][31u] = 0x00u;  /* CRC */

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_CLBCKREADERR == eFSS_LOGC_IsPageNewOrBkup(&l_tCtx, 0u, &l_bIsNewest) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 0u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 1u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxRead.eLastEr ) )
        {
            if( ( 0u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 61 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 61 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 61 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 61 -- FAIL \n");
    }

    /* ----------------------------------------------------------------------------------------- TEST FLUSHIFNOTEQUAL AS CALL BACK */
    /* Init */
    l_tStorSet.uPageVersion = 0u;

    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadErrAdapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, true ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 62 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 62 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 63 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 63 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[0u], 0, sizeof(m_auStorArea[0u]));
    (void)memset(m_auStorArea[7u], 0xFF, sizeof(m_auStorArea[7u]));

    /* Fill the two storage page */


    /* Setup storage area */
    (void)memset(m_auStorArea[0u], 0, sizeof(m_auStorArea[0u]));
    m_auStorArea[0u][8u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][9u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][10u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][11u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][12u] = 0x03u;  /* Page SUBTYPE */
    m_auStorArea[0u][13u] = 0x00u;  /* Page index */
    m_auStorArea[0u][14u] = 0x00u;  /* Page index */
    m_auStorArea[0u][15u] = 0x00u;  /* Page index */
    m_auStorArea[0u][16u] = 0x00u;  /* Page index */
    m_auStorArea[0u][17u] = 0x02u;  /* Page type */
    m_auStorArea[0u][18u] = 0x00u;  /* Page version */
    m_auStorArea[0u][19u] = 0x00u;  /* Page version */
    m_auStorArea[0u][20u] = 0x0Eu;  /* Total page */
    m_auStorArea[0u][21u] = 0x00u;  /* Total page */
    m_auStorArea[0u][22u] = 0x00u;  /* Total page */
    m_auStorArea[0u][23u] = 0x00u;  /* Total page */
    m_auStorArea[0u][24u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][25u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][26u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][27u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][28u] = 0xA6u;  /* CRC */
    m_auStorArea[0u][29u] = 0x02u;  /* CRC */
    m_auStorArea[0u][30u] = 0x00u;  /* CRC */
    m_auStorArea[0u][31u] = 0x00u;  /* CRC */

    (void)memset(m_auStorArea[1u], 0x10, sizeof(m_auStorArea[1u]));
    m_auStorArea[7u][8u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][9u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][10u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][11u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][12u] = 0x01u;  /* Page SUBTYPE */
    m_auStorArea[7u][13u] = 0x07u;  /* Page index */
    m_auStorArea[7u][14u] = 0x00u;  /* Page index */
    m_auStorArea[7u][15u] = 0x00u;  /* Page index */
    m_auStorArea[7u][16u] = 0x00u;  /* Page index */
    m_auStorArea[7u][17u] = 0x02u;  /* Page type */
    m_auStorArea[7u][18u] = 0x00u;  /* Page version */
    m_auStorArea[7u][19u] = 0x00u;  /* Page version */
    m_auStorArea[7u][20u] = 0x0eu;  /* Total page */
    m_auStorArea[7u][21u] = 0x00u;  /* Total page */
    m_auStorArea[7u][22u] = 0x00u;  /* Total page */
    m_auStorArea[7u][23u] = 0x00u;  /* Total page */
    m_auStorArea[7u][24u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][25u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][26u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][27u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][28u] = 0xF5u;  /* CRC */
    m_auStorArea[7u][29u] = 0x02u;  /* CRC */
    m_auStorArea[7u][30u] = 0x00u;  /* CRC */
    m_auStorArea[7u][31u] = 0x00u;  /* CRC */

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_CLBCKREADERR == eFSS_LOGC_FlushBuffIfNotEquals(&l_tCtx, 0u, e_eFSS_LOGC_PAGETYPE_LOG) )
    {
        if( ( 0u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 0u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 3u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxRead.eLastEr ) )
        {
            if( ( 0u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 64 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 64 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 64 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 64 -- FAIL \n");
    }

    /* Init */
    l_tStorSet.uPageVersion = 0u;

    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteErrAdapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, true ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 65 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 65 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 66 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 66 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[0u], 0, sizeof(m_auStorArea[0u]));
    (void)memset(m_auStorArea[7u], 0xFF, sizeof(m_auStorArea[7u]));

    /* Fill the two storage page */


    /* Setup storage area */
    (void)memset(m_auStorArea[0u], 0, sizeof(m_auStorArea[0u]));
    m_auStorArea[0u][8u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][9u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][10u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][11u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][12u] = 0x03u;  /* Page SUBTYPE */
    m_auStorArea[0u][13u] = 0x00u;  /* Page index */
    m_auStorArea[0u][14u] = 0x00u;  /* Page index */
    m_auStorArea[0u][15u] = 0x00u;  /* Page index */
    m_auStorArea[0u][16u] = 0x00u;  /* Page index */
    m_auStorArea[0u][17u] = 0x02u;  /* Page type */
    m_auStorArea[0u][18u] = 0x00u;  /* Page version */
    m_auStorArea[0u][19u] = 0x00u;  /* Page version */
    m_auStorArea[0u][20u] = 0x0Eu;  /* Total page */
    m_auStorArea[0u][21u] = 0x00u;  /* Total page */
    m_auStorArea[0u][22u] = 0x00u;  /* Total page */
    m_auStorArea[0u][23u] = 0x00u;  /* Total page */
    m_auStorArea[0u][24u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][25u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][26u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][27u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][28u] = 0xA6u;  /* CRC */
    m_auStorArea[0u][29u] = 0x02u;  /* CRC */
    m_auStorArea[0u][30u] = 0x00u;  /* CRC */
    m_auStorArea[0u][31u] = 0x00u;  /* CRC */

    (void)memset(m_auStorArea[1u], 0x10, sizeof(m_auStorArea[1u]));
    m_auStorArea[7u][8u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][9u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][10u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][11u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][12u] = 0x01u;  /* Page SUBTYPE */
    m_auStorArea[7u][13u] = 0x07u;  /* Page index */
    m_auStorArea[7u][14u] = 0x00u;  /* Page index */
    m_auStorArea[7u][15u] = 0x00u;  /* Page index */
    m_auStorArea[7u][16u] = 0x00u;  /* Page index */
    m_auStorArea[7u][17u] = 0x02u;  /* Page type */
    m_auStorArea[7u][18u] = 0x00u;  /* Page version */
    m_auStorArea[7u][19u] = 0x00u;  /* Page version */
    m_auStorArea[7u][20u] = 0x0eu;  /* Total page */
    m_auStorArea[7u][21u] = 0x00u;  /* Total page */
    m_auStorArea[7u][22u] = 0x00u;  /* Total page */
    m_auStorArea[7u][23u] = 0x00u;  /* Total page */
    m_auStorArea[7u][24u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][25u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][26u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][27u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][28u] = 0xF5u;  /* CRC */
    m_auStorArea[7u][29u] = 0x02u;  /* CRC */
    m_auStorArea[7u][30u] = 0x00u;  /* CRC */
    m_auStorArea[7u][31u] = 0x00u;  /* CRC */

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    memset(l_auStor, 0xFF, sizeof(l_auStor));
    l_auStor[8u] = 0x00;
    l_auStor[9u] = 0x00;
    l_auStor[10u] = 0x00;
    l_auStor[11u] = 0x00;

    if( e_eFSS_LOGC_RES_CLBCKWRITEERR == eFSS_LOGC_FlushBuffIfNotEquals(&l_tCtx, 0u, e_eFSS_LOGC_PAGETYPE_LOG) )
    {
        if( ( 2u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 3u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 1u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_LOGC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 3u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 67 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 67 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 67 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 67 -- FAIL \n");
    }

    /* Init */
    l_tStorSet.uPageVersion = 0u;

    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteErrAdapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, true ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 68 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 68 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 69 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 69 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[0u], 0, sizeof(m_auStorArea[0u]));
    (void)memset(m_auStorArea[7u], 0xFF, sizeof(m_auStorArea[7u]));

    /* Fill the two storage page */


    /* Setup storage area */
    (void)memset(m_auStorArea[0u], 0, sizeof(m_auStorArea[0u]));
    m_auStorArea[0u][8u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][9u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][10u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][11u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][12u] = 0x01u;  /* Page SUBTYPE */
    m_auStorArea[0u][13u] = 0x00u;  /* Page index */
    m_auStorArea[0u][14u] = 0x00u;  /* Page index */
    m_auStorArea[0u][15u] = 0x00u;  /* Page index */
    m_auStorArea[0u][16u] = 0x00u;  /* Page index */
    m_auStorArea[0u][17u] = 0x02u;  /* Page type */
    m_auStorArea[0u][18u] = 0x00u;  /* Page version */
    m_auStorArea[0u][19u] = 0x00u;  /* Page version */
    m_auStorArea[0u][20u] = 0x0Eu;  /* Total page */
    m_auStorArea[0u][21u] = 0x00u;  /* Total page */
    m_auStorArea[0u][22u] = 0x00u;  /* Total page */
    m_auStorArea[0u][23u] = 0x00u;  /* Total page */
    m_auStorArea[0u][24u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][25u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][26u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][27u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][28u] = 0xA4u;  /* CRC */
    m_auStorArea[0u][29u] = 0x02u;  /* CRC */
    m_auStorArea[0u][30u] = 0x00u;  /* CRC */
    m_auStorArea[0u][31u] = 0x00u;  /* CRC */

    (void)memset(m_auStorArea[1u], 0x10, sizeof(m_auStorArea[1u]));
    m_auStorArea[7u][8u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][9u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][10u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][11u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][12u] = 0x01u;  /* Page SUBTYPE */
    m_auStorArea[7u][13u] = 0x07u;  /* Page index */
    m_auStorArea[7u][14u] = 0x00u;  /* Page index */
    m_auStorArea[7u][15u] = 0x00u;  /* Page index */
    m_auStorArea[7u][16u] = 0x00u;  /* Page index */
    m_auStorArea[7u][17u] = 0x02u;  /* Page type */
    m_auStorArea[7u][18u] = 0x00u;  /* Page version */
    m_auStorArea[7u][19u] = 0x00u;  /* Page version */
    m_auStorArea[7u][20u] = 0x0eu;  /* Total page */
    m_auStorArea[7u][21u] = 0x00u;  /* Total page */
    m_auStorArea[7u][22u] = 0x00u;  /* Total page */
    m_auStorArea[7u][23u] = 0x00u;  /* Total page */
    m_auStorArea[7u][24u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][25u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][26u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][27u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][28u] = 0xF5u;  /* CRC */
    m_auStorArea[7u][29u] = 0x02u;  /* CRC */
    m_auStorArea[7u][30u] = 0x00u;  /* CRC */
    m_auStorArea[7u][31u] = 0x00u;  /* CRC */

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    memset(l_auStor, 0x00, sizeof(l_auStor));

    if( e_eFSS_LOGC_RES_CLBCKWRITEERR == eFSS_LOGC_FlushBuffIfNotEquals(&l_tCtx, 0u, e_eFSS_LOGC_PAGETYPE_LOG) )
    {
        if( ( 3u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 3u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 2u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_LOGC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 3u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 70 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 70 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 70 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 70 -- FAIL \n");
    }

    /* Init */
    l_tStorSet.uPageVersion = 0u;

    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadErrSecAdapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), false, true ) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 71 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 71 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 72 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 72 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[0u], 0, sizeof(m_auStorArea[0u]));
    (void)memset(m_auStorArea[7u], 0xFF, sizeof(m_auStorArea[7u]));

    /* Fill the two storage page */


    /* Setup storage area */
    (void)memset(m_auStorArea[0u], 0, sizeof(m_auStorArea[0u]));
    m_auStorArea[0u][8u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][9u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][10u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][11u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[0u][12u] = 0x01u;  /* Page SUBTYPE */
    m_auStorArea[0u][13u] = 0x00u;  /* Page index */
    m_auStorArea[0u][14u] = 0x00u;  /* Page index */
    m_auStorArea[0u][15u] = 0x00u;  /* Page index */
    m_auStorArea[0u][16u] = 0x00u;  /* Page index */
    m_auStorArea[0u][17u] = 0x02u;  /* Page type */
    m_auStorArea[0u][18u] = 0x00u;  /* Page version */
    m_auStorArea[0u][19u] = 0x00u;  /* Page version */
    m_auStorArea[0u][20u] = 0x0Eu;  /* Total page */
    m_auStorArea[0u][21u] = 0x00u;  /* Total page */
    m_auStorArea[0u][22u] = 0x00u;  /* Total page */
    m_auStorArea[0u][23u] = 0x00u;  /* Total page */
    m_auStorArea[0u][24u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][25u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][26u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][27u] = 0xA5u;  /* Magic number */
    m_auStorArea[0u][28u] = 0xA4u;  /* CRC */
    m_auStorArea[0u][29u] = 0x02u;  /* CRC */
    m_auStorArea[0u][30u] = 0x00u;  /* CRC */
    m_auStorArea[0u][31u] = 0x00u;  /* CRC */

    (void)memset(m_auStorArea[1u], 0x10, sizeof(m_auStorArea[1u]));
    m_auStorArea[7u][8u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][9u]  = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][10u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][11u] = 0x00u;  /* BYTE IN PAGE */
    m_auStorArea[7u][12u] = 0x01u;  /* Page SUBTYPE */
    m_auStorArea[7u][13u] = 0x07u;  /* Page index */
    m_auStorArea[7u][14u] = 0x00u;  /* Page index */
    m_auStorArea[7u][15u] = 0x00u;  /* Page index */
    m_auStorArea[7u][16u] = 0x00u;  /* Page index */
    m_auStorArea[7u][17u] = 0x02u;  /* Page type */
    m_auStorArea[7u][18u] = 0x00u;  /* Page version */
    m_auStorArea[7u][19u] = 0x00u;  /* Page version */
    m_auStorArea[7u][20u] = 0x0eu;  /* Total page */
    m_auStorArea[7u][21u] = 0x00u;  /* Total page */
    m_auStorArea[7u][22u] = 0x00u;  /* Total page */
    m_auStorArea[7u][23u] = 0x00u;  /* Total page */
    m_auStorArea[7u][24u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][25u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][26u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][27u] = 0xA5u;  /* Magic number */
    m_auStorArea[7u][28u] = 0xF5u;  /* CRC */
    m_auStorArea[7u][29u] = 0x02u;  /* CRC */
    m_auStorArea[7u][30u] = 0x00u;  /* CRC */
    m_auStorArea[7u][31u] = 0x00u;  /* CRC */

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_CLBCKREADERR == eFSS_LOGC_FlushBuffIfNotEquals(&l_tCtx, 0u, e_eFSS_LOGC_PAGETYPE_LOG) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 0u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_LOGC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 1u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_LOGC_RES_BADPOINTER == l_tCtxRead.eLastEr ) )
        {
            if( ( 0u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_LOGC_RES_OK == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 73 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_BadClBckNRetry 73 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_BadClBckNRetry 73 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_BadClBckNRetry 73 -- FAIL \n");
    }
}

static void eFSS_LOGCTST_ReadCacheTest(void)
{
    /* Local variable */
    t_eFSS_LOGC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_uStorType;
    uint8_t l_auStor[64u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_LOGC_StorBuf l_ltUseBuff;
    t_eFSS_LOGC_StorBuf l_ltUseBuff2;
    uint32_t l_uByteInPage;
    uint32_t l_uPageUsable;
    uint32_t l_uNewPIx;
    uint32_t l_uFillPIdx;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 14u;
    l_tStorSet.uPagesLen = 32u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;
    l_uStorType = 1u;
    l_uByteInPage = 0u;
    l_uPageUsable = 0u;
    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), true, false ) )
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 2  -- FAIL \n");
    }

    /* Setup storage area */
    l_ltUseBuff2.uBufL = l_ltUseBuff.uBufL  + 1u;
    l_ltUseBuff2.puBuf = &l_auStor[32];

    /* --------------------------------------------- All OK no backup generation */
    /* Setup storage area */
    (void)memset(m_auStorArea[12u], 0, sizeof(m_auStorArea[12u]));
    m_auStorArea[12u][0u]  = 0x00u; /* New index */
    m_auStorArea[12u][1u]  = 0x00u; /* New index */
    m_auStorArea[12u][2u]  = 0x00u; /* New index */
    m_auStorArea[12u][3u]  = 0x00u; /* New index */
    m_auStorArea[12u][4u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][5u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][6u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][7u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][8u]  = 0x08u; /* Byte In Page */
    m_auStorArea[12u][9u]  = 0x00u; /* Byte In Page */
    m_auStorArea[12u][10u] = 0x00u; /* Byte In Page */
    m_auStorArea[12u][11u] = 0x00u; /* Byte In Page */
    m_auStorArea[12u][12u] = 0x07u; /* Page SUBTYPE */
    m_auStorArea[12u][13u] = 0x0Cu; /* Page index */
    m_auStorArea[12u][14u] = 0x00u; /* Page index */
    m_auStorArea[12u][15u] = 0x00u; /* Page index */
    m_auStorArea[12u][16u] = 0x00u; /* Page index */
    m_auStorArea[12u][17u] = 0x02u; /* Page type */
    m_auStorArea[12u][18u] = 0x01u; /* Page version */
    m_auStorArea[12u][19u] = 0x00u; /* Page version */
    m_auStorArea[12u][20u] = 0x0Eu; /* Total page */
    m_auStorArea[12u][21u] = 0x00u; /* Total page */
    m_auStorArea[12u][22u] = 0x00u; /* Total page */
    m_auStorArea[12u][23u] = 0x00u; /* Total page */
    m_auStorArea[12u][24u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][25u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][26u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][27u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][28u] = 0xBFu; /* CRC */
    m_auStorArea[12u][29u] = 0x02u; /* CRC */
    m_auStorArea[12u][30u] = 0x00u; /* CRC */
    m_auStorArea[12u][31u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[13u], 0, sizeof(m_auStorArea[13u]));
    m_auStorArea[13u][0u]  = 0x00u; /* New index */
    m_auStorArea[13u][1u]  = 0x00u; /* New index */
    m_auStorArea[13u][2u]  = 0x00u; /* New index */
    m_auStorArea[13u][3u]  = 0x00u; /* New index */
    m_auStorArea[13u][4u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][5u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][6u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][7u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][8u]  = 0x08u; /* Byte In Page */
    m_auStorArea[13u][9u]  = 0x00u; /* Byte In Page */
    m_auStorArea[13u][10u] = 0x00u; /* Byte In Page */
    m_auStorArea[13u][11u] = 0x00u; /* Byte In Page */
    m_auStorArea[13u][12u] = 0x08u; /* Page SUBTYPE */
    m_auStorArea[13u][13u] = 0x0Du; /* Page index */
    m_auStorArea[13u][14u] = 0x00u; /* Page index */
    m_auStorArea[13u][15u] = 0x00u; /* Page index */
    m_auStorArea[13u][16u] = 0x00u; /* Page index */
    m_auStorArea[13u][17u] = 0x02u; /* Page type */
    m_auStorArea[13u][18u] = 0x01u; /* Page version */
    m_auStorArea[13u][19u] = 0x00u; /* Page version */
    m_auStorArea[13u][20u] = 0x0Eu; /* Total page */
    m_auStorArea[13u][21u] = 0x00u; /* Total page */
    m_auStorArea[13u][22u] = 0x00u; /* Total page */
    m_auStorArea[13u][23u] = 0x00u; /* Total page */
    m_auStorArea[13u][24u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][25u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][26u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][27u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][28u] = 0xC1u; /* CRC */
    m_auStorArea[13u][29u] = 0x02u; /* CRC */
    m_auStorArea[13u][30u] = 0x00u; /* CRC */
    m_auStorArea[13u][31u] = 0x00u; /* CRC */

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u; /* New index */
    l_ltUseBuff.puBuf[1u] = 0x02u; /* New index */
    l_ltUseBuff.puBuf[2u] = 0x03u; /* New index */
    l_ltUseBuff.puBuf[3u] = 0x04u; /* New index */
    l_ltUseBuff.puBuf[4u] = 0x05u; /* Filled page */
    l_ltUseBuff.puBuf[5u] = 0x06u; /* Filled page */
    l_ltUseBuff.puBuf[6u] = 0x07u; /* Filled page */
    l_ltUseBuff.puBuf[7u] = 0x08u; /* Filled page */

    l_ltUseBuff2.puBuf[0u] = 0x11u; /* New index */
    l_ltUseBuff2.puBuf[1u] = 0x12u; /* New index */
    l_ltUseBuff2.puBuf[2u] = 0x13u; /* New index */
    l_ltUseBuff2.puBuf[3u] = 0x14u; /* New index */
    l_ltUseBuff2.puBuf[4u] = 0x15u; /* Filled page */
    l_ltUseBuff2.puBuf[5u] = 0x16u; /* Filled page */
    l_ltUseBuff2.puBuf[6u] = 0x17u; /* Filled page */
    l_ltUseBuff2.puBuf[7u] = 0x18u; /* Filled page */

    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx) )
    {
        if( ( 0x00u == l_uNewPIx ) && ( 0x00u == l_uFillPIdx ) )
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 3  -- FAIL \n");
    }

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u; /* New index */
    l_ltUseBuff.puBuf[1u] = 0x02u; /* New index */
    l_ltUseBuff.puBuf[2u] = 0x03u; /* New index */
    l_ltUseBuff.puBuf[3u] = 0x04u; /* New index */
    l_ltUseBuff.puBuf[4u] = 0x05u; /* Filled page */
    l_ltUseBuff.puBuf[5u] = 0x06u; /* Filled page */
    l_ltUseBuff.puBuf[6u] = 0x07u; /* Filled page */
    l_ltUseBuff.puBuf[7u] = 0x08u; /* Filled page */

    l_ltUseBuff2.puBuf[0u] = 0x11u; /* New index */
    l_ltUseBuff2.puBuf[1u] = 0x12u; /* New index */
    l_ltUseBuff2.puBuf[2u] = 0x13u; /* New index */
    l_ltUseBuff2.puBuf[3u] = 0x14u; /* New index */
    l_ltUseBuff2.puBuf[4u] = 0x15u; /* Filled page */
    l_ltUseBuff2.puBuf[5u] = 0x16u; /* Filled page */
    l_ltUseBuff2.puBuf[6u] = 0x17u; /* Filled page */
    l_ltUseBuff2.puBuf[7u] = 0x18u; /* Filled page */

    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx) )
    {
        if( ( 0x00u == l_uNewPIx ) && ( 0x00u == l_uFillPIdx ) )
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 4  -- FAIL \n");
    }

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), true, false ) )
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 5  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 6  -- FAIL \n");
    }

    /* Setup storage area */
    l_ltUseBuff2.uBufL = l_ltUseBuff.uBufL  + 1u;
    l_ltUseBuff2.puBuf = &l_auStor[32];

    /* --------------------------------------------- All OK no backup generation */
    /* Setup storage area */
    (void)memset(m_auStorArea[12u], 0, sizeof(m_auStorArea[12u]));
    m_auStorArea[12u][0u]  = 0x00u; /* New index */
    m_auStorArea[12u][1u]  = 0x00u; /* New index */
    m_auStorArea[12u][2u]  = 0x00u; /* New index */
    m_auStorArea[12u][3u]  = 0x00u; /* New index */
    m_auStorArea[12u][4u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][5u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][6u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][7u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][8u]  = 0x08u; /* Byte In Page */
    m_auStorArea[12u][9u]  = 0x00u; /* Byte In Page */
    m_auStorArea[12u][10u] = 0x00u; /* Byte In Page */
    m_auStorArea[12u][11u] = 0x00u; /* Byte In Page */
    m_auStorArea[12u][12u] = 0x07u; /* Page SUBTYPE */
    m_auStorArea[12u][13u] = 0x0Cu; /* Page index */
    m_auStorArea[12u][14u] = 0x00u; /* Page index */
    m_auStorArea[12u][15u] = 0x00u; /* Page index */
    m_auStorArea[12u][16u] = 0x00u; /* Page index */
    m_auStorArea[12u][17u] = 0x02u; /* Page type */
    m_auStorArea[12u][18u] = 0x01u; /* Page version */
    m_auStorArea[12u][19u] = 0x00u; /* Page version */
    m_auStorArea[12u][20u] = 0x0Eu; /* Total page */
    m_auStorArea[12u][21u] = 0x00u; /* Total page */
    m_auStorArea[12u][22u] = 0x00u; /* Total page */
    m_auStorArea[12u][23u] = 0x00u; /* Total page */
    m_auStorArea[12u][24u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][25u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][26u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][27u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][28u] = 0xBFu; /* CRC */
    m_auStorArea[12u][29u] = 0x02u; /* CRC */
    m_auStorArea[12u][30u] = 0x00u; /* CRC */
    m_auStorArea[12u][31u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[13u], 0, sizeof(m_auStorArea[13u]));
    m_auStorArea[13u][0u]  = 0x00u; /* New index */
    m_auStorArea[13u][1u]  = 0x00u; /* New index */
    m_auStorArea[13u][2u]  = 0x00u; /* New index */
    m_auStorArea[13u][3u]  = 0x00u; /* New index */
    m_auStorArea[13u][4u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][5u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][6u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][7u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][8u]  = 0x08u; /* Byte In Page */
    m_auStorArea[13u][9u]  = 0x00u; /* Byte In Page */
    m_auStorArea[13u][10u] = 0x00u; /* Byte In Page */
    m_auStorArea[13u][11u] = 0x00u; /* Byte In Page */
    m_auStorArea[13u][12u] = 0x08u; /* Page SUBTYPE */
    m_auStorArea[13u][13u] = 0x0Du; /* Page index */
    m_auStorArea[13u][14u] = 0x00u; /* Page index */
    m_auStorArea[13u][15u] = 0x00u; /* Page index */
    m_auStorArea[13u][16u] = 0x00u; /* Page index */
    m_auStorArea[13u][17u] = 0x02u; /* Page type */
    m_auStorArea[13u][18u] = 0x01u; /* Page version */
    m_auStorArea[13u][19u] = 0x00u; /* Page version */
    m_auStorArea[13u][20u] = 0x0Eu; /* Total page */
    m_auStorArea[13u][21u] = 0x00u; /* Total page */
    m_auStorArea[13u][22u] = 0x00u; /* Total page */
    m_auStorArea[13u][23u] = 0x00u; /* Total page */
    m_auStorArea[13u][24u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][25u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][26u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][27u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][28u] = 0xC3u; /* CRC */
    m_auStorArea[13u][29u] = 0x02u; /* CRC */
    m_auStorArea[13u][30u] = 0x00u; /* CRC */
    m_auStorArea[13u][31u] = 0x00u; /* CRC */

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u; /* New index */
    l_ltUseBuff.puBuf[1u] = 0x02u; /* New index */
    l_ltUseBuff.puBuf[2u] = 0x03u; /* New index */
    l_ltUseBuff.puBuf[3u] = 0x04u; /* New index */
    l_ltUseBuff.puBuf[4u] = 0x05u; /* Filled page */
    l_ltUseBuff.puBuf[5u] = 0x06u; /* Filled page */
    l_ltUseBuff.puBuf[6u] = 0x07u; /* Filled page */
    l_ltUseBuff.puBuf[7u] = 0x08u; /* Filled page */

    l_ltUseBuff2.puBuf[0u] = 0x11u; /* New index */
    l_ltUseBuff2.puBuf[1u] = 0x12u; /* New index */
    l_ltUseBuff2.puBuf[2u] = 0x13u; /* New index */
    l_ltUseBuff2.puBuf[3u] = 0x14u; /* New index */
    l_ltUseBuff2.puBuf[4u] = 0x15u; /* Filled page */
    l_ltUseBuff2.puBuf[5u] = 0x16u; /* Filled page */
    l_ltUseBuff2.puBuf[6u] = 0x17u; /* Filled page */
    l_ltUseBuff2.puBuf[7u] = 0x18u; /* Filled page */

    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;
    if( e_eFSS_LOGC_RES_OK_BKP_RCVRD == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx) )
    {
        if( ( 0x00u == l_uNewPIx ) && ( 0x00u == l_uFillPIdx ) )
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 7  -- FAIL \n");
    }

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u; /* New index */
    l_ltUseBuff.puBuf[1u] = 0x02u; /* New index */
    l_ltUseBuff.puBuf[2u] = 0x03u; /* New index */
    l_ltUseBuff.puBuf[3u] = 0x04u; /* New index */
    l_ltUseBuff.puBuf[4u] = 0x05u; /* Filled page */
    l_ltUseBuff.puBuf[5u] = 0x06u; /* Filled page */
    l_ltUseBuff.puBuf[6u] = 0x07u; /* Filled page */
    l_ltUseBuff.puBuf[7u] = 0x08u; /* Filled page */

    l_ltUseBuff2.puBuf[0u] = 0x11u; /* New index */
    l_ltUseBuff2.puBuf[1u] = 0x12u; /* New index */
    l_ltUseBuff2.puBuf[2u] = 0x13u; /* New index */
    l_ltUseBuff2.puBuf[3u] = 0x14u; /* New index */
    l_ltUseBuff2.puBuf[4u] = 0x15u; /* Filled page */
    l_ltUseBuff2.puBuf[5u] = 0x16u; /* Filled page */
    l_ltUseBuff2.puBuf[6u] = 0x17u; /* Filled page */
    l_ltUseBuff2.puBuf[7u] = 0x18u; /* Filled page */

    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx) )
    {
        if( ( 0x00u == l_uNewPIx ) && ( 0x00u == l_uFillPIdx ) )
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 8  -- FAIL \n");
    }

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), true, false ) )
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 9  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 10 -- FAIL \n");
    }

    /* Setup storage area */
    l_ltUseBuff2.uBufL = l_ltUseBuff.uBufL  + 1u;
    l_ltUseBuff2.puBuf = &l_auStor[32];

    /* --------------------------------------------- All OK no backup generation */
    /* Setup storage area */
    (void)memset(m_auStorArea[12u], 0, sizeof(m_auStorArea[12u]));
    m_auStorArea[12u][0u]  = 0x00u; /* New index */
    m_auStorArea[12u][1u]  = 0x00u; /* New index */
    m_auStorArea[12u][2u]  = 0x00u; /* New index */
    m_auStorArea[12u][3u]  = 0x00u; /* New index */
    m_auStorArea[12u][4u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][5u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][6u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][7u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][8u]  = 0x08u; /* Byte In Page */
    m_auStorArea[12u][9u]  = 0x00u; /* Byte In Page */
    m_auStorArea[12u][10u] = 0x00u; /* Byte In Page */
    m_auStorArea[12u][11u] = 0x00u; /* Byte In Page */
    m_auStorArea[12u][12u] = 0x07u; /* Page SUBTYPE */
    m_auStorArea[12u][13u] = 0x0Cu; /* Page index */
    m_auStorArea[12u][14u] = 0x00u; /* Page index */
    m_auStorArea[12u][15u] = 0x00u; /* Page index */
    m_auStorArea[12u][16u] = 0x00u; /* Page index */
    m_auStorArea[12u][17u] = 0x02u; /* Page type */
    m_auStorArea[12u][18u] = 0x01u; /* Page version */
    m_auStorArea[12u][19u] = 0x00u; /* Page version */
    m_auStorArea[12u][20u] = 0x0Eu; /* Total page */
    m_auStorArea[12u][21u] = 0x00u; /* Total page */
    m_auStorArea[12u][22u] = 0x00u; /* Total page */
    m_auStorArea[12u][23u] = 0x00u; /* Total page */
    m_auStorArea[12u][24u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][25u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][26u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][27u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][28u] = 0xBCu; /* CRC */
    m_auStorArea[12u][29u] = 0x02u; /* CRC */
    m_auStorArea[12u][30u] = 0x00u; /* CRC */
    m_auStorArea[12u][31u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[13u], 0, sizeof(m_auStorArea[13u]));
    m_auStorArea[13u][0u]  = 0x00u; /* New index */
    m_auStorArea[13u][1u]  = 0x00u; /* New index */
    m_auStorArea[13u][2u]  = 0x00u; /* New index */
    m_auStorArea[13u][3u]  = 0x00u; /* New index */
    m_auStorArea[13u][4u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][5u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][6u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][7u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][8u]  = 0x08u; /* Byte In Page */
    m_auStorArea[13u][9u]  = 0x00u; /* Byte In Page */
    m_auStorArea[13u][10u] = 0x00u; /* Byte In Page */
    m_auStorArea[13u][11u] = 0x00u; /* Byte In Page */
    m_auStorArea[13u][12u] = 0x08u; /* Page SUBTYPE */
    m_auStorArea[13u][13u] = 0x0Du; /* Page index */
    m_auStorArea[13u][14u] = 0x00u; /* Page index */
    m_auStorArea[13u][15u] = 0x00u; /* Page index */
    m_auStorArea[13u][16u] = 0x00u; /* Page index */
    m_auStorArea[13u][17u] = 0x02u; /* Page type */
    m_auStorArea[13u][18u] = 0x01u; /* Page version */
    m_auStorArea[13u][19u] = 0x00u; /* Page version */
    m_auStorArea[13u][20u] = 0x0Eu; /* Total page */
    m_auStorArea[13u][21u] = 0x00u; /* Total page */
    m_auStorArea[13u][22u] = 0x00u; /* Total page */
    m_auStorArea[13u][23u] = 0x00u; /* Total page */
    m_auStorArea[13u][24u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][25u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][26u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][27u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][28u] = 0xC1u; /* CRC */
    m_auStorArea[13u][29u] = 0x02u; /* CRC */
    m_auStorArea[13u][30u] = 0x00u; /* CRC */
    m_auStorArea[13u][31u] = 0x00u; /* CRC */

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u; /* New index */
    l_ltUseBuff.puBuf[1u] = 0x02u; /* New index */
    l_ltUseBuff.puBuf[2u] = 0x03u; /* New index */
    l_ltUseBuff.puBuf[3u] = 0x04u; /* New index */
    l_ltUseBuff.puBuf[4u] = 0x05u; /* Filled page */
    l_ltUseBuff.puBuf[5u] = 0x06u; /* Filled page */
    l_ltUseBuff.puBuf[6u] = 0x07u; /* Filled page */
    l_ltUseBuff.puBuf[7u] = 0x08u; /* Filled page */

    l_ltUseBuff2.puBuf[0u] = 0x11u; /* New index */
    l_ltUseBuff2.puBuf[1u] = 0x12u; /* New index */
    l_ltUseBuff2.puBuf[2u] = 0x13u; /* New index */
    l_ltUseBuff2.puBuf[3u] = 0x14u; /* New index */
    l_ltUseBuff2.puBuf[4u] = 0x15u; /* Filled page */
    l_ltUseBuff2.puBuf[5u] = 0x16u; /* Filled page */
    l_ltUseBuff2.puBuf[6u] = 0x17u; /* Filled page */
    l_ltUseBuff2.puBuf[7u] = 0x18u; /* Filled page */

    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;

    if( e_eFSS_LOGC_RES_OK_BKP_RCVRD == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx) )
    {
        if( ( 0x00u == l_uNewPIx ) && ( 0x00u == l_uFillPIdx ) )
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 11 -- FAIL \n");
    }

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u; /* New index */
    l_ltUseBuff.puBuf[1u] = 0x02u; /* New index */
    l_ltUseBuff.puBuf[2u] = 0x03u; /* New index */
    l_ltUseBuff.puBuf[3u] = 0x04u; /* New index */
    l_ltUseBuff.puBuf[4u] = 0x05u; /* Filled page */
    l_ltUseBuff.puBuf[5u] = 0x06u; /* Filled page */
    l_ltUseBuff.puBuf[6u] = 0x07u; /* Filled page */
    l_ltUseBuff.puBuf[7u] = 0x08u; /* Filled page */

    l_ltUseBuff2.puBuf[0u] = 0x11u; /* New index */
    l_ltUseBuff2.puBuf[1u] = 0x12u; /* New index */
    l_ltUseBuff2.puBuf[2u] = 0x13u; /* New index */
    l_ltUseBuff2.puBuf[3u] = 0x14u; /* New index */
    l_ltUseBuff2.puBuf[4u] = 0x15u; /* Filled page */
    l_ltUseBuff2.puBuf[5u] = 0x16u; /* Filled page */
    l_ltUseBuff2.puBuf[6u] = 0x17u; /* Filled page */
    l_ltUseBuff2.puBuf[7u] = 0x18u; /* Filled page */

    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx) )
    {
        if( ( 0x00u == l_uNewPIx ) && ( 0x00u == l_uFillPIdx ) )
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 12 -- FAIL \n");
    }

    /* --------------------------------------------- All OK no backup generation */
    /* Setup storage area */
    (void)memset(m_auStorArea[12u], 0, sizeof(m_auStorArea[12u]));
    m_auStorArea[12u][0u]  = 0x00u; /* New index */
    m_auStorArea[12u][1u]  = 0x00u; /* New index */
    m_auStorArea[12u][2u]  = 0x00u; /* New index */
    m_auStorArea[12u][3u]  = 0x00u; /* New index */
    m_auStorArea[12u][4u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][5u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][6u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][7u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][8u]  = 0x09u; /* Byte In Page */
    m_auStorArea[12u][9u]  = 0x00u; /* Byte In Page */
    m_auStorArea[12u][10u] = 0x00u; /* Byte In Page */
    m_auStorArea[12u][11u] = 0x00u; /* Byte In Page */
    m_auStorArea[12u][12u] = 0x07u; /* Page SUBTYPE */
    m_auStorArea[12u][13u] = 0x0Cu; /* Page index */
    m_auStorArea[12u][14u] = 0x00u; /* Page index */
    m_auStorArea[12u][15u] = 0x00u; /* Page index */
    m_auStorArea[12u][16u] = 0x00u; /* Page index */
    m_auStorArea[12u][17u] = 0x02u; /* Page type */
    m_auStorArea[12u][18u] = 0x01u; /* Page version */
    m_auStorArea[12u][19u] = 0x00u; /* Page version */
    m_auStorArea[12u][20u] = 0x0Eu; /* Total page */
    m_auStorArea[12u][21u] = 0x00u; /* Total page */
    m_auStorArea[12u][22u] = 0x00u; /* Total page */
    m_auStorArea[12u][23u] = 0x00u; /* Total page */
    m_auStorArea[12u][24u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][25u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][26u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][27u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][28u] = 0xC0u; /* CRC */
    m_auStorArea[12u][29u] = 0x02u; /* CRC */
    m_auStorArea[12u][30u] = 0x00u; /* CRC */
    m_auStorArea[12u][31u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[13u], 0, sizeof(m_auStorArea[13u]));
    m_auStorArea[13u][0u]  = 0x00u; /* New index */
    m_auStorArea[13u][1u]  = 0x00u; /* New index */
    m_auStorArea[13u][2u]  = 0x00u; /* New index */
    m_auStorArea[13u][3u]  = 0x00u; /* New index */
    m_auStorArea[13u][4u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][5u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][6u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][7u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][8u]  = 0x08u; /* Byte In Page */
    m_auStorArea[13u][9u]  = 0x00u; /* Byte In Page */
    m_auStorArea[13u][10u] = 0x00u; /* Byte In Page */
    m_auStorArea[13u][11u] = 0x00u; /* Byte In Page */
    m_auStorArea[13u][12u] = 0x08u; /* Page SUBTYPE */
    m_auStorArea[13u][13u] = 0x0Du; /* Page index */
    m_auStorArea[13u][14u] = 0x00u; /* Page index */
    m_auStorArea[13u][15u] = 0x00u; /* Page index */
    m_auStorArea[13u][16u] = 0x00u; /* Page index */
    m_auStorArea[13u][17u] = 0x02u; /* Page type */
    m_auStorArea[13u][18u] = 0x01u; /* Page version */
    m_auStorArea[13u][19u] = 0x00u; /* Page version */
    m_auStorArea[13u][20u] = 0x0Eu; /* Total page */
    m_auStorArea[13u][21u] = 0x00u; /* Total page */
    m_auStorArea[13u][22u] = 0x00u; /* Total page */
    m_auStorArea[13u][23u] = 0x00u; /* Total page */
    m_auStorArea[13u][24u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][25u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][26u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][27u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][28u] = 0xC1u; /* CRC */
    m_auStorArea[13u][29u] = 0x02u; /* CRC */
    m_auStorArea[13u][30u] = 0x00u; /* CRC */
    m_auStorArea[13u][31u] = 0x00u; /* CRC */

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u; /* New index */
    l_ltUseBuff.puBuf[1u] = 0x02u; /* New index */
    l_ltUseBuff.puBuf[2u] = 0x03u; /* New index */
    l_ltUseBuff.puBuf[3u] = 0x04u; /* New index */
    l_ltUseBuff.puBuf[4u] = 0x05u; /* Filled page */
    l_ltUseBuff.puBuf[5u] = 0x06u; /* Filled page */
    l_ltUseBuff.puBuf[6u] = 0x07u; /* Filled page */
    l_ltUseBuff.puBuf[7u] = 0x08u; /* Filled page */

    l_ltUseBuff2.puBuf[0u] = 0x11u; /* New index */
    l_ltUseBuff2.puBuf[1u] = 0x12u; /* New index */
    l_ltUseBuff2.puBuf[2u] = 0x13u; /* New index */
    l_ltUseBuff2.puBuf[3u] = 0x14u; /* New index */
    l_ltUseBuff2.puBuf[4u] = 0x15u; /* Filled page */
    l_ltUseBuff2.puBuf[5u] = 0x16u; /* Filled page */
    l_ltUseBuff2.puBuf[6u] = 0x17u; /* Filled page */
    l_ltUseBuff2.puBuf[7u] = 0x18u; /* Filled page */

    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;
    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx) )
    {
        if( ( 0x00u == l_uNewPIx ) && ( 0x00u == l_uFillPIdx ) )
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 13 -- FAIL \n");
    }

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u; /* New index */
    l_ltUseBuff.puBuf[1u] = 0x02u; /* New index */
    l_ltUseBuff.puBuf[2u] = 0x03u; /* New index */
    l_ltUseBuff.puBuf[3u] = 0x04u; /* New index */
    l_ltUseBuff.puBuf[4u] = 0x05u; /* Filled page */
    l_ltUseBuff.puBuf[5u] = 0x06u; /* Filled page */
    l_ltUseBuff.puBuf[6u] = 0x07u; /* Filled page */
    l_ltUseBuff.puBuf[7u] = 0x08u; /* Filled page */

    l_ltUseBuff2.puBuf[0u] = 0x11u; /* New index */
    l_ltUseBuff2.puBuf[1u] = 0x12u; /* New index */
    l_ltUseBuff2.puBuf[2u] = 0x13u; /* New index */
    l_ltUseBuff2.puBuf[3u] = 0x14u; /* New index */
    l_ltUseBuff2.puBuf[4u] = 0x15u; /* Filled page */
    l_ltUseBuff2.puBuf[5u] = 0x16u; /* Filled page */
    l_ltUseBuff2.puBuf[6u] = 0x17u; /* Filled page */
    l_ltUseBuff2.puBuf[7u] = 0x18u; /* Filled page */

    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;
    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx) )
    {
        if( ( 0x00u == l_uNewPIx ) && ( 0x00u == l_uFillPIdx ) )
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 14 -- FAIL \n");
    }

    /* --------------------------------------------- All OK no backup generation */
    /* Setup storage area */
    (void)memset(m_auStorArea[12u], 0, sizeof(m_auStorArea[12u]));
    m_auStorArea[12u][0u]  = 0x00u; /* New index */
    m_auStorArea[12u][1u]  = 0x00u; /* New index */
    m_auStorArea[12u][2u]  = 0x00u; /* New index */
    m_auStorArea[12u][3u]  = 0x00u; /* New index */
    m_auStorArea[12u][4u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][5u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][6u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][7u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][8u]  = 0x07u; /* Byte In Page */
    m_auStorArea[12u][9u]  = 0x00u; /* Byte In Page */
    m_auStorArea[12u][10u] = 0x00u; /* Byte In Page */
    m_auStorArea[12u][11u] = 0x00u; /* Byte In Page */
    m_auStorArea[12u][12u] = 0x07u; /* Page SUBTYPE */
    m_auStorArea[12u][13u] = 0x0Cu; /* Page index */
    m_auStorArea[12u][14u] = 0x00u; /* Page index */
    m_auStorArea[12u][15u] = 0x00u; /* Page index */
    m_auStorArea[12u][16u] = 0x00u; /* Page index */
    m_auStorArea[12u][17u] = 0x02u; /* Page type */
    m_auStorArea[12u][18u] = 0x01u; /* Page version */
    m_auStorArea[12u][19u] = 0x00u; /* Page version */
    m_auStorArea[12u][20u] = 0x0Eu; /* Total page */
    m_auStorArea[12u][21u] = 0x00u; /* Total page */
    m_auStorArea[12u][22u] = 0x00u; /* Total page */
    m_auStorArea[12u][23u] = 0x00u; /* Total page */
    m_auStorArea[12u][24u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][25u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][26u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][27u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][28u] = 0xBEu; /* CRC */
    m_auStorArea[12u][29u] = 0x02u; /* CRC */
    m_auStorArea[12u][30u] = 0x00u; /* CRC */
    m_auStorArea[12u][31u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[13u], 0, sizeof(m_auStorArea[13u]));
    m_auStorArea[13u][0u]  = 0x00u; /* New index */
    m_auStorArea[13u][1u]  = 0x00u; /* New index */
    m_auStorArea[13u][2u]  = 0x00u; /* New index */
    m_auStorArea[13u][3u]  = 0x00u; /* New index */
    m_auStorArea[13u][4u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][5u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][6u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][7u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][8u]  = 0x08u; /* Byte In Page */
    m_auStorArea[13u][9u]  = 0x00u; /* Byte In Page */
    m_auStorArea[13u][10u] = 0x00u; /* Byte In Page */
    m_auStorArea[13u][11u] = 0x00u; /* Byte In Page */
    m_auStorArea[13u][12u] = 0x08u; /* Page SUBTYPE */
    m_auStorArea[13u][13u] = 0x0Du; /* Page index */
    m_auStorArea[13u][14u] = 0x00u; /* Page index */
    m_auStorArea[13u][15u] = 0x00u; /* Page index */
    m_auStorArea[13u][16u] = 0x00u; /* Page index */
    m_auStorArea[13u][17u] = 0x02u; /* Page type */
    m_auStorArea[13u][18u] = 0x01u; /* Page version */
    m_auStorArea[13u][19u] = 0x00u; /* Page version */
    m_auStorArea[13u][20u] = 0x0Eu; /* Total page */
    m_auStorArea[13u][21u] = 0x00u; /* Total page */
    m_auStorArea[13u][22u] = 0x00u; /* Total page */
    m_auStorArea[13u][23u] = 0x00u; /* Total page */
    m_auStorArea[13u][24u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][25u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][26u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][27u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][28u] = 0xC1u; /* CRC */
    m_auStorArea[13u][29u] = 0x02u; /* CRC */
    m_auStorArea[13u][30u] = 0x00u; /* CRC */
    m_auStorArea[13u][31u] = 0x00u; /* CRC */

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u; /* New index */
    l_ltUseBuff.puBuf[1u] = 0x02u; /* New index */
    l_ltUseBuff.puBuf[2u] = 0x03u; /* New index */
    l_ltUseBuff.puBuf[3u] = 0x04u; /* New index */
    l_ltUseBuff.puBuf[4u] = 0x05u; /* Filled page */
    l_ltUseBuff.puBuf[5u] = 0x06u; /* Filled page */
    l_ltUseBuff.puBuf[6u] = 0x07u; /* Filled page */
    l_ltUseBuff.puBuf[7u] = 0x08u; /* Filled page */

    l_ltUseBuff2.puBuf[0u] = 0x11u; /* New index */
    l_ltUseBuff2.puBuf[1u] = 0x12u; /* New index */
    l_ltUseBuff2.puBuf[2u] = 0x13u; /* New index */
    l_ltUseBuff2.puBuf[3u] = 0x14u; /* New index */
    l_ltUseBuff2.puBuf[4u] = 0x15u; /* Filled page */
    l_ltUseBuff2.puBuf[5u] = 0x16u; /* Filled page */
    l_ltUseBuff2.puBuf[6u] = 0x17u; /* Filled page */
    l_ltUseBuff2.puBuf[7u] = 0x18u; /* Filled page */

    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;
    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx) )
    {
        if( ( 0x00u == l_uNewPIx ) && ( 0x00u == l_uFillPIdx ) )
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 15 -- FAIL \n");
    }

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u; /* New index */
    l_ltUseBuff.puBuf[1u] = 0x02u; /* New index */
    l_ltUseBuff.puBuf[2u] = 0x03u; /* New index */
    l_ltUseBuff.puBuf[3u] = 0x04u; /* New index */
    l_ltUseBuff.puBuf[4u] = 0x05u; /* Filled page */
    l_ltUseBuff.puBuf[5u] = 0x06u; /* Filled page */
    l_ltUseBuff.puBuf[6u] = 0x07u; /* Filled page */
    l_ltUseBuff.puBuf[7u] = 0x08u; /* Filled page */

    l_ltUseBuff2.puBuf[0u] = 0x11u; /* New index */
    l_ltUseBuff2.puBuf[1u] = 0x12u; /* New index */
    l_ltUseBuff2.puBuf[2u] = 0x13u; /* New index */
    l_ltUseBuff2.puBuf[3u] = 0x14u; /* New index */
    l_ltUseBuff2.puBuf[4u] = 0x15u; /* Filled page */
    l_ltUseBuff2.puBuf[5u] = 0x16u; /* Filled page */
    l_ltUseBuff2.puBuf[6u] = 0x17u; /* Filled page */
    l_ltUseBuff2.puBuf[7u] = 0x18u; /* Filled page */

    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;
    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx) )
    {
        if( ( 0x00u == l_uNewPIx ) && ( 0x00u == l_uFillPIdx ) )
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 16 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 16 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 16 -- FAIL \n");
    }

    /* --------------------------------------------- All OK no backup generation */
    /* Setup storage area */
    (void)memset(m_auStorArea[12u], 0, sizeof(m_auStorArea[12u]));
    m_auStorArea[12u][0u]  = 0x0Cu; /* New index */
    m_auStorArea[12u][1u]  = 0x00u; /* New index */
    m_auStorArea[12u][2u]  = 0x00u; /* New index */
    m_auStorArea[12u][3u]  = 0x00u; /* New index */
    m_auStorArea[12u][4u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][5u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][6u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][7u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][8u]  = 0x08u; /* Byte In Page */
    m_auStorArea[12u][9u]  = 0x00u; /* Byte In Page */
    m_auStorArea[12u][10u] = 0x00u; /* Byte In Page */
    m_auStorArea[12u][11u] = 0x00u; /* Byte In Page */
    m_auStorArea[12u][12u] = 0x07u; /* Page SUBTYPE */
    m_auStorArea[12u][13u] = 0x0Cu; /* Page index */
    m_auStorArea[12u][14u] = 0x00u; /* Page index */
    m_auStorArea[12u][15u] = 0x00u; /* Page index */
    m_auStorArea[12u][16u] = 0x00u; /* Page index */
    m_auStorArea[12u][17u] = 0x02u; /* Page type */
    m_auStorArea[12u][18u] = 0x01u; /* Page version */
    m_auStorArea[12u][19u] = 0x00u; /* Page version */
    m_auStorArea[12u][20u] = 0x0Eu; /* Total page */
    m_auStorArea[12u][21u] = 0x00u; /* Total page */
    m_auStorArea[12u][22u] = 0x00u; /* Total page */
    m_auStorArea[12u][23u] = 0x00u; /* Total page */
    m_auStorArea[12u][24u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][25u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][26u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][27u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][28u] = 0xCBu; /* CRC */
    m_auStorArea[12u][29u] = 0x02u; /* CRC */
    m_auStorArea[12u][30u] = 0x00u; /* CRC */
    m_auStorArea[12u][31u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[13u], 0, sizeof(m_auStorArea[13u]));
    m_auStorArea[13u][0u]  = 0x0Cu; /* New index */
    m_auStorArea[13u][1u]  = 0x00u; /* New index */
    m_auStorArea[13u][2u]  = 0x00u; /* New index */
    m_auStorArea[13u][3u]  = 0x00u; /* New index */
    m_auStorArea[13u][4u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][5u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][6u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][7u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][8u]  = 0x08u; /* Byte In Page */
    m_auStorArea[13u][9u]  = 0x00u; /* Byte In Page */
    m_auStorArea[13u][10u] = 0x00u; /* Byte In Page */
    m_auStorArea[13u][11u] = 0x00u; /* Byte In Page */
    m_auStorArea[13u][12u] = 0x08u; /* Page SUBTYPE */
    m_auStorArea[13u][13u] = 0x0Du; /* Page index */
    m_auStorArea[13u][14u] = 0x00u; /* Page index */
    m_auStorArea[13u][15u] = 0x00u; /* Page index */
    m_auStorArea[13u][16u] = 0x00u; /* Page index */
    m_auStorArea[13u][17u] = 0x02u; /* Page type */
    m_auStorArea[13u][18u] = 0x01u; /* Page version */
    m_auStorArea[13u][19u] = 0x00u; /* Page version */
    m_auStorArea[13u][20u] = 0x0Eu; /* Total page */
    m_auStorArea[13u][21u] = 0x00u; /* Total page */
    m_auStorArea[13u][22u] = 0x00u; /* Total page */
    m_auStorArea[13u][23u] = 0x00u; /* Total page */
    m_auStorArea[13u][24u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][25u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][26u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][27u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][28u] = 0xCDu; /* CRC */
    m_auStorArea[13u][29u] = 0x02u; /* CRC */
    m_auStorArea[13u][30u] = 0x00u; /* CRC */
    m_auStorArea[13u][31u] = 0x00u; /* CRC */

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u; /* New index */
    l_ltUseBuff.puBuf[1u] = 0x02u; /* New index */
    l_ltUseBuff.puBuf[2u] = 0x03u; /* New index */
    l_ltUseBuff.puBuf[3u] = 0x04u; /* New index */
    l_ltUseBuff.puBuf[4u] = 0x05u; /* Filled page */
    l_ltUseBuff.puBuf[5u] = 0x06u; /* Filled page */
    l_ltUseBuff.puBuf[6u] = 0x07u; /* Filled page */
    l_ltUseBuff.puBuf[7u] = 0x08u; /* Filled page */

    l_ltUseBuff2.puBuf[0u] = 0x11u; /* New index */
    l_ltUseBuff2.puBuf[1u] = 0x12u; /* New index */
    l_ltUseBuff2.puBuf[2u] = 0x13u; /* New index */
    l_ltUseBuff2.puBuf[3u] = 0x14u; /* New index */
    l_ltUseBuff2.puBuf[4u] = 0x15u; /* Filled page */
    l_ltUseBuff2.puBuf[5u] = 0x16u; /* Filled page */
    l_ltUseBuff2.puBuf[6u] = 0x17u; /* Filled page */
    l_ltUseBuff2.puBuf[7u] = 0x18u; /* Filled page */

    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;
    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx) )
    {
        if( ( 0x0Cu == l_uNewPIx ) && ( 0x00u == l_uFillPIdx ) )
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 17 -- FAIL \n");
    }

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u; /* New index */
    l_ltUseBuff.puBuf[1u] = 0x02u; /* New index */
    l_ltUseBuff.puBuf[2u] = 0x03u; /* New index */
    l_ltUseBuff.puBuf[3u] = 0x04u; /* New index */
    l_ltUseBuff.puBuf[4u] = 0x05u; /* Filled page */
    l_ltUseBuff.puBuf[5u] = 0x06u; /* Filled page */
    l_ltUseBuff.puBuf[6u] = 0x07u; /* Filled page */
    l_ltUseBuff.puBuf[7u] = 0x08u; /* Filled page */

    l_ltUseBuff2.puBuf[0u] = 0x11u; /* New index */
    l_ltUseBuff2.puBuf[1u] = 0x12u; /* New index */
    l_ltUseBuff2.puBuf[2u] = 0x13u; /* New index */
    l_ltUseBuff2.puBuf[3u] = 0x14u; /* New index */
    l_ltUseBuff2.puBuf[4u] = 0x15u; /* Filled page */
    l_ltUseBuff2.puBuf[5u] = 0x16u; /* Filled page */
    l_ltUseBuff2.puBuf[6u] = 0x17u; /* Filled page */
    l_ltUseBuff2.puBuf[7u] = 0x18u; /* Filled page */

    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;
    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx) )
    {
        if( ( 0x0Cu == l_uNewPIx ) && ( 0x00u == l_uFillPIdx ) )
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 18 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 18 -- FAIL \n");
    }

    /* --------------------------------------------- All OK no backup generation */
    /* Setup storage area */
    (void)memset(m_auStorArea[12u], 0, sizeof(m_auStorArea[12u]));
    m_auStorArea[12u][0u]  = 0x00u; /* New index */
    m_auStorArea[12u][1u]  = 0x00u; /* New index */
    m_auStorArea[12u][2u]  = 0x00u; /* New index */
    m_auStorArea[12u][3u]  = 0x00u; /* New index */
    m_auStorArea[12u][4u]  = 0x0Au; /* Filled page */
    m_auStorArea[12u][5u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][6u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][7u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][8u]  = 0x08u; /* Byte In Page */
    m_auStorArea[12u][9u]  = 0x00u; /* Byte In Page */
    m_auStorArea[12u][10u] = 0x00u; /* Byte In Page */
    m_auStorArea[12u][11u] = 0x00u; /* Byte In Page */
    m_auStorArea[12u][12u] = 0x07u; /* Page SUBTYPE */
    m_auStorArea[12u][13u] = 0x0Cu; /* Page index */
    m_auStorArea[12u][14u] = 0x00u; /* Page index */
    m_auStorArea[12u][15u] = 0x00u; /* Page index */
    m_auStorArea[12u][16u] = 0x00u; /* Page index */
    m_auStorArea[12u][17u] = 0x02u; /* Page type */
    m_auStorArea[12u][18u] = 0x01u; /* Page version */
    m_auStorArea[12u][19u] = 0x00u; /* Page version */
    m_auStorArea[12u][20u] = 0x0Eu; /* Total page */
    m_auStorArea[12u][21u] = 0x00u; /* Total page */
    m_auStorArea[12u][22u] = 0x00u; /* Total page */
    m_auStorArea[12u][23u] = 0x00u; /* Total page */
    m_auStorArea[12u][24u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][25u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][26u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][27u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][28u] = 0xC9u; /* CRC */
    m_auStorArea[12u][29u] = 0x02u; /* CRC */
    m_auStorArea[12u][30u] = 0x00u; /* CRC */
    m_auStorArea[12u][31u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[13u], 0, sizeof(m_auStorArea[13u]));
    m_auStorArea[13u][0u]  = 0x00u; /* New index */
    m_auStorArea[13u][1u]  = 0x00u; /* New index */
    m_auStorArea[13u][2u]  = 0x00u; /* New index */
    m_auStorArea[13u][3u]  = 0x00u; /* New index */
    m_auStorArea[13u][4u]  = 0x0Au; /* Filled page */
    m_auStorArea[13u][5u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][6u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][7u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][8u]  = 0x08u; /* Byte In Page */
    m_auStorArea[13u][9u]  = 0x00u; /* Byte In Page */
    m_auStorArea[13u][10u] = 0x00u; /* Byte In Page */
    m_auStorArea[13u][11u] = 0x00u; /* Byte In Page */
    m_auStorArea[13u][12u] = 0x08u; /* Page SUBTYPE */
    m_auStorArea[13u][13u] = 0x0Du; /* Page index */
    m_auStorArea[13u][14u] = 0x00u; /* Page index */
    m_auStorArea[13u][15u] = 0x00u; /* Page index */
    m_auStorArea[13u][16u] = 0x00u; /* Page index */
    m_auStorArea[13u][17u] = 0x02u; /* Page type */
    m_auStorArea[13u][18u] = 0x01u; /* Page version */
    m_auStorArea[13u][19u] = 0x00u; /* Page version */
    m_auStorArea[13u][20u] = 0x0Eu; /* Total page */
    m_auStorArea[13u][21u] = 0x00u; /* Total page */
    m_auStorArea[13u][22u] = 0x00u; /* Total page */
    m_auStorArea[13u][23u] = 0x00u; /* Total page */
    m_auStorArea[13u][24u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][25u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][26u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][27u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][28u] = 0xCBu; /* CRC */
    m_auStorArea[13u][29u] = 0x02u; /* CRC */
    m_auStorArea[13u][30u] = 0x00u; /* CRC */
    m_auStorArea[13u][31u] = 0x00u; /* CRC */

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u; /* New index */
    l_ltUseBuff.puBuf[1u] = 0x02u; /* New index */
    l_ltUseBuff.puBuf[2u] = 0x03u; /* New index */
    l_ltUseBuff.puBuf[3u] = 0x04u; /* New index */
    l_ltUseBuff.puBuf[4u] = 0x05u; /* Filled page */
    l_ltUseBuff.puBuf[5u] = 0x06u; /* Filled page */
    l_ltUseBuff.puBuf[6u] = 0x07u; /* Filled page */
    l_ltUseBuff.puBuf[7u] = 0x08u; /* Filled page */

    l_ltUseBuff2.puBuf[0u] = 0x11u; /* New index */
    l_ltUseBuff2.puBuf[1u] = 0x12u; /* New index */
    l_ltUseBuff2.puBuf[2u] = 0x13u; /* New index */
    l_ltUseBuff2.puBuf[3u] = 0x14u; /* New index */
    l_ltUseBuff2.puBuf[4u] = 0x15u; /* Filled page */
    l_ltUseBuff2.puBuf[5u] = 0x16u; /* Filled page */
    l_ltUseBuff2.puBuf[6u] = 0x17u; /* Filled page */
    l_ltUseBuff2.puBuf[7u] = 0x18u; /* Filled page */

    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;
    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx) )
    {
        if( ( 0x00u == l_uNewPIx ) && ( 0x0Au == l_uFillPIdx ) )
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 19 -- FAIL \n");
    }

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u; /* New index */
    l_ltUseBuff.puBuf[1u] = 0x02u; /* New index */
    l_ltUseBuff.puBuf[2u] = 0x03u; /* New index */
    l_ltUseBuff.puBuf[3u] = 0x04u; /* New index */
    l_ltUseBuff.puBuf[4u] = 0x05u; /* Filled page */
    l_ltUseBuff.puBuf[5u] = 0x06u; /* Filled page */
    l_ltUseBuff.puBuf[6u] = 0x07u; /* Filled page */
    l_ltUseBuff.puBuf[7u] = 0x08u; /* Filled page */

    l_ltUseBuff2.puBuf[0u] = 0x11u; /* New index */
    l_ltUseBuff2.puBuf[1u] = 0x12u; /* New index */
    l_ltUseBuff2.puBuf[2u] = 0x13u; /* New index */
    l_ltUseBuff2.puBuf[3u] = 0x14u; /* New index */
    l_ltUseBuff2.puBuf[4u] = 0x15u; /* Filled page */
    l_ltUseBuff2.puBuf[5u] = 0x16u; /* Filled page */
    l_ltUseBuff2.puBuf[6u] = 0x17u; /* Filled page */
    l_ltUseBuff2.puBuf[7u] = 0x18u; /* Filled page */

    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;
    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx) )
    {
        if( ( 0x00u == l_uNewPIx ) && ( 0x0Au == l_uFillPIdx ) )
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 20 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 20 -- FAIL \n");
    }

    /* --------------------------------------------- All OK no backup generation */
    /* Setup storage area */
    (void)memset(m_auStorArea[12u], 0, sizeof(m_auStorArea[12u]));
    m_auStorArea[12u][0u]  = 0x00u; /* New index */
    m_auStorArea[12u][1u]  = 0x00u; /* New index */
    m_auStorArea[12u][2u]  = 0x00u; /* New index */
    m_auStorArea[12u][3u]  = 0x00u; /* New index */
    m_auStorArea[12u][4u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][5u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][6u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][7u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][8u]  = 0x08u; /* Byte In Page */
    m_auStorArea[12u][9u]  = 0x00u; /* Byte In Page */
    m_auStorArea[12u][10u] = 0x00u; /* Byte In Page */
    m_auStorArea[12u][11u] = 0x00u; /* Byte In Page */
    m_auStorArea[12u][12u] = 0x06u; /* Page SUBTYPE */
    m_auStorArea[12u][13u] = 0x0Cu; /* Page index */
    m_auStorArea[12u][14u] = 0x00u; /* Page index */
    m_auStorArea[12u][15u] = 0x00u; /* Page index */
    m_auStorArea[12u][16u] = 0x00u; /* Page index */
    m_auStorArea[12u][17u] = 0x02u; /* Page type */
    m_auStorArea[12u][18u] = 0x01u; /* Page version */
    m_auStorArea[12u][19u] = 0x00u; /* Page version */
    m_auStorArea[12u][20u] = 0x0Eu; /* Total page */
    m_auStorArea[12u][21u] = 0x00u; /* Total page */
    m_auStorArea[12u][22u] = 0x00u; /* Total page */
    m_auStorArea[12u][23u] = 0x00u; /* Total page */
    m_auStorArea[12u][24u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][25u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][26u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][27u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][28u] = 0xBEu; /* CRC */
    m_auStorArea[12u][29u] = 0x02u; /* CRC */
    m_auStorArea[12u][30u] = 0x00u; /* CRC */
    m_auStorArea[12u][31u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[13u], 0, sizeof(m_auStorArea[13u]));
    m_auStorArea[13u][0u]  = 0x00u; /* New index */
    m_auStorArea[13u][1u]  = 0x00u; /* New index */
    m_auStorArea[13u][2u]  = 0x00u; /* New index */
    m_auStorArea[13u][3u]  = 0x00u; /* New index */
    m_auStorArea[13u][4u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][5u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][6u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][7u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][8u]  = 0x08u; /* Byte In Page */
    m_auStorArea[13u][9u]  = 0x00u; /* Byte In Page */
    m_auStorArea[13u][10u] = 0x00u; /* Byte In Page */
    m_auStorArea[13u][11u] = 0x00u; /* Byte In Page */
    m_auStorArea[13u][12u] = 0x05u; /* Page SUBTYPE */
    m_auStorArea[13u][13u] = 0x0Du; /* Page index */
    m_auStorArea[13u][14u] = 0x00u; /* Page index */
    m_auStorArea[13u][15u] = 0x00u; /* Page index */
    m_auStorArea[13u][16u] = 0x00u; /* Page index */
    m_auStorArea[13u][17u] = 0x02u; /* Page type */
    m_auStorArea[13u][18u] = 0x01u; /* Page version */
    m_auStorArea[13u][19u] = 0x00u; /* Page version */
    m_auStorArea[13u][20u] = 0x0Eu; /* Total page */
    m_auStorArea[13u][21u] = 0x00u; /* Total page */
    m_auStorArea[13u][22u] = 0x00u; /* Total page */
    m_auStorArea[13u][23u] = 0x00u; /* Total page */
    m_auStorArea[13u][24u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][25u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][26u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][27u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][28u] = 0xBEu; /* CRC */
    m_auStorArea[13u][29u] = 0x02u; /* CRC */
    m_auStorArea[13u][30u] = 0x00u; /* CRC */
    m_auStorArea[13u][31u] = 0x00u; /* CRC */

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u; /* New index */
    l_ltUseBuff.puBuf[1u] = 0x02u; /* New index */
    l_ltUseBuff.puBuf[2u] = 0x03u; /* New index */
    l_ltUseBuff.puBuf[3u] = 0x04u; /* New index */
    l_ltUseBuff.puBuf[4u] = 0x05u; /* Filled page */
    l_ltUseBuff.puBuf[5u] = 0x06u; /* Filled page */
    l_ltUseBuff.puBuf[6u] = 0x07u; /* Filled page */
    l_ltUseBuff.puBuf[7u] = 0x08u; /* Filled page */

    l_ltUseBuff2.puBuf[0u] = 0x11u; /* New index */
    l_ltUseBuff2.puBuf[1u] = 0x12u; /* New index */
    l_ltUseBuff2.puBuf[2u] = 0x13u; /* New index */
    l_ltUseBuff2.puBuf[3u] = 0x14u; /* New index */
    l_ltUseBuff2.puBuf[4u] = 0x15u; /* Filled page */
    l_ltUseBuff2.puBuf[5u] = 0x16u; /* Filled page */
    l_ltUseBuff2.puBuf[6u] = 0x17u; /* Filled page */
    l_ltUseBuff2.puBuf[7u] = 0x18u; /* Filled page */

    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;
    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx) )
    {
        if( ( 0x00u == l_uNewPIx ) && ( 0x00u == l_uFillPIdx ) )
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 21 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 21 -- FAIL \n");
    }

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u; /* New index */
    l_ltUseBuff.puBuf[1u] = 0x02u; /* New index */
    l_ltUseBuff.puBuf[2u] = 0x03u; /* New index */
    l_ltUseBuff.puBuf[3u] = 0x04u; /* New index */
    l_ltUseBuff.puBuf[4u] = 0x05u; /* Filled page */
    l_ltUseBuff.puBuf[5u] = 0x06u; /* Filled page */
    l_ltUseBuff.puBuf[6u] = 0x07u; /* Filled page */
    l_ltUseBuff.puBuf[7u] = 0x08u; /* Filled page */

    l_ltUseBuff2.puBuf[0u] = 0x11u; /* New index */
    l_ltUseBuff2.puBuf[1u] = 0x12u; /* New index */
    l_ltUseBuff2.puBuf[2u] = 0x13u; /* New index */
    l_ltUseBuff2.puBuf[3u] = 0x14u; /* New index */
    l_ltUseBuff2.puBuf[4u] = 0x15u; /* Filled page */
    l_ltUseBuff2.puBuf[5u] = 0x16u; /* Filled page */
    l_ltUseBuff2.puBuf[6u] = 0x17u; /* Filled page */
    l_ltUseBuff2.puBuf[7u] = 0x18u; /* Filled page */

    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;
    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx) )
    {
        if( ( 0x00u == l_uNewPIx ) && ( 0x00u == l_uFillPIdx ) )
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 22 -- FAIL \n");
    }

    /* --------------------------------------------- All OK no backup generation */
    /* Setup storage area */
    (void)memset(m_auStorArea[12u], 0, sizeof(m_auStorArea[12u]));
    m_auStorArea[12u][0u]  = 0x00u; /* New index */
    m_auStorArea[12u][1u]  = 0x00u; /* New index */
    m_auStorArea[12u][2u]  = 0x00u; /* New index */
    m_auStorArea[12u][3u]  = 0x00u; /* New index */
    m_auStorArea[12u][4u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][5u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][6u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][7u]  = 0x00u; /* Filled page */
    m_auStorArea[12u][8u]  = 0x08u; /* Byte In Page */
    m_auStorArea[12u][9u]  = 0x00u; /* Byte In Page */
    m_auStorArea[12u][10u] = 0x00u; /* Byte In Page */
    m_auStorArea[12u][11u] = 0x00u; /* Byte In Page */
    m_auStorArea[12u][12u] = 0x07u; /* Page SUBTYPE */
    m_auStorArea[12u][13u] = 0x0Cu; /* Page index */
    m_auStorArea[12u][14u] = 0x00u; /* Page index */
    m_auStorArea[12u][15u] = 0x00u; /* Page index */
    m_auStorArea[12u][16u] = 0x00u; /* Page index */
    m_auStorArea[12u][17u] = 0x02u; /* Page type */
    m_auStorArea[12u][18u] = 0x02u; /* Page version */
    m_auStorArea[12u][19u] = 0x00u; /* Page version */
    m_auStorArea[12u][20u] = 0x0Eu; /* Total page */
    m_auStorArea[12u][21u] = 0x00u; /* Total page */
    m_auStorArea[12u][22u] = 0x00u; /* Total page */
    m_auStorArea[12u][23u] = 0x00u; /* Total page */
    m_auStorArea[12u][24u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][25u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][26u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][27u] = 0xA5u; /* Magic number */
    m_auStorArea[12u][28u] = 0xC0u; /* CRC */
    m_auStorArea[12u][29u] = 0x02u; /* CRC */
    m_auStorArea[12u][30u] = 0x00u; /* CRC */
    m_auStorArea[12u][31u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[13u], 0, sizeof(m_auStorArea[13u]));
    m_auStorArea[13u][0u]  = 0x00u; /* New index */
    m_auStorArea[13u][1u]  = 0x00u; /* New index */
    m_auStorArea[13u][2u]  = 0x00u; /* New index */
    m_auStorArea[13u][3u]  = 0x00u; /* New index */
    m_auStorArea[13u][4u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][5u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][6u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][7u]  = 0x00u; /* Filled page */
    m_auStorArea[13u][8u]  = 0x08u; /* Byte In Page */
    m_auStorArea[13u][9u]  = 0x00u; /* Byte In Page */
    m_auStorArea[13u][10u] = 0x00u; /* Byte In Page */
    m_auStorArea[13u][11u] = 0x00u; /* Byte In Page */
    m_auStorArea[13u][12u] = 0x08u; /* Page SUBTYPE */
    m_auStorArea[13u][13u] = 0x0Du; /* Page index */
    m_auStorArea[13u][14u] = 0x00u; /* Page index */
    m_auStorArea[13u][15u] = 0x00u; /* Page index */
    m_auStorArea[13u][16u] = 0x00u; /* Page index */
    m_auStorArea[13u][17u] = 0x02u; /* Page type */
    m_auStorArea[13u][18u] = 0x02u; /* Page version */
    m_auStorArea[13u][19u] = 0x00u; /* Page version */
    m_auStorArea[13u][20u] = 0x0Eu; /* Total page */
    m_auStorArea[13u][21u] = 0x00u; /* Total page */
    m_auStorArea[13u][22u] = 0x00u; /* Total page */
    m_auStorArea[13u][23u] = 0x00u; /* Total page */
    m_auStorArea[13u][24u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][25u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][26u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][27u] = 0xA5u; /* Magic number */
    m_auStorArea[13u][28u] = 0xC2u; /* CRC */
    m_auStorArea[13u][29u] = 0x02u; /* CRC */
    m_auStorArea[13u][30u] = 0x00u; /* CRC */
    m_auStorArea[13u][31u] = 0x00u; /* CRC */

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u; /* New index */
    l_ltUseBuff.puBuf[1u] = 0x02u; /* New index */
    l_ltUseBuff.puBuf[2u] = 0x03u; /* New index */
    l_ltUseBuff.puBuf[3u] = 0x04u; /* New index */
    l_ltUseBuff.puBuf[4u] = 0x05u; /* Filled page */
    l_ltUseBuff.puBuf[5u] = 0x06u; /* Filled page */
    l_ltUseBuff.puBuf[6u] = 0x07u; /* Filled page */
    l_ltUseBuff.puBuf[7u] = 0x08u; /* Filled page */

    l_ltUseBuff2.puBuf[0u] = 0x11u; /* New index */
    l_ltUseBuff2.puBuf[1u] = 0x12u; /* New index */
    l_ltUseBuff2.puBuf[2u] = 0x13u; /* New index */
    l_ltUseBuff2.puBuf[3u] = 0x14u; /* New index */
    l_ltUseBuff2.puBuf[4u] = 0x15u; /* Filled page */
    l_ltUseBuff2.puBuf[5u] = 0x16u; /* Filled page */
    l_ltUseBuff2.puBuf[6u] = 0x17u; /* Filled page */
    l_ltUseBuff2.puBuf[7u] = 0x18u; /* Filled page */

    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;
    if( e_eFSS_LOGC_RES_NEWVERSIONFOUND == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx) )
    {
        if( ( 0x00u == l_uNewPIx ) && ( 0x00u == l_uFillPIdx ) )
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 23 -- FAIL \n");
    }

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u; /* New index */
    l_ltUseBuff.puBuf[1u] = 0x02u; /* New index */
    l_ltUseBuff.puBuf[2u] = 0x03u; /* New index */
    l_ltUseBuff.puBuf[3u] = 0x04u; /* New index */
    l_ltUseBuff.puBuf[4u] = 0x05u; /* Filled page */
    l_ltUseBuff.puBuf[5u] = 0x06u; /* Filled page */
    l_ltUseBuff.puBuf[6u] = 0x07u; /* Filled page */
    l_ltUseBuff.puBuf[7u] = 0x08u; /* Filled page */

    l_ltUseBuff2.puBuf[0u] = 0x11u; /* New index */
    l_ltUseBuff2.puBuf[1u] = 0x12u; /* New index */
    l_ltUseBuff2.puBuf[2u] = 0x13u; /* New index */
    l_ltUseBuff2.puBuf[3u] = 0x14u; /* New index */
    l_ltUseBuff2.puBuf[4u] = 0x15u; /* Filled page */
    l_ltUseBuff2.puBuf[5u] = 0x16u; /* Filled page */
    l_ltUseBuff2.puBuf[6u] = 0x17u; /* Filled page */
    l_ltUseBuff2.puBuf[7u] = 0x18u; /* Filled page */

    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;
    if( e_eFSS_LOGC_RES_NEWVERSIONFOUND == eFSS_LOGC_ReadCache(&l_tCtx, &l_uNewPIx, &l_uFillPIdx) )
    {
        if( ( 0x00u == l_uNewPIx ) && ( 0x00u == l_uFillPIdx ) )
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 24 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_ReadCacheTest 24 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_ReadCacheTest 24 -- FAIL \n");
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

static void eFSS_LOGCTST_WriteCacheTest(void)
{
    /* Local variable */
    t_eFSS_LOGC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_uStorType;
    uint8_t l_auStor[64u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_LOGC_StorBuf l_ltUseBuff;
    t_eFSS_LOGC_StorBuf l_ltUseBuff2;
    uint32_t l_uByteInPage;
    uint32_t l_uPageUsable;
    uint32_t l_uNewPIx;
    uint32_t l_uFillPIdx;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 14u;
    l_tStorSet.uPagesLen = 32u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;
    l_uStorType = 1u;
    l_uByteInPage = 0u;
    l_uPageUsable = 0u;
    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor), true, false ) )
    {
        (void)printf("eFSS_LOGCTST_WriteCacheTest 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_WriteCacheTest 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uPageUsable) )
    {
        (void)printf("eFSS_LOGCTST_WriteCacheTest 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_WriteCacheTest 2  -- FAIL \n");
    }

    /* Setup storage area */
    l_ltUseBuff2.uBufL = l_ltUseBuff.uBufL  + 1u;
    l_ltUseBuff2.puBuf = &l_auStor[32];

    /* Setup storage area */
    (void)memset(m_auStorArea[12u], 0, sizeof(m_auStorArea[12u]));
    (void)memset(m_auStorArea[13u], 0, sizeof(m_auStorArea[13u]));
    (void)memset(l_ltUseBuff.puBuf, 0x10, l_ltUseBuff.uBufL);


    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_WriteCache(&l_tCtx, 0u, 0u) )
    {
        if( ( 0x00u == l_ltUseBuff.puBuf[0u] ) && ( 0x00u == l_ltUseBuff.puBuf[1u] )  && ( 0x00u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[3u] ) && ( 0x00u == l_ltUseBuff.puBuf[4u] )  && ( 0x00u == l_ltUseBuff.puBuf[5u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[6u] ) && ( 0x00u == l_ltUseBuff.puBuf[7u] )  && ( 0x08u == l_ltUseBuff.puBuf[8u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[9u] ) && ( 0x00u == l_ltUseBuff.puBuf[10u] ) && ( 0x00u == l_ltUseBuff.puBuf[11u] ) )
        {
            if( ( m_auStorArea[12u][0u]  == 0x00u ) && ( m_auStorArea[12u][1u]  == 0x00u ) && ( m_auStorArea[12u][2u]  == 0x00u ) && ( m_auStorArea[12u][3u]  == 0x00u ) &&
                ( m_auStorArea[12u][4u]  == 0x00u ) && ( m_auStorArea[12u][5u]  == 0x00u ) && ( m_auStorArea[12u][6u]  == 0x00u ) && ( m_auStorArea[12u][7u]  == 0x00u ) &&
                ( m_auStorArea[12u][8u]  == 0x08u ) && ( m_auStorArea[12u][9u]  == 0x00u ) && ( m_auStorArea[12u][10u] == 0x00u ) && ( m_auStorArea[12u][11u] == 0x00u ) &&
                ( m_auStorArea[12u][12u] == 0x07u ) && ( m_auStorArea[12u][13u] == 0x0Cu ) && ( m_auStorArea[12u][14u] == 0x00u ) && ( m_auStorArea[12u][15u] == 0x00u ) &&
                ( m_auStorArea[12u][16u] == 0x00u ) && ( m_auStorArea[12u][17u] == 0x02u ) && ( m_auStorArea[12u][18u] == 0x01u ) && ( m_auStorArea[12u][19u] == 0x00u ) &&
                ( m_auStorArea[12u][20u] == 0x0Eu ) && ( m_auStorArea[12u][21u] == 0x00u ) && ( m_auStorArea[12u][22u] == 0x00u ) && ( m_auStorArea[12u][23u] == 0x00u ) &&
                ( m_auStorArea[12u][24u] == 0xA5u ) && ( m_auStorArea[12u][25u] == 0xA5u ) && ( m_auStorArea[12u][26u] == 0xA5u ) && ( m_auStorArea[12u][27u] == 0xA5u ) &&
                ( m_auStorArea[12u][28u] == 0xBFu ) && ( m_auStorArea[12u][29u] == 0x02u ) && ( m_auStorArea[12u][30u] == 0x00u ) && ( m_auStorArea[12u][31u] == 0x00u ) &&
                ( m_auStorArea[13u][0u]  == 0x00u ) && ( m_auStorArea[13u][1u]  == 0x00u ) && ( m_auStorArea[13u][2u]  == 0x00u ) && ( m_auStorArea[13u][3u]  == 0x00u ) &&
                ( m_auStorArea[13u][4u]  == 0x00u ) && ( m_auStorArea[13u][5u]  == 0x00u ) && ( m_auStorArea[13u][6u]  == 0x00u ) && ( m_auStorArea[13u][7u]  == 0x00u ) &&
                ( m_auStorArea[13u][8u]  == 0x08u ) && ( m_auStorArea[13u][9u]  == 0x00u ) && ( m_auStorArea[13u][10u] == 0x00u ) && ( m_auStorArea[13u][11u] == 0x00u ) &&
                ( m_auStorArea[13u][12u] == 0x08u ) && ( m_auStorArea[13u][13u] == 0x0Du ) && ( m_auStorArea[13u][14u] == 0x00u ) && ( m_auStorArea[13u][15u] == 0x00u ) &&
                ( m_auStorArea[13u][16u] == 0x00u ) && ( m_auStorArea[13u][17u] == 0x02u ) && ( m_auStorArea[13u][18u] == 0x01u ) && ( m_auStorArea[13u][19u] == 0x00u ) &&
                ( m_auStorArea[13u][20u] == 0x0Eu ) && ( m_auStorArea[13u][21u] == 0x00u ) && ( m_auStorArea[13u][22u] == 0x00u ) && ( m_auStorArea[13u][23u] == 0x00u ) &&
                ( m_auStorArea[13u][24u] == 0xA5u ) && ( m_auStorArea[13u][25u] == 0xA5u ) && ( m_auStorArea[13u][26u] == 0xA5u ) && ( m_auStorArea[13u][27u] == 0xA5u ) &&
                ( m_auStorArea[13u][28u] == 0xC1u ) && ( m_auStorArea[13u][29u] == 0x02u ) && ( m_auStorArea[13u][30u] == 0x00u ) && ( m_auStorArea[13u][31u] == 0x00u ) )
              {
                  (void)printf("eFSS_LOGCTST_WriteCacheTest 3  -- OK \n");
              }
              else
              {
                  (void)printf("eFSS_LOGCTST_WriteCacheTest 3  -- FAIL \n");
              }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_WriteCacheTest 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_WriteCacheTest 3  -- FAIL \n");
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
    (void)l_ltUseBuff.puBuf;
    (void)l_ltUseBuff2.puBuf;
}

#if 0

static void eFSS_LOGCTST_LoadTest(void)
{
    /* Local variable */
    t_eFSS_LOGC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_uStorType;
    uint8_t l_auStor[56u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_LOGC_StorBuf l_ltUseBuff;
    t_eFSS_LOGC_StorBuf l_ltUseBuff2;
    uint8_t l_uSubTypeRead;
    uint32_t l_uByteInPage;
    uint32_t l_uPageUsable;
    uint32_t l_uNewPIx;
    uint32_t l_uFillPIdx;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 2u;
    l_tStorSet.uPagesLen = 28u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;
    l_uStorType = 1u;
    l_uByteInPage = 0u;
    l_uPageUsable = 0u;
    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_LOGCTST_LoadTest 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_LOGCTST_LoadTest 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 2  -- FAIL \n");
    }

    /* Setup storage area */
    l_ltUseBuff2.uBufL = l_ltUseBuff.uBufL  + 1u;
    l_ltUseBuff2.puBuf = &l_auStor[24];

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    (void)memset(m_auStorArea[p_uPageToWrite], 0xFF, sizeof(m_auStorArea[p_uPageToWrite]));

    /* Setup buffer */
    l_uSubTypeRead = 0u;
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_LoadBufferAs(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ) )
        {
            (void)printf("eFSS_LOGCTST_LoadTest 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadTest 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 3  -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff2.puBuf[0u] ) && ( 0x12u == l_ltUseBuff2.puBuf[1u] ) && ( 0x13u == l_ltUseBuff2.puBuf[2u] ) &&
        ( 0x14u == l_ltUseBuff2.puBuf[3u] ) && ( 0x15u == l_ltUseBuff2.puBuf[4u] ) )
    {
        (void)printf("eFSS_LOGCTST_LoadTest 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 4  -- FAIL \n");
    }

    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_LoadBufferAs(&l_tCtx, 1u, &l_uSubTypeRead) )
    {
        if( ( 0xFFu == l_ltUseBuff.puBuf[0u] ) && ( 0xFFu == l_ltUseBuff.puBuf[1u] ) && ( 0xFFu == l_ltUseBuff.puBuf[2u] ) &&
             ( 0xFFu == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ) )
        {
            (void)printf("eFSS_LOGCTST_LoadTest 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadTest 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 5  -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff2.puBuf[0u] ) && ( 0x12u == l_ltUseBuff2.puBuf[1u] ) && ( 0x13u == l_ltUseBuff2.puBuf[2u] ) &&
        ( 0x14u == l_ltUseBuff2.puBuf[3u] ) && ( 0x15u == l_ltUseBuff2.puBuf[4u] ) )
    {
        (void)printf("eFSS_LOGCTST_LoadTest 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 6  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][20u] = 0xFFu;
    m_auStorArea[p_uPageToWrite][21u] = 0xFFu;
    m_auStorArea[p_uPageToWrite][22u] = 0xFFu;
    m_auStorArea[p_uPageToWrite][23u] = 0xFFu;

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][20u] = 0x3Fu;
    m_auStorArea[p_uPageToWrite][21u] = 0x01u;
    m_auStorArea[p_uPageToWrite][22u] = 0x00u;
    m_auStorArea[p_uPageToWrite][23u] = 0x00u;

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_LoadBufferAs(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ) )
        {
            (void)printf("eFSS_LOGCTST_LoadTest 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadTest 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 7  -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff2.puBuf[0u] ) && ( 0x12u == l_ltUseBuff2.puBuf[1u] ) && ( 0x13u == l_ltUseBuff2.puBuf[2u] ) &&
        ( 0x14u == l_ltUseBuff2.puBuf[3u] ) && ( 0x15u == l_ltUseBuff2.puBuf[4u] ) )
    {
        (void)printf("eFSS_LOGCTST_LoadTest 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 8  -- FAIL \n");
    }

    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_LoadBufferAs(&l_tCtx, 1u, &l_uSubTypeRead) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] ) && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0x10u == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ))
        {
            (void)printf("eFSS_LOGCTST_LoadTest 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadTest 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 9  -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff2.puBuf[0u] ) && ( 0x12u == l_ltUseBuff2.puBuf[1u] ) && ( 0x13u == l_ltUseBuff2.puBuf[2u] ) &&
        ( 0x14u == l_ltUseBuff2.puBuf[3u] ) && ( 0x15u == l_ltUseBuff2.puBuf[4u] ) )
    {
        (void)printf("eFSS_LOGCTST_LoadTest 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 10 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][20u] = 0x93u;
    m_auStorArea[p_uPageToWrite][21u] = 0x02u;
    m_auStorArea[p_uPageToWrite][22u] = 0x00u;
    m_auStorArea[p_uPageToWrite][23u] = 0x00u;

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][20u] = 0x93u;
    m_auStorArea[p_uPageToWrite][21u] = 0x03u;
    m_auStorArea[p_uPageToWrite][22u] = 0x00u;
    m_auStorArea[p_uPageToWrite][23u] = 0x00u;

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_LoadBufferAs(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ))
        {
            (void)printf("eFSS_LOGCTST_LoadTest 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadTest 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 11 -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff2.puBuf[0u] ) && ( 0x12u == l_ltUseBuff2.puBuf[1u] ) && ( 0x13u == l_ltUseBuff2.puBuf[2u] ) &&
        ( 0x14u == l_ltUseBuff2.puBuf[3u] ) && ( 0x15u == l_ltUseBuff2.puBuf[4u] ) )
    {
        (void)printf("eFSS_LOGCTST_LoadTest 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 12 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][12u] = 0x02u;
    m_auStorArea[p_uPageToWrite][13u] = 0x00u;
    m_auStorArea[p_uPageToWrite][14u] = 0x00u;
    m_auStorArea[p_uPageToWrite][15u] = 0x00u;
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][20u] = 0x95u;
    m_auStorArea[p_uPageToWrite][21u] = 0x02u;
    m_auStorArea[p_uPageToWrite][22u] = 0x00u;
    m_auStorArea[p_uPageToWrite][23u] = 0x00u;

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][12u] = 0x02u;
    m_auStorArea[p_uPageToWrite][13u] = 0x00u;
    m_auStorArea[p_uPageToWrite][14u] = 0x00u;
    m_auStorArea[p_uPageToWrite][15u] = 0x00u;
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][20u] = 0x55u;
    m_auStorArea[p_uPageToWrite][21u] = 0x03u;
    m_auStorArea[p_uPageToWrite][22u] = 0x00u;
    m_auStorArea[p_uPageToWrite][23u] = 0x00u;

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_LoadBufferAs(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ))
        {
            (void)printf("eFSS_LOGCTST_LoadTest 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadTest 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 13 -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff2.puBuf[0u] ) && ( 0x12u == l_ltUseBuff2.puBuf[1u] ) && ( 0x13u == l_ltUseBuff2.puBuf[2u] ) &&
        ( 0x14u == l_ltUseBuff2.puBuf[3u] ) && ( 0x15u == l_ltUseBuff2.puBuf[4u] ) )
    {
        (void)printf("eFSS_LOGCTST_LoadTest 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 14 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][10u] = 0x01u;
    m_auStorArea[p_uPageToWrite][11u] = 0x00u;
    m_auStorArea[p_uPageToWrite][12u] = 0x02u;
    m_auStorArea[p_uPageToWrite][13u] = 0x00u;
    m_auStorArea[p_uPageToWrite][14u] = 0x00u;
    m_auStorArea[p_uPageToWrite][15u] = 0x00u;
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][20u] = 0x96u;
    m_auStorArea[p_uPageToWrite][21u] = 0x02u;
    m_auStorArea[p_uPageToWrite][22u] = 0x00u;
    m_auStorArea[p_uPageToWrite][23u] = 0x00u;

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][10u] = 0x01u;
    m_auStorArea[p_uPageToWrite][11u] = 0x00u;
    m_auStorArea[p_uPageToWrite][12u] = 0x02u;
    m_auStorArea[p_uPageToWrite][13u] = 0x00u;
    m_auStorArea[p_uPageToWrite][14u] = 0x00u;
    m_auStorArea[p_uPageToWrite][15u] = 0x00u;
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][20u] = 0x36u;
    m_auStorArea[p_uPageToWrite][21u] = 0x03u;
    m_auStorArea[p_uPageToWrite][22u] = 0x00u;
    m_auStorArea[p_uPageToWrite][23u] = 0x00u;

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_LoadBufferAs(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ))
        {
            (void)printf("eFSS_LOGCTST_LoadTest 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadTest 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 15 -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff2.puBuf[0u] ) && ( 0x12u == l_ltUseBuff2.puBuf[1u] ) && ( 0x13u == l_ltUseBuff2.puBuf[2u] ) &&
        ( 0x14u == l_ltUseBuff2.puBuf[3u] ) && ( 0x15u == l_ltUseBuff2.puBuf[4u] ) )
    {
        (void)printf("eFSS_LOGCTST_LoadTest 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 15 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][9u] = 0x01u;
    m_auStorArea[p_uPageToWrite][10u] = 0x01u;
    m_auStorArea[p_uPageToWrite][11u] = 0x00u;
    m_auStorArea[p_uPageToWrite][12u] = 0x02u;
    m_auStorArea[p_uPageToWrite][13u] = 0x00u;
    m_auStorArea[p_uPageToWrite][14u] = 0x00u;
    m_auStorArea[p_uPageToWrite][15u] = 0x00u;
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][20u] = 0x97u;
    m_auStorArea[p_uPageToWrite][21u] = 0x02u;
    m_auStorArea[p_uPageToWrite][22u] = 0x00u;
    m_auStorArea[p_uPageToWrite][23u] = 0x00u;

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][9u] = 0x01u;
    m_auStorArea[p_uPageToWrite][10u] = 0x01u;
    m_auStorArea[p_uPageToWrite][11u] = 0x00u;
    m_auStorArea[p_uPageToWrite][12u] = 0x02u;
    m_auStorArea[p_uPageToWrite][13u] = 0x00u;
    m_auStorArea[p_uPageToWrite][14u] = 0x00u;
    m_auStorArea[p_uPageToWrite][15u] = 0x00u;
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][20u] = 0x27u;
    m_auStorArea[p_uPageToWrite][21u] = 0x03u;
    m_auStorArea[p_uPageToWrite][22u] = 0x00u;
    m_auStorArea[p_uPageToWrite][23u] = 0x00u;

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_LoadBufferAs(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ))
        {
            (void)printf("eFSS_LOGCTST_LoadTest 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadTest 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 17 -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff2.puBuf[0u] ) && ( 0x12u == l_ltUseBuff2.puBuf[1u] ) && ( 0x13u == l_ltUseBuff2.puBuf[2u] ) &&
        ( 0x14u == l_ltUseBuff2.puBuf[3u] ) && ( 0x15u == l_ltUseBuff2.puBuf[4u] ) )
    {
        (void)printf("eFSS_LOGCTST_LoadTest 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 18 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][5u] = 0x01u;
    m_auStorArea[p_uPageToWrite][6u] = 0x00u;
    m_auStorArea[p_uPageToWrite][7u] = 0x00u;
    m_auStorArea[p_uPageToWrite][8u] = 0x00u;
    m_auStorArea[p_uPageToWrite][9u] = 0x01u;
    m_auStorArea[p_uPageToWrite][10u] = 0x01u;
    m_auStorArea[p_uPageToWrite][11u] = 0x00u;
    m_auStorArea[p_uPageToWrite][12u] = 0x02u;
    m_auStorArea[p_uPageToWrite][13u] = 0x00u;
    m_auStorArea[p_uPageToWrite][14u] = 0x00u;
    m_auStorArea[p_uPageToWrite][15u] = 0x00u;
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][20u] = 0x98u;
    m_auStorArea[p_uPageToWrite][21u] = 0x02u;
    m_auStorArea[p_uPageToWrite][22u] = 0x00u;
    m_auStorArea[p_uPageToWrite][23u] = 0x00u;

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));

    m_auStorArea[p_uPageToWrite][5u] = 0x01u;
    m_auStorArea[p_uPageToWrite][6u] = 0x00u;
    m_auStorArea[p_uPageToWrite][7u] = 0x00u;
    m_auStorArea[p_uPageToWrite][8u] = 0x00u;
    m_auStorArea[p_uPageToWrite][9u] = 0x01u;
    m_auStorArea[p_uPageToWrite][10u] = 0x01u;
    m_auStorArea[p_uPageToWrite][11u] = 0x00u;
    m_auStorArea[p_uPageToWrite][12u] = 0x02u;
    m_auStorArea[p_uPageToWrite][13u] = 0x00u;
    m_auStorArea[p_uPageToWrite][14u] = 0x00u;
    m_auStorArea[p_uPageToWrite][15u] = 0x00u;
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][20u] = 0xE8u;
    m_auStorArea[p_uPageToWrite][21u] = 0x02u;
    m_auStorArea[p_uPageToWrite][22u] = 0x00u;
    m_auStorArea[p_uPageToWrite][23u] = 0x00u;

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_LoadBufferAs(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ))
        {
            (void)printf("eFSS_LOGCTST_LoadTest 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadTest 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 19 -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff2.puBuf[0u] ) && ( 0x12u == l_ltUseBuff2.puBuf[1u] ) && ( 0x13u == l_ltUseBuff2.puBuf[2u] ) &&
        ( 0x14u == l_ltUseBuff2.puBuf[3u] ) && ( 0x15u == l_ltUseBuff2.puBuf[4u] ) )
    {
        (void)printf("eFSS_LOGCTST_LoadTest 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 20 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][5u] = 0x00u;
    m_auStorArea[p_uPageToWrite][6u] = 0x00u;
    m_auStorArea[p_uPageToWrite][7u] = 0x00u;
    m_auStorArea[p_uPageToWrite][8u] = 0x00u;
    m_auStorArea[p_uPageToWrite][9u] = 0x01u;
    m_auStorArea[p_uPageToWrite][10u] = 0x00u;
    m_auStorArea[p_uPageToWrite][11u] = 0x00u;
    m_auStorArea[p_uPageToWrite][12u] = 0x02u;
    m_auStorArea[p_uPageToWrite][13u] = 0x00u;
    m_auStorArea[p_uPageToWrite][14u] = 0x00u;
    m_auStorArea[p_uPageToWrite][15u] = 0x00u;
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][20u] = 0x96u;
    m_auStorArea[p_uPageToWrite][21u] = 0x02u;
    m_auStorArea[p_uPageToWrite][22u] = 0x00u;
    m_auStorArea[p_uPageToWrite][23u] = 0x00u;

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));

    m_auStorArea[p_uPageToWrite][5u] = 0x01u;
    m_auStorArea[p_uPageToWrite][6u] = 0x00u;
    m_auStorArea[p_uPageToWrite][7u] = 0x00u;
    m_auStorArea[p_uPageToWrite][8u] = 0x00u;
    m_auStorArea[p_uPageToWrite][9u] = 0x01u;
    m_auStorArea[p_uPageToWrite][10u] = 0x02u;
    m_auStorArea[p_uPageToWrite][11u] = 0x00u;
    m_auStorArea[p_uPageToWrite][12u] = 0x02u;
    m_auStorArea[p_uPageToWrite][13u] = 0x00u;
    m_auStorArea[p_uPageToWrite][14u] = 0x00u;
    m_auStorArea[p_uPageToWrite][15u] = 0x00u;
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u;
    m_auStorArea[p_uPageToWrite][20u] = 0xE9u;
    m_auStorArea[p_uPageToWrite][21u] = 0x02u;
    m_auStorArea[p_uPageToWrite][22u] = 0x00u;
    m_auStorArea[p_uPageToWrite][23u] = 0x00u;

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_LOGC_RES_NEWVERSIONFOUND == eFSS_LOGC_LoadBufferAs(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ))
        {
            (void)printf("eFSS_LOGCTST_LoadTest 21 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadTest 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 21 -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff2.puBuf[0u] ) && ( 0x12u == l_ltUseBuff2.puBuf[1u] ) && ( 0x13u == l_ltUseBuff2.puBuf[2u] ) &&
        ( 0x14u == l_ltUseBuff2.puBuf[3u] ) && ( 0x15u == l_ltUseBuff2.puBuf[4u] ) )
    {
        (void)printf("eFSS_LOGCTST_LoadTest 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 22 -- FAIL \n");
    }

    /* Setup storage area */
    l_uSubTypeRead = 0u;

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u] = 0x00u;  /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][6u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][7u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][8u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][9u] = 0x01u;  /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0x97u; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u] = 0x00u;  /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u] = 0x01u;  /* Page index */
    m_auStorArea[p_uPageToWrite][6u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][7u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][8u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][9u] = 0x01u;  /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0xD8u; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_LoadBufferAs(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ))
        {
            (void)printf("eFSS_LOGCTST_LoadTest 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadTest 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 23 -- FAIL \n");
    }

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_LoadBufferAs(&l_tCtx, 1u, &l_uSubTypeRead) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] ) && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0x10u == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ))
        {
            (void)printf("eFSS_LOGCTST_LoadTest 24 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadTest 24 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 24 -- FAIL \n");
    }

    /* Setup storage area */
    l_uSubTypeRead = 0u;

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u] = 0x03u;  /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][6u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][7u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][8u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][9u] = 0x01u;  /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0x9Au; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u] = 0x04u;  /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u] = 0x01u;  /* Page index */
    m_auStorArea[p_uPageToWrite][6u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][7u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][8u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][9u] = 0x01u;  /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0xDCu; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_LoadBufferAs(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff.puBuf[3u] ) && ( 3u == l_uSubTypeRead ))
        {
            (void)printf("eFSS_LOGCTST_LoadTest 25 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadTest 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 25 -- FAIL \n");
    }

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_LoadBufferAs(&l_tCtx, 1u, &l_uSubTypeRead) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] ) && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0x10u == l_ltUseBuff.puBuf[3u] ) && ( 4u == l_uSubTypeRead ))
        {
            (void)printf("eFSS_LOGCTST_LoadTest 26 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadTest 26 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadTest 26 -- FAIL \n");
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

static void eFSS_LOGCTST_FlushTest(void)
{
    /* Local variable */
    t_eFSS_LOGC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_uStorType;
    uint8_t l_auStor[56u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_LOGC_StorBuf l_ltUseBuff;
    t_eFSS_LOGC_StorBuf l_ltUseBuff2;
    uint8_t l_uSubTypeWrite;
    uint32_t l_uByteInPage;
    uint32_t l_uPageUsable;
    uint32_t l_uNewPIx;
    uint32_t l_uFillPIdx;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 2u;
    l_tStorSet.uPagesLen = 28u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;
    l_uStorType = 1u;
    l_uByteInPage = 0u;
    l_uPageUsable = 0u;
    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_LOGCTST_FlushTest 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_FlushTest 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_LOGCTST_FlushTest 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_FlushTest 2  -- FAIL \n");
    }

    /* Setup storage area */
    l_ltUseBuff2.uBufL = l_ltUseBuff.uBufL  + 1u;
    l_ltUseBuff2.puBuf = &l_auStor[24];

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    (void)memset(m_auStorArea[p_uPageToWrite], 0xFF, sizeof(m_auStorArea[p_uPageToWrite]));

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    l_uSubTypeWrite = 0x05u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_FlushBufferAs(&l_tCtx, 0u, l_uSubTypeWrite) )
    {
        if( ( 0x01u== m_auStorArea[p_uPageToWrite][0u] )  && ( 0x02u== m_auStorArea[p_uPageToWrite][1u] )  && ( 0x03u== m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x04u== m_auStorArea[p_uPageToWrite][3u] )  && ( 0x05u== m_auStorArea[p_uPageToWrite][4u] )  &&
            ( 0x00u== m_auStorArea[p_uPageToWrite][5u] )  && ( 0x00u== m_auStorArea[p_uPageToWrite][6u] )  && ( 0x00u== m_auStorArea[p_uPageToWrite][7u] ) &&
            ( 0x00u== m_auStorArea[p_uPageToWrite][8u] )  && ( 0x01u== m_auStorArea[p_uPageToWrite][9u] )  && ( 0x01u== m_auStorArea[p_uPageToWrite][10u] ) &&
            ( 0x00u== m_auStorArea[p_uPageToWrite][11u] ) && ( 0x02u== m_auStorArea[p_uPageToWrite][12u] ) && ( 0x00u== m_auStorArea[p_uPageToWrite][13u] ) &&
            ( 0x00u== m_auStorArea[p_uPageToWrite][14u] ) && ( 0x00u== m_auStorArea[p_uPageToWrite][15u] ) && ( 0xA5u== m_auStorArea[p_uPageToWrite][16u] ) &&
            ( 0xA5u== m_auStorArea[p_uPageToWrite][17u] ) && ( 0xA5u== m_auStorArea[p_uPageToWrite][18u] ) && ( 0xA5u== m_auStorArea[p_uPageToWrite][19u] ) &&
            ( 0xA6u== m_auStorArea[p_uPageToWrite][20u] ) && ( 0x02u== m_auStorArea[p_uPageToWrite][21u] ) && ( 0x00u== m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0x00u== m_auStorArea[p_uPageToWrite][23u] ) &&
            ( 0xFFu== m_auStorArea[p_uPageToWrite][0u] )  && ( 0xFFu== m_auStorArea[p_uPageToWrite][1u] )  && ( 0xFFu== m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0xFFu== m_auStorArea[p_uPageToWrite][3u] )  && ( 0xFFu== m_auStorArea[p_uPageToWrite][4u] )  &&
            ( 0xFFu== m_auStorArea[p_uPageToWrite][5u] )  && ( 0xFFu== m_auStorArea[p_uPageToWrite][6u] )  && ( 0xFFu== m_auStorArea[p_uPageToWrite][7u] ) &&
            ( 0xFFu== m_auStorArea[p_uPageToWrite][8u] )  && ( 0xFFu== m_auStorArea[p_uPageToWrite][9u] )  && ( 0xFFu== m_auStorArea[p_uPageToWrite][10u] ) &&
            ( 0xFFu== m_auStorArea[p_uPageToWrite][11u] ) && ( 0xFFu== m_auStorArea[p_uPageToWrite][12u] ) && ( 0xFFu== m_auStorArea[p_uPageToWrite][13u] ) &&
            ( 0xFFu== m_auStorArea[p_uPageToWrite][14u] ) && ( 0xFFu== m_auStorArea[p_uPageToWrite][15u] ) && ( 0xFFu== m_auStorArea[p_uPageToWrite][16u] ) &&
            ( 0xFFu== m_auStorArea[p_uPageToWrite][17u] ) && ( 0xFFu== m_auStorArea[p_uPageToWrite][18u] ) && ( 0xFFu== m_auStorArea[p_uPageToWrite][19u] ) &&
            ( 0xFFu== m_auStorArea[p_uPageToWrite][20u] ) && ( 0xFFu== m_auStorArea[p_uPageToWrite][21u] ) && ( 0xFFu== m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0xFFu== m_auStorArea[p_uPageToWrite][23u] ) )
        {
            if( ( 0x01u== l_ltUseBuff.puBuf[0u] )  && ( 0x02u== l_ltUseBuff.puBuf[1u] )  && ( 0x03u== l_ltUseBuff.puBuf[2u] ) &&
                ( 0x04u== l_ltUseBuff.puBuf[3u] )  && ( 0x05u== l_ltUseBuff.puBuf[4u] )  &&
                ( 0x01u== l_ltUseBuff2.puBuf[0u] )  && ( 0x02u== l_ltUseBuff2.puBuf[1u] )  && ( 0x03u== l_ltUseBuff2.puBuf[2u] ) &&
                ( 0x04u== l_ltUseBuff2.puBuf[3u] )  && ( 0x05u== l_ltUseBuff2.puBuf[4u] ) )
            {
                (void)printf("eFSS_LOGCTST_FlushTest 3  -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_FlushTest 3  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_FlushTest 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_FlushTest 3  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    (void)memset(m_auStorArea[p_uPageToWrite], 0xFF, sizeof(m_auStorArea[p_uPageToWrite]));

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    l_uSubTypeWrite = 0x05u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_FlushBufferAs(&l_tCtx, 1u, l_uSubTypeWrite) )
    {
        if( ( 0x01u== m_auStorArea[p_uPageToWrite][0u] )  && ( 0x02u== m_auStorArea[p_uPageToWrite][1u] )  && ( 0x03u== m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x04u== m_auStorArea[p_uPageToWrite][3u] )  && ( 0x05u== m_auStorArea[p_uPageToWrite][4u] )  &&
            ( 0x01u== m_auStorArea[p_uPageToWrite][5u] )  && ( 0x00u== m_auStorArea[p_uPageToWrite][6u] )  && ( 0x00u== m_auStorArea[p_uPageToWrite][7u] ) &&
            ( 0x00u== m_auStorArea[p_uPageToWrite][8u] )  && ( 0x01u== m_auStorArea[p_uPageToWrite][9u] )  && ( 0x01u== m_auStorArea[p_uPageToWrite][10u] ) &&
            ( 0x00u== m_auStorArea[p_uPageToWrite][11u] ) && ( 0x02u== m_auStorArea[p_uPageToWrite][12u] ) && ( 0x00u== m_auStorArea[p_uPageToWrite][13u] ) &&
            ( 0x00u== m_auStorArea[p_uPageToWrite][14u] ) && ( 0x00u== m_auStorArea[p_uPageToWrite][15u] ) && ( 0xA5u== m_auStorArea[p_uPageToWrite][16u] ) &&
            ( 0xA5u== m_auStorArea[p_uPageToWrite][17u] ) && ( 0xA5u== m_auStorArea[p_uPageToWrite][18u] ) && ( 0xA5u== m_auStorArea[p_uPageToWrite][19u] ) &&
            ( 0xA7u== m_auStorArea[p_uPageToWrite][20u] ) && ( 0x02u== m_auStorArea[p_uPageToWrite][21u] ) && ( 0x00u== m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0x00u== m_auStorArea[p_uPageToWrite][23u] ) &&
            ( 0x00u== m_auStorArea[p_uPageToWrite][0u] )  && ( 0x00u== m_auStorArea[p_uPageToWrite][1u] )  && ( 0x00u== m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x00u== m_auStorArea[p_uPageToWrite][3u] )  && ( 0x00u== m_auStorArea[p_uPageToWrite][4u] )  &&
            ( 0x00u== m_auStorArea[p_uPageToWrite][5u] )  && ( 0x00u== m_auStorArea[p_uPageToWrite][6u] )  && ( 0x00u== m_auStorArea[p_uPageToWrite][7u] ) &&
            ( 0x00u== m_auStorArea[p_uPageToWrite][8u] )  && ( 0x00u== m_auStorArea[p_uPageToWrite][9u] )  && ( 0x00u== m_auStorArea[p_uPageToWrite][10u] ) &&
            ( 0x00u== m_auStorArea[p_uPageToWrite][11u] ) && ( 0x00u== m_auStorArea[p_uPageToWrite][12u] ) && ( 0x00u== m_auStorArea[p_uPageToWrite][13u] ) &&
            ( 0x00u== m_auStorArea[p_uPageToWrite][14u] ) && ( 0x00u== m_auStorArea[p_uPageToWrite][15u] ) && ( 0x00u== m_auStorArea[p_uPageToWrite][16u] ) &&
            ( 0x00u== m_auStorArea[p_uPageToWrite][17u] ) && ( 0x00u== m_auStorArea[p_uPageToWrite][18u] ) && ( 0x00u== m_auStorArea[p_uPageToWrite][19u] ) &&
            ( 0x00u== m_auStorArea[p_uPageToWrite][20u] ) && ( 0x00u== m_auStorArea[p_uPageToWrite][21u] ) && ( 0x00u== m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0x00u== m_auStorArea[p_uPageToWrite][23u] ) )
        {
            if( ( 0x01u== l_ltUseBuff.puBuf[0u] )  && ( 0x02u== l_ltUseBuff.puBuf[1u] )  && ( 0x03u== l_ltUseBuff.puBuf[2u] ) &&
                ( 0x04u== l_ltUseBuff.puBuf[3u] )  && ( 0x05u== l_ltUseBuff.puBuf[4u] )  &&
                ( 0x01u== l_ltUseBuff2.puBuf[0u] )  && ( 0x02u== l_ltUseBuff2.puBuf[1u] )  && ( 0x03u== l_ltUseBuff2.puBuf[2u] ) &&
                ( 0x04u== l_ltUseBuff2.puBuf[3u] )  && ( 0x05u== l_ltUseBuff2.puBuf[4u] ) )
            {
                (void)printf("eFSS_LOGCTST_FlushTest 4  -- OK \n");
            }
            else
            {
                (void)printf("eFSS_LOGCTST_FlushTest 4  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_FlushTest 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_FlushTest 4  -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst2Adapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_LOGCTST_FlushTest 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_FlushTest 5  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_LOGCTST_FlushTest 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_FlushTest 6  -- FAIL \n");
    }

    /* Setup storage area */
    l_ltUseBuff2.uBufL = l_ltUseBuff.uBufL  + 1u;
    l_ltUseBuff2.puBuf = &l_auStor[24];

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    (void)memset(m_auStorArea[p_uPageToWrite], 0xFF, sizeof(m_auStorArea[p_uPageToWrite]));

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    l_uSubTypeWrite = 0x05u;
    if( e_eFSS_LOGC_RES_WRITENOMATCHREAD == eFSS_LOGC_FlushBufferAs(&l_tCtx, 0u, l_uSubTypeWrite) )
    {
        (void)printf("eFSS_LOGCTST_FlushTest 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_FlushTest 7  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    (void)memset(m_auStorArea[p_uPageToWrite], 0xFF, sizeof(m_auStorArea[p_uPageToWrite]));

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;
    l_ltUseBuff.puBuf[4u] = 0x05u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    l_uSubTypeWrite = 0x05u;
    if( e_eFSS_LOGC_RES_WRITENOMATCHREAD == eFSS_LOGC_FlushBufferAs(&l_tCtx, 1u, l_uSubTypeWrite) )
    {
        (void)printf("eFSS_LOGCTST_FlushTest 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_FlushTest 8  -- FAIL \n");
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
    (void)l_ltUseBuff.puBuf;
    (void)l_ltUseBuff2.puBuf;
}


static void eFSS_LOGCTST_Compare(void)
{
    /* Local variable */
    t_eFSS_LOGC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_uStorType;
    uint8_t l_auStor[56u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_LOGC_StorBuf l_ltUseBuff;
    t_eFSS_LOGC_StorBuf l_ltUseBuff2;
    uint8_t l_uSubTypeRead;
    bool_t l_bIsNewest;
    uint32_t l_uByteInPage;
    uint32_t l_uPageUsable;
    uint32_t l_uNewPIx;
    uint32_t l_uFillPIdx;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 2u;
    l_tStorSet.uPagesLen = 28u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;
    l_uStorType = 1u;
    l_bIsNewest = false;
    l_uByteInPage = 0u;
    l_uPageUsable = 0u;
    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_LOGCTST_Compare 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Compare 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_LOGCTST_Compare 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Compare 2  -- FAIL \n");
    }

    /* Setup storage area */
    l_ltUseBuff2.uBufL = l_ltUseBuff.uBufL  + 1u;
    l_ltUseBuff2.puBuf = &l_auStor[24];

    /* Setup storage area */
    l_uSubTypeRead = 0u;

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u] = 0x00u;  /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][6u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][7u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][8u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][9u] = 0x01u;  /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0x97u; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u] = 0x00u;  /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u] = 0x01u;  /* Page index */
    m_auStorArea[p_uPageToWrite][6u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][7u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][8u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][9u] = 0x01u;  /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0xD8u; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    /* fill the buffer */
    l_ltUseBuff.puBuf[0u] = 0x00u;
    l_ltUseBuff.puBuf[1u] = 0x00u;
    l_ltUseBuff.puBuf[2u] = 0x00u;
    l_ltUseBuff.puBuf[3u] = 0x00u;

    /* Compare */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_IsBuffEqualToPage(&l_tCtx, 0u, &l_bIsNewest, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0u == l_ltUseBuff.puBuf[3u] ) && ( true == l_bIsNewest ) && ( 0u == l_uSubTypeRead ) &&
            ( 0u == l_ltUseBuff2.puBuf[0u] ) && ( 0u == l_ltUseBuff2.puBuf[1u] ) && ( 0u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0u == l_ltUseBuff2.puBuf[3u] ) && ( 0u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_LOGCTST_Compare 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_Compare 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Compare 3  -- FAIL \n");
    }

    /* Compare */
    l_ltUseBuff.puBuf[0u] = 0x10u;
    l_ltUseBuff.puBuf[1u] = 0x10u;
    l_ltUseBuff.puBuf[2u] = 0x10u;
    l_ltUseBuff.puBuf[3u] = 0x10u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_IsBuffEqualToPage(&l_tCtx, 1u, &l_bIsNewest, &l_uSubTypeRead) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] ) && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] ) && ( true == l_bIsNewest ) && ( 0u == l_uSubTypeRead ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x00u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_LOGCTST_Compare 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_Compare 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Compare 4  -- FAIL \n");
    }

    /* Setup storage area */
    l_uSubTypeRead = 0u;

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u] = 0x03u;  /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][6u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][7u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][8u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][9u] = 0x01u;  /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0x9Au; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u] = 0x04u;  /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u] = 0x01u;  /* Page index */
    m_auStorArea[p_uPageToWrite][6u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][7u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][8u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][9u] = 0x01u;  /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0xDCu; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    /* fill the buffer */
    l_ltUseBuff.puBuf[0u] = 0x00u;
    l_ltUseBuff.puBuf[1u] = 0x00u;
    l_ltUseBuff.puBuf[2u] = 0x00u;
    l_ltUseBuff.puBuf[3u] = 0x00u;

    /* Compare */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_IsBuffEqualToPage(&l_tCtx, 0u, &l_bIsNewest, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0u == l_ltUseBuff.puBuf[3u] ) && ( true == l_bIsNewest ) && ( 3u == l_uSubTypeRead ) &&
            ( 0u == l_ltUseBuff2.puBuf[0u] ) && ( 0u == l_ltUseBuff2.puBuf[1u] ) && ( 0u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0u == l_ltUseBuff2.puBuf[3u] ) && ( 0x03u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_LOGCTST_Compare 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_Compare 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Compare 5  -- FAIL \n");
    }

    /* Compare */
    l_ltUseBuff.puBuf[0u] = 0x10u;
    l_ltUseBuff.puBuf[1u] = 0x10u;
    l_ltUseBuff.puBuf[2u] = 0x10u;
    l_ltUseBuff.puBuf[3u] = 0x10u;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_IsBuffEqualToPage(&l_tCtx, 1u, &l_bIsNewest, &l_uSubTypeRead) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] ) && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] ) && ( true == l_bIsNewest ) && ( 4u == l_uSubTypeRead ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_LOGCTST_Compare 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_Compare 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Compare 6  -- FAIL \n");
    }

    /* Setup storage area */
    l_uSubTypeRead = 0u;

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u] = 0x03u;  /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][6u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][7u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][8u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][9u] = 0x01u;  /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0x9Au; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u] = 0x04u;  /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u] = 0x01u;  /* Page index */
    m_auStorArea[p_uPageToWrite][6u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][7u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][8u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][9u] = 0x01u;  /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0xDCu; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    /* fill the buffer */
    l_ltUseBuff.puBuf[0u] = 0x00u;
    l_ltUseBuff.puBuf[1u] = 0x00u;
    l_ltUseBuff.puBuf[2u] = 0x00u;
    l_ltUseBuff.puBuf[3u] = 0x00u;

    /* Compare */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_IsBuffEqualToPage(&l_tCtx, 1u, &l_bIsNewest, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0u == l_ltUseBuff.puBuf[3u] ) && ( false == l_bIsNewest ) && ( 4u == l_uSubTypeRead ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_LOGCTST_Compare 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_Compare 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Compare 7  -- FAIL \n");
    }

    /* Compare */
    l_ltUseBuff.puBuf[0u] = 0x10u;
    l_ltUseBuff.puBuf[1u] = 0x10u;
    l_ltUseBuff.puBuf[2u] = 0x10u;
    l_ltUseBuff.puBuf[3u] = 0x10u;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_IsBuffEqualToPage(&l_tCtx, 0u, &l_bIsNewest, &l_uSubTypeRead) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] ) && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] ) && ( false == l_bIsNewest ) && ( 3u == l_uSubTypeRead ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[0u] ) && ( 0x00u == l_ltUseBuff2.puBuf[1u] ) && ( 0x00u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[3u] ) && ( 0x03u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_LOGCTST_Compare 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_Compare 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Compare 8  -- FAIL \n");
    }

    /* Setup storage area */
    l_uSubTypeRead = 0u;

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u] = 0x03u;  /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][6u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][7u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][8u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][9u] = 0x01u;  /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u] = 0x04u;  /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u] = 0x01u;  /* Page index */
    m_auStorArea[p_uPageToWrite][6u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][7u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][8u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][9u] = 0x01u;  /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    /* fill the buffer */
    l_ltUseBuff.puBuf[0u] = 0x00u;
    l_ltUseBuff.puBuf[1u] = 0x00u;
    l_ltUseBuff.puBuf[2u] = 0x00u;
    l_ltUseBuff.puBuf[3u] = 0x00u;

    /* Compare */
    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_IsBuffEqualToPage(&l_tCtx, 1u, &l_bIsNewest, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0u == l_ltUseBuff.puBuf[3u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_LOGCTST_Compare 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_Compare 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Compare 9  -- FAIL \n");
    }

    /* Compare */
    l_ltUseBuff.puBuf[0u] = 0x10u;
    l_ltUseBuff.puBuf[1u] = 0x10u;
    l_ltUseBuff.puBuf[2u] = 0x10u;
    l_ltUseBuff.puBuf[3u] = 0x10u;

    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_IsBuffEqualToPage(&l_tCtx, 0u, &l_bIsNewest, &l_uSubTypeRead) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] ) && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[0u] ) && ( 0x00u == l_ltUseBuff2.puBuf[1u] ) && ( 0x00u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[3u] ) && ( 0x03u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_LOGCTST_Compare 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_Compare 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Compare 10 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][5u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][6u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][7u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][8u] = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][9u] = 0x01u;  /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0x96u; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    /* Compare */
    l_ltUseBuff.puBuf[0u] = 0x10u;
    l_ltUseBuff.puBuf[1u] = 0x10u;
    l_ltUseBuff.puBuf[2u] = 0x10u;
    l_ltUseBuff.puBuf[3u] = 0x10u;

    if( e_eFSS_LOGC_RES_NEWVERSIONFOUND == eFSS_LOGC_IsBuffEqualToPage(&l_tCtx, 0u, &l_bIsNewest, &l_uSubTypeRead) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] ) && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[0u] ) && ( 0x00u == l_ltUseBuff2.puBuf[1u] ) && ( 0x00u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[3u] ) && ( 0x00u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_LOGCTST_Compare 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_Compare 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_Compare 11 -- FAIL \n");
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


static void eFSS_LOGCTST_LoadBkupTest(void)
{
    /* Local variable */
    t_eFSS_LOGC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_uStorType;
    uint8_t l_auStor[56u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_LOGC_StorBuf l_ltUseBuff;
    t_eFSS_LOGC_StorBuf l_ltUseBuff2;
    uint32_t l_uByteInPage;
    uint32_t l_uPageUsable;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 2u;
    l_tStorSet.uPagesLen = 28u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;
    l_uStorType = 1u;
    l_uByteInPage = 0u;
    l_uPageUsable = 0u;
    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_LOGCTST_LoadBkupTest 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadBkupTest 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_LOGCTST_LoadBkupTest 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadBkupTest 2  -- FAIL \n");
    }

    /* Setup storage area */
    l_ltUseBuff2.uBufL = l_ltUseBuff.uBufL  + 1u;
    l_ltUseBuff2.puBuf = &l_auStor[24];

    /* --------------------------------------------- All OK no backup generation */
    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x03u; /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u; /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0x9Au; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x04u; /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x01u; /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u; /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0x9Cu; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_LoadBufferAsNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        if( ( 0x00u == l_ltUseBuff.puBuf[0u] )  && ( 0x00u == l_ltUseBuff.puBuf[1u] ) && ( 0x00u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[3u] )  && ( 0x03u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x00u == l_ltUseBuff2.puBuf[0u] ) && ( 0x00u == l_ltUseBuff2.puBuf[1u] ) && ( 0x00u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadBkupTest 3  -- FAIL \n");
    }

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_LoadBufferAsNRipBkp(&l_tCtx, 1u, 0u, 0x04u, 0x03u) )
    {
        if( ( 0x00u == l_ltUseBuff.puBuf[0u] )  && ( 0x00u == l_ltUseBuff.puBuf[1u] ) && ( 0x00u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[3u] )  && ( 0x04u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x00u == l_ltUseBuff2.puBuf[0u] ) && ( 0x00u == l_ltUseBuff2.puBuf[1u] ) && ( 0x00u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[3u] ) && ( 0x03u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadBkupTest 4  -- FAIL \n");
    }


    /* --------------------------------------------- All OK no backup generation */
    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 1, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x03u; /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u; /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0x9Eu; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[p_uPageToWrite], 1, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x04u; /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x01u; /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u; /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0xA0u; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_LoadBufferAsNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        if( ( 0x01u == l_ltUseBuff.puBuf[0u] )  && ( 0x01u == l_ltUseBuff.puBuf[1u] ) && ( 0x01u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x01u == l_ltUseBuff.puBuf[3u] )  && ( 0x03u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x01u == l_ltUseBuff2.puBuf[0u] ) && ( 0x01u == l_ltUseBuff2.puBuf[1u] ) && ( 0x01u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x01u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadBkupTest 5  -- FAIL \n");
    }

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_LoadBufferAsNRipBkp(&l_tCtx, 1u, 0u, 0x04u, 0x03u) )
    {
        if( ( 0x01u == l_ltUseBuff.puBuf[0u] )  && ( 0x01u == l_ltUseBuff.puBuf[1u] ) && ( 0x01u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x01u == l_ltUseBuff.puBuf[3u] )  && ( 0x04u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x01u == l_ltUseBuff2.puBuf[0u] ) && ( 0x01u == l_ltUseBuff2.puBuf[1u] ) && ( 0x01u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x01u == l_ltUseBuff2.puBuf[3u] ) && ( 0x03u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadBkupTest 6  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x03u;  /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u;  /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0x9Au; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x04u;  /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x01u;  /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u;  /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0xDCu; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    if( e_eFSS_LOGC_RES_OK_BKP_RCVRD == eFSS_LOGC_LoadBufferAsNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        if( ( 0x00u == l_ltUseBuff.puBuf[0u] )  && ( 0x00u == l_ltUseBuff.puBuf[1u] ) && ( 0x00u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[3u] )  && ( 0x03u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x00u == l_ltUseBuff2.puBuf[0u] ) && ( 0x00u == l_ltUseBuff2.puBuf[1u] ) && ( 0x00u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][0u] )  && ( 0x00u == m_auStorArea[p_uPageToWrite][1u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][3u] )  && ( 0x03u == m_auStorArea[p_uPageToWrite][4u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][5u] ) &&
            ( 0x9Au == m_auStorArea[p_uPageToWrite][20u] )  && ( 0x02u == m_auStorArea[p_uPageToWrite][21u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][23u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][0u] )  && ( 0x00u == m_auStorArea[p_uPageToWrite][1u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][3u] )  && ( 0x04u == m_auStorArea[p_uPageToWrite][4u] ) && ( 0x01u == m_auStorArea[p_uPageToWrite][5u] ) &&
            ( 0x9Cu == m_auStorArea[p_uPageToWrite][20u] )  && ( 0x02u == m_auStorArea[p_uPageToWrite][21u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][23u] ) )
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadBkupTest 7  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x03u;  /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u;  /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0x9Au; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x04u;  /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x01u;  /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u;  /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u;  /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0xDCu; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    if( e_eFSS_LOGC_RES_OK_BKP_RCVRD == eFSS_LOGC_LoadBufferAsNRipBkp(&l_tCtx, 1u, 0u, 0x04u, 0x03u) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] )  && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] )  && ( 0x04u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x03u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][0u] )  && ( 0x10u == m_auStorArea[p_uPageToWrite][1u] ) && ( 0x10u == m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][3u] )  && ( 0x03u == m_auStorArea[p_uPageToWrite][4u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][5u] ) &&
            ( 0xDAu == m_auStorArea[p_uPageToWrite][20u] )  && ( 0x02u == m_auStorArea[p_uPageToWrite][21u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][23u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][0u] )  && ( 0x10u == m_auStorArea[p_uPageToWrite][1u] ) && ( 0x10u == m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][3u] )  && ( 0x04u == m_auStorArea[p_uPageToWrite][4u] ) && ( 0x01u == m_auStorArea[p_uPageToWrite][5u] ) &&
            ( 0xDCu == m_auStorArea[p_uPageToWrite][20u] )  && ( 0x02u == m_auStorArea[p_uPageToWrite][21u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][23u] ) )
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadBkupTest 8  -- FAIL \n");
    }

    /* Original corrupted, backup correct */
    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x03u; /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u; /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0xFFu; /* CRC */ /* Correct = 0x9Au */
    m_auStorArea[p_uPageToWrite][21u] = 0xFFu; /* CRC */ /* Correct = 0x02u */
    m_auStorArea[p_uPageToWrite][22u] = 0xFFu; /* CRC */ /* Correct = 0x00u */
    m_auStorArea[p_uPageToWrite][23u] = 0xFFu; /* CRC */ /* Correct = 0x00u */

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x04u; /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x01u; /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u; /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0xDCu; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    if( e_eFSS_LOGC_RES_OK_BKP_RCVRD == eFSS_LOGC_LoadBufferAsNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] )  && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] )  && ( 0x03u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][0u] )  && ( 0x10u == m_auStorArea[p_uPageToWrite][1u] ) && ( 0x10u == m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][3u] )  && ( 0x03u == m_auStorArea[p_uPageToWrite][4u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][5u] ) &&
            ( 0xDAu == m_auStorArea[p_uPageToWrite][20u] )  && ( 0x02u == m_auStorArea[p_uPageToWrite][21u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][23u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][0u] )  && ( 0x10u == m_auStorArea[p_uPageToWrite][1u] ) && ( 0x10u == m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][3u] )  && ( 0x04u == m_auStorArea[p_uPageToWrite][4u] ) && ( 0x01u == m_auStorArea[p_uPageToWrite][5u] ) &&
            ( 0xDCu == m_auStorArea[p_uPageToWrite][20u] )  && ( 0x02u == m_auStorArea[p_uPageToWrite][21u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][23u] ) )
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadBkupTest 9  -- FAIL \n");
    }

    /* Original corrupted, backup correct */
    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x03u; /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u; /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0x9Au; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x04u; /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x01u; /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u; /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0xDCu; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    if( e_eFSS_LOGC_RES_OK_BKP_RCVRD == eFSS_LOGC_LoadBufferAsNRipBkp(&l_tCtx, 1u, 0u, 0xFFu, 0x03u) )
    {
        if( ( 0x00u == l_ltUseBuff.puBuf[0u] )  && ( 0x00u == l_ltUseBuff.puBuf[1u] ) && ( 0x00u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[3u] )  && ( 0xFFu == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x00u == l_ltUseBuff2.puBuf[0u] ) && ( 0x00u == l_ltUseBuff2.puBuf[1u] ) && ( 0x00u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[3u] ) && ( 0x03u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][0u] )  && ( 0x00u == m_auStorArea[p_uPageToWrite][1u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][3u] )  && ( 0x03u == m_auStorArea[p_uPageToWrite][4u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][5u] ) &&
            ( 0x9Au == m_auStorArea[p_uPageToWrite][20u] )  && ( 0x02u == m_auStorArea[p_uPageToWrite][21u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][23u] )  &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][0u] )   && ( 0x00u == m_auStorArea[p_uPageToWrite][1u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][3u] )   && ( 0xFFu == m_auStorArea[p_uPageToWrite][4u] ) && ( 0x01u == m_auStorArea[p_uPageToWrite][5u] ) &&
            ( 0x97u == m_auStorArea[p_uPageToWrite][20u] )  && ( 0x03u == m_auStorArea[p_uPageToWrite][21u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][23u] ) )
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadBkupTest 10 -- FAIL \n");
    }

    /* Backup corrupted, original correct */
    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x03u; /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u; /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0xFFu; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0xFFu; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0xFFu; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0xFFu; /* CRC */

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x04u; /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x01u; /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u; /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0xDCu; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    if( e_eFSS_LOGC_RES_OK_BKP_RCVRD == eFSS_LOGC_LoadBufferAsNRipBkp(&l_tCtx, 1u, 0u, 0x04u, 0x03u) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] )  && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] )  && ( 0x04u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x03u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][0u] )  && ( 0x10u == m_auStorArea[p_uPageToWrite][1u] ) && ( 0x10u == m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][3u] )  && ( 0x03u == m_auStorArea[p_uPageToWrite][4u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][5u] ) &&
            ( 0xDAu == m_auStorArea[p_uPageToWrite][20u] )  && ( 0x02u == m_auStorArea[p_uPageToWrite][21u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][23u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][0u] )  && ( 0x10u == m_auStorArea[p_uPageToWrite][1u] ) && ( 0x10u == m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][3u] )  && ( 0x04u == m_auStorArea[p_uPageToWrite][4u] ) && ( 0x01u == m_auStorArea[p_uPageToWrite][5u] ) &&
            ( 0xDCu == m_auStorArea[p_uPageToWrite][20u] )  && ( 0x02u == m_auStorArea[p_uPageToWrite][21u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][23u] ) )
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadBkupTest 11 -- FAIL \n");
    }

    /* Original corrupted, backup correct */
    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x03u; /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u; /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0x9Au; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x04u; /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x01u; /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u; /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0xDCu; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    if( e_eFSS_LOGC_RES_OK_BKP_RCVRD == eFSS_LOGC_LoadBufferAsNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0xFFu) )
    {
        if( ( 0x00u == l_ltUseBuff.puBuf[0u] )  && ( 0x00u == l_ltUseBuff.puBuf[1u] ) && ( 0x00u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[3u] )  && ( 0x03u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x00u == l_ltUseBuff2.puBuf[0u] ) && ( 0x00u == l_ltUseBuff2.puBuf[1u] ) && ( 0x00u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[3u] ) && ( 0xFFu == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][0u] )  && ( 0x00u == m_auStorArea[p_uPageToWrite][1u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][3u] )  && ( 0x03u == m_auStorArea[p_uPageToWrite][4u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][5u] ) &&
            ( 0x9Au == m_auStorArea[p_uPageToWrite][20u] )  && ( 0x02u == m_auStorArea[p_uPageToWrite][21u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][23u] )  &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][0u] )   && ( 0x00u == m_auStorArea[p_uPageToWrite][1u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][3u] )   && ( 0xFFu == m_auStorArea[p_uPageToWrite][4u] ) && ( 0x01u == m_auStorArea[p_uPageToWrite][5u] ) &&
            ( 0x97u == m_auStorArea[p_uPageToWrite][20u] )  && ( 0x03u == m_auStorArea[p_uPageToWrite][21u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][23u] ) )
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadBkupTest 12 -- FAIL \n");
    }

    /* Nothing OK, but new  */
    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x03u; /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u; /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x02u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0x9Bu; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x04u; /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x01u; /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u; /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x02u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0xDDu; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    if( e_eFSS_LOGC_RES_NEWVERSIONFOUND == eFSS_LOGC_LoadBufferAsNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        if( ( 0x00u == l_ltUseBuff.puBuf[0u] )  && ( 0x00u == l_ltUseBuff.puBuf[1u] ) && ( 0x00u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[3u] )  && ( 0x03u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][0u] )  && ( 0x00u == m_auStorArea[p_uPageToWrite][1u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][3u] )  && ( 0x03u == m_auStorArea[p_uPageToWrite][4u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][5u] ) &&
            ( 0x9Bu == m_auStorArea[p_uPageToWrite][20u] )  && ( 0x02u == m_auStorArea[p_uPageToWrite][21u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][23u] )  &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][0u] )   && ( 0x10u == m_auStorArea[p_uPageToWrite][1u] ) && ( 0x10u == m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][3u] )   && ( 0x04u == m_auStorArea[p_uPageToWrite][4u] ) && ( 0x01u == m_auStorArea[p_uPageToWrite][5u] ) &&
            ( 0xDDu == m_auStorArea[p_uPageToWrite][20u] )  && ( 0x02u == m_auStorArea[p_uPageToWrite][21u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][23u] ) )
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadBkupTest 13 -- FAIL \n");
    }

    /* Original corrupted, backup correct */
    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x03u; /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u; /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x02u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0x9Bu; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x04u; /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x01u; /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u; /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0xDCu; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    if( e_eFSS_LOGC_RES_OK_BKP_RCVRD == eFSS_LOGC_LoadBufferAsNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] )  && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] )  && ( 0x03u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][0u] )  && ( 0x10u == m_auStorArea[p_uPageToWrite][1u] ) && ( 0x10u == m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][3u] )  && ( 0x03u == m_auStorArea[p_uPageToWrite][4u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][5u] ) &&
            ( 0xDAu == m_auStorArea[p_uPageToWrite][20u] )  && ( 0x02u == m_auStorArea[p_uPageToWrite][21u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][23u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][0u] )  && ( 0x10u == m_auStorArea[p_uPageToWrite][1u] ) && ( 0x10u == m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][3u] )  && ( 0x04u == m_auStorArea[p_uPageToWrite][4u] ) && ( 0x01u == m_auStorArea[p_uPageToWrite][5u] ) &&
            ( 0xDCu == m_auStorArea[p_uPageToWrite][20u] )  && ( 0x02u == m_auStorArea[p_uPageToWrite][21u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][23u] ) )
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadBkupTest 14 -- FAIL \n");
    }

    /* Backup corrupted, original correct */
    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x03u; /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u; /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x02u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0x9Bu; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x04u; /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x01u; /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u; /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x01u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0xDCu; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    if( e_eFSS_LOGC_RES_OK_BKP_RCVRD == eFSS_LOGC_LoadBufferAsNRipBkp(&l_tCtx, 1u, 0u, 0x04u, 0x03u) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] )  && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] )  && ( 0x04u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x03u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][0u] )  && ( 0x10u == m_auStorArea[p_uPageToWrite][1u] ) && ( 0x10u == m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][3u] )  && ( 0x03u == m_auStorArea[p_uPageToWrite][4u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][5u] ) &&
            ( 0xDAu == m_auStorArea[p_uPageToWrite][20u] )  && ( 0x02u == m_auStorArea[p_uPageToWrite][21u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][23u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][0u] )  && ( 0x10u == m_auStorArea[p_uPageToWrite][1u] ) && ( 0x10u == m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][3u] )  && ( 0x04u == m_auStorArea[p_uPageToWrite][4u] ) && ( 0x01u == m_auStorArea[p_uPageToWrite][5u] ) &&
            ( 0xDCu == m_auStorArea[p_uPageToWrite][20u] )  && ( 0x02u == m_auStorArea[p_uPageToWrite][21u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][23u] ) )
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadBkupTest 14 -- FAIL \n");
    }

    /* Nothing OK, but new  */
    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x03u; /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u; /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x02u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0x9Bu; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0x02u; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0x00u; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x04u; /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x01u; /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u; /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x02u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0xFFu; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0xFFu; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0xFFu; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0xFFu; /* CRC */

    if( e_eFSS_LOGC_RES_NEWVERSIONFOUND == eFSS_LOGC_LoadBufferAsNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        if( ( 0x00u == l_ltUseBuff.puBuf[0u] )  && ( 0x00u == l_ltUseBuff.puBuf[1u] ) && ( 0x00u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[3u] )  && ( 0x03u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][0u] )  && ( 0x00u == m_auStorArea[p_uPageToWrite][1u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][3u] )  && ( 0x03u == m_auStorArea[p_uPageToWrite][4u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][5u] ) &&
            ( 0x9Bu == m_auStorArea[p_uPageToWrite][20u] )  && ( 0x02u == m_auStorArea[p_uPageToWrite][21u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][23u] )  &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][0u] )   && ( 0x10u == m_auStorArea[p_uPageToWrite][1u] ) && ( 0x10u == m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][3u] )   && ( 0x04u == m_auStorArea[p_uPageToWrite][4u] ) && ( 0x01u == m_auStorArea[p_uPageToWrite][5u] ) &&
            ( 0xFFu == m_auStorArea[p_uPageToWrite][20u] )  && ( 0xFFu == m_auStorArea[p_uPageToWrite][21u] ) && ( 0xFFu == m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0xFFu == m_auStorArea[p_uPageToWrite][23u] ) )
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadBkupTest 15 -- FAIL \n");
    }

    /* Nothing OK, but new  */
    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x03u; /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u; /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x02u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0xFFu; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0xFFu; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0xFFu; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0xFFu; /* CRC */

    (void)memset(m_auStorArea[p_uPageToWrite], 0x10, sizeof(m_auStorArea[p_uPageToWrite]));
    m_auStorArea[p_uPageToWrite][4u]  = 0x04u; /* Page SUBTYPE */
    m_auStorArea[p_uPageToWrite][5u]  = 0x01u; /* Page index */
    m_auStorArea[p_uPageToWrite][6u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][7u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][8u]  = 0x00u; /* Page index */
    m_auStorArea[p_uPageToWrite][9u]  = 0x01u; /* Page type */
    m_auStorArea[p_uPageToWrite][10u] = 0x02u; /* Page version */
    m_auStorArea[p_uPageToWrite][11u] = 0x00u; /* Page version */
    m_auStorArea[p_uPageToWrite][12u] = 0x02u; /* Total page */
    m_auStorArea[p_uPageToWrite][13u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][14u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][15u] = 0x00u; /* Total page */
    m_auStorArea[p_uPageToWrite][16u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][17u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][18u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][19u] = 0xA5u; /* Magic number */
    m_auStorArea[p_uPageToWrite][20u] = 0xFFu; /* CRC */
    m_auStorArea[p_uPageToWrite][21u] = 0xFFu; /* CRC */
    m_auStorArea[p_uPageToWrite][22u] = 0xFFu; /* CRC */
    m_auStorArea[p_uPageToWrite][23u] = 0xFFu; /* CRC */

    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_LoadBufferAsNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        if( ( 0x00u == l_ltUseBuff.puBuf[0u] )  && ( 0x00u == l_ltUseBuff.puBuf[1u] ) && ( 0x00u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[3u] )  && ( 0x03u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][0u] )  && ( 0x00u == m_auStorArea[p_uPageToWrite][1u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x00u == m_auStorArea[p_uPageToWrite][3u] )  && ( 0x03u == m_auStorArea[p_uPageToWrite][4u] ) && ( 0x00u == m_auStorArea[p_uPageToWrite][5u] ) &&
            ( 0xFFu == m_auStorArea[p_uPageToWrite][20u] )  && ( 0xFFu == m_auStorArea[p_uPageToWrite][21u] ) && ( 0xFFu == m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0xFFu == m_auStorArea[p_uPageToWrite][23u] )  &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][0u] )   && ( 0x10u == m_auStorArea[p_uPageToWrite][1u] ) && ( 0x10u == m_auStorArea[p_uPageToWrite][2u] ) &&
            ( 0x10u == m_auStorArea[p_uPageToWrite][3u] )   && ( 0x04u == m_auStorArea[p_uPageToWrite][4u] ) && ( 0x01u == m_auStorArea[p_uPageToWrite][5u] ) &&
            ( 0xFFu == m_auStorArea[p_uPageToWrite][20u] )  && ( 0xFFu == m_auStorArea[p_uPageToWrite][21u] ) && ( 0xFFu == m_auStorArea[p_uPageToWrite][22u] ) &&
            ( 0xFFu == m_auStorArea[p_uPageToWrite][23u] ) )
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 16 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_LOGCTST_LoadBkupTest 16 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_LoadBkupTest 16 -- FAIL \n");
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

static void eFSS_LOGCTST_FlushBkupTest(void)
{
    /* Local variable */
    t_eFSS_LOGC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_uStorType;
    uint8_t l_auStor[56u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_LOGC_StorBuf l_ltUseBuff;
    t_eFSS_LOGC_StorBuf l_ltUseBuff2;
    uint32_t l_uByteInPage;
    uint32_t l_uPageUsable;
    uint32_t l_uNewPIx;
    uint32_t l_uFillPIdx;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 2u;
    l_tStorSet.uPagesLen = 28u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;
    l_uStorType = 1u;
    l_uByteInPage = 0u;
    l_uPageUsable = 0u;
    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_LOGCTST_FlushBkupTest 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_FlushBkupTest 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_LOGCTST_FlushBkupTest 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_FlushBkupTest 2  -- FAIL \n");
    }

    /* Setup storage area */
    l_ltUseBuff2.uBufL = l_ltUseBuff.uBufL  + 1u;
    l_ltUseBuff2.puBuf = &l_auStor[24];


    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    (void)memset(l_ltUseBuff.puBuf, 0x10, l_ltUseBuff.uBufL);


    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_FlushBufferAsNBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] )  && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] )  && ( 0x03u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) )
        {
            if( ( m_auStorArea[p_uPageToWrite][0u]  == 0x10u ) && ( m_auStorArea[p_uPageToWrite][1u]  == 0x10u ) && ( m_auStorArea[p_uPageToWrite][2u]  == 0x10u ) &&
                ( m_auStorArea[p_uPageToWrite][3u]  == 0x10u ) && ( m_auStorArea[p_uPageToWrite][4u]  == 0x03u ) && ( m_auStorArea[p_uPageToWrite][5u]  == 0x00u ) &&
                ( m_auStorArea[p_uPageToWrite][6u]  == 0x00u ) && ( m_auStorArea[p_uPageToWrite][7u]  == 0x00u ) && ( m_auStorArea[p_uPageToWrite][8u]  == 0x00u ) &&
                ( m_auStorArea[p_uPageToWrite][9u]  == 0x01u ) && ( m_auStorArea[p_uPageToWrite][10u] == 0x01u ) && ( m_auStorArea[p_uPageToWrite][11u] == 0x00u ) &&
                ( m_auStorArea[p_uPageToWrite][12u] == 0x02u ) && ( m_auStorArea[p_uPageToWrite][13u] == 0x00u ) && ( m_auStorArea[p_uPageToWrite][14u] == 0x00u ) &&
                ( m_auStorArea[p_uPageToWrite][15u] == 0x00u ) && ( m_auStorArea[p_uPageToWrite][16u] == 0xA5u ) && ( m_auStorArea[p_uPageToWrite][17u] == 0xA5u ) &&
                ( m_auStorArea[p_uPageToWrite][18u] == 0xA5u ) && ( m_auStorArea[p_uPageToWrite][19u] == 0xA5u ) && ( m_auStorArea[p_uPageToWrite][20u] == 0xDAu ) &&
                ( m_auStorArea[p_uPageToWrite][21u] == 0x02u ) && ( m_auStorArea[p_uPageToWrite][22u] == 0x00u ) && ( m_auStorArea[p_uPageToWrite][23u] == 0x00u ) &&
                ( m_auStorArea[p_uPageToWrite][0u]  == 0x10u ) && ( m_auStorArea[p_uPageToWrite][1u]  == 0x10u ) && ( m_auStorArea[p_uPageToWrite][2u]  == 0x10u ) &&
                ( m_auStorArea[p_uPageToWrite][3u]  == 0x10u ) && ( m_auStorArea[p_uPageToWrite][4u]  == 0x04u ) && ( m_auStorArea[p_uPageToWrite][5u]  == 0x01u ) &&
                ( m_auStorArea[p_uPageToWrite][6u]  == 0x00u ) && ( m_auStorArea[p_uPageToWrite][7u]  == 0x00u ) && ( m_auStorArea[p_uPageToWrite][8u]  == 0x00u ) &&
                ( m_auStorArea[p_uPageToWrite][9u]  == 0x01u ) && ( m_auStorArea[p_uPageToWrite][10u] == 0x01u ) && ( m_auStorArea[p_uPageToWrite][11u] == 0x00u ) &&
                ( m_auStorArea[p_uPageToWrite][12u] == 0x02u ) && ( m_auStorArea[p_uPageToWrite][13u] == 0x00u ) && ( m_auStorArea[p_uPageToWrite][14u] == 0x00u ) &&
                ( m_auStorArea[p_uPageToWrite][15u] == 0x00u ) && ( m_auStorArea[p_uPageToWrite][16u] == 0xA5u ) && ( m_auStorArea[p_uPageToWrite][17u] == 0xA5u ) &&
                ( m_auStorArea[p_uPageToWrite][18u] == 0xA5u ) && ( m_auStorArea[p_uPageToWrite][19u] == 0xA5u ) && ( m_auStorArea[p_uPageToWrite][20u] == 0xDCu ) &&
                ( m_auStorArea[p_uPageToWrite][21u] == 0x02u ) && ( m_auStorArea[p_uPageToWrite][22u] == 0x00u ) && ( m_auStorArea[p_uPageToWrite][23u] == 0x00u ) )
              {
                  (void)printf("eFSS_LOGCTST_FlushBkupTest 3  -- OK \n");
              }
              else
              {
                  (void)printf("eFSS_LOGCTST_FlushBkupTest 3  -- FAIL \n");
              }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_FlushBkupTest 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_FlushBkupTest 3  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    (void)memset(l_ltUseBuff.puBuf, 0x10, l_ltUseBuff.uBufL);


    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_FlushBufferAsNBkp(&l_tCtx, 1u, 0u, 0x04u, 0x03u) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] )  && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] )  && ( 0x04u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x03u == l_ltUseBuff2.puBuf[4u] ) )
        {
            if( ( m_auStorArea[p_uPageToWrite][0u]  == 0x10u ) && ( m_auStorArea[p_uPageToWrite][1u]  == 0x10u ) && ( m_auStorArea[p_uPageToWrite][2u]  == 0x10u ) &&
                ( m_auStorArea[p_uPageToWrite][3u]  == 0x10u ) && ( m_auStorArea[p_uPageToWrite][4u]  == 0x04u ) && ( m_auStorArea[p_uPageToWrite][5u]  == 0x01u ) &&
                ( m_auStorArea[p_uPageToWrite][6u]  == 0x00u ) && ( m_auStorArea[p_uPageToWrite][7u]  == 0x00u ) && ( m_auStorArea[p_uPageToWrite][8u]  == 0x00u ) &&
                ( m_auStorArea[p_uPageToWrite][9u]  == 0x01u ) && ( m_auStorArea[p_uPageToWrite][10u] == 0x01u ) && ( m_auStorArea[p_uPageToWrite][11u] == 0x00u ) &&
                ( m_auStorArea[p_uPageToWrite][12u] == 0x02u ) && ( m_auStorArea[p_uPageToWrite][13u] == 0x00u ) && ( m_auStorArea[p_uPageToWrite][14u] == 0x00u ) &&
                ( m_auStorArea[p_uPageToWrite][15u] == 0x00u ) && ( m_auStorArea[p_uPageToWrite][16u] == 0xA5u ) && ( m_auStorArea[p_uPageToWrite][17u] == 0xA5u ) &&
                ( m_auStorArea[p_uPageToWrite][18u] == 0xA5u ) && ( m_auStorArea[p_uPageToWrite][19u] == 0xA5u ) && ( m_auStorArea[p_uPageToWrite][20u] == 0xDCu ) &&
                ( m_auStorArea[p_uPageToWrite][21u] == 0x02u ) && ( m_auStorArea[p_uPageToWrite][22u] == 0x00u ) && ( m_auStorArea[p_uPageToWrite][23u] == 0x00u ) &&
                ( m_auStorArea[p_uPageToWrite][0u]  == 0x10u ) && ( m_auStorArea[p_uPageToWrite][1u]  == 0x10u ) && ( m_auStorArea[p_uPageToWrite][2u]  == 0x10u ) &&
                ( m_auStorArea[p_uPageToWrite][3u]  == 0x10u ) && ( m_auStorArea[p_uPageToWrite][4u]  == 0x03u ) && ( m_auStorArea[p_uPageToWrite][5u]  == 0x00u ) &&
                ( m_auStorArea[p_uPageToWrite][6u]  == 0x00u ) && ( m_auStorArea[p_uPageToWrite][7u]  == 0x00u ) && ( m_auStorArea[p_uPageToWrite][8u]  == 0x00u ) &&
                ( m_auStorArea[p_uPageToWrite][9u]  == 0x01u ) && ( m_auStorArea[p_uPageToWrite][10u] == 0x01u ) && ( m_auStorArea[p_uPageToWrite][11u] == 0x00u ) &&
                ( m_auStorArea[p_uPageToWrite][12u] == 0x02u ) && ( m_auStorArea[p_uPageToWrite][13u] == 0x00u ) && ( m_auStorArea[p_uPageToWrite][14u] == 0x00u ) &&
                ( m_auStorArea[p_uPageToWrite][15u] == 0x00u ) && ( m_auStorArea[p_uPageToWrite][16u] == 0xA5u ) && ( m_auStorArea[p_uPageToWrite][17u] == 0xA5u ) &&
                ( m_auStorArea[p_uPageToWrite][18u] == 0xA5u ) && ( m_auStorArea[p_uPageToWrite][19u] == 0xA5u ) && ( m_auStorArea[p_uPageToWrite][20u] == 0xDAu ) &&
                ( m_auStorArea[p_uPageToWrite][21u] == 0x02u ) && ( m_auStorArea[p_uPageToWrite][22u] == 0x00u ) && ( m_auStorArea[p_uPageToWrite][23u] == 0x00u ) )
              {
                  (void)printf("eFSS_LOGCTST_FlushBkupTest 4  -- OK \n");
              }
              else
              {
                  (void)printf("eFSS_LOGCTST_FlushBkupTest 4  -- FAIL \n");
              }
        }
        else
        {
            (void)printf("eFSS_LOGCTST_FlushBkupTest 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_LOGCTST_FlushBkupTest 4  -- FAIL \n");
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

static void eFSS_LOGCTST_GenTest(void)
{
    /* Local variable */
    t_eFSS_LOGC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_uStorType;
    uint8_t l_auStor[56u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_LOGC_StorBuf l_ltUseBuff;
    uint8_t l_uSubTypeRead;
    uint32_t l_uByteInPage;
    uint32_t l_uPageUsable;
    uint32_t l_uNewPIx;
    uint32_t l_uFillPIdx;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 2u;
    l_tStorSet.uPagesLen = 28u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;
    l_uStorType = 1u;
    l_uSubTypeRead = 0u;
    l_uByteInPage = 0u;
    l_uPageUsable = 0u;
    l_uNewPIx = 0u;
    l_uFillPIdx = 0u;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_LOGCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_LOGCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_LOGCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_LOGCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_LOGC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_LOGC_RES_OK;

    /* Setup storage area */
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));
    (void)memset(m_auStorArea[p_uPageToWrite], 0, sizeof(m_auStorArea[p_uPageToWrite]));

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_LOGCTST_GenTest 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_GenTest 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_LOGCTST_GenTest 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_GenTest 2  -- FAIL \n");
    }

    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_LoadBufferAs(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        (void)printf("eFSS_LOGCTST_GenTest 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_GenTest 3  -- FAIL \n");
    }

    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_LoadBufferAs(&l_tCtx, 1u, &l_uSubTypeRead) )
    {
        (void)printf("eFSS_LOGCTST_GenTest 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_GenTest 4  -- FAIL \n");
    }

    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_FlushBufferAs(&l_tCtx, 0u, 0x03u) )
    {
        (void)printf("eFSS_LOGCTST_GenTest 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_GenTest 5  -- FAIL \n");
    }

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_FlushBufferAs(&l_tCtx, 1u, 0x04u) )
    {
        (void)printf("eFSS_LOGCTST_GenTest 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_GenTest 6  -- FAIL \n");
    }

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_LoadBufferAsNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        (void)printf("eFSS_LOGCTST_GenTest 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_GenTest 7  -- FAIL \n");
    }

    if( ( 0x01u == l_ltUseBuff.puBuf[0u] ) && ( 0x02u == l_ltUseBuff.puBuf[1u] ) && ( 0x03u == l_ltUseBuff.puBuf[2u] ) && ( 0x04u == l_ltUseBuff.puBuf[3u] ) )
    {
        (void)printf("eFSS_LOGCTST_GenTest 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_GenTest 8  -- FAIL \n");
    }

    l_tStorSet.uPageVersion = 2u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_LOGCTST_GenTest 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_GenTest 9  -- FAIL \n");
    }

    if( e_eFSS_LOGC_RES_NEWVERSIONFOUND == eFSS_LOGC_LoadBufferAsNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        (void)printf("eFSS_LOGCTST_GenTest 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_GenTest 10 -- FAIL \n");
    }

    l_tStorSet.uPageVersion = 1u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_LOGCTST_GenTest 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_GenTest 11 -- FAIL \n");
    }

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_LoadBufferAsNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        (void)printf("eFSS_LOGCTST_GenTest 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_GenTest 12 -- FAIL \n");
    }

    if( ( 0x01u == l_ltUseBuff.puBuf[0u] ) && ( 0x02u == l_ltUseBuff.puBuf[1u] ) && ( 0x03u == l_ltUseBuff.puBuf[2u] ) && ( 0x04u == l_ltUseBuff.puBuf[3u] ) )
    {
        (void)printf("eFSS_LOGCTST_GenTest 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_GenTest 13 -- FAIL \n");
    }

    l_tStorSet.uTotPages = 3u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_LOGCTST_GenTest 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_GenTest 14 -- FAIL \n");
    }

    if( e_eFSS_LOGC_RES_NOTVALIDLOG == eFSS_LOGC_LoadBufferAsNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        (void)printf("eFSS_LOGCTST_GenTest 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_GenTest 15 -- FAIL \n");
    }

    l_tStorSet.uTotPages = 2u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_uStorType, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_LOGCTST_GenTest 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_GenTest 16 -- FAIL \n");
    }

    l_ltUseBuff.puBuf[0u] = 0x11u;
    l_ltUseBuff.puBuf[1u] = 0x12u;
    l_ltUseBuff.puBuf[2u] = 0x13u;
    l_ltUseBuff.puBuf[3u] = 0x14u;
    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_FlushBufferAsNBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        (void)printf("eFSS_LOGCTST_GenTest 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_GenTest 17 -- FAIL \n");
    }

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_LoadBufferAs(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        (void)printf("eFSS_LOGCTST_GenTest 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_GenTest 18 -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff.puBuf[0u] ) && ( 0x12u == l_ltUseBuff.puBuf[1u] ) && ( 0x13u == l_ltUseBuff.puBuf[2u] ) && ( 0x14u == l_ltUseBuff.puBuf[3u] ) &&
        ( 0x03u == l_uSubTypeRead ) )
    {
        (void)printf("eFSS_LOGCTST_GenTest 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_GenTest 19 -- FAIL \n");
    }

    if( e_eFSS_LOGC_RES_OK == eFSS_LOGC_LoadBufferAs(&l_tCtx, 1u, &l_uSubTypeRead) )
    {
        (void)printf("eFSS_LOGCTST_GenTest 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_GenTest 20 -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff.puBuf[0u] ) && ( 0x12u == l_ltUseBuff.puBuf[1u] ) && ( 0x13u == l_ltUseBuff.puBuf[2u] ) && ( 0x14u == l_ltUseBuff.puBuf[3u] ) &&
        ( 0x04u == l_uSubTypeRead ) )
    {
        (void)printf("eFSS_LOGCTST_GenTest 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_LOGCTST_GenTest 21 -- FAIL \n");
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

#endif