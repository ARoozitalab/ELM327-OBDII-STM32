/*********************************************************************************************************
*
* File                : hexstring.c
* Version             : V1.0
* By                  : alireza roozitalab
* email               : alireza.roozitalab@gmail.com
* git user            : ARoozitalab
*                                                
*                                          All Rights Reserved
*
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "hexString.h"


/*******************************************************************************
* Function Name  : hexStringToBytes
* Description    : convert string to value
* Input          : inhex 
* Output         : None
* Return         : value1 
*******************************************************************************/			
uint8_t hexStringToBytes(char *inhex)
{
	uint8_t value1=0,value2=0;

	
	value1=inhex[0]-48;
	if (value1>10)
		value1=value1-7;
	
	value2=inhex[1]-48;
	if (value2>10)
			value2=value2-7;	
	
	
	value1=(value1<<4)|value2;
	return value1;
}

/*******************************************************************************
* Function Name  : bytesToHexString
* Description    : convert value to string
* Input          : hex,bytesin
* Output         : None
* Return         : None 
*******************************************************************************/	
void bytesToHexString(char *bytesin,uint8_t hex)
{
	uint8_t value1=0,value2=0;
	
	value1=hex>>4;
	value2=hex&0x0f;
	if(value1<10)
		bytesin[0]=value1+48;
	else
		bytesin[0]=value1+55;
	
	
		if(value2<10)
		bytesin[1]=value2+48;
		else
		bytesin[1]=value2+55;
		
	bytesin[2]=0x20;
}
