/*
 * Message.h
 *
 *  Created on: 2014��6��27��
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
	int type; // ��Ϣ����
	unsigned int arg0; // ��Ϣ����1
	unsigned int arg1; // ��Ϣ����2
};

#endif /* MESSAGE_H_ */
