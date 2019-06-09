
/*********************************************************************************************************
*
* File                : OBDII.h
* Version             : V1.0
* By                  : alireza roozitalab
* email               : alireza.roozitalab@gmail.com
* git user            : ARoozitalab
*                                                
*                                          All Rights Reserved
*
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/






#ifndef __OBDII_H
#define __OBDII_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h" 
#include "stm32f10x_it.h"	 
#define OBD_RSP_TIME_OUT	0x2fffff 
#define obd_size_buf   250	
	
#define CAN_Flag   1
#define KWP_Flag  2	 
	
#define TIME_OUT_ERROR    3
#define RSP_ERROR					0
#define TURE_RSP	 				1

//PC13 ----- ISO K
//PA9  ----- ISO L _ SEND
//PA10  ----- ISO L _ RCV



#define OBD_RSP_TIME_OUT_ecu	5000
extern uint16_t init_obd;

extern uint8_t obd_data[obd_size_buf];
extern uint8_t obd_count;
extern uint8_t OBD_RCV_T;	 
	 
void K_line_RCV(uint8_t *rsp,uint8_t *size);	
void K_line_fast_init(void);
void K_line_slow_init(void);
void K_line_send(uint8_t *rsp,uint8_t size);
void OBD_Serial_Init(void);
void bus_off(void);	 
uint8_t OBD_Send_Recive_ecu(uint8_t *send_buf,uint8_t send_buf_s,uint8_t *rcv_buf,uint8_t *rcv_buf_s );
void OBD_ecu_Init(void);
uint8_t can_send(uint16_t id , uint8_t data_l,uint8_t *data);
uint8_t can_rcv(uint8_t *data_l,uint8_t *data)	;
uint8_t OBD_Send_Recive(uint8_t *send_buf,uint8_t send_buf_s,uint8_t *rcv_buf,uint8_t *rcv_buf_s );
#ifdef __cplusplus
}
#endif

#endif

