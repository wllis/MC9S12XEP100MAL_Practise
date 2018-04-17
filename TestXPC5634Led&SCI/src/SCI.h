

/**********************(c) Copyright 20017-2018, CIDI **********************
	File: SCI.h
	Description: 
    Author:
    Date: Mar/28/2018
      
    Note:

**************************************************************************/


#ifndef __SCI_H
#define __SCI_H

void InitESCIA ( void );
void InitESCIB ( void );
void SCIA_Putchar( char c );
void SCIB_Putchar( char c );
unsigned char SCIA_Getchar( void );
unsigned char SCIB_Getchar( void );
void SCIA_Putstr(char *str);
void SCIB_Putstr(char *str);

#endif


/*************************** end of file *********************************/