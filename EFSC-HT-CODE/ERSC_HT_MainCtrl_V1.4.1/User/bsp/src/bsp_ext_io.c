/*
**************************************************************************
* 名    称：IO扩展模块
* 说    明：基于74HC595级联的IO扩展模块，用于IO扩展板
* 版    本：V1.0
* 作    者：yy-AA55
* 创建时间：20190422
* 修改说明：
*			2019-04-22		首版
**************************************************************************
*/
#include "bsp.h"

/*
	IO扩展模块GPIO分配：
	DATA1/PC7		OE1/PC8
	SHCP1/PC9		STCP1/PA11
	DATA2/PA12		OE2/PC10
	SHCP2/PC11		STCP2/PC12
	DATA3/PD2		OE3/PD3
	SHCP3/PD7		STCP3/PB5
	DATA4/PB8		OE4/PB9
	SHCP4/PE0		STCP4/PE1

	一共扩展了4组EXTIO，每组扩展48个IO，6片74HC595级联
	每组扩展IO的驱动管脚相互独立
*/

#define RCC_HC595_PORT			RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE

/* 第一组EXTIO_A的驱动管脚宏定义 */
#define HC595_DATA1_PORT		GPIOC
#define HC595_DATA1_PIN			GPIO_Pin_7
#define HC595_OE1_PORT			GPIOC
#define HC595_OE1_PIN			GPIO_Pin_8
#define HC595_SHCP1_PORT		GPIOC
#define HC595_SHCP1_PIN			GPIO_Pin_9
#define HC595_STCP1_PORT		GPIOA
#define HC595_STCP1_PIN			GPIO_Pin_11

/* 第二组EXTIO_B的驱动管脚宏定义 */
#define HC595_DATA2_PORT		GPIOA
#define HC595_DATA2_PIN			GPIO_Pin_12
#define HC595_OE2_PORT			GPIOC
#define HC595_OE2_PIN			GPIO_Pin_10
#define HC595_SHCP2_PORT		GPIOC
#define HC595_SHCP2_PIN			GPIO_Pin_11
#define HC595_STCP2_PORT		GPIOA
#define HC595_STCP2_PIN			GPIO_Pin_12

/* 第三组EXTIO_C的驱动管脚宏定义 */
#define HC595_DATA3_PORT		GPIOD
#define HC595_DATA3_PIN			GPIO_Pin_2
#define HC595_OE3_PORT			GPIOD
#define HC595_OE3_PIN			GPIO_Pin_3
#define HC595_SHCP3_PORT		GPIOD
#define HC595_SHCP3_PIN			GPIO_Pin_7
#define HC595_STCP3_PORT		GPIOB
#define HC595_STCP3_PIN			GPIO_Pin_5

/* 第四组EXTIO_D的驱动管脚宏定义 */
#define HC595_DATA4_PORT		GPIOB
#define HC595_DATA4_PIN			GPIO_Pin_8
#define HC595_OE4_PORT			GPIOB
#define HC595_OE4_PIN			GPIO_Pin_9
#define HC595_SHCP4_PORT		GPIOE
#define HC595_SHCP4_PIN			GPIO_Pin_0
#define HC595_STCP4_PORT		GPIOE
#define HC595_STCP4_PIN			GPIO_Pin_1

/* 定义EXT_A的宏 */
void HC595_Data1_1(void)		{HC595_DATA1_PORT->BSRR = HC595_DATA1_PIN;}
void HC595_Data1_0(void)		{HC595_DATA1_PORT->BRR = HC595_DATA1_PIN;}

void HC595_OE1_1(void)			{HC595_OE1_PORT->BSRR = HC595_OE1_PIN;}
void HC595_OE1_0(void)			{HC595_OE1_PORT->BRR = HC595_OE1_PIN;}

void HC595_Shcp1_1(void)		{HC595_SHCP1_PORT->BSRR = HC595_SHCP1_PIN;}
void HC595_Shcp1_0(void)		{HC595_SHCP1_PORT->BRR = HC595_SHCP1_PIN;}

void HC595_Stcp1_1(void)		{HC595_STCP1_PORT->BSRR = HC595_STCP1_PIN;}
void HC595_Stcp1_0(void)		{HC595_STCP1_PORT->BRR = HC595_STCP1_PIN;}

/* 定义EXT_B的宏 */
void HC595_Data2_1(void)		{HC595_DATA2_PORT->BSRR = HC595_DATA2_PIN;}
void HC595_Data2_0(void)		{HC595_DATA2_PORT->BRR = HC595_DATA2_PIN;}

void HC595_OE2_1(void)			{HC595_OE2_PORT->BSRR = HC595_OE2_PIN;}
void HC595_OE2_0(void)			{HC595_OE2_PORT->BRR = HC595_OE2_PIN;}

void HC595_Shcp2_1(void)		{HC595_SHCP2_PORT->BSRR = HC595_SHCP2_PIN;}
void HC595_Shcp2_0(void)		{HC595_SHCP2_PORT->BRR = HC595_SHCP2_PIN;}

void HC595_Stcp2_1(void)		{HC595_STCP2_PORT->BSRR = HC595_STCP2_PIN;}
void HC595_Stcp2_0(void)		{HC595_STCP2_PORT->BRR = HC595_STCP2_PIN;}

/* 定义EXT_C的宏 */
void HC595_Data3_1(void)		{HC595_DATA3_PORT->BSRR = HC595_DATA3_PIN;}
void HC595_Data3_0(void)		{HC595_DATA3_PORT->BRR = HC595_DATA3_PIN;}

void HC595_OE3_1(void)			{HC595_OE3_PORT->BSRR = HC595_OE3_PIN;}
void HC595_OE3_0(void)			{HC595_OE3_PORT->BRR = HC595_OE3_PIN;}

void HC595_Shcp3_1(void)		{HC595_SHCP3_PORT->BSRR = HC595_SHCP3_PIN;}
void HC595_Shcp3_0(void)		{HC595_SHCP3_PORT->BRR = HC595_SHCP3_PIN;}

void HC595_Stcp3_1(void)		{HC595_STCP3_PORT->BSRR = HC595_STCP3_PIN;}
void HC595_Stcp3_0(void)		{HC595_STCP3_PORT->BRR = HC595_STCP3_PIN;}

/* 定义EXT_D的宏 */
void HC595_Data4_1(void)		{HC595_DATA4_PORT->BSRR = HC595_DATA4_PIN;}
void HC595_Data4_0(void)		{HC595_DATA4_PORT->BRR = HC595_DATA4_PIN;}

void HC595_OE4_1(void)			{HC595_OE4_PORT->BSRR = HC595_OE4_PIN;}
void HC595_OE4_0(void)			{HC595_OE4_PORT->BRR = HC595_OE4_PIN;}

void HC595_Shcp4_1(void)		{HC595_SHCP4_PORT->BSRR = HC595_SHCP4_PIN;}
void HC595_Shcp4_0(void)		{HC595_SHCP4_PORT->BRR = HC595_SHCP4_PIN;}

void HC595_Stcp4_1(void)		{HC595_STCP4_PORT->BSRR = HC595_STCP4_PIN;}
void HC595_Stcp4_0(void)		{HC595_STCP4_PORT->BRR = HC595_STCP4_PIN;}

#define EXTIO_PORT_NUM		6		/* 每组扩展IO端口所用的74HC595数量 */

static ExtIO_T  s_tExtIO[EXTIO_PORT_NUM];		/* 定义扩展IO的结构体 */

static void bsp_InitHC595Gpio(void);
static void bsp_InitHC595Var(void);

/*
*********************************************************************************************************
*	函 数 名: bsp_InitHC595
*	功能说明: 初始化HC595
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitHC595(void)
{
	bsp_InitHC595Var();			/* 结构体变量赋初值 */
	bsp_InitHC595Gpio();		/* 初始化所用GPIO */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitHC595Var
*	功能说明: 初始化HC595对应的结构体变量
*	形    参：无
*	返 回 值: 无
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
*	函 数 名: bsp_InitHC595Gpio
*	功能说明: 初始化HC595的GPIO
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitHC595Gpio(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_HC595_PORT,ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	/* 初始化EXT_A驱动管脚 */
	GPIO_InitStruct.GPIO_Pin = HC595_DATA1_PIN;
	GPIO_Init(HC595_DATA1_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = HC595_SHCP1_PIN;
	GPIO_Init(HC595_SHCP1_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = HC595_STCP1_PIN;
	GPIO_Init(HC595_STCP1_PORT,&GPIO_InitStruct);

	/* 初始化EXT_B驱动管脚 */
	GPIO_InitStruct.GPIO_Pin = HC595_DATA2_PIN;
	GPIO_Init(HC595_DATA2_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = HC595_SHCP2_PIN;
	GPIO_Init(HC595_SHCP2_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = HC595_STCP2_PIN;
	GPIO_Init(HC595_STCP2_PORT,&GPIO_InitStruct);

	/* 初始化EXT_C驱动管脚 */
	GPIO_InitStruct.GPIO_Pin = HC595_DATA3_PIN;
	GPIO_Init(HC595_DATA3_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = HC595_SHCP3_PIN;
	GPIO_Init(HC595_SHCP3_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = HC595_STCP3_PIN;
	GPIO_Init(HC595_STCP3_PORT,&GPIO_InitStruct);

	/* 初始化EXT_D驱动管脚 */
	GPIO_InitStruct.GPIO_Pin = HC595_DATA4_PIN;
	GPIO_Init(HC595_DATA4_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = HC595_SHCP4_PIN;
	GPIO_Init(HC595_SHCP4_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = HC595_STCP4_PIN;
	GPIO_Init(HC595_STCP4_PORT,&GPIO_InitStruct);
	
	/* OE外部接上拉，配置成开漏 */
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

	/* 初始化时将使能引脚拉高，使数据锁存在寄存器内 */
	HC595_OE1_1();
	HC595_OE2_1();
	HC595_OE3_1();
	HC595_OE4_1();
}

/*
*********************************************************************************************************
*	函 数 名: HC595_Delay
*	功能说明: HC595所用延时
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void HC595_Delay(void)
{
	uint8_t i;

	for(i = 1; i>0; i--);
}

/*
*********************************************************************************************************
*	函 数 名: HC595_SendByte
*	功能说明: 发送一个字节
*	形    参：_pExt：扩展IO口的结构体指针
*			  _ucByte：准备发送的数据
*	返 回 值: 无
*********************************************************************************************************
*/
static void HC595_SendByte(ExtIO_T *_pExt, uint8_t _ucByte)
{
	uint8_t i;
	
	/* 传输一字节，每次传输一个bit */
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
		/* SHCP产生一个上升沿，将当前bit传入移位寄存器中 */
		_pExt->SHCP_0();
		HC595_Delay();
		_pExt->SHCP_1();
		HC595_Delay();
		_pExt->SHCP_0();

		_ucByte <<= 1;		/* 左移一位 */
	}
}

/*
*********************************************************************************************************
*	函 数 名: HC595_StcpEN
*	功能说明: 使能存储寄存器，将数据从移位寄存器装入存储寄存器
*	形    参：_pExt：扩展IO口的结构体指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void HC595_StcpEN(ExtIO_T *_pExt)
{
	/* STCP上升沿，数据传入存储寄存器 */
	_pExt->STCP_0();
	HC595_Delay();
	_pExt->STCP_1();
	HC595_Delay();
	_pExt->STCP_0();	
}

/*
*********************************************************************************************************
*	函 数 名: HC595_OutEN
*	功能说明: HC595输出使能
*	形    参：_pExt：扩展IO口的结构体指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void HC595_OutEN(ExtIO_T *_pExt)
{
	_pExt->OE_0();		/* OE引脚拉低，数据从存储寄存器更新到管脚 */
}

/*
*********************************************************************************************************
*	函 数 名: HC595_OutDis
*	功能说明: HC595输出失能
*	形    参：_pExt：扩展IO口的结构体指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void HC595_OutDis(ExtIO_T *_pExt)
{
	_pExt->OE_1();		/* OE引脚拉高，防止重写寄存器时，输出状态发生变化 */
}

/*
*********************************************************************************************************
*	函 数 名: HC595_SendLen
*	功能说明: 发送任意长度字节
*	形    参：_port：扩展IO的端口号
*			  _pbuf：发送缓冲区指针
*			  _uclen：发送数据长度，单位字节
*	返 回 值: 无
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
		HC595连续发生数据时，最先发送的一字节会被挤入最后一片595中，
		因此需要倒序发送，先发送最后一字节
	*/
	HC595_OutDis(p);		/* 先失能输出引脚，修改完输出状态后再重新使能 */	

	_pbuf += (_uclen - 1);
	for(i = 0 ;i < _uclen ;i++)
	{		
		HC595_SendByte(p ,*_pbuf);
		_pbuf--;
	}
	HC595_StcpEN(p);		/* STCP上升沿，数据从移位寄存器填入存储寄存器 */
	HC595_OutEN(p);			/* OE使能，数据输出 */
	
}

/*
*********************************************************************************************************
*	函 数 名: HC595_WriteBit
*	功能说明: 将对应的扩展IO输出1或0
*	形    参：_port：扩展IO端口号
*			  _pin：所操作的IO,	1-48
*			  _value：置位或复位，只能输入0或1
*	返 回 值: 无
*********************************************************************************************************
*/
void HC595_WriteBit(ExtIO_Port_E _port, uint8_t _pin, uint8_t _value)
{
	uint8_t buf[EXTIO_PORT_NUM] = {0};		//存放要发送给HC595的数据
	uint8_t uctemp = 0;
	uint8_t Num,NumOfSingle;
	
	/* 写入不是0或1的话，返回 */
	if(_value > 1)
	{
		printf("参数填写错误\r\n");
		return;	
	}
	
	if(_pin == 0 && _pin > 48)
	{
		printf("参数填写错误\r\n");
		return;	
	}

	Num = (_pin - 1) /8;				/* 计算需要操作第几个HC595 */
	NumOfSingle = (_pin-1) %8;			/* 计算单字节置位或复位的偏移量 */
	
	/* 调试发现，HC595写入一字节时，先写入最低位，最低位对应着Q0 */
	uctemp = (_value << NumOfSingle);
	buf[Num] = uctemp;

	HC595_SendLen(_port,buf,EXTIO_PORT_NUM);	
	
//	for(uint8_t i=0 ; i<EXTIO_PORT_NUM ;i++)
//	{
//		printf("\r\n buf[%d] = %02x",i,buf[i]);	
//	}	
//	printf("\r\n 将pin%d 置为 %d",_pin ,_value);
}

/*
*********************************************************************************************************
*	函 数 名: HC595_Reset
*	功能说明: 复位所有扩展IO口
*	形    参：无
*	返 回 值: 无
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
