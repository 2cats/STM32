/*
 * StringMenu.cpp
 *
 *  Created on: 2014Äê6ÔÂ27ÈÕ
 *      Author: Administrator
 */

#include "StringMenu.h"

StringMenu::StringMenu(int _x0,int _y0,int _x1,int _y1,std::string text,Menu * _parent,bool _paintBase):\
 Menu(_x0,_y0,_x1,_y1,_parent),textview(_x0,_y0,_x1,_y1,text)
{
}
void StringMenu::paint() {
	textview.paint();
}
StringMenu::~StringMenu() {
	// TODO Auto-generated destructor stub
}

