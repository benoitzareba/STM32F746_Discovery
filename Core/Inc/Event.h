//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// HEADER      :  Event.h
//
//=============================================================================
#ifndef _EVENT_H_
#define _EVENT_H_

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "TypeDefs.h"
#include "cmsis_os.h"

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
extern osMessageQueueId_t BUTTON_Event;
extern osMessageQueueId_t SCREEN_Event;
extern osMessageQueueId_t CHANGE_SCREEN_Event;

//---------- Functions ----------
void EVENT_Initialize (void);

#endif
