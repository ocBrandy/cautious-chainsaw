/*
 * fpga.h
 *
 *  Created on: Nov 15, 2024
 *      Author: 16194
 */

#ifndef INC_USER_FPGA_H_
#define INC_USER_FPGA_H_


void FPGA_init(void);
unsigned char Fpga_Read(unsigned char addRead);
void Fpga_Write(unsigned char addWrite,unsigned char Data);
void Write_Fir_Parameter(void);


#endif /* INC_USER_FPGA_H_ */
