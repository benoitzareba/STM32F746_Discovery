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
#include "Event.h"
#include "Utils.h"
#include "Widgets.h"
#include "SlideBar.h"
#include "ToggleSwitch.h"
#include "RadioButton.h"
#include "CheckBox.h"
#include "NumberInput.h"
#include "CircleProgress.h"
#include "ProgressBar.h"
#include "ButtonWidget.h"
#include "TextInput.h"
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
static s_SCREEN   _curScr;

static BOOL       _updateHeader = FALSE;
static s_HEADER   _header[] = {
                                 {"NO TITLE",   "NO DESCRIPTION",       LCD_COLOR_TRANSPARENT},
                                 {"SCREEN 1",   "Description screen 1", LCD_COLOR_LIGHTCYAN},
                                 {"SCREEN 2",   "Description screen 2", LCD_COLOR_LIGHTGREEN},
                                 {"SCREEN 3",   "Description screen 3", LCD_COLOR_LIGHTYELLOW},
                                 {"SCREEN 4",   "Description screen 4", LCD_COLOR_LIGHTRED},
                              };

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

static s_SLIDE _slide[] =     {
                                 //--- Slide scr 0
                                 {CLOSE,  LCD_COLOR_LIGHTCYAN,    4, {{LCD_COLOR_LIGHTGREEN, "Wifi"}, {LCD_COLOR_LIGHTRED, "Bluetooth"}, {LCD_COLOR_LIGHTYELLOW, "Ethernet"},  {LCD_COLOR_LIGHTCYAN, "VPN"}}},

                                 //--- Slide scr 1
                                 {CLOSE,  LCD_COLOR_LIGHTGREEN,   3, {{LCD_COLOR_LIGHTRED, "Facebook"}, {LCD_COLOR_LIGHTYELLOW, "Instagram"}, {LCD_COLOR_LIGHTCYAN, "Snapchat"}, {0, ""}}},

                                 //--- Slide scr 2
                                 {CLOSE,  LCD_COLOR_LIGHTYELLOW,  2, {{LCD_COLOR_LIGHTYELLOW, "Date"}, {LCD_COLOR_LIGHTCYAN, "Hour"}, {0, ""},  {0, ""}}},

                                 //--- Slide scr 3
                                 {CLOSE,  LCD_COLOR_LIGHTRED,     1, {{LCD_COLOR_LIGHTCYAN, "Update"}, {0,""}, {0,""}, {0,""}}},
                              };

static BOOL _updateSlideMenu = TRUE;

//--- Screen 1
static s_WIDGET *_slideWidget;
static s_WIDGET *_slideWidget2;
static s_WIDGET *_toggleSwitchWidget;
static s_WIDGET *_toggleSwitchWidget2;
static s_WIDGET *_toggleSwitchWidget3;
static s_WIDGET *_toggleSwitchWidget4;
static s_WIDGET *_radioButtonWidget;
static s_WIDGET *_radioButtonWidget2;
static s_WIDGET *_radioButtonWidget3;
static s_WIDGET *_radioButtonWidget4;
static s_WIDGET *_checkBoxWidget;
static s_WIDGET *_checkBoxWidget2;
static s_WIDGET *_checkBoxWidget3;
static s_WIDGET *_checkBoxWidget4;

//--- Screen 2
static s_WIDGET *_numberInputWidget;
static s_WIDGET *_circleProgressWidget;
static s_WIDGET *_circleProgressWidget2;
static s_WIDGET *_circleProgressWidget3;
static s_WIDGET *_circleProgressWidget4;

//--- Screen 3
static s_WIDGET *_progressBarWidget;
static s_WIDGET *_progressBarWidget2;
static s_WIDGET *_progressBarWidget3;
static s_WIDGET *_progressBarWidget4;
static s_WIDGET *_progressBarWidget5;

//--- Screen 4
static s_WIDGET *_ButtonWidget;
static s_WIDGET *_ButtonWidget2;
static s_WIDGET *_ButtonWidget3;
static s_WIDGET *_ButtonWidget4;
static s_WIDGET *_ButtonWidget5;
static s_WIDGET *_ButtonWidget6;
static s_WIDGET *_ButtonWidget7;
static s_WIDGET *_ButtonWidget8;
static s_WIDGET *_ButtonWidget9;
static s_WIDGET *_ButtonWidget0;
static s_WIDGET *_ButtonWidgetClear;
static s_WIDGET *_ButtonWidgetEnter;
static s_WIDGET *_TextInputWidget;

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
   s_SCREEN*               s = (s_SCREEN *)p;
   s_WIDGET_SLIDE_BAR      slideParam;
   s_WIDGET_SLIDE_BAR      slideParam2;
   s_WIDGET_TOGGLE_SWITCH  toggleSwitchParam;
   s_WIDGET_TOGGLE_SWITCH  toggleSwitchParam2;
   s_WIDGET_TOGGLE_SWITCH  toggleSwitchParam3;
   s_WIDGET_TOGGLE_SWITCH  toggleSwitchParam4;
   s_WIDGET_RADIOBUTTON    radioButtonParam;
   s_WIDGET_RADIOBUTTON    radioButtonParam2;
   s_WIDGET_RADIOBUTTON    radioButtonParam3;
   s_WIDGET_RADIOBUTTON    radioButtonParam4;
   s_WIDGET_CHECKBOX       checkBoxParam;
   s_WIDGET_CHECKBOX       checkBoxParam2;
   s_WIDGET_CHECKBOX       checkBoxParam3;
   s_WIDGET_CHECKBOX       checkBoxParam4;

   switch (s->state)
   {
      case BEFORE_SCREEN_STATE:
      {
         DISP_ShowBodyZone();

         if (s->initWidget == TRUE)
         {
            s->initWidget = FALSE;

            //------
            //--- Allocations
            //------

            //--- Slide widget
            _slideWidget = WIDGET_Alloc(WIDGET_SLIDE_TYPE, WIDGET_FULL_CONTROL_MODE);
            _slideWidget2 = WIDGET_Alloc(WIDGET_SLIDE_TYPE, WIDGET_FULL_CONTROL_MODE);

            //--- Toggle switch widget
            _toggleSwitchWidget = WIDGET_Alloc(WIDGET_TOGGLE_SWITCH_TYPE, WIDGET_FULL_CONTROL_MODE);
            _toggleSwitchWidget2 = WIDGET_Alloc(WIDGET_TOGGLE_SWITCH_TYPE, WIDGET_FULL_CONTROL_MODE);
            _toggleSwitchWidget3 = WIDGET_Alloc(WIDGET_TOGGLE_SWITCH_TYPE, WIDGET_FULL_CONTROL_MODE);
            _toggleSwitchWidget4 = WIDGET_Alloc(WIDGET_TOGGLE_SWITCH_TYPE, WIDGET_FULL_CONTROL_MODE);

            //--- Radio button widget
            _radioButtonWidget = WIDGET_Alloc(WIDGET_RADIO_BUTTON_TYPE, WIDGET_FULL_CONTROL_MODE);
            _radioButtonWidget2 = WIDGET_Alloc(WIDGET_RADIO_BUTTON_TYPE, WIDGET_FULL_CONTROL_MODE);
            _radioButtonWidget3 = WIDGET_Alloc(WIDGET_RADIO_BUTTON_TYPE, WIDGET_FULL_CONTROL_MODE);
            _radioButtonWidget4 = WIDGET_Alloc(WIDGET_RADIO_BUTTON_TYPE, WIDGET_FULL_CONTROL_MODE);

            //--- Checkbox widget
            _checkBoxWidget = WIDGET_Alloc(WIDGET_CHECKBOX_TYPE, WIDGET_FULL_CONTROL_MODE);
            _checkBoxWidget2 = WIDGET_Alloc(WIDGET_CHECKBOX_TYPE, WIDGET_FULL_CONTROL_MODE);
            _checkBoxWidget3 = WIDGET_Alloc(WIDGET_CHECKBOX_TYPE, WIDGET_FULL_CONTROL_MODE);
            _checkBoxWidget4 = WIDGET_Alloc(WIDGET_CHECKBOX_TYPE, WIDGET_FULL_CONTROL_MODE);

            //------
            //--- Positions
            //------

            //--- Slide widget
            _slideWidget->posX               = 220;
            _slideWidget->posY               = 70;

            _slideWidget2->posX              = 390;
            _slideWidget2->posY              = 70;

            //--- Toggle switch widget
            _toggleSwitchWidget->posX        = 240;
            _toggleSwitchWidget->posY        = 110;

            _toggleSwitchWidget2->posX       = 300;
            _toggleSwitchWidget2->posY       = 110;

            _toggleSwitchWidget3->posX       = 360;
            _toggleSwitchWidget3->posY       = 110;

            _toggleSwitchWidget4->posX       = 420;
            _toggleSwitchWidget4->posY       = 110;

            //--- Radio button widget
            _radioButtonWidget->posX         = 248;
            _radioButtonWidget->posY         = 150;

            _radioButtonWidget2->posX        = 308;
            _radioButtonWidget2->posY        = 150;

            _radioButtonWidget3->posX        = 368;
            _radioButtonWidget3->posY        = 150;

            _radioButtonWidget4->posX        = 428;
            _radioButtonWidget4->posY        = 150;

            //--- Checkbox widget
            _checkBoxWidget->posX            = 240;
            _checkBoxWidget->posY            = 190;

            _checkBoxWidget2->posX           = 300;
            _checkBoxWidget2->posY           = 190;

            _checkBoxWidget3->posX           = 360;
            _checkBoxWidget3->posY           = 190;

            _checkBoxWidget4->posX           = 420;
            _checkBoxWidget4->posY           = 190;

            //------
            //--- Parametrages
            //------

            //--- Slide widget
            slideParam.color                    = LCD_COLOR_LIGHTCYAN;
            slideParam.baseColor                = LCD_COLOR_BACKGROUND_PAGE;
            slideParam.backgroundColor          = LCD_COLOR_BACKGROUND_ITEM;
            slideParam.currentValue             = 25.;
            slideParam.length                   = 150;

            //--- Slide widget
            slideParam2.color                   = LCD_COLOR_LIGHTCYAN;
            slideParam2.baseColor               = LCD_COLOR_BACKGROUND_PAGE;
            slideParam2.backgroundColor         = LCD_COLOR_BACKGROUND_ITEM;
            slideParam2.currentValue            = 75.;
            slideParam2.length                  = 70;

            //--- Toggle switch widget
            toggleSwitchParam.colorOff          = LCD_COLOR_GRAY;
            toggleSwitchParam.colorOn           = LCD_COLOR_LIGHTCYAN;
            toggleSwitchParam.currentValue      = TRUE;

            toggleSwitchParam2.colorOff         = LCD_COLOR_GRAY;
            toggleSwitchParam2.colorOn          = LCD_COLOR_LIGHTCYAN;
            toggleSwitchParam2.currentValue     = FALSE;

            toggleSwitchParam3.colorOff         = LCD_COLOR_GRAY;
            toggleSwitchParam3.colorOn          = LCD_COLOR_LIGHTCYAN;
            toggleSwitchParam3.currentValue     = TRUE;

            toggleSwitchParam4.colorOff         = LCD_COLOR_GRAY;
            toggleSwitchParam4.colorOn          = LCD_COLOR_LIGHTCYAN;
            toggleSwitchParam4.currentValue     = FALSE;

            //--- Radio button widget
            radioButtonParam.color              = LCD_COLOR_LIGHTCYAN;
            radioButtonParam.backgroundColor    = LCD_COLOR_BACKGROUND_ITEM;
            radioButtonParam.currentValue       = TRUE;

            radioButtonParam2.color             = LCD_COLOR_LIGHTCYAN;
            radioButtonParam2.backgroundColor   = LCD_COLOR_BACKGROUND_ITEM;
            radioButtonParam2.currentValue      = FALSE;

            radioButtonParam3.color             = LCD_COLOR_LIGHTCYAN;
            radioButtonParam3.backgroundColor   = LCD_COLOR_BACKGROUND_ITEM;
            radioButtonParam3.currentValue      = TRUE;

            radioButtonParam4.color             = LCD_COLOR_LIGHTCYAN;
            radioButtonParam4.backgroundColor   = LCD_COLOR_BACKGROUND_ITEM;
            radioButtonParam4.currentValue      = FALSE;

            //--- Checkbox widget
            checkBoxParam.color                 = LCD_COLOR_LIGHTCYAN;
            checkBoxParam.backgroundColor       = LCD_COLOR_BACKGROUND_ITEM;
            checkBoxParam.currentValue          = TRUE;

            checkBoxParam2.color                = LCD_COLOR_LIGHTCYAN;
            checkBoxParam2.backgroundColor      = LCD_COLOR_BACKGROUND_ITEM;
            checkBoxParam2.currentValue         = FALSE;

            checkBoxParam3.color                = LCD_COLOR_LIGHTCYAN;
            checkBoxParam3.backgroundColor      = LCD_COLOR_BACKGROUND_ITEM;
            checkBoxParam3.currentValue         = FALSE;

            checkBoxParam4.color                = LCD_COLOR_LIGHTCYAN;
            checkBoxParam4.backgroundColor      = LCD_COLOR_BACKGROUND_ITEM;
            checkBoxParam4.currentValue         = FALSE;
         }

         //------
         //--- Initialisation
         //------

         //--- Slide widget
         WIDGET_Init(_slideWidget, &slideParam);
         WIDGET_Init(_slideWidget2, &slideParam2);

         //--- Toggle switch widget
         WIDGET_Init(_toggleSwitchWidget, &toggleSwitchParam);
         WIDGET_Init(_toggleSwitchWidget2, &toggleSwitchParam2);
         WIDGET_Init(_toggleSwitchWidget3, &toggleSwitchParam3);
         WIDGET_Init(_toggleSwitchWidget4, &toggleSwitchParam4);

         //--- Radio button widget
         WIDGET_Init(_radioButtonWidget, &radioButtonParam);
         WIDGET_Init(_radioButtonWidget2, &radioButtonParam2);
         WIDGET_Init(_radioButtonWidget3, &radioButtonParam3);
         WIDGET_Init(_radioButtonWidget4, &radioButtonParam4);

         //--- Checkbox widget
         WIDGET_Init(_checkBoxWidget, &checkBoxParam);
         WIDGET_Init(_checkBoxWidget2, &checkBoxParam2);
         WIDGET_Init(_checkBoxWidget3, &checkBoxParam3);
         WIDGET_Init(_checkBoxWidget4, &checkBoxParam4);

         s->state = DURING_SCREEN_STATE;
         break;
      }

      case DURING_SCREEN_STATE:
      {
         //--- Affichage slide
         LCD_SetFont(&LCD_FONT_16);
         LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
         LCD_SetBackColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_DisplayStringAt(20, 64, (STRING)"Slide\0", LEFT_MODE);
         WIDGET_Display(_slideWidget);
         WIDGET_Display(_slideWidget2);

         //--- Affichage toggle switch
         LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
         LCD_SetBackColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_DisplayStringAt(20, 104, (STRING)"Toggle switch\0", LEFT_MODE);
         WIDGET_Display(_toggleSwitchWidget);
         WIDGET_Display(_toggleSwitchWidget2);
         WIDGET_Display(_toggleSwitchWidget3);
         WIDGET_Display(_toggleSwitchWidget4);

         //--- Radio bouton
         LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
         LCD_SetBackColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_DisplayStringAt(20, 144, (STRING)"Radio button\0", LEFT_MODE);
         WIDGET_Display(_radioButtonWidget);
         WIDGET_Display(_radioButtonWidget2);
         WIDGET_Display(_radioButtonWidget3);
         WIDGET_Display(_radioButtonWidget4);

         //--- Checkbox
         LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
         LCD_SetBackColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_DisplayStringAt(20, 184, (STRING)"Checkbox\0", LEFT_MODE);
         WIDGET_Display(_checkBoxWidget);
         WIDGET_Display(_checkBoxWidget2);
         WIDGET_Display(_checkBoxWidget3);
         WIDGET_Display(_checkBoxWidget4);
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
   s_WIDGET_NUMBER_INPUT numberInputParam;
   s_WIDGET_CIRCLE_PROGRESS circleProgressParam;
   s_WIDGET_CIRCLE_PROGRESS circleProgressParam2;
   s_WIDGET_CIRCLE_PROGRESS circleProgressParam3;
   s_WIDGET_CIRCLE_PROGRESS circleProgressParam4;

   static TickType_t _tick = 0;

   switch (s->state)
   {
      case BEFORE_SCREEN_STATE:
      {
         //--- Corps
         DISP_ShowBodyZone();

         if (s->initWidget == TRUE)
         {
            s->initWidget = FALSE;

            //------
            //--- Allocation
            //------
            _numberInputWidget                     = WIDGET_Alloc(WIDGET_NUMBER_INPUT_TYPE, WIDGET_FULL_CONTROL_MODE);
            _circleProgressWidget                  = WIDGET_Alloc(WIDGET_CIRCLE_PROGRESS_TYPE, WIDGET_DISPLAY_ONLY_MODE);
            _circleProgressWidget2                 = WIDGET_Alloc(WIDGET_CIRCLE_PROGRESS_TYPE, WIDGET_DISPLAY_ONLY_MODE);
            _circleProgressWidget3                 = WIDGET_Alloc(WIDGET_CIRCLE_PROGRESS_TYPE, WIDGET_DISPLAY_ONLY_MODE);
            _circleProgressWidget4                 = WIDGET_Alloc(WIDGET_CIRCLE_PROGRESS_TYPE, WIDGET_DISPLAY_ONLY_MODE);

            //------
            //--- Position
            //------
            _numberInputWidget->posX               = 200;
            _numberInputWidget->posY               = 60;

            _circleProgressWidget->posX            = 45;
            _circleProgressWidget->posY            = 162 + 22;

            _circleProgressWidget2->posX           = 142;
            _circleProgressWidget2->posY           = 162 + 8;

            _circleProgressWidget3->posX           = 258;
            _circleProgressWidget3->posY           = 162;

            _circleProgressWidget4->posX           = 395;
            _circleProgressWidget4->posY           = 162 - 12;

            //------
            //--- Configuration
            //------
            numberInputParam.width                 = 130;
            numberInputParam.height                = 40;
            numberInputParam.backgroundColor       = LCD_COLOR_BACKGROUND_ITEM;
            numberInputParam.color                 = LCD_COLOR_LIGHTGREEN;
            numberInputParam.currentValue          = 0;

            circleProgressParam.color              = LCD_COLOR_LIGHTYELLOW;
            circleProgressParam.backgroundColor    = LCD_COLOR_DARKGRAY;
            circleProgressParam.maskColor          = LCD_COLOR_BACKGROUND_ITEM;
            circleProgressParam.currentValue       = 25;
            circleProgressParam.radius             = 28;
            circleProgressParam.dispVal            = TRUE;

            circleProgressParam2.color             = LCD_COLOR_LIGHTCYAN;
            circleProgressParam2.backgroundColor   = LCD_COLOR_DARKGRAY;
            circleProgressParam2.maskColor         = LCD_COLOR_BACKGROUND_ITEM;
            circleProgressParam2.currentValue      = 50;
            circleProgressParam2.radius            = 42;
            circleProgressParam2.dispVal           = TRUE;

            circleProgressParam3.color             = LCD_COLOR_LIGHTRED;
            circleProgressParam3.backgroundColor   = LCD_COLOR_DARKGRAY;
            circleProgressParam3.maskColor         = LCD_COLOR_BACKGROUND_ITEM;
            circleProgressParam3.currentValue      = 75;
            circleProgressParam3.radius            = 50;
            circleProgressParam3.dispVal           = TRUE;

            circleProgressParam4.color             = LCD_COLOR_LIGHTGREEN;
            circleProgressParam4.backgroundColor   = LCD_COLOR_DARKGRAY;
            circleProgressParam4.maskColor         = LCD_COLOR_BACKGROUND_ITEM;
            circleProgressParam4.currentValue      = 100;
            circleProgressParam4.radius            = 62;
            circleProgressParam4.dispVal           = TRUE;
         }

         //------
         //--- Initialisation
         //------
         WIDGET_Init(_numberInputWidget, &numberInputParam);
         WIDGET_Init(_circleProgressWidget, &circleProgressParam);
         WIDGET_Init(_circleProgressWidget2, &circleProgressParam2);
         WIDGET_Init(_circleProgressWidget3, &circleProgressParam3);
         WIDGET_Init(_circleProgressWidget4, &circleProgressParam4);

         s->state = DURING_SCREEN_STATE;
         break;
      }

      case DURING_SCREEN_STATE:
      {
         //--- Affichage number input
         LCD_SetFont(&LCD_FONT_16);
         LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
         LCD_SetBackColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_DisplayStringAt(20, 60, (STRING)"Number input\0", LEFT_MODE);
         WIDGET_Display(_numberInputWidget);

         //--- Affichage circle progress
         LCD_SetFont(&LCD_FONT_16);
         LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
         LCD_SetBackColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_DisplayStringAt(20, 100, (STRING)"Circle progress\0", LEFT_MODE);
         WIDGET_Display(_circleProgressWidget);
         WIDGET_Display(_circleProgressWidget2);
         WIDGET_Display(_circleProgressWidget3);
         WIDGET_Display(_circleProgressWidget4);

         if (xTaskGetTickCount() - _tick >= 40)
         {
            UPDATE_CIRCLE_PROGRESS(0);
            UPDATE_NUMBER_INPUT(0);

            UPDATE_WIDGET(0);
            _tick = xTaskGetTickCount();
         }
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
   s_WIDGET_PROGRESS_BAR progressBarParam;
   s_WIDGET_PROGRESS_BAR progressBarParam2;
   s_WIDGET_PROGRESS_BAR progressBarParam3;
   s_WIDGET_PROGRESS_BAR progressBarParam4;
   s_WIDGET_PROGRESS_BAR progressBarParam5;
   s_SCREEN *s = (s_SCREEN *)p;

   static UINT8   _idx  = 0;

   switch (s->state)
   {
      case BEFORE_SCREEN_STATE:
      {
         //--- Corps
         DISP_ShowBodyZone();

         if (s->initWidget == TRUE)
         {
            s->initWidget = FALSE;

            //------
            //--- Allocation
            //------
            _progressBarWidget = WIDGET_Alloc(WIDGET_PROGRESS_BAR_TYPE, WIDGET_DISPLAY_ONLY_MODE);
            _progressBarWidget2 = WIDGET_Alloc(WIDGET_PROGRESS_BAR_TYPE, WIDGET_DISPLAY_ONLY_MODE);
            _progressBarWidget3 = WIDGET_Alloc(WIDGET_PROGRESS_BAR_TYPE, WIDGET_DISPLAY_ONLY_MODE);
            _progressBarWidget4 = WIDGET_Alloc(WIDGET_PROGRESS_BAR_TYPE, WIDGET_DISPLAY_ONLY_MODE);
            _progressBarWidget5 = WIDGET_Alloc(WIDGET_PROGRESS_BAR_TYPE, WIDGET_DISPLAY_ONLY_MODE);

            //------
            //--- Positions
            //------
            _progressBarWidget->posX               = 190;
            _progressBarWidget->posY               = 104;

            _progressBarWidget2->posX              = 260;
            _progressBarWidget2->posY              = 123;

            _progressBarWidget3->posX              = 330;
            _progressBarWidget3->posY              = 55;

            _progressBarWidget4->posX              = 390;
            _progressBarWidget4->posY              = 79;

            _progressBarWidget5->posX              = 440;
            _progressBarWidget5->posY              = 108;

            //------
            //--- Configuration
            //------
            progressBarParam.height                = 90;
            progressBarParam.width                 = 22;
            progressBarParam.backgroundColor       = LCD_COLOR_BACKGROUND_ITEM;
            progressBarParam.color                 = LCD_COLOR_WHITE;
            progressBarParam.baseColor             = LCD_COLOR_BACKGROUND_PAGE;
            progressBarParam.currentValue          = 0.2;
            progressBarParam.outline               = TRUE;
            progressBarParam.cursorValue           = TRUE;

            progressBarParam2.height               = 75;
            progressBarParam2.width                = 18;
            progressBarParam2.backgroundColor      = LCD_COLOR_BACKGROUND_ITEM;
            progressBarParam2.color                = LCD_COLOR_LIGHTCYAN;
            progressBarParam2.baseColor            = LCD_COLOR_BACKGROUND_PAGE;
            progressBarParam2.currentValue         = 1.0;
            progressBarParam2.outline              = TRUE;
            progressBarParam2.cursorValue          = TRUE;

            progressBarParam3.height               = 145;
            progressBarParam3.width                = 16;
            progressBarParam3.backgroundColor      = LCD_COLOR_BACKGROUND_ITEM;
            progressBarParam3.color                = LCD_COLOR_LIGHTYELLOW;
            progressBarParam3.baseColor            = LCD_COLOR_BACKGROUND_PAGE;
            progressBarParam3.currentValue         = 0.0;
            progressBarParam3.outline              = TRUE;
            progressBarParam3.cursorValue          = TRUE;

            progressBarParam4.height               = 125;
            progressBarParam4.width                = 12;
            progressBarParam4.backgroundColor      = LCD_COLOR_BACKGROUND_ITEM;
            progressBarParam4.color                = LCD_COLOR_LIGHTRED;
            progressBarParam4.baseColor            = LCD_COLOR_BACKGROUND_PAGE;
            progressBarParam4.currentValue         = 0.5;
            progressBarParam4.outline              = TRUE;
            progressBarParam4.cursorValue          = TRUE;

            progressBarParam5.height               = 100;
            progressBarParam5.width                = 8;
            progressBarParam5.backgroundColor      = LCD_COLOR_BACKGROUND_ITEM;
            progressBarParam5.color                = LCD_COLOR_LIGHTGREEN;
            progressBarParam5.baseColor            = LCD_COLOR_BACKGROUND_PAGE;
            progressBarParam5.currentValue         = 0.8;
            progressBarParam5.outline              = TRUE;
            progressBarParam5.cursorValue          = TRUE;
         }

         //--- Initialisation
         WIDGET_Init(_progressBarWidget, &progressBarParam);
         WIDGET_Init(_progressBarWidget2, &progressBarParam2);
         WIDGET_Init(_progressBarWidget3, &progressBarParam3);
         WIDGET_Init(_progressBarWidget4, &progressBarParam4);
         WIDGET_Init(_progressBarWidget5, &progressBarParam5);

         s->state = DURING_SCREEN_STATE;
         break;
      }

      case DURING_SCREEN_STATE:
      {
         //--- Affichage number input
         LCD_SetFont(&LCD_FONT_16);
         LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
         LCD_SetBackColor(LCD_COLOR_BACKGROUND_ITEM);
         LCD_DisplayStringAt(20, 75, (STRING)"Vertical progress bar\0", LEFT_MODE);

         WIDGET_Display(_progressBarWidget);
         WIDGET_Display(_progressBarWidget2);
         WIDGET_Display(_progressBarWidget3);
         WIDGET_Display(_progressBarWidget4);
         WIDGET_Display(_progressBarWidget5);

         UPDATE_PROGRESS_BAR(_idx);
         UPDATE_WIDGET(_idx);
         _idx++;
         _idx %= 5;
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
   s_WIDGET_BUTTON buttonParam;
   s_WIDGET_BUTTON buttonParamClear;
   s_WIDGET_BUTTON buttonParamEnter;
   s_WIDGET_TEXT_INPUT textInputParam;

   switch (s->state)
   {
      case BEFORE_SCREEN_STATE:
      {
         //--- Corps
         DISP_ShowBodyZone();

         if (s->initWidget == TRUE)
         {
            s->initWidget = FALSE;

            //------
            //--- Allocation
            //------
            _ButtonWidget        = WIDGET_Alloc(WIDGET_BUTTON_TYPE, WIDGET_FULL_CONTROL_MODE);
            _ButtonWidget2       = WIDGET_Alloc(WIDGET_BUTTON_TYPE, WIDGET_FULL_CONTROL_MODE);
            _ButtonWidget3       = WIDGET_Alloc(WIDGET_BUTTON_TYPE, WIDGET_FULL_CONTROL_MODE);
            _ButtonWidget4       = WIDGET_Alloc(WIDGET_BUTTON_TYPE, WIDGET_FULL_CONTROL_MODE);
            _ButtonWidget5       = WIDGET_Alloc(WIDGET_BUTTON_TYPE, WIDGET_FULL_CONTROL_MODE);
            _ButtonWidget6       = WIDGET_Alloc(WIDGET_BUTTON_TYPE, WIDGET_FULL_CONTROL_MODE);
            _ButtonWidget7       = WIDGET_Alloc(WIDGET_BUTTON_TYPE, WIDGET_FULL_CONTROL_MODE);
            _ButtonWidget8       = WIDGET_Alloc(WIDGET_BUTTON_TYPE, WIDGET_FULL_CONTROL_MODE);
            _ButtonWidget9       = WIDGET_Alloc(WIDGET_BUTTON_TYPE, WIDGET_FULL_CONTROL_MODE);
            _ButtonWidget0       = WIDGET_Alloc(WIDGET_BUTTON_TYPE, WIDGET_FULL_CONTROL_MODE);
            _ButtonWidgetClear   = WIDGET_Alloc(WIDGET_BUTTON_TYPE, WIDGET_FULL_CONTROL_MODE);
            _ButtonWidgetEnter   = WIDGET_Alloc(WIDGET_BUTTON_TYPE, WIDGET_FULL_CONTROL_MODE);
            _TextInputWidget     = WIDGET_Alloc(WIDGET_TEXT_INPUT_TYPE, WIDGET_FULL_CONTROL_MODE);

            //------
            //--- Positions
            //------
            _ButtonWidget->posX  = _ButtonWidget4->posX = _ButtonWidget7->posX   = 195 + 40;
            _ButtonWidget2->posX = _ButtonWidget5->posX = _ButtonWidget8->posX   = 280 + 30;
            _ButtonWidget3->posX = _ButtonWidget6->posX = _ButtonWidget9->posX   = 365 + 20;

            _ButtonWidget->posY  = _ButtonWidget2->posY = _ButtonWidget3->posY   = 54;
            _ButtonWidget4->posY = _ButtonWidget5->posY = _ButtonWidget6->posY   = 96;
            _ButtonWidget7->posY = _ButtonWidget8->posY = _ButtonWidget9->posY   = 138;

            _ButtonWidget0->posX       = 280 + 30;
            _ButtonWidget0->posY       = 180;

            _ButtonWidgetClear->posX   = 195 + 40;
            _ButtonWidgetClear->posY   = 180;

            _ButtonWidgetEnter->posX   = 365 + 20;
            _ButtonWidgetEnter->posY   = 180;

            _TextInputWidget->posX     = 20;
            _TextInputWidget->posY     = 54;

            //------
            //--- Configuration
            //------
            buttonParam.color                = LCD_COLOR_BACKGROUND_PAGE;
            buttonParam.outlineColor         = LCD_COLOR_LIGHTCYAN;
            buttonParam.height               = 37;
            buttonParam.width                = 70;
            buttonParam.txtColor             = LCD_COLOR_LIGHTCYAN;
            STRING_COPY(buttonParam.str, "0\0");

            buttonParamClear.color           = LCD_COLOR_LIGHTRED;
            buttonParamClear.outlineColor    = LCD_COLOR_LIGHTRED;
            buttonParamClear.height          = 37;
            buttonParamClear.width           = 70;
            buttonParamClear.txtColor        = LCD_COLOR_WHITE;
            STRING_COPY(buttonParamClear.str, "CLEAR\0");

            buttonParamEnter.color           = LCD_COLOR_LIGHTGREEN;
            buttonParamEnter.outlineColor    = LCD_COLOR_LIGHTGREEN;
            buttonParamEnter.height          = 37;
            buttonParamEnter.width           = 70;
            buttonParamEnter.txtColor        = LCD_COLOR_WHITE;
            STRING_COPY(buttonParamEnter.str, "ENTER\0");

            textInputParam.color             = LCD_COLOR_LIGHTCYAN;
            textInputParam.backColor         = LCD_COLOR_LIGHTGRAY;
            textInputParam.txtColor          = LCD_COLOR_BLACK;
            textInputParam.width             = 180;
            STRING_COPY(textInputParam.str, "06632169\0");
         }

         WIDGET_Init(_ButtonWidget0, &buttonParam);
         buttonParam.str[0]++;
         WIDGET_Init(_ButtonWidget, &buttonParam);
         buttonParam.str[0]++;
         WIDGET_Init(_ButtonWidget2, &buttonParam);
         buttonParam.str[0]++;
         WIDGET_Init(_ButtonWidget3, &buttonParam);
         buttonParam.str[0]++;
         WIDGET_Init(_ButtonWidget4, &buttonParam);
         buttonParam.str[0]++;
         WIDGET_Init(_ButtonWidget5, &buttonParam);
         buttonParam.str[0]++;
         WIDGET_Init(_ButtonWidget6, &buttonParam);
         buttonParam.str[0]++;
         WIDGET_Init(_ButtonWidget7, &buttonParam);
         buttonParam.str[0]++;
         WIDGET_Init(_ButtonWidget8, &buttonParam);
         buttonParam.str[0]++;

         WIDGET_Init(_ButtonWidget9, &buttonParam);
         WIDGET_Init(_ButtonWidgetClear, &buttonParamClear);
         WIDGET_Init(_ButtonWidgetEnter, &buttonParamEnter);
         WIDGET_Init(_TextInputWidget, &textInputParam);

         s->state = DURING_SCREEN_STATE;
         break;
      }

      case DURING_SCREEN_STATE:
      {
         WIDGET_Display(_ButtonWidget);
         WIDGET_Display(_ButtonWidget2);
         WIDGET_Display(_ButtonWidget3);
         WIDGET_Display(_ButtonWidget4);
         WIDGET_Display(_ButtonWidget5);
         WIDGET_Display(_ButtonWidget6);
         WIDGET_Display(_ButtonWidget7);
         WIDGET_Display(_ButtonWidget8);
         WIDGET_Display(_ButtonWidget9);
         WIDGET_Display(_ButtonWidget0);
         WIDGET_Display(_ButtonWidgetClear);
         WIDGET_Display(_ButtonWidgetEnter);
         _TextInputWidget->isUpToDate = FALSE;
         WIDGET_Display(_TextInputWidget);

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
// FONCTION    : SCREEN_UpdateSlideMenu
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
void SCREEN_UpdateSlideMenu (BOOL state)
{
   _curScr.slide->state = state;

   if (state == (BOOL)OPEN)
   {
      _updateSlideMenu = TRUE;
   }
   else
   {
      SCREEN_RefreshCurrent();
   }
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
         s->slide = &_slide[s->idScr - 1];
         s->initWidget = TRUE;

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

   if (_updateHeader == TRUE)
      DISP_ShowHeader(s->header);

   if (_updateFooter == TRUE)
      DISP_ShowFooter(s->footer);

   if (s->slide->state == FALSE)
   {
      //--- Fonction d'affichage
      dispDone = s->disp(s);
   }

   if (_updateSlideMenu == TRUE)
      DISP_ShowSlideMenu(s->slide);

   _updateHeader     = FALSE;
   _updateFooter     = FALSE;
   _updateSlideMenu  = FALSE;

   return dispDone;
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_RefreshCurrent
//
// DESCRIPTION : Rafraichissement de l'ecran courant
//-----------------------------------------------------------------------------
void SCREEN_RefreshCurrent (void)
{
   _curScr.state = BEFORE_SCREEN_STATE;

   _updateFooter = TRUE;
   _updateHeader = TRUE;
   _updateSlideMenu = TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_Initialize
//
// DESCRIPTION : Screen initialization
//-----------------------------------------------------------------------------
void SCREEN_Initialize (void)
{
   //--- Initialise l'ecran
   DISP_Initialize();

   //--- Initialise les widgets
   WIDGET_GlobalInit();

   //--- Load first screen
   SCREEN_LoadNext(&_curScr, (STRING)"DispScreen4");
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
   }
}

#warning: debug
void UPDATE_WIDGET (UINT8 idx)
{
   _numberInputWidget->isUpToDate = FALSE;
   _circleProgressWidget->isUpToDate = FALSE;
   _circleProgressWidget2->isUpToDate = FALSE;
   _circleProgressWidget3->isUpToDate = FALSE;
   _circleProgressWidget4->isUpToDate = FALSE;

   _progressBarWidget->isUpToDate   = FALSE;
   _progressBarWidget2->isUpToDate  = FALSE;
   _progressBarWidget3->isUpToDate  = FALSE;
   _progressBarWidget4->isUpToDate  = FALSE;
   _progressBarWidget5->isUpToDate  = FALSE;
}
