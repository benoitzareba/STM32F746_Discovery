//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  CheckBox.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "CheckBox.h"
#include "DispWidgets.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------
#define NB_CHECKBOX_MAX   6

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
s_WIDGET_CHECKBOX _checkBox[NB_CHECKBOX_MAX];

//---------- Functions ----------
static void*   _NewCheckBox      (e_WIDGET_MODE mode);
static BOOL    _InitCheckBox     (s_WIDGET *pWdgt, void* ptr);
static BOOL    _DispCheckBox     (s_WIDGET *pWdgt);
static BOOL    _UpdateCheckBox   (s_WIDGET *pWdgt, void* ptr);
static BOOL    _DeleteCheckBox   (s_WIDGET *pWdgt);

//=============================================================================
//--- DEFINITIONS
//=============================================================================

//-----------------------------------------------------------------------------
// FONCTION    :  _NewCheckBox
//
// DESCRIPTION : Chargement du widget
//-----------------------------------------------------------------------------
static void *_NewCheckBox (e_WIDGET_MODE mode)
{
   UINT8 i;
   s_WIDGET_CHECKBOX *pRet = NULL;

   if (_count < NB_CHECKBOX_MAX)
   {
      //--- Cherche un emplacement libre
      for (i = 0; i < NB_CHECKBOX_MAX; i++)
      {
         if (_checkBox[i].used == FALSE)
         {
            _count++;
            _checkBox[i].used = TRUE;
            pRet = &_checkBox[i];
            break;
         }
      }
   }

   return pRet;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _InitCheckBox
//
// DESCRIPTION : Initialisation du widget
//-----------------------------------------------------------------------------
static BOOL _InitCheckBox (s_WIDGET *pWdgt, void* ptr)
{
   s_WIDGET_CHECKBOX *pCheckBox = (s_WIDGET_CHECKBOX *)pWdgt->param;
   s_WIDGET_CHECKBOX *param = (s_WIDGET_CHECKBOX *)ptr;
   s_ACTIVE_ZONE* pZone;

   //--- Affectation des parametres du widget
   pCheckBox->currentValue    = param->currentValue;
   pCheckBox->color           = param->color;
   pCheckBox->backgroundColor = param->backgroundColor;

   //--- Zone active pour le touchscreen
   pZone                         = &pWdgt->activeZone;
   pZone->nbActiveZone           = 1;
   pZone->zone[0].coord.x        = pWdgt->posX;
   pZone->zone[0].coord.y        = pWdgt->posY;
   pZone->zone[0].coord.width    = CHECKBOX_WIDTH;
   pZone->zone[0].coord.height   = CHECKBOX_HEIGHT;

   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _DispCheckBox
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static BOOL _DispCheckBox (s_WIDGET *pWdgt)
{
   BOOL isDisp = FALSE;
   s_WIDGET_CHECKBOX  *pCheckBox = (s_WIDGET_CHECKBOX *)pWdgt->param;

   switch (pWdgt->state)
   {
      case TO_DISP_AND_UPDATE_WIDGET_STATE :
      case TO_DISP_WIDGET_STATE :
      {
         //--- Affichage du widget
         DISP_WDGT_ShowCheckBox(pWdgt->posX, pWdgt->posY, pCheckBox, pWdgt->isFirstTime);
         isDisp = TRUE;
         break;
      }

      default :
         break;
   }

   return isDisp;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _UpdateCheckBox
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static BOOL _UpdateCheckBox (s_WIDGET *pWdgt, void* ptr)
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
// FONCTION    :  _DeleteCheckBox
//
// DESCRIPTION : Dechargement du widget
//-----------------------------------------------------------------------------
static BOOL _DeleteCheckBox (s_WIDGET *pWdgt)
{
   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    :  WIDGET_CheckBoxGetDefaultFuncs
//
// DESCRIPTION :  Recupere les pointers de fonctions par defaut du widget
//-----------------------------------------------------------------------------
BOOL WIDGET_CheckBoxGetDefaultFuncs (s_WIDGET *pWdgt)
{
   BOOL  isGeted = FALSE;

   if (pWdgt != NULL)
   {
      pWdgt->func.new     = (void*)_NewCheckBox;
      pWdgt->func.init    = _InitCheckBox;
      pWdgt->func.disp    = _DispCheckBox;
      pWdgt->func.update  = _UpdateCheckBox;
      pWdgt->func.delete  = _DeleteCheckBox;

      isGeted = TRUE;
   }

   return isGeted;
}
