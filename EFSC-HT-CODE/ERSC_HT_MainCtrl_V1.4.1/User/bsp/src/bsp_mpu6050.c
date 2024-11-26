/*
*********************************************************************************************************
*
*	模块名称 : 三轴陀螺仪MPU-6050驱动模块
*	文件名称 : bsp_mpu6050.c
*	版    本 : V1.0
*	说    明 : 实现MPU-6050的读写操作。
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2013-02-01 armfly  	正式发布
*		V1.1	2019-04-12 yy-AA55  新增任意数据长度读写函数，为了移植MPL库
*									 
*
*********************************************************************************************************
*/

/*
	应用说明：访问MPU-6050前，请先调用一次 bsp_InitI2C()函数配置好I2C相关的GPIO.
*/

#include "bsp.h"

MPU6050_T g_tMPU6050;		/* 定义一个全局变量，保存实时数据 */

/*
*********************************************************************************************************
*	函 数 名: bsp_InitMPU6050
*	功能说明: 初始化MPU-6050
*	形    参:  无
*	返 回 值: 1 表示正常， 0 表示不正常
*********************************************************************************************************
*/
void bsp_InitMPU6050(void)
{
	uint8_t ID;
	
//	bsp_InitI2C();
	ID = MPU6050_ReadByte(0x75);
	while(ID != 0x68 )
	{
		printf("MPU6050检测ID错误，Id = %x\r\n",ID );
	}
	printf("MPU6050_CheckId = %x\r\n",ID );
	MPU6050_WriteByte(PWR_MGMT_1, 0x00);	//解除休眠状态
	MPU6050_WriteByte(SMPLRT_DIV, 0x07);
	MPU6050_WriteByte(CONFIG, 0x06);
	MPU6050_WriteByte(GYRO_CONFIG, 0xE8);
	MPU6050_WriteByte(ACCEL_CONFIG, 0x01);
}

/*
*********************************************************************************************************
*	函 数 名: BMI160_Init
*	功能说明: 三轴加速度传感器 BMI160初始化
*	形    参:  无
*	返 回 值:  无
*********************************************************************************************************
*/

void MPU6050_Init(void)
{
	uint8_t ID;
//	uint8_t PMU_STATUS;
	
    bsp_InitI2C();
	bsp_DelayMS(400);

	while(0x68 !=MPU6050_ReadByte(0x75) )
	{
		ID = MPU6050_ReadByte(0x75);
		printf("MPU6050_CheckId = %x\r\n",ID );
	}
	ID = MPU6050_ReadByte(0x75);
	printf("MPU6050_CheckId = %x\r\n",ID );
	MPU6050_WriteByte(0x6B, 0x00);
//	MPU6050_WriteByte(0x19, 0x07);
	MPU6050_WriteByte(0x19, 0x13);
	MPU6050_WriteByte(0x1A, 0x04);
//	MPU6050_WriteByte(0x1A, 0x06);
	MPU6050_WriteByte(0x1B, 0x18);
	MPU6050_WriteByte(0x1C, 0x01);
//	MPU6050_WriteByte(0x1C, 0x19);
}

/*
*********************************************************************************************************
*	函 数 名: MPU6050_WriteByte
*	功能说明: 向 MPU-6050 寄存器写入一个数据
*	形    参: _ucRegAddr : 寄存器地址
*			  _ucRegData : 寄存器数据
*	返 回 值: 无
*********************************************************************************************************
*/
void MPU6050_WriteByte(uint8_t _ucRegAddr, uint8_t _ucRegData)
{
    i2c_Start();							/* 总线开始信号 */

    i2c_SendByte(MPU6050_SLAVE_ADDRESS);	/* 发送设备地址+写信号 */
	i2c_WaitAck();

    i2c_SendByte(_ucRegAddr);				/* 内部寄存器地址 */
	i2c_WaitAck();

    i2c_SendByte(_ucRegData);				/* 内部寄存器数据 */
	i2c_WaitAck();

    i2c_Stop();                   			/* 总线停止信号 */
}

/*
*********************************************************************************************************
*	函 数 名: MPU6050_ReadByte
*	功能说明: 读取 MPU-6050 寄存器的数据
*	形    参: _ucRegAddr : 寄存器地址
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t MPU6050_ReadByte(uint8_t _ucRegAddr)
{
	uint8_t ucData;

	i2c_Start();                  			/* 总线开始信号 */
	i2c_SendByte(MPU6050_SLAVE_ADDRESS);	/* 发送设备地址+写信号 */
	i2c_WaitAck();
	i2c_SendByte(_ucRegAddr);     			/* 发送存储单元地址 */
	i2c_WaitAck();

	i2c_Start();                  			/* 总线开始信号 */

	i2c_SendByte(MPU6050_SLAVE_ADDRESS+1); 	/* 发送设备地址+读信号 */
	i2c_WaitAck();

	ucData = i2c_ReadByte();       			/* 读出寄存器数据 */
	i2c_NAck();
	i2c_Stop();                  			/* 总线停止信号 */
	return ucData;
}


/*
*********************************************************************************************************
*	函 数 名: MPU6050_ReadData
*	功能说明: 读取 MPU-6050 数据寄存器， 结果保存在全局变量 g_tMPU6050.  主程序可以定时调用该程序刷新数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MPU6050_ReadData(void)
{
	uint8_t ucReadBuf[14];
	uint8_t i;
	uint8_t ack;

#if 1 /* 连续读 */
	i2c_Start();                  			/* 总线开始信号 */
	i2c_SendByte(MPU6050_SLAVE_ADDRESS);	/* 发送设备地址+写信号 */
	ack = i2c_WaitAck();
	if (ack != 0)
	{
		i2c_Stop(); 
		return;
	}
	i2c_SendByte(ACCEL_XOUT_H);     		/* 发送存储单元地址  */
	ack = i2c_WaitAck();
	if (ack != 0)
	{
		i2c_Stop(); 
		return;
	}

	i2c_Start();                  			/* 总线开始信号 */

	i2c_SendByte(MPU6050_SLAVE_ADDRESS + 1); /* 发送设备地址+读信号 */
	ack = i2c_WaitAck();
	if (ack != 0)
	{
		i2c_Stop(); 
		return;
	}

	for (i = 0; i < 13; i++)
	{
		ucReadBuf[i] = i2c_ReadByte();       			/* 读出寄存器数据 */
		i2c_Ack();
	}

	/* 读最后一个字节，时给 NAck */
	ucReadBuf[13] = i2c_ReadByte();
	i2c_NAck();

	i2c_Stop();                  			/* 总线停止信号 */

#else	/* 单字节读 */
	for (i = 0 ; i < 14; i++)
	{
		ucReadBuf[i] = MPU6050_ReadByte(ACCEL_XOUT_H + i);
	}
#endif

	/* 将读出的数据保存到全局结构体变量 */
	g_tMPU6050.Accel_X = (ucReadBuf[0] << 8) + ucReadBuf[1];
	g_tMPU6050.Accel_Y = (ucReadBuf[2] << 8) + ucReadBuf[3];
	g_tMPU6050.Accel_Z = (ucReadBuf[4] << 8) + ucReadBuf[5];

	g_tMPU6050.Temp = (int16_t)((ucReadBuf[6] << 8) + ucReadBuf[7]);

	g_tMPU6050.GYRO_X = (ucReadBuf[8] << 8) + ucReadBuf[9];
	g_tMPU6050.GYRO_Y = (ucReadBuf[10] << 8) + ucReadBuf[11];
	g_tMPU6050.GYRO_Z = (ucReadBuf[12] << 8) + ucReadBuf[13];
}

/*
*********************************************************************************************************
*	函 数 名: MPU6050_WriteLen
*	功能说明: MPU_6050连续写任意长度数据
*	形    参: _ucSlaveAddr：从设备地址
*			  _ucRegAddr：写入寄存器地址
*			  _ucLen：写入数据长度
*			  _pdata：写入数据指针
*	返 回 值: 0:正常     1：错误
*********************************************************************************************************
*/
uint8_t MPU6050_WriteLen(uint8_t _ucSlaveAddr, uint8_t _ucRegAddr, uint8_t _ucLen, uint8_t const *_pdata)
{
	uint8_t i;
	uint8_t ack;
	
	i2c_Start();                  			/* 总线开始信号 */
	i2c_SendByte(_ucSlaveAddr<<1|0);		/* 发送设备地址+写信号 */
	ack = i2c_WaitAck();
	if (ack != 0)
	{
		i2c_Stop(); 
		return 1;
	}
	i2c_SendByte(_ucRegAddr);     			/* 发送存储单元地址  */
	ack = i2c_WaitAck();
	if (ack != 0)
	{
		i2c_Stop(); 
		return 1;
	}

	for (i = 0; i < _ucLen; i++)
	{
		i2c_SendByte(_pdata[i]);       		/* 发数据 */
		ack = i2c_WaitAck();
		if (ack != 0)
		{
			i2c_Stop(); 
			return 1;
		}
	}

	i2c_Stop();   							/* 总线停止信号 */
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: MPU6050_ReadLen
*	功能说明: MPU_6050连续读任意长度数据
*	形    参: _ucSlaveAddr：从设备地址
*			  _ucRegAddr：写入寄存器地址
*			  _ucLen：写入数据长度
*			  _pdata：写入数据指针
*	返 回 值: 0:正常     1：错误
*********************************************************************************************************
*/
uint8_t MPU6050_ReadLen(uint8_t _ucSlaveAddr, uint8_t _ucRegAddr, uint8_t _ucLen, uint8_t *_pdata)
{
	uint8_t i;
	uint8_t ack;
	
	i2c_Start();                  			/* 总线开始信号 */
	i2c_SendByte(_ucSlaveAddr<<1|0);		/* 发送设备地址+写信号 */
	ack = i2c_WaitAck();
	if (ack != 0)
	{
		i2c_Stop(); 
		return 1;
	}
	i2c_SendByte(_ucRegAddr);     			/* 发送存储单元地址  */
	ack = i2c_WaitAck();
	if (ack != 0)
	{
		i2c_Stop(); 
		return 1;
	}

	i2c_Start();                  			/* 总线开始信号 */

	i2c_SendByte(_ucSlaveAddr<<1|1); 		/* 发送设备地址+读信号 */
	ack = i2c_WaitAck();
	if (ack != 0)
	{
		i2c_Stop(); 
		return 1;
	}

	for (i = 0; i < _ucLen - 1; i++)
	{
		*_pdata = i2c_ReadByte();       /* 读出寄存器数据 */
		i2c_Ack();
		_pdata++;
	}

	/* 读最后一个字节，时给 NAck */
	*_pdata = i2c_ReadByte();
	i2c_NAck();

	i2c_Stop();                  			/* 总线停止信号 */
	return 0;
}


/************************** 以下从L476复制过来的API **************************/
/*
*********************************************************************************************************
*	函 数 名: fasong
*	功能说明: 将DMP获取数据，封装成上位机协议格式，上传至上位机
*	形    参: roll：
*			  pitch：
*			  yaw：
*	返 回 值: 0:正常     1：错误
*********************************************************************************************************
*/
void fasong(signed short roll,signed short pitch,signed short yaw)
{
//		int8_t tbuf[12];
	signed short rolll,rollh,pitchl,pitchh,yawl,yawh,zsum;	
	rolll=roll&0XFF;
    rollh=(roll>>8)&0XFF;
	rollh=(signed short)rollh;
	
	pitchl=pitch&0XFF;
    pitchh=(pitch>>8)&0XFF;
	pitchh=(signed short)pitchh;
	
	yawl=yaw&0XFF;
    yawh=(yaw>>8)&0XFF;
	yawh=(signed short)yawh;
	
	zsum=0x55+0x53+rolll+rollh+pitchl+pitchh+yawl+yawh+0xFB+0x0E;
	
	comSendChar(COM1,0x55);
    comSendChar(COM1,0x53);
		
	comSendChar(COM1,rolll);
    comSendChar(COM1,rollh);
	comSendChar(COM1,pitchl);
    comSendChar(COM1,pitchh);
	comSendChar(COM1,yawl);
    comSendChar(COM1,yawh);
	comSendChar(COM1,0xFB);
    comSendChar(COM1,0x0E);
	comSendChar(COM1,zsum);
	
}


/***************************** yy-AA55 (END OF FILE) *********************************/
