/* 
 * File:   spi.h
 * Author: cheng
 *
 * Created on July 29, 2024, 2:39 PM
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif
#define MOEN 0xB8   //MOTOR Enable
#define MODI 0xA8   //MOTOR Disable
        
#define STEP_MODE 0x16

#define STEP_MODEGET 0x36
        



#define SPI_CS_Enable _LATB2 = 0
#define SPI_CS_Disable _LATB2 = 1

void SpiInit(void);
unsigned char SPI_RW(unsigned char WD);


#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

