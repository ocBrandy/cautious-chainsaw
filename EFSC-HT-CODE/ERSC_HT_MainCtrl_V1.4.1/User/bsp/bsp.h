/*
*********************************************************************************************************
*
*	ģ������ : BSPģ��
*	�ļ����� : bsp.h
*	˵    �� : ���ǵײ�����ģ�����е�h�ļ��Ļ����ļ��� Ӧ�ó���ֻ�� #include bsp.h ���ɣ�
*			  ����Ҫ#include ÿ��ģ��� h �ļ�
*
*********************************************************************************************************
*/

#ifndef _BSP_H
#define _BSP_H

/* ���� BSP �汾�� */
#define __STM32F1_BSP_VERSION		"1.1"

/* CPU����ʱִ�еĺ��� */
//#define CPU_IDLE()		bsp_Idle()

/* ����ȫ���жϵĺ� */
#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */

/* ���������ڵ��Խ׶��Ŵ� */
#define BSP_Printf		printf
//#define BSP_Printf(...)

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/*
#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif
*/

/* ����ͷ�ļ��������ͷ�ļ�ֱ���ڴ��������� */
#include "bsp_led.h"
#include "bsp_timer.h"
#include "bsp_uart_fifo.h"
#include "bsp_key.h"
#include "bsp_fpga.h"
//#include "bsp_tim_pwm.h"
#include "bsp_spi_bus.h"
#include "bsp_spi_flash.h"
#include "bsp_i2c_gpio.h"
//#include "bsp_mpu6050.h"
//#include "bsp_ext_io.h"
#include "bsp_sdio_sd.h"
//#include "bsp_RF24L01.h"
#include "bsp_eeprom_24xx.h"
#include "bsp_adc.h"
#include "sys.h"
#include "lcd.h"
#include "touch.h" 
#include "ctiic.h"
#include "ott2001a.h"
#include "gt9147.h"
#include "ft5206.h"

/* USB���ͷ�ļ� */
#include "hw_config.h" 
#include "usb_lib.h"
#include "usb_pwr.h"


/* �ṩ������C�ļ����õĺ��� */
void bsp_InitUSB(void);
void bsp_Init(void);
void bsp_Idle(void);

#endif

/***************************** XMDNDL (END OF FILE) *********************************/
