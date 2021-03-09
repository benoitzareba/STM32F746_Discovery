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

//-----------------------------------------------------------------------------
// Structures and types
//-----------------------------------------------------------------------------
typedef BOOL (* SCREEN_DISP_FUNC)      (void *);
typedef BOOL (* SCREEN_UPDATE_FUNC)    (void *, void *);

typedef struct //--- s_HEADER
{
   STRING_TAB     title;
   STRING_TAB     description;
   UINT32         color;
} s_HEADER;

typedef struct //--- s_POSITION
{
   UINT16               x;
   UINT16               y;
   UINT16               width;
   UINT16               height;
} s_COORDINATES;

typedef struct //--- s_BUTTON
{
   UINT8                position;
   BOOL                 selected;
   UINT32               color;
   STRING_TAB           txt;
   STRING_TAB           function;
   s_COORDINATES        coordinates;
} s_BUTTON;

typedef struct //--- s_FOOTER
{
   UINT8                nbButton;
   UINT32               color;
   s_BUTTON             button[NB_BUTTON_MAX];
} s_FOOTER;

typedef struct //--- s_SCREEN
{
   UINT8                idScr;
   UINT8                oldScr;
   UINT8                state;
   SCREEN_DISP_FUNC     disp;
   SCREEN_UPDATE_FUNC   update;
   UINT32               color;
   s_HEADER             *header;
   s_FOOTER             *footer;
   //s_POPUP             *popup;
} s_SCREEN;

//-----------------------------------------------------------------------------
// External variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------
extern osThreadId_t SCREEN_TaskHandle;
extern const osThreadAttr_t SCREEN_TaskAttributes;

//---------- Functions ----------
BOOL SCREEN_DispScr1    (void *p);
BOOL SCREEN_UpdateScr1  (void *p, void *e);

BOOL SCREEN_DispScr2    (void *p);
BOOL SCREEN_UpdateScr2  (void *p, void *e);

BOOL SCREEN_DispScr3    (void *p);
BOOL SCREEN_UpdateScr3  (void *p, void *e);

BOOL SCREEN_DispScr4    (void *p);
BOOL SCREEN_UpdateScr4  (void *p, void *e);

BOOL SCREEN_LoadNext    (s_SCREEN *s, UINT8 idScr);
BOOL SCREEN_Display     (s_SCREEN* s);

void SCREEN_Initialize  (void);
void SCREEN_TaskRun     (void *argument);

#endif
