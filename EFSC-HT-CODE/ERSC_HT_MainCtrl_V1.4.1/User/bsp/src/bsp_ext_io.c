/*
**************************************************************************
* ��    �ƣ�IO��չģ��
* ˵    ��������74HC595������IO��չģ�飬����IO��չ��
* ��    ����V1.0
* ��    �ߣ�yy-AA55
* ����ʱ�䣺20190422
* �޸�˵����
*			2019-04-22		�װ�
**************************************************************************
*/
#include "bsp.h"

/*
	IO��չģ��GPIO���䣺
	DATA1/PC7		OE1/PC8
	SHCP1/PC9		STCP1/PA11
	DATA2/PA12		OE2/PC10
	SHCP2/PC11		STCP2/PC12
	DATA3/PD2		OE3/PD3
	SHCP3/PD7		STCP3/PB5
	DATA4/PB8		OE4/PB9
	SHCP4/PE0		STCP4/PE1

	һ����չ��4��EXTIO��ÿ����չ48��IO��6Ƭ74HC595����
	ÿ����չIO�������ܽ��໥����
*/

#define RCC_HC595_PORT			RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE

/* ��һ��EXTIO_A�������ܽź궨�� */
#define HC595_DATA1_PORT		GPIOC
#define HC595_DATA1_PIN			GPIO_Pin_7
#define HC595_OE1_PORT			GPIOC
#define HC595_OE1_PIN			GPIO_Pin_8
#define HC595_SHCP1_PORT		GPIOC
#define HC595_SHCP1_PIN			GPIO_Pin_9
#define HC595_STCP1_PORT		GPIOA
#define HC595_STCP1_PIN			GPIO_Pin_11

/* �ڶ���EXTIO_B�������ܽź궨�� */
#define HC595_DATA2_PORT		GPIOA
#define HC595_DATA2_PIN			GPIO_Pin_12
#define HC595_OE2_PORT			GPIOC
#define HC595_OE2_PIN			GPIO_Pin_10
#define HC595_SHCP2_PORT		GPIOC
#define HC595_SHCP2_PIN			GPIO_Pin_11
#define HC595_STCP2_PORT		GPIOA
#define HC595_STCP2_PIN			GPIO_Pin_12

/* ������EXTIO_C�������ܽź궨�� */
#define HC595_DATA3_PORT		GPIOD
#define HC595_DATA3_PIN			GPIO_Pin_2
#define HC595_OE3_PORT			GPIOD
#define HC595_OE3_PIN			GPIO_Pin_3
#define HC595_SHCP3_PORT		GPIOD
#define HC595_SHCP3_PIN			GPIO_Pin_7
#define HC595_STCP3_PORT		GPIOB
#define HC595_STCP3_PIN			GPIO_Pin_5

/* ������EXTIO_D�������ܽź궨�� */
#define HC595_DATA4_PORT		GPIOB
#define HC595_DATA4_PIN			GPIO_Pin_8
#define HC595_OE4_PORT			GPIOB
#define HC595_OE4_PIN			GPIO_Pin_9
#define HC595_SHCP4_PORT		GPIOE
#define HC595_SHCP4_PIN			GPIO_Pin_0
#define HC595_STCP4_PORT		GPIOE
#define HC595_STCP4_PIN			GPIO_Pin_1

/* ����EXT_A�ĺ� */
void HC595_Data1_1(void)		{HC595_DATA1_PORT->BSRR = HC595_DATA1_PIN;}
void HC595_Data1_0(void)		{HC595_DATA1_PORT->BRR = HC595_DATA1_PIN;}

void HC595_OE1_1(void)			{HC595_OE1_PORT->BSRR = HC595_OE1_PIN;}
void HC595_OE1_0(void)			{HC595_OE1_PORT->BRR = HC595_OE1_PIN;}

void HC595_Shcp1_1(void)		{HC595_SHCP1_PORT->BSRR = HC595_SHCP1_PIN;}
void HC595_Shcp1_0(void)		{HC595_SHCP1_PORT->BRR = HC595_SHCP1_PIN;}

void HC595_Stcp1_1(void)		{HC595_STCP1_PORT->BSRR = HC595_STCP1_PIN;}
void HC595_Stcp1_0(void)		{HC595_STCP1_PORT->BRR = HC595_STCP1_PIN;}

/* ����EXT_B�ĺ� */
void HC595_Data2_1(void)		{HC595_DATA2_PORT->BSRR = HC595_DATA2_PIN;}
void HC595_Data2_0(void)		{HC595_DATA2_PORT->BRR = HC595_DATA2_PIN;}

void HC595_OE2_1(void)			{HC595_OE2_PORT->BSRR = HC595_OE2_PIN;}
void HC595_OE2_0(void)			{HC595_OE2_PORT->BRR = HC595_OE2_PIN;}

void HC595_Shcp2_1(void)		{HC595_SHCP2_PORT->BSRR = HC595_SHCP2_PIN;}
void HC595_Shcp2_0(void)		{HC595_SHCP2_PORT->BRR = HC595_SHCP2_PIN;}

void HC595_Stcp2_1(void)		{HC595_STCP2_PORT->BSRR = HC595_STCP2_PIN;}
void HC595_Stcp2_0(void)		{HC595_STCP2_PORT->BRR = HC595_STCP2_PIN;}

/* ����EXT_C�ĺ� */
void HC595_Data3_1(void)		{HC595_DATA3_PORT->BSRR = HC595_DATA3_PIN;}
void HC595_Data3_0(void)		{HC595_DATA3_PORT->BRR = HC595_DATA3_PIN;}

void HC595_OE3_1(void)			{HC595_OE3_PORT->BSRR = HC595_OE3_PIN;}
void HC595_OE3_0(void)			{HC595_OE3_PORT->BRR = HC595_OE3_PIN;}

void HC595_Shcp3_1(void)		{HC595_SHCP3_PORT->BSRR = HC595_SHCP3_PIN;}
void HC595_Shcp3_0(void)		{HC595_SHCP3_PORT->BRR = HC595_SHCP3_PIN;}

void HC595_Stcp3_1(void)		{HC595_STCP3_PORT->BSRR = HC595_STCP3_PIN;}
void HC595_Stcp3_0(void)		{HC595_STCP3_PORT->BRR = HC595_STCP3_PIN;}

/* ����EXT_D�ĺ� */
void HC595_Data4_1(void)		{HC595_DATA4_PORT->BSRR = HC595_DATA4_PIN;}
void HC595_Data4_0(void)		{HC595_DATA4_PORT->BRR = HC595_DATA4_PIN;}

void HC595_OE4_1(void)			{HC595_OE4_PORT->BSRR = HC595_OE4_PIN;}
void HC595_OE4_0(void)			{HC595_OE4_PORT->BRR = HC595_OE4_PIN;}

void HC595_Shcp4_1(void)		{HC595_SHCP4_PORT->BSRR = HC595_SHCP4_PIN;}
void HC595_Shcp4_0(void)		{HC595_SHCP4_PORT->BRR = HC595_SHCP4_PIN;}

void HC595_Stcp4_1(void)		{HC595_STCP4_PORT->BSRR = HC595_STCP4_PIN;}
void HC595_Stcp4_0(void)		{HC595_STCP4_PORT->BRR = HC595_STCP4_PIN;}

#define EXTIO_PORT_NUM		6		/* ÿ����չIO�˿����õ�74HC595���� */

static ExtIO_T  s_tExtIO[EXTIO_PORT_NUM];		/* ������չIO�Ľṹ�� */

static void bsp_InitHC595Gpio(void);
static void bsp_InitHC595Var(void);

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitHC595
*	����˵��: ��ʼ��HC595
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitHC595(void)
{
	bsp_InitHC595Var();			/* �ṹ���������ֵ */
	bsp_InitHC595Gpio();		/* ��ʼ������GPIO */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitHC595Var
*	����˵��: ��ʼ��HC595��Ӧ�Ľṹ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitHC595Var(void)
{
	s_tExtIO[0]._port = ExtIO_A;
	s_tExtIO[0].Data_0 = HC595_Data1_0;
	s_tExtIO[0].Data_1 = HC595_Data1_1;
	s_tExtIO[0].OE_0 = HC595_OE1_0;
	s_tExtIO[0].OE_1 = HC595_OE1_1;
	s_tExtIO[0].SHCP_0 = HC595_Shcp1_0;
	s_tExtIO[0].SHCP_1 = HC595_Shcp1_1;
	s_tExtIO[0].STCP_0 = HC595_Stcp1_0;
	s_tExtIO[0].STCP_1 = HC595_Stcp1_1;

	s_tExtIO[1]._port = ExtIO_B;
	s_tExtIO[1].Data_0 = HC595_Data2_0;
	s_tExtIO[1].Data_1 = HC595_Data2_1;
	s_tExtIO[1].OE_0 = HC595_OE2_0;
	s_tExtIO[1].OE_1 = HC595_OE2_1;
	s_tExtIO[1].SHCP_0 = HC595_Shcp2_0;
	s_tExtIO[1].SHCP_1 = HC595_Shcp2_1;
	s_tExtIO[1].STCP_0 = HC595_Stcp2_0;
	s_tExtIO[1].STCP_1 = HC595_Stcp2_1;

	s_tExtIO[2]._port = ExtIO_C;
	s_tExtIO[2].Data_0 = HC595_Data3_0;
	s_tExtIO[2].Data_1 = HC595_Data3_1;
	s_tExtIO[2].OE_0 = HC595_OE3_0;
	s_tExtIO[2].OE_1 = HC595_OE3_1;
	s_tExtIO[2].SHCP_0 = HC595_Shcp3_0;
	s_tExtIO[2].SHCP_1 = HC595_Shcp3_1;
	s_tExtIO[2].STCP_0 = HC595_Stcp3_0;
	s_tExtIO[2].STCP_1 = HC595_Stcp3_1;

	s_tExtIO[3]._port = ExtIO_D;
	s_tExtIO[3].Data_0 = HC595_Data4_0;
	s_tExtIO[3].Data_1 = HC595_Data4_1;
	s_tExtIO[3].OE_0 = HC595_OE4_0;
	s_tExtIO[3].OE_1 = HC595_OE4_1;
	s_tExtIO[3].SHCP_0 = HC595_Shcp4_0;
	s_tExtIO[3].SHCP_1 = HC595_Shcp4_1;
	s_tExtIO[3].STCP_0 = HC595_Stcp4_0;
	s_tExtIO[3].STCP_1 = HC595_Stcp4_1;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitHC595Gpio
*	����˵��: ��ʼ��HC595��GPIO
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitHC595Gpio(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_HC595_PORT,ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	/* ��ʼ��EXT_A�����ܽ� */
	GPIO_InitStruct.GPIO_Pin = HC595_DATA1_PIN;
	GPIO_Init(HC595_DATA1_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = HC595_SHCP1_PIN;
	GPIO_Init(HC595_SHCP1_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = HC595_STCP1_PIN;
	GPIO_Init(HC595_STCP1_PORT,&GPIO_InitStruct);

	/* ��ʼ��EXT_B�����ܽ� */
	GPIO_InitStruct.GPIO_Pin = HC595_DATA2_PIN;
	GPIO_Init(HC595_DATA2_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = HC595_SHCP2_PIN;
	GPIO_Init(HC595_SHCP2_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = HC595_STCP2_PIN;
	GPIO_Init(HC595_STCP2_PORT,&GPIO_InitStruct);

	/* ��ʼ��EXT_C�����ܽ� */
	GPIO_InitStruct.GPIO_Pin = HC595_DATA3_PIN;
	GPIO_Init(HC595_DATA3_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = HC595_SHCP3_PIN;
	GPIO_Init(HC595_SHCP3_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = HC595_STCP3_PIN;
	GPIO_Init(HC595_STCP3_PORT,&GPIO_InitStruct);

	/* ��ʼ��EXT_D�����ܽ� */
	GPIO_InitStruct.GPIO_Pin = HC595_DATA4_PIN;
	GPIO_Init(HC595_DATA4_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = HC595_SHCP4_PIN;
	GPIO_Init(HC595_SHCP4_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = HC595_STCP4_PIN;
	GPIO_Init(HC595_STCP4_PORT,&GPIO_InitStruct);
	
	/* OE�ⲿ�����������óɿ�© */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStruct.GPIO_Pin = HC595_OE1_PIN;
	GPIO_Init(HC595_OE1_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = HC595_OE2_PIN;
	GPIO_Init(HC595_OE2_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = HC595_OE3_PIN;
	GPIO_Init(HC595_OE3_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = HC595_OE4_PIN;
	GPIO_Init(HC595_OE4_PORT,&GPIO_InitStruct);

	/* ��ʼ��ʱ��ʹ���������ߣ�ʹ���������ڼĴ����� */
	HC595_OE1_1();
	HC595_OE2_1();
	HC595_OE3_1();
	HC595_OE4_1();
}

/*
*********************************************************************************************************
*	�� �� ��: HC595_Delay
*	����˵��: HC595������ʱ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void HC595_Delay(void)
{
	uint8_t i;

	for(i = 1; i>0; i--);
}

/*
*********************************************************************************************************
*	�� �� ��: HC595_SendByte
*	����˵��: ����һ���ֽ�
*	��    �Σ�_pExt����չIO�ڵĽṹ��ָ��
*			  _ucByte��׼�����͵�����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void HC595_SendByte(ExtIO_T *_pExt, uint8_t _ucByte)
{
	uint8_t i;
	
	/* ����һ�ֽڣ�ÿ�δ���һ��bit */
	for(i=0 ;i<8 ;i++)
	{
		if(_ucByte & 0x80)
		{
			_pExt->Data_1();
		}
		else
		{
			_pExt->Data_0();
		}
		/* SHCP����һ�������أ�����ǰbit������λ�Ĵ����� */
		_pExt->SHCP_0();
		HC595_Delay();
		_pExt->SHCP_1();
		HC595_Delay();
		_pExt->SHCP_0();

		_ucByte <<= 1;		/* ����һλ */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: HC595_StcpEN
*	����˵��: ʹ�ܴ洢�Ĵ����������ݴ���λ�Ĵ���װ��洢�Ĵ���
*	��    �Σ�_pExt����չIO�ڵĽṹ��ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void HC595_StcpEN(ExtIO_T *_pExt)
{
	/* STCP�����أ����ݴ���洢�Ĵ��� */
	_pExt->STCP_0();
	HC595_Delay();
	_pExt->STCP_1();
	HC595_Delay();
	_pExt->STCP_0();	
}

/*
*********************************************************************************************************
*	�� �� ��: HC595_OutEN
*	����˵��: HC595���ʹ��
*	��    �Σ�_pExt����չIO�ڵĽṹ��ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void HC595_OutEN(ExtIO_T *_pExt)
{
	_pExt->OE_0();		/* OE�������ͣ����ݴӴ洢�Ĵ������µ��ܽ� */
}

/*
*********************************************************************************************************
*	�� �� ��: HC595_OutDis
*	����˵��: HC595���ʧ��
*	��    �Σ�_pExt����չIO�ڵĽṹ��ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void HC595_OutDis(ExtIO_T *_pExt)
{
	_pExt->OE_1();		/* OE�������ߣ���ֹ��д�Ĵ���ʱ�����״̬�����仯 */
}

/*
*********************************************************************************************************
*	�� �� ��: HC595_SendLen
*	����˵��: �������ⳤ���ֽ�
*	��    �Σ�_port����չIO�Ķ˿ں�
*			  _pbuf�����ͻ�����ָ��
*			  _uclen���������ݳ��ȣ���λ�ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void HC595_SendLen(ExtIO_Port_E _port, uint8_t *_pbuf, uint8_t _uclen)
{
	uint8_t i;
	ExtIO_T *p;

	switch(_port)
	{
		case ExtIO_A:
			p = &s_tExtIO[0];
			break;

		case ExtIO_B:
			p = &s_tExtIO[1];
			break;

		case ExtIO_C:
			p = &s_tExtIO[2];
			break;

		case ExtIO_D:
			p = &s_tExtIO[3];
			break;
		
		default:
			break;
	}

	/* 
		HC595������������ʱ�����ȷ��͵�һ�ֽڻᱻ�������һƬ595�У�
		�����Ҫ�����ͣ��ȷ������һ�ֽ�
	*/
	HC595_OutDis(p);		/* ��ʧ��������ţ��޸������״̬��������ʹ�� */	

	_pbuf += (_uclen - 1);
	for(i = 0 ;i < _uclen ;i++)
	{		
		HC595_SendByte(p ,*_pbuf);
		_pbuf--;
	}
	HC595_StcpEN(p);		/* STCP�����أ����ݴ���λ�Ĵ�������洢�Ĵ��� */
	HC595_OutEN(p);			/* OEʹ�ܣ�������� */
	
}

/*
*********************************************************************************************************
*	�� �� ��: HC595_WriteBit
*	����˵��: ����Ӧ����չIO���1��0
*	��    �Σ�_port����չIO�˿ں�
*			  _pin����������IO,	1-48
*			  _value����λ��λ��ֻ������0��1
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void HC595_WriteBit(ExtIO_Port_E _port, uint8_t _pin, uint8_t _value)
{
	uint8_t buf[EXTIO_PORT_NUM] = {0};		//���Ҫ���͸�HC595������
	uint8_t uctemp = 0;
	uint8_t Num,NumOfSingle;
	
	/* д�벻��0��1�Ļ������� */
	if(_value > 1)
	{
		printf("������д����\r\n");
		return;	
	}
	
	if(_pin == 0 && _pin > 48)
	{
		printf("������д����\r\n");
		return;	
	}

	Num = (_pin - 1) /8;				/* ������Ҫ�����ڼ���HC595 */
	NumOfSingle = (_pin-1) %8;			/* ���㵥�ֽ���λ��λ��ƫ���� */
	
	/* ���Է��֣�HC595д��һ�ֽ�ʱ����д�����λ�����λ��Ӧ��Q0 */
	uctemp = (_value << NumOfSingle);
	buf[Num] = uctemp;

	HC595_SendLen(_port,buf,EXTIO_PORT_NUM);	
	
//	for(uint8_t i=0 ; i<EXTIO_PORT_NUM ;i++)
//	{
//		printf("\r\n buf[%d] = %02x",i,buf[i]);	
//	}	
//	printf("\r\n ��pin%d ��Ϊ %d",_pin ,_value);
}

/*
*********************************************************************************************************
*	�� �� ��: HC595_Reset
*	����˵��: ��λ������չIO��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void HC595_Reset(void)
{
	HC595_WriteBit(ExtIO_A,1,0);
	HC595_WriteBit(ExtIO_B,1,0);
	HC595_WriteBit(ExtIO_C,1,0);
	HC595_WriteBit(ExtIO_D,1,0);
}

/***************************** yy-AA55 (END OF FILE) *********************************/
