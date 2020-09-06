
#ifndef _BSP_UART_H_
#define _BSP_UART_H_

#include <stdint.h>
#include <stdbool.h>
#include "stm32f0xx_usart.h"

extern void bsp_uart1_init(void);
extern void bsp_uart1_dmatx_config(uint8_t *mem_addr, uint32_t mem_size);
extern void bsp_uart1_dmarx_config(uint8_t *mem_addr, uint32_t mem_size);
extern uint16_t bsp_uart1_get_dmarx_buf_remain_size(void);

extern void bsp_uart2_init(void);
extern void bsp_uart2_dmatx_config(uint8_t *mem_addr, uint32_t mem_size);
extern void bsp_uart2_dmarx_config(uint8_t *mem_addr, uint32_t mem_size);
extern uint16_t bsp_uart2_get_dmarx_buf_remain_size(void);
extern void uart2_poll_send(const uint8_t *buf, uint16_t size);
#endif 

