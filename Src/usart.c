/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

#include "gpio.h"
#include "dma.h"

/* USER CODE BEGIN 0 */
#include "common.h"
/* USER CODE END 0 */

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_uart4_rx;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart3_rx;

/* UART4 init function */
void MX_UART4_Init(void)
{

  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}
/* UART5 init function */
void MX_UART5_Init(void)
{

  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}
/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(uartHandle->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspInit 0 */

  /* USER CODE END UART4_MspInit 0 */
    /* UART4 clock enable */
    __HAL_RCC_UART4_CLK_ENABLE();
  
    /**UART4 GPIO Configuration    
    PC10     ------> UART4_TX
    PC11     ------> UART4_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* UART4 DMA Init */
    /* UART4_RX Init */
    hdma_uart4_rx.Instance = DMA2_Channel3;
    hdma_uart4_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_uart4_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart4_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart4_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart4_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart4_rx.Init.Mode = DMA_NORMAL;
    hdma_uart4_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_uart4_rx) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_uart4_rx);

    /* UART4 interrupt Init */
    HAL_NVIC_SetPriority(UART4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspInit 1 */

  /* USER CODE END UART4_MspInit 1 */
  }
  else if(uartHandle->Instance==UART5)
  {
  /* USER CODE BEGIN UART5_MspInit 0 */

  /* USER CODE END UART5_MspInit 0 */
    /* UART5 clock enable */
    __HAL_RCC_UART5_CLK_ENABLE();
  
    /**UART5 GPIO Configuration    
    PC12     ------> UART5_TX
    PD2     ------> UART5_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* UART5 interrupt Init */
    HAL_NVIC_SetPriority(UART5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(UART5_IRQn);
  /* USER CODE BEGIN UART5_MspInit 1 */

  /* USER CODE END UART5_MspInit 1 */
  }
  else if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel5;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_MEDIUM;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();
  
    /**USART2 GPIO Configuration    
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_USART2_ENABLE();

    /* USART2 DMA Init */
    /* USART2_RX Init */
    hdma_usart2_rx.Instance = DMA1_Channel6;
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_rx.Init.Mode = DMA_NORMAL;
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_MEDIUM;
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart2_rx);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();
  
    /**USART3 GPIO Configuration    
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART3 DMA Init */
    /* USART3_RX Init */
    hdma_usart3_rx.Instance = DMA1_Channel3;
    hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_rx.Init.Mode = DMA_NORMAL;
    hdma_usart3_rx.Init.Priority = DMA_PRIORITY_MEDIUM;
    if (HAL_DMA_Init(&hdma_usart3_rx) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart3_rx);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspDeInit 0 */

  /* USER CODE END UART4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART4_CLK_DISABLE();
  
    /**UART4 GPIO Configuration    
    PC10     ------> UART4_TX
    PC11     ------> UART4_RX 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10|GPIO_PIN_11);

    /* UART4 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* UART4 interrupt Deinit */
    HAL_NVIC_DisableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspDeInit 1 */

  /* USER CODE END UART4_MspDeInit 1 */
  }
  else if(uartHandle->Instance==UART5)
  {
  /* USER CODE BEGIN UART5_MspDeInit 0 */

  /* USER CODE END UART5_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART5_CLK_DISABLE();
  
    /**UART5 GPIO Configuration    
    PC12     ------> UART5_TX
    PD2     ------> UART5_RX 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_12);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);

    /* UART5 interrupt Deinit */
    HAL_NVIC_DisableIRQ(UART5_IRQn);
  /* USER CODE BEGIN UART5_MspDeInit 1 */

  /* USER CODE END UART5_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();
  
    /**USART2 GPIO Configuration    
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX 
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_5|GPIO_PIN_6);

    /* USART2 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();
  
    /**USART3 GPIO Configuration    
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

    /* USART3 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
/**
  * @brief  Print a string on the HyperTerminal
  * @param  p_string: The string to be printed
  * @retval None
  */
void Serial_PutString(UART_HandleTypeDef *huart, uint8_t *p_string)
{
	uint16_t length = 0;

	while (p_string[length] != '\0')
	{
		length++;
	}
	HAL_UART_Transmit(huart, p_string, length, TX_TIMEOUT);
}

/**
  * @brief  Transmit a byte to the HyperTerminal
  * @param  param The byte to be sent
  * @retval HAL_StatusTypeDef HAL_OK if OK
  */
HAL_StatusTypeDef Serial_PutByte(UART_HandleTypeDef *huart, uint8_t param)
{
	/* May be timeouted... */
	if (huart->gState == HAL_UART_STATE_TIMEOUT)
	{
		huart->gState = HAL_UART_STATE_READY;
	}
	return HAL_UART_Transmit(huart, &param, 1, TX_TIMEOUT);
}
/****************** UART CallBack *******************/
/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
		Usart1Buf.RxEndFlag[Usart1Buf.RxDimension] = SET;
		Usart1Buf.RxEndIndex[Usart1Buf.RxDimension] = USART1_BUF_LENGTH - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
		Usart1Buf.RxDimension++;
		Usart1Buf.RxDimension %= USART1_BUF_DIMENSION;
		HAL_UART_DMAStop(huart);
		HAL_UART_Receive_DMA(huart, Usart1Buf.RxBuffer[Usart1Buf.RxDimension], USART1_BUF_LENGTH);
	}
	else if (huart->Instance == USART2)
	{
		Usart2Buf.RxEndFlag[Usart2Buf.RxDimension] = SET;
		Usart2Buf.RxEndIndex[Usart2Buf.RxDimension] = USART2_BUF_LENGTH - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);
		Usart2Buf.RxDimension++;
		Usart2Buf.RxDimension %= USART2_BUF_DIMENSION;
		HAL_UART_DMAStop(huart);
		HAL_UART_Receive_DMA(huart, Usart2Buf.RxBuffer[Usart2Buf.RxDimension], USART2_BUF_LENGTH);
	}
	else if (huart->Instance == USART3)
	{
		Usart3Buf.RxEndIndex[Usart3Buf.RxDimension] = USART3_BUF_LENGTH - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);
		Usart3Buf.RxEndFlag[Usart3Buf.RxDimension] = SET;
		Usart3Buf.RxDimension++;
		Usart3Buf.RxDimension %= USART3_BUF_DIMENSION;
		HAL_UART_DMAStop(huart);
		HAL_UART_Receive_DMA(huart, Usart3Buf.RxBuffer[Usart3Buf.RxDimension], USART3_BUF_LENGTH);
	}
	else if (huart->Instance == UART4)
	{
		Usart4Buf.RxEndIndex[Usart4Buf.RxDimension] = USART4_BUF_LENGTH - __HAL_DMA_GET_COUNTER(&hdma_uart4_rx);
		Usart4Buf.RxEndFlag[Usart4Buf.RxDimension] = SET;
		Usart4Buf.RxDimension++;
		Usart4Buf.RxDimension %= USART4_BUF_DIMENSION;
		HAL_UART_DMAStop(huart);
		HAL_UART_Receive_DMA(huart, Usart4Buf.RxBuffer[Usart4Buf.RxDimension], USART4_BUF_LENGTH);
	}
	else if (huart->Instance == UART5)
	{
		Usart5Buf.RxEndIndex[Usart5Buf.RxDimension] = USART5_BUF_LENGTH;
		Usart5Buf.RxEndFlag[Usart5Buf.RxDimension] = SET;
		Usart5Buf.RxDimension++;
		Usart5Buf.RxDimension %= USART5_BUF_DIMENSION;
		HAL_UART_Receive_IT(huart, Usart5Buf.RxBuffer[Usart5Buf.RxDimension], USART5_BUF_LENGTH);
	}
}

/**
  * @brief DMA IDLE接收串口数据
  * @param handle to the UART
  * @retval void 
  **/
void HW_UART_Modem_IRQHandler(UART_HandleTypeDef *huart)
{
	uint32_t tmp_flag = 0, tmp_it_source = 0;
	
	if (huart->Instance == USART1)
	{
		tmp_flag = __HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE);
		tmp_it_source = __HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_IDLE);  
		/* UART parity error interrupt occurred ------------------------------------*/
		if ((tmp_flag != RESET) && (tmp_it_source != RESET))
		{
			Usart2Buf.RxEndIndex[Usart1Buf.RxDimension] = USART1_BUF_LENGTH - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
			/* 在485通信时，又是会出现未接收到数据，空闲中断响应的情况 */
			if (Usart1Buf.RxEndIndex[Usart1Buf.RxDimension] != 0)
			{
				Usart1Buf.RxEndFlag[Usart1Buf.RxDimension] = SET;
				Usart1Buf.RxDimension++;
				Usart1Buf.RxDimension %= USART1_BUF_DIMENSION;
				HAL_UART_DMAStop(huart);
				HAL_UART_Receive_DMA(huart, Usart1Buf.RxBuffer[Usart1Buf.RxDimension], USART1_BUF_LENGTH);
			}
		}
		__HAL_UART_CLEAR_IDLEFLAG(huart);
	}
	else if (huart->Instance == USART2)
	{
		tmp_flag = __HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE);
		tmp_it_source = __HAL_UART_GET_IT_SOURCE(&huart2, UART_IT_IDLE);  
		/* UART parity error interrupt occurred ------------------------------------*/
		if ((tmp_flag != RESET) && (tmp_it_source != RESET))
		{
			Usart2Buf.RxEndIndex[Usart2Buf.RxDimension] = USART2_BUF_LENGTH - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);
			/* 在485通信时，又是会出现未接收到数据，空闲中断响应的情况 */
			if (Usart2Buf.RxEndIndex[Usart2Buf.RxDimension] != 0)
			{
				Usart2Buf.RxEndFlag[Usart2Buf.RxDimension] = SET;
				Usart2Buf.RxDimension++;
				Usart2Buf.RxDimension %= USART2_BUF_DIMENSION;
				HAL_UART_DMAStop(huart);
				HAL_UART_Receive_DMA(huart, Usart2Buf.RxBuffer[Usart2Buf.RxDimension], USART2_BUF_LENGTH);
			}
		}
		__HAL_UART_CLEAR_IDLEFLAG(huart);
	}
	else if (huart->Instance == USART3)
	{
		tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE);
		tmp_it_source = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE);  
		/* UART parity error interrupt occurred ------------------------------------*/
		if ((tmp_flag != RESET) && (tmp_it_source != RESET))
		{
			Usart3Buf.RxEndIndex[Usart3Buf.RxDimension] = USART3_BUF_LENGTH - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);
			/* 在485通信时，又是会出现未接收到数据，空闲中断响应的情况 */
			if (Usart3Buf.RxEndIndex[Usart3Buf.RxDimension] != 0)
			{
				Usart3Buf.RxEndFlag[Usart3Buf.RxDimension] = SET;
				Usart3Buf.RxDimension++;
				Usart3Buf.RxDimension %= USART3_BUF_DIMENSION;
				HAL_UART_DMAStop(huart);
				HAL_UART_Receive_DMA(huart, Usart3Buf.RxBuffer[Usart3Buf.RxDimension], USART3_BUF_LENGTH);
			}
		}
		__HAL_UART_CLEAR_IDLEFLAG(huart);
	}
	else if (huart->Instance == UART4)
	{
		tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE);
		tmp_it_source = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE);  
		/* UART parity error interrupt occurred ------------------------------------*/
		if ((tmp_flag != RESET) && (tmp_it_source != RESET))
		{
			Usart4Buf.RxEndIndex[Usart4Buf.RxDimension] = USART4_BUF_LENGTH - __HAL_DMA_GET_COUNTER(&hdma_uart4_rx);
			/* 在485通信时，又是会出现未接收到数据，空闲中断响应的情况 */
			if (Usart4Buf.RxEndIndex[Usart4Buf.RxDimension] != 0)
			{
				Usart4Buf.RxEndFlag[Usart4Buf.RxDimension] = SET;
				Usart4Buf.RxDimension++;
				Usart4Buf.RxDimension %= USART4_BUF_DIMENSION;
				HAL_UART_DMAStop(huart);
				HAL_UART_Receive_DMA(huart, Usart4Buf.RxBuffer[Usart4Buf.RxDimension], USART4_BUF_LENGTH);
			}
		}
		__HAL_UART_CLEAR_IDLEFLAG(huart);
	}
	else if (huart->Instance == UART5)
	{
		tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE);
		tmp_it_source = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE);  
		/* UART parity error interrupt occurred ------------------------------------*/
		if ((tmp_flag != RESET) && (tmp_it_source != RESET))
		{
			Usart5Buf.RxEndIndex[Usart5Buf.RxDimension] = USART5_BUF_LENGTH - huart5.RxXferCount;
			/* 在485通信时，又是会出现未接收到数据，空闲中断响应的情况 */
			if (Usart5Buf.RxEndIndex[Usart5Buf.RxDimension] != 0)
			{
				Usart5Buf.RxEndFlag[Usart5Buf.RxDimension] = SET;
				Usart5Buf.RxDimension++;
				Usart5Buf.RxDimension %= USART5_BUF_DIMENSION;
				HAL_UART_AbortReceive_IT(huart); /* 暂停接收中断 */
				HAL_UART_Receive_IT(huart, Usart5Buf.RxBuffer[Usart5Buf.RxDimension], USART5_BUF_LENGTH);
			}
		}
		__HAL_UART_CLEAR_IDLEFLAG(huart);
	}
}
/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
