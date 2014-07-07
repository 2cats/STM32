/*
 * GObject.cpp
 *
 *  Created on: 2014Äê6ÔÂ26ÈÕ
 *      Author: Administrator
 */

#include "GObject.h"

GObject::GObject(int _x0,int _y0):x0(_x0),y0(_y0)
{
	needPaint=true;
}
GObject::~GObject() {

}

void GObject::sendMessage(GObject* obj, Message*msg) {
	obj->messageReceiever(msg);
}
void GObject::sendMessage(GObject* obj,unsigned char type)
{
	Message msg;
	msg.type=type;
	GObject::sendMessage(obj,&msg);
}
void GObject::nexTextLine()
{
	GUI_GotoXY(x0,y0+MENU_TEXT_FONT_SIZE);
}
void GObject::printString(const char *s, int x, int y,int color)
{
	if(color!=GUI_COLOR_NONE)
	{
		GUI_SetColor(color);
		GUI_DispStringAt(s,x,y);
	}

}
