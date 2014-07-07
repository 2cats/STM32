/*
 * NumInputMenu.cpp
 *
 *  Created on: 2014Äê6ÔÂ28ÈÕ
 *      Author: Administrator
 */

#include "NumInputMenu.h"
#include "string.h"

void NumInputMenu::setMax(int _max) {
	max = _max;
	maxSet = true;
}
void NumInputMenu::setMin(int _min) {
	min = _min;
	minSet = true;
}

NumInputMenu::NumInputMenu(std::string str, int _type,
		void (*_doWhenChange)(NumInputMenu*), int _heightplus) :\

		InteractiveMenu(
				_heightplus + MENU_TEXT_FONT_SIZE * 2 + STATUSTEXT_FONT_SIZE,
				str), doWhenChange(_doWhenChange), textview(0, 0, 0, 0, ""), inputType(
				_type), statusBar(0, 0, 0, "") {
	maxSet = minSet = false;
	y0 = 0;
	y1 = _heightplus + MENU_TEXT_FONT_SIZE * 2 + STATUSTEXT_FONT_SIZE;
	max = min = 0;
	intSuccess = doubleSuccess = false;
	intValue = 0;
	doubleValue = 0;
}
void NumInputMenu::updatePosi(int _x0, int _y0, int _x1, int _y1) {
	x0 = _x0;
	y0 = _y0;
	x1 = _x1;
	y1 = _y1;
	textview.x0 = x0;
	textview.y0 = y0 + MENU_TEXT_FONT_SIZE;
	textview.x1 = x1;
	textview.y1 = y1 - STATUSTEXT_FONT_SIZE;

	statusBar.x0 = x0;
	statusBar.y0 = textview.y1;
	statusBar.x1 = x1;
	statusBar.y1 = y1;

}
void NumInputMenu::showNormal() {
	textview.fill(MENU_TEXT_BK_NORMAL_COLOR);
	textview.paint(TEXTVIEW_WRAP_TEXT);
}

void NumInputMenu::showHighlight() {
	textview.paint(TEXTVIEW_FILL_TEXT);
}
void NumInputMenu::paint(int arg) {

	Rectangle::fill(MENU_TEXT_BK_NORMAL_COLOR);
	printString(text.data(), x0 + 4, y0 + 4, MENU_TEXT_COLOR);
	switch (arg) {
	case MENU_SHOW_NULL:
		break;
	case MENU_SHOW_NORMAL:
		showNormal();
		break;
	case MENU_SHOW_HIGHLIGHT:
		showHighlight();
		break;
	}
	statusBar.paint();
	Rectangle::paint(MENU_TEXT_BORDER_COLOR);
	needPaint = false;
}

int NumInputMenu::toInt(std::string &str) {
	int i, tmp = 1, value = 0;
	if(str.size()==0)
		goto intFail;
	if (str.find('.') == -1) {
		for (i = str.size() - 1; i >= 0; i--) {
			value += (str[i] - '0') * tmp;
			tmp = tmp * 10;
		}
		if (maxSet && value > max) {
			goto intFail;
		}
		if (minSet && value < min) {
			goto intFail;
		}
		intSuccess = true;
		return value;
	} else {
		goto intFail;
	}
	intFail: intSuccess = false;
	return -1;

}
double NumInputMenu::toDouble(std::string &str) {
	int _posi = str.find('.'), _i, tmp = 1;
	double value;
		int maybeInt=toInt(str);
		if(intSuccess)
		{
			doubleSuccess = true;
			return maybeInt;
		}
	if (_posi == -1) {
		goto doubleFail;
	} else {
		std::string Hstr = str.substr(0, _posi);
		std::string Lstr = str.substr(_posi + 1, str.size() - _posi);
		if (Lstr.find('.') != -1)
			goto doubleFail;
		for (_i = 0; _i < Lstr.size(); _i++) {
			tmp = tmp * 10;
		}
		value = toInt(Hstr) + toInt(Lstr) * 1.0 / tmp;
		if (maxSet && value > max) {
			goto doubleFail;
		}
		if (minSet && value < min) {
			goto doubleFail;
		}
		doubleSuccess = true;
		return value;
	}
	doubleFail: doubleSuccess = false;
	return 0;
}
void NumInputMenu::onMessage(Message *msg) {
	needPaint = true;
	if (textview.text.size() < NumberInputCharsLimit) {
		if ((msg->type >= KEY_0 && msg->type <= KEY_9)) {
			textview.text.append(1, (char) (msg->type + '0'));
		}
		if (inputType == InputDouble && msg->type == KEY_RIGHT) {
			textview.text.append(1, '.');
		}
	}
	statusBar.text="";
	switch (msg->type) {

	case KEY_OK:
		switch (inputType) {
		case InputDouble:
			doubleValue = toDouble(textview.text);
			break;
		case InputInt:
			intValue = toInt(textview.text);
			break;
		}
		if (intSuccess || doubleSuccess) {
			statusBar.text = "success";
			doWhenChange(this);
		} else {
			statusBar.text = "error";
		}
		doubleSuccess = intSuccess = false;
		msg->type = KEY_NULL;
		break;
	case KEY_LEFT:
		if(textview.text.size()>0)
		textview.text.erase(textview.text.size() - 1, 1);
		msg->type = KEY_NULL;
		break;
	case KEY_ESC:
		textview.text.clear();
		break;

	}
}
NumInputMenu::~NumInputMenu() {
	// TODO Auto-generated destructor stub
}

