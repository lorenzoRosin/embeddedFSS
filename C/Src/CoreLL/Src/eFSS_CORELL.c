/**
 * @file       eFSS_CORELL.c
 *
 * @brief      Low level core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/* The written page in storage will have this struct:
 * ------------------------------------------------------------------ User data
 * - [uint8_t] -                    -> N byte of user data           |
 * ------------------------------------------------------------------ Metadata  (17 byte)
 * - uint32_t  - uPageUseSpec1      -> User specific data            |
 * - uint32_t  - uPageUseSpec2      -> User specific data            |
 * - uint32_t  - uPageUseSpec3      -> User specific data            |
 * - uint32_t  - uPageUseSpec4      -> User specific data            |
 * - uint8_t   - uPageSubType       -> Page subtype                  |
 * ------------------------------------------------------------------ Private metadata (15 byte)
 * - uint8_t   - uPageType          -> Page type                     |
 * - uint16_t  - uPageVersion       -> Page version                  |
 * - uint32_t  - uPageTot           -> Total number of page used     |
 * - uint32_t  - uPageMagicNumber   -> Magic number of the page      |
 * - uint32_t  - uPageCrc           -> 32 bit CRC                    |
 * ------------------------------------------------------------------ End of Page
 */

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_CORELL.h"
#include "eFSS_CORELLPRV.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_CORELL_IsStatusStillCoherent(const t_eFSS_CORELL_Ctx* p_ptCtx);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_CORELL_RES eFSS_CORELL_InitCtx(t_eFSS_CORELL_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx const p_tCtxCb,
									  t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL)
{
    /* Return local var */
    e_eFSS_CORELL_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_CORELL_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_tCtxCb.ptCtxErase ) || ( NULL == p_tCtxCb.fErase ) ||
            ( NULL == p_tCtxCb.ptCtxWrite ) || ( NULL == p_tCtxCb.fWrite ) ||
            ( NULL == p_tCtxCb.ptCtxRead  ) || ( NULL == p_tCtxCb.fRead  ) ||
            ( NULL == p_tCtxCb.ptCtxCrc32 ) || ( NULL == p_tCtxCb.fCrc32 ) )
        {
            l_eRes = e_eFSS_CORELL_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( p_uBuffL != ( 2u * p_tStorSet.uPagesLen ) )
            {
                l_eRes = e_eFSS_CORELL_RES_BADPARAM;
            }
            else
            {
                /* Check data validity */
                if( p_tStorSet.uTotPages <= 0u )
                {
                    l_eRes = e_eFSS_CORELL_RES_BADPARAM;
                }
                else
                {
                    /* Check data validity */
                    if( p_tStorSet.uPagesLen <= EFSS_PAGEMETALLSIZE )
                    {
                        l_eRes = e_eFSS_CORELL_RES_BADPARAM;
                    }
                    else
                    {
                        /* Check data validity */
                        if( p_tStorSet.uRWERetry <= 0u )
                        {
                            l_eRes = e_eFSS_CORELL_RES_BADPARAM;
                        }
                        else
                        {
                            /* Fill context */
                            p_ptCtx->bIsInit 	= true;
                            p_ptCtx->tCtxCb 	= p_tCtxCb;
                            p_ptCtx->tStorSett 	= p_tStorSet;

                            p_ptCtx->tBuff1.puBuf = p_puBuff;
                            p_ptCtx->tBuff1.uBufL = p_uBuffL / 2u;
                            memset(&p_ptCtx->tBuff1.tMeta, 0u, sizeof(t_eFSS_TYPE_PageMeta));

                            p_ptCtx->tBuff2.puBuf = &p_puBuff[p_ptCtx->tBuff1.uBufL];
                            p_ptCtx->tBuff2.uBufL = p_ptCtx->tBuff1.uBufL;
                            memset(&p_ptCtx->tBuff2.tMeta, 0u, sizeof(t_eFSS_TYPE_PageMeta));

                            l_eRes = e_eFSS_CORELL_RES_OK;
                        }
                    }
                }
            }
        }
    }

    return l_eRes;
}

e_eFSS_CORELL_RES eFSS_CORELL_IsInit(t_eFSS_CORELL_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
    /* Return local var */
	e_eFSS_CORELL_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSS_CORELL_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInit;
        l_eRes = e_eFSS_CORELL_RES_OK;
	}

	return l_eRes;
}

e_eFSS_CORELL_RES eFSS_CORELL_GetStorSett(t_eFSS_CORELL_Ctx* const p_ptCtx, t_eFSS_TYPE_StorSet* p_ptStorSet)
{
	/* Return local var */
	e_eFSS_CORELL_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ptStorSet ) )
	{
		l_eRes = e_eFSS_CORELL_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eFSS_CORELL_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eFSS_CORELL_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
            }
            else
            {
                /* Copy data */
                *p_ptStorSet = p_ptCtx->tStorSett;
                l_eRes = e_eFSS_CORELL_RES_OK;
            }
		}
	}

	return l_eRes;
}

e_eFSS_CORELL_RES eFSS_CORELL_GetBuff(t_eFSS_CORELL_Ctx* const p_ptCtx, t_eFSS_TYPE_StorBuf* p_ptBuff1,
                                      t_eFSS_TYPE_StorBuf* p_ptBuff2)
{
	/* Return local var */
	e_eFSS_CORELL_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ptBuff1 ) || ( NULL == p_ptBuff2 ) )
	{
		l_eRes = e_eFSS_CORELL_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eFSS_CORELL_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eFSS_CORELL_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
            }
            else
            {
                p_ptBuff1->puBuf = p_ptCtx->tBuff1.puBuf;
                p_ptBuff1->uBufL = p_ptCtx->tBuff1.uBufL - EFSS_PAGEMETALLSIZE;
                p_ptBuff1->ptMeta = &p_ptCtx->tBuff1.tMeta;

                p_ptBuff2->puBuf = p_ptCtx->tBuff2.puBuf;
                p_ptBuff2->uBufL = p_ptCtx->tBuff2.uBufL - EFSS_PAGEMETALLSIZE;
                p_ptBuff2->ptMeta = &p_ptCtx->tBuff2.tMeta;

                l_eRes = e_eFSS_CORELL_RES_OK;
            }
		}
	}

	return l_eRes;
}

e_eFSS_CORELL_RES eFSS_CORELL_LoadPageInBuff(t_eFSS_CORELL_Ctx* const p_ptCtx, e_eFSS_TYPE_BUFFTYPE p_eBuffType,
								             const uint32_t p_uPageIndx)
{
	/* Return local var */
	e_eFSS_CORELL_RES l_eRes;
    bool_t l_bCbRes;

    /* Local var used for storage  */
    t_eFSS_TYPE_StorBufPrv* l_ptStorBuff;
    t_eFSS_CORELL_privMeta l_tPrvMeta;

    /* Local var used for calculation */
    uint32_t l_uTryPerformed;
    uint32_t l_uPageCrcCalc;
    uint32_t l_uStartIndex;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_CORELL_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eFSS_CORELL_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eFSS_CORELL_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
            }
            else
            {
                /* Check param validity */
                if( p_uPageIndx >= p_ptCtx->tStorSett.uTotPages )
                {
                    l_eRes = e_eFSS_CORELL_RES_BADPARAM;
                }
                else
                {
                    switch(p_eBuffType)
                    {
                        case e_eFSS_TYPE_BUFFTYPE_1:
                        {
                            l_ptStorBuff = &p_ptCtx->tBuff1;
                            l_eRes = e_eFSS_CORELL_RES_OK;
                            break;
                        }

                        case e_eFSS_TYPE_BUFFTYPE_2:
                        {
                            l_ptStorBuff = &p_ptCtx->tBuff2;
                            l_eRes = e_eFSS_CORELL_RES_OK;
                            break;
                        }

                        default:
                        {
                            l_eRes = e_eFSS_CORELL_RES_BADPARAM;
                            break;
                        }
                    }

                    /* Check validity */
                    if( e_eFSS_CORELL_RES_OK == l_eRes )
                    {
                        /* Init var */
                        l_eRes = e_eFSS_CORELL_RES_CLBCKREADERR;
                        l_uTryPerformed = 0u;
                        l_bCbRes = false;

                        while( ( e_eFSS_CORELL_RES_OK != l_eRes ) &&
                               ( l_uTryPerformed < p_ptCtx->tStorSett.uRWERetry ) )
                        {
                            /* Read */
                            l_bCbRes = (*(p_ptCtx->tCtxCb.fRead))(p_ptCtx->tCtxCb.ptCtxRead, p_uPageIndx,
                                                                  l_ptStorBuff->puBuf, l_ptStorBuff->uBufL);
                            l_uTryPerformed++;

                            if( false == l_bCbRes )
                            {
                                l_eRes = e_eFSS_CORELL_RES_CLBCKREADERR;
                            }
                            else
                            {
                                l_eRes = e_eFSS_CORELL_RES_OK;
                            }
                        }

                        if( e_eFSS_CORELL_RES_OK == l_eRes )
                        {
                            /* Extract data */
                            l_uStartIndex = l_ptStorBuff->uBufL - EFSS_PAGEMETALLSIZE;
                            eFSS_CORELLPRV_ExtractData(&l_ptStorBuff->puBuf[l_uStartIndex], &l_tPrvMeta,
                                                       &l_ptStorBuff->tMeta);

                            /* Calculate and check CRC and others parameter */
                            /* Calculate CRC */
                            l_bCbRes = (*(p_ptCtx->tCtxCb.fCrc32))(p_ptCtx->tCtxCb.ptCtxCrc32, MAX_UINT32VAL,
                                                                  l_ptStorBuff->puBuf,( l_ptStorBuff->uBufL - 4u ),
                                                                  &l_uPageCrcCalc );

                            if( true != l_bCbRes )
                            {
                                l_eRes = e_eFSS_CORELL_RES_CLBCKCRCERR;
                            }

                            if( e_eFSS_CORELL_RES_OK == l_eRes )
                            {
                                if( l_uPageCrcCalc != l_tPrvMeta.uPageCrc )
                                {
                                    l_eRes = e_eFSS_CORELL_RES_NOTVALIDPAGE;
                                }
                                else
                                {
                                    if( ( EFSS_PAGEMAGICNUMBER            != l_tPrvMeta.uPageMagicNumber ) ||
                                        ( p_ptCtx->tStorSett.uTotPages    != l_tPrvMeta.uPageTot ) ||
                                        ( p_ptCtx->tStorSett.uPageType    != l_tPrvMeta.uPageType ) )
                                    {
                                        l_eRes = e_eFSS_CORELL_RES_NOTVALIDPAGE;
                                    }
                                    else
                                    {
                                        if( p_ptCtx->tStorSett.uPageVersion != l_tPrvMeta.uPageVersion )
                                        {
                                            l_eRes = e_eFSS_CORELL_RES_NEWVERSIONFOUND;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
		}
	}

	return l_eRes;
}

e_eFSS_CORELL_RES eFSS_CORELL_FlushBuffInPage(t_eFSS_CORELL_Ctx* const p_ptCtx, e_eFSS_TYPE_BUFFTYPE p_eBuffType,
								              const uint32_t p_uPageIndx)
{
	/* Return local var */
	e_eFSS_CORELL_RES l_eRes;
    bool_t l_bCbRes;

    /* Local var used for storage  */
    t_eFSS_TYPE_StorBufPrv* l_ptStorBuff;
    t_eFSS_TYPE_StorBufPrv* l_ptSupBuff;
    t_eFSS_CORELL_privMeta l_tPrvMeta;

    /* Local var used for calculation */
    uint32_t l_uTryPerformed;
    uint32_t l_uPageCrcCalc;
    uint32_t l_uStartIndex;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_CORELL_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eFSS_CORELL_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eFSS_CORELL_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
            }
            else
            {
                /* Check param validity */
                if( p_uPageIndx >= p_ptCtx->tStorSett.uTotPages )
                {
                    l_eRes = e_eFSS_CORELL_RES_BADPARAM;
                }
                else
                {
                    switch(p_eBuffType)
                    {
                        case e_eFSS_TYPE_BUFFTYPE_1:
                        {
                            l_ptStorBuff = &p_ptCtx->tBuff1;
                            l_ptSupBuff  = &p_ptCtx->tBuff2;
                            l_eRes = e_eFSS_CORELL_RES_OK;
                            break;
                        }

                        case e_eFSS_TYPE_BUFFTYPE_2:
                        {
                            l_ptStorBuff = &p_ptCtx->tBuff2;
                            l_ptSupBuff  = &p_ptCtx->tBuff1;
                            l_eRes = e_eFSS_CORELL_RES_OK;
                            break;
                        }

                        default:
                        {
                            l_eRes = e_eFSS_CORELL_RES_BADPARAM;
                            break;
                        }
                    }

                    /* Check validity */
                    if( e_eFSS_CORELL_RES_OK == l_eRes )
                    {
                        /* Initialize internal status */
                        l_tPrvMeta.uPageType = p_ptCtx->tStorSett.uPageType;
                        l_tPrvMeta.uPageVersion = p_ptCtx->tStorSett.uPageVersion;
                        l_tPrvMeta.uPageTot = p_ptCtx->tStorSett.uTotPages;
                        l_tPrvMeta.uPageMagicNumber = EFSS_PAGEMAGICNUMBER;
                        l_tPrvMeta.uPageCrc = 0u;

                        /* Find buffer start */
                        l_uStartIndex = l_ptStorBuff->uBufL - EFSS_PAGEMETALLSIZE;

                        /* Insert requested data */
                        eFSS_CORELLPRV_InsertData(&l_ptStorBuff->puBuf[l_uStartIndex], &l_tPrvMeta, 
                                                  &l_ptStorBuff->tMeta);

                        /* Calculate CRC */
                        l_bCbRes = (*(p_ptCtx->tCtxCb.fCrc32))(p_ptCtx->tCtxCb.ptCtxCrc32, MAX_UINT32VAL, 
                                                               l_ptStorBuff->puBuf, ( l_ptStorBuff->uBufL - 4u ), 
                                                               &l_uPageCrcCalc );

                        if( true != l_bCbRes )
                        {
                            l_eRes = e_eFSS_CORELL_RES_CLBCKCRCERR;
                        }

                        if( e_eFSS_CORELL_RES_OK == l_eRes )
                        {
                            /* Insert Crc data */
                            eFSS_CORELLPRV_InsertCrc(&l_ptStorBuff->puBuf[( l_ptStorBuff->uBufL - 4u )], 
                                                     l_uPageCrcCalc);

                            /* Init var */
                            l_eRes = e_eFSS_CORELL_RES_CLBCKWRITEERR;
                            l_uTryPerformed = 0u;
                            l_bCbRes = false;

                            while( ( false == l_bCbRes ) && ( l_uTryPerformed < p_ptCtx->tStorSett.uRWERetry ) )
                            {
                                /* Erase */
                                l_bCbRes = (*(p_ptCtx->tCtxCb.fErase))(p_ptCtx->tCtxCb.ptCtxErase, p_uPageIndx);

                                if( true == l_bCbRes )
                                {
                                    /* Write */
                                    l_bCbRes = (*(p_ptCtx->tCtxCb.fWrite))(p_ptCtx->tCtxCb.ptCtxWrite, p_uPageIndx,
                                                                           l_ptStorBuff->puBuf, l_ptStorBuff->uBufL);

                                    if( false == l_bCbRes )
                                    {
                                        /* Write error  */
                                        l_eRes = e_eFSS_CORELL_RES_CLBCKWRITEERR;
                                    }
                                }
                                else
                                {
                                    l_eRes = e_eFSS_CORELL_RES_CLBCKERASEERR;
                                }

                                if( true == l_bCbRes )
                                {
                                    /* Read */
                                    l_bCbRes = (*(p_ptCtx->tCtxCb.fRead))(p_ptCtx->tCtxCb.ptCtxRead, p_uPageIndx,
                                                                          l_ptSupBuff->puBuf, l_ptSupBuff->uBufL);

                                    if( false == l_bCbRes )
                                    {
                                        /* Write error  */
                                        l_eRes = e_eFSS_CORELL_RES_CLBCKREADERR;
                                    }
                                }

                                if( true == l_bCbRes )
                                {
                                    /* Compare buffer to write with the readed one */
                                    if( 0 == memcmp(l_ptStorBuff->puBuf, l_ptSupBuff->puBuf, l_ptStorBuff->uBufL) )
                                    {
                                        l_bCbRes = true;
                                    }
                                    else
                                    {
                                        l_bCbRes = false;
                                        l_eRes = e_eFSS_CORELL_RES_WRITENOMATCHREAD;
                                    }
                                }

                                l_uTryPerformed++;
                            }

                            if( true == l_bCbRes )
                            {
                                l_eRes = e_eFSS_CORELL_RES_OK;
                            }
                        }
                    }
                }
            }
		}
	}

	return l_eRes;
}

e_eFSS_CORELL_RES eFSS_CORELL_CalcCrcInBuff(t_eFSS_CORELL_Ctx* const p_ptCtx, e_eFSS_TYPE_BUFFTYPE p_eBuffType,
								            uint32_t p_uCrcSeed, uint32_t p_uLenCalc, uint32_t* p_puCrc)
{
	/* Return local var */
	e_eFSS_CORELL_RES l_eRes;
    bool_t l_bCbRes;

    /* Local var used for calculation */
	uint8_t* l_puBuff;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puCrc ) )
	{
		l_eRes = e_eFSS_CORELL_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eFSS_CORELL_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eFSS_CORELL_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
            }
            else
            {
                /* Check param validity */
                if( ( p_uLenCalc <= 0u ) || ( p_uLenCalc > ( p_ptCtx->tStorSett.uPagesLen - EFSS_PAGEMETALLSIZE  ) ) )
                {
                    l_eRes = e_eFSS_CORELL_RES_BADPARAM;
                }
                else
                {
                    switch(p_eBuffType)
                    {
                        case e_eFSS_TYPE_BUFFTYPE_1:
                        {
                            l_puBuff = p_ptCtx->tBuff1.puBuf;
                            l_eRes = e_eFSS_CORELL_RES_OK;
                            break;
                        }

                        case e_eFSS_TYPE_BUFFTYPE_2:
                        {
                            l_puBuff = p_ptCtx->tBuff2.puBuf;
                            l_eRes = e_eFSS_CORELL_RES_OK;
                            break;
                        }

                        default:
                        {
                            l_eRes = e_eFSS_CORELL_RES_BADPARAM;
                            break;
                        }
                    }

                    /* Check validity */
                    if( e_eFSS_CORELL_RES_OK == l_eRes )
                    {
                        /* Calc */
                        l_bCbRes = (*(p_ptCtx->tCtxCb.fCrc32))(p_ptCtx->tCtxCb.ptCtxCrc32, p_uCrcSeed, l_puBuff,
                                                               p_uLenCalc, p_puCrc );

                        if( true != l_bCbRes )
                        {
                            l_eRes = e_eFSS_CORELL_RES_CLBCKCRCERR;
                        }
                    }
                }
            }
		}
	}

	return l_eRes;
}



/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eFSS_CORELL_IsStatusStillCoherent(const t_eFSS_CORELL_Ctx* p_ptCtx)
{
    bool_t l_eRes;

	/* Check context validity */
    if( ( NULL == p_ptCtx->tCtxCb.ptCtxErase ) || ( NULL == p_ptCtx->tCtxCb.fErase ) ||
        ( NULL == p_ptCtx->tCtxCb.ptCtxWrite ) || ( NULL == p_ptCtx->tCtxCb.fWrite ) ||
        ( NULL == p_ptCtx->tCtxCb.ptCtxRead  ) || ( NULL == p_ptCtx->tCtxCb.fRead  ) ||
        ( NULL == p_ptCtx->tCtxCb.ptCtxCrc32 ) || ( NULL == p_ptCtx->tCtxCb.fCrc32 ) ||
        ( NULL == p_ptCtx->puBuf1 ) || ( NULL == p_ptCtx->puBuf2 ) )
    {
        l_eRes = false;
    }
    else
    {
        /* Check data validity */
        if( p_ptCtx->tStorSett.uTotPages <= 0u )
        {
            l_eRes = false;
        }
        else
        {
            /* Check data validity */
            if( p_ptCtx->tStorSett.uPagesLen <= EFSS_PAGEMETALLSIZE )
            {
                l_eRes = false;
            }
            else
            {
                /* Check data validity */
                if( p_ptCtx->tStorSett.uRWERetry <= 0u )
                {
                        l_eRes = false;
                }
                else
                {
                    /* Check data validity */
                    if( p_ptCtx->uBuf2L != p_ptCtx->uBuf1L )
                    {
                        l_eRes = false;
                    }
                    else
                    {
                        /* Check data validity */
                        if( p_ptCtx->tStorSett.uPagesLen != p_ptCtx->uBuf1L )
                        {
                            l_eRes = false;
                        }
                        else
                        {
                            l_eRes = true;
                        }
                    }
                }
            }
        }
    }

    return l_eRes;
}
