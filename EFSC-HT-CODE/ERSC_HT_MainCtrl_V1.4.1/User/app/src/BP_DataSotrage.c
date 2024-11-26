#include "bsp.h"
#include "ff.h"			/* FatFS文件系统模块*/
#include "BP_DataStorage.h"
#include "ERSC_HT_DataStorage.h"
#include "ETH_DataStorage.h"
#include "VGUS_LCD.h"
extern s_eth_send  send_data;
static unsigned short CRC16(unsigned char *puchMsg,unsigned int usDataLen);
////void Sendkeycmd(uint16_t reg_value);
void Send_datatoBP(void);
//ext_data ext_s_eth;//声明结构体变量
u_report u_data_report;//声明结构体变量
uint8_t BP_databuf[BP_MAX_RECV_LENGTH] = {0};	
extern uint8_t Downdata_buf[35];
uint8_t BPRxState = 0,BPdata_count = 0;
uint8_t BP_commd_flag = 0,BPdatarecv_flag = 0,BP_back_flag = 0,BPdatabuf_clear = 0;

uint8_t keycmd_buf[KEYCMD_LENGTH] = {0};
uint8_t repeat_dist_flag,repeat_response_flag;
extern uint8_t MD_commd_flag;
extern uint16_t Key_respstatus_data;
extern uint16_t Key_response_status;
extern uint8_t Key_status,MD_timer;
extern uint32_t BP_timeout;
/****************************************************/
			
void BP_data_storage(void) //变频机箱数据接收
{
	
	uint8_t ucData;
	if(comGetChar(COM2,&ucData))
	{
		BP_timeout = 0;
		switch(BPRxState)
			{
				/* 校验帧头1 */
				case 0:				
						if(ucData == 0x02)  //识别到地址位，将数据缓存
								{
									BP_databuf[BPdata_count++] = ucData;
									BPRxState++;		/* 跳转进入下一步 */
								}			
						else               //计数清零，重新接收
							 {
									BPdata_count = 0;
									BPRxState = 0;
							 }						
						break;
				case 1:				
						if(ucData == 0x03)  //识别到读寄存器，将数据缓存
								{
									BP_databuf[BPdata_count++] = ucData;
									BPRxState++;		/* 跳转进入下一步 */
								}
						else if	(ucData == 0x06)
								{
									BP_databuf[BPdata_count++] = ucData; 	//缓存剩余数据
								}							
						else               //计数清零，重新接收
							 {
									BPdata_count = 0;
									BPRxState = 0;
							 }						
						break;						 					 
				case 2:		
												
							BP_databuf[BPdata_count++] = ucData; 	//缓存剩余数据
				
							if(BPdata_count == BP_databuf[2]+5)
							{			
							
								BPdatarecv_flag = 1;	///接收完成							
							}							
							break;
				}
		}


if(BPdatarecv_flag == 1)
	{		
////		comSendBuf(COM1,BP_databuf,39);
		BPdatabuf_clear = 1;
		uint16_t crc = 0;
		BP_commd_flag = 0;//变频机箱接收数据完成，允许按键指令下发
		BPdatarecv_flag = 0;

		crc = CRC16(BP_databuf,BPdata_count - 2);
		if(crc == ((BP_databuf[BPdata_count - 2] << 8)|(BP_databuf[BPdata_count - 1])))///校验和判断正确
			{					
				uint16_t requst_flag;			

				BPdata_count = 0;
				BPRxState = 0 ;
				requst_flag = (BP_databuf[27] << 8) | BP_databuf[28];
				if(requst_flag == 1 )///变频机箱是否有数据请求
					{			
						requst_flag = 0;
						Send_datatoBP();//井下数据等传给变频机箱			
					}
					
			}
		else  //校验和有误，重新接收
			{
				for (uint8_t i = 0;i < BPdata_count;i++)           //清空接收数组,重新接收
					{
						 BP_databuf[i] = 0;                                  
					}
					BPRxState = 0;
					BPdata_count = 0;
			}
			
	}
}
/***********************************************************************
*	函 数 名: Sendkeycmd
*	功能说明: 扫描到按键动作，立即下发马达状态给变频机箱
*	形    参：马达开关状态
*	返 回 值: 无
************************************************************************/
void Sendkeycmd(uint16_t reg_value,uint16_t stauts) 
{
	uint8_t len = 0;
	keycmd_buf[len++] = 0x02;
	keycmd_buf[len++] = MODBUS_WR_REG;
	keycmd_buf[len++] = reg_value >> 8;
	keycmd_buf[len++] = (uint8_t)reg_value;
	keycmd_buf[len++] = stauts >> 8;
	keycmd_buf[len++] = (uint8_t)stauts;
	keycmd_buf[len++] = CRC16(keycmd_buf,KEYCMD_LENGTH - 2) >> 8;
	keycmd_buf[len++] = (uint8_t)CRC16(keycmd_buf,KEYCMD_LENGTH - 2);
	comSendBuf(COM2,keycmd_buf,len);
	for(uint8_t i = 0;i < len;i++)		//发送完成清空数组
		{
			keycmd_buf[i] = 0;			
		}
}
/***********************************************************************
*	函 数 名: Data_request
*	功能说明: 请求变频 机箱数据
*	形    参：无
*	返 回 值: 无
************************************************************************/
uint8_t BP_send_buf[BP_MAX_RECV_LENGTH] = {0};
void Data_request(void)
{
	
		uint8_t len = 0;
    BP_send_buf[len++] = 0x02;
    BP_send_buf[len++] = MODBUS_READ_REG;
    BP_send_buf[len++] = 0x23;//寄存器地址
		BP_send_buf[len++] = 0x00;
    BP_send_buf[len++] = REG_NUMBER >> 8;//读取寄存器个数
		BP_send_buf[len++] = REG_NUMBER;
    BP_send_buf[len++] = CRC16(BP_send_buf,6) >> 8;///CRC
		BP_send_buf[len++] = (uint8_t)CRC16(BP_send_buf,6);
	
		comSendBuf(COM2,BP_send_buf,len);
		MD_timer = 0;
		for(uint8_t i = 0;i < len;i++)		//发送完成清空数组
			{
				BP_send_buf[i] = 0;			
			}	
			BP_commd_flag = 1;//置位标志，不允许按键指令发送	
////		for(uint8_t i = 0;i<BP_MAX_RECV_LENGTH;i++)//清空BP数组
////		{
////			BP_databuf[i] = 0;
////		}
////		for(uint8_t i = 0;i<35;i++)//清空井下数组
////		{
////			Downdata_buf[i] = 0;
////		}			
}	
/***********************************************************************
*	函 数 名: Send_datatoBP
*	功能说明: 收到变频机箱数据请求，发送井下数据给变频机箱
*	形    参：无
*	返 回 值: 无
************************************************************************/
extern uint8_t BP_RequstBuf[35];
void Send_datatoBP(void)
{

	uint8_t len = 0;
	BP_send_buf[len++] = 0x02;//地址位
	BP_send_buf[len++] = 0x10;//写寄存器
	BP_send_buf[len++] = 0x1C;//数据长度
	BP_send_buf[len++] = Key_status >> 8;//按键下发状态
	BP_send_buf[len++] = Key_status;
	
	BP_send_buf[len++] =BP_RequstBuf[9]; //(u_data_report.report.AC_voltage_of_underground_motor >> 8) & 0xFF;//井下大电机交流电压
	BP_send_buf[len++] =BP_RequstBuf[10];// u_data_report.report.AC_voltage_of_underground_motor & 0xFF;
	BP_send_buf[len++] = 0;//N相电流
	BP_send_buf[len++] = 0;
	BP_send_buf[len++] = BP_RequstBuf[14] >> 8;//(u_data_report.report.Small_Motor_temperature >> 8) & 0xFF;//液压节电机温度
	BP_send_buf[len++] = BP_RequstBuf[14];//u_data_report.report.Small_Motor_temperature;
	BP_send_buf[len++] = BP_RequstBuf[15] >> 8;//(u_data_report.report.Pump_Oil_temperature >> 8) & 0xFF;//泵液压油温度
	BP_send_buf[len++] = BP_RequstBuf[15];//u_data_report.report.Pump_Oil_temperature & 0xFF;
	BP_send_buf[len++] = BP_RequstBuf[12] >> 8;//(u_data_report.report.Cable_Head_Tension_temperature >> 8) & 0xFF;//3981温度
	BP_send_buf[len++] = BP_RequstBuf[12];//u_data_report.report.Cable_Head_Tension_temperature & 0xFF;
	BP_send_buf[len++] = BP_RequstBuf[20];//(u_data_report.report.small_pump_pressure >> 8) & 0xFF;//小泵压力
	BP_send_buf[len++] = BP_RequstBuf[21];//u_data_report.report.small_pump_pressure & 0xFF;
	BP_send_buf[len++] = BP_RequstBuf[18];//(u_data_report.report.large_pump_pressure >> 8) & 0xFF;//大泵压力
	BP_send_buf[len++] = BP_RequstBuf[19];//u_data_report.report.large_pump_pressure & 0xFF;
	BP_send_buf[len++] = BP_RequstBuf[23];//(u_data_report.report.oil_position >> 8) & 0xFF;//油位位移
	BP_send_buf[len++] = BP_RequstBuf[24];//u_data_report.report.oil_position & 0xFF;
	BP_send_buf[len++] = BP_RequstBuf[8] >> 8;//(u_data_report.report.drill_bit_process >> 8) & 0xFF;//钻进位移
	BP_send_buf[len++] = BP_RequstBuf[8];//u_data_report.report.drill_bit_process & 0xFF;
	for(uint8_t i = 0;i < 6;i++) //预留位
		{
			BP_send_buf[len++] = 0;	
		}
	BP_send_buf[len++] = (Key_respstatus_data >> 8) & 0xFF;//按键反馈状态
	BP_send_buf[len++] = Key_respstatus_data & 0xFF;
	BP_send_buf[len++] = CRC16(BP_send_buf,31) >> 8;///CRC
	BP_send_buf[len++] = (uint8_t)CRC16(BP_send_buf,31);
	comSendBuf(COM2,BP_send_buf,len);
	for(uint8_t i = 0;i < len;i++)		//发送完成清空数组
		{
			BP_send_buf[i] = 0;			
		}
}



//高位字节的CRC值表
static const unsigned char auchCRCHi[] =
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

//低位字节的CRC值表
static const unsigned char auchCRCLo[] =
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

static unsigned short CRC16(unsigned char *puchMsg,unsigned int usDataLen)
{
    unsigned char uchCRCHi = 0xFF ; 
    unsigned char uchCRCLo = 0xFF ; 
    unsigned uIndex ;				
    while (usDataLen--) 		
    {
        uIndex = uchCRCHi ^ *puchMsg++ ; 
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
        uchCRCLo = auchCRCLo[uIndex];
    }
    return  (uchCRCHi << 8 | uchCRCLo) ;
}
