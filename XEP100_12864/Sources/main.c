#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "LQ12864.h"


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



void delay( unsigned int Time ) 
{
    unsigned char i;
    while(Time--)
    {
      for( i=200; i>0; i-- );
    }
}

static void Port_Init( void )
{
  DDRA = 0xff;  //LCD1100,PA0--4,PA67 D1D2
  PORTA= 0x00; 

  // 使能PORTB为输出
  DDRB = 0xFF; 
}


void main(void)
{
  byte  u8_volt,u8_cnt=0,u8_frush=168,u8_beepflag=0;   
  char  txtbuf[16]="",KBbuf[17]="",KBptr=0;
  byte  u8_zhengshu=0,u8_xiaoshu=0,u8_mintes=0,u8_key=0;
  word u16_sec=0,u16_ms=0;
  word u16_ioc_cnt=0,u16_tmv,u16_zheng,u16_xiao,tmT;  
  /* put your own code here */
    
    unsigned char i = 0;

    // 初始化端口
    Port_Init();

    // 初始化LCD模块
    LCD_Init();

    //Draw_LQLogo();

    delay(300);

    LCD_P6x8Str(2,6,"Hello world!");

    LCD_P6x8Str(22,7,"hello CIDI!");

	  EnableInterrupts;
	


  for(;;) 
  {
    
    for( i=0; i<8; i++ )
    {
      PORTB = 0xFF;
      delay(600);
      PORTB = ~(0x01<<i);
      delay(400);
    }
  } /* loop forever */
  /* please make sure that you never leave main */
}
