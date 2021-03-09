//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  Tasks.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "Tasks.h"
#include "Screen.h"
#include "Button.h"
#include "LED.h"
#include "Touchscreen.h"

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
// FONCTION    : TASK_Initialize
//
// DESCRIPTION : Task initialization
//-----------------------------------------------------------------------------
void TASK_Initialize (void)
{
   //--- Screen task
   SCREEN_Initialize();
   SCREEN_TaskHandle = osThreadNew(SCREEN_TaskRun, NULL, &SCREEN_TaskAttributes);

   //--- Touch screen task
   TOUCH_Initialize(LCD_GetXSize(), LCD_GetYSize());
   TOUCH_TaskHandle = osThreadNew(TOUCH_TaskRun, NULL, &TOUCH_TaskAttributes);

   //--- Push buttons task
   BUTTON_TaskHandle = osThreadNew(BUTTON_TaskRun, NULL, &BUTTON_TaskAttributes);

   //--- Leds task
   LED_TaskHandle    = osThreadNew(LED_TaskRun,    NULL, &LED_TaskAttributes);
}
