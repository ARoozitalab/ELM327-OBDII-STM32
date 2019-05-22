/*
  ******************************************************************************
  * @file   		 Config.h
  * @author  		alireza roozitalab
  * @version 		V0.0.1
  * @date    		10-azar-1394
  * @brief   		for gonfig pins
	* @project  	mr kokabi home automative solution
	******************************************************************************
	*/


#ifndef _CONFIG_H
#define _CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
	 

	 
	 
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include  <stdint.h>
#include "misc.h"
#include "stm32f10x_exti.h"
	 
	 
	 
//-----------------------------------------------------------------DEFINE ID
	 
	 
#define	 GUI_ID		0X4568
#define	 FRM1    	0X1
#define	 FRM2	 	 	0X2
	 
	 
	 
#define MODULE_NUM	8
	 
	 
//------------------------------------------------------------------MODBUS CONFIG PINS
#define RX_PIN               	 	GPIO_Pin_11
#define RX_PORT            		 	GPIOC
#define RX_GPIO_CLK            	RCC_APB2Periph_GPIOC


#define TX_PIN               	  GPIO_Pin_10
#define TX_PORT            			GPIOC
#define TX_GPIO_CLK             RCC_APB2Periph_GPIOC



#define Applicationend 0x08040000
#define ApplicationAddress    0x8003000
#define FLASH_PAGE_SIZE    ((uint16_t)0x400)

	
	

//-----------------------------------------------------------------TIMER CONFIG
#define TIME_PERIOD               	  (uint16_t) (SystemCoreClock / 1000) - 1


	 
	 





#define TICK_PPS					1000


#if (TICK_PPS<1000)

	#define  ms_devider			1
	#define  ms_multiple		(1000/TICK_PPS)
	#define  us_devider			1
	#define  us_multiple		(1000000/TICK_PPS)
	
#endif

#if (TICK_PPS>=1000)&&(TICK_PPS<=1000000)

	#define  ms_devider			(TICK_PPS/1000)
	#define  ms_multiple		1
	#define  us_devider			1
	#define  us_multiple		(1000000/TICK_PPS)
#endif

#if (TICK_PPS>1000000)

	#define  ms_devider			(TICK_PPS/1000)
	#define  ms_multiple		1
	#define  us_devider			(TICK_PPS/1000000)
	#define  us_multiple		1
	
#endif

#ifdef __cplusplus
}
#endif

#endif 
