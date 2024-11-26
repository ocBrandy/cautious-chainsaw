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
  * @brief  ADC GPIO 初始化
  * @param  无
  * @retval 无
  */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 打开 ADC IO端口时钟
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE ); // 这里似乎缺少对 ADC_PIN1 ，ADC_PIN2的时钟配置
	
	// 配置 ADC IO 引脚模式
	GPIO_InitStructure.GPIO_Pin = ADC_PIN1 | ADC_PIN2 | ADC_PIN3  | ADC_PIN4 | ADC_PIN5 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	// 初始化 ADC IO
	GPIO_Init(ADC_PORT1, &GPIO_InitStructure);
	GPIO_Init(ADC_PORT2, &GPIO_InitStructure);
	GPIO_Init(ADC_PORT3, &GPIO_InitStructure);	
	GPIO_Init(ADC_PORT4, &GPIO_InitStructure);	
	GPIO_Init(ADC_PORT5, &GPIO_InitStructure);
}

/****************************************
  * @brief  配置ADC工作模式
  * @param  无
  * @retval 无
 *******************************************/
static void ADCx_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	// 打开DMA时钟
	RCC_AHBPeriphClockCmd(ADC_DMA_CLK, ENABLE);
	// 打开ADC时钟
	ADC_APBxClock_FUN ( ADC_CLK, ENABLE );
	
	// 复位DMA控制器
	DMA_DeInit(ADC_DMA_CHANNEL);
	
	// 配置 DMA 初始化结构体
	// 外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( ADC_x->DR ) );
	
	// 存储器地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;
	
	// 数据源来自外设
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	// 缓冲区大小，应该等于数据目的地的大小
	DMA_InitStructure.DMA_BufferSize = NOFCHANEL;
	
	// 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	// 存储器地址递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	
	// 外设数据大小为半字，即两个字节
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	// 内存数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	// 循环传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	

	// DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	// 禁止存储器到存储器模式，因为是从外设到存储器
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	// 初始化DMA
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	
	// 使能 DMA 通道
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
	
	// ADC 模式配置
	// 只使用一个ADC，属于单模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// 扫描模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 

	// 连续转换模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

	// 不用外部触发转换，软件开启即可
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

	// 转换结果右对齐
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// 转换通道个数
	ADC_InitStructure.ADC_NbrOfChannel = NOFCHANEL;	
		
	// 初始化ADC
	ADC_Init(ADC_x, &ADC_InitStructure);
	
	// 配置ADC时钟Ｎ狿CLK2的8分频，即9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	// 配置ADC 通道的转换顺序和采样时间
	ADC_RegularChannelConfig(ADC_x, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_Channel_9, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_Channel_12, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_Channel_13, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_Channel_14, 5, ADC_SampleTime_55Cycles5);

	
	// 使能ADC DMA 请求
	ADC_DMACmd(ADC_x, ENABLE);
	
	// 开启ADC ，并开始转换
	ADC_Cmd(ADC_x, ENABLE);
	
	// 初始化ADC 校准寄存器  
	ADC_ResetCalibration(ADC_x);
	// 等待校准寄存器初始化完成
	while(ADC_GetResetCalibrationStatus(ADC_x));
	
	// ADC开始校准
	ADC_StartCalibration(ADC_x);
	// 等待校准完成
	while(ADC_GetCalibrationStatus(ADC_x));
	
	// 由于没有采用外部触发，所以使用软件触发ADC转换 
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
}

/**
  * @brief  ADC初始化
  * @param  无
  * @retval 无
  */

extern void RefreshNumber(uint16_t addr,uint16_t number);
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}



// 局部变量，用于保存转换计算后的电压值 	 
//static float ADC_ConvertedValueLocal[NOFCHANEL][64];
//  局部变量，用于保存空载时ADC采到的电压值
static float voltage[NOFCHANEL];  
uint8_t  i,j,m;
uint16_t adc_num1,adc_num2,adc_num3;
float t,sum1,sum2,sum3;
uint16_t ctrl_current,Motor_Current,motor_voltage,Motor_Voltage;
float adc_buf[640]= {0};
extern s_eth_send send_data;	
float adc_initv;
extern uint16_t TwoBytesReverse(uint16_t src);
//****************偏置电压采集*****************//
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

/*************************控制电流采集*****************************/
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
		rms0 = sqrt(sum1);  //平方和取平均，再开方
		ctrl_current = rms0/264*1000*1000; //控制电流.
//////		printf("ctrl_current %d",ctrl_current);
	if(ctrl_current < 35)
	{
		ctrl_current = 0;	
	}
	
	}
}

/*************************电机电流采集*****************************/
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
					rms1 = sqrt(sum2);  //平方和取平均，再开方
					rms2 = sqrt(sum3);//* 3961.53;
					Motor_Voltage = rms2 * 22.36 * 102.90;
					Motor_Current = rms1/264*10000;  //控制电流.
					
			
////					printf("adc_initv %x",send_data.AC_voltage_of_floor_motor);
			}

}

	
/*********************************************END OF FILE**********************/

