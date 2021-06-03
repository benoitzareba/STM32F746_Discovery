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
#include "Popup.h"
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
   s_RECT *r = NULL;

   const UINT8 buttonXPosition   = 0;
   const UINT8 buttonYPosition   = LCD_GetYSize() - 50;
   const UINT8 buttonWidth       = 112;
   const UINT8 buttonHeight      = 50;

   //--- Footer
   LCD_SetTextColor(f->color);
   LCD_DrawHLine(0, LCD_GetYSize() - 52, LCD_GetXSize());

   LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
   LCD_FillRect(0, LCD_GetYSize() - 51, LCD_GetXSize(), 51);

   for (i = 0; i < f->nbButton; i++)
   {
      //--- Init pointer
      b = &f->button[i];
      r = &f->button[i].rect;

      //--- Determine les coordonnees
      r->width    = buttonWidth;
      r->height   = buttonHeight;
      r->x        = buttonXPosition + buttonWidth * b->position + 10 * b->position;
      r->y        = buttonYPosition;

      LCD_DrawRectButton(r->x, r->y, buttonWidth, buttonHeight, b->color, (CHAR8*)b->txt, b->selected);
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : DISP_ShowSlideMenu
//
// DESCRIPTION : Draw slide menu
//-----------------------------------------------------------------------------
void DISP_ShowSlideMenu (s_HEADER* h)
{
   UINT8 i = 0;

   LCD_SetTextColor(h->color);
   LCD_DrawLine(LCD_GetXSize(), 90, LCD_GetXSize() - 10, 100);
   LCD_DrawLine(LCD_GetXSize() - 10, 170, LCD_GetXSize(), 180);

   for (i = 0; i < 10; i++)
   {
      LCD_DrawVLine(LCD_GetXSize() - (10 - i), 101 - i, 71 + (i * 2));
   }

   LCD_DrawVLine(LCD_GetXSize() - 1, 0, LCD_GetYSize());

   LCD_SetTextColor(LCD_COLOR_BLACK);
   LCD_DrawLine(LCD_GetXSize() - 4, 130, LCD_GetXSize() - 8, 135);
   LCD_DrawLine(LCD_GetXSize() - 8, 135, LCD_GetXSize() - 4, 140);
}

//-----------------------------------------------------------------------------
// FONCTION    : DISP_ShowPopup
//
// DESCRIPTION : Draw popup
//-----------------------------------------------------------------------------
void DISP_ShowPopup (s_POPUP* pop, UINT32 color)
{
   s_RECT posPopup;
   s_RECT posType;
   s_RECT posTitle;
   s_RECT posLine;
   s_RECT posRedCross;
   s_RECT* posLeftButton;
   s_RECT* posRightButton;
   UINT16 sizeTxt;
   STRING_TAB bufferTmp;

   //--- Dimension et coordonnees popup
   posPopup.width    = 250;
   posPopup.height   = 180;
   posPopup.x        = (LCD_GetXSize() / 2) - (posPopup.width / 2);
   posPopup.y        = (LCD_GetYSize() / 2) - (posPopup.height / 2);

   LCD_SetTextColor(LCD_COLOR_BACKGROUND_PAGE);
   LCD_FillRect(posPopup.x, posPopup.y, posPopup.width, posPopup.height);

   LCD_SetTextColor(LCD_COLOR_GRAY);
   LCD_DrawRectWithThickness(posPopup.x, posPopup.y, posPopup.width, posPopup.height, 1);

   //--- Croix rouge pour close le popup
   if (pop->closeCross == TRUE)
   {
      POPUP_GetCloseCrossPosition(&posRedCross);

      LCD_SetTextColor(LCD_COLOR_RED);
      LCD_DrawFilledCircle(posRedCross.x, posRedCross.y, posRedCross.width);
      LCD_FillRect(posRedCross.x - 5, posRedCross.y - 6, posRedCross.width + 3, posRedCross.height + 4);

      LCD_SetTextColor(LCD_COLOR_WHITE);
      LCD_SetBackColor(LCD_COLOR_RED);
      LCD_SetFont(&LCD_FONT_12);
      LCD_DisplayStringAt(posRedCross.x - 3, posRedCross.y - 5, (STRING)"x", LEFT_MODE);
   }

   //--- Dessine le type de popup
   if (pop->type != NO_BADGE_POPUP)
   {
      switch (pop->type)
      {
         case QUESTION_POPUP:    STRING_COPY((STRING)&bufferTmp, "?\0");   break;
         case INFORMATION_POPUP: STRING_COPY((STRING)&bufferTmp, "!\0");   break;
         case ERROR_POPUP:       STRING_COPY((STRING)&bufferTmp, "X\0");   break;
      }

      //--- Dimension et coordonnees logo type
      posType.height   = 28;
      posType.width    = 28;
      posType.x        = posPopup.x + 10;
      posType.y        = posPopup.y + 10;

      //--- Dessin du type
      LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
      LCD_FillRect(posType.x + 1, posType.y + 1, posType.width - 1, posType.height - 1);

      LCD_SetTextColor(color);
      LCD_DrawRectWithThickness(posType.x, posType.y, posType.width, posType.height, 1);

      LCD_SetFont(&LCD_FONT_24);
      LCD_SetBackColor(LCD_COLOR_BACKGROUND_ITEM);
      LCD_DisplayStringAt(posType.x + 6, posType.y + 4, (STRING)bufferTmp, LEFT_MODE);
   }

   //--- Titre
   posTitle.x = posType.x + posType.width + 10;
   posTitle.y = posType.y + 10;
   LCD_SetFont(&LCD_FONT_16);
   LCD_SetBackColor(LCD_COLOR_BACKGROUND_PAGE);
   LCD_DisplayStringAt(posTitle.x, posTitle.y, pop->title, LEFT_MODE);

   //--- Separateur
   LCD_DrawLine(posType.x, posType.y + posType.width + 8, posType.x + 230, posType.y + posType.width + 8);

   //--- Line 1, 2 and 3
   posLine.x = posType.x;
   posLine.y = posType.y + posType.width + 20;
   LCD_SetTextColor(LCD_COLOR_GRAY);
   LCD_SetFont(&LCD_FONT_12);
   LCD_DisplayStringAt(posLine.x, posLine.y, pop->line1, LEFT_MODE);
   LCD_DisplayStringAt(posLine.x, posLine.y + 25, pop->line2, LEFT_MODE);
   LCD_DisplayStringAt(posLine.x, posLine.y + 50, pop->line3, LEFT_MODE);

   //--- Affichage des boutons
   if (pop->nbButton > 0)
   {
      if (pop->nbButton == 1) //--- Affichage 1 seul bouton.
      {
         //--- Position des boutons
         posLeftButton  = &pop->btLeft->coord;

         //--- Position du bouton au centre du popup
         pop->btLeft->coord.width   = 90;
         pop->btLeft->coord.height  = 30;
         pop->btLeft->coord.x       = posPopup.x + (posPopup.width / 2) - (posLeftButton->width / 2);
         pop->btLeft->coord.y       = posLine.y + 75;

         LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_FillRect(posLeftButton->x, posLeftButton->y, posLeftButton->width, posLeftButton->height);

         LCD_SetTextColor(LCD_COLOR_GRAY);
         LCD_DrawRect(posLeftButton->x - 1, posLeftButton->y, posLeftButton->width, posLeftButton->height);

         LCD_SetFont(&LCD_FONT_12);
         LCD_SetTextColor(color);
         LCD_SetBackColor(LCD_COLOR_BACKGROUND_ITEM);
         sizeTxt = LCD_GetStringWidth((STRING)pop->btLeft->title);
         LCD_DisplayStringAt(pop->btLeft->coord.x + (pop->btLeft->coord.width / 2) - (sizeTxt / 2), pop->btLeft->coord.y + 1 + (pop->btLeft->coord.height / 2) - (LCD_FONT_12.Height / 2), (STRING)pop->btLeft->title, LEFT_MODE);
      }
      else if (pop->nbButton == 2) //--- 2 boutons
      {
         //--- Pointeurs des positions
         posLeftButton  = &pop->btLeft->coord;
         posRightButton = &pop->btRight->coord;

         //---- Left
         {
            //--- Calcul des positions
            posLeftButton->width    = 90;
            posLeftButton->height   = 30;
            posLeftButton->x        = posPopup.x + (posPopup.width / 2) - (((posLeftButton->width * 2) + 20) / 2);
            posLeftButton->y        = posLine.y + 75;

            //--- Dessine fond du bouton
            LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
            LCD_FillRect(posLeftButton->x, posLeftButton->y, posLeftButton->width, posLeftButton->height);

            //--- Dessine le contour du bouton
            LCD_SetTextColor(LCD_COLOR_GRAY);
            LCD_DrawRect(posLeftButton->x - 1, posLeftButton->y, posLeftButton->width, posLeftButton->height);

            //--- Label du bouton
            LCD_SetFont(&LCD_FONT_12);
            LCD_SetTextColor(color);
            LCD_SetBackColor(LCD_COLOR_BACKGROUND_ITEM);
            sizeTxt = LCD_GetStringWidth((STRING)pop->btLeft->title);
            LCD_DisplayStringAt(posLeftButton->x + (posLeftButton->width / 2) - (sizeTxt / 2), posLeftButton->y + 1 + (posLeftButton->height / 2) - (LCD_FONT_12.Height / 2), (STRING)pop->btLeft->title, LEFT_MODE);
         }

         //---- Left
         {
            //--- Calcul des positions
            posRightButton->width   = posLeftButton->width;
            posRightButton->height  = posLeftButton->height;
            posRightButton->x       = posLeftButton->x + posLeftButton->width + 20;
            posRightButton->y       = posLine.y + 75;

            //--- Dessine fond du bouton
            LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
            LCD_FillRect(posRightButton->x, posRightButton->y, posRightButton->width, posRightButton->height);

            //--- Dessine le contour du bouton
            LCD_SetTextColor(LCD_COLOR_GRAY);
            LCD_DrawRect(posRightButton->x - 1, posRightButton->y, posRightButton->width, posRightButton->height);

            //--- Label du bouton
            LCD_SetFont(&LCD_FONT_12);
            LCD_SetTextColor(color);
            LCD_SetBackColor(LCD_COLOR_BACKGROUND_ITEM);
            sizeTxt = LCD_GetStringWidth((STRING)pop->btRight->title);
            LCD_DisplayStringAt(posRightButton->x + (posRightButton->width / 2) - (sizeTxt / 2), posRightButton->y + 1 + (posRightButton->height / 2) - (LCD_FONT_12.Height / 2), (STRING)pop->btRight->title, LEFT_MODE);
         }
      }
   }
}
