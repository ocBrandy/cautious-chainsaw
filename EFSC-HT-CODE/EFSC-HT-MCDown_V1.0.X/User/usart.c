#include  "usart.h"
#include <p30f6014a.h>
#include <stdio.h>
#include "gpio.h"
#include "steppingmotor.h"
#include "collection.h"
#include "adc.h"
void UsartInit(void)
{
    //***************************uart2(communication)***************3************	
	U2MODE = 0x8000;				//UARTEN=1,串口2复用使能
	U2STA = 0x8000;					//UTXISEL=1,UTXEN=0,URXISEL0=0,RXISEL1=0
	U2BRG = 0x000C;					//BAUD RATE 19.2KHz   (波特率 = 4M/(16*19200)-1)
	IPC6 =  0x0007;					//UART2 Receiver Interrupt is priority 7
	_U2TXIE =0;						//TX中断关闭
	_U2RXIE =1;						//RX中断打开
	U2STAbits.UTXEN = 1;			
//***************************uart1(log)***************************
	U1MODE = 0x8000;				//UARTEN=1,串口2复用使能
	U1STA = 0x8000;					//UTXISEL=1,UTXEN=0,URXISEL0=0,RXISEL1=0
	U1BRG = 0x000C;					//BAUD RATE 19.2KHz   (波特率 = 4M/(16*19200)-1)
	_U1TXIE =0;						//TX中断关闭
	_U1RXIE = 0;					//RX中断关闭
	U1STAbits.UTXEN = 1;			
}
unsigned char uart_rx = 0;
unsigned char cmdbuf[4];
unsigned char uart_rx_num = 0;
unsigned char frameflag = 0;
unsigned char uart_rx_en = 0;
unsigned char cmdstate = 0xFF;
void __attribute__ ((__interrupt__, no_auto_psv)) _U2RXInterrupt (void) 
{
    _U2RXIF = 0;
    uart_rx = U2RXREG;
    if(uart_rx == 0xaa && frameflag == 0)
    {
        uart_rx_num = 0;
        cmdbuf[uart_rx_num] = uart_rx;
        frameflag = 1;
    }
    else
    {
        uart_rx_num ++;
        cmdbuf[uart_rx_num] = uart_rx;
        if(uart_rx_num >= 3)
        {
            uart_rx_num = 0;
            uart_rx_en = 1;
            frameflag = 0;
        }
    }
}
unsigned int motorsel;
void cmdpoll(void)
{
    if(uart_rx_en == 1)
    {
        uart_rx_en = 0;
        switch(cmdbuf[1])
        {
            case 0x02:                              //LMotor run
            GPIO_PIN_SET(MOTOR_RUN_STOP_PORT,MOTOR_RUN_STOP_PIN,1);
            cmdstate = 0x02;
            break;
            case 0x03:                              //LMotor stop
            GPIO_PIN_SET(MOTOR_RUN_STOP_PORT,MOTOR_RUN_STOP_PIN,0);
            cmdstate = 0x03;
            break;
            case 0x04:                              //Drilling
            GPIO_PIN_SET(DRILL_PORT,DRILL_PIN,1);
            cmdstate = 0x04;
            break;
            case 0x05:                              //Drill back
            GPIO_PIN_SET(DRILL_PORT,DRILL_PIN,0);
            cmdstate = 0x05;
            break;
            case 0x06:                              //push up
            GPIO_PIN_SET(PUSH_PORT,PUSH_PIN,1);
            cmdstate = 0x06;
            break;
            case 0x07:                              //push down
            GPIO_PIN_SET(PUSH_PORT,PUSH_PIN,0);
            cmdstate = 0x07;
            break;
            case 0x08:                              //pressure up
            MotorRun(PLUSE_CNT_1_point_5,1);
            motorsel = 600;
            cmdstate = 0x08;
            break;
            case 0x09:                              //pressure hold
            //MotorStop();
            cmdstate = 0x09;
            break;
            case 0x0a:                              //pressure down
            MotorRun(PLUSE_CNT_1_point_5,0);
            motorsel = 600;
            cmdstate = 0x0a;
            break;
            case 0x0b:                              //GR open
            GPIO_PIN_SET(GMMA_ONOFF_PORT,GMMA_ONOFF_PIN,0);
            cmdstate = 0x0b;
            break;
            case 0x0c:                              //GR off
            GPIO_PIN_SET(GMMA_ONOFF_PORT,GMMA_ONOFF_PIN,1);
            cmdstate = 0x0c;
            break;
            default:
            cmdstate = 0xFF;
        }
    }
}
void ComSendBuf(unsigned char com,unsigned char *data,unsigned int len)
{
    unsigned int i,y;
    if(com == 0x01)
    {
        for( i=0;i<len;i++)
        {   
            U1TXREG = data[i];
            while(!U1STAbits.TRMT); 
        }
    }
    if(com == 0x02)
    {
        for( y=0;y<len;y++)
        {   
            U2TXREG = data[y];
            while(!U2STAbits.TRMT); 
        }
    }
}
void ComSendChar(unsigned char com,unsigned char data)
{
    if(com == 0x01)
    {
     
        U1TXREG = data;
        while(!U1STAbits.TRMT); 
     
    }
    if(com == 0x02)
    {
        U2TXREG = data;
        while(!U2STAbits.TRMT); 
    }
    
}
extern unsigned char DataUpBuf[35];
extern unsigned char reportflag;
extern unsigned char Collectflag;
void DataUpload(void)
{
    if(reportflag == 1)
    {
        DataUpBuf[0]  = Dataflag; 
        DataUpBuf[25] = cmdstate;
        DataUpBuf[33] = Checksum(DataUpBuf,33) + Datatail;
        DataUpBuf[34] = Datatail;

        ComSendBuf(2,DataUpBuf,35);
        //ComSendBuf(1,DataUpBuf,35);
        
        cmdstate = 0xff;
        reportflag = 0;
        Collectflag = 1;    //send complete allow start collect
    }
}
