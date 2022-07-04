//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// HEADER      :  DispWidgets.h
//
//=============================================================================
#ifndef _DISP_WIDGETS_H_
#define _DISP_WIDGETS_H_

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include <SlideBar.h>
#include "TypeDefs.h"
#include "ToggleSwitch.h"
#include "RadioButton.h"
#include "CheckBox.h"
#include "NumberInput.h"
#include "CircleProgress.h"
#include "ProgressBar.h"
#include "ButtonWidget.h"
#include "TextInput.h"

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
void DISP_WDGT_ShowSlide            (UINT16 posX, UINT16 posY, s_WIDGET_SLIDE_BAR *pSlide, BOOL isFirstTime);
void DISP_WDGT_ShowToggleSwitch     (UINT16 posX, UINT16 posY, s_WIDGET_TOGGLE_SWITCH *pToggleSwitch, BOOL isFirstTime);
void DISP_WDGT_ShowRadioButton      (UINT16 posX, UINT16 posY, s_WIDGET_RADIOBUTTON *pRadioButton, BOOL isFirstTime);
void DISP_WDGT_ShowCheckBox         (UINT16 posX, UINT16 posY, s_WIDGET_CHECKBOX *pCheckBox, BOOL isFirstTime);
void DISP_WDGT_ShowNumberInput      (UINT16 posX, UINT16 posY, s_WIDGET_NUMBER_INPUT *pCheckBox, BOOL isFirstTime);
void DISP_WDGT_ShowCircleProgress   (UINT16 posX, UINT16 posY, s_WIDGET_CIRCLE_PROGRESS *pNumberInput, BOOL isFirstTime);
void DISP_WDGT_ShowProgressBar      (UINT16 posX, UINT16 posY, s_WIDGET_PROGRESS_BAR *pProgressBar, BOOL isFirstTime);
void DISP_WDGT_ShowButton           (UINT16 posX, UINT16 posY, s_WIDGET_BUTTON *pButton, BOOL isFirstTime);
void DISP_WDGT_ShowTextInput        (UINT16 posX, UINT16 posY, s_WIDGET_TEXT_INPUT *pTextInput, BOOL isFirstTime);
#endif
