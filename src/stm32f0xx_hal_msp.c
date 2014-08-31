/**
 * stm32f0xx_hal_msp.c
 *
 * MSP Initialization and de-Initialization codes for STM32 HAL.
 */

#include "stm32f0xx.h"

/**
 * USART1
 *
 * PA9      USART1_TX
 * PA10     USART1_RX
 */
#define USART1_GPIOx    GPIOA
#define USART1_TX       GPIO_PIN_9
#define USART1_RX       GPIO_PIN_10

/**
 * UART Init.
 */
void HAL_UART_MspInit(UART_HandleTypeDef* huart) {

    static DMA_HandleTypeDef hdma_tx;
    static DMA_HandleTypeDef hdma_rx;


    GPIO_InitTypeDef GPIO_InitStruct;
    if (huart->Instance == USART1) {
        // Peripheral clock enable
        __USART1_CLK_ENABLE();

        // USART1 GPIO Configuration
        GPIO_InitStruct.Pin       = USART1_RX | USART1_TX;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
        HAL_GPIO_Init(USART1_GPIOx, &GPIO_InitStruct);

        // TX DMA config
        hdma_tx.Instance                 = DMA1_Channel2;
        hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_tx.Init.Mode                = DMA_NORMAL;
        hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
        HAL_DMA_Init(&hdma_tx);
        __HAL_LINKDMA(huart, hdmatx, hdma_tx);

        // RX DMA config
        hdma_rx.Instance                 = DMA1_Channel3;
        hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_rx.Init.Mode                = DMA_NORMAL;
        hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
        HAL_DMA_Init(&hdma_rx);
        __HAL_LINKDMA(huart, hdmarx, hdma_rx);

        // TX DMA Transfer complete interrupts
        HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 1);
        HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

        // RX DMA Transfer complete interrupts
        HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
    }
}

/**
 * UART DeInit.
 */
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart) {

    if (huart->Instance == USART1) {
        // Peripheral clock disable
        __USART1_CLK_DISABLE();

        // USART1 GPIO Configuration
        HAL_GPIO_DeInit(USART1_GPIOx, USART1_RX | USART1_TX);

        // Disable DMA
        if (huart->hdmarx != 0) {
            HAL_DMA_DeInit(huart->hdmarx);
        }

        if (huart->hdmatx != 0) {
            HAL_DMA_DeInit(huart->hdmatx);
        }

        HAL_NVIC_DisableIRQ(DMA1_Channel2_3_IRQn);
        HAL_NVIC_DisableIRQ(DMA1_Channel2_3_IRQn);
    }
}
