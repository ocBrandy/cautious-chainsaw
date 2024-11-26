#include "bsp.h"
#include "ETH_DataStorage.h"
s_eth_send  send_data;
void CmdSendToUnderGround(uint16_t cmd);
extern uint16_t CheckSum(uint8_t *_buf, uint16_t _len);
extern uint16_t Key_respstatus_data,_cmd;
 extern uint8_t BP_databuf[BP_MAX_RECV_LENGTH];
extern uint8_t Downdata_buf[35];
static uint16_t Pressurecalculation(uint16_t src);
uint8_t ETHdata_buf[ETH_MAX_RECV_LENGTH] = {0};
uint8_t CmdSendtoUnder_buf[4] = {0};
uint8_t eth_ctrl = 0,send_cmd = 0;
uint8_t Key_disable_flag = 0;
uint8_t ETH_datarecv_flag = 0;
uint16_t ETHdata_count = 0;
uint8_t isFindHead = 0;
uint8_t count = 0;
uint16_t eth_cmd = 0;
uint8_t test_cnt= 0;
extern uint8_t downdatabuf_clear,BPdatabuf_clear; 
extern uint16_t ctrl_current,Motor_Current,Motor_Voltage;
/****************************************/
void ETH_data_storage(void) //以太网数据接收   串口1 115200 PA9 PA10
{
	uint8_t ucData;
	if(comGetChar(COM1,&ucData))
	{	
		
		if(isFindHead == 0)  //寻找帧头
		{
			if(ucData == 0x50)  //帧头*4
				{
					ETHdata_buf[ETHdata_count++] = ucData;
					count++;	
					if(count >= 4) //帧头判断完成继续接收数据
					{
						count = 0;
						isFindHead = 1;					
					}
				}		
			else
				{
					count = 0;
				}
		}
		else  //找到帧头
		{
			ETHdata_buf[ETHdata_count++] = ucData;//存储剩余数据
			if(ETHdata_count >= ETH_MAX_RECV_LENGTH)  //数组溢出重新接收
				{
					ETHdata_count = 0;
					count	= 0;
					isFindHead = 0;	
				}
				if(ETHdata_count == ETHdata_buf[7] + 12)//数据接收完成
				{
					if(ETHdata_buf[7] == 0)
					{
						ETHdata_count = 0;
						count	= 0;
						isFindHead = 0;			
					}
					else
					{
						ETH_datarecv_flag = 1;
						isFindHead = 0;
						ETHdata_count = 0;
						count	= 0;	
					}
									
				}
		}
		
	}
/*接收上位机数据完成，数据解析*/
if(ETH_datarecv_flag == 1)
{
		ETH_datarecv_flag = 0;
	  _cmd	= (uint16_t)((ETHdata_buf[12] << 8)& 0xFF00) + ETHdata_buf[13];
		eth_cmd = _cmd;//上传给上位机
		if(eth_cmd == ENABLE_UPPER_CTRL)//使能上位机主控
		{
		  Key_disable_flag = 1;//按键扫描失能
			eth_ctrl  = 1;//主控使能
		}
		if(eth_cmd == DISABLE_UPPER_CTRL)//失能上位机主控
		{
			Key_disable_flag = 0;//按键扫描使能
			eth_ctrl  = 0;//主控失能
		}
	}
}	


uint16_t TwoBytesReverse(uint16_t src)
{
	uint8_t high,low;
	low = (uint8_t)src;
	high = (uint8_t)(src >> 8);
	return (high + ((uint16_t)(low<<8)));
}
uint8_t  sendtoLCD[90] ={0};
uint8_t clear_cnt = 0,BP_clear_cnt = 0;
void senddatatoETH(void)
{
	byteBufInit(send_data.sync_header,4,0x50);
	send_data.data_type = TwoBytesReverse(0x1201);
	send_data.data_length  = TwoBytesReverse(ETH_SEND_LENGTH - 12);
  byteBufInit(send_data.reserve,4,0);//预留位
  send_data.response     = TwoBytesReverse(eth_cmd); //	cmd回应
	send_data.resp_type    = Key_respstatus_data;//回应标志
	///ADC采集数据
	send_data.data.ctrl_current = ctrl_current;
	send_data.data.motor_current = Motor_Current;
	send_data.AC_voltage_of_floor_motor = Motor_Voltage;
	///井下数据
	
	if(downdatabuf_clear == 1)
	{
		clear_cnt = 0;
		send_data.temperature                       = Downdata_buf[1];//保温瓶温度				
		send_data.gmma_cnt                          = (Downdata_buf[3] << 8)|(Downdata_buf[2]);//伽马计数
		send_data.cable_head_voltage                = (Downdata_buf[5] << 8)|(Downdata_buf[4]);//缆头电压
		send_data.tension                           = (Downdata_buf[7] << 8)|(Downdata_buf[6]);//张力
		send_data.drill_bit_process                 = Downdata_buf[8];//钻进
		send_data.AC_voltage_of_underground_motor   = (Downdata_buf[10] << 8)|(Downdata_buf[9]);//
		send_data.Stepper_motor_temperature         = Downdata_buf[11];
		send_data.Cable_Head_Tension_temperature    = Downdata_buf[12];
		send_data.elect_ext_temperature             = Downdata_buf[13];
		send_data.Small_Motor_temperature           = Downdata_buf[14];
		send_data.Pump_Oil_temperature              = Downdata_buf[15];
		send_data.push_pressure                     = 0;//Pressurecalculation((Downdata_buf[17] << 8)|(Downdata_buf[16]));
		send_data.large_pump_pressure               = 0;//Pressurecalculation((Downdata_buf[19] << 8)|(Downdata_buf[18]));
		send_data.small_pump_pressure               = Pressurecalculation((Downdata_buf[21] << 8)|(Downdata_buf[20]));	
		send_data.core_length   										= Downdata_buf[22]/3.41333;
		send_data.oil_position                      = (Downdata_buf[24] << 8)|(Downdata_buf[23]);
	}
	else ///井下无数据，清零
	{		
		clear_cnt++;
		if(clear_cnt >= 2)
		{
			clear_cnt = 0;
			send_data.temperature                       = 0;//保温瓶温度				
			send_data.gmma_cnt                          = 0;//伽马计数
			send_data.cable_head_voltage                = 0;//缆头电压
			send_data.tension                           = 0;//张力
			send_data.drill_bit_process                 = 0;//钻进
			send_data.AC_voltage_of_underground_motor   = 0;//
			send_data.Stepper_motor_temperature         = 0;
			send_data.Cable_Head_Tension_temperature    = 0;
			send_data.elect_ext_temperature             = 0;
			send_data.Small_Motor_temperature           = 0;
			send_data.Pump_Oil_temperature              = 0;
			send_data.push_pressure                     = 0;
			send_data.large_pump_pressure               = 0;
			send_data.small_pump_pressure               = 0;	
			send_data.core_length   									  = 0;
			send_data.oil_position                      = 0;	
			for(uint8_t i = 0;i<35;i++)
			{
				Downdata_buf[i] = 0;
			}		
		}
			
	}
		 //数据填充到上位机
	
	if(BPdatabuf_clear == 1)
	{	
			BP_clear_cnt = 0;
		send_data.data.error_status         = (BP_databuf[3] << 8)|(BP_databuf[4]);//故障信息
		send_data.data.ab_voltage           = (BP_databuf[5] << 8)|(BP_databuf[6]);//AB相电压
		send_data.data.UDmotor              = (BP_databuf[7] << 8)|(BP_databuf[8]);//电机输出电压有效值
		send_data.data.Cureent_proetect     = (BP_databuf[9] << 8)|(BP_databuf[10]);//三相最大瞬时电流值 
		send_data.data.set_freq             = (BP_databuf[11] << 8)|(BP_databuf[12]);//电频机箱设定频率
		send_data.data.real_freq            = (BP_databuf[13] << 8)|(BP_databuf[14]);//电频机箱运行频率
		send_data.data.BP_temperature       = (BP_databuf[15] << 8)|(BP_databuf[16]);//电频机箱的温度
		send_data.data.n_current            = (BP_databuf[17] << 8)|(BP_databuf[18]);// N相电流
		send_data.data.ab_current           = 0;  //变频机箱没有AB相电流
		send_data.data.Ad_ParmResult_Vdc    = (BP_databuf[19] << 8)|(BP_databuf[20]);//直流电压实测值
		send_data.data.Ad_ParmResult_Vac    = (BP_databuf[21] << 8)|(BP_databuf[22]);//交流电压有效值
		send_data.data.Ad_ParmLimitR  			 = (BP_databuf[23] << 8)|(BP_databuf[24]);//设定电缆电阻值
		send_data.data.Ad_ParmLimitLength   = (BP_databuf[25] << 8)|(BP_databuf[26]);//电缆长度值
		send_data.data.LogicStates    			 = (BP_databuf[35] << 8)|(BP_databuf[36]);//运行逻辑状态	
	}
	else if(BPdatabuf_clear == 0)//BP没有数据
	{
		BP_clear_cnt++;
		if(BP_clear_cnt >= 2)
		{
			BP_clear_cnt = 0;
		send_data.data.error_status         = 0;//故障信息
		send_data.data.ab_voltage           = 0;//AB相电压
		send_data.data.UDmotor              = 0;//电机输出电压有效值
		send_data.data.Cureent_proetect     = 0;//三相最大瞬时电流值 
		send_data.data.set_freq             = 0;//电频机箱设定频率
		send_data.data.real_freq            = 0;//电频机箱运行频率
		send_data.data.BP_temperature       = 0;//电频机箱的温度
		send_data.data.n_current            = 0;// N相电流
		send_data.data.ab_current           = 0;  //变频机箱没有AB相电流
		send_data.data.Ad_ParmResult_Vdc    = 0;//直流电压实测值
		send_data.data.Ad_ParmResult_Vac    = 0;//交流电压有效值
		send_data.data.Ad_ParmLimitR  			 = 0;//设定电缆电阻值
		send_data.data.Ad_ParmLimitLength   = 0;//电缆长度值
		send_data.data.LogicStates    			 = 0;//运行逻辑状态	
		for(uint8_t i = 0;i < BP_MAX_RECV_LENGTH;i++)
		{
			BP_databuf[i] = 0;
		}
		}
	}	
							
		send_data.check_sum = CheckSum((uint8_t *)&send_data,ETH_SEND_LENGTH - 1);
		//comSendBuf(COM1,(uint8_t *)&send_data,ETH_SEND_LENGTH);///打包数据，发送给上位机
		BPdatabuf_clear = 0;
		downdatabuf_clear = 0;
}

/******************************************************************************
 * 函  数：Pressurecalculation
 * 功  能：描点计算压力值,压力温度补偿
 * 参  数：src:需要计算的数据	   
 * 返回值：无
*******************************************************************************/		
static uint16_t Pressurecalculation(uint16_t src)//大泵压力，小泵压力，推靠压力计算
	{
		long temp = src;
		comSendChar(COM1,temp>>8);
		comSendChar(COM1,temp);
		temp = temp*5000;
		temp = temp/65535;
		comSendChar(COM1,temp);
		temp = temp/4;
		temp = temp/0.94775;
		/*
		temp -= 270;
		temp /= 46.8;
		uint8_t plo;
		uint16_t pensa;
//		if(temp > 150)
//		{
//			temp = 0;
//		}
		if(Downdata_buf[1] > 60&&Downdata_buf[1]<=65) //对井下仪表放大器的温漂进行补偿
		{
			plo = 66-Downdata_buf[1];
			pensa = 1+(5-plo+1)*0.2;
			if(temp>pensa)
			{
				temp = temp - pensa;
			}
			else
			{
				temp = 0;
			}
		}
		if(Downdata_buf[1] > 65&&Downdata_buf[1]<=75)
		{
			plo = 76-Downdata_buf[1];
			pensa = 2+((10-plo+1)*0.3);
			if(temp>pensa)
			{
				temp = temp - pensa;
			}
			else
			{
				temp = 0;
			}
		}
		if(Downdata_buf[1] > 75&&Downdata_buf[1]<=80)
		{
			plo = 81-Downdata_buf[1];
			pensa = 5+((5-plo+1)*0.2);
			if(temp>pensa)
			{
				temp = temp - pensa;
			}
			else
			{
				temp = 0;
			}
		}
		if(Downdata_buf[1] > 80&&Downdata_buf[1]<=85)
		{
			plo = 86-Downdata_buf[1];
			pensa = 6+((5-plo+1)*0.3);
			if(temp>pensa)
			{
				temp = temp - pensa;
			}
			else
			{
				temp = 0;
			}
		}
		if(Downdata_buf[1] > 85&&Downdata_buf[1]<=90)
		{
			plo = 91-Downdata_buf[1];
			pensa = 10+((5-plo+1)*0.3);
			if(temp>pensa)
			{
				temp = temp - pensa;
			}
			else
			{
				temp = 0;
			}
		}
		if(Downdata_buf[1] > 90&&Downdata_buf[1]<=95)
		{
			plo = 96-Downdata_buf[1];
			pensa = 12+((5-plo+1)*0.3);
			if(temp>pensa)
			{
				temp = temp - pensa;
			}
			else
			{
				temp = 0;
			}
		}
		if(Downdata_buf[1] > 95&&Downdata_buf[1]<=100)
		{
			plo = 101-Downdata_buf[1];
			pensa = 15+((5-plo+1)*0.4);
			if(temp>pensa)
			{
				temp = temp - pensa;
			}
			else
			{
				temp = 0;
			}
		}
		if(Downdata_buf[1] > 100&&Downdata_buf[1]<=105)
		{
			plo = 106-Downdata_buf[1];
			pensa = 19+((5-plo+1)*0.5);
			if(temp>pensa)
			{
				temp = temp - pensa;
			}
			else
			{
				temp = 0;
			}
		}
		if(Downdata_buf[1] > 105&&Downdata_buf[1]<=110)
		{
			plo = 111-Downdata_buf[1];
			pensa = 24+((5-plo+1)*0.7);
			if(temp>pensa)
			{
				temp = temp - pensa;
			}
			else
			{
				temp = 0;
			}
		}
		if(Downdata_buf[1]>110)
		{
			temp = temp - 31;
		}
		
		
//////		if(temp>1200)
//////		{temp = 0;}
		*/
		return temp;
	}	

/**************************************************************************
* 函数:CmdSendToUnderGround
 * 功能:发送按键命令给井下 
 * 参数:_buf:数据缓存区
 * _size:数据长度
 * 返回值:无
***************************************************************************/
uint8_t Button_flag = 0;
uint16_t commd_reg = 0;
extern uint8_t commd_response_cnt;
void CmdSendToUnderGround(uint16_t cmd)
{
	commd_response_cnt = 0;
	commd_reg = cmd;
	Button_flag = 1;///按键动作标志
  CmdSendtoUnder_buf[0]= 0xAA;
	CmdSendtoUnder_buf[1]= (uint8_t)cmd;
	CmdSendtoUnder_buf[2]= (uint8_t)cmd >> 8;
	CmdSendtoUnder_buf[3]= 0x55;
	comSendBuf(COM5,CmdSendtoUnder_buf,4);
}	
void byteBufInit(uint8_t *p_buf,uint16_t length,uint8_t init_value)
{
	uint16_t i;
	if(length >= 0xffff) {return;}
	for(i = 0;i<length;i++)
	{
		p_buf[i] = init_value;
	}
}
