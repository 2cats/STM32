#ifndef _DO_EVERY_H_
#define _DO_EVERY_H_
#include "stm32f10x.h"


/*******Config*******/
//Only TIM1-TIM4 is supported
#define DO_EVERY_TIM	TIM4
//#define DO_EVERY_IRQ_Handler TIM4_IRQHandler_OLD
//TIM1_UP_IRQHandler
//TIM2_IRQHandler
//TIM3_IRQHandler
//TIM4_IRQHandler

enum DoEvery_Resolution {DoEvery_Ms,DoEvery_Us};
void doEvery(enum DoEvery_Resolution resolution,uint16_t proportion,void(*doIt)(void));
unsigned long TIM_xillis(void);
#endif 
