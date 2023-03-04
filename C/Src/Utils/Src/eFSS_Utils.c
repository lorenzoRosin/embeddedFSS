/**
 * @file       eFSS_Utils.c
 *
 * @brief      Utils module
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_Utils.h"



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
bool_t eFSS_Utils_InsertU32(uint8_t* const p_puBuf, const uint32_t p_uToInsert)
{
	/* Local variable for the return value */
	bool_t l_bRes;

	/* Local variable for local calculation */
	uint32_t l_uIndx;

	/* Check NULL pointer */
	if( NULL == p_puBuf )
	{
		l_bRes = false;
	}
	else
	{
		/* Init var */
		l_bRes = true;
		l_uIndx = 0u;

		/* Insert */
		p_puBuf[l_uIndx] = (uint8_t) ( ( p_uToInsert        ) & 0x000000FFu );
		l_uIndx++;

		p_puBuf[l_uIndx] = (uint8_t) ( ( p_uToInsert >> 8u  ) & 0x000000FFu );
		l_uIndx++;

		p_puBuf[l_uIndx] = (uint8_t) ( ( p_uToInsert >> 16u ) & 0x000000FFu );
		l_uIndx++;

		p_puBuf[l_uIndx] = (uint8_t) ( ( p_uToInsert >> 24u ) & 0x000000FFu );
		l_uIndx++;
	}

	return l_bRes;
}

bool_t eFSS_Utils_InsertU16(uint8_t* const p_puBuf, const uint16_t p_uToInsert)
{
	/* Local variable for the return value */
	bool_t l_bRes;

	/* Local variable for local calculation */
	uint32_t l_uIndx;

	/* Check NULL pointer */
	if( NULL == p_puBuf )
	{
		l_bRes = false;
	}
	else
	{
		/* Init var */
		l_bRes = true;
		l_uIndx = 0u;

		/* Insert */
		p_puBuf[l_uIndx] = (uint8_t) ( ( p_uToInsert        ) & 0x00FFu );
		l_uIndx++;

		p_puBuf[l_uIndx] = (uint8_t) ( ( p_uToInsert >> 8u  ) & 0x00FFu );
		l_uIndx++;
	}

	return l_bRes;
}

bool_t eFSS_Utils_InsertU8(uint8_t* const p_puBuf, const uint8_t p_uToInsert)
{
	/* Local variable for the return value */
	bool_t l_bRes;

	/* Local variable for local calculation */
	uint32_t l_uIndx;

	/* Check NULL pointer */
	if( NULL == p_puBuf )
	{
		l_bRes = false;
	}
	else
	{
		/* Init var */
		l_bRes = true;
		l_uIndx = 0u;

		/* Insert */
		p_puBuf[l_uIndx] = (uint8_t) ( ( p_uToInsert        ) & 0xFFu );
		l_uIndx++;
	}

	return l_bRes;
}

bool_t eFSS_Utils_RetriveU32(const uint8_t* p_puBuf, uint32_t* const p_puToRetrive)
{
	/* Local variable for the return value */
	bool_t l_bRes;

	/* Local variable for local calculation */
	uint32_t l_uIndx;
	uint32_t l_uTemp;

	/* Check NULL pointer */
	if( ( NULL == p_puBuf ) || ( NULL == p_puToRetrive ) )
	{
		l_bRes = false;
	}
	else
	{
		/* Init var */
		l_bRes = true;
		l_uIndx = 0u;
		*p_puToRetrive = 0u;

		/* Retrive */
		l_uTemp = (uint32_t) p_puBuf[l_uIndx];
		*p_puToRetrive |= ( l_uTemp & 0x000000FFu );
		l_uIndx++;

		l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 8u  );
		*p_puToRetrive |= ( l_uTemp & 0x0000FF00u );
		l_uIndx++;

		l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 16u  );
		*p_puToRetrive |= ( l_uTemp & 0x00FF0000u );
		l_uIndx++;

		l_uTemp =  (uint32_t) ( ( (uint32_t) p_puBuf[l_uIndx] ) << 24u  );
		*p_puToRetrive |= ( l_uTemp & 0xFF000000u );
		l_uIndx++;
	}

	return l_bRes;
}

bool_t eFSS_Utils_RetriveU16(const uint8_t* p_puBuf, uint16_t* const p_puToRetrive)
{
	/* Local variable for the return value */
	bool_t l_bRes;

	/* Local variable for local calculation */
	uint32_t l_uIndx;
	uint16_t l_uTemp;

	/* Check NULL pointer */
	if( ( NULL == p_puBuf ) || ( NULL == p_puToRetrive ) )
	{
		l_bRes = false;
	}
	else
	{
		/* Init var */
		l_bRes = true;
		l_uIndx = 0u;
		*p_puToRetrive = 0u;

		/* Retrive */
		l_uTemp = (uint16_t) p_puBuf[l_uIndx];
		*p_puToRetrive |= ( l_uTemp & 0x00FFu );
		l_uIndx++;

		l_uTemp =  (uint16_t) ( ( (uint16_t) p_puBuf[l_uIndx] ) << 8u  );
		*p_puToRetrive |= ( l_uTemp & 0xFF00u );
		l_uIndx++;
	}

	return l_bRes;
}

bool_t eFSS_Utils_RetriveU8(const uint8_t* p_puBuf, uint8_t* const p_puToRetrive)
{
	/* Local variable for the return value */
	bool_t l_bRes;

	/* Local variable for local calculation */
	uint32_t l_uIndx;
	uint8_t l_uTemp;

	/* Check NULL pointer */
	if( ( NULL == p_puBuf ) || ( NULL == p_puToRetrive ) )
	{
		l_bRes = false;
	}
	else
	{
		/* Init var */
		l_bRes = true;
		l_uIndx = 0u;
		*p_puToRetrive = 0u;

		/* Retrive */
		l_uTemp = (uint8_t) p_puBuf[l_uIndx];
		*p_puToRetrive |= l_uTemp;
		l_uIndx++;
	}

	return l_bRes;
}