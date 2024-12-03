#include <p30f6014a.h>
#include "time.h"
#include "usart.h"
#include <xc.h>


void bsp_InitTimer3(void)
{
    TMR3 = 0;                         //����ֵ��0
   	PR3 = 0xFFFF;                     //1ms�ж�һ��
   	T3CON = 0x0010;					//������ʱ����8��Ƶ��FCY/8 = 1M��1us����һ��	
    //IPC1 = 0x2000;                  //�ж����ȼ�Ϊ2��
   //	_T3IF = 0;						//����жϱ�־λ
   	_T3IE = 0;			 			//ʹ�ܶ�ʱ��3�ж�
    T3CONbits.TON = 1;				//������ʱ��
}

unsigned long temp;
void Delay_ms (unsigned char ms)
{
    TMR3 = 0;
    temp = ms * 0x07D0;
    while(TMR3 < temp ){}
}