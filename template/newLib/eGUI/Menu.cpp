/*
 * Menu.cpp
 *
 *  Created on: 2014Äê6ÔÂ27ÈÕ
 *      Author: Administrator
 */

#include "Menu.h"

Menu::Menu(int _x0, int _y0, int _x1, int _y1, Menu * _parent ) :
		Rectangle(_x0, _y0, _x1, _y1),
	textview(_x0, _y0, _x1, _y1,"")	,parent(_parent)	{
	curId=0;
	bottom_y = 0;
	id = 0;
	_blood = BLOOD_IGNORE;
	type=MENU_TYPE_STRING;
	cb=NULL;
}
Menu* Menu::addMenu(std::string str)
{
	Menu *root=getRoot();
	Menu *menu=new Menu(root->getRx0(),root->getRy0()+getBottomY(),root->getRx1(),root->getRy0()+getBottomY()+MENU_TEXT_FONT_SIZE,this);
	menu->_blood=BLOOD_CARE;
	menu->id=sons.size();
	menu->textview.text=str;
	sons.push_back(menu);

	bottom_y+=menu->getHeight();
	if(bottom_y>root->getHeight())
	{
		root->y1=root->y0+bottom_y;
	}
	return menu;
}
void Menu::printString(const char *s, int x, int y,int color)
{
	GUI_SetFont (&MENU_TEXT_FONT);
	GObject::printString(s,x,y,color);
}
Menu* Menu::addMenu(Menu *menu)
{
	Menu *root=getRoot();
	menu->_blood=BLOOD_IGNORE;
	menu->id=sons.size();
	menu->parent=this;
	menu->updatePosi(root->getRx0(),root->getRy0()+getBottomY(),root->getRx1(),root->getRy0()+getBottomY()+menu->y1-menu->y0);
	sons.push_back(menu);
	bottom_y+=menu->getHeight();
	if(bottom_y>root->getHeight())
	{
		root->y1=root->y0+bottom_y;
	}
	return menu;
}
void Menu::showNormal()
{
	textview.paint(MENU_TEXT_NORMAL_MODE);
}
void Menu::showHighlight()
{
	textview.paint(MENU_TEXT_HIGHLIGHT_MODE);
}
 void Menu::paint(int mode)
{
	switch(mode)
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
	needPaint=false;
}
void Menu::updatePosi(int _x0,int _y0,int _x1,int _y1)
{

}
Menu::~Menu() {
	std::vector<Menu*>::iterator it;
	for (it = sons.begin(); it != sons.end(); it++) {
		if ((*it)->_blood == BLOOD_CARE)
			delete (*it);
	}
}
int Menu::getBottomY()
{
	return bottom_y;
}
Menu* Menu::getRoot()
{
	Menu*r=this;
	while(r->parent!=NULL)
		r=r->parent;
	return r;
}
void Menu::setCallBack(void (*_cb)(Menu*menu))
{
	cb=_cb;
}
void Menu::onMessage(Message *msg)
{
//	switch(msg->type)
//	{
//		case KEY_NULL:
//					break;
//		case KEY_UP:
//			curId=curId-1>=0?curId-1:sons.size()-1;	
//					break;
//		case KEY_DOWN:
//			curId=curId+1<sons.size()?curId+1:0;
//					break;
//		case KEY_LEFT:
//					break;
//		case KEY_RIGHT:
//					break;
//		case KEY_OK:
//			getRoot()->onMessage(msg);
//					break;
//		case KEY_ESC:
//			getRoot()->onMessage(msg);
//					break;
//	}
	getRoot()->onMessage(msg);
}
