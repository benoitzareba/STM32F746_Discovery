//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// HEADER      :  ProgressBar.h
//
//=============================================================================
#ifndef _PROGRESS_BAR_H_
#define _PROGRESS_BAR_H_

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
   UINT32   baseColor;
   UINT32   backgroundColor;
   UINT16   width;
   UINT16   height;
   FLOAT32  currentValue;
   FLOAT32  oldValue;
   BOOL     used;
   BOOL     outline;
   BOOL     cursorValue;
} s_WIDGET_PROGRESS_BAR;

//-----------------------------------------------------------------------------
// External variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------

//---------- Functions ----------
BOOL WIDGET_ProgressBarGetDefaultFuncs (s_WIDGET *pWdgt);

#warning : debug
void UPDATE_PROGRESS_BAR (UINT8 i);
#endif
