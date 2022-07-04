//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  Touchscreen.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "Touchscreen.h"
#include "Popup.h"
#include "Board.h"
#include "I2C.h"
#include "Event.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------
#define TS_I2C_ADDRESS              ((UINT16)0x70)

//-----------------------------------------------------------------------------
// Structures and types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------
I2C_HandleTypeDef    hI2cTouchScreenHandler = {0};
osThreadId_t         TOUCH_TaskHandle;
const osThreadAttr_t TOUCH_TaskAttributes   = {.name = "touchScreenTask",      .priority = (osPriority_t) osPriorityNormal,    .stack_size = 128 * 4};

//---------- Functions ----------

//-----------------------------------------------------------------------------
// Private variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------
static TS_DrvTypeDef *_Driver;
static UINT16        _XBoundary;
static UINT16        _YBoundary;
static UINT8         _orientation;
static UINT8         _I2cAddress;

//---------- Functions ----------

//=============================================================================
//--- DEFINITIONS
//=============================================================================

//-----------------------------------------------------------------------------
// FONCTION    : TOUCH_Initialize
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
UINT8 TOUCH_Initialize (UINT16 sizeX, UINT16 sizeY)
{
   UINT8 status = TS_OK;
   _XBoundary = sizeX;
   _YBoundary = sizeY;

   //--- Read ID and verify if the touch screen driver is ready
   ft5336_ts_drv.Init(TS_I2C_ADDRESS);

   if (ft5336_ts_drv.ReadID(TS_I2C_ADDRESS) == FT5336_ID_VALUE)
   {
      //--- Initialize the TS driver structure
      _Driver = &ft5336_ts_drv;
      _I2cAddress = TS_I2C_ADDRESS;
      _orientation = TS_SWAP_XY;

      //--- Initialize the TS driver
      _Driver->Start(_I2cAddress);
   }
   else
      status = TS_DEVICE_NOT_FOUND;

   return status;
}

//-----------------------------------------------------------------------------
// FONCTION    : TOUCH_ConfigInterrupt
//
// DESCRIPTION : Configures and enables the touch screen interrupts.
//-----------------------------------------------------------------------------
UINT8 TOUCH_ConfigInterrupt (void)
{
   GPIO_InitTypeDef GPIO_InitStruct = {0};

   //--- Configure Interrupt mode for SD detection pin
   GPIO_InitStruct.Pin     = GPIO_PIN_TOUCHSCREEN;
   GPIO_InitStruct.Pull    = GPIO_NOPULL;
   GPIO_InitStruct.Speed   = GPIO_SPEED_FAST;
   GPIO_InitStruct.Mode    = GPIO_MODE_IT_RISING;
   HAL_GPIO_Init(GPIO_TOUCHSCREEN_INTERRUPT, &GPIO_InitStruct);

   //--- Enable and set Touch screen EXTI Interrupt to the lowest priority
   HAL_NVIC_SetPriority((IRQn_Type)(TOUCHSCREEN_INT_EXTI_IRQn), 0x0F, 0x00);
   HAL_NVIC_EnableIRQ((IRQn_Type)(TOUCHSCREEN_INT_EXTI_IRQn));

   //--- Enable the TS ITs
   _Driver->EnableIT(_I2cAddress);

   return TS_OK;
}

//-----------------------------------------------------------------------------
// FONCTION    : TOUCH_GetInterruptStatus
//
// DESCRIPTION : Gets the touch screen interrupt status.
//-----------------------------------------------------------------------------
UINT8 TOUCH_GetInterruptStatus (void)
{
   //--- Return the TS IT status
   return (_Driver->GetITStatus(_I2cAddress));
}

//-----------------------------------------------------------------------------
// FONCTION    : TOUCH_GetState
//
// DESCRIPTION : Returns status and positions of the touch screen.
//-----------------------------------------------------------------------------
UINT8 TOUCH_GetState (TS_StateTypeDef *state)
{
   static UINT32 _x[TS_MAX_NB_TOUCH] = {0, 0};
   static UINT32 _y[TS_MAX_NB_TOUCH] = {0, 0};

   UINT8 status = TS_OK;
   UINT16 x[TS_MAX_NB_TOUCH];
   UINT16 y[TS_MAX_NB_TOUCH];
   UINT16 brute_x[TS_MAX_NB_TOUCH];
   UINT16 brute_y[TS_MAX_NB_TOUCH];
   UINT16 x_diff;
   UINT16 y_diff;
   UINT32 index;
   UINT32 weight = 0;
   UINT32 area = 0;
   UINT32 event = 0;

   //--- Check and update the number of touches active detected
   state->touchDetected = _Driver->DetectTouch(_I2cAddress);

   if (state->touchDetected)
   {
      for (index = 0; index < state->touchDetected; index++)
      {
         //--- Get each touch coordinates
         _Driver->GetXY(_I2cAddress, &(brute_x[index]), &(brute_y[index]));

         if (_orientation == TS_SWAP_NONE)
         {
            x[index] = brute_x[index];
            y[index] = brute_y[index];
         }

         if (_orientation & TS_SWAP_X)
         {
            x[index] = 4096 - brute_x[index];
         }

         if (_orientation & TS_SWAP_Y)
         {
            y[index] = 4096 - brute_y[index];
         }

         if (_orientation & TS_SWAP_XY)
         {
            y[index] = brute_x[index];
            x[index] = brute_y[index];
         }

         x_diff = x[index] > _x[index]? (x[index] - _x[index]): (_x[index] - x[index]);
         y_diff = y[index] > _y[index]? (y[index] - _y[index]): (_y[index] - y[index]);

         if ((x_diff + y_diff) > 5)
         {
            _x[index] = x[index];
            _y[index] = y[index];
         }

         if (_I2cAddress == FT5336_I2C_SLAVE_ADDRESS)
         {
            state->touchX[index] = x[index];
            state->touchY[index] = y[index];
         }
         else
         {
            //--- 2^12 = 4096 : indexes are expressed on a dynamic of 4096
            state->touchX[index] = (_XBoundary * _x[index]) >> 12;
            state->touchY[index] = (_YBoundary * _y[index]) >> 12;
         }

         //--- Get touch info related to the current touch
         ft5336_TS_GetTouchInfo(_I2cAddress, index, &weight, &area, &event);

         //--- Update TS_State structure
         state->touchWeight[index] = weight;
         state->touchArea[index]   = area;

         //--- Remap touch event
         switch (event)
         {
            case FT5336_TOUCH_EVT_FLAG_PRESS_DOWN   :
               state->touchEventId[index] = TOUCH_EVENT_PRESS_DOWN;
               break;

            case FT5336_TOUCH_EVT_FLAG_LIFT_UP :
               state->touchEventId[index] = TOUCH_EVENT_LIFT_UP;
               break;

            case FT5336_TOUCH_EVT_FLAG_CONTACT :
               state->touchEventId[index] = TOUCH_EVENT_CONTACT;
               break;

            case FT5336_TOUCH_EVT_FLAG_NO_EVENT :
               state->touchEventId[index] = TOUCH_EVENT_NO_EVT;
               break;

            default :
               status = TS_ERROR;
               break;
         }
      }

      //--- Get gesture Id
      status = TOUCH_GetGestureId(state);
   }

   return status;
}

//-----------------------------------------------------------------------------
// FONCTION    : TOUCH_GetGestureId
//
// DESCRIPTION : Update gesture Id following a touch detected.
//-----------------------------------------------------------------------------
UINT8 TOUCH_GetGestureId (TS_StateTypeDef *state)
{
   UINT32 gestureId = 0;
   UINT8  status = TS_OK;

   //--- Get gesture Id
   ft5336_TS_GetGestureID(_I2cAddress, &gestureId);

   //--- Remap gesture Id to a TS_GestureIdTypeDef value
   switch (gestureId)
   {
      case FT5336_GEST_ID_NO_GESTURE :
         state->gestureId = GEST_ID_NO_GESTURE;
         break;

      case FT5336_GEST_ID_MOVE_UP :
         state->gestureId = GEST_ID_MOVE_UP;
         break;

      case FT5336_GEST_ID_MOVE_RIGHT :
         state->gestureId = GEST_ID_MOVE_RIGHT;
         break;

      case FT5336_GEST_ID_MOVE_DOWN :
         state->gestureId = GEST_ID_MOVE_DOWN;
         break;

      case FT5336_GEST_ID_MOVE_LEFT :
         state->gestureId = GEST_ID_MOVE_LEFT;
         break;

      case FT5336_GEST_ID_ZOOM_IN :
         state->gestureId = GEST_ID_ZOOM_IN;
         break;

      case FT5336_GEST_ID_ZOOM_OUT :
         state->gestureId = GEST_ID_ZOOM_OUT;
         break;

      default :
         status = TS_ERROR;
         break;
   }

   return status;
}

//-----------------------------------------------------------------------------
// FONCTION    : TOUCH_ClearInterrupt
//
// DESCRIPTION : Clears all touch screen interrupts.
//-----------------------------------------------------------------------------
void TOUCH_ClearInterrupt (void)
{
   //--- Clear TS IT pending bits
   _Driver->ClearIT(_I2cAddress);
}

//-----------------------------------------------------------------------------
// FONCTION    : TOUCH_ResetTouchData
//
// DESCRIPTION : Function used to reset all touch data before a new acquisition of touch information.
//-----------------------------------------------------------------------------
UINT8 TOUCH_ResetTouchData (TS_StateTypeDef *state)
{
   UINT8 status = TS_ERROR;
   UINT32 index;

   if (state != (TS_StateTypeDef *)NULL)
   {
      state->gestureId = GEST_ID_NO_GESTURE;
      state->touchDetected = 0;

      for (index = 0; index < TS_MAX_NB_TOUCH; index++)
      {
         state->touchX[index]       = 0;
         state->touchY[index]       = 0;
         state->touchArea[index]    = 0;
         state->touchEventId[index] = TOUCH_EVENT_NO_EVT;
         state->touchWeight[index]  = 0;
      }

      status = TS_OK;
   }

   return (status);
}


//-----------------------------------------------------------------------------
// FONCTION    : TOUCH_IO_Init
//
// DESCRIPTION : Initializes Touchscreen low level.
//-----------------------------------------------------------------------------
void TOUCH_IO_Init (void)
{
   I2C_Init(&hI2cTouchScreenHandler);
}

//-----------------------------------------------------------------------------
// FONCTION    : TOUCH_IO_Write
//
// DESCRIPTION : Writes a single data.
//-----------------------------------------------------------------------------
void TOUCH_IO_Write (UINT8 Addr, UINT8 Reg, UINT8 Value)
{
   I2C_WriteMultiple(&hI2cTouchScreenHandler, Addr, (UINT16)Reg, I2C_MEMADD_SIZE_8BIT,(UINT8*)&Value, 1);
}

//-----------------------------------------------------------------------------
// FONCTION    : TOUCH_IO_Read
//
// DESCRIPTION : Reads a single data.
//-----------------------------------------------------------------------------
UINT8 TOUCH_IO_Read (UINT8 Addr, UINT8 Reg)
{
   UINT8 read_value = 0;

   I2C_ReadMultiple(&hI2cTouchScreenHandler, Addr, Reg, I2C_MEMADD_SIZE_8BIT, (UINT8*)&read_value, 1);

   return read_value;
}

//-----------------------------------------------------------------------------
// FONCTION    : TOUCH_IO_Delay
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
void TOUCH_IO_Delay (UINT32 Delay)
{
   HAL_Delay(Delay);
}

//-----------------------------------------------------------------------------
// FONCTION    : TOUCH_CheckFooterButtonPressed
//
// DESCRIPTION : Determine si l'appui se situe sur un bouton du footer
//-----------------------------------------------------------------------------
UINT8 TOUCH_CheckFooterButtonPressed (s_FOOTER* f, UINT16 posX, UINT16 posY)
{
   UINT8 i;
   s_RECT* r;

   for (i = 0; i < f->nbButton; i++)
   {
      r = &f->button[i].rect;
      if ((posX >= r->x && posX <= r->x + r->width) && (posY >= r->y && posY <= r->y + r->height))
      {
         break;
      }
   }

   return i;
}

//-----------------------------------------------------------------------------
// FONCTION    : TOUCH_CheckPopupButton
//
// DESCRIPTION : Determine si un bouton popup a ete presse
//-----------------------------------------------------------------------------
UINT8 TOUCH_CheckPopupButton (s_POPUP* p, UINT16 posX, UINT16 posY)
{
   UINT8 item = NO_TOUCH_ITEM;
   s_RECT posRedCross;
   s_RECT* rLeft;
   s_RECT* rRight;

   //--- Recupere les coordonnees des boutons popup
   POPUP_GetCloseCrossPosition(&posRedCross);

   //--- S'il y a au minimum 1 button
   if (p->nbButton > 0)
   {
      //--- Position des boutons
      rLeft = &p->btLeft->coord;

      if ((posX >= rLeft->x && posX <= rLeft->x + rLeft->width) && (posY >= rLeft->y && posY <= rLeft->y + rLeft->height))
      {
         item = LEFT_BUTTON;
      }

      //-- S'il y a second button
      if (p->nbButton == 2)
      {
         rRight = &p->btRight->coord;

         if ((posX >= rRight->x && posX <= rRight->x + rRight->width) && (posY >= rRight->y && posY <= rRight->y + rRight->height))
         {
            item = RIGHT_BUTTON;
         }
      }
   }

   if ((posX >= posRedCross.x - 5 && posX <= posRedCross.x + posRedCross.width + 5) && (posY >= posRedCross.y - 5 && posY <= posRedCross.y + posRedCross.height + 5))
      item = CLOSE_CROSS;

   return item;
}

//-----------------------------------------------------------------------------
// FONCTION    : TOUCH_TaskRun
//
// DESCRIPTION : Touch task run
//-----------------------------------------------------------------------------
void TOUCH_TaskRun (void *argument)
{
   TS_StateTypeDef tsState = {0};
   s_SCREEN scr;
   s_FOOTER *f;
   s_POPUP *p;
   UINT8 idButtonFooter;
   UINT8 idItemPopup;
   UINT8 touchStateOld;
   osStatus_t eventStatus;

   //--- Remove compiler warning about unused parameter.
   (void)argument;

   for ( ;; )
   {
      //--- Recupere l'ecran courant
      eventStatus = osMessageQueueGet(SCREEN_Event, &scr, NULL, 0);

      if (eventStatus == osOK)
      {
         //--- Lecture de l'etat du touchscreen         TOUCH_GetState(&tsState);

         //--- S'il y a un mouvement
         if (tsState.touchDetected != touchStateOld)
         {
             touchStateOld = tsState.touchDetected;

            if (tsState.touchDetected == 1)
            {
               if (scr.disp != SCREEN_DispPopup)
               {
                  //--- Pointeur footer
                  f = scr.footer;

                  //--- Determine si l'appui se situe sur un bouton du footer
                  idButtonFooter = TOUCH_CheckFooterButtonPressed(f, tsState.touchX[0], tsState.touchY[0]);

                  //--- Si l'appui correspond a un bouton du footer
                  if (idButtonFooter < NB_BUTTON_MAX)
                  {
                     //--- Changement d'ecran
                     osMessageQueuePut(CHANGE_SCREEN_Event, &f->button[idButtonFooter].function, 0, 0);
                  }
               }
               else if (scr.disp == SCREEN_DispPopup)
               {
                  //--- Pointeur popup
                  p = scr.popup;

                  idItemPopup = TOUCH_CheckPopupButton(p, tsState.touchX[0], tsState.touchY[0]);

                  if (idItemPopup > NO_TOUCH_ITEM && idItemPopup)
                  {
                     //--- Retourne l'item popup touche
                     osMessageQueuePut(POPUP_Event, &idItemPopup, 0, 0);
                  }
               }
            }
         }
      }
   }
}

