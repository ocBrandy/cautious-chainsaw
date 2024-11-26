/*********************************************************************************************************
*
*	ģ������ : ���������
*	�ļ����� : main.c
*	��    �� : V1.0
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		V1.0    2019-04-08 		   �װ�
*********************************************************************************************************/

#include "bsp.h"				/* �ײ�Ӳ������ */

#include "ERSC_HT_DataStorage.h"
#include "ETH_DataStorage.h"
#include "BP_DataStorage.h"
#include "VGUS_LCD.h"

void KeyDistStatusUpdata(uint8_t type);
extern uint32_t push_timer,timer;
extern uint8_t Key_disable_flag,MD_commd_flag,BP_commd_flag,Motor_status_flag;					
extern uint8_t Key_response_status;	
extern uint8_t Key_status;	
extern void Key_NoresponseRefresh_Colorblock(uint8_t _status);
extern uint8_t Button_flag,MD_cmd_dist_flag,motor_flag,MD_timer;
extern uint16_t Key_respstatus_data;
extern uint8_t repeat_dist_flag,repeat_response_flag,_7538_flag;
uint16_t DH_data_timer = 0;
uint8_t timer_out = 0;
extern void ADC_acqdata(void);
extern void send_key_cmd(void);
extern void init_key(void);
extern void Initialvoltagevalue(void);
/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: c�������
*	��    �Σ���
*	�� �� ֵ: �������(���账��)
*********************************************************************************************************
*/
int main(void)
{
	/*ST�̼����е������ļ��Ѿ�ִ���� SystemInit() �������ú����� system_stm32f4xx.c �ļ�����Ҫ������
	����CPUϵͳ��ʱ�ӣ��ڲ�Flash����ʱ������FSMC�����ⲿSRAM*/
	bsp_Init();		/* Ӳ����ʼ�� */
	bsp_StartAutoTimer(0,250);
	bsp_DelayMS(2000);////��Ļ��ʼ����
	init_key();//����ɨ��
	Initialvoltagevalue();//ƫ�õ�ѹ�ɼ�
	bsp_LedOn(1);//��ʼ����ɣ�����LED
	

	while (1)
	{	
		HT_250ms_Timerout();																//250ms��ѯ
		Downdata_storage();																	//����5���վ������ݣ����ݽ���
		if(Key_disable_flag == 0 && _7538_flag == 1)				//��λ��ʧ�ܣ�����ʹ��
			{
				KeyDistStatusUpdata(bsp_GetKey());							//����ɨ��
			}
		ETH_data_storage();																	//��̫������1������λ�����ݣ����ݽ���
		BP_data_storage();																	//485����2���ձ�Ƶ�������ݣ����ݽ���
			
		if((MD_commd_flag == 1)&&((MD_timer >= 150)))				//��ʱʱ�䵽,���߱�Ƶ�������ݽ������
		{
			MD_commd_flag = 0;																//��ﰴ����������

			if((Key_status & 0x01) == 1)											//���ָ�� �·�
				{
					Sendkeycmd(MD_DIST_REG,1);
					BP_commd_flag = 0;
				}
			else
				{
					Sendkeycmd(MD_DIST_REG,0);
					BP_commd_flag = 0;
				}	
		}

		if((Motor_status_flag == 1)&&(BP_commd_flag == 0))//��ﷴ��״̬ ����Ƶ�������������ɣ�
		{
			Motor_status_flag = 0;
			if((Key_respstatus_data & 0x01) == 1)						//��ﷴ��ָ���·�
				{
					Sendkeycmd(MD_RESPONSE_REG,1);	
				}
			else
				{
					Sendkeycmd(MD_RESPONSE_REG,0);
				}
		}	
			
		bsp_LedOn(2);
	}		
}	

/***************************** XMDNDL (END OF FILE) *********************************/
