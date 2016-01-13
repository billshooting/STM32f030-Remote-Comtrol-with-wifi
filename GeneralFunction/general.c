#include "general.h"

/**********************************************************   Key Definition   **********************************************************/
#ifdef STM32_KEY
//Interrupt Key Mode
void Key_Init()
{
	//Key_GPIO_Init();
	Key_It_Init();
}
void Key_GPIO_Init()
{
	GPIO_InitTypeDef init;
	init.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	init.GPIO_Mode = GPIO_Mode_IPU; //��������
	init.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_Init(GPIOA, &init);
	
}

//����EXTI0_IRQn ��EXTI1_IRQn �ⲿ�жϸ�key1 ��key2 ��Ӧ���ǵİ����¼�

void Key_It_Init()
{
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

	
	
	   /****************  EXTI0_IRQn�ж�����  ***************/
	    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0); //��GPIOA��Pin0����ΪEXTI��������
   	  EXTI_InitStructure.EXTI_Line=EXTI_Line0;                   //�ж���0
  	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	       //�ж�ģʽ����
  	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;    //�½��ش���
  	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	  EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
		
		  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�2 
  	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;		//�����ȼ�2
  	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ⲿ�ж�ͨ��
  	  NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	    /****************  EXTI1_IRQn�ж�����  ***************/
	    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
  	  EXTI_InitStructure.EXTI_Line=EXTI_Line1;                //�ж���1
  	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //�½��ش���
  	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	  EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
		
		  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;			//�����ȼ�1
  	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
			NVIC_Init(&NVIC_InitStructure); 
}

#endif
/**********************************************************     End Key Def    **********************************************************/


/**********************************************************     LED Key Def    **********************************************************/
#ifdef LED
void Led_Init()
{
	GPIO_InitTypeDef init;
	init.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	init.GPIO_Mode = GPIO_Mode_Out_PP; //��������
	init.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &init);
}
#endif
/**********************************************************     End LED Def    **********************************************************/