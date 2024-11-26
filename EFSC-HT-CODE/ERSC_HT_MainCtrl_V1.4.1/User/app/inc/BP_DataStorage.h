#include "bsp.h"

#define  BP_MAX_RECV_LENGTH     100
#define  KEYCMD_LENGTH          8

#define  MODBUS_READ_REG     0x03
#define  MODBUS_WR_REG       0x06
#define 	REG_NUMBER               17
#define  MD_DIST_REG         0x2311
#define  MD_RESPONSE_REG         0x2312


void BP_data_storage(void);
void Sendkeycmd(uint16_t reg_value,uint16_t stauts); 
void Send_datatoBP(void);


