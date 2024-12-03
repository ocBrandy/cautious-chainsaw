#include "GPIO.h"
#include"USART.h"
#include"Timer.h"
#include"main.h"

void init (void)
{
    gpio_initial();
    UsartInit(); 
    bsp_InitTimer3();
}

unsigned int Checksum (unsigned char *p,unsigned int _len)
{
    unsigned int i;
    unsigned int sum = 0;
    
    for(i=0;i<_len;i++)
    {
        sum = sum + p[i];
    }
    
    return sum;
}
