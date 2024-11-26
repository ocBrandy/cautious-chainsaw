/*
*********************************************************************************************************
*
*	ģ������ : io��չģ��
*	�ļ����� : bsp_ext_io.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*********************************************************************************************************
*/

#ifndef __BSP_EXT_IO_H
#define __BSP_EXT_IO_H

/* ��չ�˿�ö�� */
typedef enum
{
	ExtIO_A = 0,
	ExtIO_B,
	ExtIO_C,
	ExtIO_D,
}ExtIO_Port_E;

/* ������չIO�Ľṹ�� */
typedef struct
{
	ExtIO_Port_E  _port;			/* ��ǵ�ǰ�ṹ��Ϊ����EXT�˿� */
	void (*Data_0)(void);			/* 74HC595��Data����0����ָ�� */
	void (*Data_1)(void);			/* 74HC595��Data����1����ָ�� */
	void (*OE_0)(void);				/* 74HC595��OE����0����ָ�� */
	void (*OE_1)(void);				/* 74HC595��OE����1����ָ�� */
	void (*STCP_0)(void);			/* 74HC595��STCP����0����ָ�� */
	void (*STCP_1)(void);			/* 74HC595��STCP����1����ָ�� */
	void (*SHCP_0)(void);			/* 74HC595��SHCP����0����ָ�� */
	void (*SHCP_1)(void);			/* 74HC595��SHCP����1����ָ�� */
	
}ExtIO_T;

void bsp_InitHC595(void);		/* ��ʼ����չIOģ�� */
void HC595_WriteBit(ExtIO_Port_E _port, uint8_t _pin, uint8_t _value);		/* ��չIOģ���λ����API */
void HC595_Reset(void);		/* ��λ������չIO��Ϊ�͵�ƽ */

#endif

/***************************** yy-AA55 (END OF FILE) *********************************/
