#ifndef __MENU_H
#define __MENU_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

#define  MAX_MENU_LEVELS		4

#define MENU_SM_NORMAL			0x11		/* ����ģʽ */
#define MENU_SM_DIGIT				0x12		/* �������ѽ�� */
#define MENU_SM_CONSUME			0x13		/* ���� */
#define MENU_SM_SETUP				0x14		/* ���� */
#define MENU_SM_QUERY				0x15		/* ��ѯ */
#define MENU_SM_FUNCTION		0x16		/* �����л� */


void Menu_Start(void);
void Menu_DisplayBalance(void);
#endif /* __MENU_H */
