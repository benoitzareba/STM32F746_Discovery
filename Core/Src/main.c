//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  Main.c
// AUTHOR      :  Benoit ZAREBA
//
//-----------------------------------------------------------------------------
//
// HISTORIC    :
//
// 16/02/2021 - V1.0 : Initial revision
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included file
//-----------------------------------------------------------------------------
#include "Board.h"
#include "cmsis_os.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------
#define mainQUEUE_RECEIVE_TASK_PRIORITY      (tskIDLE_PRIORITY + 2)
#define mainQUEUE_SEND_TASK_PRIORITY         (tskIDLE_PRIORITY + 1)
#define mainQUEUE_SEND_FREQUENCY_MS          (200 / portTICK_PERIOD_MS)

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
static osMessageQueueId_t xQueueButtonState = NULL;

osThreadId_t buttonTaskHandle;
const osThreadAttr_t buttonTaskAttributes = {
  .name = "buttonTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

osThreadId_t ledTaskHandle;
const osThreadAttr_t ledTaskAttributes = {
  .name = "ledTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

//---------- Functions ----------
static void buttonTaskRun  (void *argument);
static void ledTaskRun     (void *argument);

//=============================================================================
//--- DEFINITIONS
//=============================================================================

//-----------------------------------------------------------------------------
// FONCTION    : buttonTaskRun
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static void buttonTaskRun (void *argument)
{
   BOOL buttonState = FALSE;
   BOOL buttonOldState = FALSE;

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
         osMessageQueuePut(xQueueButtonState, &buttonState, 0, 0U);
      }
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : ledTaskRun
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static void ledTaskRun (void *argument)
{
   BOOL  receivedButtonState;

   //--- Remove compiler warning about unused parameter.
   (void)argument;

   for ( ;; )
   {
      //--- Wait until something arrives in the queue
      osMessageQueueGet(xQueueButtonState, &receivedButtonState, NULL, portMAX_DELAY);

      //--- Drive output led pin
      HAL_GPIO_WritePin(GPIOI, GPIO_PIN_1, receivedButtonState);
   }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// FONCTION    : main
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int main (void)
{
   //--- Board configuration
   BOARD_ConfAll();

   //--- Initialize scheduler
   osKernelInitialize();

   //--- Create the queue
   xQueueButtonState = osMessageQueueNew(1, sizeof(BOOL), NULL);

   if (xQueueButtonState != NULL)
   {
      //--- Create the thread(s)
      buttonTaskHandle  = osThreadNew(buttonTaskRun,  NULL, &buttonTaskAttributes);
      ledTaskHandle     = osThreadNew(ledTaskRun,     NULL, &ledTaskAttributes);

      //--- Start scheduler
      osKernelStart();
   }

   for ( ;; );
}
