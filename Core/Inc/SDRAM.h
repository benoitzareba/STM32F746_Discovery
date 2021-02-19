//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// HEADER      :  SDRAM.h
//
//=============================================================================
#ifndef _SDRAM_H
#define _SDRAM_H

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
#define SDRAM_OK                                   ((UINT8)0x00)
#define SDRAM_ERROR                                ((UINT8)0x01)

#define SDRAM_DEVICE_ADDR                          ((UINT32)0xC0000000)

//--- SDRAM device size in MBytes
#define SDRAM_DEVICE_SIZE                          ((UINT32)0x800000)

#define SDRAM_MEMORY_WIDTH                         FMC_SDRAM_MEM_BUS_WIDTH_16

#define SDCLOCK_PERIOD                             FMC_SDRAM_CLOCK_PERIOD_2

//--- SDRAM refresh counter (100Mhz SD clock)
#define REFRESH_COUNT                              ((UINT32)0x0603)

#define SDRAM_TIMEOUT                              ((UINT32)0xFFFF)

//--- DMA definitions for SDRAM DMA transfer
#define __DMAx_CLK_ENABLE                          __HAL_RCC_DMA2_CLK_ENABLE
#define __DMAx_CLK_DISABLE                         __HAL_RCC_DMA2_CLK_DISABLE
#define SDRAM_DMAx_CHANNEL                         DMA_CHANNEL_0
#define SDRAM_DMAx_STREAM                          DMA2_Stream0
#define SDRAM_DMAx_IRQn                            DMA2_Stream0_IRQn
#define BSP_SDRAM_DMA_IRQHandler                   DMA2_Stream0_IRQHandler

//--- FMC SDRAM Mode definition register defines
#define SDRAM_MODEREG_BURST_LENGTH_1               ((UINT16)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2               ((UINT16)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4               ((UINT16)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8               ((UINT16)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL        ((UINT16)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED       ((UINT16)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2                ((UINT16)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3                ((UINT16)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD      ((UINT16)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED   ((UINT16)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE       ((UINT16)0x0200)


//-----------------------------------------------------------------------------
// Structures and types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------

//---------- Functions ----------
UINT8 SDRAM_Init                    (void);
UINT8 SDRAM_DeInit                  (void);
void  SDRAM_Initialization_sequence (UINT32 RefreshCount);
UINT8 SDRAM_ReadData                (UINT32 uwStartAddress, UINT32 *pData, UINT32 uwDataSize);
UINT8 SDRAM_ReadData_DMA            (UINT32 uwStartAddress, UINT32 *pData, UINT32 uwDataSize);
UINT8 SDRAM_WriteData               (UINT32 uwStartAddress, UINT32 *pData, UINT32 uwDataSize);
UINT8 SDRAM_WriteData_DMA           (UINT32 uwStartAddress, UINT32 *pData, UINT32 uwDataSize);
UINT8 SDRAM_Sendcmd                 (FMC_SDRAM_CommandTypeDef *SdramCmd);
void  SDRAM_MspInit                 (SDRAM_HandleTypeDef  *hsdram, void *Params);
void  SDRAM_MspDeInit               (SDRAM_HandleTypeDef  *hsdram, void *Params);

#endif

