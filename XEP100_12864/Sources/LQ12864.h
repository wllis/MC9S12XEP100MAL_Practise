#include "derivative.h"

extern void LCD_Init(void);
extern void LCD_CLS(void);
extern void LCD_P6x8Str(byte x,byte y,byte ch[]);
extern void LCD_P8x16Str(byte x,byte y,byte ch[]);
extern void LCD_P14x16Str(byte x,byte y,byte ch[]);
extern void LCD_Print(byte x, byte y, byte ch[]);
extern void LCD_PutPixel(byte x,byte y);
extern void LCD_Rectangle(byte x1,byte y1,byte x2,byte y2,byte gif);
extern void Draw_LQLogo(void);
extern void Draw_LibLogo(void);
extern void Draw_BMP(byte x0,byte y0,byte x1,byte y1,byte bmp[]); 

