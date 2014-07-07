/*
 * CheckBoxMenu.cpp
 *
 *  Created on: 2014Äê6ÔÂ27ÈÕ
 *      Author: Administrator
 */

#include "CheckBoxMenu.h"

CheckBoxMenu::CheckBoxMenu(int _height,std::string str,void (*_doWhenChange)(CheckBoxMenu*)):InteractiveMenu(_height,str),doWhenChange(_doWhenChange)
{
	status=false;
	CheckBox_A_margin=height/5;
	CheckBox_A_padding=height/7;
}
void CheckBoxMenu::updatePosi(int _x0,int _y0,int _x1,int _y1)
{
	x0=_x0;
	y0=_y0;
	x1=_x1;
	y1=_y1;
}
void CheckBoxMenu::showHighlight()
{
	Rectangle::fill(MENU_TEXT_BK_FILL_COLOR);
}
void CheckBoxMenu::showNormal()
{
	Rectangle::fill(MENU_TEXT_BK_NORMAL_COLOR);
}
void CheckBoxMenu::paint(int arg)
{
	switch(arg)
	{
		case MENU_SHOW_NULL:
			break;
		case MENU_SHOW_NORMAL:
			showNormal();
			break;
		case MENU_SHOW_HIGHLIGHT:
			showHighlight();
			break;
	}

	if(status)
	{
		drawOuter();
		drawInner();
	}else
	{
		drawOuter();
		clearInner();
	}
	
	printString(text.data(),x0+4+getAL()+CheckBox_A_margin,y0+4,MENU_TEXT_COLOR);
	Rectangle::paint(MENU_TEXT_BORDER_COLOR);
	needPaint=false;
}
void CheckBoxMenu::onMessage(Message *msg)
{
	needPaint=true;
	switch(msg->type)
	{
	case KEY_OK:
		status=!status;
		msg->type=KEY_NULL;
		doWhenChange(this);
		break;
	}
}
int CheckBoxMenu::getAL()
{
	return (y1-y0-CheckBox_A_margin*2);
}
int CheckBoxMenu::getAS()
{
	return (getAL()-CheckBox_A_padding*2);
}

void CheckBoxMenu::drawOuter()
{
	GUI_SetColor(CHECKBOX_OUTER_COLOR);
	GUI_DrawRect(x0+CheckBox_A_margin,
			y0+CheckBox_A_margin,
			x0+CheckBox_A_margin+getAL(),
			y0+CheckBox_A_margin+getAL());
}
void CheckBoxMenu::drawInner()
{
	GUI_SetColor(CHECKBOX_INNER_COLOR);
	GUI_FillRect(x0+CheckBox_A_margin+CheckBox_A_padding,
			y0+CheckBox_A_margin+CheckBox_A_padding,
			x0+CheckBox_A_margin+CheckBox_A_padding+getAS(),
			y0+CheckBox_A_margin+CheckBox_A_padding+getAS());
}
void CheckBoxMenu::clearInner()
{
	GUI_SetColor(MENU_TEXT_BK_NORMAL_COLOR);
	GUI_FillRect(x0+CheckBox_A_margin+CheckBox_A_padding,
			y0+CheckBox_A_margin+CheckBox_A_padding,
			x0+CheckBox_A_margin+CheckBox_A_padding+getAS(),
			y0+CheckBox_A_margin+CheckBox_A_padding+getAS());
}
CheckBoxMenu::~CheckBoxMenu() {
	// TODO Auto-generated destructor stub
}

