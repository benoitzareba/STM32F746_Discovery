//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// HEADER      :  Screen.h
//
//=============================================================================
#ifndef _SCREEN_H_
#define _SCREEN_H_

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "TypeDefs.h"
#include "cmsis_os.h"
#include "Popup.h"
#include "LCD.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------
enum
{
   NO_SCREEN = 0,

   SCREEN_1,
   SCREEN_2,
   SCREEN_3,
   SCREEN_4,

   NB_SCREEN
};

enum
{
   NO_SCREEN_STATE = 0,
   BEFORE_SCREEN_STATE,
   DURING_SCREEN_STATE,
   AFTER_SCREEN_STATE,

   MAX_SCREEN_STATES
};

#define NB_BUTTON_MAX                  4
#define NB_ICON_MAX                    4
#define NB_ITEM_MAX                    8

#define HASH_DISPSCREEN1               184555610
#define HASH_DISPSCREEN2               184555621
#define HASH_DISPSCREEN3               184555632
#define HASH_DISPSCREEN4               184555643

//-----------------------------------------------------------------------------
// Structures and types
//-----------------------------------------------------------------------------

typedef struct //--- s_HEADER
{
   STRING_TAB           title;
   STRING_TAB           description;
   UINT32               color;
} s_HEADER;

typedef struct //--- s_BUTTON
{
   UINT8                position;
   BOOL                 selected;
   UINT32               color;
   STRING_TAB           txt;
   STRING_TAB           function;
   s_RECT               rect;
} s_BUTTON;

typedef struct //--- s_FOOTER
{
   UINT8                nbButton;
   UINT32               color;
   s_BUTTON             button[NB_BUTTON_MAX];
} s_FOOTER;

typedef struct //--- s_ICON
{
   UINT32               color;
   STRING_TAB           label;
} s_ICON;

typedef struct //--- s_SLIDE
{
   BOOL                 state;
   UINT32               color;
   UINT8                nbIcon;
   s_ICON               icon[NB_ICON_MAX];
} s_SLIDE;

typedef struct //--- s_SCREEN
{
   UINT8                idScr;
   UINT8                oldScr;
   UINT8                state;
   SCREEN_DISP_FUNC     disp;
   SCREEN_UPDATE_FUNC   update;
   UINT32               color;
   s_HEADER*            header;
   s_FOOTER*            footer;
   s_POPUP*             popup;
   s_SLIDE*             slide;
   BOOL                 initWidget;
} s_SCREEN;

//-----------------------------------------------------------------------------
// External variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------
extern osThreadId_t SCREEN_TaskHandle;
extern const osThreadAttr_t SCREEN_TaskAttributes;

//---------- Functions ----------
BOOL SCREEN_DispScr1          (void *p);
BOOL SCREEN_UpdateScr1        (void *p, void *e);

BOOL SCREEN_DispScr2          (void *p);
BOOL SCREEN_UpdateScr2        (void *p, void *e);

BOOL SCREEN_DispScr3          (void *p);
BOOL SCREEN_UpdateScr3        (void *p, void *e);

BOOL SCREEN_DispScr4          (void *p);
BOOL SCREEN_UpdateScr4        (void *p, void *e);

BOOL SCREEN_DispPopup         (void *p);
BOOL SCREEN_UpdatePopup       (void *p, void *e);

void SCREEN_ShowPopup         (s_SCREEN *s, s_POPUP *popup);
void SCREEN_ClearPopup        (s_SCREEN *s);

void SCREEN_UpdateSlideMenu   (BOOL state);

BOOL SCREEN_LoadNext          (s_SCREEN *s, CHAR8* menuName);
BOOL SCREEN_LoadAndUpdate     (CHAR8* menuName, s_SCREEN* s);

BOOL SCREEN_Update            (s_SCREEN* s, void* e);
BOOL SCREEN_Display           (s_SCREEN* s);

void SCREEN_RefreshCurrent    (void);

void SCREEN_Initialize        (void);
void SCREEN_TaskRun           (void *argument);

#warning: debug
void UPDATE_WIDGET (UINT8);
#endif
