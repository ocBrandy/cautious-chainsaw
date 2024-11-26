/* 
 * File:   collection.h
 * Author: cheng
 *
 * Created on July 30, 2024, 2:42 PM
 */

#ifndef COLLECTION_H
#define	COLLECTION_H

#ifdef	__cplusplus
extern "C" {
#endif
#define Dataflag 0xAA
#define Datatail 0x55
    

    
unsigned char GetTemperature(unsigned char sel);
int GetCableHeadVoltage(void);
unsigned int GetLMotorAcVoltage(void);
long Tension_sample(void);
int Get_Tension(void);
int Get_PumpPressure(void);
int Get_Displacement(unsigned char sel);
void collection (void);
int GetMCdownTemperature(void);
int Checksum (unsigned char *p,unsigned char len);



#ifdef	__cplusplus
}
#endif

#endif	/* COLLECTION_H */

