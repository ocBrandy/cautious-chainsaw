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
uint16_t Checksum_Reverse(uint8_t * p,uint16_t _len);
uint16_t Checksum_sum(uint8_t * p,uint16_t _len);

void IWDG_FEED_DOG(void);
void MX_IWDG_Init(void);

#endif /* INC_USER_INIT_H_ */
