/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32u0xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32u0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern LPTIM_HandleTypeDef hlptim1;
extern RTC_HandleTypeDef hrtc;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SVC instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32U0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32u0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles RTC and TAMP interrupts (combined EXTI lines 20 & 21).
  */
void RTC_TAMP_IRQHandler(void)
{
  /* USER CODE BEGIN RTC_TAMP_IRQn 0 */

  /* USER CODE END RTC_TAMP_IRQn 0 */
  //HAL_RTCEx_WakeUpTimerIRQHandler(&hrtc);
  /* USER CODE BEGIN RTC_TAMP_IRQn 1 */
  if (LL_RTC_IsActiveFlag_WUT(RTC) != 0)
      {
              /* Clear the Alarm interrupt pending bit */
              LL_RTC_ClearFlag_WUT(RTC);

              /* Clear the EXTI's Flag for RTC*/
              LL_EXTI_ClearRisingFlag_0_31(LL_EXTI_LINE_28);

          WakeUp_Callback();
      }
  /* USER CODE END RTC_TAMP_IRQn 1 */
}

/**
  * @brief This function handles TIM6, DAC and LPTIM1 global Interrupts (combined with EXTI 31).
  */
void TIM6_DAC_LPTIM1_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_LPTIM1_IRQn 0 */

  /* USER CODE END TIM6_DAC_LPTIM1_IRQn 0 */
 // HAL_LPTIM_IRQHandler(&hlptim1);
  /* USER CODE BEGIN TIM6_DAC_LPTIM1_IRQn 1 */
  /* Check CC1 interrupt from falling edge*/
     if(LL_LPTIM_IsActiveFlag_CC1(LPTIM1) == 1)
     {
         /* Clear the update interrupt flag*/
         LL_LPTIM_ClearFlag_CC1(LPTIM1);

         /* TIM1 capture/compare interrupt processing(function defined in main.c) */
         CaptureFalling_Callback();
      }
     /* Check CC1 interrupt from falling edge */
     else if (LL_LPTIM_IsActiveFlag_CC3(LPTIM1) == 1)
     {
         /* Clear the update interrupt flag*/
         LL_LPTIM_ClearFlag_CC3(LPTIM1);

         /* TIM1 capture/compare interrupt processing(function defined in main.c) */
         CaptureRising_Callback();
      }
     /* Clear the EXTI's Flag for LPTIM1*/
     LL_EXTI_ClearRisingFlag_0_31(LL_EXTI_LINE_24);
  /* USER CODE END TIM6_DAC_LPTIM1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
