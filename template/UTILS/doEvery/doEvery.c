#include "doEvery.h"

/******INTERFACE*********/
/*
���ܣ�ÿ������ms��usִ��һ�δ���
void doEvery(enum DoEvery_Resolution resolution,uint16_t proportion,void(*doIt)(void));

DoEvery_Resolution:���뻹��΢��		
			DoEvery_Ms	/			DoEvery_Us
proportion	������ʱ��			
		������������������������������������������������--
		|							DoEvery_Ms			  DoEvery_Us	 	|
		|	<���ֵ>	:				6000			/			65536				|
		|	<��Сֵ>	:				1					/			1						|
		-----------------------------------------------
ͬʱ�����ṩ��ʱ����millis/uillis���ܣ�xillis����Ϊmillis/uillis����һ��ȡ����doEvery��resolution,,)

*/

/******WARNING*********/
/*
	proportion��Ҫ�������ֵ
	�ص�����ע��ʱ�䳤��
	�жϺ�InputCatcher�жϿ��ܳ�ͻ

	ʹ��xillisʱ��
		1.�ص������ϸ����ʱ�䳤�ȣ�����ֱ���޻ص���doIt=0
		2.xillis����Ϊmillis/uillis����һ
		3.xillis�������ʱ�䣺
				ms��50		days
				us��1.19	hour
	
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
	//��ʼ����ʱ��5 TIM2	 
	TIM_TimeBaseStructure.TIM_Period = _DoEvery_GetPeriod(resolution,proportion) ; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =_DoEvery_GetPrescaler(resolution); 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(DO_EVERY_TIM, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = filterTIM_IRQn_DO_EVERY(DO_EVERY_TIM);  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	TIM_ITConfig(DO_EVERY_TIM,TIM_IT_Update,ENABLE);//��������ж� ,����CC1IE�����ж�	
	TIM_ClearITPendingBit(DO_EVERY_TIM,TIM_IT_Update); //����жϱ�־λ
	
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
