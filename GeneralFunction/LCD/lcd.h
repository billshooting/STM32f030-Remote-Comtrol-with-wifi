#ifndef __LCD_H
#define __LCD_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stm32f10x.h"
#include "pinmacro.h"
//TFTLCD部分外要调用的函数		   
extern uint16_t  POINT_COLOR;//默认红色    
extern uint16_t  BACK_COLOR; //背景颜色.默认为白色

/* LED 定义 */
#define LED1 PAOUT(2)	   //板子上对应于LED1
#define LED2 PAOUT(3)	   //板子上对应于LED2

/* 按键 定义 */
#define KEY1   PAIN(0)	   //板子上的KEY1键
#define KEY2   PAIN(1)	   //板子上的KEY2键

/* LCD 控制引脚配置*/
#define LCD_CS  PCOUT(8)
#define LCD_RS	PCOUT(9)
#define LCD_WR  PCOUT(10)
#define LCD_RD	PCOUT(11)

/*触摸屏芯片连接引脚配置	*/   
#define PEN  PCIN(13)   //PC13  INT
#define DOUT PAIN(6)   //PA6  MISO
#define TDIN PAOUT(7)  //PA7  MOSI
#define TCLK PAOUT(5)  //PA5  SCLK
#define TCS  PAOUT(4)  //PA4  CS    

extern uint8_t  SD_Type;//SD卡的类型

/*SD卡SD_CS引脚配置	*/   
#define	SD_CS  PBOUT(7) //SD卡片选引脚					    	  

/*M45PE16-CS引脚配置	*/   
#define	M45PE_CS PBOUT(6)  //选中FLASH					 
#define M45PE_ID  0x204015
//定义LCD的尺寸
#define LCD_W 240
#define LCD_H 320

/* 按键状态	*/ 
#define Key_Down 0x01
#define Key_Up   0x00 

/* SPI总线速度设置*/
#define SPI_SPEED_LOW   0
#define SPI_SPEED_HIGH  1

/* 笔杆结构体 */
typedef struct 
{
	u16 X0;//原始坐标
	u16 Y0;
	u16 X; //最终/暂存坐标
	u16 Y;						   	    
	u8  Key_Sta;//笔的状态			  
	//触摸屏校准参数
	float xfac;
	float yfac;
	short xoff;
	short yoff;
}Pen_Holder;

extern Pen_Holder Pen_Point;

/* 字库函数申明*/
u8 Update_Font(void);//更新字库   
u8 Font_Init(void);//初始化字库

//中文汉字支持程序 V1.1					   

#define EN_UPDATE_FONT //使能字体更新,通过关闭这里实现禁止字库更新

 
extern uint32_t FONT16ADDR ;
extern uint32_t FONT12ADDR ;
extern uint32_t UNI2GBKADDR;

//系统文件查找代码					  
	 
extern uint32_t PICCLUSTER;//图片文件夹首地址	

uint8_t SysInfoGet(uint8_t sel);//获取系统文件信息

/***********************************************************************************
设置颜色宏定义
***********************************************************************************/
#define   BLACK        0x0000                    /* 黑色： 0, 0, 0               */
#define   NAVY         0x000F                    /* 深蓝色： 0, 0, 128           */
#define   DGREEN       0x03E0                    /* 深绿色： 0, 128, 0           */
#define   DCYAN        0x03EF                    /* 深青色： 0, 128, 128         */
#define   MAROON       0x7800                    /* 深红色：128, 0, 0            */
#define   PURPLE       0x780F                    /* 紫色： 128, 0, 128           */
#define   OLIVE        0x7BE0                    /* 橄榄绿：128, 128, 0          */
#define   LGRAY        0xC618                    /* 灰白色：192, 192, 192        */
#define   DGRAY        0x7BEF                    /* 深灰色：128, 128, 128        */
#define   BLUE         0x001F                    /* 蓝色： 0, 0, 255             */
#define   GREEN        0x07E0                 	 /* 绿色： 0, 255, 0             */
#define   CYAN         0x07FF                    /* 青色： 0, 255, 255           */
#define   RED          0xF800                    /* 红色： 255, 0, 0             */
#define   MAGENTA      0xF81F                    /* 品红： 255, 0, 255           */
#define   YELLOW       0xFFE0                    /* 黄色： 255, 255, 0           */
#define   WHITE        0xFFFF                    /* 白色： 255, 255, 255         */
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
uint32_t  M45PE_ReadID(void);  	    //读取FLASH ID
void M45PE_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);   //读取flash
void M45PE_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);//写入flash
void M45PE_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);

uint8_t SD_Init(void);
uint32_t SD_GetCapacity(void);                       //取SD卡容量
void SPI_SetSpeed(uint8_t SpeedSet);				 //设置SPI速度
uint8_t SPI_ReadWriteByte(uint8_t TxData);			 //SPI读写数据
uint8_t SD_ReceiveData(uint8_t *data, uint16_t len, uint8_t release);//SD卡读数据

uint8_t SD_ReadSingleBlock(uint32_t sector, uint8_t *buffer);  //读一个sector
uint8_t SD_WriteSingleBlock(uint32_t sector, const uint8_t *buffer); //写一个sector

//USB 读卡器 SD卡操作函数
uint8_t MSD_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
uint8_t MSD_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);

void Get_HzMat(unsigned char *code,unsigned char *mat,uint8_t size);//得到汉字的点阵码
void Show_Font(uint8_t x,uint8_t y,uint8_t *font,uint8_t size,uint8_t mode);//在指定位置显示一个汉字
void Show_Str(uint8_t x,uint8_t y,uint8_t*str,uint8_t size,uint8_t mode);//在指定位置显示一个字符串 
void Show_Str_Mid(uint8_t x,uint16_t y,uint8_t*str,uint8_t size,uint8_t len);
void Show_PTStr(uint8_t x,uint8_t y,uint8_t*str,uint8_t size,uint8_t mode,uint8_t len,uint16_t offx);//显示部分字符
void Str_Scroll(uint8_t x,uint8_t y,uint8_t*str,uint8_t size,uint8_t mode,uint8_t len,uint8_t start);//滚动字符串
void my_stradd(uint8_t*str1,uint8_t*str2);//将str2与str1相加,结果保存在str1

uint8_t SysInfoGet(uint8_t sel);

#endif	    
		
