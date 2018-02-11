#ifndef __MENU_H
#define __MENU_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

#define  MAX_MENU_LEVELS		4

#define MENU_SM_NORMAL			0x11		/* 正常模式 */
#define MENU_SM_DIGIT				0x12		/* 键入消费金额 */
#define MENU_SM_CONSUME			0x13		/* 消费 */
#define MENU_SM_SETUP				0x14		/* 设置 */
#define MENU_SM_QUERY				0x15		/* 查询 */
#define MENU_SM_FUNCTION		0x16		/* 功能切换 */


void Menu_Start(void);
void Menu_DisplayBalance(void);
#endif /* __MENU_H */
