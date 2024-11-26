/*
*********************************************************************************************************
*
*	模块名称 : BSP模块
*	文件名称 : bsp.h
*	说    明 : 这是底层驱动模块所有的h文件的汇总文件。 应用程序只需 #include bsp.h 即可，
*			  不需要#include 每个模块的 h 文件
*
*********************************************************************************************************
*/

#ifndef _BSP_H
#define _BSP_H

/* 定义 BSP 版本号 */
#define __STM32F1_BSP_VERSION		"1.1"

/* CPU空闲时执行的函数 */
//#define CPU_IDLE()		bsp_Idle()

/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */

/* 这个宏仅用于调试阶段排错 */
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

/* 声明头文件，所需的头文件直接在此声明即可 */
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

/* USB相关头文件 */
#include "hw_config.h" 
#include "usb_lib.h"
#include "usb_pwr.h"


/* 提供给其他C文件调用的函数 */
void bsp_InitUSB(void);
void bsp_Init(void);
void bsp_Idle(void);

#endif

/***************************** XMDNDL (END OF FILE) *********************************/
