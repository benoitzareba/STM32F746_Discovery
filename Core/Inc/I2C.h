//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// HEADER      :  I2C.h
//
//=============================================================================
#ifndef _I2C_H_
#define _I2C_H_

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "TypeDefs.h"
#include "stm32f7xx_hal.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------

/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated from APB1 source clock = 50 MHz */
/* Due to the big MOFSET capacity for adapting the camera level the rising time is very large (>1us) */
/* 0x40912732 takes in account the big rising and aims a clock of 100khz */
#define DISCOVERY_I2Cx_TIMING                      ((UINT32)0x40912732)

//-----------------------------------------------------------------------------
// Structures and types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------

//---------- Functions ----------
void              I2C_MspInit       (I2C_HandleTypeDef *i2c_handler);
void              I2C_Init          (I2C_HandleTypeDef *i2c_handler);
HAL_StatusTypeDef I2C_ReadMultiple  (I2C_HandleTypeDef *i2c_handler, UINT8 Addr, UINT16 Reg, UINT16 MemAddress, UINT8 *Buffer, UINT16 Length);
HAL_StatusTypeDef I2C_WriteMultiple (I2C_HandleTypeDef *i2c_handler, UINT8 Addr, UINT16 Reg, UINT16 MemAddress, UINT8 *Buffer, UINT16 Length);
void              I2C_Error         (I2C_HandleTypeDef *i2c_handler, UINT8 Addr);

#endif
