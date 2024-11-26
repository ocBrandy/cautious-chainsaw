/*
**************************************************************************
* 名    称：usb万用表模块
* 说    明：
* 版    本：V1.0
* 作    者：yy-AA55
* 创建时间：20190520
* 修改说明：
*			2019-05-20		首版
**************************************************************************
*/
#include "bsp.h"
#include "USB_DMM.h"
#include "Cmd_Parse.h"

#define  LEFT_PORT		0xA5
#define  RIGHT_PORT	0x5A

/* 调用声明命令结构体 */
extern CMD_T g_tCmd;

/* 测量信息结构体 */
MEAS_MSG_T g_tMeasMsg;

static uint8_t state;

static void MeasConnect(uint8_t _Red,uint8_t _RedPin,uint8_t _Black,uint8_t _BlackPin);
static void GetMeasMsg(MEAS_MSG_T *_tMsg,uint8_t _Red,uint8_t _RedPin,uint8_t _Black,uint8_t _BlackPin);
static uint8_t WaitAns(void);

/*
*****************************************************************************
 * 函  数：AutoMeas
 * 功  能：自动测量所有通断
 * 参  数：无
 * 返回值：无
*****************************************************************************
*/
void AutoMeas(void)
{
	uint8_t RedNum;
	uint8_t BlackNum;

	/* 先测量仪器左右48芯端口的导通情况 */
	for(RedNum = 1; RedNum <= 48 ; RedNum++)
	{
		for(BlackNum = 1; BlackNum <= 48 ; BlackNum++)
		{	
			/* 连接当前测量引脚到万用表红黑表笔 */
//			MeasConnect(LEFT_PORT,RedNum,RIGHT_PORT,BlackNum);

			/* 向上位机发送当前测量状态 */
			GetMeasMsg(&g_tMeasMsg,LEFT_PORT,RedNum,RIGHT_PORT,BlackNum);
			CmdAns(&g_tCmd);
			if(WaitAns() != 1)		/* 等待上位机响应 */
			{
				return;				/* 接收响应超时或错误，终止本次自动测量 */
			}

			/* 阻塞等待上位机回应，回应后才可测量下一次，超时不回应返回 */
			
		}
	}

	/* 测量仪器左边48芯插头相邻引脚的绝缘情况 */
	for(RedNum = 1; RedNum <= 47;RedNum++)
	{
		for(BlackNum = RedNum + 1; BlackNum <= 48;BlackNum++)
		{
			/* 连接当前测量引脚到万用表红黑表笔 */
//			MeasConnect(LEFT_PORT,RedNum,LEFT_PORT,BlackNum);

			/* 向上位机发送当前测量状态 */
			GetMeasMsg(&g_tMeasMsg,LEFT_PORT,RedNum,LEFT_PORT,BlackNum);
			CmdAns(&g_tCmd);

			/* 阻塞等待上位机回应，回应后才可测量下一次，超时不回应返回 */
			
		}
	}

	/* 测量仪器右边48芯插头相邻引脚的绝缘情况 */
	for(RedNum = 1; RedNum <= 47;RedNum++)
	{
		for(BlackNum = RedNum + 1; BlackNum <= 48;BlackNum++)
		{
			/* 连接当前测量引脚到万用表红黑表笔 */
//			MeasConnect(RIGHT_PORT,RedNum,RIGHT_PORT,BlackNum);

			/* 向上位机发送当前测量状态 */
			GetMeasMsg(&g_tMeasMsg,RIGHT_PORT,RedNum,RIGHT_PORT,BlackNum);
			CmdAns(&g_tCmd);
			
			/* 阻塞等待上位机回应，回应后才可测量下一次，超时不回应返回 */
			
		}
	}
	
}

/*
*****************************************************************************
 * 函  数：OnceMeas
 * 功  能：手动单次测量
 * 参  数：无
 * 返回值：无
*****************************************************************************
*/
void OnceMeas(void)
{
	uint8_t Red;
	uint8_t RedPin;
	uint8_t Black;
	uint8_t BlackPin;

	/* 获取待测量信息 */
	Red = g_tCmd.MsgBuf[0];
	RedPin = g_tCmd.MsgBuf[1];
	Black = g_tCmd.MsgBuf[2];
	BlackPin = g_tCmd.MsgBuf[3];

	/* 连接测量引脚 */
	MeasConnect(Red,RedPin,Black,BlackPin);
}

/*
*****************************************************************************
 * 函  数：MeasConnect
 * 功  能：连接需要测量的引脚
 * 参  数：无
 * 返回值：无
*****************************************************************************
*/
static void MeasConnect(uint8_t _Red,uint8_t _RedPin,uint8_t _Black,uint8_t _BlackPin)
{
	/* 红色端接线 */
	if(_Red == LEFT_PORT)
	{
		HC595_WriteBit(ExtIO_A,_RedPin,1);
	}
	else if (_Red == RIGHT_PORT)
	{
		HC595_WriteBit(ExtIO_B,_RedPin,1);
	}

	/* 黑色端接线 */
	if(_Black == LEFT_PORT)
	{
		HC595_WriteBit(ExtIO_C,_BlackPin,1);
	}
	else if (_Black == RIGHT_PORT)
	{
		HC595_WriteBit(ExtIO_D,_BlackPin,1);
	}
	
}
/*
*****************************************************************************
 * 函  数：GetMeasMsg
 * 功  能：获取当前测量信息
 * 参  数：无
 * 返回值：无
*****************************************************************************
*/
static void GetMeasMsg(MEAS_MSG_T *_tMsg,uint8_t _Red,uint8_t _RedPin,uint8_t _Black,uint8_t _BlackPin)
{
	_tMsg->RedPort = _Red;
	_tMsg->RedPin = _RedPin;
	_tMsg->BlackPort = _Black;
	_tMsg->BlackPin = _BlackPin;
}

/*
*****************************************************************************
 * 函  数：ResetRxState
 * 功  能：复位当前接收状态，重新开始检测命令
 * 参  数：_tCmd:需要处理的命令结构体
 * 返回值：无
*****************************************************************************
*/
static void ResetRxState2(CMD_T *_tCmd)
{
	state = 0;
	_tCmd->datalen = 0;
	_tCmd->MsgBufIndex = 0;
	_tCmd->RxBufIndex = 0;
	_tCmd->type = CMD_NULL;
	_tCmd->OverFlag = 0;
	_tCmd->check = 0;
}

/*
*****************************************************************************
 * 函  数：WaitAns
 * 功  能：等待上位机响应
 * 参  数：无
 * 返回值：无
*****************************************************************************
*/
static uint8_t WaitAns(void)
{
	uint8_t ucData;
	uint8_t ucCount = 0;
	uint8_t TimeOutFlag = 0;
	
	/* 定义命令帧结构体，并对需要校验部分赋初值 */
	CMD_T tCmdAns ={
		.RxHead1 = 0xAA,			/* 接收命令帧头第一字节 */
		.RxHead2 = 0x55,			/* 接收命令帧头第二字节 */
		.TxHead1 = 0x66,			/* 发送命令帧头第一字节 */
		.TxHead2 = 0xCC,			/* 发送命令帧头第二自己 */
		.add = 0xAA,				/* 节点地址 */
		.type = CMD_NULL,			/* 命令类型 */
		.datalen = 0,				/* 数据长度 */
		.MsgBufIndex = 0,			/* 信息净荷索引 */
		.RxBufIndex = 0,			/* 接收buf索引 */
		.check = 0,					/* 校验和 */	
		.OverFlag = 0,				/* 接收完成一帧标志 */
	};
	
	while(1)
	{
		if(comGetChar(COM1,&ucData))
		{
//			comSendChar(COM1,ucData);
			bsp_StopTimer(0);
			TimeOutFlag = 0;
			switch(state)
			{
				/* 校验帧头1 */
				case 0:				
					if(ucData == tCmdAns.RxHead1)
					{
						tCmdAns.RxBuf[tCmdAns.RxBufIndex++] = ucData;
						state++;		/* 跳转进入下一步 */
					}
					else
					{
//						printf("\r\n帧头1识别错误");
						/* 复位接收状态 */
						ResetRxState2(&tCmdAns);
						return 0;						
					}
//					printf("\r\n帧头1识别正确");
					break;
				
				/* 校验帧头2 */
				case 1:				
					if(ucData == tCmdAns.RxHead2)
					{
						tCmdAns.RxBuf[tCmdAns.RxBufIndex++] = ucData;
						state++;		/* 跳转进入下一步 */
					}
					else
					{
//						printf("\r\n帧头2识别错误");
						/* 复位接收状态 */
						ResetRxState2(&tCmdAns);
						return 0;
					}
//					printf("\r\n帧头2识别正确");
					break;
				
				/* 校验地址 */
				case 2:				
					if(ucData == tCmdAns.add)
					{
						tCmdAns.RxBuf[tCmdAns.RxBufIndex++] = ucData;
						state++;		/* 跳转下一步 */
					}
					else
					{
//						printf("\r\n地址识别错误");
						/* 复位接收状态 */
						ResetRxState2(&tCmdAns);	
						return 0;
					}
//					printf("\r\n地址识别正确");
					break;

				/* 判断当前指令类型 */
				case 3:		
					/* 自动测量 */
					if(ucData == AUTO)		
					{
						tCmdAns.type = AUTO;
						tCmdAns.RxBuf[tCmdAns.RxBufIndex++] = ucData;
						state++;		/* 跳转下一步 */
//						printf("\r\n类型为连接");
					}
					
					/* 不是有效类型时，退出接收 */
					else
					{
//						printf("\r\n不是有效类型数据");
						/* 复位接收状态 */
						ResetRxState2(&tCmdAns);
						return 0;
					}
					break;

				/* 获取长度字节 */
				case 4:				
					tCmdAns.datalen = ucData;	/* 获取长度位 */
					tCmdAns.RxBuf[tCmdAns.RxBufIndex++] = ucData;
					/* 根据长度字节，判断是否有数据段 */
					if(tCmdAns.datalen == 0)
					{
//						printf("\r\n无数据段");
						/* 复位接收状态 */
						ResetRxState2(&tCmdAns);
						return 0;
					}
					else
					{
//						printf("\r\n有数据段，长度为 %d",_tCmd.datalen);
						state++;
					}
					break;

				/* 根据长度字节读取数据段 */
				case 5:				
					tCmdAns.MsgBuf[tCmdAns.MsgBufIndex] = ucData;
					tCmdAns.RxBuf[tCmdAns.RxBufIndex++] = ucData;
					
					/* 根据协议长度获取数据 */
					if(++tCmdAns.MsgBufIndex >= tCmdAns.datalen)
					{
						state++;
					}	
//					printf("\r\n获取数据段");
					break;

				/* 校验和检测 */
				case 6:				
					tCmdAns.check = ucData;
					tCmdAns.RxBuf[tCmdAns.RxBufIndex++] = ucData;
					
					/* 判断校验和是否正确 */
					if(tCmdAns.check  == CheckSum(tCmdAns.RxBuf,tCmdAns.RxBufIndex - 1))
					{
						tCmdAns.OverFlag = 1;		/* 表示接收完一帧数据 */
						return 1;
//						printf("\r\n校验和正确");
//						printf("\r\n计算结果为 %02x",CheckSum(_tCmd.RxBuf,_tCmd.RxBufIndex - 1));
					}
					else
					{
//						printf("\r\n校验和错误");
//						for(uint8_t i=0 ; i<RXBUF_SIZE; i++)
//						{
//							printf("\r\nRxBuf[%d] = %02x,RxBufIndex = %d",i,_tCmd.RxBuf[i],_tCmd.RxBufIndex);
//						}
//						printf("\r\n计算结果为 %02x",CheckSum(_tCmd.RxBuf,_tCmd.RxBufIndex - 1));
						ResetRxState2(&tCmdAns);		/* 复位接收状态 */
						return 0;
					}
			}
			continue;	/* 可能还有数据，继续识别一次，确保无数据了 */
		}
		else
		{
			if(TimeOutFlag == 0)
			{
				bsp_StartTimer(0, 1000);
				TimeOutFlag = 1;
			}
			if(bsp_CheckTimer(0))
			{
				if(++ucCount >= 10)			/* 重发10次后退出 */
				{
					return 0;				/* 超时，退出函数 */
				}
				ResetRxState2(&tCmdAns);	/* 复位接收状态 */
				CmdAns(&g_tCmd);			/* 重发一次测量状态 */
				TimeOutFlag = 0;				
			}
		}
	}
}

