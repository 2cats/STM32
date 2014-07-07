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

	TIM_TimeBaseStructure.TIM_Period = conf.TIM_Period; //�趨�������Զ���װֵ
	TIM_TimeBaseStructure.TIM_Prescaler = conf.TIM_Prescaler; 	//Ԥ��Ƶ��
	TIM_TimeBaseStructure.TIM_ClockDivision = conf.TIM_ClockDivision; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = conf.TIM_CounterMode;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(conf.TIMx, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	NVIC_InitStructure.NVIC_IRQChannel = conf.TIMx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 

//	TIM_ClearITPendingBit(conf.TIMx, TIM_IT_Update); //����жϱ�־λ
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
	TIM_ITConfig(config.TIMx, TIM_IT_Update, ENABLE);  //��������ж�
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
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 72000 / freq - 0.5; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(config.TIMx, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
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
//		if ((*STA & 0X80) == 0) //��δ�ɹ�����
//				{
//			/*************************UPDATE�¼�***********************************************/
//			if (TIM_GetITStatus(channel->parent_conf->TIMx, TIM_IT_Update)
//					!= RESET) {
//				if ((*STA & 0X40) | (*STA & 0X100)) //�Ѿ�����2or3
//						{
//					if ((*STA & channel->flowSize) == channel->flowSize) //�ߵ�ƽ̫����
//							{
//						*STA |= 0X80; //�����˴β���
//						if (channel->catch_cb_timeout != 0)
//							channel->catch_cb_timeout();
//					} else
//						(*STA)++;
//				}
//			}
//			/***************************�����¼�***************************************************/
//
//			if (TIM_GetITStatus(channel->parent_conf->TIMx,
//					channel->config.TIM_IT_CCx) != RESET)			//����1���������¼�
//					{
//				if (*STA & 0X40)		//����2
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
//				} else if (*TIMxCHy_CAPTURE_STA & 0X100)		//����3
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
//					*STA |= 0X40;		//��ǲ���1
//					TIM_OCxPolarityConfig(TIM_Route, TIM_ICPolarity[1]);//CC1P=1 ����Ϊ�½��ز���
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
//	TIM_ClearITPendingBit(TIMCurrent, filterTIM_Input_IT_CC(TIM_Route)); //����жϱ�־λ
//}
