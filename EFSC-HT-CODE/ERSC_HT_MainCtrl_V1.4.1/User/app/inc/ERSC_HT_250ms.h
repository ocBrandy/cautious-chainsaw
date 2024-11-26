

#include "bsp.h"

typedef enum
{
    CT_mode_sw_ERSC = 0,
    CT_mode_sw_MRCT,//1
    CT_motor_run ,//2
    CT_motor_stop,//3
    CT_bit_front,//4
    CT_bit_back,//5
    CT_push_open,//6
    CT_push_close,//7
    CT_pressure_up,    // 8
    CT_pressure_hold, // 9
    CT_pressure_down, // a
    CT_gama_open,
    CT_gama_close,
   
}e_cmd_type;
