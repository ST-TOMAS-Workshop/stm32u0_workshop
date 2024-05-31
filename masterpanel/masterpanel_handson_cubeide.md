----!
Presentation
----!

# CubeIDE
- Open **CubeIDE** and related `MasterPanel` project

![image](./img/open_project.png)

# Initialization
## Variables
Copy paste following snippet in `USER CODE BEGIN PV` section in **main.c** file:

```c
uint32_t SRC_Buffer_LED_PWM[4]  = {20, 40, 60, 0}; // define Low pulse width ~1ms, 2ms, 3ms and IDLE state
```

# IPs start
This section manage complete user code structure. MCU is runing at @1MHz Sysclock so LP Run and LP Sleep are implemented.
DMA configuration is just about set source address (buffer with pulses) and destination address (Capture/Compare register). After buttton press Capture/Compare register is updated with new value.

XXSMEN registers are cleared reduce further consumption in Sleep mode when internal busses are still clocked. Peripheries are not active as their master xxEN bits are reset. However, extra 10uA/1MHz is reduced deu to limiting clock distibution within device. By default xxSMEN are set after reset.

Before entering in SLeep/LP Sleep need to suspend Systick timer otherwise MCU wakes up imidiately within 1ms.
<p> </p>

Copy paste following snippet in `USER CODE BEGIN 2` section in **main.c** file:

```c
	 /* Enable LP run mode */
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWREx_EnableLowPowerRunMode(); //we can do it as Sysclock <2MHz and enter in Sleep mode

    /*Drive LED and Generate Pulses*/
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

    /*Enable DMAMUX*/
    HAL_DMAEx_EnableMuxRequestGenerator(&hdma_dma_generator0);

    /*Link PWM buffer and TIM2 capture/compare register through DMA_Ch1*/
    HAL_DMA_Start(&hdma_dma_generator0, (uint32_t)SRC_Buffer_LED_PWM, (uint32_t)&TIM2->CCR1, 4);

    /* Keep live DMA, SRAM, TIM2 and Ports only during SLEEP mode ~ 10uA/1MHz reduction*/
    WRITE_REG(RCC->AHBSMENR, RCC_AHBSMENR_DMA1SMEN|RCC_AHBSMENR_SRAM1SMEN);  	// DMA, SRAM
    WRITE_REG(RCC->IOPSMENR, RCC_IOPSMENR_GPIOASMEN|RCC_IOPSMENR_GPIOCSMEN); 	// GPIOA, GPIOC
    WRITE_REG(RCC->APBSMENR1, RCC_APBSMENR1_TIM2SMEN);						// TIM2
    CLEAR_REG(RCC->APBSMENR2);

    /*Enter in LP SLEEP mode*/
    HAL_SuspendTick();
    HAL_PWREx_EnableFlashPowerDown(PWR_FLASHPD_LPSLEEP);
    HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    HAL_ResumeTick();
```

