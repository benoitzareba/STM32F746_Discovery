//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  Screen.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "Screen.h"
#include "LCD.h"
#include "Event.h"
#include <stdio.h>

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
osThreadId_t SCREEN_TaskHandle;
const osThreadAttr_t SCREEN_TaskAttributes = {.name = "screenTask", .priority = (osPriority_t) osPriorityNormal, .stack_size = 128 * 4};

//---------- Functions ----------

//-----------------------------------------------------------------------------
// Private variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------
static s_SCREEN _curScr;
static s_SCREEN _scr[] =   {
                              {NO_SCREEN, NO_SCREEN,  NO_SCREEN_STATE,  NULL,             NULL},
                              {SCREEN_1,  NO_SCREEN,  NO_SCREEN_STATE,  SCREEN_DispScr1,  SCREEN_UpdateScr1},
                              {SCREEN_2,  NO_SCREEN,  NO_SCREEN_STATE,  SCREEN_DispScr2,  SCREEN_UpdateScr2},
                              {SCREEN_3,  NO_SCREEN,  NO_SCREEN_STATE,  SCREEN_DispScr3,  SCREEN_UpdateScr3},
                              {SCREEN_4,  NO_SCREEN,  NO_SCREEN_STATE,  SCREEN_DispScr4,  SCREEN_UpdateScr4},
                              {SCREEN_5,  NO_SCREEN,  NO_SCREEN_STATE,  SCREEN_DispScr5,  SCREEN_UpdateScr5},
                           };

//---------- Functions ----------

//=============================================================================
//--- DEFINITIONS
//=============================================================================

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_DispScr1
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
BOOL SCREEN_DispScr1 (void *p)
{
   s_SCREEN *s = (s_SCREEN *)p;
   STRING strTmp = 0;

   const UINT8 buttonXPosition   = 2;
   const UINT8 buttonYPosition   = LCD_GetYSize() - 50;
   const UINT8 buttonWidth       = 110;
   const UINT8 buttonHeight      = 51;

   switch (s->state)
   {
      case BEFORE_SCREEN_STATE:
      {
         //--- Remplissage du background
         LCD_Clear(LCD_COLOR_BACKGROUND_PAGE);
         LCD_SetFont(&LCD_FONT_24);
         LCD_SetBackColor(LCD_COLOR_BACKGROUND_PAGE);

         //--- Header
         LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_FillRect(0, 0, LCD_GetXSize(), 40);

         LCD_SetTextColor(LCD_COLOR_WHITE);
         LCD_SetBackColor(LCD_COLOR_BACKGROUND_ITEM);
         STRING_COPY(strTmp, "SCREEN 1");
         LCD_DisplayStringAt(50, 2, strTmp, LEFT_MODE);

         LCD_SetTextColor(LCD_COLOR_CYAN);
         LCD_DrawHLine(0, 40, LCD_GetXSize());
         LCD_DrawHLine(0, 41, LCD_GetXSize());
         LCD_SetFont(&LCD_FONT_12);
         LCD_SetTextColor(LCD_COLOR_GRAY);
         STRING_COPY(strTmp, "Description screen 1");
         LCD_DisplayStringAt(52, 26, strTmp, LEFT_MODE);

         //--- Corps
         LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_FillRect(0, 50, LCD_GetXSize(), 170);
         LCD_SetTextColor(LCD_COLOR_GRAY);
         LCD_DrawHLine(0, 50, LCD_GetXSize());

         //--- Footer
         LCD_SetTextColor(LCD_COLOR_CYAN);
         LCD_DrawHLine(0, LCD_GetYSize() - 52, LCD_GetXSize());

         LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_FillRect(0, LCD_GetYSize() - 51, LCD_GetXSize(), 51);

         STRING_COPY(strTmp, "Button 1");
         LCD_DrawRectButton(buttonXPosition, buttonYPosition, buttonWidth, buttonHeight, LCD_COLOR_CYAN, strTmp);

         STRING_COPY(strTmp, "Button 2");
         LCD_DrawRectButton(buttonXPosition + buttonWidth * 1 + 14, buttonYPosition, buttonWidth, buttonHeight, LCD_COLOR_CYAN, strTmp);

         STRING_COPY(strTmp, "Button 3");
         LCD_DrawRectButton(buttonXPosition + buttonWidth * 2 + 24, buttonYPosition, buttonWidth, buttonHeight, LCD_COLOR_CYAN, strTmp);

         STRING_COPY(strTmp, "Button 4");
         LCD_DrawRectButton(buttonXPosition + buttonWidth * 3 + 34, buttonYPosition, buttonWidth, buttonHeight, LCD_COLOR_CYAN, strTmp);

         s->state = DURING_SCREEN_STATE;
         break;
      }

      case DURING_SCREEN_STATE:
      {

         break;
      }

      case AFTER_SCREEN_STATE:
      {
         break;
      }

      default:
      {
         break;
      }
   }

   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_UpdateScr1
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
BOOL SCREEN_UpdateScr1 (void *p, void *e)
{

   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_DispScr2
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
BOOL SCREEN_DispScr2 (void *p)
{
   s_SCREEN *s = (s_SCREEN *)p;
   STRING strTmp = 0;

   const UINT8 buttonXPosition   = 2;
   const UINT8 buttonYPosition   = LCD_GetYSize() - 50;
   const UINT8 buttonWidth       = 110;
   const UINT8 buttonHeight      = 51;

   switch (s->state)
   {
      case BEFORE_SCREEN_STATE:
      {
         //--- Remplissage du background
         LCD_Clear(LCD_COLOR_BACKGROUND_PAGE);
         LCD_SetFont(&LCD_FONT_24);
         LCD_SetBackColor(LCD_COLOR_BACKGROUND_PAGE);

         //--- Header
         LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_FillRect(0, 0, LCD_GetXSize(), 40);

         LCD_SetTextColor(LCD_COLOR_WHITE);
         LCD_SetBackColor(LCD_COLOR_BACKGROUND_ITEM);
         STRING_COPY(strTmp, "SCREEN 2");
         LCD_DisplayStringAt(50, 2, strTmp, LEFT_MODE);

         LCD_SetTextColor(LCD_COLOR_MAGENTA);
         LCD_DrawHLine(0, 40, LCD_GetXSize());
         LCD_DrawHLine(0, 41, LCD_GetXSize());
         LCD_SetFont(&LCD_FONT_12);
         LCD_SetTextColor(LCD_COLOR_GRAY);
         STRING_COPY(strTmp, "Description screen 2");
         LCD_DisplayStringAt(52, 26, strTmp, LEFT_MODE);

         //--- Corps
         LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_FillRect(0, 50, LCD_GetXSize(), 170);
         LCD_SetTextColor(LCD_COLOR_GRAY);
         LCD_DrawHLine(0, 50, LCD_GetXSize());

         //--- Footer
         LCD_SetTextColor(LCD_COLOR_MAGENTA);
         LCD_DrawHLine(0, LCD_GetYSize() - 52, LCD_GetXSize());

         LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_FillRect(0, LCD_GetYSize() - 51, LCD_GetXSize(), 51);

         STRING_COPY(strTmp, "Button 1");
         LCD_DrawRectButton(buttonXPosition, buttonYPosition, buttonWidth, buttonHeight, LCD_COLOR_MAGENTA, strTmp);

         STRING_COPY(strTmp, "Button 2");
         LCD_DrawRectButton(buttonXPosition + buttonWidth * 1 + 14, buttonYPosition, buttonWidth, buttonHeight, LCD_COLOR_MAGENTA, strTmp);

         STRING_COPY(strTmp, "Button 3");
         LCD_DrawRectButton(buttonXPosition + buttonWidth * 2 + 24, buttonYPosition, buttonWidth, buttonHeight, LCD_COLOR_MAGENTA, strTmp);

         STRING_COPY(strTmp, "Button 4");
         LCD_DrawRectButton(buttonXPosition + buttonWidth * 3 + 34, buttonYPosition, buttonWidth, buttonHeight, LCD_COLOR_MAGENTA, strTmp);

         s->state = DURING_SCREEN_STATE;
         break;
      }

      case DURING_SCREEN_STATE:
      {
         break;
      }

      case AFTER_SCREEN_STATE:
      {
         break;
      }

      default:
      {
         break;
      }
   }

   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_UpdateScr2
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
BOOL SCREEN_UpdateScr2  (void *p, void *e)
{

   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_DispScr3
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
BOOL SCREEN_DispScr3 (void *p)
{
   s_SCREEN *s = (s_SCREEN *)p;
   STRING strTmp = 0;

   const UINT8 buttonXPosition   = 2;
   const UINT8 buttonYPosition   = LCD_GetYSize() - 50;
   const UINT8 buttonWidth       = 110;
   const UINT8 buttonHeight      = 51;

   switch (s->state)
   {
      case BEFORE_SCREEN_STATE:
      {
         LCD_Clear(LCD_COLOR_BACKGROUND_PAGE);
         LCD_SetFont(&LCD_FONT_24);
         LCD_SetBackColor(LCD_COLOR_BACKGROUND_PAGE);

         //--- Header
         LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_FillRect(0, 0, LCD_GetXSize(), 40);

         LCD_SetTextColor(LCD_COLOR_WHITE);
         LCD_SetBackColor(LCD_COLOR_BACKGROUND_ITEM);
         STRING_COPY(strTmp, "SCREEN 3");
         LCD_DisplayStringAt(50, 2, strTmp, LEFT_MODE);

         LCD_SetTextColor(LCD_COLOR_YELLOW);
         LCD_DrawHLine(0, 40, LCD_GetXSize());
         LCD_DrawHLine(0, 41, LCD_GetXSize());
         LCD_SetFont(&LCD_FONT_12);
         LCD_SetTextColor(LCD_COLOR_GRAY);
         STRING_COPY(strTmp, "Description screen 3");
         LCD_DisplayStringAt(52, 26, strTmp, LEFT_MODE);

         //--- Corps
         LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_FillRect(0, 50, LCD_GetXSize(), 170);
         LCD_SetTextColor(LCD_COLOR_GRAY);
         LCD_DrawHLine(0, 50, LCD_GetXSize());

         //--- Footer
         LCD_SetTextColor(LCD_COLOR_YELLOW);
         LCD_DrawHLine(0, LCD_GetYSize() - 52, LCD_GetXSize());

         LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_FillRect(0, LCD_GetYSize() - 51, LCD_GetXSize(), 51);

         STRING_COPY(strTmp, "Button 1");
         LCD_DrawRectButton(buttonXPosition, buttonYPosition, buttonWidth, buttonHeight, LCD_COLOR_YELLOW, strTmp);

         STRING_COPY(strTmp, "Button 2");
         LCD_DrawRectButton(buttonXPosition + buttonWidth * 1 + 14, buttonYPosition, buttonWidth, buttonHeight, LCD_COLOR_YELLOW, strTmp);

         STRING_COPY(strTmp, "Button 3");
         LCD_DrawRectButton(buttonXPosition + buttonWidth * 2 + 24, buttonYPosition, buttonWidth, buttonHeight, LCD_COLOR_YELLOW, strTmp);

         STRING_COPY(strTmp, "Button 4");
         LCD_DrawRectButton(buttonXPosition + buttonWidth * 3 + 34, buttonYPosition, buttonWidth, buttonHeight, LCD_COLOR_YELLOW, strTmp);

         s->state = DURING_SCREEN_STATE;
         break;
      }

      case DURING_SCREEN_STATE:
      {
         break;
      }

      case AFTER_SCREEN_STATE:
      {
         break;
      }

      default:
      {
         break;
      }
   }

   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_UpdateScr3
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
BOOL SCREEN_UpdateScr3 (void *p, void *e)
{

   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_DispScr4
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
BOOL SCREEN_DispScr4 (void *p)
{
   s_SCREEN *s = (s_SCREEN *)p;
   STRING strTmp = 0;

   const UINT8 buttonXPosition   = 2;
   const UINT8 buttonYPosition   = LCD_GetYSize() - 50;
   const UINT8 buttonWidth       = 110;
   const UINT8 buttonHeight      = 51;

   switch (s->state)
   {
      case BEFORE_SCREEN_STATE:
      {
         LCD_Clear(LCD_COLOR_BACKGROUND_PAGE);
         LCD_SetFont(&LCD_FONT_24);
         LCD_SetBackColor(LCD_COLOR_BACKGROUND_PAGE);

         //--- Header
         LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_FillRect(0, 0, LCD_GetXSize(), 40);

         LCD_SetTextColor(LCD_COLOR_WHITE);
         LCD_SetBackColor(LCD_COLOR_BACKGROUND_ITEM);
         STRING_COPY(strTmp, "SCREEN 4");
         LCD_DisplayStringAt(50, 2, strTmp, LEFT_MODE);

         LCD_SetTextColor(LCD_COLOR_ORANGE);
         LCD_DrawHLine(0, 40, LCD_GetXSize());
         LCD_DrawHLine(0, 41, LCD_GetXSize());
         LCD_SetFont(&LCD_FONT_12);
         LCD_SetTextColor(LCD_COLOR_GRAY);
         STRING_COPY(strTmp, "Description screen 4");
         LCD_DisplayStringAt(52, 26, strTmp, LEFT_MODE);

         //--- Corps
         LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_FillRect(0, 50, LCD_GetXSize(), 170);
         LCD_SetTextColor(LCD_COLOR_GRAY);
         LCD_DrawHLine(0, 50, LCD_GetXSize());

         //--- Footer
         LCD_SetTextColor(LCD_COLOR_ORANGE);
         LCD_DrawHLine(0, LCD_GetYSize() - 52, LCD_GetXSize());

         LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_FillRect(0, LCD_GetYSize() - 51, LCD_GetXSize(), 51);

         STRING_COPY(strTmp, "Button 1");
         LCD_DrawRectButton(buttonXPosition, buttonYPosition, buttonWidth, buttonHeight, LCD_COLOR_ORANGE, strTmp);

         STRING_COPY(strTmp, "Button 2");
         LCD_DrawRectButton(buttonXPosition + buttonWidth * 1 + 14, buttonYPosition, buttonWidth, buttonHeight, LCD_COLOR_ORANGE, strTmp);

         STRING_COPY(strTmp, "Button 3");
         LCD_DrawRectButton(buttonXPosition + buttonWidth * 2 + 24, buttonYPosition, buttonWidth, buttonHeight, LCD_COLOR_ORANGE, strTmp);

         STRING_COPY(strTmp, "Button 4");
         LCD_DrawRectButton(buttonXPosition + buttonWidth * 3 + 34, buttonYPosition, buttonWidth, buttonHeight, LCD_COLOR_ORANGE, strTmp);

         s->state = DURING_SCREEN_STATE;
         break;
      }

      case DURING_SCREEN_STATE:
      {
         break;
      }

      case AFTER_SCREEN_STATE:
      {
         break;
      }

      default:
      {
         break;
      }
   }

   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_UpdateScr4
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
BOOL SCREEN_UpdateScr4 (void *p, void *e)
{

   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_DispScr5
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
BOOL SCREEN_DispScr5 (void *p)
{
   s_SCREEN *s = (s_SCREEN *)p;
   STRING strTmp = 0;

   const UINT8 buttonXPosition   = 2;
   const UINT8 buttonYPosition   = LCD_GetYSize() - 50;
   const UINT8 buttonWidth       = 110;
   const UINT8 buttonHeight      = 51;

   switch (s->state)
   {
      case BEFORE_SCREEN_STATE:
      {
         LCD_Clear(LCD_COLOR_BACKGROUND_PAGE);
         LCD_SetFont(&LCD_FONT_24);
         LCD_SetBackColor(LCD_COLOR_BACKGROUND_PAGE);

         //--- Header
         LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_FillRect(0, 0, LCD_GetXSize(), 40);

         LCD_SetTextColor(LCD_COLOR_WHITE);
         LCD_SetBackColor(LCD_COLOR_BACKGROUND_ITEM);
         STRING_COPY(strTmp, "SCREEN 5");
         LCD_DisplayStringAt(50, 2, strTmp, LEFT_MODE);

         LCD_SetTextColor(LCD_COLOR_RED);
         LCD_DrawHLine(0, 40, LCD_GetXSize());
         LCD_DrawHLine(0, 41, LCD_GetXSize());
         LCD_SetFont(&LCD_FONT_12);
         LCD_SetTextColor(LCD_COLOR_GRAY);
         STRING_COPY(strTmp, "Description screen 5");
         LCD_DisplayStringAt(52, 26, strTmp, LEFT_MODE);

         //--- Corps
         LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_FillRect(0, 50, LCD_GetXSize(), 170);
         LCD_SetTextColor(LCD_COLOR_GRAY);
         LCD_DrawHLine(0, 50, LCD_GetXSize());

         //--- Footer
         LCD_SetTextColor(LCD_COLOR_RED);
         LCD_DrawHLine(0, LCD_GetYSize() - 52, LCD_GetXSize());

         LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_FillRect(0, LCD_GetYSize() - 51, LCD_GetXSize(), 51);

         STRING_COPY(strTmp, "Button 1");
         LCD_DrawRectButton(buttonXPosition, buttonYPosition, buttonWidth, buttonHeight, LCD_COLOR_RED, strTmp);

         STRING_COPY(strTmp, "Button 2");
         LCD_DrawRectButton(buttonXPosition + buttonWidth * 1 + 14, buttonYPosition, buttonWidth, buttonHeight, LCD_COLOR_RED, strTmp);

         STRING_COPY(strTmp, "Button 3");
         LCD_DrawRectButton(buttonXPosition + buttonWidth * 2 + 24, buttonYPosition, buttonWidth, buttonHeight, LCD_COLOR_RED, strTmp);

         STRING_COPY(strTmp, "Button 4");
         LCD_DrawRectButton(buttonXPosition + buttonWidth * 3 + 34, buttonYPosition, buttonWidth, buttonHeight, LCD_COLOR_RED, strTmp);

         s->state = DURING_SCREEN_STATE;
         break;
      }

      case DURING_SCREEN_STATE:
      {
         break;
      }

      case AFTER_SCREEN_STATE:
      {
         break;
      }

      default:
      {
         break;
      }
   }

   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_UpdateScr5
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
BOOL SCREEN_UpdateScr5 (void *p, void *e)
{

   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_LoadNext
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
BOOL SCREEN_LoadNext (s_SCREEN *s, UINT8 idScr)
{
   BOOL success = FALSE;
   UINT8 oldScr = NO_SCREEN;

   if (idScr > NO_SCREEN && idScr < NB_SCREEN)
   {
      oldScr = s->idScr;

      *s = _scr[idScr];

      s->oldScr = oldScr;
      s->state = BEFORE_SCREEN_STATE;

      success = TRUE;
   }

   return success;
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_Display
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
BOOL SCREEN_Display (s_SCREEN* s)
{
   BOOL dispDone = FALSE;

   //--- Fonction d'affichage
   dispDone = s->disp(s);

   return dispDone;
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_Initialize
//
// DESCRIPTION : Screen initialization
//-----------------------------------------------------------------------------
void SCREEN_Initialize (void)
{
   SCREEN_LoadNext(&_curScr, SCREEN_1);
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_TaskRun
//
// DESCRIPTION : Screen task run
//-----------------------------------------------------------------------------
void SCREEN_TaskRun (void *argument)
{
   osStatus_t eventStatus;
   BOOL receivedButtonState;
   static UINT8 _idScr = SCREEN_1;

   //--- Remove compiler warning about unused parameter.
   (void)argument;

   for ( ;; )
   {
      //--- Wait until something arrives in the queue
      eventStatus = osMessageQueueGet(SCREEN_Event, &receivedButtonState, NULL, 0);

      if (eventStatus == osOK)
      {
         _idScr++;
         _idScr %= NB_SCREEN;
         if (_idScr == NO_SCREEN)
            _idScr = SCREEN_1;

         //--- Drive output led pin
         SCREEN_LoadNext(&_curScr, _idScr);
      }

      SCREEN_Display(&_curScr);
   }
}
