/*
 * GObject.h
 *
 *  Created on: 2014年6月26日
 *      Author: Administrator
 */

#ifndef GOBJECT_H_
#define GOBJECT_H_
#include "GUI.h"
#include "Message.h"
/************Config******************/

#define MENUWIDGET_BK_COLOR									GUI_LIGHTBLUE
#define MENUWIDGET_BORDER_COLOR							GUI_COLOR_NONE

#define MENU_TEXT_BORDER_COLOR							GUI_BLUE
#define MENU_TEXT_COLOR										GUI_BLACK
#define MENU_TEXT_BK_NORMAL_COLOR						GUI_WHITE
#define MENU_TEXT_BK_FILL_COLOR							GUI_GREEN
#define MENU_TEXT_NORMAL_MODE							TEXTVIEW_ONLY_TEXT
#define MENU_TEXT_HIGHLIGHT_MODE						TEXTVIEW_WRAPFILL_TEXT

#define MENU_TEXT_FONT_SIZE_BASE						16
#define MENU_TEXT_FONT_SIZE								(MENU_TEXT_FONT_SIZE_BASE+8)//默认给字符加了边缘
#define MENU_TEXT_FONT								 	GUI_Font16_ASCII


#define STATUS_TEXT_FONT_SIZE_BASE						8
#define STATUSTEXT_FONT_SIZE							(STATUS_TEXT_FONT_SIZE_BASE+4)//默认给字符加了边缘
#define STATUS_TEXT_FONT								GUI_Font8_ASCII
#define STATUS_TEXT_COLOR								GUI_RED
#define STATUS_TEXT_BORDER_COLOR						GUI_BLUE


/************DON'T CHANGE*******************/
#define GUI_COLOR_NONE			0x0fffffff

#define TEXTVIEW_ONLY_TEXT		1
#define TEXTVIEW_FILL_TEXT		2
#define TEXTVIEW_WRAP_TEXT		3
#define TEXTVIEW_WRAPFILL_TEXT	4

#define PAINT_DEFAULT									0
#define PAINT_IGNORE									1

#define BLOOD_IGNORE 									0
#define BLOOD_CARE 										1

#define MENU_SHOW_NORMAL								0
#define MENU_SHOW_HIGHLIGHT								1
#define MENU_SHOW_NULL									2

#define KEY_NULL	10
#define KEY_UP		11
#define KEY_DOWN	12
#define KEY_LEFT	13
#define KEY_RIGHT	14
#define KEY_OK		15
#define KEY_ESC		16


#define KEY_0		0
#define KEY_1		1
#define KEY_2		2
#define KEY_3		3
#define KEY_4		4
#define KEY_5		5
#define KEY_6		6
#define KEY_7		7
#define KEY_8		8
#define KEY_9		9



class GObject {
public:
	GObject(int _x0,int _y0);
	virtual ~GObject();
	virtual void paint(int arg=PAINT_DEFAULT)=0;
	virtual void messageReceiever(Message *msg){}
	void printString(const char *s, int x, int y,int color);
	void nexTextLine();
	 void static sendMessage(GObject* obj,Message*msg);
	 void static sendMessage(GObject* obj,unsigned char type);
	int x0,y0;
	bool needPaint;
};

#endif /* GOBJECT_H_ */
