/*
*********************************************************************************************************
*
*	模块名称 : SD卡Fat文件系统演示模块。
*	文件名称 : demo_sdio_fatfs.c
*	版    本 : V1.0
*	说    明 : 该例程移植FatFS文件系统（版本 R0.09b），演示如何创建文件、读取文件、创建目录和删除文件
*			并测试了文件读写速度。
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2013-02-01 armfly  正式发布
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "ff.h"			/* FatFS文件系统模块*/
#include "demo_sdio_fatfs.h"
#include "hw_config.h" 
#include "usb_lib.h"
#include "usb_pwr.h"

/* 用于测试读写速度 */
#define SDTEST_FILE_LEN			(2*1024*1024)	/* 用于测试的文件长度 */
#define SD_BUF_SIZE				(4*1024)		/* 每次读写SD卡的最大数据长度 */
uint8_t g_TestBuf_SD[SD_BUF_SIZE];

/* 仅允许本文件内调用的函数声明 */
static void SD_DispMenu(void);
static void SD_ViewRootDir(void);
static void SD_CreateNewFile(void);
static void SD_ReadFileData(void);
static void SD_CreateDir(void);
static void SD_DeleteDirFile(void);
static void SD_WriteFileTest(void);
static void GetFreeSize(void);

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

/*
*********************************************************************************************************
*	函 数 名: DemoFatFS
*	功能说明: FatFS文件系统演示主程序
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DemoFatFS_SDcard(void)
{
	uint8_t cmd;

	/* 打印命令列表，用户可以通过串口操作指令 */
	SD_DispMenu();
	while(1)
	{
		bsp_Idle();		/* 这个函数在bsp.c文件。用户可以修改这个函数实现CPU休眠和喂狗 */

		if (comGetChar(COM1, &cmd))	/* 从串口读入一个字符(非阻塞方式) */
		{
			printf("\r\n");
			switch (cmd)
			{				
				case '1':
					printf("【1 - ViewRootDir】\r\n");
					SD_ViewRootDir();		/* 显示SD卡根目录下的文件名 */
					break;

				case '2':
					printf("【2 - CreateNewFile】\r\n");
					SD_CreateNewFile();		/* 创建一个新文件,写入一个字符串 */
					break;

				case '3':
					printf("【3 - ReadFileData】\r\n");
					SD_ReadFileData();		/* 读取根目录下armfly.txt的内容 */
					break;

				case '4':
					printf("【4 - CreateDir】\r\n");
					SD_CreateDir();		/* 创建目录 */
					break;

				case '5':
					printf("【5 - DeleteDirFile】\r\n");
					SD_DeleteDirFile();	/* 删除目录和文件 */
					break;

				case '6':
					printf("【6 - TestSpeed】\r\n");
					SD_WriteFileTest();	/* 速度测试 */
					break;
				
				case '7':
					printf("【7 - 使能U盘】\r\n");
					USB_Cable_Config(ENABLE);
					break;
				
				case '8':
					printf("【8 - 卸载U盘】\r\n");
					USB_Cable_Config(DISABLE);	
					break;
				case 'a':
					printf("【a - GetFreeSize】\r\n");
					GetFreeSize();		/* 获取总容量和剩余容量*/
					break;
				
				case 'r':
					goto Exti_while;	/* 退出fatfs测试 */

				default:
					SD_DispMenu();
					break;
			}
		}

		/* 按键滤波和检测由后台systick中断服务程序实现，我们只需要调用bsp_GetKey读取键值即可。 */
		switch (bsp_GetKey())	/* bsp_GetKey()读取键值, 无键按下时返回 KEY_NONE = 0 */
		{
			case KEY_DOWN_K1:			/* K1键按下 */
				break;

			case KEY_UP_K1:				/* K1键弹起 */
				break;

			case KEY_DOWN_K2:			/* K2键按下 */
				break;

			case KEY_UP_K2:				/* K2键弹起 */
				break;

			case KEY_NONE:				/* 无键按下 */
			default:
				/* 其它的键值不处理 */
				break;
		}
	}
Exti_while:
		printf("退出fatfs测试\r\n");
}

/*
*********************************************************************************************************
*	函 数 名: DispMenu
*	功能说明: 显示操作提示菜单
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void SD_DispMenu(void)
{
	printf("\r\n------------------------------------------------\r\n");
	printf("请选择操作命令:\r\n");
	printf("1 - 显示SD卡根目录下的文件列表\r\n");
	printf("2 - 创建一个新文件armfly.txt\r\n");
	printf("3 - 读armfly.txt文件的内容\r\n");
	printf("4 - 创建目录\r\n");
	printf("5 - 删除文件和目录\r\n");
	printf("6 - 读写文件速度测试\r\n");
	printf("7 - 使能U盘\r\n");
	printf("8 - 卸载U盘\r\n");
	printf("a - 获取磁盘的总容量和剩余容量\r\n");
	printf("r - 退出fatfs测试\r\n");
}

/*
*********************************************************************************************************
*	函 数 名: ViewRootDir
*	功能说明: 显示SD卡根目录下的文件名
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void SD_ViewRootDir(void)
{
	/* 本函数使用的局部变量占用较多，请修改启动文件，保证堆栈空间够用 */
	FRESULT result;
	FATFS fs;
	DIR DirInf;
	FILINFO FileInf;
	uint32_t cnt = 0;
//	char lfname[256];

 	/* 挂载文件系统 */
	result = f_mount(&fs, FS_VOLUME_SD, 0);	/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("挂载文件系统失败 (%s)\r\n", FR_Table[result]);
	}

	/* 打开根文件夹 */
	result = f_opendir(&DirInf, "0:/"); /* 如果不带参数，则从当前目录开始 */
	if (result != FR_OK)
	{
		printf("打开根目录失败 (%s)\r\n", FR_Table[result]);
		return;
	}

	/* 读取当前文件夹下的文件和目录 */
//	FileInf.lfname = lfname;
//	FileInf.lfsize = 256;

	printf("属性 | 文件大小 | 短文件名 | 长文件名\r\n");
	for (cnt = 0; ;cnt++)
	{
		result = f_readdir(&DirInf,&FileInf); 		/* 读取目录项，索引会自动下移 */
		if (result != FR_OK || FileInf.fname[0] == 0)
		{
			break;
		}

		if (FileInf.fname[0] == '.')
		{
			continue;
		}

		/* 判断是文件还是子目录 */
		if (FileInf.fattrib & AM_DIR)
		{
			printf("(0x%02d)目录  ", FileInf.fattrib);
		}
		else
		{
			printf("(0x%02d)文件  ", FileInf.fattrib);
		}

		/* 打印文件大小, 最大4G */
		printf("%10d", FileInf.fsize);

		printf(" %s|\r\n", FileInf.fname);	/* 短文件名 */

//		printf("%s\r\n", (char *)FileInf.lfname);	/* 长文件名 */
	}

	/* 卸载文件系统 */
	f_mount(NULL, FS_VOLUME_SD, 0);
}

/*
*********************************************************************************************************
*	函 数 名: CreateNewFile
*	功能说明: 在SD卡创建一个新文件，文件内容填写“www.armfly.com”
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void SD_CreateNewFile(void)
{
	/* 本函数使用的局部变量占用较多，请修改启动文件，保证堆栈空间够用 */
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

 	/* 挂载文件系统 */
	result = f_mount(&fs, FS_VOLUME_SD, 0);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("挂载文件系统失败 (%s)\r\n", FR_Table[result]);
	}

	/* 打开根文件夹 */
	result = f_opendir(&DirInf, "0:/"); /* 如果不带参数，则从当前目录开始 */
	if (result != FR_OK)
	{
		printf("打开根目录失败 (%s)\r\n", FR_Table[result]);
		return;
	}

	/* 打开文件 */
	result = f_open(&file, "0:armfly.txt", FA_CREATE_ALWAYS | FA_WRITE);

	/* 写一串数据 */
	result = f_write(&file, "FatFS Write Demo \r\n www.armfly.com \r\n", 34, &bw);
//	result = f_write(&file, g_TestBuf_SD, sizeof(g_TestBuf_SD), &bw);
	if (result == FR_OK)
	{
		printf("armfly.txt 文件写入成功,文件大小为(%dB)\r\n",sizeof(g_TestBuf_SD));
	}
	else
	{
		printf("armfly.txt 文件写入失败(%s)\r\n",FR_Table[result]);
	}

	/* 关闭文件*/
	f_close(&file);

	/* 卸载文件系统 */
	f_mount(NULL, FS_VOLUME_SD, 0);
}

/*
*********************************************************************************************************
*	函 数 名: ReadFileData
*	功能说明: 读取文件armfly.txt前128个字符，并打印到串口
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void SD_ReadFileData(void)
{
	/* 本函数使用的局部变量占用较多，请修改启动文件，保证堆栈空间够用 */
	FRESULT result;
	FATFS fs;
	FIL file;
	DIR DirInf;
	uint32_t bw;
	char buf[SD_BUF_SIZE];

 	/* 挂载文件系统 */
	result = f_mount(&fs, FS_VOLUME_SD, 0);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("挂载文件系统失败(%s)\r\n", FR_Table[result]);
	}

	/* 打开根文件夹 */
	result = f_opendir(&DirInf, "0:/"); /* 如果不带参数，则从当前目录开始 */
	if (result != FR_OK)
	{
		printf("打开根目录失败(%s)\r\n", FR_Table[result]);
		return;
	}

	/* 打开文件 */
	result = f_open(&file, "0:armfly.txt", FA_OPEN_EXISTING | FA_READ);
	if (result !=  FR_OK)
	{
		printf("Don't Find File : armfly.txt\r\n");
		return;
	}

	/* 读取文件 */
	result = f_read(&file, &buf, sizeof(buf) - 1, &bw);
	if (bw > 0)
	{
		buf[bw] = 0;
		printf("\r\narmfly.txt 文件内容 : \r\n%s\r\n", buf);
	}
	else
	{
		printf("\r\narmfly.txt 文件内容 : \r\n");
	}

	/* 关闭文件*/
	f_close(&file);

	/* 卸载文件系统 */
	f_mount(NULL, FS_VOLUME_SD, 0);
}

/*
*********************************************************************************************************
*	函 数 名: CreateDir
*	功能说明: 在SD卡根目录创建Dir1和Dir2目录，在Dir1目录下创建子目录Dir1_1
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void SD_CreateDir(void)
{
	/* 本函数使用的局部变量占用较多，请修改启动文件，保证堆栈空间够用 */
	FRESULT result;
	FATFS fs;

 	/* 挂载文件系统 */
	result = f_mount(&fs, FS_VOLUME_SD, 0);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("挂载文件系统失败 (%s)\r\n", FR_Table[result]);
	}

	/* 创建目录/Dir1 */
	result = f_mkdir("0:/Dir1");
	if (result == FR_OK)
	{
		printf("f_mkdir Dir1 Ok\r\n");
	}
	else if (result == FR_EXIST)
	{
		printf("Dir1 目录已经存在(%s)\r\n", FR_Table[result]);
	}
	else
	{
		printf("f_mkdir Dir1 失败 (%s)\r\n", FR_Table[result]);
		return;
	}

	/* 创建目录/Dir2 */
	result = f_mkdir("0:/Dir2");
	if (result == FR_OK)
	{
		printf("f_mkdir Dir2 Ok\r\n");
	}
	else if (result == FR_EXIST)
	{
		printf("Dir2 目录已经存在(%s)\r\n", FR_Table[result]);
	}
	else
	{
		printf("f_mkdir Dir2 失败 (%s)\r\n", FR_Table[result]);
		return;
	}

	/* 创建子目录 /Dir1/Dir1_1	   注意：创建子目录Dir1_1时，必须先创建好Dir1 */
	result = f_mkdir("0:/Dir1/Dir1_1"); /* */
	if (result == FR_OK)
	{
		printf("f_mkdir Dir1_1 成功\r\n");
	}
	else if (result == FR_EXIST)
	{
		printf("Dir1_1 目录已经存在 (%s)\r\n", FR_Table[result]);
	}
	else
	{
		printf("f_mkdir Dir1_1 失败 (%s)\r\n", FR_Table[result]);
		return;
	}

	/* 卸载文件系统 */
	f_mount(NULL, FS_VOLUME_SD, 0);
}

/*
*********************************************************************************************************
*	函 数 名: DeleteDirFile
*	功能说明: 删除SD卡根目录下的 armfly.txt 文件和 Dir1，Dir2 目录
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void SD_DeleteDirFile(void)
{
	/* 本函数使用的局部变量占用较多，请修改启动文件，保证堆栈空间够用 */
	FRESULT result;
	FATFS fs;
	char FileName[13];
	uint8_t i;

 	/* 挂载文件系统 */
	result = f_mount(&fs, FS_VOLUME_SD, 0);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("挂载文件系统失败 (%s)\r\n", FR_Table[result]);
	}

	#if 0
	/* 打开根文件夹 */
	result = f_opendir(&DirInf, "/"); /* 如果不带参数，则从当前目录开始 */
	if (result != FR_OK)
	{
		printf("打开根目录失败(%d)\r\n", result);
		return;
	}
	#endif

	/* 删除目录/Dir1 【因为还存在目录非空（存在子目录)，所以这次删除会失败】*/
	result = f_unlink("0:/Dir1");
	if (result == FR_OK)
	{
		printf("删除目录Dir1成功\r\n");
	}
	else if (result == FR_NO_FILE)
	{
		printf("没有发现文件或目录 :%s\r\n", "/Dir1");
	}
	else
	{
		printf("删除Dir1失败(错误代码 = %s) 文件只读或目录非空\r\n", FR_Table[result]);
	}

	/* 先删除目录/Dir1/Dir1_1 */
	result = f_unlink("0:/Dir1/Dir1_1");
	if (result == FR_OK)
	{
		printf("删除子目录/Dir1/Dir1_1成功\r\n");
	}
	else if ((result == FR_NO_FILE) || (result == FR_NO_PATH))
	{
		printf("没有发现文件或目录 :%s\r\n", "/Dir1/Dir1_1");
	}
	else
	{
		printf("删除子目录/Dir1/Dir1_1失败(错误代码 = %s) 文件只读或目录非空\r\n", FR_Table[result]);
	}

	/* 先删除目录/Dir1 */
	result = f_unlink("0:/Dir1");
	if (result == FR_OK)
	{
		printf("删除目录Dir1成功\r\n");
	}
	else if (result == FR_NO_FILE)
	{
		printf("没有发现文件或目录 :%s\r\n", "/Dir1");
	}
	else
	{
		printf("删除Dir1失败(错误代码 = %s) 文件只读或目录非空\r\n", FR_Table[result]);
	}

	/* 删除目录/Dir2 */
	result = f_unlink("0:/Dir2");
	if (result == FR_OK)
	{
		printf("删除目录 Dir2 成功\r\n");
	}
	else if (result == FR_NO_FILE)
	{
		printf("没有发现文件或目录 :%s\r\n", "/Dir2");
	}
	else
	{
		printf("删除Dir2 失败(错误代码 = %s) 文件只读或目录非空\r\n", FR_Table[result]);
	}
	

	/* 删除文件 armfly.txt */
	result = f_unlink("0:armfly.txt");
	if (result == FR_OK)
	{
		printf("删除文件 armfly.txt 成功\r\n");
	}
	else if (result == FR_NO_FILE)
	{
		printf("没有发现文件或目录 :%s\r\n", "armfly.txt");
	}
	else
	{
		printf("删除armfly.txt失败(错误代码 = %s) 文件只读或目录非空\r\n", FR_Table[result]);
	}
	
	/* 删除文件 Test2.txt */
	result = f_unlink("0:Test2.txt");
	if (result == FR_OK)
	{
		printf("删除文件 Test2.txt 成功\r\n");
	}
	else if (result == FR_NO_FILE)
	{
		printf("没有发现文件或目录 :%s\r\n", "Test2.txt");
	}
	else
	{
		printf("删除Test2.txt失败(错误代码 = %s) 文件只读或目录非空\r\n", FR_Table[result]);
	}

	/* 删除文件 speed1.txt */
	for (i = 0; i < 10; i++)
	{
		sprintf(FileName, "0:Speed%02d.txt", i);		/* 每写1次，序号递增 */
		result = f_unlink(FileName);
		if (result == FR_OK)
		{
			printf("删除文件%s成功\r\n", FileName);
		}
		else if (result == FR_NO_FILE)
		{
			printf("没有发现文件:%s\r\n", FileName);
		}
		else
		{
			printf("删除%s文件失败(错误代码 = %s) 文件只读或目录非空\r\n", FileName, FR_Table[result]);
		}
	}

	/* 卸载文件系统 */
	f_mount(NULL, FS_VOLUME_SD, 0);
}

/*
*********************************************************************************************************
*	函 数 名: WriteFileTest
*	功能说明: 测试文件读写速度
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void SD_WriteFileTest(void)
{
	/* 本函数使用的局部变量占用较多，请修改启动文件，保证堆栈空间够用 */
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

  	/* 挂载文件系统 */
	result = f_mount(&fs, FS_VOLUME_SD, 0);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("挂载文件系统失败 (%s)\r\n", FR_Table[result]);
	}

	/* 打开根文件夹 */
	result = f_opendir(&DirInf, "0:/"); /* 如果不带参数，则从当前目录开始 */
	if (result != FR_OK)
	{
		printf("打开根目录失败 (%s)\r\n", FR_Table[result]);
		return;
	}

	/* 打开文件 */
	sprintf(TestFileName, "0:Speed%02d.txt", s_ucTestSn++);		/* 每写1次，序号递增 */
	result = f_open(&file, TestFileName, FA_CREATE_ALWAYS | FA_WRITE);
	if (result == FR_OK)
	{
		printf("文件创建成功(%s)\r\n",FR_Table[result]);
	}
	else
	{
		printf("文件创建失败(%s)\r\n",FR_Table[result]);
	}

	/* 写一串数据 */
	printf("开始写文件%s %dKB ...\r\n", TestFileName, SDTEST_FILE_LEN / 1024);
	runtime1 = bsp_GetRunTime();	/* 读取系统运行时间 */
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
			printf("%s文件写失败(%s)\r\n,", TestFileName,FR_Table[result]);
			printf("当前i = (%d)\r\n",i);
			break;
		}
	}
	runtime2 = bsp_GetRunTime();	/* 读取系统运行时间 */

	if (err == 0)
	{
		timelen = (runtime2 - runtime1);
		printf("\r\n  写耗时 : %dms   平均写速度 : %dB/S (%dKB/S)\r\n",
			timelen,
			(SDTEST_FILE_LEN * 1000) / timelen,
			((SDTEST_FILE_LEN / 1024) * 1000) / timelen);
	}

	f_close(&file);		/* 关闭文件*/


	/* 开始读文件测试 */
	result = f_open(&file, TestFileName, FA_OPEN_EXISTING | FA_READ);
	if (result !=  FR_OK)
	{
		printf("没有找到文件: %s (%s)\r\n", TestFileName,FR_Table[result]);
		return;
	}

	printf("开始读文件 %dKB ...\r\n", SDTEST_FILE_LEN / 1024);
	runtime1 = bsp_GetRunTime();	/* 读取系统运行时间 */
	for (i = 0; i < SDTEST_FILE_LEN / SD_BUF_SIZE; i++)
	{
		result = f_read(&file, g_TestBuf_SD, sizeof(g_TestBuf_SD), &bw);
		if (result == FR_OK)
		{
			if (((i + 1) % 8) == 0)
			{
				printf(".");
			}

			/* 比较写入的数据是否正确，此语句会导致读卡速度结果降低到 3.5MBytes/S */
			for (k = 0; k < sizeof(g_TestBuf_SD); k++)
			{
				if (g_TestBuf_SD[k] != (k / 512) + '0')
				{
				  	err = 1;
					printf("Speed1.txt 文件读成功，但是数据出错\r\n");
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
			printf("Speed1.txt 文件读失败\r\n");
			break;
		}
	}
	runtime2 = bsp_GetRunTime();	/* 读取系统运行时间 */

	if (err == 0)
	{
		timelen = (runtime2 - runtime1);
		printf("\r\n  读耗时 : %dms   平均读速度 : %dB/S (%dKB/S)\r\n", timelen,
			(SDTEST_FILE_LEN * 1000) / timelen, ((SDTEST_FILE_LEN / 1024) * 1000) / timelen);
	}

	/* 关闭文件*/
	f_close(&file);

	/* 卸载文件系统 */
	f_mount(NULL, FS_VOLUME_SD, 0);
}

/*
*********************************************************************************************************
*	函 数 名: GetFreeSize
*	功能说明: 获取容量大小
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void GetFreeSize(void)
{
	FRESULT result;
	FATFS fs;
	DWORD fre_clust,fre_sect,tot_sect; 
	FATFS *p;
	
	p = &fs;
	/* 挂载文件系统 */
	result = f_mount(&fs, FS_VOLUME_SD, 0);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("挂载文件系统失败 (%s)\r\n", FR_Table[result]);
	}
	
	result = f_getfree(FS_VOLUME_SD,&fre_clust,&p);
	if(result != FR_OK)
	{
		printf("获取容量失败 (%s)\r\n", FR_Table[result]);
	}
	else
	{
		tot_sect =(p->n_fatent - 2) * p->csize/2;		/* 获取总扇区数 */
		fre_sect = fre_clust * p->csize/2;			/* 剩余扇区数量 */
		
		/* 扇区大小不是512B的话，需要转化成512B的数量 */
		#if _MAX_SS != 512                                           
			tot_sect *= p->ssize / 512;
			fre_sect *= p->ssize / 512;
		#endif 
		
		/* 转换成KB为单位 */
//		tot_sect = tot_sect>>1;    	//单位KB
//        fre_sect = fre_sect>>1;   	//单位KB
		printf("tot_sect = %dKB\r\nfre_sect = %dKB\r\n",(uint32_t)tot_sect ,(uint32_t)fre_sect);
	}
	/* 卸载文件系统 */
	f_mount(NULL, FS_VOLUME_SD, 0);
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
