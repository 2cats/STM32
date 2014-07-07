#ifndef _MILLIS_H
#define _MILLIS_H
//!millis 50天后失效
#include "stm32f10x_rcc.h"
#ifdef __cplusplus
 extern "C" {
#endif
#define RTC_CURRENT_VAL ((RTC->CNTH<<16)+RTC->CNTL)

void MILLIS_Init(void);
unsigned long millis(void);

//回调函数在闹钟中断中执行	  xm范围为：xm+RTC_CURRENT_VAL<	0xffffffff
void doAfter_ms(u32 xm,void(*doSomeThing)(void)); 
 #ifdef __cplusplus
}
#endif
#endif
