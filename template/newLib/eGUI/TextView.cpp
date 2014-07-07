/*
 * TextView.cpp
 *
 *  Created on: 2014Äê6ÔÂ26ÈÕ
 *      Author: Administrator
 */

#include "TextView.h"
#include "gui.h"
TextView::TextView(int _x0, int _y0, int _x1, int _y1, std::string str) :
		Rectangle(_x0, _y0, _x1, _y1), text(str) {

}
void TextView::paint(int arg) {
	GUI_SetFont (&MENU_TEXT_FONT);
	switch (arg) {
	case TEXTVIEW_ONLY_TEXT:
		Rectangle::fill(MENU_TEXT_BK_NORMAL_COLOR);
		GUI_SetTextMode (GUI_TEXTMODE_TRANS);
		printString(text.data(),getRx0()+4, getRy0()+2,MENU_TEXT_COLOR);
		Rectangle::paint(MENU_TEXT_BORDER_COLOR);
		break;
	case TEXTVIEW_FILL_TEXT:
		Rectangle::fill(MENU_TEXT_BK_FILL_COLOR);
		GUI_SetTextMode (GUI_TEXTMODE_TRANS);
		printString(text.data(),getRx0()+4, getRy0()+1,MENU_TEXT_COLOR);
		break;
	case TEXTVIEW_WRAP_TEXT:
		Rectangle::fill(MENU_TEXT_BK_NORMAL_COLOR);
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		printString(text.data(),getRx0()+4, getRy0()+1,MENU_TEXT_COLOR);
		Rectangle::paint();
		break;
	case TEXTVIEW_WRAPFILL_TEXT:
		Rectangle::fill(MENU_TEXT_BK_FILL_COLOR);
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		printString(text.data(),getRx0()+4, getRy0()+1,MENU_TEXT_COLOR);
		Rectangle::paint();
		break;
	}
	needPaint=false;

}

