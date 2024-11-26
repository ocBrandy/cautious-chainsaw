#ifndef __FPGA_H
#define	__FPGA_H

#include "stm32f10x.h"
uint8_t Fpga_Read( unsigned char addRead);
void Fpga_Write( unsigned char addWrite,unsigned char Data);
void GPIO_FPGA_Configuration(void);
void delay(void);
void WriteGmmaCntToFpga(uint16_t gmma_cnt);
#endif
