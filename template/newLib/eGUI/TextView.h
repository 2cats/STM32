/*
 * TextView.h
 *
 *  Created on: 2014年6月26日
 *      Author: Administrator
 */

#ifndef TEXTVIEW_H_
#define TEXTVIEW_H_

#include "Rectangle.h"
#include <string>
#include "GObject.h"

/*
 *	TextView
 *	Rectangle+字符串，Config ：
 *	1.背景开关
 *	2.字体
 */

class TextView: public Rectangle {
public:
	TextView(int _x0,int _y0,int _x1,int _y1,std::string str);
	virtual void paint(int arg);
	std::string text;

};

#endif /* TEXTVIEW_H_ */
