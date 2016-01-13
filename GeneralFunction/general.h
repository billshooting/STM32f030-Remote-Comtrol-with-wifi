#ifndef GENERAL_H
#define GENERAL_H
#include "stm32f10x.h"
#include "pinmacro.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "stm32f10x_rcc.h"

/**********************************************************   Macro choice   **********************************************************/
       /**************************   !!!Choose the function you need to reduce code quantity!!!   **************************/
       /**************************************   !!!Comment the Macro to choose!!!   **************************************/
#define STM32_KEY
#define DELAY
#define USART
#define LED

/**********************************************************   Macro choice   **********************************************************/



/**********************************************************   Key Definition   **********************************************************/

#ifdef STM32_KEY
//Interrupt Key Mode
void Key_Init();
void Key_GPIO_Init();
void Key_It_Init();
#endif
/**********************************************************     End Key Def    **********************************************************/


/**********************************************************   LED Definition   **********************************************************/
#ifdef LED

#define LED1 PAOUT(2)
#define LED2 PAOUT(3)
void Led_Init();
#endif
/**********************************************************    End LED Def    **********************************************************/

#endif