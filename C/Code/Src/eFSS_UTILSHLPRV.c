/**
 * @file       eFSS_UTILSHLPRV.h
 *
 * @brief      High level utils for fail safe storage
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_UTILSHLPRV.h"
#include "eFSS_UTILSLLPRV.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_LLtoHLRes(const e_eFSS_UTILSLLPRV_RES p_eLLRes);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_GetMetaFromBuff(const uint8_t* p_puPageBuf, const uint32_t p_uPageL,
                                                      t_eFSS_TYPE_PageMeta* const p_ptPagePrm)
{
	/* Local variable */
	e_eFSS_UTILSHLPRV_RES l_eRes;
    uint32_t l_uComulIndx;
    uint32_t l_uTemp;

	/* Check pointer validity */
	if( ( NULL == p_puPageBuf ) || ( NULL == p_ptPagePrm )  )
	{
		l_eRes = e_eFSS_UTILSHLPRV_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity */
        if( p_uPageL <= EFSS_PAGEMETASIZE )
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_BADPARAM;
        }
        else
        {
            /* Initialize internal status */
            l_uComulIndx = p_uPageL - EFSS_PAGEMETASIZE;

            /* Init return data */
            p_ptPagePrm->uPageType = 0u;
            p_ptPagePrm->uPageSubType = 0u;
            p_ptPagePrm->uPageVersion = 0u;
            p_ptPagePrm->uPageUseSpecific1 = 0u;
            p_ptPagePrm->uPageUseSpecific2 = 0u;
            p_ptPagePrm->uPageSequentialN = 0u;
            p_ptPagePrm->uPageMagicNumber = 0u;
            p_ptPagePrm->uPageCrc = 0u;

            /* --- Copy data Little endian */
            l_uTemp = (uint32_t) p_puPageBuf[l_uComulIndx];
            p_ptPagePrm->uPageType |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 8u  );
            p_ptPagePrm->uPageType |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 16u  );
            p_ptPagePrm->uPageType |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 24u  );
            p_ptPagePrm->uPageType |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            /* --- Copy data Little endian */
            l_uTemp = (uint32_t) p_puPageBuf[l_uComulIndx];
            p_ptPagePrm->uPageSubType |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 8u  );
            p_ptPagePrm->uPageSubType |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 16u  );
            p_ptPagePrm->uPageSubType |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 24u  );
            p_ptPagePrm->uPageSubType |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            /* --- Copy data Little endian */
            l_uTemp = (uint32_t) p_puPageBuf[l_uComulIndx];
            p_ptPagePrm->uPageVersion |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 8u  );
            p_ptPagePrm->uPageVersion |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 16u  );
            p_ptPagePrm->uPageVersion |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 24u  );
            p_ptPagePrm->uPageVersion |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            /* --- Copy data Little endian */
            l_uTemp = (uint32_t) p_puPageBuf[l_uComulIndx];
            p_ptPagePrm->uPageUseSpecific1 |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 8u  );
            p_ptPagePrm->uPageUseSpecific1 |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 16u  );
            p_ptPagePrm->uPageUseSpecific1 |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 24u  );
            p_ptPagePrm->uPageUseSpecific1 |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            /* --- Copy data Little endian */
            l_uTemp = (uint32_t) p_puPageBuf[l_uComulIndx];
            p_ptPagePrm->uPageUseSpecific2 |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 8u  );
            p_ptPagePrm->uPageUseSpecific2 |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 16u  );
            p_ptPagePrm->uPageUseSpecific2 |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 24u  );
            p_ptPagePrm->uPageUseSpecific2 |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            /* --- Copy data Little endian */
            l_uTemp = (uint32_t) p_puPageBuf[l_uComulIndx];
            p_ptPagePrm->uPageSequentialN |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 8u  );
            p_ptPagePrm->uPageSequentialN |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 16u  );
            p_ptPagePrm->uPageSequentialN |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 24u  );
            p_ptPagePrm->uPageSequentialN |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            /* --- Copy data Little endian */
            l_uTemp = (uint32_t) p_puPageBuf[l_uComulIndx];
            p_ptPagePrm->uPageMagicNumber |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 8u  );
            p_ptPagePrm->uPageMagicNumber |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 16u  );
            p_ptPagePrm->uPageMagicNumber |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 24u  );
            p_ptPagePrm->uPageMagicNumber |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            /* --- Copy data Little endian */
            l_uTemp = (uint32_t) p_puPageBuf[l_uComulIndx];
            p_ptPagePrm->uPageCrc |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 8u  );
            p_ptPagePrm->uPageCrc |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 16u  );
            p_ptPagePrm->uPageCrc |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) p_puPageBuf[l_uComulIndx] ) << 24u  );
            p_ptPagePrm->uPageCrc |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            l_eRes = e_eFSS_UTILSHLPRV_RES_OK;
        }
	}

	return l_eRes;
}

e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_SetMetaInBuff(uint8_t* const p_puPageBuf, const uint32_t p_uPageL,
                                                    t_eFSS_TYPE_PageMeta* const p_ptPagePrm)
{
	/* Local variable */
	e_eFSS_UTILSHLPRV_RES l_eRes;
    uint32_t l_uComulIndx;

	/* Check pointer validity */
	if( ( NULL == p_puPageBuf ) || ( NULL == p_ptPagePrm )  )
	{
		l_eRes = e_eFSS_UTILSHLPRV_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity */
        if( p_uPageL <= EFSS_PAGEMETASIZE )
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_BADPARAM;
        }
        else
        {
            /* Initialize internal status */
            l_uComulIndx = p_uPageL - EFSS_PAGEMETASIZE;

            /* --- Copy data Little endian */
            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageType        ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageType >> 8u  ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageType >> 16u ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageType >> 24u ) & 0x000000FFu );
            l_uComulIndx++;

            /* --- Copy data Little endian */
            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageSubType        ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageSubType >> 8u  ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageSubType >> 16u ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageSubType >> 24u ) & 0x000000FFu );
            l_uComulIndx++;

            /* --- Copy data Little endian */
            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageVersion        ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageVersion >> 8u  ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageVersion >> 16u ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageVersion >> 24u ) & 0x000000FFu );
            l_uComulIndx++;

            /* --- Copy data Little endian */
            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageUseSpecific1        ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageUseSpecific1 >> 8u  ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageUseSpecific1 >> 16u ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageUseSpecific1 >> 24u ) & 0x000000FFu );
            l_uComulIndx++;

            /* --- Copy data Little endian */
            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageUseSpecific2        ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageUseSpecific2 >> 8u  ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageUseSpecific2 >> 16u ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageUseSpecific2 >> 24u ) & 0x000000FFu );
            l_uComulIndx++;

            /* --- Copy data Little endian */
            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageSequentialN        ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageSequentialN >> 8u  ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageSequentialN >> 16u ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageSequentialN >> 24u ) & 0x000000FFu );
            l_uComulIndx++;

            /* --- Copy data Little endian */
            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageMagicNumber        ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageMagicNumber >> 8u  ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageMagicNumber >> 16u ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageMagicNumber >> 24u ) & 0x000000FFu );
            l_uComulIndx++;

            /* --- Copy data Little endian */
            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageCrc        ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageCrc >> 8u  ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageCrc >> 16u ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_ptPagePrm->uPageCrc >> 24u ) & 0x000000FFu );
            l_uComulIndx++;

            /* All ok */
            l_eRes = e_eFSS_UTILSHLPRV_RES_OK;
        }
	}

	return l_eRes;
}

e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_SetCrcMetaInBuff(uint8_t* const p_puPageBuf, const uint32_t p_uPageL,
                                                       const uint32_t p_uCrcToSet)
{
	/* Local variable */
	e_eFSS_UTILSHLPRV_RES l_eRes;
    uint32_t l_uComulIndx;

	/* Check pointer validity */
	if( NULL == p_puPageBuf )
	{
		l_eRes = e_eFSS_UTILSHLPRV_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity */
        if( p_uPageL <= EFSS_PAGEMETASIZE )
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_BADPARAM;
        }
        else
        {
            /* Initialize internal status */
            l_uComulIndx = p_uPageL - sizeof(uint32_t);

            /* --- Copy data Little endian */
            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_uCrcToSet        ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_uCrcToSet >> 8u  ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_uCrcToSet >> 16u ) & 0x000000FFu );
            l_uComulIndx++;

            p_puPageBuf[l_uComulIndx] = (uint8_t) ( ( p_uCrcToSet >> 24u ) & 0x000000FFu );
            l_uComulIndx++;

            /* All ok */
            l_eRes = e_eFSS_UTILSHLPRV_RES_OK;
        }
	}

	return l_eRes;
}

e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_CalcMetaCrcInBuff(t_eFSS_TYPE_CbCtx* const p_ptCbCtx, uint8_t* const p_puPageBuf,
                                                        const uint32_t p_uPageL, uint32_t* const p_puCrcCalc)
{
	/* Local variable */
	e_eFSS_UTILSHLPRV_RES l_eRes;
    e_eFSS_UTILSLLPRV_RES l_eResLL;
    uint32_t l_uTemp;

	/* Check pointer validity */
	if( ( NULL == p_ptCbCtx ) || ( NULL == p_puPageBuf ) || ( NULL == p_puCrcCalc ) )
	{
		l_eRes = e_eFSS_UTILSHLPRV_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity */
        if( p_uPageL <= EFSS_PAGEMETASIZE )
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_BADPARAM;
        }
        else
        {
            /* Esclude from the calculation the CRC, that is placed in the last 4 byte */
            l_uTemp = p_uPageL - sizeof(uint32_t);

            /* Calculate CRC */
            l_eResLL = eFSS_UTILSLLPRV_CalcCrc32(p_ptCbCtx, p_puPageBuf, l_uTemp, p_puCrcCalc);
            l_eRes = eFSS_UTILSHLPRV_LLtoHLRes(l_eResLL);
        }
	}

	return l_eRes;
}

e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_SetMetaInBuffNUpdCrc(uint8_t* const p_puPageBuf, const uint32_t p_uPageL,
                                                           t_eFSS_TYPE_PageMeta* const p_ptPagePrm,
                                                           t_eFSS_TYPE_CbCtx* const p_ptCbCtx)
{
	/* Local variable */
	e_eFSS_UTILSHLPRV_RES l_eRes;
    uint32_t l_uCrcCalc;

	/* Check pointer validity */
	if( ( NULL == p_puPageBuf ) || ( NULL == p_ptPagePrm )  )
	{
		l_eRes = e_eFSS_UTILSHLPRV_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity */
        if( p_uPageL <= EFSS_PAGEMETASIZE )
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_BADPARAM;
        }
        else
        {
            /* Set the parameter in the ram buffer */
            l_eRes = eFSS_UTILSHLPRV_SetMetaInBuff(p_puPageBuf, p_uPageL, p_ptPagePrm);

            if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes)
            {
                /* Calc crc value of the page */
                l_eRes = eFSS_UTILSHLPRV_CalcMetaCrcInBuff(p_ptCbCtx, p_puPageBuf, p_uPageL, &l_uCrcCalc);

                if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes)
                {
                    /* Crc calculated, set it */
                    l_eRes = eFSS_UTILSHLPRV_SetCrcMetaInBuff(p_puPageBuf, p_uPageL, l_uCrcCalc);
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_IsValidPageInBuff(t_eFSS_TYPE_CbCtx* const p_ptCbCtx, uint8_t* const p_puPageBuf,
                                                        const uint32_t p_uPageL, bool_t* const p_pbIsValid)
{
	/* Local variable */
	e_eFSS_UTILSHLPRV_RES l_eRes;
    t_eFSS_TYPE_PageMeta l_tPagePrm;
    uint32_t l_uCrcCalc;

	/* Check pointer validity */
	if( ( NULL == p_ptCbCtx ) || ( NULL == p_puPageBuf ) || ( NULL == p_pbIsValid ) )
	{
		l_eRes = e_eFSS_UTILSHLPRV_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity */
        if( p_uPageL <= EFSS_PAGEMETASIZE )
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_BADPARAM;
        }
        else
        {
            /* Get p_puPageBuf param from the ram p_puPageBuf */
            l_eRes = eFSS_UTILSHLPRV_GetMetaFromBuff(p_puPageBuf, p_uPageL, &l_tPagePrm);

            if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes )
            {
                /* Calculate the p_puPageBuf param CRC */
                l_eRes = eFSS_UTILSHLPRV_CalcMetaCrcInBuff(p_ptCbCtx, p_puPageBuf, p_uPageL, &l_uCrcCalc);

                if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes )
                {
                    /* Verify if calculated CRC is equal to the stored one, check if magic number is equal to the
                    * setted one also */
                    if( (l_uCrcCalc == l_tPagePrm.uPageCrc) && ( EFSS_PAGEMAGICNUMBER == l_tPagePrm.uPageMagicNumber ) )
                    {
                        *p_pbIsValid = true;
                    }
                    else
                    {
                        *p_pbIsValid = false;
                    }

                    l_eRes = e_eFSS_UTILSHLPRV_RES_OK;
                }
            }
        }
	}

	return l_eRes;
}

e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_IsValidPage(t_eFSS_TYPE_CbCtx* const p_ptCbCtx, const uint32_t p_uPageIndx,
                                                  uint8_t* const p_puPageBuf, const uint32_t p_uPageL,
                                                  const uint32_t p_uReTry, bool_t* const p_pbIsValid)
{
	/* Local variable */
	e_eFSS_UTILSHLPRV_RES l_eRes;
    e_eFSS_UTILSLLPRV_RES l_eResLL;

	/* Check pointer validity */
	if( ( NULL == p_ptCbCtx ) || ( NULL == p_puPageBuf ) || ( NULL == p_pbIsValid ) )
	{
		l_eRes = e_eFSS_UTILSHLPRV_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity */
        if( p_uPageL <= EFSS_PAGEMETASIZE )
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_BADPARAM;
        }
        else
        {
            /* Get pageBuff param from the ram pageBuff */
            l_eResLL = eFSS_UTILSLLPRV_ReadPage( p_ptCbCtx, p_uPageIndx, p_puPageBuf, p_uPageL, p_uReTry );
            l_eRes = eFSS_UTILSHLPRV_LLtoHLRes(l_eResLL);

            if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes )
            {
                /* Calculate the pageBuff param CRC */
                l_eRes = eFSS_UTILSHLPRV_IsValidPageInBuff(p_ptCbCtx, p_puPageBuf, p_uPageL, p_pbIsValid);
            }
        }
	}

	return l_eRes;
}

e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_WritePagePrmNUpCrc(t_eFSS_TYPE_CbCtx* const p_ptCbCtx, const uint32_t p_uPageIndx,
                                                         uint8_t* const p_puDataW, const uint32_t p_uDataWLen,
                                                         uint8_t* const p_puDataR, const uint32_t p_uDataRLen,
                                                         t_eFSS_TYPE_PageMeta* const p_ptPagePrm,
                                                         const uint32_t p_uReTry)
{
    /* Local variable */
	e_eFSS_UTILSHLPRV_RES l_eRes;
    e_eFSS_UTILSLLPRV_RES l_eResLL;

	/* Check pointer validity */
	if( ( NULL == p_ptCbCtx ) || ( NULL == p_puDataW ) || ( NULL == p_puDataR ) || ( NULL == p_ptPagePrm ) )
	{
		l_eRes = e_eFSS_UTILSHLPRV_RES_BADPOINTER;
	}
    else
    {
        /* Check data validity */
        if( ( p_uDataWLen <= EFSS_PAGEMETASIZE ) || ( p_uDataRLen <= EFSS_PAGEMETASIZE ) ||
            ( p_uDataRLen != p_uDataWLen ) || ( p_uReTry <= 0u ) )
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_BADPARAM;
        }
        else
        {
            /* Set the page param and CRC in the page buffer */
            l_eRes = eFSS_UTILSHLPRV_SetMetaInBuffNUpdCrc(p_puDataW, p_uDataWLen, p_ptPagePrm, p_ptCbCtx);

            if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes )
            {
                /* Write the page */
                l_eResLL = eFSS_UTILSLLPRV_WritePage(p_ptCbCtx, p_puDataW, p_uDataWLen, p_puDataR, p_uDataRLen,
                                                     p_uPageIndx, p_uReTry);
                l_eRes = eFSS_UTILSHLPRV_LLtoHLRes(l_eResLL);
            }
        }
    }

    return l_eRes;
}

e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_WritePageNUpCrc(t_eFSS_TYPE_CbCtx* const p_ptCbCtx, const uint32_t p_uPageIndx,
                                                         uint8_t* const p_puDataW, const uint32_t p_uDataWLen,
                                                         uint8_t* const p_puDataR, const uint32_t p_uDataRLen,
                                                         const uint32_t p_uReTry)
{
    /* Local variable */
	e_eFSS_UTILSHLPRV_RES l_eRes;
    e_eFSS_UTILSLLPRV_RES l_eResLL;
    uint32_t l_uCrcCalc;

	/* Check pointer validity */
	if( ( NULL == p_ptCbCtx ) || ( NULL == p_puDataW ) || ( NULL == p_puDataR ) )
	{
		l_eRes = e_eFSS_UTILSHLPRV_RES_BADPOINTER;
	}
    else
    {
        /* Check data validity */
        if( ( p_uDataWLen <= EFSS_PAGEMETASIZE ) || ( p_uDataRLen <= EFSS_PAGEMETASIZE ) ||
            ( p_uDataRLen != p_uDataWLen ) || ( p_uReTry <= 0u ) )
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_BADPARAM;
        }
        else
        {
            /* Set the page param and CRC in the page buffer */
            l_eRes = eFSS_UTILSHLPRV_CalcMetaCrcInBuff(p_ptCbCtx, p_puDataW, p_uDataWLen, &l_uCrcCalc);

            if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes)
            {
                /* Crc calculated, set it */
                l_eRes = eFSS_UTILSHLPRV_SetCrcMetaInBuff(p_puDataW, p_uDataWLen, l_uCrcCalc);

                if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes )
                {
                    /* Write the page */
                    l_eResLL = eFSS_UTILSLLPRV_WritePage(p_ptCbCtx, p_puDataW, p_uDataWLen, p_puDataR, p_uDataRLen,
                                                         p_uPageIndx, p_uReTry);
                    l_eRes = eFSS_UTILSHLPRV_LLtoHLRes(l_eResLL);
                }
            }
        }
    }

    return l_eRes;
}

e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_ReadPageNPrm(t_eFSS_TYPE_CbCtx* const p_ptCbCtx, const uint32_t p_uPageIndx,
                                                   uint8_t* const p_puDataR, const uint32_t p_uDataRLen,
                                                   t_eFSS_TYPE_PageMeta* const p_ptPagePrm, const uint32_t p_uReTry)
{
    /* Local variable */
	e_eFSS_UTILSHLPRV_RES l_eRes;
    e_eFSS_UTILSLLPRV_RES l_eResLL;

	/* Check pointer validity */
	if( ( NULL == p_ptCbCtx ) || ( NULL == p_puDataR ) || ( NULL == p_ptPagePrm )  )
	{
		l_eRes = e_eFSS_UTILSHLPRV_RES_BADPOINTER;
	}
    else
    {
        /* Check data validity */
        if( p_uDataRLen <= EFSS_PAGEMETASIZE )
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_BADPARAM;
        }
        else
        {
            /* Read the page */
            l_eResLL = eFSS_UTILSLLPRV_ReadPage(p_ptCbCtx, p_uPageIndx, p_puDataR, p_uDataRLen, p_uReTry);
            l_eRes = eFSS_UTILSHLPRV_LLtoHLRes(l_eResLL);

            if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes )
            {
                /* Copy poge param */
                l_eRes = eFSS_UTILSHLPRV_GetMetaFromBuff(p_puDataR, p_uDataRLen, p_ptPagePrm);
            }
        }
    }

    return l_eRes;
}

















e_eFSS_Res cloneAPage(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, uint8_t* const pageBuff,
                      uint8_t* const suppBuff, const uint32_t origIndx, const uint32_t destIndx)
{
    /* Local variable */
    e_eFSS_Res returnVal;

    /* Check for NULL pointer */
    if( ( NULL == pageBuff )|| ( NULL == suppBuff ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Check for parameter validity */
        if( ( origIndx >= pginfo.nOfPages ) || ( destIndx >= pginfo.nOfPages )  || ( origIndx == destIndx ) )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Read the page that need to be cloned */
            returnVal = readPageLL( pginfo, cbHld, origIndx, pageBuff );

            if( EFSS_RES_OK == returnVal )
            {
                /* Erase physical page */
                returnVal = erasePageLL(pginfo, cbHld, destIndx );
                if( EFSS_RES_OK == returnVal )
                {
                    /* Write the pageBuff in the physical page */
                    returnVal = writePageLL(pginfo, cbHld, destIndx, pageBuff, suppBuff );
                }
            }
        }
    }

    return returnVal;
}









































e_eFSS_Res verifyAndRipristinateBkup(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, uint8_t* const pageOrig,
                                     uint8_t* const pageBkup, const uint32_t origIndx, const uint32_t backupIndx)
{
    /* Local variable */
    e_eFSS_Res returnVal;
    e_eFSS_Res readRetValOr;
    e_eFSS_Res readRetValBk;

    /* Check for NULL pointer */
    if( ( NULL == pageOrig )|| ( NULL == pageBkup ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Check for parameter validity */
        if( ( origIndx >= pginfo.nOfPages ) || ( backupIndx >= pginfo.nOfPages )  || ( backupIndx == origIndx ) )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Read all pages before start doing test */
            readRetValOr = readPageLL( pginfo, cbHld, origIndx, pageOrig );
            readRetValBk = readPageLL( pginfo, cbHld, backupIndx, pageBkup );

            if( ( EFSS_RES_OK == readRetValOr ) && ( EFSS_RES_OK == readRetValBk ) )
            {
                /* Verify all pages integrity */
                readRetValOr = isValidPageInBuff(pginfo, cbHld, pageOrig);
                readRetValBk = isValidPageInBuff(pginfo, cbHld, pageBkup);

                if( ( ( EFSS_RES_NOTVALIDPAGE == readRetValOr ) || ( EFSS_RES_OK == readRetValOr ) ) &&
                    ( ( EFSS_RES_NOTVALIDPAGE == readRetValBk ) || ( EFSS_RES_OK == readRetValBk ) ) )
                {
                    if( ( EFSS_RES_OK == readRetValOr ) && ( EFSS_RES_OK == readRetValBk ) )
                    {
                        /* Two pages are equal, are they identical? */
                        if( 0 == memcmp(pageOrig, pageBkup, pginfo.pageSize ) )
                        {
                            /* Page are equals*/
                            returnVal = EFSS_RES_OK;
                        }
                        else
                        {
                            /* Page are not equals, copy origin to backup */
                            returnVal = writePageLL(pginfo, cbHld, backupIndx, pageOrig, pageBkup );
                            if( EFSS_RES_OK == returnVal )
                            {
                                returnVal = EFSS_RES_OKBKPRCVRD;
                            }
                        }
                    }
                    else if( ( EFSS_RES_NOTVALIDPAGE == readRetValOr ) && ( EFSS_RES_OK == readRetValBk ) )
                    {
                        /* Origin is not valid, repristinate from backup */
                        returnVal = writePageLL(pginfo, cbHld, origIndx, pageBkup, pageOrig );
                        if( EFSS_RES_OK == returnVal )
                        {
                            returnVal = EFSS_RES_OKBKPRCVRD;
                        }
                    }
                    else if( ( EFSS_RES_OK == readRetValOr ) && ( EFSS_RES_NOTVALIDPAGE == readRetValBk ) )
                    {
                        /* Backup is not valid, repristinate from origin */
                        returnVal = writePageLL(pginfo, cbHld, backupIndx, pageOrig, pageBkup );
                        if( EFSS_RES_OK == returnVal )
                        {
                            returnVal = EFSS_RES_OKBKPRCVRD;
                        }
                    }
                    else
                    {
                        /* No a single valid pages found */
                        returnVal = EFSS_RES_NOTVALIDPAGE;
                    }
                }
                else
                {
                    returnVal = EFSS_RES_BADPARAM;
                }
            }
            else
            {
                returnVal = EFSS_RES_ERRORREAD;
            }
        }
    }

    return returnVal;
}
















/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_LLtoHLRes(const e_eFSS_UTILSLLPRV_RES p_eLLRes)
{
    e_eFSS_UTILSHLPRV_RES l_eRes;

    switch(p_eLLRes)
    {
        case e_eFSS_UTILSLLPRV_RES_OK:
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_OK;
            break;
        }

        case e_eFSS_UTILSLLPRV_RES_BADPARAM:
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_BADPARAM;
            break;
        }

        case e_eFSS_UTILSLLPRV_RES_BADPOINTER:
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_BADPOINTER;
            break;
        }

        case e_eFSS_UTILSLLPRV_RES_CLBCKERASEERR:
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_CLBCKERASEERR;
            break;
        }

        case e_eFSS_UTILSLLPRV_RES_CLBCKWRITEERR:
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_CLBCKWRITEERR;
            break;
        }

        case e_eFSS_UTILSLLPRV_RES_CLBCKREADERR:
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_CLBCKREADERR;
            break;
        }

        case e_eFSS_UTILSLLPRV_RES_CLBCKCRCERR:
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_CLBCKCRCERR;
            break;
        }

        case e_eFSS_UTILSLLPRV_RES_WRITENOMATCHREAD:
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_WRITENOMATCHREAD;
            break;
        }

        default:
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_CORRUPT;
            break;
        }
    }

    return l_eRes;
}
