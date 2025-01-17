#include  "gpio.h"
#include <p30f6014a.h>

#define MAX_PIN_NUM        16

static void GPIOA_Cfg(unsigned char pin,unsigned char value)
{
    switch(pin)
    {
        case 6:
            TRISAbits.TRISA6 = value;
            break;
        case 7:
            TRISAbits.TRISA7 = value;
            break;
        case 9:
            TRISAbits.TRISA9 = value;
            break;
        case 10:
            TRISAbits.TRISA10 = value;
            break;
        case 12:
            TRISAbits.TRISA12 = value;
            break;
        case 13:
            TRISAbits.TRISA13 = value;
            break;
        case 14:
            TRISAbits.TRISA14 = value;
            break;
        case 15:
            TRISAbits.TRISA15 = value;
            break;
        default:
            break;
    }
}
static void GPIOB_Cfg(unsigned char pin,unsigned char value)
{
    switch(pin)
    {
        case 0:
            TRISBbits.TRISB0 = value;
            break;
        case 1:
            TRISBbits.TRISB1 = value;
            break;
        case 2:
            TRISBbits.TRISB2 = value;
            break;
        case 3:
            TRISBbits.TRISB3 = value;
            break;
        case 4:
            TRISBbits.TRISB4 = value;
            break;
        case 5:
            TRISBbits.TRISB5 = value;
            break;
        case 6:
            TRISBbits.TRISB6 = value;
            break;
        case 7:
            TRISBbits.TRISB7 = value;
            break;
        case 8:
            TRISBbits.TRISB8 = value;
            break;
        case 9:
            TRISBbits.TRISB9 = value;
            break;
        case 10:
            TRISBbits.TRISB10 = value;
            break;
        case 11:
            TRISBbits.TRISB11 = value;
            break;
        case 12:
            TRISBbits.TRISB12 = value;
            break;
        case 13:
            TRISBbits.TRISB13 = value;
            break;
        case 14:
            TRISBbits.TRISB14 = value;
            break;
        case 15:
            TRISBbits.TRISB15 = value;
            break;
        default:
            break;
    }
}
static void GPIOC_Cfg(unsigned char pin,unsigned char value)
{
    switch(pin)
    {
        case 1:
            TRISCbits.TRISC1 = value;
            break;
        case 2:
            TRISCbits.TRISC2 = value;
            break;
        case 3:
            TRISCbits.TRISC3 = value;
            break;
        case 4:
            TRISCbits.TRISC4 = value;
            break;
        case 13:
            TRISCbits.TRISC13 = value;
            break;
        case 14:
            TRISCbits.TRISC14 = value;
            break;
        case 15:
            TRISCbits.TRISC15 = value;
            break;
        default:
            break;
    }
}
static void GPIOD_Cfg(unsigned char pin,unsigned char value)
{
    switch(pin)
    {
        case 0:
            TRISDbits.TRISD0 = value;
            break;
        case 1:
            TRISDbits.TRISD1 = value;
            break;
        case 2:
            TRISDbits.TRISD2 = value;
            break;
        case 3:
            TRISDbits.TRISD3 = value;
            break;
        case 4:
            TRISDbits.TRISD4 = value;
            break;
        case 5:
            TRISDbits.TRISD5 = value;
            break;
        case 6:
            TRISDbits.TRISD6 = value;
            break;
        case 7:
            TRISDbits.TRISD7 = value;
            break;
        case 8:
            TRISDbits.TRISD8 = value;
            break;
        case 9:
            TRISDbits.TRISD9 = value;
            break;
        case 10:
            TRISDbits.TRISD10 = value;
            break;
        case 11:
            TRISDbits.TRISD11 = value;
            break;
        case 12:
            TRISDbits.TRISD12 = value;
            break;
        case 13:
            TRISDbits.TRISD13 = value;
            break;
        case 14:
            TRISDbits.TRISD14 = value;
            break;
        case 15:
            TRISDbits.TRISD15 = value;
            break;
        default:
            break;
    }
}
static void GPIOF_Cfg(unsigned char pin,unsigned char value)
{
    switch(pin)
    {
        case 0:
            TRISFbits.TRISF0 = value;
            break;
        case 1:
            TRISFbits.TRISF1 = value;
            break;
        case 2:
            TRISFbits.TRISF2 = value;
            break;
        case 3:
            TRISFbits.TRISF3 = value;
            break;
        case 4:
            TRISFbits.TRISF4 = value;
            break;
        case 5:
            TRISFbits.TRISF5 = value;
            break;
        case 6:
            TRISFbits.TRISF6 = value;
            break;
        case 7:
            TRISFbits.TRISF7 = value;
            break;
        case 8:
            TRISFbits.TRISF8 = value;
            break;
        default:
            break;
    }
}
static void GPIOG_Cfg(unsigned char pin,unsigned char value)
{
    switch(pin)
    {
        case 0:
            TRISGbits.TRISG0 = value;
            break;
        case 1:
            TRISGbits.TRISG1 = value;
            break;
        case 2:
            TRISGbits.TRISG2 = value;
            break;
        case 3:
            TRISGbits.TRISG3 = value;
            break;
        case 6:
            TRISGbits.TRISG6 = value;
            break;
        case 7:
            TRISGbits.TRISG7 = value;
            break;
        case 8:
            TRISGbits.TRISG8 = value;
            break;
        case 9:
            TRISGbits.TRISG9 = value;
            break;
        case 12:
            TRISGbits.TRISG12 = value;
            break;
        case 13:
            TRISGbits.TRISG13 = value;
            break;
        case 14:
            TRISGbits.TRISG14 = value;
            break;
        case 15:
            TRISGbits.TRISG15 = value;
            break;
        default:
            break;
    }
}

void GpioPinConfig(e_IO_port port,unsigned char pin,e_IO_mode mode)
{
    unsigned char value;
    if(pin >= MAX_PIN_NUM || port >= 6) {return;}
    if(mode == GPIO_input)
    {
        value = 1;
    }
    else
    {
        value = 0;
    }
    
    switch(port)
    {
        case GPIO_a:
            GPIOA_Cfg(pin,value);
            break;
        case GPIO_b:
            GPIOB_Cfg(pin,value);
            break;
        case GPIO_c:
            GPIOC_Cfg(pin,value);
            break;
        case GPIO_d:
            GPIOD_Cfg(pin,value);
            break;
        case GPIO_f:
            GPIOF_Cfg(pin,value);
            break;
        case GPIO_g:
            GPIOG_Cfg(pin,value);
            break;
        default:
            break;
    }
}

void GpioPinSet(e_IO_port port,unsigned char pin,unsigned char value)
{
    if(pin >= MAX_PIN_NUM || port >= 6) {return;}
    _LATG15 = value;
}

unsigned char GpioPinRead(e_IO_port port,unsigned char pin)
{
    unsigned char value;
    if(pin >= MAX_PIN_NUM || port >= 6) {return 0;}
    
    value = _RG12;
    return value;
}
/****************************************************************************************
 init
 ****************************************************************************************/
void GpioInit(void)
{
    ADPCFG = 0xFFFF;         //all PORTB = Digital
    
    TRISA = 0xFFFF;
	TRISB = 0xFFFF;  			            			
	TRISC = 0xFFFF;				
	TRISD = 0xFFFF;				
	TRISF = 0xFFFF;				
	TRISG = 0xFFFF;				

    //***************************IO????????***************************		
	GpioPinConfig(TEST1_PORT,TEST1_PIN,GPIO_output);
    GpioPinConfig(TEST2_PORT,TEST2_PIN,GPIO_output);
    GpioPinConfig(TEST3_PORT,TEST3_PIN,GPIO_output); 
    
    GpioPinConfig(ADS8320_CLK_PORT,ADS8320_CLK_PIN,GPIO_output);
    GpioPinConfig(ADS8320_CS_PORT,ADS8320_CS_PIN,GPIO_output);
    GpioPinConfig(ADS8320_IN_PORT,ADS8320_IN_PIN,GPIO_input);
    
    GpioPinConfig(ADC_CHANNEL_SW_A0_PORT,ADC_CHANNEL_SW_A0_PIN,GPIO_output);
    GpioPinConfig(ADC_CHANNEL_SW_A1_PORT,ADC_CHANNEL_SW_A1_PIN,GPIO_output);
    GpioPinConfig(ADC_CHANNEL_SW_A2_PORT,ADC_CHANNEL_SW_A2_PIN,GPIO_output);
    GpioPinConfig(ADC_CHANNEL6_CHANNEL_SW_A0_PORT,ADC_CHANNEL6_CHANNEL_SW_A0_PIN,GPIO_output);
    GpioPinConfig(ADC_CHANNEL6_CHANNEL_SW_A1_PORT,ADC_CHANNEL6_CHANNEL_SW_A1_PIN,GPIO_output);
    GpioPinConfig(ADC_CHANNEL6_CHANNEL_SW_A2_PORT,ADC_CHANNEL6_CHANNEL_SW_A2_PIN,GPIO_output);
    GpioPinConfig(TEMPERATURE_PORT,TEMPERATURE_PIN,GPIO_input);
    
    
   GpioPinConfig(MOTOR_EN_PORT,MOTOR_EN_PIN,GPIO_output);// ???
   GpioPinConfig(MOTOR_PLUSE_PORT,MOTOR_PLUSE_PIN,GPIO_output);
   GpioPinConfig(MOTOR_DIRECTION_PORT,MOTOR_DIRECTION_PIN,GPIO_output);
   
   
    GpioPinConfig(PUSH_PORT,PUSH_PIN,GPIO_output);
    GpioPinConfig(MOTOR_RUN_STOP_PORT,MOTOR_RUN_STOP_PIN,GPIO_output);
    GpioPinConfig(DRILL_PORT,DRILL_PIN,GPIO_output);
    
    GpioPinConfig(GMMA_OUT_PORT,GMMA_OUT_PIN,GPIO_input);
    GpioPinConfig(GMMA_ONOFF_PORT,GMMA_ONOFF_PIN,GPIO_output);
	GpioPinConfig(SPI1SS_PORT,SPI1SS_PIN,GPIO_output);
    GpioPinConfig(SPI1MISO_PORT,SPI1MISO_PIN,GPIO_input);
    GpioPinConfig(SPI1MOSI_PORT,SPI1MOSI_PIN,GPIO_output);
    GpioPinConfig(SPI1SCK_PORT,SPI1SCK_PIN,GPIO_output);
    
    /****************************************************************************************************/
    GPIO_PIN_SET(TEST1_PORT,TEST1_PIN,0);
    GPIO_PIN_SET(TEST2_PORT,TEST2_PIN,0);
    GPIO_PIN_SET(TEST3_PORT,TEST3_PIN,0);
    
   GPIO_PIN_SET(MOTOR_EN_PORT,MOTOR_EN_PIN,1);
   GPIO_PIN_SET(MOTOR_PLUSE_PORT,MOTOR_PLUSE_PIN,1);
   GPIO_PIN_SET(MOTOR_DIRECTION_PORT,MOTOR_DIRECTION_PIN,0);
   
   
   GPIO_PIN_SET(PUSH_PORT,PUSH_PIN,0);
   GPIO_PIN_SET(MOTOR_RUN_STOP_PORT,MOTOR_RUN_STOP_PIN,0);
   GPIO_PIN_SET(DRILL_PORT,DRILL_PIN,0);
   
   GPIO_PIN_SET(GMMA_ONOFF_PORT,GMMA_ONOFF_PIN,1);
}

