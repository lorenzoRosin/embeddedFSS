/**
 * @file       eFSS_LOG.h
 *
 * @brief      LOG of fiexd length module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_LOG.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_LOG_IsStatusStillCoherent(const t_eFSS_LOG_Ctx* p_ptCtx);
static e_eFSS_LOG_RES eFSS_LOG_DBCtoDBFLRes(const e_eFSS_LOGC_RES p_eDBCRes);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_LOG_RES eFSS_LOG_InitCtx(t_eFSS_LOG_Ctx* const p_ptCtx, t_eFSS_TYPE_CbStorCtx const p_tCtxCb,
                                  t_eFSS_TYPE_StorSet p_tStorSet, uint8_t* const p_puBuff, uint32_t p_uBuffL,
                                  bool_t p_bFlashCache, bool_t p_bFullBckup)
{
    e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES  l_eDBCRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
        /* Can init low level context */
        l_eDBCRes = eFSS_LOGC_InitCtx(&p_ptCtx->tLOGCCtx, p_tCtxCb, p_tStorSet, p_puBuff, p_uBuffL,
                                      p_bFlashCache, p_bFullBckup);
        l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);
    }

    return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_IsInit(t_eFSS_LOG_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
    e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES  l_eDBCRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
        l_eDBCRes = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, p_pbIsInit);
        l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);
	}

	return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_GetLogStatus(t_eFSS_LOG_Ctx* const p_ptCtx)
{
	/* Local return variable */
    e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES  l_eDBCRes;

    /* Local variable for calculation */
    t_eFSS_TYPE_StorBuf l_tBuff;
    t_eFSS_TYPE_StorSet l_tStorSet;
    bool_t l_bIsInit;
    bool_t l_bIsPageMod;
    uint32_t l_uIndexPToCheck;
    uint32_t l_uMaxPIndex;
    uint32_t l_uMaxElemPageIndex;
    uint32_t l_uElemDone;
    uint32_t l_uElemInPageDone;
    uint16_t l_uCurrEleVer;
    uint8_t* l_puCurrEleData;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eDBCRes = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, &l_bIsInit);
        l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);

        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOG_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOG_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOG_RES_CORRUPTCTX;
                }
                else
                {
                    /* Get storage info */
                    l_eDBCRes = eFSS_LOGC_GetStorageStatus(&p_ptCtx->tLOGCCtx);
                    l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_GetLogInfo(t_eFSS_LOG_Ctx* const p_ptCtx, uint32_t *p_puNewLogI, uint32_t *p_puOldLogI,
                                       uint32_t *p_puNpageUsed, uint32_t *p_puNpageTot)
{
	/* Local return variable */
    e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES  l_eDBCRes;

    /* Local variable for calculation */
    t_eFSS_TYPE_StorBuf l_tBuff;
    t_eFSS_TYPE_StorSet l_tStorSet;
    bool_t l_bIsInit;
    bool_t l_bIsPageMod;
    uint32_t l_uIndexPToCheck;
    uint32_t l_uMaxPIndex;
    uint32_t l_uMaxElemPageIndex;
    uint32_t l_uElemDone;
    uint32_t l_uElemInPageDone;
    uint16_t l_uCurrEleVer;
    uint8_t* l_puCurrEleData;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eDBCRes = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, &l_bIsInit);
        l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);

        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOG_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOG_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOG_RES_CORRUPTCTX;
                }
                else
                {
                    /* Get storage info */
                    l_eDBCRes = eFSS_LOGC_GetLogInfo(&p_ptCtx->tLOGCCtx, p_puNewLogI, p_puOldLogI, p_puNpageUsed, p_puNpageTot);
                    l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_Format(t_eFSS_LOG_Ctx* const p_ptCtx)
{
	/* Local return variable */
    e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES  l_eDBCRes;

    /* Local variable for calculation */
    t_eFSS_TYPE_StorBuf l_tBuff;
    t_eFSS_TYPE_StorSet l_tStorSet;
    bool_t l_bIsInit;
    bool_t l_bIsPageMod;
    uint32_t l_uIndexPToCheck;
    uint32_t l_uMaxPIndex;
    uint32_t l_uMaxElemPageIndex;
    uint32_t l_uElemDone;
    uint32_t l_uElemInPageDone;
    uint16_t l_uCurrEleVer;
    uint8_t* l_puCurrEleData;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eDBCRes = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, &l_bIsInit);
        l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);

        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOG_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOG_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOG_RES_CORRUPTCTX;
                }
                else
                {
                    /* Get storage info */
                    l_eDBCRes = eFSS_LOGC_Format(&p_ptCtx->tLOGCCtx);
                    l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);
                }
            }
        }
	}

	return l_eRes;
}


e_eFSS_LOG_RES eFSS_LOG_AddLog(t_eFSS_LOG_Ctx* const p_ptCtx, uint16_t p_uElemL, uint8_t* p_puRawVal)
{
	/* Local return variable */
    e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES  l_eDBCRes;

    /* Local variable for calculation */
    t_eFSS_TYPE_StorBuf l_tBuff;
    t_eFSS_TYPE_StorSet l_tStorSet;
    bool_t l_bIsInit;
    bool_t l_bIsPageMod;
    uint32_t l_uIndexPToCheck;
    uint32_t l_uMaxPIndex;
    uint32_t l_uMaxElemPageIndex;
    uint32_t l_uElemDone;
    uint32_t l_uElemInPageDone;
    uint16_t l_uCurrEleVer;
    uint8_t* l_puCurrEleData;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eDBCRes = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, &l_bIsInit);
        l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);

        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOG_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOG_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOG_RES_CORRUPTCTX;
                }
                else
                {
                    /* Get storage info */
                    l_eDBCRes = eFSS_LOGC_AddLog(&p_ptCtx->tLOGCCtx, p_puRawVal, p_uElemL);
                    l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_GetLogOfASpecificPage(t_eFSS_LOG_Ctx* const p_ptCtx, uint32_t p_uindx, uint8_t* p_puBuf,
                                              uint32_t* p_puBufL, uint32_t p_uBufMaxL)
{
	/* Local return variable */
    e_eFSS_LOG_RES l_eRes;
    e_eFSS_LOGC_RES  l_eDBCRes;

    /* Local variable for calculation */
    t_eFSS_TYPE_StorBuf l_tBuff;
    t_eFSS_TYPE_StorSet l_tStorSet;
    bool_t l_bIsInit;
    bool_t l_bIsPageMod;
    uint32_t l_uIndexPToCheck;
    uint32_t l_uMaxPIndex;
    uint32_t l_uMaxElemPageIndex;
    uint32_t l_uElemDone;
    uint32_t l_uElemInPageDone;
    uint16_t l_uCurrEleVer;
    uint8_t* l_puCurrEleData;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSS_LOG_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
        l_eDBCRes = eFSS_LOGC_IsInit(&p_ptCtx->tLOGCCtx, &l_bIsInit);
        l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);

        if( e_eFSS_LOG_RES_OK == l_eRes )
        {
            if( false == l_bIsInit )
            {
                l_eRes = e_eFSS_LOG_RES_NOINITLIB;
            }
            else
            {
                /* Check internal status validity */
                if( false == eFSS_LOG_IsStatusStillCoherent(p_ptCtx) )
                {
                    l_eRes = e_eFSS_LOG_RES_CORRUPTCTX;
                }
                else
                {
                    /* Get storage info */
                    l_eDBCRes = eFSS_LOGC_GetLogOfASpecificPage(&p_ptCtx->tLOGCCtx, p_uindx, p_puBuf,
                                                p_puBufL, p_uBufMaxL);
                    l_eRes = eFSS_LOG_DBCtoDBFLRes(l_eDBCRes);
                }
            }
        }
	}

	return l_eRes;
}



/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eFSS_LOG_IsStatusStillCoherent(const t_eFSS_LOG_Ctx* p_ptCtx)
{
    bool_t l_eRes;

	/* Check context validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = false;
	}
	else
	{
        l_eRes = true;
	}

    return l_eRes;
}

static e_eFSS_LOG_RES eFSS_LOG_DBCtoDBFLRes(const e_eFSS_LOGC_RES p_eDBCRes)
{
    e_eFSS_LOG_RES l_eRes;

    switch(p_eDBCRes)
    {
        case e_eFSS_LOGC_RES_OK:
        {
            l_eRes = e_eFSS_LOG_RES_OK;
            break;
        }

        case e_eFSS_LOGC_RES_BADPARAM:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }

        default:
        {
            l_eRes = e_eFSS_LOG_RES_BADPARAM;
            break;
        }
    }

    return l_eRes;
}