
#include <stdint.h>
#include <string.h>
#include "bsp_uart.h"
#include "dev_uart.h"
#include <rtthread.h>
#include "stm32f10x.h"

static uint32_t s_count = 0;


void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void led_thread_entry(void* parameter)
{
	GPIO_Configuration();
    for (;;)
    {
	  	GPIO_SetBits(GPIOB, GPIO_Pin_0);
		rt_thread_mdelay(500);
        GPIO_ResetBits(GPIOB, GPIO_Pin_0);
		rt_thread_mdelay(500);
    }
}

void led_thread_init(void)
{
  	static rt_uint8_t s_led_stack[256];
  	static struct rt_thread led_thread;
    rt_err_t result;

    result = rt_thread_init(&led_thread,
                            "led",
                            led_thread_entry,
                            RT_NULL,
                            (rt_uint8_t*)&s_led_stack[0],
                            sizeof(s_led_stack),
                            7,
                            5);
    if (result == RT_EOK)
    {
        rt_thread_startup(&led_thread);
    }
}

/**
 * @brief  
 * @param  
 * @retval 
 */
int main(void)
{
  	uint16_t size = 0;
	uint8_t buf[256];
	
	led_thread_init();

	bsp_uart1_init();
	uart_device_init(DEV_UART1);
	
	bsp_uart2_init();
	uart_device_init(DEV_UART2);
	
	for(;;)
	{
        s_count++;

        /* 串口数据回环测试 */
        size = uart_read(DEV_UART1, buf, 256);
        uart_write(DEV_UART1, buf, size);
        size = uart_read(DEV_UART2, buf, 256);
        uart_write(DEV_UART2, buf, size);
        //uart2_poll_send(buf, size);
        
        /* 将fifo数据拷贝到dma buf，并启动dma传输 */
        uart_poll_dma_tx(DEV_UART1); 
        uart_poll_dma_tx(DEV_UART2);
		rt_thread_mdelay(1);
	}
}
