
#ifndef _FIFO_H_
#define _FIFO_H_
 
#include <stdbool.h>
#include <stdint.h>

typedef void (*lock_fun)(void);

typedef struct
{
	uint8_t *buf;      	    /* 缓冲区 */
	uint32_t buf_size;    	/* 缓冲区大小 */
    uint32_t occupy_size;   /* 有效数据大小 */
	uint8_t *pwrite;      	/* 写指针 */
	uint8_t *pread;       	/* 读指针 */
	void (*lock)(void);	/* 互斥上锁 */
	void (*unlock)(void);	/* 互斥解锁 */
}_fifo_t;
 
 
extern void fifo_register(_fifo_t *pfifo, uint8_t *pfifo_buf, uint32_t size,
                            lock_fun lock, lock_fun unlock);
extern void fifo_release(_fifo_t *pfifo);
extern uint32_t fifo_write(_fifo_t *pfifo, const uint8_t *pbuf, uint32_t size);
extern uint32_t fifo_read(_fifo_t *pfifo, uint8_t *pbuf, uint32_t size);
extern uint32_t fifo_get_total_size(_fifo_t *pfifo);
extern uint32_t fifo_get_free_size(_fifo_t *pfifo);
extern uint32_t fifo_get_occupy_size(_fifo_t *pfifo);

#endif
