/*
 * init.h
 *
 *  Created on: Nov 14, 2024
 *      Author: 16194
 */

#ifndef INC_USER_INIT_H_
#define INC_USER_INIT_H_

#include "stm32f1xx_hal.h"

extern IWDG_HandleTypeDef iwdg;

void init (void);
void SystemClock_Config(void);
void Error_Handler(void);
unsigned short Checksum_Reverse(unsigned char * p,unsigned short _len);
unsigned short Checksum_sum(unsigned char * p,unsigned short _len);

void IWDG_FEED_DOG(void);
void MX_IWDG_Init(void);
int PRINT_LOG(const char *fmt,...);

#endif /* INC_USER_INIT_H_ */
