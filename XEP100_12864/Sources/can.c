

/**********************(c) Copyright 20017-2018, CIDI **********************
	File： mian.c
	Description：  初始化 I/O口，串口，CAN总线，系统时钟
                   执行任务函数
    硬件平台 龙丘MC9S12XEP100多功能开发板 

    Author：
    Date: 04/17/2018
      
    Note:
  
  Version: 1.0

**************************************************************************/


#include "can.h"
#include "derivative.h"

#ifndef NO_ERR
  #define NO_ERR 0x00
  #define IS_ERR 0x01
#endif

unsigned long CAN0_ID;
unsigned long CAN4_ID; 


/*******************************************************************************
* Function Name  : ConvertID2MSCAN_ID
* Description    : 
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
unsigned long ConvertID2MSCAN_ID(unsigned long OID)
{
   unsigned long tmID=0;
   tmID=OID<<1;   
   tmID=((tmID&0x3ff80000)<<2)|(tmID&0x000fffff|0x00180000);   
   return tmID;   
}  
/*******************************************************************************
* Function Name  : InitCAN0
* Description    : CAN0初始化
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
void InitCAN0(void) 
{
  CAN0CTL0 = 0x01;            /* Enter Initialization Mode 
                               *  
                               *  0b00000001
                               *    ||||||||__ Enter Initialization Mode
                               *    |||||||___ Sleep Mode Request bit
                               *    ||||||____ Wake-Up disabled
                               *    |||||_____ Time stamping disabled
                               *    ||||______ Synchronized Status
                               *    |||_______ CAN not affected by Wait
                               *    ||________ Receiver Active Status bit
                               *    |_________ Received Frame Flag bit
                               */                                 
  while(CAN0CTL1_INITAK!=1); 	// Wait for Initialization Mode acknowledge INITRQ bit = 1  
  CAN0CTL1 = 0x80;            /* Enable MSCAN module and not LoopBack Mode
                               *  
                               *  0b10100000
                               *    ||||||||__ Initialization Mode Acknowledge
                               *    |||||||___ Sleep Mode Acknowledge
                               *    ||||||____ Wake-up low-pass filter disabled
                               *    |||||_____ Unimplemented
                               *    ||||______ Listen Only Mode disabled
                               *    |||_______ not Loop Back Mode enabled
                               *    ||________ Ext Osc/Xtal as Clock Source
                               *    |_________ MSCAN Module enabled
                               */     
  CAN0BTR0 = 0x43;            /* Synch Jump = 2 Tq clock Cycles 
                               *
                               *  0b01000011
                               *    ||||||||__
                               *    |||||||___\
                               *    ||||||____ |
                               *    |||||_____ |_ CAN Clock Prescaler = 4
                               *    ||||______ |
                               *    |||_______/
                               *    ||________
                               *    |_________>- SJW = 2
                               */     
  CAN0BTR1 = 0x14;	          /* Set Number of samples per bit, TSEG1 and TSEG2 
                               * bit rate=Fclk/pres v/(1+TSEG1+TSEG2)=16M/4/(1+2+5)=500kbps
                               *  0b00010100
                               *    ||||||||__
                               *    |||||||___\
                               *    ||||||____ |- TSEG1 = 5
                               *    |||||_____/
                               *    ||||______
                               *    |||_______\_ TSEG2 = 2
                               *    ||________/
                               *    |_________ One sample per bit
                               */                               
   CAN0IDAC = 0x00;           /* Set two 32-bit Filters 
                               *  
                               *  0b00000000
                               *    ||||||||__ 
                               *    |||||||___\_ Filter Hit Indicator 
                               *    ||||||____/
                               *    |||||_____ Unimplemented 
                               *    ||||______ 
                               *    |||_______>- two 32-bit Acceptance Filters   
                               *    ||________ 
                               *    |_________>- Unimplemented 
                               */                                
  /* note Acceptance Filters neither Acceptance Filter is accorded with,message in receivebuffer will pass  */   
  CAN0IDAR0 = ACC_CAN0_EX_ID_R0;   // |\    32 bit Filter 0
  CAN0IDAR1 = ACC_CAN0_EX_ID_R1;   // | \__ Accepts Extended Data Frame Msg 
  CAN0IDAR2 = ACC_CAN0_EX_ID_R2;   // | /   with ID: CAN0_ID 
  CAN0IDAR3 = ACC_CAN0_EX_ID_R3;   // |/
  CAN0IDAR4 = ACC_CAN0_EX_ID_R0;   // |\    32 bit Filter 0
  CAN0IDAR5 = ACC_CAN0_EX_ID_R1;   // | \__ Accepts Extended Data Frame Msg 
  CAN0IDAR6 = ACC_CAN0_EX_ID_R2;   // | /   with ID: CAN0_ID 
  CAN0IDAR7 = ACC_CAN0_EX_ID_R3;   // |/  
  CAN0IDMR0 = MASK_CAN0_EX_ID_R0;
  CAN0IDMR1 = MASK_CAN0_EX_ID_R1;
  CAN0IDMR2 = MASK_CAN0_EX_ID_R2;
  CAN0IDMR3 = MASK_CAN0_EX_ID_R3;
  CAN0IDMR4 = MASK_CAN0_EX_ID_R0;
  CAN0IDMR5 = MASK_CAN0_EX_ID_R1;
  CAN0IDMR6 = MASK_CAN0_EX_ID_R2;
  CAN0IDMR7 = MASK_CAN0_EX_ID_R3;
  
  CAN0CTL0 = 0x00;                /* Exit Initialization Mode Request */
  while ((CAN0CTL1&0x01)!=0);	    // �ȴ���ֱ��MSCAN��������
  while(!(CAN0CTL0&0x10));        // �ȴ���ֱ��MSCAN��CAN����ͬ��
  CAN0RFLG = 0xC3;          /* Reset Receiver Flags
                             *  
                             *  0b11000011
                             *    ||||||||__ Receive Buffer Full Flag
                             *    |||||||___ Overrun Interrupt Flag
                             *    ||||||____ 
                             *    |||||_____>- Transmitter Status Bits
                             *    ||||______ 
                             *    |||_______>- Receiver Status Bits
                             *    ||________ CAN Status Change Interrupt Flag
                             *    |_________ Wake-Up Interrupt Flag
                             */   
  CAN0RIER = 0x01;          /* Enable Receive Buffer Full Interrupt
                             *  
                             *  0b00000001
                             *    ||||||||__ Receive Buffer Full Int enabled
                             *    |||||||___ Overrun Int disabled
                             *    ||||||____ 
                             *    |||||_____>- Tx Status Change disabled
                             *    ||||______ 
                             *    |||_______>- Rx Status Change disabled
                             *    ||________ Status Change Int disabled
                             *    |_________ Wake-Up Int disabled
                             */ 
}
/*******************************************************************************
* Function Name  : InitCAN0
* Description    : CAN0SendFrame
* Input          : id:要发送的CAN0 ID号  priority:优先级 length:要发送的数据长度
                    *txdata:要发送的数据指针
* Output         : None
* Return         : 发送的状态 成功or失败
*******************************************************************************/
int CAN0SendFrame(unsigned long id, unsigned char priority, unsigned char length, unsigned char *txdata )
{    
    unsigned char index;                          // number for read message
    unsigned char tbsel = 0;                      // symbol for CAN0TBSEL     
    int cnt=0;  
    
    if (!CAN0TFLG) return IS_ERR;                 /* Is Transmit Buffer full?? */            
    CAN0TBSEL = CAN0TFLG;                         /* Select lowest empty buffer */
    tbsel = CAN0TBSEL;		                        /* Backup selected buffer */       
    /* Load Id to IDR Registers */
    *((unsigned long *) ((unsigned long)(&CAN0TXIDR0)))= id;
    if(length>8) length=8;    
    for (index=0;index<length;index++) 
    {
        *(&CAN0TXDSR0 + index) = txdata[index];   //  Load data to Tx buffer  Data Segment Registers(ONLY 8 BYTES?)                                                   
    }
    CAN0TXDLR  = length;                          /* Set Data Length Code */
    CAN0TXTBPR = priority;                        /* Set Priority */
    CAN0TFLG   = tbsel;	                          /* Start transmission */
    //printp("\nCAN4TXIDR0~CAN4TXIDR3:%04x,%04x,%04x,%04x",CAN4TXIDR0,CAN4TXIDR1,CAN4TXIDR2,CAN4TXIDR3) ;
    while ((CAN0TFLG & tbsel) != tbsel)
        if(++cnt>10000) return IS_ERR;           //  Wait for Transmission completion
    //putstr("\nCAN0 Sending is successful!");
    return NO_ERR;
}
/*******************************************************************************
* Function Name  : InitCAN4
* Description    : CAN4初始化
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
void InitCAN4(void) 
{
    CAN4CTL0 = 0x01;                                       
    while(CAN4CTL1_INITAK!=1); 	     // Wait for Initialization Mode acknowledge INITRQ bit = 1  
    CAN4CTL1 = 0x80;             
    CAN4BTR0 = 0x43;             
    CAN4BTR1 = 0x14;	                                      
    CAN4IDAC = 0x00;                 // two 32-bit Acceptance Filters                        
    /* note Acceptance Filters neither Acceptance Filter is accorded with,message in receivebuffer will pass  */   
    CAN4IDAR0 = ACC_CAN4_EX_ID_R0;   // |\    32 bit Filter 0
    CAN4IDAR1 = ACC_CAN4_EX_ID_R1;   // | \__ Accepts Extended Data Frame Msg 
    CAN4IDAR2 = ACC_CAN4_EX_ID_R2;   // | /   with ID: CAN0_ID 
    CAN4IDAR3 = ACC_CAN4_EX_ID_R3;   // |/
    CAN4IDAR4 = ACC_CAN4_EX_ID_R0;   // |\    32 bit Filter 0
    CAN4IDAR5 = ACC_CAN4_EX_ID_R1;   // | \__ Accepts Extended Data Frame Msg 
    CAN4IDAR6 = ACC_CAN4_EX_ID_R2;   // | /   with ID: CAN0_ID 
    CAN4IDAR7 = ACC_CAN4_EX_ID_R3;   // |/  
    CAN4IDMR0 = MASK_CAN4_EX_ID_R0;
    CAN4IDMR1 = MASK_CAN4_EX_ID_R1;
    CAN4IDMR2 = MASK_CAN4_EX_ID_R2;
    CAN4IDMR3 = MASK_CAN4_EX_ID_R3;
    CAN4IDMR4 = MASK_CAN4_EX_ID_R0;
    CAN4IDMR5 = MASK_CAN4_EX_ID_R1;
    CAN4IDMR6 = MASK_CAN4_EX_ID_R2;
    CAN4IDMR7 = MASK_CAN4_EX_ID_R3;
        
    CAN4CTL0 = 0x00;                /* Exit Initialization Mode Request */
    while ((CAN4CTL1&0x01)!=0);	    // 等待，直到MSCAN正常运行
    while(!(CAN4CTL0&0x10));        // 等待，直到MSCAN与CAN总线同步
    CAN4RFLG = 0xC3;          
    CAN4RIER = 0x01;          
}
/*******************************************************************************
* Function Name  : InitCAN4
* Description    : CAN0SendFrame
* Input          : id:要发送的CAN4 ID号  priority:优先级 length:要发送的数据长度
                    *txdata:要发送的数据指针
* Output         : None
* Return         : 发送的状态 成功or失败
*******************************************************************************/
int CAN4SendFrame(unsigned long id, unsigned char priority, unsigned char length, unsigned char *txdata )
{    
    unsigned char index;                          // number for read message
    unsigned char tbsel = 0;                      // symbol for CAN4TBSEL 
    int cnt=0;    
    
    if (!CAN4TFLG) return IS_ERR;                 /* Is Transmit Buffer full?? */            
    CAN4TBSEL = CAN4TFLG;                         /* Select lowest empty buffer */
    tbsel = CAN4TBSEL;		                        /* Backup selected buffer */       
    /* Load Id to IDR Registers */
    *((unsigned long *) ((unsigned long)(&CAN4TXIDR0)))= id;//接收方
    if(length>8) length=8;    
    for (index=0;index<length;index++) 
    {
        *(&CAN4TXDSR0 + index) = txdata[index];   //  Load data to Tx buffer  Data Segment Registers(ONLY 8 BYTES?)                                                   
    }
    CAN4TXDLR  = length;                          /* Set Data Length Code */
    CAN4TXTBPR = priority;                        /* Set Priority */
    CAN4TFLG   = tbsel;	                          /* Start transmission */
    //printp("\nCAN4TXIDR0~CAN4TXIDR3:%04x,%04x,%04x,%04x",CAN4TXIDR0,CAN4TXIDR1,CAN4TXIDR2,CAN4TXIDR3) ;
    while ((CAN4TFLG & tbsel) != tbsel)
        if(++cnt>10000) return IS_ERR;          //  Wait for Transmission completion
    //putstr("\nCAN4 Sending is successful!");
    return NO_ERR;
}  


/********************************* end of file ***********************************/
  