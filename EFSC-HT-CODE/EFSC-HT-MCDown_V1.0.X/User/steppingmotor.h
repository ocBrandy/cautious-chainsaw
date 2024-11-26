/* 
 * File:   steppingmotor.h
 * Author: cheng
 *
 * Created on July 30, 2024, 2:12 PM
 */

#ifndef STEPPINGMOTOR_H
#define	STEPPINGMOTOR_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <p30f6014a.h>

#define BSR_ENTER_CRITICAL_REGION        __asm__ volatile("disi #0x3FFF")//DISICNT = 0x3FFF is not working , I can't see why 
#define BSR_EXIT_CRITICAL_REGION         __asm__ volatile("disi #0x0000")//DISICNT = 0    

#define BSR_MAX_BLOCK_SIZE                    400
    
#define PLUSE_CNT_1_point_5     100
        
#define Full_step                0x88
#define Half_step                0x89
#define Onefourth_step           0x8A
#define Oneeighth_step           0x8B
#define Onesexteenth_step        0x8C
    
    
void ReayForPluse(unsigned int pluse_num);
void MotorRun(unsigned int pluse_num,unsigned char direct);
void MotorStop(void);
void SerParam(unsigned char addr,unsigned char value);
void StepSet(unsigned char step);
unsigned char GetParam(unsigned char addr);



#ifdef	__cplusplus
}
#endif

#endif	/* STEPPINGMOTOR_H */

