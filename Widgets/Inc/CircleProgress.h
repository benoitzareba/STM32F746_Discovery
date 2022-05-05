//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// HEADER      :  CircleProgress.h
//
//=============================================================================
#ifndef _CIRCLE_PROGRESS_H_
#define _CIRCLE_PROGRESS_H_

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
   UINT32   maskColor;
   UINT8    radius;
   UINT8    currentValue;
   FLOAT32  oldValue;
   BOOL     used;
} s_WIDGET_CIRCLE_PROGRESS;

//-----------------------------------------------------------------------------
// External variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------

//---------- Functions ----------
BOOL WIDGET_CircleProgressGetDefaultFuncs (s_WIDGET *pWdgt);

#warning : debug
void UPDATE_CIRCLE_PROGRESS (UINT16 valueProgress);
#endif
