/*
*********************************************************************************************************
*
*	ģ������ : �Ϲ����ݴ洢
*	�ļ����� : DG_DataStorage.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
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
	INT16_U Mode;         /* ͨѶģʽ */
    INT32_U DEP;          /* У����� */
	INT32_U UNDEP;        /* δУ����� */     
	INT16_U SPD;	      /* �ٶ� */
	INT16_U TEN;	      /* ���� */
	INT16_U CHT;            
	INT16_U RES;          /* Ԥ�� */
	INT16_U CHV;	
	INT16_U UPRATE;       /*�����ٶ� */
	INT16_U DOWNRATE;     /* �����ٶ� */
	INT16_U UPLOSE;       /* ���ж����� */
	INT16_U DOWNLOSE;     /* ���ж����� */
	INT16_U UPUTI;        /* ���������� */
	INT16_U MODEM;	      /* Modem״̬ */
	
}DISBUF_T; //DISBUF_T��һ���ṹ�����͵����֣����Ǳ���

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

/* ���ⲿ���õĺ������� */
uint8_t SaveTempAndStress(void);
uint8_t SaveConfigMsg(void);
void DataPoll(void);
uint8_t	SendConfigMsg(void);

#endif  /* _DG_DATA_STORAGE_H */


