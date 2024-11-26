/*
*********************************************************************************************************
*
*	ģ������ : ��������ģ��
*	�ļ����� : bsp_key.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*********************************************************************************************************
*/

#include "bsp.h"


#define __BSP_KEY_H

#define KEY_COUNT    6	   					/* ��������,  */

/* ����Ӧ�ó���Ĺ��������������� */
#define PUSH_KEY1_DOWN		     	 KEY_K1_DOWN//�ƿ�1
#define PUSH_KEY1_UP			     	 KEY_K1_UP//2
#define PUSH_KEY1_LONG		     	 KEY_K1_LONG//3

#define BIT_PROCESS_KEY2_DOWN			KEY_K2_DOWN//���4
#define BIT_PROCESS_KEY2_UP				KEY_K2_UP//5
#define BIT_PROCESS_KEY2_LONG			KEY_K2_LONG//6

#define MOTOR_KEY3_DOWN						KEY_K3_DOWN//���7
#define MOTOR_KEY3_UP							KEY_K3_UP//8
#define MOTOR_KEY3_LONG						KEY_K3_LONG//9

#define GMMA_KEY4_DOWN						KEY_K4_DOWN//٤��0A 10
#define GMMA_KEY4_UP							KEY_K4_UP//0B 11
#define GMMA_KEY4_LONG						KEY_K4_LONG   //12

#define RESSURE_UP_KEY5_DOWN			KEY_K5_DOWN//13 0x0d
#define RESSURE_UP_KEY5_UP				KEY_K5_UP//14   0x0e
#define RESSURE_UP_KEY5_LONG			KEY_K5_LONG//0X0F 15

#define PRESSURE_DOWN_KEY6_DOWN		KEY_K6_DOWN//0x10 16
#define PRESSURE_DOWN_KEY6_UP			KEY_K6_UP//0x11   17
#define PRESSURE_DOWN_KEY6_LONG		KEY_K6_LONG//0X12  18

/* ����ID, ��Ҫ����bsp_KeyState()��������ڲ��� */
typedef enum
{
	KID_K1 = 0,//�ƿ�
	KID_K2,    //���
	KID_K3,    //���
	KID_K4,   //٤��
	KID_K5,   //ѹ����
	KID_K6,  	//ѹ����
}KEY_ID_E;

/*
	�����˲�ʱ��50ms, ��λ10ms��
	ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�
	��ʹ������·����Ӳ���˲������˲�����Ҳ���Ա�֤�ɿ��ؼ�⵽�����¼�
*/
#define KEY_FILTER_TIME   5
#define KEY_LONG_TIME     20			/* ��λ10ms�� ����1�룬��Ϊ�����¼� */

/*
	ÿ��������Ӧ1��ȫ�ֵĽṹ�������
*/
typedef struct
{
	/* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
	uint8_t (*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */

	uint8_t  Count;			/* �˲��������� */
	uint16_t LongCount;		/* ���������� */
	uint16_t LongTime;		/* �������³���ʱ��, 0��ʾ����ⳤ�� */
	uint8_t  State;			/* ������ǰ״̬�����»��ǵ��� */
	uint8_t  RepeatSpeed;	/* ������������ */
	uint8_t  RepeatCount;	/* �������������� */
}KEY_T;

/*
	�����ֵ����, ���밴���´���ʱÿ�����İ��¡�����ͳ����¼�

	�Ƽ�ʹ��enum, ����#define��ԭ��
	(1) ����������ֵ,�������˳��ʹ���뿴���������
	(2) �������ɰ����Ǳ����ֵ�ظ���
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */

	KEY_K1_DOWN,				/* K1������ */
	KEY_K1_UP,				/* K1������ */
	KEY_K1_LONG,				/* K1������ */

	KEY_K2_DOWN,				/* K2������ */
	KEY_K2_UP,				/* K2������ */
	KEY_K2_LONG,				/* K2������ */
	
	KEY_K3_DOWN,				/* K3������ */
	KEY_K3_UP,				/* K3������ */
	KEY_K3_LONG,				/* K3������ */
	
	KEY_K4_DOWN,				/* K4������ */
	KEY_K4_UP,				/* K4������ */
	KEY_K4_LONG,				/* K4������ */
	
	KEY_K5_DOWN,				/* K5������ */
	KEY_K5_UP,				/* K5������ */
	KEY_K5_LONG,				/* K5������ */
	
	KEY_K6_DOWN,				/* K6������ */
	KEY_K6_UP,				/* K6������ */
	KEY_K6_LONG,				/* K6������ */

}KEY_ENUM;

/* ����FIFO�õ����� */
#define KEY_FIFO_SIZE	20
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;					/* ��������ָ��1 */
	uint8_t Write;					/* ������дָ�� */
	uint8_t Read2;					/* ��������ָ��2 */
}KEY_FIFO_T;

/* ���ⲿ���õĺ������� */
void bsp_InitKey(void);
void bsp_KeyScan(void);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);
uint8_t bsp_GetKey2(void);
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID);
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed);
void bsp_ClearKey(void);


/***************************** yy-AA55(END OF FILE) *********************************/
