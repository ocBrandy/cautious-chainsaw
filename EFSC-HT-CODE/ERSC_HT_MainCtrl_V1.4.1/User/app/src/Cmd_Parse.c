/*
**************************************************************************
* 名    称：命令解析模块
* 说    明：
* 版    本：V1.0
* 作    者：yy-AA55
* 创建时间：20190509
* 修改说明：
*			2019-05-09		V1.0       	首版
*			2019-08-07		V1.1		命令解析过程中添加串口打印，通过
*										宏#define cmd_printf	printf开启
**************************************************************************
*/
#include "bsp.h"
#include "Cmd_Parse.h"
#include "USB_DMM.h"

//#define cmd_printf	printf
#define cmd_printf(...)

/* 定义命令帧结构体，并对需要校验部分赋初值，根据不同协议，直接修改或者新增对应值即可 */
CMD_T g_tCmd ={
	.RxHead1 = 0x5A,			/* 接收命令帧头第一字节 */
	.RxHead2 = 0xA5,			/* 接收命令帧头第二字节 */
	.TxHead1 = 0x66,			/* 发送命令帧头第一字节 */
	.TxHead2 = 0xCC,			/* 发送命令帧头第二字节*/
//	.add = 0xAA,				/* 节点地址 */
	.messageId= 0x00      /* 消息类型 */
	.type = CMD_NULL,			/* 命令类型 */
	.datalen = 0,				/* 数据长度 */
	.MsgBufIndex = 0,			/* 信息净荷索引 */
	.RxBufIndex = 0,			/* 接收buf索引 */
	.check = 0,					/* 校验和 */	
	.OverFlag = 0,				/* 接收完成一帧标志 */
};

static uint8_t CmdRxState = 0;

static void CmdPro(CMD_T *_tCmd);
static void CmdAnalysis(CMD_T *_tCmd);
static void ResetRxState(CMD_T *_tCmd);

/*
*****************************************************************************
 * 函  数：UsartCmdPoll
 * 功  能：串口命令轮询
 * 参  数：无
 * 返回值：无
*****************************************************************************
*/
void CmdPoll(void)
{	
	/* 第一步：命令轮询 */
	CmdPro(&g_tCmd);			
	/* 第二步：如果检测到接收完成一帧，就进行命令解析 */
	if(g_tCmd.OverFlag == 1)
	{
		/* 命令解析+动作 */
		CmdAnalysis(&g_tCmd);
		/* 复位状态，等待下一帧数据 */
		ResetRxState(&g_tCmd);
	}	
}

/*
*****************************************************************************
 * 函  数：CmdPro
 * 功  能：命令处理
 * 参  数：_tCmd:需要处理的命令结构体
 * 返回值：无
*****************************************************************************
*/
static void CmdPro(CMD_T *_tCmd)
{
	uint8_t ucData;
	
	while(1)
	{
	#if USART_CMD == 1
		if(comGetChar(COM2,&ucData))
	#endif
		{
			switch(CmdRxState)
			{
				/* 校验帧头1 */
				case 0:				
					if(ucData == _tCmd->RxHead1)
					{
						_tCmd->RxBuf[_tCmd->RxBufIndex++] = ucData;
						CmdRxState++;		/* 跳转进入下一步 */
					}
					else
					{
						cmd_printf("\r\n帧头1识别错误");
						/* 复位接收状态 */
						ResetRxState(&g_tCmd);		
					}
					cmd_printf("\r\n帧头1识别正确");
					break;
				
				/* 校验帧头2 */
				case 1:				
					if(ucData == _tCmd->RxHead2)
					{
						_tCmd->RxBuf[_tCmd->RxBufIndex++] = ucData;
						CmdRxState++;		/* 跳转进入下一步 */
					}
					else
					{
						cmd_printf("\r\n帧头2识别错误");
						/* 复位接收状态 */
						ResetRxState(&g_tCmd);		
					}
					cmd_printf("\r\n帧头2识别正确");
					break;
				
				/* 校验地址 */
				case 2:				
					if(ucData == _tCmd->add)
					{
						_tCmd->RxBuf[_tCmd->RxBufIndex++] = ucData;
						CmdRxState++;		/* 跳转下一步 */
					}
					else
					{
						cmd_printf("\r\n地址识别错误");
						/* 复位接收状态 */
						ResetRxState(&g_tCmd);		
					}
					cmd_printf("\r\n地址识别正确");
					break;

				/* 判断当前指令类型 */
				case 3:		
					/* 通断测试消息 */
					if(ucData == CON)		
					{
						_tCmd->type = CON;
						_tCmd->RxBuf[_tCmd->RxBufIndex++] = ucData;
						CmdRxState++;		/* 跳转下一步 */
						cmd_printf("\r\n类型为连接");
					}
					
					/* 自动模式 */
					else if(ucData == AUTO)		
					{
						_tCmd->type = AUTO;
						_tCmd->RxBuf[_tCmd->RxBufIndex++] = ucData;
						CmdRxState++;		/* 跳转下一步 */
						cmd_printf("\r\n类型为自动测量");
					}
					
					/* 手动模式 */
					else if(ucData == ONCE)		
					{
						_tCmd->type = ONCE;
						_tCmd->RxBuf[_tCmd->RxBufIndex++] = ucData;
						CmdRxState++;		/* 跳转下一步 */
						cmd_printf("\r\n类型为手动测量");
					}
					
					/* 不是有效类型时，退出接收 */
					else
					{
						cmd_printf("\r\n不是有效类型数据");
						/* 复位接收状态 */
						ResetRxState(&g_tCmd);		
					}
					break;

				/* 获取长度字节 */
				case 4:				
					_tCmd->datalen = ucData;	/* 获取长度位 */
					_tCmd->RxBuf[_tCmd->RxBufIndex++] = ucData;
					/* 根据长度字节，判断是否有数据段 */
					if(_tCmd->datalen == 0)
					{
						cmd_printf("\r\n无数据段");
						CmdRxState = 6;					
					}
					else
					{
						cmd_printf("\r\n有数据段，长度为 %d",_tCmd.datalen);
						CmdRxState++;
					}
					break;

				/* 根据长度字节读取数据段 */
				case 5:				
					_tCmd->MsgBuf[_tCmd->MsgBufIndex] = ucData;
					_tCmd->RxBuf[_tCmd->RxBufIndex++] = ucData;
					
					/* 根据协议长度获取数据 */
					if(++_tCmd->MsgBufIndex >= _tCmd->datalen)
					{
						CmdRxState++;
					}	
					cmd_printf("\r\n获取数据段");
					break;

				/* 校验和检测 */
				case 6:				
					_tCmd->check = ucData;
					_tCmd->RxBuf[_tCmd->RxBufIndex++] = ucData;
					
					/* 判断校验和是否正确 */
					if(_tCmd->check  == CheckSum(_tCmd->RxBuf,_tCmd->RxBufIndex - 1))
					{
						_tCmd->OverFlag = 1;		/* 表示接收完一帧数据 */
						cmd_printf("\r\n校验和正确");
						cmd_printf("\r\n计算结果为 %02x",CheckSum(_tCmd->RxBuf,_tCmd->RxBufIndex - 1));
					}
					else
					{
						cmd_printf("\r\n校验和错误");
						for(uint8_t i=0 ; i<RXBUF_SIZE; i++)
						{
							cmd_printf("\r\nRxBuf[%d] = %02x,RxBufIndex = %d",i,_tCmd->RxBuf[i],_tCmd->RxBufIndex);
						}
						cmd_printf("\r\n计算结果为 %02x",CheckSum(_tCmd->RxBuf,_tCmd->RxBufIndex - 1));
						ResetRxState(&g_tCmd);		/* 复位接收状态 */
					}
			}
			continue;	/* 可能还有数据，继续识别一次，确保无数据了 */
		}	
		break;			/* 结束一帧数据识别 */
	}
}

/*
*****************************************************************************
 * 函  数：CmdAnalysis
 * 功  能：命令解析
 * 参  数：_tCmd:需要处理的命令结构体
 * 返回值：无
*****************************************************************************
*/
static void CmdAnalysis(CMD_T *_tCmd)
{
	switch(_tCmd->type)
	{
		/* 建立连接命令 */
		case CON:
			/* 正确识别指令时的提示，如LED闪烁，串口打印等 */
			bsp_LedOn(1);
			bsp_LedOff(2);
			bsp_LedOff(3);
			bsp_LedOff(4);

			/* 识别指令后的动作 */
		
			/* 命令应答 */
			CmdAns(&g_tCmd);				
			break;
	
		/* 自动测量命令 */
		case AUTO:
			/* 正确识别指令时的提示，如LED闪烁，串口打印等 */
			bsp_LedOn(2);
			bsp_LedOff(1);
			bsp_LedOff(3);
			bsp_LedOff(4);

			/* 识别指令后的动作 */
			AutoMeas();
		
			/* 命令应答 */
				
			break;

		/* 手动测量命令 */
		case ONCE:
			/* 正确识别指令时的提示，如LED闪烁，串口打印等 */
			bsp_LedOn(3);
			bsp_LedOff(1);
			bsp_LedOff(2);
			bsp_LedOff(4);

			/* 识别指令后的动作 */
			OnceMeas();
			
			/* 命令应答 */
			CmdAns(&g_tCmd);
			break;
	
		/* 其他命令不处理 */
		default:
			break;
	}
}

/*
*****************************************************************************
 * 函  数：ResetRxState
 * 功  能：复位当前接收状态，重新开始检测命令
 * 参  数：_tCmd:需要处理的命令结构体
 * 返回值：无
*****************************************************************************
*/
static void ResetRxState(CMD_T *_tCmd)
{
	CmdRxState = 0;
	_tCmd->datalen = 0;
	_tCmd->MsgBufIndex = 0;
	_tCmd->RxBufIndex = 0;
	_tCmd->type = CMD_NULL;
	_tCmd->OverFlag = 0;
	_tCmd->check = 0;
}

/*
*****************************************************************************
 * 函  数：CmdAns
 * 功  能：命令响应
 * 参  数：_tCmd:需要处理的命令结构体
 * 返回值：无
*****************************************************************************
*/
extern MEAS_MSG_T g_tMeasMsg;
void CmdAns(CMD_T *_tCmd)
{
	uint8_t uclen = 0;
	uint8_t sum = 0;
	
	_tCmd->TxBuf[uclen++] = _tCmd->TxHead1;
	_tCmd->TxBuf[uclen++] = _tCmd->TxHead2;
	_tCmd->TxBuf[uclen++] = 0x55;
	_tCmd->TxBuf[uclen++] = _tCmd->type;
	switch(_tCmd->type)
	{
		case CON:
			_tCmd->TxBuf[uclen++] = 0x00;
			break;
		
		case AUTO:
			_tCmd->TxBuf[uclen++] = 0x04;
			/* 将当前测量状态加入发送缓冲区 */
			_tCmd->TxBuf[uclen++] = g_tMeasMsg.RedPort;
			_tCmd->TxBuf[uclen++] = g_tMeasMsg.RedPin;
			_tCmd->TxBuf[uclen++] = g_tMeasMsg.BlackPort;
			_tCmd->TxBuf[uclen++] = g_tMeasMsg.BlackPin;
			break;
		
		case ONCE:
			_tCmd->TxBuf[uclen++] = 0x00;
			break;
		
		default:
			break;
	}
	sum = CheckSum(_tCmd->TxBuf, uclen);
	_tCmd->TxBuf[uclen++] = sum;

	comSendBuf(COM1, _tCmd->TxBuf, uclen);
}


	
/*
*****************************************************************************
 * 函  数：CheckSum
 * 功  能：计算校验和
 * 参  数：_buf:需要计算的数据buf
 *		   _len：计算的数据长度
 * 返回值：校验和的低位字节
*****************************************************************************
*/
uint16_t CheckSum(uint8_t *_buf, uint8_t _len)
{
	uint16_t sum = 0;
	uint8_t i;
	 
	for(i = 0 ; i<_len ; i++)
	{
		sum += _buf[i];
	}
	
	return sum;
}

