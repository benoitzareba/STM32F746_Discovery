//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  Event.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "Event.h"
#include "Screen.h"
#include "Widgets.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------
#define BUTTON_EVENT_BUFFER_SIZE             1
#define SCREEN_EVENT_BUFFER_SIZE             1
#define CHANGE_SCREEN_EVENT_BUFFER_SIZE      1
#define POPUP_EVENT_BUFFER_SIZE              1
#define WIDGET_EVENT_BUFFER_SIZE             1

#define BUTTON_EVENT_SIZE                    sizeof(BOOL)
#define SCREEN_EVENT_SIZE                    sizeof(s_SCREEN)
#define CHANGE_SCREEN_EVENT_SIZE             sizeof(STRING_TAB)
#define POPUP_EVENT_SIZE                     sizeof(UINT8)
#define WIDGET_EVENT_SIZE                    sizeof(s_WIDGET)

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
osMessageQueueId_t BUTTON_Event        = NULL;
osMessageQueueId_t SCREEN_Event        = NULL;
osMessageQueueId_t CHANGE_SCREEN_Event = NULL;
osMessageQueueId_t POPUP_Event         = NULL;
osMessageQueueId_t WIDGET_Event        = NULL;

//---------- Functions ----------

//=============================================================================
//--- DEFINITIONS
//=============================================================================

//-----------------------------------------------------------------------------
// FONCTION    : EVENT_Initialize
//
// DESCRIPTION : Event initialization
//-----------------------------------------------------------------------------
void EVENT_Initialize (void)
{
   //--- Button event
   BUTTON_Event = osMessageQueueNew(BUTTON_EVENT_BUFFER_SIZE, BUTTON_EVENT_SIZE, NULL);

   //--- Screen event
   SCREEN_Event = osMessageQueueNew(SCREEN_EVENT_BUFFER_SIZE, SCREEN_EVENT_SIZE, NULL);

   //--- Change screen event
   CHANGE_SCREEN_Event = osMessageQueueNew(CHANGE_SCREEN_EVENT_BUFFER_SIZE, CHANGE_SCREEN_EVENT_SIZE, NULL);

   //--- Popup event
   POPUP_Event = osMessageQueueNew(POPUP_EVENT_BUFFER_SIZE, POPUP_EVENT_SIZE, NULL);

   //--- Widget event
   WIDGET_Event = osMessageQueueNew(WIDGET_EVENT_BUFFER_SIZE, WIDGET_EVENT_SIZE, NULL);
}
