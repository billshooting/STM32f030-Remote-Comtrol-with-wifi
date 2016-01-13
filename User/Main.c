#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "smarthome.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_tim.h"
#include "delay.h"
#include "usart.h"

/* �������� */
void GPIO_Configuration(void);
void RCC_Configuration(void);

/* ���Ʋ������� */
typedef enum {STOP = 0x00, FORWARD = 0x01, REVERSE = 0x02} MotorState;

static MotorState motorState; //�����״̬����ת����ת��ͣ
static bool isChanged;        //������ǰ״̬�Ƿ񱻸ı�
static uint8_t usartState;    //���ڼ�¼�ĸ�������ѡ��
static uint8_t runState;      //���ڼ�¼���������Ŀ���״̬
volatile uint16_t counter;    //����RxBuffer ����


uint8_t RxBuffer[20];        //���ڽ��մ������� ���ʽΪ"+IPD,0,3:T,S" ��12������

int main()
{
	RCC_Configuration();
	delay_init();
	USART_IT_Init();
	USART_Configuration(115200);
	GPIO_Configuration();
	Timer_Init();
	
	/* ����wifiģ���ϵ��ʼ���ٶȱ�MCU��������Ҫ�ȴ�һ��ʱ����ܶ�wifi���г�ʼ�� */
	delay_ms(500);
	
	/* ״̬��ʼ�� */
  motorState = STOP;
	isChanged = false;
	usartState = 0x00;
	runState = 0x00;
	counter = 0;
	
	/* Wifi ģ���ʼ��������Ϊ������ģʽ�Ͷ�����ģʽ */
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	Wifi_Init();
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	/* ����ǰ��̨ģʽ����̨��ѭ�������ݱ�־����Ӧ����ǰ̨Ϊ�����жϣ���Ӧwifiģ������� */
	while(1)
	{
		if(isChanged)
		{
			usartState = RxBuffer[9] - 0x30;             /* ����ASCII���ԭ��Ҫ��ȥһ���� */
			runState = RxBuffer[11] - 0x30;
			switch((Facility)usartState)
			{
				case NONE:                                               /* ����Ϊ�յ���� */
				  break;
				case MOTOR:                                            /* ����Ϊ�������� */
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
				case REDLED:                                           /* ����Ϊ��Ƶ���� */  
				{
					LED_Switch((Facility)REDLED,(bool)!runState);
//					USART_SendString("AT+CIPSEND=0,19\r\n");
//					USART_SendString("Red led switched.\r\n");
				  break;
				}
				case GREENLED:                                         /* ����Ϊ�̵Ƶ���� */
				{
					LED_Switch((Facility)GREENLED, (bool)!runState);
//					USART_SendString("AT+CIPSEND=0,21\r\n");
//					USART_SendString("Green led switched.\r\n");
				  break;
				}
				case MULTICOLOR:                                       /* ����Ϊ�ʵƵ���� */
				{
					LED_Switch((Facility)MULTICOLOR, (bool)runState);
//					USART_SendString("AT+CIPSEND=0,26\r\n");
//					USART_SendString("Multicolor led switched.\r\n");
				  break;
				}
				case BEEP:	                                           /* ����Ϊ��������� */
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
	
	/* ����LED���� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	
	/* ���ú��PA4 �̵�PA5 ����·ʵ����Ϊ�̵���*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_5);
	
	/* ���õ��������� PA6,PA7 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* ����BEEP���� PB1 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* �������Ÿ��ã�STM32f0xxϵ������ */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
	
	/* ����USART1 TX PA9 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*����USART1 RX PA10 */
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
	
	/* ����STM32 usart��һ��Ӳ��bug���������������䣬���FLAG_ORE,�������޽���RXNE�ж� */
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
		GPIOB->ODR ^= GPIO_Pin_1;  //��PB1����ȡ��
	}
}

