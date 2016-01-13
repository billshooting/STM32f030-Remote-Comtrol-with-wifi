#ifndef __USART_H
#define __USART_H
#include <stdio.h>
#include "stm32f0xx.h"
#include <string.h>
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_conf.h"
#include "stm32f0xx_misc.h"

void USART_Configuration(uint32_t baudrate);
void USART_IT_Init(void);

void USART_Send_Enter(void);
void USART_Put_Inf(char *inf,uint32_t dat);
void USART_Put_Num(uint32_t dat);
void u32tostr(uint32_t dat,char *str); 
void USART_SendString(const char* data);
char* USART_ReceiveString(uint16_t number);

#endif