//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// HEADER      :  Display.h
//
//=============================================================================
#ifndef _DISPLAY_H_
#define _DISPLAY_H_

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "TypeDefs.h"
#include "Screen.h"

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

//---------- Functions ----------
void DISP_ShowHeader    (s_HEADER* h);
void DISP_ShowFooter    (s_FOOTER* f);
void DISP_ShowSlideMenu (s_HEADER* h);
void DISP_ShowPopup     (s_POPUP* p, UINT32 color);

#endif
