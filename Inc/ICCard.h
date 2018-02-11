#ifndef __IC_CARD_H
#define __IC_CARD_H

#include "stm32f1xx_hal.h"

/* 刷卡状态机 */
#define CARD_HAVE											0x81			/* 有卡 */
#define CARD_PASS_WORD								0x82			/* 密码正确 */
#define CARD_USER_ID									0x83			/* 用户代码 */
#define CARD_BALANCE									0x84			/* 余额 */
#define CARD_NO												0x8F			/* 无卡 */

#define SCAN_IC_CARD_FREQUENCY				100				/* 刷卡频率 单位ms*/

#define IC_CARD_IS_ILLEGAL						0x90			/* 非法IC卡 */
#define DISPLAY_BALANCE								0x91			/* 显示余额 */
#define DISPLAY_MONETORY							0x92			/* 显示余额 */


void IcCardProcess(void);
uint8_t IcCardActivate(void);

#endif /* __IC_CARD_H */
