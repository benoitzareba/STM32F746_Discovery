//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  ToggleSwitch.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "ToggleSwitch.h"
#include "DispWidgets.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------
#define NB_TOGGLE_SWITCH_MAX      4

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
static s_WIDGET_TOGGLE_SWITCH _toggleSwitch[NB_TOGGLE_SWITCH_MAX] = {0};

//---------- Functions ----------
static void*   _NewToggleSwitch     (e_WIDGET_MODE mode);
static BOOL    _InitToggleSwitch    (s_WIDGET *pWdgt, void* ptr);
static BOOL    _DispToggleSwitch    (s_WIDGET *pWdgt);
static BOOL    _UpdateToggleSwitch  (s_WIDGET *pWdgt, void* ptr);
static BOOL    _DeleteToggleSwitch  (s_WIDGET *pWdgt);

//=============================================================================
//--- DEFINITIONS
//=============================================================================

//-----------------------------------------------------------------------------
// FONCTION    :  _NewToggleSwitch
//
// DESCRIPTION : Nouveau toggle switch
//-----------------------------------------------------------------------------
static void *_NewToggleSwitch (e_WIDGET_MODE mode)
{
   UINT8 i;
   s_WIDGET_TOGGLE_SWITCH *pRet = NULL;

   if (_count < NB_TOGGLE_SWITCH_MAX)
   {
      //--- Cherche un emplacement libre
      for (i = 0; i < NB_TOGGLE_SWITCH_MAX; i++)
      {
         if (_toggleSwitch[i].used == FALSE)
         {
            _count++;
            _toggleSwitch[i].used = TRUE;
            pRet = &_toggleSwitch[i];
            break;
         }
      }
   }

   return pRet;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _InitToggleSwitch
//
// DESCRIPTION : Initialisation du widget
//-----------------------------------------------------------------------------
static BOOL _InitToggleSwitch (s_WIDGET *pWdgt, void* ptr)
{
   s_WIDGET_TOGGLE_SWITCH *pToggleSwitch = (s_WIDGET_TOGGLE_SWITCH *)pWdgt->param;
   s_WIDGET_TOGGLE_SWITCH *param = (s_WIDGET_TOGGLE_SWITCH *)ptr;

   pToggleSwitch->colorOn        = param->colorOn;
   pToggleSwitch->colorOff       = param->colorOff;
   pToggleSwitch->currentValue   = param->currentValue;

   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _DispToggleSwitch
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static BOOL _DispToggleSwitch (s_WIDGET *pWdgt)
{
   BOOL isDisp = FALSE;
   s_WIDGET_TOGGLE_SWITCH *pToggleSwitch = (s_WIDGET_TOGGLE_SWITCH *)pWdgt->param;

   switch (pWdgt->state)
   {
      case TO_DISP_AND_UPDATE_WIDGET_STATE :
      case TO_DISP_WIDGET_STATE :
      {
         //--- Affichage du widget
         DISP_WDGT_ShowToggleSwitch(pWdgt->posX, pWdgt->posY, pToggleSwitch, pWdgt->isFirstTime);
         isDisp = TRUE;
         break;
      }

      default :
         break;
   }

   return isDisp;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _UpdateToggleSwitch
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static BOOL _UpdateToggleSwitch (s_WIDGET *pWdgt, void* ptr)
{
   //BOOL isEvent = FALSE;
   BOOL isUpdated = FALSE;
   //s_WIDGET_SLIDE *pSlide = (s_WIDGET_SLIDE *)pWdgt->param;

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
// FONCTION    :  _DeleteToggleSwitch
//
// DESCRIPTION : Dechargement du widget
//-----------------------------------------------------------------------------
static BOOL _DeleteToggleSwitch (s_WIDGET *pWdgt)
{
   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    :  WIDGET_ToggleSwitchGetDefaultFuncs
//
// DESCRIPTION :  Recupere les pointers de fonctions par defaut du widget
//-----------------------------------------------------------------------------
BOOL WIDGET_ToggleSwitchGetDefaultFuncs (s_WIDGET *pWdgt)
{
   BOOL  isGeted = FALSE;

   if (pWdgt != NULL)
   {
      pWdgt->func.new     = (void*)_NewToggleSwitch;
      pWdgt->func.init    = _InitToggleSwitch;
      pWdgt->func.disp    = _DispToggleSwitch;
      pWdgt->func.update  = _UpdateToggleSwitch;
      pWdgt->func.delete  = _DeleteToggleSwitch;

      isGeted = TRUE;
   }

   return isGeted;
}
