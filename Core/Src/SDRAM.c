//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  SDRAM.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "SDRAM.h"

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
static SDRAM_HandleTypeDef       _sdramHandle;
static FMC_SDRAM_TimingTypeDef   _Timing;
static FMC_SDRAM_CommandTypeDef  _Command;

//---------- Functions ----------

//=============================================================================
//--- DEFINITIONS
//=============================================================================

//-----------------------------------------------------------------------------
// FONCTION    : SDRAM_Init
//
// DESCRIPTION : SDRAM module initialization
//-----------------------------------------------------------------------------
UINT8 SDRAM_Init (void)
{ 
   static UINT8 sdramstatus = SDRAM_ERROR;

   //--- SDRAM device configuration
   _sdramHandle.Instance = FMC_SDRAM_DEVICE;

   //--- Timing configuration for 100Mhz as SD clock frequency (System clock is up to 200Mhz)
   _Timing.LoadToActiveDelay    = 2;
   _Timing.ExitSelfRefreshDelay = 7;
   _Timing.SelfRefreshTime      = 4;
   _Timing.RowCycleDelay        = 7;
   _Timing.WriteRecoveryTime    = 2;
   _Timing.RPDelay              = 2;
   _Timing.RCDDelay             = 2;

   _sdramHandle.Init.SDBank             = FMC_SDRAM_BANK1;
   _sdramHandle.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_8;
   _sdramHandle.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_12;
   _sdramHandle.Init.MemoryDataWidth    = SDRAM_MEMORY_WIDTH;
   _sdramHandle.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
   _sdramHandle.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_2;
   _sdramHandle.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
   _sdramHandle.Init.SDClockPeriod      = SDCLOCK_PERIOD;
   _sdramHandle.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;
   _sdramHandle.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0;

   //--- SDRAM controller initialization
   SDRAM_MspInit(&_sdramHandle, NULL);

   if (HAL_SDRAM_Init(&_sdramHandle, &_Timing) != HAL_OK)
   {
      sdramstatus = SDRAM_ERROR;
   }
   else
   {
      sdramstatus = SDRAM_OK;
   }

   //--- SDRAM initialization sequence
   SDRAM_Initialization_sequence(REFRESH_COUNT);

   return sdramstatus;
}

//-----------------------------------------------------------------------------
// FONCTION    : SDRAM_DeInit
//
// DESCRIPTION : DeInitializes the SDRAM device.
//-----------------------------------------------------------------------------
UINT8 SDRAM_DeInit (void)
{ 
   static UINT8 sdramstatus = SDRAM_ERROR;

   //--- SDRAM device de-initialization
   _sdramHandle.Instance = FMC_SDRAM_DEVICE;

   if (HAL_SDRAM_DeInit(&_sdramHandle) != HAL_OK)
   {
      sdramstatus = SDRAM_ERROR;
   }
   else
   {
      sdramstatus = SDRAM_OK;
   }

   //--- SDRAM controller de-initialization
   SDRAM_MspDeInit(&_sdramHandle, NULL);

   return sdramstatus;
}

//-----------------------------------------------------------------------------
// FONCTION    : SDRAM_Initialization_sequence
//
// DESCRIPTION : Programs the SDRAM device.
//-----------------------------------------------------------------------------
void SDRAM_Initialization_sequence (UINT32 RefreshCount)
{
   __IO UINT32 tmpmrd = 0;

   //--- Step 1: Configure a clock configuration enable command
   _Command.CommandMode            = FMC_SDRAM_CMD_CLK_ENABLE;
   _Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
   _Command.AutoRefreshNumber      = 1;
   _Command.ModeRegisterDefinition = 0;

   //--- Send the command
   HAL_SDRAM_SendCommand(&_sdramHandle, &_Command, SDRAM_TIMEOUT);

   //--- Step 2: Insert 100 us minimum delay
   //--- Inserted delay is equal to 1 ms due to systick time base unit (ms)
   HAL_Delay(1);

   //--- Step 3: Configure a PALL (precharge all) command
   _Command.CommandMode            = FMC_SDRAM_CMD_PALL;
   _Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
   _Command.AutoRefreshNumber      = 1;
   _Command.ModeRegisterDefinition = 0;

   //--- Send the command
   HAL_SDRAM_SendCommand(&_sdramHandle, &_Command, SDRAM_TIMEOUT);

   //--- Step 4: Configure an Auto Refresh command
   _Command.CommandMode            = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
   _Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
   _Command.AutoRefreshNumber      = 8;
   _Command.ModeRegisterDefinition = 0;

   //--- Send the command
   HAL_SDRAM_SendCommand(&_sdramHandle, &_Command, SDRAM_TIMEOUT);

   //--- Step 5: Program the external memory mode register
   tmpmrd = (UINT32)SDRAM_MODEREG_BURST_LENGTH_1          |\
                    SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |\
                    SDRAM_MODEREG_CAS_LATENCY_2           |\
                    SDRAM_MODEREG_OPERATING_MODE_STANDARD |\
                    SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

   _Command.CommandMode            = FMC_SDRAM_CMD_LOAD_MODE;
   _Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
   _Command.AutoRefreshNumber      = 1;
   _Command.ModeRegisterDefinition = tmpmrd;

   //--- Send the command
   HAL_SDRAM_SendCommand(&_sdramHandle, &_Command, SDRAM_TIMEOUT);

   //--- Step 6: Set the refresh rate counter
   //--- Set the device refresh rate
   HAL_SDRAM_ProgramRefreshRate(&_sdramHandle, RefreshCount);
}

//-----------------------------------------------------------------------------
// FONCTION    : SDRAM_ReadData
//
// DESCRIPTION : Reads an amount of data from the SDRAM memory in polling mode.
//-----------------------------------------------------------------------------
UINT8 SDRAM_ReadData (UINT32 uwStartAddress, UINT32 *pData, UINT32 uwDataSize)
{
   if (HAL_SDRAM_Read_32b(&_sdramHandle, (UINT32 *)uwStartAddress, pData, uwDataSize) != HAL_OK)
   {
      return SDRAM_ERROR;
   }
   else
   {
      return SDRAM_OK;
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : SDRAM_ReadData_DMA
//
// DESCRIPTION : Reads an amount of data from the SDRAM memory in DMA mode.
//-----------------------------------------------------------------------------
UINT8 SDRAM_ReadData_DMA (UINT32 uwStartAddress, UINT32 *pData, UINT32 uwDataSize)
{
   if (HAL_SDRAM_Read_DMA(&_sdramHandle, (UINT32 *)uwStartAddress, pData, uwDataSize) != HAL_OK)
   {
      return SDRAM_ERROR;
   }
   else
   {
      return SDRAM_OK;
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : SDRAM_WriteData
//
// DESCRIPTION : Writes an amount of data to the SDRAM memory in polling mode.
//-----------------------------------------------------------------------------
UINT8 SDRAM_WriteData (UINT32 uwStartAddress, UINT32 *pData, UINT32 uwDataSize)
{
   if (HAL_SDRAM_Write_32b(&_sdramHandle, (UINT32 *)uwStartAddress, pData, uwDataSize) != HAL_OK)
   {
      return SDRAM_ERROR;
   }
   else
   {
      return SDRAM_OK;
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : SDRAM_WriteData_DMA
//
// DESCRIPTION : Writes an amount of data to the SDRAM memory in DMA mode.
//-----------------------------------------------------------------------------
UINT8 SDRAM_WriteData_DMA(UINT32 uwStartAddress, UINT32 *pData, UINT32 uwDataSize)
{
   if (HAL_SDRAM_Write_DMA(&_sdramHandle, (UINT32 *)uwStartAddress, pData, uwDataSize) != HAL_OK)
   {
      return SDRAM_ERROR;
   }
   else
   {
      return SDRAM_OK;
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : SDRAM_Sendcmd
//
// DESCRIPTION : Sends command to the SDRAM bank.
//-----------------------------------------------------------------------------
UINT8 SDRAM_Sendcmd(FMC_SDRAM_CommandTypeDef *SdramCmd)
{
   if (HAL_SDRAM_SendCommand(&_sdramHandle, SdramCmd, SDRAM_TIMEOUT) != HAL_OK)
   {
      return SDRAM_ERROR;
   }
   else
   {
      return SDRAM_OK;
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : SDRAM_MspInit
//
// DESCRIPTION : Initializes SDRAM MSP.
//-----------------------------------------------------------------------------
__weak void SDRAM_MspInit (SDRAM_HandleTypeDef  *hsdram, void *Params)
{  
   static DMA_HandleTypeDef dma_handle;
   GPIO_InitTypeDef gpio_init_structure;

   //--- Enable FMC clock
   __HAL_RCC_FMC_CLK_ENABLE();

   //--- Enable chosen DMAx clock
   __DMAx_CLK_ENABLE();

   //--- Enable GPIOs clock
   __HAL_RCC_GPIOC_CLK_ENABLE();
   __HAL_RCC_GPIOD_CLK_ENABLE();
   __HAL_RCC_GPIOE_CLK_ENABLE();
   __HAL_RCC_GPIOF_CLK_ENABLE();
   __HAL_RCC_GPIOG_CLK_ENABLE();
   __HAL_RCC_GPIOH_CLK_ENABLE();

   //--- Common GPIO configuration
   gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
   gpio_init_structure.Pull      = GPIO_PULLUP;
   gpio_init_structure.Speed     = GPIO_SPEED_FAST;
   gpio_init_structure.Alternate = GPIO_AF12_FMC;

   //--- GPIOC configuration
   gpio_init_structure.Pin   = GPIO_PIN_3;
   HAL_GPIO_Init(GPIOC, &gpio_init_structure);

   //--- GPIOD configuration
   gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 |
                               GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
   HAL_GPIO_Init(GPIOD, &gpio_init_structure);

   //--- GPIOE configuration
   gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7| GPIO_PIN_8 | GPIO_PIN_9 |\
                               GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                               GPIO_PIN_15;
   HAL_GPIO_Init(GPIOE, &gpio_init_structure);

   //--- GPIOF configuration
   gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4 |\
                               GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                               GPIO_PIN_15;
   HAL_GPIO_Init(GPIOF, &gpio_init_structure);

   //--- GPIOG configuration
   gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4| GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15;
   HAL_GPIO_Init(GPIOG, &gpio_init_structure);

   //--- GPIOH configuration
   gpio_init_structure.Pin   = GPIO_PIN_3 | GPIO_PIN_5;
   HAL_GPIO_Init(GPIOH, &gpio_init_structure);

   //--- Configure common DMA parameters
   dma_handle.Init.Channel             = SDRAM_DMAx_CHANNEL;
   dma_handle.Init.Direction           = DMA_MEMORY_TO_MEMORY;
   dma_handle.Init.PeriphInc           = DMA_PINC_ENABLE;
   dma_handle.Init.MemInc              = DMA_MINC_ENABLE;
   dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
   dma_handle.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
   dma_handle.Init.Mode                = DMA_NORMAL;
   dma_handle.Init.Priority            = DMA_PRIORITY_HIGH;
   dma_handle.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
   dma_handle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
   dma_handle.Init.MemBurst            = DMA_MBURST_SINGLE;
   dma_handle.Init.PeriphBurst         = DMA_PBURST_SINGLE;

   dma_handle.Instance = SDRAM_DMAx_STREAM;

   //--- Associate the DMA handle
   __HAL_LINKDMA(hsdram, hdma, dma_handle);

   //--- Deinitialize the stream for new transfer
   HAL_DMA_DeInit(&dma_handle);

   //--- Configure the DMA stream
   HAL_DMA_Init(&dma_handle);

   //--- NVIC configuration for DMA transfer complete interrupt
   HAL_NVIC_SetPriority(SDRAM_DMAx_IRQn, 0x0F, 0);
   HAL_NVIC_EnableIRQ(SDRAM_DMAx_IRQn);
}

//-----------------------------------------------------------------------------
// FONCTION    : SDRAM_MspDeInit
//
// DESCRIPTION : DeInitializes SDRAM MSP.
//-----------------------------------------------------------------------------
__weak void SDRAM_MspDeInit(SDRAM_HandleTypeDef  *hsdram, void *Params)
{  
   static DMA_HandleTypeDef dma_handle;

   //--- Disable NVIC configuration for DMA interrupt
   HAL_NVIC_DisableIRQ(SDRAM_DMAx_IRQn);

   //--- Deinitialize the stream for new transfer
   dma_handle.Instance = SDRAM_DMAx_STREAM;

   HAL_DMA_DeInit(&dma_handle);
}
