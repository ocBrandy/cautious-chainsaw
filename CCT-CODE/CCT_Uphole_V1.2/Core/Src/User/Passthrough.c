/*
 * Passthrough.c
 *
 *  Created on: Nov 14, 2024
 *      Author: 16194
 */
#include "main.h"

extern unsigned short Downdatatimeoutnum;
extern unsigned short isdowndcmdreport;

extern _Bool Downtoolctrlcmdback;
extern unsigned char Sertableactbuf[50];
extern unsigned char Contralcmdbuf[50];

unsigned char ToolID;							//ETH数据包体中的仪器地址
unsigned char Subsetnumber;						//subsetnumber数据包体中的subset号，与包头中不同
unsigned short Tool_Datalen;					//仪器返回数据长度，与数据包长度区别
unsigned short Sampleperiod = 0xFFFF;			//ETH下发的时间采样率，决定仪器以多少频率上传数据
unsigned short Delaytime;						//仪器数据超时时间，与ETH数据包包头中的Overtime区别
unsigned char Teleset;							//配置仪器是否采样等标志
unsigned char Data_reserve;						//ETH下发数据包中的预留位，与ETH数据包包头中的预留区别
unsigned char Sertableactflag = 0xff;
unsigned short Sertableactlen;
unsigned short Contralcmdlen;
unsigned char resendtpe;

_Bool TOOLCONTRALCMD = false;

void ETH_CMDPOLL(void)
{
	if(ETHdata.overflag == 1)
	{
		Resetstate(ETHCMD);

		switch(ETHdata.Workmode)
		{
			case SERVICETABLE:								//服务表激活
					ToolID = ETHdata.bodybuf[0];
					Subsetnumber = ETHdata.bodybuf[1];
					Tool_Datalen = (ETHdata.bodybuf[2] << 8) + ETHdata.bodybuf[3];
					Sampleperiod = (ETHdata.bodybuf[4] << 8) + ETHdata.bodybuf[5];
					Delaytime = (ETHdata.bodybuf[6] << 8) + ETHdata.bodybuf[7];
					Teleset = (ETHdata.bodybuf[8] << 8) + ETHdata.bodybuf[9];
					Data_reserve = (ETHdata.bodybuf[10] << 8) + ETHdata.bodybuf[11];		//各参数暂存

					PRINT_LOG("CMD is SERVICETABLE\n\r");
					Sertableactlen = ETHdata.datalen;

					for(unsigned char i=0;i<Sertableactlen;i++)
					{
						Sertableactbuf[i] = ETHdata.bodybuf[i];
					}

					Sertableactflag = 1;
					isdowndcmdreport = Sampleperiod;
					Downdatatimeoutnum = 0;
			break;
			case CONTRALCMD:								//控制命令

					Contralcmdlen = ETHdata.datalen;
					PRINT_LOG("CMD is TOOLCONTRALCMD\n\r");

					for(unsigned char i=0;i<Contralcmdlen;i++)
					{
						Contralcmdbuf[i] = ETHdata.bodybuf[i];
					}

					TOOLCONTRALCMD = true;
			break;
			case DEACTIVE:									//服务表反激活
					Sertableactflag = 0;
					PRINT_LOG("CMD is DEACTIVE\n\r");
					Sampleperiod = 0xFFFF;
			break;
		}
	}
	if(Sertableactflag == 1 && isdowndcmdreport >= Sampleperiod)		//服务表激活后 按照采样率向井下下发
	{
		if(TOOLCONTRALCMD)
		{
			Downcmdsend(TOOLCONTRALCMD);
			TOOLCONTRALCMD = false;
			resendtpe = 0;
			//PRINT_LOG("CMD is Toolcontralcmd\n\r");
		}
		else
		{
			if(Downtoolctrlcmdback == false && resendtpe < 3)			//控制命令下发，若井下无响应，重发，若三次无响应，舍弃该次控制命令。
			{
				Downcmdsend(TOOLCONTRALCMD);
				resendtpe++;
			}
			else
			Downcmdsend(SERVICETABLE);
			//PRINT_LOG("CMD is Servicetable\n\r");
		}
		isdowndcmdreport = 0;
	}
}

_Bool OVERTIME;
void DOWN_DATAPOLL(void)
{
	if(DOWNdata.overflag == 1 && Downdatatimeoutnum <= Delaytime)	//数据接收完毕且切在上位机允许的时间范围内
	{
		Downdatasend(DATABACK);
		Resetstate(TOOLDATA);
	}
	else if(Sertableactflag == 1 && Downdatatimeoutnum > Delaytime)	//无数据或数据接收完毕但超出上位机允许的时间范围
	{
		Downdatasend(TIMEOUT);
		Resetstate(TOOLDATA);
	}
}

extern unsigned char istimestampreport;
extern unsigned int Report_Timestamp;
unsigned char Timestampbuf[10];
void Sendtimestamp(void)
{
	if(istimestampreport >= 9)
	{
		Timestampbuf[1] = 0x47;
		Timestampbuf[2] = 0x47;
		Timestampbuf[2] = Report_Timestamp;
		Timestampbuf[3] = Report_Timestamp>>8;
		Timestampbuf[4] = Report_Timestamp>>16;
		Timestampbuf[5] = Report_Timestamp>>24;
		Timestampbuf[6] = 0xab;
		ComSendBuf(COM4,Timestampbuf,7);
		istimestampreport = 0;
	}
}

