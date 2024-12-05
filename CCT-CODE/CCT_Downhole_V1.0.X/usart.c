#include "main.h"


void UsartInit(void)
{
	U1MODE = 0x8000;                //UARTEN=1,启用UART1模块;
    U1STA = 0x8000;                 //UTXISEL=1,UTXEN=0,URXISEL0=0,RXISEL1=0;
    U1BRG = 0x0007;                 //BAUD RATE 125000,(BAUD RATE=4M/(16(BRG+1));
    _U1TXIE =0;						
	_U1RXIE =1;						//Enable UART1 Receive
	U1STAbits.UTXEN = 1;
    
    
    U2MODE = 0x8000;                //UARTEN=1,启用UART2模块;
    U2STA = 0x8000;                 //UTXISEL=1,UTXEN=0,URXISEL0=0,RXISEL1=0;
    U2BRG = 0x0007;                 //BAUD RATE 125000,(BAUD RATE=4M/(16(BRG+1));
    _U2TXIE =0;						
	_U2RXIE =1;						//Enable UART2 Receive
	U2STAbits.UTXEN = 1;	
}

unsigned char Upcmdbuf[30];
unsigned char Tooldata[500];
unsigned char T2rxnum = 0;
unsigned int T1rxnum = 0;
unsigned char headflag;
unsigned int Up_cmd_checksum;
unsigned int Up_datalen;
unsigned char Updata_over;
unsigned char Tooldata_over;
unsigned int DD;

extern unsigned int Tooldatalen;
void __attribute__ ((__interrupt__, no_auto_psv)) _U2RXInterrupt (void) 
{
    _U2RXIF = 0;
    Upcmdbuf[T2rxnum]  = U2RXREG;
//////        U1TXREG = U2RXREG;
//////        while(!U1STAbits.TRMT);  
//        U1TXREG = U2RXREG;
//        while(!U1STAbits.TRMT);  
    
    if(headflag == 0 && Upcmdbuf[T2rxnum-1] == 0x55 && Upcmdbuf[T2rxnum] == 0xAA)
    {
        headflag = 1;
        Upcmdbuf[0] = Upcmdbuf[T2rxnum-1];
        Upcmdbuf[1] = Upcmdbuf[T2rxnum];
        T2rxnum = 1; 
    }
    T2rxnum++;
    Up_datalen = (Upcmdbuf[2]<<8) + Upcmdbuf[3]; 
    if(T2rxnum == Up_datalen + 6)
    {
        Up_cmd_checksum = (Upcmdbuf[Up_datalen + 4]<<8) + Upcmdbuf[Up_datalen + 5];
        
        if(Up_cmd_checksum == Checksum(Upcmdbuf,Up_datalen+4))
        {
            Updata_over = 1;
        }
    }
    
    if(T2rxnum > 253)
        T2rxnum = 0;
}
void __attribute__ ((__interrupt__, no_auto_psv)) _U1RXInterrupt (void) 
{
    _U1RXIF = 0;
    Tooldata[T1rxnum] = U1RXREG;
    T1rxnum++;
    
    if(T1rxnum == Tooldatalen - 8)           //Tooldatalen - 8：井上下发数据长度-通讯板补充的数据长度 = 井下仪器返回的数据长度
    {
        Tooldata_over = 1;
    }
    
    if(T1rxnum > 599)                       //串口接收错误超时
        T1rxnum = 0;
}

void ComSendChar (unsigned char com,unsigned char data)
{
    if (com == 0xee)
    {
        U1TXREG = data;
        while(!U1STAbits.TRMT);   
    }
    else if(com == 0xdd)
    {
         U2TXREG = data;
        while(!U2STAbits.TRMT);
    }
}

void ComSendBuf (unsigned char com,unsigned char *data,unsigned int len )
{
    unsigned char i;
    if(com == 0xee)
    {
        for(i=0;i<len;i++)
        {
            U1TXREG = data[i];
            while(!U1STAbits.TRMT);
        }
        
    }
    else if(com == 0xdd)
    {
        for(i=0;i<len;i++)
        {
            U2TXREG = data[i];
            while(!U2STAbits.TRMT);
        }
        
    }
        
}









