/**************************************************************
** 	火牛开发板
**	功能描述: STM32串口操作
**  版本：V1.0  
**	论坛：www.openmcu.com
**	淘宝：http://shop36995246.taobao.com/   
**  技术支持群：121939788 
***************************************************************/ 
#include "usart.h"

/*****************************************************************************
** 函数名称: USART1_Send_Byte
** 功能描述: 串口发送一个字符串
** 作  　者: Dream
** 日　  期: 2010年12月17日
*****************************************************************************/
void USART1_Send_Byte(uint16_t Data)
{ 
   	while (!(USART1->ISR & USART_FLAG_TXE));
   	USART1->TDR = (Data & (uint16_t)0x01FF);	   
}
/*****************************************************************************
** 函数名称: USART_Send_Str
** 功能描述: 串口发送字符串
				s:指向字符串的指针 
					注：如果在字符串中有'\n'，则会发送一个回车换行
** 作  　者: Dream
** 日　  期: 2010年12月17日
*****************************************************************************/
void USART_SendString(const char* data)
{
	uint16_t i;
	uint16_t len = strlen(data);
	for (i=0; i<len; i++)
	{
		//USART1_Send_Byte(data[i]);
		while (!(USART1->ISR & USART_FLAG_TXE));
   	USART1->TDR = (data[i] & (uint16_t)0x01FF);	
	}
}
/*****************************************************************************/
char* USART_ReceiveString(uint16_t number)
{
	char * temp;
	uint16_t counter = 0;
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET)
	{
		temp[counter++] = USART_ReceiveData(USART1);
		if(counter == number)
			return temp;
	}
}
/*****************************************************************************
** 函数名称: USART_Send_Enter
** 功能描述: 串口发送字符串
				串口发送0d 0a ，即回车换行
					注：此函数就是发送0d 0a这两个字节，在“超级终端”上会有回车换行的效果
** 作  　者: Dream
** 日　  期: 2010年12月17日
*****************************************************************************/
void USART_Send_Enter(void)
{
	USART1_Send_Byte(0x0d);
	USART1_Send_Byte(0x0a);
}
/*****************************************************************************
** 函数名称: USART_Put_Num
** 功能描述: 串口发送数值
				函数中会将数值转为相应的字符串，发送出去。比如 4567 转为 "4567" 
** 作  　者: Dream
** 日　  期: 2010年12月17日
*****************************************************************************/	
void USART_Put_Num(uint32_t dat)
{
 	char temp[20];
 	u32tostr(dat,temp);
 	USART_SendString(temp);
}
/*****************************************************************************
** 函数名称: USART_Put_Inf
** 功能描述: 串口发送调试信息
				inf:指向提示信息字符串的指针
             dat:一个数值，前面的提示信息就是在说明这个数值的意义 
** 作  　者: Dream
** 日　  期: 2010年12月17日
*****************************************************************************/
void USART_Put_Inf(char *inf,uint32_t dat)
{
 	USART_SendString(inf);
 	USART_Put_Num(dat);
 	USART_SendString("\n");  
}
/*****************************************************************************
** 函数名称: USART_Configuration
** 功能描述: 串口配置函数
				配置STM32串口1 波特率115200 
** 作  　者: Dream
** 日　  期: 2010年12月17日
*****************************************************************************/
void USART_Configuration(uint32_t baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate            = baudrate  ;	  //波特率115200
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b; //8位数据
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;	 //停止位1位
	USART_InitStructure.USART_Parity              = USART_Parity_No ;	 //无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_Cmd(USART1, ENABLE);     
}
/*****************************************************************************
** 函数名称: USART_IT_Init
** 功能描述: 配置串口中断
** 作  　者: Dream
** 日　  期: 2010年12月17日
*****************************************************************************/
void USART_IT_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			//使能按键所在的外部中断通道
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;//抢占优先级2 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
}
/*****************************************************************************
** 函数名称: USART1_IRQHandler
** 功能描述: 串口中断函数
** 作  　者: Dream
** 日　  期: 2010年12月17日
*****************************************************************************/

/*****************************************************************************
** 函数名称: u32tostr
** 功能描述: 将一个32位的变量dat转为字符串，比如把1234转为"1234"
				dat:带转的long型的变量
             str:指向字符数组的指针，转换后的字节串放在其中
** 作  　者: Dream
** 日　  期: 2010年12月17日
*****************************************************************************/
void u32tostr(uint32_t dat,char *str) 
{
 	char temp[20];
 	uint8_t i=0,j=0;
 	i=0;
 	while(dat)
 	{
  		temp[i]=dat%10+0x30;
  		i++;
  		dat/=10;
 	}
 	j=i;
 	for(i=0;i<j;i++)
 	{
  		str[i]=temp[j-i-1];
 	}
 	if(!i) {str[i++]='0';}
 	str[i]=0;
}

#ifndef MicroLIB
//#pragma import(__use_no_semihosting)             //没有实现fgetc时需要声明该参数   
/* 标准库需要的支持函数 使用printf()调试打印不需要实现该函数 */               
struct __FILE 
{ 
	int handle; 
    /* Whatever you require here. If the only file you are using is */    
    /* standard output using printf() for debugging, no file handling */    
    /* is required. */
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
/* 重定义fputc函数 如果使用MicroLIB只需要重定义fputc函数即可 */  
int fputc(int ch, FILE *f)
{
    /* Place your implementation of fputc here */
    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    {}

    /* e.g. write a character to the USART */
    USART_SendData(USART1, (uint8_t) ch);

    return ch;
}
/*
可以直接使用putchar
不需要再定义 int putchar(int ch)，因为stdio.h中有如下定义
 #define putchar(c) putc(c, stdout)
*/

int ferror(FILE *f) {  
  /* Your implementation of ferror */  
  return EOF;  
} 
#endif 

FILE __stdin;

int fgetc(FILE *fp)
{
	int ch = 0;
	
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
    {
    }

    ch = (int)USART1->RDR & 0x01FF;
	
    putchar(ch); //回显
	
	return ch;
}
/*******************************************************************************
** End of File
********************************************************************************/
