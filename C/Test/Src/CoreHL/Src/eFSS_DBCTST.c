/**
 * @file       eFSS_DBCTST.c
 *
 * @brief      Database Core module test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_DBCTST.h"
#include "eFSS_DBC.h"
#include <stdio.h>



/***********************************************************************************************************************
 *   PRIVATE TEST STRUCT DECLARATION
 **********************************************************************************************************************/
struct t_eFSS_TYPE_EraseCtxUser
{
    e_eFSS_DBC_RES eLastEr;
    uint32_t uTimeUsed;
};

struct t_eFSS_TYPE_WriteCtxUser
{
    e_eFSS_DBC_RES eLastEr;
    uint32_t uTimeUsed;
};

struct t_eFSS_TYPE_ReadCtxUser
{
    e_eFSS_DBC_RES eLastEr;
    uint32_t uTimeUsed;
};

struct t_eFSS_TYPE_CrcCtxUser
{
    e_eFSS_DBC_RES eLastEr;
    uint32_t uTimeUsed;
};



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_DBCTST_EraseAdapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase);

static bool_t eFSS_DBCTST_EraseErrAdapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase);

static bool_t eFSS_DBCTST_EraseTst1Adapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase);

static bool_t eFSS_DBCTST_EraseTstAAdapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase);

static bool_t eFSS_DBCTST_WriteAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                        const uint32_t p_uPageToWrite, const uint8_t* p_puDataToWrite,
                                        const uint32_t p_uDataToWriteL );

static bool_t eFSS_DBCTST_WriteErrAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                           const uint32_t p_uPageToWrite, const uint8_t* p_puDataToWrite,
                                           const uint32_t p_uDataToWriteL );

static bool_t eFSS_DBCTST_WriteErrSecAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                              const uint32_t p_uPageToWrite, const uint8_t* p_puDataToWrite,
                                              const uint32_t p_uDataToWriteL );

static bool_t eFSS_DBCTST_WriteTst1Adapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                        const uint32_t p_uPageToWrite, const uint8_t* p_puDataToWrite,
                                        const uint32_t p_uDataToWriteL );

static bool_t eFSS_DBCTST_WriteTstAAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
                                        const uint32_t p_uPageToWrite, const uint8_t* p_puDataToWrite,
                                        const uint32_t p_uDataToWriteL );

static bool_t eFSS_DBCTST_ReadAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                       const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                       const uint32_t p_uReadBufferL );

static bool_t eFSS_DBCTST_ReadErrAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                          const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                          const uint32_t p_uReadBufferL );

static bool_t eFSS_DBCTST_ReadErrSecAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                             const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                             const uint32_t p_uReadBufferL );

static bool_t eFSS_DBCTST_ReadTst1Adapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                       const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                       const uint32_t p_uReadBufferL );

static bool_t eFSS_DBCTST_ReadTst2Adapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                        const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                        const uint32_t p_uReadBufferL );

static bool_t eFSS_DBCTST_ReadTstAAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
                                       const uint32_t p_uPageToRead, uint8_t* const p_puReadBuffer,
                                       const uint32_t p_uReadBufferL );

static bool_t eFSS_DBCTST_CrcAdapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
                                      const uint8_t* p_puData, const uint32_t p_uDataL,
                                      uint32_t* const p_puCrc32Val );

static bool_t eFSS_DBCTST_CrcErrAdapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
                                         const uint8_t* p_puData, const uint32_t p_uDataL,
                                         uint32_t* const p_puCrc32Val );

static bool_t eFSS_DBCTST_CrcTst1Adapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
                                          const uint8_t* p_puData, const uint32_t p_uDataL,
                                          uint32_t* const p_puCrc32Val );

static bool_t eFSS_DBCTST_CrcTstAAdapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
                                          const uint8_t* p_puData, const uint32_t p_uDataL,
                                          uint32_t* const p_puCrc32Val );

/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void eFSS_DBCTST_BadPointer(void);
static void eFSS_DBCTST_BadInit(void);
static void eFSS_DBCTST_BadParamEntr(void);
static void eFSS_DBCTST_CorruptedCtx(void);
static void eFSS_DBCTST_Basic(void);
static void eFSS_DBCTST_BadClBckNRetry(void);
static void eFSS_DBCTST_LoadBkupTest(void);
static void eFSS_DBCTST_FlushBkupTest(void);
static void eFSS_DBCTST_GenTest(void);
static void eFSS_DBCTST_CodeCoverage(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eFSS_DBCTST_ExeTest(void)
{
	(void)printf("\n\nDB CORE TEST START \n\n");

    eFSS_DBCTST_BadPointer();
    eFSS_DBCTST_BadInit();
    eFSS_DBCTST_BadParamEntr();
    eFSS_DBCTST_CorruptedCtx();
    eFSS_DBCTST_Basic();
    eFSS_DBCTST_BadClBckNRetry();
    eFSS_DBCTST_LoadBkupTest();
    eFSS_DBCTST_FlushBkupTest();
    eFSS_DBCTST_GenTest();
    eFSS_DBCTST_CodeCoverage();

    (void)printf("\n\nDB CORE TEST END \n\n");
}



/***********************************************************************************************************************
 *   PRIVATE MODULES VARIABLE DECLARATION
 **********************************************************************************************************************/
static bool_t  m_bIsErased1 = false;
static bool_t  m_bIsErased2 = false;
static bool_t  m_bIsErasedA[6u];
static uint8_t m_auStorArea1[24u];
static uint8_t m_auStorArea2[24u];
static uint8_t m_auStorAreaA[6u][24u];


/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-8.13"
    /* Suppressed for code clarity in test execution*/
#endif


static bool_t eFSS_DBCTST_EraseAdapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase)
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
        p_ptCtx->eLastEr = e_eFSS_DBC_RES_OK;
    }

    return l_bRes;
}

static bool_t eFSS_DBCTST_EraseErrAdapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase)
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
        p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPOINTER;
    }

    return l_bRes;
}

static bool_t eFSS_DBCTST_EraseTst1Adapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase)
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
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPARAM;
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

static bool_t eFSS_DBCTST_EraseTstAAdapt(t_eFSS_TYPE_EraseCtx* const p_ptCtx, const uint32_t p_uPageToErase)
{
    bool_t l_bRes;

    if( NULL == p_ptCtx )
    {
        l_bRes = false;
    }
    else
    {
        if( p_uPageToErase >= 6u )
        {
            l_bRes = false;
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPARAM;
        }
        else
        {
            p_ptCtx->uTimeUsed++;

            l_bRes = true;
            m_bIsErasedA[p_uPageToErase] = true;
            (void)memset(m_auStorAreaA[p_uPageToErase], 0, sizeof(m_auStorAreaA[p_uPageToErase]));
        }
    }

    return l_bRes;
}

static bool_t eFSS_DBCTST_WriteAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPOINTER;
        }
        else
        {
            l_bRes = true;
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_OK;
        }
    }

    return l_bRes;
}

static bool_t eFSS_DBCTST_WriteErrAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPOINTER;
        }
        else
        {
            l_bRes = false;
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPOINTER;
        }
    }

    return l_bRes;
}

static bool_t eFSS_DBCTST_WriteErrSecAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPOINTER;
        }
        else
        {
            if( p_uPageToWrite >= 2u )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPARAM;
            }
            else
            {
                if( 24u != p_uDataToWriteL )
                {
                    l_bRes = false;
                    p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPARAM;
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
                                p_ptCtx->eLastEr = e_eFSS_DBC_RES_CORRUPTCTX;
                            }
                            else
                            {
                                l_bRes = true;
                                p_ptCtx->eLastEr = e_eFSS_DBC_RES_OK;
                                m_bIsErased1 = false;
                                (void)memcpy(m_auStorArea1, p_puDataToWrite, p_uDataToWriteL);
                            }
                        }
                        else
                        {
                            if( false == m_bIsErased2 )
                            {
                                l_bRes = false;
                                p_ptCtx->eLastEr = e_eFSS_DBC_RES_CORRUPTCTX;
                            }
                            else
                            {
                                l_bRes = true;
                                p_ptCtx->eLastEr = e_eFSS_DBC_RES_OK;
                                m_bIsErased2 = false;
                                (void)memcpy(m_auStorArea2, p_puDataToWrite, p_uDataToWriteL);
                            }
                        }

                        p_ptCtx->uTimeUsed++;

                    }
                    else
                    {
                        l_bRes = false;
                        p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPOINTER;
                    }
                }
            }
        }
    }

    return l_bRes;
}

static bool_t eFSS_DBCTST_WriteTst1Adapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPOINTER;
        }
        else
        {
            if( p_uPageToWrite >= 2u )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPARAM;
            }
            else
            {
                if( 24u != p_uDataToWriteL )
                {
                    l_bRes = false;
                    p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPARAM;
                }
                else
                {
                    p_ptCtx->uTimeUsed++;

                    if( 0u == p_uPageToWrite )
                    {
                        if( false == m_bIsErased1 )
                        {
                            l_bRes = false;
                            p_ptCtx->eLastEr = e_eFSS_DBC_RES_CORRUPTCTX;
                        }
                        else
                        {
                            l_bRes = true;
                            p_ptCtx->eLastEr = e_eFSS_DBC_RES_OK;
                            m_bIsErased1 = false;
                            (void)memcpy(m_auStorArea1, p_puDataToWrite, p_uDataToWriteL);
                        }
                    }
                    else
                    {
                        if( false == m_bIsErased2 )
                        {
                            l_bRes = false;
                            p_ptCtx->eLastEr = e_eFSS_DBC_RES_CORRUPTCTX;
                        }
                        else
                        {
                            l_bRes = true;
                            p_ptCtx->eLastEr = e_eFSS_DBC_RES_OK;
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

static bool_t eFSS_DBCTST_WriteTstAAdapt(t_eFSS_TYPE_WriteCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPOINTER;
        }
        else
        {
            if( p_uPageToWrite >= 6u )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPARAM;
            }
            else
            {
                if( 24u != p_uDataToWriteL )
                {
                    l_bRes = false;
                    p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPARAM;
                }
                else
                {
                    p_ptCtx->uTimeUsed++;

                    if( false == m_bIsErasedA[p_uPageToWrite] )
                    {
                        l_bRes = false;
                        p_ptCtx->eLastEr = e_eFSS_DBC_RES_CORRUPTCTX;
                    }
                    else
                    {
                        l_bRes = true;
                        p_ptCtx->eLastEr = e_eFSS_DBC_RES_OK;
                        m_bIsErasedA[p_uPageToWrite] = false;
                        (void)memcpy(m_auStorAreaA[p_uPageToWrite], p_puDataToWrite, p_uDataToWriteL);
                    }
                }
            }
        }
    }

    return l_bRes;
}

static bool_t eFSS_DBCTST_ReadAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPOINTER;
        }
        else
        {
            l_bRes = true;
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_OK;
        }
    }

    return l_bRes;
}

static bool_t eFSS_DBCTST_ReadErrAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPOINTER;
        }
        else
        {
            l_bRes = false;
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPOINTER;
        }
    }

    return l_bRes;
}


static bool_t eFSS_DBCTST_ReadErrSecAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPOINTER;
        }
        else
        {
            if( p_uPageToRead >= 2u )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPARAM;
            }
            else
            {
                if( 24u != p_uReadBufferL )
                {
                    l_bRes = false;
                    p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPARAM;
                }
                else
                {
                    if( 0u == p_ptCtx->uTimeUsed )
                    {
                        if( 0u == p_uPageToRead )
                        {
                            l_bRes = true;
                            p_ptCtx->eLastEr = e_eFSS_DBC_RES_OK;
                            (void)memcpy(p_puReadBuffer, m_auStorArea1, p_uReadBufferL);
                        }
                        else
                        {
                            l_bRes = true;
                            p_ptCtx->eLastEr = e_eFSS_DBC_RES_OK;
                            (void)memcpy(p_puReadBuffer, m_auStorArea2, p_uReadBufferL);
                        }

                        p_ptCtx->uTimeUsed++;
                    }
                    else
                    {
                        l_bRes = false;
                        p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPOINTER;
                    }
                }
            }
        }
    }

    return l_bRes;
}

static bool_t eFSS_DBCTST_ReadTst1Adapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPOINTER;
        }
        else
        {
            if( p_uPageToRead >= 2u )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPARAM;
            }
            else
            {
                if( 24u != p_uReadBufferL )
                {
                    l_bRes = false;
                    p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPARAM;
                }
                else
                {
                    p_ptCtx->uTimeUsed++;

                    if( 0u == p_uPageToRead )
                    {
                        l_bRes = true;
                        p_ptCtx->eLastEr = e_eFSS_DBC_RES_OK;
                        (void)memcpy(p_puReadBuffer, m_auStorArea1, p_uReadBufferL);
                    }
                    else
                    {
                        l_bRes = true;
                        p_ptCtx->eLastEr = e_eFSS_DBC_RES_OK;
                        (void)memcpy(p_puReadBuffer, m_auStorArea2, p_uReadBufferL);
                    }
                }
            }
        }
    }

    return l_bRes;
}

static bool_t eFSS_DBCTST_ReadTst2Adapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPOINTER;
        }
        else
        {
            if( p_uPageToRead >= 2u )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPARAM;
            }
            else
            {
                if( 24u != p_uReadBufferL )
                {
                    l_bRes = false;
                    p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPARAM;
                }
                else
                {
                    p_ptCtx->uTimeUsed++;

                    if( 0u == p_uPageToRead )
                    {
                        l_bRes = true;
                        p_ptCtx->eLastEr = e_eFSS_DBC_RES_OK;
                        (void)memcpy(p_puReadBuffer, m_auStorArea1, p_uReadBufferL);
                        p_puReadBuffer[0] = p_puReadBuffer[0] + 2u;
                    }
                    else
                    {
                        l_bRes = true;
                        p_ptCtx->eLastEr = e_eFSS_DBC_RES_OK;
                        (void)memcpy(p_puReadBuffer, m_auStorArea2, p_uReadBufferL);
                        p_puReadBuffer[0] = p_puReadBuffer[0] + 2u;
                    }
                }
            }
        }
    }

    return l_bRes;
}

static bool_t eFSS_DBCTST_ReadTstAAdapt(t_eFSS_TYPE_ReadCtx* const p_ptCtx,
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
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPOINTER;
        }
        else
        {
            if( p_uPageToRead >= 6u )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPARAM;
            }
            else
            {
                if( 24u != p_uReadBufferL )
                {
                    l_bRes = false;
                    p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPARAM;
                }
                else
                {
                    p_ptCtx->uTimeUsed++;

                    l_bRes = true;
                    p_ptCtx->eLastEr = e_eFSS_DBC_RES_OK;
                    (void)memcpy(p_puReadBuffer, m_auStorAreaA[p_uPageToRead], p_uReadBufferL);
                }
            }
        }
    }

    return l_bRes;
}

static bool_t eFSS_DBCTST_CrcAdapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
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
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPOINTER;
        }
        else
        {
            l_bRes = true;
            *p_puCrc32Val = 0u;
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_OK;
        }
    }

    return l_bRes;
}

static bool_t eFSS_DBCTST_CrcErrAdapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
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
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPOINTER;
        }
        else
        {
            l_bRes = false;
            *p_puCrc32Val = 0u;
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPOINTER;
        }
    }

    return l_bRes;
}

static bool_t eFSS_DBCTST_CrcTst1Adapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
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
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPOINTER;
        }
        else
        {
            if( 0u == p_uDataL )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPARAM;
            }
            else
            {
                l_bRes = true;
                p_ptCtx->eLastEr = e_eFSS_DBC_RES_OK;
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

static bool_t eFSS_DBCTST_CrcTstAAdapt(t_eFSS_TYPE_CrcCtx* const p_ptCtx, const uint32_t p_uUseed,
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
            p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPOINTER;
        }
        else
        {
            if( 0u == p_uDataL )
            {
                l_bRes = false;
                p_ptCtx->eLastEr = e_eFSS_DBC_RES_BADPARAM;
            }
            else
            {
                l_bRes = true;
                p_ptCtx->eLastEr = e_eFSS_DBC_RES_OK;
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
void eFSS_DBCTST_BadPointer(void)
{
    /* Local variable */
    t_eFSS_DBC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    bool_t l_bIsInit;
    t_eFSS_DBC_StorBuf l_ltUseBuff;
    uint32_t l_uTotPage;

    /* Misra complaiant */
    l_tCtxErase.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxWrite.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxCrc32.uTimeUsed = 0u;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_DBCTST_EraseAdapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_DBCTST_WriteAdapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_DBCTST_ReadAdapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcAdapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 2u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 2u;
    l_tStorSet.uPageVersion = 1u;
    l_uTotPage = 0u;

    /* Function */
    if( e_eFSS_DBC_RES_BADPOINTER == eFSS_DBC_InitCtx(NULL, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadPointer 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_BADPOINTER == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, NULL, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadPointer 2  -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.ptCtxErase = NULL;
    if( e_eFSS_DBC_RES_BADPOINTER == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadPointer 3  -- OK \n");
        l_tCtxCb.ptCtxErase = &l_tCtxErase;
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadPointer 3  -- FAIL \n");
        l_tCtxCb.ptCtxErase = &l_tCtxErase;
    }

    /* Function */
    l_tCtxCb.fErase = NULL;
    if( e_eFSS_DBC_RES_BADPOINTER == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadPointer 4  -- OK \n");
        l_tCtxCb.fErase = &eFSS_DBCTST_EraseAdapt;
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadPointer 4  -- FAIL \n");
        l_tCtxCb.fErase = &eFSS_DBCTST_EraseAdapt;
    }

    /* Function */
    l_tCtxCb.ptCtxWrite = NULL;
    if( e_eFSS_DBC_RES_BADPOINTER == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadPointer 5  -- OK \n");
        l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadPointer 5  -- FAIL \n");
        l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    }

    /* Function */
    l_tCtxCb.fWrite = NULL;
    if( e_eFSS_DBC_RES_BADPOINTER == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadPointer 6  -- OK \n");
        l_tCtxCb.fWrite = &eFSS_DBCTST_WriteAdapt;
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadPointer 6  -- FAIL \n");
        l_tCtxCb.fWrite = &eFSS_DBCTST_WriteAdapt;
    }

    /* Function */
    l_tCtxCb.ptCtxRead = NULL;
    if( e_eFSS_DBC_RES_BADPOINTER == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadPointer 7  -- OK \n");
        l_tCtxCb.ptCtxRead = &l_tCtxRead;
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadPointer 7  -- FAIL \n");
        l_tCtxCb.ptCtxRead = &l_tCtxRead;
    }

    /* Function */
    l_tCtxCb.fRead = NULL;
    if( e_eFSS_DBC_RES_BADPOINTER == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadPointer 8  -- OK \n");
        l_tCtxCb.fRead = &eFSS_DBCTST_ReadAdapt;
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadPointer 8  -- FAIL \n");
        l_tCtxCb.fRead = &eFSS_DBCTST_ReadAdapt;
    }

    /* Function */
    l_tCtxCb.ptCtxCrc32 = NULL;
    if( e_eFSS_DBC_RES_BADPOINTER == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadPointer 9  -- OK \n");
        l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadPointer 9  -- FAIL \n");
        l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    }

    /* Function */
    l_tCtxCb.fCrc32 = NULL;
    if( e_eFSS_DBC_RES_BADPOINTER == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadPointer 10 -- OK \n");
        l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcAdapt;
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadPointer 10 -- FAIL \n");
        l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcAdapt;
    }

    /* Function */
    if( e_eFSS_DBC_RES_BADPOINTER == eFSS_DBC_IsInit(NULL, &l_bIsInit) )
    {
        (void)printf("eFSS_DBCTST_BadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadPointer 11 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_BADPOINTER == eFSS_DBC_IsInit(&l_tCtx, NULL) )
    {
        (void)printf("eFSS_DBCTST_BadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadPointer 12 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadPointer 13 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_BADPOINTER == eFSS_DBC_GetBuffNUsable(NULL, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_BadPointer 14 -- OK \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadPointer 14 -- FAIL \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadPointer 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadPointer 15 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_BADPOINTER == eFSS_DBC_GetBuffNUsable(&l_tCtx, NULL, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_BadPointer 16 -- OK \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadPointer 16 -- FAIL \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadPointer 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadPointer 17 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_BADPOINTER == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, NULL) )
    {
        (void)printf("eFSS_DBCTST_BadPointer 18 -- OK \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadPointer 18 -- FAIL \n");
        (void)memset(&l_tCtx, 0, sizeof(l_tCtx));
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadPointer 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadPointer 19 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_BADPOINTER == eFSS_DBC_LoadPageInBuff(NULL, 0u) )
    {
        (void)printf("eFSS_DBCTST_BadPointer 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadPointer 20 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadPointer 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadPointer 21 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_BADPOINTER == eFSS_DBC_FlushBuffInPage(NULL, 0u) )
    {
        (void)printf("eFSS_DBCTST_BadPointer 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadPointer 22 -- FAIL \n");
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

void eFSS_DBCTST_BadInit(void)
{
    /* Local variable */
    t_eFSS_DBC_Ctx l_tCtx;
    bool_t l_bIsInit;
    t_eFSS_DBC_StorBuf l_ltUseBuff;
    uint32_t l_uTotPage;

    /* Init the remainings var */
    (void)memset(&l_tCtx, 0, sizeof(l_tCtx));

    /* Function */
    if( e_eFSS_DBC_RES_NOINITLIB == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_BadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadInit 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_NOINITLIB == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        (void)printf("eFSS_DBCTST_BadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadInit 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_NOINITLIB == eFSS_DBC_FlushBuffInPage(&l_tCtx, 0u) )
    {
        (void)printf("eFSS_DBCTST_BadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadInit 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_IsInit(&l_tCtx, &l_bIsInit) )
    {
        if( false == l_bIsInit )
        {
            (void)printf("eFSS_DBCTST_BadInit 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_BadInit 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadInit 4  -- FAIL \n");
    }
}

void eFSS_DBCTST_BadParamEntr(void)
{
    /* Local variable */
    t_eFSS_DBC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_DBCTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_DBCTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_DBCTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcTst1Adapt;

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
    l_tStorSet.uRWERetry = 2u;
    l_tStorSet.uPageVersion = 1u;


    /* Function */
    l_tStorSet.uPagesLen = 25u;
    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 1  -- OK \n");
        l_tStorSet.uPagesLen = 24u;
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 1  -- FAIL \n");
        l_tStorSet.uPagesLen = 24u;
    }

    /* Function */
    l_tStorSet.uPagesLen = 23u;
    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 2  -- OK \n");
        l_tStorSet.uPagesLen = 24u;
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 2  -- FAIL \n");
        l_tStorSet.uPagesLen = 24u;
    }

    /* Function */
    l_tStorSet.uTotPages = 0u;
    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 3  -- OK \n");
        l_tStorSet.uTotPages = 2u;
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 3  -- FAIL \n");
        l_tStorSet.uTotPages = 2u;
    }

    /* Function */
    l_tStorSet.uPagesLen = 19u;
    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u ) )
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 4  -- OK \n");
        l_tStorSet.uPagesLen = 24u;
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 4  -- FAIL \n");
        l_tStorSet.uPagesLen = 24u;
    }

    /* Function */
    l_tStorSet.uPagesLen = 18u;
    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u ) )
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 5  -- OK \n");
        l_tStorSet.uPagesLen = 24u;
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 5  -- FAIL \n");
        l_tStorSet.uPagesLen = 24u;
    }

    /* Function */
    l_tStorSet.uRWERetry = 0u;
    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 6  -- OK \n");
        l_tStorSet.uRWERetry = 1u;
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 6  -- FAIL \n");
        l_tStorSet.uRWERetry = 1u;
    }

    /* Function */
    l_tStorSet.uPagesLen = 20u;
    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, l_tStorSet.uPagesLen * 2u ) )
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 7  -- OK \n");
        l_tStorSet.uPagesLen = 24u;
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 7  -- FAIL \n");
        l_tStorSet.uPagesLen = 24u;
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 8  -- FAIL \n");
    }

    /* Function */
    l_tStorSet.uTotPages = 1u;
    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 9  -- OK \n");
        l_tStorSet.uTotPages = 2u;
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 9  -- FAIL \n");
        l_tStorSet.uTotPages = 2u;
    }

    /* Function */
    l_tStorSet.uTotPages = 3u;
    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 10 -- OK \n");
        l_tStorSet.uTotPages = 2u;
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 10 -- FAIL \n");
        l_tStorSet.uTotPages = 2u;
    }

    /* Function */
    l_tStorSet.uTotPages = 5u;
    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 11 -- OK \n");
        l_tStorSet.uTotPages = 2u;
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 11 -- FAIL \n");
        l_tStorSet.uTotPages = 2u;
    }

    /* Function */
    l_tStorSet.uTotPages = 4u;
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 12 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_LoadPageInBuff(&l_tCtx, 4u) )
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 13 -- OK \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_LoadPageInBuff(&l_tCtx, 3u) )
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 14 -- OK \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_LoadPageInBuff(&l_tCtx, 2u) )
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 15 -- OK \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_FlushBuffInPage(&l_tCtx, 4u) )
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 16 -- OK \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_FlushBuffInPage(&l_tCtx, 3u) )
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 17 -- OK \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_FlushBuffInPage(&l_tCtx, 2u) )
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadParamEntr 18 -- OK \n");
    }

}

void eFSS_DBCTST_CorruptedCtx(void)
{
    /* Local variable */
    t_eFSS_DBC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    uint32_t l_uTotPage;
    t_eFSS_DBC_StorBuf l_ltUseBuff;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_DBCTST_EraseAdapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_DBCTST_WriteAdapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_DBCTST_ReadAdapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcAdapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 4u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 2u;
    l_tStorSet.uPageVersion = 1u;
    l_uTotPage = 0u;

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 2  -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxErase = NULL;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 3  -- OK \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxErase = &l_tCtxErase;
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 3  -- FAIL \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxErase = &l_tCtxErase;
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 4  -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fErase = NULL;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 5  -- OK \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fErase = &eFSS_DBCTST_EraseAdapt;
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 5  -- FAIL \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fErase = &eFSS_DBCTST_EraseAdapt;
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 6  -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxWrite = NULL;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 7  -- OK \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxWrite = &l_tCtxWrite;
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 7  -- FAIL \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxWrite = &l_tCtxWrite;
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 8  -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fWrite = NULL;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 9  -- OK \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fWrite = &eFSS_DBCTST_WriteAdapt;
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 9  -- FAIL \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fWrite = &eFSS_DBCTST_WriteAdapt;
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 10 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxRead = NULL;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 11 -- OK \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxRead = &l_tCtxRead;
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 11 -- FAIL \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxRead = &l_tCtxRead;
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 12 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fRead = NULL;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 13 -- OK \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fRead = &eFSS_DBCTST_ReadAdapt;
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 13 -- FAIL \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fRead = &eFSS_DBCTST_ReadAdapt;
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 14 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxCrc32 = NULL;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 15 -- OK \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 15 -- FAIL \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 16 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fCrc32 = NULL;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 17 -- OK \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fCrc32 = &eFSS_DBCTST_CrcAdapt;
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 17 -- FAIL \n");
        l_tCtx.tCOREHLCtx.tCORELLCtx.tCtxCb.fCrc32 = &eFSS_DBCTST_CrcAdapt;
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 18 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff1.puBuf = NULL;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 19 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 20 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff2.puBuf = NULL;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 21 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 22 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uTotPages = 0u;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 23 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 24 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uPagesLen = 19u;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 25 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 26 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uRWERetry = 0u;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 27 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 27 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 28 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 28 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff1.uBufL = l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff2.uBufL + 1u;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 29 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 29 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 30 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 30 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uPagesLen = l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff1.uBufL + 1u;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 31 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 31 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 32 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 32 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uPagesLen = 20u;
    l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff1.uBufL = 20u;
    l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff2.uBufL = 20u;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 33 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 33 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 34 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 34 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff2.puBuf = l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff1.puBuf;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 35 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 35 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 36 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 36 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uTotPages = 1u;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 37 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 37 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 38 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 38 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uTotPages = 3u;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 39 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 39 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 40 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 40 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uTotPages = 5u;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 41 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 41 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 42 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 42 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uPagesLen = l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff1.uBufL + 1u;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 43 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 43 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 44 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 44 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uPagesLen = l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff1.uBufL + 1u;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 45 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 45 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 46 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 46 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tCOREHLCtx.tCORELLCtx.tStorSett.uPagesLen = l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff1.uBufL + 1u;
    if( e_eFSS_DBC_RES_CORRUPTCTX == eFSS_DBC_FlushBuffInPage(&l_tCtx, 0u) )
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 47 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CorruptedCtx 47 -- FAIL \n");
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

void eFSS_DBCTST_Basic(void)
{
    /* Local variable */
    t_eFSS_DBC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    bool_t l_bIsInit;
    uint32_t l_uTotPage;
    t_eFSS_DBC_StorBuf l_ltUseBuff;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_DBCTST_EraseAdapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_DBCTST_WriteAdapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_DBCTST_ReadErrAdapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcAdapt;

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
    l_tStorSet.uTotPages = 4u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_Basic 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_Basic 1  -- FAIL \n");
    }

    /* Verify buffer validity */
    if( l_auStor == l_tCtx.tCOREHLCtx.tCORELLCtx.tBuff1.puBuf )
    {
        (void)printf("eFSS_DBCTST_Basic 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_Basic 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_IsInit(&l_tCtx, &l_bIsInit ) )
    {
        if( true == l_bIsInit )
        {
            (void)printf("eFSS_DBCTST_Basic 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_Basic 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_Basic 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        if( ( l_auStor == l_ltUseBuff.puBuf ) && ( (24u-20u) == l_ltUseBuff.uBufL ) )
        {
            (void)printf("eFSS_DBCTST_Basic 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_Basic 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_Basic 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        if( ( 2u ) == l_uTotPage )
        {
            (void)printf("eFSS_DBCTST_Basic 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_Basic 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_Basic 5  -- FAIL \n");
    }

    /* Init storage settings */
    l_tStorSet.uTotPages = 10u;

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_Basic 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_Basic 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        if( ( 5u ) == l_uTotPage )
        {
            (void)printf("eFSS_DBCTST_Basic 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_Basic 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_Basic 7  -- FAIL \n");
    }
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "CERT-INT31-C_c"
    /* Suppressed for code clarity in test execution*/
#endif

void eFSS_DBCTST_BadClBckNRetry(void)
{
    /* Local variable */
    t_eFSS_DBC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    bool_t l_bIsInit;
    t_eFSS_DBC_StorBuf l_ltUseBuff;
    uint32_t l_uTotPage;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_DBCTST_EraseAdapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_DBCTST_WriteAdapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_DBCTST_ReadErrAdapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcAdapt;

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
    l_uTotPage = 0u;

    /* ------------------------------------------------------------------------------------------ TEST READ CALL BACK */
    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_IsInit(&l_tCtx, &l_bIsInit ) )
    {
        if( true == l_bIsInit )
        {
            (void)printf("eFSS_DBCTST_BadClBckNRetry 2  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_BadClBckNRetry 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        if( ( l_auStor == l_ltUseBuff.puBuf ) && ( (24u-20u) == l_ltUseBuff.uBufL ) && ( 1u == l_uTotPage ) )
        {
            (void)printf("eFSS_DBCTST_BadClBckNRetry 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_BadClBckNRetry 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_IsInit(&l_tCtx, &l_bIsInit ) )
    {
        if( true == l_bIsInit )
        {
            (void)printf("eFSS_DBCTST_BadClBckNRetry 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_BadClBckNRetry 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 4  -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_DBCTST_EraseAdapt;
    l_tCtxCb.fWrite = &eFSS_DBCTST_WriteErrAdapt;
    l_tCtxCb.fRead = &eFSS_DBCTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 5  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 6  -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;

    if( e_eFSS_DBC_RES_CLBCKWRITEERR == eFSS_DBC_FlushBuffInPage(&l_tCtx, 0u) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_DBC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 3u == l_tCtxErase.uTimeUsed ) && ( e_eFSS_DBC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 0u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_DBC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 3u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_DBC_RES_BADPOINTER == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_DBCTST_BadClBckNRetry 7  -- OK \n");
            }
            else
            {
                (void)printf("eFSS_DBCTST_BadClBckNRetry 7  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_DBCTST_BadClBckNRetry 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 7  -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_DBCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_DBCTST_WriteErrSecAdapt;
    l_tCtxCb.fRead  = &eFSS_DBCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 8  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 9  -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;

    if( e_eFSS_DBC_RES_CLBCKWRITEERR == eFSS_DBC_FlushBuffInPage(&l_tCtx, 0u) )
    {
        if( ( 2u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_DBC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 4u == l_tCtxErase.uTimeUsed ) && ( e_eFSS_DBC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 1u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_DBC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 1u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_DBC_RES_BADPOINTER == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_DBCTST_BadClBckNRetry 10 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_DBCTST_BadClBckNRetry 10 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_DBCTST_BadClBckNRetry 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 10 -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_DBCTST_EraseAdapt;
    l_tCtxCb.fWrite = &eFSS_DBCTST_WriteAdapt;
    l_tCtxCb.fRead = &eFSS_DBCTST_ReadErrAdapt;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 11 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 12 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;

    if( e_eFSS_DBC_RES_CLBCKREADERR == eFSS_DBC_FlushBuffInPage(&l_tCtx, 0u) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_DBC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 3u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_DBC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 3u == l_tCtxWrite.uTimeUsed )  && ( e_eFSS_DBC_RES_OK == l_tCtxWrite.eLastEr ) )
        {
            if( ( 3u == l_tCtxRead.uTimeUsed ) && ( e_eFSS_DBC_RES_BADPOINTER == l_tCtxRead.eLastEr ) )
            {
                (void)printf("eFSS_DBCTST_BadClBckNRetry 13 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_DBCTST_BadClBckNRetry 13 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_DBCTST_BadClBckNRetry 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 13 -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_DBCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_DBCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_DBCTST_ReadErrSecAdapt;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 14 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 15 -- FAIL \n");
    }

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;

    if( e_eFSS_DBC_RES_CLBCKREADERR == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_DBC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 0u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_DBC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 0u == l_tCtxWrite.uTimeUsed )  && ( e_eFSS_DBC_RES_OK == l_tCtxWrite.eLastEr ) )
        {
            if( ( 1u == l_tCtxRead.uTimeUsed ) && ( e_eFSS_DBC_RES_BADPOINTER == l_tCtxRead.eLastEr ) )
            {
                (void)printf("eFSS_DBCTST_BadClBckNRetry 16 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_DBCTST_BadClBckNRetry 16 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_DBCTST_BadClBckNRetry 16 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 16 -- FAIL \n");
    }

    /* Init */
    l_tStorSet.uPageVersion = 0u;

    /* Function */
    l_tCtxCb.fErase = &eFSS_DBCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_DBCTST_WriteErrAdapt;
    l_tCtxCb.fRead = &eFSS_DBCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 17 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 18 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0xFF, sizeof(m_auStorArea2));

    /* Fill the two storage page */


    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u] = 0x01u;
    m_auStorArea1[5u] = 0x00u;
    m_auStorArea1[6u] = 0x00u;
    m_auStorArea1[7u] = 0x00u;
    m_auStorArea1[8u] = 0x00u;
    m_auStorArea1[9u] = 0x03u;
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
    m_auStorArea1[20u] = 0x99u;
    m_auStorArea1[21u] = 0x02u;
    m_auStorArea1[22u] = 0x00u;
    m_auStorArea1[23u] = 0x00u;

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[4u] = 0x10u;
    m_auStorArea2[5u] = 0x01u;
    m_auStorArea2[6u] = 0x00u;
    m_auStorArea2[7u] = 0x00u;
    m_auStorArea2[8u] = 0x00u;
    m_auStorArea2[9u] = 0x03u;
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
    m_auStorArea2[20u] = 0xE9u;
    m_auStorArea2[21u] = 0x02u;
    m_auStorArea2[22u] = 0x00u;
    m_auStorArea2[23u] = 0x00u;

    /* Test FLUSH */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;

    if( e_eFSS_DBC_RES_CLBCKWRITEERR == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 3u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_DBC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 3u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_DBC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 2u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_DBC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 3u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_DBC_RES_BADPOINTER == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_DBCTST_BadClBckNRetry 19 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_DBCTST_BadClBckNRetry 19 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_DBCTST_BadClBckNRetry 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 19 -- FAIL \n");
    }

    /* Init */
    l_tStorSet.uPageVersion = 0u;

    /* Function */
    l_tCtxCb.fErase = &eFSS_DBCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_DBCTST_WriteErrAdapt;
    l_tCtxCb.fRead = &eFSS_DBCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 20 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 21 -- FAIL \n");
    }


    /* Init */
    l_tStorSet.uPageVersion = 0u;

    /* Function */
    l_tCtxCb.fErase = &eFSS_DBCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_DBCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_DBCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcErrAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 26 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 27 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 27 -- FAIL \n");
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
    l_tCtxErase.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;

    if( e_eFSS_DBC_RES_CLBCKCRCERR == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) &&  ( e_eFSS_DBC_RES_BADPOINTER == l_tCtxCrc32.eLastEr ) &&
            ( 0u == l_tCtxErase.uTimeUsed ) &&  ( e_eFSS_DBC_RES_OK == l_tCtxErase.eLastEr ) &&
            ( 1u == l_tCtxRead.uTimeUsed  )  && ( e_eFSS_DBC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 0u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_DBC_RES_OK == l_tCtxWrite.eLastEr ) )
            {
                (void)printf("eFSS_DBCTST_BadClBckNRetry 28 -- OK \n");
            }
            else
            {
                (void)printf("eFSS_DBCTST_BadClBckNRetry 28 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_DBCTST_BadClBckNRetry 28 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_BadClBckNRetry 28 -- FAIL \n");
    }
}

static void eFSS_DBCTST_LoadBkupTest(void)
{
    /* Local variable */
    t_eFSS_DBC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_DBC_StorBuf l_ltUseBuff;
    t_eFSS_DBC_StorBuf l_ltUseBuff2;
    uint32_t l_uTotPage;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_DBCTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_DBCTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_DBCTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcTst1Adapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 2u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;
    l_uTotPage = 0u;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_DBCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_DBCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_DBCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_LoadBkupTest 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_LoadBkupTest 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_LoadBkupTest 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_LoadBkupTest 2  -- FAIL \n");
    }

    /* Setup storage area */
    l_ltUseBuff2.uBufL = l_ltUseBuff.uBufL  + 1u;
    l_ltUseBuff2.puBuf = &l_auStor[24];

    /* --------------------------------------------- All OK no backup generation */
    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u]  = 0x01u; /* Page SUBTYPE */
    m_auStorArea1[5u]  = 0x00u; /* Page index */
    m_auStorArea1[6u]  = 0x00u; /* Page index */
    m_auStorArea1[7u]  = 0x00u; /* Page index */
    m_auStorArea1[8u]  = 0x00u; /* Page index */
    m_auStorArea1[9u]  = 0x03u; /* Page type */
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
    m_auStorArea2[4u]  = 0x02u; /* Page SUBTYPE */
    m_auStorArea2[5u]  = 0x01u; /* Page index */
    m_auStorArea2[6u]  = 0x00u; /* Page index */
    m_auStorArea2[7u]  = 0x00u; /* Page index */
    m_auStorArea2[8u]  = 0x00u; /* Page index */
    m_auStorArea2[9u]  = 0x03u; /* Page type */
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

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 0x00u == l_ltUseBuff.puBuf[0u] )  && ( 0x00u == l_ltUseBuff.puBuf[1u] ) && ( 0x00u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[3u] )  && ( 0x01u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x00u == l_ltUseBuff2.puBuf[0u] ) && ( 0x00u == l_ltUseBuff2.puBuf[1u] ) && ( 0x00u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[3u] ) && ( 0x02u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_DBCTST_LoadBkupTest 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_LoadBkupTest 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_LoadBkupTest 3  -- FAIL \n");
    }

    /* --------------------------------------------- All OK no backup generation */
    /* Setup storage area */
    (void)memset(m_auStorArea1, 1, sizeof(m_auStorArea1));
    m_auStorArea1[4u]  = 0x01u; /* Page SUBTYPE */
    m_auStorArea1[5u]  = 0x00u; /* Page index */
    m_auStorArea1[6u]  = 0x00u; /* Page index */
    m_auStorArea1[7u]  = 0x00u; /* Page index */
    m_auStorArea1[8u]  = 0x00u; /* Page index */
    m_auStorArea1[9u]  = 0x03u; /* Page type */
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
    m_auStorArea2[4u]  = 0x02u; /* Page SUBTYPE */
    m_auStorArea2[5u]  = 0x01u; /* Page index */
    m_auStorArea2[6u]  = 0x00u; /* Page index */
    m_auStorArea2[7u]  = 0x00u; /* Page index */
    m_auStorArea2[8u]  = 0x00u; /* Page index */
    m_auStorArea2[9u]  = 0x03u; /* Page type */
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

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 0x01u == l_ltUseBuff.puBuf[0u] )  && ( 0x01u == l_ltUseBuff.puBuf[1u] ) && ( 0x01u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x01u == l_ltUseBuff.puBuf[3u] )  && ( 0x01u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x01u == l_ltUseBuff2.puBuf[0u] ) && ( 0x01u == l_ltUseBuff2.puBuf[1u] ) && ( 0x01u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x01u == l_ltUseBuff2.puBuf[3u] ) && ( 0x02u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_DBCTST_LoadBkupTest 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_LoadBkupTest 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_LoadBkupTest 4  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u]  = 0x02u; /* Page SUBTYPE */
    m_auStorArea1[5u]  = 0x00u; /* Page index */
    m_auStorArea1[6u]  = 0x00u; /* Page index */
    m_auStorArea1[7u]  = 0x00u; /* Page index */
    m_auStorArea1[8u]  = 0x00u; /* Page index */
    m_auStorArea1[9u]  = 0x03u; /* Page type */
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
    m_auStorArea1[20u] = 0x9Bu; /* CRC */
    m_auStorArea1[21u] = 0x02u; /* CRC */
    m_auStorArea1[22u] = 0x00u; /* CRC */
    m_auStorArea1[23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea2, 0x10, sizeof(m_auStorArea2));
    m_auStorArea2[4u]  = 0x02u;  /* Page SUBTYPE */
    m_auStorArea2[5u]  = 0x01u;  /* Page index */
    m_auStorArea2[6u]  = 0x00u;  /* Page index */
    m_auStorArea2[7u]  = 0x00u;  /* Page index */
    m_auStorArea2[8u]  = 0x00u;  /* Page index */
    m_auStorArea2[9u]  = 0x03u;  /* Page type */
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

    if( e_eFSS_DBC_RES_OK_BKP_RCVRD == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] )  && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] )  && ( 0x01u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x02u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x10u == m_auStorArea1[0u] )  && ( 0x10u == m_auStorArea1[1u] ) && ( 0x10u == m_auStorArea1[2u] ) &&
            ( 0x10u == m_auStorArea1[3u] )  && ( 0x01u == m_auStorArea1[4u] ) && ( 0x00u == m_auStorArea1[5u] ) &&
            ( 0xDAu == m_auStorArea1[20u] )  && ( 0x02u == m_auStorArea1[21u] ) && ( 0x00u == m_auStorArea1[22u] ) &&
            ( 0x00u == m_auStorArea1[23u] ) &&
            ( 0x10u == m_auStorArea2[0u] )  && ( 0x10u == m_auStorArea2[1u] ) && ( 0x10u == m_auStorArea2[2u] ) &&
            ( 0x10u == m_auStorArea2[3u] )  && ( 0x02u == m_auStorArea2[4u] ) && ( 0x01u == m_auStorArea2[5u] ) &&
            ( 0xDCu == m_auStorArea2[20u] )  && ( 0x02u == m_auStorArea2[21u] ) && ( 0x00u == m_auStorArea2[22u] ) &&
            ( 0x00u == m_auStorArea2[23u] ) )
        {
            (void)printf("eFSS_DBCTST_LoadBkupTest 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_LoadBkupTest 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_LoadBkupTest 5  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u]  = 0x01u; /* Page SUBTYPE */
    m_auStorArea1[5u]  = 0x00u; /* Page index */
    m_auStorArea1[6u]  = 0x00u; /* Page index */
    m_auStorArea1[7u]  = 0x00u; /* Page index */
    m_auStorArea1[8u]  = 0x00u; /* Page index */
    m_auStorArea1[9u]  = 0x03u; /* Page type */
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
    m_auStorArea2[4u]  = 0x01u;  /* Page SUBTYPE */
    m_auStorArea2[5u]  = 0x01u;  /* Page index */
    m_auStorArea2[6u]  = 0x00u;  /* Page index */
    m_auStorArea2[7u]  = 0x00u;  /* Page index */
    m_auStorArea2[8u]  = 0x00u;  /* Page index */
    m_auStorArea2[9u]  = 0x03u;  /* Page type */
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
    m_auStorArea2[20u] = 0xDBu; /* CRC */
    m_auStorArea2[21u] = 0x02u; /* CRC */
    m_auStorArea2[22u] = 0x00u; /* CRC */
    m_auStorArea2[23u] = 0x00u; /* CRC */

    if( e_eFSS_DBC_RES_OK_BKP_RCVRD == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 0x00u == l_ltUseBuff.puBuf[0u] )  && ( 0x00u == l_ltUseBuff.puBuf[1u] ) && ( 0x00u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[3u] )  && ( 0x01u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x00u == l_ltUseBuff2.puBuf[0u] ) && ( 0x00u == l_ltUseBuff2.puBuf[1u] ) && ( 0x00u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[3u] ) && ( 0x02u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x00u == m_auStorArea1[0u] )  && ( 0x00u == m_auStorArea1[1u] ) && ( 0x00u == m_auStorArea1[2u] ) &&
            ( 0x00u == m_auStorArea1[3u] )  && ( 0x01u == m_auStorArea1[4u] ) && ( 0x00u == m_auStorArea1[5u] ) &&
            ( 0x9Au == m_auStorArea1[20u] )  && ( 0x02u == m_auStorArea1[21u] ) && ( 0x00u == m_auStorArea1[22u] ) &&
            ( 0x00u == m_auStorArea1[23u] ) &&
            ( 0x00u == m_auStorArea2[0u] )  && ( 0x00u == m_auStorArea2[1u] ) && ( 0x00u == m_auStorArea2[2u] ) &&
            ( 0x00u == m_auStorArea2[3u] )  && ( 0x02u == m_auStorArea2[4u] ) && ( 0x01u == m_auStorArea2[5u] ) &&
            ( 0x9Cu == m_auStorArea2[20u] )  && ( 0x02u == m_auStorArea2[21u] ) && ( 0x00u == m_auStorArea2[22u] ) &&
            ( 0x00u == m_auStorArea2[23u] ) )
        {
            (void)printf("eFSS_DBCTST_LoadBkupTest 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_LoadBkupTest 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_LoadBkupTest 6  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u]  = 0x01u; /* Page SUBTYPE */
    m_auStorArea1[5u]  = 0x00u; /* Page index */
    m_auStorArea1[6u]  = 0x00u; /* Page index */
    m_auStorArea1[7u]  = 0x00u; /* Page index */
    m_auStorArea1[8u]  = 0x00u; /* Page index */
    m_auStorArea1[9u]  = 0x03u; /* Page type */
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
    m_auStorArea2[4u]  = 0x02u;  /* Page SUBTYPE */
    m_auStorArea2[5u]  = 0x01u;  /* Page index */
    m_auStorArea2[6u]  = 0x00u;  /* Page index */
    m_auStorArea2[7u]  = 0x00u;  /* Page index */
    m_auStorArea2[8u]  = 0x00u;  /* Page index */
    m_auStorArea2[9u]  = 0x03u;  /* Page type */
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
    m_auStorArea2[20u] = 0xFFu; /* CRC */
    m_auStorArea2[21u] = 0xFFu; /* CRC */
    m_auStorArea2[22u] = 0xFFu; /* CRC */
    m_auStorArea2[23u] = 0xFFu; /* CRC */

    if( e_eFSS_DBC_RES_OK_BKP_RCVRD == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 0x00u == l_ltUseBuff.puBuf[0u] )  && ( 0x00u == l_ltUseBuff.puBuf[1u] ) && ( 0x00u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[3u] )  && ( 0x01u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x00u == l_ltUseBuff2.puBuf[0u] ) && ( 0x00u == l_ltUseBuff2.puBuf[1u] ) && ( 0x00u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[3u] ) && ( 0x02u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x00u == m_auStorArea1[0u] )  && ( 0x00u == m_auStorArea1[1u] ) && ( 0x00u == m_auStorArea1[2u] ) &&
            ( 0x00u == m_auStorArea1[3u] )  && ( 0x01u == m_auStorArea1[4u] ) && ( 0x00u == m_auStorArea1[5u] ) &&
            ( 0x9Au == m_auStorArea1[20u] )  && ( 0x02u == m_auStorArea1[21u] ) && ( 0x00u == m_auStorArea1[22u] ) &&
            ( 0x00u == m_auStorArea1[23u] ) &&
            ( 0x00u == m_auStorArea2[0u] )  && ( 0x00u == m_auStorArea2[1u] ) && ( 0x00u == m_auStorArea2[2u] ) &&
            ( 0x00u == m_auStorArea2[3u] )  && ( 0x02u == m_auStorArea2[4u] ) && ( 0x01u == m_auStorArea2[5u] ) &&
            ( 0x9Cu == m_auStorArea2[20u] )  && ( 0x02u == m_auStorArea2[21u] ) && ( 0x00u == m_auStorArea2[22u] ) &&
            ( 0x00u == m_auStorArea2[23u] ) )
        {
            (void)printf("eFSS_DBCTST_LoadBkupTest 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_LoadBkupTest 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_LoadBkupTest 7  -- FAIL \n");
    }


    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u]  = 0x01u; /* Page SUBTYPE */
    m_auStorArea1[5u]  = 0x00u; /* Page index */
    m_auStorArea1[6u]  = 0x00u; /* Page index */
    m_auStorArea1[7u]  = 0x00u; /* Page index */
    m_auStorArea1[8u]  = 0x00u; /* Page index */
    m_auStorArea1[9u]  = 0x03u; /* Page type */
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
    m_auStorArea2[4u]  = 0x02u;  /* Page SUBTYPE */
    m_auStorArea2[5u]  = 0x01u;  /* Page index */
    m_auStorArea2[6u]  = 0x00u;  /* Page index */
    m_auStorArea2[7u]  = 0x00u;  /* Page index */
    m_auStorArea2[8u]  = 0x00u;  /* Page index */
    m_auStorArea2[9u]  = 0x03u;  /* Page type */
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

    if( e_eFSS_DBC_RES_OK_BKP_RCVRD == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] )  && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] )  && ( 0x01u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x02u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x10u == m_auStorArea1[0u] )  && ( 0x10u == m_auStorArea1[1u] ) && ( 0x10u == m_auStorArea1[2u] ) &&
            ( 0x10u == m_auStorArea1[3u] )  && ( 0x01u == m_auStorArea1[4u] ) && ( 0x00u == m_auStorArea1[5u] ) &&
            ( 0xDAu == m_auStorArea1[20u] )  && ( 0x02u == m_auStorArea1[21u] ) && ( 0x00u == m_auStorArea1[22u] ) &&
            ( 0x00u == m_auStorArea1[23u] ) &&
            ( 0x10u == m_auStorArea2[0u] )  && ( 0x10u == m_auStorArea2[1u] ) && ( 0x10u == m_auStorArea2[2u] ) &&
            ( 0x10u == m_auStorArea2[3u] )  && ( 0x02u == m_auStorArea2[4u] ) && ( 0x01u == m_auStorArea2[5u] ) &&
            ( 0xDCu == m_auStorArea2[20u] )  && ( 0x02u == m_auStorArea2[21u] ) && ( 0x00u == m_auStorArea2[22u] ) &&
            ( 0x00u == m_auStorArea2[23u] ) )
        {
            (void)printf("eFSS_DBCTST_LoadBkupTest 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_LoadBkupTest 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_LoadBkupTest 8  -- FAIL \n");
    }

    /* --------------------------------------------- All OK no backup generation */
    /* Setup storage area */
    (void)memset(m_auStorArea1, 1, sizeof(m_auStorArea1));
    m_auStorArea1[4u]  = 0x01u; /* Page SUBTYPE */
    m_auStorArea1[5u]  = 0x00u; /* Page index */
    m_auStorArea1[6u]  = 0x00u; /* Page index */
    m_auStorArea1[7u]  = 0x00u; /* Page index */
    m_auStorArea1[8u]  = 0x00u; /* Page index */
    m_auStorArea1[9u]  = 0x03u; /* Page type */
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
    m_auStorArea1[20u] = 0x9Fu; /* CRC */
    m_auStorArea1[21u] = 0x02u; /* CRC */
    m_auStorArea1[22u] = 0x00u; /* CRC */
    m_auStorArea1[23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea2, 1, sizeof(m_auStorArea2));
    m_auStorArea2[4u]  = 0x02u; /* Page SUBTYPE */
    m_auStorArea2[5u]  = 0x01u; /* Page index */
    m_auStorArea2[6u]  = 0x00u; /* Page index */
    m_auStorArea2[7u]  = 0x00u; /* Page index */
    m_auStorArea2[8u]  = 0x00u; /* Page index */
    m_auStorArea2[9u]  = 0x03u; /* Page type */
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
    m_auStorArea2[20u] = 0xA1u; /* CRC */
    m_auStorArea2[21u] = 0x02u; /* CRC */
    m_auStorArea2[22u] = 0x00u; /* CRC */
    m_auStorArea2[23u] = 0x00u; /* CRC */

    if( e_eFSS_DBC_RES_NEWVERSIONFOUND == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 0x01u == l_ltUseBuff.puBuf[0u] )  && ( 0x01u == l_ltUseBuff.puBuf[1u] ) && ( 0x01u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x01u == l_ltUseBuff.puBuf[3u] )  && ( 0x01u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x01u == l_ltUseBuff2.puBuf[0u] ) && ( 0x01u == l_ltUseBuff2.puBuf[1u] ) && ( 0x01u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x01u == l_ltUseBuff2.puBuf[3u] ) && ( 0x02u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_DBCTST_LoadBkupTest 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_LoadBkupTest 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_LoadBkupTest 9  -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u]  = 0x01u; /* Page SUBTYPE */
    m_auStorArea1[5u]  = 0x00u; /* Page index */
    m_auStorArea1[6u]  = 0x00u; /* Page index */
    m_auStorArea1[7u]  = 0x00u; /* Page index */
    m_auStorArea1[8u]  = 0x00u; /* Page index */
    m_auStorArea1[9u]  = 0x03u; /* Page type */
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
    m_auStorArea2[4u]  = 0x02u;  /* Page SUBTYPE */
    m_auStorArea2[5u]  = 0x01u;  /* Page index */
    m_auStorArea2[6u]  = 0x00u;  /* Page index */
    m_auStorArea2[7u]  = 0x00u;  /* Page index */
    m_auStorArea2[8u]  = 0x00u;  /* Page index */
    m_auStorArea2[9u]  = 0x03u;  /* Page type */
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

    if( e_eFSS_DBC_RES_OK_BKP_RCVRD == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] )  && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] )  && ( 0x01u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x02u == l_ltUseBuff2.puBuf[4u] ) &&
            ( 0x10u == m_auStorArea1[0u] )  && ( 0x10u == m_auStorArea1[1u] ) && ( 0x10u == m_auStorArea1[2u] ) &&
            ( 0x10u == m_auStorArea1[3u] )  && ( 0x01u == m_auStorArea1[4u] ) && ( 0x00u == m_auStorArea1[5u] ) &&
            ( 0xDAu == m_auStorArea1[20u] )  && ( 0x02u == m_auStorArea1[21u] ) && ( 0x00u == m_auStorArea1[22u] ) &&
            ( 0x00u == m_auStorArea1[23u] ) &&
            ( 0x10u == m_auStorArea2[0u] )  && ( 0x10u == m_auStorArea2[1u] ) && ( 0x10u == m_auStorArea2[2u] ) &&
            ( 0x10u == m_auStorArea2[3u] )  && ( 0x02u == m_auStorArea2[4u] ) && ( 0x01u == m_auStorArea2[5u] ) &&
            ( 0xDCu == m_auStorArea2[20u] )  && ( 0x02u == m_auStorArea2[21u] ) && ( 0x00u == m_auStorArea2[22u] ) &&
            ( 0x00u == m_auStorArea2[23u] ) )
        {
            (void)printf("eFSS_DBCTST_LoadBkupTest 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_LoadBkupTest 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_LoadBkupTest 10 -- FAIL \n");
    }

    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    m_auStorArea1[4u]  = 0x02u; /* Page SUBTYPE */
    m_auStorArea1[5u]  = 0x00u; /* Page index */
    m_auStorArea1[6u]  = 0x00u; /* Page index */
    m_auStorArea1[7u]  = 0x00u; /* Page index */
    m_auStorArea1[8u]  = 0x00u; /* Page index */
    m_auStorArea1[9u]  = 0x03u; /* Page type */
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
    m_auStorArea1[20u] = 0x9Bu; /* CRC */
    m_auStorArea1[21u] = 0x02u; /* CRC */
    m_auStorArea1[22u] = 0x00u; /* CRC */
    m_auStorArea1[23u] = 0x00u; /* CRC */

    (void)memset(m_auStorArea2, 0, sizeof(m_auStorArea2));
    m_auStorArea2[4u]  = 0x01u; /* Page SUBTYPE */
    m_auStorArea2[5u]  = 0x01u; /* Page index */
    m_auStorArea2[6u]  = 0x00u; /* Page index */
    m_auStorArea2[7u]  = 0x00u; /* Page index */
    m_auStorArea2[8u]  = 0x00u; /* Page index */
    m_auStorArea2[9u]  = 0x03u; /* Page type */
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
    m_auStorArea2[20u] = 0x9Bu; /* CRC */
    m_auStorArea2[21u] = 0x02u; /* CRC */
    m_auStorArea2[22u] = 0x00u; /* CRC */
    m_auStorArea2[23u] = 0x00u; /* CRC */

    if( e_eFSS_DBC_RES_NOTVALIDDB == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 0x00u == l_ltUseBuff.puBuf[0u] )  && ( 0x00u == l_ltUseBuff.puBuf[1u] ) && ( 0x00u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff.puBuf[3u] )  && ( 0x02u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x00u == l_ltUseBuff2.puBuf[0u] ) && ( 0x00u == l_ltUseBuff2.puBuf[1u] ) && ( 0x00u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x00u == l_ltUseBuff2.puBuf[3u] ) && ( 0x01u == l_ltUseBuff2.puBuf[4u] ) )
        {
            (void)printf("eFSS_DBCTST_LoadBkupTest 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_LoadBkupTest 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_LoadBkupTest 11 -- FAIL \n");
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

static void eFSS_DBCTST_FlushBkupTest(void)
{
    /* Local variable */
    t_eFSS_DBC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_DBC_StorBuf l_ltUseBuff;
    t_eFSS_DBC_StorBuf l_ltUseBuff2;
    uint32_t l_uTotPage;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_DBCTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_DBCTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_DBCTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcTst1Adapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 2u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_DBCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_DBCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_DBCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_FlushBkupTest 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_FlushBkupTest 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_FlushBkupTest 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_FlushBkupTest 2  -- FAIL \n");
    }

    /* Setup storage area */
    l_ltUseBuff2.uBufL = l_ltUseBuff.uBufL  + 1u;
    l_ltUseBuff2.puBuf = &l_auStor[24];


    /* Setup storage area */
    (void)memset(m_auStorArea1, 0, sizeof(m_auStorArea1));
    (void)memset(m_auStorArea2, 0, sizeof(m_auStorArea2));
    (void)memset(l_ltUseBuff.puBuf, 0x10, l_ltUseBuff.uBufL);


    if( e_eFSS_DBC_RES_OK == eFSS_DBC_FlushBuffInPage(&l_tCtx, 0u) )
    {
        if( ( 0x10u == l_ltUseBuff.puBuf[0u] )  && ( 0x10u == l_ltUseBuff.puBuf[1u] ) && ( 0x10u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff.puBuf[3u] )  && ( 0x01u == l_ltUseBuff.puBuf[4u] )  &&
            ( 0x10u == l_ltUseBuff2.puBuf[0u] ) && ( 0x10u == l_ltUseBuff2.puBuf[1u] ) && ( 0x10u == l_ltUseBuff2.puBuf[2u] ) &&
            ( 0x10u == l_ltUseBuff2.puBuf[3u] ) && ( 0x02u == l_ltUseBuff2.puBuf[4u] ) )
        {
            if( ( m_auStorArea1[0u]  == 0x10u ) && ( m_auStorArea1[1u]  == 0x10u ) && ( m_auStorArea1[2u]  == 0x10u ) &&
                ( m_auStorArea1[3u]  == 0x10u ) && ( m_auStorArea1[4u]  == 0x01u ) && ( m_auStorArea1[5u]  == 0x00u ) &&
                ( m_auStorArea1[6u]  == 0x00u ) && ( m_auStorArea1[7u]  == 0x00u ) && ( m_auStorArea1[8u]  == 0x00u ) &&
                ( m_auStorArea1[9u]  == 0x03u ) && ( m_auStorArea1[10u] == 0x01u ) && ( m_auStorArea1[11u] == 0x00u ) &&
                ( m_auStorArea1[12u] == 0x02u ) && ( m_auStorArea1[13u] == 0x00u ) && ( m_auStorArea1[14u] == 0x00u ) &&
                ( m_auStorArea1[15u] == 0x00u ) && ( m_auStorArea1[16u] == 0xA5u ) && ( m_auStorArea1[17u] == 0xA5u ) &&
                ( m_auStorArea1[18u] == 0xA5u ) && ( m_auStorArea1[19u] == 0xA5u ) && ( m_auStorArea1[20u] == 0xDAu ) &&
                ( m_auStorArea1[21u] == 0x02u ) && ( m_auStorArea1[22u] == 0x00u ) && ( m_auStorArea1[23u] == 0x00u ) &&
                ( m_auStorArea2[0u]  == 0x10u ) && ( m_auStorArea2[1u]  == 0x10u ) && ( m_auStorArea2[2u]  == 0x10u ) &&
                ( m_auStorArea2[3u]  == 0x10u ) && ( m_auStorArea2[4u]  == 0x02u ) && ( m_auStorArea2[5u]  == 0x01u ) &&
                ( m_auStorArea2[6u]  == 0x00u ) && ( m_auStorArea2[7u]  == 0x00u ) && ( m_auStorArea2[8u]  == 0x00u ) &&
                ( m_auStorArea2[9u]  == 0x03u ) && ( m_auStorArea2[10u] == 0x01u ) && ( m_auStorArea2[11u] == 0x00u ) &&
                ( m_auStorArea2[12u] == 0x02u ) && ( m_auStorArea2[13u] == 0x00u ) && ( m_auStorArea2[14u] == 0x00u ) &&
                ( m_auStorArea2[15u] == 0x00u ) && ( m_auStorArea2[16u] == 0xA5u ) && ( m_auStorArea2[17u] == 0xA5u ) &&
                ( m_auStorArea2[18u] == 0xA5u ) && ( m_auStorArea2[19u] == 0xA5u ) && ( m_auStorArea2[20u] == 0xDCu ) &&
                ( m_auStorArea2[21u] == 0x02u ) && ( m_auStorArea2[22u] == 0x00u ) && ( m_auStorArea2[23u] == 0x00u ) )
              {
                  (void)printf("eFSS_DBCTST_FlushBkupTest 3  -- OK \n");
              }
              else
              {
                  (void)printf("eFSS_DBCTST_FlushBkupTest 3  -- FAIL \n");
              }
        }
        else
        {
            (void)printf("eFSS_DBCTST_FlushBkupTest 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_FlushBkupTest 3  -- FAIL \n");
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

static void eFSS_DBCTST_GenTest(void)
{
    /* Local variable */
    t_eFSS_DBC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_DBC_StorBuf l_ltUseBuff;
    uint32_t l_uTotPage;

    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_DBCTST_EraseTstAAdapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_DBCTST_WriteTstAAdapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_DBCTST_ReadTstAAdapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcTstAAdapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 6u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;
    l_uTotPage = 0u;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_DBCTST_EraseTstAAdapt;
    l_tCtxCb.fWrite = &eFSS_DBCTST_WriteTstAAdapt;
    l_tCtxCb.fRead = &eFSS_DBCTST_ReadTstAAdapt;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcTstAAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;

    /* Setup storage area */
    (void)memset(m_auStorAreaA, 0, sizeof(m_auStorAreaA));

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_GenTest 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_GenTest 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 2  -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_NOTVALIDDB == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 3  -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_NOTVALIDDB == eFSS_DBC_LoadPageInBuff(&l_tCtx, 1u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 4  -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_NOTVALIDDB == eFSS_DBC_LoadPageInBuff(&l_tCtx, 2u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 5  -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_LoadPageInBuff(&l_tCtx, 3u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 6  -- FAIL \n");
    }

    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;
    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_FlushBuffInPage(&l_tCtx, 3u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 7  -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_NOTVALIDDB == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 8  -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_NOTVALIDDB == eFSS_DBC_LoadPageInBuff(&l_tCtx, 1u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 8  -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_NOTVALIDDB == eFSS_DBC_LoadPageInBuff(&l_tCtx, 2u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 9  -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_LoadPageInBuff(&l_tCtx, 3u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 10 -- FAIL \n");
    }

    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_FlushBuffInPage(&l_tCtx, 2u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 11 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_NOTVALIDDB == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 12 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_NOTVALIDDB == eFSS_DBC_LoadPageInBuff(&l_tCtx, 1u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 13 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 2u) )
    {
        if( ( 0x01u == l_ltUseBuff.puBuf[0u] ) && ( 0x02u == l_ltUseBuff.puBuf[1u] ) && ( 0x03u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x04u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 14 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_LoadPageInBuff(&l_tCtx, 3u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 15 -- FAIL \n");
    }

    l_ltUseBuff.puBuf[0u] = 0x05u;
    l_ltUseBuff.puBuf[1u] = 0x06u;
    l_ltUseBuff.puBuf[2u] = 0x07u;
    l_ltUseBuff.puBuf[3u] = 0x08u;
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_FlushBuffInPage(&l_tCtx, 1u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 16 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_NOTVALIDDB == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 17 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 1u) )
    {
        if( ( 0x05u == l_ltUseBuff.puBuf[0u] ) && ( 0x06u == l_ltUseBuff.puBuf[1u] ) && ( 0x07u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x08u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 18 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 18 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 2u) )
    {
        if( ( 0x01u == l_ltUseBuff.puBuf[0u] ) && ( 0x02u == l_ltUseBuff.puBuf[1u] ) && ( 0x03u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x04u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 19 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_LoadPageInBuff(&l_tCtx, 3u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 20 -- FAIL \n");
    }

    l_ltUseBuff.puBuf[0u] = 0x09u;
    l_ltUseBuff.puBuf[1u] = 0x0Au;
    l_ltUseBuff.puBuf[2u] = 0x0Bu;
    l_ltUseBuff.puBuf[3u] = 0x0Cu;
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_FlushBuffInPage(&l_tCtx, 0u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 21 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 0x09u == l_ltUseBuff.puBuf[0u] ) && ( 0x0Au == l_ltUseBuff.puBuf[1u] ) && ( 0x0Bu == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x0Cu == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 22 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 1u) )
    {
        if( ( 0x05u == l_ltUseBuff.puBuf[0u] ) && ( 0x06u == l_ltUseBuff.puBuf[1u] ) && ( 0x07u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x08u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 23 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 2u) )
    {
        if( ( 0x01u == l_ltUseBuff.puBuf[0u] ) && ( 0x02u == l_ltUseBuff.puBuf[1u] ) && ( 0x03u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x04u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 24 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 24 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 24 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_LoadPageInBuff(&l_tCtx, 3u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 25 -- FAIL \n");
    }

    l_ltUseBuff.puBuf[0u] = 0xF9u;
    l_ltUseBuff.puBuf[1u] = 0xFAu;
    l_ltUseBuff.puBuf[2u] = 0xFBu;
    l_ltUseBuff.puBuf[3u] = 0xFCu;
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_FlushBuffInPage(&l_tCtx, 0u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 26 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 0xF9u == l_ltUseBuff.puBuf[0u] ) && ( 0xFAu == l_ltUseBuff.puBuf[1u] ) && ( 0xFBu == l_ltUseBuff.puBuf[2u] ) &&
            ( 0xFCu == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 27 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 27 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 27 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 1u) )
    {
        if( ( 0x05u == l_ltUseBuff.puBuf[0u] ) && ( 0x06u == l_ltUseBuff.puBuf[1u] ) && ( 0x07u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x08u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 28 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 28 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 28 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 2u) )
    {
        if( ( 0x01u == l_ltUseBuff.puBuf[0u] ) && ( 0x02u == l_ltUseBuff.puBuf[1u] ) && ( 0x03u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x04u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 29 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 29 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 29 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_LoadPageInBuff(&l_tCtx, 3u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 30 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 30 -- FAIL \n");
    }


    l_ltUseBuff.puBuf[0u] = 0x09u;
    l_ltUseBuff.puBuf[1u] = 0x0Au;
    l_ltUseBuff.puBuf[2u] = 0x0Bu;
    l_ltUseBuff.puBuf[3u] = 0x0Cu;
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_FlushBuffInPage(&l_tCtx, 0u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 31 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 31 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 0x09u == l_ltUseBuff.puBuf[0u] ) && ( 0x0Au == l_ltUseBuff.puBuf[1u] ) && ( 0x0Bu == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x0Cu == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 32 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 32 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 32 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 1u) )
    {
        if( ( 0x05u == l_ltUseBuff.puBuf[0u] ) && ( 0x06u == l_ltUseBuff.puBuf[1u] ) && ( 0x07u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x08u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 33 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 33 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 33 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 2u) )
    {
        if( ( 0x01u == l_ltUseBuff.puBuf[0u] ) && ( 0x02u == l_ltUseBuff.puBuf[1u] ) && ( 0x03u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x04u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 34 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 34 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 34 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_LoadPageInBuff(&l_tCtx, 3u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 35 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 35 -- FAIL \n");
    }

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    l_tStorSet.uPageVersion = 2u;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_GenTest 36 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 36 -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_GenTest 37 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 37 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_NEWVERSIONFOUND == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 38 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 38 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_NEWVERSIONFOUND == eFSS_DBC_LoadPageInBuff(&l_tCtx, 1u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 39 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 39 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_NEWVERSIONFOUND == eFSS_DBC_LoadPageInBuff(&l_tCtx, 2u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 40 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 40 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_LoadPageInBuff(&l_tCtx, 3u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 41 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 41 -- FAIL \n");
    }

    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;
    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_FlushBuffInPage(&l_tCtx, 3u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 42 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 42 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_NEWVERSIONFOUND == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 43 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 43 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_NEWVERSIONFOUND == eFSS_DBC_LoadPageInBuff(&l_tCtx, 1u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 44 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 44 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_NEWVERSIONFOUND == eFSS_DBC_LoadPageInBuff(&l_tCtx, 2u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 45 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 45 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_LoadPageInBuff(&l_tCtx, 3u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 46 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 46 -- FAIL \n");
    }

    l_ltUseBuff.puBuf[0u] = 0x01u;
    l_ltUseBuff.puBuf[1u] = 0x02u;
    l_ltUseBuff.puBuf[2u] = 0x03u;
    l_ltUseBuff.puBuf[3u] = 0x04u;
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_FlushBuffInPage(&l_tCtx, 2u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 47 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 47 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_NEWVERSIONFOUND == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 48 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 48 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_NEWVERSIONFOUND == eFSS_DBC_LoadPageInBuff(&l_tCtx, 1u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 49 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 49 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 2u) )
    {
        if( ( 0x01u == l_ltUseBuff.puBuf[0u] ) && ( 0x02u == l_ltUseBuff.puBuf[1u] ) && ( 0x03u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x04u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 50 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 50 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 50 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_LoadPageInBuff(&l_tCtx, 3u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 51 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 51 -- FAIL \n");
    }

    l_ltUseBuff.puBuf[0u] = 0x05u;
    l_ltUseBuff.puBuf[1u] = 0x06u;
    l_ltUseBuff.puBuf[2u] = 0x07u;
    l_ltUseBuff.puBuf[3u] = 0x08u;
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_FlushBuffInPage(&l_tCtx, 1u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 52 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 52 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_NEWVERSIONFOUND == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 53 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 53 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 1u) )
    {
        if( ( 0x05u == l_ltUseBuff.puBuf[0u] ) && ( 0x06u == l_ltUseBuff.puBuf[1u] ) && ( 0x07u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x08u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 54 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 54 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 54 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 2u) )
    {
        if( ( 0x01u == l_ltUseBuff.puBuf[0u] ) && ( 0x02u == l_ltUseBuff.puBuf[1u] ) && ( 0x03u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x04u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 55 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 55 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 55 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_LoadPageInBuff(&l_tCtx, 3u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 56 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 56 -- FAIL \n");
    }

    l_ltUseBuff.puBuf[0u] = 0x09u;
    l_ltUseBuff.puBuf[1u] = 0x0Au;
    l_ltUseBuff.puBuf[2u] = 0x0Bu;
    l_ltUseBuff.puBuf[3u] = 0x0Cu;
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_FlushBuffInPage(&l_tCtx, 0u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 57 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 57 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 0x09u == l_ltUseBuff.puBuf[0u] ) && ( 0x0Au == l_ltUseBuff.puBuf[1u] ) && ( 0x0Bu == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x0Cu == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 58 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 58 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 58 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 1u) )
    {
        if( ( 0x05u == l_ltUseBuff.puBuf[0u] ) && ( 0x06u == l_ltUseBuff.puBuf[1u] ) && ( 0x07u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x08u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 59 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 59 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 59 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 2u) )
    {
        if( ( 0x01u == l_ltUseBuff.puBuf[0u] ) && ( 0x02u == l_ltUseBuff.puBuf[1u] ) && ( 0x03u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x04u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 60 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 60 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 60 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_LoadPageInBuff(&l_tCtx, 3u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 61 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 61 -- FAIL \n");
    }

    l_ltUseBuff.puBuf[0u] = 0xF9u;
    l_ltUseBuff.puBuf[1u] = 0xFAu;
    l_ltUseBuff.puBuf[2u] = 0xFBu;
    l_ltUseBuff.puBuf[3u] = 0xFCu;
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_FlushBuffInPage(&l_tCtx, 0u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 62 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 62 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 0xF9u == l_ltUseBuff.puBuf[0u] ) && ( 0xFAu == l_ltUseBuff.puBuf[1u] ) && ( 0xFBu == l_ltUseBuff.puBuf[2u] ) &&
            ( 0xFCu == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 63 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 63 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 63 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 1u) )
    {
        if( ( 0x05u == l_ltUseBuff.puBuf[0u] ) && ( 0x06u == l_ltUseBuff.puBuf[1u] ) && ( 0x07u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x08u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 64 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 64 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 64 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 2u) )
    {
        if( ( 0x01u == l_ltUseBuff.puBuf[0u] ) && ( 0x02u == l_ltUseBuff.puBuf[1u] ) && ( 0x03u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x04u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 65 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 65 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 65 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_LoadPageInBuff(&l_tCtx, 3u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 66 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 66 -- FAIL \n");
    }

    l_ltUseBuff.puBuf[0u] = 0x09u;
    l_ltUseBuff.puBuf[1u] = 0x0Au;
    l_ltUseBuff.puBuf[2u] = 0x0Bu;
    l_ltUseBuff.puBuf[3u] = 0x0Cu;
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_FlushBuffInPage(&l_tCtx, 0u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 67 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 67 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 0x09u == l_ltUseBuff.puBuf[0u] ) && ( 0x0Au == l_ltUseBuff.puBuf[1u] ) && ( 0x0Bu == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x0Cu == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 68 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 68 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 68 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 1u) )
    {
        if( ( 0x05u == l_ltUseBuff.puBuf[0u] ) && ( 0x06u == l_ltUseBuff.puBuf[1u] ) && ( 0x07u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x08u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 69 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 69 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 69 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 2u) )
    {
        if( ( 0x01u == l_ltUseBuff.puBuf[0u] ) && ( 0x02u == l_ltUseBuff.puBuf[1u] ) && ( 0x03u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x04u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 70 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 70 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 70 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_LoadPageInBuff(&l_tCtx, 3u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 71 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 71 -- FAIL \n");
    }

    /* Check recover */
    (void)memset(&m_auStorAreaA[3u], 0, 24u);
    (void)memset(&m_auStorAreaA[4u], 0, 24u);
    (void)memset(&m_auStorAreaA[5u], 0, 24u);


    if( e_eFSS_DBC_RES_OK_BKP_RCVRD == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 0x09u == l_ltUseBuff.puBuf[0u] ) && ( 0x0Au == l_ltUseBuff.puBuf[1u] ) && ( 0x0Bu == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x0Cu == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 72 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 72 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 72 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK_BKP_RCVRD == eFSS_DBC_LoadPageInBuff(&l_tCtx, 1u) )
    {
        if( ( 0x05u == l_ltUseBuff.puBuf[0u] ) && ( 0x06u == l_ltUseBuff.puBuf[1u] ) && ( 0x07u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x08u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 73 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 73 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 73 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK_BKP_RCVRD == eFSS_DBC_LoadPageInBuff(&l_tCtx, 2u) )
    {
        if( ( 0x01u == l_ltUseBuff.puBuf[0u] ) && ( 0x02u == l_ltUseBuff.puBuf[1u] ) && ( 0x03u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x04u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 74 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 74 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 74 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_LoadPageInBuff(&l_tCtx, 3u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 75 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 75 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 0x09u == l_ltUseBuff.puBuf[0u] ) && ( 0x0Au == l_ltUseBuff.puBuf[1u] ) && ( 0x0Bu == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x0Cu == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 76 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 76 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 76 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 1u) )
    {
        if( ( 0x05u == l_ltUseBuff.puBuf[0u] ) && ( 0x06u == l_ltUseBuff.puBuf[1u] ) && ( 0x07u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x08u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 77 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 77 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 77 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 2u) )
    {
        if( ( 0x01u == l_ltUseBuff.puBuf[0u] ) && ( 0x02u == l_ltUseBuff.puBuf[1u] ) && ( 0x03u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x04u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 78 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 78 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 78 -- FAIL \n");
    }

    /* Check recover */
    (void)memset(&m_auStorAreaA[0u], 0, 24u);
    (void)memset(&m_auStorAreaA[1u], 0, 24u);
    (void)memset(&m_auStorAreaA[2u], 0, 24u);


    if( e_eFSS_DBC_RES_OK_BKP_RCVRD == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 0x09u == l_ltUseBuff.puBuf[0u] ) && ( 0x0Au == l_ltUseBuff.puBuf[1u] ) && ( 0x0Bu == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x0Cu == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 79 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 79 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 79 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK_BKP_RCVRD == eFSS_DBC_LoadPageInBuff(&l_tCtx, 1u) )
    {
        if( ( 0x05u == l_ltUseBuff.puBuf[0u] ) && ( 0x06u == l_ltUseBuff.puBuf[1u] ) && ( 0x07u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x08u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 80 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 80 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 80 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK_BKP_RCVRD == eFSS_DBC_LoadPageInBuff(&l_tCtx, 2u) )
    {
        if( ( 0x01u == l_ltUseBuff.puBuf[0u] ) && ( 0x02u == l_ltUseBuff.puBuf[1u] ) && ( 0x03u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x04u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 81 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 81 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 81 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_BADPARAM == eFSS_DBC_LoadPageInBuff(&l_tCtx, 3u) )
    {
        (void)printf("eFSS_DBCTST_GenTest 82 -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 82 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 0x09u == l_ltUseBuff.puBuf[0u] ) && ( 0x0Au == l_ltUseBuff.puBuf[1u] ) && ( 0x0Bu == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x0Cu == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 83 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 83 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 83 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 1u) )
    {
        if( ( 0x05u == l_ltUseBuff.puBuf[0u] ) && ( 0x06u == l_ltUseBuff.puBuf[1u] ) && ( 0x07u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x08u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 84 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 84 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 84 -- FAIL \n");
    }

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_LoadPageInBuff(&l_tCtx, 2u) )
    {
        if( ( 0x01u == l_ltUseBuff.puBuf[0u] ) && ( 0x02u == l_ltUseBuff.puBuf[1u] ) && ( 0x03u == l_ltUseBuff.puBuf[2u] ) &&
            ( 0x04u == l_ltUseBuff.puBuf[3u] )  )
        {
            (void)printf("eFSS_DBCTST_GenTest 85 -- OK \n");
        }
        else
        {
            (void)printf("eFSS_DBCTST_GenTest 85 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_GenTest 85 -- FAIL \n");
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

static void eFSS_DBCTST_CodeCoverage(void)
{
    /* Local variable */
    t_eFSS_DBC_Ctx l_tCtx;
    t_eFSS_TYPE_CbStorCtx l_tCtxCb;
    t_eFSS_TYPE_StorSet l_tStorSet;
    uint8_t l_auStor[48u];
    t_eFSS_TYPE_EraseCtx  l_tCtxErase;
	t_eFSS_TYPE_WriteCtx  l_tCtxWrite;
	t_eFSS_TYPE_ReadCtx   l_tCtxRead;
	t_eFSS_TYPE_CrcCtx    l_tCtxCrc32;
    t_eFSS_DBC_StorBuf l_ltUseBuff;
    t_eFSS_DBC_StorBuf l_ltUseBuff2;
    uint32_t l_uTotPage;


    /* Init callback var */
    l_tCtxCb.ptCtxErase = &l_tCtxErase;
    l_tCtxCb.fErase = &eFSS_DBCTST_EraseTst1Adapt;
	l_tCtxCb.ptCtxWrite = &l_tCtxWrite;
    l_tCtxCb.fWrite = &eFSS_DBCTST_WriteTst1Adapt;
	l_tCtxCb.ptCtxRead = &l_tCtxRead;
    l_tCtxCb.fRead = &eFSS_DBCTST_ReadTst1Adapt;
	l_tCtxCb.ptCtxCrc32 = &l_tCtxCrc32;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcTst1Adapt;

    /* Init storage settings */
    l_tStorSet.uTotPages = 2u;
    l_tStorSet.uPagesLen = 24u;
    l_tStorSet.uRWERetry = 3u;
    l_tStorSet.uPageVersion = 1u;

    /* ------------------------------------------------------------------------------------------- TEST CRC CALL BACK */
    /* Function */
    l_tCtxCb.fErase = &eFSS_DBCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_DBCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_DBCTST_ReadTst1Adapt;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;

    /* Function */
    l_tCtxCb.fErase = &eFSS_DBCTST_EraseTst1Adapt;
    l_tCtxCb.fWrite = &eFSS_DBCTST_WriteTst1Adapt;
    l_tCtxCb.fRead = &eFSS_DBCTST_ReadTst2Adapt;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcTst1Adapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;

    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CodeCoverage 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CodeCoverage 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CodeCoverage 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CodeCoverage 2  -- FAIL \n");
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

    if( e_eFSS_DBC_RES_WRITENOMATCHREAD == eFSS_DBC_FlushBuffInPage(&l_tCtx, 0u) )
    {
        (void)printf("eFSS_DBCTST_CodeCoverage 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CodeCoverage 3  -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fRead = &eFSS_DBCTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcErrAdapt;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CodeCoverage 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CodeCoverage 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CodeCoverage 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CodeCoverage 5  -- FAIL \n");
    }

    /* Test LOAD */
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;

    if( e_eFSS_DBC_RES_CLBCKCRCERR == eFSS_DBC_LoadPageInBuff(&l_tCtx, 0u) )
    {
        if( ( 1u == l_tCtxRead.uTimeUsed ) && ( e_eFSS_DBC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_DBC_RES_BADPOINTER == l_tCtxCrc32.eLastEr ) )
            {
                (void)printf("eFSS_DBCTST_CodeCoverage 6  -- OK \n");
            }
            else
            {
                (void)printf("eFSS_DBCTST_CodeCoverage 6  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_DBCTST_CodeCoverage 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_CodeCoverage 6  -- FAIL \n");
    }

    /* Function */
    l_tCtxCb.fErase = &eFSS_DBCTST_EraseErrAdapt;
    l_tCtxCb.fWrite = &eFSS_DBCTST_WriteAdapt;
    l_tCtxCb.fRead = &eFSS_DBCTST_ReadAdapt;
    l_tCtxCb.fCrc32 = &eFSS_DBCTST_CrcAdapt;

    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_InitCtx(&l_tCtx, l_tCtxCb, l_tStorSet, l_auStor, sizeof(l_auStor) ) )
    {
        (void)printf("eFSS_DBCTST_CodeCoverage 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CodeCoverage 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSS_DBC_RES_OK == eFSS_DBC_GetBuffNUsable(&l_tCtx, &l_ltUseBuff, &l_uTotPage) )
    {
        (void)printf("eFSS_DBCTST_CodeCoverage 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSS_DBCTST_CodeCoverage 8  -- FAIL \n");
    }

    /* Test LOAD */
    l_tCtxErase.uTimeUsed = 0u;
    l_tCtxErase.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxWrite.uTimeUsed = 0u;
    l_tCtxWrite.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxRead.uTimeUsed = 0u;
    l_tCtxRead.eLastEr = e_eFSS_DBC_RES_OK;
    l_tCtxCrc32.uTimeUsed = 0u;
    l_tCtxCrc32.eLastEr = e_eFSS_DBC_RES_OK;

    if( e_eFSS_DBC_RES_CLBCKERASEERR == eFSS_DBC_FlushBuffInPage(&l_tCtx, 0u) )
    {
        if( ( 1u == l_tCtxCrc32.uTimeUsed ) && ( e_eFSS_DBC_RES_OK == l_tCtxCrc32.eLastEr ) &&
            ( 0u == l_tCtxWrite.uTimeUsed ) && ( e_eFSS_DBC_RES_OK == l_tCtxWrite.eLastEr ) &&
            ( 0u == l_tCtxRead.uTimeUsed )  && ( e_eFSS_DBC_RES_OK == l_tCtxRead.eLastEr ) )
        {
            if( ( 3u == l_tCtxErase.uTimeUsed ) && ( e_eFSS_DBC_RES_BADPOINTER == l_tCtxErase.eLastEr ) )
            {
                (void)printf("eFSS_DBCTST_CodeCoverage 9  -- OK \n");
            }
            else
            {
                (void)printf("eFSS_DBCTST_CodeCoverage 9  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSS_DBCTST_CodeCoverage 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSS_DBCTST_CodeCoverage 9  -- FAIL \n");
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
