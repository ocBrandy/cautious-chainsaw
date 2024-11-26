#ifndef __LOGDEBUGSWITCH_H
#define __LOGDEBUGSWITCH_H

#ifdef __cplusplus
	extern "C"{
#endif

/*****************************SYS_INIT*************************************/
#define LOG_DEBUG_METHOD 	SeggerRTT_LogDebug
#define USE_INSERT_DEBUG		0
		
/******************************************************************************/

/******************************************************************************/

#if (USE_INSERT_DEBUG != 0)
	#define INSERT(log_method)  insert_##log_method
#else
	#define INSERT(log_method)  remove_##log_method
#endif

void LogDebugInit(void);
void SeggerRTT_LogDebug(const char * sFormat, ...);


#define insert_LogDebug(flag,message)   do{ 																\
												if((flag) != 0)	\
												{	LOG_DEBUG_METHOD	message ;	   	}			\
											}while(0)

#define remove_LogDebug(flag,message)   do{ }while(0)

/*****************************SYS_INIT*************************************/
#define SYS_INIT_DEBUG		1	 //初始化完成
#define SYS_DEBUG			    1  //while1运行状态
#define TIMER2_DEBUG      1  //定时器2运行状态

#define Rx_data_DEBUG     1  //一帧数据接收完成
#define RxData_over_DEBUG         1  //数据接收完成
#define RxData_display_DEBUG   1   //接收数据显示








//#define CAN_INT_DEBUG     1  //can中断
//#define CAN_pack_DEBUG    1  //双探针can接收完成
//#define CAN_data_DEBUG    1  //CAN数据内容

//#define ACQ_pack_DEBUG    1 //双探针采集完成
//#define SOL4_ON_DEBUG     1 //开4号阀
//#define SOL4_OFF_DEBUG    1 //关4号阀
//#define SOL5_ON_DEBUG     1 //开5号阀
//#define SOL5_OFF_DEBUG    1 //关5号阀

/**********************************************************************/
/***********************************************************************
INSERT(LogDebug)(SYS_INIT_DEBUG,("\r\n---DGJC_System started successfully---\r\n")); 
**************************************************************************/

#ifdef __cplusplus
		}
#endif
#endif
