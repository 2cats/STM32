/*
 * MenuWidget.h
 *
 *  Created on: 2014Äê6ÔÂ27ÈÕ
 *      Author: Administrator
 */

#ifndef MENUWIDGET_H_
#define MENUWIDGET_H_
#include "Rectangle.h"
#include "GObject.h"
#include "Menu.h"

class MenuWidget : public Menu
{
public:
	MenuWidget(int _x0,int _y0,int _x1,int _y1,int _pandding=0);
	virtual void paint();
	virtual void onMessage(Message *msg);
	void messageReceiever(Message *msg);
private:
	Menu *top;
	bool visible;
};


#endif /* MENUWIDGET_H_ */
