/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "dev_uart.h"
#include "bsp_uart.h"

uint32_t g_DmaTxCount[2] = {0};
uint32_t g_DmaRxCount[6] = {0};
uint32_t g_DmaTxErrCount[4] = {0};

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/*
void SysTick_Handler(void)
{
   
}*/

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		uart_dmarx_idle_isr(DEV_UART1);
		/* read the data to clear receive idle interrupt flag */
		USART_ReceiveData(USART1);
		//DMA_ClearFlag(DMA1_FLAG_GL5);
		g_DmaRxCount[0]++;
	}
}

void DMA1_Channel4_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC4))
	{
		g_DmaTxCount[0]++;	
		uart_dmatx_done_isr(DEV_UART1);
		DMA_ClearFlag(DMA1_FLAG_TC4);
        DMA_Cmd(DMA1_Channel4, DISABLE);
		uart_poll_dma_tx(DEV_UART2);
	}
}

void DMA1_Channel5_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC5))
	{
		g_DmaRxCount[1]++;
		uart_dmarx_done_isr(DEV_UART1);
		DMA_ClearFlag(DMA1_FLAG_TC5);
	}
	if(DMA_GetITStatus(DMA1_IT_HT5))
	{
		g_DmaRxCount[2]++;
		uart_dmarx_half_done_isr(DEV_UART1);
		DMA_ClearFlag(DMA1_FLAG_HT5);
	}
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		uart_dmarx_idle_isr(DEV_UART2);
		/* read the data to clear receive idle interrupt flag */
		USART_ReceiveData(USART2);
		g_DmaRxCount[3]++;
	}
}

void DMA1_Channel7_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC7))
	{
		g_DmaTxCount[1]++;	
		uart_dmatx_done_isr(DEV_UART2);
		DMA_ClearFlag(DMA1_FLAG_TC7);
        DMA_Cmd(DMA1_Channel7, DISABLE);
		uart_poll_dma_tx(DEV_UART2); 
	}
}

void DMA1_Channel6_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC6))
	{
		g_DmaRxCount[4]++;
		uart_dmarx_done_isr(DEV_UART2);
		DMA_ClearFlag(DMA1_FLAG_TC6);
	}
	if(DMA_GetITStatus(DMA1_IT_HT6))
	{
		g_DmaRxCount[5]++;
		uart_dmarx_half_done_isr(DEV_UART2);
		DMA_ClearFlag(DMA1_FLAG_HT6);
	}
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/


#ifndef STM32F10X_CL
/* CAN and USB IRQ for stm32 none connectivity line devices
 */
void USB_LP_CAN1_RX0_IRQHandler(void)
{

}
void USB_HP_CAN1_TX_IRQHandler(void)
{

}
#endif


/**
  * @}
  */


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
