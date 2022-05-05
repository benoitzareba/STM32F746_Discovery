//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  SlideBar.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "SlideBar.h"
#include "DispWidgets.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------
#define NB_SLIDE_BAR_MAX               4

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
static s_WIDGET_SLIDE_BAR _slide[NB_SLIDE_BAR_MAX];

//---------- Functions ----------
static void*   _NewSlide      (e_WIDGET_MODE mode);
static BOOL    _InitSlide     (s_WIDGET *pWdgt, void* ptr);
static BOOL    _DispSlide     (s_WIDGET *pWdgt);
static BOOL    _UpdateSlide   (s_WIDGET *pWdgt, void* ptr);
static BOOL    _DeleteSlide   (s_WIDGET *pWdgt);

//=============================================================================
//--- DEFINITIONS
//=============================================================================

//-----------------------------------------------------------------------------
// FONCTION    :  _NewSlide
//
// DESCRIPTION : Chargement du widget
//-----------------------------------------------------------------------------
static void *_NewSlide (e_WIDGET_MODE mode)
{
   UINT8 i;
   s_WIDGET_SLIDE_BAR *pRet = NULL;

   if (_count < NB_SLIDE_BAR_MAX)
   {
      //--- Cherche un emplacement libre
      for (i = 0; i < NB_SLIDE_BAR_MAX; i++)
      {
         if (_slide[i].used == FALSE)
         {
            _count++;
            _slide[i].used = TRUE;
            pRet = &_slide[i];
            break;
         }
      }
   }

   return pRet;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _InitSlide
//
// DESCRIPTION : Initialisation du widget
//-----------------------------------------------------------------------------
static BOOL _InitSlide (s_WIDGET *pWdgt, void* ptr)
{
   s_WIDGET_SLIDE_BAR *pSlide = (s_WIDGET_SLIDE_BAR *)pWdgt->param;
   s_WIDGET_SLIDE_BAR *param = (s_WIDGET_SLIDE_BAR *)ptr;

   pSlide->backgroundColor = param->backgroundColor;
   pSlide->baseColor       = param->baseColor;
   pSlide->color           = param->color;
   pSlide->currentValue    = param->currentValue;
   pSlide->length          = param->length;

   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _DispSlide
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static BOOL _DispSlide (s_WIDGET *pWdgt)
{
   BOOL isDisp = FALSE;
   s_WIDGET_SLIDE_BAR  *pSlide = (s_WIDGET_SLIDE_BAR *)pWdgt->param;

   switch (pWdgt->state)
   {
      case TO_DISP_AND_UPDATE_WIDGET_STATE :
      case TO_DISP_WIDGET_STATE :
      {
         //--- Affichage du widget
         DISP_WDGT_ShowSlide(pWdgt->posX, pWdgt->posY, pSlide, pWdgt->isFirstTime);
         isDisp = TRUE;
         break;
      }

      default :
         break;
   }

   return isDisp;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _UpdateSlide
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static BOOL _UpdateSlide (s_WIDGET *pWdgt, void* ptr)
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
// FONCTION    :  _DeleteSlide
//
// DESCRIPTION : Dechargement du widget
//-----------------------------------------------------------------------------
static BOOL _DeleteSlide (s_WIDGET *pWdgt)
{
   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    :  WIDGET_SlideGetDefaultFuncs
//
// DESCRIPTION :  Recupere les pointers de fonctions par defaut du widget
//-----------------------------------------------------------------------------
BOOL WIDGET_SlideGetDefaultFuncs (s_WIDGET *pWdgt)
{
   BOOL  isGeted = FALSE;

   if (pWdgt != NULL)
   {
      pWdgt->func.new     = (void*)_NewSlide;
      pWdgt->func.init    = _InitSlide;
      pWdgt->func.disp    = _DispSlide;
      pWdgt->func.update  = _UpdateSlide;
      pWdgt->func.delete  = _DeleteSlide;

      isGeted = TRUE;
   }

   return isGeted;
}

#warning : debug
void UPDATE_SLIDE_BAR (FLOAT32 valSlideBar)
{
   _slide[0].currentValue = valSlideBar;
   _slide[1].currentValue = 100 - valSlideBar;
}
