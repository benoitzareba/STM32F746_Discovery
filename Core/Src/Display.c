//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  Display.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "Display.h"
#include "LCD.h"

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
// FONCTION    : DISP_ShowHeader
//
// DESCRIPTION : Draw header
//-----------------------------------------------------------------------------
void DISP_ShowHeader (s_HEADER* h)
{
   const UINT8 height = 40;
   const UINT8 posXTitle = 50;
   const UINT8 posYTitle = 5;
   const UINT8 posXDescription = 52;
   const UINT8 posYDescription = 26;

   //--- Zone
   LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
   LCD_FillRect(0, 0, LCD_GetXSize(), height);

   //--- Logo
   LCD_SetTextColor(h->color);
   LCD_FillRect(8, 4, 32, 32);

   //--- Title
   LCD_SetFont(&LCD_FONT_20);
   LCD_SetTextColor(h->color);
   LCD_SetBackColor(LCD_COLOR_BACKGROUND_ITEM);
   LCD_DisplayStringAt(posXTitle, posYTitle, (STRING)h->title, LEFT_MODE);

   //--- Description
   LCD_SetFont(&LCD_FONT_12);
   LCD_SetTextColor(LCD_COLOR_GRAY);
   LCD_DisplayStringAt(posXDescription, posYDescription, (STRING)h->description, LEFT_MODE);

   LCD_SetTextColor(h->color);
   LCD_DrawHLineThickness(0, height, LCD_GetXSize(), 2);
}

//-----------------------------------------------------------------------------
// FONCTION    : DISP_ShowFooter
//
// DESCRIPTION : Draw footer
//-----------------------------------------------------------------------------
void DISP_ShowFooter (s_FOOTER* f)
{
   UINT8 i;
   s_BUTTON *b = NULL;

   const UINT8 buttonXPosition   = 0;
   const UINT8 buttonYPosition   = LCD_GetYSize() - 50;
   const UINT8 buttonWidth       = 112;
   const UINT8 buttonHeight      = 51;

   //--- Footer
   LCD_SetTextColor(f->color);
   LCD_DrawHLine(0, LCD_GetYSize() - 52, LCD_GetXSize());

   LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
   LCD_FillRect(0, LCD_GetYSize() - 51, LCD_GetXSize(), 51);

   for (i = 0; i < f->nbButton; i++)
   {
      b = &f->button[i];

      LCD_DrawRectButton(buttonXPosition + buttonWidth * b->position + 10 * b->position, buttonYPosition, buttonWidth, buttonHeight, b->color, (CHAR8*)b->txt, b->selected);
   }
}
