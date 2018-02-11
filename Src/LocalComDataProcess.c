/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32F1xx_hal.h"
#include "LocalComDataProcess.h"
#include "common.h"
#include "usart.h"
#include "CmdFuncTable.h"
#include "GPIO.h"
#include "ICCard.h"
#include "MFRC522.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void LocalComDataProcess(void)
{
	uint16_t loop;
	uint16_t index;
	
	for(loop=0;loop<USART1_BUF_DIMENSION;loop++)
	{
		if(LocalCom->RxEndFlag[loop] == SET)
		{
			index = LocalChechDataIsOk(LocalCom->RxBuffer[loop],LocalCom->RxEndIndex[loop]);
			if(index != 0xFFFF)
			{
				if(sLocalCmdFuncWithSn[LocalCom->RxBuffer[loop][index+FRAME_AFN_INDEX]].pCmdFunc !=NULL)
				{
					/* 调用处理函数 */
					sLocalCmdFuncWithSn[LocalCom->RxBuffer[loop][index+FRAME_AFN_INDEX]].pCmdFunc \
					(sLocalCmdFuncWithSn[LocalCom->RxBuffer[loop][index+FRAME_AFN_INDEX]].CmdSn,&LocalCom->RxBuffer[loop][index],LocalCom->RxEndIndex[loop]-index);
				}
				LocalCom->RxEndFlag[loop] = RESET;
				LocalCom->RxEndIndex[loop] = 0;
			}
		}
	}
}
/**
  * @brief  读用户代码和密码
  * @param  None
  * @retval ret 找到
  */
void LocalReadUserIdAndPassWord(void)
{
	uint16_t index = 0;
	uint8_t report[50];
	
	report[index++] = 0xEE;report[index++] = 0xCC;
	index += 1;
	report[index++] = 0x60;
	/* 长度 */
	report[FRAME_LEGTH_INDEX] = index - 3;
	/* 检验 */
	report[index++] = CRC_Byte(&report[FRAME_AFN_INDEX],report[FRAME_LEGTH_INDEX]);
	report[index++] = 0x55;report[index++] = 0xAA;
	
	HAL_UART_Transmit(LocalComHandle, report, index, TX_TIMEOUT);
}
/**
  * @brief  读用户代码和密码 应答
  * @param  None
  * @retval ret 找到
  */
void LocalReadUserIdAndPassWordResp(uint8_t Sn,uint8_t *buf,uint8_t length)
{
	uint16_t index = 0;
	
	if(buf[FRAME_LEGTH_INDEX] == 11)
	{
		index = FRAME_TEXT_INDEX;
		/* 用户代码 */
		SysVar.UserID.u8[b0] = buf[index++];
		SysVar.UserID.u8[b1] = buf[index++];
		SysVar.UserID.u8[b2] = buf[index++];
		SysVar.UserID.u8[b3] = buf[index++];
		/* 密码 */
		SysVar.PassWord[b0] = buf[index++];
		SysVar.PassWord[b1] = buf[index++];
		SysVar.PassWord[b2] = buf[index++];
		SysVar.PassWord[b3] = buf[index++];
		SysVar.PassWord[b4] = buf[index++];
		SysVar.PassWord[b5] = buf[index++];
		
		SysVar.LocalTx.Flag = DISABLE;
		SysVar.LocalTx.Seconds.u16 = 0;
		SysVar.SM = SM_FIND_CARD;
	}
	else
	{
		LocalErrorResp(ERROR_TEXT);
	}
}
/**
  * @brief  ?
  * @param  None
  * @retval ret 找到
  */
void LocalUploadIcId(void)
{
	uint16_t index = 0;
	uint8_t report[50];
	
	report[index++] = 0xEE;report[index++] = 0xCC;
	index += 1;
	report[index++] = 0x62;
	report[index++] = SysVar.Card.Status;
	report[index++] = SysVar.Card.IdCurrent.u8[b3];
	report[index++] = SysVar.Card.IdCurrent.u8[b2];
	report[index++] = SysVar.Card.IdCurrent.u8[b1];
	report[index++] = SysVar.Card.IdCurrent.u8[b0];
	/* 余额 */
	report[index++] = SysVar.Card.Balance.u8[b3];
	report[index++] = SysVar.Card.Balance.u8[b2];
	report[index++] = SysVar.Card.Balance.u8[b1];
	report[index++] = SysVar.Card.Balance.u8[b0];
	/* 长度 */
	report[FRAME_LEGTH_INDEX] = index - 3;
	/* 检验 */
	report[index++] = CRC_Byte(&report[FRAME_AFN_INDEX],report[FRAME_LEGTH_INDEX]);
	report[index++] = 0x55;report[index++] = 0xAA;
	
	HAL_UART_Transmit(LocalComHandle, report, index, TX_TIMEOUT);
}
/**
  * @brief  上传IC卡号 应答
  * @param  None
  * @retval ret 找到
  */
void LocalUploadIcIdResp(uint8_t Sn,uint8_t *buf,uint8_t length)
{
	uint16_t index = 0;
	uu32 icidTemp;
	
	if(buf[FRAME_LEGTH_INDEX] == 5)
	{
		index = FRAME_TEXT_INDEX;
		icidTemp.u8[b0] = buf[index++];
		icidTemp.u8[b1] = buf[index++];
		icidTemp.u8[b2] = buf[index++];
		icidTemp.u8[b3] = buf[index++];
		if(icidTemp.u32 != SysVar.Card.IdCurrent.u32)
		{
			LocalErrorResp(ERROR_CARD_ID);
		}
	}
	else
	{
		LocalErrorResp(ERROR_TEXT);
	}
}
/**
  * @brief  读IC卡卡号
  * @param  None
  * @retval ret 找到
  */
void LocalReadIcId(uint8_t Sn,uint8_t *buf,uint8_t length)
{
	uint16_t index = 0;
	uint8_t report[50];
	
	report[index++] = 0xEE;report[index++] = 0xCC;
	index += 1;
	report[index++] = Sn;
	report[index++] = SysVar.Card.Status;
	report[index++] = SysVar.Card.IdCurrent.u8[b3];
	report[index++] = SysVar.Card.IdCurrent.u8[b2];
	report[index++] = SysVar.Card.IdCurrent.u8[b1];
	report[index++] = SysVar.Card.IdCurrent.u8[b0];
	/* 余额 */
	report[index++] = SysVar.Card.Balance.u8[b3];
	report[index++] = SysVar.Card.Balance.u8[b2];
	report[index++] = SysVar.Card.Balance.u8[b1];
	report[index++] = SysVar.Card.Balance.u8[b0];
	/* 长度 */
	report[FRAME_LEGTH_INDEX] = index - 3;
	/* 检验 */
	report[index++] = CRC_Byte(&report[FRAME_AFN_INDEX],report[FRAME_LEGTH_INDEX]);
	report[index++] = 0x55;report[index++] = 0xAA;
	
	HAL_UART_Transmit(LocalComHandle, report, index, TX_TIMEOUT);
}
/**
  * @brief  激活IC卡
  * @param  None
  * @retval ret 找到
  */
void LocalActivateIcCard(uint8_t Sn,uint8_t *buf,uint8_t length)
{
	uint16_t index = 0;
	uu32 icidTemp;
	uint8_t report[50];
	
	if(SysVar.Card.Status == CARD_HAVE)
	{
		index = FRAME_TEXT_INDEX;
		icidTemp.u8[b0] = buf[index++];
		icidTemp.u8[b1] = buf[index++];
		icidTemp.u8[b2] = buf[index++];
		icidTemp.u8[b3] = buf[index++];
		/* IC卡号相同 */
		if(icidTemp.u32 == SysVar.Card.IdCurrent.u32)
		{
			if(IcCardActivate() == 0)
			{
				index =0;
				report[index++] = 0xEE;report[index++] = 0xCC;
				index += 1;
				report[index++] = Sn;
				report[index++] = SysVar.Card.IdCurrent.u8[b3];
				report[index++] = SysVar.Card.IdCurrent.u8[b2];
				report[index++] = SysVar.Card.IdCurrent.u8[b1];
				report[index++] = SysVar.Card.IdCurrent.u8[b0];
				/* 长度 */
				report[FRAME_LEGTH_INDEX] = index - 3;
				/* 检验 */
				report[index++] = CRC_Byte(&report[FRAME_AFN_INDEX],report[FRAME_LEGTH_INDEX]);
				report[index++] = 0x55;report[index++] = 0xAA;
				
				HAL_UART_Transmit(LocalComHandle, report, index, TX_TIMEOUT);
			}
			else
			{
				LocalErrorResp(ERROR_ACTIVATE);
			}
		}
		else
		{
			LocalErrorResp(ERROR_CARD_ID);
		}

	}
	else
	{
		LocalErrorResp(ERROR_NO_CARD);
	}
}
/**
  * @brief  充值
  * @param  None
  * @retval ret 找到
  */
void LocalRecharge(uint8_t Sn,uint8_t *buf,uint8_t length)
{
	uint16_t index = 0;
	uu32 icidTemp;
	uu32 money;
	uint8_t report[50];
	
	if(SysVar.Card.Status == CARD_BALANCE)
	{
		if(buf[FRAME_LEGTH_INDEX] == 9)
		{
			index = FRAME_TEXT_INDEX;
			icidTemp.u8[b3] = buf[index++];
			icidTemp.u8[b2] = buf[index++];
			icidTemp.u8[b1] = buf[index++];
			icidTemp.u8[b0] = buf[index++];
			money.u8[b0] = buf[index++];			/* 低字节在前 */
			money.u8[b1] = buf[index++];
			money.u8[b2] = buf[index++];
			money.u8[b3] = buf[index++];
			if(icidTemp.u32 == SysVar.Card.IdCurrent.u32)
			{
				if(MFRC522_PcdValue(PICC_INCREMENT,IC_CARD_WALLET_BLOCK_ID,&(money.u8[b0])) == MI_OK)
				{
					/* 备份钱包 */
					MFRC522_PcdBakValue(IC_CARD_WALLET_BLOCK_ID,IC_CARD_BK_WALLET_BLOCK_ID);
					index =0;
					report[index++] = 0xEE;report[index++] = 0xCC;
					index += 1;
					report[index++] = Sn;
					report[index++] = SysVar.Card.IdCurrent.u8[b3];
					report[index++] = SysVar.Card.IdCurrent.u8[b2];
					report[index++] = SysVar.Card.IdCurrent.u8[b1];
					report[index++] = SysVar.Card.IdCurrent.u8[b0];
					report[index++] = money.u8[b3];
					report[index++] = money.u8[b2];
					report[index++] = money.u8[b1];
					report[index++] = money.u8[b0];
					/* 长度 */
					report[FRAME_LEGTH_INDEX] = index - 3;
					/* 检验 */
					report[index++] = CRC_Byte(&report[FRAME_AFN_INDEX],report[FRAME_LEGTH_INDEX]);
					report[index++] = 0x55;report[index++] = 0xAA;
					
					HAL_UART_Transmit(LocalComHandle, report, index, TX_TIMEOUT);
					}
				else
				{
					LocalErrorResp(ERROR_RECHARGE);
				}
			}
			else
			{
				LocalErrorResp(ERROR_CARD_ID);
			}
		}
		else
		{
			LocalErrorResp(ERROR_TEXT);
		}
	}
	else
	{
		LocalErrorResp(ERROR_NO_CARD);
	}
}

/**
  * @brief  错误应答
  * @param  None
  * @retval ret 找到
  */
void LocalErrorResp(uint8_t errorCode)
{
	uint16_t index;
	uint8_t report[100];
	
	/* 准备应答帧 */
	index = 0;
	report[index++] = 0xEE;report[index++] = 0xCC;
	index += 1;		/* 先跳过长度 */
	report[index++] = 0xFE;
	report[index++] = errorCode;
	/* 长度 */
	report[FRAME_LEGTH_INDEX] = index - 3;
	/* 检验位 */
	report[index++] = CRC_Byte(&report[FRAME_AFN_INDEX],report[FRAME_LEGTH_INDEX]);
	report[index++] = 0x55;report[index++] = 0xAA;
	
	HAL_UART_Transmit(LocalComHandle, report, index, TX_TIMEOUT);
}
/**
  * @brief  本地串口数据检验.
  * @param  None
  * @retval ret 找到
  */
uint16_t LocalChechDataIsOk(uint8_t *buf,uint16_t length)
{
	uint16_t loop;
	uint8_t frameStart[2] = {0xEE,0xCC};
	uint8_t frameEnd[2] = {0x55,0xAA};
	uint8_t crcTemp;
	uint8_t frameLength;
	uint16_t ret;
	
	ret = 0xFFFF;
	for(loop = 0;loop < length;loop++)
	{
		/* 帧头 */
		if((buf[loop] == frameStart[0]) && (buf[loop+1] == frameStart[1]))
		{
			frameLength = buf[loop+FRAME_LEGTH_INDEX];
			/* 帧尾 */
			if((buf[frameLength+4+loop] == frameEnd[0]) && (buf[frameLength+5+loop] == frameEnd[1]))
			{
				/* 检验 */
				crcTemp = CRC_Byte(&buf[loop+FRAME_AFN_INDEX],frameLength);
				if(buf[frameLength+3+loop] == crcTemp || buf[frameLength+3+loop] == 0)
				{
					ret = loop;
					loop = length;			/* 跳出for循环 */
				}
				else
				{
					LocalErrorResp(ERROR_CHECK);
				}
			}
		}
	}
	
	return ret;
}


