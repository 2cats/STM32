/*
 * StatusBar.cpp
 *
 *  Created on: 2014Äê6ÔÂ28ÈÕ
 *      Author: Administrator
 */

#include "StatusBar.h"
StatusBar::StatusBar(int _x0, int _y0, int _x1, std::string str) :
		TextView(_x0, _y0, _x1, _y0+STATUSTEXT_FONT_SIZE,str){
}
void StatusBar::paint()
{
	GUI_SetFont (&STATUS_TEXT_FONT);
	Rectangle::fill(MENU_TEXT_BK_NORMAL_COLOR);
	GUI_SetTextMode (GUI_TEXTMODE_TRANS);
	printString(text.data(),getRx0()+2, getRy0(),STATUS_TEXT_COLOR);
	Rectangle::paint(STATUS_TEXT_BORDER_COLOR);
	needPaint=false;
}
StatusBar::~StatusBar() {
	// TODO Auto-generated destructor stub
}

