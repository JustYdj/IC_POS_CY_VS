/**
  ******************************************************************************
  * @file    IAP_Main/Inc/common.h
  * @author  MCD Application Team
  * @version V1.6.0
  * @date    12-May-2017
  * @brief   This file provides all the headers of the common functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMON_H
#define __COMMON_H

/* Includes ------------------------------------------------------------------*/
//#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "string.h"
/* Exported types ------------------------------------------------------------*/
// used with UU16
# define LSB 0
# define MSB 1

// used with UU32 (b0 is least-significant byte)
# define b0 0
# define b1 1
# define b2 2
# define b3 3
# define b4 4
# define b5 5
# define b6 6
# define b7 7
typedef union uu16
{
   uint16_t u16;
   int16_t s16;
   uint8_t u8[2];
   int8_t s8[2];
} uu16;

typedef union uu32
{
   uint32_t u32;
   int32_t s32;
   uu16 uu16[2];
   uint16_t u16[2];
   int16_t s16[2];
   uint8_t u8[4];
   int8_t s8[4];
//	float f;
} uu32;
typedef union uu64
{
   uint64_t u64;
   int64_t s64;
   uint32_t u32[2];
   int32_t s32[2];
   uu16 uu16[4];
   uint16_t u16[4];
   int16_t s16[4];
   uint8_t u8[8];
   int8_t s8[8];
//	float f;
} uu64;


/* Constants used by Serial Command Line Mode */
#define TX_TIMEOUT          ((uint32_t)10000)
#define RX_TIMEOUT          ((uint32_t)10000)

#define USART1_BUF_LENGTH      255
#define USART1_BUF_DIMENSION   5

#define USART2_BUF_LENGTH      255
#define USART2_BUF_DIMENSION   5

#define USART3_BUF_LENGTH      255
#define USART3_BUF_DIMENSION   5

#define USART4_BUF_LENGTH      255
#define USART4_BUF_DIMENSION   5

#define USART5_BUF_LENGTH      255
#define USART5_BUF_DIMENSION   5

typedef struct
{
	uint8_t RxBuffer[USART1_BUF_DIMENSION][USART1_BUF_LENGTH];	 /* ���ջ����� */
	uint16_t RxEndIndex[USART1_BUF_DIMENSION];				 					 /* β����ֵ */
	FlagStatus RxEndFlag[USART1_BUF_DIMENSION];				 					 /* ���ս�����ʶ */
	uint8_t RxDimension;																				 /* ����ά�� */
}Usart1BufTypeDef;

typedef struct
{
	uint8_t RxBuffer[USART2_BUF_DIMENSION][USART2_BUF_LENGTH];	 /* ���ջ����� */
	uint16_t RxEndIndex[USART2_BUF_DIMENSION];				 					 /* β����ֵ */
	FlagStatus RxEndFlag[USART2_BUF_DIMENSION];				 					 /* ���ս�����ʶ */
	uint8_t RxDimension;																				 /* ����ά�� */
}Usart2BufTypeDef;

typedef struct
{
	uint8_t RxBuffer[USART3_BUF_DIMENSION][USART3_BUF_LENGTH];	 /* ���ջ����� */
	uint16_t RxEndIndex[USART3_BUF_DIMENSION];				 					 /* β����ֵ */
	FlagStatus RxEndFlag[USART3_BUF_DIMENSION];				 					 /* ���ս�����ʶ */
	uint8_t RxDimension;																				 /* ����ά�� */
}Usart3BufTypeDef;

typedef struct
{
	uint8_t RxBuffer[USART4_BUF_DIMENSION][USART4_BUF_LENGTH];	 /* ���ջ����� */
	uint16_t RxEndIndex[USART4_BUF_DIMENSION];				 					 /* β����ֵ */
	FlagStatus RxEndFlag[USART4_BUF_DIMENSION];				 					 /* ���ս�����ʶ */
	uint8_t RxDimension;																				 /* ����ά�� */
}Usart4BufTypeDef;

typedef struct
{
	uint8_t RxBuffer[USART5_BUF_DIMENSION][USART5_BUF_LENGTH];	 /* ���ջ����� */
	uint16_t RxEndIndex[USART5_BUF_DIMENSION];				 					 /* β����ֵ */
	FlagStatus RxEndFlag[USART5_BUF_DIMENSION];				 					 /* ���ս�����ʶ */
	uint8_t RxDimension;																				 /* ����ά�� */
}Usart5BufTypeDef;

/* ����ģʽ */
typedef enum
{
	CONSUME_MODE_RANDOM = 0,				/* �������ģʽ */
	CONSUME_MODE_FIXED,							/* �̶�����ģʽ */
	CONSUME_MODE_MENU								/* �˵�����ģʽ */
} ConsumeModeTypedef;
/* ���� */
typedef enum
{
	MONDAY	= 1,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY,
	SUNDAY
} WeekTypeDef;
typedef struct
{
	uint8_t seconds;
	uint8_t minute;
	uint8_t hour;
} ClockTypedef;
/* ����ģʽ */
typedef struct
{
	ClockTypedef Start;
	ClockTypedef End;
	ConsumeModeTypedef ConsumeMode;
	uint32_t Money;
} WorkModeTypedef;

/* ʱ����� */
typedef struct
{
	FunctionalState Flag;
	uu16	Seconds;
}TimeCtrlTypeDef;
/* ʱ��������� */
typedef struct
{
	FunctionalState Flag;
	uu16 Seconds;
  uint16_t Cnt;
}TimeCntCtrlTypeDef;
/* IC�� */
typedef struct
{
	uu16 Type;													/* ������ */
	uu32 IdCurrent;											/* ��ǰ��ID */
	uu32 IdLast;												/* �ϴο�ID */
	uu32 Balance;												/* ��� */
	uint8_t Status;											/* IC Card״̬ */
	uint32_t QueryCardMs;								/* Ѱ������ */
	FlagStatus ComeFlag;								/* IC������ */
	FlagStatus LeaveFlag;								/* IC���뿪 */
}IcCardTypeDef;
/* Exported constants --------------------------------------------------------*/
/* ϵͳ���� */
typedef struct
{
	uint16_t ID;												/* �豸�� */
	uint32_t BaudRate;									/* ������ */
	WorkModeTypedef	WorkMode[2][5];			/* ����ģʽ����ͬʱ�ι���ģʽ��һ�� */
	FlagStatus CustomConsumeModeFlag;		/* ��������ģʽʹ�� */
	FlagStatus ChargeBackNeedOkFlag;		/* �ۿ���ҪOK */
	uu16 PassWord;											/* ���� */
	uint8_t WalletSectorPassWordA[6];		/* Ǯ������������A */
	uint8_t WalletSectorPassWordB[6];		/* Ǯ������������B */
	uint8_t UserIdSectorPassWordA[6];		/* �û�ID����������A */	
	uint8_t UserIdSectorPassWordB[6];		/* �û�ID����������A */	
} SysParamTypeDef;
/* ϵͳ���� */
typedef struct
{
	uint8_t Xpos;
	uint8_t Ypos;
	uint8_t ICCardSM;									/* IC Card״̬�� */
	IcCardTypeDef Card;
	uint8_t CardBuf[16];							/* �������Ļ����� */
	uint8_t SM;			
	uu32 UserID;											/* �û����� */ 
	uint8_t PassWord[6];							/* ���� */
	FlagStatus PowerUpFlag;						/* �ϵ��� */
	TimeCtrlTypeDef LocalTx;					/* ���ط��� */
} SysVarTypeDef;


/* Exported macro ------------------------------------------------------------*/
#define IS_CAP_LETTER(c)    (((c) >= 'A') && ((c) <= 'F'))
#define IS_LC_LETTER(c)     (((c) >= 'a') && ((c) <= 'f'))
#define IS_09(c)            (((c) >= '0') && ((c) <= '9'))
#define ISVALIDHEX(c)       (IS_CAP_LETTER(c) || IS_LC_LETTER(c) || IS_09(c))
#define ISVALIDDEC(c)       IS_09(c)
#define CONVERTDEC(c)       (c - '0')

#define CONVERTHEX_ALPHA(c) (IS_CAP_LETTER(c) ? ((c) - 'A'+10) : ((c) - 'a'+10))
#define CONVERTHEX(c)       (IS_09(c) ? ((c) - '0') : CONVERTHEX_ALPHA(c))

#define countof(a) (sizeof(a) / sizeof(*(a)))
	
#define USER_ID													0xFB6A		/* �û�ID */
#define EEPROM_SYS_PARAM_ADDR 					0

#define IC_CARD_WALLET_BLOCK_ID					8
#define IC_CARD_BK_WALLET_BLOCK_ID			9		/* */
#define IC_CARD_USER_ID_BLOCK_ID				10	/* �����û�ID�������ж��Ƿ�Ϊ���ǵĿ� */


#define SM_DISPALY_START_MENU		0x81				/* ��ʾ��ʼ�˵� */
#define SM_FIND_CARD						0x82				/* ɨ�� */
#define SM_DISPALY_BALANCE			0x83				/* ��ʾ��� */
#define SM_PASS_WORD						0x84				/* ȡ������û����� */
#define SM_IDLE									0x8F				/* ���� */

extern UART_HandleTypeDef *LocalComHandle;
extern UART_HandleTypeDef *GprsComHandle;

extern Usart1BufTypeDef Usart1Buf;
extern Usart2BufTypeDef Usart2Buf;
extern Usart3BufTypeDef Usart3Buf;
extern Usart4BufTypeDef Usart4Buf;
extern Usart5BufTypeDef Usart5Buf;

extern UART_HandleTypeDef *LocalComHandle;

extern SysParamTypeDef SysParam;
extern SysVarTypeDef SysVar;
extern Usart5BufTypeDef *LocalCom;

/* Exported functions ------------------------------------------------------- */
void Int2Str(uint8_t *p_str, uint32_t intnum);
uint32_t Str2Int(uint8_t *inputstr, uint32_t *intnum);
uint8_t AsciiToHex(uint8_t ascii,uint8_t *hex);
uint8_t HexToASCII(uint8_t hex);
uint16_t CRC_Modbus(uint8_t *buf,uint16_t index_header,uint16_t index_end);
uint8_t CRC_Byte(uint8_t *buf, uint8_t length);
uint8_t Buffercmp(const uint8_t* pBuffer, uint8_t* pBuffer1, uint16_t BufferLength);
uint16_t FindCharater(uint8_t* buf,uint16_t length, char charater);
void ClearBuffer(uint8_t* buf,uint16_t length);
char *mystrstr(uint8_t *longStr,uint16_t longStrLen,char *shortStr);
#endif  /* __COMMON_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
