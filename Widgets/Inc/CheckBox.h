//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// HEADER      :  CheckBox.h
//
//=============================================================================
#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_

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

//-----------------------------------------------------------------------------
// Structures and types
//-----------------------------------------------------------------------------
typedef struct
{
   UINT32   color;
   UINT32   backgroundColor;
   BOOL     currentValue;
   BOOL     used;
} s_WIDGET_CHECKBOX;

//-----------------------------------------------------------------------------
// External variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------

//---------- Functions ----------
BOOL WIDGET_CheckBoxGetDefaultFuncs (s_WIDGET *pWdgt);

#warning : debug
void UPDATE_CHECK_BOX (BOOL valCheckBox);
#endif