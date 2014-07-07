/*
 * StringMenu.h
 *
 *  Created on: 2014Äê6ÔÂ27ÈÕ
 *      Author: Administrator
 */

#ifndef STRINGMENU_H_
#define STRINGMENU_H_
#include "Menu.h"
class StringMenu : public Menu {
public:
	StringMenu(int _x0,int _y0,int _x1,int _y1,std::string text,Menu * _parent,bool _paintBase=false);
	virtual void paint();
	virtual ~StringMenu();
private:
	TextView textview;
};

#endif /* STRINGMENU_H_ */
