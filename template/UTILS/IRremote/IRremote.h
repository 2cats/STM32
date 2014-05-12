#ifndef IRREMOTE_H
#define IRREMOTE_H
#include "PWMManager.h"
#include "InputCatcher.h"
#include "delay.h"
static PWMManagerTypeDef PWMManager;
static InputCatchManagerTypeDef InputCatchManager;
#define IR_CHANNEL	TIM2CHANNEL1
void IRsend_mark(int time) {
  // Sends an IR mark for the specified number of microseconds.
  // The mark output is modulated at the PWM frequency.
  PWMManager.changeRatio(IR_CHANNEL,0.5); // Enable pin 3 PWM output
  delay_ms(time);
}

/* Leave pin off for time (given in microseconds) */
void IRsend_space(int time) {
  // Sends an IR space for the specified number of microseconds.
  // A space is no output, so the PWM output is disabled.
  PWMManager.changeRatio(IR_CHANNEL,0); // Enable pin 3 PWM output
  delay_ms(time);
}
void IRrecv_Init(void)
{

		InputCatchManagerStructure(&InputCatchManager);
		InputCatchManager.startCatching(TIM3CHANNEL2,HIGH_LOW_HIGH_DURATION,1);//TIM3CHANNEL2捕获高电平时间，分辨率1us
		InputCatchManager.setDoWhenDone(TIM3CHANNEL2,doSomeThing);//optional
		InputCatchManager.setDoWhenTimeout(TIM3CHANNEL2,doSomeThingWhenTimeout);//optional
}
void IRsend_Init(void)
{
		PWMManagerStructure(&PWMManager);
		PWMManager.ProduceNewPWM(IR_CHANNEL,38000,0);
}
void IRsend_enableIROut(int khz) {
	PWMManager.changeFrequency(IR_CHANNEL,khz);
}
#endif 