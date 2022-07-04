//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  ProgressBar.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "ProgressBar.h"
#include "DispWidgets.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------
#define NB_PROGRESS_BAR_MAX               5

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
static s_WIDGET_PROGRESS_BAR _progressBar[NB_PROGRESS_BAR_MAX];

//---------- Functions ----------
static void*   _NewProgressBar      (e_WIDGET_MODE mode);
static BOOL    _InitProgressBar     (s_WIDGET *pWdgt, void* ptr);
static BOOL    _DispProgressBar     (s_WIDGET *pWdgt);
static BOOL    _UpdateProgressBar   (s_WIDGET *pWdgt, void* ptr);
static BOOL    _DeleteProgressBar   (s_WIDGET *pWdgt);

//=============================================================================
//--- DEFINITIONS
//=============================================================================

//-----------------------------------------------------------------------------
// FONCTION    :  _NewProgressBar
//
// DESCRIPTION : Chargement du widget
//-----------------------------------------------------------------------------
static void *_NewProgressBar (e_WIDGET_MODE mode)
{
   UINT8 i;
   s_WIDGET_PROGRESS_BAR *pRet = NULL;

   if (_count < NB_PROGRESS_BAR_MAX)
   {
      //--- Cherche un emplacement libre
      for (i = 0; i < NB_PROGRESS_BAR_MAX; i++)
      {
         if (_progressBar[i].used == FALSE)
         {
            _count++;
            _progressBar[i].used = TRUE;
            pRet = &_progressBar[i];
            break;
         }
      }
   }

   return pRet;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _InitProgressBar
//
// DESCRIPTION : Initialisation du widget
//-----------------------------------------------------------------------------
static BOOL _InitProgressBar (s_WIDGET *pWdgt, void* ptr)
{
   s_WIDGET_PROGRESS_BAR *pProgressBar = (s_WIDGET_PROGRESS_BAR *)pWdgt->param;
   s_WIDGET_PROGRESS_BAR *param        = (s_WIDGET_PROGRESS_BAR *)ptr;

   pProgressBar->backgroundColor = param->backgroundColor;
   pProgressBar->baseColor       = param->baseColor;
   pProgressBar->color           = param->color;
   pProgressBar->currentValue    = param->currentValue;
   pProgressBar->height          = param->height;
   pProgressBar->width           = param->width;
   pProgressBar->outline         = param->outline;
   pProgressBar->cursorValue     = param->cursorValue;

   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _DispProgressBar
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static BOOL _DispProgressBar (s_WIDGET *pWdgt)
{
   BOOL isDisp = FALSE;
   s_WIDGET_SLIDE_BAR  *pSlide = (s_WIDGET_SLIDE_BAR *)pWdgt->param;

   switch (pWdgt->state)
   {
      case TO_DISP_AND_UPDATE_WIDGET_STATE :
      case TO_DISP_WIDGET_STATE :
      {
         //--- Affichage du widget
         DISP_WDGT_ShowProgressBar(pWdgt->posX, pWdgt->posY, pSlide, pWdgt->isFirstTime);
         isDisp = TRUE;
         break;
      }

      default :
         break;
   }

   return isDisp;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _UpdateProgressBar
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static BOOL _UpdateProgressBar (s_WIDGET *pWdgt, void* ptr)
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
// FONCTION    :  _DeleteProgressBar
//
// DESCRIPTION : Dechargement du widget
//-----------------------------------------------------------------------------
static BOOL _DeleteProgressBar (s_WIDGET *pWdgt)
{
   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    :  WIDGET_ProgressBarGetDefaultFuncs
//
// DESCRIPTION :  Recupere les pointers de fonctions par defaut du widget
//-----------------------------------------------------------------------------
BOOL WIDGET_ProgressBarGetDefaultFuncs (s_WIDGET *pWdgt)
{
   BOOL  isGeted = FALSE;

   if (pWdgt != NULL)
   {
      pWdgt->func.new     = (void*)_NewProgressBar;
      pWdgt->func.init    = _InitProgressBar;
      pWdgt->func.disp    = _DispProgressBar;
      pWdgt->func.update  = _UpdateProgressBar;
      pWdgt->func.delete  = _DeleteProgressBar;

      isGeted = TRUE;
   }

   return isGeted;
}

#warning : debug
void UPDATE_PROGRESS_BAR (UINT8 unused)
{
   UINT8 i;

   static BOOL way[NB_PROGRESS_BAR_MAX] = {FALSE};

   for (i = 0; i < NB_PROGRESS_BAR_MAX; i++)
   {
      if (way[i] == FALSE)
      {
         _progressBar[i].currentValue += 0.01;
         if (_progressBar[i].currentValue >= 1.00 && _progressBar[i].currentValue < 1.05)
            way[i] = TRUE;
      }
      else
      {
         _progressBar[i].currentValue -= 0.01;
         if (_progressBar[i].currentValue <= 0.01)
            way[i] = FALSE;
      }
   }
}
