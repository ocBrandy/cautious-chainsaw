#include "main.h"

extern unsigned char Updata_over;
extern unsigned char Upcmdbuf[30];
extern unsigned int Up_datalen;
extern unsigned char Tooldata_over;
extern unsigned char Tooldata[500];
extern unsigned char T2rxnum;
extern unsigned char headflag;

unsigned char Toolactive[2] = {0x10,0x00};
unsigned char Toolcmd[30];
unsigned char Updataload[500] = {0x66,0xCC};
unsigned char ToolID;                               //仪器ID
unsigned char Subsetnum;                            //Subset号
unsigned int Tooldatalen;                           //仪器需返回数据长度
unsigned int SamplePeriod;                          //数据采样率
unsigned int TimeOut;                               //允许数据超时时间
unsigned int Teleset;                               //null
unsigned int Desc;                                  //预留
unsigned char Cmdtype;
unsigned char Datatype;

void Up_Cmd_poll (void)
{
    unsigned char i;
    if(Updata_over == 1)
    {
        Cmdtype = Upcmdbuf[4];                      //井上命令类型：0x01为服务表类型，向井下仪器发送0x1000，并返回仪器数据//0x02为控制命令类型，向井下仪器发送控制命令
        
        ToolID = Upcmdbuf[5];

        Subsetnum = Upcmdbuf[6];

        Tooldatalen = Upcmdbuf[7];
        Tooldatalen = (Tooldatalen<<8) + Upcmdbuf[8];

        SamplePeriod = Upcmdbuf[9];
        SamplePeriod = (SamplePeriod<<8) + Upcmdbuf[10];

        TimeOut = Upcmdbuf[11];
        TimeOut = (TimeOut<<8) + Upcmdbuf[12];

        Teleset = Upcmdbuf[13];
        Teleset = (Teleset<<8) + Upcmdbuf[14];

        Desc = Upcmdbuf[15];
        Desc = (Desc<<8) + Upcmdbuf[16];

        ComSendBuf(COM1,Toolactive,2);
        Datatype = 0x01;
        
        if(Cmdtype == 0x02)
        {
            //Delay_ms(80);
            for(i=0;i<Up_datalen-1;i++)
            {
                Toolcmd[i] = Upcmdbuf[i+5];
            }
            ComSendBuf(COM1,Toolcmd,Up_datalen - 1);
            Datatype = 0x02;                            //井上指令为仪器控制命令时，成功给仪器下发控制命令时回复井上0x02
        }
        
            Updata_over = 0;
            T2rxnum = 0;
            headflag = 0;
    }
}

extern unsigned char T1rxnum;
void Tool_data_poll(void)
{
    unsigned char i,j;
    
    if(Tooldata_over == 1)
    {
        Updataload[2] = 0x00;
        Updataload[3] = Tooldatalen + 1;
        Updataload[4] = Datatype;                   
        
        for(i=0;i<Tooldatalen - 8;i++)               //Tooldatalen-8:井下仪器数据=井上下发数据长度-通讯板补充的数据长度
        {
            Updataload[i+5] = Tooldata[i];
        }
        for(j=0;j<8;j++)
        {
            Updataload[75+j] = 0x00;                 //通讯板需补充的数据
        }
        Updataload[83] = Checksum(Updataload,Tooldatalen + 5)>>8;
        Updataload[84] = Checksum(Updataload,Tooldatalen + 5);
        ComSendBuf(COM2,Updataload,Tooldatalen + 7);
        
        T1rxnum = 0;
        Tooldata_over = 0;
    }
}


