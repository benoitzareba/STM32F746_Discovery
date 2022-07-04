//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  TextInput.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "TextInput.h"
#include "DispWidgets.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------
#define NB_TEXT_INPUT_MAX               8

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
static s_WIDGET_TEXT_INPUT _textInput[NB_TEXT_INPUT_MAX];

//---------- Functions ----------
static void*   _NewTextInput     (e_WIDGET_MODE mode);
static BOOL    _InitTextInput    (s_WIDGET *pWdgt, void* ptr);
static BOOL    _DispTextInput    (s_WIDGET *pWdgt);
static BOOL    _UpdateTextInput  (s_WIDGET *pWdgt, void* ptr);
static BOOL    _DeleteTextInput  (s_WIDGET *pWdgt);

//=============================================================================
//--- DEFINITIONS
//=============================================================================

//-----------------------------------------------------------------------------
// FONCTION    :  _NewTextInput
//
// DESCRIPTION : Chargement du widget
//-----------------------------------------------------------------------------
static void *_NewTextInput (e_WIDGET_MODE mode)
{
   UINT8 i;
   s_WIDGET_TEXT_INPUT *pRet = NULL;

   if (_count < NB_TEXT_INPUT_MAX)
   {
      //--- Cherche un emplacement libre
      for (i = 0; i < NB_TEXT_INPUT_MAX; i++)
      {
         if (_textInput[i].used == FALSE)
         {
            _count++;
            _textInput[i].used = TRUE;
            pRet = &_textInput[i];
            break;
         }
      }
   }

   return pRet;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _InitTextInput
//
// DESCRIPTION : Initialisation du widget
//-----------------------------------------------------------------------------
static BOOL _InitTextInput (s_WIDGET *pWdgt, void* ptr)
{
   s_WIDGET_TEXT_INPUT *pTxtInput   = (s_WIDGET_TEXT_INPUT *)pWdgt->param;
   s_WIDGET_TEXT_INPUT *param       = (s_WIDGET_TEXT_INPUT *)ptr;

   pTxtInput->color        = param->color;
   pTxtInput->backColor    = param->backColor;
   pTxtInput->width        = param->width;
   pTxtInput->txtColor     = param->txtColor;
   STRING_COPY(pTxtInput->str, param->str);

   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _DispTextInput
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static BOOL _DispTextInput (s_WIDGET *pWdgt)
{
   BOOL isDisp = FALSE;
   s_WIDGET_TEXT_INPUT *pTxtInput = (s_WIDGET_TEXT_INPUT *)pWdgt->param;

   switch (pWdgt->state)
   {
      case TO_DISP_AND_UPDATE_WIDGET_STATE :
      case TO_DISP_WIDGET_STATE :
      {
         //--- Affichage du widget
         DISP_WDGT_ShowTextInput(pWdgt->posX, pWdgt->posY, pTxtInput, pWdgt->isFirstTime);
         isDisp = TRUE;
         break;
      }

      default :
         break;
   }

   return isDisp;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _UpdateTextInput
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static BOOL _UpdateTextInput (s_WIDGET *pWdgt, void* ptr)
{
   //BOOL isEvent = FALSE;
   BOOL isUpdated = FALSE;
   //s_WIDGET_TEXT_INPUT *pButton = (s_WIDGET_TEXT_INPUT *)pWdgt->param;

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
// FONCTION    :  _DeleteTextInput
//
// DESCRIPTION : Dechargement du widget
//-----------------------------------------------------------------------------
static BOOL _DeleteTextInput (s_WIDGET *pWdgt)
{
   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    :  WIDGET_TextInputGetDefaultFuncs
//
// DESCRIPTION :  Recupere les pointers de fonctions par defaut du widget
//-----------------------------------------------------------------------------
BOOL WIDGET_TextInputGetDefaultFuncs (s_WIDGET *pWdgt)
{
   BOOL  isGeted = FALSE;

   if (pWdgt != NULL)
   {
      pWdgt->func.new     = (void*)_NewTextInput;
      pWdgt->func.init    = _InitTextInput;
      pWdgt->func.disp    = _DispTextInput;
      pWdgt->func.update  = _UpdateTextInput;
      pWdgt->func.delete  = _DeleteTextInput;

      isGeted = TRUE;
   }

   return isGeted;
}
