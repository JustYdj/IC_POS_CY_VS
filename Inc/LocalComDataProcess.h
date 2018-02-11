#ifndef __LOCAL_COM_DATA_PROCESS
#define __LOCAL_COM_DATA_PROCESS


#define ERROR_AFN							0xC0			/* ������ */
#define ERROR_CHECK						0xC1			/* ���� */
#define ERROR_DEV_ADDR				0xC2			/* �豸��ַ */
#define ERROR_PASS_WORD				0xC3			/* ���� */
#define ERROR_TEXT						0xC4			/* ��Ϣ���� */
#define ERROR_NO_CARD					0xC5			/* �޿� */
#define ERROR_CARD_ID					0xC6			/* IC���� */
#define ERROR_ACTIVATE				0xC7			/* ����ʧ�� */
#define ERROR_RECHARGE				0xC8			/* ����ʧ�� */

#define FRAME_LEGTH_INDEX				2
#define FRAME_AFN_INDEX					3
#define FRAME_TEXT_INDEX				4

#define TX_MESSAGE_TIME_MAX			180  /* ��λS*/




void LocalComDataProcess(void);
void LocalReadUserIdAndPassWord(void);
void LocalReadUserIdAndPassWordResp(uint8_t Sn,uint8_t *buf,uint8_t length);
void LocalUploadIcId(void);
void LocalUploadIcIdResp(uint8_t Sn,uint8_t *buf,uint8_t length);
void LocalReadIcId(uint8_t Sn,uint8_t *buf,uint8_t length);
void LocalActivateIcCard(uint8_t Sn,uint8_t *buf,uint8_t length);
void LocalRecharge(uint8_t Sn,uint8_t *buf,uint8_t length);
void LocalErrorResp(uint8_t errorCode);
uint16_t LocalChechDataIsOk(uint8_t *buf,uint16_t length);
#endif /* __LOCAL_COM_DATA_PROCESS */
