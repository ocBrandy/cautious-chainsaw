/*
*********************************************************************************************************
*
*	模块名称 : 串口命令解析
*	文件名称 : Cmd_Parse.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*********************************************************************************************************
*/

#ifndef		_CMD_PARSE_H
#define		_CMD_PARSE_H

#define RXBUF_SIZE		10
#define TXBUF_SIZE      10
#define MSGBUF_SIZE		4

/* 宏定义通讯方式 */
#define USART_CMD		1
#define ETH_CMD			0

/* 类型字段枚举 */
typedef	 enum
{
	CMD_NULL = 0,		/* 空类型 */
	CON = 0x01,			/* 建立连接 */
	DISCON = 0x02,		/* 预留 */
	AUTO = 0x03,		/* 自动模式 */
	ONCE = 0x04			/* 手动模式 */
}CMD_TYPE_E;


/* 协议命令格式结构体 */
typedef struct
{
	const uint8_t  RxHead1;				/* 接收命令帧头1 */
	const uint8_t  RxHead2;				/* 接收命令帧头2 */
	const uint8_t  TxHead1;				/* 发送命令帧头1 */
	const uint8_t  TxHead2;				/* 发送命令帧头2 */
	uint8_t  add;						/* 节点地址 */
	CMD_TYPE_E  type;					/* 命令类型 */
	uint8_t  datalen;					/* 长度 */
	__IO uint8_t  MsgBufIndex;			/* 信息净荷索引 */
	__IO uint16_t RxBufIndex;			/* 接收缓冲区索引 */
	uint8_t  check;						/* 校验和 */
	uint8_t  OverFlag;					/* 接收一帧数据完成标志 */
	
	uint8_t  MsgBuf[MSGBUF_SIZE];		/* 信息净荷缓冲区 */	
	uint8_t  RxBuf[RXBUF_SIZE];			/* 命令接收缓冲区 */
	uint8_t  TxBuf[TXBUF_SIZE];			/* 命令发送缓冲区 */
	
}CMD_T;

/* 供外部调用API */
void CmdPoll(void);										/* 命令轮询 */
void CmdAns(CMD_T *_tCmd);								/* 命令应答 */
uint16_t CheckSum(uint8_t *_buf, uint8_t _len);			/* 计算校验和 */

#endif
