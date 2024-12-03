/* 
 * File:   init.h
 * Author: CJCJ
 *
 * Created on November 14, 2024, 10:10 AM
 */

#ifndef INIT_H
#define	INIT_H

#ifdef	__cplusplus
extern "C" {
#endif
void init(void);
void gpio_initial(void);
void UsartInit_1(void);
void UsartInit_2(void);
void bsp_InitTimer3(void);


unsigned int Checksum (unsigned char *p,unsigned int _len);


#ifdef	__cplusplus
}
#endif

#endif	/* INIT_H */

