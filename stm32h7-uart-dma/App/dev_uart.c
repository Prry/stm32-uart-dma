
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "bsp_uart.h"
#include "fifo.h"
#include "dev_uart.h"
#include "main.h"

/* 串口设备数据结构 */
typedef struct
{
	uint8_t status;		/* 发送状态 */
	_fifo_t tx_fifo;	/* 发送fifo */
	_fifo_t rx_fifo;	/* 接收fifo */
	uint8_t *dmarx_buf;	/* dma接收缓存 */
	uint16_t dmarx_buf_size;/* dma接收缓存大小*/
	uint8_t *dmatx_buf;	/* dma发送缓存 */
	uint16_t dmatx_buf_size;/* dma发送缓存大小 */
	uint16_t last_dmarx_size;/* dma上一次接收数据大小 */
}uart_device_t;

/* 串口缓存大小 */
#define UART1_TX_BUF_SIZE           1024
#define UART1_RX_BUF_SIZE           1024
#define	UART1_DMA_RX_BUF_SIZE		128
#define	UART1_DMA_TX_BUF_SIZE		128

#define UART2_TX_BUF_SIZE           1024
#define UART2_RX_BUF_SIZE           124
#define	UART2_DMA_RX_BUF_SIZE		128
#define	UART2_DMA_TX_BUF_SIZE		128

/* 串口缓存 */
static uint8_t s_uart1_tx_buf[UART1_TX_BUF_SIZE];
static uint8_t s_uart1_rx_buf[UART1_RX_BUF_SIZE];
static uint8_t s_uart1_dmarx_buf[UART1_DMA_RX_BUF_SIZE] __attribute__((section(".ARM.__at_0x24000000")));
static uint8_t s_uart1_dmatx_buf[UART1_DMA_TX_BUF_SIZE] __attribute__((section(".ARM.__at_0x24000080")));

static uint8_t s_uart2_tx_buf[UART2_TX_BUF_SIZE];
static uint8_t s_uart2_rx_buf[UART2_RX_BUF_SIZE];
static uint8_t s_uart2_dmarx_buf[UART2_DMA_RX_BUF_SIZE];
static uint8_t s_uart2_dmatx_buf[UART2_DMA_TX_BUF_SIZE];

/* 串口设备实例 */
static uart_device_t s_uart_dev[2] = {0};

/* 测试 */
uint32_t s_UartTxRxCount[4] = {0};


/* fifo上锁函数 */
static void fifo_lock(void)
{
    __disable_irq();
}

/* fifo解锁函数 */
static void fifo_unlock(void)
{
    __enable_irq();
}

/**
 * @brief 串口设备初始化 
 * @param  
 * @retval 
 */
void uart_device_init(uint8_t uart_id)
{
  	if (uart_id == 0)
	{
		/* 配置串口1收发fifo */
		fifo_register(&s_uart_dev[uart_id].tx_fifo, &s_uart1_tx_buf[0], 
                      sizeof(s_uart1_tx_buf), NULL, NULL);
		fifo_register(&s_uart_dev[uart_id].rx_fifo, &s_uart1_rx_buf[0], 
                      sizeof(s_uart1_rx_buf), fifo_lock, fifo_unlock);
		
		/* 配置串口1 DMA收发buf */
		s_uart_dev[uart_id].dmarx_buf = &s_uart1_dmarx_buf[0];
		s_uart_dev[uart_id].dmarx_buf_size = sizeof(s_uart1_dmarx_buf);
		s_uart_dev[uart_id].dmatx_buf = &s_uart1_dmatx_buf[0];
		s_uart_dev[uart_id].dmatx_buf_size = sizeof(s_uart1_dmatx_buf);
		uart5_dma_init(s_uart_dev[uart_id].dmarx_buf, 
							   sizeof(s_uart1_dmarx_buf));/* 只需配置接收模式DMA，发送模式需发送数据时才配置 */
		s_uart_dev[uart_id].status  = 0;
	}
	else if (uart_id == 1)
	{
		/* 配置串口2收发fifo */
		fifo_register(&s_uart_dev[uart_id].tx_fifo, &s_uart2_tx_buf[0], 
                      sizeof(s_uart2_tx_buf), fifo_lock, fifo_unlock);
		fifo_register(&s_uart_dev[uart_id].rx_fifo, &s_uart2_rx_buf[0], 
                      sizeof(s_uart2_rx_buf), fifo_lock, fifo_unlock);
		
		/* 配置串口2 DMA收发buf */
		s_uart_dev[uart_id].dmarx_buf = &s_uart2_dmarx_buf[0];
		s_uart_dev[uart_id].dmarx_buf_size = sizeof(s_uart2_dmarx_buf);
		s_uart_dev[uart_id].dmatx_buf = &s_uart2_dmatx_buf[0];
		s_uart_dev[uart_id].dmatx_buf_size = sizeof(s_uart2_dmatx_buf);
		//bsp_uart2_dmarx_config(s_uart_dev[uart_id].dmarx_buf, 
		//					   sizeof(s_uart2_dmarx_buf));
		s_uart_dev[uart_id].status  = 0;
	}
}

/**
 * @brief  串口发送数据接口，实际是写入发送fifo，发送由dma处理
 * @param  
 * @retval 
 */
uint16_t uart_write(uint8_t uart_id, const uint8_t *buf, uint16_t size)
{
	return fifo_write(&s_uart_dev[uart_id].tx_fifo, buf, size);
}

/**
 * @brief  串口读取数据接口，实际是从接收fifo读取
 * @param  
 * @retval 
 */
uint16_t uart_read(uint8_t uart_id, uint8_t *buf, uint16_t size)
{
	return fifo_read(&s_uart_dev[uart_id].rx_fifo, buf, size);
}

/**
 * @brief  串口dma接收完成中断处理
 * @param  
 * @retval 
 */
void uart_dmarx_done_isr(uint8_t uart_id)
{
  	uint16_t recv_size;
	
	recv_size = s_uart_dev[uart_id].dmarx_buf_size - s_uart_dev[uart_id].last_dmarx_size;
    s_UartTxRxCount[uart_id*2+1] += recv_size;
	fifo_write(&s_uart_dev[uart_id].rx_fifo, 
				   (const uint8_t *)&(s_uart_dev[uart_id].dmarx_buf[s_uart_dev[uart_id].last_dmarx_size]), recv_size);

	s_uart_dev[uart_id].last_dmarx_size = 0;
}

/**
 * @brief  串口dma接收缓存大小一半数据中断处理
 * @param  
 * @retval 
 */
void uart_dmarx_half_done_isr(uint8_t uart_id)
{
  	uint16_t recv_total_size;
  	uint16_t recv_size;
	
	if(uart_id == 0)
	{
	  	recv_total_size = s_uart_dev[uart_id].dmarx_buf_size - bsp_uart5_get_dmarx_buf_remain_size();
	}
	else if (uart_id == 1)
	{
		//recv_total_size = s_uart_dev[uart_id].dmarx_buf_size - bsp_uart2_get_dmarx_buf_remain_size();
	}
	recv_size = recv_total_size - s_uart_dev[uart_id].last_dmarx_size;
	s_UartTxRxCount[uart_id*2+1] += recv_size;
    
	fifo_write(&s_uart_dev[uart_id].rx_fifo, 
				   (const uint8_t *)&(s_uart_dev[uart_id].dmarx_buf[s_uart_dev[uart_id].last_dmarx_size]), recv_size);
	s_uart_dev[uart_id].last_dmarx_size = recv_total_size;
}

/**
 * @brief  串口空闲中断处理
 * @param  
 * @retval 
 */
void uart_dmarx_idle_isr(uint8_t uart_id)
{
  	uint16_t recv_total_size;
  	uint16_t recv_size;
	
	if(uart_id == 0)
	{
	  	recv_total_size = s_uart_dev[uart_id].dmarx_buf_size - bsp_uart5_get_dmarx_buf_remain_size();
	}
	else if (uart_id == 1)
	{
		//recv_total_size = s_uart_dev[uart_id].dmarx_buf_size - bsp_uart2_get_dmarx_buf_remain_size();
	}
	recv_size = recv_total_size - s_uart_dev[uart_id].last_dmarx_size;
	s_UartTxRxCount[uart_id*2+1] += recv_size;
	fifo_write(&s_uart_dev[uart_id].rx_fifo, 
				   (const uint8_t *)&(s_uart_dev[uart_id].dmarx_buf[s_uart_dev[uart_id].last_dmarx_size]), recv_size);
	s_uart_dev[uart_id].last_dmarx_size = recv_total_size;
}

/**
 * @brief  串口dma发送完成中断处理
 * @param  
 * @retval 
 */
void uart_dmatx_done_isr(uint8_t uart_id)
{
 	s_uart_dev[uart_id].status = 0;	/* DMA发送空闲 */
}

/**
 * @brief  循环从串口发送fifo读出数据，放置于dma发送缓存，并启动dma传输
 * @param  
 * @retval 
 */
void uart_poll_dma_tx(uint8_t uart_id)
{
  	uint16_t size = 0;
	
	if (0x01 == s_uart_dev[uart_id].status)
    {
        return;
    }
	size = fifo_read(&s_uart_dev[uart_id].tx_fifo, s_uart_dev[uart_id].dmatx_buf,
					 s_uart_dev[uart_id].dmatx_buf_size);
	if (size != 0)
	{	
        s_UartTxRxCount[uart_id*2+0] += size;
	  	if (uart_id == 0)
		{
            s_uart_dev[uart_id].status = 0x01;	/* DMA发送状态 */
		  	bsp_uart5_dmatx_config(s_uart_dev[uart_id].dmatx_buf, size);
		}
		else if (uart_id == 1)
		{
            s_uart_dev[uart_id].status = 0x01;	/* DMA发送状态,必须在使能DMA传输前置位，否则有可能DMA已经传输并进入中断 */
			//bsp_uart2_dmatx_config(s_uart_dev[uart_id].dmatx_buf, size);
		}
	}
}
