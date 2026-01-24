/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "spi.h"

/* USER CODE BEGIN 0 */
#include "LCD.h"
/* USER CODE END 0 */

SPI_HandleTypeDef hspi1;
/* SPI1_TX DMA句柄 */
DMA_HandleTypeDef hdma_spi1_tx;
///* DMA传输完成信号量（RTOS同步用） */
//SemaphoreHandle_t spi1_dma_sem = NULL;
//
///**
//  * @brief SPI1 MSP初始化（底层硬件配置：GPIO、DMA、NVIC）
//  * @note HAL_SPI_Init会自动调用此函数
//  */
//void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  if(hspi->Instance==SPI1)
//  {
//    /* 1. 使能外设时钟 */
//    __HAL_RCC_SPI1_CLK_ENABLE();
//    __HAL_RCC_GPIOA_CLK_ENABLE();
//    __HAL_RCC_GPIOB_CLK_ENABLE();
//    __HAL_RCC_DMA2_CLK_ENABLE(); // DMA2时钟使能
//
//    /* 2. SPI1引脚配置（PA4=NSS、PA5=SCK、PA7=MOSI） */
//    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;    // 复用推挽输出
//    GPIO_InitStruct.Pull = GPIO_NOPULL;        // 无上下拉
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // 最高速度
//    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1; // 复用功能映射
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//    /* LCD控制引脚配置（PB0=RST、PB1=DC、PB14=BL，根据实际功能调整） */
//    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_14;
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//    /* 3. SPI1_TX DMA配置 */
//    hdma_spi1_tx.Instance = DMA2_Stream3;                // SPI1_TX对应DMA2 Stream3
//    hdma_spi1_tx.Init.Channel = DMA_CHANNEL_3;           // 通道3
//    hdma_spi1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;   // 内存→外设（发送）
//    hdma_spi1_tx.Init.PeriphInc = DMA_PINC_DISABLE;       // 外设地址不增量（SPI数据寄存器固定）
//    hdma_spi1_tx.Init.MemInc = DMA_MINC_ENABLE;           // 内存地址增量（缓冲区连续）
//    hdma_spi1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; // 8位数据
//    hdma_spi1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     // 8位数据
//    hdma_spi1_tx.Init.Mode = DMA_NORMAL;                 // 正常模式（非循环）
//    hdma_spi1_tx.Init.Priority = DMA_PRIORITY_HIGH;       // 高优先级（保证LCD传输优先）
//    hdma_spi1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//    if (HAL_DMA_Init(&hdma_spi1_tx) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    /* 关联SPI和DMA句柄 */
//    __HAL_LINKDMA(hspi, hdmatx, hdma_spi1_tx);
//
//    /* 4. DMA中断配置（RTOS下优先级需低于内核） */
//    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 5, 0); // 优先级5（根据RTOS调整）
//    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
//
//    /* 5. 创建DMA传输完成信号量（RTOS同步） */
//    spi1_dma_sem = xSemaphoreCreateBinary();
//    if(spi1_dma_sem == NULL)
//    {
//      Error_Handler(); // 信号量创建失败
//    }
//  }
//}
//
///**
//  * @brief SPI1 MSP反初始化
//  */
//void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
//{
//  if(hspi->Instance==SPI1)
//  {
//    __HAL_RCC_SPI1_CLK_DISABLE();
//    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7);
//    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_14);
//    HAL_DMA_DeInit(hspi->hdmatx);
//    vSemaphoreDelete(spi1_dma_sem); // 释放信号量
//  }
//}
//
///**
//  * @brief DMA2 Stream3中断服务函数
//  */
//void DMA2_Stream3_IRQHandler(void)
//{
//  HAL_DMA_IRQHandler(&hdma_spi1_tx);
//}
//
///**
//  * @brief SPI DMA发送完成回调函数（RTOS安全版）
//  */
//void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
//{
//  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//  if(hspi->Instance == SPI1)
//  {
//    // 释放二进制信号量，通知传输完成（中断内用FromISR版本）
//    xSemaphoreGiveFromISR(spi1_dma_sem, &xHigherPriorityTaskWoken);
//    portYIELD_FROM_ISR(xHigherPriorityTaskWoken); // 触发任务调度
//  }
//}
//
///**
//  * @brief LCD SPI DMA发送数据
//  * @param pData: 数据缓冲区
//  * @param len: 数据长度
//  * @retval HAL状态（OK/超时/错误）
//  */
//HAL_StatusTypeDef lcd_spi_send_data_dma(uint8_t *pData, uint16_t len)
//{
//  // 等待前一次DMA传输完成（防止数据叠加，超时100ms）
//  if(xSemaphoreTake(spi1_dma_sem, pdMS_TO_TICKS(100)) == pdFALSE)
//  {
//    return HAL_TIMEOUT;
//  }
//
//  // 启动DMA非阻塞传输
//  if(HAL_SPI_Transmit_DMA(&hspi1, pData, len) != HAL_OK)
//  {
//    return HAL_ERROR;
//  }
//
//  return HAL_OK;
//}
//
///**
//  * @brief 等待DMA传输完成
//  * @param timeout: 超时时间（ms）
//  * @retval true-完成，false-超时
//  */
//bool lcd_spi_wait_dma_complete(uint32_t timeout)
//{
//  if(xSemaphoreTake(spi1_dma_sem, pdMS_TO_TICKS(timeout)) == pdTRUE)
//  {
//    // 释放信号量，为下一次传输做准备
//    xSemaphoreGive(spi1_dma_sem);
//    return true;
//  }
//  return false;
//}









/* SPI1 init function */
void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspInit 0 */

  /* USER CODE END SPI1_MspInit 0 */
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = LCD_SCL_Pin|LCD_SDA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* SPI1 DMA Init */
    /* SPI1_TX Init */
    hdma_spi1_tx.Instance = DMA2_Stream5;
    hdma_spi1_tx.Init.Channel = DMA_CHANNEL_3;
    hdma_spi1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi1_tx.Init.Mode = DMA_NORMAL;
    hdma_spi1_tx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_spi1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_spi1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(spiHandle,hdmatx,hdma_spi1_tx);

  /* USER CODE BEGIN SPI1_MspInit 1 */

  /* USER CODE END SPI1_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspDeInit 0 */

  /* USER CODE END SPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();

    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA7     ------> SPI1_MOSI
    */
    HAL_GPIO_DeInit(GPIOA, LCD_SCL_Pin|LCD_SDA_Pin);

    /* SPI1 DMA DeInit */
    HAL_DMA_DeInit(spiHandle->hdmatx);
  /* USER CODE BEGIN SPI1_MspDeInit 1 */

  /* USER CODE END SPI1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi){
	if (hspi->Instance == SPI1) {
        LCD_DMA_TX_Callback(); // 调用LCD的DMA完成回调，设置标志
    }
}


void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance == SPI1) {
        LCD_DMA_TX_Callback(); // 调用LCD的DMA完成回调，设置标志
    }
}
/* USER CODE END 1 */
