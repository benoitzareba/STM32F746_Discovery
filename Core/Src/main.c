//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  Main.c
// AUTHOR      :  Benoit ZAREBA
//
//-----------------------------------------------------------------------------
//
// HISTORIC    :
//
// 16/02/2021 - V1.0 : Initial revision
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included file
//-----------------------------------------------------------------------------
#include "LCD.h"
#include "Board.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Structures and types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------

//---------- Functions ----------

//-----------------------------------------------------------------------------
// Private variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------

//---------- Functions ----------

//=============================================================================
//--- DEFINITIONS
//=============================================================================

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// FONCTION    : main
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int main (void)
{
   //--- Board configuration
   BOARD_ConfAll();

   //--- LCD Initialization
   LCD_Init();
   LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
   LCD_SelectLayer(LTDC_ACTIVE_LAYER);

   //--- Infinite loop
   while (1)
   {
      HAL_GPIO_WritePin(GPIOI, GPIO_PIN_1, HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_11));

      LCD_Clear(LCD_COLOR_BLACK);
      LCD_SetFont(&LCD_DEFAULT_FONT);
      LCD_SetBackColor(LCD_COLOR_WHITE);
      LCD_SetTextColor(LCD_COLOR_DARKBLUE);

      LCD_DisplayStringAt(0, 1, (UINT8 *)"MBED EXAMPLE", CENTER_MODE);
      HAL_Delay(2000);

      LCD_SetBackColor(LCD_COLOR_BLACK);
      LCD_SetTextColor(LCD_COLOR_ORANGE);
      LCD_DisplayStringAt(0, 100, (UINT8 *)"DISCOVERY STM32F746NG", CENTER_MODE);
      HAL_Delay(2000);

      LCD_Clear(LCD_COLOR_GREEN);
      LCD_SetTextColor(LCD_COLOR_BLUE);
      LCD_DrawRect(10, 20, 50, 50);
      LCD_SetTextColor(LCD_COLOR_BROWN);
      LCD_DrawCircle(80, 80, 50);
      LCD_SetTextColor(LCD_COLOR_YELLOW);
      LCD_DrawEllipse(150, 150, 50, 100);
      LCD_SetTextColor(LCD_COLOR_RED);
      LCD_FillCircle(200, 200, 40);
      HAL_Delay(2000);

      LCD_SetBackColor(LCD_COLOR_GREEN);
      LCD_SetTextColor(LCD_COLOR_CYAN);
      LCD_DisplayStringAt(0, 6, (UINT8 *)"HAVE FUN !!!", RIGHT_MODE);
      HAL_Delay(2000);
   }
}
