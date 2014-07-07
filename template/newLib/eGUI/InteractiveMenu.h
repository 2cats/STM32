/*
 * InteractiveMenu.h
 *
 *  Created on: 2014Äê6ÔÂ28ÈÕ
 *      Author: Administrator
 */

#ifndef INTERACTIVEMENU_H_
#define INTERACTIVEMENU_H_
#include "Menu.h"
class InteractiveMenu: public Menu {
public:
	InteractiveMenu(int _height,std::string str);
	virtual ~InteractiveMenu();
	std::string text;
	int height;
	virtual void updatePosi(int,int,int,int)=0;
	virtual void onMessage(Message *msg)=0;
	virtual void paint(int arg=PAINT_DEFAULT)=0;
};

#endif /* INTERACTIVEMENU_H_ */
