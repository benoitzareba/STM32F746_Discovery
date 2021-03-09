//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  I2C.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "I2C.h"
#include "Board.h"
#include "Touchscreen.h"

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
// FONCTION    : I2C_MspInit
//
// DESCRIPTION : Initializes I2C MSP
//-----------------------------------------------------------------------------
void I2C_MspInit (I2C_HandleTypeDef *i2c_handler)
{
   GPIO_InitTypeDef gpio_init_structure;

   if (i2c_handler == (I2C_HandleTypeDef*)(&hI2cTouchScreenHandler))
   {
      //--- Configure the GPIOs
      //---- Enable GPIO clock
      __HAL_RCC_GPIOH_CLK_ENABLE();

      //--- Configure I2C Tx as alternate function
      gpio_init_structure.Pin = TOUCHSCREEN_I2Cx_SCL_PIN;
      gpio_init_structure.Mode = GPIO_MODE_AF_OD;
      gpio_init_structure.Pull = GPIO_NOPULL;
      gpio_init_structure.Speed = GPIO_SPEED_FAST;
      gpio_init_structure.Alternate = TOUCHSCREEN_I2Cx_SCL_SDA_AF;
      HAL_GPIO_Init(TOUCHSCREEN_I2Cx_SCL_SDA_GPIO_PORT, &gpio_init_structure);

      //--- Configure I2C Rx as alternate function
      gpio_init_structure.Pin = TOUCHSCREEN_I2Cx_SDA_PIN;
      HAL_GPIO_Init(TOUCHSCREEN_I2Cx_SCL_SDA_GPIO_PORT, &gpio_init_structure);

      //--- Configure the I2C peripheral
      //--- Enable I2C clock
      __HAL_RCC_I2C3_CLK_ENABLE();

      //--- Force the I2C peripheral clock reset
      __HAL_RCC_I2C3_FORCE_RESET();

      //--- Release the I2C peripheral clock reset
      __HAL_RCC_I2C3_RELEASE_RESET();

      //--- Enable and set I2Cx Interrupt to a lower priority */
      HAL_NVIC_SetPriority(TOUCHSCREEN_I2Cx_EV_IRQn, 0x0F, 0);
      HAL_NVIC_EnableIRQ(TOUCHSCREEN_I2Cx_EV_IRQn);

      //--- Enable and set I2Cx Interrupt to a lower priority */
      HAL_NVIC_SetPriority(TOUCHSCREEN_I2Cx_ER_IRQn, 0x0F, 0);
      HAL_NVIC_EnableIRQ(TOUCHSCREEN_I2Cx_ER_IRQn);
   }
   else
   {
#if 0
      /* External, camera and Arduino connector I2C MSP init */

      /*** Configure the GPIOs ***/
      /* Enable GPIO clock */
      DISCOVERY_EXT_I2Cx_SCL_SDA_GPIO_CLK_ENABLE();

      /* Configure I2C Tx as alternate function */
      gpio_init_structure.Pin = DISCOVERY_EXT_I2Cx_SCL_PIN;
      gpio_init_structure.Mode = GPIO_MODE_AF_OD;
      gpio_init_structure.Pull = GPIO_NOPULL;
      gpio_init_structure.Speed = GPIO_SPEED_FAST;
      gpio_init_structure.Alternate = DISCOVERY_EXT_I2Cx_SCL_SDA_AF;
      HAL_GPIO_Init(DISCOVERY_EXT_I2Cx_SCL_SDA_GPIO_PORT, &gpio_init_structure);

      /* Configure I2C Rx as alternate function */
      gpio_init_structure.Pin = DISCOVERY_EXT_I2Cx_SDA_PIN;
      HAL_GPIO_Init(DISCOVERY_EXT_I2Cx_SCL_SDA_GPIO_PORT, &gpio_init_structure);

      /*** Configure the I2C peripheral ***/
      /* Enable I2C clock */
      DISCOVERY_EXT_I2Cx_CLK_ENABLE();

      /* Force the I2C peripheral clock reset */
      DISCOVERY_EXT_I2Cx_FORCE_RESET();

      /* Release the I2C peripheral clock reset */
      DISCOVERY_EXT_I2Cx_RELEASE_RESET();

      /* Enable and set I2Cx Interrupt to a lower priority */
      HAL_NVIC_SetPriority(DISCOVERY_EXT_I2Cx_EV_IRQn, 0x0F, 0);
      HAL_NVIC_EnableIRQ(DISCOVERY_EXT_I2Cx_EV_IRQn);

      /* Enable and set I2Cx Interrupt to a lower priority */
      HAL_NVIC_SetPriority(DISCOVERY_EXT_I2Cx_ER_IRQn, 0x0F, 0);
      HAL_NVIC_EnableIRQ(DISCOVERY_EXT_I2Cx_ER_IRQn);
#endif
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : I2C_Init
//
// DESCRIPTION : Initializes I2C HAL.
//-----------------------------------------------------------------------------
void I2C_Init (I2C_HandleTypeDef *i2c_handler)
{
   if (HAL_I2C_GetState(i2c_handler) == HAL_I2C_STATE_RESET)
   {
      if (i2c_handler == (I2C_HandleTypeDef*)(&hI2cTouchScreenHandler))
      {
         //--- LCD I2C configuration
         i2c_handler->Instance = TOUCHSCREEN_I2Cx;
      }
      else
      {
         //--- External, camera and Arduino connector  I2C configuration */
         //i2c_handler->Instance = DISCOVERY_EXT_I2Cx;
      }

      i2c_handler->Init.Timing           = DISCOVERY_I2Cx_TIMING;
      i2c_handler->Init.OwnAddress1      = 0;
      i2c_handler->Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
      i2c_handler->Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
      i2c_handler->Init.OwnAddress2      = 0;
      i2c_handler->Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
      i2c_handler->Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;

      //--- Init the I2C
      I2C_MspInit(i2c_handler);
      HAL_I2C_Init(i2c_handler);
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : I2C_ReadMultiple
//
// DESCRIPTION : Reads multiple data.
//-----------------------------------------------------------------------------
HAL_StatusTypeDef I2C_ReadMultiple(I2C_HandleTypeDef *i2c_handler, UINT8 Addr, UINT16 Reg, UINT16 MemAddress, UINT8 *Buffer, UINT16 Length)
{
   HAL_StatusTypeDef status = HAL_OK;

   status = HAL_I2C_Mem_Read(i2c_handler, Addr, (UINT16)Reg, MemAddress, Buffer, Length, 1000);

   //--- Check the communication status
   if (status != HAL_OK)
   {
      //--- I2C error occurred
      I2C_Error(i2c_handler, Addr);
   }

   return status;
}

 //-----------------------------------------------------------------------------
 // FONCTION    : I2C_WriteMultiple
 //
 // DESCRIPTION : Writes a value in a register of the device through BUS in using DMA mode.
 //-----------------------------------------------------------------------------
HAL_StatusTypeDef I2C_WriteMultiple (I2C_HandleTypeDef *i2c_handler, UINT8 Addr, UINT16 Reg, UINT16 MemAddress, UINT8 *Buffer, UINT16 Length)
{
   HAL_StatusTypeDef status = HAL_OK;

   status = HAL_I2C_Mem_Write(i2c_handler, Addr, (UINT16)Reg, MemAddress, Buffer, Length, 1000);

   //--- Check the communication status
   if (status != HAL_OK)
   {
      //--- Re-Initiaize the I2C Bus
      I2C_Error(i2c_handler, Addr);
   }

   return status;
}

//-----------------------------------------------------------------------------
// FONCTION    : I2C_Error
//
// DESCRIPTION : Manages error callback by re-initializing I2C.
//-----------------------------------------------------------------------------
void I2C_Error (I2C_HandleTypeDef *i2c_handler, UINT8 Addr)
{
   //--- De-initialize the I2C communication bus
   HAL_I2C_DeInit(i2c_handler);

   //--- Re-Initialize the I2C communication bus
   I2C_Init(i2c_handler);
}
