#include "bsp.h"
#include "ff.h"			/* FatFS�ļ�ϵͳģ��*/
#include "ERSC_HT_DataStorage.h"
#include "ETH_DataStorage.h"
#include "VGUS_LCD.h"
#include "ERSC_HT_250ms.h"
extern s_eth_send  send_data;
extern uint8_t DoseResend;
uint16_t CheckSum(uint8_t *_buf, uint16_t _len);

uint8_t Downdata_count = 0;
void KeyStatusResponse(uint8_t type);
uint8_t motorcycle_REV_count = 0,motorcycle_FWD_count = 0;
uint8_t Downdata_buf[50] = {0};
extern void send_key_cmd(void);
uint8_t Downdatarecv_flag = 0;
extern uint8_t ETH_datarecv_flag;
uint8_t Motor_status_flag = 0;
uint8_t Key_response_status = 0;
uint8_t g_ucTimeOut2 = 0;
uint16_t Error_timer = 0;
uint16_t temp = 0;
uint16_t temp0 = 0;
uint8_t  uptemperature;
uint8_t motor_cycle = 0;
uint8_t downcnt = 0,downdatabuf_clear = 0;
uint8_t motorrad = 0;
extern void MotorCycle_FWD(void);//��ת
extern void MotorCycle_REV(void);//��ת
extern void KeyresponseRefresh_Colorblock(uint8_t Keystatus_response);
extern void Key_NoresponseRefresh_Colorblock(uint8_t _status);
extern void KeyDistStatusUpdata(uint8_t type);
extern uint8_t Button_flag,Key_status;
extern uint8_t start_push;
extern uint32_t ds18b20init ;
extern uint32_t push_timer,MD_cmd_dist_flag,timer,DH_data_timer,timerout,timerout_resetLCD,cmd_back,Rx_out;
extern uint8_t push_flag,motor_flag,pressuer_flag,bit_process_flag,gama_flag;
extern uint8_t Key_disable_flag;
extern uint8_t lcd_flag,flag_allkey,eth_ctrl;
uint8_t Button_response_flag = 0;
uint8_t resetLCD_flag = 0;
extern uint16_t eth_cmd,_cmd;
uint8_t BP_RequstBuf[35] = {0};
extern void ETH_DistStatusUpdata(uint8_t type);
extern void ETH_StatusResponse(uint8_t type);
uint8_t cnt = 0,header_flag = 0,resend_cnt = 0;
/*********************************************************/
void Downdata_storage(void) //�������ݽ���   ����5 19200 PD2 PC12
{
	uint8_t ucData;
	if(comGetChar(COM5,&ucData))
	{
		timerout = 0;
		timerout_resetLCD = 0;
		resetLCD_flag = 0;	
		if(ucData == 0xAA &&	header_flag == 0)  //ʶ��֡ͷ�������ݻ���
		{			
			Downdata_count = 0;
			header_flag  =  1;								
			Downdata_buf[Downdata_count] = ucData;
			Downdata_count++;
		}

		else if(header_flag == 1)//�ҵ�֡ͷ
		{
			Downdata_buf[Downdata_count++] = ucData;
			if(Downdata_count >= 35)
			{
				if(ucData == 0x55)
				{
				Downdatarecv_flag = 1;
				}			
				else
				{
					header_flag = 0;
					for(uint8_t i = 0;i<Downdata_count;i++)
					{
					Downdata_buf[i] = 0;
					}
					Downdata_count = 0;
				}
			}
			
		}
		}	
		if(timerout >= 300 && lcd_flag == 1)///��ʱʱ�䵽,��ʱ�������͸���Ļ
			{
				timerout = 0;
				DH_data_timer++;//��������
				
				if((motor_flag == 1) ||(bit_process_flag == 1)||(push_flag == 1)||(gama_flag == 1)||(pressuer_flag == 1))
					{
						Button_flag = 0;
						Key_NoresponseRefresh_Colorblock(Key_status);		///��ʱ����δ�ظ������ɫ		
					}				
			}		
		if(timerout_resetLCD >= 1000)////��ʱ1S,����
			{
				timerout_resetLCD = 0;	
				for(uint8_t i = 0;i<35;i++)//��վ�������
				{
					Downdata_buf[i] = 0;
				}		
			}
	/*���ݽ���*/		
	if(Downdatarecv_flag == 1)
	{	
		//comSendBuf(COM1,&Downdata_buf[16],2);
		header_flag = 0;
		downdatabuf_clear = 1;
		uint8_t  sum = 0;
		Downdatarecv_flag = 0;
		sum = CheckSum(Downdata_buf,Downdata_count - 2) + 0x55;
		if(sum == Downdata_buf[Downdata_count - 2])////�ж�У���   Downdata_count=35  У����� ��33�ֽ�
			{				
			//�������ݸ���λ��
				if(uptemperature > 0 && (Downdata_buf[1] - uptemperature >= 8))//ds18b20init>=2000 && 
				{
				Downdata_buf[1] = uptemperature;
				}
				uptemperature = Downdata_buf[1];

				for(uint8_t i= 0;i < 35;i++)//�������Ȼ���
					{
						BP_RequstBuf[i] = Downdata_buf[i];			
					}
				Key_response_status = Downdata_buf[Downdata_count - 10];//�����ظ�״̬
				if((DoseResend != Key_response_status)&&((Key_response_status != CT_pressure_up)||(Key_response_status != CT_pressure_hold)||(Key_response_status != CT_pressure_down)))	//�ط�
				{			
					if(resend_cnt < 2)//1��
					{
						CmdSendToUnderGround(DoseResend);//�ط�
						resend_cnt++;
					}
				}
							
					
				if(Key_disable_flag == 0)///��λ��ʧ�ܣ�����ʹ��,,,�Ż�ˢ��ɫ��
					{
						KeyStatusResponse(Key_response_status);//ˢ��ɫ�飨����ɫ/��ɫ��		
					}				
				else
					{
						ETH_StatusResponse(Key_response_status);//����״̬
					}

				if(Key_response_status == CT_push_open)///�����¿��ƿ�����ʼ��ʱ
					{	
						if(cmd_back > 1000)
						{
							cmd_back = 0;
							timer = 0;
							push_timer = 0;
						}
						start_push = 1;	///��ʱ���ж����ʼ��ʱ						
					}				
				else if(Key_response_status == CT_push_close)//�����¹��ƿ���ֹͣ��ʱ,ˢ����Ļ
					{
						start_push = 0;
					}
				else if(Key_response_status == CT_pressure_up)//ѹ������ת��++
					{
							motor_cycle ++ ;
							downcnt++;
							if(downcnt>128)
							{
								motor_cycle = 256 - downcnt;
							}
							if(motor_cycle==42)
							{
								motor_cycle = 0;
								downcnt = 0;
								motorcycle_FWD_count++;
								motorrad++;							
								if(motorrad>128)
								{
									motorcycle_FWD_count = 256-motorrad;
								}
							}
					}
				else if(Key_response_status == CT_pressure_down)//ѹ������ת��--
					{
								motor_cycle--;
								downcnt --;
								if(downcnt>128)
								{
									motor_cycle = 256 - downcnt;
								}
								if(motor_cycle==42)
								{
									motor_cycle = 0;
									downcnt = 0;
									motorcycle_FWD_count--;
									motorrad--;
									if(motorrad>128)
									{
										motorcycle_FWD_count = 256-motorrad;
									}
								}
					}
					
				else if((Key_response_status == CT_motor_run) || (Key_response_status == CT_motor_stop))
					{
						Motor_status_flag = 1;	//�յ���ﷴ��ָ��ñ�־λ�����жϱ�Ƶ�����Ƿ��ڷ����ݣ�
					}	
					
				if(eth_ctrl == 1 &&((_cmd == CT_motor_run)||(_cmd == CT_motor_stop)||(_cmd == CT_bit_front)||(_cmd == CT_bit_back)
					||(_cmd == CT_push_open)||(_cmd == CT_push_close)||(_cmd == CT_pressure_up)||(_cmd == CT_pressure_hold)
					||(_cmd == CT_pressure_down)||(_cmd == CT_gama_open)||(_cmd == CT_gama_close)))	////��λ���������͵�����
					{
						ETH_DistStatusUpdata(_cmd);///�����·�ָ�����					
						send_key_cmd();//��������͵�����
					}	
				else
					{
						send_key_cmd();//��������͵����£�
					}
					
					
					
					
			}
		else          ///���������
		{	
			Error_timer++;//��������ϴ�����Ļ
			for (uint8_t i = 0;i < Downdata_count;i++)           //��ս�������,���½���
				{
					 Downdata_buf[i] = 0;                                  
				}
				Downdata_count = 0;		
		}	
	}					
}



/*****************************************************************************
 * ��  ����CheckSum
 * ��  �ܣ�����У���
 * ��  ����_buf:��Ҫ���������buf
 *		   _len����������ݳ���
 * ����ֵ��У��͵ĵ�λ�ֽ�
*****************************************************************************/
 uint16_t CheckSum(uint8_t *_buf, uint16_t _len)
{
	uint16_t sum = 0;
	uint16_t i;
	 
	for(i = 0 ; i<_len ; i++)
		{
			sum += _buf[i];
		}	
	return sum;
}
