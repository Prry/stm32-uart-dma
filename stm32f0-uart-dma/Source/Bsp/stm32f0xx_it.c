/**
******************************************************************************
* @file    Project/STM32F0xx_StdPeriph_Templates/stm32f0xx_it.c 
* @author  MCD Application Team
* @version V1.3.1
* @date    17-January-2014
* @brief   Main Interrupt Service Routines.
*          This file provides template for all exceptions handler and 
*          peripherals interrupt service routine.
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
*
* Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
* You may not use this file except in compliance with the License.
* You may obtain a copy of the License at:
*
*        http://www.st.com/software_license_agreement_liberty_v2
*
* Unless required by applicable law or agreed to in writing, software 
* distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
#include "dev_uart.h"

/** @addtogroup Template_Project
* @{
*/

uint32_t g_DmaTxCount[2] = {0};
uint32_t g_DmaRxCount[6] = {0};
uint32_t g_DmaTxErrCount[4] = {0};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
* @brief  This function handles NMI exception.
* @param  None
* @retval None
*/
void NMI_Handler(void)
{
}

/**
* @brief  This function handles Hard Fault exception.
* @param  None
* @retval None
*/
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    NVIC_SystemReset();
    
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
* @brief  This function handles PendSVC exception.
* @param  None
* @retval None
*/
void PendSV_Handler(void)
{
}

/**
* @brief  This function handles SysTick Handler.
* @param  None
* @retval None
*/
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                            */
/******************************************************************************/

/**
* @brief  This function handles USART1 interrupt request.
* @param  None
* @retval None
*/
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
        g_DmaRxCount[0]++;
		uart_dmarx_idle_isr(DEV_UART1);
		USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);
	}
}

/**
* @brief  This function handles USART2 interrupt request.
* @param  None
* @retval None
*/
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
        g_DmaRxCount[3]++;
		uart_dmarx_idle_isr(DEV_UART2);
		USART_ReceiveData(USART2);
		USART_ClearITPendingBit(USART2, USART_IT_IDLE);
	}
}

void DMA1_Channel2_3_IRQHandler(void)
{
  	if(DMA_GetITStatus(DMA1_IT_TE2))
	{
		g_DmaTxErrCount[0]++;
		DMA_ClearFlag(DMA1_IT_TE2);
	}
	
	if(DMA_GetITStatus(DMA1_IT_TE3))
	{
		g_DmaTxErrCount[1]++;
		DMA_ClearFlag(DMA1_IT_TE3);
	}
	
	if(DMA_GetITStatus(DMA1_IT_TC2))
	{
		g_DmaTxCount[0]++;	
		uart_dmatx_done_isr(DEV_UART1);
		DMA_ClearFlag(DMA1_FLAG_TC2);
        DMA_Cmd(DMA1_Channel2, DISABLE);
	}
    if(DMA_GetITStatus(DMA1_IT_HT3))
	{
		g_DmaRxCount[1]++;
		DMA_ClearFlag(DMA1_FLAG_HT3);
		uart_dmarx_half_done_isr(DEV_UART1);
	}
	if(DMA_GetITStatus(DMA1_IT_TC3))
	{
		g_DmaRxCount[2]++;
		DMA_ClearFlag(DMA1_FLAG_TC3);
		uart_dmarx_done_isr(DEV_UART1);
	}
}

void DMA1_Channel4_5_IRQHandler(void)
{
  	if(DMA_GetITStatus(DMA1_IT_TE4))
	{
		g_DmaTxErrCount[2]++;
		DMA_ClearFlag(DMA1_IT_TE4);
	}
	
	if(DMA_GetITStatus(DMA1_IT_TE5))
	{
		g_DmaTxErrCount[3]++;
		DMA_ClearFlag(DMA1_IT_TE5);
	}
	
	if(DMA_GetITStatus(DMA1_IT_TC4))
	{
		g_DmaTxCount[1]++;
		uart_dmatx_done_isr(DEV_UART2);
		DMA_ClearFlag(DMA1_FLAG_TC4);
		DMA_Cmd(DMA1_Channel4, DISABLE);
	}
    if(DMA_GetITStatus(DMA1_IT_HT5))
	{
		g_DmaRxCount[4]++;
		DMA_ClearFlag(DMA1_FLAG_HT5);
		uart_dmarx_half_done_isr(DEV_UART2);
	}
	if(DMA_GetITStatus(DMA1_IT_TC5))
	{
		g_DmaRxCount[5]++;
		DMA_ClearFlag(DMA1_FLAG_TC5);
		uart_dmarx_done_isr(DEV_UART2);
	}
}

/**
* @}
*/ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
