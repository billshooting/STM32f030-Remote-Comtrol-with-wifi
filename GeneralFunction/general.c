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
	init.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
	init.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_Init(GPIOA, &init);
	
}

//配置EXTI0_IRQn 和EXTI1_IRQn 外部中断给key1 和key2 响应它们的按下事件

void Key_It_Init()
{
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   //设置NVIC中断分组2:2位抢占优先级，2位响应优先级

	
	
	   /****************  EXTI0_IRQn中断配置  ***************/
	    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0); //把GPIOA的Pin0设置为EXTI的输入线
   	  EXTI_InitStructure.EXTI_Line=EXTI_Line0;                   //中断线0
  	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	       //中断模式设置
  	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;    //下降沿触发
  	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	  EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
		
		  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键所在的外部中断通道
  	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级2 
  	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;		//子优先级2
  	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能外部中断通道
  	  NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	    /****************  EXTI1_IRQn中断配置  ***************/
	    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
  	  EXTI_InitStructure.EXTI_Line=EXTI_Line1;                //中断线1
  	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //下降沿触发
  	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	  EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
		
		  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//使能按键所在的外部中断通道
  	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;			//子优先级1
  	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
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
	init.GPIO_Mode = GPIO_Mode_Out_PP; //上拉输入
	init.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &init);
}
#endif
/**********************************************************     End LED Def    **********************************************************/