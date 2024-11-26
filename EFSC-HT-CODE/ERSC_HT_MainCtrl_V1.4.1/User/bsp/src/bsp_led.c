/*
*********************************************************************************************************
*
*	模块名称 : LED指示灯驱动模块
*	文件名称 : bsp_led.c
*	版    本 : V1.0
*	说    明 : 驱动LED指示灯
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2013-02-01          正式发布
*
*
*********************************************************************************************************
*/

#include "bsp.h"

/*
	如果用于其它硬件，请修改GPIO定义
	
	如果用户的LED指示灯个数小于4个，可以将多余的LED全部定义为和第1个LED一样，并不影响程序功能
*/


/* 按键口对应的RCC时钟 */
#define RCC_ALL_LED 	(RCC_APB2Periph_GPIOB)

#define GPIO_PORT_LED1  GPIOB
#define GPIO_PIN_LED1	GPIO_Pin_12

#define GPIO_PORT_LED2  GPIOB
#define GPIO_PIN_LED2	GPIO_Pin_13

//#define GPIO_PORT_LED3  GPIOC
//#define GPIO_PIN_LED3	GPIO_Pin_2

//#define GPIO_PORT_LED4  GPIOC
//#define GPIO_PIN_LED4	GPIO_Pin_3

/*
*********************************************************************************************************
*	函 数 名: bsp_InitLed
*	功能说明: 配置LED指示灯相关的GPIO,  该函数被 bsp_Init() 调用。
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitLed(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 打开GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_ALL_LED, ENABLE);

	/*
		配置所有的LED指示灯GPIO为推挽输出模式
		由于将GPIO设置为输出时，GPIO输出寄存器的值缺省是0，因此会驱动LED点亮.
		这是我不希望的，因此在改变GPIO为输出前，先关闭LED指示灯
	*/
		bsp_LedOff(1);
		bsp_LedOff(2);
//	bsp_LedOff(1);
//	bsp_LedOff(2);
//	bsp_LedOff(3);
//	bsp_LedOff(4);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* 推挽输出模式 */
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED1;
	GPIO_Init(GPIO_PORT_LED1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED2;
	GPIO_Init(GPIO_PORT_LED2, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED3;
//	GPIO_Init(GPIO_PORT_LED3, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED4;
//	GPIO_Init(GPIO_PORT_LED4, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_LedOn
*	功能说明: 点亮指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_LedOff(uint8_t _no)
{
	_no--;

	if (_no == 0)
	{
		GPIO_PORT_LED1->BRR = GPIO_PIN_LED1;
	}
	else if (_no == 1)
	{
		GPIO_PORT_LED2->BRR = GPIO_PIN_LED2;
	}
//	else if (_no == 2)
//	{
//		GPIO_PORT_LED3->BRR = GPIO_PIN_LED3;
//	}
//	else if (_no == 3)
//	{
//		GPIO_PORT_LED4->BRR = GPIO_PIN_LED4;
//	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_LedOff
*	功能说明: 熄灭指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_LedOn(uint8_t _no)
{
	_no--;

	if (_no == 0)
	{
		GPIO_PORT_LED1->BSRR = GPIO_PIN_LED1;
	}
	else if (_no == 1)
	{
		GPIO_PORT_LED2->BSRR = GPIO_PIN_LED2;
	}
//	else if (_no == 2)
//	{
//		GPIO_PORT_LED3->BSRR = GPIO_PIN_LED3;
//	}
//	else if (_no == 3)
//	{
//		GPIO_PORT_LED4->BSRR = GPIO_PIN_LED4;
//	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_LedToggle
*	功能说明: 翻转指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 按键代码
*********************************************************************************************************
*/
void bsp_LedToggle(uint8_t _no)
{
	if (_no == 1)
	{
		GPIO_PORT_LED1->ODR ^= GPIO_PIN_LED1;  //异或 相同为0，不同为1
	}
	else if (_no == 2)
	{
		GPIO_PORT_LED2->ODR ^= GPIO_PIN_LED2;
	}
//	else if (_no == 3)
//	{
//		GPIO_PORT_LED3->ODR ^= GPIO_PIN_LED3;
//	}
//	else if (_no == 4)
//	{
//		GPIO_PORT_LED4->ODR ^= GPIO_PIN_LED4;
//	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_IsLedOn
*	功能说明: 判断LED指示灯是否已经点亮。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 1表示已经点亮，0表示未点亮
*********************************************************************************************************
*/
uint8_t bsp_IsLedOn(uint8_t _no)
{
	if (_no == 1)
	{
		if ((GPIO_PORT_LED1->ODR & GPIO_PIN_LED1) == 0)
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 2)
	{
		if ((GPIO_PORT_LED2->ODR & GPIO_PIN_LED2) == 0)
		{
			return 1;
		}
		return 0;
	}
//	else if (_no == 3)
//	{
//		if ((GPIO_PORT_LED3->ODR & GPIO_PIN_LED3) == 0)
//		{
//			return 1;
//		}
//		return 0;
//	}
//	else if (_no == 4)
//	{
//		if ((GPIO_PORT_LED4->ODR & GPIO_PIN_LED4) == 0)
//		{
//			return 1;
//		}
//		return 0;
//	}

	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: RunWaterLed
*	功能说明: 状态1。 LED1 - LED4 依次流水显示。每次点亮1个LED。状态持续5秒后返回。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void RunWaterLed(void)
{
	uint8_t led_no = 1;		/* LED指示灯序号 1-4 */
	
	bsp_StartTimer(0, 10000);		/* 定时器0是5000ms 单次定时器 */
	bsp_StartAutoTimer(1, 200);		/* 定时器1是500ms 自动重装定时器, 控制LED1按1Hz频率翻转闪烁 */
	bsp_LedOn(1);
	led_no = 1;
	while (1)
	{			
		bsp_Idle();		/* CPU空闲时执行的函数，在 bsp.c */
		
		/* 这个地方可以插入其他任务 */		
		
		/* 检查定时器0时间是否到 */
		if (bsp_CheckTimer(0))
		{
			break;
		}

		if (bsp_CheckTimer(1))		/* 检查自动定时器2，间隔200ms翻转一次LED1 */
		{
			/* 先关闭所有的LED，然后在打开其中一个 */
			bsp_LedOff(1);
			bsp_LedOff(2);
			bsp_LedOff(3);
			bsp_LedOff(4);
			
			if (++led_no == 5)
			{
				led_no = 1;
			}

			bsp_LedOn(led_no);	/* 点亮其中一个LED */	
		}		
	}
	
	/* 任务结束时，应该关闭定时器，因为他们会占用后台的资源 */
	//bsp_StopTimer(0);	 单次定时器如果超时到过一次后，可以不必关闭
	bsp_StopTimer(1);
	bsp_LedOff(1);
	bsp_LedOff(2);
	bsp_LedOff(3);
	bsp_LedOff(4);
	printf("流水灯测试结束\r\n");
	printf("*************************************************************\n\r");
}


/***************************** yy-AA55(END OF FILE) *********************************/
