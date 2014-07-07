/*
 * TimerChannel.cpp
 *
 *  Created on: 2014��6��30��
 *      Author: Administrator
 */

#include "TimerChannel.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
TimerChannel::TimerChannel(TIMER_ChannelBase conf,TIMER_ConfBase *_parent_conf) {
	// TODO Auto-generated constructor stub
	config = conf;
	parent_conf=_parent_conf;
}

TimerChannel::~TimerChannel() {
	// TODO Auto-generated destructor stub
}
void TimerChannel::startPWM(double ratio)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

 	RCC_APB2PeriphClockCmd(config.RCC_GPIO, ENABLE);  //ʹ��GPIO����

   //��������Ϊ�����������
	GPIO_InitStructure.GPIO_Pin = config.GPIO_Pin; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(config.GPIOx, &GPIO_InitStructure);//��ʼ��GPIO


	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	config.TIM_OCxInit(parent_conf->TIMx, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������

	changeRatio(ratio);
//	TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);  //ʹ��Ԥװ�ؼĴ���

}
void TimerChannel::changeRatio(double ratio)
{
	config.TIM_SetComparex(parent_conf->TIMx,parent_conf->TIM_Period*ratio);
}
//void TimerChannel::startCatch(u8 _catchType,int resolution,int _flowSize,Catch_cb cb_done,Catch_cb cb_timeout=0)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	TIM_ICInitTypeDef  TIM_ICInitStructure;
//
//	catchType=_catchType;
//	flowSize=_flowSize;
//	RCC_APB2PeriphClockCmd(config.RCC_GPIO, ENABLE);
//	GPIO_InitStructure.GPIO_Pin  = config.GPIO_Pin;
//	if(catchType==CATCH_HIGH||catchType==CATCH_PWM_FTRIG_UP)
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
//	else
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_Init(config.GPIOx, &GPIO_InitStructure);
//	if(catchType==CATCH_HIGH||catchType==CATCH_PWM_FTRIG_UP)
//	GPIO_ResetBits(config.GPIOx,config.GPIO_Pin);
//	else
//	GPIO_SetBits(config.GPIOx,config.GPIO_Pin);
//
//	TIM_TimeBaseStructure.TIM_Period = 0xffff; //�趨�������Զ���װֵ
//	TIM_TimeBaseStructure.TIM_Prescaler =(72*resolution-1); 	//Ԥ��Ƶ��
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(parent_conf->TIMx, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
//	//��ʼ��TIM5���벶�����
//
//	TIM_ICInitStructure.TIM_Channel = config.TIM_Channelx; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
//	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
//	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
//	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ
//	TIM_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
//	TIM_ICInit(parent_conf->TIMx, &TIM_ICInitStructure);
//	//�жϷ����ʼ��
//	NVIC_InitStructure.NVIC_IRQChannel = parent_conf->TIMx_IRQn;  //TIM3�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
//
//	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
//
//	TIM_ITConfig(parent_conf->TIMx,TIM_IT_Update|config.TIM_IT_CCx,ENABLE);//��������ж� ,����CC1IE�����ж�
//	TIM_Cmd(parent_conf->TIMx,ENABLE );
//
//}

