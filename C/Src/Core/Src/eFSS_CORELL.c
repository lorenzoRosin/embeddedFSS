/**
 * @file       eFSS_CORELL.h
 *
 * @brief      Low level core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_LOGC.h"
#include "eFSS_LOGCPRV.h"
#include "eFSS_UTILSHLPRV.h"


/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static e_eFSS_LOGC_RES eFSS_LOGC_LoadIndexNRepair(t_eFSS_LOGC_Ctx* const p_ptCtx);
static e_eFSS_LOGC_RES eFSS_LOGC_LoadIndexFromCache(t_eFSS_LOGC_Ctx* const p_ptCtx);
static e_eFSS_LOGC_RES eFSS_LOGC_LoadIndxBySearch(t_eFSS_LOGC_Ctx* const p_ptCtx);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_CORELL_RES eFSS_CORELL_InitCtx(t_eFSS_CORELL_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx const p_tCtxCb,
									  t_eFSS_CORELL_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL)
{
    e_eFSS_LOGC_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_tCtxCb.ptCtxErase ) || ( NULL == p_tCtxCb.fErase ) ||
            ( NULL == p_tCtxCb.ptCtxWrite ) || ( NULL == p_tCtxCb.fWrite ) ||
            ( NULL == p_tCtxCb.ptCtxRead  ) || ( NULL == p_tCtxCb.fRead  ) ||
            ( NULL == p_tCtxCb.ptCtxCrc32 ) || ( NULL == p_tCtxCb.fCrc32 ) )
        {
            l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity */
            if( p_uBuffL != ( 2u * p_tStorSet.uPagesLen ) )
            {
                l_eRes = e_eFSS_LOGC_RES_BADPARAM;
            }
            else
            {
                /* Check data validity */
                if( ( 0u != ( p_tStorSet.uTotPages % 2u ) )
                {
                    l_eRes = e_eFSS_LOGC_RES_BADPARAM;
                }
                else
                {
                    /* Check data validity */
                    if( ( p_tStorSet.uPagesLen <= EFSS_PAGEMETASIZE ) || ( 0u != ( p_tStorSet.uPagesLen % 2u ) ) )
                    {
                        l_eRes = e_eFSS_LOGC_RES_BADPARAM;
                    }
                    else
                    {
                        /* Check data validity */
                        if( p_tStorSet.uRWERetry <= 0u )
                        {
                            l_eRes = e_eFSS_LOGC_RES_BADPARAM;
                        }
                        else
                        {
                            /* Fill context */
                            p_ptCtx->bIsInit 	= true;
                            p_ptCtx->tCtxCb 	= p_tCtxCb;
                            p_ptCtx->tStorSett 	= p_tStorSet;
                            p_ptCtx->puBuf1 	= p_puBuff;
                            p_ptCtx->uBuf1L 	= p_uBuffL \ 2u;
                            p_ptCtx->puBuf2 	= p_puBuff[p_uBuffL \ 2u];
                            p_ptCtx->uBuf2L 	= p_uBuffL \ 2u;
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
	/* Local variable */
	e_eFSS_LOGC_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInit;
        l_eRes = e_eFSS_LOGC_RES_OK;
	}

	return l_eRes;
}

e_eFSS_CORELL_RES eFSS_CORELL_GetBuff(t_eFSS_CORELL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_tBuffType, 
								      uint8_t** p_ppuBuff, uint32_t* p_puBuffL)
{
	/* Local variable */
	e_eFSS_LOGC_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ppuBuff ) || ( NULL == p_puBuffL ) )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eFSS_LOGC_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eFSS_LOGCPRV_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
            }
            else
            {
				switch(p_tBuffType)
				{
					case e_eFSS_CORELL_BUFTYPE_1:
					{
						*p_ppuBuff = p_ptCtx->puBuf1;
						*p_puBuffL = p_ptCtx->uBuf1L;
						l_eRes = e_eFSS_LOGC_RES_OK;	
						break;	
					}
					
					case e_eFSS_CORELL_BUFTYPE_2:
					{
						*p_ppuBuff = p_ptCtx->puBuf2;
						*p_puBuffL = p_ptCtx->uBuf2L;
						l_eRes = e_eFSS_LOGC_RES_OK;
						break;
					}

					default:
					{
						l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
						break;
					}					
				}
				
				/* Check internal status validity */
				if( false == p_tBuffType )
				{
					l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
				}
				else
				{
					l_eRes = eFSS_LOGC_LoadIndexNRepair(p_ptCtx);
				}
            }
		}
	}

	return l_eRes;
}

e_eFSS_CORELL_RES eFSS_CORELL_LoadPage(t_eFSS_CORELL_Ctx* const p_ptCtx, e_eFSS_CORELL_BUFTYPE p_tBuffType, 
								       const uint32_t p_uPageIndx)
{
	/* Local variable */
	e_eFSS_LOGC_RES l_eRes;
    uint32_t l_uTryPerformed;
    bool_t l_bCbRes;
	
	uint8_t* l_puBuff;
	uint32_t l_uBuffL;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOGC_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eFSS_LOGC_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eFSS_LOGCPRV_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
            }
            else
            {
				switch(p_tBuffType)
				{
					case e_eFSS_CORELL_BUFTYPE_1:
					{
						l_puBuff = p_ptCtx->puBuf1;
						l_uBuffL = p_ptCtx->uBuf1L;
						l_eRes = e_eFSS_LOGC_RES_OK;	
						break;	
					}
					
					case e_eFSS_CORELL_BUFTYPE_2:
					{
						l_puBuff = p_ptCtx->puBuf2;
						l_uBuffL = p_ptCtx->uBuf2L;
						l_eRes = e_eFSS_LOGC_RES_OK;
						break;
					}

					default:
					{
						l_eRes = e_eFSS_LOGC_RES_CORRUPTCTX;
						break;
					}					
				}
				
				/* Check internal status validity */
				if( e_eFSS_LOGC_RES_OK == l_eRes )
				{
					while( ( false == l_bCbRes ) && ( l_uTryPerformed < p_uReTry ) )
					{
						/* Read */
						l_bCbRes = (*(p_ptCbCtx->fRead))(p_ptCbCtx->ptCtxRead, p_uPageIndx, p_puDataR, p_uDataRLen);
						l_uTryPerformed++;

						if( false == l_bCbRes )
						{
							l_eRes = e_eFSS_UTILSLLPRV_RES_CLBCKREADERR;
						}
						else
						{
							l_eRes = e_eFSS_UTILSLLPRV_RES_OK;
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
static e_eFSS_LOGC_RES eFSS_LOGC_LoadIndexNRepair(t_eFSS_LOGC_Ctx* const p_ptCtx)
{
    e_eFSS_LOGC_RES l_eRes;

    /* Search for index in the flash cache if present */
    if( true == p_ptCtx->bUseFlashCache )
    {
        /* Load index from cache */
        l_eRes = eFSS_LOGC_LoadIndexFromCache(p_ptCtx);
    }
    else
    {
        /* Search for index */
        l_eRes = eFSS_LOGC_LoadIndxBySearch(p_ptCtx);
    }

    return l_eRes;
}

static e_eFSS_LOGC_RES eFSS_LOGC_LoadIndexFromCache(t_eFSS_LOGC_Ctx* const p_ptCtx)
{
    /* Index are present in the first page and it's backup */
    e_eFSS_UTILSHLPRV_RES l_eIntegrityRes;
    e_eFSS_LOGC_RES l_eRes;
    t_eFSS_TYPE_PageMeta l_tPagePrm;

    /* Verify and create page index integrity */
    l_eIntegrityRes =  eFSS_UTILSHLPRV_VerifyNRipristBkup(p_ptCtx->tCtxCb, p_ptCtx->uReTry,
                                                          p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                          p_ptCtx->puBuff2, p_ptCtx->uBuff2L,
                                                          0u, 1u);
    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eIntegrityRes);

    if( ( e_eFSS_LOGC_RES_OK == l_eRes ) || ( e_eFSS_LOGC_RES_OK_BKP_RCVRD == l_eRes ) )
    {
        /* Index page is OK, we can retrive info from that */
        l_eIntegrityRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->tCtxCb, 0u,
                                                       p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                       &l_tPagePrm, p_ptCtx->uReTry);
        l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eIntegrityRes);
        if( e_eFSS_LOGC_RES_OK == l_eRes )
        {
            /* can load finded index in the context */
            p_ptCtx->uNPagIdxFound = l_tPagePrm.uPageUseSpecific1;
            p_ptCtx->uOPagIdxFound = l_tPagePrm.uPageUseSpecific2;

            /* Verify log version */
            if( p_ptCtx->uVersion != l_tPagePrm.uPageVersion )
            {
                /* New version of log found */
                l_eRes = e_eFSS_LOGC_RES_NEWVERSIONLOG;
            }
            else
            {
                /* Verify page type, if not coherent we were formatting the area before */
            }
        }
    }

    return l_eRes;
}

static e_eFSS_LOGC_RES eFSS_LOGC_LoadIndxBySearch(t_eFSS_LOGC_Ctx* const p_ptCtx)
{
    /* Verify if index already present are valid, if not search for them */
    e_eFSS_UTILSHLPRV_RES l_eIntegrityRes;
    e_eFSS_LOGC_RES l_eRes;
    bool_t l_bIsValid;
    t_eFSS_TYPE_PageMeta p_ptPagePrm;
    bool_t l_bIsIndexFound;
    uint32_t l_i;

    /* Init parameter */
    l_bIsIndexFound = false;

    /* Verify integrity first */
    l_eIntegrityRes = eFSS_UTILSHLPRV_IsValidPage(p_ptCtx->tCtxCb, p_ptCtx->uNPagIdxFound,
                                                  p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                  p_ptCtx->uReTry,  &l_bIsValid );
    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eIntegrityRes);

    if( e_eFSS_LOGC_RES_OK == l_eRes )
    {
        if( true == l_bIsValid )
        {
            /* The page pointed by uNPagIdxFound is ok, need to check if it'is the newest page */
            l_eIntegrityRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->tCtxCb, p_ptCtx->uNPagIdxFound,
                                                           p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                           &p_ptPagePrm, p_ptCtx->uReTry);
            l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eIntegrityRes);
            if( e_eFSS_LOGC_RES_OK == l_eRes )
            {
                if( ( EFSS_PAGETYPE_LOG == p_ptPagePrm.uPageType ) &&
                    ( EFSS_PAGESUBTYPE_LOGNEWEST == p_ptPagePrm.uPageSubType ) &&
                    ( p_ptCtx->uVersion == p_ptPagePrm.uPageVersion ) &&
                    ( EFSS_PAGEMAGICNUMBER == p_ptPagePrm.uPageMagicNumber ) )
                {
                    /* Very good, this is the newwest page avaiable  */
                    l_bIsIndexFound = true;
                }
            }
        }

        if( false == l_bIsIndexFound )
        {
            /* Read every page and search for the newest page type */
            l_i = 0u;

            while( ( false == l_bIsIndexFound ) && ( l_i < p_ptCtx->uNPage )  )
            {
                l_eIntegrityRes = eFSS_UTILSHLPRV_ReadPageNPrm(p_ptCtx->tCtxCb, l_i,
                                                               p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                               &p_ptPagePrm, p_ptCtx->uReTry);
                l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eIntegrityRes);
                if( e_eFSS_LOGC_RES_OK == l_eRes )
                {
                    /* Is the page valid? */
                    l_eIntegrityRes = eFSS_UTILSHLPRV_IsValidPageInBuff(p_ptCtx->tCtxCb,
                                                                        p_ptCtx->puBuff1, p_ptCtx->uBuff1L,
                                                                        &l_bIsValid);
                    l_eRes = eFSS_LOGCPRV_HLtoLogRes(l_eIntegrityRes);

                    if( e_eFSS_LOGC_RES_OK == l_eRes )
                    {
                        if( true == l_bIsValid )
                        {
                            /* Valid page, is the one we where searching? */
                            if( ( EFSS_PAGETYPE_LOG == p_ptPagePrm.uPageType ) &&
                                ( EFSS_PAGESUBTYPE_LOGNEWEST == p_ptPagePrm.uPageSubType ) &&
                                ( p_ptCtx->uVersion == p_ptPagePrm.uPageVersion ) &&
                                ( EFSS_PAGEMAGICNUMBER == p_ptPagePrm.uPageMagicNumber ) )
                            {
                                /* Super top, this is the page, nothing more to do  */
                                l_bIsIndexFound = true;
                                p_ptCtx->uNPagIdxFound = l_i;
                                p_ptCtx->uOPagIdxFound = p_ptPagePrm.uPageUseSpecific1;
                            }
                            else if( ( EFSS_PAGETYPE_LOG == p_ptPagePrm.uPageType ) &&
                                ( EFSS_PAGESUBTYPE_LOGNEWESTBKP == p_ptPagePrm.uPageSubType ) &&
                                ( p_ptCtx->uVersion == p_ptPagePrm.uPageVersion ) &&
                                ( EFSS_PAGEMAGICNUMBER == p_ptPagePrm.uPageMagicNumber ) )
                            {
                                /* Super top, this is the page, nothing more to do  */
                                l_bIsIndexFound = true;
                                p_ptCtx->uNPagIdxFound = eFSS_LOGCPRV_GetPrevIndex( l_i, p_ptCtx->uNPage );
                                p_ptCtx->uOPagIdxFound = p_ptPagePrm.uPageUseSpecific1;
                            }
                        }
                    }
                }
            }
        }
    }

    return l_eRes;
}