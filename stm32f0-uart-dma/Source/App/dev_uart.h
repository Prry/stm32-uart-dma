
#ifndef _DEV_UART_H_
#define _DEV_UART_H_

#include <stdbool.h>
#include <stdint.h>

#define DEV_UART1	0
#define DEV_UART2	1

extern void uart_device_init(uint8_t uart_id);
extern uint16_t uart_write(uint8_t uart_id, const uint8_t *buf, uint16_t size);
extern uint16_t uart_read(uint8_t uart_id, uint8_t *buf, uint16_t size);
extern void uart_dmarx_done_isr(uint8_t uart_id);
extern void uart_dmarx_half_done_isr(uint8_t uart_id);
extern void uart_dmarx_idle_isr(uint8_t uart_id);
extern void uart_dmatx_done_isr(uint8_t uart_id);
extern void uart_poll_dma_tx(uint8_t uart_id);

#endif 
