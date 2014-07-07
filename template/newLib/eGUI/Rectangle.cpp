/*
 * Rectangle.cpp
 *
 *  Created on: 2014Äê6ÔÂ26ÈÕ
 *      Author: Administrator
 */

#include "Rectangle.h"
Rectangle::Rectangle(int _x0,int _y0,int _x1,int _y1):GObject(_x0,_y0),x1(_x1),y1(_y1)
{
	paddingL=paddingR=paddingT=paddingB=0;
}

void Rectangle::alignLeft(Rectangle *son, Rectangle *parent) {
	son->x1 = parent->x0 + son->getWidth();
	son->x0 = parent->x0;
}
void Rectangle::alignRight(Rectangle *son, Rectangle *parent) {
	son->x0 = parent->x1 - son->getWidth();
	son->x1 = parent->x1;
}
void Rectangle::alignTop(Rectangle *son, Rectangle *parent) {
	son->y1 = parent->y0 + son->getHeight();
	son->y0 = parent->y0;
}
void Rectangle::alignBottom(Rectangle *son, Rectangle *parent) {
	son->y0 = parent->y1 - son->getHeight();
	son->y1 = parent->y1;
}
void Rectangle::alignHCenter(Rectangle *son, Rectangle *parent) {
	son->move(parent->getHCenter() - son->getHCenter(), 0);
}
void Rectangle::alignVCenter(Rectangle *son, Rectangle *parent) {
	son->move(parent->getVCenter() - son->getVCenter(), 0);
}
void Rectangle::alignCenter(Rectangle *son, Rectangle *parent) {
	alignVCenter(son, parent);
	alignHCenter(son, parent);
}
int Rectangle::getWidth() {
	return x1 - x0;
}
int Rectangle::getHeight() {
	return y1 - y0;
}
void Rectangle::move(int dx, int dy) {
	x0 += dx;
	x1 += dx;
	y0 += dy;
	y1 += dy;
}
int Rectangle::getHCenter() {
	return (x0 + x1) / 2;
}
int Rectangle::getVCenter() {
	return (y0 + y1) / 2;
}
void Rectangle::paint(int arg) {
	if(GUI_COLOR_NONE!=arg)
	{
		GUI_SetColor(arg);
		GUI_DrawRect(x0, y0, x1, y1);
	}
}
void Rectangle::fill(int arg)
{
	if(GUI_COLOR_NONE!=arg)
	{
		GUI_SetColor(arg);
		GUI_FillRect(x0, y0, x1, y1);
	}

}
Rectangle::~Rectangle() {

	// TODO Auto-generated destructor stub
}
int Rectangle::getRx0()
{
	return x0+paddingL;
}
int Rectangle::getRx1()
{
	return x1-paddingR;
}

int Rectangle::getRy0()
{
	return y0+paddingT;
}

int Rectangle::getRy1()
{
	return y1-paddingB;
}


