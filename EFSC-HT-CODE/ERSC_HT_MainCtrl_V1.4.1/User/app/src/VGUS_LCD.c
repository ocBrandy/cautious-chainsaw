#include "bsp.h"
#include "ff.h"			/* FatFS�ļ�ϵͳģ��*/
#include "VGUS_LCD.h"
#include "ERSC_HT_DataStorage.h"
#include "ETH_DataStorage.h"
extern s_eth_send send_data; 
extern uint8_t Button_flag,Key_status;
static const uint8_t common_header[2] = {0xA5,0x5A};
extern uint16_t Error_timer,DH_data_timer;
extern uint16_t Motor_Current,ctrl_current;
extern uint32_t push_timer;
extern uint16_t motor_cycle;
extern uint8_t timer_out;
extern uint8_t MD_cmd_dist_flag;
extern uint16_t tor_uphole,tspe_uphole;
uint8_t Button_MD_flag = 0;
typedef struct
{
    uint16_t addr;
    uint16_t x;
    uint16_t y;
    uint16_t x1;
    uint16_t y1;
    uint16_t default_color;
}s_button_info;
/*�������ο����� /٤��id=0/ѹ������id=1/�ƿ�id=2/���id=3/���id=4/  id= 0 �� id= 1 �� */
static const s_button_info button_table[5][2] = {
    {
        {GMMA_BUTTON_CLOSE_ADDR,
            GMMA_BUTTON_CLOSE_LT_X,GMMA_BUTTON_CLOSE_LT_Y,
            GMMA_BUTTON_CLOSE_LT_X + GMMA_BUTTON_CLOSE_LENGTH,GMMA_BUTTON_CLOSE_LT_Y + GMMA_BUTTON_CLOSE_WIDTH,
            COLOR_LIGHT_BLUE},
        {GMMA_BUTTON_OPEN_ADDR,
            GMMA_BUTTON_OPEN_LT_X,GMMA_BUTTON_OPEN_LT_Y,
            GMMA_BUTTON_OPEN_LT_X + GMMA_BUTTON_OPEN_LENGTH, GMMA_BUTTON_OPEN_LT_Y + GMMA_BUTTON_OPEN_WIDTH,
            COLOR_LIGHT_BLUE},
    },
    {
        {PRESSURE_BUTTON_DOWN_ADDR,
            PRESSURE_BUTTON_DOWN_LT_X,PRESSURE_BUTTON_DOWN_LT_Y,
            PRESSURE_BUTTON_DOWN_LT_X + PRESSURE_BUTTON_DOWN_LENGTH,PRESSURE_BUTTON_DOWN_LT_Y + PRESSURE_BUTTON_DOWN_WIDTH,
            COLOR_LIGHT_BLUE},
        {PRESSURE_BUTTON_UP_ADDR,
            PRESSURE_BUTTON_UP_LT_X,PRESSURE_BUTTON_UP_LT_Y,
            PRESSURE_BUTTON_UP_LT_X + PRESSURE_BUTTON_UP_LENGTH, PRESSURE_BUTTON_UP_LT_Y + PRESSURE_BUTTON_UP_WIDTH,
            COLOR_LIGHT_BLUE},
    },
    {
        {PUSH_BUTTON_CLOSE_ADDR,
            PUSH_BUTTON_CLOSE_LT_X,PUSH_BUTTON_CLOSE_LT_Y,
            PUSH_BUTTON_CLOSE_LT_X + PUSH_BUTTON_CLOSE_LENGTH,PUSH_BUTTON_CLOSE_LT_Y + PUSH_BUTTON_CLOSE_WIDTH,
            COLOR_LIGHT_BLUE},
        {PUSH_BUTTON_OPEN_ADDR,
            PUSH_BUTTON_OPEN_LT_X,PUSH_BUTTON_OPEN_LT_Y,
            PUSH_BUTTON_OPEN_LT_X + PUSH_BUTTON_OPEN_LENGTH,PUSH_BUTTON_OPEN_LT_Y + PUSH_BUTTON_OPEN_WIDTH,
            COLOR_LIGHT_BLUE},
    },
    {
        {BIT_BUTTON_BACK_ADDR,
            BIT_BUTTON_BACK_LT_X,BIT_BUTTON_BACK_LT_Y,
            BIT_BUTTON_BACK_LT_X + BIT_BUTTON_BACK_LENGTH,BIT_BUTTON_BACK_LT_Y + BIT_BUTTON_BACK_WIDTH,
            COLOR_LIGHT_BLUE},
        {BIT_BUTTON_FORWARD_ADDR,
            BIT_BUTTON_FORWARD_LT_X,BIT_BUTTON_FORWARD_LT_Y,
            BIT_BUTTON_FORWARD_LT_X + BIT_BUTTON_FORWARD_LENGTH,BIT_BUTTON_FORWARD_LT_Y + BIT_BUTTON_FORWARD_WIDTH,
            COLOR_LIGHT_BLUE},
    },
    {
        {MOTOR_BUTTON_STOP_ADDR,
            MOTOR_BUTTON_STOP_LT_X,MOTOR_BUTTON_STOP_LT_Y,
            MOTOR_BUTTON_STOP_LT_X + MOTOR_BUTTON_STOP_LENGTH , MOTOR_BUTTON_STOP_LT_Y + MOTOR_BUTTON_STOP_WIDTH,
            COLOR_LIGHT_BLUE},
        {MOTOR_BUTTON_RUN_ADDR,
            MOTOR_BUTTON_RUN_LT_X,MOTOR_BUTTON_RUN_LT_Y,
            MOTOR_BUTTON_RUN_LT_X + MOTOR_BUTTON_RUN_LENGTH, MOTOR_BUTTON_RUN_LT_Y + MOTOR_BUTTON_RUN_WIDTH,
            COLOR_LIGHT_BLUE},
    },



};
/*************************************************************
 * ��  ����ResetLCD
 * ��  �ܣ��ϵ��ʼ������
 * ��  ������
 * ����ֵ����
****************************************************************/
void ResetLCD(void)  
{  
	//�������
    RefreshNumber(TEMPERATURE1_ADDR,0);
    RefreshNumber(TEMPERATURE2_ADDR,0);
    RefreshNumber(TEMPERATURE3_ADDR,0);
    RefreshNumber(TEMPERATURE4_ADDR,0);
    RefreshNumber(TEMPERATURE5_ADDR,0);
    RefreshNumber(TEMPERATURE_BOARD_ADDR,0);
    
    RefreshNumber(AC_VOLTAGE_ADDR,0);
    RefreshNumber(CABLE_VOLTAGE_ADDR,0);
	   /*���������*/
		RefreshNumber(CABLE_VOLTAGE_ADDR,0);
		RefreshNumber(MOTOR_CYCLE_ADDR,0);
    RefreshNumber(VARIABLE_FREQ_ADDR,0);
		RefreshNumber(VARIABLE_FREQ_MACHINE_BOX_TEMPERATURE_ADDR,0);
    RefreshNumber(CABLE_CURRENT_ADDR,0);
		RefreshNumber(MOTOR_CURRENT_ADDR,0);
    RefreshNumber(CABLE_LENGTH_ADDR,0);
	
    RefreshNumber(BIG_PUMP_ADDR,0);
    RefreshNumber(PUSH_PRESSURE_ADDR,0);
    RefreshNumber(SMALL_PUMP_ADDR,0);
    
    RefreshNumber(BIT_PROCESS_DISPLACEMENT,0);
    RefreshNumber(OIL_POSITION_DISPLACEMENT,0);
    RefreshNumber(CORE_LENGTH_DISPLACEMENT,0);
    
    RefreshNumber(GMMA_ADDR,0);
    RefreshNumber(TENSION_ADDR,0);
    RefreshNumber(ERROR_CODE_ADDR,0);
    RefreshNumber(TIME_OUT_ADDR,0);
    
    RefreshNumber(HOUR_ADDR,0);
    RefreshNumber(MINUTE_ADDR,0);
    RefreshNumber(SECOND_ADDR,0); 
		
		Text_ResetLCD();//����ı�
		//��ս������������ľ�����䱣��
		/*���*/
		RectangleFill(PROCESS_BAR_UP_ADDR,
                    PB_UP_LT_POSITION_X,PB_UP_LT_POSITION_Y,
                    PB_UP_LT_POSITION_X,PB_UP_LT_POSITION_Y,
                    COLOR_LIGHT_BLUE);
		/*�ĳ�*/								
		RectangleFill(PROCESS_BAR_MIDDLE_ADDR,
                    PB_MIDDLE_LT_POSITION_X,PB_MIDDLE_LT_POSITION_Y,
                    PB_MIDDLE_LT_POSITION_X,PB_MIDDLE_LT_POSITION_Y,
                    COLOR_LIGHT_BLUE);
		
}

/*************************************************************
 * ��  ����Timeout_ResetLCD
 * ��  �ܣ���ʱ�微������
 * ��  ������
 * ����ֵ����
****************************************************************/
extern uint8_t resetLCD_flag;
void Timeout_ResetLCD(void)  
{  
		
	//�微������
    RefreshNumber(TEMPERATURE1_ADDR,0);// ���ӽ��ⲿ�¶�
    RefreshNumber(TEMPERATURE2_ADDR,0);// ������������¶�
    RefreshNumber(TEMPERATURE3_ADDR,0);// Һѹ�ڵ���¶�
    RefreshNumber(TEMPERATURE4_ADDR,0);// ��Һѹ���¶�
    RefreshNumber(TEMPERATURE5_ADDR,0);// 3981�¶�
    RefreshNumber(TEMPERATURE_BOARD_ADDR,0);// ����ƿ�ڲ��¶�
    

	
    RefreshNumber(AC_VOLTAGE_ADDR,0);//���µ����ѹ
    RefreshNumber(CABLE_VOLTAGE_ADDR,0);//���Ƶ�ѹ
    
    RefreshNumber(BIG_PUMP_ADDR,0);//С��ѹ��
    RefreshNumber(PUSH_PRESSURE_ADDR,0);//���ѹ��
    RefreshNumber(SMALL_PUMP_ADDR,0);//�ƿ�ѹ��
    
    RefreshNumber(BIT_PROCESS_DISPLACEMENT,0);//���λ��
		RefreshNumber(OIL_POSITION_DISPLACEMENT,0);//��λλ��
    RefreshNumber(CORE_LENGTH_DISPLACEMENT,0);//�ĳ�λ��
    
    RefreshNumber(GMMA_ADDR,0);//٤��
    RefreshNumber(TENSION_ADDR,0);//����
//////////    RefreshNumber(ERROR_CODE_ADDR,0);
//////////    RefreshNumber(TIME_OUT_ADDR,0);
		
    RefreshNumber(MOTOR_CYCLE_ADDR,0);//�������ת��
    RefreshNumber(HOUR_ADDR,0);//ʱ����
    RefreshNumber(MINUTE_ADDR,0);
    RefreshNumber(SECOND_ADDR,0);
    //��ս�����
		/*���*/
		RectangleFill(PROCESS_BAR_UP_ADDR,
                    PB_UP_LT_POSITION_X,PB_UP_LT_POSITION_Y,
                    PB_UP_LT_POSITION_X,PB_UP_LT_POSITION_Y,
                    COLOR_LIGHT_BLUE);
		/*�ĳ�*/								
		RectangleFill(PROCESS_BAR_MIDDLE_ADDR,
                    PB_MIDDLE_LT_POSITION_X,PB_MIDDLE_LT_POSITION_Y,
                    PB_MIDDLE_LT_POSITION_X,PB_MIDDLE_LT_POSITION_Y,
                    COLOR_LIGHT_BLUE);
		//����ı�							
		Text_ResetLCD();	
	
}
/*************************************************************
 * ��  ����Text_ResetLCD
 * ��  �ܣ�����ı�
 * ��  ������
 * ����ֵ����
****************************************************************/
void Text_ResetLCD(void)
{
	RefreshText(TEXT_CORE_LENGTH_ADDR,"low",TEXT_CORE_LENGTH_POINTER_ADDR,COLOR_BLUE);
	RefreshText(TEXT_BIT_PROCESS_ADDR,"low",TEXT_BIT_PROCESS_POINTER_ADDR,COLOR_BLUE);
}

/*****************************************************************************
 * ��  ����RefreshNumber
 * ��  �ܣ�ˢ������
 * ��  ����addr:д��ַ
 *		   number������
 * ����ֵ����
*****************************************************************************/
void RefreshNumber(uint16_t addr,uint16_t number) ///ˢ������   A5 /5A / ���ݳ��ȣ�д�Ĵ���+��ַ+���ݣ�/ 82��д��/��ַ/���� 
{
	if(number > 65535)
    {
      number = 65535;
    }
		uint8_t len = 0;
    uint8_t length = 5;
    uint8_t temp_buf[MAX_LCD_UART_CMD_LENGTH];
		temp_buf[len++] = common_header[0];
		temp_buf[len++] = common_header[1]; 
    temp_buf[len++] = length;//���ݳ���
    temp_buf[len++] = CMD_WRITE_REG;   //0x82(д)
    temp_buf[len++] = addr >> 8;
    temp_buf[len++] = addr;
    temp_buf[len++] = number >> 8;
    temp_buf[len++] = number;    
		comSendBuf(COM3,temp_buf,len);
		for(uint8_t i = 0;i < len;i++)//�������
			{
				temp_buf[i] = 0;
			}
		
}
/*****************************************************************************
 * ��  ����RefreshNumber
 * ��  �ܣ�ˢ������
 * ��  ����addr:д��ַ
 *		   number������
 * ����ֵ��
*****************************************************************************/
uint8_t RefreshText(uint16_t addr,char * p_string,uint16_t color_addr,uint16_t color)//ˢ���ı�  A5 5A 
{
    uint8_t length,string_length,i;
    uint8_t temp_buf[MAX_LCD_UART_CMD_LENGTH];
    uint8_t color_buf[MAX_LCD_UART_CMD_LENGTH];
    temp_buf[0] = common_header[0];
		temp_buf[1] = common_header[1];
    
    if(p_string == X_Null) {return X_False;}
    string_length = strlen(p_string);
    if(string_length == 0 || string_length > (MAX_LCD_UART_CMD_LENGTH - 30)) {return X_False;}
    
    temp_buf[3] = CMD_WRITE_REG;
    temp_buf[4] = addr >> 8;
    temp_buf[5] = (uint8_t)addr;
    
    for(i=0;i<string_length;i++)
			{
				temp_buf[6+i] = p_string[i]; //����ı�low normal warning
			}
    
    length = 5 + i;
    
    /************************************************************************************/
    color_buf[0] = common_header[0];
		color_buf[1] = common_header[1];
    color_buf[2] = 5;
    color_buf[3] = CMD_WRITE_REG;
    color_buf[4] = (color_addr + 3) >> 8;
    color_buf[5] = (uint8_t)(color_addr + 3);
    color_buf[6] = color >> 8;
    color_buf[7] = color;
		comSendBuf(COM3,color_buf,8);
    /************************************************************************************/
    
    if((length + 3) <= MAX_LCD_UART_CMD_LENGTH)
    {
        temp_buf[2] = length;  
        temp_buf[length + 3 - 2] = 0xFF;
        temp_buf[length + 4 - 2] = 0xFF;
        comSendBuf(COM3,temp_buf,length + 3);
    }
		return X_False;
}
/**********************************************************************************************************
*	�� �� ��: RectangleFill
*	����˵��: �������
*	��    �Σ���ַ����ʼ���꣬��ɫ
*	�� �� ֵ: ��
********************************************************************************************************/
void RectangleFill(uint16_t addr,uint16_t ltx,uint16_t lty,uint16_t rdx,uint16_t rdy,uint16_t color)//�������
{
    uint8_t length = 17;
    uint8_t temp_buf[MAX_LCD_UART_CMD_LENGTH];
    temp_buf[0] = common_header[0];
		temp_buf[1] = common_header[1];
    
    temp_buf[2] = length;
    temp_buf[3] = CMD_WRITE_REG;
    temp_buf[4] = addr >> 8;
    temp_buf[5] = addr;
    temp_buf[6] = RECT_FILL_CMD >> 8;
    temp_buf[7] = RECT_FILL_CMD;
    temp_buf[8] = 0x00;
    temp_buf[9] = 0x01; // one rectangle once 
    
    temp_buf[10] = ltx >> 8;
    temp_buf[11] = ltx;
    temp_buf[12] = lty >> 8;
    temp_buf[13] = lty;
    temp_buf[14] = rdx >> 8;
    temp_buf[15] = rdx;
    temp_buf[16] = rdy >> 8;
    temp_buf[17] = rdy;
    
    temp_buf[18] = color >> 8;
    temp_buf[19] = color;
    
    comSendBuf(COM3,temp_buf,20);
}
/**********************************************************************************************************
*	�� �� ��: onDisplacementUpdata_Text
*	����˵��: �ĳ�����ı���ʾ
*	��    �Σ���
*	�� �� ֵ: ��
********************************************************************************************************/
void onDisplacementUpdata_Text(void)
{
		
		// 0 ~ 255�ĳ�
		if(send_data.core_length < 15)
		{
			RefreshText(TEXT_CORE_LENGTH_ADDR,"low",TEXT_CORE_LENGTH_POINTER_ADDR,COLOR_BLUE);	
		}
		else if(send_data.core_length < 60)
		{
			RefreshText(TEXT_CORE_LENGTH_ADDR,"normal",TEXT_CORE_LENGTH_POINTER_ADDR,COLOR_GREEN);
		}
		else
		{
			RefreshText(TEXT_CORE_LENGTH_ADDR,"warning",TEXT_CORE_LENGTH_POINTER_ADDR,COLOR_RED);
		}

		// 0 ~ 255���
		if(send_data.drill_bit_process < 50)
		{
		RefreshText(TEXT_BIT_PROCESS_ADDR,"low",TEXT_BIT_PROCESS_POINTER_ADDR,COLOR_BLUE);
		}
		else if(send_data.drill_bit_process < 200)
		{
		RefreshText(TEXT_BIT_PROCESS_ADDR,"normal",TEXT_BIT_PROCESS_POINTER_ADDR,COLOR_GREEN);
		}
		else
		{
		RefreshText(TEXT_BIT_PROCESS_ADDR,"warning",TEXT_BIT_PROCESS_POINTER_ADDR,COLOR_RED);
		}      
}


/**********************************************************************************************************
*	�� �� ��: onDisplacementUpdata_Rect bit_processHandle core_lengthHandle
*	����˵��: �ĳ��������������
*	��    �Σ����ݳ���
*	�� �� ֵ: ��
********************************************************************************************************/
#define CORE_LENGTH_LIMIT      75
#define BIT_PROCESS_LIMIT      255

static void bit_processHandle(uint16_t bit_process_value) //��� ����
{
    uint32_t process;
    if(bit_process_value >= BIT_PROCESS_LIMIT) {process = PB_UP_LENGTH;}
    else{
        process = PB_UP_LENGTH * bit_process_value;
        process/= BIT_PROCESS_LIMIT;
    }
    RectangleFill(PROCESS_BAR_UP_ADDR,
                    PB_UP_LT_POSITION_X,PB_UP_LT_POSITION_Y,
                    PB_UP_LT_POSITION_X + process,PB_UP_LT_POSITION_Y + PB_UP_WIDTH,
                    COLOR_DEEP_BLUE);
}

static void core_lengthHandle(uint16_t core_length_value) //�ĳ� ��
{
    uint32_t process;
    if(core_length_value >= CORE_LENGTH_LIMIT) {process = PB_MIDDLE_LENGTH;}
    else
			{
        process = PB_MIDDLE_LENGTH * core_length_value;
        process/= CORE_LENGTH_LIMIT;
			}
    RectangleFill(PROCESS_BAR_MIDDLE_ADDR,
                    PB_MIDDLE_LT_POSITION_X,PB_MIDDLE_LT_POSITION_Y,
                    PB_MIDDLE_LT_POSITION_X + process,PB_MIDDLE_LT_POSITION_Y + PB_MIDDLE_WIDTH,
                    COLOR_GREEN);
}
void onDisplacementUpdata_Rect(void)/////�ĳ������ �������
{ 
	bit_processHandle(send_data.drill_bit_process);         
	core_lengthHandle(send_data.core_length );
}


/*
void Rect_ResetLCD(void)
{
    uint8_t i;
    
    for(i=0;i<5;i++)
    {
        RectangleFill(  button_table[i][0].addr,
                        button_table[i][0].x,button_table[i][0].y,button_table[i][0].x1,button_table[i][0].y1,
                        button_table[i][0].default_color);
        
        RectangleFill(  button_table[i][1].addr,
                        button_table[i][1].x,button_table[i][1].y,button_table[i][1].x1,button_table[i][1].y1,
                        button_table[i][1].default_color);

    }
}*/

void ButtonClear(uint8_t id)///ѹ��KEEP��ȫ�壩
{
    uint16_t addr = 0,x,y,rx,ry,color;
    uint16_t addr1 = 0,x1,y1,rx1,ry1,color1;
    
    if(id >= 5) {return;}
    
    addr    = button_table[id][0].addr;
    x       = button_table[id][0].x;
    y       = button_table[id][0].y;
    rx      = button_table[id][0].x1;
    ry      = button_table[id][0].y1;
    color   = button_table[id][0].default_color;
    
    addr1   = button_table[id][1].addr;
    x1      = button_table[id][1].x;
    y1      = button_table[id][1].y;
    rx1     = button_table[id][1].x1;
    ry1     = button_table[id][1].y1;
    color1  = button_table[id][1].default_color;
            
    if(addr != 0)
    {
        RectangleFill(addr,x,y,rx,ry,color);
    }
    if(addr1 != 0)
    {
        RectangleFill(addr1,x1,y1,rx1,ry1,color1);
    }
}

 void ButtonDeepGreen(uint8_t id,uint8_t up_or_down)//�������ݻ�Ӧ������ɫ�����
{
    uint16_t addr = 0,x,y,rx,ry,color;
    uint16_t addr1 = 0,x1,y1,rx1,ry1,color1;
    
    addr    = button_table[id][0].addr;
    x       = button_table[id][0].x;
    y       = button_table[id][0].y;
    rx      = button_table[id][0].x1;
    ry      = button_table[id][0].y1;
    color   = (up_or_down != 0) ? COLOR_DEEP_GREEN : button_table[id][0].default_color;
    
    addr1   = button_table[id][1].addr;
    x1      = button_table[id][1].x;
    y1      = button_table[id][1].y;
    rx1     = button_table[id][1].x1;
    ry1     = button_table[id][1].y1;
    color1  = (up_or_down != 0) ? button_table[id][1].default_color : COLOR_DEEP_GREEN;
            
    if(addr != 0)
    {
        RectangleFill(addr,x,y,rx,ry,color);
    }
    if(addr1 != 0)
    {
        RectangleFill(addr1,x1,y1,rx1,ry1,color1);
    }
}
/*
static void ButtonYellow(uint8_t id,uint8_t up_or_down)
{
    uint16_t addr = 0,x,y,rx,ry,color;
    uint16_t addr1 = 0,x1,y1,rx1,ry1,color1;
    
    addr    = button_table[id][0].addr;
    x       = button_table[id][0].x;
    y       = button_table[id][0].y;
    rx      = button_table[id][0].x1;
    ry      = button_table[id][0].y1;
    color   = (up_or_down != 0) ? COLOR_YELLOW : button_table[id][0].default_color;
    
    addr1   = button_table[id][1].addr;
    x1      = button_table[id][1].x;
    y1      = button_table[id][1].y;
    rx1     = button_table[id][1].x1;
    ry1     = button_table[id][1].y1;
    color1  = (up_or_down != 0) ? button_table[id][1].default_color : COLOR_YELLOW;
            
    if(addr != 0)
    {
        RectangleFill(addr,x,y,rx,ry,color);
    }
    if(addr1 != 0)
    {
        RectangleFill(addr1,x1,y1,rx1,ry1,color1);
    }
}

*/
/*ɫ����ʾ��ɫ*/
 void ButtonRed(uint8_t id,uint8_t up_or_down)
{
    uint16_t addr = 0,x,y,rx,ry,color;
    uint16_t addr1 = 0,x1,y1,rx1,ry1,color1;
    
    addr    = button_table[id][0].addr;
    x       = button_table[id][0].x;
    y       = button_table[id][0].y;
    rx      = button_table[id][0].x1;
    ry      = button_table[id][0].y1;
    color   = (up_or_down != 0) ? COLOR_RED : button_table[id][0].default_color;
    
    addr1   = button_table[id][1].addr;
    x1      = button_table[id][1].x;
    y1      = button_table[id][1].y;
    rx1     = button_table[id][1].x1;
    ry1     = button_table[id][1].y1;
    color1  = (up_or_down != 0) ? button_table[id][1].default_color : COLOR_RED;
            
    if(addr != 0)
    {
        RectangleFill(addr,x,y,rx,ry,color);
    }
    if(addr1 != 0)
    {
        RectangleFill(addr1,x1,y1,rx1,ry1,color1);
    }
}

void onButtonUpdata_Rect(uint8_t id,uint16_t temp,uint16_t lcd_color)//����������������ɫ��仯
{
	uint16_t addr = 0,x,y,rx,ry,color;
	uint16_t addr1 = 0,x1,y1,rx1,ry1,color1;
	if(temp == open)////�������£������ȱ��̣�������ɫ����֮�����µ������ű��̣�������ɫ
    {
        addr    = button_table[id][0].addr;
        x       = button_table[id][0].x;
        y       = button_table[id][0].y;
        rx      = button_table[id][0].x1;
        ry      = button_table[id][0].y1;
        color   = lcd_color;
        
        addr1   = button_table[id][1].addr;
        x1      = button_table[id][1].x;
        y1      = button_table[id][1].y;
        rx1     = button_table[id][1].x1;
        ry1     = button_table[id][1].y1;
        color1  = button_table[id][1].default_color;
    }
    else if(temp == close)
    {
        addr    = button_table[id][1].addr;
        x       = button_table[id][1].x;
        y       = button_table[id][1].y;
        rx     = button_table[id][1].x1;
        ry     = button_table[id][1].y1;
        color   = lcd_color;
        
        addr1    = button_table[id][0].addr;
        x1       = button_table[id][0].x;
        y1       = button_table[id][0].y;
        rx1     = button_table[id][0].x1;
        ry1      = button_table[id][0].y1;
        color1   = button_table[id][0].default_color;
        
        
    }
    else
    {
        addr    = button_table[id][0].addr;
        x       = button_table[id][0].x;
        y       = button_table[id][0].y;
        rx      = button_table[id][0].x1;
        ry      = button_table[id][0].y1;
        color   = button_table[id][0].default_color;
        
        addr1   = button_table[id][1].addr;
        x1      = button_table[id][1].x;
        y1      = button_table[id][1].y;
        rx1     = button_table[id][1].x1;
        ry1     = button_table[id][1].y1;
        color1  = button_table[id][1].default_color;
        
    }
            
    if(addr != 0)////ɫ����ʾ��LCD
    {
        RectangleFill(addr,x,y,rx,ry,color);
    }
    if(addr1 != 0)
    {
        RectangleFill(addr1,x1,y1,rx1,ry1,color1);
    }
	}
/**********************************************************************************************************
*	�� �� ��: LCD_Updata
*	����˵��: ˢ����ʾ
*	��    �Σ���
*	�� �� ֵ: ��
********************************************************************************************************/
extern uint8_t motorcycle_FWD_count;
extern	uint8_t motor_cycle_flag;
extern	uint16_t motor_cycle_lcd,ctrl_current;
extern	uint16_t TwoBytesReverse(uint16_t src);
void LCD_Updata(void)
{
	/*�¶�*/
	RefreshNumber(TEMPERATURE1_ADDR,send_data.elect_ext_temperature); //���µ�Դ�¶ȣ����ӽ��ⲿ�¶ȣ�
	RefreshNumber(TEMPERATURE2_ADDR,send_data.Pump_Oil_temperature);//��Һѹ��
	RefreshNumber(TEMPERATURE3_ADDR,send_data.Small_Motor_temperature);//Һѹ�ڵ���¶�
	RefreshNumber(TEMPERATURE4_ADDR,send_data.Stepper_motor_temperature);//�������
	RefreshNumber(TEMPERATURE5_ADDR,send_data.Cable_Head_Tension_temperature);//3981
	RefreshNumber(TEMPERATURE_BOARD_ADDR,send_data.temperature);//����ƿ�¶ȣ��Ѿ����㣩
	
	RefreshNumber(VARIABLE_FREQ_MACHINE_BOX_TEMPERATURE_ADDR,send_data.data.BP_temperature/10);//��Ƶ�����¶�
  /*��ѹ*/
	RefreshNumber(AC_VOLTAGE_ADDR,send_data.AC_voltage_of_underground_motor);//����������ѹ
	RefreshNumber(CABLE_VOLTAGE_ADDR,send_data.cable_head_voltage);//��ͷ��ѹ�����Ƶ�ѹ��
  /*ѹ��*///�Ѿ������
  RefreshNumber(BIG_PUMP_ADDR,send_data.large_pump_pressure);//���ѹ��
	RefreshNumber(PUSH_PRESSURE_ADDR,send_data.push_pressure);//�ƿ�ѹ��
	RefreshNumber(SMALL_PUMP_ADDR,send_data.small_pump_pressure);//С��ѹ��
   /*λ��*/
	RefreshNumber(BIT_PROCESS_DISPLACEMENT,send_data.drill_bit_process);//���λ�ƣ����ݣ�
	RefreshNumber(OIL_POSITION_DISPLACEMENT,send_data.oil_position);//��λλ�ƣ����ݣ�
	RefreshNumber(CORE_LENGTH_DISPLACEMENT,send_data.core_length);//�ĳ�λ�ƣ����ݣ�
  /* ���� */        
	RefreshNumber(GMMA_ADDR,send_data.gmma_cnt);//٤����������£�
	RefreshNumber(TENSION_ADDR,send_data.tension); //���������£�
	RefreshNumber(ERROR_CODE_ADDR,Error_timer);//����
	RefreshNumber(TIME_OUT_ADDR,DH_data_timer);//��ʱʱ��
	RefreshNumber(MOTOR_CYCLE_ADDR,motorcycle_FWD_count);//���ת��
	RefreshNumber(TOR_UPHOLE,tor_uphole);//Ť��tor_uphole
	RefreshNumber(TSPE_UPHOLE,tspe_uphole);//ת��

	RefreshNumber(MOTOR_CURRENT_ADDR,Motor_Current);//ADC�ɼ� ������� ��Ļ��ʾһλС��
	RefreshNumber(CABLE_CURRENT_ADDR,ctrl_current/10);//ADC�ɼ� ��ͷ���������Ƶ�������Ļ��ʾһλС��
	
	RefreshNumber(VARIABLE_FREQ_ADDR,send_data.data.real_freq/100);//��Ƶ����Ƶ��
	RefreshNumber(CABLE_LENGTH_ADDR,send_data.data.Ad_ParmLimitLength);//���³���
	onTimeUpdata(push_timer);///ˢ��ʱ����

	onDisplacementUpdata_Text();///�ĳ���������ı���ʾ��
	onDisplacementUpdata_Rect();//�ĳ��������������䣩
	//����ɫ����ʾ,����ɨ��������·�
}



void onTimeUpdata(uint32_t sec)/////��Ļʱ������㣬��ˢ�µ���Ļ
{
	uint8_t hour,min,second;	
	hour = sec/3600;
	min=sec%3600/60;
	second = sec % 60;
	RefreshNumber(HOUR_ADDR,hour);
	RefreshNumber(MINUTE_ADDR,min);
	RefreshNumber(SECOND_ADDR,second); 
}

void VGUS_Init(void)
{
	ResetLCD();
}
