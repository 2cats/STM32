#include "stm32f10x.h"
#include "PWMManager.h"  //<---��������˵��




/*********��WARNING��*********/
/*
ʹ��ע�⣺

	ͬʱʹ��һ��TIM�Ķ��ͨ��ʱ������ͨ����"Ƶ��"������ͬ
	���ܺ�InputCatcher����ͬһ����ʱ��

����ֵ��
	����Ƶ�ʷ�Χ��20Hz--100KHz   �������Ƽ�Ƶ�ʷ�Χ�����ܻ��޷��������λ�׼ȷ�ȵͣ�Ƶ�ʱ���������ʱ�����Ƶ��Ĭ��ʹ��72Mʱ�ӵ�72��Ƶ��


��λ��
	Ƶ�ʵ�λ��Hz
	

*/
/*********��INTERFACE��*********/
/*
//Example��
		PWMManagerTypeDef PWMManager;
		PWMManagerStructure(&PWMManager);
		PWMManager.ProduceNewPWM(TIM2CHANNEL2,1000,0.8);
		PWMManager.changeRatio(TIM2CHANNEL2,0.3);//Ƶ�ʲ���
		PWMManager.changeFrequency(TIM2CHANNEL2,2000);//ռ�ձȲ���
	
	CatchType(����):
		���16·PWM����
*/
/***********************************/
PWMManagerTypeDef *defaultPWMManager;
uint32_t  filterGPIO_RCC( TIM_RouteTypeDef TIM_Route)
{
	if(TIM_Route<10)
		return RCC_APB2Periph_GPIOA;
	else
		return RCC_APB2Periph_GPIOB;
}
GPIO_TypeDef * filterGPIO( TIM_RouteTypeDef TIM_Route)
{
	if(TIM_Route<10)
		return GPIOA;
	else
		return GPIOB;
}
uint16_t  filterPin( TIM_RouteTypeDef TIM_Route)
{
	switch((u8)TIM_Route)
	{
		case 0:return GPIO_Pin_8;
		case 1:return GPIO_Pin_9;
		case 2:return GPIO_Pin_10;
		case 3:return GPIO_Pin_11;
		case 4:return GPIO_Pin_0;
		case 5:return GPIO_Pin_1;
		case 6:return GPIO_Pin_2;
		case 7:return GPIO_Pin_3;
		case 8:return GPIO_Pin_6;
		case 9:return GPIO_Pin_7;
		case 10:return GPIO_Pin_0;
		case 11:return GPIO_Pin_1;
		case 12:return GPIO_Pin_6;
		case 13:return GPIO_Pin_7;
		case 14:return GPIO_Pin_8;
		case 15:return GPIO_Pin_9;
	}
	return 0xff;
}


TIM_TypeDef*  filterTIM( TIM_RouteTypeDef TIM_Route)
{
	if(TIM_Route<4)
		return  TIM1;
	else if(TIM_Route<8)
		return  TIM2;
	else if(TIM_Route<12)
		return  TIM3;
	else 
		return  TIM4;
}
uint32_t  filterTIM_RCC( TIM_RouteTypeDef TIM_Route)
{
	if(TIM_Route<4)
		return RCC_APB2Periph_TIM1;
	else if(TIM_Route<8)
		return RCC_APB1Periph_TIM2;
	else if(TIM_Route<12)
		return RCC_APB1Periph_TIM3;
	else 
		return RCC_APB1Periph_TIM4;
}

void TIM_OCxInit( TIM_RouteTypeDef TIM_Route, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
	TIM_TypeDef* TIMx=filterTIM(TIM_Route);

	if(TIM_Route%4==0)
		TIM_OC1Init(TIMx,TIM_OCInitStruct);
	else if((TIM_Route-1)%4==0)
		TIM_OC2Init(TIMx,TIM_OCInitStruct);
	else if((TIM_Route-2)%4==0)
		TIM_OC3Init(TIMx,TIM_OCInitStruct);
	else if((TIM_Route-3)%4==0)
		TIM_OC4Init(TIMx,TIM_OCInitStruct);
}
void TIM_OCxPreloadConfig( TIM_RouteTypeDef TIM_Route,uint16_t TIM_OCPreload)
{
	TIM_TypeDef* TIMx=filterTIM(TIM_Route);

	if(TIM_Route%4==0)
		TIM_OC1PreloadConfig(TIMx,TIM_OCPreload);
	else if((TIM_Route-1)%4==0)
		TIM_OC2PreloadConfig(TIMx,TIM_OCPreload);
	else if((TIM_Route-2)%4==0)
		TIM_OC3PreloadConfig(TIMx,TIM_OCPreload);
	else if((TIM_Route-3)%4==0)
		TIM_OC4PreloadConfig(TIMx,TIM_OCPreload);
}

void PWMProducer_StructureandStart(PWMProducerTypeDef*PWMProducer);
void PWM_chageRatio(PWMProducerTypeDef* PWMProducer, float ratio);

void PWM_chageRatio(PWMProducerTypeDef* PWMProducer, float ratio)
{
	uint16_t Compare=TIM_REAL_FREQ/PWMProducer->frequency*ratio;

	TIM_TypeDef* TIMx=filterTIM(PWMProducer->route);
	if(PWMProducer->route%4==0)
		TIM_SetCompare1(filterTIM(PWMProducer->route),Compare);
	else if((PWMProducer->route-1)%4==0)
		TIM_SetCompare2(filterTIM(PWMProducer->route),Compare);
	else if((PWMProducer->route-2)%4==0)
		TIM_SetCompare3(filterTIM(PWMProducer->route),Compare);
	else if((PWMProducer->route-3)%4==0)
		TIM_SetCompare4(filterTIM(PWMProducer->route),Compare);
}

void PWMStart(PWMProducerTypeDef*PWMProducer)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	uint32_t TIMx_RCC=filterTIM_RCC(PWMProducer->route);
	if(TIMx_RCC==RCC_APB2Periph_TIM1)
		RCC_APB2PeriphClockCmd(TIMx_RCC, ENABLE);//###
	else
		RCC_APB1PeriphClockCmd(TIMx_RCC, ENABLE);//###	
	
	RCC_APB2PeriphClockCmd(filterGPIO_RCC(PWMProducer->route), ENABLE);//###  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��ʹ��
	GPIO_InitStructure.GPIO_Pin = filterPin(PWMProducer->route);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(filterGPIO(PWMProducer->route), &GPIO_InitStructure);//###


	TIM_TimeBaseStructure.TIM_Period =(u16)(TIM_REAL_FREQ/PWMProducer->frequency); //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ     80K
	TIM_TimeBaseStructure.TIM_Prescaler =TIM_FREQ_DIV-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(filterTIM(PWMProducer->route), &TIM_TimeBaseStructure); //###//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	if(filterTIM(PWMProducer->route)==TIM1)
	{
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; //�Ƚ����ʹ��
	}
	else
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = TIM_REAL_FREQ/PWMProducer->frequency*PWMProducer->ratio; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
  TIM_OCxInit(PWMProducer->route,&TIM_OCInitStructure);


	TIM_Cmd(filterTIM(PWMProducer->route), ENABLE); //### //ʹ��TIMx����
	if(filterTIM(PWMProducer->route)==TIM1)
		TIM_CtrlPWMOutputs(TIM1,ENABLE);
	TIM_ARRPreloadConfig(filterTIM(PWMProducer->route), DISABLE);
	TIM_OCxPreloadConfig(PWMProducer->route, TIM_OCPreload_Disable); //### //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
//	else
//		PWM_chageRatio(PWMProducer,PWMProducer->ratio);
}
void PWMManagerStructure(PWMManagerTypeDef *PWMManager)
{

	defaultPWMManager=PWMManager;
	defaultPWMManager->num=0;
	PWMManager->ProduceNewPWM=&PWMManager_ProduceNewPWM;
	PWMManager->changeRatio=&PWMManager_changeRatio;
	PWMManager->changeFrequency=PWMManager_changeFrequency;
}
void PWMManager_ProduceNewPWM(TIM_RouteTypeDef route,float frequency,float ratio)
{
	u8 i;
	for(i=0;i<defaultPWMManager->num;i++)
		if(route==defaultPWMManager->PWMProducer[i].route)
		{
			return;
		}
	defaultPWMManager->PWMProducer[defaultPWMManager->num].frequency=frequency;
	defaultPWMManager->PWMProducer[defaultPWMManager->num].ratio=ratio;
	defaultPWMManager->PWMProducer[defaultPWMManager->num].route=route;

	PWMStart(&defaultPWMManager->PWMProducer[defaultPWMManager->num]);
	defaultPWMManager->num++;
}
void PWMManager_changeRatio(TIM_RouteTypeDef route,float ratio)
{
	u8 i=0;
	for(i=0;i<defaultPWMManager->num;i++)
	if(route==defaultPWMManager->PWMProducer[i].route)
	{
		defaultPWMManager->PWMProducer[i].ratio=ratio;
		PWM_chageRatio(&defaultPWMManager->PWMProducer[i],ratio);
	}
}
void PWMManager_changeFrequency(TIM_RouteTypeDef route,float frequency)
{
	u8 i;
	for(i=0;i<defaultPWMManager->num;i++)
		if(route==defaultPWMManager->PWMProducer[i].route)
		{
			defaultPWMManager->PWMProducer[i].frequency=frequency;
			filterTIM(route)->ARR=(u16)(TIM_REAL_FREQ/defaultPWMManager->PWMProducer[i].frequency);
			PWM_chageRatio(&defaultPWMManager->PWMProducer[i],defaultPWMManager->PWMProducer[i].ratio);
			filterTIM(route)->CNT=0;
		}

}
