//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  DispWidgets.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "DispWidgets.h"
#include "Display.h"

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
// FONCTION    : DISP_WDGT_ShowSlide
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
void DISP_WDGT_ShowSlide (UINT16 posX, UINT16 posY, s_WIDGET_SLIDE_BAR *pSlide, BOOL isFirstTime)
{
   FLOAT32 percentValue;

   percentValue = (FLOAT32)(pSlide->currentValue / 100.);
   percentValue *= pSlide->length;

   LCD_SetTextColor(pSlide->backgroundColor);
   LCD_DrawHLineThickness(posX - 12, posY - 12, pSlide->length + 24, 24);

   //--- Slide base
   LCD_SetTextColor(pSlide->baseColor);
   LCD_DrawHLineThickness(posX, posY, pSlide->length, 2);

   LCD_SetTextColor(pSlide->color);
   LCD_DrawHLineThickness(posX, posY, (UINT16)percentValue, 2);

   LCD_SetTextColor(pSlide->color);
   LCD_DrawFilledCircle(posX + (UINT16)percentValue, posY, 7);
}

//-----------------------------------------------------------------------------
// FONCTION    : DISP_WDGT_ShowToggleSwitch
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
void DISP_WDGT_ShowToggleSwitch (UINT16 posX, UINT16 posY, s_WIDGET_TOGGLE_SWITCH *pToggleSwitch, BOOL isFirstTime)
{
   UINT32 color;

   //--- Determine la couleur en fonction de l'etat du toggle switch
   color = (pToggleSwitch->currentValue == TRUE) ? pToggleSwitch->colorOn : pToggleSwitch->colorOff;

   //--- Base du toggle
   LCD_SetTextColor(color);
   LCD_DrawFilledCircle(posX, posY, 10);
   LCD_DrawFilledCircle(posX + 18, posY, 10);
   LCD_FillRect(posX + 2, posY - 10, 13, 21);

   //--- Position du toggle switch
   LCD_SetTextColor(LCD_COLOR_WHITE);
   LCD_DrawFilledCircle((pToggleSwitch->currentValue == FALSE) ? posX : posX + 18, posY, 8);
}

//-----------------------------------------------------------------------------
// FONCTION    : DISP_WDGT_ShowRadioButton
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
void DISP_WDGT_ShowRadioButton (UINT16 posX, UINT16 posY, s_WIDGET_RADIOBUTTON *pRadioButton, BOOL isFirstTime)
{
   if (isFirstTime == TRUE)
   {
      //--- Contour du bouton radio
      LCD_SetTextColor(pRadioButton->color);
      LCD_DrawFilledCircle(posX, posY, 8);
   }

   LCD_SetTextColor(pRadioButton->backgroundColor);
   LCD_DrawFilledCircle(posX, posY, 7);

   if (pRadioButton->currentValue == TRUE)
   {
      LCD_SetTextColor(pRadioButton->color);
      LCD_DrawFilledCircle(posX, posY, 6);
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : DISP_WDGT_ShowCheckBox
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
void DISP_WDGT_ShowCheckBox (UINT16 posX, UINT16 posY, s_WIDGET_CHECKBOX *pCheckBox, BOOL isFirstTime)
{
   if (isFirstTime == TRUE)
   {
      //--- Contour du checkbox
      LCD_SetTextColor(pCheckBox->color);
      LCD_FillRect(posX, posY, 15, 15);
   }

   LCD_SetTextColor(pCheckBox->backgroundColor);
   LCD_FillRect(posX + 1, posY + 1, 13, 13);

   if (pCheckBox->currentValue == TRUE)
   {
      LCD_SetTextColor(pCheckBox->color);
      LCD_FillRect(posX + 2, posY + 2, 11, 11);
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : DISP_WDGT_ShowNumberInput
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
void DISP_WDGT_ShowNumberInput (UINT16 posX, UINT16 posY, s_WIDGET_NUMBER_INPUT *pNumberInput, BOOL isFirstTime)
{
   UINT16 widthButton;
   UINT16 lenStr;
   STRING_TAB valStr;

   widthButton = (UINT16)((FLOAT32)(pNumberInput->width) * 0.2);

   if (isFirstTime == TRUE)
   {
      //--- Background
      LCD_SetTextColor(pNumberInput->color);
      LCD_FillRect(posX, posY, pNumberInput->width, pNumberInput->height);
      LCD_SetTextColor(pNumberInput->backgroundColor);
      LCD_FillRect(posX + 1, posY + 1, pNumberInput->width - 2, pNumberInput->height - 2);

      //--- Bouton moins et plus
      LCD_SetTextColor(pNumberInput->color);
      LCD_FillRect(posX, posY, widthButton, pNumberInput->height);
      LCD_FillRect(posX + pNumberInput->width - widthButton, posY, widthButton, pNumberInput->height);

      LCD_SetTextColor(pNumberInput->backgroundColor);
      LCD_SetBackColor(pNumberInput->color);
      LCD_SetFont(&LCD_FONT_24);
      LCD_DisplayStringAt(posX + ((widthButton / 2) - (LCD_FONT_24.Width / 2)), posY + ((pNumberInput->height / 2) - (LCD_FONT_24.Height / 2)) + 2, (STRING)"-\0", LEFT_MODE);
      LCD_DisplayStringAt(posX + pNumberInput->width - widthButton + ((widthButton / 2) - (LCD_FONT_24.Width / 2)), posY + ((pNumberInput->height / 2) - (LCD_FONT_24.Height / 2)) + 2, (STRING)"+\0", LEFT_MODE);
   }

   STRING_FORMAT(valStr, "%u", pNumberInput->currentValue);
   lenStr = STRING_LEN(valStr);
   lenStr *= LCD_FONT_24.Width;

   LCD_SetTextColor(pNumberInput->backgroundColor);
   LCD_FillRect(posX + widthButton + 4, posY + 4, pNumberInput->width - 6 - widthButton * 2, pNumberInput->height - 6);

   LCD_SetTextColor(pNumberInput->color);
   LCD_SetBackColor(pNumberInput->backgroundColor);
   LCD_SetFont(&LCD_FONT_24);
   LCD_DisplayStringAt(posX + ((pNumberInput->width / 2) - (lenStr / 2)), posY + ((pNumberInput->height / 2) - (LCD_FONT_24.Height / 2)) + 2, (STRING)valStr, LEFT_MODE);
}

//-----------------------------------------------------------------------------
// FONCTION    : DISP_WDGT_ShowCircleProgress
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
void DISP_WDGT_ShowCircleProgress (UINT16 posX, UINT16 posY, s_WIDGET_CIRCLE_PROGRESS *pCircleProgress, BOOL isFirstTime)
{
   STRING_TAB strTmp;
   UINT8 len;
   UINT32 color;

   if (isFirstTime == TRUE)
   {
      LCD_SetTextColor(pCircleProgress->backgroundColor);
      LCD_DrawFilledCircle(posX - 1, posY - 1, pCircleProgress->radius + 2);

      LCD_SetTextColor(pCircleProgress->maskColor);
      LCD_DrawFilledCircle(posX, posY, (UINT8)(pCircleProgress->radius * 0.8));

      LCD_DrawProgressCircle(posX, posY, pCircleProgress->radius * 0.2, 0, pCircleProgress->currentValue, pCircleProgress->radius, pCircleProgress->color);
   }
   else
   {
      color = (pCircleProgress->currentValue < pCircleProgress->oldValue) ? pCircleProgress->backgroundColor : pCircleProgress->color;
      LCD_DrawProgressCircle(posX, posY, pCircleProgress->radius * 0.2, pCircleProgress->oldValue, pCircleProgress->currentValue, pCircleProgress->radius, color);
   }

   LCD_SetTextColor(pCircleProgress->maskColor);
   LCD_FillRect(posX - (pCircleProgress->radius * 0.5), posY - 8, pCircleProgress->radius, LCD_FONT_12.Height + 4);

   LCD_SetTextColor(pCircleProgress->color);
   LCD_SetFont(&LCD_FONT_12);
   STRING_FORMAT(strTmp, "%u%%", (UINT16)pCircleProgress->currentValue);
   len = LCD_FONT_12.Width * STRING_LEN(strTmp);
   LCD_DisplayStringAt(posX - len / 2, posY - LCD_FONT_12.Height / 2, (STRING)strTmp, LEFT_MODE);
}
