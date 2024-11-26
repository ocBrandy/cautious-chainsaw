#include "bsp.h"
#include "ff.h"			/* FatFS文件系统模块*/
#include "ETH_DataStorage.h"
#include "BP_DataStorage.h"
#include "VGUS_LCD.h"
#include "math.h"
#include "ERSC_HT_250ms.h"
extern void LCD_Updata(void);
extern void ADC_Motor_Current_acqdata(void);
extern void ADC_Ctrl_Current_acqdata(void);
extern void ADC_Motor_Voltage_acqdata(void);
extern void Data_request(void);
extern  void ButtonRed(uint8_t id,uint8_t up_or_down);
void Key_NoresponseRefresh_Colorblock(uint8_t _status);
void KeyresposeStatusUpdata(uint8_t Keystatus);
void Torque_acq(void);
uint8_t Key_status = 0;
uint8_t MD_commd_flag = 0;
uint8_t MD_cmd_dist_flag = 0;
void MotorCycle_FWD(void);//正转
void MotorCycle_REV(void);//反转
extern uint16_t CheckSum(uint8_t *_buf, uint16_t _len);
extern uint8_t eth_cmd,resend_cnt;

extern uint8_t BP_commd_flag;
extern uint16_t ctrl_current;
extern uint8_t start_push,resetLCD_flag;
uint8_t push_flag=0,motor_flag=0,pressuer_flag=0,bit_process_flag=0,gama_flag=0,flag_yellow;
extern uint32_t DH_data_timer;
extern uint32_t adc_cnt;
extern uint8_t downdatabuf_clear,BPdatabuf_clear; 
 extern uint8_t BP_databuf[BP_MAX_RECV_LENGTH];
extern uint8_t Downdata_buf[35];
/******************************************************************************
 * 函  数：HT_250ms_Timerout
 * 功  能：地面250ms轮询，
 * 参  数：无	   
 * 返回值：无
*******************************************************************************/
extern s_eth_send  send_data;

void HT_250ms_Timerout(void)
{
	if(bsp_CheckTimer(0))
	{
		LCD_Updata();		//刷屏显示	

		WriteGmmaCntToFpga(send_data.gmma_cnt);					//给FPGA写入伽马值
		Torque_acq();																		//扭矩测试
		senddatatoETH();															//给上位机上传数据
		Data_request();																//数据请求	
	}

		ADC_Ctrl_Current_acqdata();																																			//缆头电流采集
		ADC_Motor_Current_acqdata();																																		//电机电流采集井上电机电压采集
}
/******************************************************************************
 * 函  数：KeyDistStatusUpdata
 * 功  能：按键状态更新
 * 参  数：按键扫描值	   
 * 返回值：无
*******************************************************************************/
///关推靠，钻进指令不下达，，，检测到马达指令，单独下发给变频机箱

////extern uint8_t timeout_cnt;
uint8_t upflag = 0;
uint8_t downflag = 0;
uint16_t motor_cycle_lcd;
uint8_t motor_cycle_flag;
uint8_t flag_allkey = 0;
uint8_t  Push_open_flag,Push_close_flag,bit_process_open_flag,bit_process_close_flag,motor_open_flag,motor_close_flag,
gama_open_flag,gama_close_flag,pressure_up_flag,pressure_down_flag,pressure_keep_flag;

uint16_t _cmd;
void ETH_DistStatusUpdata(uint8_t type)
{
	if(_cmd == CT_push_open)//推靠开
	{
		resend_cnt = 0;//重发计数清零
		Push_open_flag = 1;//等井下数据上传完以后在下发指令
		Key_status |= 0x04;
	}				
	if(_cmd == CT_push_close)//推靠关	
	{		
		resend_cnt = 0;//重发计数清零
		Push_close_flag = 1;//等井下数据上传完以后在下发指令	
		Key_status &= ~0x04;
	}			
	if(_cmd == CT_bit_front)//钻进开
	{		
		resend_cnt = 0;//重发计数清零
		bit_process_open_flag = 1;
		if(start_push == 1)//井下推靠打开
			{						
				Key_status |= 0x02;				
			}
	}		
	if(_cmd == CT_bit_back)//钻进关			
	{
		resend_cnt = 0;//重发计数清零
		bit_process_close_flag = 1;
		if(start_push == 1)
			{
				Key_status &= ~0x02;
			}
	}
	if(_cmd == CT_motor_run)//马达开					
	{
		resend_cnt = 0;//重发计数清零
		motor_open_flag = 1;

		if(BP_commd_flag == 1)//变频在接收数据，标志位置一 
			{
				MD_commd_flag = 1;
			}
			else///立即下发马达指令给变频机箱
			{

				Sendkeycmd(MD_DIST_REG,1);				
			}
		Key_status |= 0x01;						

	}
	if(_cmd == CT_motor_stop)//马达关			
	{
		resend_cnt = 0;//重发计数清零
		motor_close_flag = 1;
		if(BP_commd_flag == 1)//变频在接收数据，标志位置一 
			{
				MD_commd_flag = 1;
			}
		else///立即下发马达指令给变频机箱
			{
				Sendkeycmd(MD_DIST_REG,0);				
			}
		Key_status &= ~0x01;			
	}
	if(_cmd == CT_gama_open)//伽马开				
	{
		resend_cnt = 0;//重发计数清零
		gama_open_flag = 1;
		Key_status |= 0x08; 	
	}
	if(_cmd == CT_gama_close)//伽马关										
	{
		resend_cnt = 0;//重发计数清零
		gama_close_flag = 1;
		Key_status &= ~0x08;	
	}
	if(_cmd == CT_pressure_up)//压力升			
	{
		resend_cnt = 0;//重发计数清零
			pressure_up_flag = 1;
			Key_status |= 0x10;
			Key_status &= ~0x20;
	}
	if(_cmd == CT_pressure_hold)//压力保持		
	{
		resend_cnt = 0;//重发计数清零
		pressure_keep_flag = 1;
		Key_status &= ~0x10;
		Key_status &= ~0x20;
	}
	if(_cmd == CT_pressure_down)//压力降					
	{
		resend_cnt = 0;//重发计数清零
		pressure_down_flag = 1;
		Key_status |= 0x20;
		Key_status &= ~0x10; 
	}
_cmd = 0x00;	
}
//
void KeyDistStatusUpdata(uint8_t type)
{
	switch(type)
	{
		case PUSH_KEY1_DOWN://推靠开
				resend_cnt = 0;//重发计数清零
				bit_process_open_flag = 0;
				bit_process_close_flag = 0;
////				timeout_cnt = 0;			
				push_flag	 = 1;					
				Push_open_flag = 1;
				Key_status |= 0x04;
        onButtonUpdata_Rect(2,open,COLOR_GREEN);//绿色					
				break;
		case PUSH_KEY1_UP://推靠关
				resend_cnt = 0;//重发计数清零
////				timeout_cnt = 0;		
				Push_close_flag = 1;
				push_flag	 = 1;				
				Key_status &= ~0x04;
				onButtonUpdata_Rect(2,close,COLOR_GREEN);				
				break;
		case BIT_PROCESS_KEY2_DOWN://钻进开
				resend_cnt = 0;//重发计数清零
////				timeout_cnt = 0;			
				bit_process_flag = 1;
				bit_process_open_flag = 1;
				if(start_push == 1)//井下推靠打开
					{						
						Key_status |= 0x02;
						onButtonUpdata_Rect(3,open,COLOR_GREEN);//色块显示绿色					
					}
				else
					{
						onButtonUpdata_Rect(3,open,COLOR_YELLOW);//色块显示黄色，，，指令不下发
					}
				break;
		case BIT_PROCESS_KEY2_UP://钻进关
				resend_cnt = 0;//重发计数清零
////				timeout_cnt = 0;			
				bit_process_flag = 1;
				bit_process_close_flag = 1;
				if(start_push == 1)
					{
						
						Key_status &= ~0x02;
						onButtonUpdata_Rect(3,close,COLOR_GREEN);//色块显示绿色						
						flag_yellow = 1;
					}
				else
					{
						onButtonUpdata_Rect(3,close,COLOR_YELLOW);//色块显示黄色，，，指令不下发
						flag_yellow = 0;
					}
				break;
		case MOTOR_KEY3_DOWN://马达开
				resend_cnt = 0;//重发计数清零
////				timeout_cnt = 0;	
				motor_open_flag = 1;
				motor_flag =1;

				if(BP_commd_flag == 1)//变频在接收数据，标志位置一 
					{
						MD_commd_flag = 1;
					}
					else///立即下发马达指令给变频机箱
          {
						Sendkeycmd(MD_DIST_REG,1);				
					}
				Key_status |= 0x01;					
		    onButtonUpdata_Rect(4,open,COLOR_GREEN);				
				break;
		case MOTOR_KEY3_UP://马达关
				resend_cnt = 0;//重发计数清零
////				timeout_cnt = 0;	
				motor_close_flag = 1;
				motor_flag = 1;

				if(BP_commd_flag == 1)//变频在接收数据，标志位置一 
					{
						MD_commd_flag = 1;
					}
					else///立即下发马达指令给变频机箱
          {
						Sendkeycmd(MD_DIST_REG,0);				
					}
				Key_status &= ~0x01;
				onButtonUpdata_Rect(4,close,COLOR_GREEN);				
				break;
		case GMMA_KEY4_DOWN://伽马开
				resend_cnt = 0;//重发计数清零
//////				timeout_cnt = 0;	
				gama_open_flag = 1;
				gama_flag = 1;
				Key_status |= 0x08; 
				onButtonUpdata_Rect(0,open,COLOR_GREEN);			
				break;
		case GMMA_KEY4_UP://伽马关
				resend_cnt = 0;//重发计数清零
////				timeout_cnt = 0;	
				gama_close_flag = 1;
				gama_flag = 1;
				Key_status &= ~0x08;
				onButtonUpdata_Rect(0,close,COLOR_GREEN);		
				break;
		case RESSURE_UP_KEY5_LONG://压力升
				resend_cnt = 0;//重发计数清零
//////				timeout_cnt = 0;	
				pressure_up_flag = 1;
				pressuer_flag = 1;

				Key_status |= 0x10;
				Key_status &= ~0x20;
				onButtonUpdata_Rect(1,open,COLOR_GREEN);		
				break;
		case RESSURE_UP_KEY5_UP://压力保持
				resend_cnt = 0;//重发计数清零
				pressure_keep_flag = 1;
				pressuer_flag = 1;

				Key_status &= ~0x10;
				Key_status &= ~0x20;
				ButtonClear(1);//两色块全清色
				break;
		case PRESSURE_DOWN_KEY6_LONG://压力降
				resend_cnt = 0;//重发计数清零
////				timeout_cnt = 0;	
				pressure_down_flag = 1;
				pressuer_flag = 1;

				Key_status |= 0x20;
				Key_status &= ~0x10; 
				onButtonUpdata_Rect(1,close,COLOR_GREEN);			
				break;
		case PRESSURE_DOWN_KEY6_UP://压力保持
				resend_cnt = 0;//重发计数清零
				pressure_keep_flag = 1;
				pressuer_flag = 1;
				Key_status  &= ~0x10;
				Key_status  &= ~0x20; 
				ButtonClear(1);//两色块全清色		
				break;
		default:
				break;
  	
}
}

//
uint16_t Key_respstatus_data = 0;
extern uint16_t commd_reg;
extern uint8_t Button_flag;
uint8_t commd_response_cnt;
void KeyStatusResponse(uint8_t type)
{
switch(type)
{
	case CT_push_open:
		Button_flag = 0;//用于超时刷新色块
		ButtonDeepGreen(2,open);
		Key_respstatus_data |= 0x04;
		break;
	case CT_push_close:
		Button_flag = 0;
		ButtonDeepGreen(2,close);
		Key_respstatus_data &= ~0x04;
		break;
	case CT_bit_front:
		Button_flag = 0;
		ButtonDeepGreen(3,open);
		Key_respstatus_data |= 0x02;
		break;
	case CT_bit_back:
		Button_flag = 0;
		ButtonDeepGreen(3,close);
		Key_respstatus_data &= ~0x02;
		break;
	case CT_motor_run:
		Button_flag = 0;
		ButtonDeepGreen(4,open);
		Key_respstatus_data |= 0x01;
		break;
	case CT_motor_stop:
		Button_flag = 0;
		ButtonDeepGreen(4,close);
		Key_respstatus_data &= ~0x01;
		break;
	case CT_gama_open:
		Button_flag = 0;
		ButtonDeepGreen(0,open);
		Key_respstatus_data |= 0x08;
		break;
	case CT_gama_close:
		Button_flag = 0;
		ButtonDeepGreen(0,close);
		Key_respstatus_data &= ~0x08;
		break;
	case CT_pressure_up:
		Button_flag = 0;
		ButtonDeepGreen(1,open);
		Key_respstatus_data |= 0x10;
		Key_respstatus_data &= ~0x20;
		break;
	case CT_pressure_hold:
		Button_flag = 0;
		ButtonClear(1);
		Key_respstatus_data  &= ~0x10;
		Key_respstatus_data  &= ~0x20; 
		break;
	case CT_pressure_down:
		Button_flag = 0;
		ButtonDeepGreen(1,close);
		Key_respstatus_data |= 0x20;
		Key_respstatus_data &= ~0x10; 
		break;
	default:
			if(Button_flag == 1)
			{				
				commd_response_cnt++;
				if(commd_response_cnt > 1)
				{
					commd_response_cnt = 0;
					Button_flag = 0;
					switch(commd_reg)///下发给井下指令
					{
						case CT_push_open:
							ButtonRed(2,open);
							break;
						case CT_push_close:
							ButtonRed(2,close);
							break;
						case CT_bit_front:
							ButtonRed(3,open);
							break;
						case CT_bit_back:
							ButtonRed(3,close);
							break;
						case CT_motor_run:
							ButtonRed(4,open);
							break;
						case CT_motor_stop:
							ButtonRed(4,close);
							break;
						case CT_gama_open:
							ButtonRed(0,open);
							break;
						case CT_gama_close:
							ButtonRed(0,close);
							break;
						case CT_pressure_up:
							ButtonRed(1,open);
							break;
						case CT_pressure_hold:
							ButtonClear(1);
							break;
						case CT_pressure_down:
							ButtonRed(1,close);
							break;
						default:							
									break;
					}
				}
			
			}
				break;
}
}



void ETH_StatusResponse(uint8_t type)
{
		switch(type)
		{
			case CT_push_open:
				Button_flag = 0;
				Key_respstatus_data |= 0x04;
				break;
			case CT_push_close:
				Button_flag = 0;
				Key_respstatus_data &= ~0x04;
				break;
			case CT_bit_front:
				Button_flag = 0;
				Key_respstatus_data |= 0x02;
				break;
			case CT_bit_back:
				Button_flag = 0;
				Key_respstatus_data &= ~0x02;
				break;
			case CT_motor_run:
				Button_flag = 0;
				Key_respstatus_data |= 0x01;
				break;
			case CT_motor_stop:
				Button_flag = 0;
				Key_respstatus_data &= ~0x01;
				break;
			case CT_gama_open:
				Button_flag = 0;
				Key_respstatus_data |= 0x08;
				break;
			case CT_gama_close:
				Button_flag = 0;
				Key_respstatus_data &= ~0x08;
				break;
			case CT_pressure_up:
				Button_flag = 0;
				Key_respstatus_data |= 0x10;
				Key_respstatus_data &= ~0x20;
				break;
			case CT_pressure_hold:
				Button_flag = 0;
				Key_respstatus_data  &= ~0x10;
				Key_respstatus_data  &= ~0x20; 
				break;
			case CT_pressure_down:
				Button_flag = 0;
				Key_respstatus_data |= 0x20;
				Key_respstatus_data &= ~0x10; 
				break;
			default:
						break;
		}
}

/******************************************************************************
 * 函  数：Key_NoresponseRefresh_Colorblock
 * 功  能：井下未回复，按键色块变红
 * 参  数：按键反馈状态   
 * 返回值：无
*******************************************************************************/
void Key_NoresponseRefresh_Colorblock(uint8_t _status)
{
	if(motor_flag == 1)///色块刷新标志，，，上位机不需要
		{
			motor_flag = 0;
			if((_status & 0x01)== 1)//马达开
				 {
						ButtonRed(4,open);
				 }
			else//马达关
				{
						ButtonRed(4,close);
				}
		}
	if((bit_process_flag == 1)&&(flag_yellow == 1))//黄色不会变红，绿色变红
		{
			bit_process_flag = 0;
			if((_status & 0x02)== 0x02)//钻进开
					 {
							ButtonRed(3,open);
					 }
				else//钻进关
					 {
							ButtonRed(3,close);
					 }
		}
		if(push_flag == 1)
			{
				push_flag = 0;
				if((_status & 0x04) == 0x04)//推靠开
					 {						
							ButtonRed(2,open);					
					 }
				else//推靠关
					 {
							ButtonRed(2,close);
					 }	
			}
		if(gama_flag == 1)
			{
			gama_flag = 0;	
			if((_status & 0x08)== 0x08)//伽马开
					 {
							ButtonRed(0,open);
					 }
			else//伽马关
					 {
							ButtonRed(0,close);
					 }
			}
			if(pressuer_flag == 1)
			{
				pressuer_flag = 0;
				if(((_status & 0x10) == 0x10)&&((_status & 0x20) == 0))//压力升
					 {
							ButtonRed(1,open);
					 }
				else if(((_status & 0x20) == 0x20)&&((_status & 0x10) == 0))//压力降
					 {
							ButtonRed(1,close);
					 }
				else//压力保持，色块全清
						{
								ButtonClear(1);
						}
			}
}	

uint8_t Torque_buf[15] = {0};
uint8_t H_buf[15] = {0};
uint8_t L_buf[15] = {0};
uint32_t tor_data,tspe_data,power_data;
uint8_t tor_value[4],tspe_value[4],power_value[4];
uint8_t tor_order,tspe_order,power_order;
float tor,tspe,power;
uint16_t tor_uphole,tspe_uphole;
void Torque_acq(void)
{
	for(uint8_t i = 0;i < 15;i++)
	{
		Fpga_Write(0x03,i+1);
		send_data.tor_uphole[i] = Fpga_Read(0x02);
		Torque_buf[i] = Fpga_Read(0x02);
	}
		Fpga_Write(0x04,0x80);
		Fpga_Write(0x04,0x00);
	
	for(uint8_t i = 0;i < 15;i++)
	{
		H_buf[i] = Torque_buf[i]/16;
		L_buf[i] = Torque_buf[i]%16;		
	}
	tor_data = H_buf[0]*10000000 + L_buf[0]*1000000 + H_buf[1]*100000 + L_buf[1]*10000 + H_buf[2]*1000 + L_buf[2]*100 + H_buf[3]*10 + L_buf[3];
	tspe_data = H_buf[5]*10000000 + L_buf[5]*1000000 + H_buf[6]*100000 + L_buf[6]*10000 + H_buf[7]*1000 + L_buf[7]*100 + H_buf[8]*10 + L_buf[8];
	power_data = H_buf[10]*10000000 + L_buf[10]*1000000 + H_buf[11]*100000 + L_buf[11]*10000 + H_buf[12]*1000 + L_buf[12]*100 + H_buf[13]*10 + L_buf[13];
	
	tor_order = 7 - L_buf[4];
	tor = tor_data/ pow(10,tor_order);
	tor_uphole = tor*10000;

	tspe_order = 7 - L_buf[9];
	tspe = tspe_data/ pow(10,tspe_order);
	tspe_uphole = tspe*10;
	
	power_order = 7 - L_buf[14];
	power = power_data/ pow(10,tor_order);
	
	
}	

uint8_t DoseResend = 0;
void send_key_cmd(void)
{
	if(Push_open_flag == 1)///按键扫描到标志位1，等到井下接受完，再下发
	{
		push_flag = 0;
		Push_open_flag = 0;
		CmdSendToUnderGround(CT_push_open);
		DoseResend = CT_push_open;
		
	}
	if(Push_close_flag == 1)
	{
		push_flag = 0;
		Push_close_flag = 0;
		CmdSendToUnderGround(CT_push_close);
		DoseResend = CT_push_close;
	}
	if(bit_process_open_flag == 1 && start_push == 1)
	{
		bit_process_flag = 0;
		bit_process_open_flag = 0;
		CmdSendToUnderGround(CT_bit_front);
		DoseResend = CT_bit_front;
	}
	if(bit_process_close_flag == 1 && start_push == 1)
	{
		bit_process_flag = 0;
		bit_process_close_flag = 0;
		CmdSendToUnderGround(CT_bit_back);
		DoseResend = CT_bit_back;
	}
	if(motor_open_flag == 1)
	{
		motor_flag = 0;
		motor_open_flag = 0;
		CmdSendToUnderGround(CT_motor_run);
		DoseResend = CT_motor_run;
	}
	if(motor_close_flag == 1)
	{
		motor_flag = 0;
		motor_close_flag = 0;
		CmdSendToUnderGround(CT_motor_stop);
		DoseResend = CT_motor_stop;
	}
	if(gama_open_flag == 1)
	{
		gama_flag = 0;
		gama_open_flag = 0;
		CmdSendToUnderGround(CT_gama_open);
		DoseResend = CT_gama_open;
	}
	if(gama_close_flag == 1)
	{
		gama_flag = 0;
		gama_close_flag = 0;
		CmdSendToUnderGround(CT_gama_close);
		DoseResend = CT_gama_close;
	}
	if(pressure_up_flag == 1)
	{
		pressuer_flag = 0;
		CmdSendToUnderGround(CT_pressure_up);
		DoseResend = CT_pressure_up;
	}

	if(pressure_keep_flag == 1)
	{
		pressure_up_flag = 0;////按键弹起时候，禁止上一包数据UP或DOWN下发，会导致井下 数据接收不正确
		pressure_down_flag = 0;
		pressuer_flag = 0;
		pressure_keep_flag = 0;
		CmdSendToUnderGround(CT_pressure_hold);
		DoseResend = CT_pressure_hold;
	}
	if(pressure_down_flag == 1)
	{
		pressuer_flag = 0;
		CmdSendToUnderGround(CT_pressure_down);
		DoseResend = CT_pressure_down;
	}
}

