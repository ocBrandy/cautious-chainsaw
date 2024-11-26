/*
*********************************************************************************************************
*
*	模块名称 : 系统bootloader升级头文件
*	文件名称 : SysBootUpdate.h
*	说    明 : 使用时，需要根据当前使用的STM32的具体型号，打开对应的宏定义，下面的条件编译会给系统Boot地址
*				赋值，如当前的宏没包含所使用的型号,则应该将下面的STM32型号宏定义都屏蔽，并在下面条件编译
*				的#else分支下修改系统Boot地址，具体型号的系统boot地址，查阅ST官方的AN2606文档，该文档在
*				STM32CubeProg软件安装目录下的D:\STMicroelectronics\STM32Cube\STM32CubeProgrammer\doc内
*
*********************************************************************************************************
*/
#ifndef __SYSBOOTUPDATE_H
#define __SYSBOOTUPDATE_H

/* 定义所使用的STM32型号 */
//#define   _STM32F103VET6
#define	_STM32F103ZET6
//#define	_STM32F107VCT6
//#define	_STM32F429IGT6


#if defined (_STM32F103VET6) || defined (_STM32F103ZET6)
	#define	SYS_BOOT_ADDR	0x1FFFF000	 
#elif	defined _STM32F107VCT6
	#define	SYS_BOOT_ADDR	0x1FFFB000
#elif	defined _STM32F429IGT6
	#define	SYS_BOOT_ADDR	0x1FFF0000
#else
	#define	SYS_BOOT_ADDR	0x1FFFF000
#endif


/* 提供给其他C文件调用的函数 */
void JumpToBootloader(void);

#endif

/***************************** yy-AA55 (END OF FILE) *********************************/
