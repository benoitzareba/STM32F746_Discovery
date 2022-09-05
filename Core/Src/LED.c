//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  LED.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "LED.h"
#include "Board.h"
#include "Event.h"
#include "Screen.h"
#include "SlideBar.h"
#include "ToggleSwitch.h"
#include "CheckBox.h"
#include "RadioButton.h"
#include "NumberInput.h"
#include "CircleProgress.h"

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
osThreadId_t         LED_TaskHandle;
const osThreadAttr_t LED_TaskAttributes = {.name = "ledTask", .priority = (osPriority_t) osPriorityNormal, .stack_size = 128 * 4};

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
// FONCTION    : LED_TaskRun
//
// DESCRIPTION : LED task run
//-----------------------------------------------------------------------------
void LED_TaskRun (void *argument)
{
   BOOL  receivedButtonState;
   osStatus_t eventStatus;
#if 0
   BOOL slideM = TRUE;
#endif

   //--- Remove compiler warning about unused parameter.
   (void)argument;

   for ( ;; )
   {
      //--- Wait until something arrives in the queue
      eventStatus = osMessageQueueGet(BUTTON_Event, &receivedButtonState, NULL, portMAX_DELAY);

      if (eventStatus == osOK)
      {
         //--- Drive output led pin
         HAL_GPIO_WritePin(GPIOI, GPIO_PIN_1, receivedButtonState);
#if 0
         if (receivedButtonState == TRUE)
         {
            SCREEN_UpdateSlideMenu(slideM);
            slideM = !slideM;
         }
#endif
      }
   }
}
