/*
*********************************************************************************************************
*
*	模块名称 : io扩展模块
*	文件名称 : bsp_ext_io.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*********************************************************************************************************
*/

#ifndef __BSP_EXT_IO_H
#define __BSP_EXT_IO_H

/* 扩展端口枚举 */
typedef enum
{
	ExtIO_A = 0,
	ExtIO_B,
	ExtIO_C,
	ExtIO_D,
}ExtIO_Port_E;

/* 定义扩展IO的结构体� */
typedef struct
{
	ExtIO_Port_E  _port;			/* 标记当前结构体为哪组EXT端口 */
	void (*Data_0)(void);			/* 74HC595的Data脚置0函数指针 */
	void (*Data_1)(void);			/* 74HC595的Data脚置1函数指针 */
	void (*OE_0)(void);				/* 74HC595的OE脚置0函数指针 */
	void (*OE_1)(void);				/* 74HC595的OE脚置1函数指针 */
	void (*STCP_0)(void);			/* 74HC595的STCP脚置0函数指针 */
	void (*STCP_1)(void);			/* 74HC595的STCP脚置1函数指针 */
	void (*SHCP_0)(void);			/* 74HC595的SHCP脚置0函数指针 */
	void (*SHCP_1)(void);			/* 74HC595的SHCP脚置1函数指针 */
	
}ExtIO_T;

void bsp_InitHC595(void);		/* 初始化扩展IO模块 */
void HC595_WriteBit(ExtIO_Port_E _port, uint8_t _pin, uint8_t _value);		/* 扩展IO模块的位操作API */
void HC595_Reset(void);		/* 复位所有扩展IO口为低电平 */

#endif

/***************************** yy-AA55 (END OF FILE) *********************************/
