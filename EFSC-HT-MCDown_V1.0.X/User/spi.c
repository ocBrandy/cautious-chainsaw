#include <p30f6014a.h>
#include "spi.h"
	/*spi INIT*/   
void SpiInit(void)
{
    IFS0bits.SPI1IF = 0;    //Clear the Interrupt Flag
    IEC0bits.SPI1IE = 0;    //disable the Interrupt 
    IPC2bits.SPI1IP = 4;
    
    // SPI1CON1 Register Settings 
    SPI1CONbits.FRMEN = 0;  //Frme mode disable. 
    SPI1CONbits.SPIFSD = 0; //
    SPI1CONbits.DISSDO = 0; //SDO1pin is controlled by the module.
    SPI1CONbits.MODE16 = 0; //Communication is byte-wide (8 bits). 
    SPI1CONbits.SMP = 0;    //Input Data is sampled at the middle of data output time.
    SPI1CONbits.CKE = 1;    //Serial output data changes on transition from active clock state to Idle clock state SSEN
    SPI1CONbits.SSEN = 0;   //SS1 pin is controlled by IO Port
    SPI1CONbits.CKP = 0;    //Idle state for clock is a low level; active state is a high level
    SPI1CONbits.MSTEN = 1;  //Master Mode Enabled
    SPI1CONbits.SPRE = 7;  //
    SPI1CONbits.PPRE = 3;  //
    
    // SPI1STA Register Settings 
    SPI1STATbits.SPIROV = 0;
    SPI1STATbits.SPISIDL = 1;   //Disable SPI Module when idle
    SPI1STATbits.SPIEN = 1;     //Enable SPI Module 
    
    //Interrupt Controller Settings
    IFS0bits.SPI1IF = 0; //Clear the Interrupt Flag
    IEC0bits.SPI1IE = 0; //Enable the Interrupt
}

unsigned char SPI_RW(unsigned char WD)
{
  	unsigned char result;
  	unsigned int count = 0;
  		
    SPI1BUF = WD;   //send Char           
    while(IFS0bits.SPI1IF == 0)
    {
        count++;
        if(count>30000)
            break;
    }
    _SPI1IF = 0;
    result = SPI1BUF;

    return result;    
}
