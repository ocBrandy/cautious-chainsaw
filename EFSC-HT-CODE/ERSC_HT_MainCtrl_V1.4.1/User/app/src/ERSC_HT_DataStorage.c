#include "bsp.h"
#include "ff.h"			/* FatFS文件系统模块*/
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
extern void MotorCycle_FWD(void);//正转
extern void MotorCycle_REV(void);//反转
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
void Downdata_storage(void) //井下数据接收   串口5 19200 PD2 PC12
{
	uint8_t ucData;
	if(comGetChar(COM5,&ucData))
	{
		timerout = 0;
		timerout_resetLCD = 0;
		resetLCD_flag = 0;	
		if(ucData == 0xAA &&	header_flag == 0)  //识别到帧头，将数据缓存
		{			
			Downdata_count = 0;
			header_flag  =  1;								
			Downdata_buf[Downdata_count] = ucData;
			Downdata_count++;
		}

		else if(header_flag == 1)//找到帧头
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
		if(timerout >= 300 && lcd_flag == 1)///超时时间到,超时计数发送给屏幕
			{
				timerout = 0;
				DH_data_timer++;//不用清零
				
				if((motor_flag == 1) ||(bit_process_flag == 1)||(push_flag == 1)||(gama_flag == 1)||(pressuer_flag == 1))
					{
						Button_flag = 0;
						Key_NoresponseRefresh_Colorblock(Key_status);		///超时井下未回复，变红色		
					}				
			}		
		if(timerout_resetLCD >= 1000)////超时1S,清屏
			{
				timerout_resetLCD = 0;	
				for(uint8_t i = 0;i<35;i++)//清空井下数组
				{
					Downdata_buf[i] = 0;
				}		
			}
	/*数据解析*/		
	if(Downdatarecv_flag == 1)
	{	
		//comSendBuf(COM1,&Downdata_buf[16],2);
		header_flag = 0;
		downdatabuf_clear = 1;
		uint8_t  sum = 0;
		Downdatarecv_flag = 0;
		sum = CheckSum(Downdata_buf,Downdata_count - 2) + 0x55;
		if(sum == Downdata_buf[Downdata_count - 2])////判断校验和   Downdata_count=35  校验和在 第33字节
			{				
			//井下数据给上位机
				if(uptemperature > 0 && (Downdata_buf[1] - uptemperature >= 8))//ds18b20init>=2000 && 
				{
				Downdata_buf[1] = uptemperature;
				}
				uptemperature = Downdata_buf[1];

				for(uint8_t i= 0;i < 35;i++)//将数据先缓存
					{
						BP_RequstBuf[i] = Downdata_buf[i];			
					}
				Key_response_status = Downdata_buf[Downdata_count - 10];//按键回复状态
				if((DoseResend != Key_response_status)&&((Key_response_status != CT_pressure_up)||(Key_response_status != CT_pressure_hold)||(Key_response_status != CT_pressure_down)))	//重发
				{			
					if(resend_cnt < 2)//1次
					{
						CmdSendToUnderGround(DoseResend);//重发
						resend_cnt++;
					}
				}
							
					
				if(Key_disable_flag == 0)///上位机失能，按键使能,,,才会刷新色块
					{
						KeyStatusResponse(Key_response_status);//刷新色块（深蓝色/红色）		
					}				
				else
					{
						ETH_StatusResponse(Key_response_status);//更新状态
					}

				if(Key_response_status == CT_push_open)///代表井下开推靠，开始计时
					{	
						if(cmd_back > 1000)
						{
							cmd_back = 0;
							timer = 0;
							push_timer = 0;
						}
						start_push = 1;	///定时器中断里，开始计时						
					}				
				else if(Key_response_status == CT_push_close)//代表井下关推靠，停止计时,刷新屏幕
					{
						start_push = 0;
					}
				else if(Key_response_status == CT_pressure_up)//压力升，转数++
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
				else if(Key_response_status == CT_pressure_down)//压力降，转数--
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
						Motor_status_flag = 1;	//收到马达反馈指令，置标志位（需判断变频机箱是否在发数据）
					}	
					
				if(eth_ctrl == 1 &&((_cmd == CT_motor_run)||(_cmd == CT_motor_stop)||(_cmd == CT_bit_front)||(_cmd == CT_bit_back)
					||(_cmd == CT_push_open)||(_cmd == CT_push_close)||(_cmd == CT_pressure_up)||(_cmd == CT_pressure_hold)
					||(_cmd == CT_pressure_down)||(_cmd == CT_gama_open)||(_cmd == CT_gama_close)))	////上位机按键发送到井下
					{
						ETH_DistStatusUpdata(_cmd);///按键下发指令更新					
						send_key_cmd();//按键命令发送到井下
					}	
				else
					{
						send_key_cmd();//按键命令发送到井下，
					}
					
					
					
					
			}
		else          ///误码计数加
		{	
			Error_timer++;//误码计数上传给屏幕
			for (uint8_t i = 0;i < Downdata_count;i++)           //清空接收数组,重新接收
				{
					 Downdata_buf[i] = 0;                                  
				}
				Downdata_count = 0;		
		}	
	}					
}



/*****************************************************************************
 * 函  数：CheckSum
 * 功  能：计算校验和
 * 参  数：_buf:需要计算的数据buf
 *		   _len：计算的数据长度
 * 返回值：校验和的低位字节
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
