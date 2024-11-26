#include "bsp.h"				/* 底层硬件驱动 */
#include "bsp_fpga.h"

void GPIO_FPGA_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    		//??????
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);		    		 			//A??

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    		//??????
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);		    		 			//E??

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 					//????
    GPIO_Init(GPIOA, &GPIO_InitStructure);									//A??

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 					//????
    GPIO_Init(GPIOB, &GPIO_InitStructure);									//B??

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;		 //LED2, LED3	 V7 V8
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 					//????
    GPIO_Init(GPIOC, &GPIO_InitStructure);									//C??

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 					//????
    GPIO_Init(GPIOD, &GPIO_InitStructure);									//D??

    GPIO_SetBits(GPIOE, GPIO_Pin_0);

    ////////////////////////////////////////
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;	 	//USART1 CFG/RST(EarthNet)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    		//??????
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);		    		 			//C??
    GPIO_SetBits(GPIOC, GPIO_Pin_8);
    GPIO_SetBits(GPIOC, GPIO_Pin_9);

}

/****************************************************************************
* ?    ?:Fpga_Read
* ?    ?:ARM?FPGA???
* ????:addRead
* ????:?
* ?    ?:
* ????:?
****************************************************************************/
uint8_t Fpga_Read( unsigned char addRead)
{
    uint8_t  data,A,B,C,D,E,F,G,H;

    GPIO_ResetBits(GPIOA, GPIO_Pin_11); //WR=0,
    //???
    if(addRead & 0x08)
        GPIO_SetBits(GPIOE, GPIO_Pin_5);
    else
        GPIO_ResetBits(GPIOE, GPIO_Pin_5);

    if(addRead & 0x04)
        GPIO_SetBits(GPIOE, GPIO_Pin_4);
    else
        GPIO_ResetBits(GPIOE, GPIO_Pin_4);

    if(addRead & 0x02)
        GPIO_SetBits(GPIOE, GPIO_Pin_3);
    else
        GPIO_ResetBits(GPIOE, GPIO_Pin_3);

    if(addRead & 0x01)
        GPIO_SetBits(GPIOE, GPIO_Pin_2);
    else
        GPIO_ResetBits(GPIOE, GPIO_Pin_2);

    GPIO_ResetBits(GPIOE, GPIO_Pin_0); 	//CS=0,enable

    A = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12);
    B = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10);
    C = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11);
    D = GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3);
    E = GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6);
    F = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5);
    G = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8);
    H = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9);

    GPIO_SetBits(GPIOE, GPIO_Pin_0);  	//CS=1,Disable
    data = H << 7;
    data += G <<  6;
    data += F <<  5;
    data += E <<  4;
    data += D <<  3;
    data += C <<  2;
    data += B <<  1;
    data += A;

    return data;
}

/****************************************************************************
* ?    ?:Fpga_Write
* ?    ?:ARM?FPGA????
* ????:addWrite
* ????:?
* ?    ?:
* ????:?
****************************************************************************/
void Fpga_Write( unsigned char addWrite,unsigned char Data)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //--------------???8bits?????------------------
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9;	//???A5,A6,A7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   	 			//????
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    		//??????
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_6;			//???A3,A4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   	 			//????
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;			//???A1,A2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   	 			//????
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;						//???A0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   	 			//????
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //-----------------------------------------------------

    GPIO_SetBits(GPIOA, GPIO_Pin_11); 	//WR=1,
    //???
    if(addWrite & 0x08)
        GPIO_SetBits(GPIOE, GPIO_Pin_5);
    else
        GPIO_ResetBits(GPIOE, GPIO_Pin_5);

    if(addWrite & 0x04)
        GPIO_SetBits(GPIOE, GPIO_Pin_4);
    else
        GPIO_ResetBits(GPIOE, GPIO_Pin_4);

    if(addWrite & 0x02)
        GPIO_SetBits(GPIOE, GPIO_Pin_3);
    else
        GPIO_ResetBits(GPIOE, GPIO_Pin_3);

    if(addWrite & 0x01)
        GPIO_SetBits(GPIOE, GPIO_Pin_2);
    else
        GPIO_ResetBits(GPIOE, GPIO_Pin_2);


    //???
    if(Data & 0x80)
        GPIO_SetBits(GPIOB, GPIO_Pin_9);
    else
        GPIO_ResetBits(GPIOB, GPIO_Pin_9);

    if(Data & 0x40)
        GPIO_SetBits(GPIOB, GPIO_Pin_8);
    else
        GPIO_ResetBits(GPIOB, GPIO_Pin_8);

    if(Data & 0x20)
        GPIO_SetBits(GPIOB, GPIO_Pin_5);
    else
        GPIO_ResetBits(GPIOB, GPIO_Pin_5);

    if(Data & 0x10)
        GPIO_SetBits(GPIOD, GPIO_Pin_6);
    else
        GPIO_ResetBits(GPIOD, GPIO_Pin_6);

    if(Data & 0x08)
        GPIO_SetBits(GPIOD, GPIO_Pin_3);
    else
        GPIO_ResetBits(GPIOD, GPIO_Pin_3);

    if(Data & 0x04)
        GPIO_SetBits(GPIOC, GPIO_Pin_11);
    else
        GPIO_ResetBits(GPIOC, GPIO_Pin_11);

    if(Data & 0x02)
        GPIO_SetBits(GPIOC, GPIO_Pin_10);
    else
        GPIO_ResetBits(GPIOC, GPIO_Pin_10);

    if(Data & 0x01)
        GPIO_SetBits(GPIOA, GPIO_Pin_12);
    else
        GPIO_ResetBits(GPIOA, GPIO_Pin_12);

    GPIO_ResetBits(GPIOE, GPIO_Pin_0); 	//CS=0,enable
    GPIO_SetBits(GPIOE, GPIO_Pin_0);  	//CS=1,Disable

    //--------------???8bits?????------------------
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9;	//???A5,A6,A7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	 	//????
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_6;			//???A3,A4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	 	//????
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;			//???A1,A2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	 	//????
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;						//???A0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	 	//????
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //-----------------------------------------------------
}
/*给FPGA写入GMMA值*/
//static uint8_t *p_data;
//static uint16_t length;

static uint8_t  gmma_index = 0;
static uint16_t gmma_buf[4] = {0};
void WriteGmmaCntToFpga(uint16_t gmma_cnt)
{
    uint16_t gmma;
    
    if(gmma_index >= 4) {gmma_index = 0;}
    
    gmma_buf[gmma_index++] = gmma_cnt;
    gmma = gmma_buf[0] + gmma_buf[1] + gmma_buf[2] + gmma_buf[3];
    
    Fpga_Write(0x0d,gmma>>8);//gama转模拟
    Fpga_Write(0x0e,gmma&0x00ff);
}
