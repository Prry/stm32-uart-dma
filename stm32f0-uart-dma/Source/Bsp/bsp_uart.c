
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32f0xx.h"
#include "bsp_uart.h"

/**
 * @brief  uart1 gpio配置
 * @param  
 * @retval 
 */
static void bsp_uart1_gpio_init(void)
{
    GPIO_InitTypeDef    GPIO_InitStructure;
#if 0
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_0); 
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed  	= GPIO_Speed_Level_3;
    GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#else
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_1); 
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed  	= GPIO_Speed_Level_3;
    GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
}

/**
 * @brief   uart2 gpio配置
 * @param  
 * @retval 
 */
static void bsp_uart2_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief  uart1初始化配置
 * @param  
 * @retval 
 */
void bsp_uart1_init(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	bsp_uart1_gpio_init();
	
	/* 使能串口和DMA时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	

	USART_InitStructure.USART_BaudRate            = 115200;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);	/* 使能空闲中断 */
	USART_OverrunDetectionConfig(USART1, USART_OVRDetection_Disable);
	
	USART_Cmd(USART1, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx|USART_DMAReq_Tx, ENABLE); /* 使能DMA收发 */

	/* 串口中断 */
	NVIC_InitStructure.NVIC_IRQChannel         = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd      = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* DMA中断 */
  	NVIC_InitStructure.NVIC_IRQChannel 		   = DMA1_Channel2_3_IRQn;       
  	NVIC_InitStructure.NVIC_IRQChannelPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd      = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  uart2初始化配置
 * @param  
 * @retval 
 */
void bsp_uart2_init(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	bsp_uart2_gpio_init();
	
	/* 使能串口和DMA时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	

	USART_InitStructure.USART_BaudRate            = 1500000;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);	/* 使能空闲中断 */
	USART_OverrunDetectionConfig(USART2, USART_OVRDetection_Disable);
	
	USART_Cmd(USART2, ENABLE);
	USART_DMACmd(USART2, USART_DMAReq_Rx|USART_DMAReq_Tx, ENABLE); 	/* 使能DMA收发 */

	/* 串口中断 */
	NVIC_InitStructure.NVIC_IRQChannel         = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd      = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* DMA中断 */
	NVIC_InitStructure.NVIC_IRQChannel         = DMA1_Channel4_5_IRQn;       
  	NVIC_InitStructure.NVIC_IRQChannelPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd      = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  uart1 dma发送通道配置
 * @param  
 * @retval 
 */
void bsp_uart1_dmatx_config(uint8_t *mem_addr, uint32_t mem_size)
{
  	DMA_InitTypeDef DMA_InitStructure;
	
	DMA_DeInit(DMA1_Channel2);
	DMA_Cmd(DMA1_Channel2, DISABLE);
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&(USART1->TDR);
	DMA_InitStructure.DMA_MemoryBaseAddr 		= (uint32_t)mem_addr; 
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_PeripheralDST; 	/* 传输方向:内存->外设 */
	DMA_InitStructure.DMA_BufferSize 			= mem_size; 
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable; 
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte; 
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Normal; 
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_High; 
	DMA_InitStructure.DMA_M2M 					= DMA_M2M_Disable; 
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);  
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC|DMA_IT_TE, ENABLE); 
	DMA_ClearFlag(DMA1_IT_TC2);	/* 清除发送完成标识 */
	DMA_Cmd(DMA1_Channel2, ENABLE); 
}

/**
 * @brief  uart1 dma接收通道配置
 * @param  
 * @retval 
 */
void bsp_uart1_dmarx_config(uint8_t *mem_addr, uint32_t mem_size)
{
  	DMA_InitTypeDef DMA_InitStructure;
	
	DMA_DeInit(DMA1_Channel3); 
	DMA_Cmd(DMA1_Channel3, DISABLE);
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&(USART1->RDR);
	DMA_InitStructure.DMA_MemoryBaseAddr 		= (uint32_t)mem_addr; 
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_PeripheralSRC; 	/* 传输方向:外设->内存 */
	DMA_InitStructure.DMA_BufferSize 			= mem_size; 
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable; 
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte; 
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Circular; 
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_VeryHigh; 
	DMA_InitStructure.DMA_M2M 					= DMA_M2M_Disable; 
	DMA_Init(DMA1_Channel3, &DMA_InitStructure); 
	DMA_ITConfig(DMA1_Channel3, DMA_IT_TC|DMA_IT_HT|DMA_IT_TE, ENABLE);/* 使能DMA半满、溢满、错误中断 */
	DMA_ClearFlag(DMA1_IT_TC3);
	DMA_ClearFlag(DMA1_IT_HT3);
	DMA_Cmd(DMA1_Channel3, ENABLE); 
}

/**
 * @brief  获取DMA接收buf剩余空间
 * @param  
 * @retval 
 */
uint16_t bsp_uart1_get_dmarx_buf_remain_size(void)
{
	return DMA_GetCurrDataCounter(DMA1_Channel3);	
}

/**
 * @brief  uart2 dma发送通道配置
 * @param  
 * @retval 
 */
void bsp_uart2_dmatx_config(uint8_t *mem_addr, uint32_t mem_size)
{
  	DMA_InitTypeDef DMA_InitStructure;
	
	DMA_DeInit(DMA1_Channel4);
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&(USART2->TDR);
	DMA_InitStructure.DMA_MemoryBaseAddr 		= (uint32_t)mem_addr; 
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_PeripheralDST; 	/* 传输方向:内存->外设 */
	DMA_InitStructure.DMA_BufferSize 			= mem_size; 
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable; 
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte; 
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Normal; 
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_High; 
	DMA_InitStructure.DMA_M2M 					= DMA_M2M_Disable; 
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);  
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC|DMA_IT_TE, ENABLE); 
	DMA_ClearFlag(DMA1_IT_TC4);	/* 清除发送完成标识 */
	DMA_Cmd(DMA1_Channel4, ENABLE); 
}

/**
 * @brief  uart2 dma接收通道配置
 * @param  
 * @retval 
 */
void bsp_uart2_dmarx_config(uint8_t *mem_addr, uint32_t mem_size)
{
  	DMA_InitTypeDef DMA_InitStructure;
	
	DMA_DeInit(DMA1_Channel5); 
	DMA_Cmd(DMA1_Channel5, DISABLE);
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&(USART2->RDR);
	DMA_InitStructure.DMA_MemoryBaseAddr 		= (uint32_t)mem_addr; 
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_PeripheralSRC; 	/* 传输方向:外设->内存 */
	DMA_InitStructure.DMA_BufferSize 			= mem_size; 
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable; 
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte; 
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Circular; 
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_VeryHigh; 
	DMA_InitStructure.DMA_M2M 					= DMA_M2M_Disable; 
	DMA_Init(DMA1_Channel5, &DMA_InitStructure); 
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC|DMA_IT_HT|DMA_IT_TE, ENABLE);/* 使能DMA半满、溢满、错误中断 */
	DMA_ClearFlag(DMA1_IT_TC5);
	DMA_ClearFlag(DMA1_IT_HT5);
	DMA_Cmd(DMA1_Channel5, ENABLE); 
}

/**
 * @brief  获取DMA接收buf剩余空间
 * @param  
 * @retval 
 */
uint16_t bsp_uart2_get_dmarx_buf_remain_size(void)
{
	return DMA_GetCurrDataCounter(DMA1_Channel5);	
}

/**
 * @brief  uart1循环发送
 * @param  
 * @retval 
 */
void uart1_poll_send(const uint8_t *buf, uint16_t size)
{	
    uint16_t i = 0;
    
    for (i=0; i<size; i++)
    {
	  	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        USART_SendData(USART1, *(buf+i));
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    }
}

/**
 * @brief  uart2循环发送
 * @param  
 * @retval 
 */
void uart2_poll_send(const uint8_t *buf, uint16_t size)
{	
    uint16_t i = 0;
    
    for (i=0; i<size; i++)
    {
	  	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        USART_SendData(USART2, *(buf+i));
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    }
}