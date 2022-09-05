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
   s_ACTIVE_ZONE* pZone;

   //--- Initialisation du widget
   pNumberInput->width           = param->width;
   pNumberInput->height          = param->height;
   pNumberInput->backgroundColor = param->backgroundColor;
   pNumberInput->color           = param->color;
   pNumberInput->currentValue    = param->currentValue;

   //--- Zone active pour le touchscreen
   pZone                         = &pWdgt->activeZone;
   pZone->nbActiveZone           = 2;

   //--- Zone 1
   pZone->zone[0].idFunc         = NUMBER_INPUT_MINUS_ONE;
   pZone->zone[0].coord.x        = pWdgt->posX;
   pZone->zone[0].coord.y        = pWdgt->posY;
   pZone->zone[0].coord.width    = param->width * NUMBER_INPUT_BUTTON_PERCENT;
   pZone->zone[0].coord.height   = param->height;

   //--- Zone 2
   pZone->zone[1].idFunc         = NUMBER_INPUT_PLUS_ONE;
   pZone->zone[1].coord.x        = pWdgt->posX + param->width - (param->width * NUMBER_INPUT_BUTTON_PERCENT);
   pZone->zone[1].coord.y        = pWdgt->posY;
   pZone->zone[1].coord.width    = param->width * NUMBER_INPUT_BUTTON_PERCENT;
   pZone->zone[1].coord.height   = param->height;

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
   BOOL isUpdated = FALSE;
   s_WIDGET_NUMBER_INPUT *pNumberInput = (s_WIDGET_NUMBER_INPUT *)pWdgt->param;

   switch (pWdgt->state)
   {
      case TO_DISP_AND_UPDATE_WIDGET_STATE :
      {
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
void UPDATE_NUMBER_INPUT (INT16 unused)
{
   UINT8 i;

   static BOOL way[NB_NUMBER_INPUT_MAX] = {FALSE};

   for (i = 0; i < NB_NUMBER_INPUT_MAX; i++)
   {
      if (way[i] == FALSE)
      {
         _numberInput[i].currentValue += 1;
         if (_numberInput[i].currentValue >= 100)
            way[i] = TRUE;
      }
      else
      {
         _numberInput[i].currentValue -= 1;
         if (_numberInput[i].currentValue == 0)
            way[i] = FALSE;
      }
   }
}
