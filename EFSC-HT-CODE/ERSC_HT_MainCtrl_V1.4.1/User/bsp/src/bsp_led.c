/*
*********************************************************************************************************
*
*	ģ������ : LEDָʾ������ģ��
*	�ļ����� : bsp_led.c
*	��    �� : V1.0
*	˵    �� : ����LEDָʾ��
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-02-01          ��ʽ����
*
*
*********************************************************************************************************
*/

#include "bsp.h"

/*
	�����������Ӳ�������޸�GPIO����
	
	����û���LEDָʾ�Ƹ���С��4�������Խ������LEDȫ������Ϊ�͵�1��LEDһ��������Ӱ�������
*/


/* �����ڶ�Ӧ��RCCʱ�� */
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
*	�� �� ��: bsp_InitLed
*	����˵��: ����LEDָʾ����ص�GPIO,  �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitLed(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_ALL_LED, ENABLE);

	/*
		�������е�LEDָʾ��GPIOΪ�������ģʽ
		���ڽ�GPIO����Ϊ���ʱ��GPIO����Ĵ�����ֵȱʡ��0����˻�����LED����.
		�����Ҳ�ϣ���ģ�����ڸı�GPIOΪ���ǰ���ȹر�LEDָʾ��
	*/
		bsp_LedOff(1);
		bsp_LedOff(2);
//	bsp_LedOff(1);
//	bsp_LedOff(2);
//	bsp_LedOff(3);
//	bsp_LedOff(4);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* �������ģʽ */
	
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
*	�� �� ��: bsp_LedOn
*	����˵��: ����ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_LedOff
*	����˵��: Ϩ��ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_LedToggle
*	����˵��: ��תָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
void bsp_LedToggle(uint8_t _no)
{
	if (_no == 1)
	{
		GPIO_PORT_LED1->ODR ^= GPIO_PIN_LED1;  //��� ��ͬΪ0����ͬΪ1
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
*	�� �� ��: bsp_IsLedOn
*	����˵��: �ж�LEDָʾ���Ƿ��Ѿ�������
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: 1��ʾ�Ѿ�������0��ʾδ����
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
*	�� �� ��: RunWaterLed
*	����˵��: ״̬1�� LED1 - LED4 ������ˮ��ʾ��ÿ�ε���1��LED��״̬����5��󷵻ء�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RunWaterLed(void)
{
	uint8_t led_no = 1;		/* LEDָʾ����� 1-4 */
	
	bsp_StartTimer(0, 10000);		/* ��ʱ��0��5000ms ���ζ�ʱ�� */
	bsp_StartAutoTimer(1, 200);		/* ��ʱ��1��500ms �Զ���װ��ʱ��, ����LED1��1HzƵ�ʷ�ת��˸ */
	bsp_LedOn(1);
	led_no = 1;
	while (1)
	{			
		bsp_Idle();		/* CPU����ʱִ�еĺ������� bsp.c */
		
		/* ����ط����Բ����������� */		
		
		/* ��鶨ʱ��0ʱ���Ƿ� */
		if (bsp_CheckTimer(0))
		{
			break;
		}

		if (bsp_CheckTimer(1))		/* ����Զ���ʱ��2�����200ms��תһ��LED1 */
		{
			/* �ȹر����е�LED��Ȼ���ڴ�����һ�� */
			bsp_LedOff(1);
			bsp_LedOff(2);
			bsp_LedOff(3);
			bsp_LedOff(4);
			
			if (++led_no == 5)
			{
				led_no = 1;
			}

			bsp_LedOn(led_no);	/* ��������һ��LED */	
		}		
	}
	
	/* �������ʱ��Ӧ�ùرն�ʱ������Ϊ���ǻ�ռ�ú�̨����Դ */
	//bsp_StopTimer(0);	 ���ζ�ʱ�������ʱ����һ�κ󣬿��Բ��عر�
	bsp_StopTimer(1);
	bsp_LedOff(1);
	bsp_LedOff(2);
	bsp_LedOff(3);
	bsp_LedOff(4);
	printf("��ˮ�Ʋ��Խ���\r\n");
	printf("*************************************************************\n\r");
}


/***************************** yy-AA55(END OF FILE) *********************************/
