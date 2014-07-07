/*
 * CheckBoxMenu.h
 *
 *  Created on: 2014Äê6ÔÂ27ÈÕ
 *      Author: Administrator
 */

#ifndef CHECKBOXMENU_H_
#define CHECKBOXMENU_H_
#include "InteractiveMenu.h"

#define CHECKBOX_OUTER_COLOR GUI_RED
#define CHECKBOX_INNER_COLOR GUI_DARKRED

class CheckBoxMenu: public InteractiveMenu {
public:
	CheckBoxMenu(int _height,std::string str,void (*_doWhenChange)(CheckBoxMenu*));
	virtual void paint(int arg=PAINT_DEFAULT);
	virtual ~CheckBoxMenu();
	void (*doWhenChange)(CheckBoxMenu*);
	int getAL();
	int getAS();
	void drawOuter();
	void drawInner();
	void clearInner();
	virtual void onMessage(Message *msg);
	void showNormal();
	void showHighlight();
	void updatePosi(int,int,int,int);
	bool status;
	int CheckBox_A_margin	;
	int CheckBox_A_padding	;
};

#endif /* CHECKBOXMENU_H_ */
