//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// HEADER      :  SlideBar.h
//
//=============================================================================
#ifndef _SLIDE_BAR_H_
#define _SLIDE_BAR_H_

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
   UINT16   length;
   FLOAT32  currentValue;
   BOOL     used;
} s_WIDGET_SLIDE_BAR;

//-----------------------------------------------------------------------------
// External variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------

//---------- Functions ----------
BOOL WIDGET_SlideGetDefaultFuncs (s_WIDGET *pWdgt);

#warning : debug
void UPDATE_SLIDE_BAR (FLOAT32 valSlideBar);
#endif