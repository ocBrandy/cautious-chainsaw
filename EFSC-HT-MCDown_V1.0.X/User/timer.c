#include <p30f6014A.h>
#include  "timer.h"
#include "steppingmotor.h"
#include "usart.h"


unsigned int delaycnt;
void TimerInit(void)
{
   //***************************timer1(Gmma)***************************	
    TMR1=0;							//GR??
   	PR1=0Xffff;			 		
    T1CONbits.TCS = 1;
   	_T1IF=0;							//Timer1?????
   	_T1IE=0;							//Timer1????
   	T1CON=0X0002;				    //????1?256  ??????
   	T1CONbits.TON = 1;				//Timer1????	
//***************************timer2(10ms)***************************		
    TMR2 = 0;
    PR2 = 625;
    _T2IF = 0;
    _T2IP = 2;
    _T2IE = 1;
    T2CON = 0x8020;                     //1:64
    T2CONbits.TON = 1;
 //***************************timer5()***************************
   	TMR5 = 0;						
   	PR5 = 1;//0X030d;//1F40;            
   	T5CON = 0x0030;                     //1:256	 		
   	_T5IF = 0;							//Timer5?????
   	_T5IE = 0;			 				//Timer5????
    IPC5 = 0x0320;
    T5CONbits.TON =1;
  //**************************timer4(delay)******************************
    TMR4 = 0;
    PR4 = 0xffff;
    _T4IF = 0;
   // _T4IP = 2;
    _T4IE = 0;
    T4CON = 0x8010;                     //1:8
   // T4CONbits.TON = 1;
}
unsigned char reportflag = 0;
static unsigned char report_cnt = 0;
unsigned int cur_gmma_cnt = 0;
extern unsigned int motorsel;
extern unsigned int pluse_cnt;
void __attribute__ ((__interrupt__, no_auto_psv)) _T2Interrupt (void) 
{
    _T2IF = 0;
    motorsel--;
    if(motorsel == 100)
    {
       MotorStop();
    }
    report_cnt ++;
    if(report_cnt >= 25)
    {
        cur_gmma_cnt = TMR1;
        TMR1=0;
        report_cnt = 0;
        reportflag = 1;
    }
}

void __attribute__ ((__interrupt__, no_auto_psv)) _T5Interrupt (void) 
{
	_T5IF = 0;
    if(pluse_cnt & 0X0001)
    {
        //ComSendChar(1,0x55);
        _LATB13 = 0;
    }     
    else
    {
        _LATB13 = 1;
    }
    
    if(pluse_cnt > 0)
    {
        pluse_cnt --;
    }
}
//void __attribute__ ((__interrupt__, no_auto_psv)) _T4Interrupt (void) // 1us 
//{
//	_T4IF = 0;
//    _LATG15 = ~_LATG15; 
//}
void delay_us(unsigned int temp)
{
    TMR4 = 0;
    while(TMR4<(temp>>1)){}
}