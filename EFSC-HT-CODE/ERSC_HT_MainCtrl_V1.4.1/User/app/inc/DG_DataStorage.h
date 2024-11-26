/*
*********************************************************************************************************
*
*	模块名称 : 断轨数据存储
*	文件名称 : DG_DataStorage.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2020-2022, XMDNDL
*
*********************************************************************************************************
*/

#include "bsp.h"

#ifndef _DG_DATA_STORAGE_H
#define _DG_DATA_STORAGE_H
#define FS_VOLUME_SD		"0:"
typedef struct
{
    uint16_t Year;
	uint8_t Mon;
	uint8_t Day;
}SYS_CLOCK_T;
typedef union  
{
	int32_t a;
	uint8_t b[4]; 
}INT32_U;
typedef union
{
	
	int16_t c;
	uint16_t d;
	uint8_t e[2]; 
}INT16_U;
typedef struct
{
	INT16_U Mode;         /* 通讯模式 */
    INT32_U DEP;          /* 校正深度 */
	INT32_U UNDEP;        /* 未校正深度 */     
	INT16_U SPD;	      /* 速度 */
	INT16_U TEN;	      /* 张力 */
	INT16_U CHT;            
	INT16_U RES;          /* 预留 */
	INT16_U CHV;	
	INT16_U UPRATE;       /*上行速度 */
	INT16_U DOWNRATE;     /* 下行速度 */
	INT16_U UPLOSE;       /* 上行丢包数 */
	INT16_U DOWNLOSE;     /* 下行丢包数 */
	INT16_U UPUTI;        /* 上行利用率 */
	INT16_U MODEM;	      /* Modem状态 */
	
}DISBUF_T; //DISBUF_T是一个结构体类型的名字，不是变量

typedef enum
{
	DEP = 0,
	SPD,
	TEN,
	CHT,
	CHV,
	UPRATE,
	DOWNRATE,
	MODEM,
	ROW,
	
	
}DIS_TYPE_E;

/*************************/

/* 供外部调用的函数声明 */
uint8_t SaveTempAndStress(void);
uint8_t SaveConfigMsg(void);
void DataPoll(void);
uint8_t	SendConfigMsg(void);

#endif  /* _DG_DATA_STORAGE_H */


