#include "smarthome.h"

void LED_Switch(Facility LEDx, bool state)
{
	switch(LEDx)
	{
		case 0x02:
			GPIO_WriteBit(GPIOA, GPIO_Pin_4,(BitAction)state);
		  break;
		case 0x04:
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)state);
		  break;
		case 0x08:
			GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)state);
		  GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)state);
	    break;
		default:
			return;
	}
}


void LED_OFF()
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, RESET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_1, RESET);
}

void Wifi_Init()
{
	delay_ms(10);
	USART_SendString("AT+CIPMUX=1\r\n");    //����Ϊ������ģʽ
	delay_ms(10);
	USART_SendString("AT+CIPSERVER=1\r\n"); //����Ϊ������ģʽ
	delay_ms(5);
	USART_ReceiveData(USART1);
	USART_ClearFlag(USART1, USART_FLAG_RXNE);
}

void Timer_Init()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	
	/* ���ü�ʱ����ʱ�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x03;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* APB1 ����ʱ��Ϊ48MHz����Ƶϵ��Ϊ1����������Ƶ 24Mhz
	** Ԥװ����ֵΪ48000����������¼�ÿ 2ms ����һ�� */
	TIM_TimeBaseStructure.TIM_Prescaler = 2 - 1;
	TIM_TimeBaseStructure.TIM_Period = 48000 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, DISABLE);
}

void BeepCmd(bool state)
{
	TIM_Cmd(TIM3, (FunctionalState)state);
	if(!state)
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);
}

void MotorForward()
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, SET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, RESET);
}

void MotorReverse()
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, RESET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, SET);
}

void MotorStop()
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, RESET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, RESET);
}