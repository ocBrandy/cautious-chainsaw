#include <p30f6014a.h>


void gpio_initial(void)
{
    ADPCFG = 0xFFFF;           //all PORTB = Digital
    TRISA = 0xFFFF;
    TRISB = 0xFFFF;
    TRISC = 0xFFFF;
    TRISD = 0xFFFE;           
    TRISF = 0xFEDF;            
    TRISG = 0x4FFF;
}    
