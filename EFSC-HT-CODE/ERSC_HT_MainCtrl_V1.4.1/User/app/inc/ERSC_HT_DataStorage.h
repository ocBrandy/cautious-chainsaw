#ifndef ERSC_HT_DATASTORAAGE_H
#define ERSC_HT_DATASTORAAGE_H

#include "bsp.h"




void HT_250ms_Timerout(void);

void Downdata_storage(void);
			



/*井下数据上传*/
typedef struct
{
    uint8_t  head;
    uint8_t  temperature;          // 保温瓶温度 
    uint16_t gmma_cnt;
    uint16_t cable_head_voltage;          // 交流缆头电压
    uint16_t tension;                     // 张力
    uint8_t  drill_bit_process;           // 钻进位移
    uint16_t AC_voltage_of_underground_motor; // 大电机交流电压
		uint8_t  Stepper_motor_temperature;    // 步进电机驱动温度  4
    uint8_t  Cable_Head_Tension_temperature;   // 3981温度        5
    uint8_t  elect_ext_temperature;   // 电子节外部温度  1
		uint8_t  Small_Motor_temperature;   //  液压节电机温度 2
    uint8_t  Pump_Oil_temperature;  //  泵液压油温度   3
    uint16_t push_pressure;          //推靠压力      
		uint16_t large_pump_pressure;      //大泵压力  	
    uint16_t small_pump_pressure;   //小泵压力                       
    uint8_t  core_length;                //  芯长位移
		uint16_t oil_position;// 油位位移
    uint8_t  resp_type;
    uint8_t  reserve[7];
    uint8_t  check_sum;
}s_report;

typedef union{
    uint8_t BP_RequstBuf[35];
    s_report report;             
}u_report;

#endif





