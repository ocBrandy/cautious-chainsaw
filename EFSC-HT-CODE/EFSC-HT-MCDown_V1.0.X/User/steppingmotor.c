#include "gpio.h"
#include "spi.h"
#include "timer.h"
#include "steppingmotor.h"



void MotorRun(unsigned int pluse_num,unsigned char direct)
{
    _T5IE = 1;
    SPI_CS_Enable;
    SPI_RW(MOEN);
    SPI_CS_Disable;
    GPIO_PIN_SET(MOTOR_DIRECTION_PORT,MOTOR_DIRECTION_PIN,direct);
    ReayForPluse(pluse_num);
}

void MotorStop(void)
{
    //GPIO_PIN_SET(MOTOR_EN_PORT,MOTOR_EN_PIN,1);
    SPI_CS_Enable;
    SPI_RW(MODI);
    SPI_CS_Disable;
   _T5IE = 0;
}

void StepSet(unsigned char step)
{
    SerParam(STEP_MODE,step);
}

void SerParam(unsigned char addr,unsigned char value)
{
    SPI_CS_Enable;
    SPI_RW(addr);
    SPI_CS_Disable;
    
    SPI_CS_Enable;
    SPI_RW(value);
    SPI_CS_Disable;
}

unsigned char GetParam(unsigned char addr)
{
    unsigned char DD[3];
    SPI_CS_Enable;
    SPI_RW(addr);
    SPI_CS_Disable;
    
    SPI_CS_Enable;
    DD[0] = SPI_RW(0);
    SPI_CS_Disable;
    return DD[0];
}
    
unsigned int pluse_cnt = 0; 
void ReayForPluse(unsigned int pluse_num)
{
    BSR_ENTER_CRITICAL_REGION;
    pluse_cnt = pluse_num*2;
    BSR_EXIT_CRITICAL_REGION;
}