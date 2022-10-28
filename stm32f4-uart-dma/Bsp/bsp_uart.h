
#ifndef _BSP_UART_H_
#define _BSP_UART_H_

#include <stdint.h>
#include <stdbool.h>

void uart1_dma_init(uint8_t *mem_addr, uint32_t mem_size);
void bsp_uart1_dmatx_config(uint8_t *mem_addr, uint32_t mem_size);
void bsp_uart1_dmarx_config(uint8_t *mem_addr, uint32_t mem_size);
uint16_t bsp_uart1_get_dmarx_buf_remain_size(void);

#endif /* _BSP_UART_H_ */

