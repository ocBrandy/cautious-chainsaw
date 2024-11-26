#include <p30f6014A.h>
#include "adc.h"
#include "gpio.h"
#include "timer.h"
#include "usart.h"
void adcInit(void)
{
//  ADC Timer3...    
	T3CON = 0;                //T3
	TMR3 = 0;                       
	PR3 = 0x0FA0;             //sample time1MS
    
	ADPCFG = 0xFFF7;		// RB3 = analog
	ADCON1 = 0x0044;		//compare with T3 ends sampling and starts converting
	ADCON2 = 0x043C;        //channel scan enable,generate interrupt 16 times sample 
	ADCON3 = 0x0002;		//ADCS<5:0> = 2(TAD/TCY) ? 1(Minimum TAD = 334 nsec,TCY = 500nsec(2MIPS))
    ADCHS = 0x0003;
    ADCSSL = 0X000F;
    ADCON1bits.ADON = 1;
}

int ADC(void)
{
    unsigned long int ADCValue;
    T3CONbits.TON = 1;
	while(!IFS0bits.ADIF);
    IFS0bits.ADIF = 0;
    T3CONbits.TON = 0;
    
    ADCValue = ADCBUF3 + ADCBUF7 + ADCBUFB + ADCBUFF;
    ADCValue = ADCValue>>2;
//    ADCValue = ADCValue *5000; //mv
//    ADCValue = ADCValue>>12;
    return ADCValue;
//    ComSendChar(2,ADCValue>>8);
//    ComSendChar(2,ADCValue);
    
}
unsigned int ads8320(void)
{
	unsigned int temp;
	unsigned char b,c; 
	_RC4 = 0;
	_RC4 = 1;
	_RG7 = 0; 				//????
	for(b=0;b<6;b++)
	{
		_RC4 = 0;
		_RC4 = 1;
	}
	for(c=0;c<16;c++)
	{
		_RC4 = 0;			//dclock????
		_RC4 = 1;			//1/20*10^(-6)=50 000hz
		temp = (temp<<1)+_RG6;
	}
	_RG7 = 1;			//????		
	return temp ;
}
void Adc_Channel_SW(unsigned char channel)
{
    switch(channel) // 
	{
		case 1:	//Cable head AC voltage
            GPIO_PIN_SET(ADC_CHANNEL_SW_A2_PORT,ADC_CHANNEL_SW_A2_PIN,0);
            GPIO_PIN_SET(ADC_CHANNEL_SW_A1_PORT,ADC_CHANNEL_SW_A1_PIN,0);
            GPIO_PIN_SET(ADC_CHANNEL_SW_A0_PORT,ADC_CHANNEL_SW_A0_PIN,0);
            break; 
		case 2:	//Valiant voltage
            GPIO_PIN_SET(ADC_CHANNEL_SW_A2_PORT,ADC_CHANNEL_SW_A2_PIN,0);
            GPIO_PIN_SET(ADC_CHANNEL_SW_A1_PORT,ADC_CHANNEL_SW_A1_PIN,0);
            GPIO_PIN_SET(ADC_CHANNEL_SW_A0_PORT,ADC_CHANNEL_SW_A0_PIN,1);
            break; 
		case 3:	//Tension
            GPIO_PIN_SET(ADC_CHANNEL_SW_A2_PORT,ADC_CHANNEL_SW_A2_PIN,0);
            GPIO_PIN_SET(ADC_CHANNEL_SW_A1_PORT,ADC_CHANNEL_SW_A1_PIN,1);
            GPIO_PIN_SET(ADC_CHANNEL_SW_A0_PORT,ADC_CHANNEL_SW_A0_PIN,0);
            break; 
		case 4:	//Drilling displacement
            GPIO_PIN_SET(ADC_CHANNEL_SW_A2_PORT,ADC_CHANNEL_SW_A2_PIN,0);
            GPIO_PIN_SET(ADC_CHANNEL_SW_A1_PORT,ADC_CHANNEL_SW_A1_PIN,1);
            GPIO_PIN_SET(ADC_CHANNEL_SW_A0_PORT,ADC_CHANNEL_SW_A0_PIN,1);
            break; 
		case 5:	//LMotor AC voltage
            GPIO_PIN_SET(ADC_CHANNEL_SW_A2_PORT,ADC_CHANNEL_SW_A2_PIN,1);
            GPIO_PIN_SET(ADC_CHANNEL_SW_A1_PORT,ADC_CHANNEL_SW_A1_PIN,0);
            GPIO_PIN_SET(ADC_CHANNEL_SW_A0_PORT,ADC_CHANNEL_SW_A0_PIN,0);
            break; 
		case 6:	//temp sw
            GPIO_PIN_SET(ADC_CHANNEL_SW_A2_PORT,ADC_CHANNEL_SW_A2_PIN,1);
            GPIO_PIN_SET(ADC_CHANNEL_SW_A1_PORT,ADC_CHANNEL_SW_A1_PIN,0);
            GPIO_PIN_SET(ADC_CHANNEL_SW_A0_PORT,ADC_CHANNEL_SW_A0_PIN,1);
            break; 
		case 7:	//heart length displacement
            GPIO_PIN_SET(ADC_CHANNEL_SW_A2_PORT,ADC_CHANNEL_SW_A2_PIN,1);
            GPIO_PIN_SET(ADC_CHANNEL_SW_A1_PORT,ADC_CHANNEL_SW_A1_PIN,1);
            GPIO_PIN_SET(ADC_CHANNEL_SW_A0_PORT,ADC_CHANNEL_SW_A0_PIN,0);
            break; 
		case 8://oil level displacement
            GPIO_PIN_SET(ADC_CHANNEL_SW_A2_PORT,ADC_CHANNEL_SW_A2_PIN,1);
            GPIO_PIN_SET(ADC_CHANNEL_SW_A1_PORT,ADC_CHANNEL_SW_A1_PIN,1);
            GPIO_PIN_SET(ADC_CHANNEL_SW_A0_PORT,ADC_CHANNEL_SW_A0_PIN,1);
            break; 
		default :      
		break;
	}
}

void Adc_Channel6_Channel_SW(unsigned char channel)
{
    switch(channel) // choose channel
	{
		case 1:	//stepping motor temp
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A2_PORT,ADC_CHANNEL6_CHANNEL_SW_A2_PIN,0); 
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A1_PORT,ADC_CHANNEL6_CHANNEL_SW_A1_PIN,0);
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A0_PORT,ADC_CHANNEL6_CHANNEL_SW_A0_PIN,0);
            break; 
		case 2:	//3981 temp
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A2_PORT,ADC_CHANNEL6_CHANNEL_SW_A2_PIN,0);
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A1_PORT,ADC_CHANNEL6_CHANNEL_SW_A1_PIN,0);
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A0_PORT,ADC_CHANNEL6_CHANNEL_SW_A0_PIN,1);
            break; 
		case 3://Power temp
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A2_PORT,ADC_CHANNEL6_CHANNEL_SW_A2_PIN,0);
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A1_PORT,ADC_CHANNEL6_CHANNEL_SW_A1_PIN,1);
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A0_PORT,ADC_CHANNEL6_CHANNEL_SW_A0_PIN,0);
            break; 
		case 4:	//LMotor temp	
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A2_PORT,ADC_CHANNEL6_CHANNEL_SW_A2_PIN,0);
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A1_PORT,ADC_CHANNEL6_CHANNEL_SW_A1_PIN,1);
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A0_PORT,ADC_CHANNEL6_CHANNEL_SW_A0_PIN,1);
            break; 
		case 5:	//Pump oil temp
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A2_PORT,ADC_CHANNEL6_CHANNEL_SW_A2_PIN,1);
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A1_PORT,ADC_CHANNEL6_CHANNEL_SW_A1_PIN,0);
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A0_PORT,ADC_CHANNEL6_CHANNEL_SW_A0_PIN,0);
            break; 
		case 6:	//Pushing pressure
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A2_PORT,ADC_CHANNEL6_CHANNEL_SW_A2_PIN,1);
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A1_PORT,ADC_CHANNEL6_CHANNEL_SW_A1_PIN,0);
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A0_PORT,ADC_CHANNEL6_CHANNEL_SW_A0_PIN,1);
            break; 
		case 7:	//Large Pump pressure
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A2_PORT,ADC_CHANNEL6_CHANNEL_SW_A2_PIN,1);
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A1_PORT,ADC_CHANNEL6_CHANNEL_SW_A1_PIN,1);
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A0_PORT,ADC_CHANNEL6_CHANNEL_SW_A0_PIN,0);
            break; 
		case 8://Small Pump pressure
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A2_PORT,ADC_CHANNEL6_CHANNEL_SW_A2_PIN,1);
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A1_PORT,ADC_CHANNEL6_CHANNEL_SW_A1_PIN,1);
            GPIO_PIN_SET(ADC_CHANNEL6_CHANNEL_SW_A0_PORT,ADC_CHANNEL6_CHANNEL_SW_A0_PIN,1);
            break; 
		default :      
		break;
	}
}