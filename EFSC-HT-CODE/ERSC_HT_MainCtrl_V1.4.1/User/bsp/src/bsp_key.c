/*
*********************************************************************************************************
*
*	ģ������ : ������������ģ��
*	�ļ����� : bsp_key.c
*	��    �� : V1.0
*	˵    �� : ɨ�������������������˲����ƣ����а���FIFO�����Լ�������¼���
*				(1) ��������
*				(2) ��������
*				(3) ������
*				(4) ����ʱ�Զ�����
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-02-01         ��ʽ����
*		V1.1    2013-06-29         ����1����ָ�룬����bsp_Idle() ������ȡϵͳ������ϼ���������
*								   ���� K1 K2 ��ϼ� �� K2 K3 ��ϼ�������ϵͳ����
*		V1.2    2015-08-08         K1��K2��K3�����������������жϣ��޸�  IsKeyDown1()�Ⱥ���
*
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "ERSC_HT_250ms.h"
#include "VGUS_LCD.h"
/*
	�����������Ӳ�������޸�GPIO����� IsKeyDown1 - IsKeyDown2 ����
	���ʵ�ʰ�����������2����������GPIO����� ��ӦIsKeyDown����
	#define KEY_COUNT    2	  ����� bsp_key.h �ļ��ж���
*/

/*
	���Ժк������ذ� �������߷��䣺
		K1 ��      :    (�͵�ƽ��ʾ����)
		K2 ��      :     (�͵�ƽ��ʾ����)
*/
extern void CmdSendToUnderGround(uint16_t cmd);
/* �����ڶ�Ӧ��RCCʱ�� */
#define RCC_ALL_KEY 	 RCC_APB2Periph_GPIOE
//�ƿ�
#define GPIO_PORT_K1    GPIOE
#define GPIO_PIN_K1	    GPIO_Pin_11
//���
#define GPIO_PORT_K2    GPIOE
#define GPIO_PIN_K2	    GPIO_Pin_10
//���
#define GPIO_PORT_K3    GPIOE
#define GPIO_PIN_K3	    GPIO_Pin_9
//٤��
#define GPIO_PORT_K4    GPIOE
#define GPIO_PIN_K4	    GPIO_Pin_14
//ѹ����
#define GPIO_PORT_K5    GPIOE
#define GPIO_PIN_K5	    GPIO_Pin_13
//ѹ����
#define GPIO_PORT_K6    GPIOE
#define GPIO_PIN_K6     GPIO_Pin_12


KEY_T s_tBtn[KEY_COUNT];
KEY_FIFO_T s_tKey;		/* ����FIFO����,�ṹ�� */

static void bsp_InitKeyVar(void);
static void bsp_InitKeyHard(void);
static void bsp_DetectKey(uint8_t i);

/*
*********************************************************************************************************
*	�� �� ��: IsKeyDownX
*	����˵��: �жϰ����Ƿ���
*	��    ��: ��
*	�� �� ֵ: ����ֵ1 ��ʾ���£�0��ʾδ����
*********************************************************************************************************
*/
#if 0	/* Ϊ������3���¼�:��K1��������, K2�������£� K1��K2ͬʱ���� */
static uint8_t IsKeyDown1(void)
{
	if ((GPIO_PORT_K1->IDR & GPIO_PIN_K1) == 0 && (GPIO_PORT_K2->IDR & GPIO_PIN_K2) == 0
		&& (GPIO_PORT_K3->IDR & GPIO_PIN_K3) != 0)
		return 1;
	else 
		return 0;
}
static uint8_t IsKeyDown2(void)
{
	if ((GPIO_PORT_K1->IDR & GPIO_PIN_K1) != 0 && (GPIO_PORT_K2->IDR & GPIO_PIN_K2) != 0
		&& (GPIO_PORT_K3->IDR & GPIO_PIN_K3) != 0)
		return 1;
	else 
		return 0;
}
static uint8_t IsKeyDown3(void)
{
	if ((GPIO_PORT_K1->IDR & GPIO_PIN_K1) != 0 && (GPIO_PORT_K2->IDR & GPIO_PIN_K2) == 0
		&& (GPIO_PORT_K3->IDR & GPIO_PIN_K3) == 0)
		return 1;
	else 
		return 0;
}
static uint8_t IsKeyDown9(void)	/* K1 K2��ϼ� */
{
	if ((GPIO_PORT_K1->IDR & GPIO_PIN_K1) == 0 && (GPIO_PORT_K2->IDR & GPIO_PIN_K2) != 0
		&& (GPIO_PORT_K3->IDR & GPIO_PIN_K3) != 0)
		return 1;
	else 
		return 0;
}
static uint8_t IsKeyDown10(void)	/* K2 K3��ϼ� */
{
	if ((GPIO_PORT_K1->IDR & GPIO_PIN_K1) != 0 && (GPIO_PORT_K2->IDR & GPIO_PIN_K2) != 0
		&& (GPIO_PORT_K3->IDR & GPIO_PIN_K3) == 0)
		return 1;
	else 
		return 0;
}
#else	
//�������£��ߵ�ƽ����1
static uint8_t IsKeyDownK1(void) {if ((GPIO_PORT_K1->IDR & GPIO_PIN_K1) != 0) return 1;else return 0;}
static uint8_t IsKeyDownK2(void) {if ((GPIO_PORT_K2->IDR & GPIO_PIN_K2) != 0) return 1;else return 0;}
static uint8_t IsKeyDownK3(void) {if ((GPIO_PORT_K3->IDR & GPIO_PIN_K3) != 0) return 1;else return 0;}
static uint8_t IsKeyDownK4(void) {if ((GPIO_PORT_K4->IDR & GPIO_PIN_K4) != 0) return 0;else return 1;}
static uint8_t IsKeyDownK5(void) {if ((GPIO_PORT_K5->IDR & GPIO_PIN_K5) != 0) return 0;else return 1;}
static uint8_t IsKeyDownK6(void) {if ((GPIO_PORT_K6->IDR & GPIO_PIN_K6) != 0) return 0;else return 1;}
//static uint8_t IsKeyDown10(void) {if (IsKeyDown1() && IsKeyDown2()) return 1;else return 0;}		/* K1 K2��ϼ� */
#endif
void init_key(void)
{
	
	s_tKey.Read = 0;
	s_tKey.Write = 0;	
	if(IsKeyDownK1() == 1)
	{
		onButtonUpdata_Rect(2,open,COLOR_GREEN); 
	}
	else
	{
		onButtonUpdata_Rect(2,close,COLOR_GREEN);		
	}
	if(IsKeyDownK2() == 1)
	{
		onButtonUpdata_Rect(3,open,COLOR_GREEN); 
	}
	else
	{
		onButtonUpdata_Rect(3,close,COLOR_GREEN);		
	}
	if(IsKeyDownK3() == 1)
	{
		onButtonUpdata_Rect(4,open,COLOR_GREEN); 
	}
	else
	{
		onButtonUpdata_Rect(4,close,COLOR_GREEN);		
	}
	if(IsKeyDownK4() == 1)
	{
		onButtonUpdata_Rect(0,open,COLOR_GREEN); 
	}
	else
	{
		onButtonUpdata_Rect(0,close,COLOR_GREEN);		
	}
	if(IsKeyDownK5() == 1)
	{
		
		onButtonUpdata_Rect(1,open,COLOR_GREEN); 
	}
	else
	{
		ButtonClear(1);//��ɫ��ȫ��ɫ		
//		onButtonUpdata_Rect(1,close,COLOR_GREEN);		
	}
	if(IsKeyDownK6() == 1)
	{
			
		onButtonUpdata_Rect(1,open,COLOR_GREEN); 
	}
	else
	{
		ButtonClear(1);//��ɫ��ȫ��ɫ	
//		onButtonUpdata_Rect(1,close,COLOR_GREEN);		
	}
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitKey
*	����˵��: ��ʼ������. �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitKey(void)
{
	bsp_InitKeyVar();		/* ��ʼ���������� */
	bsp_InitKeyHard();		/* ��ʼ������Ӳ�� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_PutKey
*	����˵��: ��1����ֵѹ�밴��FIFO��������������ģ��һ��������
*	��    ��:  _KeyCode : ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_PutKey(uint8_t _KeyCode)
{
	s_tKey.Buf[s_tKey.Write] = _KeyCode;

	if (++s_tKey.Write  >= KEY_FIFO_SIZE)
	{
		s_tKey.Write = 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetKey
*	����˵��: �Ӱ���FIFO��������ȡһ����ֵ��
*	��    ��:  ��
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
uint8_t bsp_GetKey(void)
{
	uint8_t ret;

	if (s_tKey.Read == s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tKey.Buf[s_tKey.Read];

		if (++s_tKey.Read >= KEY_FIFO_SIZE)
		{
			s_tKey.Read = 0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetKey2
*	����˵��: �Ӱ���FIFO��������ȡһ����ֵ�������Ķ�ָ�롣
*	��    ��:  ��
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
uint8_t bsp_GetKey2(void)
{
	uint8_t ret;

	if (s_tKey.Read2 == s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tKey.Buf[s_tKey.Read2];

		if (++s_tKey.Read2 >= KEY_FIFO_SIZE)
		{
			s_tKey.Read2 = 0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetKeyState
*	����˵��: ��ȡ������״̬
*	��    ��:  _ucKeyID : ����ID����0��ʼ
*	�� �� ֵ: 1 ��ʾ���£� 0 ��ʾδ����
*********************************************************************************************************
*/
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID)
{
	return s_tBtn[_ucKeyID].State;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SetKeyParam
*	����˵��: ���ð�������
*	��    �Σ�_ucKeyID : ����ID����0��ʼ
*			_LongTime : �����¼�ʱ��
*			 _RepeatSpeed : �����ٶ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed)
{
	s_tBtn[_ucKeyID].LongTime = _LongTime;			/* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
	s_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed;			/* �����������ٶȣ�0��ʾ��֧������ */
	s_tBtn[_ucKeyID].RepeatCount = 0;						/* ���������� */
}


/*
*********************************************************************************************************
*	�� �� ��: bsp_ClearKey
*	����˵��: ��հ���FIFO������
*	��    �Σ���
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
void bsp_ClearKey(void)
{
	s_tKey.Read = s_tKey.Write;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitKeyHard
*	����˵��: ���ð�����Ӧ��GPIO
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitKeyHard(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��1������GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_ALL_KEY, ENABLE);

	/* ��2�����������еİ���GPIOΪ��������ģʽ(ʵ����CPU��λ���������״̬) */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	/* ���븡��ģʽ */
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K1;
	GPIO_Init(GPIO_PORT_K1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K2;
	GPIO_Init(GPIO_PORT_K2, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K3;
	GPIO_Init(GPIO_PORT_K3, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K4;
	GPIO_Init(GPIO_PORT_K4, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K5;
	GPIO_Init(GPIO_PORT_K5, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K6;
	GPIO_Init(GPIO_PORT_K6, &GPIO_InitStructure);
	
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitKeyVar
*	����˵��: ��ʼ����������
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitKeyVar(void)
{
	uint8_t i;

	/* �԰���FIFO��дָ������ */
	s_tKey.Read = 0;
	s_tKey.Write = 0;
	s_tKey.Read2 = 0;

	/* ��ÿ�������ṹ���Ա������һ��ȱʡֵ */
	for (i = 0; i < KEY_COUNT; i++)
	{
		s_tBtn[i].LongTime = KEY_LONG_TIME;			/* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
		s_tBtn[i].Count = KEY_FILTER_TIME / 2;		/* ����������Ϊ�˲�ʱ���һ�� */
		s_tBtn[i].State = 0;							/* ����ȱʡ״̬��0Ϊδ���� */
		//s_tBtn[i].KeyCodeDown = 3 * i + 1;				/* �������µļ�ֵ���� */
		//s_tBtn[i].KeyCodeUp   = 3 * i + 2;				/* ��������ļ�ֵ���� */
		//s_tBtn[i].KeyCodeLong = 3 * i + 3;				/* �������������µļ�ֵ���� */
		s_tBtn[i].RepeatSpeed = 0;						/* �����������ٶȣ�0��ʾ��֧������ */
		s_tBtn[i].RepeatCount = 0;						/* ���������� */
	}
	/* �����Ҫ��������ĳ�������Ĳ����������ڴ˵������¸�ֵ */
	/* ���磬����ϣ������1���³���1s���Զ��ط���ͬ��ֵ */	
//	s_tBtn[KID_K5].LongTime = 100;
	s_tBtn[KID_K5].RepeatSpeed = 50;	/* ÿ��250ms�Զ����ͼ�ֵ */
	

	s_tBtn[KID_K6].RepeatSpeed = 50;
//	s_tBtn[KID_K6].LongTime = 100;
//	s_tBtn[KID_K6].State = 1;	
	/* �жϰ������µĺ�������ⰴ���Ƿ��£� */
	s_tBtn[0].IsKeyDownFunc = IsKeyDownK1;
	s_tBtn[1].IsKeyDownFunc = IsKeyDownK2;
	s_tBtn[2].IsKeyDownFunc = IsKeyDownK3;
	s_tBtn[3].IsKeyDownFunc = IsKeyDownK4;
	s_tBtn[4].IsKeyDownFunc = IsKeyDownK5;
	s_tBtn[5].IsKeyDownFunc = IsKeyDownK6;
	/* ��ϼ� */
//	s_tBtn[3].IsKeyDownFunc = IsKeyDown10;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_DetectKey
*	����˵��: ���һ��������������״̬�����뱻�����Եĵ��á�
*	��    ��:  �����ṹ����ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_DetectKey(uint8_t i)
{
	KEY_T *pBtn;

	/*
		���û�г�ʼ�������������򱨴�
		if (s_tBtn[i].IsKeyDownFunc == 0)
		{
			printf("Fault : DetectButton(), s_tBtn[i].IsKeyDownFunc undefine");
		}
	*/
//��������
	pBtn = &s_tBtn[i];
	if (pBtn->IsKeyDownFunc())
	{
		//�̰����������������
		if (pBtn->Count < KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count < 2 * KEY_FILTER_TIME)
		{
			pBtn->Count++;
		}
		else
		{
			if (pBtn->State == 0)
			{
				pBtn->State = 1;

				/* ���Ͱ�ť���µ���Ϣ */
				bsp_PutKey((uint8_t)(3 * i + 1));
//				comSendChar(COM1,(uint8_t)(3 * i + 1));	
			}
	//��������
			if (pBtn->LongTime > 0)
			{
				
				if (pBtn->LongCount < pBtn->LongTime)
				{
					/* ���Ͱ�ť�������µ���Ϣ */
					if (++pBtn->LongCount == pBtn->LongTime)
					{
						/* ��ֵ���밴��FIFO */
						bsp_PutKey((uint8_t)(3 * i + 3));
//						comSendChar(COM1,(uint8_t)(3 * i + 3));
					}
				}
				else
				{
					if (pBtn->RepeatSpeed > 0)
					{
						if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
						{
							pBtn->RepeatCount = 0;
							/* ��������ÿ��pBtn->RepeatSpeed*10ms����1������  */
							bsp_PutKey((uint8_t)(3 * i + 3));				
//						comSendChar(COM1,(uint8_t)(3 * i + 3));
						}
					}
				}
			}
		}
	}
	else
	{
		if(pBtn->Count > KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count != 0)
		{
			pBtn->Count--;
		}
		else
		{
			if (pBtn->State == 1)
			{
				pBtn->State = 0;

				/* �ɿ�����KEY_FILTER_TIME�� ���Ͱ�ť�������Ϣ */
				bsp_PutKey((uint8_t)(3 * i + 2));
//				comSendChar(COM1,(uint8_t)(3 * i + 2));
			}
		}

		pBtn->LongCount = 0;
		pBtn->RepeatCount = 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_KeyScan
*	����˵��: ɨ�����а���������������systick�ж������Եĵ���
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_KeyScan(void)
{
	uint8_t i;

	for (i = 0; i < KEY_COUNT; i++)
	{
		bsp_DetectKey(i);
	}
}

/***************************** yy-AA55 (END OF FILE) *********************************/
