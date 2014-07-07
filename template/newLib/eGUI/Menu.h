/*
 * Menu.h
 *
 *  Created on: 2014Äê6ÔÂ27ÈÕ
 *      Author: Administrator
 */

#ifndef MENU_H_
#define MENU_H_
#include "Rectangle.h"
#include <string>
#include "GObject.h"
#include <vector>
#include <string>
#include "Message.h"
#include "TextView.h"
#define MENU_TYPE_STRING 		0
#define MENU_TYPE_INTERACTIVE	1
class Menu :public Rectangle{
public:
	Menu(int _x0,int _y0,int _x1,int _y1,Menu * _parent);
	Menu* addMenu(std::string str);
	Menu* addMenu(Menu *menu);
	virtual void paint(int arg=PAINT_DEFAULT);
	void showNormal();
	void showHighlight();
	virtual void updatePosi(int,int,int,int);
	void printString(const char *s, int x, int y,int color);
	int getBottomY();
	virtual void onMessage(Message *msg);
	virtual ~Menu();
	int _blood;
	Menu* getRoot();
	std::vector<Menu*>sons;
	void setCallBack(void (*)(Menu*menu));
	int id;
	int curId;
	int bottom_y;
	TextView textview;
	Menu *parent;
	int type;
	void (*cb)(Menu*menu);
};


#endif /* MENU_H_ */
