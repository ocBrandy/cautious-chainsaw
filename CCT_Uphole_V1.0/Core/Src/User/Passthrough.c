/*
 * Passthrough.c
 *
 *  Created on: Nov 14, 2024
 *      Author: 16194
 */
#include "main.h"

extern unsigned char Workmode;
extern unsigned char ETHbodybuf[256];
extern unsigned char ETHdatabuf[256];
extern unsigned char ETHdatalen;
extern unsigned int isdowndcmdreport;
extern unsigned char ETHdataoverflag;

unsigned char ToolID;							//ETH数据包体中的仪器地址
unsigned char Subsetnumber;						//subsetnumber数据包体中的subset号，与包头中不同
unsigned int Tool_Datalen;						//仪器返回数据长度，与数据包长度区别
unsigned int Sampleperiod = 0xFFFF;				//ETH下发的时间采样率，决定仪器以多少频率上传数据
unsigned char Delaytime;						//仪器数据超时时间，与ETH数据包包头中的Overtime区别
unsigned char Teleset;							//配置仪器是否采样等标志
unsigned char Data_reserve;						//ETH下发数据包中的预留位，与ETH数据包包头中的预留区别
unsigned char Sertableactflag = 0xff;

void ETH_CMDPOLL(void)
{
	if(ETHdataoverflag == 1)
	{
		ToolID = ETHbodybuf[0];
		Subsetnumber = ETHbodybuf[1];
		Tool_Datalen = (ETHbodybuf[2] << 8) + ETHbodybuf[3];
		Sampleperiod = (ETHbodybuf[4] << 8) + ETHbodybuf[5];
		Delaytime = (ETHbodybuf[6] << 8) + ETHbodybuf[7];
		Teleset = (ETHbodybuf[8] << 8) + ETHbodybuf[9];
		Data_reserve = (ETHbodybuf[10] << 8) + ETHbodybuf[11];		//各参数暂存

		ETHdataoverflag = 0;
		switch(Workmode)
		{
			case SERVICETABLE:								//服务表激活
					Sertableactflag = 1;
			break;
			case CONTRALCMD:								//控制命令
					Downcmdsend(CONTRALCMD);
			break;
			case DEACTIVE:									//服务表反激活
					Sertableactflag = 0;
					Sampleperiod = 0xFFFF;
			break;
		}
	}
	if(Sertableactflag == 1 && isdowndcmdreport == Sampleperiod)		//服务表激活后 按照采样率向井下下发
	{
		Downcmdsend(SERVICETABLE);
		isdowndcmdreport = 0;
	}
}
extern unsigned char DOWNdataoverflag;
extern unsigned int Downdatatimeoutnum;
extern unsigned char DOWNdatabuf[256];
void DOWN_DATAPOLL(void)
{
	if(DOWNdataoverflag == 1 )//&& Downdatatimeoutnum <= Sampleperiod + Delaytime		//数据接收完毕且切在上位机允许的时间范围内
	{
		//Downdatasend(DATABACK);
		//Downdatatimeoutnum = 0;
		ComSendBuf(COM4,DOWNdatabuf,85);
		DOWNdataoverflag = 0;
	}
	else if(Downdatatimeoutnum > Sampleperiod + Delaytime)	//无数据或数据接收完毕但超出上位机允许的时间范围
	{
		//Downdatasend(TIMEOUT);
		//ComSendChar(COM4,0x55);
		//Downdatatimeoutnum = 0;
		//DOWNdataoverflag = 0;
	}
}

extern unsigned char istimestampreport;
extern unsigned long Report_Timestamp;
unsigned char Timestampbuf[10]={0x47,0x47};
void Sendtimestamp(void)
{
	if(istimestampreport == 9)
	{
		Timestampbuf[2] = Report_Timestamp;
		Timestampbuf[3] = Report_Timestamp>>8;
		Timestampbuf[4] = Report_Timestamp>>16;
		Timestampbuf[5] = Report_Timestamp>>24;
		Timestampbuf[6] = 0xab;
		ComSendBuf(COM4,Timestampbuf,7);
		istimestampreport = 0;
	}
}
