/* 
 * File:   usart.h
 * Author: CJCJ
 *
 * Created on November 14, 2024, 10:10 AM
 */

#ifndef USART_H
#define	USART_H
#include <p30f6014a.h>
#ifdef	__cplusplus
extern "C" {
#endif

void UsartInit(void);
void ComSendChar (unsigned char com,unsigned char data);
void ComSendBuf(unsigned char com,unsigned char *data,unsigned int len );


#ifdef	__cplusplus
}
#endif

#endif	/* USART_H */

