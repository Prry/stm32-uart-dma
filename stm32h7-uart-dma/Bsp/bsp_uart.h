
#ifndef _BSP_UART_H_
#define _BSP_UART_H_

#include <stdint.h>
#include <stdbool.h>

void uart5_dma_init(uint8_t *mem_addr, uint32_t mem_size);
void bsp_uart5_dmatx_config(uint8_t *mem_addr, uint32_t mem_size);
void bsp_uart5_dmarx_config(uint8_t *mem_addr, uint32_t mem_size);
uint16_t bsp_uart5_get_dmarx_buf_remain_size(void);

void bsp_uart2_init(void);
void bsp_uart2_dmatx_config(uint8_t *mem_addr, uint32_t mem_size);
void bsp_uart2_dmarx_config(uint8_t *mem_addr, uint32_t mem_size);
uint16_t bsp_uart2_get_dmarx_buf_remain_size(void);
void uart2_poll_send(const uint8_t *buf, uint16_t size);

#endif /* _BSP_UART_H_ */

