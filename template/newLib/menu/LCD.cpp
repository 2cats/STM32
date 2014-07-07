#include "menu/LCD.h"
#include "ili93xx.h"
LCD::LCD()
{
	//delay_init();
	GUI_Init();
}
void LCD::setCursor(int x,int y)
{
	p_x=x;
	p_y=y;
	GUI_GotoXY(x,y);
}
void LCD::print(char const*str)
{
	GUI_DispString(str);
	//LCD_ShowString(p_x,p_y,u16 width,u16 height,u8 size,u8 *p);
}
void LCD::write(char c)
{
	GUI_DispChar(c);
	//LCD_ShowChar(p_x,p_y,c,12,0);
}