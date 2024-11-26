#ifndef __ADC_INIT_H
#define	__ADC_INIT_H


#include "stm32f10x.h"
#include "stm32f10x_adc.h"

// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��
/********************ADC1����ͨ�������ţ�����**************************/
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_CLK                       RCC_APB2Periph_ADC1

#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
 #define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOC  


// ע��
// 1-PC0 �ڰԵ�����ӵ��Ƿ�������Ĭ�ϱ�����
// 2-PC0 ��ָ��������ӵ���SPI FLASH�� Ƭѡ��Ĭ�ϱ�����
// ���� PC0 �� ADC ת��ͨ����ʱ�򣬽�����ܻ������

// ת��ͨ������ PA4 PB1 PC2 PC3 PC4
#define    NOFCHANEL								5		 

#define    ADC_PIN1                      GPIO_Pin_4
#define    ADC_CHANNEL1                  ADC_Channel_4
#define    ADC_PORT1                      GPIOA

#define    ADC_PIN2                      GPIO_Pin_1
#define    ADC_CHANNEL2                  ADC_Channel_9
#define    ADC_PORT2                      GPIOB

#define    ADC_PIN3                      GPIO_Pin_2
#define    ADC_CHANNEL3                  ADC_Channel_12
#define    ADC_PORT3                      GPIOC

#define    ADC_PIN4                      GPIO_Pin_3
#define    ADC_CHANNEL4                  ADC_Channel_13
#define    ADC_PORT4                     GPIOC

#define    ADC_PIN5                      GPIO_Pin_4
#define    ADC_CHANNEL5                  ADC_Channel_14
#define    ADC_PORT5                     GPIOC



// ADC1 ��Ӧ DMA1ͨ��1��ADC3��ӦDMA2ͨ��5��ADC2û��DMA����
#define    ADC_x                         ADC1
#define    ADC_DMA_CHANNEL               DMA1_Channel1
#define    ADC_DMA_CLK                   RCC_AHBPeriph_DMA1


/**************************��������********************************/
void ADCx_Init(void);

void rmsvalue(void);
#endif /* __ADC_H */



