/*********************************************************************************************************
*
* File                : blu.h
* Version             : V1.0
* By                  : alireza roozitalab
* email               : alireza.roozitalab@gmail.com
* git user            : ARoozitalab
*                                                
*                                          All Rights Reserved
*
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
 
 
#ifndef BLU_H
#define BLU_H

#ifdef __cplusplus
 extern "C" {
#endif	 
#include "stm32f10x.h"
#include <stdio.h>	 

#define buf_Size 			200 
extern uint16_t AT_count;
extern char AT_buf[buf_Size];

extern uint8_t deviceId;
	 
void BL_init();	 
void BL_ClearPairList(uint8_t 	retry );
void BL_ClearPairDevice(uint8_t id);
uint8_t BL_TurnON();
uint8_t BL_TurnOff();
void BL_Status();
void BL_AcceptPairRequest();
void BL_AcceptConnection();
void BL_Send(uint8_t conId,uint8_t size, char* data);
void BL_Recive(char* buffer,char* size);	 
 
	 
#ifdef __cplusplus
}
#endif

#endif
