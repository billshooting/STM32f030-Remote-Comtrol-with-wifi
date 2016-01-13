#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "smarthome.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_tim.h"
#include "delay.h"
#include "usart.h"

/* 函数声明 */
void GPIO_Configuration(void);
void RCC_Configuration(void);

/* 控制参数声明 */
typedef enum {STOP = 0x00, FORWARD = 0x01, REVERSE = 0x02} MotorState;

static MotorState motorState; //电机的状态：正转，反转，停
static bool isChanged;        //表明当前状态是否被改变
static uint8_t usartState;    //用于记录哪个器件被选中
static uint8_t runState;      //用于记录各个器件的开关状态
volatile uint16_t counter;    //用于RxBuffer 计数


uint8_t RxBuffer[20];        //用于接收串口数据 其格式为"+IPD,0,3:T,S" 共12个数据

int main()
{
	RCC_Configuration();
	delay_init();
	USART_IT_Init();
	USART_Configuration(115200);
	GPIO_Configuration();
	Timer_Init();
	
	/* 由于wifi模块上电初始化速度比MCU慢，必须要等待一段时间才能对wifi进行初始化 */
	delay_ms(500);
	
	/* 状态初始化 */
  motorState = STOP;
	isChanged = false;
	usartState = 0x00;
	runState = 0x00;
	counter = 0;
	
	/* Wifi 模块初始化，配置为服务器模式和多连接模式 */
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	Wifi_Init();
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	/* 采用前后台模式，后台死循环，依据标志做相应处理；前台为串口中断，响应wifi模块的数据 */
	while(1)
	{
		if(isChanged)
		{
			usartState = RxBuffer[9] - 0x30;             /* 由于ASCII码的原因要减去一个数 */
			runState = RxBuffer[11] - 0x30;
			switch((Facility)usartState)
			{
				case NONE:                                               /* 发送为空的情况 */
				  break;
				case MOTOR:                                            /* 发送为电机的情况 */
				{
					if(runState == STOP)
					{
						MotorStop();
//						USART_SendString("AT+CIPSEND=0,16\r\n");
//						USART_SendString("Motor stopped.\r\n");
						break;
					}
					else if(runState == FORWARD)
					{
						MotorForward();
//						USART_SendString("AT+CIPSEND=0,20\r\n");
//						USART_SendString("Motor is rotating.\r\n");
						break;
					}
					else if(runState == REVERSE)
					{
						MotorReverse();
//						USART_SendString("AT+CIPSEND=0,21\r\n");
//						USART_SendString("Motor is reversing.\r\n");
						break;
					}
				  break;
				}
				case REDLED:                                           /* 发送为红灯的情况 */  
				{
					LED_Switch((Facility)REDLED,(bool)!runState);
//					USART_SendString("AT+CIPSEND=0,19\r\n");
//					USART_SendString("Red led switched.\r\n");
				  break;
				}
				case GREENLED:                                         /* 发送为绿灯的情况 */
				{
					LED_Switch((Facility)GREENLED, (bool)!runState);
//					USART_SendString("AT+CIPSEND=0,21\r\n");
//					USART_SendString("Green led switched.\r\n");
				  break;
				}
				case MULTICOLOR:                                       /* 发送为彩灯的情况 */
				{
					LED_Switch((Facility)MULTICOLOR, (bool)runState);
//					USART_SendString("AT+CIPSEND=0,26\r\n");
//					USART_SendString("Multicolor led switched.\r\n");
				  break;
				}
				case BEEP:	                                           /* 发送为蜂鸣的情况 */
				{					
 				  BeepCmd((bool)runState);
//					USART_SendString("AT+CIPSEND=0,6\r\n");
//					USART_SendString("Beep swtiched.\r\n");
				  break;
				}
				default:
					break;
			}
			isChanged = false;
		}
	}
	
}

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 配置LED引脚 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	
	/* 配置红灯PA4 绿灯PA5 这两路实际上为继电器*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_5);
	
	/* 配置电机输出引脚 PA6,PA7 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* 配置BEEP引脚 PB1 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* 配置引脚复用，STM32f0xx系列特有 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
	
	/* 配置USART1 TX PA9 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*配置USART1 RX PA10 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

void RCC_Configuration(void)
{
	SystemInit();
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
}



void USART1_IRQHandler(void)
{
	/* WARNING: in DEBUG mode, DO NOT set your breakpoint in interrupt handler, it will ruin everything!!! */
	if((USART_GetITStatus(USART1, USART_IT_RXNE) == SET))
	{
		
		RxBuffer[counter++] = (uint8_t)USART_ReceiveData(USART1);
		if((RxBuffer[0] != '+'))
		{
			counter = 0;
		}
		if(counter == 13)
		{
			isChanged = true;
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
			counter = 0;
			
		}
	}
	
	/* 由于STM32 usart的一个硬件bug，必须存在以下语句，清空FLAG_ORE,避免无限进入RXNE中断 */
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) != RESET)
	{
		USART_ClearFlag(USART1, USART_FLAG_ORE);
		USART_ReceiveData(USART1);
	}
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		GPIOB->ODR ^= GPIO_Pin_1;  //对PB1进行取反
	}
}

