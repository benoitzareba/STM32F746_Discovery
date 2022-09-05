//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// HEADER      :  NumberInput.h
//
//=============================================================================
#ifndef _NUMBER_INPUT_H_
#define _NUMBER_INPUT_H_

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
#define NUMBER_INPUT_BUTTON_PERCENT    0.25

//-----------------------------------------------------------------------------
// Structures and types
//-----------------------------------------------------------------------------
typedef struct
{
   UINT16   width;
   UINT16   height;
   UINT32   color;
   UINT32   backgroundColor;
   INT16    currentValue;
   BOOL     used;
} s_WIDGET_NUMBER_INPUT;

enum
{
   NUMBER_INPUT_MINUS_ONE = 0,
   NUMBER_INPUT_PLUS_ONE
};

//-----------------------------------------------------------------------------
// External variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------

//---------- Functions ----------
BOOL WIDGET_NumberInputGetDefaultFuncs (s_WIDGET *pWdgt);

#warning : debug
void UPDATE_NUMBER_INPUT (INT16 valInput);
#endif
