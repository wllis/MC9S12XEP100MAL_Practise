/**********************(c) Copyright 20017-2018, CIDI **********************
	File： LCD_12864.h
	Description：  

    Author：wllis
    Date: 04/17/2018
      
    Note:
  
  Version: 1.0

**************************************************************************/

#ifndef __USART_H
#define __USART_H


extern unsigned char uart_getkey(void);
extern void uart_init(void);
extern void uart_putchar(unsigned char ch);
extern void putstr(char ch[]);
//unsigned char TERMIO_GetChar(void);
void printp( char * , ...);

#endif

/************************* enf of file **********************************/