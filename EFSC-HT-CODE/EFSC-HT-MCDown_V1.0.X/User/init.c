#include "timer.h"
#include "gpio.h"
#include "spi.h"
#include "usart.h"
#include "timer.h"
#include "adc.h"
void init(void)
{
    GpioInit();             //io��ʼ��
    SpiInit();              //SPI��ʼ��
    UsartInit();            //���ڳ�ʼ��
    TimerInit();            //��ʱ����ʼ��
    adcInit();              //ADC��ʼ��
    
    ComSendChar(1,0x99);    //init complete     ��ʼ�����
}