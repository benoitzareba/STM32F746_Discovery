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
   pCircleProgress->dispVal         = param->dispVal;

   //--- Zone active touchscreen
   pWdgt->activeZone.nbActiveZone   = 0;

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
	BOOL status = FALSE;
	UINT8 i;
	s_WIDGET_CIRCLE_PROGRESS *pCompare = (s_WIDGET_CIRCLE_PROGRESS*)pWdgt->param;

	//--- Determine le widget a supprimer
	for (i = 0; i < NB_CIRCLE_PROGRESS_MAX; i++)
	{
		//--- S'il y a une correspondance
		if (&_circleProgress[i] == pCompare)
		{
			//--- On le libere
			_circleProgress[i].used = FALSE;

			//--- On decremente le nombre de widget utilise
			if (_count > 0)
				_count--;

			status = TRUE;
			break;
		}
	}

   return status;
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
void UPDATE_CIRCLE_PROGRESS (UINT8 unused)
{
   UINT8 i;

   static BOOL way[NB_CIRCLE_PROGRESS_MAX] = {FALSE};

   for (i = 0; i < NB_CIRCLE_PROGRESS_MAX; i++)
   {
      _circleProgress[i].oldValue = _circleProgress[i].currentValue;

      if (way[i] == FALSE)
      {
         _circleProgress[i].currentValue += 1;
         if (_circleProgress[i].currentValue >= 100)
            way[i] = TRUE;
      }
      else
      {
         _circleProgress[i].currentValue -= 1;
         if (_circleProgress[i].currentValue == 0)
            way[i] = FALSE;
      }
   }
}
