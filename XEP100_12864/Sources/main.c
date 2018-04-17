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


/*---------------------------- Includes ---------------------------------*/

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "LCD_12864.h"
#include "usart.h"


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


/*******************************************************************************
* Function Name  : delay
* Description    : 简单的延时函数 
* Input          : unsigned int 型数据
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
* Function Name  : Set_PLL_Bus_clock
* Description    : PLL初始化  BUS Clock=16M（外部晶振16M）
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
                        // fPLL= fVCO/(2 × POSTDIV) 
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
* Function Name  : SCI_Init 内部函数
* Description    : 串口初始化函数
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
  DDRA = 0xff;  //LCD1100,PA0--4,PA67 D1D2
  PORTA= 0x00; 

  // 使能PORTB为输出
  DDRB = 0xFF; 
}

/*******************************************************************************
* Function Name  : main
* Description    : 主函数
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
void main(void)
{
  //byte  u8_volt,u8_cnt=0,u8_frush=168,u8_beepflag=0;   
  char  txtbuf[16]="",KBbuf[17]="",KBptr=0;
  byte  u8_zhengshu=0,u8_xiaoshu=0,u8_mintes=0,u8_key=0;
  word u16_sec=0,u16_ms=0;
  //word u16_ioc_cnt=0,u16_tmv,u16_zheng,u16_xiao,tmT;  
  /* put your own code here */
    
    unsigned char i = 0;
    
    // PLL初始化
    Set_PLL_Bus_clock();

    // 初始化端口
    Port_Init();
    
    // 串口初始化 9600Kps
    SCI_Init();

    // 初始化LCD模块
    LCD_Init();
    
    delay(300);

    LCD_P6x8Str(22,5,"Hello world!");

    LCD_P6x8Str(22,2,"hello CIDI!");

    // 开启中断
	  EnableInterrupts;
	


  for(;;) 
  {
    
    for( i=0; i<8; i++ )
    {
      PORTB = 0xFF;
      putstr("Hello world!\n");
      delay(600);
      PORTB = ~(0x01<<i);
      delay(400);
    }
  } /* loop forever */
  /* please make sure that you never leave main */
}
