/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

int main(void)
{
  init();												//硬件初始化

  while (1)
  {
	  ETH_CMDPOLL();									//ETH指令轮询
	  DOWN_DATAPOLL();									//DOWN数据轮询
	  Sendtimestamp();									//时间深度同步
	  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);

	  IWDG_FEED_DOG();									//喂狗，超过410ms未清看门狗，系统复位
  }
}
