//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  Popup.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "Popup.h"
#include "Screen.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------
#define RED_CROSS_X              365
#define RED_CROSS_Y              46
#define RED_CROSS_WIDTH          8
#define RED_CROSS_HEIGHT         8

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

static s_BUTTON_POP  btLeftPopup    = {"Oui", TRUE, 0, "", {0}, NULL};
static s_BUTTON_POP  btRightPopup   = {"Non", TRUE, 0, "", {0}, NULL};
static STRING_TAB    popupTitle;
static STRING_TAB    popupLine1;
static STRING_TAB    popupLine2;
static STRING_TAB    popupLine3;
static s_POPUP       dummyPopup     = {QUESTION_POPUP, (STRING)popupTitle, (STRING)popupLine1, (STRING)popupLine2, (STRING)popupLine3, 2, TRUE, &btLeftPopup, &btRightPopup, 0};

//---------- Functions ----------
static void _InitPopup (UINT8 type, STRING title, STRING line1, STRING line2, STRING line3, UINT8 nbBt, s_BUTTON_POP *leftBt, s_BUTTON_POP *rightBt, UINT8 btSel);

//=============================================================================
//--- DEFINITIONS
//=============================================================================

//-----------------------------------------------------------------------------
// FONCTION    :  _InitPopup
//
// DESCRIPTION :  Initialisation du popup avant de l'afficher
//-----------------------------------------------------------------------------
static void _InitPopup (UINT8 type, STRING title, STRING line1, STRING line2, STRING line3, UINT8 nbBt, s_BUTTON_POP *leftBt, s_BUTTON_POP *rightBt, UINT8 btSel)
{
   //--- Initialisation du texte de la popup
   memset(popupLine1, 0, sizeof(popupLine1));
   memset(popupLine2, 0, sizeof(popupLine2));
   memset(popupLine3, 0, sizeof(popupLine3));

   //--- Type de popup
   dummyPopup.type = type;

   //--- Copie du texte sur les lignes
   STRING_COPY(dummyPopup.title, "");
   if (title != NULL)
   {
      STRING_COPY(dummyPopup.title, title);
   }

   //--- Copie du texte sur les lignes
   STRING_COPY(dummyPopup.line1, "");
   if (line1 != NULL)
   {
      STRING_COPY(dummyPopup.line1, line1);
   }

   STRING_COPY(dummyPopup.line2, "");
   if (line2 != NULL)
   {
      STRING_COPY(dummyPopup.line2, line2);
   }

   STRING_COPY(dummyPopup.line3, "");
   if (line3 != NULL)
   {
      STRING_COPY(dummyPopup.line3, line3);
   }

   //--- Nombre de boutons
   dummyPopup.nbButton= nbBt;

   //--- Bouton selectionne
   dummyPopup.idSel = btSel;

   //--- Bouton de gauche
   dummyPopup.btLeft = NULL;
   if (leftBt != NULL)
   {
      //--- Pointeur
      dummyPopup.btLeft = &btLeftPopup;

      //--- Configuration du bouton
      dummyPopup.btLeft->func       = leftBt->func;
      dummyPopup.btLeft->level      = leftBt->level;
      dummyPopup.btLeft->isEnabled  = leftBt->isEnabled;

      dummyPopup.btLeft->coord.height  = 0;
      dummyPopup.btLeft->coord.width   = 0;
      dummyPopup.btLeft->coord.x       = 0;
      dummyPopup.btLeft->coord.y       = 0;

      STRING_COPY(dummyPopup.btLeft->screen, leftBt->screen);
      STRING_COPY(dummyPopup.btLeft->title, leftBt->title);
   }

   //--- Bouton de droite
   dummyPopup.btRight = NULL;
   if (rightBt != NULL)
   {
      //--- Pointeur
      dummyPopup.btRight = &btRightPopup;

      //--- Configuration du bouton
      dummyPopup.btRight->func       = rightBt->func;
      dummyPopup.btRight->level      = rightBt->level;
      dummyPopup.btRight->isEnabled  = rightBt->isEnabled;

      dummyPopup.btRight->coord.height = 0;
      dummyPopup.btRight->coord.width  = 0;
      dummyPopup.btRight->coord.x      = 0;
      dummyPopup.btRight->coord.y      = 0;

      STRING_COPY(dummyPopup.btRight->screen, rightBt->screen);
      STRING_COPY(dummyPopup.btRight->title, rightBt->title);
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : POPUP_Informations
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
void POPUP_Informations (void *p)
{
   s_SCREEN *s = (s_SCREEN *)p;

   s_BUTTON_POP okBt =  {"OK", TRUE, 0, "", {0}, NULL};

   //--- Preparation du popup
   _InitPopup(INFORMATION_POPUP, (STRING)"Informations", (STRING)"Lorem ipsum dolor sit amet alun,\0", (STRING)"consectetur adipiscing elit.\0", (STRING)"Integer rutrum accumsan pretium.\0", 1, &okBt, NULL, 0);

   //--- Charge les fonctions d'affichage d'un popup
   SCREEN_ShowPopup(s, &dummyPopup);
}

//-----------------------------------------------------------------------------
// FONCTION    : POPUP_GetCloseCrossPosition
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
void POPUP_GetCloseCrossPosition (s_RECT* crossPosition)
{
   if (crossPosition != NULL)
   {
      crossPosition->height = RED_CROSS_HEIGHT;
      crossPosition->width = RED_CROSS_WIDTH;
      crossPosition->x = RED_CROSS_X;
      crossPosition->y = RED_CROSS_Y;
   }
}
