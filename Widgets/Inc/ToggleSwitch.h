//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// HEADER      :  ToggleSwitch.h
//
//=============================================================================
#ifndef _TOGGLE_SWITCH_H_
#define _TOGGLE_SWITCH_H_

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "TypeDefs.h"
#include "Widgets.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------
#define TOGGLE_SWITCH_RADIUS        10
#define TOGGLE_SWITCH_OFFSET        18
#define TOGGLE_SWITCH_WIDTH         TOGGLE_SWITCH_RADIUS + TOGGLE_SWITCH_OFFSET
#define TOGGLE_SWITCH_HEIGHT        TOGGLE_SWITCH_RADIUS * 2 + 1

//-----------------------------------------------------------------------------
// Structures and types
//-----------------------------------------------------------------------------
typedef struct
{
   UINT32   colorOn;
   UINT32   colorOff;
   BOOL     currentValue;
   BOOL     used;
} s_WIDGET_TOGGLE_SWITCH;

//-----------------------------------------------------------------------------
// External variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------

//---------- Functions ----------
BOOL WIDGET_ToggleSwitchGetDefaultFuncs (s_WIDGET *pWdgt);

#warning : debug
void UPDATE_TOGGLE_SWITCH (BOOL valToggle);
#endif
