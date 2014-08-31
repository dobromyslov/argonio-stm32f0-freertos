#include "stm32f0xx.h"
#include <string.h>

/**
 * UART1 bus.
 *
 * PA9      USART1_TX
 * PA10     USART1_RX
 */
UART_HandleTypeDef huart1;
__IO ITStatus UartReady = RESET;

__attribute__( ( always_inline ) ) __STATIC_INLINE void SystemClock_Config(void);
__attribute__( ( always_inline ) ) __STATIC_INLINE void MX_GPIO_Init(void);
__attribute__( ( always_inline ) ) __STATIC_INLINE void MX_UART1_Init(void);

static void Console_log(char *message);

int main(void) {

    // Reset of all peripherals, Initializes the Flash interface and the Systick
    HAL_Init();

    // Configure the system clock
    SystemClock_Config();

    // System interrupt init
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

    // Initialize all configured peripherals
    MX_GPIO_Init();

    // Init UART bus
    MX_UART1_Init();
    Console_log("\r\nUART1 initialized");
    
    while(1) {
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
      HAL_Delay(500);
    }

    return 0;
}

void Error_Handler(void) {

}

/**
 * System Clock Configuration
 */
__attribute__( ( always_inline ) ) __STATIC_INLINE void SystemClock_Config(void) {
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

  __SYSCFG_CLK_ENABLE();
}

/**
 * Configure pins as
 * Analog
 * Input
 * Output
 * EVENT_OUT
 * EXTI
 */
__attribute__( ( always_inline ) ) __STATIC_INLINE void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();

    // DMA Clock Enable
    __DMA1_CLK_ENABLE();

    /*Configure GPIO pin : PA0 */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : PC8 PC9 */
    /**
     * PC8 - LD4 [blue led]
     * PC9 - LD3 [green led]
     */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/**
 * UART bus init.
 */
__attribute__( ( always_inline ) ) __STATIC_INLINE void MX_UART1_Init(void) {
    huart1.Instance            = USART1;
    huart1.Init.BaudRate       = 115200;
    huart1.Init.WordLength     = UART_WORDLENGTH_8B;
    huart1.Init.StopBits       = UART_STOPBITS_1;
    huart1.Init.Parity         = UART_PARITY_NONE;
    huart1.Init.Mode           = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl      = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling   = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED ;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();
    }
}

static void Console_log(char *message) {

    if (HAL_UART_Transmit_DMA(&huart1, (uint8_t*)message, strlen(message)) != HAL_OK) {
        Error_Handler();
    }
    while (UartReady != SET) {
        // do nothing
    }
    UartReady = RESET;


    if (HAL_UART_Transmit_DMA(&huart1, (uint8_t*)"\r\n", 2) != HAL_OK) {
        Error_Handler();
    }
    while (UartReady != SET) {
        // do nothing
    }
    UartReady = RESET;
}

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle.
  * @note   This example shows a simple way to report end of DMA Tx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set transmission flag: trasfer complete*/
  UartReady = SET;
}


/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set transmission flag: trasfer complete*/
  UartReady = SET;
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle) {

    Error_Handler();
}

/***************************************************************************************************
 * Interrupt handlers.
 **************************************************************************************************/

extern void xPortSysTickHandler(void);

void SysTick_Handler(void) {
  HAL_IncTick();
}

/**
  * USART1 RX/TX DMA interrupt request handler.
  *
  * @Note   This function is redefined in "main.h" and related to DMA
  *         used for USART data transmission
  */
void DMA1_Channel2_3_IRQHandler(void) {
  HAL_DMA_IRQHandler(huart1.hdmatx);
  HAL_DMA_IRQHandler(huart1.hdmarx);
}
