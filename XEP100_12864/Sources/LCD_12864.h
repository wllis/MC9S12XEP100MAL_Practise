/**********************(c) Copyright 20017-2018, CIDI **********************
	File： LCD_12864.h
	Description：  

    Author：wllis
    Date: 04/17/2018
      
    Note:
  
  Version: 1.0

**************************************************************************/

#ifndef __LCD_12864_H
#define __LCD_12864_H

#include "derivative.h"

extern void LCD_Init(void);
extern void LCD_CLS(void);
extern void LCD_P6x8Str(byte x,byte y,byte ch[]);
extern void LCD_P8x16Str(byte x,byte y,byte ch[]);
extern void LCD_P14x16Str(byte x,byte y,byte ch[]);
extern void LCD_Print(byte x, byte y, byte ch[]);
extern void LCD_PutPixel(byte x,byte y);
extern void LCD_Rectangle(byte x1,byte y1,byte x2,byte y2,byte gif);
extern void Draw_LibLogo(void);
extern void Draw_BMP(byte x0,byte y0,byte x1,byte y1,byte bmp[]); 

#endif

/****************************** end of file *****************************/

