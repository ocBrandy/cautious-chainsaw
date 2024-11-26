/*********************************************************************************************************
*
*	模块名称 : 主程序入口
*	文件名称 : main.c
*	版    本 : V1.0
*	说    明 : 
*	修改记录 :
*		版本号  日期       作者    说明
*		V1.0    2019-04-08 		   首版
*********************************************************************************************************/

#include "bsp.h"				/* 底层硬件驱动 */

#include "ERSC_HT_DataStorage.h"
#include "ETH_DataStorage.h"
#include "BP_DataStorage.h"
#include "VGUS_LCD.h"

void KeyDistStatusUpdata(uint8_t type);
extern uint32_t push_timer,timer;
extern uint8_t Key_disable_flag,MD_commd_flag,BP_commd_flag,Motor_status_flag;					
extern uint8_t Key_response_status;	
extern uint8_t Key_status;	
extern void Key_NoresponseRefresh_Colorblock(uint8_t _status);
extern uint8_t Button_flag,MD_cmd_dist_flag,motor_flag,MD_timer;
extern uint16_t Key_respstatus_data;
extern uint8_t repeat_dist_flag,repeat_response_flag,_7538_flag;
uint16_t DH_data_timer = 0;
uint8_t timer_out = 0;
extern void ADC_acqdata(void);
extern void send_key_cmd(void);
extern void init_key(void);
extern void Initialvoltagevalue(void);
/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: c程序入口
*	形    参：无
*	返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/
int main(void)
{
	/*ST固件库中的启动文件已经执行了 SystemInit() 函数，该函数在 system_stm32f4xx.c 文件，主要功能是
	配置CPU系统的时钟，内部Flash访问时序，配置FSMC用于外部SRAM*/
	bsp_Init();		/* 硬件初始化 */
	bsp_StartAutoTimer(0,250);
	bsp_DelayMS(2000);////屏幕初始化慢
	init_key();//按键扫描
	Initialvoltagevalue();//偏置电压采集
	bsp_LedOn(1);//初始化完成，点亮LED
	

	while (1)
	{	
		HT_250ms_Timerout();																//250ms轮询
		Downdata_storage();																	//串口5接收井下数据，数据解析
		if(Key_disable_flag == 0 && _7538_flag == 1)				//上位机失能，按键使能
			{
				KeyDistStatusUpdata(bsp_GetKey());							//按键扫描
			}
		ETH_data_storage();																	//以太网串口1接收上位机数据，数据解析
		BP_data_storage();																	//485串口2接收变频机箱数据，数据解析
			
		if((MD_commd_flag == 1)&&((MD_timer >= 150)))				//定时时间到,或者变频机箱数据接收完成
		{
			MD_commd_flag = 0;																//马达按键动作清零

			if((Key_status & 0x01) == 1)											//马达指令 下发
				{
					Sendkeycmd(MD_DIST_REG,1);
					BP_commd_flag = 0;
				}
			else
				{
					Sendkeycmd(MD_DIST_REG,0);
					BP_commd_flag = 0;
				}	
		}

		if((Motor_status_flag == 1)&&(BP_commd_flag == 0))//马达反馈状态 ，变频机箱接收数据完成，
		{
			Motor_status_flag = 0;
			if((Key_respstatus_data & 0x01) == 1)						//马达反馈指令下发
				{
					Sendkeycmd(MD_RESPONSE_REG,1);	
				}
			else
				{
					Sendkeycmd(MD_RESPONSE_REG,0);
				}
		}	
			
		bsp_LedOn(2);
	}		
}	

/***************************** XMDNDL (END OF FILE) *********************************/
