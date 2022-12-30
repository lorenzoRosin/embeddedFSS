/**
 * @file Prv_eFSSUtils.h
 *
 */

#ifndef PRVEFSSUTILS_H
#define PRVEFSSUTILS_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSSType.h"



/***********************************************************************************************************************
 *      PRIVATE TYPEDEFS
 **********************************************************************************************************************/
typedef struct
{
    uint16_t    pageType;
    uint16_t    allPageAlignmentNumber;
    uint32_t    pageVersion;
    uint32_t    pageMagicNumber;
    uint32_t    pageCrc;
}s_prv_pagePrm;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * Get s_prv_pagePrm from a page already loaded in a Buffer
 * @param pginfo Information about memory and pages
 * @param pageBuff Pointer to a page loaded in a buffer
 * @param pagePrm Pointer to a s_prv_pagePrm where we will copy the data readed from the buffer
 * @return EFSS_RES_BADPOINTER in case of bad pointer
 *         EFSS_RES_BADPARAM in case of a wrong param passed
 *         EFSS_RES_OK operation ended successfully
 */
e_eFSS_Res getPagePrmFromBuff(const s_eFSS_PgInfo pginfo, const uint8_t* pageBuff, s_prv_pagePrm* const pagePrm);

/**
 * Set s_prv_pagePrm in a page already loaded in a Buffer
 * @param pginfo Information about memory and pages
 * @param pageBuff Pointer to a page loaded in a buffer
 * @param pagePrm Pointer to s_prv_pagePrm that must be copied in the buffer
 * @return EFSS_RES_BADPOINTER in case of bad pointer
 *         EFSS_RES_BADPARAM in case of a wrong param passed
 *         EFSS_RES_OK operation ended successfully
 */
e_eFSS_Res setPagePrmInBuff(const s_eFSS_PgInfo pginfo, uint8_t* const pageBuff, const s_prv_pagePrm* pagePrm);

/**
 * Set CRC in a page already loaded in a buffer
 * @param pginfo Information about memory and pages
 * @param pageBuff Pointer to a page loaded in a buffer
 * @param crcToSet CRC we need to set
 * @return EFSS_RES_BADPOINTER in case of bad pointer
 *         EFSS_RES_BADPARAM in case of a wrong param passed
 *         EFSS_RES_OK operation ended successfully
 */
e_eFSS_Res setCrcInPagePrmBuff(const s_eFSS_PgInfo pginfo, uint8_t* const pageBuff, const uint32_t crcToSet);

/**
 * Calculate the CRC of an already loaded Page in a buffer, excluding the CRC itself from the calc
 * @param pginfo Information about memory and pages
 * @param cbHld Struct containing all callback reference
 * @param pageBuff Pointer to a page loaded in a buffer
 * @param crcCalc CRC calculated
 * @return EFSS_RES_BADPOINTER in case of bad pointer
 *         EFSS_RES_BADPARAM in case of a wrong param passed
 *         EFSS_RES_OK operation ended successfully
 */
e_eFSS_Res calcPagePrmCrcInBuff(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, const uint8_t* pageBuff,
                                uint32_t* const crcCalc);

/**
 * Set s_prv_pagePrm in a page already loaded in a buffer, and set a recalculated CRC
 * @param pginfo Information about memory and pages
 * @param cbHld Struct containing all callback reference
 * @param pageBuff Pointer to a page loaded in a buffer
 * @param pagePrm  Page parameter that we want to set
 * @return EFSS_RES_BADPOINTER in case of bad pointer
 *         EFSS_RES_BADPARAM in case of a wrong param passed
 *         EFSS_RES_OK operation ended successfully
 */
e_eFSS_Res setPagePrmInBuffNCrcUp(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, uint8_t* const pageBuff,
                                  const s_prv_pagePrm* pagePrm);

/**
 * Verify if the data present in a buffer page has CRC, magic number, and page type coherent with all the calculated
 * value
 * @param pginfo Information about memory and pages
 * @param cbHld Struct containing all callback reference
 * @param pageBuff Pointer to a page loaded in a buffer
 * @return EFSS_RES_BADPOINTER in case of bad pointer
 *         EFSS_RES_BADPARAM in case of a wrong param passed
 *         EFSS_RES_NOTVALIDPAGE the page is not valid, crc or somethings else dosent return
 *         EFSS_RES_OK operation ended successfully
 */
e_eFSS_Res isValidPageInBuff(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, const uint8_t* pageBuff);

/**
 * Verify if the data present in a page has CRC, magic number, and page type coherent with all the calculated
 * value
 * @param pginfo Information about memory and pages
 * @param cbHld Struct containing all callback reference
 * @param suppBuff Support buffer to do the calculation, must be atleast a page size
 * @param pageIndex Index of the page to check
 * @return EFSS_RES_BADPOINTER in case of bad pointer
 *         EFSS_RES_BADPARAM in case of a wrong param passed
 *         EFSS_RES_NOTVALIDPAGE the page is not valid, crc or somethings else dosent return
 *         EFSS_RES_ERRORREAD error reported from read callback
 *         EFSS_RES_OK operation ended successfully
 */
e_eFSS_Res isValidPage(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, uint8_t* const suppBuff,
                       const uint32_t pageIndx);

/**
 * Erase and write a page with the param passed, and automatically update the CRC of the page it self
 * @param pginfo Information about memory and pages
 * @param cbHld Struct containing all callback reference
 * @param pageBuff Pointer to a buffer containing the data that we want to write in the page
 * @param suppBuff Support buffer to do the calculation, must be atleast a page size
 * @param pageIndex Index of the page to write
 * @param prmPage Param that we want to write in the page
 * @return EFSS_RES_BADPOINTER in case of bad pointer
 *         EFSS_RES_BADPARAM in case of a wrong param passed
 *         EFSS_RES_NOTVALIDPAGE the page is not valid, crc or somethings else dosent return
 *         EFSS_RES_ERRORWRITE error reported from write callback
 *         EFSS_RES_ERRORERASE error reported from erase callback
 *         EFSS_RES_OK operation ended successfully
 */
e_eFSS_Res writePageNPrmNUpdateCrc(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, uint8_t* const pageBuff,
                                   uint8_t* const suppBuff, const uint32_t pageIndx, const s_prv_pagePrm* prmPage);

/**
 * Erase and write n page with the param passed, and automatically update the CRC of all the pages
 * @param pginfo Information about memory and pages
 * @param cbHld Struct containing all callback reference
 * @param pageBuff Pointer to a buffer containing the data that we want to write in the page
 * @param suppBuff Support buffer to do the calculation, must be atleast a page size
 * @param nPageToWrite Number of pages to write
 * @param startPageIndx Index of the page to write
 * @param prmPage Param that we want to write in the page
 * @return EFSS_RES_BADPOINTER in case of bad pointer
 *         EFSS_RES_BADPARAM in case of a wrong param passed
 *         EFSS_RES_NOTVALIDPAGE the page is not valid, crc or somethings else dosent return
 *         EFSS_RES_ERRORWRITE error reported from write callback
 *         EFSS_RES_ERRORERASE error reported from erase callback
 *         EFSS_RES_OK operation ended successfully
 */
e_eFSS_Res writeNPageNPrmNUpdateCrc(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, uint8_t* const pageBuff,
                                    uint8_t* const suppBuff, const uint32_t nPageToWrite, const uint32_t startPageIndx,
                                    const s_prv_pagePrm* prmPage);

/**
 * Read a page and load it in the buffer, and copy data param in pagePrm also
 * @param pginfo Information about memory and pages
 * @param cbHld Struct containing all callback reference
 * @param pageBuff Pointer to a buffer that will be filled with data from the page
 * @param pageIndex Index of the page to read
 * @param prmPage Param that we fill with the information readed in the page
 * @return EFSS_RES_BADPOINTER in case of bad pointer
 *         EFSS_RES_BADPARAM in case of a wrong param passed
 *         EFSS_RES_NOTVALIDPAGE the page is not valid, crc or somethings else dosent return
 *         EFSS_RES_ERRORWRITE error reported from write callback
 *         EFSS_RES_ERRORERASE error reported from erase callback
 *         EFSS_RES_OK operation ended successfully
 */
e_eFSS_Res readPageNPrm(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, uint8_t* const pageBuff,
                        const uint32_t pageIndx, s_prv_pagePrm* const pagePrm);

/**
 * Verify the validity of the page in origIndx and backupIndx.
 * 1 - If origIndx and backupIndx are valid, verify if they are equals. If not copy origIndx in backupIndx
 * 2 - If origIndx is not valid copy backupIndx in origIndx
 * 3 - If backupIndx is not valid copy origIndx in backupIndx
 * 4 - If origIndx and backupIndx are not valid we cann not do nothing
 * @param pginfo Information about memory and pages
 * @param cbHld Struct containing all callback reference
 * @param pageOrig Support buffer to do the calculation, must be atleast a page size
 * @param pageBkup Support buffer to do the calculation, must be atleast a page size
 * @param origIndx Index of the original page
 * @param backupIndx Index of the backup page
 * @return EFSS_RES_BADPOINTER in case of bad pointer
 *         EFSS_RES_BADPARAM in case of a wrong param passed
 *         EFSS_RES_NOTVALIDPAGE both origin and backup pages are corrupted
 *         EFSS_RES_ERRORWRITE error reported from write callback
 *         EFSS_RES_ERRORERASE error reported from erase callback
 *         EFSS_RES_ERRORREAD error reported from read callback
 *         EFSS_RES_OK_BKP_RCVRD operation ended successfully recovering a backup or an origin page
 *         EFSS_RES_OK operation ended successfully
 */
e_eFSS_Res verifyAndRipristinateBkup(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, uint8_t* const pageOrig,
                                     uint8_t* const pageBkup, const uint32_t origIndx, const uint32_t backupIndx);

/**
 * Clone a page from origIndx to destIndx
 * @param pginfo Information about memory and pages
 * @param cbHld Struct containing all callback reference
 * @param pageBuff Support buffer to do the calculation, must be atleast a page size
 * @param suppBuff Support buffer to do the calculation, must be atleast a page size
 * @param origIndx Index of the original page
 * @param destIndx Index of the destination page
 * @return EFSS_RES_BADPOINTER in case of bad pointer
 *         EFSS_RES_BADPARAM in case of a wrong param passed
 *         EFSS_RES_ERRORWRITE error reported from write callback
 *         EFSS_RES_ERRORERASE error reported from erase callback
 *         EFSS_RES_ERRORREAD error reported from read callback
 *         EFSS_RES_OK operation ended successfully
 */
e_eFSS_Res cloneAPage(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, uint8_t* const pageBuff,
                      uint8_t* const suppBuff, const uint32_t origIndx, const uint32_t destIndx);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* PRVEFSSUTILS_H */
