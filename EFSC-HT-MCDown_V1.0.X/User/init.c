#include "timer.h"
#include "gpio.h"
#include "spi.h"
#include "usart.h"
#include "timer.h"
#include "adc.h"
void init(void)
{
    GpioInit();
    SpiInit();
    UsartInit();
    TimerInit();
    adcInit();
    ComSendChar(1,0x99);     //init complete
}