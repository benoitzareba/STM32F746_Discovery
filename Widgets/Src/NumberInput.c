//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  NumberInput.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "NumberInput.h"
#include "DispWidgets.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------
#define NB_NUMBER_INPUT_MAX               4

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
static s_WIDGET_NUMBER_INPUT _numberInput[NB_NUMBER_INPUT_MAX];

//---------- Functions ----------
static void*   _NewInputValue    (e_WIDGET_MODE mode);
static BOOL    _InitInputValue   (s_WIDGET *pWdgt, void* ptr);
static BOOL    _DispInputValue   (s_WIDGET *pWdgt);
static BOOL    _UpdateInputValue (s_WIDGET *pWdgt, void* ptr);
static BOOL    _DeleteInputValue (s_WIDGET *pWdgt);

//=============================================================================
//--- DEFINITIONS
//=============================================================================

//-----------------------------------------------------------------------------
// FONCTION    :  _NewInputValue
//
// DESCRIPTION : Chargement du widget
//-----------------------------------------------------------------------------
static void *_NewInputValue (e_WIDGET_MODE mode)
{
   UINT8 i;
   s_WIDGET_NUMBER_INPUT *pRet = NULL;

   if (_count < NB_NUMBER_INPUT_MAX)
   {
      //--- Cherche un emplacement libre
      for (i = 0; i < NB_NUMBER_INPUT_MAX; i++)
      {
         if (_numberInput[i].used == FALSE)
         {
            _count++;
            _numberInput[i].used = TRUE;
            pRet = &_numberInput[i];
            break;
         }
      }
   }

   return pRet;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _InitInputValue
//
// DESCRIPTION : Initialisation du widget
//-----------------------------------------------------------------------------
static BOOL _InitInputValue (s_WIDGET *pWdgt, void* ptr)
{
   s_WIDGET_NUMBER_INPUT *pNumberInput = (s_WIDGET_NUMBER_INPUT *)pWdgt->param;
   s_WIDGET_NUMBER_INPUT *param = (s_WIDGET_NUMBER_INPUT *)ptr;

   pNumberInput->width           = param->width;
   pNumberInput->height          = param->height;
   pNumberInput->backgroundColor = param->backgroundColor;
   pNumberInput->color           = param->color;
   pNumberInput->currentValue    = param->currentValue;

   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _DispInputValue
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static BOOL _DispInputValue (s_WIDGET *pWdgt)
{
   BOOL isDisp = FALSE;
   s_WIDGET_NUMBER_INPUT *pNumberInput = (s_WIDGET_NUMBER_INPUT *)pWdgt->param;

   switch (pWdgt->state)
   {
      case TO_DISP_AND_UPDATE_WIDGET_STATE :
      case TO_DISP_WIDGET_STATE :
      {
         //--- Affichage du widget
         DISP_WDGT_ShowNumberInput(pWdgt->posX, pWdgt->posY, pNumberInput, pWdgt->isFirstTime);
         isDisp = TRUE;
         break;
      }

      default :
         break;
   }

   return isDisp;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _UpdateInputValue
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static BOOL _UpdateInputValue (s_WIDGET *pWdgt, void* ptr)
{
   //BOOL isEvent = FALSE;
   BOOL isUpdated = FALSE;
   //s_WIDGET_NUMBER_INPUT *pNumberInput = (s_WIDGET_NUMBER_INPUT *)pWdgt->param;

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
// FONCTION    :  _DeleteInputValue
//
// DESCRIPTION : Dechargement du widget
//-----------------------------------------------------------------------------
static BOOL _DeleteInputValue (s_WIDGET *pWdgt)
{
   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    :  WIDGET_NumberInputGetDefaultFuncs
//
// DESCRIPTION :  Recupere les pointers de fonctions par defaut du widget
//-----------------------------------------------------------------------------
BOOL WIDGET_NumberInputGetDefaultFuncs (s_WIDGET *pWdgt)
{
   BOOL  isGeted = FALSE;

   if (pWdgt != NULL)
   {
      pWdgt->func.new      = (void*)_NewInputValue;
      pWdgt->func.init     = _InitInputValue;
      pWdgt->func.disp     = _DispInputValue;
      pWdgt->func.update   = _UpdateInputValue;
      pWdgt->func.delete   = _DeleteInputValue;

      isGeted = TRUE;
   }

   return isGeted;
}

#warning : debug
void UPDATE_NUMBER_INPUT (INT16 valInput)
{
   _numberInput[0].currentValue = valInput;
}
