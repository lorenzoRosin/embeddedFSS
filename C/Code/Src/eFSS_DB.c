/**
 * @file       eFSS_DB.c
 *
 * @brief      High level utils for fail safe storage
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_DB.h"



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_GetMetaFromBuff(const uint8_t* pageBuff, const uint32_t p_pageL,
                                                      t_eFSS_TYPE_PageMeta* const pagePrm)
{
	/* Local variable */
	e_eFSS_UTILSHLPRV_RES l_eRes;
    uint32_t l_uComulIndx;
    uint32_t l_uTemp;

	/* Check pointer validity */
	if( ( NULL == pageBuff ) || ( NULL == pagePrm )  )
	{
		l_eRes = e_eFSS_UTILSHLPRV_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity */
        if( p_pageL < EFSS_PAGEMETASIZE )
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_BADPARAM;
        }
        else
        {
            /* Initialize internal status */
            l_uComulIndx = p_pageL - EFSS_PAGEMETASIZE;

            /* Init return data */
            pagePrm->uPageType = 0u;
            pagePrm->uPageSubType = 0u;
            pagePrm->uPageVersion = 0u;
            pagePrm->uPageByteFilled = 0u;
            pagePrm->uPagePresentElement = 0u;
            pagePrm->uPageSequentialN = 0u;
            pagePrm->uPageMagicNumber = 0u;
            pagePrm->uPageCrc = 0u;

            /* Copy data Little endian */
            l_uTemp = (uint32_t) pageBuff[l_uComulIndx];
            pagePrm->uPageType |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 8u  );
            pagePrm->uPageType |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 16u  );
            pagePrm->uPageType |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 24u  );
            pagePrm->uPageType |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            /* Copy data Little endian */
            l_uTemp = (uint32_t) pageBuff[l_uComulIndx];
            pagePrm->uPageSubType |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 8u  );
            pagePrm->uPageSubType |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 16u  );
            pagePrm->uPageSubType |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 24u  );
            pagePrm->uPageSubType |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            /* Copy data Little endian */
            l_uTemp = (uint32_t) pageBuff[l_uComulIndx];
            pagePrm->uPageVersion |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 8u  );
            pagePrm->uPageVersion |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 16u  );
            pagePrm->uPageVersion |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 24u  );
            pagePrm->uPageVersion |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            /* Copy data Little endian */
            l_uTemp = (uint32_t) pageBuff[l_uComulIndx];
            pagePrm->uPageByteFilled |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 8u  );
            pagePrm->uPageByteFilled |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 16u  );
            pagePrm->uPageByteFilled |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 24u  );
            pagePrm->uPageByteFilled |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            /* Copy data Little endian */
            l_uTemp = (uint32_t) pageBuff[l_uComulIndx];
            pagePrm->uPagePresentElement |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 8u  );
            pagePrm->uPagePresentElement |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 16u  );
            pagePrm->uPagePresentElement |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 24u  );
            pagePrm->uPagePresentElement |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            /* Copy data Little endian */
            l_uTemp = (uint32_t) pageBuff[l_uComulIndx];
            pagePrm->uPageSequentialN |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 8u  );
            pagePrm->uPageSequentialN |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 16u  );
            pagePrm->uPageSequentialN |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 24u  );
            pagePrm->uPageSequentialN |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            /* Copy data Little endian */
            l_uTemp = (uint32_t) pageBuff[l_uComulIndx];
            pagePrm->uPageMagicNumber |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 8u  );
            pagePrm->uPageMagicNumber |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 16u  );
            pagePrm->uPageMagicNumber |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 24u  );
            pagePrm->uPageMagicNumber |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            /* Copy data Little endian */
            l_uTemp = (uint32_t) pageBuff[l_uComulIndx];
            pagePrm->uPageCrc |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 8u  );
            pagePrm->uPageCrc |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 16u  );
            pagePrm->uPageCrc |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 24u  );
            pagePrm->uPageCrc |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            l_eRes = e_eFSS_UTILSHLPRV_RES_OK;
        }
	}

	return l_eRes;
}



