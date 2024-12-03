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
unsigned char ToolID;
unsigned char Subsetnum;
unsigned int Tooldatalen;
unsigned int SamplePeriod;
unsigned int TimeOut;
unsigned int Teleset;
unsigned int Desc;
unsigned char Cmdtype;
unsigned char Datatype;

void Up_Cmd_poll (void)
{
    unsigned char i;
    if(Updata_over == 1)
    {
        Cmdtype = Upcmdbuf[4];
        
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

        ComSendBuf(1,Toolactive,2);
        Datatype = 1;
        
        if(Cmdtype == 0x02)
        {
            Delay_ms(80);
            for(i=0;i<Up_datalen-1;i++)
            {
                Toolcmd[i] = Upcmdbuf[i+5];
            }
            ComSendBuf(1,Toolcmd,Up_datalen - 1);
            Datatype = 2;
        }
        
            Updata_over = 0;
            T2rxnum = 0;
            headflag = 0;
    }
}
extern unsigned char T1rxnum;
void Tool_data_poll(void)
{
    unsigned char i;
    
    if(Tooldata_over == 1)
    {
        Updataload[2] = 0x00;
        Updataload[3] = 0x4F;
        Updataload[4] = Datatype;
        
        for(i=0;i<Tooldatalen - 8;i++)               //调试为70 实际井上下发的Tooldatalen
        {
            Updataload[i+5] = Tooldata[i];
        }
        Updataload[75] = 0x00;                   //通讯板需补充的数据
        Updataload[76] = 0x00;
        Updataload[77] = 0x00;
        Updataload[78] = 0x00;
        Updataload[79] = 0x00;
        Updataload[80] = 0x00;
        Updataload[81] = 0x00;
        Updataload[82] = 0x00;
        Updataload[83] = Checksum(Updataload,83)>>8;
        Updataload[84] = Checksum(Updataload,83);
        ComSendBuf(2,Updataload,85);
        
        T1rxnum = 0;
        Tooldata_over = 0;
    }
}


