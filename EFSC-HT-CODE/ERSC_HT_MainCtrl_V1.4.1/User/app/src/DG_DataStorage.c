/*
*********************************************************************************************************
*
*	模块名称 : 一体化采集数据存储
*	文件名称 : DG_DataStorage.c
*	版    本 : V1.0
*	说    明 : 通过Fatfs保存断轨数据，以TXT格式存储
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0   2020-10-11  XMDNDL   首版 
*
*	Copyright (C), 2020-2022, XMDNDL
*
*********************************************************************************************************
*/
#define CONFIGMSG 93
#include "bsp.h"
#include "ff.h"			/* FatFS文件系统模块*/
#include "DG_DataStorage.h"
#include  "LogDebugSwitch.h"
// #include "hw_config.h" 
// #include "usb_lib.h"
// #include "usb_pwr.h"
#define SD_BUF_SIZE				(4*1024)	
/* 定义数据缓存区 */
uint8_t g_ucTempAndStress[426] = {0};                                             //数组全局变量赋初值   
uint8_t g_ucConfigMsg[CONFIGMSG] = {0};
uint8_t g_LcdTxBuf[100] = {0};
uint8_t row_buf;
/* 平年的每月天数表 */
const uint8_t mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};   //const 值固定不变

/* 日期 */
static SYS_CLOCK_T s_tDate;
DISBUF_T g_tDisDate; //声明结构体变量
static uint8_t CmdRxState = 0;
uint8_t g_ucTimeOut2 = 0; 
uint8_t g_TxBuf[46] = {0};
//uint8_t  g_BUF[44]={0};
uint8_t g_CmdRxOver = 0;
/* FatFs API的返回值 */
static const char * FR_Table[]= 
{
	"FR_OK：成功",				                             /* (0) Succeeded */
	"FR_DISK_ERR：底层硬件错误",			                 /* (1) A hard error occurred in the low level disk I/O layer */
	"FR_INT_ERR：断言失败",				                     /* (2) Assertion failed */
	"FR_NOT_READY：物理驱动没有工作",			             /* (3) The physical drive cannot work */
	"FR_NO_FILE：文件不存在",				                 /* (4) Could not find the file */
	"FR_NO_PATH：路径不存在",				                 /* (5) Could not find the path */
	"FR_INVALID_NAME：无效文件名",		                     /* (6) The path name format is invalid */
	"FR_DENIED：由于禁止访问或者目录已满访问被拒绝",         /* (7) Access denied due to prohibited access or directory full */
	"FR_EXIST：文件已经存在",			                     /* (8) Access denied due to prohibited access */
	"FR_INVALID_OBJECT：文件或者目录对象无效",		         /* (9) The file/directory object is invalid */
	"FR_WRITE_PROTECTED：物理驱动被写保护",		             /* (10) The physical drive is write protected */
	"FR_INVALID_DRIVE：逻辑驱动号无效",		                 /* (11) The logical drive number is invalid */
	"FR_NOT_ENABLED：卷中无工作区",			                 /* (12) The volume has no work area */
	"FR_NO_FILESYSTEM：没有有效的FAT卷",		             /* (13) There is no valid FAT volume */
	"FR_MKFS_ABORTED：由于参数错误f_mkfs()被终止",	         /* (14) The f_mkfs() aborted due to any parameter error */
	"FR_TIMEOUT：在规定的时间内无法获得访问卷的许可",		 /* (15) Could not get a grant to access the volume within defined period */
	"FR_LOCKED：由于文件共享策略操作被拒绝",				 /* (16) The operation is rejected according to the file sharing policy */
	"FR_NOT_ENOUGH_CORE：无法分配长文件名工作区",		     /* (17) LFN working buffer could not be allocated */
	"FR_TOO_MANY_OPEN_FILES：当前打开的文件数大于_FS_SHARE", /* (18) Number of open files > _FS_SHARE */
	"FR_INVALID_PARAMETER：参数无效"	                     /* (19) Given parameter is invalid */
};
void ReadSysClock(SYS_CLOCK_T *_clock);
void DisPlay(DIS_TYPE_E _type);
static int32_t BC95_StrToHex(char *_pStr, uint8_t _ucLen);
void *memset(void *s, int ch, size_t n);
uint32_t pre_DEP= 0;
uint8_t add_buf[9][2]=
                    {
	                 0x10,0x00,
                     0x10,0x06,
	                 0x10,0x16,
	                 0x10,0x24,
	                 0x10,0x32,
	                 0x10,0x38,
	                 0x10,0x39,
	                 0x10,0x40,
					 0x10,0x90
                    };	
				

/*
*****************************************************************************
 * 函  数：CheckSum
 * 功  能：计算校验和
 * 参  数：_buf:需要计算的数据buf
 *		   _len：计算的数据长度
 * 返回值：校验和的低位字节
*****************************************************************************
*/
static uint16_t CheckSum(uint8_t *_buf, uint16_t _len)
{
	uint16_t sum = 0;
	uint16_t i;
	 
	for(i = 0 ; i<_len ; i++)
		{
			sum += _buf[i];
		}
	
	return sum;
}
/*
*****************************************************************************
 * 函数:DataUpload
 * 功能:上传数据至上位机
 * 参数:_buf:数据缓存区
 * _size:数据长度
 * 返回值:无
*****************************************************************************
*/
void DataPoll(void)
{
	uint8_t ucData;
	uint16_t ucRxCount = 0; 
	uint8_t overflag = 0;
	uint8_t buf[44] = {0};                  //数据存储426，不满足255
//    uint8_t uclen;
	uint8_t i;
	
	while(1)
	{
	  /* 轮询是否有数据上传，超时 20ms无数据则退出 */
		if(comGetChar(COM2,&ucData))
		{
		    g_ucTimeOut2 = 0;
		    bsp_StopTimer(1);			
			switch(CmdRxState)
			{
				/* 校验帧头1 */
				case 0:				
					if(ucData == 0x2F)
						{
							CmdRxState++;		/* 跳转进入下一步 */
						}
			
				break;
				
				/* 校验帧头2 */
				case 1:				
					if(ucData == 0x2F)
					    {
							CmdRxState++;		/* 跳转进入下一步 */
						}
					break;
				case 2:  
					    
          		 		buf[ucRxCount++] = ucData;    /* 存储剩余数据*/
//				          INSERT(LogDebug)(TIMER2_DEBUG,("\r\n TIME run \r\n"));
					         
					    break;
	             /* 可能还有数据，继续识别一次，确保无数据了 */
			}
			 continue; 
		}		
		else if(g_ucTimeOut2 == 0)
		{
		   bsp_StartTimer(1,3);   //等待3ms若还没有数据，标志位置1,10/19200=(一字节)
		   g_ucTimeOut2 = 1;
		}
  
		/* 连续3ms没有收到数据，退出循环 */
		if(bsp_CheckTimer(1))
		{
		   g_ucTimeOut2 = 0;
		   CmdRxState = 0;	
		   break;
		}		  
		/* 没有任何数据 */
		if(g_ucTimeOut2 == 0 && ucRxCount == 0)
		{
			break;
		}
		
		
	}
	//数据解析//
	if(ucRxCount >= 44)    //接受完一帧数据
	{	
		      INSERT(LogDebug)(Rx_data_DEBUG,("\r\n %x %x %x %x %x %x %x \r\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6])); //打印数组
					INSERT(LogDebug)(Rx_data_DEBUG,("\r\n %x %x %x %x %x %x %x \r\n",buf[7],buf[8],buf[9],buf[10],buf[11],buf[12],buf[13])); //打印数组
				  INSERT(LogDebug)(Rx_data_DEBUG,("\r\n %x %x %x %x %x %x %x \r\n",buf[14],buf[15],buf[16],buf[17],buf[18],buf[19],buf[20])); //打印数组
				  INSERT(LogDebug)(Rx_data_DEBUG,("\r\n %x %x %x %x %x %x %x \r\n",buf[21],buf[22],buf[23],buf[24],buf[25],buf[26],buf[27])); //打印数组
			   	INSERT(LogDebug)(Rx_data_DEBUG,("\r\n %x %x %x %x %x %x %x \r\n",buf[28],buf[29],buf[30],buf[31],buf[32],buf[33],buf[34])); //打印数组
			   	INSERT(LogDebug)(Rx_data_DEBUG,("\r\n %x %x %x %x %x %x %x \r\n",buf[35],buf[36],buf[37],buf[38],buf[39],buf[40],buf[41])); //打印数组
				  INSERT(LogDebug)(Rx_data_DEBUG,("\r\n %x %x \r\n",buf[42],buf[43])); //打印数组
		      INSERT(LogDebug)(RxData_over_DEBUG,("\r\n RD over ! \r\n"));
//		comSendBuf(COM2,buf,44);
		g_tDisDate.Mode.d = ((uint16_t)buf[1] << 8)|(uint16_t)buf[0];
	    g_tDisDate.DEP.a  = ((int32_t)buf[5] << 24) | ((int32_t)buf[4] << 16)|((int32_t)buf[3] << 8)|((int32_t)buf[2]);	
		g_tDisDate.DEP.a /=100;
		if(pre_DEP > g_tDisDate.DEP.a )
		{
			row_buf = 0x01;                        //  向上
			pre_DEP = g_tDisDate.DEP.a; 
		}	
		else if(pre_DEP < g_tDisDate.DEP.a )
        {
		    row_buf = 0x02;                          //向下
			pre_DEP = g_tDisDate.DEP.a;
		}
		else
        {
		    row_buf = 0x00;             //隐藏
			pre_DEP = g_tDisDate.DEP.a;
		}
		g_tDisDate.UNDEP.a = ((int32_t)buf[9] << 24) | ((int32_t)buf[8] << 16)|((int32_t)buf[7] << 8)|((int32_t)buf[6]);
		g_tDisDate.SPD.c = ((int16_t)buf[11] << 8)|((int16_t)buf[10]);
		g_tDisDate.TEN.c = ((int16_t)buf[13] << 8)|((int16_t)buf[12]);
		g_tDisDate.CHT.c = ((int16_t)buf[15] << 8)|((int16_t)buf[14]);
		for(i = 0;i < 14;i++)
		   {
		     g_tDisDate.RES.c = buf[i+16];
		   }
		g_tDisDate.CHV.d = ((uint16_t)buf[31] << 8)|((uint16_t)buf[30]);
		g_tDisDate.DOWNRATE.d = ((uint16_t)buf[33] << 8)|((uint16_t)buf[32]);
		g_tDisDate.UPRATE.d = ((uint16_t)buf[35] << 8)|((uint16_t)buf[34]);
		g_tDisDate.UPLOSE.d = ((uint16_t)buf[37] << 8)|((uint16_t)buf[36]);
		g_tDisDate.DOWNLOSE.d = ((uint16_t)buf[39] << 8)|((uint16_t)buf[38]);
		g_tDisDate.UPUTI.d = ((uint16_t)buf[41] << 8)|((uint16_t)buf[40]);
		g_tDisDate.MODEM.d = ((uint16_t)buf[43] << 8)|((uint16_t)buf[42]);
		overflag = 1; 
	}
	if(overflag == 1)
	{
		
		DisPlay(DEP); 
		DisPlay(SPD);
		DisPlay(TEN);
		DisPlay(CHT);
		DisPlay(CHV);
		DisPlay(MODEM);
		DisPlay(UPRATE);
		DisPlay(DOWNRATE);
		DisPlay(ROW);
		overflag = 0;
		INSERT(LogDebug)(RxData_display_DEBUG,("\r\n RD display ! \r\n"));
	}	
}

/*
*****************************************************************************
 * 函数:DisPlay
 * 功能:上传数据至屏幕
 * 参数:_size:类型
 * 返回值:无
*****************************************************************************
*/
void DisPlay(DIS_TYPE_E _type)                        //枚举函数，一般和switch连用
{
	uint8_t uclen = 0;
	g_LcdTxBuf[uclen++] = 0x5A;
	g_LcdTxBuf[uclen++] = 0xA5;
	g_LcdTxBuf[uclen++] = 0x00;                       //最后根据接收数据位判断数据长度
	g_LcdTxBuf[uclen++] = 0x82;                       //写指令
	switch(_type)
	{
		/* 校正深度 */
		case DEP:
			g_LcdTxBuf[uclen++] = add_buf[0][0];      //数据只能一位一位的，如果多位可以利用for循环
			g_LcdTxBuf[uclen++] = add_buf[0][1];
			g_LcdTxBuf[uclen++] = g_tDisDate.DEP.b[3];
			g_LcdTxBuf[uclen++] = g_tDisDate.DEP.b[2];
			g_LcdTxBuf[uclen++] = g_tDisDate.DEP.b[1];
			g_LcdTxBuf[uclen++] = g_tDisDate.DEP.b[0];

			break;
		/* 速度 */
		case SPD:
			g_LcdTxBuf[uclen++] = add_buf[1][0];      
			g_LcdTxBuf[uclen++] = add_buf[1][1];
			g_LcdTxBuf[uclen++] = g_tDisDate.SPD.e[1];
			g_LcdTxBuf[uclen++] = g_tDisDate.SPD.e[0];
	
			break;
		/* 张力 */
		case TEN:
			g_LcdTxBuf[uclen++] = add_buf[2][0];     
			g_LcdTxBuf[uclen++] = add_buf[2][1];
			g_LcdTxBuf[uclen++] = g_tDisDate.TEN.e[1];
			g_LcdTxBuf[uclen++] = g_tDisDate.TEN.e[0];

			break;	
	    
		case CHT:
			g_LcdTxBuf[uclen++] = add_buf[3][0];     
			g_LcdTxBuf[uclen++] = add_buf[3][1];
			g_LcdTxBuf[uclen++] = g_tDisDate.CHT.e[1];
			g_LcdTxBuf[uclen++] = g_tDisDate.CHT.e[0];
		
			break;	
	    
		case CHV:
			g_LcdTxBuf[uclen++] = add_buf[4][0];      
			g_LcdTxBuf[uclen++] = add_buf[4][1];
			g_LcdTxBuf[uclen++] = g_tDisDate.CHV.e[1];
			g_LcdTxBuf[uclen++] = g_tDisDate.CHV.e[0];
		
			break;	
	    /* Modem状态 */
		case MODEM:
			g_LcdTxBuf[uclen++] = add_buf[5][0];      
			g_LcdTxBuf[uclen++] = add_buf[5][1];
			g_LcdTxBuf[uclen++] = g_tDisDate.MODEM.e[1];
			g_LcdTxBuf[uclen++] = g_tDisDate.MODEM.e[0]; 
	
		    break;
		/* 上行速度 */
		case UPRATE:
			g_LcdTxBuf[uclen++] = add_buf[6][0];      
			g_LcdTxBuf[uclen++] = add_buf[6][1];
			g_LcdTxBuf[uclen++] = g_tDisDate.UPRATE.e[1];
			g_LcdTxBuf[uclen++] = g_tDisDate.UPRATE.e[0];
		
			break;	
	   /* 下行速度 */
		case DOWNRATE:
			g_LcdTxBuf[uclen++] = add_buf[7][0];      
			g_LcdTxBuf[uclen++] = add_buf[7][1];
			g_LcdTxBuf[uclen++] = g_tDisDate.DOWNRATE.e[1];
			g_LcdTxBuf[uclen++] = g_tDisDate.DOWNRATE.e[0];
	
			break;	
		/* 箭头向上/向下*/
		case ROW:
			g_LcdTxBuf[uclen++] = add_buf[8][0];      
			g_LcdTxBuf[uclen++] = add_buf[8][1];
			g_LcdTxBuf[uclen++] = 0x00;
			g_LcdTxBuf[uclen++] = row_buf;   //00隐藏01向上02向下
			break;	
		
		 default:  
			      break;
	
	}
	     g_LcdTxBuf[2] = uclen - 3;                 //数据长度=写指令+地址+数据
	     comSendBuf(COM5,g_LcdTxBuf,uclen);

}

/*
*********************************************************************************************************
*	函 数 名: GetTimeStamp
*	功能说明: 获取时间戳
*	形    参：无
*	返 回 值: 时间戳计数值
*********************************************************************************************************
*/
static uint32_t GetTimeStamp(void)
{
	uint32_t temp;	
	temp = ((uint32_t)g_ucTempAndStress[3] << 24) | ((uint32_t)g_ucTempAndStress[4] << 16)|((uint32_t)g_ucTempAndStress[5] << 8)|((uint32_t)g_ucTempAndStress[6]);	
	return temp;
}

/*
*********************************************************************************************************
*	函 数 名: Is_Leap_Year
*	功能说明: 判断是否为闰年
*	形    参：无
*	返 回 值: 1,是.0,不是
*********************************************************************************************************
*/
static uint8_t Is_Leap_Year(uint16_t _year)
{                     
	if (_year % 4 == 0) /* 必须能被4整除 */
	{ 
		if (_year % 100 == 0) 
		{ 
			if (_year % 400 == 0)
			{
				return 1;	/* 如果以00结尾,还要能被400整除 */
			}
			else 
			{
				return 0;   
			}

		}
		else 
		{
			return 1;   
		}
	}
	else 
	{
		return 0; 
	}
}  

/*
*********************************************************************************************************
*	函 数 名: ReadSysClock
*	功能说明: 根据时间戳计算当前日期
*	形    参：_clock：保存年月日的缓存区指针
*	返 回 值: 无
*********************************************************************************************************
*/
void ReadSysClock(SYS_CLOCK_T *_clock)
{
	static uint16_t daycnt = 0;
	uint32_t uiTimeCount = 0; 
	uint32_t temp = 0;
	uint16_t temp1 = 0;

	uiTimeCount = GetTimeStamp();     

	temp = uiTimeCount / 86400;   /* 得到天数 */

	if (daycnt != temp)	/* 超过一天了 */
	{       
		daycnt = temp;
		temp1 = 1970;  /* 从1970年开始 */

		while (temp >= 365)
		{                          
			if (Is_Leap_Year(temp1))	/* 是闰年 */
			{
				if (temp >= 366)
				{
					temp -= 366;		/* 闰年的秒钟数 */
				}
				else
				{
					//temp1++;		/* armfly: 这里闰年处理错误，不能加1 */
					break;
				}  
            }
			else 
			{
				temp -= 365;       /* 平年 */
			}
			temp1++;  
		}   
		_clock->Year = temp1;	/* 得到年份 */
		
		temp1 = 0;
		while (temp >= 28)	/* 超过了一个月 */
		{
			if(Is_Leap_Year(_clock->Year) && temp1 == 1)	/* 当年是不是闰年/2月份 */
			{
				if (temp >= 29)
				{
					temp -= 29;	/* 闰年的秒钟数 */
				}
				else
				{
					break; 
				}
            }
            else 
			{
				if (temp >= mon_table[temp1])
				{
					temp -= mon_table[temp1];	/* 平年 */
				}
				else 
				{
					break;
				}
			}
			temp1++;  
		}
		_clock->Mon = temp1 + 1;	/* 得到月份 */
		_clock->Day = temp + 1;  	/* 得到日期 */
	}
}

/*
*********************************************************************************************************
*	函 数 名: SaveTempAndStress
*	功能说明: 保存单点频谱数据
*	形    参：无
*	返 回 值: 0：保存成功		1：保存失败
*********************************************************************************************************
*/
uint8_t SaveTempAndStress(void)
{
/* 本函数使用的局部变量占用较多，请修改启动文件，保证堆栈空间够用 */
	FRESULT result;
	FATFS fs;
	FIL file;
	DIR DirInf;
	uint32_t bw;
//	uint32_t i;
	char buf[100] = {0};
	char str[3000] = {0};
//	for (i = 0; i < sizeof(g_TestBuf_SD); i++)
//	{
//		g_TestBuf_SD[i] = (i/512) + '0';
//	}

 	/* 挂载文件系统 */
	result = f_mount(&fs, FS_VOLUME_SD, 0);			/* 挂载逻辑驱动器 */
	if (result != FR_OK)
	{
		printf("挂载文件系统失败 (%s)\r\n", FR_Table[result]);
	}

	/* 打开根文件夹 */
	result = f_opendir(&DirInf, "0:/Temp"); /* 如果不带参数，则从当前目录开始 */
	if (result != FR_OK)
	{
		printf("打开根目录失败 (%s)\r\n", FR_Table[result]);
		return 1;
	}

//	printf("Year=%d,Mon=%d,Day=%d",s_tDate.Year,s_tDate.Mon,s_tDate.Day);
	sprintf((char*)buf,"0:/Temp/%d-%d-%d.txt",s_tDate.Year,s_tDate.Mon,s_tDate.Day);
	sprintf((char*)str, "0x%02x,", g_ucTempAndStress[0]);
	for(uint16_t i = 1; i < sizeof(g_ucTempAndStress); i++)  
	{
		sprintf((char*)&str[strlen((char*)str)],"0x%02x,",g_ucTempAndStress[i] );  
    }
	printf("len=%d\r\n",strlen((char*)str));
	/* 打开文件 */
	result = f_open(&file, buf, FA_CREATE_ALWAYS | FA_WRITE);

	/* 写一串数据 */
	result = f_write(&file, str, strlen((char*)str), &bw);
//	result = f_write(&file, g_TestBuf_SD, sizeof(g_TestBuf_SD), &bw);
	if (result == FR_OK)
	{
		printf("TempAndStress.txt 文件写入成功,文件大小为(%dB)\r\n",strlen((char*)str));
	}
	else
	{
		printf("TempAndStress.txt 文件写入失败(%s)\r\n",FR_Table[result]);
	}

	/* 关闭文件*/
	f_close(&file);

	/* 卸载文件系统 */
	f_mount(NULL, FS_VOLUME_SD, 0);
}

/*
*********************************************************************************************************
*	函 数 名: SaveConfigMsg
*	功能说明: 保存配置信息
*	形    参：无
*	返 回 值: 0：保存成功		1：保存失败
*********************************************************************************************************
*/
//uint8_t SaveConfigMsg(void)
//{
///* 本函数使用的局部变量占用较多，请修改启动文件，保证堆栈空间够用 */
//	FRESULT result;
//	FATFS fs;
//	FIL file;
//	DIR DirInf;
//	uint32_t bw;
////	uint32_t i;
//	char str[3000]={0};
////	
////	for (i = 0; i < sizeof(g_TestBuf_SD); i++)
////	{
////		g_TestBuf_SD[i] = (i/512) + '0';
////	}

// 	/* 挂载文件系统 */
//	result = f_mount(&fs, FS_VOLUME_SD, 0);			/* Mount a logical drive */
//	if (result != FR_OK)
//	{
//		printf("挂载文件系统失败 (%s)\r\n", FR_Table[result]);
//	}

//	/* 打开根文件夹 */
//	result = f_opendir(&DirInf, "0:/Conf"); /* 如果不带参数，则从当前目录开始 */
//	if (result != FR_OK)
//	{
//		printf("打开根目录失败 (%s)\r\n", FR_Table[result]);
//		return 1;
//	}
//	sprintf((char*)str, "%02x", g_ucConfigMsg[0]);
//	for(uint16_t i = 1; i < sizeof(g_ucConfigMsg); i++)  
//    {
//	sprintf((char*)&str[strlen((char*)str)],"%02x",g_ucConfigMsg[i] );  
//    }
//	/*打开文件 */
//	result = f_open(&file, "0:/Conf/Conf.txt", FA_CREATE_ALWAYS | FA_WRITE);

//	/* 写一串数据 */
//	result = f_write(&file, str, strlen((char*)str), &bw);
////	result = f_write(&file, g_TestBuf_SD, sizeof(g_TestBuf_SD), &bw);
//	if (result == FR_OK)
//	{
//		printf("ConfigMsg.txt 文件写入成功,文件大小为(%dB)\r\n",sizeof(g_ucConfigMsg));
//	}
//	else
//	{
//		printf("ConfigMsg.txt 文件写入失败(%s)\r\n",FR_Table[result]);
//	}

//	/* 关闭文件*/
//	f_close(&file);

//	/* 卸载文件系统 */
//	f_mount(NULL, FS_VOLUME_SD, 0);	
//	
//}

/*
*********************************************************************************************************
*	函 数 名: SendConfigMsg
*	功能说明: 读取配置信息，并通过串口发送至主控板
*	形    参：无
*	返 回 值: 0：成功	1：失败
*********************************************************************************************************
*/
uint8_t	SendConfigMsg(void)
{
    /* 本函数使用的局部变量占用较多，请修改启动文件，保证堆栈空间够用 */
	FRESULT result;
	FATFS fs;
	FIL file;
	DIR DirInf;
	uint32_t bw;
	char buf[SD_BUF_SIZE];
	char str[1000]={0};
	char *q;   //指针指向buf
	uint8_t temp=0;
	uint8_t ucData;
 	/* 挂载文件系统 */
	result = f_mount(&fs, FS_VOLUME_SD, 0);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("挂载文件系统失败(%s)\r\n", FR_Table[result]);
	}

	/* 打开根文件夹 */
	result = f_opendir(&DirInf, "0:/Conf"); /* 如果不带参数，则从当前目录开始 */
	if (result != FR_OK)
	{
		printf("打开根目录失败(%s)\r\n", FR_Table[result]);
		return 1;
	}
	/* 打开文件 */
	result = f_open(&file, "0:/Conf/Conf.txt", FA_OPEN_EXISTING | FA_READ);
	if (result !=  FR_OK)
	{
		printf("Don't Find File : Conf.txt\r\n");
		return 1;
	}
    
	/* 读取文件 */
	result = f_read(&file, &buf, sizeof(buf) - 1, &bw);    //将file的存到buf
	if (bw > 0)
	{
		buf[bw] = 0;
		printf("\r\nConf.txt 文件内容 : \r\n%s\r\n", buf);
		q = buf;   //指向数据开头
     for(uint8_t i = 0; i<CONFIGMSG;i++)
     {
         memset(buf,0,sizeof(buf)+1); 
		 g_TxBuf[i] = BC95_StrToHex(q,2);               //两个两个字符开始转换
      q += 2;
    
	 }			
		 	
						
	}
	else
	{
		printf("\r\nConf.txt 文件内容 : \r\n");
	}

	/* 关闭文件*/
	f_close(&file);

	/* 卸载文件系统 */
	f_mount(NULL, FS_VOLUME_SD, 0);
}



/*
*********************************************************************************************************
* 函数名: DecToIntFix
* 功能说明:将十进制转换成十六进制，给定长度
* 返回值: 0
*********************************************************************************************************
*/

//static int16_t DecToIntFix()

//{

//      int decimal_number, remainder, hexadecimal_number = 0;

//      int count;

//      printf("Enter a Decimal Number:\t");

//      scanf("%d", &decimal_number);

//      for(count = 0; decimal_number > 0; count++)

//      {

//            remainder = decimal_number % 16;

//            hexadecimal_number = hexadecimal_number + remainder * pow(10, count);

//            decimal_number = decimal_number / 16;

//      }

//      printf("\nHexadecimal Equivalent:\t%d\n", hexadecimal_number);

//      return 0;

//}
/*
*********************************************************************************************************
* 函数名: StrToIntFix
* 功能说明: 将ASCII码字符串转换成十六进制，给定长度
* 形  参: _pStr :待转换的ASCII码串. 可以以逗号或0结束
*    _ucLen : 固定长度
* 返回值: 二进制整数数值
*********************************************************************************************************
*/
static int32_t BC95_StrToHex(char *_pStr, uint8_t _ucLen)
{

 char *p;
 uint32_t ulInt;
 uint8_t i;
 uint8_t ucTemp;

 p = _pStr;

 ulInt = 0;
 for (i = 0; i < _ucLen; i++)
 {
  ucTemp = *p;

  if ((ucTemp >= '0') && (ucTemp <= '9'))
  {
   ulInt = ( ulInt << 4) + (ucTemp - '0');
   p++;
  }
  else if ((ucTemp >= 'A') && (ucTemp <= 'F'))
  {
   ulInt = ( ulInt <<4) + (ucTemp - 55);
   p++;
  }
  else if ((ucTemp >= 'a') && (ucTemp <= 'f'))
  {
   ulInt = (ulInt <<4) + (ucTemp - 87);
   p++;
  }
  else
  {
   break;
  }
 }

 return ulInt;
}

/***************************** XMDNDL (END OF FILE) *********************************/
