/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
uint8_t Uart1_RxBuff[1];
uint8_t Uart5_RxBuff[1];
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart4_tx;


void usart_init (void)
{
	MX_DMA_Init();
	MX_UART4_Init();				//串口4初始化:串口4为时间深度同步信息上传串口
	MX_UART5_Init();				//串口5初始化:串口5为MCU与FPGA通讯串口
	MX_USART1_UART_Init();			//串口1初始化:串口1为主控板与ETH通讯串口
}
/* UART4 init function */
void MX_UART4_Init(void)
{
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;						//波特率115200
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  //HAL_DMA_IRQHandler(&hdma_usart4_tx);
}
void MX_UART5_Init(void)
{
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_UART_Receive_IT(&huart5,(uint8_t*)Uart5_RxBuff, 1);
}

void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_UART_Receive_IT(&huart1,(uint8_t*)Uart1_RxBuff, 1);

  HAL_DMA_IRQHandler(&hdma_usart1_tx);
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspInit 0 */

  /* USER CODE END UART4_MspInit 0 */
    /* UART4 clock enable */
    __HAL_RCC_UART4_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**UART4 GPIO Configuration
    PC10     ------> UART4_TX
    PC11     ------> UART4_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*usart4 dma tx init*/
    /*hdma_usart4_tx.Instance = DMA2_Channel5;
    hdma_usart4_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart4_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart4_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart4_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart4_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart4_tx.Init.Mode = DMA_NORMAL;
    hdma_usart4_tx.Init.Priority = DMA_PRIORITY_MEDIUM;

    if (HAL_DMA_Init(&hdma_usart4_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart4_tx);*/

    //HAL_NVIC_SetPriority(UART4_IRQn, 0, 0);
    //HAL_NVIC_EnableIRQ(UART4_IRQn);

    HAL_NVIC_SetPriority(UART4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspInit 1 */

  /* USER CODE END UART4_MspInit 1 */
  }
  else if(uartHandle->Instance==UART5)
  {
  /* USER CODE BEGIN UART5_MspInit 0 */

  /* USER CODE END UART5_MspInit 0 */
    /* UART5 clock enable */
    __HAL_RCC_UART5_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**UART5 GPIO Configuration
    PC12     ------> UART5_TX
    PD2     ------> UART5_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* UART5 interrupt Init */
    HAL_NVIC_SetPriority(UART5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(UART5_IRQn);
  /* USER CODE BEGIN UART5_MspInit 1 */

  /* USER CODE END UART5_MspInit 1 */
  }
  else if(uartHandle->Instance==USART1)
  {
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*usart1 dma tx init*/
    hdma_usart1_tx.Instance = DMA1_Channel4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_MEDIUM;

    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspDeInit 0 */

  /* USER CODE END UART4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART4_CLK_DISABLE();

    /**UART4 GPIO Configuration
    PC10     ------> UART4_TX
    PC11     ------> UART4_RX
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10|GPIO_PIN_11);

  /* USER CODE BEGIN UART4_MspDeInit 1 */

    HAL_DMA_DeInit(uartHandle->hdmatx);
   // HAL_NVIC_DisableIRQ(UART4_IRQn);
  /* USER CODE END UART4_MspDeInit 1 */
  }
  else if(uartHandle->Instance==UART5)
  {
  /* USER CODE BEGIN UART5_MspDeInit 0 */

  /* USER CODE END UART5_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART5_CLK_DISABLE();

    /**UART5 GPIO Configuration
    PC12     ------> UART5_TX
    PD2     ------> UART5_RX
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_12);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);

    HAL_DMA_DeInit(uartHandle->hdmatx);
    /* UART5 interrupt Deinit */
    HAL_NVIC_DisableIRQ(UART5_IRQn);
  /* USER CODE BEGIN UART5_MspDeInit 1 */

  /* USER CODE END UART5_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    HAL_DMA_DeInit(uartHandle->hdmatx);
    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

void ComSendBuf(uint8_t com,uint8_t *p,uint16_t len)
{
	if(com == 0xdd)
	{
		HAL_UART_Transmit_DMA(&huart1, p, len);
	}
	if(com == 0xee)
	{
		HAL_UART_Transmit(&huart5, p, len, 0xff);
	}
	if(com == 0xcc)
	{
		HAL_UART_Transmit(&huart4, p, len, 0xff);
	}
}

void ComSendChar(unsigned char com,unsigned char p)
{
	unsigned char sendbuf[1];
	sendbuf[0] = p;
	if(com == 0xdd)
	{
		HAL_UART_Transmit_DMA(&huart1, sendbuf, 1);
	}
	if(com == 0xee)
	{
		HAL_UART_Transmit(&huart5, sendbuf, 1, 0xff);
	}
	if(com == 0xcc)
	{
		HAL_UART_Transmit(&huart4, sendbuf, 1, 0xff);
	}
}
extern unsigned short Downdatatimeoutnum;


unsigned char ETHdatastate = 0;				//ETH数据包接收状态
unsigned char DOWNdatastate = 0;			//井下数据包接收状态
unsigned char ETHdataheadnum = 0;			//ETH数据包包头接收计数
unsigned char DOWNheaddatanum = 0;			//井下数据包包头计数
unsigned char ETHdatabodynum = 0;			//ETH数据包包体计数
unsigned char DOWNdatabodynum = 0;			//井下数据包包体计数

unsigned char OveralltypeH,OveralltypeL;	//数据总类型
unsigned char Totaltype;					//下发数据类型
unsigned char ETHdatabuf[256];				//ETH数据包缓存
unsigned char DOWNdatabuf[256];				//井下数据数据包缓存
unsigned char ETHbodybuf[256];				//ETH数据包包体缓存
unsigned char DOWNbodybuf[256];				//井下数据包包体缓存
unsigned short ETHdatalen;					//ETH数据包体长度
unsigned short DOWNdatalen;					//井下数据包体长度
unsigned char Workmode;						//工作模式
unsigned char Tooladdress;					//ETH数据包头中的仪器地址
unsigned short Cmdword;						//命令字
unsigned char Timestampnum;
unsigned long ETH_Timestamp;				//ETH下发的时间标
unsigned short Overtime;						//仪器命令超时时间，与Delaytime不同
unsigned short Reserve;						//ETH数据包包头预留位
unsigned short ETH_Checksum;					//ETH下发的校验和
unsigned short DOWN_Checksum;					//井下上传的校验和
unsigned char ETHdataoverflag;				//ETH数据接收完毕标志
unsigned char DOWNdataoverflag;				//井下数据接收完毕标志

_Bool Downtoolctrlcmdback;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)		//串口中断回调函数
{
	if(huart->Instance == USART1) 		//如果产生中断的是串口1
	{
		if(Uart1_RxBuff[0] == 0xFF && ETHdatastate<3) {ETHdatastate = 0;}
		switch(ETHdatastate)
		{
			case 0:
				if(Uart1_RxBuff[0] == 0xFF)					//同步头FF 00 55 AA
				{
					ETHdatabuf[ETHdataheadnum++] = Uart1_RxBuff[0];
					ETHdatastate++ ;
				}
				else
				{
					Resetstate(ETHCMD);
				}
				break;
			case 1:
				if(Uart1_RxBuff[0] == 0x00)
				{
					ETHdatabuf[ETHdataheadnum++] = Uart1_RxBuff[0];
					ETHdatastate++ ;
				}
				else
				{
					Resetstate(ETHCMD);
				}
				break;
			case 2:
				if(Uart1_RxBuff[0] == 0x55)
				{
					ETHdatabuf[ETHdataheadnum++] = Uart1_RxBuff[0];
					ETHdatastate++ ;
				}
				else
				{
					Resetstate(ETHCMD);
				}
				break;
			case 3:
				if(Uart1_RxBuff[0] == 0xAA)
				{
					ETHdatabuf[ETHdataheadnum++] = Uart1_RxBuff[0];
					ETHdatastate++ ;
				}
				else
				{
					Resetstate(ETHCMD);
				}
				break;
			case 4:
				ETHdatabuf[ETHdataheadnum++] = Uart1_RxBuff[0];
				ETHdatalen = Uart1_RxBuff[0];						//上位机数据长度高字节
				ETHdatastate++ ;
				break;
			case 5:
				ETHdatabuf[ETHdataheadnum++] = Uart1_RxBuff[0];
				ETHdatalen = (ETHdatalen<<8) + Uart1_RxBuff[0];				//上位机数据长度低字节
				ETHdatastate++ ;
				break;
			case 6:
				ETHdatabuf[ETHdataheadnum++] = Uart1_RxBuff[0];
				OveralltypeH = Uart1_RxBuff[0]>>4;						//该字节为数据读写总类型标示字节，解析读写数据类型，高位代表读或写，低位代表数据类型
				OveralltypeL = Uart1_RxBuff[0] & 0x0F;
				if(OveralltypeH == 0x01)
				{
					Totaltype = DATA_WR;							//向井下写数据
					if(OveralltypeL == 0x00)
					{
						Workmode = SERVICETABLE;					//服务表激活
						ETHdatastate++ ;
					}
					else if(OveralltypeL == 0x01)
					{
						Workmode = CONTRALCMD;						//下发控制命令
						ETHdatastate++ ;
					}
					else if(OveralltypeL == 0x03)
					{
						//Workmode = ACQMODE;							//采集命令，带一个参数
						ETHdatastate++ ;
					}
					else if(OveralltypeL == 0x04)
					{
						Workmode = DEACTIVE;						//服务表反激活
						ETHdatastate++ ;
					}
					else
					{
						Resetstate(ETHCMD);
					}
				}
				else if(OveralltypeH == 0x02)
				{
					Totaltype = DATA_RD;							//从井下读数据
					ETHdatastate++ ;
				}
				else
				{
					Resetstate(ETHCMD);
				}
				break;
			case 7:
				ETHdatabuf[ETHdataheadnum++] = Uart1_RxBuff[0];		//仪器地址
				Tooladdress = Uart1_RxBuff[0];
				ETHdatastate++ ;
				break;
			case 8:
				ETHdatabuf[ETHdataheadnum++] = Uart1_RxBuff[0];
				Cmdword = Uart1_RxBuff[0];
				ETHdatastate++ ;
				break;
			case 9:
				ETHdatabuf[ETHdataheadnum++] = Uart1_RxBuff[0];		//命令字/subset number
				Cmdword = (Cmdword << 8) + Uart1_RxBuff[0];
				ETHdatastate++ ;
				break;
			case 10:
				if(Timestampnum <= 4)									//四字节时间戳
				{
					Timestampnum++;
					ETHdatabuf[ETHdataheadnum++] = Uart1_RxBuff[0];
					ETH_Timestamp = (ETH_Timestamp << 8) + Uart1_RxBuff[0];
					HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_12);
					if(Timestampnum >=4)
					{
						ETHdatastate++;
						Timestampnum = 0;
					}
				}
				break;
			case 11:
				ETHdatabuf[ETHdataheadnum++] = Uart1_RxBuff[0];
				Overtime = Uart1_RxBuff[0];
				ETHdatastate++;
				break;
			case 12:
				ETHdatabuf[ETHdataheadnum++] = Uart1_RxBuff[0];
				Overtime = (Overtime << 8) + Uart1_RxBuff[0];						//超时时间
				ETHdatastate++;
				break;
			case 13:
				ETHdatabuf[ETHdataheadnum++] = Uart1_RxBuff[0];
				Reserve = Uart1_RxBuff[0];
				ETHdatastate++;
				break;
			case 14:
				ETHdatabuf[ETHdataheadnum++] = Uart1_RxBuff[0];
				Reserve = (Reserve << 8) + Uart1_RxBuff[0];							//预留
				ETHdatastate++;
				break;
			case 15:
				if(Workmode == DEACTIVE)											//服务表反激活没有包体，只有包头
				{
					ETHdatabuf[ETHdataheadnum++] = Uart1_RxBuff[0];
					ETH_Checksum = Uart1_RxBuff[0];
					ETHdatastate = 17;
				}
				ETHdatabuf[ETHdataheadnum++] = Uart1_RxBuff[0];
				ETHbodybuf[ETHdatabodynum++] = Uart1_RxBuff[0];
				if(ETHdatabodynum >= ETHdatalen)
				{
					ETHdatastate++;
				}
				break;
			case 16:
				ETHdatabuf[ETHdataheadnum++] = Uart1_RxBuff[0];
				ETH_Checksum = Uart1_RxBuff[0];
				ETHdatastate++;
				break;
			case 17:
				ETHdatabuf[ETHdataheadnum++] = Uart1_RxBuff[0];
				ETH_Checksum = (ETH_Checksum << 8) + Uart1_RxBuff[0];

				if(ETH_Checksum == Checksum_Reverse(ETHbodybuf,ETHdatalen))
				{
					ETHdataoverflag = 1;
				}
				else
				{
					Resetstate(ETHCMD);
				}
				break;
		}
		HAL_UART_Receive_IT(&huart1,Uart1_RxBuff, 1);
	}
	if(huart->Instance == UART5)				//如果产生中断的是串口5
	{
		//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
		Downdatatimeoutnum = 0;
		switch(DOWNdatastate)
		{
			case 0:
				if(Uart5_RxBuff[0] == 0x66)
				{
					DOWNdatabuf[DOWNheaddatanum++] = Uart5_RxBuff[0];
					DOWNdatastate++;
				}
				else
				{
					Resetstate(TOOLDATA);
				}
				break;
			case 1:
				if(Uart5_RxBuff[0] == 0xCC)
				{
					DOWNdatabuf[DOWNheaddatanum++] = Uart5_RxBuff[0];
					DOWNdatastate++;
				}
				else
				{
					Resetstate(TOOLDATA);
				}
				break;
			case 2:
				DOWNdatabuf[DOWNheaddatanum++] = Uart5_RxBuff[0];
				DOWNdatalen = Uart5_RxBuff[0]<<8;
				DOWNdatastate++;
				break;
			case 3:
				DOWNdatabuf[DOWNheaddatanum++] = Uart5_RxBuff[0];
				DOWNdatalen = DOWNdatalen + Uart5_RxBuff[0];
				DOWNdatastate++;
				break;
			case 4:
				DOWNdatabuf[DOWNheaddatanum++] = Uart5_RxBuff[0];		//数据类型，井下上传只有服务表数据，应为0x01
				if(Uart5_RxBuff[0] == 0x01)
				{
					DOWNdatastate++;
				}
				else if(Uart5_RxBuff[0] == 0x02)
				{
					Downtoolctrlcmdback = true;
					DOWNdatastate++;
				}
				else
					Resetstate(TOOLDATA);
				break;
			case 5:
				DOWNdatabuf[DOWNheaddatanum++] = Uart5_RxBuff[0];
				DOWNbodybuf[DOWNdatabodynum++] = Uart5_RxBuff[0];
				if(DOWNdatabodynum >= DOWNdatalen-1)
				{
					DOWNdatastate++;
				}
				break;
			case 6:
				DOWNdatabuf[DOWNheaddatanum++] = Uart5_RxBuff[0];
				DOWN_Checksum = Uart5_RxBuff[0]<<8;
				DOWNdatastate++;
				break;
			case 7:
				DOWNdatabuf[DOWNheaddatanum++] = Uart5_RxBuff[0];
				DOWN_Checksum = DOWN_Checksum + Uart5_RxBuff[0];
				if(DOWN_Checksum == Checksum_sum(DOWNdatabuf,DOWNheaddatanum-2))
				{
					DOWNdataoverflag = 1;
				}
				else
				{
					Resetstate(TOOLDATA);
				}

				break;
		}
		HAL_UART_Receive_IT(&huart5,Uart5_RxBuff,1);
	}
}
/* USER CODE BEGIN 1 */

void DMA1_Channel4_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_usart1_tx);
}

void DMA2_Channel4_5_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_usart4_tx);
}

unsigned char Downcmdbuf[30];
unsigned char Downdataloadbuf[256] = {0xFF,0x00,0xAA,0x55};

extern unsigned short Sertableactlen;
extern unsigned short Contralcmdlen;

unsigned char Sertableactbuf[50];
unsigned char Contralcmdbuf[50];

void Downcmdsend (uint8_t type)
{
	if(type == SERVICETABLE)
	{
		Downcmdbuf[0] = 0x55;  								//发送包头
		Downcmdbuf[1] = 0xAA;
		Downcmdbuf[2] = (Sertableactlen+1)>>8;						//Sertableactlen+1:上位机数据体长度+Downcmdbuf[3]
		Downcmdbuf[3] = Sertableactlen+1;
		Downcmdbuf[4] = SERVICETABLE;						//数据类型，不属于上位机数据体

		for(unsigned char i=0;i<Sertableactlen;i++)
		{
			Downcmdbuf[i+5] = Sertableactbuf[i];				//上位机数据体
		}
		Downcmdbuf[Sertableactlen+5] = Checksum_sum(Downcmdbuf,Sertableactlen+5)>>8;
		Downcmdbuf[Sertableactlen+6] = Checksum_sum(Downcmdbuf,Sertableactlen+5);
		ComSendBuf(COM5,Downcmdbuf,Sertableactlen+7);			//ETH_Datalen+6:上位机数据体+2byte帧头+2byte长度和数据类型+2byte校验和/0-ETH_Datalen+5
		Workmode = 0;

	}
	else if(type == CONTRALCMD)
	{
		Downcmdbuf[0] = 0x55;  								//发送包头
		Downcmdbuf[1] = 0xAA;
		Downcmdbuf[2] = (Contralcmdlen+3)>>8;
		Downcmdbuf[3] = Contralcmdlen+3;
		Downcmdbuf[4] = CONTRALCMD;							//数据类型，不属于上位机数据体
		Downcmdbuf[5] = Tooladdress;						//命令字，切割仪器为5004
		Downcmdbuf[6] = Cmdword>>8;

		for(uint8_t i=0;i<Contralcmdlen;i++)
		{
			Downcmdbuf[i+7] = Contralcmdbuf[i];				//上位机数据体，切割仪器控制命令为8字节
		}

		Downcmdbuf[Contralcmdlen+7] = Checksum_sum(Downcmdbuf,Contralcmdlen+7)>>8;
		Downcmdbuf[Contralcmdlen+8] = Checksum_sum(Downcmdbuf,Contralcmdlen+7);

		ComSendBuf(COM5,Downcmdbuf,Contralcmdlen+9);			//ETH_Datalen+8:上位机数据体+2byte帧头+2byte长度和数据类型+2byte校验和+2byte命令字/0-ETH_Datalen+7
		Workmode = 0;

	}
/****************************DEACTIVE现由井上执行*******************************/
	/*else if(type == DEACTIVE)
	{
		Downcmdbuf[0] = 0x55;  								//发送包头
		Downcmdbuf[1] = 0xAA;
		Downcmdbuf[2] = ETHdatalen+1;						//ETHdatalen+1:上位机数据体长度+Downcmdbuf[3]
		Downcmdbuf[3] = DEACTIVE;							//数据类型，不属于上位机数据体

		for(unsigned char i=0;i<ETHdatalen;i++)
		{
			Downcmdbuf[i+4] = ETHbodybuf[i];				//上位机数据体
		}

		Downcmdbuf[ETHdatalen+4] = Checksum_sum(Downcmdbuf,ETHdatalen+4)>>8;
		Downcmdbuf[ETHdatalen+5] = Checksum_sum(Downcmdbuf,ETHdatalen+4);

		//ComSendBuf(COM5,Downcmdbuf,ETH_Datalen+6);			//ETH_Datalen+6:上位机数据体+2byte帧头+2byte长度和数据类型+2byte校验和/0-ETH_Datalen+5
		Workmode = 0;
	}*/
	/***************************************************************************/
}


extern unsigned char Subsetnumber;
extern unsigned int Report_Timestamp;
void Downdatasend (unsigned char type)
{
	Downdataloadbuf[4] = DOWNdatabodynum>>8;
	Downdataloadbuf[5] = DOWNdatabodynum;
	Downdataloadbuf[6] = 0x22;									//从井下读数据：0x20000000，仪器采集数据上行：0x02000000
	Downdataloadbuf[7] = Tooladdress;
	Downdataloadbuf[8] = Subsetnumber>>8;
	Downdataloadbuf[9] = Subsetnumber;							//subset号，若为命令返回参数则为命令字
	Downdataloadbuf[10] = Report_Timestamp;
	Downdataloadbuf[11] = Report_Timestamp>>8;
	Downdataloadbuf[12] = Report_Timestamp>>16;
	Downdataloadbuf[13] = Report_Timestamp>>24;					//时间标
	Downdataloadbuf[14] = 0x00;
	Downdataloadbuf[15] = 0x00;									//超时时间,井下仪器控制命令超时时间，暂时没有返回
	Downdataloadbuf[16] = type;									//超时or数据
	Downdataloadbuf[17] = 0x00;									//预留
	for(unsigned char i=0;i<DOWNdatabodynum;i++)
	{
		Downdataloadbuf[18+i] = DOWNbodybuf[i];
	}
	Downdataloadbuf[DOWNdatabodynum+18] = Checksum_sum(Downdataloadbuf,DOWNdatabodynum+18)>>8;
	Downdataloadbuf[DOWNdatabodynum+19] = Checksum_sum(Downdataloadbuf,DOWNdatabodynum+18);
	ComSendBuf(COM1,Downdataloadbuf,DOWNdatabodynum+20);
}
/* USER CODE END 1 */

void Resetstate(unsigned char tep)
{
	if(tep == ETHCMD)
	{
		ETHdatabodynum = 0;
		ETHdatastate = 0;
		ETHdataheadnum = 0;
		ETHdataoverflag = 0;
	}
	else if(tep == TOOLDATA)
	{
		DOWNdataoverflag = 0;
		DOWNdatastate = 0;
		DOWNheaddatanum = 0;
		DOWNdatabodynum = 0;
	}
}
