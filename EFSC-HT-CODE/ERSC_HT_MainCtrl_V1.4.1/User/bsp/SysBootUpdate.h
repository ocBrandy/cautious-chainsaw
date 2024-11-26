/*
*********************************************************************************************************
*
*	ģ������ : ϵͳbootloader����ͷ�ļ�
*	�ļ����� : SysBootUpdate.h
*	˵    �� : ʹ��ʱ����Ҫ���ݵ�ǰʹ�õ�STM32�ľ����ͺţ��򿪶�Ӧ�ĺ궨�壬���������������ϵͳBoot��ַ
*				��ֵ���統ǰ�ĺ�û������ʹ�õ��ͺ�,��Ӧ�ý������STM32�ͺź궨�嶼���Σ�����������������
*				��#else��֧���޸�ϵͳBoot��ַ�������ͺŵ�ϵͳboot��ַ������ST�ٷ���AN2606�ĵ������ĵ���
*				STM32CubeProg�����װĿ¼�µ�D:\STMicroelectronics\STM32Cube\STM32CubeProgrammer\doc��
*
*********************************************************************************************************
*/
#ifndef __SYSBOOTUPDATE_H
#define __SYSBOOTUPDATE_H

/* ������ʹ�õ�STM32�ͺ� */
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


/* �ṩ������C�ļ����õĺ��� */
void JumpToBootloader(void);

#endif

/***************************** yy-AA55 (END OF FILE) *********************************/
