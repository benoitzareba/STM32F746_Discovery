//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  stm32f7xx_it.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include <Tasks.h>
#include "Board.h"
#include "stm32f7xx_it.h"
#include "FreeRTOS.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------
#define POLY_X(Z)                      ((INT32)((Points + Z)->X))
#define POLY_Y(Z)                      ((INT32)((Points + Z)->Y))
#define ABS(X)                         ((X) > 0 ? (X) : -(X))

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
// FONCTION    : NMI_Handler
//
// DESCRIPTION : This function handles Non maskable interrupt.
//-----------------------------------------------------------------------------
void NMI_Handler (void)
{
   while (1)
   {
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : HardFault_Handler
//
// DESCRIPTION : This function handles Hard fault interrupt.
//-----------------------------------------------------------------------------
void HardFault_Handler (void)
{
   while (1)
   {
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : MemManage_Handler
//
// DESCRIPTION : This function handles Memory management fault.
//-----------------------------------------------------------------------------
void MemManage_Handler (void)
{
   while (1)
   {
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : BusFault_Handler
//
// DESCRIPTION : This function handles Pre-fetch fault, memory access fault.
//-----------------------------------------------------------------------------
void BusFault_Handler (void)
{
   while (1)
   {
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : UsageFault_Handler
//
// DESCRIPTION : This function handles Undefined instruction or illegal state.
//-----------------------------------------------------------------------------
void UsageFault_Handler (void)
{
   while (1)
   {
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : DebugMon_Handler
//
// DESCRIPTION : This function handles Debug monitor.
//-----------------------------------------------------------------------------
void DebugMon_Handler (void)
{
}

//-----------------------------------------------------------------------------
// FONCTION    : SysTick_Handler
//
// DESCRIPTION : This function handles System tick timer.
//-----------------------------------------------------------------------------
void SysTick_Handler (void)
{
   HAL_IncTick();

#if (INCLUDE_xTaskGetSchedulerState == 1)
   if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
   {
#endif
   xPortSysTickHandler();
#if (INCLUDE_xTaskGetSchedulerState == 1)
   }
#endif
}
