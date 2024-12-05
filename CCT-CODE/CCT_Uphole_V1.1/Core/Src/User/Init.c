/*
 * init.c
 *
 *  Created on: Nov 14, 2024
 *      Author: 16194
 */
#include "main.h"

void init (void)
{
	SystemClock_Config();			//时钟配置

	HAL_Init();						//HAL库初始化
	MX_GPIO_Init();					//GPIO初始化
	TIM_init();						//定时器初始化
	usart_init();					//串口初始化
	FPGA_init();					//FPGA初始化
	MX_IWDG_Init();					//独立看门狗初始化

	SEGGER_RTT_Init();				//SEGGERRTT初始化

	PRINT_LOG("initover\n\r");
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

IWDG_HandleTypeDef iwdg;
void MX_IWDG_Init(void)
{
	iwdg.Instance = IWDG;
	iwdg.Init.Prescaler = IWDG_PRESCALER_4;
	iwdg.Init.Reload = 5000;
	if(HAL_IWDG_Init(&iwdg) != HAL_OK)
	{
		Error_Handler();
	}
}

void IWDG_FEED_DOG(void)
{
	HAL_IWDG_Refresh(&iwdg);	//410ms内清掉看门狗
}

/****************************************************************/
/*-----------------ETH下发数据校验和计算函数------------------------*/
unsigned short Checksum_Reverse(unsigned char * p,unsigned short _len)
{
	uint16_t sum = 0;
	uint16_t i,y;
	uint16_t Buf[100] = {0};
	for(i=0;i<(_len/2);i++)
	{
		Buf[i] = (p[2*i+1] << 8) + p[2*i];
	}
	for(y=0;y<(_len/2);y++)
	{
		sum += Buf[y];
	}
	return sum;
}

/*---------------------------校验和计算函数------------------------*/
unsigned short Checksum_sum(unsigned char * p,unsigned short _len)
{
	uint16_t sum = 0;
	for(unsigned char i=0;i<_len;i++)
	{
		sum += p[i];
	}
	return sum;
}

int PRINT_LOG(const char *fmt,...)
{	int n;
	char aBuffer[256];
	va_list args;
	va_start(args,fmt);
	n = vsnprintf(aBuffer, sizeof(aBuffer), fmt, args);
	if (n > (int)sizeof(aBuffer))
	{
		SEGGER_RTT_Write(0, aBuffer, sizeof(aBuffer));
	}
	else if(n > 0)
	{
		SEGGER_RTT_Write(0, aBuffer, n);
	}
	va_end(args);
	return n;
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
