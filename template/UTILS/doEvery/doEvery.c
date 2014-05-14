#include "doEvery.h"

/******INTERFACE*********/
/*
功能：每隔多少ms或us执行一段代码
void doEvery(enum DoEvery_Resolution resolution,uint16_t proportion,void(*doIt)(void));

DoEvery_Resolution:毫秒还是微妙		
			DoEvery_Ms	/			DoEvery_Us
proportion	具体间隔时间			
		————————————————————————--
		|							DoEvery_Ms			  DoEvery_Us	 	|
		|	<最大值>	:				6000			/			65536				|
		|	<最小值>	:				1					/			1						|
		-----------------------------------------------
同时还可提供定时器版millis/uillis功能：xillis功能为millis/uillis中其一，取决于doEvery（resolution,,)

*/

/******WARNING*********/
/*
	proportion不要超过最大值
	回调函数注意时间长度
	中断和InputCatcher中断可能冲突

	使用xillis时：
		1.回调函数严格控制时间长度，或者直接无回调，doIt=0
		2.xillis功能为millis/uillis中其一
		3.xillis计数溢出时间：
				ms：50		days
				us：1.19	hour
	
*/


static void (*defaultDoEvery)(void)=0;
unsigned long DoEvery_xillis_count;//[0]:1.19hour

uint32_t  filterTIM_RCC_DO_EVERY( TIM_TypeDef *TIMx)
{
	if(TIMx==TIM1)
		return RCC_APB2Periph_TIM1;
	else if(TIMx==TIM2)
		return RCC_APB1Periph_TIM2;
	else if(TIMx==TIM3)
		return RCC_APB1Periph_TIM3;
	else if(TIMx==TIM4)
		return RCC_APB1Periph_TIM4;
	return 0;
}
uint32_t  filterTIM_IRQn_DO_EVERY(TIM_TypeDef *TIMx)
{
	if(TIMx==TIM1)
		return TIM1_UP_IRQn;
	else if(TIMx==TIM2)
		return TIM2_IRQn;
	else if(TIMx==TIM3)
		return TIM3_IRQn;
	else if(TIMx==TIM4)
		return TIM4_IRQn;
	return 0;
}
uint16_t  _DoEvery_GetPrescaler(enum DoEvery_Resolution resolution)
{
	switch(resolution)
	{
		case DoEvery_Ms://0.1ms
			return 0x1C20-1;
		case DoEvery_Us://1us
			return 71;
	}
	return 0;
}
uint16_t _DoEvery_GetPeriod(enum DoEvery_Resolution resolution,uint16_t proportion)
{
	switch(resolution)
	{
		case DoEvery_Ms:
			return proportion*10-1;
		case DoEvery_Us:
			return proportion-1;
	}
	return 0;
}
void doEvery(enum DoEvery_Resolution resolution,uint16_t proportion,void(*doIt)(void))
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	uint32_t TIMx_RCC=filterTIM_RCC_DO_EVERY(DO_EVERY_TIM);
	if(TIMx_RCC==RCC_APB2Periph_TIM1)
		RCC_APB2PeriphClockCmd(TIMx_RCC, ENABLE);//###
	else
		RCC_APB1PeriphClockCmd(TIMx_RCC, ENABLE);//###	
	//初始化定时器5 TIM2	 
	TIM_TimeBaseStructure.TIM_Period = _DoEvery_GetPeriod(resolution,proportion) ; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =_DoEvery_GetPrescaler(resolution); 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(DO_EVERY_TIM, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = filterTIM_IRQn_DO_EVERY(DO_EVERY_TIM);  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	TIM_ITConfig(DO_EVERY_TIM,TIM_IT_Update,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	TIM_ClearITPendingBit(DO_EVERY_TIM,TIM_IT_Update); //清除中断标志位
	
	defaultDoEvery=doIt;
	TIM_Cmd(DO_EVERY_TIM,ENABLE ); 
}
unsigned long TIM_xillis(void)
{
	return DoEvery_xillis_count;
}
//void DO_EVERY_IRQ_Handler(void)
//{
//	if(defaultDoEvery!=0)
//	{
//		defaultDoEvery();
//	}
//	DoEvery_xillis_count++;
//	DO_EVERY_TIM->SR&=~((uint16_t)1);
//}
