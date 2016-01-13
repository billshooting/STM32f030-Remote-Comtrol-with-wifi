#include "delay.h"
#include "core_cm0.h"

static uint8_t  fac_us=0;//us微秒延时倍乘数
static uint16_t fac_ms=0;//ms毫秒延时倍乘数

void delay_init()	 
{

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//可以查看用户手册，此处为选择外部时钟  HCLK/8
	fac_us = SystemCoreClock/8000000;	//SystemCoreClock为系统时钟，此处定义fac_us是系统时钟的1/8  

/* reload为24位寄存器,最大值:2的24次方1024*1024*16=16777216,在72M下,约合1.864s左右
  因为systick是72M的8分之一即9M  那么16777216/9M = 1.864s*/	 

	fac_ms = (uint16_t)fac_us*1000; //非ucos下,代表每个ms需要的systick时钟数   
}

	    								   
void delay_us(uint32_t xus)	//延时xus个微秒
{		
	uint32_t temp;	    	 
	SysTick->LOAD=xus*fac_us;                         //时间加载	  		 
	SysTick->VAL=0x00;                                //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;       	  //设置开始倒数计时的掩码

	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));            //等待时间到达   
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器

	SysTick->VAL =0X00;                            //清空计数器	 
}

//延时xms  注意xms的范围  SysTick->LOAD为24位寄存器,所以,最大延时为:xms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,xms单位为ms   对72M条件下,xms<=1864 
void delay_ms(uint16_t xms)
{	 		  	  
	uint32_t temp;		   
	SysTick->LOAD=(uint32_t)xms*fac_ms;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  	    
} 