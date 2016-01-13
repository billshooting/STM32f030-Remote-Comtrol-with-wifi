/**************************************************************
** 	��ţ������
**	��������: STM32���ڲ���
**  �汾��V1.0  
**	��̳��www.openmcu.com
**	�Ա���http://shop36995246.taobao.com/   
**  ����֧��Ⱥ��121939788 
***************************************************************/ 
#include "usart.h"

/*****************************************************************************
** ��������: USART1_Send_Byte
** ��������: ���ڷ���һ���ַ���
** ��  ����: Dream
** �ա�  ��: 2010��12��17��
*****************************************************************************/
void USART1_Send_Byte(uint16_t Data)
{ 
   	while (!(USART1->ISR & USART_FLAG_TXE));
   	USART1->TDR = (Data & (uint16_t)0x01FF);	   
}
/*****************************************************************************
** ��������: USART_Send_Str
** ��������: ���ڷ����ַ���
				s:ָ���ַ�����ָ�� 
					ע��������ַ�������'\n'����ᷢ��һ���س�����
** ��  ����: Dream
** �ա�  ��: 2010��12��17��
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
** ��������: USART_Send_Enter
** ��������: ���ڷ����ַ���
				���ڷ���0d 0a �����س�����
					ע���˺������Ƿ���0d 0a�������ֽڣ��ڡ������նˡ��ϻ��лس����е�Ч��
** ��  ����: Dream
** �ա�  ��: 2010��12��17��
*****************************************************************************/
void USART_Send_Enter(void)
{
	USART1_Send_Byte(0x0d);
	USART1_Send_Byte(0x0a);
}
/*****************************************************************************
** ��������: USART_Put_Num
** ��������: ���ڷ�����ֵ
				�����лὫ��ֵתΪ��Ӧ���ַ��������ͳ�ȥ������ 4567 תΪ "4567" 
** ��  ����: Dream
** �ա�  ��: 2010��12��17��
*****************************************************************************/	
void USART_Put_Num(uint32_t dat)
{
 	char temp[20];
 	u32tostr(dat,temp);
 	USART_SendString(temp);
}
/*****************************************************************************
** ��������: USART_Put_Inf
** ��������: ���ڷ��͵�����Ϣ
				inf:ָ����ʾ��Ϣ�ַ�����ָ��
             dat:һ����ֵ��ǰ�����ʾ��Ϣ������˵�������ֵ������ 
** ��  ����: Dream
** �ա�  ��: 2010��12��17��
*****************************************************************************/
void USART_Put_Inf(char *inf,uint32_t dat)
{
 	USART_SendString(inf);
 	USART_Put_Num(dat);
 	USART_SendString("\n");  
}
/*****************************************************************************
** ��������: USART_Configuration
** ��������: �������ú���
				����STM32����1 ������115200 
** ��  ����: Dream
** �ա�  ��: 2010��12��17��
*****************************************************************************/
void USART_Configuration(uint32_t baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate            = baudrate  ;	  //������115200
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b; //8λ����
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;	 //ֹͣλ1λ
	USART_InitStructure.USART_Parity              = USART_Parity_No ;	 //��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_Cmd(USART1, ENABLE);     
}
/*****************************************************************************
** ��������: USART_IT_Init
** ��������: ���ô����ж�
** ��  ����: Dream
** �ա�  ��: 2010��12��17��
*****************************************************************************/
void USART_IT_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;//��ռ���ȼ�2 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
}
/*****************************************************************************
** ��������: USART1_IRQHandler
** ��������: �����жϺ���
** ��  ����: Dream
** �ա�  ��: 2010��12��17��
*****************************************************************************/

/*****************************************************************************
** ��������: u32tostr
** ��������: ��һ��32λ�ı���datתΪ�ַ����������1234תΪ"1234"
				dat:��ת��long�͵ı���
             str:ָ���ַ������ָ�룬ת������ֽڴ���������
** ��  ����: Dream
** �ա�  ��: 2010��12��17��
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
//#pragma import(__use_no_semihosting)             //û��ʵ��fgetcʱ��Ҫ�����ò���   
/* ��׼����Ҫ��֧�ֺ��� ʹ��printf()���Դ�ӡ����Ҫʵ�ָú��� */               
struct __FILE 
{ 
	int handle; 
    /* Whatever you require here. If the only file you are using is */    
    /* standard output using printf() for debugging, no file handling */    
    /* is required. */
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
/* �ض���fputc���� ���ʹ��MicroLIBֻ��Ҫ�ض���fputc�������� */  
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
����ֱ��ʹ��putchar
����Ҫ�ٶ��� int putchar(int ch)����Ϊstdio.h�������¶���
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
	
    putchar(ch); //����
	
	return ch;
}
/*******************************************************************************
** End of File
********************************************************************************/
