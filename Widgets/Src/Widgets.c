//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  Widgets.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "Widgets.h"
#include "ToggleSwitch.h"
#include "SlideBar.h"
#include "CheckBox.h"
#include "RadioButton.h"
#include "NumberInput.h"
#include "CircleProgress.h"
#include "ProgressBar.h"
#include "ButtonWidget.h"
#include "TextInput.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------

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
static s_WIDGET      _widget[MAX_WIDGETS];
static s_WIDGET_TAB  _tabWdgt = {0, 0, _widget};

//---------- Functions ----------
static s_WIDGET*  _AddInTab         (void);
static BOOL       _DelInTab         (s_WIDGET *pWdgt);
static BOOL       _GetDefaultFuncs  (s_WIDGET *pWdgt);

//=============================================================================
//--- DEFINITIONS
//=============================================================================

//-----------------------------------------------------------------------------
// FONCTION    :  _AddInTab
//
// DESCRIPTION :  Recuperation d'un pointer sur un widget disponible
//-----------------------------------------------------------------------------
static s_WIDGET *_AddInTab (void)
{
   s_WIDGET *pWdgt   = NULL;
   s_WIDGET *pNextFreeWdgt;

   //--- Le nombre max de widgets n'est pas atteint
   if ((_tabWdgt.count < MAX_WIDGETS) && (_tabWdgt.firstFreeIdx < MAX_WIDGETS))
   {
      //--- Incremente le nombre de ctrls utilises
      _tabWdgt.count++;

      //--- Pointer sur le premier ctrl de libre dans le tableau
      pWdgt = &_tabWdgt.pWdgt[_tabWdgt.firstFreeIdx];

      pNextFreeWdgt = pWdgt + 1;
      _tabWdgt.firstFreeIdx++;

      //--- Recherche du prochain emplacement de libre
      while (pNextFreeWdgt->state != NO_WIDGET_STATE && _tabWdgt.firstFreeIdx < MAX_WIDGETS)
      {
         pNextFreeWdgt++;
         _tabWdgt.firstFreeIdx++;
      }
   }

   return pWdgt;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _DelInTab
//
// DESCRIPTION :  Suppression d'un wigdet
//-----------------------------------------------------------------------------
static BOOL _DelInTab (s_WIDGET *pWdgt)
{
   BOOL  isDeleted = FALSE;
   UINT8 freeIdx;

   if ((pWdgt != NULL) && (_tabWdgt.count > 0))
   {
      //--- Verification que le widget soit dans le tableau
      if ((pWdgt >= _widget) && (pWdgt < (_widget + MAX_WIDGETS)))
      {
         //--- Decremente le nombre de ctrls utilises
         _tabWdgt.count--;

         //--- Initialisation du ctrl
         pWdgt->state = NO_WIDGET_STATE;

         //--- Mise a jour de l'idx du premier widget de libre
         freeIdx = pWdgt - _tabWdgt.pWdgt;
         if (_tabWdgt.firstFreeIdx > freeIdx)
            _tabWdgt.firstFreeIdx = freeIdx;

         isDeleted = TRUE;
      }
   }

   return isDeleted;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _GetDefaultFuncs
//
// DESCRIPTION :  Recuperation des fonction par defaut d'un widget
//-----------------------------------------------------------------------------
static BOOL _GetDefaultFuncs (s_WIDGET *pWdgt)
{
   BOOL found = FALSE;

   if (pWdgt != NULL)
   {
      //--- En fonction type de widget souhaite
      switch (pWdgt->type)
      {
         case WIDGET_SLIDE_TYPE:
         {
            found = WIDGET_SlideGetDefaultFuncs(pWdgt);
            break;
         }

         case WIDGET_TOGGLE_SWITCH_TYPE:
         {
            found = WIDGET_ToggleSwitchGetDefaultFuncs(pWdgt);
            break;
         }

         case WIDGET_RADIO_BUTTON_TYPE:
         {
            found = WIDGET_RadioButtonGetDefaultFuncs(pWdgt);
            break;
         }

         case WIDGET_CHECKBOX_TYPE:
         {
            found = WIDGET_CheckBoxGetDefaultFuncs(pWdgt);
            break;
         }

         case WIDGET_NUMBER_INPUT_TYPE:
         {
            found = WIDGET_NumberInputGetDefaultFuncs(pWdgt);
            break;
         }

         case WIDGET_CIRCLE_PROGRESS_TYPE:
         {
            found = WIDGET_CircleProgressGetDefaultFuncs(pWdgt);
            break;
         }

         case WIDGET_PROGRESS_BAR_TYPE:
         {
            found = WIDGET_ProgressBarGetDefaultFuncs(pWdgt);
            break;
         }

         case WIDGET_BUTTON_TYPE:
         {
            found = WIDGET_ButtonGetDefaultFuncs(pWdgt);
            break;
         }

         case WIDGET_TEXT_INPUT_TYPE:
         {
            found = WIDGET_TextInputGetDefaultFuncs(pWdgt);
            break;
         }

         default:
            break;
      }
   }

   return found;
}

//-----------------------------------------------------------------------------
// FONCTION    :  WIDGET_GlobalInit
//
// DESCRIPTION :  Initialisation globale
//-----------------------------------------------------------------------------
void WIDGET_GlobalInit (void)
{
   _tabWdgt.count          = 0;
   _tabWdgt.firstFreeIdx   = 0;
   _tabWdgt.pWdgt          = _widget;

   memset(_widget, 0, sizeof(_widget));
}

//-----------------------------------------------------------------------------
// FONCTION    :  WIDGET_Create
//
// DESCRIPTION :  Creation d'un widget
//-----------------------------------------------------------------------------
void *WIDGET_Create (s_WIDGET *pWdgt)
{
   if (pWdgt != NULL)
   {
      if (pWdgt->state == TO_CREATE_WIDGET_STATE)
      {
         //--- Creation du widget
         if (pWdgt->func.new != NULL)
            pWdgt->param = pWdgt->func.new();
      }

      //--- Vers initialisation
      pWdgt->state = TO_INIT_WIDGET_STATE;

      return pWdgt->param;
   }
   else
      return NULL;
}

//-----------------------------------------------------------------------------
// FONCTION    :  WIDGET_Init
//
// DESCRIPTION :  Initialisation d'un widget
//-----------------------------------------------------------------------------
BOOL WIDGET_Init (s_WIDGET *pWdgt, void *ptr)
{
   BOOL isInited = FALSE;

   if (pWdgt != NULL)
   {
      if (pWdgt->state == TO_INIT_WIDGET_STATE)
      {
         //--- Fonction d'initialisation du widget
         if (pWdgt->func.init != NULL)
            isInited = pWdgt->func.init(pWdgt, ptr);
         else
            isInited = TRUE;

         if (isInited == TRUE)
         {
            pWdgt->isUpToDate    = FALSE;
            pWdgt->isFirstTime   = TRUE;

            //--- Nouvel etat en fonction du mode
            if (pWdgt->mode == WIDGET_DISPLAY_ONLY_MODE)
               pWdgt->state = TO_DISP_WIDGET_STATE;
            else
               pWdgt->state = TO_DISP_AND_UPDATE_WIDGET_STATE;
         }
         else
         {
            //--- Suppression du widget
            WIDGET_Delete(pWdgt);
         }
      }
      else
      {
         pWdgt->isUpToDate    = FALSE;
         pWdgt->isFirstTime   = TRUE;
      }
   }

   return isInited;
}

//------------------------------------------------------------------------------
// FONCTION    : WIDGET_Update
//
// DESCRIPTION :  MAJ d'un widget
//------------------------------------------------------------------------------
BOOL WIDGET_Update (s_WIDGET *pWdgt, void *event)
{
   BOOL isUpdated = FALSE;

   //--- Si on a bien un widget
   if (pWdgt != NULL)
   {
      //--- Si on a un evenement a traiter et que le ctrl n'est pas de type dispOnly
      /*
      if (EVENT_HAS(pEvent) && (pWdgt->mode != WIDGET_DISPLAY_ONLY_MODE))
      {
         //--- Si le ctrl a une fonction d'update
         if (pWdgt->funcs.update != NULL)
            isUpdated |= pWdgt->funcs.update(pWdgt, event);

         pWdgt->isUpToDate &= !isUpdated;

         //--- Si widget a effacer
         if (pWdgt->state == TO_DELETE_WIDGET_STATE)
         {
            //--- Suppression du widget
            WIDGET_Delete(pWdgt);
         }
      }
      */
   }

   return isUpdated;
}

//------------------------------------------------------------------------------
// FONCTION    :  WIDGET_Display
//
// DESCRIPTION :  Affichage d'un widget
//------------------------------------------------------------------------------
BOOL WIDGET_Display (s_WIDGET *pWdgt)
{
   BOOL isDisped = FALSE;

   //--- Si on a bien un ctrl
   if (pWdgt != NULL)
   {
      if (pWdgt->state != WAIT_WIDGET_STATE)
      {
         //--- Si on doit appeler la fonction d'affichage
         if (!pWdgt->isUpToDate)
         {
            //--- Si le CTRL a une fonction d'affichage
            if (pWdgt->func.disp != NULL)
            {
               isDisped = pWdgt->func.disp(pWdgt);

               pWdgt->isUpToDate    = isDisped;
               pWdgt->isFirstTime   = FALSE;
            }
            else
               isDisped = TRUE;
         }
      }
   }

   return isDisped;
}

//------------------------------------------------------------------------------
// FONCTION    :  WIDGET_Delete
//
// DESCRIPTION :  Dechargement d'un widget
//------------------------------------------------------------------------------
BOOL WIDGET_Delete (s_WIDGET *pWdgt)
{
   BOOL isDeleted = FALSE;

   //--- Si on a bien un ctrl
   if (pWdgt != NULL)
   {
      //--- Si le ctrl a une fonction de dechargement
      if (pWdgt->func.delete != NULL)
         isDeleted = pWdgt->func.delete(pWdgt);
      else
         isDeleted = TRUE;

      //--- Suppression du tableau de widget
      if (isDeleted == TRUE)
         _DelInTab(pWdgt);
   }

   return isDeleted;
}

//-----------------------------------------------------------------------------
// FONCTION    :  WIDGET_Alloc
//
// DESCRIPTION :  Allocation d'un nouveau widget
//-----------------------------------------------------------------------------
s_WIDGET *WIDGET_Alloc (e_WIDGET_TYPE type, e_WIDGET_MODE mode)
{
   s_WIDGET *pWdgt = NULL;

   if (pWdgt->state != TO_DISP_WIDGET_STATE && pWdgt->state != TO_DISP_AND_UPDATE_WIDGET_STATE && pWdgt->state != TO_UPDATE_WIDGET_STATE)
      pWdgt = _AddInTab();

   if (pWdgt != NULL)
   {
      //--- Initialisation de la structure
      memset(pWdgt, 0, sizeof(s_WIDGET));

      //--- Init
      pWdgt->type = type;
      pWdgt->state = TO_CREATE_WIDGET_STATE;
      pWdgt->mode = mode;

      //--- Recuperation des fonctions par defaut
      _GetDefaultFuncs(pWdgt);

      //--- Chargement
      WIDGET_Create(pWdgt);
   }

   return pWdgt;
}
