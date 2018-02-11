/**
  ******************************************************************************
  * File Name          : menu.c
  * Description        : ∞¥º¸…®√Ë£¨CH450Ω”ø⁄◊”≥Ã–Ú
  ******************************************************************************

  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "menu.h"
#include "common.h"
#include "LCD12864.h"
#include "ICCard.h"
/* Private typedef -----------------------------------------------------------*/
typedef void (* tMenuFunc)(void);

/* Private variables ---------------------------------------------------------*/
tMenuFunc *CurrentMenu;
tMenuFunc *psPrevMenu[MAX_MENU_LEVELS];
uint8_t MenuItemIndex = 0, nMenuLevel = 0;

/* Private function prototypes -----------------------------------------------*/

/* Private function ----------------------------------------------------------*/
/**
  * @brief  ÷˜≤Àµ•
  * @param  None
  * @retval None
  */
void Menu_Start(void)
{
	LCD12864_ClearDisplay();
	LCD12864_DisplayString(1,2,(uint8_t *)"≥‰÷µª˙");
	LCD12864_DisplayString(2,0,(uint8_t *)"ø®∫≈£∫");
	LCD12864_DisplayString(3,0,(uint8_t *)"”‡∂Ó£∫");
//	LCD12864_DisplayString(4,0,(uint8_t *)"≥‰÷µ£∫");
}
/**
  * @brief  ÷˜≤Àµ•
  * @param  None
  * @retval None
  */
void Menu_DisplayBalance(void)
{
	uint8_t temp[16] = {0};
	
	switch(SysVar.Card.Status)
	{
		case CARD_HAVE :
			/* ø®∫≈ */
			temp[0] = HexToASCII((SysVar.Card.IdCurrent.u8[b0] & 0xF0)>>4);
			temp[1] = HexToASCII((SysVar.Card.IdCurrent.u8[b0] & 0x0F));
			temp[2] = HexToASCII((SysVar.Card.IdCurrent.u8[b1] & 0xF0)>>4);
			temp[3] = HexToASCII((SysVar.Card.IdCurrent.u8[b1] & 0x0F));
			temp[4] = HexToASCII((SysVar.Card.IdCurrent.u8[b2] & 0xF0)>>4);
			temp[5] = HexToASCII((SysVar.Card.IdCurrent.u8[b2] & 0x0F));
			temp[6] = HexToASCII((SysVar.Card.IdCurrent.u8[b3] & 0xF0)>>4);
			temp[7] = HexToASCII((SysVar.Card.IdCurrent.u8[b3] & 0x0F));
			LCD12864_ClearDisplay();
			LCD12864_DisplayString(1,2,(uint8_t *)"≥‰÷µª˙");
			LCD12864_DisplayString(2,0,(uint8_t *)"ø®∫≈£∫");
			LCD12864_DisplayString(2,4,temp);
			LCD12864_DisplayString(3,0,(uint8_t *)"”‡∂Ó£∫");
//			LCD12864_DisplayString(4,0,(uint8_t *)"≥‰÷µ£∫");
			break;
		case CARD_NO :
			break;
		case CARD_USER_ID :
			break;
		case CARD_BALANCE :
			/* ø®∫≈ */
			temp[0] = HexToASCII((SysVar.Card.IdCurrent.u8[b0] & 0xF0)>>4);
			temp[1] = HexToASCII((SysVar.Card.IdCurrent.u8[b0] & 0x0F));
			temp[2] = HexToASCII((SysVar.Card.IdCurrent.u8[b1] & 0xF0)>>4);
			temp[3] = HexToASCII((SysVar.Card.IdCurrent.u8[b1] & 0x0F));
			temp[4] = HexToASCII((SysVar.Card.IdCurrent.u8[b2] & 0xF0)>>4);
			temp[5] = HexToASCII((SysVar.Card.IdCurrent.u8[b2] & 0x0F));
			temp[6] = HexToASCII((SysVar.Card.IdCurrent.u8[b3] & 0xF0)>>4);
			temp[7] = HexToASCII((SysVar.Card.IdCurrent.u8[b3] & 0x0F));
			LCD12864_ClearDisplay();
			LCD12864_DisplayString(1,2,(uint8_t *)"≥‰÷µª˙");
			LCD12864_DisplayString(2,0,(uint8_t *)"ø®∫≈£∫");
			LCD12864_DisplayString(2,4,temp);
			/* ”‡∂Ó */
			temp[0] = HexToASCII((SysVar.Card.Balance.u8[b0] & 0xF0)>>4);
			temp[1] = HexToASCII((SysVar.Card.Balance.u8[b0] & 0x0F));
			temp[2] = HexToASCII((SysVar.Card.Balance.u8[b1] & 0xF0)>>4);
			temp[3] = HexToASCII((SysVar.Card.Balance.u8[b1] & 0x0F));
			temp[4] = HexToASCII((SysVar.Card.Balance.u8[b2] & 0xF0)>>4);
			temp[5] = HexToASCII((SysVar.Card.Balance.u8[b2] & 0x0F));
			temp[6] = '.';
			temp[7] = HexToASCII((SysVar.Card.Balance.u8[b3] & 0xF0)>>4);
			temp[8] = HexToASCII((SysVar.Card.Balance.u8[b3] & 0x0F));
			LCD12864_DisplayString(3,0,(uint8_t *)"”‡∂Ó£∫");
			LCD12864_DisplayString(3,4,temp);
//			LCD12864_DisplayString(4,0,(uint8_t *)"≥‰÷µ£∫");
			break;
		case CARD_PASS_WORD :
			break;
		default :
			break;
			
	}

}
