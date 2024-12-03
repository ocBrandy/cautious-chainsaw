#include <p30f6014a.h>
#include "time.h"
#include "usart.h"
#include <xc.h>


void bsp_InitTimer3(void)
{
    TMR3 = 0;                         //计数值清0
   	PR3 = 0xFFFF;                     //1ms中断一次
   	T3CON = 0x0010;					//启动定时器；8分频，FCY/8 = 1M，1us计数一次	
    //IPC1 = 0x2000;                  //中断优先级为2级
   //	_T3IF = 0;						//清除中断标志位
   	_T3IE = 0;			 			//使能定时器3中断
    T3CONbits.TON = 1;				//启动定时器
}

unsigned long temp;
void Delay_ms (unsigned char ms)
{
    TMR3 = 0;
    temp = ms * 0x07D0;
    while(TMR3 < temp ){}
}