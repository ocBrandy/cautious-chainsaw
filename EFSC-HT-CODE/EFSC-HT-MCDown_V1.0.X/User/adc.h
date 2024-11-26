/* 
 * File:   adc.h
 * Author: cheng
 *
 * Created on July 29, 2024, 5:44 PM
 */

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif
void adcInit(void);
unsigned int ads8320(void);
void Adc_Channel_SW(unsigned char channel);
void Adc_Channel6_Channel_SW(unsigned char channel);
int ADC(void);



#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */