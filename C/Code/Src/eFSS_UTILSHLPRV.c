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

e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_ClonePage( t_eFSS_TYPE_CbCtx* const p_ptCbCtx, const uint32_t p_uReTry,
                                                 uint8_t* const p_puDataW, const uint32_t p_uDataWLen,
                                                 uint8_t* const p_puDataR, const uint32_t p_uDataRLen,
                                                 const uint32_t p_uOrigIndx, const uint32_t p_uDestIndx )
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
            ( p_uDataRLen != p_uDataWLen ) || ( p_uReTry <= 0u ) || ( p_uOrigIndx == p_uDestIndx) )
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_BADPARAM;
        }
        else
        {
            /* Read the page to clone */
            l_eResLL = eFSS_UTILSLLPRV_ReadPage(p_ptCbCtx, p_uOrigIndx, p_puDataR, p_uDataRLen, p_uReTry);
            l_eRes = eFSS_UTILSHLPRV_LLtoHLRes(l_eResLL);
            if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes)
            {
                /* Crc calculated, set it */
                l_uCrcCalc = 0u;
                l_eRes = eFSS_UTILSHLPRV_SetCrcMetaInBuff(p_puDataW, p_uDataWLen, l_uCrcCalc);

                if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes )
                {
                    /* Write the page */
                    l_eResLL = eFSS_UTILSLLPRV_WritePage(p_ptCbCtx, p_puDataR, p_uDataRLen, p_puDataW, p_uDataWLen,
                                                         p_uDestIndx, p_uReTry);
                    l_eRes = eFSS_UTILSHLPRV_LLtoHLRes(l_eResLL);
                }
            }
        }
    }

    return l_eRes;
}

e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_VerifyNRipristBkup( t_eFSS_TYPE_CbCtx* const p_ptCbCtx, const uint32_t p_uReTry,
                                                          uint8_t* const p_puDataW, const uint32_t p_uDataWLen,
                                                          uint8_t* const p_puDataR, const uint32_t p_uDataRLen,
                                                          const uint32_t p_uOrigIndx, const uint32_t p_uBackupIndx )
{
    /* Local variable */
	e_eFSS_UTILSHLPRV_RES l_eRes;
    e_eFSS_UTILSLLPRV_RES l_eResLL;

    bool_t isOriginValid;
    bool_t isBackupValid;

	/* Check pointer validity */
	if( ( NULL == p_ptCbCtx ) || ( NULL == p_puDataW ) || ( NULL == p_puDataR ) )
	{
		l_eRes = e_eFSS_UTILSHLPRV_RES_BADPOINTER;
	}
    else
    {
        /* Check data validity */
        if( ( p_uDataWLen <= EFSS_PAGEMETASIZE ) || ( p_uDataRLen <= EFSS_PAGEMETASIZE ) ||
            ( p_uDataRLen != p_uDataWLen ) || ( p_uReTry <= 0u ) || ( p_uOrigIndx == p_uBackupIndx) )
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_BADPARAM;
        }
        else
        {
            /* Read origin page */
            l_eResLL = eFSS_UTILSLLPRV_ReadPage(p_ptCbCtx, p_uOrigIndx, p_puDataW, p_uDataWLen, p_uReTry);
            l_eRes = eFSS_UTILSHLPRV_LLtoHLRes(l_eResLL);

            if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes)
            {
                /* Read backup page */
                l_eResLL = eFSS_UTILSLLPRV_ReadPage(p_ptCbCtx, p_uBackupIndx, p_puDataR, p_uDataRLen, p_uReTry);
                l_eRes = eFSS_UTILSHLPRV_LLtoHLRes(l_eResLL);

                if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes )
                {
                    /* Verify origin integrity */
                    l_eRes = eFSS_UTILSHLPRV_IsValidPage(p_ptCbCtx, p_uOrigIndx, p_puDataW, p_uDataWLen, p_uReTry,
                                                         &isOriginValid);
                    if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes )
                    {
                        /* Verify backup integrity */
                        l_eRes = eFSS_UTILSHLPRV_IsValidPage(p_ptCbCtx, p_uBackupIndx, p_puDataR, p_uDataRLen, p_uReTry,
                                                            &isBackupValid);

                        if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes )
                        {
                            if( ( true == isOriginValid ) && ( true == isBackupValid ) )
                            {
                                /* Both page are valid, are they identical? */
                                if( 0 == memcmp(p_puDataW, p_puDataR, p_uDataRLen ) )
                                {
                                    /* Page are equals*/
                                    l_eRes = e_eFSS_UTILSHLPRV_RES_OK;
                                }
                                else
                                {
                                    /* Page are not equals, copy origin in backup */
                                    l_eResLL = eFSS_UTILSLLPRV_WritePage(p_ptCbCtx, p_puDataW, p_uDataWLen, p_puDataR,
                                                                        p_uDataRLen, p_uBackupIndx, p_uReTry);
                                    l_eRes = eFSS_UTILSHLPRV_LLtoHLRes(l_eResLL);

                                    if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes )
                                    {
                                        l_eRes = e_eFSS_UTILSLLPRV_RES_OK_BKP_RCVRD;
                                    }
                                }
                            }
                            else if( ( false == isOriginValid ) && ( true == isBackupValid ) )
                            {
                                /* Origin is not valid, repristinate from backup */
                                l_eResLL = eFSS_UTILSLLPRV_WritePage(p_ptCbCtx, p_puDataR, p_uDataRLen, p_puDataW,
                                                                     p_uDataWLen, p_uOrigIndx, p_uReTry);
                                l_eRes = eFSS_UTILSHLPRV_LLtoHLRes(l_eResLL);

                                if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes )
                                {
                                    l_eRes = e_eFSS_UTILSLLPRV_RES_OK_BKP_RCVRD;
                                }
                            }
                            else if( ( true == isOriginValid ) && ( false == isBackupValid ) )
                            {
                                /* Backup is not valid, repristinate from origin */
                                l_eResLL = eFSS_UTILSLLPRV_WritePage(p_ptCbCtx, p_puDataW, p_uDataWLen, p_puDataR,
                                                                     p_uDataRLen, p_uBackupIndx, p_uReTry);
                                l_eRes = eFSS_UTILSHLPRV_LLtoHLRes(l_eResLL);

                                if( e_eFSS_UTILSHLPRV_RES_OK == l_eRes )
                                {
                                    l_eRes = e_eFSS_UTILSLLPRV_RES_OK_BKP_RCVRD;
                                }
                            }
                            else
                            {
                                /* No a single valid pages found */
                                l_eRes = e_eFSS_UTILSLLPRV_RES_NOTVALIDPAGE;
                            }
                        }
                    }
                }
            }
        }
    }

    return l_eRes;
}

e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_CrcDigest( t_eFSS_TYPE_CbCtx* const p_ptCbCtx, const uint32_t p_uSeedOrCrc,
                                                 uint8_t* const p_puData, const uint32_t p_uDataLen,
                                                 uint32_t* const p_puCrcCal )
{
    /* Local variable */
	e_eFSS_UTILSHLPRV_RES l_eRes;
    e_eFSS_UTILSLLPRV_RES l_eResLL;

	/* Check pointer validity */
	if( ( NULL == p_ptCbCtx ) || ( NULL == p_puData ) || ( NULL == p_puCrcCal ) )
	{
		l_eRes = e_eFSS_UTILSHLPRV_RES_BADPOINTER;
	}
    else
    {
        /* Check data validity */
        if( p_uDataLen <= 0u )
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_BADPARAM;
        }
        else
        {
            /* Read the page to clone */
            l_eResLL = eFSS_UTILSLLPRV_CalcCrc32CS(p_ptCbCtx, p_uSeedOrCrc, p_puData, p_uDataLen, p_puCrcCal);
            l_eRes = eFSS_UTILSHLPRV_LLtoHLRes(l_eResLL);
        }
    }

    return l_eRes;
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