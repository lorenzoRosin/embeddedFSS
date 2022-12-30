/**
 * @file       eFSS_UTILSHLPRV.c
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

e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_SetMetaInBuff(uint8_t* const pageBuff, const uint32_t p_pageL,
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

            /* Copy data Little endian */
            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType        ) & 0x000000FFu );
            l_uComulIndx++;

            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType >> 8u  ) & 0x000000FFu );
            l_uComulIndx++;

            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType >> 16u ) & 0x000000FFu );
            l_uComulIndx++;

            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType >> 24u ) & 0x000000FFu );
            l_uComulIndx++;

            /* Copy data Little endian */
            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType        ) & 0x000000FFu );
            l_uComulIndx++;

            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType >> 8u  ) & 0x000000FFu );
            l_uComulIndx++;

            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType >> 16u ) & 0x000000FFu );
            l_uComulIndx++;

            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType >> 24u ) & 0x000000FFu );
            l_uComulIndx++;

            /* Copy data Little endian */
            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType        ) & 0x000000FFu );
            l_uComulIndx++;

            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType >> 8u  ) & 0x000000FFu );
            l_uComulIndx++;

            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType >> 16u ) & 0x000000FFu );
            l_uComulIndx++;

            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType >> 24u ) & 0x000000FFu );
            l_uComulIndx++;

            /* Copy data Little endian */
            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType        ) & 0x000000FFu );
            l_uComulIndx++;

            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType >> 8u  ) & 0x000000FFu );
            l_uComulIndx++;

            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType >> 16u ) & 0x000000FFu );
            l_uComulIndx++;

            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType >> 24u ) & 0x000000FFu );
            l_uComulIndx++;

            /* Copy data Little endian */
            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType        ) & 0x000000FFu );
            l_uComulIndx++;

            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType >> 8u  ) & 0x000000FFu );
            l_uComulIndx++;

            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType >> 16u ) & 0x000000FFu );
            l_uComulIndx++;

            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType >> 24u ) & 0x000000FFu );
            l_uComulIndx++;

            /* Copy data Little endian */
            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType        ) & 0x000000FFu );
            l_uComulIndx++;

            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType >> 8u  ) & 0x000000FFu );
            l_uComulIndx++;

            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType >> 16u ) & 0x000000FFu );
            l_uComulIndx++;

            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType >> 24u ) & 0x000000FFu );
            l_uComulIndx++;

            /* Copy data Little endian */
            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType        ) & 0x000000FFu );
            l_uComulIndx++;

            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType >> 8u  ) & 0x000000FFu );
            l_uComulIndx++;

            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType >> 16u ) & 0x000000FFu );
            l_uComulIndx++;

            pageBuff[l_uComulIndx] = (uint8_t) ( ( pagePrm->uPageType >> 24u ) & 0x000000FFu );
            l_uComulIndx++;

            l_eRes = e_eFSS_UTILSHLPRV_RES_OK;
        }
	}

	return l_eRes;
}




















































e_eFSS_Res setCrcInPagePrmBuff(const s_eFSS_PgInfo pginfo, uint8_t* const pageBuff, const uint32_t crcToSet)
{
    /* Local variable */
    e_eFSS_Res returnVal;

    /* Check for NULL pointer */
    if( NULL == pageBuff )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Check for parameter validity */
        if( ( pginfo.pageSize < EFSS_MIN_PAGE_SIZE_BYTE ) || ( pginfo.pageSize > EFSS_MAX_PAGE_SIZE_BYTE ) )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Calculating offset */
            uint32_t offset5 = pginfo.pageSize - ( sizeof(uint32_t) );

            /* Copy CRC passed in to the ram buffer */
            (void)memcpy( &pageBuff[offset5], (const uint8_t*)&crcToSet, sizeof(uint32_t) );
            returnVal = EFSS_RES_OK;
        }
    }

    return returnVal;
}
























e_eFSS_Res calcPagePrmCrcInBuff(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, const uint8_t* pageBuff,
                                uint32_t* const crcCalc)
{
    /* Local variable */
    e_eFSS_Res returnVal;
    uint32_t pageCrcSizeToCalc;

    /* Check for NULL pointer */
    if( ( NULL == pageBuff ) || ( NULL == crcCalc ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Check for parameter validity */
        if( ( pginfo.pageSize < EFSS_MIN_PAGE_SIZE_BYTE ) || ( pginfo.pageSize > EFSS_MAX_PAGE_SIZE_BYTE ) )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Esclude from the calculation the CRC, that is placed in the last 4 byte */
            pageCrcSizeToCalc = pginfo.pageSize - sizeof(uint32_t);

            /* Calculate CRC */
            returnVal = calcCrcLL(cbHld, crcCalc, pageBuff, pageCrcSizeToCalc);
        }
    }

    return returnVal;
}

e_eFSS_Res setPagePrmInBuffNCrcUp(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, uint8_t* const pageBuff,
                                  const s_prv_pagePrm* pagePrm)
{
    /* Local variable */
    e_eFSS_Res returnVal;
    uint32_t crcCalc;

    /* Check for NULL pointer */
    if( ( NULL == pageBuff ) || ( NULL == pagePrm ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Set the parameter in the ram buffer */
        returnVal = setPagePrmInBuff(pginfo, pageBuff, pagePrm);

        if( EFSS_RES_OK == returnVal)
        {
            /* Calc crc value of the page */
            returnVal = calcPagePrmCrcInBuff(pginfo, cbHld, pageBuff, &crcCalc);

            if( EFSS_RES_OK == returnVal)
            {
                /* Crc calculated, set it */
                returnVal = setCrcInPagePrmBuff(pginfo, pageBuff, crcCalc);
            }
        }
    }

    return returnVal;
}

e_eFSS_Res isValidPageInBuff(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, const uint8_t* pageBuff)
{
    /* Local variable */
    e_eFSS_Res returnVal;
    s_prv_pagePrm prmPage;
    uint32_t crcCalc;

    /* Check for NULL pointer */
    if( NULL == pageBuff )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Get pageBuff param from the ram pageBuff */
        returnVal = getPagePrmFromBuff(pginfo, pageBuff, &prmPage);

        if( EFSS_RES_OK == returnVal )
        {
            /* Calculate the pageBuff param CRC */
            returnVal = calcPagePrmCrcInBuff(pginfo, cbHld, pageBuff, &crcCalc);

            if( EFSS_RES_OK == returnVal )
            {
                /* Verify if calculated CRC is equal to the stored one, check if magic number is equal to the
                * setted one also */
                if( (crcCalc == prmPage.pageCrc) && ( PARAM_32_MAGIC_NUMBER == prmPage.pageMagicNumber ) )
                {
                    /* Verify if the pageBuff type is what we are aspecting */
                    if( prmPage.pageType == ( (uint16_t)pginfo.pageType ) )
                    {
                        returnVal = EFSS_RES_OK;
                    }
                    else
                    {
                        returnVal = EFSS_RES_NOTVALIDPAGE;
                    }
                }
                else
                {
                    returnVal = EFSS_RES_NOTVALIDPAGE;
                }
            }
        }
    }

    return returnVal;
}

e_eFSS_Res isValidPage(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, uint8_t* const suppBuff,
                       const uint32_t pageIndx)
{
    /* Local variable */
    e_eFSS_Res returnVal;

    /* Check for NULL pointer */
    if( NULL == suppBuff )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Check for parameter validity */
        if( pageIndx >= pginfo.nOfPages )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Get the page to check in the support ram buffer */
            returnVal = readPageLL( pginfo, cbHld, pageIndx, suppBuff );
            if( EFSS_RES_OK == returnVal )
            {
                /* now verify the page loaded in the buffer */
                returnVal = isValidPageInBuff(pginfo, cbHld, suppBuff);
            }
        }
    }

    return returnVal;
}

e_eFSS_Res writePageNPrmNUpdateCrc(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, uint8_t* const pageBuff,
                                   uint8_t* const suppBuff, const uint32_t pageIndx, const s_prv_pagePrm* prmPage)
{
    /* Local variable */
    e_eFSS_Res returnVal;

    /* Check for NULL pointer */
    if( ( NULL == pageBuff ) || ( NULL == prmPage ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Check for parameter validity */
        if( pageIndx >= pginfo.nOfPages )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Set the page param and CRC in the page buffer */
            returnVal = setPagePrmInBuffNCrcUp(pginfo, cbHld, pageBuff, prmPage);
            if( EFSS_RES_OK == returnVal )
            {
                /* Erase physical page */
                returnVal = erasePageLL(pginfo, cbHld, pageIndx);
                if( EFSS_RES_OK == returnVal )
                {
                    /* Write the pageBuff in the physical page */
                    returnVal = writePageLL(pginfo, cbHld, pageIndx, pageBuff, suppBuff );
                }
            }
        }
    }

    return returnVal;
}

e_eFSS_Res writeNPageNPrmNUpdateCrc(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, uint8_t* const pageBuff,
                                    uint8_t* const suppBuff, const uint32_t nPageToWrite, const uint32_t startPageIndx,
                                    const s_prv_pagePrm* prmPage)
{
    /* Local variable */
    e_eFSS_Res returnVal;
    uint32_t iterator;

    /* Check for NULL pointer */
    if( ( NULL == pageBuff )|| ( NULL == suppBuff ) || ( NULL == prmPage ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Check for parameter validity */
        if( ( 0u == nPageToWrite ) || ( startPageIndx >= pginfo.nOfPages ) ||
          ( ( startPageIndx + nPageToWrite - 1u ) >= pginfo.nOfPages ) )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Set the page param and CRC in the page buffer */
            returnVal = setPagePrmInBuffNCrcUp(pginfo, cbHld, pageBuff, prmPage);
            if( EFSS_RES_OK == returnVal )
            {
                iterator = 0u;
                while( ( iterator < nPageToWrite ) && ( EFSS_RES_OK == returnVal ) )
                {
                    /* Erase physical page */
                    returnVal = erasePageLL(pginfo, cbHld, ( startPageIndx + iterator ) );
                    if( EFSS_RES_OK == returnVal )
                    {
                        /* Write the pageBuff in the physical page */
                        returnVal = writePageLL(pginfo, cbHld, ( startPageIndx + iterator ), pageBuff, suppBuff );
                    }

                    iterator++;
                }
            }
        }
    }

    return returnVal;
}

e_eFSS_Res readPageNPrm(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, uint8_t* const pageBuff,
                        const uint32_t pageIndx, s_prv_pagePrm* const pagePrm)
{
    /* Local variable */
    e_eFSS_Res returnVal;

    /* Check for NULL pointer */
    if( ( NULL == pageBuff ) || ( NULL == pagePrm ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Check for parameter validity */
        if( pageIndx >= pginfo.nOfPages )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Get pageBuff */
            returnVal = readPageLL( pginfo, cbHld, pageIndx, pageBuff );

            if( EFSS_RES_OK == returnVal )
            {
                /* Fill even param only for comodity */
                returnVal = getPagePrmFromBuff(pginfo, pageBuff, pagePrm);
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
