/*
*********************************************************************************************************
*
*	Ä£¿éÃû³Æ : ioÀ©Õ¹Ä£¿é
*	ÎÄ¼şÃû³Æ : bsp_ext_io.h
*	°æ    ±¾ : V1.0
*	Ëµ    Ã÷ : Í·ÎÄ¼ş
*
*********************************************************************************************************
*/

#ifndef __BSP_EXT_IO_H
#define __BSP_EXT_IO_H

/* À©Õ¹¶Ë¿ÚÃ¶¾Ù */
typedef enum
{
	ExtIO_A = 0,
	ExtIO_B,
	ExtIO_C,
	ExtIO_D,
}ExtIO_Port_E;

/* ¶¨ÒåÀ©Õ¹IOµÄ½á¹¹Ìå£ */
typedef struct
{
	ExtIO_Port_E  _port;			/* ±ê¼Çµ±Ç°½á¹¹ÌåÎªÄÄ×éEXT¶Ë¿Ú */
	void (*Data_0)(void);			/* 74HC595µÄData½ÅÖÃ0º¯ÊıÖ¸Õë */
	void (*Data_1)(void);			/* 74HC595µÄData½ÅÖÃ1º¯ÊıÖ¸Õë */
	void (*OE_0)(void);				/* 74HC595µÄOE½ÅÖÃ0º¯ÊıÖ¸Õë */
	void (*OE_1)(void);				/* 74HC595µÄOE½ÅÖÃ1º¯ÊıÖ¸Õë */
	void (*STCP_0)(void);			/* 74HC595µÄSTCP½ÅÖÃ0º¯ÊıÖ¸Õë */
	void (*STCP_1)(void);			/* 74HC595µÄSTCP½ÅÖÃ1º¯ÊıÖ¸Õë */
	void (*SHCP_0)(void);			/* 74HC595µÄSHCP½ÅÖÃ0º¯ÊıÖ¸Õë */
	void (*SHCP_1)(void);			/* 74HC595µÄSHCP½ÅÖÃ1º¯ÊıÖ¸Õë */
	
}ExtIO_T;

void bsp_InitHC595(void);		/* ³õÊ¼»¯À©Õ¹IOÄ£¿é */
void HC595_WriteBit(ExtIO_Port_E _port, uint8_t _pin, uint8_t _value);		/* À©Õ¹IOÄ£¿éµÄÎ»²Ù×÷API */
void HC595_Reset(void);		/* ¸´Î»ËùÓĞÀ©Õ¹IO¿ÚÎªµÍµçÆ½ */

#endif

/***************************** yy-AA55 (END OF FILE) *********************************/
