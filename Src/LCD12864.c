/*********************************************************
 * 名    称：LCD12864.c
 * 作    者：YDJ
 * 创建时间：20171212
 * 简    介：串行模式下，不能从LCD12864中读出数据。只能写入数据。
 *
 * 重点说一下GDRAM：AC7 AC6 AC5 AC4 AC3 AC2 AC1 AC0
 * AC7必须=1，先设置垂直地址再设水平地址（连续写入两个字节的数据来完成垂直和水平地址
 * 的坐标地址）垂直地址范围AC6~AC0;水平地址范围AC3~AC0
 * GDRAM之地址计数器（AC）只会对水平地址（X轴）自动加一，当水平地址=0FH时会重新设为
 * 00H，但不会对垂直地址做进行自动加一，故当连续写入多笔数据时，程序需自动判断地址是
 * 否需重新设定。
 *	
*********************************************************/
/* Includes ------------------------------------------------------------------*/
#include "LCD12864.h"
#include "delay.h"

/* Private variables ---------------------------------------------------------*/
uint8_t LcdCGRAM[64][16];		/* 64*128点 */

/* Private function prototypes -----------------------------------------------*/
static void LCD12864_GPIOInit(void);
static void LCD12864_GPIOOut(void);
#ifndef LCD12864_8080
static void LCD12864_SendByte(uint8_t data);
//static uint8_t LCD12864_ReadByte(void);
#endif /* LCD12864_8080 */
#ifdef LCD12864_8080
static void LCD12864_GPIOIn(void);
static void LCD12864_ReadBF(void);
#endif
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes LCD12864 GPIO
  * @param  None
  * @retval None
  */
static void LCD12864_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	LCD12864_RST_CLK_ENABLE();
	/* RST */
	GPIO_InitStruct.Pin = LCD12864_RST_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(LCD12864_RST_PORT, &GPIO_InitStruct);

#ifdef LCD12864_8080	/* 并行 */
	LCD12864_RS_CLK_ENABLE();
	LCD12864_RW_CLK_ENABLE();
	LCD12864_E_CLK_ENABLE();
	LCD12864_DB0_CLK_ENABLE();LCD12864_DB1_CLK_ENABLE();LCD12864_DB2_CLK_ENABLE();LCD12864_DB3_CLK_ENABLE();
	LCD12864_DB4_CLK_ENABLE();LCD12864_DB5_CLK_ENABLE();LCD12864_DB6_CLK_ENABLE();LCD12864_DB7_CLK_ENABLE();
	
	/* RS */
	GPIO_InitStruct.Pin = LCD12864_RS_PIN;
	HAL_GPIO_Init(LCD12864_RS_PORT, &GPIO_InitStruct);
	/* RW */
	GPIO_InitStruct.Pin = LCD12864_RS_PIN;
	HAL_GPIO_Init(LCD12864_RS_PORT, &GPIO_InitStruct);
	/* E */
	GPIO_InitStruct.Pin = LCD12864_E_PIN;
	HAL_GPIO_Init(LCD12864_E_PORT, &GPIO_InitStruct);
	/* DB0 */
	GPIO_InitStruct.Pin = LCD12864_DB0_PIN;
	HAL_GPIO_Init(LCD12864_DB0_PORT, &GPIO_InitStruct);
	/* DB1 */
	GPIO_InitStruct.Pin = LCD12864_DB1_PIN;
	HAL_GPIO_Init(LCD12864_DB1_PORT, &GPIO_InitStruct);
	/* DB2 */
	GPIO_InitStruct.Pin = LCD12864_DB2_PIN;
	HAL_GPIO_Init(LCD12864_DB2_PORT, &GPIO_InitStruct);
	/* DB3 */
	GPIO_InitStruct.Pin = LCD12864_DB3_PIN;
	HAL_GPIO_Init(LCD12864_DB3_PORT, &GPIO_InitStruct);
	/* DB4 */
	GPIO_InitStruct.Pin = LCD12864_DB4_PIN;
	HAL_GPIO_Init(LCD12864_DB4_PORT, &GPIO_InitStruct);
	/* DB5 */
	GPIO_InitStruct.Pin = LCD12864_DB5_PIN;
	HAL_GPIO_Init(LCD12864_DB5_PORT, &GPIO_InitStruct);
	/* DB6 */
	GPIO_InitStruct.Pin = LCD12864_DB6_PIN;
	HAL_GPIO_Init(LCD12864_DB6_PORT, &GPIO_InitStruct);
	/* DB7 */
	GPIO_InitStruct.Pin = LCD12864_DB7_PIN;
	HAL_GPIO_Init(LCD12864_DB7_PORT, &GPIO_InitStruct);
#else	/* 串行 */
	LCD12864_CS_CLK_ENABLE();
	LCD12864_IO_CLK_ENABLE();
	LCD12864_CLK_CLK_ENABLE();
	/* CS */
	GPIO_InitStruct.Pin = LCD12864_CS_PIN;
	HAL_GPIO_Init(LCD12864_CS_PORT, &GPIO_InitStruct);
	/* IO */
	GPIO_InitStruct.Pin = LCD12864_IO_PIN;
	HAL_GPIO_Init(LCD12864_IO_PORT, &GPIO_InitStruct);
	/* CLK */
	GPIO_InitStruct.Pin = LCD12864_CLK_PIN;
	HAL_GPIO_Init(LCD12864_CLK_PORT, &GPIO_InitStruct);
	
#endif /* LCD12864_MODE == 0 */
}
#ifdef LCD12864_8080	/* 并行 */
/**
  * @brief  Initializes LCD12864 GPIO
  * @param  None
  * @retval None
  */
static void LCD12864_GPIOIn(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	
	GPIO_InitStruct.Pin = LCD12864_DB0_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(LCD12864_DB0_PORT, &GPIO_InitStruct);
	/* DB1 */
	GPIO_InitStruct.Pin = LCD12864_DB1_PIN;
	HAL_GPIO_Init(LCD12864_DB1_PORT, &GPIO_InitStruct);
	/* DB2 */
	GPIO_InitStruct.Pin = LCD12864_DB2_PIN;
	HAL_GPIO_Init(LCD12864_DB2_PORT, &GPIO_InitStruct);
	/* DB3 */
	GPIO_InitStruct.Pin = LCD12864_DB3_PIN;
	HAL_GPIO_Init(LCD12864_DB3_PORT, &GPIO_InitStruct);
	/* DB4 */
	GPIO_InitStruct.Pin = LCD12864_DB4_PIN;
	HAL_GPIO_Init(LCD12864_DB4_PORT, &GPIO_InitStruct);
	/* DB5 */
	GPIO_InitStruct.Pin = LCD12864_DB5_PIN;
	HAL_GPIO_Init(LCD12864_DB5_PORT, &GPIO_InitStruct);
	/* DB6 */
	GPIO_InitStruct.Pin = LCD12864_DB6_PIN;
	HAL_GPIO_Init(LCD12864_DB6_PORT, &GPIO_InitStruct);
	/* DB7 */
	GPIO_InitStruct.Pin = LCD12864_DB7_PIN;
	HAL_GPIO_Init(LCD12864_DB7_PORT, &GPIO_InitStruct);
}
#endif /* LCD12864_8080 */
/**
  * @brief  Initializes LCD12864 GPIO
  * @param  None
  * @retval None
  */
static void LCD12864_GPIOOut(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
#ifdef LCD12864_8080	/* 并行 */
	GPIO_InitStruct.Pin = LCD12864_DB0_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(LCD12864_DB0_PORT, &GPIO_InitStruct);
	/* DB1 */
	GPIO_InitStruct.Pin = LCD12864_DB1_PIN;
	HAL_GPIO_Init(LCD12864_DB1_PORT, &GPIO_InitStruct);
	/* DB2 */
	GPIO_InitStruct.Pin = LCD12864_DB2_PIN;
	HAL_GPIO_Init(LCD12864_DB2_PORT, &GPIO_InitStruct);
	/* DB3 */
	GPIO_InitStruct.Pin = LCD12864_DB3_PIN;
	HAL_GPIO_Init(LCD12864_DB3_PORT, &GPIO_InitStruct);
	/* DB4 */
	GPIO_InitStruct.Pin = LCD12864_DB4_PIN;
	HAL_GPIO_Init(LCD12864_DB4_PORT, &GPIO_InitStruct);
	/* DB5 */
	GPIO_InitStruct.Pin = LCD12864_DB5_PIN;
	HAL_GPIO_Init(LCD12864_DB5_PORT, &GPIO_InitStruct);
	/* DB6 */
	GPIO_InitStruct.Pin = LCD12864_DB6_PIN;
	HAL_GPIO_Init(LCD12864_DB6_PORT, &GPIO_InitStruct);
	/* DB7 */
	GPIO_InitStruct.Pin = LCD12864_DB7_PIN;
	HAL_GPIO_Init(LCD12864_DB7_PORT, &GPIO_InitStruct);
#else	/* 串行 */
	GPIO_InitStruct.Pin = LCD12864_IO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(LCD12864_IO_PORT, &GPIO_InitStruct);
#endif /* LCD12864_MODE == 0 */
}
#ifndef LCD12864_8080
/**
  * @brief  Initializes LCD12864 GPIO
  * @param  None
  * @retval None
  */
static void LCD12864_SendByte(uint8_t data)
{
	uint8_t loop;
	
	LCD12864_GPIOOut();
	for(loop=0;loop<8;loop++)
	{
		LCD12864_CLK_LOW();
//		Delay_Us(1);
		NOP(1);
		if((data & 0x80) != 0)
			LCD12864_IO_HIGH();
		else
			LCD12864_IO_LOW();
		data <<= 1;
//		Delay_Us(1);
		NOP(1);
		LCD12864_CLK_HIGH();
	}
}
/**
  * @brief  Initializes LCD12864 GPIO
  * @param  None
  * @retval None
  */
//static uint8_t LCD12864_ReadByte(void)
//{
//	uint8_t loop,data1=0,data2=0;
//	uint8_t temp;
//	
//	LCD12864_GPIOIn();
//	/* 第一个字节 */
//	for(loop=0;loop<8;loop++)
//	{
//		LCD12864_CLK_LOW();
//		Delay_Us(1);
//		LCD12864_CLK_HIGH();
//		Delay_Us(1);
//		if(HAL_GPIO_ReadPin(LCD12864_IO_PORT,LCD12864_IO_PIN) == GPIO_PIN_SET)
//			data1 |= 1<<(7-loop);
//	}
//	/* 第二字节 */
//	for(loop=0;loop<8;loop++)
//	{
//		LCD12864_CLK_LOW();
//		Delay_Us(1);
//		LCD12864_CLK_HIGH();
//		Delay_Us(1);
//		if(HAL_GPIO_ReadPin(LCD12864_IO_PORT,LCD12864_IO_PIN) == GPIO_PIN_SET)
//			data2 |= 1<<(7-loop);
//	}
//	
//	temp = (data1 & 0xF0) + (data2 & 0xF0);
//	return temp;
//}
#endif /* LCD12864_8080 */
/**
  * @brief  读忙信号
  * @param  None
  * @retval None
  */
#ifdef LCD12864_8080	/* 并行 */
static void LCD12864_ReadBF(void)
{
	uint8_t waitTime = 100;
	
	LCD12864_GPIOIn();
	LCD12864_RS_LOW();
	LCD12864_RW_HIGH();
	Delay_Us(5);
	LCD12864_E_HIGH();
	Delay_Us(5);
	while(HAL_GPIO_ReadPin(LCD12864_DB7_PORT,LCD12864_DB7_PIN) && waitTime)
	{
		waitTime--;
	}
	LCD12864_E_LOW();
}
#endif /* LCD12864_8080 */
/**
  * @brief  向LCD12864写入命令
  * @param  None
  * @retval None
  */
void LCD12864_WriteCmd(uint8_t cmd)
{
#ifdef LCD12864_8080	/* 并行 */
  LCD12864_ReadBF();		/* 等待busy */
	LCD12864_GPIOOut();
	LCD12864_RS_LOW();		/* 命令*/
	LCD12864_RW_LOW();		/* 写入 */
	LCD12864_E_LOW();
	Delay_Us(1);
	//写数据
	if(cmd & 0x01)	LCD12864_DB0_HIGH();
	else						LCD12864_DB0_LOW();
	if(cmd & 0x02)	LCD12864_DB1_HIGH();
	else						LCD12864_DB1_LOW();
	if(cmd & 0x04)	LCD12864_DB2_HIGH();
	else						LCD12864_DB2_LOW();
	if(cmd & 0x08)	LCD12864_DB3_HIGH();
	else						LCD12864_DB3_LOW();
	if(cmd & 0x10)	LCD12864_DB4_HIGH();
	else						LCD12864_DB4_LOW();
	if(cmd & 0x20)	LCD12864_DB5_HIGH();
	else						LCD12864_DB5_LOW();
	if(cmd & 0x40)	LCD12864_DB6_HIGH();
	else						LCD12864_DB6_LOW();
	if(cmd & 0x80)	LCD12864_DB7_HIGH();
	else						LCD12864_DB7_LOW();
	Delay_Us(1);
	LCD12864_E_HIGH();
	Delay_Us(1);
	LCD12864_E_LOW();
#else	/* 串行 */
	LCD12864_CS_HIGH();
	LCD12864_SendByte(0xF8);
	LCD12864_SendByte(cmd & 0xF0);
	LCD12864_SendByte((cmd<<4) & 0xF0);
	LCD12864_CS_LOW();
	Delay_Us(5);
#endif /* LCD12864_8080 */
}
/**
  * @brief  向LCD12864写数据
  * @param  None
  * @retval None
  */
void LCD12864_WriteData(uint8_t data)
{
#ifdef LCD12864_8080	/* 并行 */
  LCD12864_ReadBF();		/* 等待busy */
	LCD12864_GPIOOut();
	LCD12864_RS_HIGH();		/* 数据 */
	LCD12864_RW_LOW();		/* 写入 */
	LCD12864_E_LOW();
	Delay_Us(1);
	//写数据
	if(data & 0x01)	LCD12864_DB0_HIGH();
	else						LCD12864_DB0_LOW();
	if(data & 0x02)	LCD12864_DB1_HIGH();
	else						LCD12864_DB1_LOW();
	if(data & 0x04)	LCD12864_DB2_HIGH();
	else						LCD12864_DB2_LOW();
	if(data & 0x08)	LCD12864_DB3_HIGH();
	else						LCD12864_DB3_LOW();
	if(data & 0x10)	LCD12864_DB4_HIGH();
	else						LCD12864_DB4_LOW();
	if(data & 0x20)	LCD12864_DB5_HIGH();
	else						LCD12864_DB5_LOW();
	if(data & 0x40)	LCD12864_DB6_HIGH();
	else						LCD12864_DB6_LOW();
	if(data & 0x80)	LCD12864_DB7_HIGH();
	else						LCD12864_DB7_LOW();
	Delay_Us(1);
	LCD12864_E_HIGH();
	Delay_Us(1);
	LCD12864_E_LOW();
#else	/* 串行 */
	LCD12864_CS_HIGH();
	LCD12864_SendByte(0xFA);
	LCD12864_SendByte(data & 0xF0);
	LCD12864_SendByte((data<<4) & 0xF0);
	LCD12864_CS_LOW();
//	Delay_Us(100);
#endif /* LCD12864_8080 */
}

/**
  * @brief  Initializes LCD12864
  * @param  None
  * @retval None
  */
void LCD12864_Init(void)
{
	LCD12864_GPIOInit();
	LCD12864_WriteCmd(LCD12864_CMD_BASE_COMMAND);		/* 基本指令集 */
	LCD12864_WriteCmd(LCD12864_CMD_CLEAR_DISPALY);
	HAL_Delay(10);
	LCD12864_WriteCmd(LCD12864_CMD_ADDR_RST1);
	LCD12864_WriteCmd(LCD12864_CMD_DISPALY_ON);
}
/**
  * @brief  Initializes LCD12864
  * @param  None
  * @retval None
  */
void LCD12864_DisplayString(uint8_t row,uint8_t column,uint8_t *s)
{
	switch(row)
	{
		case 1: 
			LCD12864_WriteCmd(0x80+column); 
			break;
		case 2: 
			LCD12864_WriteCmd(0x90+column); 
			break;
		case 3: 
			LCD12864_WriteCmd(0x88+column); 
			break;
		case 4: 
			LCD12864_WriteCmd(0x98+column); 
			break;
		default :
			break;
	}
	while(*s != '\0')
	{
		LCD12864_WriteData(*s);
		s++;
	}
}
/**
  * @brief  Initializes LCD12864
  * @param  None
  * @retval None
  */
void LCD12864_RefreshCGRAM(void)
{
	uint8_t x,y;
	
	for(y=0;y<64;y++)
	{
		/* 扩展指令集、关闭绘图功能 */
		LCD12864_WriteCmd(0x34);
		/* 写入水平坐标X和垂直坐标Y */
		if(y < 32)
		{
			LCD12864_WriteCmd(0x80+y);
			LCD12864_WriteCmd(0x80);
		}
		else
		{
			LCD12864_WriteCmd(0x80+y-32);
			LCD12864_WriteCmd(0x88);
		}
		LCD12864_WriteCmd(0x30);
		/* 写入数据 */
		for(x=0;x<16;x+=2)
		{
			LCD12864_WriteData(LcdCGRAM[y][x]);
			LCD12864_WriteData(LcdCGRAM[y][x+1]);
		}
	}
	/* 开绘图显示功能 */
	LCD12864_WriteCmd(0x36);
	/* 基本指令集 */
	LCD12864_WriteCmd(0x30);
}
/**
  * @brief  画点
  * @param  x：0~127;y：0~63；fill：1 填充，0 清空
  * @retval None
  */
void LCD12864_DrawPoint(uint8_t xPos,uint8_t yPos,uint8_t fill)
{
	uint8_t pos,bx,temp=0;
	
	if(xPos > 127 || yPos > 63)
		return;
	
	pos = xPos/8;
	bx = xPos%8;
	temp = 1<<(7-bx);
	
	if(fill)
		LcdCGRAM[yPos][pos] |= temp;
	else
		LcdCGRAM[yPos][pos] &= ~temp;
}
/**
  * @brief  显示一个字符
  * @param  x：0~127;y：0~63；fill：1 填充，0 清空
	* 
  * @retval None
  */
void LCD12864_ShowChar(uint8_t xPos,uint8_t yPos,uint8_t chr,sFONT font,uint8_t fill)
{
	uint32_t counterh = 0, counterw = 0;
	uint32_t line,offset;
	uint8_t *pchar = NULL;				/* 指向字体数据 */
	uint8_t x;
	
	x = xPos;
	offset = chr - ' ';				/* 字库偏移量 */
	offset = ((font.Width + 7)/8)*font.Height*offset;		
	/* 找到字体对应的首地址 */
	pchar = (uint8_t *)(&font.table[offset]);
	
	/* 根据字体高度循环填充 */
	for(counterh = 0; counterh < font.Height; counterh++)
	{
		line = 0;
//		pchar = ((uint8_t *)pchar + (font.Width + 7)/8 * counterh);
		if((font.Width + 7)/8 == 3)
		{
			line =  (pchar[0]<< 24) | (pchar[1]<< 16) | (pchar[2]<< 8);
		}
		else if((font.Width + 7)/8 == 2)
		{
			line =  (pchar[0]<< 24) | (pchar[1]<< 16);
		}
		else if((font.Width + 7)/8 == 1)
		{
			line =  (pchar[0]<< 24);
		}
		else
		{
		
		}
		pchar += (font.Width + 7)/8;
		/* 按点填充 */
		for(counterw = 0;counterw < font.Width;counterw++)
		{
			if(line & 0x80000000)
			{
				LCD12864_DrawPoint(xPos,yPos,fill);
			}
			else
			{
				LCD12864_DrawPoint(xPos,yPos,!fill);
			}
			line <<= 1;
			xPos++;
		}
		yPos++;					/* 下一行 */
		xPos = x;
	}
}
/**
  * @brief  显示一个字符串
  * @param  row：0~127;column：0~63；fill：1 填充，0 清空
	*         
  * @retval None
  */
void LCD12864_ShowString(uint8_t xPos,uint8_t yPos,uint8_t *chr,sFONT font,uint8_t fill)
{
	while((*chr <= '~') && (*chr >= ' '))
	{
		if(xPos + font.Width > 128)
		{
			xPos = 0;
			yPos += font.Height;
		}
		if(yPos + font.Height > 64)
		{
			xPos = yPos = 0;
		}
		/* 显示字体 */
		LCD12864_ShowChar(xPos,yPos,*chr,font,fill);
		xPos += font.Width;
		chr++;
	}
}
/**
  * @brief  显示一个汉字
  * @param  x：0~127;y：0~63；fill：1 填充，0 清空
	* 
  * @retval None
  */
void LCD12864_ShowGB(uint8_t xPos,uint8_t yPos,uint8_t *chr,uint8_t fill)
{
	uint32_t counterh = 0, counterw = 0;
	uint32_t line;
	uint8_t *pchar = chr;				/* 指向字体数据 */
	uint8_t x;
	
	x = xPos;
	/* 根据字体高度循环填充 */
	for(counterh = 0; counterh < 16; counterh++)
	{
		line =  (pchar[0]<< 24) | (pchar[1]<< 16);
		
		pchar += 2;
		/* 按点填充 */
		for(counterw = 0;counterw < 16;counterw++)
		{
			if(line & 0x80000000)
			{
				LCD12864_DrawPoint(xPos,yPos,fill);
			}
			else
			{
				LCD12864_DrawPoint(xPos,yPos,!fill);
			}
			line <<= 1;
			xPos++;
		}
		yPos++;					/* 下一行 */
		xPos = x;
	}
}
/**
  * @brief  清屏
  * @param  None
  * @retval None
  */
void LCD12864_ClearDisplay(void)
{
//	uint8_t loop,lp;
//	
//	for(loop=0;loop<64;loop++)
//	{
//		for(lp=0;lp<16;lp++)
//		{
//			LcdCGRAM[loop][lp] = 0;
//		}
//	}
//	LCD12864_RefreshCGRAM();
	
	LCD12864_WriteCmd(0x01);
	HAL_Delay(10);
}
