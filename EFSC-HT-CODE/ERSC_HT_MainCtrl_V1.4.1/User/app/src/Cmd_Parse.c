/*
**************************************************************************
* ��    �ƣ��������ģ��
* ˵    ����
* ��    ����V1.0
* ��    �ߣ�yy-AA55
* ����ʱ�䣺20190509
* �޸�˵����
*			2019-05-09		V1.0       	�װ�
*			2019-08-07		V1.1		���������������Ӵ��ڴ�ӡ��ͨ��
*										��#define cmd_printf	printf����
**************************************************************************
*/
#include "bsp.h"
#include "Cmd_Parse.h"
#include "USB_DMM.h"

//#define cmd_printf	printf
#define cmd_printf(...)

/* ��������֡�ṹ�壬������ҪУ�鲿�ָ���ֵ�����ݲ�ͬЭ�飬ֱ���޸Ļ���������Ӧֵ���� */
CMD_T g_tCmd ={
	.RxHead1 = 0x5A,			/* ��������֡ͷ��һ�ֽ� */
	.RxHead2 = 0xA5,			/* ��������֡ͷ�ڶ��ֽ� */
	.TxHead1 = 0x66,			/* ��������֡ͷ��һ�ֽ� */
	.TxHead2 = 0xCC,			/* ��������֡ͷ�ڶ��ֽ�*/
//	.add = 0xAA,				/* �ڵ��ַ */
	.messageId= 0x00      /* ��Ϣ���� */
	.type = CMD_NULL,			/* �������� */
	.datalen = 0,				/* ���ݳ��� */
	.MsgBufIndex = 0,			/* ��Ϣ�������� */
	.RxBufIndex = 0,			/* ����buf���� */
	.check = 0,					/* У��� */	
	.OverFlag = 0,				/* �������һ֡��־ */
};

static uint8_t CmdRxState = 0;

static void CmdPro(CMD_T *_tCmd);
static void CmdAnalysis(CMD_T *_tCmd);
static void ResetRxState(CMD_T *_tCmd);

/*
*****************************************************************************
 * ��  ����UsartCmdPoll
 * ��  �ܣ�����������ѯ
 * ��  ������
 * ����ֵ����
*****************************************************************************
*/
void CmdPoll(void)
{	
	/* ��һ����������ѯ */
	CmdPro(&g_tCmd);			
	/* �ڶ����������⵽�������һ֡���ͽ���������� */
	if(g_tCmd.OverFlag == 1)
	{
		/* �������+���� */
		CmdAnalysis(&g_tCmd);
		/* ��λ״̬���ȴ���һ֡���� */
		ResetRxState(&g_tCmd);
	}	
}

/*
*****************************************************************************
 * ��  ����CmdPro
 * ��  �ܣ������
 * ��  ����_tCmd:��Ҫ���������ṹ��
 * ����ֵ����
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
				/* У��֡ͷ1 */
				case 0:				
					if(ucData == _tCmd->RxHead1)
					{
						_tCmd->RxBuf[_tCmd->RxBufIndex++] = ucData;
						CmdRxState++;		/* ��ת������һ�� */
					}
					else
					{
						cmd_printf("\r\n֡ͷ1ʶ�����");
						/* ��λ����״̬ */
						ResetRxState(&g_tCmd);		
					}
					cmd_printf("\r\n֡ͷ1ʶ����ȷ");
					break;
				
				/* У��֡ͷ2 */
				case 1:				
					if(ucData == _tCmd->RxHead2)
					{
						_tCmd->RxBuf[_tCmd->RxBufIndex++] = ucData;
						CmdRxState++;		/* ��ת������һ�� */
					}
					else
					{
						cmd_printf("\r\n֡ͷ2ʶ�����");
						/* ��λ����״̬ */
						ResetRxState(&g_tCmd);		
					}
					cmd_printf("\r\n֡ͷ2ʶ����ȷ");
					break;
				
				/* У���ַ */
				case 2:				
					if(ucData == _tCmd->add)
					{
						_tCmd->RxBuf[_tCmd->RxBufIndex++] = ucData;
						CmdRxState++;		/* ��ת��һ�� */
					}
					else
					{
						cmd_printf("\r\n��ַʶ�����");
						/* ��λ����״̬ */
						ResetRxState(&g_tCmd);		
					}
					cmd_printf("\r\n��ַʶ����ȷ");
					break;

				/* �жϵ�ǰָ������ */
				case 3:		
					/* ͨ�ϲ�����Ϣ */
					if(ucData == CON)		
					{
						_tCmd->type = CON;
						_tCmd->RxBuf[_tCmd->RxBufIndex++] = ucData;
						CmdRxState++;		/* ��ת��һ�� */
						cmd_printf("\r\n����Ϊ����");
					}
					
					/* �Զ�ģʽ */
					else if(ucData == AUTO)		
					{
						_tCmd->type = AUTO;
						_tCmd->RxBuf[_tCmd->RxBufIndex++] = ucData;
						CmdRxState++;		/* ��ת��һ�� */
						cmd_printf("\r\n����Ϊ�Զ�����");
					}
					
					/* �ֶ�ģʽ */
					else if(ucData == ONCE)		
					{
						_tCmd->type = ONCE;
						_tCmd->RxBuf[_tCmd->RxBufIndex++] = ucData;
						CmdRxState++;		/* ��ת��һ�� */
						cmd_printf("\r\n����Ϊ�ֶ�����");
					}
					
					/* ������Ч����ʱ���˳����� */
					else
					{
						cmd_printf("\r\n������Ч��������");
						/* ��λ����״̬ */
						ResetRxState(&g_tCmd);		
					}
					break;

				/* ��ȡ�����ֽ� */
				case 4:				
					_tCmd->datalen = ucData;	/* ��ȡ����λ */
					_tCmd->RxBuf[_tCmd->RxBufIndex++] = ucData;
					/* ���ݳ����ֽڣ��ж��Ƿ������ݶ� */
					if(_tCmd->datalen == 0)
					{
						cmd_printf("\r\n�����ݶ�");
						CmdRxState = 6;					
					}
					else
					{
						cmd_printf("\r\n�����ݶΣ�����Ϊ %d",_tCmd.datalen);
						CmdRxState++;
					}
					break;

				/* ���ݳ����ֽڶ�ȡ���ݶ� */
				case 5:				
					_tCmd->MsgBuf[_tCmd->MsgBufIndex] = ucData;
					_tCmd->RxBuf[_tCmd->RxBufIndex++] = ucData;
					
					/* ����Э�鳤�Ȼ�ȡ���� */
					if(++_tCmd->MsgBufIndex >= _tCmd->datalen)
					{
						CmdRxState++;
					}	
					cmd_printf("\r\n��ȡ���ݶ�");
					break;

				/* У��ͼ�� */
				case 6:				
					_tCmd->check = ucData;
					_tCmd->RxBuf[_tCmd->RxBufIndex++] = ucData;
					
					/* �ж�У����Ƿ���ȷ */
					if(_tCmd->check  == CheckSum(_tCmd->RxBuf,_tCmd->RxBufIndex - 1))
					{
						_tCmd->OverFlag = 1;		/* ��ʾ������һ֡���� */
						cmd_printf("\r\nУ�����ȷ");
						cmd_printf("\r\n������Ϊ %02x",CheckSum(_tCmd->RxBuf,_tCmd->RxBufIndex - 1));
					}
					else
					{
						cmd_printf("\r\nУ��ʹ���");
						for(uint8_t i=0 ; i<RXBUF_SIZE; i++)
						{
							cmd_printf("\r\nRxBuf[%d] = %02x,RxBufIndex = %d",i,_tCmd->RxBuf[i],_tCmd->RxBufIndex);
						}
						cmd_printf("\r\n������Ϊ %02x",CheckSum(_tCmd->RxBuf,_tCmd->RxBufIndex - 1));
						ResetRxState(&g_tCmd);		/* ��λ����״̬ */
					}
			}
			continue;	/* ���ܻ������ݣ�����ʶ��һ�Σ�ȷ���������� */
		}	
		break;			/* ����һ֡����ʶ�� */
	}
}

/*
*****************************************************************************
 * ��  ����CmdAnalysis
 * ��  �ܣ��������
 * ��  ����_tCmd:��Ҫ���������ṹ��
 * ����ֵ����
*****************************************************************************
*/
static void CmdAnalysis(CMD_T *_tCmd)
{
	switch(_tCmd->type)
	{
		/* ������������ */
		case CON:
			/* ��ȷʶ��ָ��ʱ����ʾ����LED��˸�����ڴ�ӡ�� */
			bsp_LedOn(1);
			bsp_LedOff(2);
			bsp_LedOff(3);
			bsp_LedOff(4);

			/* ʶ��ָ���Ķ��� */
		
			/* ����Ӧ�� */
			CmdAns(&g_tCmd);				
			break;
	
		/* �Զ��������� */
		case AUTO:
			/* ��ȷʶ��ָ��ʱ����ʾ����LED��˸�����ڴ�ӡ�� */
			bsp_LedOn(2);
			bsp_LedOff(1);
			bsp_LedOff(3);
			bsp_LedOff(4);

			/* ʶ��ָ���Ķ��� */
			AutoMeas();
		
			/* ����Ӧ�� */
				
			break;

		/* �ֶ��������� */
		case ONCE:
			/* ��ȷʶ��ָ��ʱ����ʾ����LED��˸�����ڴ�ӡ�� */
			bsp_LedOn(3);
			bsp_LedOff(1);
			bsp_LedOff(2);
			bsp_LedOff(4);

			/* ʶ��ָ���Ķ��� */
			OnceMeas();
			
			/* ����Ӧ�� */
			CmdAns(&g_tCmd);
			break;
	
		/* ����������� */
		default:
			break;
	}
}

/*
*****************************************************************************
 * ��  ����ResetRxState
 * ��  �ܣ���λ��ǰ����״̬�����¿�ʼ�������
 * ��  ����_tCmd:��Ҫ���������ṹ��
 * ����ֵ����
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
 * ��  ����CmdAns
 * ��  �ܣ�������Ӧ
 * ��  ����_tCmd:��Ҫ���������ṹ��
 * ����ֵ����
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
			/* ����ǰ����״̬���뷢�ͻ����� */
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
 * ��  ����CheckSum
 * ��  �ܣ�����У���
 * ��  ����_buf:��Ҫ���������buf
 *		   _len����������ݳ���
 * ����ֵ��У��͵ĵ�λ�ֽ�
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

