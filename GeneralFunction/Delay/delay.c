#include "delay.h"
#include "core_cm0.h"

static uint8_t  fac_us=0;//us΢����ʱ������
static uint16_t fac_ms=0;//ms������ʱ������

void delay_init()	 
{

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//���Բ鿴�û��ֲᣬ�˴�Ϊѡ���ⲿʱ��  HCLK/8
	fac_us = SystemCoreClock/8000000;	//SystemCoreClockΪϵͳʱ�ӣ��˴�����fac_us��ϵͳʱ�ӵ�1/8  

/* reloadΪ24λ�Ĵ���,���ֵ:2��24�η�1024*1024*16=16777216,��72M��,Լ��1.864s����
  ��Ϊsystick��72M��8��֮һ��9M  ��ô16777216/9M = 1.864s*/	 

	fac_ms = (uint16_t)fac_us*1000; //��ucos��,����ÿ��ms��Ҫ��systickʱ����   
}

	    								   
void delay_us(uint32_t xus)	//��ʱxus��΢��
{		
	uint32_t temp;	    	 
	SysTick->LOAD=xus*fac_us;                         //ʱ�����	  		 
	SysTick->VAL=0x00;                                //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;       	  //���ÿ�ʼ������ʱ������

	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));            //�ȴ�ʱ�䵽��   
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����

	SysTick->VAL =0X00;                            //��ռ�����	 
}

//��ʱxms  ע��xms�ķ�Χ  SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:xms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,xms��λΪms   ��72M������,xms<=1864 
void delay_ms(uint16_t xms)
{	 		  	  
	uint32_t temp;		   
	SysTick->LOAD=(uint32_t)xms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	  	    
} 