/**
  ******************************************************************************
  * File Name          : MFRC522.c
  * Description        : RFIC��ɨ��
  ******************************************************************************
	����AĬ��Ϊ 00H 00H 00H 00H 00H 00H
	����BĬ��Ϊ FFH FFH FFH FFH FFH FFH
	������Ĭ��Ϊ FFH 07H 80H 69H
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

	/* ��ʹ��MFRC522 */
	MFRC522_SPI_CS_HIGH();
	/* ��λ */
	MFRC522_Reset();
  /* SPI FLASH Config */
	MX_SPI3_Init();
}
/**
  * @brief  ���Ĵ���
  * @param  �Ĵ�����ַ
  * @retval ����������
  */
static uint8_t MFRC522_ReadRawRC(uint8_t address)
{
  uint8_t ReadValue = 0;
	uint8_t ucAddr;
	uint8_t dummy = 0;
	
	MFRC522_SPI_CS_LOW();
	
	ucAddr = ((address<<1)&0x7E) | 0x80;
	/* ���͵�ַ */
	if(HAL_SPI_Transmit(MFRC522_SpiHandle,&ucAddr,1,1000) != HAL_OK)
	{
		MFRC522_SPI_Error();
	}
	/* ��һ���ֽ����� */
	if(HAL_SPI_TransmitReceive(MFRC522_SpiHandle,&dummy,&ReadValue,1,1000) != HAL_OK)
	{
		MFRC522_SPI_Error();
	}
	
	MFRC522_SPI_CS_HIGH();
	return ReadValue;
}
/**
  * @brief  д�Ĵ���
  * @param  Address�Ĵ�����ַ,valueд���ֵ
  * @retval None
  */
static void MFRC522_WriteRawRC(uint8_t address, uint8_t value)
{  
	uint8_t ucAddr;
//	uint8_t ReadValue = 0;
	
	MFRC522_SPI_CS_LOW();
	
	ucAddr = ((address<<1)&0x7E);
	/* ���͵�ַ */
	if(HAL_SPI_Transmit(MFRC522_SpiHandle,&ucAddr,1,1000) != HAL_OK)
	{
		MFRC522_SPI_Error();
	}
	/* ��һ���ֽ����� */
	if(HAL_SPI_Transmit(MFRC522_SpiHandle,&value,1,1000) != HAL_OK)
	{
		MFRC522_SPI_Error();
	}
	
	MFRC522_SPI_CS_HIGH();
}

/**
  * @brief  �Ĵ���λ��λ
  * @param  reg�Ĵ�����ַ,maskд���ֵ
  * @retval None
  */
static void MFRC522_SetBitMask(uint8_t reg,uint8_t mask)  
{
    char tmp = 0x0;
	
    tmp = MFRC522_ReadRawRC(reg);
    MFRC522_WriteRawRC(reg,tmp | mask);  // set bit mask
}
/**
  * @brief  �Ĵ���λ����
  * @param  reg�Ĵ�����ַ,maskд���ֵ
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
	/* ��Ҫ�����ߣ��ٿ����� */
	MFRC522_PcdAntennaOff();
	MFRC522_PcdAntennaOn();
	MFRC522_PcdConfigIsoType('A');
}
/**
  * @brief  MFRC522��λ
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
//��    �ܣ�Ѱ��
//����˵��: req_code[IN]:Ѱ����ʽ
//                0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�
//                0x26 = Ѱδ��������״̬�Ŀ�
//          pTagType[OUT]����Ƭ���ʹ���
//                0x4400 = Mifare_UltraLight
//                0x0400 = Mifare_One(S50)
//                0x0200 = Mifare_One(S70)
//                0x0800 = Mifare_Pro(X)
//                0x4403 = Mifare_DESFire
//��    ��: �ɹ�����MI_OK
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
//��    �ܣ�����ײ
//����˵��: pSnr[OUT]:��Ƭ���кţ�4�ֽ�
//��    ��: �ɹ�����MI_OK
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
//��    �ܣ�ѡ����Ƭ
//����˵��: pSnr[IN]:��Ƭ���кţ�4�ֽ�
//��    ��: �ɹ�����MI_OK
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
//��    �ܣ���֤��Ƭ����
//����˵��: auth_mode[IN]: ������֤ģʽ
//                 0x60 = ��֤A��Կ
//                 0x61 = ��֤B��Կ 
//          addr[IN]�����ַ
//          pKey[IN]������
//          pSnr[IN]����Ƭ���кţ�4�ֽ�
//��    ��: �ɹ�����MI_OK
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
//��    �ܣ���ȡM1��һ������
//����˵��: addr[IN]�����ַ
//          pData[OUT]�����������ݣ�16�ֽ�
//��    ��: �ɹ�����MI_OK
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
//��    �ܣ�д���ݵ�M1��һ��
//����˵��: addr[IN]�����ַ
//          pData[IN]��д������ݣ�16�ֽ�
//��    ��: �ɹ�����MI_OK
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
//��    �ܣ��ۿ�ͳ�ֵ
//����˵��: dd_mode[IN]��������
//               0xC0 = �ۿ�
//               0xC1 = ��ֵ
//          addr[IN]��Ǯ����ַ
//          pValue[IN]��4�ֽ���(��)ֵ����λ��ǰ
//��    ��: �ɹ�����MI_OK
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
//��    �ܣ�����Ǯ��
//����˵��: sourceaddr[IN]��Դ��ַ
//          goaladdr[IN]��Ŀ���ַ
//��    ��: �ɹ�����MI_OK
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
//��    �ܣ����Ƭ��������״̬
//��    ��: �ɹ�����MI_OK
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
//��MF522����CRC16����
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
//��    �ܣ�ͨ��RC522��ISO14443��ͨѶ
//����˵����Command[IN]:RC522������
//          pInData[IN]:ͨ��RC522���͵���Ƭ������
//          InLenByte[IN]:�������ݵ��ֽڳ���
//          pOutData[OUT]:���յ��Ŀ�Ƭ��������
//          *pOutLenBit[OUT]:�������ݵ�λ����
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
	
	i = 600;//����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
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
//��������  
//ÿ��������ر����շ���֮��Ӧ������1ms�ļ��
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
//�ر�����
/////////////////////////////////////////////////////////////////////
void MFRC522_PcdAntennaOff(void)
{
    MFRC522_ClearBitMask(TxControlReg, 0x03);
}

