/*********************************************************************************************************
*
* File                : blu.c
* Version             : V1.0
* By                  : alireza roozitalab
* email               : alireza.roozitalab@gmail.com
* git user            : ARoozitalab
*                                                
*                                          All Rights Reserved
*
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include <Blu.h>
#include <stm32f10x_it.h>
#include <config.h>
#include <stdlib.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t deviceId=1;
uint16_t AT_count=0;
char AT_buf[buf_Size];

/*******************************************************************************
* Function Name  : BL_init
* Description    : init serial port for bluetooth
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/	


void BL_init(){
	int i=0;
	uint8_t rsp_flag=0;
	uint16_t add_write=0;
	uint16_t temp = 0;

	
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA ,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 ,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);		   

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 38400;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART2, &USART_InitStructure); 
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
  USART_ClearFlag(USART2,USART_FLAG_TC);
  USART_Cmd(USART2, ENABLE);
	
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	USART_ClearFlag(USART2,USART_FLAG_RXNE);
	USART_Cmd(USART2, ENABLE);
}


/*******************************************************************************
* Function Name  : BL_Send
* Description    : send serial data by bluetooth spp protocol
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/	
void BL_Send(uint8_t conId,uint8_t size, char* data){
	
uint8_t i=0;

	for(i=0;i<size;i++)
	{
	 while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
  USART_SendData(USART2, (uint8_t) data[i]);
	}
	
	
}







/*******************************************************************************
* Function Name  : BL_Recive
* Description    : receive serial data by bluetooth spp protocol
* Input          : buffer,size
* Output         : None
* Return         : None
*******************************************************************************/	
void BL_Recive(char* buffer,char* size){

	uint8_t i;
		uint8_t rc_glag=0;
	
	// if somthing recived
	if(AT_count>0)
	{
		for(i=0;i<=AT_count;i++)
		{
			if(AT_buf[i]==0x0d)
			{
				rc_glag=1;
				break;
			}
		}
		//if true packet recived copy until 0x0d
		if(rc_glag==1)
		{
		size[0]=i;
		memcpy(buffer,AT_buf,size[0]);
			
			///clear buffer
			for(i=0;i<buf_Size;i++)
			AT_buf[i]=0;
			AT_count=0;
			}					
					
		}				
		
	}
