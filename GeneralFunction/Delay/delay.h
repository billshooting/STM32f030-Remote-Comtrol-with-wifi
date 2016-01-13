#ifndef DELAY_H
#define DELAY_H


#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f0xx.h"

void delay_init(void);
void delay_ms(uint16_t xms);
void delay_us(uint32_t xus);


#ifdef __cplusplus
  } 
#endif
	
	
#endif  //End of file Delay.h