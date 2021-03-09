//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// HEADER      :  Touchscreen.h
//
//=============================================================================
#ifndef _TOUCHSCREEN_H_
#define _TOUCHSCREEN_H_

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "TypeDefs.h"
#include "stm32f7xx_hal.h"
#include "ft5336.h"
#include "cmsis_os.h"
#include "Screen.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------
#define TS_MAX_NB_TOUCH                 ((UINT32) FT5336_MAX_DETECTABLE_TOUCH)

#define TS_NO_IRQ_PENDING               ((UINT8) 0)
#define TS_IRQ_PENDING                  ((UINT8) 1)

#define TS_SWAP_NONE                    ((UINT8) 0x01)
#define TS_SWAP_X                       ((UINT8) 0x02)
#define TS_SWAP_Y                       ((UINT8) 0x04)
#define TS_SWAP_XY                      ((UINT8) 0x08)

typedef enum
{
   TS_OK                   = 0,
   TS_ERROR,
   TS_TIMEOUT,
   TS_DEVICE_NOT_FOUND
} TS_StatusTypeDef;

typedef enum
{
   GEST_ID_NO_GESTURE      = 0,  //--- Gesture not defined / recognized
   GEST_ID_MOVE_UP,              //--- Gesture Move Up
   GEST_ID_MOVE_RIGHT,           //--- Gesture Move Right
   GEST_ID_MOVE_DOWN,            //--- Gesture Move Down
   GEST_ID_MOVE_LEFT,            //--- Gesture Move Left
   GEST_ID_ZOOM_IN,              //--- Gesture Zoom In
   GEST_ID_ZOOM_OUT,             //--- Gesture Zoom Out
   GEST_ID_NB_MAX                //--- Max number of gesture id
} TS_GestureIdTypeDef;

typedef enum
{
  TOUCH_EVENT_NO_EVT       = 0,  //--- Touch Event : undetermined
  TOUCH_EVENT_PRESS_DOWN,        //--- Touch Event Press Down
  TOUCH_EVENT_LIFT_UP,           //--- Touch Event Lift Up
  TOUCH_EVENT_CONTACT,           //--- Touch Event Contact
  TOUCH_EVENT_NB_MAX             //--- max number of touch events kind

} TS_TouchEventTypeDef;

//-----------------------------------------------------------------------------
// Structures and types
//-----------------------------------------------------------------------------
typedef struct
{
  UINT8  touchDetected;                   //--- Total number of active touches detected at last scan
  UINT16 touchX[TS_MAX_NB_TOUCH];         //--- Touch X[0], X[1] coordinates on 12 bits
  UINT16 touchY[TS_MAX_NB_TOUCH];         //--- Touch Y[0], Y[1] coordinates on 12 bits
  UINT8  touchWeight[TS_MAX_NB_TOUCH];    //--- Touch_Weight[0], Touch_Weight[1] : weight property of touches
  UINT8  touchEventId[TS_MAX_NB_TOUCH];   //--- Touch_EventId[0], Touch_EventId[1] : take value of type @ref TS_TouchEventTypeDef
  UINT8  touchArea[TS_MAX_NB_TOUCH];      //--- Touch_Area[0], Touch_Area[1] : touch area of each touch
  UINT32 gestureId;                       //--- Type of gesture detected : take value of type @ref TS_GestureIdTypeDef
} TS_StateTypeDef;


//-----------------------------------------------------------------------------
// External variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------
extern osThreadId_t TOUCH_TaskHandle;
extern const osThreadAttr_t TOUCH_TaskAttributes;
//extern char * ts_event_string_tab[TOUCH_EVENT_NB_MAX];
//extern char * ts_gesture_id_string_tab[GEST_ID_NB_MAX];
extern I2C_HandleTypeDef hI2cTouchScreenHandler;

//---------- Functions ----------
UINT8 TOUCH_Initialize                 (UINT16 sizeX, UINT16 sizeY);
UINT8 TOUCH_ConfigInterrupt            (void);
UINT8 TOUCH_GetInterruptStatus         (void);
UINT8 TOUCH_GetState                   (TS_StateTypeDef *state);
UINT8 TOUCH_GetGestureId               (TS_StateTypeDef *state);
void  TOUCH_ClearInterrupt             (void);
UINT8 TOUCH_ResetTouchData             (TS_StateTypeDef *state);

void  TOUCH_IO_Init                    (void);
void  TOUCH_IO_Write                   (UINT8 Addr, UINT8 Reg, UINT8 Value);
UINT8 TOUCH_IO_Read                    (UINT8 Addr, UINT8 Reg);
void  TOUCH_IO_Delay                   (UINT32 Delay);

UINT8 TOUCH_CheckFooterButtonPressed   (s_FOOTER* f, UINT16 posX, UINT16 posY);

void  TOUCH_TaskRun                    (void *argument);
#endif
