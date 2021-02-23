//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  Button.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "Button.h"
#include "Board.h"
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
osThreadId_t BUTTON_TaskHandle;
const osThreadAttr_t BUTTON_TaskAttributes   = {.name = "buttonTask",      .priority = (osPriority_t) osPriorityNormal,    .stack_size = 128 * 4};

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
// FONCTION    : BUTTON_TaskRun
//
// DESCRIPTION : Button task run
//-----------------------------------------------------------------------------
void BUTTON_TaskRun (void *argument)
{
   BOOL buttonState     = FALSE;
   BOOL buttonOldState  = FALSE;
   BOOL updateScreen    = TRUE;

   //--- Remove compiler warning about unused parameter.
   (void)argument;

   for ( ;; )
   {
      //--- Read button input state
      buttonState = HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_11);

      if (buttonOldState != buttonState)
      {
         buttonOldState = buttonState;

         //--- Send value to queue
         osMessageQueuePut(BUTTON_Event, &buttonState, 0, 0);

         if (buttonState == TRUE)
            osMessageQueuePut(SCREEN_Event, &updateScreen, 0, 0);
      }
   }
}
