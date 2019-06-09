
/*********************************************************************************************************
*
* File                : main.c
* Version             : V1.0
* By                  : alireza roozitalab
* email               : alireza.roozitalab@gmail.com
* git user            : ARoozitalab
*                                                
*                                          All Rights Reserved
*
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "stm32f10x_flash.h"
#include "stm32f10x_it.h"
#include <config.h>
#include "blu.h"
#include "stm32f10x_tim.h"
#include <obdii.h>
#include "FreeRTOS.h"
#include "task.h"
#include "hexString.h"
#include "stm32f10x_iwdg.h"





uint8_t send_cmd_obd=0;
uint8_t init_obd_port=0;
uint8_t k_cmd4[30]={0};
char msg_log[500]={0};
int msg_size=0;
uint8_t size_obd=0;
uint8_t obd_send_flag=0;



void delay()
	{
uint32_t i;
for(i=0;i<0x1fffff;i++);		
	
	}

	
	
/*******************************************************************************
* Function Name  : OBD_Task
* Description    : task for handleing obdii (init and send/receive data)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/		

void OBD_Task(void * pvArg)
	{

  uint8_t k_cmd[20];
	uint8_t k_cmd2[30];
	char bl_cmd2[30];
	uint8_t obd_sz=0;
	int i,j=0;
	OBD_Serial_Init();
	

	while(1)
	{
		vTaskDelay(1);
		
		
	if(obd_send_flag)	
	 {		 
		 obd_sz=0;
		 OBD_Send_Recive(k_cmd4,(size_obd/3)+1,k_cmd2,&obd_sz);
		 obd_send_flag=0;
		 size_obd=0;
	 }
	else if(init_obd_port==1)
	{	
		while(obd_sz!=3)
		{
			OBD_ecu_Init();
			k_cmd[0]=0x81;
			OBD_Send_Recive(k_cmd,1,k_cmd2,&obd_sz);

		}
		init_obd_port=2;
		BL_Send(deviceId,16,"\r\rELM327 v2.1\r\r>");
	}

	
		if(send_cmd_obd==1)
			j++;
		if(obd_sz!=0)
		{
			send_cmd_obd=0;
			j=0;
			for(i=0;i<30;i++)
			bl_cmd2[i]=0;
			for(i=0;i<obd_sz;i++)
			{
			bytesToHexString(&bl_cmd2[i*3],k_cmd2[i]);			
			}
			bl_cmd2[(obd_sz*3)-1]=0x20;
			bl_cmd2[(obd_sz*3)]=0x0d;
			bl_cmd2[(obd_sz*3)+1]=0x0d;
			bl_cmd2[(obd_sz*3)+2]=0x3E;
			BL_Send(deviceId,(obd_sz*3)+3,bl_cmd2);
			obd_sz=0;
			for(i=0;i<30;i++)
			k_cmd2[i]=0;
		}
		
		else if(j==50)
		{
			BL_Send(deviceId,10,"NO DATA\r\r>");
			j=0;	
      send_cmd_obd=0;			
		}
	
		if((dif_count_obd2())&(init_obd_port==2))
		{
			k_cmd[0]=0x01;
			k_cmd[1]=0x00;
			if(obd_sz==0)
			{	
			OBD_Send_Recive(k_cmd,2,k_cmd2,&obd_sz);
			obd_sz=0;
			for(i=0;i<30;i++)
			k_cmd2[i]=0;
			}
		}	
	}			
	}
	
	
/*******************************************************************************
* Function Name  : BL_Task
* Description    : task for handleing bluetooth hc-05
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/	
	
	
	
void BL_Task(void * pvArg)
	{
	char  buffer_r[50];
	int i=0;
	char k_cmd3[30]={0};
  char size_b=0;
	char check_validate=5,valid_link=0;
	BL_init();
	setms_timecount_HTGP(1500);
		while(1)
		{
	 IWDG_ReloadCounter();
		size_b=0;
    BL_Recive(buffer_r,&size_b);
	
			if(size_b!=0)
			{
			if((buffer_r[0]=='A')&(buffer_r[1]=='T')&(buffer_r[3]=='Z'))
			{
			init_obd_port=1;			
			}
			
			else if((buffer_r[0]=='G')&(buffer_r[1]=='E')&(buffer_r[2]=='T'))
			{
				if(msg_size>10)
				{
				BL_Send(deviceId,msg_size,msg_log);
				for(i=0;i<500;i++)
				msg_log[i]=0;
				msg_size=0;
				}
				else
				BL_Send(deviceId,4,"none");
			}
			else if((buffer_r[0]=='A')&(buffer_r[1]=='T')&(buffer_r[3]!='Z'))
							BL_Send(deviceId,5,"OK\r\r>");

				else
				{
					for(i=0;i<size_b;i++)
					k_cmd4[i]=hexStringToBytes(&buffer_r[3*i]);
					obd_send_flag=1;
					size_obd=size_b;
					obd_count=0;
					send_cmd_obd=1;
			 
				}
				size_b=0;
			}
			

			
			
			}
	

		}
		

	

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
	
// enable whatch dog
IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  IWDG_SetPrescaler(IWDG_Prescaler_32);
  IWDG_SetReload(4000);
  IWDG_ReloadCounter();
  IWDG_Enable();


 xTaskCreate( OBD_Task , ( signed char * ) NULL , 3000 , NULL , 3 , NULL );
 xTaskCreate( BL_Task , ( signed char * ) NULL , 3000 , NULL , 2 , NULL );	
 vTaskStartScheduler();
	

	
	
    while (1);

}




#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

