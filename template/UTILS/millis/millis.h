#ifndef _MILLIS_H
#define _MILLIS_H
//!millis 50���ʧЧ
#include "stm32f10x_rcc.h"
#define RTC_CURRENT_VAL ((RTC->CNTH<<16)+RTC->CNTL)

void MILLIS_Init(void);
unsigned long millis(void);

//�ص������������ж���ִ��	  xm��ΧΪ��xm+RTC_CURRENT_VAL<	0xffffffff
void doAfter_ms(u32 xm,void(*doSomeThing)(void)); 
#endif
