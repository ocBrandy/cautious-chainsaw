/*
*********************************************************************************************************
*
*	模块名称 : 系统bootloader升级
*	文件名称 : SysBootUpdate.c
*	版    本 : V1.0
*	说    明 : 跳转系统bootloader进行IAP更新，具体型号支持哪些IAP方式，参照ST官方的AN2606文档，该文档在
*				STM32CubeProg软件安装目录下的D:\STMicroelectronics\STM32Cube\STM32CubeProgrammer\doc内
*				使用方法：	1.添加SysBootUpdate.c和SysBootUpdate.h到工程下
*							2.修改SysBootUpdate.h下的STM32型号宏定义定义，确保系统boot地址与AN2606文档对应上
*							3.在应用程序中，需要进行IAP升级的地方，声明头文件#include "SysBootUpdate.h"，并在相应地方调用JumpToBootloader()
*							4.打开STM32CubeProgrammer，首先保证所使用的升级接口没有被其他东西占用，如串口
*							  升级，要确保串口1没有被串口助手之类的占用，最好关闭串口助手；然后在STM32CubeProgrammer
*							  下选择连接方式，点击Connect,如果当前已经正确进入到系统BOOT，会显示出当前芯片信息。
*							5.选择要更新的hex固件，勾选Verify programming，点击Start programming进行下载
*							6.显示下载成功后，将芯片复位即可
*	修改记录 :
*		版本号  日期       		作者     		说明
*		V1.0    2020-04-09 		yy-AA55   		首版
*
*********************************************************************************************************
*/

#include "SysBootUpdate.h"
#include "bsp.h"

/*
*********************************************************************************************************
*	函 数 名: JumpToBootloader
*	功能说明: 跳转到系统BootLoader
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void JumpToBootloader(void)
{
	uint32_t i=0;
	void (*SysMemBootJump)(void);        /* 声明一个函数指针 */
	__IO uint32_t BootAddr = SYS_BOOT_ADDR; /* 系统BootLoader地址 */

	/* 关闭全局中断 */
	__set_PRIMASK(1); 

	/* 关闭滴答定时器，复位到默认值 */
	SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;

	/* 设置所有时钟到默认状态，使用HSI时钟 */	
#if defined(USE_HAL_DRIVER)
    HAL_RCC_DeInit();
#endif /* defined(USE_HAL_DRIVER) */
#if defined(USE_STDPERIPH_DRIVER)
    RCC_DeInit();
#endif /* defined(USE_STDPERIPH_DRIVER) */

	/* 关闭所有中断，清除所有中断挂起标志 */
	for (i = 0; i < 8; i++)
	{
		NVIC->ICER[i]=0xFFFFFFFF;
		NVIC->ICPR[i]=0xFFFFFFFF;
	}	

	/* 使能全局中断 */
	__set_PRIMASK(0);

	/* 跳转到系统BootLoader，首地址是MSP，地址+4是复位中断服务程序地址 */
	SysMemBootJump = (void (*)(void)) (*((uint32_t *) (BootAddr + 4)));

	/* 设置主堆栈指针 */
	__set_MSP(*(uint32_t *)BootAddr);
	
	/* 在RTOS工程，这条语句很重要，设置为特权级模式，使用MSP指针 */
	__set_CONTROL(0);

	/* 跳转到系统BootLoader */
	SysMemBootJump(); 

	/* 跳转成功的话，不会执行到这里，用户可以在这里添加代码 */
	while (1)
	{
		
	}
}

/***************************** yy-AA55 (END OF FILE) *********************************/
