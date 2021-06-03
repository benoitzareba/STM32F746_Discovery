//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  Utils.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "Utils.h"

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


//---------- Functions ----------

//=============================================================================
//--- DEFINITIONS
//=============================================================================

//-----------------------------------------------------------------------------
// FONCTION    :  UTILS_CalcHash
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
UINT32 UTILS_CalcHash (STRING s)
{
   UINT8    strLen = 0;
   UINT8    i;
   UINT32   hash;
   UINT16   chkSum;

   strLen = STRING_LEN(s);
   hash =  strLen << 24;
   chkSum = 0;

   for (i = 0; i < strLen; i++)
      chkSum += s[i] * (i + 1);

   hash |= chkSum;

   return hash;
}