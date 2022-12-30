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
    uint32_t l_uTryPerformed;
    bool_t l_bCbRes;

	/* Check pointer validity */
	if( ( NULL == pageBuff ) || ( NULL == pagePrm )  )
	{
		l_eRes = e_eFSS_UTILSHLPRV_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity */
        if( p_pageL <= 0u )
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_BADPARAM;
        }
        else
        {
            /* Initialize internal status */
            l_uTryPerformed = 0u;
            l_bCbRes = false;

            /* Do operation */
            while( ( false == l_bCbRes ) && ( l_uTryPerformed < p_uReTry ) )
            {
                l_bCbRes = (*(p_ptCtx->fErase))(p_ptCtx->ptCtxErase, p_uPageIndx);
                l_uTryPerformed++;
            }

            if( false == l_bCbRes )
            {
                l_eRes = e_eFSS_UTILSLLPRV_RES_CLBCKREPORTERROR;
            }
            else
            {
                l_eRes = e_eFSS_UTILSLLPRV_RES_OK;
            }
        }
	}

	return l_eRes;
}


e_eFSS_Res getPagePrmFromBuff(const s_eFSS_PgInfo pginfo, const uint8_t* pageBuff, s_prv_pagePrm* const pagePrm)
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
        if( ( pginfo.pageSize < EFSS_MIN_PAGE_SIZE_BYTE ) || ( pginfo.pageSize > EFSS_MAX_PAGE_SIZE_BYTE ) )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Calculating offset */
            uint32_t offset1 = pginfo.pageSize - ( sizeof(uint64_t) + sizeof(uint64_t) );
            uint32_t offset2 = pginfo.pageSize - ( sizeof(uint64_t) + sizeof(uint32_t) + sizeof(uint16_t) );
            uint32_t offset3 = pginfo.pageSize - ( sizeof(uint64_t) + sizeof(uint32_t) );
            uint32_t offset4 = pginfo.pageSize - ( sizeof(uint32_t) + sizeof(uint32_t) );
            uint32_t offset5 = pginfo.pageSize - ( sizeof(uint32_t) );

            /* fill the parameter using the passed buffer */
            (void)memcpy( (uint8_t*)&pagePrm->pageType,               &pageBuff[offset1], sizeof(uint16_t) );
            (void)memcpy( (uint8_t*)&pagePrm->allPageAlignmentNumber, &pageBuff[offset2], sizeof(uint16_t) );
            (void)memcpy( (uint8_t*)&pagePrm->pageVersion,            &pageBuff[offset3], sizeof(uint32_t) );
            (void)memcpy( (uint8_t*)&pagePrm->pageMagicNumber,        &pageBuff[offset4], sizeof(uint32_t) );
            (void)memcpy( (uint8_t*)&pagePrm->pageCrc,                &pageBuff[offset5], sizeof(uint32_t) );
            returnVal = EFSS_RES_OK;
        }
    }
    return returnVal;
}

e_eFSS_Res setPagePrmInBuff(const s_eFSS_PgInfo pginfo, uint8_t* const pageBuff, const s_prv_pagePrm* pagePrm)
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
        if( ( pginfo.pageSize < EFSS_MIN_PAGE_SIZE_BYTE ) || ( pginfo.pageSize > EFSS_MAX_PAGE_SIZE_BYTE ) )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Calculating offset */
            uint32_t offset1 = pginfo.pageSize - ( sizeof(uint64_t) + sizeof(uint64_t) );
            uint32_t offset2 = pginfo.pageSize - ( sizeof(uint64_t) + sizeof(uint32_t) + sizeof(uint16_t) );
            uint32_t offset3 = pginfo.pageSize - ( sizeof(uint64_t) + sizeof(uint32_t) );
            uint32_t offset4 = pginfo.pageSize - ( sizeof(uint32_t) + sizeof(uint32_t) );
            uint32_t offset5 = pginfo.pageSize - ( sizeof(uint32_t) );

            /* fill the paramebuffer using the passed parameter */
            (void)memcpy( &pageBuff[offset1], (const uint8_t*)&pagePrm->pageType,                 sizeof(uint16_t) );
            (void)memcpy( &pageBuff[offset2], (const uint8_t*)&pagePrm->allPageAlignmentNumber,   sizeof(uint16_t) );
            (void)memcpy( &pageBuff[offset3], (const uint8_t*)&pagePrm->pageVersion,              sizeof(uint32_t) );
            (void)memcpy( &pageBuff[offset4], (const uint8_t*)&pagePrm->pageMagicNumber,          sizeof(uint32_t) );
            (void)memcpy( &pageBuff[offset5], (const uint8_t*)&pagePrm->pageCrc,                  sizeof(uint32_t) );

            returnVal = EFSS_RES_OK;
        }
    }

    return returnVal;
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
