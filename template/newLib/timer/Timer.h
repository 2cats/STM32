#ifndef __TIMER_H
#define __TIMER_H 

/*			Interface:
 *
 * 	�޸�Configure�ļ���������ARR&PSc
 * 	doEvery��tick��λ�Ƕ�ʱ���������ʱ��
 *
 * Ĭ������£�
 *	ARR(999)&PSC(71):doEvery��ʱ����λ��1ms
 *	��ʹ��PWMʱ��ı�doEvery��ʱ����λΪPWM������
 */



/************START**************/
	 
#include <stm32f10x.h>
#include "config/Configure.h"
#include <map>
#include "TimerChannel.h"
#define TIM_PWM_DEFALUT_ARR		999
typedef void (*VV_CallBack_TypeDef)(void) ;
class Timer
{
public:
		Timer(TIMER_ConfBase& conf);
		unsigned long  ticks();
		void doEvery(int x_ticks,void (*cb)(void));
		void setPWMFreq(double freq,int arr=TIM_PWM_DEFALUT_ARR);
		void pause();
		void resume();

		TimerChannel channel1;
		TimerChannel channel2;
		TimerChannel channel3;
		TimerChannel channel4;

		TIMER_ConfBase config;

};
	 
	 
	 
/*************END*************/
#endif
