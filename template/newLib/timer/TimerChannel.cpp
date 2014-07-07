/*
 * TimerChannel.cpp
 *
 *  Created on: 2014年6月30日
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

 	RCC_APB2PeriphClockCmd(config.RCC_GPIO, ENABLE);  //使能GPIO外设

   //设置引脚为复用输出功能
	GPIO_InitStructure.GPIO_Pin = config.GPIO_Pin; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(config.GPIOx, &GPIO_InitStructure);//初始化GPIO


	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	config.TIM_OCxInit(parent_conf->TIMx, &TIM_OCInitStructure);  //根据T指定的参数初始化外设

	changeRatio(ratio);
//	TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);  //使能预装载寄存器

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
//	TIM_TimeBaseStructure.TIM_Period = 0xffff; //设定计数器自动重装值
//	TIM_TimeBaseStructure.TIM_Prescaler =(72*resolution-1); 	//预分频器
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(parent_conf->TIMx, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
//	//初始化TIM5输入捕获参数
//
//	TIM_ICInitStructure.TIM_Channel = config.TIM_Channelx; //CC1S=01 	选择输入端 IC1映射到TI1上
//	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
//	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
//	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频
//	TIM_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
//	TIM_ICInit(parent_conf->TIMx, &TIM_ICInitStructure);
//	//中断分组初始化
//	NVIC_InitStructure.NVIC_IRQChannel = parent_conf->TIMx_IRQn;  //TIM3中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
//
//	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
//
//	TIM_ITConfig(parent_conf->TIMx,TIM_IT_Update|config.TIM_IT_CCx,ENABLE);//允许更新中断 ,允许CC1IE捕获中断
//	TIM_Cmd(parent_conf->TIMx,ENABLE );
//
//}

