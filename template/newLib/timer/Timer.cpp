#include "Timer.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
Timer::Timer(TIMER_ConfBase& conf) :
		config(conf), channel1(conf.TIMER_Channels[0], &config), channel2(
				conf.TIMER_Channels[1], &config), channel3(
				conf.TIMER_Channels[2], &config), channel4(
				conf.TIMER_Channels[3], &config) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	conf.RCC_APBxPeriphClockCmd(conf.RCC_TIM, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = conf.TIM_Period; //设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler = conf.TIM_Prescaler; 	//预分频器
	TIM_TimeBaseStructure.TIM_ClockDivision = conf.TIM_ClockDivision; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = conf.TIM_CounterMode;  //TIM向上计数模式
	TIM_TimeBaseInit(conf.TIMx, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	NVIC_InitStructure.NVIC_IRQChannel = conf.TIMx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 

//	TIM_ClearITPendingBit(conf.TIMx, TIM_IT_Update); //清除中断标志位
	if (conf.TIMx == TIM1)
		TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_Cmd(conf.TIMx, ENABLE);
}
void (*TIM_UP_callback[5])(void);
unsigned long ticksCount[5];

unsigned long Timer::ticks() {
	return ticksCount[config.id];
}
std::map<int, VV_CallBack_TypeDef> cb_maps[5];
void Timer::doEvery(int x_ticks, void (*cb)(void)) {
	TIM_ITConfig(config.TIMx, TIM_IT_Update, ENABLE);  //允许更新中断
	cb_maps[config.id].insert(std::pair<int, VV_CallBack_TypeDef>(x_ticks, cb));
}
void ticks_callback_proc(int id) {
	std::map<int, VV_CallBack_TypeDef>::iterator it = cb_maps[id].begin();
	while (it != cb_maps[id].end()) {
		if (ticksCount[id] % it->first == 0 && it->second != 0) {
			it->second();
		}
		it++;
	}
}
void Timer::setPWMFreq(double freq, int arr) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = 72000 / freq - 0.5; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(config.TIMx, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	config.TIM_Period = TIM_TimeBaseStructure.TIM_Period;
	config.TIM_Prescaler = TIM_TimeBaseStructure.TIM_Prescaler;
}
extern "C" {
void TIM_IT_UP_Handler(int id) {
	if (TIM_UP_callback[id] != 0) {
		TIM_UP_callback[id]();
	}
	ticksCount[id]++;
	ticks_callback_proc(id);
}
void TIM1_UP_IRQHandler() {
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
		TIM_IT_UP_Handler(1);
		TIM1->SR &= ~((uint16_t) 1);
	}
}

void TIM2_IRQHandler() {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		TIM_IT_UP_Handler(2);
		TIM2->SR &= ~((uint16_t) 1);
	}
}
//void TIM3_IRQHandler() {
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
//		TIM_IT_UP_Handler(3);
//		TIM3->SR &= ~((uint16_t) 1);
//	}
//}
void TIM4_IRQHandler() {
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
		TIM_IT_UP_Handler(4);
		TIM4->SR &= ~((uint16_t) 1);
	}
}
}

//#include <Vector>
//using namespace std;
//struct TIM_CC_Info {
//	unsigned int STA_VAL[4];
//	vector<TimerChannel*> channels;
//};
//TIM_CC_Info info[5];

//void TIM_IT_CC_Handler(int id) {
//
//	vector<TimerChannel*>::iterator itc;
//	unsigned int *STA = &(info[id].STA_VAL[0]);
//	unsigned int *VAL1 = &(info[id].STA_VAL[1]);
//	unsigned int *VAL2 = &(info[id].STA_VAL[2]);
//	unsigned int *VAL3 = &(info[id].STA_VAL[3]);
//	for (itc = info[id].channels.begin(); itc != info[id].channels.end();
//			itc++) {
//		TimerChannel* channel = *itc;
//		if ((*STA & 0X80) == 0) //还未成功捕获
//				{
//			/*************************UPDATE事件***********************************************/
//			if (TIM_GetITStatus(channel->parent_conf->TIMx, TIM_IT_Update)
//					!= RESET) {
//				if ((*STA & 0X40) | (*STA & 0X100)) //已经捕获2or3
//						{
//					if ((*STA & channel->flowSize) == channel->flowSize) //高电平太长了
//							{
//						*STA |= 0X80; //结束此次捕获
//						if (channel->catch_cb_timeout != 0)
//							channel->catch_cb_timeout();
//					} else
//						(*STA)++;
//				}
//			}
//			/***************************捕获事件***************************************************/
//
//			if (TIM_GetITStatus(channel->parent_conf->TIMx,
//					channel->config.TIM_IT_CCx) != RESET)			//捕获1发生捕获事件
//					{
//				if (*STA & 0X40)		//捕获到2
//						{
//					if (channel->catchType == CATCH_PWM_FTRIG_UP
//							| channel->catchType
//									== CATCH_PWM_FTRIG_DOWN) {
//						*STA &= ~0x40;
//						*STA |= 0x100;
//						*VAL2 = TIM_getCRRx(TIM_Route)
//								+ (*STA &  channel->flowSize)
//										* 65536;
//					} else {
//						*STA |= 0x80;
//
//						*VAL2 = TIM_getCRRx(TIM_Route)
//								+ (*STA & channel->flowSize)
//										* 65536;
//						CatchInfo info;
//						info.high_duration=
//						currentCatcher->duration = (*VAL2
//								- *VAL1)
//								* currentCatcher->resolution;
//						if (channel->catch_cb_timeout != 0)
//							channel->catch_cb_done();
//						}
//						*STA = 0;
//					}
//					TIM_OCxPolarityConfig(TIM_Route, TIM_ICPolarity[0]);
//
//				} else if (*TIMxCHy_CAPTURE_STA & 0X100)		//捕获到3
//						{
//					uint32_t CCRx = TIM_getCRRx(TIM_Route);
//					*TIMxCHy_CAPTURE_VAL3 = (u32) (CCRx
//							+ (*TIMxCHy_CAPTURE_STA & CATCHING_FLOW_SIZE)
//									* 65536);
//					TIM_OCxPolarityConfig(TIM_Route, TIM_ICPolarity[1]);
//					currentCatcher->duration = (*TIMxCHy_CAPTURE_VAL3
//							- *TIMxCHy_CAPTURE_VAL1)
//							* currentCatcher->resolution;
//					currentCatcher->duty = (*TIMxCHy_CAPTURE_VAL2
//							- *TIMxCHy_CAPTURE_VAL1)
//							* currentCatcher->resolution;
//					currentCatcher->ratio = currentCatcher->duty
//							/ currentCatcher->duration;
//
//					if (currentCatcher->doWhenDone) {
//						currentCatcher->doWhenDone(currentCatcher);
//					}
//					*VAL1 = CCRx;
//					*STA = 0x40;
//				} else {
//					*STA = 0;
//					*VAL1 = TIM_getCRRx(TIM_Route);
//					*STA |= 0X40;		//标记捕获到1
//					TIM_OCxPolarityConfig(TIM_Route, TIM_ICPolarity[1]);//CC1P=1 设置为下降沿捕获
//				}
//			}
//
//		}
//	}
//	if (currentCatcher->duration == -1000) {
//		if (currentCatcher->doWhenTimeout) {
//			currentCatcher->doWhenTimeout(currentCatcher);
//		}
//		*TIMxCHy_CAPTURE_STA = 0;
//		TIM_OCxPolarityConfig(TIM_Route, TIM_ICPolarity[0]);
//		currentCatcher->duration = -1;
//	}
//	TIM_ClearITPendingBit(TIMCurrent, filterTIM_Input_IT_CC(TIM_Route)); //清除中断标志位
//}
