/*
 * Fpga.c
 *
 *  Created on: Nov 15, 2024
 *      Author: 16194
 */


#include "main.h"

const int FIR_Parameter[255] = {
    64,     -4,     -8,    -11,    -11,     -8,     -5,     -7,    -14,
    -20,    -18,     -6,     14,     31,     33,     16,    -15,    -43,
    -53,    -39,     -8,     23,     39,     33,     13,     -6,    -12,
    -4,      5,      3,    -16,    -41,    -53,    -36,      8,     60,
    90,     76,     24,    -42,    -86,    -86,    -48,      4,     38,
    39,     18,      0,      7,     37,     63,     55,      0,    -81,
    -143,   -140,    -63,     55,    154,    180,    120,     10,    -87,
    -124,    -92,    -31,     10,      2,    -36,    -56,    -15,     84,
    186,    215,    127,    -54,   -239,   -324,   -255,    -62,    153,
    277,    256,    124,    -23,    -95,    -70,     -2,     17,    -61,
    -200,   -288,   -218,     27,    345,    555,    510,    197,   -241,
    -575,   -630,   -388,      0,    309,    386,    247,     56,     -1,
    138,    341,    369,     47,   -564,  -1144,  -1271,   -694,    460,
    1671,   2267,   1797,    332,  -1503,  -2801,  -2848,  -1521,    604,				//fpga滤波参数
    2516,   3280,   2516,    604,  -1521,  -2848,  -2801,  -1503,    332,
    1797,   2267,   1671,    460,   -694,  -1271,  -1144,   -564,     47,
    369,    341,    138,     -1,     56,    247,    386,    309,      0,
    -388,   -630,   -575,   -241,    197,    510,    555,    345,     27,
    -218,   -288,   -200,    -61,     17,     -2,    -70,    -95,    -23,
    124,    256,    277,    153,    -62,   -255,   -324,   -239,    -54,
    127,    215,    186,     84,    -15,    -56,    -36,      2,     10,
    -31,    -92,   -124,    -87,     10,    120,    180,    154,     55,
    -63,   -140,   -143,    -81,      0,     55,     63,     37,      7,
    0,     18,     39,     38,      4,    -48,    -86,    -86,    -42,
    24,     76,     90,     60,      8,    -36,    -53,    -41,    -16,
    3,      5,     -4,    -12,     -6,     13,     33,     39,     23,
    -8,    -39,    -53,    -43,    -15,     16,     33,     31,     14,
    -6,    -18,    -20,    -14,     -7,     -5,     -8,    -11,    -11,
    -8,     -4,     64
};


GPIO_InitTypeDef GPIO_InitStruct = {0};


unsigned char fp_online;

void FPGA_init(void)
{
	while(fp_online != 0xA8)
	{
		fp_online = Fpga_Read(0x00);
		for(unsigned int i=0;i<1000;i++)
		{
			for(unsigned int j=0;j<5000;j++);
		}
	}
	Write_Fir_Parameter();
}

unsigned char Fpga_Read(unsigned char addRead)
{
	unsigned char data,A,B,C,D,E,F,G,H;

	/*-----------------------地址&片选初始化，�?�时为输�?----------------------------*/

	GPIO_InitStruct.Pin = ADDR0_Pin|ADDR1_Pin|ADDR2_Pin|ADDR3_Pin|MCU_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*-----------------------读写初�?�化，�?�时为输�?---------------------------------*/

	GPIO_InitStruct.Pin = MCU_WR_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*-----------------------数据初�?�化，�?�时为输�?---------------------------------*/

	GPIO_InitStruct.Pin = DATA1_Pin|DATA0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = DATA2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DATA2_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = DATA3_Pin|DATA4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = DATA5_Pin|DATA6_Pin|DATA7_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


	HAL_GPIO_WritePin(GPIOE,MCU_WR_Pin,GPIO_PIN_RESET);			//�?:WR拉低

	if(addRead & 0x08)
		HAL_GPIO_WritePin(ADDR3_GPIO_Port,ADDR3_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(ADDR3_GPIO_Port,ADDR3_Pin,GPIO_PIN_RESET);

	if(addRead & 0x04)
		HAL_GPIO_WritePin(ADDR2_GPIO_Port,ADDR2_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(ADDR2_GPIO_Port,ADDR2_Pin,GPIO_PIN_RESET);		//�?:addRead = d'6/b'0110,  ADDR3=0,ADDR2=1,ADDR1=1;ADDR0=0

	if(addRead & 0x02)
		HAL_GPIO_WritePin(ADDR1_GPIO_Port,ADDR1_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(ADDR1_GPIO_Port,ADDR1_Pin,GPIO_PIN_RESET);

	if(addRead & 0x01)
		HAL_GPIO_WritePin(ADDR0_GPIO_Port,ADDR0_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(ADDR0_GPIO_Port,ADDR0_Pin,GPIO_PIN_RESET);

	HAL_GPIO_WritePin(GPIOE,MCU_CS_Pin,GPIO_PIN_RESET);			//片选拉�?

	A = HAL_GPIO_ReadPin(DATA0_GPIO_Port,DATA0_Pin);
	B = HAL_GPIO_ReadPin(DATA1_GPIO_Port,DATA1_Pin);
	C = HAL_GPIO_ReadPin(DATA2_GPIO_Port,DATA2_Pin);
	D = HAL_GPIO_ReadPin(DATA3_GPIO_Port,DATA3_Pin);
	E = HAL_GPIO_ReadPin(DATA4_GPIO_Port,DATA4_Pin);
	F = HAL_GPIO_ReadPin(DATA5_GPIO_Port,DATA5_Pin);
	G = HAL_GPIO_ReadPin(DATA6_GPIO_Port,DATA6_Pin);
	H = HAL_GPIO_ReadPin(DATA7_GPIO_Port,DATA7_Pin);

	HAL_GPIO_WritePin(GPIOE,MCU_CS_Pin,GPIO_PIN_SET);			//片选拉�?

	data =  H<<7;
	data += G<<6;
	data += F<<5;
	data += E<<4;
	data += D<<3;
	data += C<<2;
	data += B<<1;
	data += A;

	return data;
}

void Fpga_Write(unsigned char addWrite,unsigned char Data)
{

	/*------------------------------数据，写时为输出---------------------------------*/

	GPIO_InitStruct.Pin = DATA1_Pin|DATA0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = DATA2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DATA2_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = DATA3_Pin|DATA4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = DATA5_Pin|DATA6_Pin|DATA7_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOE,MCU_CS_Pin,GPIO_PIN_SET);			//片选拉�?

	/*----------------------4位地址--------------------------------*/
	if(addWrite & 0x08)
		HAL_GPIO_WritePin(ADDR3_GPIO_Port,ADDR3_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(ADDR3_GPIO_Port,ADDR3_Pin,GPIO_PIN_RESET);

	if(addWrite & 0x04)
		HAL_GPIO_WritePin(ADDR2_GPIO_Port,ADDR2_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(ADDR2_GPIO_Port,ADDR2_Pin,GPIO_PIN_RESET);		//�?:addRead = d'6/b'0110,  ADDR3=0,ADDR2=1,ADDR1=1;ADDR0=0

	if(addWrite & 0x02)
		HAL_GPIO_WritePin(ADDR1_GPIO_Port,ADDR1_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(ADDR1_GPIO_Port,ADDR1_Pin,GPIO_PIN_RESET);

	if(addWrite & 0x01)
		HAL_GPIO_WritePin(ADDR0_GPIO_Port,ADDR0_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(ADDR0_GPIO_Port,ADDR0_Pin,GPIO_PIN_RESET);

	/*----------------------8位数�?--------------------------------*/
	if(Data & 0x80)
		HAL_GPIO_WritePin(DATA7_GPIO_Port,DATA7_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(DATA7_GPIO_Port,DATA7_Pin,GPIO_PIN_RESET);

	if(Data & 0x40)
		HAL_GPIO_WritePin(DATA6_GPIO_Port,DATA6_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(DATA6_GPIO_Port,DATA6_Pin,GPIO_PIN_RESET);

	if(Data & 0x20)
		HAL_GPIO_WritePin(DATA5_GPIO_Port,DATA5_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(DATA5_GPIO_Port,DATA5_Pin,GPIO_PIN_RESET);

	if(Data & 0x10)
		HAL_GPIO_WritePin(DATA4_GPIO_Port,DATA4_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(DATA4_GPIO_Port,DATA4_Pin,GPIO_PIN_RESET);

	if(Data & 0x08)
		HAL_GPIO_WritePin(DATA3_GPIO_Port,DATA3_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(DATA3_GPIO_Port,DATA3_Pin,GPIO_PIN_RESET);

	if(Data & 0x04)
		HAL_GPIO_WritePin(DATA2_GPIO_Port,DATA2_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(DATA2_GPIO_Port,DATA2_Pin,GPIO_PIN_RESET);

	if(Data & 0x02)
		HAL_GPIO_WritePin(DATA1_GPIO_Port,DATA1_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(DATA1_GPIO_Port,DATA1_Pin,GPIO_PIN_RESET);

	if(Data & 0x01)
		HAL_GPIO_WritePin(DATA0_GPIO_Port,DATA0_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(DATA0_GPIO_Port,DATA0_Pin,GPIO_PIN_RESET);

	HAL_GPIO_WritePin(GPIOE,MCU_CS_Pin,GPIO_PIN_RESET);			//片选拉�?

	HAL_GPIO_WritePin(GPIOE,MCU_CS_Pin,GPIO_PIN_SET);			//片选拉�?


	GPIO_InitStruct.Pin = DATA1_Pin|DATA0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = DATA2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DATA2_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = DATA3_Pin|DATA4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = DATA5_Pin|DATA6_Pin|DATA7_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

#define Filter_Order 		255

void Write_Fir_Parameter(void)
{
    unsigned int i,j,t,d;
	unsigned char init_cn;
	unsigned char sum1,sum2;

    Fpga_Write(7,Filter_Order >> 8);
    Fpga_Write(8,(Filter_Order & 0x00FF));

	init_cn = 0;
	while(init_cn < 10)
	{
		sum1 = 0;
		sum2 = 0;

		t = 0x80;
		for(i=0; i<Filter_Order;)
		{
			for(j=0; j<32; j++,i++)
			{
				if(i < Filter_Order)
				{
					Fpga_Write(0x09,j);
					Fpga_Write(0x0A,FIR_Parameter[i] >> 8);
					Fpga_Write(0x0B,FIR_Parameter[i]);
					Fpga_Write(0x0C,t);		//Fir_Parameter_Ram_WR(MSB),Fir_Parameter_Ram_sel
					sum1 += FIR_Parameter[i];
				}
				else
				{
					Fpga_Write(0x09,j);
					Fpga_Write(0x0A,0);
					Fpga_Write(0x0B,0);
					Fpga_Write(0x0C,t);		//Fir_Parameter_Ram_WR(MSB),Fir_Parameter_Ram_sel
				}
			}
			t++;
		}

		t = 0x00;
		for(i=0; i<Filter_Order;)
		{
			for(j=0; j<32; j++,i++)
			{
				if(i < Filter_Order)
				{
					Fpga_Write(0x09,j);
					Fpga_Write(0x0C,t);		//Fir_Parameter_Ram_WR(MSB),Fir_Parameter_Ram_sel
					d = Fpga_Read(0x0A);
					d = d << 8;
					d |= Fpga_Read(0x0B);
					sum2 += d;
				}
				else
				{
					d = 0;
				}
			}
			t++;
		}

		if(sum1 != sum2)
			init_cn++;
		else
			init_cn = 10;
	}
//
//	comSendChar(COM1,(sum1 >> 8));
//	comSendChar(COM1,sum1);
//
//	comSendChar(COM1,(sum2 >> 8));
//	comSendChar(COM1,sum2);
}

















