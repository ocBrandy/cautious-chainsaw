/*
*********************************************************************************************************
*
*	ģ������ : һ�廯�ɼ����ݴ洢
*	�ļ����� : DG_DataStorage.c
*	��    �� : V1.0
*	˵    �� : ͨ��Fatfs����Ϲ����ݣ���TXT��ʽ�洢
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0   2020-10-11  XMDNDL   �װ� 
*
*	Copyright (C), 2020-2022, XMDNDL
*
*********************************************************************************************************
*/
#define CONFIGMSG 93
#include "bsp.h"
#include "ff.h"			/* FatFS�ļ�ϵͳģ��*/
#include "DG_DataStorage.h"
#include  "LogDebugSwitch.h"
// #include "hw_config.h" 
// #include "usb_lib.h"
// #include "usb_pwr.h"
#define SD_BUF_SIZE				(4*1024)	
/* �������ݻ����� */
uint8_t g_ucTempAndStress[426] = {0};                                             //����ȫ�ֱ�������ֵ   
uint8_t g_ucConfigMsg[CONFIGMSG] = {0};
uint8_t g_LcdTxBuf[100] = {0};
uint8_t row_buf;
/* ƽ���ÿ�������� */
const uint8_t mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};   //const ֵ�̶�����

/* ���� */
static SYS_CLOCK_T s_tDate;
DISBUF_T g_tDisDate; //�����ṹ�����
static uint8_t CmdRxState = 0;
uint8_t g_ucTimeOut2 = 0; 
uint8_t g_TxBuf[46] = {0};
//uint8_t  g_BUF[44]={0};
uint8_t g_CmdRxOver = 0;
/* FatFs API�ķ���ֵ */
static const char * FR_Table[]= 
{
	"FR_OK���ɹ�",				                             /* (0) Succeeded */
	"FR_DISK_ERR���ײ�Ӳ������",			                 /* (1) A hard error occurred in the low level disk I/O layer */
	"FR_INT_ERR������ʧ��",				                     /* (2) Assertion failed */
	"FR_NOT_READY����������û�й���",			             /* (3) The physical drive cannot work */
	"FR_NO_FILE���ļ�������",				                 /* (4) Could not find the file */
	"FR_NO_PATH��·��������",				                 /* (5) Could not find the path */
	"FR_INVALID_NAME����Ч�ļ���",		                     /* (6) The path name format is invalid */
	"FR_DENIED�����ڽ�ֹ���ʻ���Ŀ¼�������ʱ��ܾ�",         /* (7) Access denied due to prohibited access or directory full */
	"FR_EXIST���ļ��Ѿ�����",			                     /* (8) Access denied due to prohibited access */
	"FR_INVALID_OBJECT���ļ�����Ŀ¼������Ч",		         /* (9) The file/directory object is invalid */
	"FR_WRITE_PROTECTED������������д����",		             /* (10) The physical drive is write protected */
	"FR_INVALID_DRIVE���߼���������Ч",		                 /* (11) The logical drive number is invalid */
	"FR_NOT_ENABLED�������޹�����",			                 /* (12) The volume has no work area */
	"FR_NO_FILESYSTEM��û����Ч��FAT��",		             /* (13) There is no valid FAT volume */
	"FR_MKFS_ABORTED�����ڲ�������f_mkfs()����ֹ",	         /* (14) The f_mkfs() aborted due to any parameter error */
	"FR_TIMEOUT���ڹ涨��ʱ�����޷���÷��ʾ�����",		 /* (15) Could not get a grant to access the volume within defined period */
	"FR_LOCKED�������ļ�������Բ������ܾ�",				 /* (16) The operation is rejected according to the file sharing policy */
	"FR_NOT_ENOUGH_CORE���޷����䳤�ļ���������",		     /* (17) LFN working buffer could not be allocated */
	"FR_TOO_MANY_OPEN_FILES����ǰ�򿪵��ļ�������_FS_SHARE", /* (18) Number of open files > _FS_SHARE */
	"FR_INVALID_PARAMETER��������Ч"	                     /* (19) Given parameter is invalid */
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
 * ��  ����CheckSum
 * ��  �ܣ�����У���
 * ��  ����_buf:��Ҫ���������buf
 *		   _len����������ݳ���
 * ����ֵ��У��͵ĵ�λ�ֽ�
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
 * ����:DataUpload
 * ����:�ϴ���������λ��
 * ����:_buf:���ݻ�����
 * _size:���ݳ���
 * ����ֵ:��
*****************************************************************************
*/
void DataPoll(void)
{
	uint8_t ucData;
	uint16_t ucRxCount = 0; 
	uint8_t overflag = 0;
	uint8_t buf[44] = {0};                  //���ݴ洢426��������255
//    uint8_t uclen;
	uint8_t i;
	
	while(1)
	{
	  /* ��ѯ�Ƿ��������ϴ�����ʱ 20ms���������˳� */
		if(comGetChar(COM2,&ucData))
		{
		    g_ucTimeOut2 = 0;
		    bsp_StopTimer(1);			
			switch(CmdRxState)
			{
				/* У��֡ͷ1 */
				case 0:				
					if(ucData == 0x2F)
						{
							CmdRxState++;		/* ��ת������һ�� */
						}
			
				break;
				
				/* У��֡ͷ2 */
				case 1:				
					if(ucData == 0x2F)
					    {
							CmdRxState++;		/* ��ת������һ�� */
						}
					break;
				case 2:  
					    
          		 		buf[ucRxCount++] = ucData;    /* �洢ʣ������*/
//				          INSERT(LogDebug)(TIMER2_DEBUG,("\r\n TIME run \r\n"));
					         
					    break;
	             /* ���ܻ������ݣ�����ʶ��һ�Σ�ȷ���������� */
			}
			 continue; 
		}		
		else if(g_ucTimeOut2 == 0)
		{
		   bsp_StartTimer(1,3);   //�ȴ�3ms����û�����ݣ���־λ��1,10/19200=(һ�ֽ�)
		   g_ucTimeOut2 = 1;
		}
  
		/* ����3msû���յ����ݣ��˳�ѭ�� */
		if(bsp_CheckTimer(1))
		{
		   g_ucTimeOut2 = 0;
		   CmdRxState = 0;	
		   break;
		}		  
		/* û���κ����� */
		if(g_ucTimeOut2 == 0 && ucRxCount == 0)
		{
			break;
		}
		
		
	}
	//���ݽ���//
	if(ucRxCount >= 44)    //������һ֡����
	{	
		      INSERT(LogDebug)(Rx_data_DEBUG,("\r\n %x %x %x %x %x %x %x \r\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6])); //��ӡ����
					INSERT(LogDebug)(Rx_data_DEBUG,("\r\n %x %x %x %x %x %x %x \r\n",buf[7],buf[8],buf[9],buf[10],buf[11],buf[12],buf[13])); //��ӡ����
				  INSERT(LogDebug)(Rx_data_DEBUG,("\r\n %x %x %x %x %x %x %x \r\n",buf[14],buf[15],buf[16],buf[17],buf[18],buf[19],buf[20])); //��ӡ����
				  INSERT(LogDebug)(Rx_data_DEBUG,("\r\n %x %x %x %x %x %x %x \r\n",buf[21],buf[22],buf[23],buf[24],buf[25],buf[26],buf[27])); //��ӡ����
			   	INSERT(LogDebug)(Rx_data_DEBUG,("\r\n %x %x %x %x %x %x %x \r\n",buf[28],buf[29],buf[30],buf[31],buf[32],buf[33],buf[34])); //��ӡ����
			   	INSERT(LogDebug)(Rx_data_DEBUG,("\r\n %x %x %x %x %x %x %x \r\n",buf[35],buf[36],buf[37],buf[38],buf[39],buf[40],buf[41])); //��ӡ����
				  INSERT(LogDebug)(Rx_data_DEBUG,("\r\n %x %x \r\n",buf[42],buf[43])); //��ӡ����
		      INSERT(LogDebug)(RxData_over_DEBUG,("\r\n RD over ! \r\n"));
//		comSendBuf(COM2,buf,44);
		g_tDisDate.Mode.d = ((uint16_t)buf[1] << 8)|(uint16_t)buf[0];
	    g_tDisDate.DEP.a  = ((int32_t)buf[5] << 24) | ((int32_t)buf[4] << 16)|((int32_t)buf[3] << 8)|((int32_t)buf[2]);	
		g_tDisDate.DEP.a /=100;
		if(pre_DEP > g_tDisDate.DEP.a )
		{
			row_buf = 0x01;                        //  ����
			pre_DEP = g_tDisDate.DEP.a; 
		}	
		else if(pre_DEP < g_tDisDate.DEP.a )
        {
		    row_buf = 0x02;                          //����
			pre_DEP = g_tDisDate.DEP.a;
		}
		else
        {
		    row_buf = 0x00;             //����
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
 * ����:DisPlay
 * ����:�ϴ���������Ļ
 * ����:_size:����
 * ����ֵ:��
*****************************************************************************
*/
void DisPlay(DIS_TYPE_E _type)                        //ö�ٺ�����һ���switch����
{
	uint8_t uclen = 0;
	g_LcdTxBuf[uclen++] = 0x5A;
	g_LcdTxBuf[uclen++] = 0xA5;
	g_LcdTxBuf[uclen++] = 0x00;                       //�����ݽ�������λ�ж����ݳ���
	g_LcdTxBuf[uclen++] = 0x82;                       //дָ��
	switch(_type)
	{
		/* У����� */
		case DEP:
			g_LcdTxBuf[uclen++] = add_buf[0][0];      //����ֻ��һλһλ�ģ������λ��������forѭ��
			g_LcdTxBuf[uclen++] = add_buf[0][1];
			g_LcdTxBuf[uclen++] = g_tDisDate.DEP.b[3];
			g_LcdTxBuf[uclen++] = g_tDisDate.DEP.b[2];
			g_LcdTxBuf[uclen++] = g_tDisDate.DEP.b[1];
			g_LcdTxBuf[uclen++] = g_tDisDate.DEP.b[0];

			break;
		/* �ٶ� */
		case SPD:
			g_LcdTxBuf[uclen++] = add_buf[1][0];      
			g_LcdTxBuf[uclen++] = add_buf[1][1];
			g_LcdTxBuf[uclen++] = g_tDisDate.SPD.e[1];
			g_LcdTxBuf[uclen++] = g_tDisDate.SPD.e[0];
	
			break;
		/* ���� */
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
	    /* Modem״̬ */
		case MODEM:
			g_LcdTxBuf[uclen++] = add_buf[5][0];      
			g_LcdTxBuf[uclen++] = add_buf[5][1];
			g_LcdTxBuf[uclen++] = g_tDisDate.MODEM.e[1];
			g_LcdTxBuf[uclen++] = g_tDisDate.MODEM.e[0]; 
	
		    break;
		/* �����ٶ� */
		case UPRATE:
			g_LcdTxBuf[uclen++] = add_buf[6][0];      
			g_LcdTxBuf[uclen++] = add_buf[6][1];
			g_LcdTxBuf[uclen++] = g_tDisDate.UPRATE.e[1];
			g_LcdTxBuf[uclen++] = g_tDisDate.UPRATE.e[0];
		
			break;	
	   /* �����ٶ� */
		case DOWNRATE:
			g_LcdTxBuf[uclen++] = add_buf[7][0];      
			g_LcdTxBuf[uclen++] = add_buf[7][1];
			g_LcdTxBuf[uclen++] = g_tDisDate.DOWNRATE.e[1];
			g_LcdTxBuf[uclen++] = g_tDisDate.DOWNRATE.e[0];
	
			break;	
		/* ��ͷ����/����*/
		case ROW:
			g_LcdTxBuf[uclen++] = add_buf[8][0];      
			g_LcdTxBuf[uclen++] = add_buf[8][1];
			g_LcdTxBuf[uclen++] = 0x00;
			g_LcdTxBuf[uclen++] = row_buf;   //00����01����02����
			break;	
		
		 default:  
			      break;
	
	}
	     g_LcdTxBuf[2] = uclen - 3;                 //���ݳ���=дָ��+��ַ+����
	     comSendBuf(COM5,g_LcdTxBuf,uclen);

}

/*
*********************************************************************************************************
*	�� �� ��: GetTimeStamp
*	����˵��: ��ȡʱ���
*	��    �Σ���
*	�� �� ֵ: ʱ�������ֵ
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
*	�� �� ��: Is_Leap_Year
*	����˵��: �ж��Ƿ�Ϊ����
*	��    �Σ���
*	�� �� ֵ: 1,��.0,����
*********************************************************************************************************
*/
static uint8_t Is_Leap_Year(uint16_t _year)
{                     
	if (_year % 4 == 0) /* �����ܱ�4���� */
	{ 
		if (_year % 100 == 0) 
		{ 
			if (_year % 400 == 0)
			{
				return 1;	/* �����00��β,��Ҫ�ܱ�400���� */
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
*	�� �� ��: ReadSysClock
*	����˵��: ����ʱ������㵱ǰ����
*	��    �Σ�_clock�����������յĻ�����ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ReadSysClock(SYS_CLOCK_T *_clock)
{
	static uint16_t daycnt = 0;
	uint32_t uiTimeCount = 0; 
	uint32_t temp = 0;
	uint16_t temp1 = 0;

	uiTimeCount = GetTimeStamp();     

	temp = uiTimeCount / 86400;   /* �õ����� */

	if (daycnt != temp)	/* ����һ���� */
	{       
		daycnt = temp;
		temp1 = 1970;  /* ��1970�꿪ʼ */

		while (temp >= 365)
		{                          
			if (Is_Leap_Year(temp1))	/* ������ */
			{
				if (temp >= 366)
				{
					temp -= 366;		/* ����������� */
				}
				else
				{
					//temp1++;		/* armfly: �������괦����󣬲��ܼ�1 */
					break;
				}  
            }
			else 
			{
				temp -= 365;       /* ƽ�� */
			}
			temp1++;  
		}   
		_clock->Year = temp1;	/* �õ���� */
		
		temp1 = 0;
		while (temp >= 28)	/* ������һ���� */
		{
			if(Is_Leap_Year(_clock->Year) && temp1 == 1)	/* �����ǲ�������/2�·� */
			{
				if (temp >= 29)
				{
					temp -= 29;	/* ����������� */
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
					temp -= mon_table[temp1];	/* ƽ�� */
				}
				else 
				{
					break;
				}
			}
			temp1++;  
		}
		_clock->Mon = temp1 + 1;	/* �õ��·� */
		_clock->Day = temp + 1;  	/* �õ����� */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: SaveTempAndStress
*	����˵��: ���浥��Ƶ������
*	��    �Σ���
*	�� �� ֵ: 0������ɹ�		1������ʧ��
*********************************************************************************************************
*/
uint8_t SaveTempAndStress(void)
{
/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
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

 	/* �����ļ�ϵͳ */
	result = f_mount(&fs, FS_VOLUME_SD, 0);			/* �����߼������� */
	if (result != FR_OK)
	{
		printf("�����ļ�ϵͳʧ�� (%s)\r\n", FR_Table[result]);
	}

	/* �򿪸��ļ��� */
	result = f_opendir(&DirInf, "0:/Temp"); /* ���������������ӵ�ǰĿ¼��ʼ */
	if (result != FR_OK)
	{
		printf("�򿪸�Ŀ¼ʧ�� (%s)\r\n", FR_Table[result]);
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
	/* ���ļ� */
	result = f_open(&file, buf, FA_CREATE_ALWAYS | FA_WRITE);

	/* дһ������ */
	result = f_write(&file, str, strlen((char*)str), &bw);
//	result = f_write(&file, g_TestBuf_SD, sizeof(g_TestBuf_SD), &bw);
	if (result == FR_OK)
	{
		printf("TempAndStress.txt �ļ�д��ɹ�,�ļ���СΪ(%dB)\r\n",strlen((char*)str));
	}
	else
	{
		printf("TempAndStress.txt �ļ�д��ʧ��(%s)\r\n",FR_Table[result]);
	}

	/* �ر��ļ�*/
	f_close(&file);

	/* ж���ļ�ϵͳ */
	f_mount(NULL, FS_VOLUME_SD, 0);
}

/*
*********************************************************************************************************
*	�� �� ��: SaveConfigMsg
*	����˵��: ����������Ϣ
*	��    �Σ���
*	�� �� ֵ: 0������ɹ�		1������ʧ��
*********************************************************************************************************
*/
//uint8_t SaveConfigMsg(void)
//{
///* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
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

// 	/* �����ļ�ϵͳ */
//	result = f_mount(&fs, FS_VOLUME_SD, 0);			/* Mount a logical drive */
//	if (result != FR_OK)
//	{
//		printf("�����ļ�ϵͳʧ�� (%s)\r\n", FR_Table[result]);
//	}

//	/* �򿪸��ļ��� */
//	result = f_opendir(&DirInf, "0:/Conf"); /* ���������������ӵ�ǰĿ¼��ʼ */
//	if (result != FR_OK)
//	{
//		printf("�򿪸�Ŀ¼ʧ�� (%s)\r\n", FR_Table[result]);
//		return 1;
//	}
//	sprintf((char*)str, "%02x", g_ucConfigMsg[0]);
//	for(uint16_t i = 1; i < sizeof(g_ucConfigMsg); i++)  
//    {
//	sprintf((char*)&str[strlen((char*)str)],"%02x",g_ucConfigMsg[i] );  
//    }
//	/*���ļ� */
//	result = f_open(&file, "0:/Conf/Conf.txt", FA_CREATE_ALWAYS | FA_WRITE);

//	/* дһ������ */
//	result = f_write(&file, str, strlen((char*)str), &bw);
////	result = f_write(&file, g_TestBuf_SD, sizeof(g_TestBuf_SD), &bw);
//	if (result == FR_OK)
//	{
//		printf("ConfigMsg.txt �ļ�д��ɹ�,�ļ���СΪ(%dB)\r\n",sizeof(g_ucConfigMsg));
//	}
//	else
//	{
//		printf("ConfigMsg.txt �ļ�д��ʧ��(%s)\r\n",FR_Table[result]);
//	}

//	/* �ر��ļ�*/
//	f_close(&file);

//	/* ж���ļ�ϵͳ */
//	f_mount(NULL, FS_VOLUME_SD, 0);	
//	
//}

/*
*********************************************************************************************************
*	�� �� ��: SendConfigMsg
*	����˵��: ��ȡ������Ϣ����ͨ�����ڷ��������ذ�
*	��    �Σ���
*	�� �� ֵ: 0���ɹ�	1��ʧ��
*********************************************************************************************************
*/
uint8_t	SendConfigMsg(void)
{
    /* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	FATFS fs;
	FIL file;
	DIR DirInf;
	uint32_t bw;
	char buf[SD_BUF_SIZE];
	char str[1000]={0};
	char *q;   //ָ��ָ��buf
	uint8_t temp=0;
	uint8_t ucData;
 	/* �����ļ�ϵͳ */
	result = f_mount(&fs, FS_VOLUME_SD, 0);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("�����ļ�ϵͳʧ��(%s)\r\n", FR_Table[result]);
	}

	/* �򿪸��ļ��� */
	result = f_opendir(&DirInf, "0:/Conf"); /* ���������������ӵ�ǰĿ¼��ʼ */
	if (result != FR_OK)
	{
		printf("�򿪸�Ŀ¼ʧ��(%s)\r\n", FR_Table[result]);
		return 1;
	}
	/* ���ļ� */
	result = f_open(&file, "0:/Conf/Conf.txt", FA_OPEN_EXISTING | FA_READ);
	if (result !=  FR_OK)
	{
		printf("Don't Find File : Conf.txt\r\n");
		return 1;
	}
    
	/* ��ȡ�ļ� */
	result = f_read(&file, &buf, sizeof(buf) - 1, &bw);    //��file�Ĵ浽buf
	if (bw > 0)
	{
		buf[bw] = 0;
		printf("\r\nConf.txt �ļ����� : \r\n%s\r\n", buf);
		q = buf;   //ָ�����ݿ�ͷ
     for(uint8_t i = 0; i<CONFIGMSG;i++)
     {
         memset(buf,0,sizeof(buf)+1); 
		 g_TxBuf[i] = BC95_StrToHex(q,2);               //���������ַ���ʼת��
      q += 2;
    
	 }			
		 	
						
	}
	else
	{
		printf("\r\nConf.txt �ļ����� : \r\n");
	}

	/* �ر��ļ�*/
	f_close(&file);

	/* ж���ļ�ϵͳ */
	f_mount(NULL, FS_VOLUME_SD, 0);
}



/*
*********************************************************************************************************
* ������: DecToIntFix
* ����˵��:��ʮ����ת����ʮ�����ƣ���������
* ����ֵ: 0
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
* ������: StrToIntFix
* ����˵��: ��ASCII���ַ���ת����ʮ�����ƣ���������
* ��  ��: _pStr :��ת����ASCII�봮. �����Զ��Ż�0����
*    _ucLen : �̶�����
* ����ֵ: ������������ֵ
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
