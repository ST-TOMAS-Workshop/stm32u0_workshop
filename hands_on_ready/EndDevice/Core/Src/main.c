/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
COMP_HandleTypeDef hcomp1;

LPTIM_HandleTypeDef hlptim1;

RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN PV */
uint32_t newValueRise = 0;
uint32_t negDutyCycle = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_COMP1_Init(void);
static void MX_LPTIM1_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */
void Convert_IntegerIntoChar(uint32_t number, uint16_t *p_tab);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_BACKUPRESET_FORCE();
  while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY));
  __HAL_RCC_BACKUPRESET_RELEASE();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_COMP1_Init();
  MX_LPTIM1_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  /* Initialization of LCD module */
  BSP_LCD_GLASS_Init();
  BSP_LCD_GLASS_Clear();

  /*Enable LPRun mode, as Sysclock <2MHz and enter in Stop1 mode*/
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_EnableLowPowerRunMode();

  /*Launch PWM Measure*/
  HAL_COMP_Start(&hcomp1);
  HAL_LPTIM_IC_Start_IT(&hlptim1, LPTIM_CHANNEL_1);
  HAL_LPTIM_IC_Start_IT(&hlptim1, LPTIM_CHANNEL_3);

  /*Show welcome word*/
  BSP_LCD_GLASS_ScrollSentence((uint8_t *)"       waiting for signal and go to STOP mode ", 1 , 149);
  BSP_LCD_GLASS_Clear();

  /* Reduce Contrast  after scrolling message -> lower consumption
   *  VLCD set to minimum with good contrast, disable High drive and PulseOn duration to minimum*/
  MODIFY_REG(LCD->FCR, (LCD_FCR_PON | LCD_FCR_CC | LCD_FCR_HD), ( LCD_PULSEONDURATION_1 | LCD_CONTRASTLEVEL_2 | LCD_HIGHDRIVE_DISABLE));
  while (READ_BIT(LCD->SR, LCD_SR_FCRSR) == 0);

  BSP_LCD_GLASS_DisplayString((uint8_t *)"IDLE");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  /*Enter in STOPx mode*/
	  HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief COMP1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_COMP1_Init(void)
{

  /* USER CODE BEGIN COMP1_Init 0 */

  /* USER CODE END COMP1_Init 0 */

  /* USER CODE BEGIN COMP1_Init 1 */

  /* USER CODE END COMP1_Init 1 */
  hcomp1.Instance = COMP1;
  hcomp1.Init.InputPlus = COMP_INPUT_PLUS_IO3;
  hcomp1.Init.InputMinus = COMP_INPUT_MINUS_VREFINT;
  hcomp1.Init.OutputPol = COMP_OUTPUTPOL_NONINVERTED;
  hcomp1.Init.WindowOutput = COMP_WINDOWOUTPUT_EACH_COMP;
  hcomp1.Init.Hysteresis = COMP_HYSTERESIS_NONE;
  hcomp1.Init.BlankingSrce = COMP_BLANKINGSRC_NONE;
  hcomp1.Init.Mode = COMP_POWERMODE_ULTRALOWPOWER;
  hcomp1.Init.WindowMode = COMP_WINDOWMODE_DISABLE;
  hcomp1.Init.TriggerMode = COMP_TRIGGERMODE_NONE;
  if (HAL_COMP_Init(&hcomp1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN COMP1_Init 2 */

  /* USER CODE END COMP1_Init 2 */

}

/**
  * @brief LPTIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPTIM1_Init(void)
{

  /* USER CODE BEGIN LPTIM1_Init 0 */

  /* USER CODE END LPTIM1_Init 0 */

  LPTIM_IC_ConfigTypeDef sConfig = {0};

  /* USER CODE BEGIN LPTIM1_Init 1 */

  /* USER CODE END LPTIM1_Init 1 */
  hlptim1.Instance = LPTIM1;
  hlptim1.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  hlptim1.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
  hlptim1.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
  hlptim1.Init.Period = 65535;
  hlptim1.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
  hlptim1.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
  hlptim1.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
  hlptim1.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
  hlptim1.Init.RepetitionCounter = 0;
  if (HAL_LPTIM_Init(&hlptim1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfig.ICInputSource = LPTIM_IC1SOURCE_COMP1;
  sConfig.ICPrescaler = LPTIM_ICPSC_DIV1;
  sConfig.ICPolarity = LPTIM_ICPOLARITY_FALLING;
  sConfig.ICFilter = LPTIM_ICFLT_CLOCK_DIV1;
  if (HAL_LPTIM_IC_ConfigChannel(&hlptim1, &sConfig, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfig.ICInputSource = LPTIM_IC3SOURCE_COMP1;
  sConfig.ICPolarity = LPTIM_ICPOLARITY_RISING;
  if (HAL_LPTIM_IC_ConfigChannel(&hlptim1, &sConfig, LPTIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPTIM1_Init 2 */

  /* USER CODE END LPTIM1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  hrtc.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
  hrtc.Init.BinMode = RTC_BINARY_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the WakeUp
  */
  if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0, RTC_WAKEUPCLOCK_CK_SPRE_16BITS, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */
  HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/*ISR for Falling edges from CH1_LPTIM1 and calculate duty cycle of negative pulse
 *
 * ____IDLE_____negDC_______
 *                |    |        |    |
 *                 |___|        |___|
 *             oldVF  newVR  newVF
 * */
void CaptureFalling_Callback(void)
{
    static uint32_t oldValueFall = 0;
    static uint32_t newValueFall = 0;
    static uint32_t DiffValueFall = 0;

    oldValueFall = newValueFall;

    newValueFall = LL_LPTIM_IC_GetCaptureCH1(LPTIM1);
    /* Capture computation */
    if (newValueFall > oldValueFall && newValueRise > oldValueFall )
    {
        DiffValueFall = (newValueFall - oldValueFall);
        negDutyCycle = (newValueRise - oldValueFall) * 100 / DiffValueFall;
        /*Enable RTC WakeUp*/
        /* Configure the Interrupt in the RTC_CR register and Enable the Wakeup Timer*/
        LL_RTC_EnableIT_WUT(RTC);
        LL_RTC_WAKEUP_Enable(RTC);
    }
    else
    {
        /*
         * For simplicity rest combination like "newValueFall < oldValueFall".... are NOT handled
         * DiffValueFall = ((LPTIM1_ARR_MAX - oldValueFall) + newValueFall) + 1;
        */
    }
}
  /*ISR for Rising edges from CH3_LPTIM1 and calculate duty cycle of negative pulse*/
void CaptureRising_Callback(void)
{
    newValueRise = LL_LPTIM_IC_GetCaptureCH3(LPTIM1);
}

/*ISR from 1s periodic timer*/
void WakeUp_Callback(void)
{
    static uint32_t actValueRise = 0;
    static uint32_t preValueRise = 0;
    static uint16_t tab[6];
    /*Time out for IDLE state on bus*/
    preValueRise = actValueRise;
    actValueRise = LL_LPTIM_IC_GetCaptureCH3(LPTIM1);

    /*1s TimeOut for IDLE state on bus -> display Measured duty cycle or "IDLE*/
    if(preValueRise != actValueRise)
    {
        Convert_IntegerIntoChar(negDutyCycle, (uint16_t *)tab);
        tab[0] = 0x44; //"D"
        tab[1] = 0x75; //"u"
        tab[2] = 0x43; //"C"
        tab[2] |= DOUBLE_DOT ; //add double dot
        tab[3] = 0x30; //"0"
    }
    else
    {
        /*no captured edges on bus ->Display IDLE state*/
        tab[0] = 0x49; //"I"
        tab[1] = 0x44; //"D"
        tab[2] = 0x4C; //"L"
        tab[3] = 0x45; //"E"
        tab[4] = ' ';  //' '
        tab[5] = ' ';  //' '

        /* reset DutyCycle and enter in Stop mode*/
        negDutyCycle = 0;

        /*Disable RTC WakeUp Interrupt*/
        LL_RTC_DisableIT_WUT(RTC);
        LL_RTC_WAKEUP_Disable(RTC);
        }
    /*Display processed signal*/
    BSP_LCD_GLASS_Clear();
    BSP_LCD_GLASS_DisplayStrDeci(tab);

}
/*Simplified for Two Digits only, rest is kept as a reference*/
void  Convert_IntegerIntoChar(uint32_t number, uint16_t *p_tab)
{
  uint16_t units=0, tens=0; // hundreds=0, thousands=0, tenthousand=0, hundredthousand=0;

  uint32_t number1 = (uint32_t) number;

  units = number1%10;
  tens = ((number1-units)/10)%10;

  /* Full Integer converts as follows*/
  /*units = ((((number1%100000)%10000)%1000)%100)%10;
  tens = (((((number1-units)/10)%10000)%1000)%100)%10;
  hundreds = ((((number1-tens-units)/100)%1000)%100)%10;
  thousands = (((number1-hundreds-tens-units)/1000)%100)%10;
  tenthousand = ((number1-thousands-hundreds-tens-units)/10000)%10;
  hundredthousand = ((number1-tenthousand-thousands-hundreds-tens-units)/100000);*/

  *(p_tab+5) = units + 0x30;
  *(p_tab+4) = tens + 0x30;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
