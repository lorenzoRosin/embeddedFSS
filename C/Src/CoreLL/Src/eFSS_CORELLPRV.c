/**
 * @file       eFSS_CORELLPRV.h
 *
 * @brief      Low level private core module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_CORELLPRV.h"



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eFSS_CORELLPRV_ExtractData(uint8_t* p_puBuff, t_eFSS_CORELL_privMeta* p_ptPrvMeta,
                                t_eFSS_TYPE_PageMeta* p_ptMetaB)
{
    /* Local var used for calculation */
    uint32_t l_uComulIndx;
    uint32_t l_uTemp;

    /* Init var */
    l_uComulIndx = 0u;
    memset(p_ptMetaB,   0u, sizeof(t_eFSS_TYPE_PageMeta));
    memset(p_ptPrvMeta, 0u, sizeof(t_eFSS_CORELL_privMeta));

    /* --- Copy data Little endian -- uPageUseSpec1 */
    l_uTemp = (uint32_t) p_puBuff[l_uComulIndx];
    p_ptMetaB->uPageUseSpec1 |= ( l_uTemp & 0x000000FFu );
    l_uComulIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuff[l_uComulIndx] ) << 8u  );
    p_ptMetaB->uPageUseSpec1 |= ( l_uTemp & 0x0000FF00u );
    l_uComulIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuff[l_uComulIndx] ) << 16u  );
    p_ptMetaB->uPageUseSpec1 |= ( l_uTemp & 0x00FF0000u );
    l_uComulIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuff[l_uComulIndx] ) << 24u  );
    p_ptMetaB->uPageUseSpec1 |= ( l_uTemp & 0xFF000000u );
    l_uComulIndx++;

    /* --- Copy data Little endian -- uPageUseSpec2 */
    l_uTemp = (uint32_t) p_puBuff[l_uComulIndx];
    p_ptMetaB->uPageUseSpec2 |= ( l_uTemp & 0x000000FFu );
    l_uComulIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuff[l_uComulIndx] ) << 8u  );
    p_ptMetaB->uPageUseSpec2 |= ( l_uTemp & 0x0000FF00u );
    l_uComulIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuff[l_uComulIndx] ) << 16u  );
    p_ptMetaB->uPageUseSpec2 |= ( l_uTemp & 0x00FF0000u );
    l_uComulIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuff[l_uComulIndx] ) << 24u  );
    p_ptMetaB->uPageUseSpec2 |= ( l_uTemp & 0xFF000000u );
    l_uComulIndx++;

    /* --- Copy data Little endian -- uPageUseSpec3 */
    l_uTemp = (uint32_t) p_puBuff[l_uComulIndx];
    p_ptMetaB->uPageUseSpec3 |= ( l_uTemp & 0x000000FFu );
    l_uComulIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuff[l_uComulIndx] ) << 8u  );
    p_ptMetaB->uPageUseSpec3 |= ( l_uTemp & 0x0000FF00u );
    l_uComulIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuff[l_uComulIndx] ) << 16u  );
    p_ptMetaB->uPageUseSpec3 |= ( l_uTemp & 0x00FF0000u );
    l_uComulIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuff[l_uComulIndx] ) << 24u  );
    p_ptMetaB->uPageUseSpec3 |= ( l_uTemp & 0xFF000000u );
    l_uComulIndx++;

    /* --- Copy data Little endian -- uPageUseSpec4 */
    l_uTemp = (uint32_t) p_puBuff[l_uComulIndx];
    p_ptMetaB->uPageUseSpec4 |= ( l_uTemp & 0x000000FFu );
    l_uComulIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuff[l_uComulIndx] ) << 8u  );
    p_ptMetaB->uPageUseSpec4 |= ( l_uTemp & 0x0000FF00u );
    l_uComulIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuff[l_uComulIndx] ) << 16u  );
    p_ptMetaB->uPageUseSpec4 |= ( l_uTemp & 0x00FF0000u );
    l_uComulIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuff[l_uComulIndx] ) << 24u  );
    p_ptMetaB->uPageUseSpec4 |= ( l_uTemp & 0xFF000000u );
    l_uComulIndx++;

    /* --- Copy data Little endian -- PAGE SUBTYPE */
    p_ptMetaB->uPageSubType = p_puBuff[l_uComulIndx];
    l_uComulIndx++;

    /* --- Copy data Little endian -- PAGE TYPE */
    p_ptPrvMeta->uPageType = p_puBuff[l_uComulIndx];
    l_uComulIndx++;

    /* --- Copy data Little endian -- PAGE VERSION */
    l_uTemp = (uint16_t) p_puBuff[l_uComulIndx];
    p_ptPrvMeta->uPageVersion |= ( l_uTemp & 0x00FFu );
    l_uComulIndx++;

    l_uTemp =  (uint16_t) ( ( (uint16_t) p_puBuff[l_uComulIndx] ) << 8u  );
    p_ptPrvMeta->uPageVersion |= ( l_uTemp & 0xFF00u );
    l_uComulIndx++;

    /* --- Copy data Little endian -- MAGIC NUMBER */
    l_uTemp = (uint32_t) p_puBuff[l_uComulIndx];
    p_ptPrvMeta->uPageMagicNumber |= ( l_uTemp & 0x000000FFu );
    l_uComulIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuff[l_uComulIndx] ) << 8u  );
    p_ptPrvMeta->uPageMagicNumber |= ( l_uTemp & 0x0000FF00u );
    l_uComulIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuff[l_uComulIndx] ) << 16u  );
    p_ptPrvMeta->uPageMagicNumber |= ( l_uTemp & 0x00FF0000u );
    l_uComulIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuff[l_uComulIndx] ) << 24u  );
    p_ptPrvMeta->uPageMagicNumber |= ( l_uTemp & 0xFF000000u );
    l_uComulIndx++;

    /* --- Copy data Little endian -- CRC */
    l_uTemp = (uint32_t) p_puBuff[l_uComulIndx];
    p_ptPrvMeta->uPageCrc |= ( l_uTemp & 0x000000FFu );
    l_uComulIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuff[l_uComulIndx] ) << 8u  );
    p_ptPrvMeta->uPageCrc |= ( l_uTemp & 0x0000FF00u );
    l_uComulIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuff[l_uComulIndx] ) << 16u  );
    p_ptPrvMeta->uPageCrc |= ( l_uTemp & 0x00FF0000u );
    l_uComulIndx++;

    l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuff[l_uComulIndx] ) << 24u  );
    p_ptPrvMeta->uPageCrc |= ( l_uTemp & 0xFF000000u );
    l_uComulIndx++;
}

void eFSS_CORELLPRV_InsertData(uint8_t* p_puBuff, t_eFSS_CORELL_privMeta* p_ptPrvMeta,
                               t_eFSS_TYPE_PageMeta* p_ptMetaB)
{
    /* Local var used for calculation */
    uint32_t l_uComulIndx;

    /* Init var */
    l_uComulIndx = 0u;

    /* --- Copy data Little endian -- uPageUseSpec1 */
    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptMetaB->uPageUseSpec1        ) & 0x000000FFu );
    l_uComulIndx++;

    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptMetaB->uPageUseSpec1 >> 8u  ) & 0x000000FFu );
    l_uComulIndx++;

    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptMetaB->uPageUseSpec1 >> 16u ) & 0x000000FFu );
    l_uComulIndx++;

    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptMetaB->uPageUseSpec1 >> 24u ) & 0x000000FFu );
    l_uComulIndx++;

    /* --- Copy data Little endian -- uPageUseSpec2 */
    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptMetaB->uPageUseSpec1        ) & 0x000000FFu );
    l_uComulIndx++;

    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptMetaB->uPageUseSpec1 >> 8u  ) & 0x000000FFu );
    l_uComulIndx++;

    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptMetaB->uPageUseSpec2 >> 16u ) & 0x000000FFu );
    l_uComulIndx++;

    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptMetaB->uPageUseSpec2 >> 24u ) & 0x000000FFu );
    l_uComulIndx++;

    /* --- Copy data Little endian -- uPageUseSpec3 */
    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptMetaB->uPageUseSpec1        ) & 0x000000FFu );
    l_uComulIndx++;

    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptMetaB->uPageUseSpec1 >> 8u  ) & 0x000000FFu );
    l_uComulIndx++;

    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptMetaB->uPageUseSpec3 >> 16u ) & 0x000000FFu );
    l_uComulIndx++;

    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptMetaB->uPageUseSpec3 >> 24u ) & 0x000000FFu );
    l_uComulIndx++;

    /* --- Copy data Little endian -- uPageUseSpec4 */
    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptMetaB->uPageUseSpec4        ) & 0x000000FFu );
    l_uComulIndx++;

    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptMetaB->uPageUseSpec4 >> 8u  ) & 0x000000FFu );
    l_uComulIndx++;

    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptMetaB->uPageUseSpec4 >> 16u ) & 0x000000FFu );
    l_uComulIndx++;

    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptMetaB->uPageUseSpec4 >> 24u ) & 0x000000FFu );
    l_uComulIndx++;

    /* --- Copy data Little endian -- PAGE SUBTYPE */
    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptMetaB->uPageSubType         ) & 0xFFu );
    l_uComulIndx++;

    /* --- Copy data Little endian -- PAGE TYPE */
    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageType          ) & 0xFFu );
    l_uComulIndx++;

    /* --- Copy data Little endian -- PAGE VERSION */
    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageVersion        ) & 0x00FFu );
    l_uComulIndx++;

    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageVersion >> 8u  ) & 0x00FFu );
    l_uComulIndx++;

    /* --- Copy data Little endian -- MAGIC NUMBER */
    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageMagicNumber        ) & 0x000000FFu );
    l_uComulIndx++;

    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageMagicNumber >> 8u  ) & 0x000000FFu );
    l_uComulIndx++;

    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageMagicNumber >> 16u ) & 0x000000FFu );
    l_uComulIndx++;

    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageMagicNumber >> 24u ) & 0x000000FFu );
    l_uComulIndx++;

    /* --- Copy data Little endian -- CRC */
    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageCrc        ) & 0x000000FFu );
    l_uComulIndx++;

    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageCrc  >> 8u  ) & 0x000000FFu );
    l_uComulIndx++;

    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageCrc  >> 16u ) & 0x000000FFu );
    l_uComulIndx++;

    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_ptPrvMeta->uPageCrc  >> 24u ) & 0x000000FFu );
    l_uComulIndx++;

}


void eFSS_CORELLPRV_InsertCrc(uint8_t* p_puBuff, uint32_t p_uCrc)
{
    /* Local var used for calculation */
    uint32_t l_uComulIndx;

    /* Init var */
    l_uComulIndx = 0u;

    /* --- Copy data Little endian -- CRC */
    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_uCrc        ) & 0x000000FFu );
    l_uComulIndx++;

    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_uCrc  >> 8u  ) & 0x000000FFu );
    l_uComulIndx++;

    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_uCrc  >> 16u ) & 0x000000FFu );
    l_uComulIndx++;

    p_puBuff[l_uComulIndx] = (uint8_t) ( ( p_uCrc  >> 24u ) & 0x000000FFu );
    l_uComulIndx++;
}