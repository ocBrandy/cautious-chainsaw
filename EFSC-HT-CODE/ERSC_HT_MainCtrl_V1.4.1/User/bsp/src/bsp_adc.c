#include "bsp_adc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_adc.h"
#include "ETH_DataStorage.h"
#include "VGUS_LCD.h"
#include "math.h"
//static uint8_t collect_cnt = 0;
#define ADC_PERIOD_MS             80

// motor current
#define RESULT_BUF_SIZE           4
//static uint32_t ms_backup = 0;
//static uint16_t min = 0xffff,max = 0,cur_adc,result_buf[RESULT_BUF_SIZE];
//uint32_t motor_current = 0,cable_voltage = 0,motor_voltage = 0;
//static uint32_t voltage_mf,result_index = 0;

//// cable voltage 
//#define CABLE_RESULT_BUF_SIZE           16
//static uint16_t min_cable_v = 0xffff,max_cable_v = 0,cur_adc_cable_v,result_buf_cable_v[CABLE_RESULT_BUF_SIZE];
//static uint32_t result_index_cable_v = 0;

//// motor voltage 
//#define MOTOR_RESULT_BUF_SIZE           16
//static uint16_t min_motor_v = 0xffff,max_motor_v = 0,cur_adc_motor_v,result_buf_motor_v[MOTOR_RESULT_BUF_SIZE];
//static uint32_t result_index_motor_v = 0;

__IO uint16_t ADC_ConvertedValue[NOFCHANEL]={0,0,0,0,0,};


/**
  * @brief  ADC GPIO ��ʼ��
  * @param  ��
  * @retval ��
  */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// �� ADC IO�˿�ʱ��
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE ); // �����ƺ�ȱ�ٶ� ADC_PIN1 ��ADC_PIN2��ʱ������
	
	// ���� ADC IO ����ģʽ
	GPIO_InitStructure.GPIO_Pin = ADC_PIN1 | ADC_PIN2 | ADC_PIN3  | ADC_PIN4 | ADC_PIN5 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	// ��ʼ�� ADC IO
	GPIO_Init(ADC_PORT1, &GPIO_InitStructure);
	GPIO_Init(ADC_PORT2, &GPIO_InitStructure);
	GPIO_Init(ADC_PORT3, &GPIO_InitStructure);	
	GPIO_Init(ADC_PORT4, &GPIO_InitStructure);	
	GPIO_Init(ADC_PORT5, &GPIO_InitStructure);
}

/****************************************
  * @brief  ����ADC����ģʽ
  * @param  ��
  * @retval ��
 *******************************************/
static void ADCx_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	// ��DMAʱ��
	RCC_AHBPeriphClockCmd(ADC_DMA_CLK, ENABLE);
	// ��ADCʱ��
	ADC_APBxClock_FUN ( ADC_CLK, ENABLE );
	
	// ��λDMA������
	DMA_DeInit(ADC_DMA_CHANNEL);
	
	// ���� DMA ��ʼ���ṹ��
	// �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( ADC_x->DR ) );
	
	// �洢����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;
	
	// ����Դ��������
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	// ��������С��Ӧ�õ�������Ŀ�ĵصĴ�С
	DMA_InitStructure.DMA_BufferSize = NOFCHANEL;
	
	// ����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	// �洢����ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	
	// �������ݴ�СΪ���֣��������ֽ�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	// �ڴ����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	// ѭ������ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	

	// DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	// ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	// ��ʼ��DMA
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	
	// ʹ�� DMA ͨ��
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
	
	// ADC ģʽ����
	// ֻʹ��һ��ADC�����ڵ�ģʽ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// ɨ��ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 

	// ����ת��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

	// �����ⲿ����ת���������������
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

	// ת������Ҷ���
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// ת��ͨ������
	ADC_InitStructure.ADC_NbrOfChannel = NOFCHANEL;	
		
	// ��ʼ��ADC
	ADC_Init(ADC_x, &ADC_InitStructure);
	
	// ����ADCʱ�ӣΪPCLK2��8��Ƶ����9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	// ����ADC ͨ����ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC_x, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_Channel_9, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_Channel_12, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_Channel_13, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_Channel_14, 5, ADC_SampleTime_55Cycles5);

	
	// ʹ��ADC DMA ����
	ADC_DMACmd(ADC_x, ENABLE);
	
	// ����ADC ������ʼת��
	ADC_Cmd(ADC_x, ENABLE);
	
	// ��ʼ��ADC У׼�Ĵ���  
	ADC_ResetCalibration(ADC_x);
	// �ȴ�У׼�Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(ADC_x));
	
	// ADC��ʼУ׼
	ADC_StartCalibration(ADC_x);
	// �ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADC_x));
	
	// ����û�в����ⲿ����������ʹ���������ADCת�� 
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
}

/**
  * @brief  ADC��ʼ��
  * @param  ��
  * @retval ��
  */

extern void RefreshNumber(uint16_t addr,uint16_t number);
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}



// �ֲ����������ڱ���ת�������ĵ�ѹֵ 	 
//static float ADC_ConvertedValueLocal[NOFCHANEL][64];
//  �ֲ����������ڱ������ʱADC�ɵ��ĵ�ѹֵ
static float voltage[NOFCHANEL];  
uint8_t  i,j,m;
uint16_t adc_num1,adc_num2,adc_num3;
float t,sum1,sum2,sum3;
uint16_t ctrl_current,Motor_Current,motor_voltage,Motor_Voltage;
float adc_buf[640]= {0};
extern s_eth_send send_data;	
float adc_initv;
extern uint16_t TwoBytesReverse(uint16_t src);
//****************ƫ�õ�ѹ�ɼ�*****************//
void Initialvoltagevalue(void)	
{
	u16 i;
	u32 s0,s1,s2;
	
	s0 = 0;
	s1 = 0;
	s2 = 0;
	for(i=0;i<512;i++)
	{
		s0 += ADC_ConvertedValue[2];
		s1 += ADC_ConvertedValue[3];
		s2 += ADC_ConvertedValue[4];
		bsp_DelayUS(100);
	}
	s0 = s0 >> 9;
	s1 = s1 >> 9;
	s2 = s2 >> 9;
	voltage[0] = (float)s0/4096*3.3;	
	voltage[1] = (float)s1/4096*3.3;
	voltage[2] = (float)s2/4096*3.3;	
}
uint8_t send_flag = 0;

/*************************���Ƶ����ɼ�*****************************/
void ADC_Ctrl_Current_acqdata(void)
{
	
	float adc0,rms0;
	bsp_DelayUS(230);
	
	adc0 = (float)ADC_ConvertedValue[2]/4096*3.3- voltage[0];
	
	adc0 = adc0 * adc0;
	sum1 += adc0;
	adc_num1++;
	if(adc_num1>154)
	{
		adc_num1 = 0;
		
		sum1 = sum1 / 154;
		rms0 = sqrt(sum1);  //ƽ����ȡƽ�����ٿ���
		ctrl_current = rms0/264*1000*1000; //���Ƶ���.
//////		printf("ctrl_current %d",ctrl_current);
	if(ctrl_current < 35)
	{
		ctrl_current = 0;	
	}
	
	}
}

/*************************��������ɼ�*****************************/
void ADC_Motor_Current_acqdata(void)
	{
			float adc1,adc2,rms1,rms2;
			float Adc_freq_point;
			float  adc_time;
			float freq;
				
				
			freq = send_data.data.real_freq;
			freq = freq/100;
			adc_time = 1/freq;
			adc_time = adc_time*1000;//ms
			Adc_freq_point = (int)adc_time*2/0.324;
			
			adc1 = (float)ADC_ConvertedValue[3]/4096*3.3 - voltage[1];
			adc2 = (float)ADC_ConvertedValue[4]/4096*3.3 - voltage[2];
			
			adc2 = adc2 * adc2;
			adc1 = adc1 * adc1;
			sum2 += adc1;
			sum3 += adc2;
			adc_num2++;
			
			
			if(adc_num2 > Adc_freq_point)
			{
					adc_num2 = 0;
					
////					GPIOE ->ODR ^= GPIO_Pin_6;
					sum2 = sum2 / Adc_freq_point;
					sum3 = sum3 / Adc_freq_point;
					rms1 = sqrt(sum2);  //ƽ����ȡƽ�����ٿ���
					rms2 = sqrt(sum3);//* 3961.53;
					Motor_Voltage = rms2 * 22.36 * 102.90;
					Motor_Current = rms1/264*10000;  //���Ƶ���.
					
			
////					printf("adc_initv %x",send_data.AC_voltage_of_floor_motor);
			}

}

	
/*********************************************END OF FILE**********************/

