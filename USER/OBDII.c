
/*********************************************************************************************************
*
* File                : OBDII.c
* Version             : V1.0
* By                  : alireza roozitalab
* email               : alireza.roozitalab@gmail.com
* git user            : ARoozitalab
*                                                
*                                          All Rights Reserved
*
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include <OBDII.h>

uint8_t c_dt[8]={0};
uint8_t c_rcv_flag=0;
uint8_t flasg_t=0;
uint16_t init_obd=0;
uint8_t obd_turn_on=0;
uint8_t obd_data[obd_size_buf]=
	{
	0
	};
char obd_count=0;
char OBD_RCV_T=0;	
	
	
	
	
	
/*******************************************************************************
* Function Name  : can_send
* Description    : send data through can protocol (ISO 15765)
* Input          : data (data),data_l (data length),id
* Output         : None
* Return         : None
*******************************************************************************/			
	
uint8_t can_send(uint16_t id , uint8_t data_l,char *data)	
{
	uint8_t a;
		uint8_t t_out=0;
	CanTxMsg TxMessage;

		a=0;
	
	TxMessage.StdId=id;
  TxMessage.RTR=CAN_RTR_DATA;
  TxMessage.IDE=CAN_ID_STD;
  TxMessage.DLC=data_l;
  TxMessage.Data[0]=data[0];
  TxMessage.Data[1]=data[1];
	TxMessage.Data[2]=data[2];
	TxMessage.Data[3]=data[3];
	TxMessage.Data[4]=data[4];
	TxMessage.Data[5]=data[5];
	TxMessage.Data[6]=data[6];
	TxMessage.Data[7]=data[7];	
		
  a=CAN_Transmit(CAN1, &TxMessage);
	
	while(((CAN_TransmitStatus(CAN1, a) != CANTXOK))&&(t_out<100))
	{
		vTaskDelay(3);
		t_out++;
	}
	if(t_out==100)
	{
		CAN_CancelTransmit(CAN1, a);
		return 0;	
	}	
	c_rcv_flag=0;
  return 1;	
}



/*******************************************************************************
* Function Name  : can_rcv
* Description    : receive data through can protocol (ISO 15765)
* Input          : data_l(data length),data(data)
* Output         : None
* Return         : None
*******************************************************************************/	

uint8_t can_rcv(uint8_t *data_l,char *data)	
{
		uint8_t t_out=0;
			uint8_t i=0;
while((!c_rcv_flag)&&(t_out<100))
{
	vTaskDelay(40);
	t_out++;
}
if (t_out==100)
	return 0 ;
if((c_dt[1]==0x41))
{
for(i=1;i<=c_dt[0];i++)
	{
  data[i-1]=c_dt[i];
		}
	data_l[0]=c_dt[0];
	return 1 ;
}
	return 0 ;
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
/*******************************************************************************
* Function Name  : K_line_RCV
* Description    : receive data through serial protocol (ISO 9141-2)
* Input          : rsp (response address to store) ,size(size of recive to store)
* Output         : None
* Return         : None
*******************************************************************************/		
	
void K_line_RCV(char *rsp,char *size)	
{
	uint32_t timeout=0;
	int i=0;

	if((dif_count_obd())&(obd_count==(obd_data[0]&0xf)+4))
	{
		
	size[0]=obd_data[0]&0xf;
	for(i=0;i<size[0];i++)
	rsp[i]=obd_data[i+3];
		
	for(i=0;i<obd_size_buf;i++)
	obd_data[i]=0;
	obd_count=0;
	}
	else if((dif_count_obd())&((obd_data[0]==0x80)|(obd_data[3]==obd_count-5)))
	{
		size[0]=obd_data[3];
		for(i=0;i<size[0];i++)
		rsp[i]=obd_data[i+4];
			
		for(i=0;i<obd_size_buf;i++)
		obd_data[i]=0;
		obd_count=0;		
	}		
	else if((dif_count_obd())&&(obd_count>0)&&((obd_data[0]&0x80)!=0x80))
	{
		for(i=0;i<15;i++)
		obd_data[i]=0;
		obd_count=0;				
	}
	
	else
	{
	size[0]=0;
	}
		if(obd_count>20)
		size[0]=0;
}
	
	
	
/*******************************************************************************
* Function Name  : K_line_send
* Description    : send data through serial protocol (ISO 9141-2)
* Input          : rsp (response address to store) ,size(size of recive to store)
* Output         : None
* Return         : None
*******************************************************************************/		
void K_line_send(char *rsp,char size)
{
	int i=0;
	char crc=0;
		OBD_RCV_T=0;
	
	while(!dif_count_obd());
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
  USART_SendData(USART1,0xc0+size);			
	crc=0xc0+size;
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
  USART_SendData(USART1,0x33);		
	crc=crc+0x33;
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
  USART_SendData(USART1,0xf1);	
	crc=crc+0xf1;

	for(i=0;i<size;i++)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		USART_SendData(USART1,rsp[i]);
		crc=crc+rsp[i];

	}
	
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
  USART_SendData(USART1,crc);	
	 while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	for(i=0;i<obd_size_buf;i++)
	obd_data[i]=0;
	
	while(!dif_count_obd());
		OBD_RCV_T=1;
	setms_timecount_obd2(4000);
	obd_count=0;
}


/*******************************************************************************
* Function Name  : K_line_slow_init
* Description    : slow init method serial protocol (ISO 9141-2)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/		
	void K_line_slow_init(void)
	{ 
	GPIO_InitTypeDef GPIO_InitStructure;	
uint16_t temp=0;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA,ENABLE);
		GPIO_SetBits(GPIOA,GPIO_Pin_9);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);		   
 
		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
  TIM_DeInit(TIM2);
  TIM_TimeBaseStructure.TIM_Period=1000;
  TIM_TimeBaseStructure.TIM_Prescaler= (36 - 1);		
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);							 
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM2, ENABLE);	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	
		


	while(init_obd!=600);
	GPIO_ResetBits(GPIOA,GPIO_Pin_9);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	
	while(init_obd!=1000);
	GPIO_SetBits(GPIOA,GPIO_Pin_9);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	
	while(init_obd!=1800);
	GPIO_ResetBits(GPIOA,GPIO_Pin_9);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	
	while(init_obd!=2600);
	GPIO_SetBits(GPIOA,GPIO_Pin_9);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	
	while(init_obd!=3400);
	GPIO_ResetBits(GPIOA,GPIO_Pin_9);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	
	while(init_obd!=4200);
	GPIO_SetBits(GPIOA,GPIO_Pin_9);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	obd_count=0;
//while(init_obd!=4700);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);		 
	
	OBD_RCV_T=1;

while((obd_count<2)&&(init_obd<4880));


if(obd_count==2)
	{
		temp=init_obd;
		OBD_RCV_T=0;	
	while(init_obd!=(temp+80));
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
  USART_SendData(USART1,0x70);
	while(init_obd!=(temp+160));
	
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
  USART_SendData(USART1,0xcc);
	while(init_obd!=(temp+760));
		obd_count=0;
	}


	TIM_DeInit(TIM2);
	init_obd=0;
	}
	
/*******************************************************************************
* Function Name  : K_line_fast_init
* Description    : fast init method serial protocol (ISO 9141-2)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/	
void K_line_fast_init(void)
	{ 
	GPIO_InitTypeDef GPIO_InitStructure;	
uint16_t temp=0;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA,ENABLE);
		GPIO_SetBits(GPIOA,GPIO_Pin_9);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);		   
 	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
  TIM_DeInit(TIM2);
  TIM_TimeBaseStructure.TIM_Period=1000;
  TIM_TimeBaseStructure.TIM_Prescaler= (36 - 1);		
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);							 
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM2, ENABLE);	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	
		
	while(init_obd!=600);
	GPIO_ResetBits(GPIOA,GPIO_Pin_9);
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
	while(init_obd!=650);
	GPIO_SetBits(GPIOA,GPIO_Pin_9);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	while(init_obd!=700);

	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);		 
	TIM_DeInit(TIM2);
	init_obd=0;
	}	
/*******************************************************************************
* Function Name  : OBD_ecu_Init
* Description    : select obdii init method (slow ISO 9141-2 , or fast ISO 9141-2,ISO 15765  )
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/	
void OBD_ecu_Init(void)
{
	uint8_t size_R=0;	
	char Data1[2]={0x1,0x0c};	
	char Data2[4]={0,0,0,0};
		char Data3[8]={2,1,0,0,0,0,0,0};
		char Data4[8]={2,1,0x46,0,0,0,0,0};
		char Data5[8]={0,0,0,0,0,0,0,0};
	char test_ecu=0;		
	char init=0;
		while(init==0)
		{
			vTaskDelay(5000);
			if(obd_turn_on==1)
			{				
			K_line_fast_init();
			Data1[0]=0x81;
			Data1[1]=0;
			OBD_Send_Recive_ecu(Data1,1,Data2,&size_R);
		//K_line_RCV(k_cmd2,&size_R);	
		 if(size_R==3)
		 {
			init=1;
			flasg_t=KWP_Flag; 
			 break;
		 }
			else
			{
			vTaskDelay(1000);
			K_line_slow_init();
			Data1[0]=0x01;
			Data1[1]=0x0C;
			OBD_Send_Recive_ecu(Data1,2,Data2,&size_R);			
		 if(size_R==4)
		 {
			init=1;
			flasg_t=KWP_Flag; 
			 break;
		 }
		if(can_send(0x7DF,8,Data3))
		{
				 	vTaskDelay(1000);
		 if(can_send(0x7DF,8,Data4))
			 if(can_rcv(&size_R,Data5))
			 {
				 			init=1;
			 		flasg_t=CAN_Flag; 
			 			 break;
			 }
		}

			test_ecu++;
			}
			}
			 if (test_ecu>1)
			 {
				obd_turn_on=0;
				bus_off();
				test_ecu=0;
			}
			
		}



}
	
	
	
	
	
	
	
	
	
	
	
/*******************************************************************************
* Function Name  : OBD_Send_Recive_ecu
* Description    : send and Recive OBDII data high layer
* Input          : send_buf,send_buf_s,rcv_buf,rcv_buf_s
* Output         : None
* Return         : None
*******************************************************************************/	
uint8_t OBD_Send_Recive_ecu(char *send_buf,char send_buf_s,char *rcv_buf,char *rcv_buf_s )
	{
	
			uint16_t time_R=0;
			static char ecu_fail=0;
		
			K_line_send(send_buf,send_buf_s);
		while(((!obd_count)&(time_R<OBD_RSP_TIME_OUT_ecu))|(!dif_count_obd()))
		{
			vTaskDelay(1);
			time_R++;
		}
		if(time_R==OBD_RSP_TIME_OUT)
		{		
			return TIME_OUT_ERROR;
		}
				
		ecu_fail=0;	
	K_line_RCV(rcv_buf,&rcv_buf_s[0]);			
	
	if(rcv_buf[0]==0X7F)
	return RSP_ERROR;
	
	return TURE_RSP;	
	}

	
	////////////////////////////////////////////////////	
	
/*******************************************************************************
* Function Name  : OBD_Serial_Init
* Description    : micro serial init for ISO 9141-2
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/	
void OBD_Serial_Init(void)
	{
	NVIC_InitTypeDef NVIC_InitStructure;	
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure; 

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
  /*
  *  USART1_TX -> PA9 , USART1_RX ->	PA10
  */				
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);		   

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 10400;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure); 
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART1, USART_IT_TXE,DISABLE);
  USART_ClearFlag(USART1,USART_FLAG_TC);
  USART_Cmd(USART1, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	USART_ClearFlag(USART1,USART_FLAG_RXNE);
	USART_ClearFlag(USART1,USART_FLAG_TC);
	USART_Cmd(USART1, ENABLE);
	
		
	}
	
/*******************************************************************************
* Function Name  : can_Configuration
* Description    : micro serial init for ISO 15765
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/	
	
	void can_Configuration(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
	
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE); 

				 
  GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);
  
  /* Configure CAN pin: TX */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Configure CAN pin: TX */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);		
  /* CAN register init */
  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_StructInit(&CAN_InitStructure);

  /* CAN1 and CAN2  cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = ENABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	
	
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;  
  CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq;
  CAN_InitStructure.CAN_Prescaler =8;
//  CAN_InitStructure.CAN_Prescaler =16;
  CAN_Init(CAN1, &CAN_InitStructure);
  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 1;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
   CAN_FilterInitStructure.CAN_FilterIdHigh = 0xE000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xE000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
	


  /* Enable CAN1 RX0 interrupt IRQ channel */
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	  CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);
}

	
	
	
/*******************************************************************************
* Function Name  : bus_off
* Description    : sleep bus
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/	
void bus_off(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;	
   uint16_t temp=0;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);		   
 
		GPIO_SetBits(GPIOA,GPIO_Pin_9);
  	GPIO_ResetBits(GPIOC,GPIO_Pin_13);	

}
		
	