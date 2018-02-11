/**
  ******************************************************************************
  * File Name          : ICCard.c
  * Description        : 
  ******************************************************************************
	
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ICCard.h"
#include "common.h"
#include "MFRC522.h"
#include "string.h"
#include "LocalComDataProcess.h"
/* Private variables ---------------------------------------------------------*/
const uint8_t DefaultPassWord[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}; 


/* Private function prototypes -----------------------------------------------*/



/* Private function ----------------------------------------------------------*/
void IcCardProcess(void)
{
	uu32 temp;
	
	/* 每次寻卡前清除卡号 */
	SysVar.Card.IdCurrent.u32 = 0;
	/* 寻卡 */
	if(MFRC522_PcdRequest(PICC_REQALL,(uint8_t *)(&SysVar.Card.Type)) == MI_OK)
	{
		/* 防碰撞 */
		if(MFRC522_PcdAnticoll((uint8_t *)(&SysVar.Card.IdCurrent)) == MI_OK)
		{
			/* 选卡 */
			if(MFRC522_PcdSelect((uint8_t *)(&SysVar.Card.IdCurrent)) == MI_OK)
			{
				if(SysVar.Card.Status == CARD_NO)
				{
					SysVar.Card.ComeFlag = SET;
				}
				SysVar.Card.Status = CARD_HAVE;
				SysVar.Card.Balance.u32 = 0;
				/* 验证密码 */
				if(MFRC522_PcdAuthState(PICC_AUTHENT1B,IC_CARD_WALLET_BLOCK_ID, \
					SysVar.PassWord,(uint8_t *)(&SysVar.Card.IdCurrent)) == MI_OK)
				{
					SysVar.Card.Status = CARD_PASS_WORD;
					/* 读用户代码 */
					if(MFRC522_PcdRead(IC_CARD_USER_ID_BLOCK_ID,SysVar.CardBuf) == MI_OK)
					{
						temp.u8[b3] = SysVar.CardBuf[12];
						temp.u8[b2] = SysVar.CardBuf[13];
						temp.u8[b1] = SysVar.CardBuf[14];
						temp.u8[b0] = SysVar.CardBuf[15];
						/* 用户代码一致，说明此卡已经激活，只能进行充值操作 */
						if(temp.u32 == SysVar.UserID.u32)
						{
							SysVar.Card.Status = CARD_USER_ID;
							/* 读余额 */
							if(MFRC522_PcdRead(IC_CARD_WALLET_BLOCK_ID,SysVar.CardBuf) == MI_OK)
							{
								SysVar.Card.Balance.u8[b0] = SysVar.CardBuf[12];
								SysVar.Card.Balance.u8[b1] = SysVar.CardBuf[13];
								SysVar.Card.Balance.u8[b2] = SysVar.CardBuf[14];
								SysVar.Card.Balance.u8[b3] = SysVar.CardBuf[15];
								SysVar.Card.Status = CARD_BALANCE;
							}
						}
					}
				}
			}
		}
	}
	/* 无卡 */
	else
	{
		if(SysVar.Card.Status != CARD_NO)
		{
			SysVar.Card.IdCurrent.u32 = 0;
			LocalErrorResp(CARD_NO);
		}
		SysVar.Card.Status = CARD_NO;
	}
	/* 有新卡进入 */
	if(SysVar.Card.ComeFlag == SET)
	{
		SysVar.Card.ComeFlag = RESET;
		LocalUploadIcId();								/* 上报IC卡号 */
	}
}
/**
  * @brief  激活IC Card
  * @param  None
  * @retval ret 找到
  */
uint8_t IcCardActivate(void)
{
	uint8_t ret = 1;
	
	/* 验证默认密码B */
	if(MFRC522_PcdAuthState(PICC_AUTHENT1B,IC_CARD_WALLET_BLOCK_ID,(uint8_t *)DefaultPassWord, \
		(uint8_t *)(&SysVar.Card.IdCurrent)) == MI_OK)
	{
		/* 先写入用户代码 */
		SysVar.CardBuf[0] = 0;SysVar.CardBuf[1] = 0;SysVar.CardBuf[2] = 0;SysVar.CardBuf[3] = 0;
		SysVar.CardBuf[4] = 0;SysVar.CardBuf[5] = 0;SysVar.CardBuf[6] = 0;SysVar.CardBuf[7] = 0;
		SysVar.CardBuf[8] = 0;SysVar.CardBuf[9] = 0;SysVar.CardBuf[10] = 0;SysVar.CardBuf[11] = 0;
		SysVar.CardBuf[12] = SysVar.UserID.u8[b0];
		SysVar.CardBuf[13] = SysVar.UserID.u8[b1];
		SysVar.CardBuf[14] = SysVar.UserID.u8[b2];
		SysVar.CardBuf[15] = SysVar.UserID.u8[b3];
		if(MFRC522_PcdWrite(IC_CARD_USER_ID_BLOCK_ID,SysVar.CardBuf) == MI_OK)
		{
			/* 钱包地址清理 */
			if(MFRC522_PcdWrite(IC_CARD_WALLET_BLOCK_ID,SysVar.CardBuf) == MI_OK)
			{
				MFRC522_PcdWrite(IC_CARD_BK_WALLET_BLOCK_ID,SysVar.CardBuf);
				/* 修改控制字 */
				SysVar.CardBuf[0] = SysVar.PassWord[0];SysVar.CardBuf[1] = SysVar.PassWord[1];
				SysVar.CardBuf[2] = SysVar.PassWord[2];SysVar.CardBuf[3] = SysVar.PassWord[3];
				SysVar.CardBuf[4] = SysVar.PassWord[4];SysVar.CardBuf[5] = SysVar.PassWord[5];
				SysVar.CardBuf[6] = 0x73;SysVar.CardBuf[7] = 0xC3;			/*	控制块---不能读/写；用户代码块---只读;钱包块---读写*/
				SysVar.CardBuf[8] = 0xC8;SysVar.CardBuf[9] = 0x69;
				SysVar.CardBuf[10] = SysVar.PassWord[0];SysVar.CardBuf[11] = SysVar.PassWord[1];
				SysVar.CardBuf[12] = SysVar.PassWord[2];SysVar.CardBuf[13] = SysVar.PassWord[3];
				SysVar.CardBuf[14] = SysVar.PassWord[4];SysVar.CardBuf[15] = SysVar.PassWord[5];
				if(MFRC522_PcdWrite(IC_CARD_WALLET_BLOCK_ID+3,SysVar.CardBuf) == MI_OK)
				{
					ret = 0;
				}
			}
		}
	}
	
	return ret;
}
