/*
 * Message.h
 *
 *  Created on: 2014年6月27日
 *      Author: Administrator
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#define MENU_SHOW_NORMAL								0
#define MENU_SHOW_HIGHLIGHT							1
#define MENU_SHOW_NULL									2

class Message {
public:
	Message();
	int type; // 消息类型
	unsigned int arg0; // 消息参数1
	unsigned int arg1; // 消息参数2
};

#endif /* MESSAGE_H_ */
