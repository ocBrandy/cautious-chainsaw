/*
*********************************************************************************************************
*
*	ģ������ : ����������MPU-6050����ģ��
*	�ļ����� : bsp_mpu6050.c
*	��    �� : V1.0
*	˵    �� : ʵ��MPU-6050�Ķ�д������
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-02-01 armfly  	��ʽ����
*		V1.1	2019-04-12 yy-AA55  �����������ݳ��ȶ�д������Ϊ����ֲMPL��
*									 
*
*********************************************************************************************************
*/

/*
	Ӧ��˵��������MPU-6050ǰ�����ȵ���һ�� bsp_InitI2C()�������ú�I2C��ص�GPIO.
*/

#include "bsp.h"

MPU6050_T g_tMPU6050;		/* ����һ��ȫ�ֱ���������ʵʱ���� */

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitMPU6050
*	����˵��: ��ʼ��MPU-6050
*	��    ��:  ��
*	�� �� ֵ: 1 ��ʾ������ 0 ��ʾ������
*********************************************************************************************************
*/
void bsp_InitMPU6050(void)
{
	uint8_t ID;
	
//	bsp_InitI2C();
	ID = MPU6050_ReadByte(0x75);
	while(ID != 0x68 )
	{
		printf("MPU6050���ID����Id = %x\r\n",ID );
	}
	printf("MPU6050_CheckId = %x\r\n",ID );
	MPU6050_WriteByte(PWR_MGMT_1, 0x00);	//�������״̬
	MPU6050_WriteByte(SMPLRT_DIV, 0x07);
	MPU6050_WriteByte(CONFIG, 0x06);
	MPU6050_WriteByte(GYRO_CONFIG, 0xE8);
	MPU6050_WriteByte(ACCEL_CONFIG, 0x01);
}

/*
*********************************************************************************************************
*	�� �� ��: BMI160_Init
*	����˵��: ������ٶȴ����� BMI160��ʼ��
*	��    ��:  ��
*	�� �� ֵ:  ��
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
*	�� �� ��: MPU6050_WriteByte
*	����˵��: �� MPU-6050 �Ĵ���д��һ������
*	��    ��: _ucRegAddr : �Ĵ�����ַ
*			  _ucRegData : �Ĵ�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void MPU6050_WriteByte(uint8_t _ucRegAddr, uint8_t _ucRegData)
{
    i2c_Start();							/* ���߿�ʼ�ź� */

    i2c_SendByte(MPU6050_SLAVE_ADDRESS);	/* �����豸��ַ+д�ź� */
	i2c_WaitAck();

    i2c_SendByte(_ucRegAddr);				/* �ڲ��Ĵ�����ַ */
	i2c_WaitAck();

    i2c_SendByte(_ucRegData);				/* �ڲ��Ĵ������� */
	i2c_WaitAck();

    i2c_Stop();                   			/* ����ֹͣ�ź� */
}

/*
*********************************************************************************************************
*	�� �� ��: MPU6050_ReadByte
*	����˵��: ��ȡ MPU-6050 �Ĵ���������
*	��    ��: _ucRegAddr : �Ĵ�����ַ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t MPU6050_ReadByte(uint8_t _ucRegAddr)
{
	uint8_t ucData;

	i2c_Start();                  			/* ���߿�ʼ�ź� */
	i2c_SendByte(MPU6050_SLAVE_ADDRESS);	/* �����豸��ַ+д�ź� */
	i2c_WaitAck();
	i2c_SendByte(_ucRegAddr);     			/* ���ʹ洢��Ԫ��ַ */
	i2c_WaitAck();

	i2c_Start();                  			/* ���߿�ʼ�ź� */

	i2c_SendByte(MPU6050_SLAVE_ADDRESS+1); 	/* �����豸��ַ+���ź� */
	i2c_WaitAck();

	ucData = i2c_ReadByte();       			/* �����Ĵ������� */
	i2c_NAck();
	i2c_Stop();                  			/* ����ֹͣ�ź� */
	return ucData;
}


/*
*********************************************************************************************************
*	�� �� ��: MPU6050_ReadData
*	����˵��: ��ȡ MPU-6050 ���ݼĴ����� ���������ȫ�ֱ��� g_tMPU6050.  ��������Զ�ʱ���øó���ˢ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void MPU6050_ReadData(void)
{
	uint8_t ucReadBuf[14];
	uint8_t i;
	uint8_t ack;

#if 1 /* ������ */
	i2c_Start();                  			/* ���߿�ʼ�ź� */
	i2c_SendByte(MPU6050_SLAVE_ADDRESS);	/* �����豸��ַ+д�ź� */
	ack = i2c_WaitAck();
	if (ack != 0)
	{
		i2c_Stop(); 
		return;
	}
	i2c_SendByte(ACCEL_XOUT_H);     		/* ���ʹ洢��Ԫ��ַ  */
	ack = i2c_WaitAck();
	if (ack != 0)
	{
		i2c_Stop(); 
		return;
	}

	i2c_Start();                  			/* ���߿�ʼ�ź� */

	i2c_SendByte(MPU6050_SLAVE_ADDRESS + 1); /* �����豸��ַ+���ź� */
	ack = i2c_WaitAck();
	if (ack != 0)
	{
		i2c_Stop(); 
		return;
	}

	for (i = 0; i < 13; i++)
	{
		ucReadBuf[i] = i2c_ReadByte();       			/* �����Ĵ������� */
		i2c_Ack();
	}

	/* �����һ���ֽڣ�ʱ�� NAck */
	ucReadBuf[13] = i2c_ReadByte();
	i2c_NAck();

	i2c_Stop();                  			/* ����ֹͣ�ź� */

#else	/* ���ֽڶ� */
	for (i = 0 ; i < 14; i++)
	{
		ucReadBuf[i] = MPU6050_ReadByte(ACCEL_XOUT_H + i);
	}
#endif

	/* �����������ݱ��浽ȫ�ֽṹ����� */
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
*	�� �� ��: MPU6050_WriteLen
*	����˵��: MPU_6050����д���ⳤ������
*	��    ��: _ucSlaveAddr�����豸��ַ
*			  _ucRegAddr��д��Ĵ�����ַ
*			  _ucLen��д�����ݳ���
*			  _pdata��д������ָ��
*	�� �� ֵ: 0:����     1������
*********************************************************************************************************
*/
uint8_t MPU6050_WriteLen(uint8_t _ucSlaveAddr, uint8_t _ucRegAddr, uint8_t _ucLen, uint8_t const *_pdata)
{
	uint8_t i;
	uint8_t ack;
	
	i2c_Start();                  			/* ���߿�ʼ�ź� */
	i2c_SendByte(_ucSlaveAddr<<1|0);		/* �����豸��ַ+д�ź� */
	ack = i2c_WaitAck();
	if (ack != 0)
	{
		i2c_Stop(); 
		return 1;
	}
	i2c_SendByte(_ucRegAddr);     			/* ���ʹ洢��Ԫ��ַ  */
	ack = i2c_WaitAck();
	if (ack != 0)
	{
		i2c_Stop(); 
		return 1;
	}

	for (i = 0; i < _ucLen; i++)
	{
		i2c_SendByte(_pdata[i]);       		/* ������ */
		ack = i2c_WaitAck();
		if (ack != 0)
		{
			i2c_Stop(); 
			return 1;
		}
	}

	i2c_Stop();   							/* ����ֹͣ�ź� */
	return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: MPU6050_ReadLen
*	����˵��: MPU_6050���������ⳤ������
*	��    ��: _ucSlaveAddr�����豸��ַ
*			  _ucRegAddr��д��Ĵ�����ַ
*			  _ucLen��д�����ݳ���
*			  _pdata��д������ָ��
*	�� �� ֵ: 0:����     1������
*********************************************************************************************************
*/
uint8_t MPU6050_ReadLen(uint8_t _ucSlaveAddr, uint8_t _ucRegAddr, uint8_t _ucLen, uint8_t *_pdata)
{
	uint8_t i;
	uint8_t ack;
	
	i2c_Start();                  			/* ���߿�ʼ�ź� */
	i2c_SendByte(_ucSlaveAddr<<1|0);		/* �����豸��ַ+д�ź� */
	ack = i2c_WaitAck();
	if (ack != 0)
	{
		i2c_Stop(); 
		return 1;
	}
	i2c_SendByte(_ucRegAddr);     			/* ���ʹ洢��Ԫ��ַ  */
	ack = i2c_WaitAck();
	if (ack != 0)
	{
		i2c_Stop(); 
		return 1;
	}

	i2c_Start();                  			/* ���߿�ʼ�ź� */

	i2c_SendByte(_ucSlaveAddr<<1|1); 		/* �����豸��ַ+���ź� */
	ack = i2c_WaitAck();
	if (ack != 0)
	{
		i2c_Stop(); 
		return 1;
	}

	for (i = 0; i < _ucLen - 1; i++)
	{
		*_pdata = i2c_ReadByte();       /* �����Ĵ������� */
		i2c_Ack();
		_pdata++;
	}

	/* �����һ���ֽڣ�ʱ�� NAck */
	*_pdata = i2c_ReadByte();
	i2c_NAck();

	i2c_Stop();                  			/* ����ֹͣ�ź� */
	return 0;
}


/************************** ���´�L476���ƹ�����API **************************/
/*
*********************************************************************************************************
*	�� �� ��: fasong
*	����˵��: ��DMP��ȡ���ݣ���װ����λ��Э���ʽ���ϴ�����λ��
*	��    ��: roll��
*			  pitch��
*			  yaw��
*	�� �� ֵ: 0:����     1������
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
