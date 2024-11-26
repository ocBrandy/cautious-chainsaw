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
#define SYS_INIT_DEBUG		1	 //��ʼ�����
#define SYS_DEBUG			    1  //while1����״̬
#define TIMER2_DEBUG      1  //��ʱ��2����״̬

#define Rx_data_DEBUG     1  //һ֡���ݽ������
#define RxData_over_DEBUG         1  //���ݽ������
#define RxData_display_DEBUG   1   //����������ʾ








//#define CAN_INT_DEBUG     1  //can�ж�
//#define CAN_pack_DEBUG    1  //˫̽��can�������
//#define CAN_data_DEBUG    1  //CAN��������

//#define ACQ_pack_DEBUG    1 //˫̽��ɼ����
//#define SOL4_ON_DEBUG     1 //��4�ŷ�
//#define SOL4_OFF_DEBUG    1 //��4�ŷ�
//#define SOL5_ON_DEBUG     1 //��5�ŷ�
//#define SOL5_OFF_DEBUG    1 //��5�ŷ�

/**********************************************************************/
/***********************************************************************
INSERT(LogDebug)(SYS_INIT_DEBUG,("\r\n---DGJC_System started successfully---\r\n")); 
**************************************************************************/

#ifdef __cplusplus
		}
#endif
#endif
