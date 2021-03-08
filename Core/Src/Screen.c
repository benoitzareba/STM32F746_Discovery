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
#include "Display.h"
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
static BOOL       _updateHeader = FALSE;
static s_HEADER   _header[] = {
                                 {"NO TITLE",   "NO DESCRIPTION",       LCD_COLOR_TRANSPARENT},
                                 {"SCREEN 1",   "Description screen 1", LCD_COLOR_LIGHTCYAN},
                                 {"SCREEN 2",   "Description screen 2", LCD_COLOR_LIGHTGREEN},
                                 {"SCREEN 3",   "Description screen 3", LCD_COLOR_LIGHTYELLOW},
                                 {"SCREEN 4",   "Description screen 4", LCD_COLOR_LIGHTRED},
                              };

static s_SCREEN   _curScr;
static s_SCREEN   _scr[] =    {
                                 {NO_SCREEN, NO_SCREEN,  NO_SCREEN_STATE,  NULL,             NULL,                LCD_COLOR_TRANSPARENT,  NULL, NULL},
                                 {SCREEN_1,  NO_SCREEN,  NO_SCREEN_STATE,  SCREEN_DispScr1,  SCREEN_UpdateScr1,   LCD_COLOR_LIGHTCYAN,    NULL, NULL},
                                 {SCREEN_2,  NO_SCREEN,  NO_SCREEN_STATE,  SCREEN_DispScr2,  SCREEN_UpdateScr2,   LCD_COLOR_LIGHTGREEN,   NULL, NULL},
                                 {SCREEN_3,  NO_SCREEN,  NO_SCREEN_STATE,  SCREEN_DispScr3,  SCREEN_UpdateScr3,   LCD_COLOR_LIGHTYELLOW,  NULL, NULL},
                                 {SCREEN_4,  NO_SCREEN,  NO_SCREEN_STATE,  SCREEN_DispScr4,  SCREEN_UpdateScr4,   LCD_COLOR_LIGHTRED,     NULL, NULL},
                              };

static BOOL       _updateFooter = FALSE;
static s_FOOTER   _footer[] = {
                                 //--- Footer scr 0
                                 {
                                    0,
                                    LCD_COLOR_TRANSPARENT,
                                    {
                                       {0, FALSE, LCD_COLOR_TRANSPARENT, "", ""},
                                       {0, FALSE, LCD_COLOR_TRANSPARENT, "", ""},
                                       {0, FALSE, LCD_COLOR_TRANSPARENT, "", ""},
                                       {0, FALSE, LCD_COLOR_TRANSPARENT, "", ""},
                                    }
                                 },


                                 //--- Footer scr 1
                                 {
                                    4,
                                    LCD_COLOR_LIGHTCYAN,
                                    {
                                       {0, FALSE, LCD_COLOR_LIGHTCYAN,   "Screen 1" , "DispScreen1" },
                                       {1, FALSE, LCD_COLOR_LIGHTGREEN,  "Screen 2" , "DispScreen2" },
                                       {2, FALSE, LCD_COLOR_LIGHTYELLOW, "Screen 3" , "DispScreen3" },
                                       {3, FALSE, LCD_COLOR_LIGHTRED,    "Screen 4" , "DispScreen4" },
                                    }
                                 },

                                 //--- Footer scr 2
                                 {
                                    4,
                                    LCD_COLOR_LIGHTGREEN,
                                    {
                                       {0, FALSE, LCD_COLOR_LIGHTCYAN,   "Screen 1" , "DispScreen1" },
                                       {1, FALSE, LCD_COLOR_LIGHTGREEN,  "Screen 2" , "DispScreen2" },
                                       {2, FALSE, LCD_COLOR_LIGHTYELLOW, "Screen 3" , "DispScreen3" },
                                       {3, FALSE, LCD_COLOR_LIGHTRED,    "Screen 4" , "DispScreen4" },
                                    }
                                 },

                                 //--- Footer scr 3
                                 {
                                    4,
                                    LCD_COLOR_LIGHTYELLOW,
                                    {
                                       {0, FALSE, LCD_COLOR_LIGHTCYAN,   "Screen 1" , "DispScreen1" },
                                       {1, FALSE, LCD_COLOR_LIGHTGREEN,  "Screen 2" , "DispScreen2" },
                                       {2, FALSE, LCD_COLOR_LIGHTYELLOW, "Screen 3" , "DispScreen3" },
                                       {3, FALSE, LCD_COLOR_LIGHTRED,    "Screen 4" , "DispScreen4" },
                                    }
                                 },

                                 //--- Footer scr 4
                                 {
                                    4,
                                    LCD_COLOR_LIGHTRED,
                                    {
                                       {0, FALSE, LCD_COLOR_LIGHTCYAN,   "Screen 1" , "DispScreen1" },
                                       {1, FALSE, LCD_COLOR_LIGHTGREEN,  "Screen 2" , "DispScreen2" },
                                       {2, FALSE, LCD_COLOR_LIGHTYELLOW, "Screen 3" , "DispScreen3" },
                                       {3, FALSE, LCD_COLOR_LIGHTRED,    "Screen 4" , "DispScreen4" },
                                    }
                                 },
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

   switch (s->state)
   {
      case BEFORE_SCREEN_STATE:
      {
         //--- Corps
         LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_FillRect(0, 50, LCD_GetXSize(), 170);
         LCD_SetTextColor(LCD_COLOR_GRAY);
         LCD_DrawHLine(0, 50, LCD_GetXSize());

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

   switch (s->state)
   {
      case BEFORE_SCREEN_STATE:
      {
         //--- Corps
         LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_FillRect(0, 50, LCD_GetXSize(), 170);
         LCD_SetTextColor(LCD_COLOR_GRAY);
         LCD_DrawHLine(0, 50, LCD_GetXSize());

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

   switch (s->state)
   {
      case BEFORE_SCREEN_STATE:
      {
         //--- Corps
         LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_FillRect(0, 50, LCD_GetXSize(), 170);
         LCD_SetTextColor(LCD_COLOR_GRAY);
         LCD_DrawHLine(0, 50, LCD_GetXSize());

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

   switch (s->state)
   {
      case BEFORE_SCREEN_STATE:
      {
         //--- Corps
         LCD_SetTextColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_FillRect(0, 50, LCD_GetXSize(), 170);
         LCD_SetTextColor(LCD_COLOR_GRAY);
         LCD_DrawHLine(0, 50, LCD_GetXSize());

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
// FONCTION    : SCREEN_LoadNext
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
BOOL SCREEN_LoadNext (s_SCREEN *s, UINT8 idScr)
{
   BOOL success = FALSE;
   UINT8 oldScr = NO_SCREEN;
   UINT8 i;

   if (idScr > NO_SCREEN && idScr < NB_SCREEN)
   {
      //--- Sauvegarde de l'ecran precedent
      oldScr = s->idScr;

      //--- Chargement de l'ecran
      *s = _scr[idScr];

      //--- MaJ des variables
      s->oldScr = oldScr;
      s->state = BEFORE_SCREEN_STATE;

      //--- Chargement des headers et footers associes
      s->header = &_header[s->idScr];
      s->footer = &_footer[s->idScr];

      for (i = 0; i < NB_BUTTON_MAX; i++)
         s->footer->button[i].selected = FALSE;
      s->footer->button[s->idScr - 1].selected = TRUE;

      _updateHeader = TRUE;
      _updateFooter = TRUE;

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

   //--- Disp header
   if (_updateHeader == TRUE)
      DISP_ShowHeader(s->header);

   if (_updateFooter == TRUE)
      DISP_ShowFooter(s->footer);

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
   LCD_Clear(LCD_COLOR_BACKGROUND_PAGE);

   SCREEN_LoadNext(&_curScr, SCREEN_1);
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_TaskRun
//
// DESCRIPTION : Screen task run
//-----------------------------------------------------------------------------
void SCREEN_TaskRun (void *argument)
{
   osStatus_t  eventStatus;
   BOOL        receivedButtonState;

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

      _updateHeader = FALSE;
      _updateFooter = FALSE;
   }
}
