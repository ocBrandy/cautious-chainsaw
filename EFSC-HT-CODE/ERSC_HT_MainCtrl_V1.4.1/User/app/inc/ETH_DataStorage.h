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
    uint16_t error_status;//����״̬
    uint16_t set_freq;    //�趨Ƶ��
    uint16_t real_freq;   //����Ƶ��
    uint16_t ab_voltage;  //AB���ѹ
    uint16_t ab_current;  //AB�����
    uint16_t n_current;   //N�����
    uint16_t BP_temperature; //��Ƶ������¶�
		uint16_t motor_current;  //����������������زɼ���
	  uint16_t ctrl_current; //���Ƶ������������زɼ���
		uint16_t UDmotor;      //�����ĵ�������ѹ ��Чֵ
		uint16_t Cureent_proetect;//�������˲ʱ����ֵ
		uint16_t Ad_ParmResult_Vdc;//380VAC�����ѹ�ȶ����ֱ����ѹʵ��ֵ
		uint16_t Ad_ParmResult_Vac;//���뽻����ѹ��Чֵ
		uint16_t Ad_ParmLimitR;//�趨���µ���ֵ
		uint16_t Ad_ParmLimitLength;//���³���ֵ
		uint16_t LogicStates;//���³���ֵ
}ext_data;

/*�ϴ� ����λ��*/
typedef struct
{
    uint8_t sync_header[4];    //֡ͷ50*4
    uint16_t data_type;        // 0x1201
    uint16_t data_length;      //����0x3a
    uint8_t reserve[4];        //Ԥ��
    uint16_t response;         //cmd��Ӧ
    
    uint8_t temperature;  // �����¶�
    uint16_t gmma_cnt;    //٤�����
		uint16_t cable_head_voltage;          // ������ͷ��ѹ
    uint16_t tension;                     // ����
    uint8_t drill_bit_process;           // ���λ��
    uint16_t AC_voltage_of_underground_motor; // ����������ѹ
		uint8_t Pump_Oil_temperature;    //��Һѹ���¶�  
    uint8_t Cable_Head_Tension_temperature;   // 3981�¶�        
    uint8_t elect_ext_temperature;   // ���ӽ��ⲿ�¶�  
		uint8_t Stepper_motor_temperature;   //������������¶�    
    uint8_t Small_Motor_temperature;  //Һѹ�ڵ���¶� 
    uint16_t push_pressure;          //�ƿ�ѹ��      
		uint16_t large_pump_pressure;      //���ѹ��  	
    uint16_t small_pump_pressure;   //С��ѹ��                       
    uint8_t core_length;                //  о��λ��
		uint16_t oil_position;// ��λλ��
	
    uint8_t resp_type;   //��Ӧ��־
    ext_data data;  //����״̬....�߼�����״̬	

	uint8_t tor_uphole[15];
	uint16_t AC_voltage_of_floor_motor;
    uint8_t check_sum;
	
}s_eth_send;

#pragma pack()
#endif
