/*
 * MessageDispatcher.h
 *
 *  Created on: 2014Äê6ÔÂ27ÈÕ
 *      Author: Administrator
 */

#ifndef MESSAGEDISPATCHER_H_
#define MESSAGEDISPATCHER_H_

#include <queue>
#include "Message.h"
class MessageDispatcher {
public:
	MessageDispatcher();
	void sendMessage(Message *msg)
	{

	}
	virtual ~MessageDispatcher();
private:
};

#endif /* MESSAGEDISPATCHER_H_ */
