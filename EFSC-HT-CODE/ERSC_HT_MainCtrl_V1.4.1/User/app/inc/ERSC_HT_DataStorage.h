#ifndef ERSC_HT_DATASTORAAGE_H
#define ERSC_HT_DATASTORAAGE_H

#include "bsp.h"




void HT_250ms_Timerout(void);

void Downdata_storage(void);
			



/*���������ϴ�*/
typedef struct
{
    uint8_t  head;
    uint8_t  temperature;          // ����ƿ�¶� 
    uint16_t gmma_cnt;
    uint16_t cable_head_voltage;          // ������ͷ��ѹ
    uint16_t tension;                     // ����
    uint8_t  drill_bit_process;           // ���λ��
    uint16_t AC_voltage_of_underground_motor; // ����������ѹ
		uint8_t  Stepper_motor_temperature;    // ������������¶�  4
    uint8_t  Cable_Head_Tension_temperature;   // 3981�¶�        5
    uint8_t  elect_ext_temperature;   // ���ӽ��ⲿ�¶�  1
		uint8_t  Small_Motor_temperature;   //  Һѹ�ڵ���¶� 2
    uint8_t  Pump_Oil_temperature;  //  ��Һѹ���¶�   3
    uint16_t push_pressure;          //�ƿ�ѹ��      
		uint16_t large_pump_pressure;      //���ѹ��  	
    uint16_t small_pump_pressure;   //С��ѹ��                       
    uint8_t  core_length;                //  о��λ��
		uint16_t oil_position;// ��λλ��
    uint8_t  resp_type;
    uint8_t  reserve[7];
    uint8_t  check_sum;
}s_report;

typedef union{
    uint8_t BP_RequstBuf[35];
    s_report report;             
}u_report;

#endif





