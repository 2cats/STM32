/*
 * StatusBar.h
 *
 *  Created on: 2014Äê6ÔÂ28ÈÕ
 *      Author: Administrator
 */

#ifndef STATUSBAR_H_
#define STATUSBAR_H_
#include "TextView.h"
#include <string>
class StatusBar: public TextView {
public:
	StatusBar(int _x0,int _y0,int _x1,std::string str);
	virtual ~StatusBar();
	virtual void paint();
};

#endif /* STATUSBAR_H_ */
