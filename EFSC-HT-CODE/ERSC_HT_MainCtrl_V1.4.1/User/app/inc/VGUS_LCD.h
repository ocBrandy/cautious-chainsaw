#ifndef VGUS_LCD_H
#define VGUS_LCD_H

#include "bsp.h"
#include "ff.h"			/* FatFS????????*/


#define X_Null              ((void*)0)
#define X_True              (0 == 0)
#define X_False             (0 != 0)
#define MAX_LCD_UART_CMD_LENGTH      100
#define CMD_WRITE_REG   0x82
#define RECT_FILL_CMD   0x0004



// value object addr 

#define TOR_UPHOLE                      0xFFD0 // ???
#define TSPE_UPHOLE                       0xFFE0 // ???


#define TEMPERATURE1_ADDR                        0x0810 // ??????????
#define TEMPERATURE2_ADDR                        0x0850 // ?????????
#define TEMPERATURE3_ADDR                        0x0840 // ?????????
#define TEMPERATURE4_ADDR                        0x0830 // ??????????????
#define TEMPERATURE5_ADDR                        0x0570 // 3981???
#define TEMPERATURE_BOARD_ADDR                   0x0820 // ???????????
        
#define MOTOR_CYCLE_ADDR                         0x0110     //?????????? 
#define VARIABLE_FREQ_ADDR                       0xFF00 //0x0104     //??????
#define VARIABLE_FREQ_MACHINE_BOX_TEMPERATURE_ADDR           0x3000    //??????????    
        
#define CABLE_CURRENT_ADDR                       0x0860 //???????
#define MOTOR_CURRENT_ADDR                       0x0250 //???????
#define CABLE_VOLTAGE_ADDR                       0x0630 //??????
#define AC_VOLTAGE_ADDR                          0x0620  //???¦Ì?????                                      
#define TENSION_ADDR                             0x0650 //????
#define GMMA_ADDR                                0x0580 //???
        
#define CORE_LENGTH_DISPLACEMENT                 0x0800  //??????????
#define OIL_POSITION_DISPLACEMENT                0x0660 //??¦Ë???????
#define BIT_PROCESS_DISPLACEMENT                 0x0670 //??????????
#define CABLE_LENGTH_ADDR                        0x0120 //???3???

#define BIG_PUMP_ADDR                            0x0790  //??????
#define SMALL_PUMP_ADDR                          0x0600 //§³?????
#define PUSH_PRESSURE_ADDR                       0x0590  //??????

#define HOUR_ADDR                                0x0680 //?????
#define MINUTE_ADDR                              0x0690
#define SECOND_ADDR                              0x0710

#define ERROR_CODE_ADDR                          0x0510  //????
#define TIME_OUT_ADDR                            0x0520  //???

//rectangle object addr 
#define PROCESS_BAR_UP_ADDR                      0xD000        //???¦Ë???????
#define PB_UP_LT_POSITION_X                      139        //X
#define PB_UP_LT_POSITION_Y                      351       //Y
#define PB_UP_WIDTH                              32         //??
#define PB_UP_LENGTH                             236        //??

#define PROCESS_BAR_MIDDLE_ADDR                  0xB000       // ???¦Ë???????
#define PB_MIDDLE_LT_POSITION_X                  139
#define PB_MIDDLE_LT_POSITION_Y                  391
#define PB_MIDDLE_WIDTH                          32
#define PB_MIDDLE_LENGTH                         236

#define PROCESS_BAR_DOWN_ADDR                    0xC000        // ??¦Ë¦Ë???????
#define PB_DOWN_LT_POSITION_X                    134
#define PB_DOWN_LT_POSITION_Y                    432
#define PB_DOWN_WIDTH                            30
#define PB_DOWN_LENGTH                           234

#define GMMA_BUTTON_CLOSE_ADDR  0x1000// GMMA button????
#define GMMA_BUTTON_CLOSE_LT_X     359
#define GMMA_BUTTON_CLOSE_LT_Y   13
#define GMMA_BUTTON_CLOSE_WIDTH    33
#define GMMA_BUTTON_CLOSE_LENGTH   62 

#define GMMA_BUTTON_OPEN_ADDR   0x2000
#define GMMA_BUTTON_OPEN_LT_X      359
#define GMMA_BUTTON_OPEN_LT_Y      50
#define GMMA_BUTTON_OPEN_WIDTH     33
#define GMMA_BUTTON_OPEN_LENGTH   62 

#define PRESSURE_BUTTON_DOWN_ADDR   0xE000// pressure button????
#define PRESSURE_BUTTON_DOWN_LT_X       431
#define PRESSURE_BUTTON_DOWN_LT_Y       13
#define PRESSURE_BUTTON_DOWN_WIDTH    33
#define PRESSURE_BUTTON_DOWN_LENGTH   62 

#define PRESSURE_BUTTON_UP_ADDR     0xF000
#define PRESSURE_BUTTON_UP_LT_X     431
#define PRESSURE_BUTTON_UP_LT_Y     50
#define PRESSURE_BUTTON_UP_WIDTH     33
#define PRESSURE_BUTTON_UP_LENGTH    62 

#define PUSH_BUTTON_CLOSE_ADDR       0x5000      // push button???
#define PUSH_BUTTON_CLOSE_LT_X          32
#define PUSH_BUTTON_CLOSE_LT_Y          13
#define PUSH_BUTTON_CLOSE_WIDTH       33
#define PUSH_BUTTON_CLOSE_LENGTH      62

#define PUSH_BUTTON_OPEN_ADDR         0x6000
#define PUSH_BUTTON_OPEN_LT_X       32
#define PUSH_BUTTON_OPEN_LT_Y       48
#define PUSH_BUTTON_OPEN_WIDTH      33
#define PUSH_BUTTON_OPEN_LENGTH     62

#define BIT_BUTTON_BACK_ADDR          0x7000     // bit button???
#define BIT_BUTTON_BACK_LT_X        141
#define BIT_BUTTON_BACK_LT_Y        13
#define BIT_BUTTON_BACK_WIDTH       33
#define BIT_BUTTON_BACK_LENGTH      62

#define BIT_BUTTON_FORWARD_ADDR        0x8000
#define BIT_BUTTON_FORWARD_LT_X     141
#define BIT_BUTTON_FORWARD_LT_Y     49
#define BIT_BUTTON_FORWARD_WIDTH    33
#define BIT_BUTTON_FORWARD_LENGTH   62

#define MOTOR_BUTTON_STOP_ADDR        0x9000// motor button ????
#define MOTOR_BUTTON_STOP_LT_X      250
#define MOTOR_BUTTON_STOP_LT_Y      13
#define MOTOR_BUTTON_STOP_WIDTH     33
#define MOTOR_BUTTON_STOP_LENGTH    62

#define MOTOR_BUTTON_RUN_ADDR         0xA000
#define MOTOR_BUTTON_RUN_LT_X       250
#define MOTOR_BUTTON_RUN_LT_Y       50
#define MOTOR_BUTTON_RUN_WIDTH      33
#define MOTOR_BUTTON_RUN_LENGTH     62

//text object addr 
#define TEXT_CORE_LENGTH_ADDR                    0x0001      // ???
#define TEXT_OIL_POSITION_ADDR                   0x0011      // ??¦Ë
#define TEXT_BIT_PROCESS_ADDR                    0x0021      // ?????
#define TEXT_CORE_LENGTH_POINTER_ADDR            0x0100     //???
#define TEXT_OIL_POSITION_POINTER_ADDR           0x0200      
#define TEXT_BIT_PROCESS_POINTER_ADDR            0x0300 //???

//color 
#define COLOR_WHITE          0xFFFF
#define COLOR_GREEN          0x07E0 //0x07,0xE0};//???¡ê????
#define COLOR_DEEP_GREEN     0x03E0 //0x03,0xE0};//????????????? 
#define COLOR_LIGHT_BLUE     0xAFFF //0xAF,0xFF};//??????????
#define COLOR_BLUE           0x001F //0x00,0x1F};//???
#define COLOR_DEEP_BLUE      0x000E//0x7C76 //0x7C,0x76};//?????
#define COLOR_RED            0xF800 //0xF8,0x00};//???
#define COLOR_YELLOW            0xFF00 //0xF8,0x00};//???
// state  color 
#define BUTTON_PUSH_COLOR      COLOR_GREEN
#define BUTTON_IDLE_COLOR      COLOR_LIGHT_BLUE
#define BUTTON_CMD_SEND_COLOR  COLOR_DEEP_GREEN



typedef enum
{
   ED_core_length = 0,
   ED_oil_pos,
   ED_bit_process,
}e_Displacement;
typedef enum
{
	close = 0,
	open,
}e_cmd_status;

/******************************************************************/
void RectangleFill(uint16_t addr,uint16_t ltx,uint16_t lty,uint16_t rdx,uint16_t rdy,uint16_t color);
void RefreshNumber(uint16_t addr,uint16_t number);
uint8_t RefreshText(uint16_t addr,char * p_string,uint16_t color_addr,uint16_t color);
void Text_ResetLCD(void);
void onButtonUpdata_Rect(uint8_t id,uint16_t temp,uint16_t lcd_color);
void ButtonClear(uint8_t id);
void ButtonDeepGreen(uint8_t id,uint8_t up_or_down);
void onTimeUpdata(uint32_t sec);
void ButtonRed(uint8_t id,uint8_t up_or_down);
void Timeout_ResetLCD(void);
#endif
