//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// HEADER      :  Widgets.h
//
//=============================================================================
#ifndef _WIDGETS_H
#define _WIDGETS_H

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "TypeDefs.h"
#include "Event.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------
#define MAX_WIDGETS              16
#define NB_ACTIVE_ZONE_MAX       2

typedef enum
{
   WIDGET_NO_TYPE = 0,
   WIDGET_SLIDE_TYPE,
   WIDGET_TOGGLE_SWITCH_TYPE,
   WIDGET_RADIO_BUTTON_TYPE,
   WIDGET_CHECKBOX_TYPE,
   WIDGET_NUMBER_INPUT_TYPE,
   WIDGET_CIRCLE_PROGRESS_TYPE,
   WIDGET_PROGRESS_BAR_TYPE,
   WIDGET_BUTTON_TYPE,
   WIDGET_TEXT_INPUT_TYPE,

   WIDGET_TYPE_NB
} e_WIDGET_TYPE;

typedef enum
{
   NO_WIDGET_STATE = 0,
   TO_CREATE_WIDGET_STATE,
   TO_INIT_WIDGET_STATE,
   TO_UPDATE_WIDGET_STATE,
   TO_DISP_WIDGET_STATE,
   TO_DELETE_WIDGET_STATE,
   TO_DISP_AND_UPDATE_WIDGET_STATE,
   WAIT_WIDGET_STATE,
} e_WIDGET_STATE;

typedef enum
{
   WIDGET_FULL_CONTROL_MODE = 0,
   WIDGET_DISPLAY_ONLY_MODE,
} e_WIDGET_MODE;

//-----------------------------------------------------------------------------
// Structures and types
//-----------------------------------------------------------------------------
struct WIDGET;

typedef void   *(*WIDGET_NEW_FUNC)     (void);
typedef BOOL   (*WIDGET_INIT_FUNC)     (struct WIDGET*, void*);
typedef BOOL   (*WIDGET_DISP_FUNC)     (struct WIDGET*);
typedef BOOL   (*WIDGET_UPDATE_FUNC)   (struct WIDGET*, void*);
typedef BOOL   (*WIDGET_DELETE_FUNC)   (struct WIDGET*);

typedef struct
{
   WIDGET_NEW_FUNC      new;
   WIDGET_INIT_FUNC     init;
   WIDGET_DISP_FUNC     disp;
   WIDGET_UPDATE_FUNC   update;
   WIDGET_DELETE_FUNC   delete;

} s_WIDGET_FUNC;

typedef struct
{
   UINT8    idFunc;
   s_RECT   coord;
} s_ZONE;

typedef struct
{
   UINT8 nbActiveZone;
   s_ZONE zone[NB_ACTIVE_ZONE_MAX];
} s_ACTIVE_ZONE;

typedef struct
{
   UINT16               posX;
   UINT16               posY;
   e_WIDGET_TYPE        type;
   s_WIDGET_FUNC        func;   e_WIDGET_STATE       state;
   e_WIDGET_MODE        mode;
   BOOL                 isUpToDate;
   BOOL                 isFirstTime;
   void*                param;
   s_ACTIVE_ZONE        activeZone;
} s_WIDGET;

typedef struct  //--- s_WIDGET_TAB
{
   UINT8                count;
   UINT8                firstFreeIdx;
   s_WIDGET             *pWdgt;
} s_WIDGET_TAB;

//-----------------------------------------------------------------------------
// External variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------

//---------- Functions ----------
void        WIDGET_GlobalInit             (void);
void*       WIDGET_Create                 (s_WIDGET *pWdgt);
BOOL        WIDGET_Init                   (s_WIDGET *pWdgt, void *ptr);
BOOL        WIDGET_Update                 (s_WIDGET *pWdgt, void *event);
BOOL        WIDGET_Display                (s_WIDGET *pWdgt);
BOOL        WIDGET_Delete                 (s_WIDGET *pWdgt);
s_WIDGET*   WIDGET_Alloc                  (e_WIDGET_TYPE type, e_WIDGET_MODE mode);
BOOL        WIDGET_GetTouchActiveZone     (UINT16 posX, UINT16 posY, s_WIDGET** pWdgt);
BOOL        WIDGET_GetTouchZoneFromWidget (UINT16 posX, UINT16 posY, s_WIDGET* pWdgt, s_ZONE **pZone);

#endif
