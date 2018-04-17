/********************************************
龙丘MC9S12XEP100多功能开发板 
Designed by Chiu Sir
E-mail:chiusir@163.com
软件版本:V1.1
最后更新:2009年4月12日
相关信息参考下列地址：
博客：  http://longqiu.21ic.org
淘宝店：http://shop36265907.taobao.com
------------------------------------
Code Warrior 5.0
Target : MC9S12XEP100
Crystal: 16.000Mhz
busclock:16.000MHz
pllclock:32.000MHz
============================================*/
#include <ctype.h>
#include <string.h>
#include <stdarg.h>

#ifndef  TRUE
#define  TRUE 1
#endif
#ifndef  FALSE
#define  FALSE 0
#endif

#define Baud_9600  55
#define Baud_19200 26
#define SCI0CR2     (*((volatile unsigned char*)(0x00CB)))
#define SCI0SR1     (*((volatile unsigned char*)(0x00CC)))
#define SCI0BDH     (*((volatile unsigned char*)(0x00C8)))
#define SCI0BDL     (*((volatile unsigned char*)(0x00C9)))
#define SCI0DRL     (*((volatile unsigned char*)(0x00CF)))

#define CR_as_CRLF  TRUE             // if true , you can use "\n" to act as CR/LF, 
                                     // if false, you have to use "\n\r",but can get a higher speed  
static int do_padding;
static int left_flag;
static int len;
static int num1;
static int num2;
static char pad_character;

unsigned char uart_getkey(void)
{  
   while(!(SCI0SR1&0x80)) ; 		 //keep waiting when not empty  
   return SCI0DRL;
}
/*
void uart_init(void) {
  SCI0CR2=0x0c;
  SCI0BDH=0x00;//16MHz,19200bps,SCI0BDL=0x1a
  SCI0BDL=0x34;//16MHz,9600bps,SCI0BDL=0x34 
}
*/ 
void uart_putchar(unsigned char ch)
{ 
  if (ch == '\n')  
  {
      while(!(SCI0SR1&0x80)) ;     
      SCI0DRL= 0x0d;       				 //output'CR'
	    return;
  }
  while(!(SCI0SR1&0x80)) ; 		    //keep waiting when not empty  
  SCI0DRL=ch;
}

void putstr(char ch[])
{
  unsigned char ptr=0;
  while(ch[ptr]){
      uart_putchar((unsigned char)ch[ptr++]);
  }     
  
}
                                                 
static void padding( const int l_flag)
{
   int i;

   if (do_padding && l_flag && (len < num1))
      for (i=len; i<num1; i++)
          uart_putchar( pad_character);
}

static void outs( char* lp)
{
  /* pad on left if needed                          */
  len = strlen( lp);
  padding( !left_flag);

  /* Move string to the buffer                      */
  while (*lp && num2--)  uart_putchar( *lp++);

  /* Pad on right if needed                         */
  len = strlen( lp);
  padding( left_flag);
}

static void reoutnum(unsigned long num, unsigned int negative, const long base ) 
{
  char* cp;
  char outbuf[32];
  const char digits[] = "0123456789ABCDEF";
 
  /* Build number (backwards) in outbuf             */
  cp = outbuf;
  do {
    *cp++ = digits[(int)(num % base)];
    } while ((num /= base) > 0);
  if (negative)  *cp++ = '-';
  *cp-- = 0;

  /* Move the converted number to the buffer and    */
  /* add in the padding where needed.               */
  len = strlen(outbuf);
  padding( !left_flag);
  while (cp >= outbuf)
    uart_putchar( *cp--);
  padding( left_flag);
}

static void outnum(long num, const long base ,unsigned char sign)//1, signed  0 unsigned
{   
  unsigned int negative;

  if ( (num < 0L) && sign ) 
  {  
    negative=1;
    num = -num;
  }
  else negative=0;
 
  reoutnum(num,negative,base);  
} 

static int getnum( char** linep)
{
   int n;
   char* cp;

   n = 0;
   cp = *linep;
   while (isdigit(*cp))
      n = n*10 + ((*cp++) - '0');
   *linep = cp;
   return(n);
}

void printp( char* ctrl, ...)
{ 
   int long_flag;
   int dot_flag;

   char ch;
   va_list argp; 
   va_start( argp, ctrl); 
   for ( ; *ctrl; ctrl++) {  
      /* move format string chars to buffer until a  format control is found. */
      if (*ctrl != '%') {
         uart_putchar(*ctrl);
#if CR_as_CRLF==TRUE         
         if(*ctrl=='\n') uart_putchar('\r');
#endif         
         continue;
         } 
      /* initialize all the flags for this format.   */
      dot_flag = long_flag = left_flag = do_padding = 0;
      pad_character = ' ';
      num2=32767;  
try_next:
      ch = *(++ctrl);
      if (isdigit(ch)){
         if (dot_flag)
            num2 = getnum(&ctrl);
         else {
            if (ch == '0')
               pad_character = '0'; 
            num1 = getnum(&ctrl);
            do_padding = 1;
         }
         ctrl--;
         goto try_next;
      }       
      switch (tolower(ch)) {
         case '%':
              uart_putchar( '%');
              continue;  
         case '-':
              left_flag = 1;
              break;  
         case '.':
              dot_flag = 1;
              break;  
         case 'l':
              long_flag = 1;
              break;  
         case 'd':
              if (long_flag ==1 ) 
              {
              		if(ch == 'D')                {outnum( va_arg(argp, unsigned long), 10L , 0);continue;}
              	        else  /* ch == 'd' */        {outnum( va_arg(argp, long), 10L,1);continue;}
              }
              else 
              {
              		if(ch == 'D') {outnum( va_arg(argp, unsigned int),10L,0);continue;}
              		else  /* ch == 'd' */        
              		{
              		  outnum( va_arg(argp, int), 10L,1);
              		  continue;
              		}
              }                 
         case 'x':    // X 无符号 ， x  有符号
              if (long_flag ==1 )
              {
              	if(ch == 'X')                
              	{
              	  outnum( va_arg(argp, unsigned long), 16L,0);
              	  continue;
              	}
              	else  /* ch == 'x' */        
              	{
              	  outnum( va_arg(argp, long), 16L,1);
              	  continue;
              	} 
              }
              else
              {
              	if(ch == 'X')                
              		{
              		  outnum( va_arg(argp, unsigned int), 16L,0);
              		  continue;
              		}
              		else  /* ch == 'x' */        
              		{
              		  outnum( va_arg(argp, int), 16L,1);
              		  continue;
              		}
              } //如果按照16进制打印，将全部按照无符号数进行
              continue; 
         case 's':
              outs( va_arg( argp, char*));
              continue;   
         case 'c':
              uart_putchar( va_arg( argp, int));
              continue;  
         default:
              continue;
         }
      goto try_next;
      }
   va_end( argp);
   }

