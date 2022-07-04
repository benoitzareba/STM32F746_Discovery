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
#include "Utils.h"

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

   widthButton = (UINT16)((FLOAT32)(pNumberInput->width) * 0.25);

   if (isFirstTime == TRUE)
   {
      //--- Background
      LCD_SetTextColor(pNumberInput->color);
      LCD_FillRect(posX, posY, pNumberInput->width, pNumberInput->height);
      LCD_SetTextColor(pNumberInput->backgroundColor);
      LCD_FillRect(posX + 1, posY + 1, pNumberInput->width - 2, pNumberInput->height - 2);

      //--- Boutons moins et plus
      LCD_SetTextColor(pNumberInput->color);
      LCD_FillRect(posX, posY, widthButton, pNumberInput->height);
      LCD_FillRect(posX + pNumberInput->width - widthButton, posY, widthButton, pNumberInput->height);

      LCD_SetTextColor(pNumberInput->backgroundColor);
      LCD_SetBackColor(pNumberInput->color);
      LCD_SetFont(&LCD_FONT_24);
      LCD_DisplayStringAt(posX + ((widthButton / 2) - (LCD_FONT_24.Width / 2)), posY + ((pNumberInput->height / 2) - (LCD_FONT_24.Height / 2)) + 2, (STRING)"-\0", LEFT_MODE);
      LCD_DisplayStringAt(posX + pNumberInput->width - widthButton + ((widthButton / 2) - (LCD_FONT_24.Width / 2)), posY + ((pNumberInput->height / 2) - (LCD_FONT_24.Height / 2)) + 2, (STRING)"+\0", LEFT_MODE);
   }

   //STRING_FORMAT(valStr, "%u", pNumberInput->currentValue);
   if (pNumberInput->currentValue == 0)
      lenStr = 1;
   else if (pNumberInput->currentValue == 100)
      lenStr = 3;
   else
      lenStr = 2;

   UTILS_FormatWidgetValue(valStr, lenStr, pNumberInput->currentValue);
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
   CHAR8 strTmp[8];
   UINT8 len;
   UINT8 i;
   UINT32 color;

   if (isFirstTime == TRUE)
   {
      LCD_DrawProgressCircle(posX, posY, 0.85, 0, 100, pCircleProgress->radius, pCircleProgress->backgroundColor);
      //LCD_DrawProgressCircle(posX, posY, 0.85, 0, pCircleProgress->currentValue, pCircleProgress->radius, pCircleProgress->color);
   }
   else
   {
      color = (pCircleProgress->currentValue < pCircleProgress->oldValue) ? pCircleProgress->backgroundColor : pCircleProgress->color;
      LCD_DrawProgressCircle(posX, posY, 0.85, pCircleProgress->oldValue, pCircleProgress->currentValue, pCircleProgress->radius, color);
   }

   if (pCircleProgress->dispVal == TRUE)
   {
      LCD_SetFont(&LCD_FONT_12);

      if (pCircleProgress->currentValue == 0)
         len = 1;
      else if (pCircleProgress->currentValue == 100)
         len = 3;
      else
         len = 2;

      UTILS_FormatWidgetValue(strTmp, len, pCircleProgress->currentValue);

      for (i = 0; i < ARRAY_SIZE(strTmp); i++)
      {
         if (strTmp[i] == '\0')
         {
            strTmp[i] = '%';
            strTmp[i + 1] = '\0';
            break;
         }
      }

      len = LCD_FONT_12.Width * STRING_LEN(strTmp);

      LCD_SetTextColor(pCircleProgress->maskColor);
      LCD_FillRect(posX - (2 * LCD_FONT_12.Width), posY - LCD_FONT_12.Height / 2 - 1, 4 * LCD_FONT_12.Width, LCD_FONT_12.Height + 2);

      LCD_SetTextColor(pCircleProgress->color);
      LCD_DisplayStringAt(posX - len / 2, posY - LCD_FONT_12.Height / 2, (STRING)strTmp, LEFT_MODE);
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : DISP_WDGT_ShowProgressBar
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
void DISP_WDGT_ShowProgressBar (UINT16 posX, UINT16 posY, s_WIDGET_PROGRESS_BAR *pProgressBar, BOOL isFirstTime)
{
   static STRING_TAB strTmp;
   UINT16 posCursor;
   UINT16 heightVal;
   static UINT8 len;
   UINT8 i;

   if (isFirstTime == TRUE && pProgressBar->outline == TRUE)
   {
      //--- Selection de la couleur choisi
      LCD_SetTextColor(pProgressBar->color);

      //--- Cercle plein du haut couleur
      LCD_DrawFilledCircle(posX + pProgressBar->width/2,
                           posY + pProgressBar->width/2,
                           pProgressBar->width / 2);

      //--- Rectangle vertical plein couleur
      LCD_FillRect(posX,
                   posY + (pProgressBar->width / 2),
                   pProgressBar->width,
                   pProgressBar->height);

      //--- Cercle plein du bas couleur
      LCD_DrawFilledCircle(posX + pProgressBar->width/2,
                           posY + (pProgressBar->width / 2) + pProgressBar->height,
                           pProgressBar->width / 2);
   }

   //--- Fond de la progress bar
   {
      //--- Selection de la couleur de base
      LCD_SetTextColor(pProgressBar->baseColor);

      //--- Cercle plein du haut couleur de base
      LCD_DrawFilledCircle(posX + 1 + ((pProgressBar->width - 2) / 2),
                           posY + 1 + ((pProgressBar->width - 2) / 2),
                           ((pProgressBar->width - 2) / 2));

      //--- Rectangle vertical plein couleur de base
      LCD_FillRect(posX + 1,
                   posY + 1 + ((pProgressBar->width - 2) / 2),
                   pProgressBar->width - 2,
                   pProgressBar->height - 1);

      //--- Cercle plein du bas couleur de base
      LCD_DrawFilledCircle(posX + 1 + ((pProgressBar->width - 2) / 2),
                           posY + 1 + ((pProgressBar->width - 2) / 2) + pProgressBar->height,
                           ((pProgressBar->width - 2) / 2));
   }

   //--- Progress bar
   {
      //--- Selection de la couleur
      LCD_SetTextColor(pProgressBar->color);

      //--- Calculs dimensions
      heightVal = pProgressBar->height * pProgressBar->currentValue;
      posCursor = posY + pProgressBar->height - heightVal;

      //--- Cercle plein du haut couleur
      LCD_DrawFilledCircle(posX + 2 + ((pProgressBar->width - 4) / 2),
                           posCursor + 2 + ((pProgressBar->width - 4) / 2),
                           ((pProgressBar->width - 4) / 2));

      //--- Rectangle vertical plein couleur
      LCD_FillRect(posX + 2,
                   posCursor + 2 + ((pProgressBar->width - 4) / 2),
                   pProgressBar->width - 4,
                   heightVal);

      //--- Cercle plein du bas couleur
      LCD_DrawFilledCircle(posX + 2 + ((pProgressBar->width - 4) / 2),
                           posY + 2 + ((pProgressBar->width - 4) / 2) + pProgressBar->height,
                           ((pProgressBar->width - 4) / 2));
   }

   //--- Si curseur
   if (pProgressBar->cursorValue == TRUE)
   {
      LCD_SetFont(&LCD_FONT_12);
      //STRING_FORMAT(strTmp, "% 3u%%", (UINT16)(pProgressBar->currentValue*100));

      if (pProgressBar->currentValue >= 1.00 && pProgressBar->currentValue < 1.05)
         len = 3;
      else if (pProgressBar->currentValue <= 0.01)
         len = 0;
      else
         len = 2;

      UTILS_FormatWidgetValue(strTmp, len, (UINT8)(pProgressBar->currentValue * 100));
      for (i = 0; i < ARRAY_SIZE(strTmp); i++)
      {
         if (strTmp[i] == '\0')
         {
            strTmp[i] = '%';
            strTmp[i + 1] = '\0';
            break;
         }
      }

      len = LCD_FONT_12.Width * STRING_LEN(strTmp);

      //--- Curseur et valeur
      LCD_SetTextColor(pProgressBar->backgroundColor);
      LCD_FillRect(posX - pProgressBar->width - 28,
                   posY,
                   (pProgressBar->width / 2) + 2 + 28,
                   pProgressBar->height + pProgressBar->width + 2);

      //--- Selection de la couleur
      LCD_SetTextColor(pProgressBar->color);
      LCD_DrawSmallTriangle(posX - pProgressBar->width/2,
                            posCursor + 2 + ((pProgressBar->width - 4) / 2),
                            RIGHT_DIR,
                            4,
                            pProgressBar->color);

      LCD_DisplayStringAt(posX - len - pProgressBar->width,
                          posCursor,
                          (STRING)strTmp,
                          LEFT_MODE);
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : DISP_WDGT_ShowButton
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
void DISP_WDGT_ShowButton (UINT16 posX, UINT16 posY, s_WIDGET_BUTTON *pButton, BOOL isFirstTime)
{
   UINT16 i;
   UINT8 strWidth;
   UINT8 radius;

   radius = pButton->height / 2;

   //--- Contours
   LCD_SetTextColor(pButton->outlineColor);
   LCD_DrawFilledCircle(posX + radius, posY + radius, radius);
   LCD_DrawFilledCircle(posX + pButton->width - radius, posY + radius, radius);

   for (i = posY; i < posY + pButton->height; i++)
      LCD_DrawHLine(posX + radius, i, pButton->width - radius * 2);

   //--- Fond
   LCD_SetTextColor(pButton->color);
   LCD_DrawFilledCircle(posX + radius, posY + radius, radius - 1);
   LCD_DrawFilledCircle(posX + pButton->width - radius, posY + radius, radius - 1);

   for (i = posY + 1; i < posY + pButton->height - 1; i++)
      LCD_DrawHLine(posX + radius, i, pButton->width - radius * 2);

   //--- Texte
   LCD_SetBackColor(pButton->color);
   LCD_SetTextColor(pButton->txtColor);
   LCD_SetFont(&LCD_FONT_16);

   strWidth = STRING_LEN(pButton->str) * LCD_FONT_16.Width;
   LCD_DisplayStringAt(posX + ((pButton->width / 2) - strWidth / 2), posY + ((pButton->height / 2) - LCD_FONT_16.Height / 2) + 2, pButton->str, LEFT_MODE);
}

//-----------------------------------------------------------------------------
// FONCTION    : DISP_WDGT_ShowTextInput
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
void DISP_WDGT_ShowTextInput (UINT16 posX, UINT16 posY, s_WIDGET_TEXT_INPUT *pTextInput, BOOL isFirstTime)
{
   static TickType_t _tick;
   static BOOL _way = FALSE;

   UINT8 strLen;

   if (isFirstTime == TRUE)
   {
      LCD_SetTextColor(pTextInput->backColor);
      LCD_FillRect(posX, posY, pTextInput->width, 24);
   }

   //--- Calcul longueur du texte en px
   strLen = STRING_LEN(pTextInput->str) * LCD_FONT_20.Width;

   //--- Affichage du texte
   LCD_SetFont(&LCD_FONT_20);
   LCD_SetTextColor(pTextInput->txtColor);
   LCD_SetBackColor(pTextInput->backColor);
   LCD_DisplayStringAt(posX + 2, posY + 4, pTextInput->str, LEFT_MODE);

   if (xTaskGetTickCount() - _tick >= 500)
   {
      //--- Clignotement du curseur
      LCD_SetTextColor((_way == TRUE) ? pTextInput->txtColor : pTextInput->backColor);
      LCD_DrawVLine(posX + 2 + strLen, posY + 2, 20);
      _tick = xTaskGetTickCount();
      _way = !_way;
   }

   LCD_SetTextColor(pTextInput->color);
   LCD_DrawHLine(posX, posY + 24, pTextInput->width);
   LCD_DrawHLine(posX, posY + 25, pTextInput->width);
}
