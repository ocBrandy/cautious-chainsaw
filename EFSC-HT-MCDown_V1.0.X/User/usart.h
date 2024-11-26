/* 
 * File:   usart.h
 * Author: cheng
 *
 * Created on July 29, 2024, 2:46 PM
 */

#ifndef USART_H
#define	USART_H

#ifdef	__cplusplus
extern "C" {
#endif

void UsartInit(void);
void ComSendBuf(unsigned char com,unsigned char *data,unsigned int len);
void cmdpoll(void);
void ComSendChar(unsigned char com,unsigned char data);
void DataUpload(void);

#ifdef	__cplusplus
}
#endif

#endif	/* USART_H */

