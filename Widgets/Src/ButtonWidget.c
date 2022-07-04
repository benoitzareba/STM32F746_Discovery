//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  ButtonWidget.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "ButtonWidget.h"
#include "DispWidgets.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------
#define NB_BUTTON_MAX               16

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
static UINT8 _count;
static s_WIDGET_BUTTON _button[NB_BUTTON_MAX];

//---------- Functions ----------
static void*   _NewButton     (e_WIDGET_MODE mode);
static BOOL    _InitButton    (s_WIDGET *pWdgt, void* ptr);
static BOOL    _DispButton    (s_WIDGET *pWdgt);
static BOOL    _UpdateButton  (s_WIDGET *pWdgt, void* ptr);
static BOOL    _DeleteButton  (s_WIDGET *pWdgt);

//=============================================================================
//--- DEFINITIONS
//=============================================================================

//-----------------------------------------------------------------------------
// FONCTION    :  _NewButton
//
// DESCRIPTION : Chargement du widget
//-----------------------------------------------------------------------------
static void *_NewButton (e_WIDGET_MODE mode)
{
   UINT8 i;
   s_WIDGET_BUTTON *pRet = NULL;

   if (_count < NB_BUTTON_MAX)
   {
      //--- Cherche un emplacement libre
      for (i = 0; i < NB_BUTTON_MAX; i++)
      {
         if (_button[i].used == FALSE)
         {
            _count++;
            _button[i].used = TRUE;
            pRet = &_button[i];
            break;
         }
      }
   }

   return pRet;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _InitButton
//
// DESCRIPTION : Initialisation du widget
//-----------------------------------------------------------------------------
static BOOL _InitButton (s_WIDGET *pWdgt, void* ptr)
{
   s_WIDGET_BUTTON *pButton   = (s_WIDGET_BUTTON *)pWdgt->param;
   s_WIDGET_BUTTON *param     = (s_WIDGET_BUTTON *)ptr;

   pButton->color          = param->color;
   pButton->outlineColor   = param->outlineColor;
   pButton->height         = param->height;
   pButton->width          = param->width;
   pButton->pressed        = FALSE;
   pButton->txtColor       = param->txtColor;
   STRING_COPY(pButton->str, param->str);

   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _DispButton
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static BOOL _DispButton (s_WIDGET *pWdgt)
{
   BOOL isDisp = FALSE;
   s_WIDGET_BUTTON *pButton = (s_WIDGET_BUTTON *)pWdgt->param;

   switch (pWdgt->state)
   {
      case TO_DISP_AND_UPDATE_WIDGET_STATE :
      case TO_DISP_WIDGET_STATE :
      {
         //--- Affichage du widget
         DISP_WDGT_ShowButton(pWdgt->posX, pWdgt->posY, pButton, pWdgt->isFirstTime);
         isDisp = TRUE;
         break;
      }

      default :
         break;
   }

   return isDisp;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _UpdateButton
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static BOOL _UpdateButton (s_WIDGET *pWdgt, void* ptr)
{
   //BOOL isEvent = FALSE;
   BOOL isUpdated = FALSE;
   //s_WIDGET_BUTTON *pButton = (s_WIDGET_BUTTON *)pWdgt->param;

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
// FONCTION    :  _DeleteButton
//
// DESCRIPTION : Dechargement du widget
//-----------------------------------------------------------------------------
static BOOL _DeleteButton (s_WIDGET *pWdgt)
{
   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    :  WIDGET_ButtonGetDefaultFuncs
//
// DESCRIPTION :  Recupere les pointers de fonctions par defaut du widget
//-----------------------------------------------------------------------------
BOOL WIDGET_ButtonGetDefaultFuncs (s_WIDGET *pWdgt)
{
   BOOL  isGeted = FALSE;

   if (pWdgt != NULL)
   {
      pWdgt->func.new     = (void*)_NewButton;
      pWdgt->func.init    = _InitButton;
      pWdgt->func.disp    = _DispButton;
      pWdgt->func.update  = _UpdateButton;
      pWdgt->func.delete  = _DeleteButton;

      isGeted = TRUE;
   }

   return isGeted;
}
