#ifndef __IC_CARD_H
#define __IC_CARD_H

#include "stm32f1xx_hal.h"

/* ˢ��״̬�� */
#define CARD_HAVE											0x81			/* �п� */
#define CARD_PASS_WORD								0x82			/* ������ȷ */
#define CARD_USER_ID									0x83			/* �û����� */
#define CARD_BALANCE									0x84			/* ��� */
#define CARD_NO												0x8F			/* �޿� */

#define SCAN_IC_CARD_FREQUENCY				100				/* ˢ��Ƶ�� ��λms*/

#define IC_CARD_IS_ILLEGAL						0x90			/* �Ƿ�IC�� */
#define DISPLAY_BALANCE								0x91			/* ��ʾ��� */
#define DISPLAY_MONETORY							0x92			/* ��ʾ��� */


void IcCardProcess(void);
uint8_t IcCardActivate(void);

#endif /* __IC_CARD_H */
