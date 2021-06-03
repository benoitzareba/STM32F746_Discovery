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
#include "Utils.h"
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
osThreadId_t         SCREEN_TaskHandle;
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

static BOOL       _updateFooter = FALSE;
static s_FOOTER   _footer[] = {
                                 //--- Footer scr 0
                                 {
                                    0,
                                    LCD_COLOR_TRANSPARENT,
                                    {
                                       {0, FALSE, LCD_COLOR_TRANSPARENT, "", "", {0, 0, 0, 0}},
                                       {0, FALSE, LCD_COLOR_TRANSPARENT, "", "", {0, 0, 0, 0}},
                                       {0, FALSE, LCD_COLOR_TRANSPARENT, "", "", {0, 0, 0, 0}},
                                       {0, FALSE, LCD_COLOR_TRANSPARENT, "", "", {0, 0, 0, 0}},
                                    }
                                 },


                                 //--- Footer scr 1
                                 {
                                    4,
                                    LCD_COLOR_LIGHTCYAN,
                                    {
                                       {0, FALSE, LCD_COLOR_LIGHTCYAN,   "Screen 1" , "DispScreen1", {0, 0, 0, 0}},
                                       {1, FALSE, LCD_COLOR_LIGHTGREEN,  "Screen 2" , "DispScreen2", {0, 0, 0, 0}},
                                       {2, FALSE, LCD_COLOR_LIGHTYELLOW, "Screen 3" , "DispScreen3", {0, 0, 0, 0}},
                                       {3, FALSE, LCD_COLOR_LIGHTRED,    "Screen 4" , "DispScreen4", {0, 0, 0, 0}},
                                    }
                                 },

                                 //--- Footer scr 2
                                 {
                                    4,
                                    LCD_COLOR_LIGHTGREEN,
                                    {
                                       {0, FALSE, LCD_COLOR_LIGHTCYAN,   "Screen 1" , "DispScreen1", {0, 0, 0, 0}},
                                       {1, FALSE, LCD_COLOR_LIGHTGREEN,  "Screen 2" , "DispScreen2", {0, 0, 0, 0}},
                                       {2, FALSE, LCD_COLOR_LIGHTYELLOW, "Screen 3" , "DispScreen3", {0, 0, 0, 0}},
                                       {3, FALSE, LCD_COLOR_LIGHTRED,    "Screen 4" , "DispScreen4", {0, 0, 0, 0}},
                                    }
                                 },

                                 //--- Footer scr 3
                                 {
                                    4,
                                    LCD_COLOR_LIGHTYELLOW,
                                    {
                                       {0, FALSE, LCD_COLOR_LIGHTCYAN,   "Screen 1" , "DispScreen1", {0, 0, 0, 0}},
                                       {1, FALSE, LCD_COLOR_LIGHTGREEN,  "Screen 2" , "DispScreen2", {0, 0, 0, 0}},
                                       {2, FALSE, LCD_COLOR_LIGHTYELLOW, "Screen 3" , "DispScreen3", {0, 0, 0, 0}},
                                       {3, FALSE, LCD_COLOR_LIGHTRED,    "Screen 4" , "DispScreen4", {0, 0, 0, 0}},
                                    }
                                 },

                                 //--- Footer scr 4
                                 {
                                    4,
                                    LCD_COLOR_LIGHTRED,
                                    {
                                       {0, FALSE, LCD_COLOR_LIGHTCYAN,   "Screen 1" , "DispScreen1", {0, 0, 0, 0}},
                                       {1, FALSE, LCD_COLOR_LIGHTGREEN,  "Screen 2" , "DispScreen2", {0, 0, 0, 0}},
                                       {2, FALSE, LCD_COLOR_LIGHTYELLOW, "Screen 3" , "DispScreen3", {0, 0, 0, 0}},
                                       {3, FALSE, LCD_COLOR_LIGHTRED,    "Screen 4" , "DispScreen4", {0, 0, 0, 0}},
                                    }
                                 },
                              };

static BOOL _updateSlideMenu = FALSE;

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
         //POPUP_Informations(s);
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
// FONCTION    : SCREEN_UpdatePopup
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
BOOL SCREEN_DispPopup (void *p)
{
   s_SCREEN *s = (s_SCREEN *)p;
   s_POPUP *pop = s->popup;

   switch (s->state)
   {
      case BEFORE_SCREEN_STATE:
      {
         //--- Affichage de la popup
         DISP_ShowPopup(pop, s->header->color);

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
// FONCTION    : SCREEN_UpdatePopup
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
BOOL SCREEN_UpdatePopup (void *p, void *e)
{
   s_SCREEN *s = (s_SCREEN *)p;

   SCREEN_ClearPopup(s);

   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_ShowPopup
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
void SCREEN_ShowPopup (s_SCREEN *s, s_POPUP *popup)
{
   if (s->popup == NULL)
   {
      s->state = BEFORE_SCREEN_STATE;

      popup->prevDisp = s->disp;
      popup->prevUpdate = s->update;

      s->disp     = SCREEN_DispPopup;
      s->update   = SCREEN_UpdatePopup;
      s->popup    = popup;
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_ShowPopup
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
void SCREEN_ClearPopup (s_SCREEN *s)
{
   s->disp = s->popup->prevDisp;
   s->update = s->popup->prevUpdate;

   s->state = BEFORE_SCREEN_STATE;

   LCD_Clear(LCD_COLOR_BACKGROUND_PAGE);

   _updateFooter = TRUE;
   _updateHeader = TRUE;
   _updateSlideMenu = TRUE;

   s->popup = NULL;
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_LoadNext
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
BOOL SCREEN_LoadNext (s_SCREEN *s, CHAR8* menuName)
{
   BOOL success = FALSE;
   BOOL screenLoaded = FALSE;

   //--- Chargement de l'ecran
   screenLoaded = SCREEN_LoadAndUpdate(menuName, s);

   if (screenLoaded == TRUE)
   {
      _updateHeader = TRUE;
      _updateFooter = TRUE;
      _updateSlideMenu = TRUE;

      success = TRUE;
   }

   return success;
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_LoadAndUpdate
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
BOOL SCREEN_LoadAndUpdate (CHAR8* menuName, s_SCREEN* s)
{
   BOOL success = TRUE;
   UINT8 i;
   UINT32 hashMenuName;

   //--- Sauvegarde de l'ecran precedent
   s->oldScr = s->idScr;

   //--- Conversion de la fonction en hash
   hashMenuName = UTILS_CalcHash(menuName);

   //--- Charge les fonctions disp et update de la fonction
   switch (hashMenuName)
   {
      case HASH_DISPSCREEN1:
      {
         s->idScr    = SCREEN_1;
         s->disp     = SCREEN_DispScr1;
         s->update   = SCREEN_UpdateScr1;
         break;
      }

      case HASH_DISPSCREEN2:
      {
         s->idScr    = SCREEN_2;
         s->disp     = SCREEN_DispScr2;
         s->update   = SCREEN_UpdateScr2;
         break;
      }

      case HASH_DISPSCREEN3:
      {
         s->idScr    = SCREEN_3;
         s->disp     = SCREEN_DispScr3;
         s->update   = SCREEN_UpdateScr3;
         break;
      }

      case HASH_DISPSCREEN4:
      {
         s->idScr    = SCREEN_4;
         s->disp     = SCREEN_DispScr4;
         s->update   = SCREEN_UpdateScr4;
         break;
      }

      default:
      {
         success = FALSE;
         break;
      }
   }

   //--- Si la fonction existe
   if (success == TRUE)
   {
      if (s->oldScr != s->idScr)
      {
         //--- MaJ des variables
         s->state = BEFORE_SCREEN_STATE;

         //--- Chargement des headers et footers associes
         s->header = &_header[s->idScr];
         s->footer = &_footer[s->idScr];

         for (i = 0; i < NB_BUTTON_MAX; i++)
            s->footer->button[i].selected = FALSE;
         s->footer->button[s->idScr - 1].selected = TRUE;

         s->popup = NULL;
      }
      else
         success = FALSE;
   }

   return success;
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_Update
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
BOOL SCREEN_Update (s_SCREEN* s, void* e)
{
   BOOL updateDone = FALSE;

   //--- Fonction d'update de l'ecran
   updateDone = s->update(s, e);

   return updateDone;
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

   if (_updateSlideMenu == TRUE)
      DISP_ShowSlideMenu(s->header);

   return dispDone;
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_Initialize
//
// DESCRIPTION : Screen initialization
//-----------------------------------------------------------------------------
void SCREEN_Initialize (void)
{
   //--- LCD Initialization
   LCD_Init();

   //--- Clear
   LCD_Clear(LCD_COLOR_BACKGROUND_PAGE);

   //--- Load first screen
   SCREEN_LoadNext(&_curScr, (STRING)"DispScreen1");
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_TaskRun
//
// DESCRIPTION : Screen task run
//-----------------------------------------------------------------------------
void SCREEN_TaskRun (void *argument)
{
   STRING_TAB function;
   UINT8 idItem = NO_TOUCH_ITEM;
   osStatus_t eventStatus;

   //--- Remove compiler warning about unused parameter.
   (void)argument;

   for ( ;; )
   {
      //--- Evenement changement d'ecran
      eventStatus = osMessageQueueGet(CHANGE_SCREEN_Event, &function, NULL, 0);
      if (eventStatus == osOK)
      {
         SCREEN_LoadNext(&_curScr, (STRING)function);
      }

      eventStatus = osMessageQueueGet(POPUP_Event, &idItem, NULL, 0);
      if (eventStatus == osOK)
      {
         SCREEN_Update(&_curScr, &idItem);
      }

      //--- Send current screen to touchscreen task
      osMessageQueuePut(SCREEN_Event, &_curScr, 0, 0);

      SCREEN_Display(&_curScr);

      _updateHeader     = FALSE;
      _updateFooter     = FALSE;
      _updateSlideMenu  = FALSE;
   }
}
