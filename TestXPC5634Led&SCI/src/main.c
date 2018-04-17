/**********************(c) Copyright 20017-2018, CIDI **********************
	File: main.c
	Description: 
    Author:
    Date: Mar/28/2018
      
    Note:

**************************************************************************/


#include "config.h"

/************************************************************
 *  Name:   Pll_Init_64M
 *  Input:  none
 *  output: none
 *  Description:  main clock initial to 64M
 *  Note:
 * *********************************************************/
void Pll_Init_64M(void)
{  
    FMPLL.ESYNCR1.B.CLKCFG = 0X7;       /* Change clk to PLL normal mode from crystal */  
    FMPLL.SYNCR.R = 0x16080000;         /* Initial values: PREDIV=1, MFD=12, RFD=1 */
    while (FMPLL.SYNSR.B.LOCK != 1) {}; /* Wait for FMPLL to LOCK  */
    FMPLL.SYNCR.R = 0x16000000;         /* Final value for 64 MHz: RFD=0 */ 	
}
/************************************************************
 *  Name:   Pll_Init_80M
 *  Input:  none
 *  output: none
 *  Description:  main clock initial to 80M
 *  Note:
 * *********************************************************/
void Pll_Init_80M(void)
{  
  FMPLL.ESYNCR1.B.CLKCFG = 0X7;       /* Change clk to PLL normal mode from crystal */  
  FMPLL.SYNCR.R = 0x16080000;         /* Initial values: PREDIV=1, MFD=12, RFD=1 */
  while (FMPLL.SYNSR.B.LOCK != 1) {}; /* Wait for FMPLL to LOCK  */
  //FMPLL.SYNCR.R = 0x16000000;         /* Final value for 80 MHz: RFD=0 */ 
  FMPLL.SYNCR.R = 0x03000430; /* Final setting: 80 MHz for 8 MHz crystal */	
}
/************************************************************
 *  Name:   delay
 *  Input:  int 
 *  output: none
 *  Description:   
 *  Note:
 * *********************************************************/
void delay(int ms)
{   
    int ii,jj;
    if (ms<1) ms=1;
    for(ii=0;ii<ms;ii++)
        for(jj=0;jj<5500;jj++){;}    
}


void main (void) 
{	
    vuint32_t i = 0;	 /* Dummy idle counter */   

    // main clock initionlization
    Pll_Init_80M();

    // ESCIA Init
    InitESCIA();

    // ESCIB Init
    InitESCIB();

    SIU.PCR[114].R = 0x0303;//ETPUA0,OUTPUT
    SIU.PCR[115].R = 0x0303;//1  
    SIU.PCR[116].R = 0x0303;//2
    SIU.PCR[117].R = 0x0303;//3 

    SIU.PCR[188].R = 0x0303;//EMIOS9,OUTPUT 
    SIU.PCR[189].R = 0x0303;//10
    SIU.PCR[190].R = 0x0303;//11 
    SIU.PCR[191].R = 0x0303;//EMIOS12

    while (1) 
    { 
        for(i=188;i<192;i++)
        { 
            SIU.GPDO[i].R=0;   
            delay(200);
            SIU.GPDO[i].R=1;
            delay(200); 
        }
        SCIA_Putstr("Hello SCIA\r\n");
        for(i=114;i<118;i++)
        { 
            SIU.GPDO[i].R=0;   
            delay(200);
            SIU.GPDO[i].R=1;
            delay(200); 
        }
        SCIB_Putstr("Hello SCIB\r\n");
    }	
}


/****************************** end of file ***********************************/
