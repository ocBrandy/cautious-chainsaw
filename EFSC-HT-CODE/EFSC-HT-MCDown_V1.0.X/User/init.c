#include "timer.h"
#include "gpio.h"
#include "spi.h"
#include "usart.h"
#include "timer.h"
#include "adc.h"
void init(void)
{
    GpioInit();             //io初始化
    SpiInit();              //SPI初始化
    UsartInit();            //串口初始化
    TimerInit();            //定时器初始化
    adcInit();              //ADC初始化
    
    ComSendChar(1,0x99);    //init complete     初始化完成
}