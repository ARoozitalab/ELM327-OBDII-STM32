/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    10/15/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "config.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_tim.h"
#include "obdii.h"
#include "blu.h"
char program_s=0;
uint64_t count=0;
uint64_t difs_count=0;
uint64_t difs_count1=0;
uint64_t difs_count_obd=0;
uint64_t difs_count_obd2=0;
uint64_t difs_count_HTGP=0;
extern uint8_t c_dt[8];
extern uint8_t c_rcv_flag;





/** @addtogroup STM32F10x_StdPeriph_Template
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
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
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
//void SVC_Handler(void)
//{
//}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
//}

//---------------------get csystick count param------------------------//
uint64_t GeterSystick()
{
	return count;
}


//---------------------set csystick count param------------------------//
void seterSystick(uint64_t set_count)
{
	count=set_count;
}


//---------------------get ms of start------------------------//100µS
uint64_t Geterms()
{	
	return ((count*ms_multiple)/ms_devider);
}



//---------------------get us of start------------------------//100µS
uint64_t Geterus()
{
	return ((count*us_multiple)/us_devider);
}

//---------------------set ms for reach------------------------//10µS
void setms_timecount(uint32_t set_count)
{
	difs_count=count +((set_count*ms_devider)/ms_multiple);
}
//---------------------set us for reach------------------------//10µS
void setus_timecount(uint32_t set_count)
{
	difs_count=count+((set_count*us_devider)/us_multiple);
}

void setms_timecount_obd(uint32_t set_count)
{
	difs_count_obd=count +((set_count*ms_devider)/ms_multiple);
}

void setms_timecount_obd2(uint32_t set_count)
{
	difs_count_obd2=count +((set_count*ms_devider)/ms_multiple);
}

void setms_timecount_HTGP(uint32_t set_count)
{
	difs_count_HTGP=count +((set_count*ms_devider)/ms_multiple);
}

//---------------------cheak for reach to time------------------------//5µS

uint8_t dif_count()
{
	if(difs_count>=count)
		return 0;
	
	return 1;
	
}

uint8_t dif_count_obd()
{
	if(difs_count_obd>=count)
		return 0;
	
	return 1;
	
}


uint8_t dif_count_obd2()
{
	if(difs_count_obd2>=count)
		return 0;
	
	return 1;
	
}


uint8_t dif_count_HTGP()
{
	if(difs_count_HTGP>=count)
		return 0;
	
	return 1;
	
}


void USART2_IRQHandler(void)
{
	
	 AT_buf[AT_count]=USART_ReceiveData(USART2);
		AT_count++;
		setms_timecount(1);
		USART_ClearFlag(USART2,USART_FLAG_RXNE);
}





void USART1_IRQHandler(void)
{
	if(OBD_RCV_T==1)
	{
	 obd_data[obd_count]=USART_ReceiveData(USART1);
		obd_count++;
	}
	setms_timecount_obd(1);
		
	USART_ClearFlag(USART1,USART_FLAG_RXNE);
}



void TIM2_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
	init_obd++;
	
	
	
}







void USB_LP_CAN1_RX0_IRQHandler(void)
{
	int i=0;

	CanRxMsg RxMessage;
 
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
if((RxMessage.StdId==0x7E8)&&(c_rcv_flag==0))
{
	for(i=0;i<8;i++)
	c_dt[i]= RxMessage.Data[i];
  c_rcv_flag=1;
	
	}
	CAN_ClearFlag(CAN1,CAN_IT_FMP0);
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
