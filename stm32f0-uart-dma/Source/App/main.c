
#include <stdint.h>
#include <string.h>
#include "stm32f0xx.h"
#include "bsp_uart.h"
#include "dev_uart.h"

static uint32_t s_count = 0;

/**
 * @brief  
 * @param  
 * @retval 
 */
int main(void)
{
  	uint16_t size = 0;
	uint8_t buf[256];
	
	bsp_uart1_init();
	uart_device_init(DEV_UART1);
	
	bsp_uart2_init();
	uart_device_init(DEV_UART2);
	
	for(;;)
	{
        s_count++;
	  	
        if (s_count%5000)
	  	{
            /* 串口数据回环测试 */
            size = uart_read(DEV_UART1, buf, 256);
            uart_write(DEV_UART1, buf, size);
            size = uart_read(DEV_UART2, buf, 256);
            uart_write(DEV_UART2, buf, size);
            //uart2_poll_send(buf, size);
            
            /* 将fifo数据拷贝到dma buf，并启动dma传输 */
            uart_poll_dma_tx(DEV_UART1); 
            uart_poll_dma_tx(DEV_UART2);
        }
	}
}
