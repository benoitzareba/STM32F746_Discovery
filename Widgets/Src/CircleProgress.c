//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  CircleProgress.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "CircleProgress.h"
#include "DispWidgets.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------
#define NB_CIRCLE_PROGRESS_MAX            6

//-----------------------------------------------------------------------------
// Structures and types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------

//---------- Functions ----------

//-----------------------------------------------------------------------------
// Private variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------
static UINT8 _count = 0;
s_WIDGET_CIRCLE_PROGRESS _circleProgress[NB_CIRCLE_PROGRESS_MAX];

//---------- Functions ----------
static void*   _NewCircleProgress      (e_WIDGET_MODE mode);
static BOOL    _InitCircleProgress     (s_WIDGET *pWdgt, void* ptr);
static BOOL    _DispCircleProgress     (s_WIDGET *pWdgt);
static BOOL    _UpdateCircleProgress   (s_WIDGET *pWdgt, void* ptr);
static BOOL    _DeleteCircleProgress   (s_WIDGET *pWdgt);

//=============================================================================
//--- DEFINITIONS
//=============================================================================

//-----------------------------------------------------------------------------
// FONCTION    :  _NewCircleProgress
//
// DESCRIPTION : Chargement du widget
//-----------------------------------------------------------------------------
static void *_NewCircleProgress (e_WIDGET_MODE mode)
{
   UINT8 i;
   s_WIDGET_CIRCLE_PROGRESS *pRet = NULL;

   if (_count < NB_CIRCLE_PROGRESS_MAX)
   {
      //--- Cherche un emplacement libre
      for (i = 0; i < NB_CIRCLE_PROGRESS_MAX; i++)
      {
         if (_circleProgress[i].used == FALSE)
         {
            _count++;
            _circleProgress[i].used = TRUE;
            pRet = &_circleProgress[i];
            break;
         }
      }
   }

   return pRet;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _InitCircleProgress
//
// DESCRIPTION : Initialisation du widget
//-----------------------------------------------------------------------------
static BOOL _InitCircleProgress (s_WIDGET *pWdgt, void* ptr)
{
   s_WIDGET_CIRCLE_PROGRESS *pCircleProgress = (s_WIDGET_CIRCLE_PROGRESS *)pWdgt->param;
   s_WIDGET_CIRCLE_PROGRESS *param = (s_WIDGET_CIRCLE_PROGRESS *)ptr;

   //--- Affectation des parametres du widget
   pCircleProgress->currentValue    = param->currentValue;
   pCircleProgress->color           = param->color;
   pCircleProgress->backgroundColor = param->backgroundColor;
   pCircleProgress->maskColor       = param->maskColor;
   pCircleProgress->radius          = param->radius;

   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _DispCircleProgress
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static BOOL _DispCircleProgress (s_WIDGET *pWdgt)
{
   BOOL isDisp = FALSE;
   s_WIDGET_CIRCLE_PROGRESS *pCircleProgress = (s_WIDGET_CIRCLE_PROGRESS *)pWdgt->param;

   switch (pWdgt->state)
   {
      case TO_DISP_AND_UPDATE_WIDGET_STATE :
      case TO_DISP_WIDGET_STATE :
      {
         //--- Affichage du widget
         DISP_WDGT_ShowCircleProgress(pWdgt->posX, pWdgt->posY, pCircleProgress, pWdgt->isFirstTime);
         isDisp = TRUE;
         break;
      }

      default :
         break;
   }

   return isDisp;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _UpdateCircleProgress
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static BOOL _UpdateCircleProgress (s_WIDGET *pWdgt, void* ptr)
{
   //BOOL isEvent = FALSE;
   BOOL isUpdated = FALSE;
   //s_WIDGET_CHECKBOX *pSlide = (s_WIDGET_CHECKBOX *)pWdgt->param;

   switch (pWdgt->state)
   {
      case TO_DISP_AND_UPDATE_WIDGET_STATE :
      {
         /*
         isEvent = getevent ?

         if (isEvent == TRUE)
         {

         }
         isUpdated = TRUE;
         */
         break;
      }

      default :
         break;
   }

   return isUpdated;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _DeleteCircleProgress
//
// DESCRIPTION : Dechargement du widget
//-----------------------------------------------------------------------------
static BOOL _DeleteCircleProgress (s_WIDGET *pWdgt)
{
   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    :  WIDGET_CircleProgressGetDefaultFuncs
//
// DESCRIPTION :  Recupere les pointers de fonctions par defaut du widget
//-----------------------------------------------------------------------------
BOOL WIDGET_CircleProgressGetDefaultFuncs (s_WIDGET *pWdgt)
{
   BOOL  isGeted = FALSE;

   if (pWdgt != NULL)
   {
      pWdgt->func.new     = (void*)_NewCircleProgress;
      pWdgt->func.init    = _InitCircleProgress;
      pWdgt->func.disp    = _DispCircleProgress;
      pWdgt->func.update  = _UpdateCircleProgress;
      pWdgt->func.delete  = _DeleteCircleProgress;

      isGeted = TRUE;
   }

   return isGeted;
}

#warning : debug
void UPDATE_CIRCLE_PROGRESS (UINT16 valueProgress)
{
   _circleProgress[0].oldValue = _circleProgress[0].currentValue;
   _circleProgress[0].currentValue = valueProgress;

   _circleProgress[1].oldValue = 100 - valueProgress + 1;
   _circleProgress[1].currentValue = _circleProgress[1].oldValue - 1;

   _circleProgress[2].oldValue = _circleProgress[2].currentValue;
   _circleProgress[2].currentValue = valueProgress;
}
