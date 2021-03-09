//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// HEADER      :  Board.h
//
//=============================================================================
#ifndef _BOARD_H_
#define _BOARD_H_

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "stm32f7xx_hal.h"
#include "TypeDefs.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------
#define BOARD_GetSystemClockMHz()                        ((UINT16)(SystemCoreClock * (FLOAT32)0.000001))

//--- LED 1
#define GPIO_LED_1                                       GPIOI
#define GPIO_PIN_LED_1                                   GPIO_PIN_1

//--- BUTTON 1
#define GPIO_BUTTON_1                                    GPIOI
#define GPIO_PIN_BUTTON_1                                GPIO_PIN_11

//--- Touch screen interrupt signal
#define GPIO_TOUCHSCREEN_INTERRUPT                       GPIOI
#define GPIO_PIN_TOUCHSCREEN                             GPIO_PIN_13
#define TOUCHSCREEN_INT_EXTI_IRQn                        EXTI15_10_IRQn

//--- I2C Touch screen
#define TOUCHSCREEN_I2Cx                                 I2C3
#define TOUCHSCREEN_I2Cx_CLK_ENABLE()                    __HAL_RCC_I2C3_CLK_ENABLE()
#define TOUCHSCREEN_DMAx_CLK_ENABLE()                    __HAL_RCC_DMA1_CLK_ENABLE()
#define TOUCHSCREEN_I2Cx_SCL_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOH_CLK_ENABLE()

#define TOUCHSCREEN_I2Cx_FORCE_RESET()                   __HAL_RCC_I2C3_FORCE_RESET()
#define TOUCHSCREEN_I2Cx_RELEASE_RESET()                 __HAL_RCC_I2C3_RELEASE_RESET()

//--- Definition for I2Cx Pins
#define TOUCHSCREEN_I2Cx_SCL_PIN                         GPIO_PIN_7
#define TOUCHSCREEN_I2Cx_SCL_SDA_GPIO_PORT               GPIOH
#define TOUCHSCREEN_I2Cx_SCL_SDA_AF                      GPIO_AF4_I2C3
#define TOUCHSCREEN_I2Cx_SDA_PIN                         GPIO_PIN_8

//--- I2C interrupt requests
#define TOUCHSCREEN_I2Cx_EV_IRQn                         I2C3_EV_IRQn
#define TOUCHSCREEN_I2Cx_ER_IRQn                         I2C3_ER_IRQn

//-----------------------------------------------------------------------------
// Structures and types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------

//---------- Functions ----------
void BOARD_ConfAll (void);

#endif
