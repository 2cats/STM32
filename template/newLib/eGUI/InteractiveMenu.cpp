/*
 * InteractiveMenu.cpp
 *
 *  Created on: 2014Äê6ÔÂ28ÈÕ
 *      Author: Administrator
 */

#include "InteractiveMenu.h"

InteractiveMenu::InteractiveMenu(int _height,std::string str):Menu(0,0,0,0,NULL),height(_height),text(str)
{
	// TODO Auto-generated constructor stub
	y0=0;
	y1=_height;
	type=MENU_TYPE_INTERACTIVE;
}
InteractiveMenu::~InteractiveMenu() {
	// TODO Auto-generated destructor stub
}

