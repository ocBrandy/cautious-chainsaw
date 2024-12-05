/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart4;

extern UART_HandleTypeDef huart5;

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
#define DATA_WR 01
#define DATA_RD 02

#define SERVICETABLE 01
#define CONTRALCMD 02
#define ACQMODE 03
#define DEACTIVE 04

#define COM1	0xdd
#define COM4	0xcc
#define COM5	0xee

#define DATABACK 00
#define TIMEOUT 01

#define ETHCMD		0x01
#define TOOLDATA	0x02




/* USER CODE END Private defines */

void usart_init (void);
void MX_UART4_Init(void);
void MX_UART5_Init(void);
void MX_USART1_UART_Init(void);
void Downcmdsend (uint8_t type);
void Downdatasend (unsigned char type);
void ComSendBuf(uint8_t com,uint8_t *p,uint16_t len);
void ComSendChar(unsigned char com,unsigned char p);
void Resetstate (unsigned char tep);
/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

