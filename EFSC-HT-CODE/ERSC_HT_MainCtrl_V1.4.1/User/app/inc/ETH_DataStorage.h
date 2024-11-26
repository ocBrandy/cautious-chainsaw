#ifndef ETH_DATASTORAAGE_H
#define ETH_DATASTORAAGE_H

#include "bsp.h"
#include "BP_DataStorage.h"

#define ETH_MAX_RECV_LENGTH 100
#define ENABLE_UPPER_CTRL           0xF0
#define DISABLE_UPPER_CTRL          0xF5
	
#define ETH_SEND_LENGTH           ((uint16_t)(sizeof(s_eth_send)/sizeof(uint8_t)))

void ETH_data_storage(void);
void senddatatoETH(void);
void CmdSendToUnderGround(uint16_t cmd);
void byteBufInit(uint8_t *p_buf,uint16_t length,uint8_t init_value);

#pragma pack(1)
typedef struct
{
    uint16_t error_status;//故障状态
    uint16_t set_freq;    //设定频率
    uint16_t real_freq;   //运行频率
    uint16_t ab_voltage;  //AB相电压
    uint16_t ab_current;  //AB相电流
    uint16_t n_current;   //N相电流
    uint16_t BP_temperature; //变频机箱的温度
		uint16_t motor_current;  //电机电流（地面主控采集）
	  uint16_t ctrl_current; //控制电流（地面主控采集）
		uint16_t UDmotor;      //计数的电机输出电压 有效值
		uint16_t Cureent_proetect;//三相最大瞬时电流值
		uint16_t Ad_ParmResult_Vdc;//380VAC输入电压稳定后的直流电压实测值
		uint16_t Ad_ParmResult_Vac;//输入交流电压有效值
		uint16_t Ad_ParmLimitR;//设定电缆电阻值
		uint16_t Ad_ParmLimitLength;//电缆长度值
		uint16_t LogicStates;//电缆长度值
}ext_data;

/*上传 到上位机*/
typedef struct
{
    uint8_t sync_header[4];    //帧头50*4
    uint16_t data_type;        // 0x1201
    uint16_t data_length;      //长度0x3a
    uint8_t reserve[4];        //预留
    uint16_t response;         //cmd回应
    
    uint8_t temperature;  // 胆内温度
    uint16_t gmma_cnt;    //伽马计数
		uint16_t cable_head_voltage;          // 交流缆头电压
    uint16_t tension;                     // 张力
    uint8_t drill_bit_process;           // 钻进位移
    uint16_t AC_voltage_of_underground_motor; // 大电机交流电压
		uint8_t Pump_Oil_temperature;    //泵液压油温度  
    uint8_t Cable_Head_Tension_temperature;   // 3981温度        
    uint8_t elect_ext_temperature;   // 电子节外部温度  
		uint8_t Stepper_motor_temperature;   //步进电机驱动温度    
    uint8_t Small_Motor_temperature;  //液压节电机温度 
    uint16_t push_pressure;          //推靠压力      
		uint16_t large_pump_pressure;      //大泵压力  	
    uint16_t small_pump_pressure;   //小泵压力                       
    uint8_t core_length;                //  芯长位移
		uint16_t oil_position;// 油位位移
	
    uint8_t resp_type;   //回应标志
    ext_data data;  //故障状态....逻辑运行状态	

	uint8_t tor_uphole[15];
	uint16_t AC_voltage_of_floor_motor;
    uint8_t check_sum;
	
}s_eth_send;

#pragma pack()
#endif
