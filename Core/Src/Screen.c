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

//---------- Functions ----------

//=============================================================================
//--- DEFINITIONS
//=============================================================================

//-----------------------------------------------------------------------------
// FONCTION    : SCREEN_TaskRun
//
// DESCRIPTION : Screen task run
//-----------------------------------------------------------------------------
void SCREEN_TaskRun (void *argument)
{
   BOOL firstTime    = TRUE;
   BOOL updateScreen = FALSE;
   UINT16 cptUpdate  = 0;
   STRING_TAB txt;
   osStatus_t eventStatus;

   //--- Remove compiler warning about unused parameter.
   (void)argument;

   for ( ;; )
   {
      if (firstTime == TRUE)
      {
         LCD_Clear(LCD_COLOR_WHITE);
         LCD_SetFont(&LCD_DEFAULT_FONT);
         LCD_SetBackColor(LCD_COLOR_WHITE);
         LCD_SetTextColor(LCD_COLOR_DARKBLUE);

         LCD_DisplayStringAt(0, 1, (UINT8 *)"Hello world ! :-)", CENTER_MODE);

         firstTime = FALSE;
      }
      else
      {
         //--- Wait until something arrives in the queue
         eventStatus = osMessageQueueGet(SCREEN_Event, &updateScreen, NULL, portMAX_DELAY);

         if (eventStatus == osOK)
         {
            if (updateScreen == TRUE)
            {
               LCD_SetBackColor(LCD_COLOR_WHITE);
               LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);
               LCD_SetFont(&LCD_FONT_16);

               STRING_FORMAT(txt, "Button pressed : %u", (UINT16)cptUpdate);
               LCD_DisplayStringAt(20, 124, (UINT8 *)txt, LEFT_MODE);

               cptUpdate++;
            }
         }
      }
   }
}
