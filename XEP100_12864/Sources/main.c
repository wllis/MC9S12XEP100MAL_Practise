/**********************(c) Copyright 20017-2018, CIDI **********************
	File�� mian.c
	Description��  ��ʼ�� I/O�ڣ����ڣ�CAN���ߣ�ϵͳʱ��
                   ִ��������
    Ӳ��ƽ̨ ����MC9S12XEP100�๦�ܿ����� 

    Author��
    Date: 04/17/2018
      
    Note:
  
  Version: 1.0

**************************************************************************/


/*---------------------------- Includes ---------------------------------*/

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "LCD_12864.h"
#include "usart.h"
#include "can.h"
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

/******************************** DEFINES ***********************************/

#define LED0     PORTB_PB0
#define LED1     PORTB_PB1
#define LED2     PORTB_PB2
#define LED3     PORTB_PB3
#define LED4     PORTB_PB4
#define LED5     PORTB_PB5
#define LED6     PORTB_PB6
#define LED7     PORTB_PB7

#define LED0_DIR DDRB_DDRB0
#define LED1_DIR DDRB_DDRB1
#define LED2_DIR DDRB_DDRB2
#define LED3_DIR DDRB_DDRB3
#define LED4_DIR DDRB_DDRB4
#define LED5_DIR DDRB_DDRB5
#define LED6_DIR DDRB_DDRB6
#define LED7_DIR DDRB_DDRB7

volatile byte C0rxdata[9][9]={""};           
volatile byte C4rxdata[9][9]={""};



//
#pragma CODE_SEG __NEAR_SEG NON_BANKED 

void interrupt VectorNumber_Vcan0rx  CAN0RxISR(void)
{
    byte length,index,rxdlr;
    
    length = (CAN0RXDLR & 0x0F);
    rxdlr=CAN0RXIDR0&0X0F;
    for (index=0; index<length; index++)
    C0rxdata[rxdlr][index] = *(&CAN0RXDSR0 + index); /* Get received data */
    printp("\nCAN0 RX DATA IS:%s",C0rxdata[rxdlr]); 
    printp("\nHello CAN0");   
    PORTB = 0x00;
    CAN0RFLG = 0x01;   /* Clear RXF */
}
void interrupt 54  CAN4RxISR(void)
{
    byte length,index,rxdlr; 
    
    length = (CAN4RXDLR & 0x0F);
    rxdlr=CAN4RXIDR0&0X0F;
    for (index=0; index<length; index++)
      C4rxdata[rxdlr][index] = *(&CAN4RXDSR0 + index); /* Get received data */
    printp("\nCAN4 RX DATA IS:%s",C4rxdata[rxdlr]);   
    printp("Hello CAN4");  
    PORTB = 0x00;
    CAN4RFLG = 0x01;   /* Clear RXF */
} 

//#pragma CODE_SEG DEFAULT
/*******************************************************************************
* Function Name  : delay
* Description    : �򵥵���ʱ���� 
* Input          : unsigned int ������
* Output         : None
* Return         : None
*******************************************************************************/

void delay( unsigned int Time ) 
{
    unsigned char i;
    while(Time--)
    {
      for( i=200; i>0; i-- );
    }
}
/*******************************************************************************
* Function Name  : AD_Init
* Description    : PLL��ʼ��  BUS Clock=16M���ⲿ����16M��
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
void AD_Init(void) 
{     
  ATD0CTL1=0x00;   //7:1-外部触发,65:00-8位精度,4:放电,3210:ch
  ATD0CTL2=0x40;   //禁止外部触发, 中断禁止    
  ATD0CTL3=0xa0;   //右对齐无符号,每次转换4个序列, No FIFO, Freeze模式下继续转    
  ATD0CTL4=0x01;   //765:采样时间为4个AD时钟周期,ATDClock=[BusClock*0.5]/[PRS+1]
  ATD0CTL5=0x30;   //6:0特殊通道禁止,5:1连续转换 ,4:1多通道轮流采样
  ATD0DIEN=0x00;   //禁止数字输入   
    
} 
/*******************************************************************************
* Function Name  : Set_PLL_Bus_clock
* Description    : PLL��ʼ��  BUS Clock=16M���ⲿ����16M��
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
void Set_PLL_Bus_clock(void)
{   
    CLKSEL=0X00;				// disengage PLL to system
    PLLCTL_PLLON=1;			// turn on PLL
    SYNR=0x00 | 0x01; 	// VCOFRQ[7:6];SYNDIV[5:0]
                        // fVCO= 2*fOSC*(SYNDIV + 1)/(REFDIV + 1)
                        // fPLL= fVCO/(2 �� POSTDIV) 
                        // fBUS= fPLL/2 
                        // VCOCLK Frequency Ranges  VCOFRQ[7:6]
                        // 32MHz <= fVCO <= 48MHz    00
                        // 48MHz <  fVCO <= 80MHz    01
                        // Reserved                  10
                        // 80MHz <  fVCO <= 120MHz   11				
    REFDV=0x80 | 0x01;  // REFFRQ[7:6];REFDIV[5:0]
                        // fREF=fOSC/(REFDIV + 1)
                        // REFCLK Frequency Ranges  REFFRQ[7:6]
                        // 1MHz <= fREF <=  2MHz       00
                        // 2MHz <  fREF <=  6MHz       01
                        // 6MHz <  fREF <= 12MHz       10
                        // fREF >  12MHz               11                         
                        // pllclock=2*osc*(1+SYNR)/(1+REFDV)=32MHz;
    POSTDIV=0x00;       // 4:0, fPLL= fVCO/(2xPOSTDIV)
                        // If POSTDIV = $00 then fPLL is identical to fVCO (divide by one).
    _asm(nop);          // BUS CLOCK=16M
    _asm(nop);
    while(!(CRGFLG_LOCK==1));	  //when pll is steady ,then use it;
    CLKSEL_PLLSEL =1;		        //engage PLL to system; 
}
/*******************************************************************************
* Function Name  : SCI_Init �ڲ�����
* Description    : ���ڳ�ʼ������
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
static void SCI_Init(void) 
{
    SCI0CR2=0x2c; //enable Receive Full Interrupt,RX enable,Tx enable
    SCI0BDH=0x00; //busclk  8MHz,19200bps,SCI0BDL=0x1a
    SCI0BDL=0x68; //SCI0BDL=busclk/(16*SCI0BDL)
                  //busclk  8MHz, 9600bps,SCI0BDL=0x34
                  //busclk  8MHz, 9600bps,SCI0BDL=0x68
                  //busclk 24MHz, 9600bps,SCI0BDL=0x9C
}                 //busclk 32MHz, 9600bps,SCI0BDL=0xD0
                  //busclk 40MHz, 9600bps,SCI0BD =0x104

static void Port_Init( void )
{
  RDRT=0xff;          // reduce the power of T port   
  RDRIV=0x93;         // reduce the power   of
  
  DDRA = 0xff;  //LCD1100,PA0--4,PA67 D1D2
  PORTA= 0x00; 

  // ʹ��PORTBΪ���
  DDRB=0xFF;          // set  port B bit0 as output
  PORTB=0X00;
  DDRT_DDRT4=1;       // set portT bit 4 as output
  DDRP_DDRP3=1;       // reduce the power of port P3
  RDRP_RDRP3=1;       // set portP bit 3 as output
  
}

/*******************************************************************************
* Function Name  : main
* Description    : ������
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
void main(void)
{  

  byte  u8_AD1,u8_AD2,u8_AD3,u8_AD4,u8_AD5;
  char  txtbuf[16]="",KBbuf[17]="",KBptr=0;
  byte  u8_zhengshu=0,u8_xiaoshu=0,u8_mintes=0,u8_key=0;
  word u16_sec=0,u16_ms=0;
  word u16_tmv_AD1,u16_tmv_AD2,u16_tmv_AD3,u16_tmv_AD4,u16_tmv_AD5,
       u16_zheng,u16_xiao;

  char can0_txbuf[8] = {0x12,0x34,0x56,0x78,0x9a,0x01,0x01,0x01};
  char can4_txbuf[8] = {0x44,0x44,0x44,0x44,0x9a,0x00,0x00,0x00};

    unsigned char i = 0;
    
    unsigned char cantx_err_flag;

    CAN0_ID = 0xAAAA;
    CAN4_ID = 0x4444;
    
    DisableInterrupts;
    
    // PLL��ʼ��
    Set_PLL_Bus_clock();

    // ��ʼ���˿�
    Port_Init();
    
    // ���ڳ�ʼ�� 9600Kps
    SCI_Init();

    // ��ʼ��LCDģ��
    LCD_Init();
    
    // CAN0 ��ʼ��
    InitCAN0();

    // CAN4 ��ʼ��
    InitCAN4();

    // AD 初始化
    AD_Init();
    
    
    delay(300);

    LCD_P6x8Str(22,0,"Hello world!");

    LCD_P6x8Str(22,1,"hello CIDI!");

    // �����ж�
	  EnableInterrupts;
	
    for(;;) 
    {   
        for( i=0; i<8; i++ )
        {
            PORTB = 0xFF;
            printp("Hello CIDI!\n");
            delay(2000);
            PORTB = ~(0x01<<i);
            delay(1000);

            //----------ADC转换部分---------------------------      
            while(!ATD0STAT0_SCF);
            u8_AD1=ATD0DR0L;
            u8_AD2=ATD0DR1L;
            u8_AD3=ATD0DR2L;
            u8_AD4=ATD0DR3L;
            u8_AD5=ATD0DR4L;
            
            u16_tmv_AD1=u8_AD1*100/51;
            u16_tmv_AD2=u8_AD2*100/51;
            u16_tmv_AD3=u8_AD3*100/51;
            u16_tmv_AD4=u8_AD4*100/51;
            u16_tmv_AD5=u8_AD5*100/51;
            
            u16_zheng=u16_tmv_AD1/100;
            u16_xiao=u16_tmv_AD1%100;
            sprintf(txtbuf,"AD0_Vol:%d.%02dV",u16_zheng,u16_xiao);
            LCD_P6x8Str(2,2,txtbuf);
            //printp("\n voltage: %d.%dV",u16_zheng,u16_xiao);
            
            u16_zheng=u16_tmv_AD2/100;
            u16_xiao=u16_tmv_AD2%100;
            sprintf(txtbuf,"AD1_Vol:%d.%02dV",u16_zheng,u16_xiao);
            LCD_P6x8Str(2,3,txtbuf);
            //printp("\n voltage: %d.%dV",u16_zheng,u16_xiao); 
            
            u16_zheng=u16_tmv_AD3/100;
            u16_xiao=u16_tmv_AD3%100;
            sprintf(txtbuf,"AD2_Vol:%d.%02dV",u16_zheng,u16_xiao);
            LCD_P6x8Str(2,4,txtbuf);
            
            u16_zheng=u16_tmv_AD4/100;
            u16_xiao=u16_tmv_AD4%100;
            sprintf(txtbuf,"AD3_Vol:%d.%02dV",u16_zheng,u16_xiao);
            LCD_P6x8Str(2,5,txtbuf);
            
            u16_zheng=u16_tmv_AD5/100;
            u16_xiao=u16_tmv_AD5%100;
            sprintf(txtbuf,"AD4_Vol:%d.%02dV",u16_zheng,u16_xiao);
            LCD_P6x8Str(2,6,txtbuf);
            //printp("\n voltage: %d.%dV",u16_zheng,u16_xiao);

            cantx_err_flag = CAN0SendFrame(CAN0_ID,0x00,strlen(can0_txbuf),can0_txbuf);
            cantx_err_flag = CAN4SendFrame(CAN4_ID,0x00,strlen(can4_txbuf),can4_txbuf);
        }
    } 
}
