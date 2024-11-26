/*
*********************************************************************************************************
*
*	ģ������ : ϵͳbootloader����
*	�ļ����� : SysBootUpdate.c
*	��    �� : V1.0
*	˵    �� : ��תϵͳbootloader����IAP���£������ͺ�֧����ЩIAP��ʽ������ST�ٷ���AN2606�ĵ������ĵ���
*				STM32CubeProg�����װĿ¼�µ�D:\STMicroelectronics\STM32Cube\STM32CubeProgrammer\doc��
*				ʹ�÷�����	1.���SysBootUpdate.c��SysBootUpdate.h��������
*							2.�޸�SysBootUpdate.h�µ�STM32�ͺź궨�嶨�壬ȷ��ϵͳboot��ַ��AN2606�ĵ���Ӧ��
*							3.��Ӧ�ó����У���Ҫ����IAP�����ĵط�������ͷ�ļ�#include "SysBootUpdate.h"��������Ӧ�ط�����JumpToBootloader()
*							4.��STM32CubeProgrammer�����ȱ�֤��ʹ�õ������ӿ�û�б���������ռ�ã��紮��
*							  ������Ҫȷ������1û�б���������֮���ռ�ã���ùرմ������֣�Ȼ����STM32CubeProgrammer
*							  ��ѡ�����ӷ�ʽ�����Connect,�����ǰ�Ѿ���ȷ���뵽ϵͳBOOT������ʾ����ǰоƬ��Ϣ��
*							5.ѡ��Ҫ���µ�hex�̼�����ѡVerify programming�����Start programming��������
*							6.��ʾ���سɹ��󣬽�оƬ��λ����
*	�޸ļ�¼ :
*		�汾��  ����       		����     		˵��
*		V1.0    2020-04-09 		yy-AA55   		�װ�
*
*********************************************************************************************************
*/

#include "SysBootUpdate.h"
#include "bsp.h"

/*
*********************************************************************************************************
*	�� �� ��: JumpToBootloader
*	����˵��: ��ת��ϵͳBootLoader
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void JumpToBootloader(void)
{
	uint32_t i=0;
	void (*SysMemBootJump)(void);        /* ����һ������ָ�� */
	__IO uint32_t BootAddr = SYS_BOOT_ADDR; /* ϵͳBootLoader��ַ */

	/* �ر�ȫ���ж� */
	__set_PRIMASK(1); 

	/* �رյδ�ʱ������λ��Ĭ��ֵ */
	SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;

	/* ��������ʱ�ӵ�Ĭ��״̬��ʹ��HSIʱ�� */	
#if defined(USE_HAL_DRIVER)
    HAL_RCC_DeInit();
#endif /* defined(USE_HAL_DRIVER) */
#if defined(USE_STDPERIPH_DRIVER)
    RCC_DeInit();
#endif /* defined(USE_STDPERIPH_DRIVER) */

	/* �ر������жϣ���������жϹ����־ */
	for (i = 0; i < 8; i++)
	{
		NVIC->ICER[i]=0xFFFFFFFF;
		NVIC->ICPR[i]=0xFFFFFFFF;
	}	

	/* ʹ��ȫ���ж� */
	__set_PRIMASK(0);

	/* ��ת��ϵͳBootLoader���׵�ַ��MSP����ַ+4�Ǹ�λ�жϷ�������ַ */
	SysMemBootJump = (void (*)(void)) (*((uint32_t *) (BootAddr + 4)));

	/* ��������ջָ�� */
	__set_MSP(*(uint32_t *)BootAddr);
	
	/* ��RTOS���̣�����������Ҫ������Ϊ��Ȩ��ģʽ��ʹ��MSPָ�� */
	__set_CONTROL(0);

	/* ��ת��ϵͳBootLoader */
	SysMemBootJump(); 

	/* ��ת�ɹ��Ļ�������ִ�е�����û�������������Ӵ��� */
	while (1)
	{
		
	}
}

/***************************** yy-AA55 (END OF FILE) *********************************/
