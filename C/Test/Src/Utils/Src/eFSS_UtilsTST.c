/**
 * @file       eFSS_UtilsTST.c
 *
 * @brief      Utils module test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_UtilsTST.h"
#include "eFSS_Utils.h"
#include <stdio.h>



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void eFSP_UtilsTST_BadPointer(void);
static void eFSP_UtilsTST_Insert(void);
static void eFSP_UtilsTST_Retrive(void);
static void eFSP_UtilsTST_InsRetr(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eFSS_UtilsTST_ExeTest(void)
{
	(void)printf("\n\nUTILS TEST START \n\n");

    eFSP_UtilsTST_BadPointer();
    eFSP_UtilsTST_Insert();
    eFSP_UtilsTST_Retrive();
    eFSP_UtilsTST_InsRetr();

    (void)printf("\n\nUTIL TEST END \n\n");
}



/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
static void eFSP_UtilsTST_BadPointer(void)
{
    /* Local variable */
    uint8_t  l_auMemArea[10u];
    uint32_t l_uTest32;
    uint16_t l_uTest16;
    uint8_t l_uTest8;

    /* Init var */
    l_uTest32 = 0u;
    l_uTest16 = 0u;
    l_uTest8 = 0u;
    (void)memset(l_auMemArea, 0, sizeof(l_auMemArea));

    /* Function */
    if( false == eFSS_Utils_InsertU32(NULL, l_uTest32) )
    {
        (void)printf("eFSP_UtilsTST_BadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_UtilsTST_BadPointer 1  -- FAIL \n");
    }

    if( false == eFSS_Utils_InsertU16(NULL, l_uTest16) )
    {
        (void)printf("eFSP_UtilsTST_BadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_UtilsTST_BadPointer 2  -- FAIL \n");
    }

    if( false == eFSS_Utils_InsertU8(NULL, l_uTest8) )
    {
        (void)printf("eFSP_UtilsTST_BadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_UtilsTST_BadPointer 3  -- FAIL \n");
    }

    if( false == eFSS_Utils_RetriveU32(NULL, &l_uTest32) )
    {
        (void)printf("eFSP_UtilsTST_BadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_UtilsTST_BadPointer 4  -- FAIL \n");
    }

    if( false == eFSS_Utils_RetriveU32(l_auMemArea, NULL) )
    {
        (void)printf("eFSP_UtilsTST_BadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_UtilsTST_BadPointer 5  -- FAIL \n");
    }

    if( false == eFSS_Utils_RetriveU16(NULL, &l_uTest16) )
    {
        (void)printf("eFSP_UtilsTST_BadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_UtilsTST_BadPointer 6  -- FAIL \n");
    }

    if( false == eFSS_Utils_RetriveU16(l_auMemArea, NULL) )
    {
        (void)printf("eFSP_UtilsTST_BadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_UtilsTST_BadPointer 7  -- FAIL \n");
    }

    if( false == eFSS_Utils_RetriveU8(NULL, &l_uTest8) )
    {
        (void)printf("eFSP_UtilsTST_BadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_UtilsTST_BadPointer 8  -- FAIL \n");
    }

    if( false == eFSS_Utils_RetriveU8(l_auMemArea, NULL) )
    {
        (void)printf("eFSP_UtilsTST_BadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_UtilsTST_BadPointer 9  -- FAIL \n");
    }
}

static void eFSP_UtilsTST_Insert(void)
{
    /* Local variable */
    uint8_t  l_auMemArea[4u];
    uint32_t l_uTest32;
    uint16_t l_uTest16;
    uint8_t l_uTest8;

    /* Function */
    (void)memset(l_auMemArea, 0, sizeof( l_auMemArea ));
    l_uTest32 = 0x12345678u;
    if( true == eFSS_Utils_InsertU32(l_auMemArea, l_uTest32) )
    {
        if( ( 0x78u == l_auMemArea[0u]) && ( 0x56u == l_auMemArea[1u]) && ( 0x34u == l_auMemArea[2u]) &&
            ( 0x12u == l_auMemArea[3u]) )
        {
            (void)printf("eFSP_UtilsTST_Insert 1  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_UtilsTST_Insert 1  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_UtilsTST_Insert 1  -- FAIL \n");
    }

    /* Function */
    (void)memset(l_auMemArea, 0, sizeof( l_auMemArea ));
    l_uTest16 = 0x1234u;
    if( true == eFSS_Utils_InsertU16(l_auMemArea, l_uTest16) )
    {
        if( ( 0x34u == l_auMemArea[0u]) && ( 0x12u == l_auMemArea[1u]) && ( 0x00u == l_auMemArea[2u]) &&
            ( 0x00u == l_auMemArea[3u]) )
        {
            (void)printf("eFSP_UtilsTST_Insert 2  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_UtilsTST_Insert 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_UtilsTST_Insert 2  -- FAIL \n");
    }

    /* Function */
    (void)memset(l_auMemArea, 0, sizeof( l_auMemArea ));
    l_uTest8 = 0x12u;
    if( true == eFSS_Utils_InsertU8(l_auMemArea, l_uTest8) )
    {
        if( ( 0x12u == l_auMemArea[0u]) && ( 0x00u == l_auMemArea[1u]) && ( 0x00u == l_auMemArea[2u]) &&
            ( 0x00u == l_auMemArea[3u]) )
        {
            (void)printf("eFSP_UtilsTST_Insert 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_UtilsTST_Insert 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_UtilsTST_Insert 3  -- FAIL \n");
    }

}

static void eFSP_UtilsTST_Retrive(void)
{
    /* Local variable */
    uint8_t  l_auMemArea[4u];
    uint32_t l_uTest32;
    uint16_t l_uTest16;
    uint8_t l_uTest8;

    /* Function */
    l_auMemArea[0u] = 0x78u;
    l_auMemArea[1u] = 0x56u;
    l_auMemArea[2u] = 0x34u;
    l_auMemArea[3u] = 0x12u;
    l_uTest32 = 0x00u;
    if( true == eFSS_Utils_RetriveU32(l_auMemArea, &l_uTest32) )
    {
        if( 0x12345678u == l_uTest32 )
        {
            (void)printf("eFSP_UtilsTST_Retrive 1  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_UtilsTST_Retrive 1  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_UtilsTST_Retrive 1  -- FAIL \n");
    }

    /* Function */
    l_auMemArea[0u] = 0x34u;
    l_auMemArea[1u] = 0x12u;
    l_auMemArea[2u] = 0x00u;
    l_auMemArea[3u] = 0x00u;
    l_uTest16 = 0x00u;
    if( true == eFSS_Utils_RetriveU16(l_auMemArea, &l_uTest16) )
    {
        if( 0x1234u == l_uTest16 )
        {
            (void)printf("eFSP_UtilsTST_Retrive 2  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_UtilsTST_Retrive 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_UtilsTST_Retrive 2  -- FAIL \n");
    }

    /* Function */
    l_auMemArea[0u] = 0x12u;
    l_auMemArea[1u] = 0x00u;
    l_auMemArea[2u] = 0x00u;
    l_auMemArea[3u] = 0x00u;
    l_uTest8 = 0x00u;
    if( true == eFSS_Utils_RetriveU8(l_auMemArea, &l_uTest8) )
    {
        if( 0x12u == l_uTest8 )
        {
            (void)printf("eFSP_UtilsTST_Retrive 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_UtilsTST_Retrive 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_UtilsTST_Retrive 3  -- FAIL \n");
    }

}

static void eFSP_UtilsTST_InsRetr(void)
{
    /* Local variable */
    uint8_t  l_auMemArea[4u];
    uint32_t l_uTest32;
    uint16_t l_uTest16;
    uint8_t  l_uTest8;

    /* Function */
    (void)memset(l_auMemArea, 0, sizeof( l_auMemArea ));
    l_uTest32 = 0xABCDEF12u;
    if( true == eFSS_Utils_InsertU32(l_auMemArea, l_uTest32) )
    {
        if( true == eFSS_Utils_RetriveU32(l_auMemArea, &l_uTest32) )
        {
            if( 0xABCDEF12u == l_uTest32 )
            {
                (void)printf("eFSP_UtilsTST_InsRetr 1  -- OK \n");
            }
            else
            {
                (void)printf("eFSP_UtilsTST_InsRetr 1  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_UtilsTST_InsRetr 1  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_UtilsTST_InsRetr 1  -- FAIL \n");
    }

    /* Function */
    (void)memset(l_auMemArea, 0, sizeof( l_auMemArea ));
    l_uTest16 = 0xCE91u;
    if( true == eFSS_Utils_InsertU16(l_auMemArea, l_uTest16) )
    {
        if( true == eFSS_Utils_RetriveU16(l_auMemArea, &l_uTest16) )
        {
            if( 0xCE91u == l_uTest16 )
            {
                (void)printf("eFSP_UtilsTST_InsRetr 2  -- OK \n");
            }
            else
            {
                (void)printf("eFSP_UtilsTST_InsRetr 2  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_UtilsTST_InsRetr 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_UtilsTST_InsRetr 2  -- FAIL \n");
    }

    /* Function */
    (void)memset(l_auMemArea, 0, sizeof( l_auMemArea ));
    l_uTest8 = 0xB2u;
    if( true == eFSS_Utils_InsertU8(l_auMemArea, l_uTest8) )
    {
        if( true == eFSS_Utils_RetriveU8(l_auMemArea, &l_uTest8) )
        {
            if( 0xB2u == l_uTest8 )
            {
                (void)printf("eFSP_UtilsTST_InsRetr 3  -- OK \n");
            }
            else
            {
                (void)printf("eFSP_UtilsTST_InsRetr 3  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_UtilsTST_InsRetr 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_UtilsTST_InsRetr 3  -- FAIL \n");
    }
}
