/*
*********************************************************************************************************
*
*	模块名称 : 串口命令解析
*	文件名称 : USB_DMM.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*********************************************************************************************************
*/
#ifndef _USB_DMM_H
#define _USB_DMM_H


typedef struct
{
	uint8_t  RedPort;			/* 红色端接线，左或是右 */		
	uint8_t  BlackPort;			/* 黑色端接线，左或是右 */
	uint8_t  RedPin;			/* 红色端接的具体引脚编号 */
	uint8_t  BlackPin;			/* 黑色端接的具体引脚编号 */
}MEAS_MSG_T;

void AutoMeas(void);
void OnceMeas(void);

#endif
