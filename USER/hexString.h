/*********************************************************************************************************
*
* File                : hexstring.h
* Version             : V1.0
* By                  : alireza roozitalab
* email               : alireza.roozitalab@gmail.com
* git user            : ARoozitalab
*                                                
*                                          All Rights Reserved
*
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/


#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t hexStringToBytes(char *inhex);

void bytesToHexString(uint8_t *bytesin,uint8_t hex);
