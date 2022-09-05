//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// HEADER      :  TextInputWidget.h
//
//=============================================================================
#ifndef _TEXT_INPUT_WIDGET_H_
#define _TEXT_INPUT_WIDGET_H_

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
#define NB_CHAR_MAX_TEXT_INPUT               32
#define TEXT_INPUT_HEIGHT                    24

//-----------------------------------------------------------------------------
// Structures and types
//-----------------------------------------------------------------------------
typedef struct
{
   UINT32   color;
   UINT32   backColor;
   UINT32   txtColor;
   UINT16   width;
   BOOL     used;
   CHAR8    str[NB_CHAR_MAX_TEXT_INPUT];
} s_WIDGET_TEXT_INPUT;

//-----------------------------------------------------------------------------
// External variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------

//---------- Functions ----------
BOOL WIDGET_TextInputGetDefaultFuncs (s_WIDGET *pWdgt);

#warning : debug
void UPDATE_TEXT_INPUT_WIDGET (UINT8 i);
#endif
