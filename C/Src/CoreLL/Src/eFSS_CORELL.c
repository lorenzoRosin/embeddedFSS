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
 * ------------------------------------------------------------------ Private metadata (19 byte)
 * - uint32_t  - uPageIndx          -> Page Index                    |
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
#include "eFSS_Utils.h"



/***********************************************************************************************************************
 *      PRIVATE DEFINE
 **********************************************************************************************************************/
#define EFSS_CORELL_PAGEMIN_L                                                                      ( ( uint32_t )  19u )
#define EFSS_CORELL_CRC_L                                                                          ( ( uint32_t )   4u )
#define EFSS_CORELL_PAGEMAGNUM                                                             ( ( uint32_t )  0xA5A5A5A5u )



/***********************************************************************************************************************
 *      PRIVATE TYPEDEFS
 **********************************************************************************************************************/
typedef struct
{
    uint32_t    uPageIndx;
    uint8_t     uPageType;
    uint16_t    uPageVersion;
    uint32_t    uPageTot;
    uint32_t    uPageMagicNumber;
    uint32_t    uPageCrc;
}t_eFSS_CORELLPRV_PrvMeta;



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_CORELL_IsStatusStillCoherent(const t_eFSS_CORELL_Ctx* p_ptCtx);
static e_eFSS_CORELL_RES eFSS_CORELLPRV_ExtractData(const t_eFSS_CORELL_StorBufPrv* p_ptBuff,
                                                    t_eFSS_CORELLPRV_PrvMeta* const p_ptPar);
static e_eFSS_CORELL_RES eFSS_CORELLPRV_InsertData(t_eFSS_CORELL_StorBufPrv* const p_ptBuff,
                                                   const t_eFSS_CORELLPRV_PrvMeta* p_ptPar);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_CORELL_RES eFSS_CORELL_InitCtx(t_eFSS_CORELL_Ctx* const p_ptCtx, const t_eFSS_TYPE_CbStorCtx p_tCtxCb,
									  const t_eFSS_TYPE_StorSet p_tStorSet, const uint8_t p_uStorType,
                                      uint8_t* const p_puBuff, const uint32_t p_uBuffL)
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
                    if( p_tStorSet.uPagesLen <= EFSS_CORELL_PAGEMIN_L )
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
                            p_ptCtx->uStorType  = p_uStorType;
                            p_ptCtx->tBuff1.puBuf = p_puBuff;
                            p_ptCtx->tBuff1.uBufL = p_uBuffL / 2u;
                            p_ptCtx->tBuff2.puBuf = &p_puBuff[p_ptCtx->tBuff1.uBufL];
                            p_ptCtx->tBuff2.uBufL = p_ptCtx->tBuff1.uBufL;

                            /* All OK */
                            l_eRes = e_eFSS_CORELL_RES_OK;
                        }
                    }
                }
            }
        }
    }

    return l_eRes;
}

e_eFSS_CORELL_RES eFSS_CORELL_IsInit(const t_eFSS_CORELL_Ctx* p_ptCtx, bool_t* const p_pbIsInit)
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

e_eFSS_CORELL_RES eFSS_CORELL_GetStorSett(t_eFSS_CORELL_Ctx* const p_ptCtx, t_eFSS_TYPE_StorSet* const p_ptStorSet)
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

e_eFSS_CORELL_RES eFSS_CORELL_GetBuff(t_eFSS_CORELL_Ctx* const p_ptCtx, t_eFSS_CORELL_StorBuf* const p_ptBuff1,
                                      t_eFSS_CORELL_StorBuf* const p_ptBuff2)
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
                /* Return the usable buffer to the upper layer */
                /* Remeber that we don't return the whole buffer because some private metadata are managerd here */
                p_ptBuff1->puBuf = p_ptCtx->tBuff1.puBuf;
                p_ptBuff1->uBufL = p_ptCtx->tBuff1.uBufL - EFSS_CORELL_PAGEMIN_L;

                p_ptBuff2->puBuf = p_ptCtx->tBuff2.puBuf;
                p_ptBuff2->uBufL = p_ptCtx->tBuff2.uBufL - EFSS_CORELL_PAGEMIN_L;

                l_eRes = e_eFSS_CORELL_RES_OK;
            }
		}
	}

	return l_eRes;
}

e_eFSS_CORELL_RES eFSS_CORELL_GetBuffNStor(t_eFSS_CORELL_Ctx* const p_ptCtx, t_eFSS_CORELL_StorBuf* const p_ptBuff1,
                                           t_eFSS_CORELL_StorBuf* const p_ptBuff2,
                                           t_eFSS_TYPE_StorSet* const p_ptStorSet)
{
	/* Return local var */
	e_eFSS_CORELL_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ptBuff1 ) || ( NULL == p_ptBuff2 ) || ( NULL == p_ptStorSet ) )
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
                /* Return the usable buffer to the upper layer */
                /* Remeber that we don't return the whole buffer because some private metadata are managerd here */
                p_ptBuff1->puBuf = p_ptCtx->tBuff1.puBuf;
                p_ptBuff1->uBufL = p_ptCtx->tBuff1.uBufL - EFSS_CORELL_PAGEMIN_L;

                p_ptBuff2->puBuf = p_ptCtx->tBuff2.puBuf;
                p_ptBuff2->uBufL = p_ptCtx->tBuff2.uBufL - EFSS_CORELL_PAGEMIN_L;

                *p_ptStorSet = p_ptCtx->tStorSett;

                l_eRes = e_eFSS_CORELL_RES_OK;
            }
		}
	}

	return l_eRes;
}

e_eFSS_CORELL_RES eFSS_CORELL_LoadPageInBuff(t_eFSS_CORELL_Ctx* const p_ptCtx, const e_eFSS_CORELL_BUFFTYPE p_eBuffType,
								             const uint32_t p_uPageIndx)
{
	/* Return local var */
	e_eFSS_CORELL_RES l_eRes;
    bool_t l_bCbRes;

    /* Local var used for storage */
    t_eFSS_CORELL_StorBufPrv* l_ptMainBuf;
    t_eFSS_CORELLPRV_PrvMeta l_tPrvMeta;

    /* Local var used for calculation */
    uint32_t l_uTryPerformed;
    uint32_t l_uPageCrcCalc;
    uint32_t l_uBuffCrcLen;

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
                /* Check parameter validity */
                if( p_uPageIndx >= p_ptCtx->tStorSett.uTotPages )
                {
                    l_eRes = e_eFSS_CORELL_RES_BADPARAM;
                }
                else
                {
                    switch(p_eBuffType)
                    {
                        case e_eFSS_CORELL_BUFFTYPE_1:
                        {
                            l_ptMainBuf = &p_ptCtx->tBuff1;
                            l_eRes = e_eFSS_CORELL_RES_OK;
                            break;
                        }

                        case e_eFSS_CORELL_BUFFTYPE_2:
                        {
                            l_ptMainBuf = &p_ptCtx->tBuff2;
                            l_eRes = e_eFSS_CORELL_RES_OK;
                            break;
                        }

                        default:
                        {
                            l_ptMainBuf = &p_ptCtx->tBuff2;
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

                        /* Read at least uRWERetry times, after that surrend */
                        while( ( e_eFSS_CORELL_RES_OK != l_eRes ) &&
                               ( l_uTryPerformed < p_ptCtx->tStorSett.uRWERetry ) )
                        {
                            l_bCbRes = (*(p_ptCtx->tCtxCb.fRead))(p_ptCtx->tCtxCb.ptCtxRead, p_uPageIndx,
                                                                  l_ptMainBuf->puBuf, l_ptMainBuf->uBufL);
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
                            /* Page readed, extract metadata and check validity */
                            l_eRes = eFSS_CORELLPRV_ExtractData(l_ptMainBuf, &l_tPrvMeta);

                            if( e_eFSS_CORELL_RES_OK == l_eRes )
                            {
                                /* First, calculate and verify CRC value */
                                /* Init var */
                                l_uPageCrcCalc = 0u;

                                if( l_ptMainBuf->uBufL > EFSS_CORELL_PAGEMIN_L )
                                {
                                    l_uBuffCrcLen = ( l_ptMainBuf->uBufL - EFSS_CORELL_CRC_L );
                                    l_bCbRes = (*(p_ptCtx->tCtxCb.fCrc32))(p_ptCtx->tCtxCb.ptCtxCrc32, MAX_UINT32VAL,
                                                                          l_ptMainBuf->puBuf, l_uBuffCrcLen,
                                                                          &l_uPageCrcCalc );

                                    if( true != l_bCbRes )
                                    {
                                        l_eRes = e_eFSS_CORELL_RES_CLBCKCRCERR;
                                    }
                                }
                                else
                                {
                                    /* Just to delete a c stat warning even if is not needed */
                                    l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
                                }

                                if( e_eFSS_CORELL_RES_OK == l_eRes )
                                {
                                    /* calculated CRC must be equals to the stored CRC */
                                    if( l_uPageCrcCalc != l_tPrvMeta.uPageCrc )
                                    {
                                        l_eRes = e_eFSS_CORELL_RES_NOTVALIDPAGE;
                                    }
                                    else
                                    {
                                        /* Crc is correct, page validity is cofirmed. Check others metadata in order
                                           to be sure that the previously saved page is related to this subtype */
                                        if( ( EFSS_CORELL_PAGEMAGNUM          != l_tPrvMeta.uPageMagicNumber ) ||
                                            ( p_ptCtx->tStorSett.uTotPages    != l_tPrvMeta.uPageTot ) ||
                                            ( p_ptCtx->uStorType              != l_tPrvMeta.uPageType ) ||
                                            ( p_uPageIndx                     != l_tPrvMeta.uPageIndx ) )
                                        {
                                            l_eRes = e_eFSS_CORELL_RES_NOTVALIDPAGE;
                                        }
                                        else
                                        {
                                            /* All ok, page is correct. Verify if we are reading the same page version,
                                               or if the previously stored page was related to anothers version */
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
	}

	return l_eRes;
}

e_eFSS_CORELL_RES eFSS_CORELL_FlushBuffInPage(t_eFSS_CORELL_Ctx* const p_ptCtx,
								              const e_eFSS_CORELL_BUFFTYPE p_eBuffType, const uint32_t p_uPageIndx)
{
	/* Return local var */
	e_eFSS_CORELL_RES l_eRes;
    bool_t l_bCbRes;

    /* Local var used for storage */
    t_eFSS_CORELL_StorBufPrv* l_ptMainBuf;
    const t_eFSS_CORELL_StorBufPrv* l_ptBkpBuf;
    t_eFSS_CORELLPRV_PrvMeta l_tPrvMeta;

    /* Local var used for calculation */
    uint32_t l_uTryPerformed;
    uint32_t l_uPageCrcCalc;
    uint32_t l_uBuffCrcLen;

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
                        case e_eFSS_CORELL_BUFFTYPE_1:
                        {
                            l_ptMainBuf = &p_ptCtx->tBuff1;
                            l_ptBkpBuf  = &p_ptCtx->tBuff2;
                            l_eRes = e_eFSS_CORELL_RES_OK;
                            break;
                        }

                        case e_eFSS_CORELL_BUFFTYPE_2:
                        {
                            l_ptMainBuf = &p_ptCtx->tBuff2;
                            l_ptBkpBuf  = &p_ptCtx->tBuff1;
                            l_eRes = e_eFSS_CORELL_RES_OK;
                            break;
                        }

                        default:
                        {
                            l_ptMainBuf = &p_ptCtx->tBuff2;
                            l_ptBkpBuf  = &p_ptCtx->tBuff1;
                            l_eRes = e_eFSS_CORELL_RES_BADPARAM;
                            break;
                        }
                    }

                    /* Check validity */
                    if( e_eFSS_CORELL_RES_OK == l_eRes )
                    {
                        /* Initialize internal status */
                        l_tPrvMeta.uPageIndx = p_uPageIndx;
                        l_tPrvMeta.uPageType = p_ptCtx->uStorType;
                        l_tPrvMeta.uPageVersion = p_ptCtx->tStorSett.uPageVersion;
                        l_tPrvMeta.uPageTot = p_ptCtx->tStorSett.uTotPages;
                        l_tPrvMeta.uPageMagicNumber = EFSS_CORELL_PAGEMAGNUM;
                        l_tPrvMeta.uPageCrc = 0u;

                        /* Insert requested data, even if CRC is still not calculated */
                        l_eRes = eFSS_CORELLPRV_InsertData(l_ptMainBuf, &l_tPrvMeta);

                        if( e_eFSS_CORELL_RES_OK == l_eRes )
                        {
                            /* Init var */
                            l_uPageCrcCalc = 0u;

                            /* Calculate CRC */
                            if( l_ptMainBuf->uBufL > EFSS_CORELL_PAGEMIN_L )
                            {
                                l_uBuffCrcLen = ( l_ptMainBuf->uBufL - EFSS_CORELL_CRC_L );
                                l_bCbRes = (*(p_ptCtx->tCtxCb.fCrc32))(p_ptCtx->tCtxCb.ptCtxCrc32, MAX_UINT32VAL,
                                                                       l_ptMainBuf->puBuf, l_uBuffCrcLen,
                                                                       &l_uPageCrcCalc );

                                if( true != l_bCbRes )
                                {
                                    l_eRes = e_eFSS_CORELL_RES_CLBCKCRCERR;
                                }
                            }
                            else
                            {
                                /* Just to delete a c stat warning even if is not needed */
                                l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
                            }

                            if( e_eFSS_CORELL_RES_OK == l_eRes )
                            {
                                /* Insert Crc data in to the metadata, and insert metadata in to the buffer to flush */
                                l_tPrvMeta.uPageCrc = l_uPageCrcCalc;
                                l_eRes = eFSS_CORELLPRV_InsertData(l_ptMainBuf, &l_tPrvMeta);

                                if( e_eFSS_CORELL_RES_OK == l_eRes )
                                {
                                    /* Now that we have the buffer ready we need to: erase, write, and check the
                                       storage area indicated by the index */
                                    /* Init var */
                                    l_eRes = e_eFSS_CORELL_RES_CLBCKERASEERR;
                                    l_uTryPerformed = 0u;

                                    while( ( e_eFSS_CORELL_RES_OK != l_eRes ) &&
                                           ( l_uTryPerformed < p_ptCtx->tStorSett.uRWERetry ) )
                                    {
                                        /* Erase */
                                        l_bCbRes = (*(p_ptCtx->tCtxCb.fErase))(p_ptCtx->tCtxCb.ptCtxErase, p_uPageIndx);

                                        if( true == l_bCbRes )
                                        {
                                            l_eRes = e_eFSS_CORELL_RES_OK;
                                        }
                                        else
                                        {
                                            l_eRes = e_eFSS_CORELL_RES_CLBCKERASEERR;
                                        }

                                        if( e_eFSS_CORELL_RES_OK == l_eRes )
                                        {
                                            /* Write */
                                            l_bCbRes = (*(p_ptCtx->tCtxCb.fWrite))(p_ptCtx->tCtxCb.ptCtxWrite,
                                                                                   p_uPageIndx, l_ptMainBuf->puBuf,
                                                                                   l_ptMainBuf->uBufL);

                                            if( true == l_bCbRes )
                                            {
                                                l_eRes = e_eFSS_CORELL_RES_OK;
                                            }
                                            else
                                            {
                                                l_eRes = e_eFSS_CORELL_RES_CLBCKWRITEERR;
                                            }
                                        }

                                        if( e_eFSS_CORELL_RES_OK == l_eRes )
                                        {
                                            /* Read */
                                            l_bCbRes = (*(p_ptCtx->tCtxCb.fRead))(p_ptCtx->tCtxCb.ptCtxRead,
                                                                                  p_uPageIndx, l_ptBkpBuf->puBuf,
                                                                                  l_ptBkpBuf->uBufL);

                                            if( true == l_bCbRes )
                                            {
                                                l_eRes = e_eFSS_CORELL_RES_OK;
                                            }
                                            else
                                            {
                                                l_eRes = e_eFSS_CORELL_RES_CLBCKREADERR;
                                            }
                                        }

                                        if( e_eFSS_CORELL_RES_OK == l_eRes )
                                        {
                                            /* Compare buffer to write with the readed one */
                                            if( 0 == memcmp(l_ptMainBuf->puBuf, l_ptBkpBuf->puBuf, l_ptMainBuf->uBufL) )
                                            {
                                                l_eRes = e_eFSS_CORELL_RES_OK;
                                            }
                                            else
                                            {
                                                l_eRes = e_eFSS_CORELL_RES_WRITENOMATCHREAD;
                                            }
                                        }

                                        l_uTryPerformed++;
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

e_eFSS_CORELL_RES eFSS_CORELL_CalcCrcInBuff(const t_eFSS_CORELL_Ctx* p_ptCtx,
                                            const e_eFSS_CORELL_BUFFTYPE p_eBuffType, const uint32_t p_uCrcSeed,
								            const uint32_t p_uLenCalc, uint32_t* const p_puCrc)
{
	/* Return local var */
	e_eFSS_CORELL_RES l_eRes;
    bool_t l_bCbRes;

    /* Local var used for calculation */
	const uint8_t* l_puBuff;

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
                if( ( p_uLenCalc <= 0u ) || ( p_uLenCalc > ( p_ptCtx->tStorSett.uPagesLen - EFSS_CORELL_PAGEMIN_L ) ) )
                {
                    l_eRes = e_eFSS_CORELL_RES_BADPARAM;
                }
                else
                {
                    switch(p_eBuffType)
                    {
                        case e_eFSS_CORELL_BUFFTYPE_1:
                        {
                            l_puBuff = p_ptCtx->tBuff1.puBuf;
                            l_eRes = e_eFSS_CORELL_RES_OK;
                            break;
                        }

                        case e_eFSS_CORELL_BUFFTYPE_2:
                        {
                            l_puBuff = p_ptCtx->tBuff2.puBuf;
                            l_eRes = e_eFSS_CORELL_RES_OK;
                            break;
                        }

                        default:
                        {
                            l_puBuff = p_ptCtx->tBuff2.puBuf;
                            l_eRes = e_eFSS_CORELL_RES_BADPARAM;
                            break;
                        }
                    }

                    /* Check validity */
                    if( e_eFSS_CORELL_RES_OK == l_eRes )
                    {
                        /* Set default value */
                        *p_puCrc = 0u;

                        /* Calc */
                        l_bCbRes = (*(p_ptCtx->tCtxCb.fCrc32))(p_ptCtx->tCtxCb.ptCtxCrc32, p_uCrcSeed, l_puBuff,
                                                               p_uLenCalc, p_puCrc);

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
    /* Return local var */
    bool_t l_eRes;

	/* Check context pointer validity */
    if( ( NULL == p_ptCtx->tCtxCb.ptCtxErase ) || ( NULL == p_ptCtx->tCtxCb.fErase ) ||
        ( NULL == p_ptCtx->tCtxCb.ptCtxWrite ) || ( NULL == p_ptCtx->tCtxCb.fWrite ) ||
        ( NULL == p_ptCtx->tCtxCb.ptCtxRead  ) || ( NULL == p_ptCtx->tCtxCb.fRead  ) ||
        ( NULL == p_ptCtx->tCtxCb.ptCtxCrc32 ) || ( NULL == p_ptCtx->tCtxCb.fCrc32 ) ||
        ( NULL == p_ptCtx->tBuff1.puBuf )      || ( NULL == p_ptCtx->tBuff2.puBuf )  ||
        ( p_ptCtx->tBuff2.puBuf == p_ptCtx->tBuff1.puBuf ) )
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
            if( p_ptCtx->tStorSett.uPagesLen <= EFSS_CORELL_PAGEMIN_L )
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
                    if( p_ptCtx->tBuff1.uBufL != p_ptCtx->tBuff2.uBufL )
                    {
                        l_eRes = false;
                    }
                    else
                    {
                        /* Check data validity */
                        if( p_ptCtx->tStorSett.uPagesLen != p_ptCtx->tBuff1.uBufL )
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

static e_eFSS_CORELL_RES eFSS_CORELLPRV_ExtractData(const t_eFSS_CORELL_StorBufPrv* p_ptBuff,
                                                    t_eFSS_CORELLPRV_PrvMeta* const p_ptPar)
{
	/* Return local var */
	e_eFSS_CORELL_RES l_eRes;

    /* Local var for calculation */
    uint32_t l_uCurrIdx;
    uint8_t* l_puCBuff;

    if( ( NULL == p_ptBuff ) || ( NULL == p_ptPar ) )
    {
        l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
    }
    else
    {
        if( p_ptBuff->uBufL <= EFSS_CORELL_PAGEMIN_L )
        {
            l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
        }
        else
        {
            /* Init variable */
            l_puCBuff = &p_ptBuff->puBuf[p_ptBuff->uBufL - EFSS_CORELL_PAGEMIN_L];
            l_uCurrIdx = 0u;

            /* Estract */
            if( false == eFSS_Utils_RetriveU32(&l_puCBuff[l_uCurrIdx], &p_ptPar->uPageIndx) )
            {
                l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
            }
            else
            {
                l_uCurrIdx += 4u;
                if( false == eFSS_Utils_RetriveU8(&l_puCBuff[l_uCurrIdx], &p_ptPar->uPageType) )
                {
                    l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
                }
                else
                {
                    l_uCurrIdx += 1u;
                    if( false == eFSS_Utils_RetriveU16(&l_puCBuff[l_uCurrIdx], &p_ptPar->uPageVersion) )
                    {
                        l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
                    }
                    else
                    {
                        l_uCurrIdx += 2u;
                        if( false == eFSS_Utils_RetriveU32(&l_puCBuff[l_uCurrIdx], &p_ptPar->uPageTot) )
                        {
                            l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
                        }
                        else
                        {
                            l_uCurrIdx += 4u;
                            if( false == eFSS_Utils_RetriveU32(&l_puCBuff[l_uCurrIdx], &p_ptPar->uPageMagicNumber) )
                            {
                                l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
                            }
                            else
                            {
                                l_uCurrIdx += 4u;
                                if( false == eFSS_Utils_RetriveU32(&l_puCBuff[l_uCurrIdx], &p_ptPar->uPageCrc) )
                                {
                                    l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
                                }
                                else
                                {
                                    l_eRes = e_eFSS_CORELL_RES_OK;
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

static e_eFSS_CORELL_RES eFSS_CORELLPRV_InsertData(t_eFSS_CORELL_StorBufPrv* const p_ptBuff,
                                                   const t_eFSS_CORELLPRV_PrvMeta* p_ptPar)
{
	/* Return local var */
	e_eFSS_CORELL_RES l_eRes;

    /* Local var for calculation */
    uint32_t l_uCurrIdx;
    uint8_t* l_puCBuff;

    if( ( NULL == p_ptBuff ) || ( NULL == p_ptPar ) )
    {
        l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
    }
    else
    {
        if( p_ptBuff->uBufL <= EFSS_CORELL_PAGEMIN_L )
        {
            l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
        }
        else
        {
            /* Init variable */
            l_puCBuff = &p_ptBuff->puBuf[p_ptBuff->uBufL - EFSS_CORELL_PAGEMIN_L];
            l_uCurrIdx = 0u;

            /* Insert */
            if( false == eFSS_Utils_InsertU32(&l_puCBuff[l_uCurrIdx], p_ptPar->uPageIndx) )
            {
                l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
            }
            else
            {
                l_uCurrIdx += 4u;
                if( false == eFSS_Utils_InsertU8(&l_puCBuff[l_uCurrIdx], p_ptPar->uPageType) )
                {
                    l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
                }
                else
                {
                    l_uCurrIdx += 1u;
                    if( false == eFSS_Utils_InsertU16(&l_puCBuff[l_uCurrIdx], p_ptPar->uPageVersion) )
                    {
                        l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
                    }
                    else
                    {
                        l_uCurrIdx += 2u;
                        if( false == eFSS_Utils_InsertU32(&l_puCBuff[l_uCurrIdx], p_ptPar->uPageTot) )
                        {
                            l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
                        }
                        else
                        {
                            l_uCurrIdx += 4u;
                            if( false == eFSS_Utils_InsertU32(&l_puCBuff[l_uCurrIdx], p_ptPar->uPageMagicNumber) )
                            {
                                l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
                            }
                            else
                            {
                                l_uCurrIdx += 4u;
                                if( false == eFSS_Utils_InsertU32(&l_puCBuff[l_uCurrIdx], p_ptPar->uPageCrc) )
                                {
                                    l_eRes = e_eFSS_CORELL_RES_CORRUPTCTX;
                                }
                                else
                                {
                                    l_eRes = e_eFSS_CORELL_RES_OK;
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