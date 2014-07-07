/*
 * NumInputMenu.h
 *
 *  Created on: 2014Äê6ÔÂ28ÈÕ
 *      Author: Administrator
 */

#ifndef NUMINPUTMENU_H_
#define NUMINPUTMENU_H_
#include "InteractiveMenu.h"
#include "StatusBar.h"
#define  	InputDouble	0
#define  	InputInt	1
#define 	NumberInputCharsLimit	6
class NumInputMenu :public InteractiveMenu{
public:
	NumInputMenu(std::string str,int _type,void (*_doWhenSet)(NumInputMenu*),int heightplus=0);
	virtual void onMessage(Message *msg);
	virtual void paint(int arg=PAINT_DEFAULT);
	void (*doWhenChange)(NumInputMenu*);
	virtual ~NumInputMenu();
	TextView textview;
	void showHighlight();
	void showNormal();
	void updatePosi(int _x0,int _y0,int _x1,int _y1);
	int intValue;
	double doubleValue;
	bool intSuccess,doubleSuccess;
	void setMax(int _max);
	void setMin(int _min);
private:
	int max,min;
	bool maxSet,minSet;
	int inputType;
	int toInt(std::string &str);
	double toDouble(std::string &str);
	StatusBar statusBar;
};

#endif /* NUMINPUTMENU_H_ */
