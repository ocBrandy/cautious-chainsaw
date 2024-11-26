/*
*********************************************************************************************************
*
*	ģ������ : SD��Fat�ļ�ϵͳ��ʾģ�顣
*	�ļ����� : demo_sdio_fatfs.c
*	��    �� : V1.0
*	˵    �� : ��������ֲFatFS�ļ�ϵͳ���汾 R0.09b������ʾ��δ����ļ�����ȡ�ļ�������Ŀ¼��ɾ���ļ�
*			���������ļ���д�ٶȡ�
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-02-01 armfly  ��ʽ����
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "ff.h"			/* FatFS�ļ�ϵͳģ��*/
#include "demo_sdio_fatfs.h"
#include "hw_config.h" 
#include "usb_lib.h"
#include "usb_pwr.h"

/* ���ڲ��Զ�д�ٶ� */
#define SDTEST_FILE_LEN			(2*1024*1024)	/* ���ڲ��Ե��ļ����� */
#define SD_BUF_SIZE				(4*1024)		/* ÿ�ζ�дSD����������ݳ��� */
uint8_t g_TestBuf_SD[SD_BUF_SIZE];

/* �������ļ��ڵ��õĺ������� */
static void SD_DispMenu(void);
static void SD_ViewRootDir(void);
static void SD_CreateNewFile(void);
static void SD_ReadFileData(void);
static void SD_CreateDir(void);
static void SD_DeleteDirFile(void);
static void SD_WriteFileTest(void);
static void GetFreeSize(void);

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

/*
*********************************************************************************************************
*	�� �� ��: DemoFatFS
*	����˵��: FatFS�ļ�ϵͳ��ʾ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DemoFatFS_SDcard(void)
{
	uint8_t cmd;

	/* ��ӡ�����б��û�����ͨ�����ڲ���ָ�� */
	SD_DispMenu();
	while(1)
	{
		bsp_Idle();		/* ���������bsp.c�ļ����û������޸��������ʵ��CPU���ߺ�ι�� */

		if (comGetChar(COM1, &cmd))	/* �Ӵ��ڶ���һ���ַ�(��������ʽ) */
		{
			printf("\r\n");
			switch (cmd)
			{				
				case '1':
					printf("��1 - ViewRootDir��\r\n");
					SD_ViewRootDir();		/* ��ʾSD����Ŀ¼�µ��ļ��� */
					break;

				case '2':
					printf("��2 - CreateNewFile��\r\n");
					SD_CreateNewFile();		/* ����һ�����ļ�,д��һ���ַ��� */
					break;

				case '3':
					printf("��3 - ReadFileData��\r\n");
					SD_ReadFileData();		/* ��ȡ��Ŀ¼��armfly.txt������ */
					break;

				case '4':
					printf("��4 - CreateDir��\r\n");
					SD_CreateDir();		/* ����Ŀ¼ */
					break;

				case '5':
					printf("��5 - DeleteDirFile��\r\n");
					SD_DeleteDirFile();	/* ɾ��Ŀ¼���ļ� */
					break;

				case '6':
					printf("��6 - TestSpeed��\r\n");
					SD_WriteFileTest();	/* �ٶȲ��� */
					break;
				
				case '7':
					printf("��7 - ʹ��U�̡�\r\n");
					USB_Cable_Config(ENABLE);
					break;
				
				case '8':
					printf("��8 - ж��U�̡�\r\n");
					USB_Cable_Config(DISABLE);	
					break;
				case 'a':
					printf("��a - GetFreeSize��\r\n");
					GetFreeSize();		/* ��ȡ��������ʣ������*/
					break;
				
				case 'r':
					goto Exti_while;	/* �˳�fatfs���� */

				default:
					SD_DispMenu();
					break;
			}
		}

		/* �����˲��ͼ���ɺ�̨systick�жϷ������ʵ�֣�����ֻ��Ҫ����bsp_GetKey��ȡ��ֵ���ɡ� */
		switch (bsp_GetKey())	/* bsp_GetKey()��ȡ��ֵ, �޼�����ʱ���� KEY_NONE = 0 */
		{
			case KEY_DOWN_K1:			/* K1������ */
				break;

			case KEY_UP_K1:				/* K1������ */
				break;

			case KEY_DOWN_K2:			/* K2������ */
				break;

			case KEY_UP_K2:				/* K2������ */
				break;

			case KEY_NONE:				/* �޼����� */
			default:
				/* �����ļ�ֵ������ */
				break;
		}
	}
Exti_while:
		printf("�˳�fatfs����\r\n");
}

/*
*********************************************************************************************************
*	�� �� ��: DispMenu
*	����˵��: ��ʾ������ʾ�˵�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void SD_DispMenu(void)
{
	printf("\r\n------------------------------------------------\r\n");
	printf("��ѡ���������:\r\n");
	printf("1 - ��ʾSD����Ŀ¼�µ��ļ��б�\r\n");
	printf("2 - ����һ�����ļ�armfly.txt\r\n");
	printf("3 - ��armfly.txt�ļ�������\r\n");
	printf("4 - ����Ŀ¼\r\n");
	printf("5 - ɾ���ļ���Ŀ¼\r\n");
	printf("6 - ��д�ļ��ٶȲ���\r\n");
	printf("7 - ʹ��U��\r\n");
	printf("8 - ж��U��\r\n");
	printf("a - ��ȡ���̵���������ʣ������\r\n");
	printf("r - �˳�fatfs����\r\n");
}

/*
*********************************************************************************************************
*	�� �� ��: ViewRootDir
*	����˵��: ��ʾSD����Ŀ¼�µ��ļ���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void SD_ViewRootDir(void)
{
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	FATFS fs;
	DIR DirInf;
	FILINFO FileInf;
	uint32_t cnt = 0;
//	char lfname[256];

 	/* �����ļ�ϵͳ */
	result = f_mount(&fs, FS_VOLUME_SD, 0);	/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("�����ļ�ϵͳʧ�� (%s)\r\n", FR_Table[result]);
	}

	/* �򿪸��ļ��� */
	result = f_opendir(&DirInf, "0:/"); /* ���������������ӵ�ǰĿ¼��ʼ */
	if (result != FR_OK)
	{
		printf("�򿪸�Ŀ¼ʧ�� (%s)\r\n", FR_Table[result]);
		return;
	}

	/* ��ȡ��ǰ�ļ����µ��ļ���Ŀ¼ */
//	FileInf.lfname = lfname;
//	FileInf.lfsize = 256;

	printf("���� | �ļ���С | ���ļ��� | ���ļ���\r\n");
	for (cnt = 0; ;cnt++)
	{
		result = f_readdir(&DirInf,&FileInf); 		/* ��ȡĿ¼��������Զ����� */
		if (result != FR_OK || FileInf.fname[0] == 0)
		{
			break;
		}

		if (FileInf.fname[0] == '.')
		{
			continue;
		}

		/* �ж����ļ�������Ŀ¼ */
		if (FileInf.fattrib & AM_DIR)
		{
			printf("(0x%02d)Ŀ¼  ", FileInf.fattrib);
		}
		else
		{
			printf("(0x%02d)�ļ�  ", FileInf.fattrib);
		}

		/* ��ӡ�ļ���С, ���4G */
		printf("%10d", FileInf.fsize);

		printf(" %s|\r\n", FileInf.fname);	/* ���ļ��� */

//		printf("%s\r\n", (char *)FileInf.lfname);	/* ���ļ��� */
	}

	/* ж���ļ�ϵͳ */
	f_mount(NULL, FS_VOLUME_SD, 0);
}

/*
*********************************************************************************************************
*	�� �� ��: CreateNewFile
*	����˵��: ��SD������һ�����ļ����ļ�������д��www.armfly.com��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void SD_CreateNewFile(void)
{
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	FATFS fs;
	FIL file;
	DIR DirInf;
	uint32_t bw;
	uint32_t i;
	
	for (i = 0; i < sizeof(g_TestBuf_SD); i++)
	{
		g_TestBuf_SD[i] = (i/512) + '0';
	}

 	/* �����ļ�ϵͳ */
	result = f_mount(&fs, FS_VOLUME_SD, 0);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("�����ļ�ϵͳʧ�� (%s)\r\n", FR_Table[result]);
	}

	/* �򿪸��ļ��� */
	result = f_opendir(&DirInf, "0:/"); /* ���������������ӵ�ǰĿ¼��ʼ */
	if (result != FR_OK)
	{
		printf("�򿪸�Ŀ¼ʧ�� (%s)\r\n", FR_Table[result]);
		return;
	}

	/* ���ļ� */
	result = f_open(&file, "0:armfly.txt", FA_CREATE_ALWAYS | FA_WRITE);

	/* дһ������ */
	result = f_write(&file, "FatFS Write Demo \r\n www.armfly.com \r\n", 34, &bw);
//	result = f_write(&file, g_TestBuf_SD, sizeof(g_TestBuf_SD), &bw);
	if (result == FR_OK)
	{
		printf("armfly.txt �ļ�д��ɹ�,�ļ���СΪ(%dB)\r\n",sizeof(g_TestBuf_SD));
	}
	else
	{
		printf("armfly.txt �ļ�д��ʧ��(%s)\r\n",FR_Table[result]);
	}

	/* �ر��ļ�*/
	f_close(&file);

	/* ж���ļ�ϵͳ */
	f_mount(NULL, FS_VOLUME_SD, 0);
}

/*
*********************************************************************************************************
*	�� �� ��: ReadFileData
*	����˵��: ��ȡ�ļ�armfly.txtǰ128���ַ�������ӡ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void SD_ReadFileData(void)
{
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	FATFS fs;
	FIL file;
	DIR DirInf;
	uint32_t bw;
	char buf[SD_BUF_SIZE];

 	/* �����ļ�ϵͳ */
	result = f_mount(&fs, FS_VOLUME_SD, 0);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("�����ļ�ϵͳʧ��(%s)\r\n", FR_Table[result]);
	}

	/* �򿪸��ļ��� */
	result = f_opendir(&DirInf, "0:/"); /* ���������������ӵ�ǰĿ¼��ʼ */
	if (result != FR_OK)
	{
		printf("�򿪸�Ŀ¼ʧ��(%s)\r\n", FR_Table[result]);
		return;
	}

	/* ���ļ� */
	result = f_open(&file, "0:armfly.txt", FA_OPEN_EXISTING | FA_READ);
	if (result !=  FR_OK)
	{
		printf("Don't Find File : armfly.txt\r\n");
		return;
	}

	/* ��ȡ�ļ� */
	result = f_read(&file, &buf, sizeof(buf) - 1, &bw);
	if (bw > 0)
	{
		buf[bw] = 0;
		printf("\r\narmfly.txt �ļ����� : \r\n%s\r\n", buf);
	}
	else
	{
		printf("\r\narmfly.txt �ļ����� : \r\n");
	}

	/* �ر��ļ�*/
	f_close(&file);

	/* ж���ļ�ϵͳ */
	f_mount(NULL, FS_VOLUME_SD, 0);
}

/*
*********************************************************************************************************
*	�� �� ��: CreateDir
*	����˵��: ��SD����Ŀ¼����Dir1��Dir2Ŀ¼����Dir1Ŀ¼�´�����Ŀ¼Dir1_1
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void SD_CreateDir(void)
{
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	FATFS fs;

 	/* �����ļ�ϵͳ */
	result = f_mount(&fs, FS_VOLUME_SD, 0);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("�����ļ�ϵͳʧ�� (%s)\r\n", FR_Table[result]);
	}

	/* ����Ŀ¼/Dir1 */
	result = f_mkdir("0:/Dir1");
	if (result == FR_OK)
	{
		printf("f_mkdir Dir1 Ok\r\n");
	}
	else if (result == FR_EXIST)
	{
		printf("Dir1 Ŀ¼�Ѿ�����(%s)\r\n", FR_Table[result]);
	}
	else
	{
		printf("f_mkdir Dir1 ʧ�� (%s)\r\n", FR_Table[result]);
		return;
	}

	/* ����Ŀ¼/Dir2 */
	result = f_mkdir("0:/Dir2");
	if (result == FR_OK)
	{
		printf("f_mkdir Dir2 Ok\r\n");
	}
	else if (result == FR_EXIST)
	{
		printf("Dir2 Ŀ¼�Ѿ�����(%s)\r\n", FR_Table[result]);
	}
	else
	{
		printf("f_mkdir Dir2 ʧ�� (%s)\r\n", FR_Table[result]);
		return;
	}

	/* ������Ŀ¼ /Dir1/Dir1_1	   ע�⣺������Ŀ¼Dir1_1ʱ�������ȴ�����Dir1 */
	result = f_mkdir("0:/Dir1/Dir1_1"); /* */
	if (result == FR_OK)
	{
		printf("f_mkdir Dir1_1 �ɹ�\r\n");
	}
	else if (result == FR_EXIST)
	{
		printf("Dir1_1 Ŀ¼�Ѿ����� (%s)\r\n", FR_Table[result]);
	}
	else
	{
		printf("f_mkdir Dir1_1 ʧ�� (%s)\r\n", FR_Table[result]);
		return;
	}

	/* ж���ļ�ϵͳ */
	f_mount(NULL, FS_VOLUME_SD, 0);
}

/*
*********************************************************************************************************
*	�� �� ��: DeleteDirFile
*	����˵��: ɾ��SD����Ŀ¼�µ� armfly.txt �ļ��� Dir1��Dir2 Ŀ¼
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void SD_DeleteDirFile(void)
{
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	FATFS fs;
	char FileName[13];
	uint8_t i;

 	/* �����ļ�ϵͳ */
	result = f_mount(&fs, FS_VOLUME_SD, 0);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("�����ļ�ϵͳʧ�� (%s)\r\n", FR_Table[result]);
	}

	#if 0
	/* �򿪸��ļ��� */
	result = f_opendir(&DirInf, "/"); /* ���������������ӵ�ǰĿ¼��ʼ */
	if (result != FR_OK)
	{
		printf("�򿪸�Ŀ¼ʧ��(%d)\r\n", result);
		return;
	}
	#endif

	/* ɾ��Ŀ¼/Dir1 ����Ϊ������Ŀ¼�ǿգ�������Ŀ¼)���������ɾ����ʧ�ܡ�*/
	result = f_unlink("0:/Dir1");
	if (result == FR_OK)
	{
		printf("ɾ��Ŀ¼Dir1�ɹ�\r\n");
	}
	else if (result == FR_NO_FILE)
	{
		printf("û�з����ļ���Ŀ¼ :%s\r\n", "/Dir1");
	}
	else
	{
		printf("ɾ��Dir1ʧ��(������� = %s) �ļ�ֻ����Ŀ¼�ǿ�\r\n", FR_Table[result]);
	}

	/* ��ɾ��Ŀ¼/Dir1/Dir1_1 */
	result = f_unlink("0:/Dir1/Dir1_1");
	if (result == FR_OK)
	{
		printf("ɾ����Ŀ¼/Dir1/Dir1_1�ɹ�\r\n");
	}
	else if ((result == FR_NO_FILE) || (result == FR_NO_PATH))
	{
		printf("û�з����ļ���Ŀ¼ :%s\r\n", "/Dir1/Dir1_1");
	}
	else
	{
		printf("ɾ����Ŀ¼/Dir1/Dir1_1ʧ��(������� = %s) �ļ�ֻ����Ŀ¼�ǿ�\r\n", FR_Table[result]);
	}

	/* ��ɾ��Ŀ¼/Dir1 */
	result = f_unlink("0:/Dir1");
	if (result == FR_OK)
	{
		printf("ɾ��Ŀ¼Dir1�ɹ�\r\n");
	}
	else if (result == FR_NO_FILE)
	{
		printf("û�з����ļ���Ŀ¼ :%s\r\n", "/Dir1");
	}
	else
	{
		printf("ɾ��Dir1ʧ��(������� = %s) �ļ�ֻ����Ŀ¼�ǿ�\r\n", FR_Table[result]);
	}

	/* ɾ��Ŀ¼/Dir2 */
	result = f_unlink("0:/Dir2");
	if (result == FR_OK)
	{
		printf("ɾ��Ŀ¼ Dir2 �ɹ�\r\n");
	}
	else if (result == FR_NO_FILE)
	{
		printf("û�з����ļ���Ŀ¼ :%s\r\n", "/Dir2");
	}
	else
	{
		printf("ɾ��Dir2 ʧ��(������� = %s) �ļ�ֻ����Ŀ¼�ǿ�\r\n", FR_Table[result]);
	}
	

	/* ɾ���ļ� armfly.txt */
	result = f_unlink("0:armfly.txt");
	if (result == FR_OK)
	{
		printf("ɾ���ļ� armfly.txt �ɹ�\r\n");
	}
	else if (result == FR_NO_FILE)
	{
		printf("û�з����ļ���Ŀ¼ :%s\r\n", "armfly.txt");
	}
	else
	{
		printf("ɾ��armfly.txtʧ��(������� = %s) �ļ�ֻ����Ŀ¼�ǿ�\r\n", FR_Table[result]);
	}
	
	/* ɾ���ļ� Test2.txt */
	result = f_unlink("0:Test2.txt");
	if (result == FR_OK)
	{
		printf("ɾ���ļ� Test2.txt �ɹ�\r\n");
	}
	else if (result == FR_NO_FILE)
	{
		printf("û�з����ļ���Ŀ¼ :%s\r\n", "Test2.txt");
	}
	else
	{
		printf("ɾ��Test2.txtʧ��(������� = %s) �ļ�ֻ����Ŀ¼�ǿ�\r\n", FR_Table[result]);
	}

	/* ɾ���ļ� speed1.txt */
	for (i = 0; i < 10; i++)
	{
		sprintf(FileName, "0:Speed%02d.txt", i);		/* ÿд1�Σ���ŵ��� */
		result = f_unlink(FileName);
		if (result == FR_OK)
		{
			printf("ɾ���ļ�%s�ɹ�\r\n", FileName);
		}
		else if (result == FR_NO_FILE)
		{
			printf("û�з����ļ�:%s\r\n", FileName);
		}
		else
		{
			printf("ɾ��%s�ļ�ʧ��(������� = %s) �ļ�ֻ����Ŀ¼�ǿ�\r\n", FileName, FR_Table[result]);
		}
	}

	/* ж���ļ�ϵͳ */
	f_mount(NULL, FS_VOLUME_SD, 0);
}

/*
*********************************************************************************************************
*	�� �� ��: WriteFileTest
*	����˵��: �����ļ���д�ٶ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void SD_WriteFileTest(void)
{
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	FATFS fs;
	FIL file;
	DIR DirInf;
	uint32_t bw;
	uint32_t i,k;
	uint32_t runtime1,runtime2,timelen;
	uint8_t err = 0;
	char TestFileName[20];
	static uint8_t s_ucTestSn = 0;

	for (i = 0; i < sizeof(g_TestBuf_SD); i++)
	{
		g_TestBuf_SD[i] = (i / 512) + '0';
	}

  	/* �����ļ�ϵͳ */
	result = f_mount(&fs, FS_VOLUME_SD, 0);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("�����ļ�ϵͳʧ�� (%s)\r\n", FR_Table[result]);
	}

	/* �򿪸��ļ��� */
	result = f_opendir(&DirInf, "0:/"); /* ���������������ӵ�ǰĿ¼��ʼ */
	if (result != FR_OK)
	{
		printf("�򿪸�Ŀ¼ʧ�� (%s)\r\n", FR_Table[result]);
		return;
	}

	/* ���ļ� */
	sprintf(TestFileName, "0:Speed%02d.txt", s_ucTestSn++);		/* ÿд1�Σ���ŵ��� */
	result = f_open(&file, TestFileName, FA_CREATE_ALWAYS | FA_WRITE);
	if (result == FR_OK)
	{
		printf("�ļ������ɹ�(%s)\r\n",FR_Table[result]);
	}
	else
	{
		printf("�ļ�����ʧ��(%s)\r\n",FR_Table[result]);
	}

	/* дһ������ */
	printf("��ʼд�ļ�%s %dKB ...\r\n", TestFileName, SDTEST_FILE_LEN / 1024);
	runtime1 = bsp_GetRunTime();	/* ��ȡϵͳ����ʱ�� */
	for (i = 0; i < SDTEST_FILE_LEN / SD_BUF_SIZE; i++)
	{
		result = f_write(&file, g_TestBuf_SD, sizeof(g_TestBuf_SD), &bw);
		if (result == FR_OK)
		{
			if (((i + 1) % 8) == 0)
			{
				printf(".");
			}
		}
		else
		{
			err = 1;
			printf("%s�ļ�дʧ��(%s)\r\n,", TestFileName,FR_Table[result]);
			printf("��ǰi = (%d)\r\n",i);
			break;
		}
	}
	runtime2 = bsp_GetRunTime();	/* ��ȡϵͳ����ʱ�� */

	if (err == 0)
	{
		timelen = (runtime2 - runtime1);
		printf("\r\n  д��ʱ : %dms   ƽ��д�ٶ� : %dB/S (%dKB/S)\r\n",
			timelen,
			(SDTEST_FILE_LEN * 1000) / timelen,
			((SDTEST_FILE_LEN / 1024) * 1000) / timelen);
	}

	f_close(&file);		/* �ر��ļ�*/


	/* ��ʼ���ļ����� */
	result = f_open(&file, TestFileName, FA_OPEN_EXISTING | FA_READ);
	if (result !=  FR_OK)
	{
		printf("û���ҵ��ļ�: %s (%s)\r\n", TestFileName,FR_Table[result]);
		return;
	}

	printf("��ʼ���ļ� %dKB ...\r\n", SDTEST_FILE_LEN / 1024);
	runtime1 = bsp_GetRunTime();	/* ��ȡϵͳ����ʱ�� */
	for (i = 0; i < SDTEST_FILE_LEN / SD_BUF_SIZE; i++)
	{
		result = f_read(&file, g_TestBuf_SD, sizeof(g_TestBuf_SD), &bw);
		if (result == FR_OK)
		{
			if (((i + 1) % 8) == 0)
			{
				printf(".");
			}

			/* �Ƚ�д��������Ƿ���ȷ�������ᵼ�¶����ٶȽ�����͵� 3.5MBytes/S */
			for (k = 0; k < sizeof(g_TestBuf_SD); k++)
			{
				if (g_TestBuf_SD[k] != (k / 512) + '0')
				{
				  	err = 1;
					printf("Speed1.txt �ļ����ɹ����������ݳ���\r\n");
					break;
				}
			}
			if (err == 1)
			{
				break;
			}
		}
		else
		{
			err = 1;
			printf("Speed1.txt �ļ���ʧ��\r\n");
			break;
		}
	}
	runtime2 = bsp_GetRunTime();	/* ��ȡϵͳ����ʱ�� */

	if (err == 0)
	{
		timelen = (runtime2 - runtime1);
		printf("\r\n  ����ʱ : %dms   ƽ�����ٶ� : %dB/S (%dKB/S)\r\n", timelen,
			(SDTEST_FILE_LEN * 1000) / timelen, ((SDTEST_FILE_LEN / 1024) * 1000) / timelen);
	}

	/* �ر��ļ�*/
	f_close(&file);

	/* ж���ļ�ϵͳ */
	f_mount(NULL, FS_VOLUME_SD, 0);
}

/*
*********************************************************************************************************
*	�� �� ��: GetFreeSize
*	����˵��: ��ȡ������С
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void GetFreeSize(void)
{
	FRESULT result;
	FATFS fs;
	DWORD fre_clust,fre_sect,tot_sect; 
	FATFS *p;
	
	p = &fs;
	/* �����ļ�ϵͳ */
	result = f_mount(&fs, FS_VOLUME_SD, 0);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("�����ļ�ϵͳʧ�� (%s)\r\n", FR_Table[result]);
	}
	
	result = f_getfree(FS_VOLUME_SD,&fre_clust,&p);
	if(result != FR_OK)
	{
		printf("��ȡ����ʧ�� (%s)\r\n", FR_Table[result]);
	}
	else
	{
		tot_sect =(p->n_fatent - 2) * p->csize/2;		/* ��ȡ�������� */
		fre_sect = fre_clust * p->csize/2;			/* ʣ���������� */
		
		/* ������С����512B�Ļ�����Ҫת����512B������ */
		#if _MAX_SS != 512                                           
			tot_sect *= p->ssize / 512;
			fre_sect *= p->ssize / 512;
		#endif 
		
		/* ת����KBΪ��λ */
//		tot_sect = tot_sect>>1;    	//��λKB
//        fre_sect = fre_sect>>1;   	//��λKB
		printf("tot_sect = %dKB\r\nfre_sect = %dKB\r\n",(uint32_t)tot_sect ,(uint32_t)fre_sect);
	}
	/* ж���ļ�ϵͳ */
	f_mount(NULL, FS_VOLUME_SD, 0);
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
