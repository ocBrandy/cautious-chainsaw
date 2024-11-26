/*
*********************************************************************************************************
*
*	模块名称 : LCD模块测试demo头文件。
*	文件名称 : demo_lcd.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2019-2020, yy-AA55
*
*********************************************************************************************************
*/
#ifndef _DEMO_LCD_H
#define _DEMO_LCD_H

#include "bsp.h"

extern const u16 POINT_COLOR_TBL[CT_MAX_TOUCH];  
/* 供外部调用的函数声明 */
void Load_Drow_Dialog(void);
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color);
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color);
u16 my_abs(u16 x1,u16 x2);
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color);
void rtp_test(void);
void ctp_test(void);
void DemoLcdTouch(void);

#endif


