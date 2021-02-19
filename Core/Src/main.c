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
#include "LCD.h"
#include "cmsis_os.h"
#include <stdio.h>

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
static osMessageQueueId_t queueButtonState = NULL;
static osMessageQueueId_t queueScreenUpdate = NULL;

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

osThreadId_t screenTaskHandle;
const osThreadAttr_t screenTaskAttributes = {
   .name = "screenTask",
   .priority = (osPriority_t) osPriorityNormal,
   .stack_size = 128 * 4
};

//---------- Functions ----------
static void buttonTaskRun  (void *argument);
static void ledTaskRun     (void *argument);
static void screenTaskRun  (void *argument);

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
         osMessageQueuePut(queueButtonState, &buttonState, 0, 0);
         osMessageQueuePut(queueScreenUpdate, &updateScreen, 0, 0);
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
      osMessageQueueGet(queueButtonState, &receivedButtonState, NULL, portMAX_DELAY);

      //--- Drive output led pin
      HAL_GPIO_WritePin(GPIOI, GPIO_PIN_1, receivedButtonState);
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : screenTaskRun
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static void screenTaskRun (void *argument)
{
   BOOL firstTime = TRUE;
   BOOL updateScreen = FALSE;
   UINT16 cptUpdate = 0;
   STRING_TAB txt;

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
         osMessageQueueGet(queueScreenUpdate, &updateScreen, NULL, portMAX_DELAY);

         LCD_SetBackColor(LCD_COLOR_WHITE);
         LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);
         LCD_SetFont(&LCD_FONT_16);

         sprintf(txt, "Button pressed : %d", cptUpdate);
         LCD_DisplayStringAt(20, 124, (UINT8 *)txt, LEFT_MODE);

         cptUpdate++;
      }
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
   queueButtonState  = osMessageQueueNew(1, sizeof(BOOL), NULL);
   queueScreenUpdate = osMessageQueueNew(1, sizeof(BOOL), NULL);

   if (queueButtonState != NULL && queueScreenUpdate != NULL)
   {
      //--- Create the thread(s)
      buttonTaskHandle  = osThreadNew(buttonTaskRun,  NULL, &buttonTaskAttributes);
      ledTaskHandle     = osThreadNew(ledTaskRun,     NULL, &ledTaskAttributes);
      screenTaskHandle  = osThreadNew(screenTaskRun,  NULL, &screenTaskAttributes);

      //--- Start scheduler
      osKernelStart();
   }

   for ( ;; );
}
