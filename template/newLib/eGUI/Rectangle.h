/*
 * Rectangle.h
 *
 *  Created on: 2014年6月26日
 *      Author: Administrator
 */

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "GObject.h"

#define RECT_DEFAULT_LINE_COLOR	GUI_RED
#define RECT_DEFAULT_FILL_COLOR	GUI_GREEN
struct RectVector
{
	int _x0;
	int _y0;
	int _x1;
	int _y1;
};
/*
 * Rectangle
 * 1.两点坐标
 * 2.颜色
 * 3.坐标变化
 */
class Rectangle :public GObject{
protected:
	Rectangle(int _x0,int _y0,int _x1,int _y1);
public:
	static void alignLeft(Rectangle *son,Rectangle *parent);
	static void alignRight(Rectangle *son,Rectangle *parent);
	static void alignHCenter(Rectangle *son,Rectangle *parent);
	static void alignVCenter(Rectangle *son,Rectangle *parent);
	static void alignCenter(Rectangle *son,Rectangle *parent);
	static void alignTop(Rectangle *son,Rectangle *parent);
	static void alignBottom(Rectangle *son,Rectangle *parent);
	int getWidth();
	int getHeight();
	int getHCenter();
	int getVCenter();
	void move(int dx,int dy);
	virtual ~Rectangle();
	virtual void paint(int arg=PAINT_DEFAULT);
	void fill(int arg=PAINT_DEFAULT);
	int x1, y1;
	int paddingL,paddingR,paddingT,paddingB;
	int getRx0();
	int getRy0();
	int getRx1();
	int getRy1();

};

#endif /* RECTANGLE_H_ */
