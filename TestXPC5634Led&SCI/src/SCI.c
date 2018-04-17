/**********************(c) Copyright 20017-2018, CIDI **********************
	File: SCI.c
	Description: 
    Author:
    Date: Mar/28/2018
      
    Note:

**************************************************************************/

#include "config.h"

/************************************************************
 *  Name:   InitESCIA
 *  Input:  none
 *  output: none
 *  Description:  ESCIA Initial & configure parameter
 *  Note:
 * *********************************************************/
void InitESCIA (void) {
  ESCI_A.CR2.R = 0x2000;      /* Module is enabled (default setting ) */
  //ESCI_A.CR1.R = 0x01A1000C;  /* 64M:9600 baud, 8 bits, no parity, Tx & Rx enabled */
  ESCI_A.CR1.R = 0x0208000C;  /* 80M:9600 baud, 8 bits, no parity, Tx & Rx enabled */
  SIU.PCR[89].R = 0x400;      /* Configure pad for primary func: TxDA */
  SIU.PCR[90].R = 0x400;      /* Configure pad for primary func: RxDA */
}
/************************************************************
 *  Name:   InitESCIB
 *  Input:  none
 *  output: none
 *  Description:  ESCIB Initial & configure parameter
 *  Note:
 * *********************************************************/
void InitESCIB (void) {
  ESCI_B.CR2.R = 0x2000;      /* Module is enabled (default setting ) */
  //ESCI_B.CR1.R = 0x01A1000C;  /*64M: 9600 baud, 8 bits, no parity, Tx & Rx enabled */
  ESCI_B.CR1.R = 0x0208000C;  /* 80M:9600 baud, 8 bits, no parity, Tx & Rx enabled */
  SIU.PCR[91].R = 0x400;      /* Configure pad for primary func: TxDA */
  SIU.PCR[92].R = 0x400;      /* Configure pad for primary func: RxDA */
}
/************************************************************
 *  Name:   SCIA_Putchar
 *  Input:  none
 *  output: none
 *  Description:  ESCIA Send a char
 *  Note:
 * *********************************************************/
void SCIA_Putchar(char c)
{
    while (ESCI_A.SR.B.TDRE == 0) {}       /* Wait for transmit data reg empty = 1 */
    ESCI_A.SR.R = 0x80000000;              /* Clear TDRE flag */
	ESCI_A.DR.B.D = c;          /* Transmit 8 bits Data */
}
/************************************************************
 *  Name:   SCIB_Putchar
 *  Input:  none
 *  output: none
 *  Description:  ESCIB Send a char
 *  Note:
 * *********************************************************/
unsigned char SCIA_Getchar()
{
  while (ESCI_A.SR.B.RDRF == 0) {}    /* Wait for receive data reg full = 1 */
  ESCI_A.SR.R = 0x20000000;           /* Clear RDRF flag */ 

  return ESCI_A.DR.B.D;            /* Read byte of Data*/	
}
/************************************************************
 *  Name:   SCIA_Putstr
 *  Input:  none
 *  output: none
 *  Description:  ESCIA Send a string
 *  Note:
 * *********************************************************/
void SCIA_Putstr(char *str)
{
	while(*str!=0)
	{
		SCIA_Putchar(*str++);
	}
}
/************************************************************
 *  Name:   SCIB_Putstr
 *  Input:  none
 *  output: none
 *  Description:  ESCIB Send a string
 *  Note:
 * *********************************************************/
void SCIB_Putchar(char c)
{
    while (ESCI_B.SR.B.TDRE == 0) {}       /* Wait for transmit data reg empty = 1 */
    ESCI_B.SR.R = 0x80000000;              /* Clear TDRE flag */
	ESCI_B.DR.B.D = c;                     /* Transmit 8 bits Data */
}

unsigned char SCIB_Getchar()
{
  while (ESCI_B.SR.B.RDRF == 0) {}    /* Wait for receive data reg full = 1 */
  ESCI_B.SR.R = 0x20000000;           /* Clear RDRF flag */
  return ESCI_B.DR.B.D;               /* Read byte of Data*/	
}

void SCIB_Putstr(char *str)
{
	while(*str!=0)
	{
		SCIB_Putchar(*str++);
	}
}


/******************** end of file ************************/