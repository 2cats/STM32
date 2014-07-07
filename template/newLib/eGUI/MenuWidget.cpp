/*
 * MenuWidget.cpp
 *
 *  Created on: 2014Äê6ÔÂ27ÈÕ
 *      Author: Administrator
 */

#include "MenuWidget.h"

MenuWidget::MenuWidget(int _x0,int _y0,int _x1,int _y1,int _pandding):Menu(_x0,_y0,_x1,_y1,NULL)
{
	top=(Menu*)this;
	top->parent=NULL;
	visible=true;
}
 void MenuWidget::paint() {
	if(!visible)
	{
		Rectangle::fill(MENUWIDGET_BK_COLOR);
		GUI_SetDrawMode(GUI_DM_XOR);
		Rectangle::fill(MENUWIDGET_BK_COLOR);
		GUI_SetDrawMode(GUI_DM_NORMAL);
		return ;
	}
	std::vector<Menu*>::iterator it;
	if(top->needPaint)
	{
		Rectangle::fill(MENUWIDGET_BK_COLOR);
		for (it = top->sons.begin(); it != top->sons.end(); it++) {

		if((*it)->id==top->curId)
			(*it)->paint(MENU_SHOW_HIGHLIGHT);
		else
			(*it)->paint(MENU_SHOW_NORMAL);
		}
		top->needPaint=false;
	}else
	{
		for (it = top->sons.begin(); it != top->sons.end(); it++)
		{
		if((*it)->needPaint!=true)
			continue;
		if((*it)->id==top->curId)
			(*it)->paint(MENU_SHOW_HIGHLIGHT);
		else
			(*it)->paint(MENU_SHOW_NORMAL);
		}
	}
	Rectangle::paint(MENUWIDGET_BORDER_COLOR);
	needPaint=false;
}
 int ttp;
 void MenuWidget::onMessage(Message *msg) {
	switch (msg->type) {
	case KEY_NULL:
		break;
	case KEY_UP:
		top->sons.at(top->curId)->needPaint=true;
		top->curId=top->curId-1>=0?top->curId-1:top->sons.size()-1;
		top->sons.at(top->curId)->needPaint=true;
		break;
	case KEY_DOWN:
			top->sons.at(top->curId)->needPaint=true;
		top->curId=top->curId+1<top->sons.size()?top->curId+1:0;
	top->sons.at(top->curId)->needPaint=true;
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case KEY_OK:
		if(top->sons.at(top->curId)->sons.size()>0)
		{
			top->needPaint=true;
			top = top->sons.at(top->curId);
			top->needPaint=true;
		}
		if(top->sons.at(top->curId)->cb!=NULL)
		{
			top->sons.at(top->curId)->cb(top->sons.at(top->curId));
		}
		break;
	case KEY_ESC:
		if(top->parent!=NULL)
		{
			top->needPaint=true;
			top = (Menu *)top->parent;
			top->needPaint=true;
		}
		break;
	}
}
void MenuWidget::messageReceiever(Message *msg) {
	if(top->sons.at(top->curId)->type==MENU_TYPE_INTERACTIVE)
	{
		top->sons.at(top->curId)->onMessage(msg);
	}
	top->onMessage(msg);
	paint();

}
