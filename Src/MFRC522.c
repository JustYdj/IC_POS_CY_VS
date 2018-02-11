/**
  ******************************************************************************
  * File Name          : MFRC522.c
  * Description        : RFIC卡扫描
  ******************************************************************************
	密码A默认为 00H 00H 00H 00H 00H 00H
	密码B默认为 FFH FFH FFH FFH FFH FFH
	控制字默认为 FFH 07H 80H 69H
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "MFRC522.h"
#include "spi.h"
#include "delay.h"
/* Private variables ---------------------------------------------------------*/
#define MAXRLEN 18
SPI_HandleTypeDef *MFRC522_SpiHandle = &hspi3;

/* Private function prototypes -----------------------------------------------*/
static void MFRC522_SPI_Init(void);
static void MFRC522_Reset(void);
static uint8_t MFRC522_ReadRawRC(uint8_t Address);
static void MFRC522_WriteRawRC(uint8_t Address, uint8_t value);
static void MFRC522_SetBitMask(uint8_t reg,uint8_t mask);
static void MFRC522_ClearBitMask(unsigned char reg,unsigned char mask);
static void MFRC522_SPI_Error(void);
static int8_t MFRC522_PcdConfigIsoType(uint8_t type);
/* Private function ----------------------------------------------------------*/
/**
  * @brief  SPI Write a byte to device
  * @param  WriteValue to be written
  * @retval The value of the received byte.
  */
static void MFRC522_SPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	MFRC522_SPI_CS_GPIO_CLK_ENABLE();
	MFRC522_RST_GPIO_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = MFRC522_SPI_CS_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(MFRC522_SPI_CS_GPIO_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = MFRC522_RST_PIN;
	HAL_GPIO_Init(MFRC522_RST_GPIO_PORT,&GPIO_InitStruct);

	/* 不使能MFRC522 */
	MFRC522_SPI_CS_HIGH();
	/* 复位 */
	MFRC522_Reset();
  /* SPI FLASH Config */
	MX_SPI3_Init();
}
/**
  * @brief  读寄存器
  * @param  寄存器地址
  * @retval 读到的数据
  */
static uint8_t MFRC522_ReadRawRC(uint8_t address)
{
  uint8_t ReadValue = 0;
	uint8_t ucAddr;
	uint8_t dummy = 0;
	
	MFRC522_SPI_CS_LOW();
	
	ucAddr = ((address<<1)&0x7E) | 0x80;
	/* 发送地址 */
	if(HAL_SPI_Transmit(MFRC522_SpiHandle,&ucAddr,1,1000) != HAL_OK)
	{
		MFRC522_SPI_Error();
	}
	/* 读一个字节数据 */
	if(HAL_SPI_TransmitReceive(MFRC522_SpiHandle,&dummy,&ReadValue,1,1000) != HAL_OK)
	{
		MFRC522_SPI_Error();
	}
	
	MFRC522_SPI_CS_HIGH();
	return ReadValue;
}
/**
  * @brief  写寄存器
  * @param  Address寄存器地址,value写入的值
  * @retval None
  */
static void MFRC522_WriteRawRC(uint8_t address, uint8_t value)
{  
	uint8_t ucAddr;
//	uint8_t ReadValue = 0;
	
	MFRC522_SPI_CS_LOW();
	
	ucAddr = ((address<<1)&0x7E);
	/* 发送地址 */
	if(HAL_SPI_Transmit(MFRC522_SpiHandle,&ucAddr,1,1000) != HAL_OK)
	{
		MFRC522_SPI_Error();
	}
	/* 读一个字节数据 */
	if(HAL_SPI_Transmit(MFRC522_SpiHandle,&value,1,1000) != HAL_OK)
	{
		MFRC522_SPI_Error();
	}
	
	MFRC522_SPI_CS_HIGH();
}

/**
  * @brief  寄存器位置位
  * @param  reg寄存器地址,mask写入的值
  * @retval None
  */
static void MFRC522_SetBitMask(uint8_t reg,uint8_t mask)  
{
    char tmp = 0x0;
	
    tmp = MFRC522_ReadRawRC(reg);
    MFRC522_WriteRawRC(reg,tmp | mask);  // set bit mask
}
/**
  * @brief  寄存器位清零
  * @param  reg寄存器地址,mask写入的值
  * @retval None
  */
static void MFRC522_ClearBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = MFRC522_ReadRawRC(reg);
    MFRC522_WriteRawRC(reg, tmp & ~mask);  // clear bit mask
}        
/**
  * @brief  SPI Write a byte to device
  * @param  WriteValue to be written
  * @retval The value of the received byte.
  */
static void MFRC522_SPI_Error(void)
{
  /* De-initialize the SPI communication BUS */
  HAL_SPI_DeInit(MFRC522_SpiHandle);
  
  /* Re- Initiaize the SPI communication BUS */
  MFRC522_SPI_Init();
}
/**
  * @brief  SPI Write a byte to device
  * @param  WriteValue to be written
  * @retval The value of the received byte.
  */
void MFRC522_Init(void)
{
	MFRC522_SPI_Init();
	
	MFRC522_WriteRawRC(CommandReg,PCD_RESETPHASE);
	Delay_Us(1);

	MFRC522_WriteRawRC(ModeReg,0x3D);
//	temp = MFRC522_ReadRawRC(ModeReg);
	MFRC522_WriteRawRC(TReloadRegL,30);
	MFRC522_WriteRawRC(TReloadRegH,0);
	MFRC522_WriteRawRC(TModeReg,0x8D);
	MFRC522_WriteRawRC(TPrescalerReg,0x3E);
	MFRC522_WriteRawRC(TxAutoReg,0x40);
	/* 需要关天线，再开天线 */
	MFRC522_PcdAntennaOff();
	MFRC522_PcdAntennaOn();
	MFRC522_PcdConfigIsoType('A');
}
/**
  * @brief  MFRC522复位
  * @param  None
  * @retval None
  */
static void MFRC522_Reset(void)
{
	MFRC522_RST_HIGH();
	Delay_Us(1);
	MFRC522_RST_LOW();
	Delay_Us(1);
	MFRC522_RST_HIGH();
}

static int8_t MFRC522_PcdConfigIsoType(uint8_t type)
{
	if (type == 'A')                     //ISO14443_A
	{ 
		MFRC522_ClearBitMask(Status2Reg,0x08);
		MFRC522_WriteRawRC(ModeReg,0x3D);//3F
		MFRC522_WriteRawRC(RxSelReg,0x86);//84
		MFRC522_WriteRawRC(RFCfgReg,0x7F);   //4F
		MFRC522_WriteRawRC(TReloadRegL,30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
		MFRC522_WriteRawRC(TReloadRegH,0);
		MFRC522_WriteRawRC(TModeReg,0x8D);
		MFRC522_WriteRawRC(TPrescalerReg,0x3E);
		Delay_Us(1);
		MFRC522_PcdAntennaOn();
	}
	else
	{ 
		return 1; 
	}
   
   return MI_OK;
}
/////////////////////////////////////////////////////////////////////
//功    能：寻卡
//参数说明: req_code[IN]:寻卡方式
//                0x52 = 寻感应区内所有符合14443A标准的卡
//                0x26 = 寻未进入休眠状态的卡
//          pTagType[OUT]：卡片类型代码
//                0x4400 = Mifare_UltraLight
//                0x0400 = Mifare_One(S50)
//                0x0200 = Mifare_One(S70)
//                0x0800 = Mifare_Pro(X)
//                0x4403 = Mifare_DESFire
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
int8_t MFRC522_PcdRequest(uint8_t req_code,uint8_t *pTagType)
{
   int8_t status;  
   uint32_t  unLen;
   uint8_t ucComMF522Buf[MAXRLEN]; 

   MFRC522_ClearBitMask(Status2Reg,0x08);
   MFRC522_WriteRawRC(BitFramingReg,0x07);
   MFRC522_SetBitMask(TxControlReg,0x03);
 
   ucComMF522Buf[0] = req_code;

   status = MFRC522_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
   
   if ((status == MI_OK) && (unLen == 0x10))
   {    
       *pTagType     = ucComMF522Buf[0];
       *(pTagType+1) = ucComMF522Buf[1];
   }
   else
   {   status = MI_ERR;   }
   
   return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：防冲撞
//参数说明: pSnr[OUT]:卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////  
int8_t MFRC522_PcdAnticoll(uint8_t *pSnr)
{
    int8_t status;
    uint8_t i,snr_check=0;
    uint32_t  unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 
    

    MFRC522_ClearBitMask(Status2Reg,0x08);
    MFRC522_WriteRawRC(BitFramingReg,0x00);
    MFRC522_ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = MFRC522_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
         }
         if (snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }
    
    MFRC522_SetBitMask(CollReg,0x80);
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：选定卡片
//参数说明: pSnr[IN]:卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
int8_t MFRC522_PcdSelect(uint8_t *pSnr)
{
    int8_t status;
    uint8_t i;
    uint32_t  unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    MFRC522_CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    MFRC522_ClearBitMask(Status2Reg,0x08);

    status = MFRC522_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }
    else
    {   status = MI_ERR;    }

    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：验证卡片密码
//参数说明: auth_mode[IN]: 密码验证模式
//                 0x60 = 验证A密钥
//                 0x61 = 验证B密钥 
//          addr[IN]：块地址
//          pKey[IN]：密码
//          pSnr[IN]：卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////               
int8_t MFRC522_PcdAuthState(uint8_t auth_mode,uint8_t addr,uint8_t *pKey,uint8_t *pSnr)
{
    int8_t status;
    uint32_t  unLen;
    uint8_t i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
    
    status = MFRC522_PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(MFRC522_ReadRawRC(Status2Reg) & 0x08)))
    {   status = MI_ERR;   }
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：读取M1卡一块数据
//参数说明: addr[IN]：块地址
//          pData[OUT]：读出的数据，16字节
//返    回: 成功返回MI_OK
///////////////////////////////////////////////////////////////////// 
int8_t MFRC522_PcdRead(uint8_t addr,uint8_t *pData)
{
    int8_t status;
    uint32_t  unLen;
    uint8_t i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    MFRC522_CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = MFRC522_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
    {
        for (i=0; i<16; i++)
        {    *(pData+i) = ucComMF522Buf[i];   }
    }
    else
    {   status = MI_ERR;   }
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：写数据到M1卡一块
//参数说明: addr[IN]：块地址
//          pData[IN]：写入的数据，16字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////                  
int8_t MFRC522_PcdWrite(uint8_t addr,uint8_t *pData)
{
    int8_t status;
    uint32_t  unLen;
    uint8_t i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    MFRC522_CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = MFRC522_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
        for (i=0; i<16; i++)
        {    ucComMF522Buf[i] = *(pData+i);   }
        MFRC522_CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = MFRC522_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：扣款和充值
//参数说明: dd_mode[IN]：命令字
//               0xC0 = 扣款
//               0xC1 = 充值
//          addr[IN]：钱包地址
//          pValue[IN]：4字节增(减)值，低位在前
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////                 
int8_t MFRC522_PcdValue(uint8_t dd_mode,uint8_t addr,uint8_t *pValue)
{
	int8_t status;
	uint32_t  unLen;
	uint8_t i,ucComMF522Buf[MAXRLEN]; 

	ucComMF522Buf[0] = dd_mode;
	ucComMF522Buf[1] = addr;
	MFRC522_CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);

	status = MFRC522_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

	if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
	{   status = MI_ERR;   }
			
	if (status == MI_OK)
	{
			for (i=0; i<4; i++)
			{    ucComMF522Buf[i] = *(pValue+i);   }
			MFRC522_CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
			unLen = 0;
			status = MFRC522_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
			if (status != MI_ERR)
			{    status = MI_OK;    }
	}

	if (status == MI_OK)
	{
			ucComMF522Buf[0] = PICC_TRANSFER;
			ucComMF522Buf[1] = addr;
			MFRC522_CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]); 

			status = MFRC522_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

			if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
			{   status = MI_ERR;   }
	}
	return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：备份钱包
//参数说明: sourceaddr[IN]：源地址
//          goaladdr[IN]：目标地址
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
int8_t MFRC522_PcdBakValue(uint8_t sourceaddr, uint8_t goaladdr)
{
    int8_t status;
    uint32_t  unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_RESTORE;
    ucComMF522Buf[1] = sourceaddr;
    MFRC522_CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = MFRC522_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
    
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = 0;
        ucComMF522Buf[1] = 0;
        ucComMF522Buf[2] = 0;
        ucComMF522Buf[3] = 0;
        MFRC522_CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
 
        status = MFRC522_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
        if (status != MI_ERR)
        {    status = MI_OK;    }
    }
    
    if (status != MI_OK)
    {    return MI_ERR;   }
    
    ucComMF522Buf[0] = PICC_TRANSFER;
    ucComMF522Buf[1] = goaladdr;

    MFRC522_CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = MFRC522_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }

    return status;
}


/////////////////////////////////////////////////////////////////////
//功    能：命令卡片进入休眠状态
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
int8_t MFRC522_PcdHalt(void)
{
	int8_t status;
	uint32_t  unLen;
	uint8_t ucComMF522Buf[MAXRLEN]; 

	ucComMF522Buf[0] = PICC_HALT;
	ucComMF522Buf[1] = 0;
	MFRC522_CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);

	status = MFRC522_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

	return status;
}

/////////////////////////////////////////////////////////////////////
//用MF522计算CRC16函数
/////////////////////////////////////////////////////////////////////
void MFRC522_CalulateCRC(uint8_t *pIndata,uint8_t len,uint8_t *pOutData)
{
    uint8_t i,n;
	
    MFRC522_ClearBitMask(DivIrqReg,0x04);
    MFRC522_WriteRawRC(CommandReg,PCD_IDLE);
    MFRC522_SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
    {
			MFRC522_WriteRawRC(FIFODataReg, *(pIndata+i));
		}
    MFRC522_WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = MFRC522_ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOutData[0] = MFRC522_ReadRawRC(CRCResultRegL);
    pOutData[1] = MFRC522_ReadRawRC(CRCResultRegM);
}


/////////////////////////////////////////////////////////////////////
//功    能：通过RC522和ISO14443卡通讯
//参数说明：Command[IN]:RC522命令字
//          pInData[IN]:通过RC522发送到卡片的数据
//          InLenByte[IN]:发送数据的字节长度
//          pOutData[OUT]:接收到的卡片返回数据
//          *pOutLenBit[OUT]:返回数据的位长度
/////////////////////////////////////////////////////////////////////
int8_t MFRC522_PcdComMF522(uint8_t Command, uint8_t *pInData, uint8_t InLenByte,uint8_t *pOutData, uint32_t  *pOutLenBit)
{
	int8_t status = MI_ERR;
	uint8_t irqEn   = 0x00;
	uint8_t waitFor = 0x00;
	uint8_t lastBits;
	uint8_t n;
	uint32_t i;
	
	switch (Command)
	{
		 case PCD_AUTHENT:
				irqEn   = 0x12;
				waitFor = 0x10;
				break;
		 case PCD_TRANSCEIVE:
				irqEn   = 0x77;
				waitFor = 0x30;
				break;
		 default:
			 break;
	}
   
	MFRC522_WriteRawRC(ComIEnReg,irqEn|0x80);
	MFRC522_ClearBitMask(ComIrqReg,0x80);
	MFRC522_WriteRawRC(CommandReg,PCD_IDLE);
	MFRC522_SetBitMask(FIFOLevelReg,0x80);
	
	for (i=0; i<InLenByte; i++)
	{  
		MFRC522_WriteRawRC(FIFODataReg, pInData[i]);    
	}
	MFRC522_WriteRawRC(CommandReg, Command);
 
	
	if (Command == PCD_TRANSCEIVE)
	{   
		MFRC522_SetBitMask(BitFramingReg,0x80);  
	}
	
	i = 600;//根据时钟频率调整，操作M1卡最大等待时间25ms
	do 
	{
		n = MFRC522_ReadRawRC(ComIrqReg);
		i--;
	}
	while ((i!=0) && !(n&0x01) && !(n&waitFor));
	
	MFRC522_ClearBitMask(BitFramingReg,0x80);
			
	if (i!=0)
	{    
		if(!(MFRC522_ReadRawRC(ErrorReg)&0x1B))
		{
			status = MI_OK;
			if (n & irqEn & 0x01)
			{   
				status = MI_NOTAGERR;   
			}
			if (Command == PCD_TRANSCEIVE)
			{
				n = MFRC522_ReadRawRC(FIFOLevelReg);
				lastBits = MFRC522_ReadRawRC(ControlReg) & 0x07;
				if (lastBits)
				{   
					*pOutLenBit = (n-1)*8 + lastBits;   
				}
				else
				{   
					*pOutLenBit = n*8;   
				}
				if (n == 0)
				{   
					n = 1;    
				}
				if (n > MAXRLEN)
				{  
					n = MAXRLEN;   
				}
				for (i=0; i<n; i++)
				{   
					pOutData[i] = MFRC522_ReadRawRC(FIFODataReg);    
				}
			}
		}
		else
		{   
			status = MI_ERR;   
		}
			
 }
 

 MFRC522_SetBitMask(ControlReg,0x80);           // stop timer now
 MFRC522_WriteRawRC(CommandReg,PCD_IDLE); 
 return status;
}


/////////////////////////////////////////////////////////////////////
//开启天线  
//每次启动或关闭天险发射之间应至少有1ms的间隔
/////////////////////////////////////////////////////////////////////
void MFRC522_PcdAntennaOn(void)
{
    unsigned char i;
    i = MFRC522_ReadRawRC(TxControlReg);
    if (!(i & 0x03))
    {
        MFRC522_SetBitMask(TxControlReg, 0x03);
    }
}


/////////////////////////////////////////////////////////////////////
//关闭天线
/////////////////////////////////////////////////////////////////////
void MFRC522_PcdAntennaOff(void)
{
    MFRC522_ClearBitMask(TxControlReg, 0x03);
}

