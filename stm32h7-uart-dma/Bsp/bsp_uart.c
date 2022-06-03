
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "bsp_uart.h"

void uart5_dma_init(uint8_t *mem_addr, uint32_t mem_size)
{
    LL_USART_InitTypeDef UART_InitStruct = {0};
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_UART5;
    PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART5);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
    LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_12|LL_GPIO_PIN_13;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_14;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    bsp_uart5_dmarx_config(mem_addr, mem_size);
    
    NVIC_SetPriority(DMA1_Stream0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
    NVIC_EnableIRQ(DMA1_Stream0_IRQn);
    NVIC_SetPriority(DMA1_Stream1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 1));
    NVIC_EnableIRQ(DMA1_Stream1_IRQn);
    
    NVIC_SetPriority(UART5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 2, 0));
    NVIC_EnableIRQ(UART5_IRQn);

    UART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
    UART_InitStruct.BaudRate = 1500000;
    UART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    UART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    UART_InitStruct.Parity = LL_USART_PARITY_NONE;
    UART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    UART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    UART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(UART5, &UART_InitStruct);
    LL_USART_DisableFIFO(UART5);
    LL_USART_SetTXFIFOThreshold(UART5, LL_USART_FIFOTHRESHOLD_1_8);
    LL_USART_SetRXFIFOThreshold(UART5, LL_USART_FIFOTHRESHOLD_1_8);
    LL_USART_ConfigAsyncMode(UART5);
    
    LL_USART_EnableDMAReq_RX(UART5);
    LL_USART_EnableDMAReq_TX(UART5);
    LL_USART_EnableIT_IDLE(UART5);
    LL_USART_Enable(UART5);
}

void bsp_uart5_dmatx_config(uint8_t *mem_addr, uint32_t mem_size)
{
    LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_1);
    LL_DMA_SetPeriphRequest(DMA1, LL_DMA_STREAM_1, LL_DMAMUX1_REQ_UART5_TX);
    LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_1, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
    LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_1, LL_DMA_PRIORITY_LOW);
    LL_DMA_SetMode(DMA1, LL_DMA_STREAM_1, LL_DMA_MODE_NORMAL);
    LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_1, LL_DMA_PERIPH_NOINCREMENT);
    LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_1, LL_DMA_MEMORY_INCREMENT);
    LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_1, LL_DMA_PDATAALIGN_BYTE);
    LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_1, LL_DMA_MDATAALIGN_BYTE);
    LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_1);
    LL_DMA_SetPeriphAddress(DMA1, LL_DMA_STREAM_1, LL_USART_DMA_GetRegAddr(UART5, LL_USART_DMA_REG_DATA_TRANSMIT));
    
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_1, (uint32_t)mem_addr);
    LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_1, mem_size);
    
    LL_DMA_ClearFlag_TC1(DMA1);
    LL_DMA_ClearFlag_HT1(DMA1);
    LL_DMA_ClearFlag_TE1(DMA1);
    LL_DMA_ClearFlag_DME1(DMA1);
    LL_DMA_ClearFlag_FE1(DMA1);
        
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_1);
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_1);
}

void bsp_uart5_dmarx_config(uint8_t *mem_addr, uint32_t mem_size)
{
    LL_DMA_DeInit(DMA1, LL_DMA_STREAM_0);
    LL_DMA_SetPeriphRequest(DMA1, LL_DMA_STREAM_0, LL_DMAMUX1_REQ_UART5_RX);
    LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_0, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
    LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_0, LL_DMA_PRIORITY_LOW);
    LL_DMA_SetMode(DMA1, LL_DMA_STREAM_0, LL_DMA_MODE_CIRCULAR);
    LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_0, LL_DMA_PERIPH_NOINCREMENT);
    LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_0, LL_DMA_MEMORY_INCREMENT);
    LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_0, LL_DMA_PDATAALIGN_BYTE);
    LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_0, LL_DMA_MDATAALIGN_BYTE);
    LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_0);
    LL_DMA_SetPeriphAddress(DMA1, LL_DMA_STREAM_0, LL_USART_DMA_GetRegAddr(UART5, LL_USART_DMA_REG_DATA_RECEIVE));
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_0, (uint32_t)mem_addr);
    LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, mem_size);
    
    LL_DMA_EnableIT_HT(DMA1, LL_DMA_STREAM_0);
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_0);
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0);
}

uint16_t bsp_uart5_get_dmarx_buf_remain_size(void)
{
	return LL_DMA_GetDataLength(DMA1, LL_DMA_STREAM_0);	
}
