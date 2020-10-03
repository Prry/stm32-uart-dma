# STM32高速串口通信DMA收发实现

<br>

## 测试平台

* STM32F030C8T6
* UART1/UART2
* DMA1 Channel2—Channel5
* ST标准库
* 主频48MHz（外部12MHz晶振）

<br>

## 实现功能

* 收/发环形缓冲区
* 不定长度接收处理
* 高速（1.5Mbps）通信不丢数据

<br>

## 关键实现

### DMA发送模式

* 线程循环查询发送环形缓冲区数据，然后启动MDA传输

* DMA传输完成中断，连续发送
* 定时器中断周期发送

<br>

### DMA接收模式

* DMA缓存半满中断（如CPU硬件支持，可使用DMA双缓存机制）
* DMA缓存传输完成中断
* 串口空闲中断实现

<br>

## 其他

更详细的描述过程参考文章[一个严谨的STM32串口DMA发送&接收（1.5Mbps波特率）机制](https://blog.csdn.net/qq_20553613/article/details/108367512)