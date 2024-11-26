/*
**************************************************************************
* ��    �ƣ�usb���ñ�ģ��
* ˵    ����
* ��    ����V1.0
* ��    �ߣ�yy-AA55
* ����ʱ�䣺20190520
* �޸�˵����
*			2019-05-20		�װ�
**************************************************************************
*/
#include "bsp.h"
#include "USB_DMM.h"
#include "Cmd_Parse.h"

#define  LEFT_PORT		0xA5
#define  RIGHT_PORT	0x5A

/* ������������ṹ�� */
extern CMD_T g_tCmd;

/* ������Ϣ�ṹ�� */
MEAS_MSG_T g_tMeasMsg;

static uint8_t state;

static void MeasConnect(uint8_t _Red,uint8_t _RedPin,uint8_t _Black,uint8_t _BlackPin);
static void GetMeasMsg(MEAS_MSG_T *_tMsg,uint8_t _Red,uint8_t _RedPin,uint8_t _Black,uint8_t _BlackPin);
static uint8_t WaitAns(void);

/*
*****************************************************************************
 * ��  ����AutoMeas
 * ��  �ܣ��Զ���������ͨ��
 * ��  ������
 * ����ֵ����
*****************************************************************************
*/
void AutoMeas(void)
{
	uint8_t RedNum;
	uint8_t BlackNum;

	/* �Ȳ�����������48о�˿ڵĵ�ͨ��� */
	for(RedNum = 1; RedNum <= 48 ; RedNum++)
	{
		for(BlackNum = 1; BlackNum <= 48 ; BlackNum++)
		{	
			/* ���ӵ�ǰ�������ŵ����ñ��ڱ�� */
//			MeasConnect(LEFT_PORT,RedNum,RIGHT_PORT,BlackNum);

			/* ����λ�����͵�ǰ����״̬ */
			GetMeasMsg(&g_tMeasMsg,LEFT_PORT,RedNum,RIGHT_PORT,BlackNum);
			CmdAns(&g_tCmd);
			if(WaitAns() != 1)		/* �ȴ���λ����Ӧ */
			{
				return;				/* ������Ӧ��ʱ�������ֹ�����Զ����� */
			}

			/* �����ȴ���λ����Ӧ����Ӧ��ſɲ�����һ�Σ���ʱ����Ӧ���� */
			
		}
	}

	/* �����������48о��ͷ�������ŵľ�Ե��� */
	for(RedNum = 1; RedNum <= 47;RedNum++)
	{
		for(BlackNum = RedNum + 1; BlackNum <= 48;BlackNum++)
		{
			/* ���ӵ�ǰ�������ŵ����ñ��ڱ�� */
//			MeasConnect(LEFT_PORT,RedNum,LEFT_PORT,BlackNum);

			/* ����λ�����͵�ǰ����״̬ */
			GetMeasMsg(&g_tMeasMsg,LEFT_PORT,RedNum,LEFT_PORT,BlackNum);
			CmdAns(&g_tCmd);

			/* �����ȴ���λ����Ӧ����Ӧ��ſɲ�����һ�Σ���ʱ����Ӧ���� */
			
		}
	}

	/* ���������ұ�48о��ͷ�������ŵľ�Ե��� */
	for(RedNum = 1; RedNum <= 47;RedNum++)
	{
		for(BlackNum = RedNum + 1; BlackNum <= 48;BlackNum++)
		{
			/* ���ӵ�ǰ�������ŵ����ñ��ڱ�� */
//			MeasConnect(RIGHT_PORT,RedNum,RIGHT_PORT,BlackNum);

			/* ����λ�����͵�ǰ����״̬ */
			GetMeasMsg(&g_tMeasMsg,RIGHT_PORT,RedNum,RIGHT_PORT,BlackNum);
			CmdAns(&g_tCmd);
			
			/* �����ȴ���λ����Ӧ����Ӧ��ſɲ�����һ�Σ���ʱ����Ӧ���� */
			
		}
	}
	
}

/*
*****************************************************************************
 * ��  ����OnceMeas
 * ��  �ܣ��ֶ����β���
 * ��  ������
 * ����ֵ����
*****************************************************************************
*/
void OnceMeas(void)
{
	uint8_t Red;
	uint8_t RedPin;
	uint8_t Black;
	uint8_t BlackPin;

	/* ��ȡ��������Ϣ */
	Red = g_tCmd.MsgBuf[0];
	RedPin = g_tCmd.MsgBuf[1];
	Black = g_tCmd.MsgBuf[2];
	BlackPin = g_tCmd.MsgBuf[3];

	/* ���Ӳ������� */
	MeasConnect(Red,RedPin,Black,BlackPin);
}

/*
*****************************************************************************
 * ��  ����MeasConnect
 * ��  �ܣ�������Ҫ����������
 * ��  ������
 * ����ֵ����
*****************************************************************************
*/
static void MeasConnect(uint8_t _Red,uint8_t _RedPin,uint8_t _Black,uint8_t _BlackPin)
{
	/* ��ɫ�˽��� */
	if(_Red == LEFT_PORT)
	{
		HC595_WriteBit(ExtIO_A,_RedPin,1);
	}
	else if (_Red == RIGHT_PORT)
	{
		HC595_WriteBit(ExtIO_B,_RedPin,1);
	}

	/* ��ɫ�˽��� */
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
 * ��  ����GetMeasMsg
 * ��  �ܣ���ȡ��ǰ������Ϣ
 * ��  ������
 * ����ֵ����
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
 * ��  ����ResetRxState
 * ��  �ܣ���λ��ǰ����״̬�����¿�ʼ�������
 * ��  ����_tCmd:��Ҫ���������ṹ��
 * ����ֵ����
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
 * ��  ����WaitAns
 * ��  �ܣ��ȴ���λ����Ӧ
 * ��  ������
 * ����ֵ����
*****************************************************************************
*/
static uint8_t WaitAns(void)
{
	uint8_t ucData;
	uint8_t ucCount = 0;
	uint8_t TimeOutFlag = 0;
	
	/* ��������֡�ṹ�壬������ҪУ�鲿�ָ���ֵ */
	CMD_T tCmdAns ={
		.RxHead1 = 0xAA,			/* ��������֡ͷ��һ�ֽ� */
		.RxHead2 = 0x55,			/* ��������֡ͷ�ڶ��ֽ� */
		.TxHead1 = 0x66,			/* ��������֡ͷ��һ�ֽ� */
		.TxHead2 = 0xCC,			/* ��������֡ͷ�ڶ��Լ� */
		.add = 0xAA,				/* �ڵ��ַ */
		.type = CMD_NULL,			/* �������� */
		.datalen = 0,				/* ���ݳ��� */
		.MsgBufIndex = 0,			/* ��Ϣ�������� */
		.RxBufIndex = 0,			/* ����buf���� */
		.check = 0,					/* У��� */	
		.OverFlag = 0,				/* �������һ֡��־ */
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
				/* У��֡ͷ1 */
				case 0:				
					if(ucData == tCmdAns.RxHead1)
					{
						tCmdAns.RxBuf[tCmdAns.RxBufIndex++] = ucData;
						state++;		/* ��ת������һ�� */
					}
					else
					{
//						printf("\r\n֡ͷ1ʶ�����");
						/* ��λ����״̬ */
						ResetRxState2(&tCmdAns);
						return 0;						
					}
//					printf("\r\n֡ͷ1ʶ����ȷ");
					break;
				
				/* У��֡ͷ2 */
				case 1:				
					if(ucData == tCmdAns.RxHead2)
					{
						tCmdAns.RxBuf[tCmdAns.RxBufIndex++] = ucData;
						state++;		/* ��ת������һ�� */
					}
					else
					{
//						printf("\r\n֡ͷ2ʶ�����");
						/* ��λ����״̬ */
						ResetRxState2(&tCmdAns);
						return 0;
					}
//					printf("\r\n֡ͷ2ʶ����ȷ");
					break;
				
				/* У���ַ */
				case 2:				
					if(ucData == tCmdAns.add)
					{
						tCmdAns.RxBuf[tCmdAns.RxBufIndex++] = ucData;
						state++;		/* ��ת��һ�� */
					}
					else
					{
//						printf("\r\n��ַʶ�����");
						/* ��λ����״̬ */
						ResetRxState2(&tCmdAns);	
						return 0;
					}
//					printf("\r\n��ַʶ����ȷ");
					break;

				/* �жϵ�ǰָ������ */
				case 3:		
					/* �Զ����� */
					if(ucData == AUTO)		
					{
						tCmdAns.type = AUTO;
						tCmdAns.RxBuf[tCmdAns.RxBufIndex++] = ucData;
						state++;		/* ��ת��һ�� */
//						printf("\r\n����Ϊ����");
					}
					
					/* ������Ч����ʱ���˳����� */
					else
					{
//						printf("\r\n������Ч��������");
						/* ��λ����״̬ */
						ResetRxState2(&tCmdAns);
						return 0;
					}
					break;

				/* ��ȡ�����ֽ� */
				case 4:				
					tCmdAns.datalen = ucData;	/* ��ȡ����λ */
					tCmdAns.RxBuf[tCmdAns.RxBufIndex++] = ucData;
					/* ���ݳ����ֽڣ��ж��Ƿ������ݶ� */
					if(tCmdAns.datalen == 0)
					{
//						printf("\r\n�����ݶ�");
						/* ��λ����״̬ */
						ResetRxState2(&tCmdAns);
						return 0;
					}
					else
					{
//						printf("\r\n�����ݶΣ�����Ϊ %d",_tCmd.datalen);
						state++;
					}
					break;

				/* ���ݳ����ֽڶ�ȡ���ݶ� */
				case 5:				
					tCmdAns.MsgBuf[tCmdAns.MsgBufIndex] = ucData;
					tCmdAns.RxBuf[tCmdAns.RxBufIndex++] = ucData;
					
					/* ����Э�鳤�Ȼ�ȡ���� */
					if(++tCmdAns.MsgBufIndex >= tCmdAns.datalen)
					{
						state++;
					}	
//					printf("\r\n��ȡ���ݶ�");
					break;

				/* У��ͼ�� */
				case 6:				
					tCmdAns.check = ucData;
					tCmdAns.RxBuf[tCmdAns.RxBufIndex++] = ucData;
					
					/* �ж�У����Ƿ���ȷ */
					if(tCmdAns.check  == CheckSum(tCmdAns.RxBuf,tCmdAns.RxBufIndex - 1))
					{
						tCmdAns.OverFlag = 1;		/* ��ʾ������һ֡���� */
						return 1;
//						printf("\r\nУ�����ȷ");
//						printf("\r\n������Ϊ %02x",CheckSum(_tCmd.RxBuf,_tCmd.RxBufIndex - 1));
					}
					else
					{
//						printf("\r\nУ��ʹ���");
//						for(uint8_t i=0 ; i<RXBUF_SIZE; i++)
//						{
//							printf("\r\nRxBuf[%d] = %02x,RxBufIndex = %d",i,_tCmd.RxBuf[i],_tCmd.RxBufIndex);
//						}
//						printf("\r\n������Ϊ %02x",CheckSum(_tCmd.RxBuf,_tCmd.RxBufIndex - 1));
						ResetRxState2(&tCmdAns);		/* ��λ����״̬ */
						return 0;
					}
			}
			continue;	/* ���ܻ������ݣ�����ʶ��һ�Σ�ȷ���������� */
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
				if(++ucCount >= 10)			/* �ط�10�κ��˳� */
				{
					return 0;				/* ��ʱ���˳����� */
				}
				ResetRxState2(&tCmdAns);	/* ��λ����״̬ */
				CmdAns(&g_tCmd);			/* �ط�һ�β���״̬ */
				TimeOutFlag = 0;				
			}
		}
	}
}

