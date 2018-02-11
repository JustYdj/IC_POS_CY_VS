#ifndef _CMD_FUNC_TABLE_H
#define _CMD_FUNC_TABLE_H
#include "stm32f1xx.h"

typedef void (*CmdFuncTypedef_p)(uint8_t Sn,uint8_t *buf,uint8_t length);

typedef struct
{
    uint8_t CmdSn;                     /* ������ */
    CmdFuncTypedef_p pCmdFunc;    /* ��ں���ָ�� */
} CmdFuncWithSnTypedef_s;




extern const CmdFuncWithSnTypedef_s sLocalCmdFuncWithSn[256];
#endif //_CMD_FUNC_TABLE_H

