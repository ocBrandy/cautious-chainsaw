/*
*********************************************************************************************************
*
*	ģ������ : �����������
*	�ļ����� : Cmd_Parse.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*********************************************************************************************************
*/

#ifndef		_CMD_PARSE_H
#define		_CMD_PARSE_H

#define RXBUF_SIZE		10
#define TXBUF_SIZE      10
#define MSGBUF_SIZE		4

/* �궨��ͨѶ��ʽ */
#define USART_CMD		1
#define ETH_CMD			0

/* �����ֶ�ö�� */
typedef	 enum
{
	CMD_NULL = 0,		/* ������ */
	CON = 0x01,			/* �������� */
	DISCON = 0x02,		/* Ԥ�� */
	AUTO = 0x03,		/* �Զ�ģʽ */
	ONCE = 0x04			/* �ֶ�ģʽ */
}CMD_TYPE_E;


/* Э�������ʽ�ṹ�� */
typedef struct
{
	const uint8_t  RxHead1;				/* ��������֡ͷ1 */
	const uint8_t  RxHead2;				/* ��������֡ͷ2 */
	const uint8_t  TxHead1;				/* ��������֡ͷ1 */
	const uint8_t  TxHead2;				/* ��������֡ͷ2 */
	uint8_t  add;						/* �ڵ��ַ */
	CMD_TYPE_E  type;					/* �������� */
	uint8_t  datalen;					/* ���� */
	__IO uint8_t  MsgBufIndex;			/* ��Ϣ�������� */
	__IO uint16_t RxBufIndex;			/* ���ջ��������� */
	uint8_t  check;						/* У��� */
	uint8_t  OverFlag;					/* ����һ֡������ɱ�־ */
	
	uint8_t  MsgBuf[MSGBUF_SIZE];		/* ��Ϣ���ɻ����� */	
	uint8_t  RxBuf[RXBUF_SIZE];			/* ������ջ����� */
	uint8_t  TxBuf[TXBUF_SIZE];			/* ����ͻ����� */
	
}CMD_T;

/* ���ⲿ����API */
void CmdPoll(void);										/* ������ѯ */
void CmdAns(CMD_T *_tCmd);								/* ����Ӧ�� */
uint16_t CheckSum(uint8_t *_buf, uint8_t _len);			/* ����У��� */

#endif
