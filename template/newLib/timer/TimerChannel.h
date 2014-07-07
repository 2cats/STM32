/*
 * TimerChannel.h
 *
 *  Created on: 2014Äê6ÔÂ30ÈÕ
 *      Author: Administrator
 */

#ifndef TIMERCHANNEL_H_
#define TIMERCHANNEL_H_
#include "config/Configure.h"
#define CATCH_HIGH							0
#define CATCH_LOW							1
#define CATCH_PWM_FTRIG_UP					2
#define CATCH_PWM_FTRIG_DOWN				3
struct CatchInfo
{
	double high_duration;
	double low_duration;
};
typedef void (*Catch_cb)(CatchInfo *);
class TimerChannel {
public:

	TimerChannel(TIMER_ChannelBase conf,TIMER_ConfBase *_parent_conf);
	virtual ~TimerChannel();
	void startPWM(double ratio);
	void changeRatio(double ratio);
	TIMER_ChannelBase config;
	TIMER_ConfBase *parent_conf;

//	void startCatch(u8 _catchType,int resolution,int _flowSize,Catch_cb cb_done,Catch_cb cb_timeout=0);
//
//	Catch_cb catch_cb_done,catch_cb_timeout;
//	u8 catchType;
//	int flowSize;

};

#endif /* TIMERCHANNEL_H_ */
