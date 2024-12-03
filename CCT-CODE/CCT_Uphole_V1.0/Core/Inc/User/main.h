/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "init.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "Passthrough.h"
#include "fpga.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

//ADDR
#define ADDR0_Pin GPIO_PIN_2
#define ADDR0_GPIO_Port GPIOE

#define ADDR1_Pin GPIO_PIN_3
#define ADDR1_GPIO_Port GPIOE

#define ADDR2_Pin GPIO_PIN_4
#define ADDR2_GPIO_Port GPIOE

#define ADDR3_Pin GPIO_PIN_5
#define ADDR3_GPIO_Port GPIOE



//DATA
#define DATA0_Pin GPIO_PIN_12
#define DATA0_GPIO_Port GPIOA

#define DATA1_Pin GPIO_PIN_2
#define DATA1_GPIO_Port GPIOA

#define DATA2_Pin GPIO_PIN_3
#define DATA2_GPIO_Port GPIOA

#define DATA3_Pin GPIO_PIN_3
#define DATA3_GPIO_Port GPIOD

#define DATA4_Pin GPIO_PIN_6
#define DATA4_GPIO_Port GPIOD

#define DATA5_Pin GPIO_PIN_5
#define DATA5_GPIO_Port GPIOB

#define DATA6_Pin GPIO_PIN_8
#define DATA6_GPIO_Port GPIOB

#define DATA7_Pin GPIO_PIN_9
#define DATA7_GPIO_Port GPIOB


//WR&CS
#define MCU_WR_Pin GPIO_PIN_11
#define MCU_WR_GPIO_Port GPIOA

#define MCU_CS_Pin GPIO_PIN_0
#define MCU_CS_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
