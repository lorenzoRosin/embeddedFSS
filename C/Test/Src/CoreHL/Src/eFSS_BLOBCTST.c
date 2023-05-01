/**
 * @file       eFSS_BLOBCTST.c
 *
 * @brief      Blob Core - large file core module test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_BLOBCTST.h"
#include "eFSS_BLOBC.h"
#include <stdio.h>



/***********************************************************************************************************************
 *   PRIVATE TEST STRUCT DECLARATION
 **********************************************************************************************************************/
struct t_eFSS_TYPE_EraseCtxUser
{
    e_eFSS_BLOBC_RES eLastEr;
    uint32_t uTimeUsed;
};

struct t_eFSS_TYPE_WriteCtxUser
{
    e_eFSS_BLOBC_RES eLastEr;
    uint32_t uTimeUsed;
};

struct t_eFSS_TYPE_ReadCtxUser
{
    e_eFSS_BLOBC_RES eLastEr;
    uint32_t uTimeUsed;
};

struct t_eFSS_TYPE_CrcCtxUser
{
    e_eFSS_BLOBC_RES eLastEr;
    uint32_t uTimeUsed;
};



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_BLOBCTST_EraseAdapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase);

static bool_t eFSS_BLOBCTST_EraseErrAdapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase);

static bool_t eFSS_BLOBCTST_EraseTst1Adapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase);


static bool_t eFSS_BLOBCTST_WriteAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                        const uint32_t p_uPageToWrite, const uint8_t* p_puDataToWrite,
                                        const uint32_t p_uDataToWriteL );

static bool_t eFSS_BLOBCTST_WriteErrAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                           const uint32_t p_uPageToWrite, const uint8_t* p_puDataToWrite,
                                           const uint32_t p_uDataToWriteL );

static bool_t eFSS_BLOBCTST_WriteErrSecAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                              const uint32_t p_uPageToWrite, const uint8_t* p_puDataToWrite,
                                              const uint32_t p_uDataToWriteL );

static bool_t eFSS_BLOBCTST_WriteTst1Adapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                        const uint32_t p_uPageToWrite, const uint8_t* p_puDataToWrite,
                                        const uint32_t p_uDataToWriteL );

static bool_t eFSS_BLOBCTST_ReadAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                       const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                       const uint32_t p_uReadBufferL );

static bool_t eFSS_BLOBCTST_ReadErrAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                          const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                          const uint32_t p_uReadBufferL );

static bool_t eFSS_BLOBCTST_ReadErrSecAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                             const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                             const uint32_t p_uReadBufferL );

static bool_t eFSS_BLOBCTST_ReadTst1Adapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                       const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                       const uint32_t p_uReadBufferL );

static bool_t eFSS_BLOBCTST_ReadTst2Adapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                       const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                       const uint32_t p_uReadBufferL );

static bool_t eFSS_BLOBCTST_CrcAdapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
                                      const uint8_t* p_puData, const uint32_t p_uDataL,
                                      uint32_t* const p_puCrc32Val );

static bool_t eFSS_BLOBCTST_CrcErrAdapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
                                         const uint8_t* p_puData, const uint32_t p_uDataL,
                                         uint32_t* const p_puCrc32Val );

static bool_t eFSS_BLOBCTST_CrcTst1Adapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
                                          const uint8_t* p_puData, const uint32_t p_uDataL,
                                          uint32_t* const p_puCrc32Val );

/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void eFSS_BLOBCTST_BadPointer(void);

#if 0
static void eFSS_BLOBCTST_BadInit(void);
static void eFSS_BLOBCTST_BadParamEntr(void);
static void eFSS_BLOBCTST_CorruptedCtx(void);
static void eFSS_BLOBCTST_Basic(void);
static void eFSS_BLOBCTST_BadClBckNRetry(void);
static void eFSS_BLOBCTST_CrcTest(void);
static void eFSS_BLOBCTST_LoadTest(void);
static void eFSS_BLOBCTST_FlushTest(void);
static void eFSS_BLOBCTST_Compare(void);
static void eFSS_BLOBCTST_LoadBkupTest(void);
static void eFSS_BLOBCTST_FlushBkupTest(void);
static void eFSS_BLOBCTST_GenTest(void);
#endif


/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eFSS_BLOBCTST_ExeTest(void)
{
	(void)printf("\n\nBLOB CORE TEST START \n\n");

    eFSS_BLOBCTST_BadPointer();

    #if 0
    eFSS_BLOBCTST_BadInit();
    eFSS_BLOBCTST_BadParamEntr();
    eFSS_BLOBCTST_CorruptedCtx();
    eFSS_BLOBCTST_Basic();
    eFSS_BLOBCTST_BadClBckNRetry();
    eFSS_BLOBCTST_CrcTest();
    eFSS_BLOBCTST_LoadTest();
    eFSS_BLOBCTST_FlushTest();
    eFSS_BLOBCTST_Compare();
    eFSS_BLOBCTST_LoadBkupTest();
    eFSS_BLOBCTST_FlushBkupTest();
    eFSS_BLOBCTST_GenTest();
    #endif

    (void)printf("\n\nBLOB CORE TEST END \n\n");
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


static bool_t eFSS_BLOBCTST_EraseAdapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase)
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
        p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_OK;
    }

    return l_bRes;
}

static bool_t eFSS_BLOBCTST_EraseErrAdapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase)
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
        p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPOINTER;
    }

    return l_bRes;
}

static bool_t eFSS_BLOBCTST_EraseTst1Adapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase)
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
            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPARAM;
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

static bool_t eFSS_BLOBCTST_WriteAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPOINTER;
        }
        else
        {
            l_bRes = true;
            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_OK;
        }
    }

    return l_bRes;
}

static bool_t eFSS_BLOBCTST_WriteErrAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPOINTER;
        }
        else
        {
            l_bRes = false;
            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPOINTER;
        }
    }

    return l_bRes;
}

static bool_t eFSS_BLOBCTST_WriteErrSecAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPOINTER;
        }
        else
        {
            if( p_uPageToWrite >= 2u )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPARAM;
            }
            else
            {
                if( 24u != p_uDataToWriteL )
                {
                    l_bRes = false;
                    p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPARAM;
                }
                else
                {
                    if( 0u == p_ptCtx->uTimeUsed )
                    {
                        if( 0u == p_uPageToWrite )
                        {
                            if( false == m_bIsErased1 )
                            {
                                l_bRes = false;
                                p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_CORRUPTCTX;
                            }
                            else
                            {
                                l_bRes = true;
                                p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_OK;
                                m_bIsErased1 = false;
                                (void)memcpy(m_auStorArea1, p_puDataToWrite, p_uDataToWriteL);
                            }
                        }
                        else
                        {
                            if( false == m_bIsErased2 )
                            {
                                l_bRes = false;
                                p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_CORRUPTCTX;
                            }
                            else
                            {
                                l_bRes = true;
                                p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_OK;
                                m_bIsErased2 = false;
                                (void)memcpy(m_auStorArea2, p_puDataToWrite, p_uDataToWriteL);
                            }
                        }

                        p_ptCtx->uTimeUsed++;

                    }
                    else
                    {
                        l_bRes = false;
                        p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPOINTER;
                    }
                }
            }
        }
    }

    return l_bRes;
}

static bool_t eFSS_BLOBCTST_WriteTst1Adapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPOINTER;
        }
        else
        {
            if( p_uPageToWrite >= 2u )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPARAM;
            }
            else
            {
                if( 24u != p_uDataToWriteL )
                {
                    l_bRes = false;
                    p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPARAM;
                }
                else
                {
                    p_ptCtx->uTimeUsed++;

                    if( 0u == p_uPageToWrite )
                    {
                        if( false == m_bIsErased1 )
                        {
                            l_bRes = false;
                            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_CORRUPTCTX;
                        }
                        else
                        {
                            l_bRes = true;
                            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_OK;
                            m_bIsErased1 = false;
                            (void)memcpy(m_auStorArea1, p_puDataToWrite, p_uDataToWriteL);
                        }
                    }
                    else
                    {
                        if( false == m_bIsErased2 )
                        {
                            l_bRes = false;
                            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_CORRUPTCTX;
                        }
                        else
                        {
                            l_bRes = true;
                            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_OK;
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

static bool_t eFSS_BLOBCTST_ReadAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPOINTER;
        }
        else
        {
            l_bRes = true;
            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_OK;
        }
    }

    return l_bRes;
}

static bool_t eFSS_BLOBCTST_ReadErrAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPOINTER;
        }
        else
        {
            l_bRes = false;
            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPOINTER;
        }
    }

    return l_bRes;
}


static bool_t eFSS_BLOBCTST_ReadErrSecAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPOINTER;
        }
        else
        {
            if( p_uPageToRead >= 2u )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPARAM;
            }
            else
            {
                if( 24u != p_uReadBufferL )
                {
                    l_bRes = false;
                    p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPARAM;
                }
                else
                {
                    if( 0u == p_ptCtx->uTimeUsed )
                    {
                        if( 0u == p_uPageToRead )
                        {
                            l_bRes = true;
                            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_OK;
                            (void)memcpy(p_puReadBuffer, m_auStorArea1, p_uReadBufferL);
                        }
                        else
                        {
                            l_bRes = true;
                            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_OK;
                            (void)memcpy(p_puReadBuffer, m_auStorArea2, p_uReadBufferL);
                        }

                        p_ptCtx->uTimeUsed++;
                    }
                    else
                    {
                        l_bRes = false;
                        p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPOINTER;
                    }
                }
            }
        }
    }

    return l_bRes;
}

static bool_t eFSS_BLOBCTST_ReadTst1Adapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPOINTER;
        }
        else
        {
            if( p_uPageToRead >= 2u )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPARAM;
            }
            else
            {
                if( 24u != p_uReadBufferL )
                {
                    l_bRes = false;
                    p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPARAM;
                }
                else
                {
                    p_ptCtx->uTimeUsed++;

                    if( 0u == p_uPageToRead )
                    {
                        l_bRes = true;
                        p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_OK;
                        (void)memcpy(p_puReadBuffer, m_auStorArea1, p_uReadBufferL);
                    }
                    else
                    {
                        l_bRes = true;
                        p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_OK;
                        (void)memcpy(p_puReadBuffer, m_auStorArea2, p_uReadBufferL);
                    }
                }
            }
        }
    }

    return l_bRes;
}

static bool_t eFSS_BLOBCTST_ReadTst2Adapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPOINTER;
        }
        else
        {
            if( p_uPageToRead >= 2u )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPARAM;
            }
            else
            {
                if( 24u != p_uReadBufferL )
                {
                    l_bRes = false;
                    p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPARAM;
                }
                else
                {
                    p_ptCtx->uTimeUsed++;

                    if( 0u == p_uPageToRead )
                    {
                        l_bRes = true;
                        p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_OK;
                        (void)memcpy(p_puReadBuffer, m_auStorArea1, p_uReadBufferL);
                        p_puReadBuffer[0] = p_puReadBuffer[0] + 2u;
                    }
                    else
                    {
                        l_bRes = true;
                        p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_OK;
                        (void)memcpy(p_puReadBuffer, m_auStorArea2, p_uReadBufferL);
                        p_puReadBuffer[0] = p_puReadBuffer[0] + 2u;
                    }
                }
            }
        }
    }

    return l_bRes;
}

static bool_t eFSS_BLOBCTST_CrcAdapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
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
            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPOINTER;
        }
        else
        {
            l_bRes = true;
            *p_puCrc32Val = 0u;
            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_OK;
        }
    }

    return l_bRes;
}

static bool_t eFSS_BLOBCTST_CrcErrAdapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
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
            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPOINTER;
        }
        else
        {
            l_bRes = false;
            *p_puCrc32Val = 0u;
            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPOINTER;
        }
    }

    return l_bRes;
}

static bool_t eFSS_BLOBCTST_CrcTst1Adapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
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
            p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPOINTER;
        }
        else
        {
            if( 0u == p_uDataL )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_BADPARAM;
            }
            else
            {
                l_bRes = true;
                p_ptCtx->eLastEr = e_eFSS_BLOBC_RES_OK;
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
void eFSS_BLOBCTST_BadPointer(void)
{
    /* Local variable */
    t_eFSS_BLOBC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tGetStorSet;
    t_eFSS_BLOBC_StorBuf l_ltUseBuff;
    uint32_t l_uCrcGetted;
    uint8_t l_uSubTypeRead;
    uint8_t l_uSubTypeWrite;
    bool_t l_bIsEquals;

    /* Misra complaiant */
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxCrc32.uTimeUsed = 0u;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseAdapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteAdapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadAdapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcAdapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 1u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 2u;
    l_tStorSet.uPageVersion = 1u;

    l_uSubTypeRead = 0u;
    l_uSubTypeWrite = 0u;
    l_bIsEquals = false;

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_InitCtx(NULL, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, NULL, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 2  -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.ptCtxErase = NULL;
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 3  -- OK \n");
        l_tCtxCb.ptCtxErase = &l_tCtxErase;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 3  -- FAIL \n");
        l_tCtxCb.ptCtxErase = &l_tCtxErase;
    }

    /* Function */
    l_tCtxCb.fErase = NULL;
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 4  -- OK \n");
        l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseAdapt;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 4  -- FAIL \n");
        l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseAdapt;
    }

    /* Function */
    l_tCtxCb.ptCtxWrite = NULL;
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 5  -- OK \n");
        l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 5  -- FAIL \n");
        l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    }

    /* Function */
    l_tCtxCb.fWrite = NULL;
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 6  -- OK \n");
        l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteAdapt;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 6  -- FAIL \n");
        l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteAdapt;
    }

    /* Function */
    l_tCtxCb.ptCtxRead = NULL;
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 7  -- OK \n");
        l_tCtxCb.ptCtxRead = &l_tCtxRead;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 7  -- FAIL \n");
        l_tCtxCb.ptCtxRead = &l_tCtxRead;
    }

    /* Function */
    l_tCtxCb.fRead = NULL;
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 8  -- OK \n");
        l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadAdapt;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 8  -- FAIL \n");
        l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadAdapt;
    }

    /* Function */
    l_tCtxCb.ptCtxCrc32 = NULL;
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 9  -- OK \n");
        l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 9  -- FAIL \n");
        l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    }

    /* Function */
    l_tCtxCb.fCrc32 = NULL;
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 10 -- OK \n");
        l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcAdapt;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 10 -- FAIL \n");
        l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcAdapt;
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_IsInit(NULL, &l_bIsInit) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 11 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_IsInit(&l_tCtx, NULL) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 12 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_GetStorSett(NULL, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 13 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 14 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_GetStorSett(&l_tCtx, NULL) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 15 -- OK \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 15 -- FAIL \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_GetBuff(NULL, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 16 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_GetBuff(&l_tCtx, NULL) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 17 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_GetBuffNStor(NULL, &l_ltUseBuff, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 18 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 19 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_GetBuffNStor(&l_tCtx, NULL, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 20 -- OK \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 20 -- FAIL \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 21 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_GetBuffNStor(&l_tCtx, &l_ltUseBuff, NULL) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 22 -- OK \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 22 -- FAIL \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_LoadPageInBuff(NULL, 0u, &l_uSubTypeRead) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 23 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 0u, NULL) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 24 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_LoadPageInBuff(NULL, 0u, NULL) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 25 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_FlushBuffInPage(NULL, 0u, l_uSubTypeWrite) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 26 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_CalcCrcInBuff(NULL, 0u, 1u, &l_uCrcGetted) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 27 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 27 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 28 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 28 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_CalcCrcInBuff(&l_tCtx,  0u, 1u, NULL) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 29 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 29 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_FlushBuffInPageNBkp(NULL,  0u, 1u, 0u, 0u) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 30 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 30 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_LoadPageInBuffNRipBkp(NULL,  0u, 1u, 0u, 0u) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 31 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 31 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_IsBuffEqualToPage(NULL,  0u, &l_bIsEquals, &l_uSubTypeRead) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 32 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 32 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_IsBuffEqualToPage(&l_tCtx,  0u, NULL, &l_uSubTypeRead) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 33 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 33 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPOINTER == eFSS_BLOBC_IsBuffEqualToPage(&l_tCtx,  0u, &l_bIsEquals, NULL) )
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 34 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadPointer 34 -- FAIL \n");
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

#if 0

void eFSS_BLOBCTST_BadInit(void)
{
    /* Local variable */
    t_eFSS_BLOBC_Ctx l_tCtx;
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tGetStorSet;
    t_eFSS_BLOBC_StorBuf l_ltUseBuff;
    uint32_t l_uCrcGetted;
    uint8_t l_uSubTypeRead;
    uint8_t l_uSubTypeWrite;
    bool_t l_bIsEquals;

    l_uSubTypeRead = 0u;
    l_uSubTypeWrite = 0u;
    l_bIsEquals = false;

    /* Init the remainings var */
    (void)memset(&l_tCtx, 0, sizeof(l_tCtx));

    /* Function */
    if( e_eFSS_BLOBC_RES_NOINITLIB == eFSS_BLOBC_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_BadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadInit 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_NOINITLIB == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_BadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadInit 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_NOINITLIB == eFSS_BLOBC_GetBuffNStor(&l_tCtx, &l_ltUseBuff, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_BadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadInit 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_NOINITLIB == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        (void)printf("eFSS_BLOBCTST_BadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadInit 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_NOINITLIB == eFSS_BLOBC_FlushBuffInPage(&l_tCtx, 0u, l_uSubTypeWrite) )
    {
        (void)printf("eFSS_BLOBCTST_BadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadInit 5  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_NOINITLIB == eFSS_BLOBC_CalcCrcInBuff(&l_tCtx, 0u, 1u, &l_uCrcGetted) )
    {
        (void)printf("eFSS_BLOBCTST_BadInit 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadInit 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_IsInit(&l_tCtx, &l_bIsInit) )
    {
        if( false == l_bIsInit )
        {
            (void)printf("eFSS_BLOBCTST_BadInit 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_BadInit 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadInit 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_NOINITLIB == eFSS_BLOBC_FlushBuffInPageNBkp(&l_tCtx, 0u, 1u, 0u, 0u) )
    {
        (void)printf("eFSS_BLOBCTST_BadInit 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadInit 8  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_NOINITLIB == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 0u, 1u, 0u, 0u) )
    {
        (void)printf("eFSS_BLOBCTST_BadInit 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadInit 9  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_NOINITLIB == eFSS_BLOBC_IsBuffEqualToPage(&l_tCtx, 0u, &l_bIsEquals, &l_uSubTypeRead) )
    {
        (void)printf("eFSS_BLOBCTST_BadInit 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadInit 10 -- FAIL \n");
    }
}

void eFSS_BLOBCTST_BadParamEntr(void)
{
    /* Local variable */
    t_eFSS_BLOBC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    uint32_t l_uCrcGetted;
    uint8_t l_uSubTypeRead;
    uint8_t l_uSubTypeWrite;
    bool_t l_bIsEquals;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcTst1Adapt;

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

    l_uSubTypeRead = 0u;
    l_uSubTypeWrite = 0u;
    l_bIsEquals = false;

    /* Function */
    l_tStorSet.uPagesLen = 25u;
    if( e_eFSS_BLOBC_RES_BADPARAM == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 1  -- OK \n");
        l_tStorSet.uPagesLen = 24u;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 1  -- FAIL \n");
        l_tStorSet.uPagesLen = 24u;
    }

    /* Function */
    l_tStorSet.uPagesLen = 23u;
    if( e_eFSS_BLOBC_RES_BADPARAM == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 2  -- OK \n");
        l_tStorSet.uPagesLen = 24u;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 2  -- FAIL \n");
        l_tStorSet.uPagesLen = 24u;
    }

    /* Function */
    l_tStorSet.uTotPages = 0u;
    if( e_eFSS_BLOBC_RES_BADPARAM == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 3  -- OK \n");
        l_tStorSet.uTotPages = 1u;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 3  -- FAIL \n");
        l_tStorSet.uTotPages = 1u;
    }

    /* Function */
    l_tStorSet.uPagesLen = 19u;
    if( e_eFSS_BLOBC_RES_BADPARAM == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u ) )
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 4  -- OK \n");
        l_tStorSet.uPagesLen = 24u;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 4  -- FAIL \n");
        l_tStorSet.uPagesLen = 24u;
    }

    /* Function */
    l_tStorSet.uPagesLen = 18u;
    if( e_eFSS_BLOBC_RES_BADPARAM == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u ) )
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 5  -- OK \n");
        l_tStorSet.uPagesLen = 24u;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 5  -- FAIL \n");
        l_tStorSet.uPagesLen = 24u;
    }

    /* Function */
    l_tStorSet.uRWERetry = 0u;
    if( e_eFSS_BLOBC_RES_BADPARAM == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 6  -- OK \n");
        l_tStorSet.uRWERetry = 1u;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 6  -- FAIL \n");
        l_tStorSet.uRWERetry = 1u;
    }

    /* Function */
    l_tStorSet.uPagesLen = 20u;
    if( e_eFSS_BLOBC_RES_BADPARAM == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u ) )
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 7  -- OK \n");
        l_tStorSet.uPagesLen = 24u;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 7  -- FAIL \n");
        l_tStorSet.uPagesLen = 24u;
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 8  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPARAM == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 1u, &l_uSubTypeRead) )
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 9  -- OK \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPARAM == eFSS_BLOBC_FlushBuffInPage(&l_tCtx, 1u, l_uSubTypeWrite) )
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 10 -- OK \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPARAM == eFSS_BLOBC_CalcCrcInBuff(&l_tCtx, 0u, 0u, &l_uCrcGetted) )
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 11 -- OK \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPARAM == eFSS_BLOBC_CalcCrcInBuff(&l_tCtx, 0u, 6u, &l_uCrcGetted) )
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 12 -- OK \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPARAM == eFSS_BLOBC_CalcCrcInBuff(&l_tCtx, 0u, 5u, &l_uCrcGetted) )
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 13 -- OK \n");
    }

    /* Function */
    l_tStorSet.uTotPages = 2u;
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 14 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPARAM == eFSS_BLOBC_FlushBuffInPageNBkp(&l_tCtx, 2u, 1u, 0u, 1u) )
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 15 -- OK \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPARAM == eFSS_BLOBC_FlushBuffInPageNBkp(&l_tCtx, 0u, 2u, 0u, 1u) )
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 16 -- OK \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPARAM == eFSS_BLOBC_FlushBuffInPageNBkp(&l_tCtx, 0u, 0u, 0u, 1u) )
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 17 -- OK \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPARAM == eFSS_BLOBC_IsBuffEqualToPage(&l_tCtx, 2u, &l_bIsEquals, &l_uSubTypeRead) )
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 18 -- OK \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPARAM == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 0u, 0u, 0u, 1u) )
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 19 -- OK \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPARAM == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 2u, 1u, 0u, 1u) )
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 20 -- OK \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_BADPARAM == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 0u, 2u, 0u, 1u) )
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadParamEntr 21 -- OK \n");
    }
}

void eFSS_BLOBCTST_CorruptedCtx(void)
{
    /* Local variable */
    t_eFSS_BLOBC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_TYPE_StorSet l_tGetStorSet;
    t_eFSS_BLOBC_StorBuf l_ltUseBuff;
    uint32_t l_uCrcGetted;
    uint8_t l_uSubTypeRead;
    uint8_t l_uSubTypeWrite;
    bool_t l_bIsEquals;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseAdapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteAdapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadAdapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcAdapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 1u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 2u;
    l_tStorSet.uPageVersion = 1u;
    l_uSubTypeRead = 0u;
    l_uSubTypeWrite = 0u;
    l_bIsEquals = false;

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 2  -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tCtxCb.ptCtxErase = NULL;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 3  -- OK \n");
        l_tCtx.tCORELLCtx.tCtxCb.ptCtxErase = &l_tCtxErase;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 3  -- FAIL \n");
        l_tCtx.tCORELLCtx.tCtxCb.ptCtxErase = &l_tCtxErase;
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 4  -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tCtxCb.fErase = NULL;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 5  -- OK \n");
        l_tCtx.tCORELLCtx.tCtxCb.fErase = &eFSS_BLOBCTST_EraseAdapt;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 5  -- FAIL \n");
        l_tCtx.tCORELLCtx.tCtxCb.fErase = &eFSS_BLOBCTST_EraseAdapt;
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 6  -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tCtxCb.ptCtxWrite = NULL;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 7  -- OK \n");
        l_tCtx.tCORELLCtx.tCtxCb.ptCtxWrite = &l_tCtxWrite;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 7  -- FAIL \n");
        l_tCtx.tCORELLCtx.tCtxCb.ptCtxWrite = &l_tCtxWrite;
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 8  -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tCtxCb.fWrite = NULL;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 9  -- OK \n");
        l_tCtx.tCORELLCtx.tCtxCb.fWrite = &eFSS_BLOBCTST_WriteAdapt;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 9  -- FAIL \n");
        l_tCtx.tCORELLCtx.tCtxCb.fWrite = &eFSS_BLOBCTST_WriteAdapt;
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 10 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tCtxCb.ptCtxRead = NULL;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 11 -- OK \n");
        l_tCtx.tCORELLCtx.tCtxCb.ptCtxRead = &l_tCtxRead;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 11 -- FAIL \n");
        l_tCtx.tCORELLCtx.tCtxCb.ptCtxRead = &l_tCtxRead;
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 12 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tCtxCb.fRead = NULL;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 13 -- OK \n");
        l_tCtx.tCORELLCtx.tCtxCb.fRead = &eFSS_BLOBCTST_ReadAdapt;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 13 -- FAIL \n");
        l_tCtx.tCORELLCtx.tCtxCb.fRead = &eFSS_BLOBCTST_ReadAdapt;
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 14 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tCtxCb.ptCtxCrc32 = NULL;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 15 -- OK \n");
        l_tCtx.tCORELLCtx.tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 15 -- FAIL \n");
        l_tCtx.tCORELLCtx.tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 16 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tCtxCb.fCrc32 = NULL;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 17 -- OK \n");
        l_tCtx.tCORELLCtx.tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcAdapt;
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 17 -- FAIL \n");
        l_tCtx.tCORELLCtx.tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcAdapt;
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 18 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tBuff1.puBuf = NULL;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 19 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 20 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tBuff2.puBuf = NULL;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 21 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 22 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tStorSett.uTotPages = 0u;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 23 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 24 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tStorSett.uPagesLen = 19u;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 25 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 26 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tStorSett.uRWERetry = 0u;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 27 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 27 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 28 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 28 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tBuff1.uBufL = l_tCtx.tCORELLCtx.tBuff2.uBufL + 1u;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 29 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 29 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 30 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 30 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tStorSett.uPagesLen = l_tCtx.tCORELLCtx.tBuff1.uBufL + 1u;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 31 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 31 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 32 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 32 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tStorSett.uPagesLen = 20u;
    l_tCtx.tCORELLCtx.tBuff1.uBufL = 20u;
    l_tCtx.tCORELLCtx.tBuff2.uBufL = 20u;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 33 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 33 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 34 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 34 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tStorSett.uPagesLen = l_tCtx.tCORELLCtx.tBuff1.uBufL + 1u;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 35 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 35 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 36 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 36 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tStorSett.uPagesLen = l_tCtx.tCORELLCtx.tBuff1.uBufL + 1u;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_GetBuffNStor(&l_tCtx, &l_ltUseBuff, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 37 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 37 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 38 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 38 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tStorSett.uPagesLen = l_tCtx.tCORELLCtx.tBuff1.uBufL + 1u;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 39 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 39 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 40 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 40 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tStorSett.uPagesLen = l_tCtx.tCORELLCtx.tBuff1.uBufL + 1u;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_FlushBuffInPage(&l_tCtx, 0u, l_uSubTypeWrite) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 41 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 41 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 42 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 42 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tStorSett.uPagesLen = l_tCtx.tCORELLCtx.tBuff1.uBufL + 1u;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_CalcCrcInBuff(&l_tCtx, 0u, 5u, &l_uCrcGetted) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 43 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 43 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 44 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 44 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tStorSett.uPagesLen = l_tCtx.tCORELLCtx.tBuff1.uBufL + 1u;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_FlushBuffInPageNBkp(&l_tCtx, 0u, 1u, 0u, 1u) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 45 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 45 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 46 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 46 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tStorSett.uPagesLen = l_tCtx.tCORELLCtx.tBuff1.uBufL + 1u;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 0u, 1u, 0u, 1u) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 47 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 47 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 48 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 48 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tStorSett.uPagesLen = l_tCtx.tCORELLCtx.tBuff1.uBufL + 1u;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_IsBuffEqualToPage(&l_tCtx, 0u, &l_bIsEquals, &l_uSubTypeRead) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 49 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 49 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 50 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 50 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCORELLCtx.tBuff2.puBuf = l_tCtx.tCORELLCtx.tBuff1.puBuf;
    if( e_eFSS_BLOBC_RES_CORRUPTCTX == eFSS_BLOBC_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 51 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CorruptedCtx 51 -- FAIL \n");
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

void eFSS_BLOBCTST_Basic(void)
{
    /* Local variable */
    t_eFSS_BLOBC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tGetStorSet;
    t_eFSS_BLOBC_StorBuf l_ltUseBuff;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseAdapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteAdapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadErrAdapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcAdapt;

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

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_Basic 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_Basic 1  -- FAIL \n");
    }

    /* Verify buffer validity */
    if( l_auStor == l_tCtx.tCORELLCtx.tBuff1.puBuf )
    {
        (void)printf("eFSS_BLOBCTST_Basic 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_Basic 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_IsInit(&l_tCtx, &l_bIsInit ) )
    {
        if( true == l_bIsInit )
        {
            (void)printf("eFSS_BLOBCTST_Basic 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_Basic 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_Basic 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        if( ( l_auStor == l_ltUseBuff.puBuf ) && ( (24u-20u) == l_ltUseBuff.uBufL ) )
        {
            (void)printf("eFSS_BLOBCTST_Basic 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_Basic 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_Basic 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuffNStor(&l_tCtx, &l_ltUseBuff, &l_tGetStorSet) )
    {
        if( ( l_auStor == l_ltUseBuff.puBuf ) && ( (24u-20u) == l_ltUseBuff.uBufL ) )
        {
            (void)printf("eFSS_BLOBCTST_Basic 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_Basic 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_Basic 5  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuffNStor(&l_tCtx, &l_ltUseBuff, &l_tGetStorSet) )
    {
        if( ( 1u == l_tGetStorSet.uTotPages ) && ( 24u == l_tGetStorSet.uPagesLen ) &&
            ( 3u == l_tGetStorSet.uRWERetry ) && ( 1u == l_tGetStorSet.uPageVersion ) )
        {
            (void)printf("eFSS_BLOBCTST_Basic 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_Basic 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_Basic 6  -- FAIL \n");
    }


    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetStorSett(&l_tCtx, &l_tGetStorSet) )
    {
        if( ( 1u == l_tGetStorSet.uTotPages ) && ( 24u == l_tGetStorSet.uPagesLen ) &&
            ( 3u == l_tGetStorSet.uRWERetry ) && ( 1u == l_tGetStorSet.uPageVersion ) )
        {
            (void)printf("eFSS_BLOBCTST_Basic 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_Basic 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_Basic 7  -- FAIL \n");
    }
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "CERT-INT31-C_c"
    /* Suppressed for code clarity in test execution*/
#endif

void eFSS_BLOBCTST_BadClBckNRetry(void)
{
    /* Local variable */
    t_eFSS_BLOBC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    bool_t l_bIsInit;
    t_eFSS_TYPE_StorSet l_tGetStorSet;
    t_eFSS_BLOBC_StorBuf l_ltUseBuff;
    uint32_t l_uCrcGetted;
    uint8_t l_uSubTypeRead;
    uint8_t l_uSubTypeWrite;
    bool_t l_bIsEquals;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseAdapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteAdapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadErrAdapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcAdapt;

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
    l_tStorSet.uTotPages = 2u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;
    l_uSubTypeRead = 0u;
    l_uSubTypeWrite = 0u;
    l_bIsEquals = false;

    /* ------------------------------------------------------------------------------------------ TEST READ CALL BACK */
    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_IsInit(&l_tCtx, &l_bIsInit ) )
    {
        if( true == l_bIsInit )
        {
            (void)printf("eFSS_BLOBCTST_BadClBckNRetry 2  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_BadClBckNRetry 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuffNStor(&l_tCtx, &l_ltUseBuff, &l_tGetStorSet) )
    {
        if( ( l_auStor == l_ltUseBuff.puBuf ) && ( (24u-20u) == l_ltUseBuff.uBufL ) )
        {
            (void)printf("eFSS_BLOBCTST_BadClBckNRetry 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_BadClBckNRetry 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_IsInit(&l_tCtx, &l_bIsInit ) )
    {
        if( true == l_bIsInit )
        {
            (void)printf("eFSS_BLOBCTST_BadClBckNRetry 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_BadClBckNRetry 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 4  -- FAIL \n");
    }

    /* Test LOAD */
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;
    if( e_eFSS_BLOBC_RES_CLBCKREADERR == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        if( ( l_tStorSet.uRWERetry == l_tCtxRead.uTimeUsed ) && ( e_eFSS_BLOBC_RES_BADPOINTER == l_tCtxRead.eLastEr ) &&
            ( 0u == l_tCtxCrc32.uTimeUsed ) )
        {
            (void)printf("eFSS_BLOBCTST_BadClBckNRetry 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_BadClBckNRetry 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 5  -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcErrAdapt;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 7  -- FAIL \n");
    }

    /* Test LOAD */
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_CLBCKCRCERR == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        if( ( 1u == l_tCtxRead.uTimeUsed ) && ( e_eFSS_BLOBC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_BLOBC_RES_BADPOINTER == l_tCtxCrc32.eLastEr ) )
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 8  -- OK \n");
            }
            else
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 8  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_BadClBckNRetry 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 8  -- FAIL \n");
    }

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcErrAdapt;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 9  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 10 -- FAIL \n");
    }

    /* Test LOAD */
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_CLBCKCRCERR == eFSS_BLOBC_CalcCrcInBuff(&l_tCtx, 0u, 4u, &l_uCrcGetted) )
    {
        if( ( 0u == l_tCtxRead.uTimeUsed ) && ( e_eFSS_BLOBC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_BLOBC_RES_BADPOINTER == l_tCtxCrc32.eLastEr ) )
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 11 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 11 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_BadClBckNRetry 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 11 -- FAIL \n");
    }

    /* ----------------------------------------------------------------------------------------- TEST WRITE CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseAdapt;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteAdapt;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcErrAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 12 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 13 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_CLBCKCRCERR == eFSS_BLOBC_FlushBuffInPage(&l_tCtx, 0u, l_uSubTypeWrite) )
    {
        if( ( 0u == l_tCtxErase.uTimeUsed ) && ( e_eFSS_BLOBC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 0u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_BLOBC_RES_OK == l_tCtxWrite.eLastEr ) &&
            ( 0u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_BLOBC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_BLOBC_RES_BADPOINTER == l_tCtxCrc32.eLastEr ) )
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 14 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 14 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_BadClBckNRetry 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 14 -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseErrAdapt;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteAdapt;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 15 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 16 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_CLBCKERASEERR == eFSS_BLOBC_FlushBuffInPage(&l_tCtx, 0u, l_uSubTypeWrite) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_BLOBC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 0u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_BLOBC_RES_OK == l_tCtxWrite.eLastEr ) &&
            ( 0u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_BLOBC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 3u == l_tCtxErase.uTimeUsed ) && ( e_eFSS_BLOBC_RES_BADPOINTER == l_tCtxErase.eLastEr ) )
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 17 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 17 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_BadClBckNRetry 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 17 -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseAdapt;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteErrAdapt;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 18 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 19 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_CLBCKWRITEERR == eFSS_BLOBC_FlushBuffInPage(&l_tCtx, 0u, l_uSubTypeWrite) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_BLOBC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 3u == l_tCtxErase.uTimeUsed ) && ( e_eFSS_BLOBC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 0u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_BLOBC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 3u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_BLOBC_RES_BADPOINTER == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 20 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 20 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_BadClBckNRetry 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 20 -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseAdapt;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteAdapt;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadErrAdapt;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 21 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 22 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_CLBCKREADERR == eFSS_BLOBC_FlushBuffInPage(&l_tCtx, 0u, l_uSubTypeWrite) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_BLOBC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 3u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_BLOBC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 3u == l_tCtxWrite.uTimeUsed )  && ( e_eFSS_BLOBC_RES_OK == l_tCtxWrite.eLastEr ) )
        {
            if( ( 3u == l_tCtxRead.uTimeUsed ) && ( e_eFSS_BLOBC_RES_BADPOINTER == l_tCtxRead.eLastEr ) )
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 23 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 23 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_BadClBckNRetry 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 23 -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseAdapt;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteErrAdapt;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 24 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 25 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_CLBCKWRITEERR == eFSS_BLOBC_FlushBuffInPageNBkp(&l_tCtx, 0u, 1u, l_uSubTypeWrite, l_uSubTypeWrite) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_BLOBC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 3u == l_tCtxErase.uTimeUsed ) && ( e_eFSS_BLOBC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 0u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_BLOBC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 3u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_BLOBC_RES_BADPOINTER == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 26 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 26 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_BadClBckNRetry 26 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 26 -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteErrSecAdapt;
    l_tCtxCb.fRead  = &eFSS_BLOBCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 27 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 27 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 28 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 28 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_CLBCKWRITEERR == eFSS_BLOBC_FlushBuffInPageNBkp(&l_tCtx, 0u, 1u, l_uSubTypeWrite, l_uSubTypeWrite) )
    {
        if( ( 2u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_BLOBC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 4u == l_tCtxErase.uTimeUsed ) && ( e_eFSS_BLOBC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 1u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_BLOBC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 1u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_BLOBC_RES_BADPOINTER == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 29 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 29 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_BadClBckNRetry 29 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 29 -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseAdapt;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteAdapt;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadErrAdapt;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 30 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 30 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 31 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 31 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_CLBCKREADERR == eFSS_BLOBC_IsBuffEqualToPage(&l_tCtx, 0u, &l_bIsEquals, &l_uSubTypeRead) )
    {
        if( ( 0u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_BLOBC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 0u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_BLOBC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 0u == l_tCtxWrite.uTimeUsed )  && ( e_eFSS_BLOBC_RES_OK == l_tCtxWrite.eLastEr ) )
        {
            if( ( 3u == l_tCtxRead.uTimeUsed ) && ( e_eFSS_BLOBC_RES_BADPOINTER == l_tCtxRead.eLastEr ) )
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 32 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 32 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_BadClBckNRetry 32 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 32 -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseAdapt;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteAdapt;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadErrAdapt;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 33 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 33 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 34 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 34 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_CLBCKREADERR == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 0u, 1u, 0u, 0u) )
    {
        if( ( 0u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_BLOBC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 0u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_BLOBC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 0u == l_tCtxWrite.uTimeUsed )  && ( e_eFSS_BLOBC_RES_OK == l_tCtxWrite.eLastEr ) )
        {
            if( ( 3u == l_tCtxRead.uTimeUsed ) && ( e_eFSS_BLOBC_RES_BADPOINTER == l_tCtxRead.eLastEr ) )
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 35 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 35 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_BadClBckNRetry 35 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 35 -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadErrSecAdapt;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 36 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 36 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 37 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 37 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_CLBCKREADERR == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 0u, 1u, 0u, 0u) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_BLOBC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 0u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_BLOBC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 0u == l_tCtxWrite.uTimeUsed )  && ( e_eFSS_BLOBC_RES_OK == l_tCtxWrite.eLastEr ) )
        {
            if( ( 1u == l_tCtxRead.uTimeUsed ) && ( e_eFSS_BLOBC_RES_BADPOINTER == l_tCtxRead.eLastEr ) )
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 38 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 38 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_BadClBckNRetry 38 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 38 -- FAIL \n");
    }

    /* Init */
    l_tStorSet.uPageVersion = 0u;

    /* Function */
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteErrAdapt;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 39 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 39 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 40 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 40 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0xFF, sizeof(m_auStorArea2));

    /* Fill the two storage page */


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
    m_auStorArea2[10u] = 0x00u;
    m_auStorArea2[11u] = 0x00u;
    m_auStorArea2[12u] = 0x02u;
    m_auStorArea2[13u] = 0x00u;
    m_auStorArea2[14u] = 0x00u;
    m_auStorArea2[15u] = 0x00u;
    m_auStorArea2[16u] = 0xA5u;
    m_auStorArea2[17u] = 0xA5u;
    m_auStorArea2[18u] = 0xA5u;
    m_auStorArea2[19u] = 0xA5u;
    m_auStorArea2[20u] = 0xE7u;
    m_auStorArea2[21u] = 0x02u;
    m_auStorArea2[22u] = 0x00u;
    m_auStorArea2[23u] = 0x00u;

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_CLBCKWRITEERR == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 0u, 1u, 0u, 0x10u) )
    {
        if( ( 3u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_BLOBC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 3u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_BLOBC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 2u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_BLOBC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 3u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_BLOBC_RES_BADPOINTER == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 41 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 41 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_BadClBckNRetry 41 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 41 -- FAIL \n");
    }

    /* Init */
    l_tStorSet.uPageVersion = 0u;

    /* Function */
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteErrAdapt;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 42 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 42 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 43 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 43 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0xFF, sizeof(m_auStorArea2));

    /* Fill the two storage page */


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
    m_auStorArea1[20u] = 0x00u;
    m_auStorArea1[21u] = 0x00u;
    m_auStorArea1[22u] = 0x00u;
    m_auStorArea1[23u] = 0x00u;

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));

    m_auStorArea2[5u] = 0x01u;
    m_auStorArea2[6u] = 0x00u;
    m_auStorArea2[7u] = 0x00u;
    m_auStorArea2[8u] = 0x00u;
    m_auStorArea2[9u] = 0x01u;
    m_auStorArea2[10u] = 0x00u;
    m_auStorArea2[11u] = 0x00u;
    m_auStorArea2[12u] = 0x02u;
    m_auStorArea2[13u] = 0x00u;
    m_auStorArea2[14u] = 0x00u;
    m_auStorArea2[15u] = 0x00u;
    m_auStorArea2[16u] = 0xA5u;
    m_auStorArea2[17u] = 0xA5u;
    m_auStorArea2[18u] = 0xA5u;
    m_auStorArea2[19u] = 0xA5u;
    m_auStorArea2[20u] = 0xE7u;
    m_auStorArea2[21u] = 0x02u;
    m_auStorArea2[22u] = 0x00u;
    m_auStorArea2[23u] = 0x00u;

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_CLBCKWRITEERR == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 0u, 1u, 0u, 0x10u) )
    {
        if( ( 3u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_BLOBC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 3u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_BLOBC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 2u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_BLOBC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 3u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_BLOBC_RES_BADPOINTER == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 44 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 44 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_BadClBckNRetry 44 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 44 -- FAIL \n");
    }

    /* Init */
    l_tStorSet.uPageVersion = 0u;

    /* Function */
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteErrAdapt;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 45 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 45 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 46 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 46 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0xFF, sizeof(m_auStorArea2));

    /* Fill the two storage page */


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
    m_auStorArea2[10u] = 0x00u;
    m_auStorArea2[11u] = 0x00u;
    m_auStorArea2[12u] = 0x02u;
    m_auStorArea2[13u] = 0x00u;
    m_auStorArea2[14u] = 0x00u;
    m_auStorArea2[15u] = 0x00u;
    m_auStorArea2[16u] = 0xA5u;
    m_auStorArea2[17u] = 0xA5u;
    m_auStorArea2[18u] = 0xA5u;
    m_auStorArea2[19u] = 0xA5u;
    m_auStorArea2[20u] = 0x00u;
    m_auStorArea2[21u] = 0x00u;
    m_auStorArea2[22u] = 0x00u;
    m_auStorArea2[23u] = 0x00u;

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_CLBCKWRITEERR == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 0u, 1u, 0u, 0x10u) )
    {
        if( ( 3u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_BLOBC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 3u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_BLOBC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 2u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_BLOBC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 3u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_BLOBC_RES_BADPOINTER == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 47 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_BLOBCTST_BadClBckNRetry 47 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_BadClBckNRetry 47 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_BadClBckNRetry 47 -- FAIL \n");
    }
}

static void eFSS_BLOBCTST_CrcTest(void)
{
    /* Local variable */
    t_eFSS_BLOBC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_BLOBC_StorBuf l_ltUseBuff;
    uint32_t l_uCrcGetted;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseAdapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteAdapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadErrAdapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcAdapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 1u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseAdapt;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteAdapt;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_CrcTest 1 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CrcTest 1 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_CrcTest 2 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CrcTest 2 -- FAIL \n");
    }

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x00u;
    l_ltUseBuff.puBuf[2u] = 0x00u;
    l_ltUseBuff.puBuf[3u] = 0x00u;

    /* Function */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_CalcCrcInBuff(&l_tCtx, 0u, l_ltUseBuff.uBufL, &l_uCrcGetted) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_BLOBC_RES_OK == l_tCtxCrc32.eLastEr ) )
        {
            if( 4u == l_ltUseBuff.uBufL )
            {
                if( 1u == l_uCrcGetted )
                {
                    (void)printf("eFSS_BLOBCTST_CrcTest 3 -- OK \n");
                }
                else
                {
                    (void)printf("eFSS_BLOBCTST_CrcTest 3 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eFSS_BLOBCTST_CrcTest 3 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_CrcTest 3 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CrcTest 3 -- FAIL \n");
    }

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x00u;
    l_ltUseBuff.puBuf[2u] = 0x00u;
    l_ltUseBuff.puBuf[3u] = 0x00u;
    l_ltUseBuff.puBuf[4u] = 0x10u;

    /* Function */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_CalcCrcInBuff(&l_tCtx, 0u, l_ltUseBuff.uBufL, &l_uCrcGetted) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_BLOBC_RES_OK == l_tCtxCrc32.eLastEr ) )
        {
            if( 4u == l_ltUseBuff.uBufL )
            {
                if( 1u == l_uCrcGetted )
                {
                    (void)printf("eFSS_BLOBCTST_CrcTest 4 -- OK \n");
                }
                else
                {
                    (void)printf("eFSS_BLOBCTST_CrcTest 4 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eFSS_BLOBCTST_CrcTest 4 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_CrcTest 4 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CrcTest 4 -- FAIL \n");
    }

    /* Function */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_CalcCrcInBuff(&l_tCtx, 0x10u, l_ltUseBuff.uBufL, &l_uCrcGetted) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_BLOBC_RES_OK == l_tCtxCrc32.eLastEr ) )
        {
            if( 4u == l_ltUseBuff.uBufL )
            {
                if( 0x11u == l_uCrcGetted )
                {
                    (void)printf("eFSS_BLOBCTST_CrcTest 5 -- OK \n");
                }
                else
                {
                    (void)printf("eFSS_BLOBCTST_CrcTest 5 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eFSS_BLOBCTST_CrcTest 5 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_CrcTest 5 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CrcTest 5 -- FAIL \n");
    }

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;
    l_ltUseBuff.puBuf[4u] = 0x05u;

    /* Function */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_CalcCrcInBuff(&l_tCtx, 0u, l_ltUseBuff.uBufL, &l_uCrcGetted) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_BLOBC_RES_OK == l_tCtxCrc32.eLastEr ) )
        {
            if( 4u == l_ltUseBuff.uBufL )
            {
                if( 0x0Au == l_uCrcGetted )
                {
                    (void)printf("eFSS_BLOBCTST_CrcTest 6 -- OK \n");
                }
                else
                {
                    (void)printf("eFSS_BLOBCTST_CrcTest 6 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eFSS_BLOBCTST_CrcTest 6 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_CrcTest 6 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CrcTest 6 -- FAIL \n");
    }

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;
    l_ltUseBuff.puBuf[4u] = 0x05u;

    /* Function */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_CalcCrcInBuff(&l_tCtx, 0u, 2u, &l_uCrcGetted) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_BLOBC_RES_OK == l_tCtxCrc32.eLastEr ) )
        {
            if( 4u == l_ltUseBuff.uBufL )
            {
                if( 0x03u == l_uCrcGetted )
                {
                    (void)printf("eFSS_BLOBCTST_CrcTest 7 -- OK \n");
                }
                else
                {
                    (void)printf("eFSS_BLOBCTST_CrcTest 7 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eFSS_BLOBCTST_CrcTest 7 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_CrcTest 7 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CrcTest 7 -- FAIL \n");
    }

    /* Setup buffer */
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;
    l_ltUseBuff.puBuf[4u] = 0x05u;

    /* Function */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_CalcCrcInBuff(&l_tCtx, 0u, 1u, &l_uCrcGetted) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_BLOBC_RES_OK == l_tCtxCrc32.eLastEr ) )
        {
            if( 4u == l_ltUseBuff.uBufL )
            {
                if( 0x01u == l_uCrcGetted )
                {
                    (void)printf("eFSS_BLOBCTST_CrcTest 8 -- OK \n");
                }
                else
                {
                    (void)printf("eFSS_BLOBCTST_CrcTest 8 -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eFSS_BLOBCTST_CrcTest 8 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_CrcTest 8 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CrcTest 8 -- FAIL \n");
    }

    if( e_eFSS_BLOBC_RES_BADPARAM == eFSS_BLOBC_CalcCrcInBuff(&l_tCtx, 0u, 5u, &l_uCrcGetted) )
    {
        (void)printf("eFSS_BLOBCTST_CrcTest 9 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_CrcTest 9 -- FAIL \n");
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

static void eFSS_BLOBCTST_LoadTest(void)
{
    /* Local variable */
    t_eFSS_BLOBC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_BLOBC_StorBuf l_ltUseBuff;
    t_eFSS_BLOBC_StorBuf l_ltUseBuff2;
    uint8_t l_uSubTypeRead;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcTst1Adapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 2u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 2  -- FAIL \n");
    }

    /* Setup storage area */
    l_ltUseBuff2.uBufL = l_ltUseBuff.uBufL  + 1u;
    l_ltUseBuff2.puBuf = &l_auStor[24];

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0xFF, sizeof(m_auStorArea2));

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

    if( e_eFSS_BLOBC_RES_NOTVALIDPAGE == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ) )
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 3  -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff2.puBuf[0u] ) && ( 0x12u == l_ltUseBuff2.puBuf[1u] ) && ( 0x13u == l_ltUseBuff2.puBuf[2u] ) &&
        ( 0x14u == l_ltUseBuff2.puBuf[3u] ) && ( 0x15u == l_ltUseBuff2.puBuf[4u] ) )
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 4  -- FAIL \n");
    }

    if( e_eFSS_BLOBC_RES_NOTVALIDPAGE == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 1u, &l_uSubTypeRead) )
    {
        if( ( 0xFFu == l_ltUseBuff.puBuf[0u] ) && ( 0xFFu == l_ltUseBuff.puBuf[1u] ) && ( 0xFFu == l_ltUseBuff.puBuf[2u] ) &&
             ( 0xFFu == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ) )
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 5  -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff2.puBuf[0u] ) && ( 0x12u == l_ltUseBuff2.puBuf[1u] ) && ( 0x13u == l_ltUseBuff2.puBuf[2u] ) &&
        ( 0x14u == l_ltUseBuff2.puBuf[3u] ) && ( 0x15u == l_ltUseBuff2.puBuf[4u] ) )
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 6  -- FAIL \n");
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
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_BLOBC_RES_NOTVALIDPAGE == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ) )
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 7  -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff2.puBuf[0u] ) && ( 0x12u == l_ltUseBuff2.puBuf[1u] ) && ( 0x13u == l_ltUseBuff2.puBuf[2u] ) &&
        ( 0x14u == l_ltUseBuff2.puBuf[3u] ) && ( 0x15u == l_ltUseBuff2.puBuf[4u] ) )
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 8  -- FAIL \n");
    }

    if( e_eFSS_BLOBC_RES_NOTVALIDPAGE == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 1u, &l_uSubTypeRead) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] ) && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0x10u == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ))
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 9  -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff2.puBuf[0u] ) && ( 0x12u == l_ltUseBuff2.puBuf[1u] ) && ( 0x13u == l_ltUseBuff2.puBuf[2u] ) &&
        ( 0x14u == l_ltUseBuff2.puBuf[3u] ) && ( 0x15u == l_ltUseBuff2.puBuf[4u] ) )
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 10 -- FAIL \n");
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
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_BLOBC_RES_NOTVALIDPAGE == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ))
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 11 -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff2.puBuf[0u] ) && ( 0x12u == l_ltUseBuff2.puBuf[1u] ) && ( 0x13u == l_ltUseBuff2.puBuf[2u] ) &&
        ( 0x14u == l_ltUseBuff2.puBuf[3u] ) && ( 0x15u == l_ltUseBuff2.puBuf[4u] ) )
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 12 -- FAIL \n");
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
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_BLOBC_RES_NOTVALIDPAGE == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ))
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 13 -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff2.puBuf[0u] ) && ( 0x12u == l_ltUseBuff2.puBuf[1u] ) && ( 0x13u == l_ltUseBuff2.puBuf[2u] ) &&
        ( 0x14u == l_ltUseBuff2.puBuf[3u] ) && ( 0x15u == l_ltUseBuff2.puBuf[4u] ) )
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 14 -- FAIL \n");
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
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_BLOBC_RES_NOTVALIDPAGE == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ))
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 15 -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff2.puBuf[0u] ) && ( 0x12u == l_ltUseBuff2.puBuf[1u] ) && ( 0x13u == l_ltUseBuff2.puBuf[2u] ) &&
        ( 0x14u == l_ltUseBuff2.puBuf[3u] ) && ( 0x15u == l_ltUseBuff2.puBuf[4u] ) )
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 15 -- FAIL \n");
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
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ))
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 17 -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff2.puBuf[0u] ) && ( 0x12u == l_ltUseBuff2.puBuf[1u] ) && ( 0x13u == l_ltUseBuff2.puBuf[2u] ) &&
        ( 0x14u == l_ltUseBuff2.puBuf[3u] ) && ( 0x15u == l_ltUseBuff2.puBuf[4u] ) )
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 18 -- FAIL \n");
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
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_BLOBC_RES_NOTVALIDPAGE == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ))
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 19 -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff2.puBuf[0u] ) && ( 0x12u == l_ltUseBuff2.puBuf[1u] ) && ( 0x13u == l_ltUseBuff2.puBuf[2u] ) &&
        ( 0x14u == l_ltUseBuff2.puBuf[3u] ) && ( 0x15u == l_ltUseBuff2.puBuf[4u] ) )
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 20 -- FAIL \n");
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
    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;

    l_ltUseBuff2.puBuf[0u] = 0x11u;
    l_ltUseBuff2.puBuf[1u] = 0x12u;
    l_ltUseBuff2.puBuf[2u] = 0x13u;
    l_ltUseBuff2.puBuf[3u] = 0x14u;
    l_ltUseBuff2.puBuf[4u] = 0x15u;

    if( e_eFSS_BLOBC_RES_NEWVERSIONFOUND == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ))
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 21 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 21 -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff2.puBuf[0u] ) && ( 0x12u == l_ltUseBuff2.puBuf[1u] ) && ( 0x13u == l_ltUseBuff2.puBuf[2u] ) &&
        ( 0x14u == l_ltUseBuff2.puBuf[3u] ) && ( 0x15u == l_ltUseBuff2.puBuf[4u] ) )
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 22 -- FAIL \n");
    }

    /* Setup storage area */
    l_uSubTypeRead = 0u;

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u] = 0x00u;  /* Page SUBTYPE */
    m_auStorArea1[5u] = 0x00u;  /* Page index */
    m_auStorArea1[6u] = 0x00u;  /* Page index */
    m_auStorArea1[7u] = 0x00u;  /* Page index */
    m_auStorArea1[8u] = 0x00u;  /* Page index */
    m_auStorArea1[9u] = 0x01u;  /* Page type */
    m_auStorArea1[10u] = 0x01u; /* Page version */
    m_auStorArea1[11u] = 0x00u; /* Page version */
    m_auStorArea1[12u] = 0x02u; /* Total page */
    m_auStorArea1[13u] = 0x00u; /* Total page */
    m_auStorArea1[14u] = 0x00u; /* Total page */
    m_auStorArea1[15u] = 0x00u; /* Total page */
    m_auStorArea1[16u] = 0xA5u; /* Magic number */
    m_auStorArea1[17u] = 0xA5u; /* Magic number */
    m_auStorArea1[18u] = 0xA5u; /* Magic number */
    m_auStorArea1[19u] = 0xA5u; /* Magic number */
    m_auStorArea1[20u] = 0x97u; /* CRC */
    m_auStorArea1[21u] = 0x02u; /* CRC */
    m_auStorArea1[22u] = 0x00u; /* CRC */
    m_auStorArea1[23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[4u] = 0x00u;  /* Page SUBTYPE */
    m_auStorArea2[5u] = 0x01u;  /* Page index */
    m_auStorArea2[6u] = 0x00u;  /* Page index */
    m_auStorArea2[7u] = 0x00u;  /* Page index */
    m_auStorArea2[8u] = 0x00u;  /* Page index */
    m_auStorArea2[9u] = 0x01u;  /* Page type */
    m_auStorArea2[10u] = 0x01u; /* Page version */
    m_auStorArea2[11u] = 0x00u; /* Page version */
    m_auStorArea2[12u] = 0x02u; /* Total page */
    m_auStorArea2[13u] = 0x00u; /* Total page */
    m_auStorArea2[14u] = 0x00u; /* Total page */
    m_auStorArea2[15u] = 0x00u; /* Total page */
    m_auStorArea2[16u] = 0xA5u; /* Magic number */
    m_auStorArea2[17u] = 0xA5u; /* Magic number */
    m_auStorArea2[18u] = 0xA5u; /* Magic number */
    m_auStorArea2[19u] = 0xA5u; /* Magic number */
    m_auStorArea2[20u] = 0xD8u; /* CRC */
    m_auStorArea2[21u] = 0x02u; /* CRC */
    m_auStorArea2[22u] = 0x00u; /* CRC */
    m_auStorArea2[23u] = 0x00u; /* CRC */

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ))
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 23 -- FAIL \n");
    }

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 1u, &l_uSubTypeRead) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] ) && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0x10u == l_ltUseBuff.puBuf[3u] ) && ( 0u == l_uSubTypeRead ))
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 24 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 24 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 24 -- FAIL \n");
    }

    /* Setup storage area */
    l_uSubTypeRead = 0u;

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u] = 0x03u;  /* Page SUBTYPE */
    m_auStorArea1[5u] = 0x00u;  /* Page index */
    m_auStorArea1[6u] = 0x00u;  /* Page index */
    m_auStorArea1[7u] = 0x00u;  /* Page index */
    m_auStorArea1[8u] = 0x00u;  /* Page index */
    m_auStorArea1[9u] = 0x01u;  /* Page type */
    m_auStorArea1[10u] = 0x01u; /* Page version */
    m_auStorArea1[11u] = 0x00u; /* Page version */
    m_auStorArea1[12u] = 0x02u; /* Total page */
    m_auStorArea1[13u] = 0x00u; /* Total page */
    m_auStorArea1[14u] = 0x00u; /* Total page */
    m_auStorArea1[15u] = 0x00u; /* Total page */
    m_auStorArea1[16u] = 0xA5u; /* Magic number */
    m_auStorArea1[17u] = 0xA5u; /* Magic number */
    m_auStorArea1[18u] = 0xA5u; /* Magic number */
    m_auStorArea1[19u] = 0xA5u; /* Magic number */
    m_auStorArea1[20u] = 0x9Au; /* CRC */
    m_auStorArea1[21u] = 0x02u; /* CRC */
    m_auStorArea1[22u] = 0x00u; /* CRC */
    m_auStorArea1[23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[4u] = 0x04u;  /* Page SUBTYPE */
    m_auStorArea2[5u] = 0x01u;  /* Page index */
    m_auStorArea2[6u] = 0x00u;  /* Page index */
    m_auStorArea2[7u] = 0x00u;  /* Page index */
    m_auStorArea2[8u] = 0x00u;  /* Page index */
    m_auStorArea2[9u] = 0x01u;  /* Page type */
    m_auStorArea2[10u] = 0x01u; /* Page version */
    m_auStorArea2[11u] = 0x00u; /* Page version */
    m_auStorArea2[12u] = 0x02u; /* Total page */
    m_auStorArea2[13u] = 0x00u; /* Total page */
    m_auStorArea2[14u] = 0x00u; /* Total page */
    m_auStorArea2[15u] = 0x00u; /* Total page */
    m_auStorArea2[16u] = 0xA5u; /* Magic number */
    m_auStorArea2[17u] = 0xA5u; /* Magic number */
    m_auStorArea2[18u] = 0xA5u; /* Magic number */
    m_auStorArea2[19u] = 0xA5u; /* Magic number */
    m_auStorArea2[20u] = 0xDCu; /* CRC */
    m_auStorArea2[21u] = 0x02u; /* CRC */
    m_auStorArea2[22u] = 0x00u; /* CRC */
    m_auStorArea2[23u] = 0x00u; /* CRC */

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0u == l_ltUseBuff.puBuf[3u] ) && ( 3u == l_uSubTypeRead ))
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 25 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 25 -- FAIL \n");
    }

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 1u, &l_uSubTypeRead) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] ) && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
             ( 0x10u == l_ltUseBuff.puBuf[3u] ) && ( 4u == l_uSubTypeRead ))
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 26 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadTest 26 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadTest 26 -- FAIL \n");
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

static void eFSS_BLOBCTST_FlushTest(void)
{
    /* Local variable */
    t_eFSS_BLOBC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_BLOBC_StorBuf l_ltUseBuff;
    t_eFSS_BLOBC_StorBuf l_ltUseBuff2;
    uint8_t l_uSubTypeWrite;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcTst1Adapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 2u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_FlushTest 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_FlushTest 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_FlushTest 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_FlushTest 2  -- FAIL \n");
    }

    /* Setup storage area */
    l_ltUseBuff2.uBufL = l_ltUseBuff.uBufL  + 1u;
    l_ltUseBuff2.puBuf = &l_auStor[24];

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0xFF, sizeof(m_auStorArea2));

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
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_FlushBuffInPage(&l_tCtx, 0u, l_uSubTypeWrite) )
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
            if( ( 0x01u== l_ltUseBuff.puBuf[0u] )  && ( 0x02u== l_ltUseBuff.puBuf[1u] )  && ( 0x03u== l_ltUseBuff.puBuf[2u] ) &&
                ( 0x04u== l_ltUseBuff.puBuf[3u] )  && ( 0x05u== l_ltUseBuff.puBuf[4u] )  &&
                ( 0x01u== l_ltUseBuff2.puBuf[0u] )  && ( 0x02u== l_ltUseBuff2.puBuf[1u] )  && ( 0x03u== l_ltUseBuff2.puBuf[2u] ) &&
                ( 0x04u== l_ltUseBuff2.puBuf[3u] )  && ( 0x05u== l_ltUseBuff2.puBuf[4u] ) )
            {
                (void)printf("eFSS_BLOBCTST_FlushTest 3  -- OK \n");
            }
            else
            {
                (void)printf("eFSS_BLOBCTST_FlushTest 3  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_FlushTest 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_FlushTest 3  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0xFF, sizeof(m_auStorArea2));

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
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_FlushBuffInPage(&l_tCtx, 1u, l_uSubTypeWrite) )
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
            if( ( 0x01u== l_ltUseBuff.puBuf[0u] )  && ( 0x02u== l_ltUseBuff.puBuf[1u] )  && ( 0x03u== l_ltUseBuff.puBuf[2u] ) &&
                ( 0x04u== l_ltUseBuff.puBuf[3u] )  && ( 0x05u== l_ltUseBuff.puBuf[4u] )  &&
                ( 0x01u== l_ltUseBuff2.puBuf[0u] )  && ( 0x02u== l_ltUseBuff2.puBuf[1u] )  && ( 0x03u== l_ltUseBuff2.puBuf[2u] ) &&
                ( 0x04u== l_ltUseBuff2.puBuf[3u] )  && ( 0x05u== l_ltUseBuff2.puBuf[4u] ) )
            {
                (void)printf("eFSS_BLOBCTST_FlushTest 4  -- OK \n");
            }
            else
            {
                (void)printf("eFSS_BLOBCTST_FlushTest 4  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_FlushTest 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_FlushTest 4  -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadTst2Adapt;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_FlushTest 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_FlushTest 5  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_FlushTest 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_FlushTest 6  -- FAIL \n");
    }

    /* Setup storage area */
    l_ltUseBuff2.uBufL = l_ltUseBuff.uBufL  + 1u;
    l_ltUseBuff2.puBuf = &l_auStor[24];

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0xFF, sizeof(m_auStorArea2));

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
    if( e_eFSS_BLOBC_RES_WRITENOMATCHREAD == eFSS_BLOBC_FlushBuffInPage(&l_tCtx, 0u, l_uSubTypeWrite) )
    {
        (void)printf("eFSS_BLOBCTST_FlushTest 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_FlushTest 7  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0xFF, sizeof(m_auStorArea2));

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
    if( e_eFSS_BLOBC_RES_WRITENOMATCHREAD == eFSS_BLOBC_FlushBuffInPage(&l_tCtx, 1u, l_uSubTypeWrite) )
    {
        (void)printf("eFSS_BLOBCTST_FlushTest 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_FlushTest 8  -- FAIL \n");
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

static void eFSS_BLOBCTST_Compare(void)
{
    /* Local variable */
    t_eFSS_BLOBC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_BLOBC_StorBuf l_ltUseBuff;
    t_eFSS_BLOBC_StorBuf l_ltUseBuff2;
    uint8_t l_uSubTypeRead;
    bool_t l_bIsEquals;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcTst1Adapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 2u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;
    l_bIsEquals = false;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_Compare 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_Compare 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_Compare 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_Compare 2  -- FAIL \n");
    }

    /* Setup storage area */
    l_ltUseBuff2.uBufL = l_ltUseBuff.uBufL  + 1u;
    l_ltUseBuff2.puBuf = &l_auStor[24];

    /* Setup storage area */
    l_uSubTypeRead = 0u;

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u] = 0x00u;  /* Page SUBTYPE */
    m_auStorArea1[5u] = 0x00u;  /* Page index */
    m_auStorArea1[6u] = 0x00u;  /* Page index */
    m_auStorArea1[7u] = 0x00u;  /* Page index */
    m_auStorArea1[8u] = 0x00u;  /* Page index */
    m_auStorArea1[9u] = 0x01u;  /* Page type */
    m_auStorArea1[10u] = 0x01u; /* Page version */
    m_auStorArea1[11u] = 0x00u; /* Page version */
    m_auStorArea1[12u] = 0x02u; /* Total page */
    m_auStorArea1[13u] = 0x00u; /* Total page */
    m_auStorArea1[14u] = 0x00u; /* Total page */
    m_auStorArea1[15u] = 0x00u; /* Total page */
    m_auStorArea1[16u] = 0xA5u; /* Magic number */
    m_auStorArea1[17u] = 0xA5u; /* Magic number */
    m_auStorArea1[18u] = 0xA5u; /* Magic number */
    m_auStorArea1[19u] = 0xA5u; /* Magic number */
    m_auStorArea1[20u] = 0x97u; /* CRC */
    m_auStorArea1[21u] = 0x02u; /* CRC */
    m_auStorArea1[22u] = 0x00u; /* CRC */
    m_auStorArea1[23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[4u] = 0x00u;  /* Page SUBTYPE */
    m_auStorArea2[5u] = 0x01u;  /* Page index */
    m_auStorArea2[6u] = 0x00u;  /* Page index */
    m_auStorArea2[7u] = 0x00u;  /* Page index */
    m_auStorArea2[8u] = 0x00u;  /* Page index */
    m_auStorArea2[9u] = 0x01u;  /* Page type */
    m_auStorArea2[10u] = 0x01u; /* Page version */
    m_auStorArea2[11u] = 0x00u; /* Page version */
    m_auStorArea2[12u] = 0x02u; /* Total page */
    m_auStorArea2[13u] = 0x00u; /* Total page */
    m_auStorArea2[14u] = 0x00u; /* Total page */
    m_auStorArea2[15u] = 0x00u; /* Total page */
    m_auStorArea2[16u] = 0xA5u; /* Magic number */
    m_auStorArea2[17u] = 0xA5u; /* Magic number */
    m_auStorArea2[18u] = 0xA5u; /* Magic number */
    m_auStorArea2[19u] = 0xA5u; /* Magic number */
    m_auStorArea2[20u] = 0xD8u; /* CRC */
    m_auStorArea2[21u] = 0x02u; /* CRC */
    m_auStorArea2[22u] = 0x00u; /* CRC */
    m_auStorArea2[23u] = 0x00u; /* CRC */

    /* fill the buffer */
    l_ltUseBuff.puBuf[0u] = 0x00u;
    l_ltUseBuff.puBuf[1u] = 0x00u;
    l_ltUseBuff.puBuf[2u] = 0x00u;
    l_ltUseBuff.puBuf[3u] = 0x00u;

    /* Compare */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_IsBuffEqualToPage(&l_tCtx, 0u, &l_bIsEquals, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0u == l_ltUseBuff.puBuf[3u] ) && ( true == l_bIsEquals ) && ( 0u == l_uSubTypeRead ) &&
            ( 0u == l_ltUseBuff2.puBuf[0u] ) && ( 0u == l_ltUseBuff2.puBuf[1u] ) && ( 0u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0u == l_ltUseBuff2.puBuf[3u] ) && ( 0u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_BLOBCTST_Compare 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_Compare 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_Compare 3  -- FAIL \n");
    }

    /* Compare */
    l_ltUseBuff.puBuf[0u] = 0x10u;
    l_ltUseBuff.puBuf[1u] = 0x10u;
    l_ltUseBuff.puBuf[2u] = 0x10u;
    l_ltUseBuff.puBuf[3u] = 0x10u;
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_IsBuffEqualToPage(&l_tCtx, 1u, &l_bIsEquals, &l_uSubTypeRead) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] ) && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] ) && ( true == l_bIsEquals ) && ( 0u == l_uSubTypeRead ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x00u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_BLOBCTST_Compare 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_Compare 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_Compare 4  -- FAIL \n");
    }

    /* Setup storage area */
    l_uSubTypeRead = 0u;

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u] = 0x03u;  /* Page SUBTYPE */
    m_auStorArea1[5u] = 0x00u;  /* Page index */
    m_auStorArea1[6u] = 0x00u;  /* Page index */
    m_auStorArea1[7u] = 0x00u;  /* Page index */
    m_auStorArea1[8u] = 0x00u;  /* Page index */
    m_auStorArea1[9u] = 0x01u;  /* Page type */
    m_auStorArea1[10u] = 0x01u; /* Page version */
    m_auStorArea1[11u] = 0x00u; /* Page version */
    m_auStorArea1[12u] = 0x02u; /* Total page */
    m_auStorArea1[13u] = 0x00u; /* Total page */
    m_auStorArea1[14u] = 0x00u; /* Total page */
    m_auStorArea1[15u] = 0x00u; /* Total page */
    m_auStorArea1[16u] = 0xA5u; /* Magic number */
    m_auStorArea1[17u] = 0xA5u; /* Magic number */
    m_auStorArea1[18u] = 0xA5u; /* Magic number */
    m_auStorArea1[19u] = 0xA5u; /* Magic number */
    m_auStorArea1[20u] = 0x9Au; /* CRC */
    m_auStorArea1[21u] = 0x02u; /* CRC */
    m_auStorArea1[22u] = 0x00u; /* CRC */
    m_auStorArea1[23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[4u] = 0x04u;  /* Page SUBTYPE */
    m_auStorArea2[5u] = 0x01u;  /* Page index */
    m_auStorArea2[6u] = 0x00u;  /* Page index */
    m_auStorArea2[7u] = 0x00u;  /* Page index */
    m_auStorArea2[8u] = 0x00u;  /* Page index */
    m_auStorArea2[9u] = 0x01u;  /* Page type */
    m_auStorArea2[10u] = 0x01u; /* Page version */
    m_auStorArea2[11u] = 0x00u; /* Page version */
    m_auStorArea2[12u] = 0x02u; /* Total page */
    m_auStorArea2[13u] = 0x00u; /* Total page */
    m_auStorArea2[14u] = 0x00u; /* Total page */
    m_auStorArea2[15u] = 0x00u; /* Total page */
    m_auStorArea2[16u] = 0xA5u; /* Magic number */
    m_auStorArea2[17u] = 0xA5u; /* Magic number */
    m_auStorArea2[18u] = 0xA5u; /* Magic number */
    m_auStorArea2[19u] = 0xA5u; /* Magic number */
    m_auStorArea2[20u] = 0xDCu; /* CRC */
    m_auStorArea2[21u] = 0x02u; /* CRC */
    m_auStorArea2[22u] = 0x00u; /* CRC */
    m_auStorArea2[23u] = 0x00u; /* CRC */

    /* fill the buffer */
    l_ltUseBuff.puBuf[0u] = 0x00u;
    l_ltUseBuff.puBuf[1u] = 0x00u;
    l_ltUseBuff.puBuf[2u] = 0x00u;
    l_ltUseBuff.puBuf[3u] = 0x00u;

    /* Compare */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_IsBuffEqualToPage(&l_tCtx, 0u, &l_bIsEquals, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0u == l_ltUseBuff.puBuf[3u] ) && ( true == l_bIsEquals ) && ( 3u == l_uSubTypeRead ) &&
            ( 0u == l_ltUseBuff2.puBuf[0u] ) && ( 0u == l_ltUseBuff2.puBuf[1u] ) && ( 0u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0u == l_ltUseBuff2.puBuf[3u] ) && ( 0x03u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_BLOBCTST_Compare 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_Compare 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_Compare 5  -- FAIL \n");
    }

    /* Compare */
    l_ltUseBuff.puBuf[0u] = 0x10u;
    l_ltUseBuff.puBuf[1u] = 0x10u;
    l_ltUseBuff.puBuf[2u] = 0x10u;
    l_ltUseBuff.puBuf[3u] = 0x10u;

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_IsBuffEqualToPage(&l_tCtx, 1u, &l_bIsEquals, &l_uSubTypeRead) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] ) && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] ) && ( true == l_bIsEquals ) && ( 4u == l_uSubTypeRead ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_BLOBCTST_Compare 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_Compare 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_Compare 6  -- FAIL \n");
    }

    /* Setup storage area */
    l_uSubTypeRead = 0u;

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u] = 0x03u;  /* Page SUBTYPE */
    m_auStorArea1[5u] = 0x00u;  /* Page index */
    m_auStorArea1[6u] = 0x00u;  /* Page index */
    m_auStorArea1[7u] = 0x00u;  /* Page index */
    m_auStorArea1[8u] = 0x00u;  /* Page index */
    m_auStorArea1[9u] = 0x01u;  /* Page type */
    m_auStorArea1[10u] = 0x01u; /* Page version */
    m_auStorArea1[11u] = 0x00u; /* Page version */
    m_auStorArea1[12u] = 0x02u; /* Total page */
    m_auStorArea1[13u] = 0x00u; /* Total page */
    m_auStorArea1[14u] = 0x00u; /* Total page */
    m_auStorArea1[15u] = 0x00u; /* Total page */
    m_auStorArea1[16u] = 0xA5u; /* Magic number */
    m_auStorArea1[17u] = 0xA5u; /* Magic number */
    m_auStorArea1[18u] = 0xA5u; /* Magic number */
    m_auStorArea1[19u] = 0xA5u; /* Magic number */
    m_auStorArea1[20u] = 0x9Au; /* CRC */
    m_auStorArea1[21u] = 0x02u; /* CRC */
    m_auStorArea1[22u] = 0x00u; /* CRC */
    m_auStorArea1[23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[4u] = 0x04u;  /* Page SUBTYPE */
    m_auStorArea2[5u] = 0x01u;  /* Page index */
    m_auStorArea2[6u] = 0x00u;  /* Page index */
    m_auStorArea2[7u] = 0x00u;  /* Page index */
    m_auStorArea2[8u] = 0x00u;  /* Page index */
    m_auStorArea2[9u] = 0x01u;  /* Page type */
    m_auStorArea2[10u] = 0x01u; /* Page version */
    m_auStorArea2[11u] = 0x00u; /* Page version */
    m_auStorArea2[12u] = 0x02u; /* Total page */
    m_auStorArea2[13u] = 0x00u; /* Total page */
    m_auStorArea2[14u] = 0x00u; /* Total page */
    m_auStorArea2[15u] = 0x00u; /* Total page */
    m_auStorArea2[16u] = 0xA5u; /* Magic number */
    m_auStorArea2[17u] = 0xA5u; /* Magic number */
    m_auStorArea2[18u] = 0xA5u; /* Magic number */
    m_auStorArea2[19u] = 0xA5u; /* Magic number */
    m_auStorArea2[20u] = 0xDCu; /* CRC */
    m_auStorArea2[21u] = 0x02u; /* CRC */
    m_auStorArea2[22u] = 0x00u; /* CRC */
    m_auStorArea2[23u] = 0x00u; /* CRC */

    /* fill the buffer */
    l_ltUseBuff.puBuf[0u] = 0x00u;
    l_ltUseBuff.puBuf[1u] = 0x00u;
    l_ltUseBuff.puBuf[2u] = 0x00u;
    l_ltUseBuff.puBuf[3u] = 0x00u;

    /* Compare */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_IsBuffEqualToPage(&l_tCtx, 1u, &l_bIsEquals, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0u == l_ltUseBuff.puBuf[3u] ) && ( false == l_bIsEquals ) && ( 4u == l_uSubTypeRead ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_BLOBCTST_Compare 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_Compare 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_Compare 7  -- FAIL \n");
    }

    /* Compare */
    l_ltUseBuff.puBuf[0u] = 0x10u;
    l_ltUseBuff.puBuf[1u] = 0x10u;
    l_ltUseBuff.puBuf[2u] = 0x10u;
    l_ltUseBuff.puBuf[3u] = 0x10u;

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_IsBuffEqualToPage(&l_tCtx, 0u, &l_bIsEquals, &l_uSubTypeRead) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] ) && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] ) && ( false == l_bIsEquals ) && ( 3u == l_uSubTypeRead ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[0u] ) && ( 0x00u == l_ltUseBuff2.puBuf[1u] ) && ( 0x00u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[3u] ) && ( 0x03u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_BLOBCTST_Compare 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_Compare 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_Compare 8  -- FAIL \n");
    }

    /* Setup storage area */
    l_uSubTypeRead = 0u;

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u] = 0x03u;  /* Page SUBTYPE */
    m_auStorArea1[5u] = 0x00u;  /* Page index */
    m_auStorArea1[6u] = 0x00u;  /* Page index */
    m_auStorArea1[7u] = 0x00u;  /* Page index */
    m_auStorArea1[8u] = 0x00u;  /* Page index */
    m_auStorArea1[9u] = 0x01u;  /* Page type */
    m_auStorArea1[10u] = 0x01u; /* Page version */
    m_auStorArea1[11u] = 0x00u; /* Page version */
    m_auStorArea1[12u] = 0x02u; /* Total page */
    m_auStorArea1[13u] = 0x00u; /* Total page */
    m_auStorArea1[14u] = 0x00u; /* Total page */
    m_auStorArea1[15u] = 0x00u; /* Total page */
    m_auStorArea1[16u] = 0xA5u; /* Magic number */
    m_auStorArea1[17u] = 0xA5u; /* Magic number */
    m_auStorArea1[18u] = 0xA5u; /* Magic number */
    m_auStorArea1[19u] = 0xA5u; /* Magic number */
    m_auStorArea1[20u] = 0x00u; /* CRC */
    m_auStorArea1[21u] = 0x00u; /* CRC */
    m_auStorArea1[22u] = 0x00u; /* CRC */
    m_auStorArea1[23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[4u] = 0x04u;  /* Page SUBTYPE */
    m_auStorArea2[5u] = 0x01u;  /* Page index */
    m_auStorArea2[6u] = 0x00u;  /* Page index */
    m_auStorArea2[7u] = 0x00u;  /* Page index */
    m_auStorArea2[8u] = 0x00u;  /* Page index */
    m_auStorArea2[9u] = 0x01u;  /* Page type */
    m_auStorArea2[10u] = 0x01u; /* Page version */
    m_auStorArea2[11u] = 0x00u; /* Page version */
    m_auStorArea2[12u] = 0x02u; /* Total page */
    m_auStorArea2[13u] = 0x00u; /* Total page */
    m_auStorArea2[14u] = 0x00u; /* Total page */
    m_auStorArea2[15u] = 0x00u; /* Total page */
    m_auStorArea2[16u] = 0xA5u; /* Magic number */
    m_auStorArea2[17u] = 0xA5u; /* Magic number */
    m_auStorArea2[18u] = 0xA5u; /* Magic number */
    m_auStorArea2[19u] = 0xA5u; /* Magic number */
    m_auStorArea2[20u] = 0x00u; /* CRC */
    m_auStorArea2[21u] = 0x00u; /* CRC */
    m_auStorArea2[22u] = 0x00u; /* CRC */
    m_auStorArea2[23u] = 0x00u; /* CRC */

    /* fill the buffer */
    l_ltUseBuff.puBuf[0u] = 0x00u;
    l_ltUseBuff.puBuf[1u] = 0x00u;
    l_ltUseBuff.puBuf[2u] = 0x00u;
    l_ltUseBuff.puBuf[3u] = 0x00u;

    /* Compare */
    if( e_eFSS_BLOBC_RES_NOTVALIDPAGE == eFSS_BLOBC_IsBuffEqualToPage(&l_tCtx, 1u, &l_bIsEquals, &l_uSubTypeRead) )
    {
        if( ( 0u == l_ltUseBuff.puBuf[0u] ) && ( 0u == l_ltUseBuff.puBuf[1u] ) && ( 0u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0u == l_ltUseBuff.puBuf[3u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_BLOBCTST_Compare 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_Compare 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_Compare 9  -- FAIL \n");
    }

    /* Compare */
    l_ltUseBuff.puBuf[0u] = 0x10u;
    l_ltUseBuff.puBuf[1u] = 0x10u;
    l_ltUseBuff.puBuf[2u] = 0x10u;
    l_ltUseBuff.puBuf[3u] = 0x10u;

    if( e_eFSS_BLOBC_RES_NOTVALIDPAGE == eFSS_BLOBC_IsBuffEqualToPage(&l_tCtx, 0u, &l_bIsEquals, &l_uSubTypeRead) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] ) && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[0u] ) && ( 0x00u == l_ltUseBuff2.puBuf[1u] ) && ( 0x00u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[3u] ) && ( 0x03u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_BLOBCTST_Compare 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_Compare 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_Compare 10 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[5u] = 0x00u;  /* Page index */
    m_auStorArea1[6u] = 0x00u;  /* Page index */
    m_auStorArea1[7u] = 0x00u;  /* Page index */
    m_auStorArea1[8u] = 0x00u;  /* Page index */
    m_auStorArea1[9u] = 0x01u;  /* Page type */
    m_auStorArea1[10u] = 0x00u; /* Page version */
    m_auStorArea1[11u] = 0x00u; /* Page version */
    m_auStorArea1[12u] = 0x02u; /* Total page */
    m_auStorArea1[13u] = 0x00u; /* Total page */
    m_auStorArea1[14u] = 0x00u; /* Total page */
    m_auStorArea1[15u] = 0x00u; /* Total page */
    m_auStorArea1[16u] = 0xA5u; /* Magic number */
    m_auStorArea1[17u] = 0xA5u; /* Magic number */
    m_auStorArea1[18u] = 0xA5u; /* Magic number */
    m_auStorArea1[19u] = 0xA5u; /* Magic number */
    m_auStorArea1[20u] = 0x96u; /* CRC */
    m_auStorArea1[21u] = 0x02u; /* CRC */
    m_auStorArea1[22u] = 0x00u; /* CRC */
    m_auStorArea1[23u] = 0x00u; /* CRC */

    /* Compare */
    l_ltUseBuff.puBuf[0u] = 0x10u;
    l_ltUseBuff.puBuf[1u] = 0x10u;
    l_ltUseBuff.puBuf[2u] = 0x10u;
    l_ltUseBuff.puBuf[3u] = 0x10u;

    if( e_eFSS_BLOBC_RES_NEWVERSIONFOUND == eFSS_BLOBC_IsBuffEqualToPage(&l_tCtx, 0u, &l_bIsEquals, &l_uSubTypeRead) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] ) && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[0u] ) && ( 0x00u == l_ltUseBuff2.puBuf[1u] ) && ( 0x00u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[3u] ) && ( 0x00u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_BLOBCTST_Compare 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_Compare 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_Compare 11 -- FAIL \n");
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


static void eFSS_BLOBCTST_LoadBkupTest(void)
{
    /* Local variable */
    t_eFSS_BLOBC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_BLOBC_StorBuf l_ltUseBuff;
    t_eFSS_BLOBC_StorBuf l_ltUseBuff2;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcTst1Adapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 2u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_LoadBkupTest 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadBkupTest 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_LoadBkupTest 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadBkupTest 2  -- FAIL \n");
    }

    /* Setup storage area */
    l_ltUseBuff2.uBufL = l_ltUseBuff.uBufL  + 1u;
    l_ltUseBuff2.puBuf = &l_auStor[24];

    /* --------------------------------------------- All OK no backup generation */
    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u]  = 0x03u; /* Page SUBTYPE */
    m_auStorArea1[5u]  = 0x00u; /* Page index */
    m_auStorArea1[6u]  = 0x00u; /* Page index */
    m_auStorArea1[7u]  = 0x00u; /* Page index */
    m_auStorArea1[8u]  = 0x00u; /* Page index */
    m_auStorArea1[9u]  = 0x01u; /* Page type */
    m_auStorArea1[10u] = 0x01u; /* Page version */
    m_auStorArea1[11u] = 0x00u; /* Page version */
    m_auStorArea1[12u] = 0x02u; /* Total page */
    m_auStorArea1[13u] = 0x00u; /* Total page */
    m_auStorArea1[14u] = 0x00u; /* Total page */
    m_auStorArea1[15u] = 0x00u; /* Total page */
    m_auStorArea1[16u] = 0xA5u; /* Magic number */
    m_auStorArea1[17u] = 0xA5u; /* Magic number */
    m_auStorArea1[18u] = 0xA5u; /* Magic number */
    m_auStorArea1[19u] = 0xA5u; /* Magic number */
    m_auStorArea1[20u] = 0x9Au; /* CRC */
    m_auStorArea1[21u] = 0x02u; /* CRC */
    m_auStorArea1[22u] = 0x00u; /* CRC */
    m_auStorArea1[23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea2, 0, sizeof(m_auStorArea2));
    m_auStorArea2[4u]  = 0x04u; /* Page SUBTYPE */
    m_auStorArea2[5u]  = 0x01u; /* Page index */
    m_auStorArea2[6u]  = 0x00u; /* Page index */
    m_auStorArea2[7u]  = 0x00u; /* Page index */
    m_auStorArea2[8u]  = 0x00u; /* Page index */
    m_auStorArea2[9u]  = 0x01u; /* Page type */
    m_auStorArea2[10u] = 0x01u; /* Page version */
    m_auStorArea2[11u] = 0x00u; /* Page version */
    m_auStorArea2[12u] = 0x02u; /* Total page */
    m_auStorArea2[13u] = 0x00u; /* Total page */
    m_auStorArea2[14u] = 0x00u; /* Total page */
    m_auStorArea2[15u] = 0x00u; /* Total page */
    m_auStorArea2[16u] = 0xA5u; /* Magic number */
    m_auStorArea2[17u] = 0xA5u; /* Magic number */
    m_auStorArea2[18u] = 0xA5u; /* Magic number */
    m_auStorArea2[19u] = 0xA5u; /* Magic number */
    m_auStorArea2[20u] = 0x9Cu; /* CRC */
    m_auStorArea2[21u] = 0x02u; /* CRC */
    m_auStorArea2[22u] = 0x00u; /* CRC */
    m_auStorArea2[23u] = 0x00u; /* CRC */

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        if( ( 0x00u == l_ltUseBuff.puBuf[0u] )  && ( 0x00u == l_ltUseBuff.puBuf[1u] ) && ( 0x00u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[3u] )  && ( 0x03u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x00u == l_ltUseBuff2.puBuf[0u] ) && ( 0x00u == l_ltUseBuff2.puBuf[1u] ) && ( 0x00u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadBkupTest 3  -- FAIL \n");
    }

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 1u, 0u, 0x04u, 0x03u) )
    {
        if( ( 0x00u == l_ltUseBuff.puBuf[0u] )  && ( 0x00u == l_ltUseBuff.puBuf[1u] ) && ( 0x00u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[3u] )  && ( 0x04u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x00u == l_ltUseBuff2.puBuf[0u] ) && ( 0x00u == l_ltUseBuff2.puBuf[1u] ) && ( 0x00u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[3u] ) && ( 0x03u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadBkupTest 4  -- FAIL \n");
    }


    /* --------------------------------------------- All OK no backup generation */
    /* Setup storage area */
    (void)memset(m_auStorArea1, 1, sizeof(m_auStorArea1));
    m_auStorArea1[4u]  = 0x03u; /* Page SUBTYPE */
    m_auStorArea1[5u]  = 0x00u; /* Page index */
    m_auStorArea1[6u]  = 0x00u; /* Page index */
    m_auStorArea1[7u]  = 0x00u; /* Page index */
    m_auStorArea1[8u]  = 0x00u; /* Page index */
    m_auStorArea1[9u]  = 0x01u; /* Page type */
    m_auStorArea1[10u] = 0x01u; /* Page version */
    m_auStorArea1[11u] = 0x00u; /* Page version */
    m_auStorArea1[12u] = 0x02u; /* Total page */
    m_auStorArea1[13u] = 0x00u; /* Total page */
    m_auStorArea1[14u] = 0x00u; /* Total page */
    m_auStorArea1[15u] = 0x00u; /* Total page */
    m_auStorArea1[16u] = 0xA5u; /* Magic number */
    m_auStorArea1[17u] = 0xA5u; /* Magic number */
    m_auStorArea1[18u] = 0xA5u; /* Magic number */
    m_auStorArea1[19u] = 0xA5u; /* Magic number */
    m_auStorArea1[20u] = 0x9Eu; /* CRC */
    m_auStorArea1[21u] = 0x02u; /* CRC */
    m_auStorArea1[22u] = 0x00u; /* CRC */
    m_auStorArea1[23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea2, 1, sizeof(m_auStorArea2));
    m_auStorArea2[4u]  = 0x04u; /* Page SUBTYPE */
    m_auStorArea2[5u]  = 0x01u; /* Page index */
    m_auStorArea2[6u]  = 0x00u; /* Page index */
    m_auStorArea2[7u]  = 0x00u; /* Page index */
    m_auStorArea2[8u]  = 0x00u; /* Page index */
    m_auStorArea2[9u]  = 0x01u; /* Page type */
    m_auStorArea2[10u] = 0x01u; /* Page version */
    m_auStorArea2[11u] = 0x00u; /* Page version */
    m_auStorArea2[12u] = 0x02u; /* Total page */
    m_auStorArea2[13u] = 0x00u; /* Total page */
    m_auStorArea2[14u] = 0x00u; /* Total page */
    m_auStorArea2[15u] = 0x00u; /* Total page */
    m_auStorArea2[16u] = 0xA5u; /* Magic number */
    m_auStorArea2[17u] = 0xA5u; /* Magic number */
    m_auStorArea2[18u] = 0xA5u; /* Magic number */
    m_auStorArea2[19u] = 0xA5u; /* Magic number */
    m_auStorArea2[20u] = 0xA0u; /* CRC */
    m_auStorArea2[21u] = 0x02u; /* CRC */
    m_auStorArea2[22u] = 0x00u; /* CRC */
    m_auStorArea2[23u] = 0x00u; /* CRC */

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        if( ( 0x01u == l_ltUseBuff.puBuf[0u] )  && ( 0x01u == l_ltUseBuff.puBuf[1u] ) && ( 0x01u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x01u == l_ltUseBuff.puBuf[3u] )  && ( 0x03u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x01u == l_ltUseBuff2.puBuf[0u] ) && ( 0x01u == l_ltUseBuff2.puBuf[1u] ) && ( 0x01u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x01u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadBkupTest 5  -- FAIL \n");
    }

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 1u, 0u, 0x04u, 0x03u) )
    {
        if( ( 0x01u == l_ltUseBuff.puBuf[0u] )  && ( 0x01u == l_ltUseBuff.puBuf[1u] ) && ( 0x01u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x01u == l_ltUseBuff.puBuf[3u] )  && ( 0x04u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x01u == l_ltUseBuff2.puBuf[0u] ) && ( 0x01u == l_ltUseBuff2.puBuf[1u] ) && ( 0x01u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x01u == l_ltUseBuff2.puBuf[3u] ) && ( 0x03u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadBkupTest 6  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u]  = 0x03u;  /* Page SUBTYPE */
    m_auStorArea1[5u]  = 0x00u;  /* Page index */
    m_auStorArea1[6u]  = 0x00u;  /* Page index */
    m_auStorArea1[7u]  = 0x00u;  /* Page index */
    m_auStorArea1[8u]  = 0x00u;  /* Page index */
    m_auStorArea1[9u]  = 0x01u;  /* Page type */
    m_auStorArea1[10u] = 0x01u; /* Page version */
    m_auStorArea1[11u] = 0x00u; /* Page version */
    m_auStorArea1[12u] = 0x02u; /* Total page */
    m_auStorArea1[13u] = 0x00u; /* Total page */
    m_auStorArea1[14u] = 0x00u; /* Total page */
    m_auStorArea1[15u] = 0x00u; /* Total page */
    m_auStorArea1[16u] = 0xA5u; /* Magic number */
    m_auStorArea1[17u] = 0xA5u; /* Magic number */
    m_auStorArea1[18u] = 0xA5u; /* Magic number */
    m_auStorArea1[19u] = 0xA5u; /* Magic number */
    m_auStorArea1[20u] = 0x9Au; /* CRC */
    m_auStorArea1[21u] = 0x02u; /* CRC */
    m_auStorArea1[22u] = 0x00u; /* CRC */
    m_auStorArea1[23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[4u]  = 0x04u;  /* Page SUBTYPE */
    m_auStorArea2[5u]  = 0x01u;  /* Page index */
    m_auStorArea2[6u]  = 0x00u;  /* Page index */
    m_auStorArea2[7u]  = 0x00u;  /* Page index */
    m_auStorArea2[8u]  = 0x00u;  /* Page index */
    m_auStorArea2[9u]  = 0x01u;  /* Page type */
    m_auStorArea2[10u] = 0x01u; /* Page version */
    m_auStorArea2[11u] = 0x00u; /* Page version */
    m_auStorArea2[12u] = 0x02u; /* Total page */
    m_auStorArea2[13u] = 0x00u; /* Total page */
    m_auStorArea2[14u] = 0x00u; /* Total page */
    m_auStorArea2[15u] = 0x00u; /* Total page */
    m_auStorArea2[16u] = 0xA5u; /* Magic number */
    m_auStorArea2[17u] = 0xA5u; /* Magic number */
    m_auStorArea2[18u] = 0xA5u; /* Magic number */
    m_auStorArea2[19u] = 0xA5u; /* Magic number */
    m_auStorArea2[20u] = 0xDCu; /* CRC */
    m_auStorArea2[21u] = 0x02u; /* CRC */
    m_auStorArea2[22u] = 0x00u; /* CRC */
    m_auStorArea2[23u] = 0x00u; /* CRC */

    if( e_eFSS_BLOBC_RES_OK_BKP_RCVRD == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        if( ( 0x00u == l_ltUseBuff.puBuf[0u] )  && ( 0x00u == l_ltUseBuff.puBuf[1u] ) && ( 0x00u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[3u] )  && ( 0x03u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x00u == l_ltUseBuff2.puBuf[0u] ) && ( 0x00u == l_ltUseBuff2.puBuf[1u] ) && ( 0x00u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x00u == m_auStorArea1[0u] )  && ( 0x00u == m_auStorArea1[1u] ) && ( 0x00u == m_auStorArea1[2u] ) &&
            ( 0x00u == m_auStorArea1[3u] )  && ( 0x03u == m_auStorArea1[4u] ) && ( 0x00u == m_auStorArea1[5u] ) &&
            ( 0x9Au == m_auStorArea1[20u] )  && ( 0x02u == m_auStorArea1[21u] ) && ( 0x00u == m_auStorArea1[22u] ) &&
            ( 0x00u == m_auStorArea1[23u] ) &&
            ( 0x00u == m_auStorArea2[0u] )  && ( 0x00u == m_auStorArea2[1u] ) && ( 0x00u == m_auStorArea2[2u] ) &&
            ( 0x00u == m_auStorArea2[3u] )  && ( 0x04u == m_auStorArea2[4u] ) && ( 0x01u == m_auStorArea2[5u] ) &&
            ( 0x9Cu == m_auStorArea2[20u] )  && ( 0x02u == m_auStorArea2[21u] ) && ( 0x00u == m_auStorArea2[22u] ) &&
            ( 0x00u == m_auStorArea2[23u] ) )
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadBkupTest 7  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u]  = 0x03u;  /* Page SUBTYPE */
    m_auStorArea1[5u]  = 0x00u;  /* Page index */
    m_auStorArea1[6u]  = 0x00u;  /* Page index */
    m_auStorArea1[7u]  = 0x00u;  /* Page index */
    m_auStorArea1[8u]  = 0x00u;  /* Page index */
    m_auStorArea1[9u]  = 0x01u;  /* Page type */
    m_auStorArea1[10u] = 0x01u; /* Page version */
    m_auStorArea1[11u] = 0x00u; /* Page version */
    m_auStorArea1[12u] = 0x02u; /* Total page */
    m_auStorArea1[13u] = 0x00u; /* Total page */
    m_auStorArea1[14u] = 0x00u; /* Total page */
    m_auStorArea1[15u] = 0x00u; /* Total page */
    m_auStorArea1[16u] = 0xA5u; /* Magic number */
    m_auStorArea1[17u] = 0xA5u; /* Magic number */
    m_auStorArea1[18u] = 0xA5u; /* Magic number */
    m_auStorArea1[19u] = 0xA5u; /* Magic number */
    m_auStorArea1[20u] = 0x9Au; /* CRC */
    m_auStorArea1[21u] = 0x02u; /* CRC */
    m_auStorArea1[22u] = 0x00u; /* CRC */
    m_auStorArea1[23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[4u]  = 0x04u;  /* Page SUBTYPE */
    m_auStorArea2[5u]  = 0x01u;  /* Page index */
    m_auStorArea2[6u]  = 0x00u;  /* Page index */
    m_auStorArea2[7u]  = 0x00u;  /* Page index */
    m_auStorArea2[8u]  = 0x00u;  /* Page index */
    m_auStorArea2[9u]  = 0x01u;  /* Page type */
    m_auStorArea2[10u] = 0x01u; /* Page version */
    m_auStorArea2[11u] = 0x00u; /* Page version */
    m_auStorArea2[12u] = 0x02u; /* Total page */
    m_auStorArea2[13u] = 0x00u; /* Total page */
    m_auStorArea2[14u] = 0x00u; /* Total page */
    m_auStorArea2[15u] = 0x00u; /* Total page */
    m_auStorArea2[16u] = 0xA5u; /* Magic number */
    m_auStorArea2[17u] = 0xA5u; /* Magic number */
    m_auStorArea2[18u] = 0xA5u; /* Magic number */
    m_auStorArea2[19u] = 0xA5u; /* Magic number */
    m_auStorArea2[20u] = 0xDCu; /* CRC */
    m_auStorArea2[21u] = 0x02u; /* CRC */
    m_auStorArea2[22u] = 0x00u; /* CRC */
    m_auStorArea2[23u] = 0x00u; /* CRC */

    if( e_eFSS_BLOBC_RES_OK_BKP_RCVRD == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 1u, 0u, 0x04u, 0x03u) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] )  && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] )  && ( 0x04u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x03u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x10u == m_auStorArea1[0u] )  && ( 0x10u == m_auStorArea1[1u] ) && ( 0x10u == m_auStorArea1[2u] ) &&
            ( 0x10u == m_auStorArea1[3u] )  && ( 0x03u == m_auStorArea1[4u] ) && ( 0x00u == m_auStorArea1[5u] ) &&
            ( 0xDAu == m_auStorArea1[20u] )  && ( 0x02u == m_auStorArea1[21u] ) && ( 0x00u == m_auStorArea1[22u] ) &&
            ( 0x00u == m_auStorArea1[23u] ) &&
            ( 0x10u == m_auStorArea2[0u] )  && ( 0x10u == m_auStorArea2[1u] ) && ( 0x10u == m_auStorArea2[2u] ) &&
            ( 0x10u == m_auStorArea2[3u] )  && ( 0x04u == m_auStorArea2[4u] ) && ( 0x01u == m_auStorArea2[5u] ) &&
            ( 0xDCu == m_auStorArea2[20u] )  && ( 0x02u == m_auStorArea2[21u] ) && ( 0x00u == m_auStorArea2[22u] ) &&
            ( 0x00u == m_auStorArea2[23u] ) )
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadBkupTest 8  -- FAIL \n");
    }

    /* Original corrupted, backup correct */
    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u]  = 0x03u; /* Page SUBTYPE */
    m_auStorArea1[5u]  = 0x00u; /* Page index */
    m_auStorArea1[6u]  = 0x00u; /* Page index */
    m_auStorArea1[7u]  = 0x00u; /* Page index */
    m_auStorArea1[8u]  = 0x00u; /* Page index */
    m_auStorArea1[9u]  = 0x01u; /* Page type */
    m_auStorArea1[10u] = 0x01u; /* Page version */
    m_auStorArea1[11u] = 0x00u; /* Page version */
    m_auStorArea1[12u] = 0x02u; /* Total page */
    m_auStorArea1[13u] = 0x00u; /* Total page */
    m_auStorArea1[14u] = 0x00u; /* Total page */
    m_auStorArea1[15u] = 0x00u; /* Total page */
    m_auStorArea1[16u] = 0xA5u; /* Magic number */
    m_auStorArea1[17u] = 0xA5u; /* Magic number */
    m_auStorArea1[18u] = 0xA5u; /* Magic number */
    m_auStorArea1[19u] = 0xA5u; /* Magic number */
    m_auStorArea1[20u] = 0xFFu; /* CRC */ /* Correct = 0x9Au */
    m_auStorArea1[21u] = 0xFFu; /* CRC */ /* Correct = 0x02u */
    m_auStorArea1[22u] = 0xFFu; /* CRC */ /* Correct = 0x00u */
    m_auStorArea1[23u] = 0xFFu; /* CRC */ /* Correct = 0x00u */

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[4u]  = 0x04u; /* Page SUBTYPE */
    m_auStorArea2[5u]  = 0x01u; /* Page index */
    m_auStorArea2[6u]  = 0x00u; /* Page index */
    m_auStorArea2[7u]  = 0x00u; /* Page index */
    m_auStorArea2[8u]  = 0x00u; /* Page index */
    m_auStorArea2[9u]  = 0x01u; /* Page type */
    m_auStorArea2[10u] = 0x01u; /* Page version */
    m_auStorArea2[11u] = 0x00u; /* Page version */
    m_auStorArea2[12u] = 0x02u; /* Total page */
    m_auStorArea2[13u] = 0x00u; /* Total page */
    m_auStorArea2[14u] = 0x00u; /* Total page */
    m_auStorArea2[15u] = 0x00u; /* Total page */
    m_auStorArea2[16u] = 0xA5u; /* Magic number */
    m_auStorArea2[17u] = 0xA5u; /* Magic number */
    m_auStorArea2[18u] = 0xA5u; /* Magic number */
    m_auStorArea2[19u] = 0xA5u; /* Magic number */
    m_auStorArea2[20u] = 0xDCu; /* CRC */
    m_auStorArea2[21u] = 0x02u; /* CRC */
    m_auStorArea2[22u] = 0x00u; /* CRC */
    m_auStorArea2[23u] = 0x00u; /* CRC */

    if( e_eFSS_BLOBC_RES_OK_BKP_RCVRD == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] )  && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] )  && ( 0x03u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x10u == m_auStorArea1[0u] )  && ( 0x10u == m_auStorArea1[1u] ) && ( 0x10u == m_auStorArea1[2u] ) &&
            ( 0x10u == m_auStorArea1[3u] )  && ( 0x03u == m_auStorArea1[4u] ) && ( 0x00u == m_auStorArea1[5u] ) &&
            ( 0xDAu == m_auStorArea1[20u] )  && ( 0x02u == m_auStorArea1[21u] ) && ( 0x00u == m_auStorArea1[22u] ) &&
            ( 0x00u == m_auStorArea1[23u] ) &&
            ( 0x10u == m_auStorArea2[0u] )  && ( 0x10u == m_auStorArea2[1u] ) && ( 0x10u == m_auStorArea2[2u] ) &&
            ( 0x10u == m_auStorArea2[3u] )  && ( 0x04u == m_auStorArea2[4u] ) && ( 0x01u == m_auStorArea2[5u] ) &&
            ( 0xDCu == m_auStorArea2[20u] )  && ( 0x02u == m_auStorArea2[21u] ) && ( 0x00u == m_auStorArea2[22u] ) &&
            ( 0x00u == m_auStorArea2[23u] ) )
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadBkupTest 9  -- FAIL \n");
    }

    /* Original corrupted, backup correct */
    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u]  = 0x03u; /* Page SUBTYPE */
    m_auStorArea1[5u]  = 0x00u; /* Page index */
    m_auStorArea1[6u]  = 0x00u; /* Page index */
    m_auStorArea1[7u]  = 0x00u; /* Page index */
    m_auStorArea1[8u]  = 0x00u; /* Page index */
    m_auStorArea1[9u]  = 0x01u; /* Page type */
    m_auStorArea1[10u] = 0x01u; /* Page version */
    m_auStorArea1[11u] = 0x00u; /* Page version */
    m_auStorArea1[12u] = 0x02u; /* Total page */
    m_auStorArea1[13u] = 0x00u; /* Total page */
    m_auStorArea1[14u] = 0x00u; /* Total page */
    m_auStorArea1[15u] = 0x00u; /* Total page */
    m_auStorArea1[16u] = 0xA5u; /* Magic number */
    m_auStorArea1[17u] = 0xA5u; /* Magic number */
    m_auStorArea1[18u] = 0xA5u; /* Magic number */
    m_auStorArea1[19u] = 0xA5u; /* Magic number */
    m_auStorArea1[20u] = 0x9Au; /* CRC */
    m_auStorArea1[21u] = 0x02u; /* CRC */
    m_auStorArea1[22u] = 0x00u; /* CRC */
    m_auStorArea1[23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[4u]  = 0x04u; /* Page SUBTYPE */
    m_auStorArea2[5u]  = 0x01u; /* Page index */
    m_auStorArea2[6u]  = 0x00u; /* Page index */
    m_auStorArea2[7u]  = 0x00u; /* Page index */
    m_auStorArea2[8u]  = 0x00u; /* Page index */
    m_auStorArea2[9u]  = 0x01u; /* Page type */
    m_auStorArea2[10u] = 0x01u; /* Page version */
    m_auStorArea2[11u] = 0x00u; /* Page version */
    m_auStorArea2[12u] = 0x02u; /* Total page */
    m_auStorArea2[13u] = 0x00u; /* Total page */
    m_auStorArea2[14u] = 0x00u; /* Total page */
    m_auStorArea2[15u] = 0x00u; /* Total page */
    m_auStorArea2[16u] = 0xA5u; /* Magic number */
    m_auStorArea2[17u] = 0xA5u; /* Magic number */
    m_auStorArea2[18u] = 0xA5u; /* Magic number */
    m_auStorArea2[19u] = 0xA5u; /* Magic number */
    m_auStorArea2[20u] = 0xDCu; /* CRC */
    m_auStorArea2[21u] = 0x02u; /* CRC */
    m_auStorArea2[22u] = 0x00u; /* CRC */
    m_auStorArea2[23u] = 0x00u; /* CRC */

    if( e_eFSS_BLOBC_RES_OK_BKP_RCVRD == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 1u, 0u, 0xFFu, 0x03u) )
    {
        if( ( 0x00u == l_ltUseBuff.puBuf[0u] )  && ( 0x00u == l_ltUseBuff.puBuf[1u] ) && ( 0x00u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[3u] )  && ( 0xFFu == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x00u == l_ltUseBuff2.puBuf[0u] ) && ( 0x00u == l_ltUseBuff2.puBuf[1u] ) && ( 0x00u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[3u] ) && ( 0x03u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x00u == m_auStorArea1[0u] )  && ( 0x00u == m_auStorArea1[1u] ) && ( 0x00u == m_auStorArea1[2u] ) &&
            ( 0x00u == m_auStorArea1[3u] )  && ( 0x03u == m_auStorArea1[4u] ) && ( 0x00u == m_auStorArea1[5u] ) &&
            ( 0x9Au == m_auStorArea1[20u] )  && ( 0x02u == m_auStorArea1[21u] ) && ( 0x00u == m_auStorArea1[22u] ) &&
            ( 0x00u == m_auStorArea1[23u] )  &&
            ( 0x00u == m_auStorArea2[0u] )   && ( 0x00u == m_auStorArea2[1u] ) && ( 0x00u == m_auStorArea2[2u] ) &&
            ( 0x00u == m_auStorArea2[3u] )   && ( 0xFFu == m_auStorArea2[4u] ) && ( 0x01u == m_auStorArea2[5u] ) &&
            ( 0x97u == m_auStorArea2[20u] )  && ( 0x03u == m_auStorArea2[21u] ) && ( 0x00u == m_auStorArea2[22u] ) &&
            ( 0x00u == m_auStorArea2[23u] ) )
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadBkupTest 10 -- FAIL \n");
    }

    /* Backup corrupted, original correct */
    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u]  = 0x03u; /* Page SUBTYPE */
    m_auStorArea1[5u]  = 0x00u; /* Page index */
    m_auStorArea1[6u]  = 0x00u; /* Page index */
    m_auStorArea1[7u]  = 0x00u; /* Page index */
    m_auStorArea1[8u]  = 0x00u; /* Page index */
    m_auStorArea1[9u]  = 0x01u; /* Page type */
    m_auStorArea1[10u] = 0x01u; /* Page version */
    m_auStorArea1[11u] = 0x00u; /* Page version */
    m_auStorArea1[12u] = 0x02u; /* Total page */
    m_auStorArea1[13u] = 0x00u; /* Total page */
    m_auStorArea1[14u] = 0x00u; /* Total page */
    m_auStorArea1[15u] = 0x00u; /* Total page */
    m_auStorArea1[16u] = 0xA5u; /* Magic number */
    m_auStorArea1[17u] = 0xA5u; /* Magic number */
    m_auStorArea1[18u] = 0xA5u; /* Magic number */
    m_auStorArea1[19u] = 0xA5u; /* Magic number */
    m_auStorArea1[20u] = 0xFFu; /* CRC */
    m_auStorArea1[21u] = 0xFFu; /* CRC */
    m_auStorArea1[22u] = 0xFFu; /* CRC */
    m_auStorArea1[23u] = 0xFFu; /* CRC */

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[4u]  = 0x04u; /* Page SUBTYPE */
    m_auStorArea2[5u]  = 0x01u; /* Page index */
    m_auStorArea2[6u]  = 0x00u; /* Page index */
    m_auStorArea2[7u]  = 0x00u; /* Page index */
    m_auStorArea2[8u]  = 0x00u; /* Page index */
    m_auStorArea2[9u]  = 0x01u; /* Page type */
    m_auStorArea2[10u] = 0x01u; /* Page version */
    m_auStorArea2[11u] = 0x00u; /* Page version */
    m_auStorArea2[12u] = 0x02u; /* Total page */
    m_auStorArea2[13u] = 0x00u; /* Total page */
    m_auStorArea2[14u] = 0x00u; /* Total page */
    m_auStorArea2[15u] = 0x00u; /* Total page */
    m_auStorArea2[16u] = 0xA5u; /* Magic number */
    m_auStorArea2[17u] = 0xA5u; /* Magic number */
    m_auStorArea2[18u] = 0xA5u; /* Magic number */
    m_auStorArea2[19u] = 0xA5u; /* Magic number */
    m_auStorArea2[20u] = 0xDCu; /* CRC */
    m_auStorArea2[21u] = 0x02u; /* CRC */
    m_auStorArea2[22u] = 0x00u; /* CRC */
    m_auStorArea2[23u] = 0x00u; /* CRC */

    if( e_eFSS_BLOBC_RES_OK_BKP_RCVRD == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 1u, 0u, 0x04u, 0x03u) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] )  && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] )  && ( 0x04u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x03u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x10u == m_auStorArea1[0u] )  && ( 0x10u == m_auStorArea1[1u] ) && ( 0x10u == m_auStorArea1[2u] ) &&
            ( 0x10u == m_auStorArea1[3u] )  && ( 0x03u == m_auStorArea1[4u] ) && ( 0x00u == m_auStorArea1[5u] ) &&
            ( 0xDAu == m_auStorArea1[20u] )  && ( 0x02u == m_auStorArea1[21u] ) && ( 0x00u == m_auStorArea1[22u] ) &&
            ( 0x00u == m_auStorArea1[23u] ) &&
            ( 0x10u == m_auStorArea2[0u] )  && ( 0x10u == m_auStorArea2[1u] ) && ( 0x10u == m_auStorArea2[2u] ) &&
            ( 0x10u == m_auStorArea2[3u] )  && ( 0x04u == m_auStorArea2[4u] ) && ( 0x01u == m_auStorArea2[5u] ) &&
            ( 0xDCu == m_auStorArea2[20u] )  && ( 0x02u == m_auStorArea2[21u] ) && ( 0x00u == m_auStorArea2[22u] ) &&
            ( 0x00u == m_auStorArea2[23u] ) )
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadBkupTest 11 -- FAIL \n");
    }

    /* Original corrupted, backup correct */
    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u]  = 0x03u; /* Page SUBTYPE */
    m_auStorArea1[5u]  = 0x00u; /* Page index */
    m_auStorArea1[6u]  = 0x00u; /* Page index */
    m_auStorArea1[7u]  = 0x00u; /* Page index */
    m_auStorArea1[8u]  = 0x00u; /* Page index */
    m_auStorArea1[9u]  = 0x01u; /* Page type */
    m_auStorArea1[10u] = 0x01u; /* Page version */
    m_auStorArea1[11u] = 0x00u; /* Page version */
    m_auStorArea1[12u] = 0x02u; /* Total page */
    m_auStorArea1[13u] = 0x00u; /* Total page */
    m_auStorArea1[14u] = 0x00u; /* Total page */
    m_auStorArea1[15u] = 0x00u; /* Total page */
    m_auStorArea1[16u] = 0xA5u; /* Magic number */
    m_auStorArea1[17u] = 0xA5u; /* Magic number */
    m_auStorArea1[18u] = 0xA5u; /* Magic number */
    m_auStorArea1[19u] = 0xA5u; /* Magic number */
    m_auStorArea1[20u] = 0x9Au; /* CRC */
    m_auStorArea1[21u] = 0x02u; /* CRC */
    m_auStorArea1[22u] = 0x00u; /* CRC */
    m_auStorArea1[23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[4u]  = 0x04u; /* Page SUBTYPE */
    m_auStorArea2[5u]  = 0x01u; /* Page index */
    m_auStorArea2[6u]  = 0x00u; /* Page index */
    m_auStorArea2[7u]  = 0x00u; /* Page index */
    m_auStorArea2[8u]  = 0x00u; /* Page index */
    m_auStorArea2[9u]  = 0x01u; /* Page type */
    m_auStorArea2[10u] = 0x01u; /* Page version */
    m_auStorArea2[11u] = 0x00u; /* Page version */
    m_auStorArea2[12u] = 0x02u; /* Total page */
    m_auStorArea2[13u] = 0x00u; /* Total page */
    m_auStorArea2[14u] = 0x00u; /* Total page */
    m_auStorArea2[15u] = 0x00u; /* Total page */
    m_auStorArea2[16u] = 0xA5u; /* Magic number */
    m_auStorArea2[17u] = 0xA5u; /* Magic number */
    m_auStorArea2[18u] = 0xA5u; /* Magic number */
    m_auStorArea2[19u] = 0xA5u; /* Magic number */
    m_auStorArea2[20u] = 0xDCu; /* CRC */
    m_auStorArea2[21u] = 0x02u; /* CRC */
    m_auStorArea2[22u] = 0x00u; /* CRC */
    m_auStorArea2[23u] = 0x00u; /* CRC */

    if( e_eFSS_BLOBC_RES_OK_BKP_RCVRD == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0xFFu) )
    {
        if( ( 0x00u == l_ltUseBuff.puBuf[0u] )  && ( 0x00u == l_ltUseBuff.puBuf[1u] ) && ( 0x00u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[3u] )  && ( 0x03u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x00u == l_ltUseBuff2.puBuf[0u] ) && ( 0x00u == l_ltUseBuff2.puBuf[1u] ) && ( 0x00u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[3u] ) && ( 0xFFu == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x00u == m_auStorArea1[0u] )  && ( 0x00u == m_auStorArea1[1u] ) && ( 0x00u == m_auStorArea1[2u] ) &&
            ( 0x00u == m_auStorArea1[3u] )  && ( 0x03u == m_auStorArea1[4u] ) && ( 0x00u == m_auStorArea1[5u] ) &&
            ( 0x9Au == m_auStorArea1[20u] )  && ( 0x02u == m_auStorArea1[21u] ) && ( 0x00u == m_auStorArea1[22u] ) &&
            ( 0x00u == m_auStorArea1[23u] )  &&
            ( 0x00u == m_auStorArea2[0u] )   && ( 0x00u == m_auStorArea2[1u] ) && ( 0x00u == m_auStorArea2[2u] ) &&
            ( 0x00u == m_auStorArea2[3u] )   && ( 0xFFu == m_auStorArea2[4u] ) && ( 0x01u == m_auStorArea2[5u] ) &&
            ( 0x97u == m_auStorArea2[20u] )  && ( 0x03u == m_auStorArea2[21u] ) && ( 0x00u == m_auStorArea2[22u] ) &&
            ( 0x00u == m_auStorArea2[23u] ) )
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadBkupTest 12 -- FAIL \n");
    }

    /* Nothing OK, but new  */
    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u]  = 0x03u; /* Page SUBTYPE */
    m_auStorArea1[5u]  = 0x00u; /* Page index */
    m_auStorArea1[6u]  = 0x00u; /* Page index */
    m_auStorArea1[7u]  = 0x00u; /* Page index */
    m_auStorArea1[8u]  = 0x00u; /* Page index */
    m_auStorArea1[9u]  = 0x01u; /* Page type */
    m_auStorArea1[10u] = 0x02u; /* Page version */
    m_auStorArea1[11u] = 0x00u; /* Page version */
    m_auStorArea1[12u] = 0x02u; /* Total page */
    m_auStorArea1[13u] = 0x00u; /* Total page */
    m_auStorArea1[14u] = 0x00u; /* Total page */
    m_auStorArea1[15u] = 0x00u; /* Total page */
    m_auStorArea1[16u] = 0xA5u; /* Magic number */
    m_auStorArea1[17u] = 0xA5u; /* Magic number */
    m_auStorArea1[18u] = 0xA5u; /* Magic number */
    m_auStorArea1[19u] = 0xA5u; /* Magic number */
    m_auStorArea1[20u] = 0x9Bu; /* CRC */
    m_auStorArea1[21u] = 0x02u; /* CRC */
    m_auStorArea1[22u] = 0x00u; /* CRC */
    m_auStorArea1[23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[4u]  = 0x04u; /* Page SUBTYPE */
    m_auStorArea2[5u]  = 0x01u; /* Page index */
    m_auStorArea2[6u]  = 0x00u; /* Page index */
    m_auStorArea2[7u]  = 0x00u; /* Page index */
    m_auStorArea2[8u]  = 0x00u; /* Page index */
    m_auStorArea2[9u]  = 0x01u; /* Page type */
    m_auStorArea2[10u] = 0x02u; /* Page version */
    m_auStorArea2[11u] = 0x00u; /* Page version */
    m_auStorArea2[12u] = 0x02u; /* Total page */
    m_auStorArea2[13u] = 0x00u; /* Total page */
    m_auStorArea2[14u] = 0x00u; /* Total page */
    m_auStorArea2[15u] = 0x00u; /* Total page */
    m_auStorArea2[16u] = 0xA5u; /* Magic number */
    m_auStorArea2[17u] = 0xA5u; /* Magic number */
    m_auStorArea2[18u] = 0xA5u; /* Magic number */
    m_auStorArea2[19u] = 0xA5u; /* Magic number */
    m_auStorArea2[20u] = 0xDDu; /* CRC */
    m_auStorArea2[21u] = 0x02u; /* CRC */
    m_auStorArea2[22u] = 0x00u; /* CRC */
    m_auStorArea2[23u] = 0x00u; /* CRC */

    if( e_eFSS_BLOBC_RES_NEWVERSIONFOUND == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        if( ( 0x00u == l_ltUseBuff.puBuf[0u] )  && ( 0x00u == l_ltUseBuff.puBuf[1u] ) && ( 0x00u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[3u] )  && ( 0x03u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x00u == m_auStorArea1[0u] )  && ( 0x00u == m_auStorArea1[1u] ) && ( 0x00u == m_auStorArea1[2u] ) &&
            ( 0x00u == m_auStorArea1[3u] )  && ( 0x03u == m_auStorArea1[4u] ) && ( 0x00u == m_auStorArea1[5u] ) &&
            ( 0x9Bu == m_auStorArea1[20u] )  && ( 0x02u == m_auStorArea1[21u] ) && ( 0x00u == m_auStorArea1[22u] ) &&
            ( 0x00u == m_auStorArea1[23u] )  &&
            ( 0x10u == m_auStorArea2[0u] )   && ( 0x10u == m_auStorArea2[1u] ) && ( 0x10u == m_auStorArea2[2u] ) &&
            ( 0x10u == m_auStorArea2[3u] )   && ( 0x04u == m_auStorArea2[4u] ) && ( 0x01u == m_auStorArea2[5u] ) &&
            ( 0xDDu == m_auStorArea2[20u] )  && ( 0x02u == m_auStorArea2[21u] ) && ( 0x00u == m_auStorArea2[22u] ) &&
            ( 0x00u == m_auStorArea2[23u] ) )
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadBkupTest 13 -- FAIL \n");
    }

    /* Original corrupted, backup correct */
    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u]  = 0x03u; /* Page SUBTYPE */
    m_auStorArea1[5u]  = 0x00u; /* Page index */
    m_auStorArea1[6u]  = 0x00u; /* Page index */
    m_auStorArea1[7u]  = 0x00u; /* Page index */
    m_auStorArea1[8u]  = 0x00u; /* Page index */
    m_auStorArea1[9u]  = 0x01u; /* Page type */
    m_auStorArea1[10u] = 0x02u; /* Page version */
    m_auStorArea1[11u] = 0x00u; /* Page version */
    m_auStorArea1[12u] = 0x02u; /* Total page */
    m_auStorArea1[13u] = 0x00u; /* Total page */
    m_auStorArea1[14u] = 0x00u; /* Total page */
    m_auStorArea1[15u] = 0x00u; /* Total page */
    m_auStorArea1[16u] = 0xA5u; /* Magic number */
    m_auStorArea1[17u] = 0xA5u; /* Magic number */
    m_auStorArea1[18u] = 0xA5u; /* Magic number */
    m_auStorArea1[19u] = 0xA5u; /* Magic number */
    m_auStorArea1[20u] = 0x9Bu; /* CRC */
    m_auStorArea1[21u] = 0x02u; /* CRC */
    m_auStorArea1[22u] = 0x00u; /* CRC */
    m_auStorArea1[23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[4u]  = 0x04u; /* Page SUBTYPE */
    m_auStorArea2[5u]  = 0x01u; /* Page index */
    m_auStorArea2[6u]  = 0x00u; /* Page index */
    m_auStorArea2[7u]  = 0x00u; /* Page index */
    m_auStorArea2[8u]  = 0x00u; /* Page index */
    m_auStorArea2[9u]  = 0x01u; /* Page type */
    m_auStorArea2[10u] = 0x01u; /* Page version */
    m_auStorArea2[11u] = 0x00u; /* Page version */
    m_auStorArea2[12u] = 0x02u; /* Total page */
    m_auStorArea2[13u] = 0x00u; /* Total page */
    m_auStorArea2[14u] = 0x00u; /* Total page */
    m_auStorArea2[15u] = 0x00u; /* Total page */
    m_auStorArea2[16u] = 0xA5u; /* Magic number */
    m_auStorArea2[17u] = 0xA5u; /* Magic number */
    m_auStorArea2[18u] = 0xA5u; /* Magic number */
    m_auStorArea2[19u] = 0xA5u; /* Magic number */
    m_auStorArea2[20u] = 0xDCu; /* CRC */
    m_auStorArea2[21u] = 0x02u; /* CRC */
    m_auStorArea2[22u] = 0x00u; /* CRC */
    m_auStorArea2[23u] = 0x00u; /* CRC */

    if( e_eFSS_BLOBC_RES_OK_BKP_RCVRD == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] )  && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] )  && ( 0x03u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x10u == m_auStorArea1[0u] )  && ( 0x10u == m_auStorArea1[1u] ) && ( 0x10u == m_auStorArea1[2u] ) &&
            ( 0x10u == m_auStorArea1[3u] )  && ( 0x03u == m_auStorArea1[4u] ) && ( 0x00u == m_auStorArea1[5u] ) &&
            ( 0xDAu == m_auStorArea1[20u] )  && ( 0x02u == m_auStorArea1[21u] ) && ( 0x00u == m_auStorArea1[22u] ) &&
            ( 0x00u == m_auStorArea1[23u] ) &&
            ( 0x10u == m_auStorArea2[0u] )  && ( 0x10u == m_auStorArea2[1u] ) && ( 0x10u == m_auStorArea2[2u] ) &&
            ( 0x10u == m_auStorArea2[3u] )  && ( 0x04u == m_auStorArea2[4u] ) && ( 0x01u == m_auStorArea2[5u] ) &&
            ( 0xDCu == m_auStorArea2[20u] )  && ( 0x02u == m_auStorArea2[21u] ) && ( 0x00u == m_auStorArea2[22u] ) &&
            ( 0x00u == m_auStorArea2[23u] ) )
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadBkupTest 14 -- FAIL \n");
    }

    /* Backup corrupted, original correct */
    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u]  = 0x03u; /* Page SUBTYPE */
    m_auStorArea1[5u]  = 0x00u; /* Page index */
    m_auStorArea1[6u]  = 0x00u; /* Page index */
    m_auStorArea1[7u]  = 0x00u; /* Page index */
    m_auStorArea1[8u]  = 0x00u; /* Page index */
    m_auStorArea1[9u]  = 0x01u; /* Page type */
    m_auStorArea1[10u] = 0x02u; /* Page version */
    m_auStorArea1[11u] = 0x00u; /* Page version */
    m_auStorArea1[12u] = 0x02u; /* Total page */
    m_auStorArea1[13u] = 0x00u; /* Total page */
    m_auStorArea1[14u] = 0x00u; /* Total page */
    m_auStorArea1[15u] = 0x00u; /* Total page */
    m_auStorArea1[16u] = 0xA5u; /* Magic number */
    m_auStorArea1[17u] = 0xA5u; /* Magic number */
    m_auStorArea1[18u] = 0xA5u; /* Magic number */
    m_auStorArea1[19u] = 0xA5u; /* Magic number */
    m_auStorArea1[20u] = 0x9Bu; /* CRC */
    m_auStorArea1[21u] = 0x02u; /* CRC */
    m_auStorArea1[22u] = 0x00u; /* CRC */
    m_auStorArea1[23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[4u]  = 0x04u; /* Page SUBTYPE */
    m_auStorArea2[5u]  = 0x01u; /* Page index */
    m_auStorArea2[6u]  = 0x00u; /* Page index */
    m_auStorArea2[7u]  = 0x00u; /* Page index */
    m_auStorArea2[8u]  = 0x00u; /* Page index */
    m_auStorArea2[9u]  = 0x01u; /* Page type */
    m_auStorArea2[10u] = 0x01u; /* Page version */
    m_auStorArea2[11u] = 0x00u; /* Page version */
    m_auStorArea2[12u] = 0x02u; /* Total page */
    m_auStorArea2[13u] = 0x00u; /* Total page */
    m_auStorArea2[14u] = 0x00u; /* Total page */
    m_auStorArea2[15u] = 0x00u; /* Total page */
    m_auStorArea2[16u] = 0xA5u; /* Magic number */
    m_auStorArea2[17u] = 0xA5u; /* Magic number */
    m_auStorArea2[18u] = 0xA5u; /* Magic number */
    m_auStorArea2[19u] = 0xA5u; /* Magic number */
    m_auStorArea2[20u] = 0xDCu; /* CRC */
    m_auStorArea2[21u] = 0x02u; /* CRC */
    m_auStorArea2[22u] = 0x00u; /* CRC */
    m_auStorArea2[23u] = 0x00u; /* CRC */

    if( e_eFSS_BLOBC_RES_OK_BKP_RCVRD == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 1u, 0u, 0x04u, 0x03u) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] )  && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] )  && ( 0x04u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x03u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x10u == m_auStorArea1[0u] )  && ( 0x10u == m_auStorArea1[1u] ) && ( 0x10u == m_auStorArea1[2u] ) &&
            ( 0x10u == m_auStorArea1[3u] )  && ( 0x03u == m_auStorArea1[4u] ) && ( 0x00u == m_auStorArea1[5u] ) &&
            ( 0xDAu == m_auStorArea1[20u] )  && ( 0x02u == m_auStorArea1[21u] ) && ( 0x00u == m_auStorArea1[22u] ) &&
            ( 0x00u == m_auStorArea1[23u] ) &&
            ( 0x10u == m_auStorArea2[0u] )  && ( 0x10u == m_auStorArea2[1u] ) && ( 0x10u == m_auStorArea2[2u] ) &&
            ( 0x10u == m_auStorArea2[3u] )  && ( 0x04u == m_auStorArea2[4u] ) && ( 0x01u == m_auStorArea2[5u] ) &&
            ( 0xDCu == m_auStorArea2[20u] )  && ( 0x02u == m_auStorArea2[21u] ) && ( 0x00u == m_auStorArea2[22u] ) &&
            ( 0x00u == m_auStorArea2[23u] ) )
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadBkupTest 14 -- FAIL \n");
    }

    /* Nothing OK, but new  */
    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u]  = 0x03u; /* Page SUBTYPE */
    m_auStorArea1[5u]  = 0x00u; /* Page index */
    m_auStorArea1[6u]  = 0x00u; /* Page index */
    m_auStorArea1[7u]  = 0x00u; /* Page index */
    m_auStorArea1[8u]  = 0x00u; /* Page index */
    m_auStorArea1[9u]  = 0x01u; /* Page type */
    m_auStorArea1[10u] = 0x02u; /* Page version */
    m_auStorArea1[11u] = 0x00u; /* Page version */
    m_auStorArea1[12u] = 0x02u; /* Total page */
    m_auStorArea1[13u] = 0x00u; /* Total page */
    m_auStorArea1[14u] = 0x00u; /* Total page */
    m_auStorArea1[15u] = 0x00u; /* Total page */
    m_auStorArea1[16u] = 0xA5u; /* Magic number */
    m_auStorArea1[17u] = 0xA5u; /* Magic number */
    m_auStorArea1[18u] = 0xA5u; /* Magic number */
    m_auStorArea1[19u] = 0xA5u; /* Magic number */
    m_auStorArea1[20u] = 0x9Bu; /* CRC */
    m_auStorArea1[21u] = 0x02u; /* CRC */
    m_auStorArea1[22u] = 0x00u; /* CRC */
    m_auStorArea1[23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[4u]  = 0x04u; /* Page SUBTYPE */
    m_auStorArea2[5u]  = 0x01u; /* Page index */
    m_auStorArea2[6u]  = 0x00u; /* Page index */
    m_auStorArea2[7u]  = 0x00u; /* Page index */
    m_auStorArea2[8u]  = 0x00u; /* Page index */
    m_auStorArea2[9u]  = 0x01u; /* Page type */
    m_auStorArea2[10u] = 0x02u; /* Page version */
    m_auStorArea2[11u] = 0x00u; /* Page version */
    m_auStorArea2[12u] = 0x02u; /* Total page */
    m_auStorArea2[13u] = 0x00u; /* Total page */
    m_auStorArea2[14u] = 0x00u; /* Total page */
    m_auStorArea2[15u] = 0x00u; /* Total page */
    m_auStorArea2[16u] = 0xA5u; /* Magic number */
    m_auStorArea2[17u] = 0xA5u; /* Magic number */
    m_auStorArea2[18u] = 0xA5u; /* Magic number */
    m_auStorArea2[19u] = 0xA5u; /* Magic number */
    m_auStorArea2[20u] = 0xFFu; /* CRC */
    m_auStorArea2[21u] = 0xFFu; /* CRC */
    m_auStorArea2[22u] = 0xFFu; /* CRC */
    m_auStorArea2[23u] = 0xFFu; /* CRC */

    if( e_eFSS_BLOBC_RES_NEWVERSIONFOUND == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        if( ( 0x00u == l_ltUseBuff.puBuf[0u] )  && ( 0x00u == l_ltUseBuff.puBuf[1u] ) && ( 0x00u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[3u] )  && ( 0x03u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x00u == m_auStorArea1[0u] )  && ( 0x00u == m_auStorArea1[1u] ) && ( 0x00u == m_auStorArea1[2u] ) &&
            ( 0x00u == m_auStorArea1[3u] )  && ( 0x03u == m_auStorArea1[4u] ) && ( 0x00u == m_auStorArea1[5u] ) &&
            ( 0x9Bu == m_auStorArea1[20u] )  && ( 0x02u == m_auStorArea1[21u] ) && ( 0x00u == m_auStorArea1[22u] ) &&
            ( 0x00u == m_auStorArea1[23u] )  &&
            ( 0x10u == m_auStorArea2[0u] )   && ( 0x10u == m_auStorArea2[1u] ) && ( 0x10u == m_auStorArea2[2u] ) &&
            ( 0x10u == m_auStorArea2[3u] )   && ( 0x04u == m_auStorArea2[4u] ) && ( 0x01u == m_auStorArea2[5u] ) &&
            ( 0xFFu == m_auStorArea2[20u] )  && ( 0xFFu == m_auStorArea2[21u] ) && ( 0xFFu == m_auStorArea2[22u] ) &&
            ( 0xFFu == m_auStorArea2[23u] ) )
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadBkupTest 15 -- FAIL \n");
    }

    /* Nothing OK, but new  */
    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u]  = 0x03u; /* Page SUBTYPE */
    m_auStorArea1[5u]  = 0x00u; /* Page index */
    m_auStorArea1[6u]  = 0x00u; /* Page index */
    m_auStorArea1[7u]  = 0x00u; /* Page index */
    m_auStorArea1[8u]  = 0x00u; /* Page index */
    m_auStorArea1[9u]  = 0x01u; /* Page type */
    m_auStorArea1[10u] = 0x02u; /* Page version */
    m_auStorArea1[11u] = 0x00u; /* Page version */
    m_auStorArea1[12u] = 0x02u; /* Total page */
    m_auStorArea1[13u] = 0x00u; /* Total page */
    m_auStorArea1[14u] = 0x00u; /* Total page */
    m_auStorArea1[15u] = 0x00u; /* Total page */
    m_auStorArea1[16u] = 0xA5u; /* Magic number */
    m_auStorArea1[17u] = 0xA5u; /* Magic number */
    m_auStorArea1[18u] = 0xA5u; /* Magic number */
    m_auStorArea1[19u] = 0xA5u; /* Magic number */
    m_auStorArea1[20u] = 0xFFu; /* CRC */
    m_auStorArea1[21u] = 0xFFu; /* CRC */
    m_auStorArea1[22u] = 0xFFu; /* CRC */
    m_auStorArea1[23u] = 0xFFu; /* CRC */

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[4u]  = 0x04u; /* Page SUBTYPE */
    m_auStorArea2[5u]  = 0x01u; /* Page index */
    m_auStorArea2[6u]  = 0x00u; /* Page index */
    m_auStorArea2[7u]  = 0x00u; /* Page index */
    m_auStorArea2[8u]  = 0x00u; /* Page index */
    m_auStorArea2[9u]  = 0x01u; /* Page type */
    m_auStorArea2[10u] = 0x02u; /* Page version */
    m_auStorArea2[11u] = 0x00u; /* Page version */
    m_auStorArea2[12u] = 0x02u; /* Total page */
    m_auStorArea2[13u] = 0x00u; /* Total page */
    m_auStorArea2[14u] = 0x00u; /* Total page */
    m_auStorArea2[15u] = 0x00u; /* Total page */
    m_auStorArea2[16u] = 0xA5u; /* Magic number */
    m_auStorArea2[17u] = 0xA5u; /* Magic number */
    m_auStorArea2[18u] = 0xA5u; /* Magic number */
    m_auStorArea2[19u] = 0xA5u; /* Magic number */
    m_auStorArea2[20u] = 0xFFu; /* CRC */
    m_auStorArea2[21u] = 0xFFu; /* CRC */
    m_auStorArea2[22u] = 0xFFu; /* CRC */
    m_auStorArea2[23u] = 0xFFu; /* CRC */

    if( e_eFSS_BLOBC_RES_NOTVALIDPAGE == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        if( ( 0x00u == l_ltUseBuff.puBuf[0u] )  && ( 0x00u == l_ltUseBuff.puBuf[1u] ) && ( 0x00u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[3u] )  && ( 0x03u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x00u == m_auStorArea1[0u] )  && ( 0x00u == m_auStorArea1[1u] ) && ( 0x00u == m_auStorArea1[2u] ) &&
            ( 0x00u == m_auStorArea1[3u] )  && ( 0x03u == m_auStorArea1[4u] ) && ( 0x00u == m_auStorArea1[5u] ) &&
            ( 0xFFu == m_auStorArea1[20u] )  && ( 0xFFu == m_auStorArea1[21u] ) && ( 0xFFu == m_auStorArea1[22u] ) &&
            ( 0xFFu == m_auStorArea1[23u] )  &&
            ( 0x10u == m_auStorArea2[0u] )   && ( 0x10u == m_auStorArea2[1u] ) && ( 0x10u == m_auStorArea2[2u] ) &&
            ( 0x10u == m_auStorArea2[3u] )   && ( 0x04u == m_auStorArea2[4u] ) && ( 0x01u == m_auStorArea2[5u] ) &&
            ( 0xFFu == m_auStorArea2[20u] )  && ( 0xFFu == m_auStorArea2[21u] ) && ( 0xFFu == m_auStorArea2[22u] ) &&
            ( 0xFFu == m_auStorArea2[23u] ) )
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 16 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_LoadBkupTest 16 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_LoadBkupTest 16 -- FAIL \n");
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

static void eFSS_BLOBCTST_FlushBkupTest(void)
{
    /* Local variable */
    t_eFSS_BLOBC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_BLOBC_StorBuf l_ltUseBuff;
    t_eFSS_BLOBC_StorBuf l_ltUseBuff2;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcTst1Adapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 2u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_FlushBkupTest 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_FlushBkupTest 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_FlushBkupTest 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_FlushBkupTest 2  -- FAIL \n");
    }

    /* Setup storage area */
    l_ltUseBuff2.uBufL = l_ltUseBuff.uBufL  + 1u;
    l_ltUseBuff2.puBuf = &l_auStor[24];


    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0, sizeof(m_auStorArea2));
    (void)memset(l_ltUseBuff.puBuf, 0x10, l_ltUseBuff.uBufL);


    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_FlushBuffInPageNBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] )  && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] )  && ( 0x03u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x04u == l_ltUseBuff2.puBuf[4u] ) )
        {
            if( ( m_auStorArea1[0u]  == 0x10u ) && ( m_auStorArea1[1u]  == 0x10u ) && ( m_auStorArea1[2u]  == 0x10u ) &&
                ( m_auStorArea1[3u]  == 0x10u ) && ( m_auStorArea1[4u]  == 0x03u ) && ( m_auStorArea1[5u]  == 0x00u ) &&
                ( m_auStorArea1[6u]  == 0x00u ) && ( m_auStorArea1[7u]  == 0x00u ) && ( m_auStorArea1[8u]  == 0x00u ) &&
                ( m_auStorArea1[9u]  == 0x01u ) && ( m_auStorArea1[10u] == 0x01u ) && ( m_auStorArea1[11u] == 0x00u ) &&
                ( m_auStorArea1[12u] == 0x02u ) && ( m_auStorArea1[13u] == 0x00u ) && ( m_auStorArea1[14u] == 0x00u ) &&
                ( m_auStorArea1[15u] == 0x00u ) && ( m_auStorArea1[16u] == 0xA5u ) && ( m_auStorArea1[17u] == 0xA5u ) &&
                ( m_auStorArea1[18u] == 0xA5u ) && ( m_auStorArea1[19u] == 0xA5u ) && ( m_auStorArea1[20u] == 0xDAu ) &&
                ( m_auStorArea1[21u] == 0x02u ) && ( m_auStorArea1[22u] == 0x00u ) && ( m_auStorArea1[23u] == 0x00u ) &&
                ( m_auStorArea2[0u]  == 0x10u ) && ( m_auStorArea2[1u]  == 0x10u ) && ( m_auStorArea2[2u]  == 0x10u ) &&
                ( m_auStorArea2[3u]  == 0x10u ) && ( m_auStorArea2[4u]  == 0x04u ) && ( m_auStorArea2[5u]  == 0x01u ) &&
                ( m_auStorArea2[6u]  == 0x00u ) && ( m_auStorArea2[7u]  == 0x00u ) && ( m_auStorArea2[8u]  == 0x00u ) &&
                ( m_auStorArea2[9u]  == 0x01u ) && ( m_auStorArea2[10u] == 0x01u ) && ( m_auStorArea2[11u] == 0x00u ) &&
                ( m_auStorArea2[12u] == 0x02u ) && ( m_auStorArea2[13u] == 0x00u ) && ( m_auStorArea2[14u] == 0x00u ) &&
                ( m_auStorArea2[15u] == 0x00u ) && ( m_auStorArea2[16u] == 0xA5u ) && ( m_auStorArea2[17u] == 0xA5u ) &&
                ( m_auStorArea2[18u] == 0xA5u ) && ( m_auStorArea2[19u] == 0xA5u ) && ( m_auStorArea2[20u] == 0xDCu ) &&
                ( m_auStorArea2[21u] == 0x02u ) && ( m_auStorArea2[22u] == 0x00u ) && ( m_auStorArea2[23u] == 0x00u ) )
              {
                  (void)printf("eFSS_BLOBCTST_FlushBkupTest 3  -- OK \n");
              }
              else
              {
                  (void)printf("eFSS_BLOBCTST_FlushBkupTest 3  -- FAIL \n");
              }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_FlushBkupTest 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_FlushBkupTest 3  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0, sizeof(m_auStorArea2));
    (void)memset(l_ltUseBuff.puBuf, 0x10, l_ltUseBuff.uBufL);


    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_FlushBuffInPageNBkp(&l_tCtx, 1u, 0u, 0x04u, 0x03u) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] )  && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] )  && ( 0x04u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x03u == l_ltUseBuff2.puBuf[4u] ) )
        {
            if( ( m_auStorArea2[0u]  == 0x10u ) && ( m_auStorArea2[1u]  == 0x10u ) && ( m_auStorArea2[2u]  == 0x10u ) &&
                ( m_auStorArea2[3u]  == 0x10u ) && ( m_auStorArea2[4u]  == 0x04u ) && ( m_auStorArea2[5u]  == 0x01u ) &&
                ( m_auStorArea2[6u]  == 0x00u ) && ( m_auStorArea2[7u]  == 0x00u ) && ( m_auStorArea2[8u]  == 0x00u ) &&
                ( m_auStorArea2[9u]  == 0x01u ) && ( m_auStorArea2[10u] == 0x01u ) && ( m_auStorArea2[11u] == 0x00u ) &&
                ( m_auStorArea2[12u] == 0x02u ) && ( m_auStorArea2[13u] == 0x00u ) && ( m_auStorArea2[14u] == 0x00u ) &&
                ( m_auStorArea2[15u] == 0x00u ) && ( m_auStorArea2[16u] == 0xA5u ) && ( m_auStorArea2[17u] == 0xA5u ) &&
                ( m_auStorArea2[18u] == 0xA5u ) && ( m_auStorArea2[19u] == 0xA5u ) && ( m_auStorArea2[20u] == 0xDCu ) &&
                ( m_auStorArea2[21u] == 0x02u ) && ( m_auStorArea2[22u] == 0x00u ) && ( m_auStorArea2[23u] == 0x00u ) &&
                ( m_auStorArea1[0u]  == 0x10u ) && ( m_auStorArea1[1u]  == 0x10u ) && ( m_auStorArea1[2u]  == 0x10u ) &&
                ( m_auStorArea1[3u]  == 0x10u ) && ( m_auStorArea1[4u]  == 0x03u ) && ( m_auStorArea1[5u]  == 0x00u ) &&
                ( m_auStorArea1[6u]  == 0x00u ) && ( m_auStorArea1[7u]  == 0x00u ) && ( m_auStorArea1[8u]  == 0x00u ) &&
                ( m_auStorArea1[9u]  == 0x01u ) && ( m_auStorArea1[10u] == 0x01u ) && ( m_auStorArea1[11u] == 0x00u ) &&
                ( m_auStorArea1[12u] == 0x02u ) && ( m_auStorArea1[13u] == 0x00u ) && ( m_auStorArea1[14u] == 0x00u ) &&
                ( m_auStorArea1[15u] == 0x00u ) && ( m_auStorArea1[16u] == 0xA5u ) && ( m_auStorArea1[17u] == 0xA5u ) &&
                ( m_auStorArea1[18u] == 0xA5u ) && ( m_auStorArea1[19u] == 0xA5u ) && ( m_auStorArea1[20u] == 0xDAu ) &&
                ( m_auStorArea1[21u] == 0x02u ) && ( m_auStorArea1[22u] == 0x00u ) && ( m_auStorArea1[23u] == 0x00u ) )
              {
                  (void)printf("eFSS_BLOBCTST_FlushBkupTest 4  -- OK \n");
              }
              else
              {
                  (void)printf("eFSS_BLOBCTST_FlushBkupTest 4  -- FAIL \n");
              }
        }
        else
        {
            (void)printf("eFSS_BLOBCTST_FlushBkupTest 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_FlushBkupTest 4  -- FAIL \n");
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

static void eFSS_BLOBCTST_GenTest(void)
{
    /* Local variable */
    t_eFSS_BLOBC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_BLOBC_StorBuf l_ltUseBuff;
    uint8_t l_uSubTypeRead;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcTst1Adapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 2u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;
    l_uSubTypeRead = 0u;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_BLOBCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_BLOBCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_BLOBCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_BLOBCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_BLOBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_BLOBC_RES_OK;

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0, sizeof(m_auStorArea2));

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_GenTest 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_GenTest 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_GetBuff(&l_tCtx, &l_ltUseBuff) )
    {
        (void)printf("eFSS_BLOBCTST_GenTest 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_GenTest 2  -- FAIL \n");
    }

    if( e_eFSS_BLOBC_RES_NOTVALIDPAGE == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        (void)printf("eFSS_BLOBCTST_GenTest 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_GenTest 3  -- FAIL \n");
    }

    if( e_eFSS_BLOBC_RES_NOTVALIDPAGE == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 1u, &l_uSubTypeRead) )
    {
        (void)printf("eFSS_BLOBCTST_GenTest 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_GenTest 4  -- FAIL \n");
    }

    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_FlushBuffInPage(&l_tCtx, 0u, 0x03u) )
    {
        (void)printf("eFSS_BLOBCTST_GenTest 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_GenTest 5  -- FAIL \n");
    }

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_FlushBuffInPage(&l_tCtx, 1u, 0x04u) )
    {
        (void)printf("eFSS_BLOBCTST_GenTest 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_GenTest 6  -- FAIL \n");
    }

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        (void)printf("eFSS_BLOBCTST_GenTest 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_GenTest 7  -- FAIL \n");
    }

    if( ( 0x01u == l_ltUseBuff.puBuf[0u] ) && ( 0x02u == l_ltUseBuff.puBuf[1u] ) && ( 0x03u == l_ltUseBuff.puBuf[2u] ) && ( 0x04u == l_ltUseBuff.puBuf[3u] ) )
    {
        (void)printf("eFSS_BLOBCTST_GenTest 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_GenTest 8  -- FAIL \n");
    }

    l_tStorSet.uPageVersion = 2u;
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_GenTest 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_GenTest 9  -- FAIL \n");
    }

    if( e_eFSS_BLOBC_RES_NEWVERSIONFOUND == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        (void)printf("eFSS_BLOBCTST_GenTest 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_GenTest 10 -- FAIL \n");
    }

    l_tStorSet.uPageVersion = 1u;
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_GenTest 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_GenTest 11 -- FAIL \n");
    }

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        (void)printf("eFSS_BLOBCTST_GenTest 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_GenTest 12 -- FAIL \n");
    }

    if( ( 0x01u == l_ltUseBuff.puBuf[0u] ) && ( 0x02u == l_ltUseBuff.puBuf[1u] ) && ( 0x03u == l_ltUseBuff.puBuf[2u] ) && ( 0x04u == l_ltUseBuff.puBuf[3u] ) )
    {
        (void)printf("eFSS_BLOBCTST_GenTest 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_GenTest 13 -- FAIL \n");
    }

    l_tStorSet.uTotPages = 3u;
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_GenTest 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_GenTest 14 -- FAIL \n");
    }

    if( e_eFSS_BLOBC_RES_NOTVALIDPAGE == eFSS_BLOBC_LoadPageInBuffNRipBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        (void)printf("eFSS_BLOBCTST_GenTest 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_GenTest 15 -- FAIL \n");
    }

    l_tStorSet.uTotPages = 2u;
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_BLOBCTST_GenTest 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_GenTest 16 -- FAIL \n");
    }

    l_ltUseBuff.puBuf[0u] = 0x11u;
    l_ltUseBuff.puBuf[1u] = 0x12u;
    l_ltUseBuff.puBuf[2u] = 0x13u;
    l_ltUseBuff.puBuf[3u] = 0x14u;
    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_FlushBuffInPageNBkp(&l_tCtx, 0u, 1u, 0x03u, 0x04u) )
    {
        (void)printf("eFSS_BLOBCTST_GenTest 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_GenTest 17 -- FAIL \n");
    }

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 0u, &l_uSubTypeRead) )
    {
        (void)printf("eFSS_BLOBCTST_GenTest 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_GenTest 18 -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff.puBuf[0u] ) && ( 0x12u == l_ltUseBuff.puBuf[1u] ) && ( 0x13u == l_ltUseBuff.puBuf[2u] ) && ( 0x14u == l_ltUseBuff.puBuf[3u] ) &&
        ( 0x03u == l_uSubTypeRead ) )
    {
        (void)printf("eFSS_BLOBCTST_GenTest 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_GenTest 19 -- FAIL \n");
    }

    if( e_eFSS_BLOBC_RES_OK == eFSS_BLOBC_LoadPageInBuff(&l_tCtx, 1u, &l_uSubTypeRead) )
    {
        (void)printf("eFSS_BLOBCTST_GenTest 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_GenTest 20 -- FAIL \n");
    }

    if( ( 0x11u == l_ltUseBuff.puBuf[0u] ) && ( 0x12u == l_ltUseBuff.puBuf[1u] ) && ( 0x13u == l_ltUseBuff.puBuf[2u] ) && ( 0x14u == l_ltUseBuff.puBuf[3u] ) &&
        ( 0x04u == l_uSubTypeRead ) )
    {
        (void)printf("eFSS_BLOBCTST_GenTest 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_BLOBCTST_GenTest 21 -- FAIL \n");
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