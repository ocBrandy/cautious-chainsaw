/*
*********************************************************************************************************
*
*	模块名称 : 按键驱动模块
*	文件名称 : bsp_key.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*********************************************************************************************************
*/

#include "bsp.h"


#define __BSP_KEY_H

#define KEY_COUNT    6	   					/* 按键个数,  */

/* 根据应用程序的功能重命名按键宏 */
#define PUSH_KEY1_DOWN		     	 KEY_K1_DOWN//推靠1
#define PUSH_KEY1_UP			     	 KEY_K1_UP//2
#define PUSH_KEY1_LONG		     	 KEY_K1_LONG//3

#define BIT_PROCESS_KEY2_DOWN			KEY_K2_DOWN//钻进4
#define BIT_PROCESS_KEY2_UP				KEY_K2_UP//5
#define BIT_PROCESS_KEY2_LONG			KEY_K2_LONG//6

#define MOTOR_KEY3_DOWN						KEY_K3_DOWN//马达7
#define MOTOR_KEY3_UP							KEY_K3_UP//8
#define MOTOR_KEY3_LONG						KEY_K3_LONG//9

#define GMMA_KEY4_DOWN						KEY_K4_DOWN//伽马0A 10
#define GMMA_KEY4_UP							KEY_K4_UP//0B 11
#define GMMA_KEY4_LONG						KEY_K4_LONG   //12

#define RESSURE_UP_KEY5_DOWN			KEY_K5_DOWN//13 0x0d
#define RESSURE_UP_KEY5_UP				KEY_K5_UP//14   0x0e
#define RESSURE_UP_KEY5_LONG			KEY_K5_LONG//0X0F 15

#define PRESSURE_DOWN_KEY6_DOWN		KEY_K6_DOWN//0x10 16
#define PRESSURE_DOWN_KEY6_UP			KEY_K6_UP//0x11   17
#define PRESSURE_DOWN_KEY6_LONG		KEY_K6_LONG//0X12  18

/* 按键ID, 主要用于bsp_KeyState()函数的入口参数 */
typedef enum
{
	KID_K1 = 0,//推靠
	KID_K2,    //钻进
	KID_K3,    //马达
	KID_K4,   //伽马
	KID_K5,   //压力升
	KID_K6,  	//压力降
}KEY_ID_E;

/*
	按键滤波时间50ms, 单位10ms。
	只有连续检测到50ms状态不变才认为有效，包括弹起和按下两种事件
	即使按键电路不做硬件滤波，该滤波机制也可以保证可靠地检测到按键事件
*/
#define KEY_FILTER_TIME   5
#define KEY_LONG_TIME     20			/* 单位10ms， 持续1秒，认为长按事件 */

/*
	每个按键对应1个全局的结构体变量。
*/
typedef struct
{
	/* 下面是一个函数指针，指向判断按键手否按下的函数 */
	uint8_t (*IsKeyDownFunc)(void); /* 按键按下的判断函数,1表示按下 */

	uint8_t  Count;			/* 滤波器计数器 */
	uint16_t LongCount;		/* 长按计数器 */
	uint16_t LongTime;		/* 按键按下持续时间, 0表示不检测长按 */
	uint8_t  State;			/* 按键当前状态（按下还是弹起） */
	uint8_t  RepeatSpeed;	/* 连续按键周期 */
	uint8_t  RepeatCount;	/* 连续按键计数器 */
}KEY_T;

/*
	定义键值代码, 必须按如下次序定时每个键的按下、弹起和长按事件

	推荐使用enum, 不用#define，原因：
	(1) 便于新增键值,方便调整顺序，使代码看起来舒服点
	(2) 编译器可帮我们避免键值重复。
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 表示按键事件 */

	KEY_K1_DOWN,				/* K1键按下 */
	KEY_K1_UP,				/* K1键弹起 */
	KEY_K1_LONG,				/* K1键长按 */

	KEY_K2_DOWN,				/* K2键按下 */
	KEY_K2_UP,				/* K2键弹起 */
	KEY_K2_LONG,				/* K2键长按 */
	
	KEY_K3_DOWN,				/* K3键按下 */
	KEY_K3_UP,				/* K3键弹起 */
	KEY_K3_LONG,				/* K3键长按 */
	
	KEY_K4_DOWN,				/* K4键按下 */
	KEY_K4_UP,				/* K4键弹起 */
	KEY_K4_LONG,				/* K4键长按 */
	
	KEY_K5_DOWN,				/* K5键按下 */
	KEY_K5_UP,				/* K5键弹起 */
	KEY_K5_LONG,				/* K5键长按 */
	
	KEY_K6_DOWN,				/* K6键按下 */
	KEY_K6_UP,				/* K6键弹起 */
	KEY_K6_LONG,				/* K6键长按 */

}KEY_ENUM;

/* 按键FIFO用到变量 */
#define KEY_FIFO_SIZE	20
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* 键值缓冲区 */
	uint8_t Read;					/* 缓冲区读指针1 */
	uint8_t Write;					/* 缓冲区写指针 */
	uint8_t Read2;					/* 缓冲区读指针2 */
}KEY_FIFO_T;

/* 供外部调用的函数声明 */
void bsp_InitKey(void);
void bsp_KeyScan(void);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);
uint8_t bsp_GetKey2(void);
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID);
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed);
void bsp_ClearKey(void);


/***************************** yy-AA55(END OF FILE) *********************************/
