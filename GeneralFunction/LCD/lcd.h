#ifndef __LCD_H
#define __LCD_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stm32f10x.h"
#include "pinmacro.h"
//TFTLCD������Ҫ���õĺ���		   
extern uint16_t  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern uint16_t  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ

/* LED ���� */
#define LED1 PAOUT(2)	   //�����϶�Ӧ��LED1
#define LED2 PAOUT(3)	   //�����϶�Ӧ��LED2

/* ���� ���� */
#define KEY1   PAIN(0)	   //�����ϵ�KEY1��
#define KEY2   PAIN(1)	   //�����ϵ�KEY2��

/* LCD ������������*/
#define LCD_CS  PCOUT(8)
#define LCD_RS	PCOUT(9)
#define LCD_WR  PCOUT(10)
#define LCD_RD	PCOUT(11)

/*������оƬ������������	*/   
#define PEN  PCIN(13)   //PC13  INT
#define DOUT PAIN(6)   //PA6  MISO
#define TDIN PAOUT(7)  //PA7  MOSI
#define TCLK PAOUT(5)  //PA5  SCLK
#define TCS  PAOUT(4)  //PA4  CS    

extern uint8_t  SD_Type;//SD��������

/*SD��SD_CS��������	*/   
#define	SD_CS  PBOUT(7) //SD��Ƭѡ����					    	  

/*M45PE16-CS��������	*/   
#define	M45PE_CS PBOUT(6)  //ѡ��FLASH					 
#define M45PE_ID  0x204015
//����LCD�ĳߴ�
#define LCD_W 240
#define LCD_H 320

/* ����״̬	*/ 
#define Key_Down 0x01
#define Key_Up   0x00 

/* SPI�����ٶ�����*/
#define SPI_SPEED_LOW   0
#define SPI_SPEED_HIGH  1

/* �ʸ˽ṹ�� */
typedef struct 
{
	u16 X0;//ԭʼ����
	u16 Y0;
	u16 X; //����/�ݴ�����
	u16 Y;						   	    
	u8  Key_Sta;//�ʵ�״̬			  
	//������У׼����
	float xfac;
	float yfac;
	short xoff;
	short yoff;
}Pen_Holder;

extern Pen_Holder Pen_Point;

/* �ֿ⺯������*/
u8 Update_Font(void);//�����ֿ�   
u8 Font_Init(void);//��ʼ���ֿ�

//���ĺ���֧�ֳ��� V1.1					   

#define EN_UPDATE_FONT //ʹ���������,ͨ���ر�����ʵ�ֽ�ֹ�ֿ����

 
extern uint32_t FONT16ADDR ;
extern uint32_t FONT12ADDR ;
extern uint32_t UNI2GBKADDR;

//ϵͳ�ļ����Ҵ���					  
	 
extern uint32_t PICCLUSTER;//ͼƬ�ļ����׵�ַ	

uint8_t SysInfoGet(uint8_t sel);//��ȡϵͳ�ļ���Ϣ

/***********************************************************************************
������ɫ�궨��
***********************************************************************************/
#define   BLACK        0x0000                    /* ��ɫ�� 0, 0, 0               */
#define   NAVY         0x000F                    /* ����ɫ�� 0, 0, 128           */
#define   DGREEN       0x03E0                    /* ����ɫ�� 0, 128, 0           */
#define   DCYAN        0x03EF                    /* ����ɫ�� 0, 128, 128         */
#define   MAROON       0x7800                    /* ���ɫ��128, 0, 0            */
#define   PURPLE       0x780F                    /* ��ɫ�� 128, 0, 128           */
#define   OLIVE        0x7BE0                    /* ����̣�128, 128, 0          */
#define   LGRAY        0xC618                    /* �Ұ�ɫ��192, 192, 192        */
#define   DGRAY        0x7BEF                    /* ���ɫ��128, 128, 128        */
#define   BLUE         0x001F                    /* ��ɫ�� 0, 0, 255             */
#define   GREEN        0x07E0                 	 /* ��ɫ�� 0, 255, 0             */
#define   CYAN         0x07FF                    /* ��ɫ�� 0, 255, 255           */
#define   RED          0xF800                    /* ��ɫ�� 255, 0, 0             */
#define   MAGENTA      0xF81F                    /* Ʒ�죺 255, 0, 255           */
#define   YELLOW       0xFFE0                    /* ��ɫ�� 255, 255, 0           */
#define   WHITE        0xFFFF                    /* ��ɫ�� 255, 255, 255         */
#define   IDMCOLOR(color) (((color & 0x001F) << 11) | ((color & 0xF800) >> 11) | (color & 0x07E0))

void WriteString(uint16_t x0, uint16_t y0,uint8_t *pcStr, uint16_t color);
void LCD_ShowNum(uint8_t x,uint16_t y,u32 num,uint8_t len,uint8_t size);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawPoint(uint16_t x,uint16_t y);
void Draw_Circle(uint8_t x0,uint16_t y0,uint8_t r);
void LCD_DrawPoint(uint16_t x,uint16_t y);
void LCD_Clear(uint16_t Color);
void LCD_ShowString(uint8_t x,uint16_t y,__I uint8_t *p);
void LCD_ShowChar(uint8_t x,uint16_t y,uint8_t chars,uint8_t size,uint8_t mode);

void Touch_Init(void);
void Refreshes_Screen(void);
void Pen_Int_Set(uint8_t en);
void Convert_Pos(void);
void Draw_Big_Point(uint8_t x,uint16_t y);
void Touch_Adjust(void);
void Save_Adjdata(void);


void M45PE_Init(void);
void SPI_Configuration(void);
uint32_t  M45PE_ReadID(void);  	    //��ȡFLASH ID
void M45PE_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);   //��ȡflash
void M45PE_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);//д��flash
void M45PE_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);

uint8_t SD_Init(void);
uint32_t SD_GetCapacity(void);                       //ȡSD������
void SPI_SetSpeed(uint8_t SpeedSet);				 //����SPI�ٶ�
uint8_t SPI_ReadWriteByte(uint8_t TxData);			 //SPI��д����
uint8_t SD_ReceiveData(uint8_t *data, uint16_t len, uint8_t release);//SD��������

uint8_t SD_ReadSingleBlock(uint32_t sector, uint8_t *buffer);  //��һ��sector
uint8_t SD_WriteSingleBlock(uint32_t sector, const uint8_t *buffer); //дһ��sector

//USB ������ SD����������
uint8_t MSD_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
uint8_t MSD_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);

void Get_HzMat(unsigned char *code,unsigned char *mat,uint8_t size);//�õ����ֵĵ�����
void Show_Font(uint8_t x,uint8_t y,uint8_t *font,uint8_t size,uint8_t mode);//��ָ��λ����ʾһ������
void Show_Str(uint8_t x,uint8_t y,uint8_t*str,uint8_t size,uint8_t mode);//��ָ��λ����ʾһ���ַ��� 
void Show_Str_Mid(uint8_t x,uint16_t y,uint8_t*str,uint8_t size,uint8_t len);
void Show_PTStr(uint8_t x,uint8_t y,uint8_t*str,uint8_t size,uint8_t mode,uint8_t len,uint16_t offx);//��ʾ�����ַ�
void Str_Scroll(uint8_t x,uint8_t y,uint8_t*str,uint8_t size,uint8_t mode,uint8_t len,uint8_t start);//�����ַ���
void my_stradd(uint8_t*str1,uint8_t*str2);//��str2��str1���,���������str1

uint8_t SysInfoGet(uint8_t sel);

#endif	    
		
