//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  Board.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "Board.h"
#include "LCD.h"

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
static void _SystemClockConfig   (void);
static void _GPIOInit            (void);
static void _ErrorHandler        (void);

//=============================================================================
//--- DEFINITIONS
//=============================================================================

//-----------------------------------------------------------------------------
// FONCTION    : _SystemClockConfig
//
// DESCRIPTION : System Clock Configuration
//-----------------------------------------------------------------------------
static void _SystemClockConfig (void)
{
   RCC_OscInitTypeDef RCC_OscInitStruct = {0};
   RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

   //--- Configure the main internal regulator output voltage
   __HAL_RCC_PWR_CLK_ENABLE();
   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

   //--- Initializes the RCC Oscillators according to the specified parameters in the RCC_OscInitTypeDef structure.
   RCC_OscInitStruct.OscillatorType       = RCC_OSCILLATORTYPE_HSI;
   RCC_OscInitStruct.HSIState             = RCC_HSI_ON;
   RCC_OscInitStruct.HSICalibrationValue  = RCC_HSICALIBRATION_DEFAULT;
   RCC_OscInitStruct.PLL.PLLState         = RCC_PLL_ON;
   RCC_OscInitStruct.PLL.PLLSource        = RCC_PLLSOURCE_HSI;
   RCC_OscInitStruct.PLL.PLLM             = 8;
   RCC_OscInitStruct.PLL.PLLN             = 216;
   RCC_OscInitStruct.PLL.PLLP             = RCC_PLLP_DIV2;
   RCC_OscInitStruct.PLL.PLLQ             = 2;

   if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
   {
      _ErrorHandler();
   }

   //--- Activate the Over-Drive mode
   if (HAL_PWREx_EnableOverDrive() != HAL_OK)
   {
      _ErrorHandler();
   }

   //--- Initializes the CPU, AHB and APB buses clocks
   RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
   RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
   RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

   if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
   {
      _ErrorHandler();
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : _GPIOInit
//
// DESCRIPTION : GPIO Initialization Function
//-----------------------------------------------------------------------------
static void _GPIOInit (void)
{
   GPIO_InitTypeDef GPIO_InitStruct = {0};

   //--- GPIO Ports Clock Enable
   __HAL_RCC_GPIOI_CLK_ENABLE();

   //--- LED 1
   GPIO_InitStruct.Pin     = GPIO_PIN_1;
   GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull    = GPIO_NOPULL;
   GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

   //--- BUTTON 1
   GPIO_InitStruct.Pin     = GPIO_PIN_11;
   GPIO_InitStruct.Mode    = GPIO_MODE_INPUT;
   GPIO_InitStruct.Pull    = GPIO_NOPULL;
   GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
}

//-----------------------------------------------------------------------------
// FONCTION    : _ErrorHandler
//
// DESCRIPTION : This function is executed in case of error occurrence.
//-----------------------------------------------------------------------------
static void _ErrorHandler (void)
{
   __disable_irq();
   while (1)
   {
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : BOARD_ConfAll
//
// DESCRIPTION : Board configuration
//-----------------------------------------------------------------------------
void BOARD_ConfAll (void)
{
   //--- Initialize the HAL Library
   HAL_Init();

   //--- Configure the system clock
   _SystemClockConfig();

   //--- Initialize all configured peripherals
   _GPIOInit();

   //--- LCD Initialization
   LCD_Init();
}
