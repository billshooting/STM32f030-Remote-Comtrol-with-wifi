/* This file includes the functions and definitions
** used in SmartHome Demo 
** Modified Time: 2015/12/25 
*/


#ifndef SMARTHOME_H
#define SMARTHOME_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f0xx_tim.h"
#include "stm32f0xx_gpio.h"
#include "delay.h"
	 
typedef enum {NONE =0x00, MOTOR= 0x01, REDLED = 0x02, GREENLED = 0x04, MULTICOLOR =0x08, BEEP = 0x09}Facility;
typedef enum {false = 0x00, true = !false} bool;

void LED_Switch(Facility LEDx, bool state);
void LED_OFF();
void Wifi_Init();
void Timer_Init();
void BeepCmd(bool state);
void MotorForward();
void MotorReverse();
void MotorStop();



	 
	 
	 
	 
	 
#ifdef __cplusplus
 }
#endif

#endif