#ifndef __LCD_H
#define __LCD_H
#include "gui.h"

class LCD
{
public :
		LCD();
		void setCursor(int x,int y);
		void print(char const*);
		void write(char);
private:
	int p_x,p_y;
};
#endif