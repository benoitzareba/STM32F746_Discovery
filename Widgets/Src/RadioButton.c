//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  RadioButton.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "RadioButton.h"
#include "DispWidgets.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------
#define NB_RADIO_BUTTON_MAX      6

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
static s_WIDGET_RADIOBUTTON _radioButton[NB_RADIO_BUTTON_MAX];

//---------- Functions ----------
static void*   _NewRadioButton      (e_WIDGET_MODE mode);
static BOOL    _InitRadioButton     (s_WIDGET *pWdgt, void* ptr);
static BOOL    _DispRadioButton     (s_WIDGET *pWdgt);
static BOOL    _UpdateRadioButton   (s_WIDGET *pWdgt, void* ptr);
static BOOL    _DeleteRadioButton   (s_WIDGET *pWdgt);

//=============================================================================
//--- DEFINITIONS
//=============================================================================

//-----------------------------------------------------------------------------
// FONCTION    :  _NewRadioButton
//
// DESCRIPTION : Chargement du widget
//-----------------------------------------------------------------------------
static void *_NewRadioButton (e_WIDGET_MODE mode)
{
   UINT8 i;
   s_WIDGET_RADIOBUTTON *pRet = NULL;

   if (_count < NB_RADIO_BUTTON_MAX)
   {
      //--- Cherche un emplacement libre
      for (i = 0; i < NB_RADIO_BUTTON_MAX; i++)
      {
         if (_radioButton[i].used == FALSE)
         {
            _count++;
            _radioButton[i].used = TRUE;
            pRet = &_radioButton[i];
            break;
         }
      }
   }

   return pRet;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _InitRadioButton
//
// DESCRIPTION : Initialisation du widget
//-----------------------------------------------------------------------------
static BOOL _InitRadioButton (s_WIDGET *pWdgt, void* ptr)
{
   s_WIDGET_RADIOBUTTON *pRadioButton = (s_WIDGET_RADIOBUTTON *)pWdgt->param;
   s_WIDGET_RADIOBUTTON *param = (s_WIDGET_RADIOBUTTON *)ptr;

   //--- Affectation des parametres du widget
   pRadioButton->currentValue    = param->currentValue;
   pRadioButton->color           = param->color;
   pRadioButton->backgroundColor = param->backgroundColor;

   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _DispRadioButton
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static BOOL _DispRadioButton (s_WIDGET *pWdgt)
{
   BOOL isDisp = FALSE;
   s_WIDGET_RADIOBUTTON  *pSlide = (s_WIDGET_RADIOBUTTON *)pWdgt->param;

   switch (pWdgt->state)
   {
      case TO_DISP_AND_UPDATE_WIDGET_STATE :
      case TO_DISP_WIDGET_STATE :
      {
         //--- Affichage du widget
         DISP_WDGT_ShowRadioButton(pWdgt->posX, pWdgt->posY, pSlide, pWdgt->isFirstTime);
         isDisp = TRUE;
         break;
      }

      default :
         break;
   }

   return isDisp;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _UpdateRadioButton
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static BOOL _UpdateRadioButton (s_WIDGET *pWdgt, void* ptr)
{
   //BOOL isEvent = FALSE;
   BOOL isUpdated = FALSE;
   //s_WIDGET_RADIOBUTTON *pSlide = (s_WIDGET_SLIDE *)pWdgt->param;

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
// FONCTION    :  _DeleteRadioButton
//
// DESCRIPTION : Dechargement du widget
//-----------------------------------------------------------------------------
static BOOL _DeleteRadioButton (s_WIDGET *pWdgt)
{
   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    :  WIDGET_RadioButtonGetDefaultFuncs
//
// DESCRIPTION :  Recupere les pointers de fonctions par defaut du widget
//-----------------------------------------------------------------------------
BOOL WIDGET_RadioButtonGetDefaultFuncs (s_WIDGET *pWdgt)
{
   BOOL  isGeted = FALSE;

   if (pWdgt != NULL)
   {
      pWdgt->func.new     = (void*)_NewRadioButton;
      pWdgt->func.init    = _InitRadioButton;
      pWdgt->func.disp    = _DispRadioButton;
      pWdgt->func.update  = _UpdateRadioButton;
      pWdgt->func.delete  = _DeleteRadioButton;

      isGeted = TRUE;
   }

   return isGeted;
}

#warning : debug
void UPDATE_RADIO_BUTTON (BOOL valRadioButton)
{
   _radioButton[0].currentValue = FALSE;
   _radioButton[1].currentValue = valRadioButton;
   _radioButton[2].currentValue = !valRadioButton;
   _radioButton[3].currentValue = FALSE;
}
