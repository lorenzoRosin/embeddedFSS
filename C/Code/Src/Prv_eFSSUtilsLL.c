/**
 * @file Prv_eFSSUtilsLL.c
 *
 */



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "Prv_eFSSUtilsLL.h"



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_Res erasePageLL( const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cb, const uint32_t pageIndx )
{
    /* Local variable */
    e_eFSS_Res returnVal;
    uint32_t retryDone;
    bool_t operationResult;

    /* Check for NULL pointer */
    if( NULL == cb.pErasePg )
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
            /* Erase physical page */
            retryDone = 0u;
            operationResult = false;

            while( ( false == operationResult ) && ( retryDone < cb.operationRetry ) )
            {
                operationResult = (*(cb.pErasePg))(pginfo.areaId, pageIndx, pginfo.pageSize);
                retryDone++;
            }

            if( false == operationResult )
            {
                returnVal = EFSS_RES_ERRORERASE;
            }
            else
            {
                returnVal = EFSS_RES_OK;
            }
        }
    }
    return returnVal;
}

e_eFSS_Res writePageLL( const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cb, const uint32_t pageIndx, const uint8_t* dataW,
                        uint8_t* const supportMemory )
{
    /* Local variable */
    e_eFSS_Res returnVal;
    uint32_t retryDone;
    bool_t operationResult;

    /* Check for NULL pointer */
    if( ( NULL == cb.pWritePg ) || ( NULL == cb.pReadPg ) || ( NULL == dataW ) || ( NULL == supportMemory ) )
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
            /* Write physical page */
            retryDone = 0;
            operationResult = false;

            while( ( false == operationResult ) && ( retryDone < cb.operationRetry ) )
            {
                operationResult = (*(cb.pWritePg))(pginfo.areaId, pageIndx, pginfo.pageSize, dataW);

                if( ( true == cb.enableWriteCheck ) && ( true == operationResult ) )
                {
                    /* Read the page and verify if it'is writted corretly */
                    operationResult = (*(cb.pReadPg))(pginfo.areaId, pageIndx, pginfo.pageSize, supportMemory);

                    if( true == operationResult )
                    {
                        if( 0 == memcmp(dataW, supportMemory, pginfo.pageSize) )
                        {
                            operationResult = true;
                        }
                        else
                        {
                            operationResult = false;
                        }
                    }
                }

                retryDone++;
            }

            if( false == operationResult )
            {
                returnVal = EFSS_RES_ERRORWRITE;
            }
            else
            {
                returnVal = EFSS_RES_OK;
            }
        }
    }
    return returnVal;
}

e_eFSS_Res readPageLL( const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cb, const uint32_t pageIndx, uint8_t* const dataR )
{
    /* Local variable */
    e_eFSS_Res returnVal;
    uint32_t retryDone;
    bool_t operationResult;

    /* Check for NULL pointer */
    if( ( NULL == cb.pReadPg ) || ( NULL == dataR ) )
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
            /* Read physical page */
            retryDone = 0;
            operationResult = false;

            while( ( false == operationResult ) && ( retryDone < cb.operationRetry ) )
            {
                operationResult = (*(cb.pReadPg))(pginfo.areaId, pageIndx, pginfo.pageSize, dataR);
                retryDone++;
            }

            if( false == operationResult )
            {
                returnVal = EFSS_RES_ERRORREAD;
            }
            else
            {
                returnVal = EFSS_RES_OK;
            }
        }
    }
    return returnVal;
}

e_eFSS_Res calcCrcLL(const s_eFSS_Cb cb, uint32_t* const crc, const uint8_t* data, const uint32_t dataLen)
{
    /* Local variable */
    e_eFSS_Res returnVal;
    uint32_t retryDone;
    bool_t operationResult;

    /* Check for NULL pointer */
    if( ( NULL == cb.pCrc32 ) || ( NULL == crc) || ( NULL == data) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Crc Func */
        retryDone = 0u;
        operationResult = false;

        while( ( false == operationResult ) && ( retryDone < cb.operationRetry ) )
        {
            operationResult = (*(cb.pCrc32))(crc, data, dataLen, CRC_BASE_SEED);
            retryDone++;
        }

        if( false == operationResult )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            returnVal = EFSS_RES_OK;
        }
    }

    return returnVal;
}




e_eFSS_Res calcCrcSeedLL(const s_eFSS_Cb cb, uint32_t* const crc, const uint8_t* data, const uint32_t dataLen,
                         const uint32_t seed )
{
    /* Local variable */
    e_eFSS_Res returnVal;
    uint32_t retryDone;
    bool_t operationResult;

    /* Check for NULL pointer */
    if( ( NULL == cb.pCrc32 ) || ( NULL == crc) || ( NULL == data) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Crc Func */
        retryDone = 0u;
        operationResult = false;

        while( ( false == operationResult ) && ( retryDone < cb.operationRetry ) )
        {
            operationResult = (*(cb.pCrc32))(crc, data, dataLen, seed);
            retryDone++;
        }

        if( false == operationResult )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            returnVal = EFSS_RES_OK;
        }
    }

    return returnVal;
}

