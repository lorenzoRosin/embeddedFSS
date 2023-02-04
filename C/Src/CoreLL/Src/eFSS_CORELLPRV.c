/**
 * @file       eFSS_CORELLPRV.c
 *
 * @brief      Low level private core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/* The written page in storage will have this struct:
 * ------------------------------------------------------------------ User data
 * - [uint8_t] -                    -> N byte of user data           |
 * ------------------------------------------------------------------ Metadata  (17 byte)
 * - uint32_t  - uPageUseSpec1      -> User specific data            |
 * - uint32_t  - uPageUseSpec2      -> User specific data            |
 * - uint32_t  - uPageUseSpec3      -> User specific data            |
 * - uint32_t  - uPageUseSpec4      -> User specific data            |
 * - uint8_t   - uPageSubType       -> Page subtype                  |
 * ------------------------------------------------------------------ Private metadata (15 byte)
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
#include "eFSS_CORELLPRV.h"



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eFSS_CORELLPRV_ExtractData(uint8_t* p_puBuf, t_eFSS_CORELL_privMeta* p_ptPrvMeta, t_eFSS_TYPE_PageMeta* p_ptMeta)
{
    /* Local var used for calculation */
    uint32_t l_uIndx;
    uint32_t l_uTemp;

    /* Init var */
    l_uIndx = 0u;

    /* Clear passed data */
    memset(p_ptPrvMeta, 0u, sizeof(t_eFSS_CORELL_privMeta));
    memset(p_ptMeta,    0u, sizeof(t_eFSS_TYPE_PageMeta));

    /* --- Copy data Little endian -- uPageUseSpec1 */
    l_uTemp = (uint32_t) p_puBuf[l_uIndx];
    p_ptMeta->uPageUseSpec1 |= ( l_uTemp & 0x000000FFu );
    l_uIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 8u  );
    p_ptMeta->uPageUseSpec1 |= ( l_uTemp & 0x0000FF00u );
    l_uIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 16u  );
    p_ptMeta->uPageUseSpec1 |= ( l_uTemp & 0x00FF0000u );
    l_uIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 24u  );
    p_ptMeta->uPageUseSpec1 |= ( l_uTemp & 0xFF000000u );
    l_uIndx++;

    /* --- Copy data Little endian -- uPageUseSpec2 */
    l_uTemp = (uint32_t) p_puBuf[l_uIndx];
    p_ptMeta->uPageUseSpec2 |= ( l_uTemp & 0x000000FFu );
    l_uIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 8u  );
    p_ptMeta->uPageUseSpec2 |= ( l_uTemp & 0x0000FF00u );
    l_uIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 16u  );
    p_ptMeta->uPageUseSpec2 |= ( l_uTemp & 0x00FF0000u );
    l_uIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 24u  );
    p_ptMeta->uPageUseSpec2 |= ( l_uTemp & 0xFF000000u );
    l_uIndx++;

    /* --- Copy data Little endian -- uPageUseSpec3 */
    l_uTemp = (uint32_t) p_puBuf[l_uIndx];
    p_ptMeta->uPageUseSpec3 |= ( l_uTemp & 0x000000FFu );
    l_uIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 8u  );
    p_ptMeta->uPageUseSpec3 |= ( l_uTemp & 0x0000FF00u );
    l_uIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 16u  );
    p_ptMeta->uPageUseSpec3 |= ( l_uTemp & 0x00FF0000u );
    l_uIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 24u  );
    p_ptMeta->uPageUseSpec3 |= ( l_uTemp & 0xFF000000u );
    l_uIndx++;

    /* --- Copy data Little endian -- uPageUseSpec4 */
    l_uTemp = (uint32_t) p_puBuf[l_uIndx];
    p_ptMeta->uPageUseSpec4 |= ( l_uTemp & 0x000000FFu );
    l_uIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 8u  );
    p_ptMeta->uPageUseSpec4 |= ( l_uTemp & 0x0000FF00u );
    l_uIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 16u  );
    p_ptMeta->uPageUseSpec4 |= ( l_uTemp & 0x00FF0000u );
    l_uIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 24u  );
    p_ptMeta->uPageUseSpec4 |= ( l_uTemp & 0xFF000000u );
    l_uIndx++;

    /* --- Copy data Little endian -- PAGE SUBTYPE */
    p_ptMeta->uPageSubType = p_puBuf[l_uIndx];
    l_uIndx++;

    /* --- Copy data Little endian -- PAGE TYPE */
    p_ptPrvMeta->uPageType = p_puBuf[l_uIndx];
    l_uIndx++;

    /* --- Copy data Little endian -- PAGE VERSION */
    l_uTemp = (uint16_t) p_puBuf[l_uIndx];
    p_ptPrvMeta->uPageVersion |= ( l_uTemp & 0x00FFu );
    l_uIndx++;

    l_uTemp =  (uint16_t) ( ( (uint16_t) p_puBuf[l_uIndx] ) << 8u  );
    p_ptPrvMeta->uPageVersion |= ( l_uTemp & 0xFF00u );
    l_uIndx++;

    /* --- Copy data Little endian -- PAGE TOTAL */
    l_uTemp = (uint32_t) p_puBuf[l_uIndx];
    p_ptPrvMeta->uPageTot |= ( l_uTemp & 0x000000FFu );
    l_uIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 8u  );
    p_ptPrvMeta->uPageTot |= ( l_uTemp & 0x0000FF00u );
    l_uIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 16u  );
    p_ptPrvMeta->uPageTot |= ( l_uTemp & 0x00FF0000u );
    l_uIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 24u  );
    p_ptPrvMeta->uPageTot |= ( l_uTemp & 0xFF000000u );
    l_uIndx++;

    /* --- Copy data Little endian -- MAGIC NUMBER */
    l_uTemp = (uint32_t) p_puBuf[l_uIndx];
    p_ptPrvMeta->uPageMagicNumber |= ( l_uTemp & 0x000000FFu );
    l_uIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 8u  );
    p_ptPrvMeta->uPageMagicNumber |= ( l_uTemp & 0x0000FF00u );
    l_uIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 16u  );
    p_ptPrvMeta->uPageMagicNumber |= ( l_uTemp & 0x00FF0000u );
    l_uIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 24u  );
    p_ptPrvMeta->uPageMagicNumber |= ( l_uTemp & 0xFF000000u );
    l_uIndx++;

    /* --- Copy data Little endian -- CRC */
    l_uTemp = (uint32_t) p_puBuf[l_uIndx];
    p_ptPrvMeta->uPageCrc |= ( l_uTemp & 0x000000FFu );
    l_uIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 8u  );
    p_ptPrvMeta->uPageCrc |= ( l_uTemp & 0x0000FF00u );
    l_uIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 16u  );
    p_ptPrvMeta->uPageCrc |= ( l_uTemp & 0x00FF0000u );
    l_uIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 24u  );
    p_ptPrvMeta->uPageCrc |= ( l_uTemp & 0xFF000000u );
    l_uIndx++;
}

void eFSS_CORELLPRV_InsertData(uint8_t* p_puBuf, t_eFSS_CORELL_privMeta* p_ptPrvMeta, t_eFSS_TYPE_PageMeta* p_ptMeta)
{
    /* Local var used for calculation */
    uint32_t l_uIndx;

    /* Init var */
    l_uIndx = 0u;

    /* --- Copy data Little endian -- uPageUseSpec1 */
    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptMeta->uPageUseSpec1        ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptMeta->uPageUseSpec1 >> 8u  ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptMeta->uPageUseSpec1 >> 16u ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptMeta->uPageUseSpec1 >> 24u ) & 0x000000FFu );
    l_uIndx++;

    /* --- Copy data Little endian -- uPageUseSpec2 */
    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptMeta->uPageUseSpec2        ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptMeta->uPageUseSpec2 >> 8u  ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptMeta->uPageUseSpec2 >> 16u ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptMeta->uPageUseSpec2 >> 24u ) & 0x000000FFu );
    l_uIndx++;

    /* --- Copy data Little endian -- uPageUseSpec3 */
    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptMeta->uPageUseSpec3        ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptMeta->uPageUseSpec3 >> 8u  ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptMeta->uPageUseSpec3 >> 16u ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptMeta->uPageUseSpec3 >> 24u ) & 0x000000FFu );
    l_uIndx++;

    /* --- Copy data Little endian -- uPageUseSpec4 */
    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptMeta->uPageUseSpec4        ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptMeta->uPageUseSpec4 >> 8u  ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptMeta->uPageUseSpec4 >> 16u ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptMeta->uPageUseSpec4 >> 24u ) & 0x000000FFu );
    l_uIndx++;

    /* --- Copy data Little endian -- PAGE SUBTYPE */
    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptMeta->uPageSubType         ) & 0xFFu );
    l_uIndx++;

    /* --- Copy data Little endian -- PAGE TYPE */
    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageType          ) & 0xFFu );
    l_uIndx++;

    /* --- Copy data Little endian -- PAGE VERSION */
    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageVersion        ) & 0x00FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageVersion >> 8u  ) & 0x00FFu );
    l_uIndx++;

    /* --- Copy data Little endian -- PAGE TOTAL */
    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageTot        ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageTot >> 8u  ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageTot >> 16u ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageTot >> 24u ) & 0x000000FFu );
    l_uIndx++;

    /* --- Copy data Little endian -- MAGIC NUMBER */
    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageMagicNumber        ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageMagicNumber >> 8u  ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageMagicNumber >> 16u ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageMagicNumber >> 24u ) & 0x000000FFu );
    l_uIndx++;

    /* --- Copy data Little endian -- CRC */
    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageCrc        ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageCrc  >> 8u  ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageCrc  >> 16u ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageCrc  >> 24u ) & 0x000000FFu );
    l_uIndx++;

}

void eFSS_CORELLPRV_InsertCrc(uint8_t* p_puBuf, uint32_t p_uCrc)
{
    /* Local var used for calculation */
    uint32_t l_uIndx;

    /* Init var */
    l_uIndx = 0u;

    /* --- Copy data Little endian -- CRC */
    p_puBuf[l_uIndx] = (uint8_t) ( ( p_uCrc        ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_uCrc  >> 8u  ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_uCrc  >> 16u ) & 0x000000FFu );
    l_uIndx++;

    p_puBuf[l_uIndx] = (uint8_t) ( ( p_uCrc  >> 24u ) & 0x000000FFu );
    l_uIndx++;
}