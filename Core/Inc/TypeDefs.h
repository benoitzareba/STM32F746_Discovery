//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// HEADER      :  TypeDefs.h
//
//=============================================================================
#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------
#define GET_MIN(X, Y)                     ((X) < (Y) ? (X) : (Y))
#define GET_MAX(X, Y)                     ((X) > (Y) ? (X) : (Y))
#define ARRAY_SIZE(x)                     (sizeof(x) / sizeof((x)[0]))
#define OUTPUT			                     0
#define INPUT			                     1
#define MAX_CHARS_STRING                  50
#define STRING_FORMAT(S, F, ...)          sprintf((S), (const char *)(F), __VA_ARGS__)
#define STRING_COPY(D,S)                  strcpy((char *)(D), (const char *)(S))
#define STRING_COPY_LEN(D,S,L)            strncpy((char *)(D), (const char *)(S), (L))
#define STRING_LEN(S)                     strlen((char *)(S))

//-----------------------------------------------------------------------------
// Types
//-----------------------------------------------------------------------------
typedef enum //--- BOOL
{
   FALSE = 0,
   TRUE
} BOOL;

typedef int				         INT;
typedef unsigned int	         UINT;
typedef unsigned char		   BYTE;
typedef unsigned short int	   WORD;
typedef unsigned long		   DWORD;
typedef signed char			   CHAR;
typedef signed short int	   SHORT;
typedef signed long			   LONG;
typedef unsigned char         CHAR8;
typedef CHAR8*                STRING;
typedef signed char           INT8;
typedef signed short int      INT16;
typedef signed long int       INT32;
typedef signed long long      INT64;
typedef unsigned char         UINT8;
typedef unsigned short int    UINT16;
typedef unsigned long int     UINT32;
typedef unsigned long long    UINT64;
typedef float                 FLOAT32;
typedef long double           FLOAT64;
typedef char                  STRING_TAB[MAX_CHARS_STRING];

typedef BOOL (* SCREEN_DISP_FUNC)      (void *);
typedef BOOL (* SCREEN_UPDATE_FUNC)    (void *, void *);

typedef union //--- u_UINT8
{
   struct
   {
      unsigned char b0:1;
      unsigned char b1:1;
      unsigned char b2:1;
      unsigned char b3:1;
      unsigned char b4:1;
      unsigned char b5:1;
      unsigned char b6:1;
      unsigned char b7:1;
   } bits;
   UINT8  val;
} u_UINT8;

typedef union //--- u_UINT16
{
   UINT8  tab[sizeof(UINT16)];
   UINT16 val;
   struct
   {
      unsigned char b0:1;
      unsigned char b1:1;
      unsigned char b2:1;
      unsigned char b3:1;
      unsigned char b4:1;
      unsigned char b5:1;
      unsigned char b6:1;
      unsigned char b7:1;
      unsigned char b8:1;
      unsigned char b9:1;
      unsigned char b10:1;
      unsigned char b11:1;
      unsigned char b12:1;
      unsigned char b13:1;
      unsigned char b14:1;
      unsigned char b15:1;
   } bits;
} u_UINT16;

typedef union //--- u_UINT32
{
   UINT8  tab[sizeof(UINT32)];
   UINT32 val;
} u_UINT32;

typedef struct //--- s_RECT
{
   UINT16               x;
   UINT16               y;
   UINT16               width;
   UINT16               height;
} s_RECT;

typedef struct //--- s_BOUTON_POP
{
   STRING_TAB     title;
   BOOL           isEnabled;
   INT8           level;
   STRING_TAB     screen;
   s_RECT         coord;
   void*          func;
} s_BUTTON_POP;

typedef struct //--- s_POPUP
{
   UINT8                type;
   STRING               title;
   STRING               line1;
   STRING               line2;
   STRING               line3;
   UINT8                nbButton;
   BOOL                 closeCross;
   s_BUTTON_POP         *btLeft;
   s_BUTTON_POP         *btRight;
   UINT8                idSel;
   SCREEN_DISP_FUNC     prevDisp;
   SCREEN_UPDATE_FUNC   prevUpdate;
} s_POPUP;

#endif
