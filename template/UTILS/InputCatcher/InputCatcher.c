#include "InputCatcher.h"
#include "stm32f10x.h"

//
/*********��WARNING��*********/
/*

ʹ��ע�⣺
	
	���ܺ�PWMManager����ͬһ����ʱ��
	
	ͬһ����ʱ������ͨ���ķֱ��ʱ�����ͬ

	�ص������ĺ�ʱ�����̣����ڷ��͵Ⱥ�ʱ���Ĳ������ܻ�����������صĴ���
	
	ͬʱʹ��һ��TIM�Ķ��ͨ��ʱ������ͨ����"�ֱ���"������ͬ


����ֵ��

	���޷ֱ��ʣ�0.01388888888888888888888888888889��72M����Ƶ��
	
	����Ƶ�ʷ�Χ��20Hz--100KHz	
	
	���ʱ��=63*65536*�ֱ��ʣ��ֱ���Ϊ1usʱ�����ʱ��ԼΪ4s��


��λ��

	ȫ��ʱ�䵥λ��us

	
*/
/*********��INTERFACE��*********/
/*
//Example��
		InputCatchManagerTypeDef InputCatchManager;
		InputCatchManagerStructure(&InputCatchManager);
		InputCatchManager.startCatching(TIM3CHANNEL2,HIGH_DURATION,1);//TIM3CHANNEL2����ߵ�ƽʱ�䣬�ֱ���1us
		InputCatchManager.setDoWhenDone(TIM3CHANNEL2,doSomeThing);//optional
		InputCatchManager.setDoWhenTimeout(TIM3CHANNEL2,doSomeThingWhenTimeout);//optional
	
	CatchType(����):
		16·���벶��
		�������ͣ�
		HIGH_DURATION:��ߵ�ƽʱ��
		LOW_DURATION:��͵�ƽʱ��
		HIGH_LOW_HIGH_DURATION:��PWM����Ϣ,����duration���ߵ�ƽʱ��duty,ռ�ձ�duty/duration,�״δ���Ϊ�ߵ�ƽ
		LOW_HIGH_LOW_DURATION����PWM����Ϣ,����duration���͵�ƽʱ��duty,ռ�ձ�duty/duration��=1-HIGH_LOW_HIGH_DURATION���״δ���Ϊ�͵�ƽ
		
*/
/***********************************/
//variables
u32 TIM_CAPTURE_STA_VAL1_VAL2_VAL3[16][4];
u32 catchWhat_Flag;
u32 ISROUTEENABLED_Flag;
InputCatchManagerTypeDef *defaultInputCatchManager;
/**********************************/

//filters
uint32_t  filterGPIO_Input_RCC_Input( TIM_RouteTypeDef TIM_Route)
{
	if(TIM_Route<10)
		return RCC_APB2Periph_GPIOA;
	else
		return RCC_APB2Periph_GPIOB;
}
GPIO_TypeDef * filterGPIO_Input( TIM_RouteTypeDef TIM_Route)
{
	if(TIM_Route<10)
		return GPIOA;
	else
		return GPIOB;
}
uint16_t  filterPin_Input( TIM_RouteTypeDef TIM_Route)
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


TIM_TypeDef*  filterTIM_Input( TIM_RouteTypeDef TIM_Route)
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
uint32_t  filterTIM_Input_RCC_Input( TIM_RouteTypeDef TIM_Route)
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
uint32_t  filterTIM_Input_IRQn( TIM_RouteTypeDef TIM_Route)
{
	if(TIM_Route<4)
		return TIM1_CC_IRQn;
	else if(TIM_Route<8)
		return TIM2_IRQn;
	else if(TIM_Route<12)
		return TIM3_IRQn;
	else 
		return TIM4_IRQn;
}
uint32_t  filterTIM_Input_IT_CC( TIM_RouteTypeDef TIM_Route)
{
	if(TIM_Route%4==0)
		return TIM_IT_CC1;
	else if((TIM_Route-1)%4==0)
		return TIM_IT_CC2;
	else if((TIM_Route-2)%4==0)
		return TIM_IT_CC3;
	else if((TIM_Route-3)%4==0)
		return TIM_IT_CC4;
	return 0;
}

uint16_t filterTIM_InputChannel( TIM_RouteTypeDef TIM_Route)
{
	if(TIM_Route%4==0)
		return TIM_Channel_1;
	else if((TIM_Route-1)%4==0)
		return TIM_Channel_2;
	else if((TIM_Route-2)%4==0)
		return TIM_Channel_3;
	else if((TIM_Route-3)%4==0)
		return TIM_Channel_4;
	return 0;
}

uint16_t filterPolarity( CatchWhatTypeDef CatchWhat)
{
	switch((u8)CatchWhat)
	{
		case HIGH_DURATION:return TIM_ICPolarity_Rising;
		case LOW_DURATION:return TIM_ICPolarity_Falling;
		case HIGH_LOW_HIGH_DURATION:return TIM_ICPolarity_Rising;
		case LOW_HIGH_LOW_DURATION:return TIM_ICPolarity_Falling;
		
	}
	return 0;
}
InputCatcherTypeDef* filterInputCatcher(TIM_RouteTypeDef TIM_Route)
{
	u8 i;
	for(i=0;i<defaultInputCatchManager->_num;i++)
	{
		if(defaultInputCatchManager->InputCatcher[i].route==TIM_Route)
			return &defaultInputCatchManager->InputCatcher[i];
	}
	return 0;
}
void TIM_OCxPolarityConfig( TIM_RouteTypeDef Route, uint16_t TIM_OCPolarity)
{
	TIM_TypeDef* TIMx=filterTIM_Input(Route);

	if(Route%4==0)
		TIM_OC1PolarityConfig(TIMx,TIM_OCPolarity);
	else if((Route-1)%4==0)
		TIM_OC2PolarityConfig(TIMx,TIM_OCPolarity);
	else if((Route-2)%4==0)
		TIM_OC3PolarityConfig(TIMx,TIM_OCPolarity);
	else if((Route-3)%4==0)
		TIM_OC4PolarityConfig(TIMx,TIM_OCPolarity);
}
uint16_t TIM_getCRRx( TIM_RouteTypeDef Route)
{
	TIM_TypeDef* TIMx=filterTIM_Input(Route);

	if(Route%4==0)
		return TIMx->CCR1;
	else if((Route-1)%4==0)
		return TIMx->CCR2;
	else if((Route-2)%4==0)
		return TIMx->CCR3;
	else if((Route-3)%4==0)
	  return TIMx->CCR4;
	return 0;
}

/**********************************/
//InputCatchManager Funcs
void InputCatchManager_startCatching(TIM_RouteTypeDef route,CatchWhatTypeDef catchWhat,double resolution)
{
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	uint32_t TIMx_RCC=filterTIM_Input_RCC_Input(route);
	
	InputCatcherTypeDef *currentCatcher=&defaultInputCatchManager->InputCatcher[defaultInputCatchManager->_num];
	defaultInputCatchManager->_num++;
	currentCatcher->route=route;
	currentCatcher->duration=-1;
	currentCatcher->resolution=resolution;
	currentCatcher->catchWhat=catchWhat;
	currentCatcher->doWhenDone=0;
	currentCatcher->doWhenTimeout=0;
	SETFLAG(route);
	ENABLEROUTE(route);
	
	if(TIMx_RCC==RCC_APB2Periph_TIM1)
		RCC_APB2PeriphClockCmd(TIMx_RCC, ENABLE);//###
	else
		RCC_APB1PeriphClockCmd(TIMx_RCC, ENABLE);//###	
	
	RCC_APB2PeriphClockCmd(filterGPIO_Input_RCC_Input(route), ENABLE);//###  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��ʹ��
	GPIO_InitStructure.GPIO_Pin = filterPin_Input(currentCatcher->route);
	if(currentCatcher->catchWhat==HIGH_DURATION|currentCatcher->catchWhat==HIGH_LOW_HIGH_DURATION)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	else
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(filterGPIO_Input(currentCatcher->route), &GPIO_InitStructure);//###
	
	//��ʼ����ʱ��5 TIM2	 
	TIM_TimeBaseStructure.TIM_Period = 0xffff; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =(u32)(72*resolution-1); 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(filterTIM_Input(route), &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	//��ʼ��TIM2���벶�����
 
	TIM_ICInitStructure.TIM_Channel = filterTIM_InputChannel(route);; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM_ICInitStructure.TIM_ICPolarity = filterPolarity(currentCatcher->catchWhat);;	//�����ز���
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
	TIM_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(filterTIM_Input(route), &TIM_ICInitStructure);

	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = filterTIM_Input_IRQn(route);  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	if(filterTIM_Input(route)==TIM1)
	{
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	}

	TIM_ITConfig(filterTIM_Input(route),TIM_IT_Update|filterTIM_Input_IT_CC(route),ENABLE);//��������ж� ,����CC1IE�����ж�	

	TIM_Cmd(filterTIM_Input(route),ENABLE ); 
	TIM_ClearITPendingBit(filterTIM_Input(route),TIM_IT_Update); //����жϱ�־λ

}

void InputCatchManager_setDoWhenDone(TIM_RouteTypeDef Route ,	void (*func)(struct InputCatcherType*))
{
	filterInputCatcher(Route)->doWhenDone=func;
}
//u16 FAKE_DMA_BUF[2];
//DMA_Channel_TypeDef* defaultFAKE_DMA=DMA1_Channel6; 
//IRQn_Type defaultFAKE_DMA_IRQn = DMA1_Channel6_IRQn;
//void InputCatchManager_setDoWhenRatioChanged(TIM_RouteTypeDef Route ,double offset,	void (*func)(struct InputCatcherType*))
//{
//	
//	  DMA_InitTypeDef DMA_InitStructure; 
//    NVIC_InitTypeDef NVIC_InitStructure;
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA??
//	
//		filterInputCatcher(Route)->doWhenRatioChanged=func;
//		filterInputCatcher(Route)->ratio_offet=offset;
//    DMA_DeInit(defaultFAKE_DMA);

//    DMA_InitStructure.DMA_PeripheralBaseAddr =  (uint32_t) FAKE_DMA_BUF;
//    DMA_InitStructure.DMA_MemoryBaseAddr =(u32)&FAKE_DMA_BUF[1]; 
//    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //????
//    DMA_InitStructure.DMA_BufferSize = 1;//Size
//    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //???????????
//    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; //RAM?????????

//		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//		DMA_InitStructure.DMA_MemoryDataSize =DMA_MemoryDataSize_HalfWord; //????!!!!!!!!

//    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Normal; //??????
//    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //???
//    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable; //???RAM2RAM
//    DMA_Init(defaultFAKE_DMA, &DMA_InitStructure); 
//    
//    DMA_ITConfig(defaultFAKE_DMA,DMA_IT_TC,ENABLE ); //DMA??(???NVIC)
//		
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//		NVIC_InitStructure.NVIC_IRQChannel =defaultFAKE_DMA_IRQn;  //TIM3�ж�
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x11;  //��ռ���ȼ�2��
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
//		NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
//	

//}
void InputCatchManager_setDoWhenTimeout(TIM_RouteTypeDef Route ,	void (*func)(struct InputCatcherType*))
{
	filterInputCatcher(Route)->doWhenTimeout=func;
}
void InputCatchManagerStructure(InputCatchManagerTypeDef *InputCatchManager)
{
	defaultInputCatchManager=InputCatchManager;
	defaultInputCatchManager->startCatching=InputCatchManager_startCatching;
	defaultInputCatchManager->_num=0;
//	defaultInputCatchManager->getduration=InputCatchManager_getduration;
	defaultInputCatchManager->setDoWhenDone=InputCatchManager_setDoWhenDone;
	defaultInputCatchManager->setDoWhenTimeout=InputCatchManager_setDoWhenTimeout;
//	defaultInputCatchManager->setDoWhenRatioChanged=InputCatchManager_setDoWhenRatioChanged;

	//defaultInputCatchManager->isCatchingDone=InputCatchManager_isCatchingDone;
}
u8 InputCatchManager_isCatchingDone(TIM_RouteTypeDef Route)
{
	return TIM_CAPTURE_STA_VAL1_VAL2_VAL3[Route][0]&0x80 ;
}
double InputCatchManager_getduration(TIM_RouteTypeDef Route)
{
	return filterInputCatcher(Route)->duration;
}
/**********************************/
//IRQs
//double abs(double a,double b)
//{
//	return a>b?a:b;
//}

void IRQ_HIGH_LOW(TIM_RouteTypeDef TIM_Route)
{
	u32  *TIMxCHy_CAPTURE_STA=&TIM_CAPTURE_STA_VAL1_VAL2_VAL3[TIM_Route][0];    				
  u32	 *TIMxCHy_CAPTURE_VAL1=&TIM_CAPTURE_STA_VAL1_VAL2_VAL3[TIM_Route][1];
	u32 *TIMxCHy_CAPTURE_VAL2=&TIM_CAPTURE_STA_VAL1_VAL2_VAL3[TIM_Route][2];
	u32 *TIMxCHy_CAPTURE_VAL3=&TIM_CAPTURE_STA_VAL1_VAL2_VAL3[TIM_Route][3];
	TIM_TypeDef *TIMCurrent=filterTIM_Input(TIM_Route);
	InputCatcherTypeDef *currentCatcher=filterInputCatcher(TIM_Route);
	uint16_t TIM_ICPolarity[2];

	if(currentCatcher->catchWhat==HIGH_DURATION|currentCatcher->catchWhat==HIGH_LOW_HIGH_DURATION)
	{
		TIM_ICPolarity[0]=TIM_ICPolarity_Rising;
		TIM_ICPolarity[1]=TIM_ICPolarity_Falling;
	}else
	{
		TIM_ICPolarity[1]=TIM_ICPolarity_Rising;
		TIM_ICPolarity[0]=TIM_ICPolarity_Falling;
	}
	if((*TIMxCHy_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{	  
		if (TIM_GetITStatus(TIMCurrent, TIM_IT_Update) != RESET)
		{	    
			if((*TIMxCHy_CAPTURE_STA&0X40) | (*TIMxCHy_CAPTURE_STA&0X100))//�Ѿ�����2or3
			{
				if((*TIMxCHy_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					*TIMxCHy_CAPTURE_STA|=0X80;//�����˴β���
					//*TIMxCHy_CAPTURE_VAL=0XFFFF;
					currentCatcher->duration=-1000;
					
				}else 
				(*TIMxCHy_CAPTURE_STA)++;
			}	 
		}
	if (TIM_GetITStatus(TIMCurrent, filterTIM_Input_IT_CC(TIM_Route)) != RESET)//����1���������¼�
		{	
			if(*TIMxCHy_CAPTURE_STA&0X40)		//����2	
			{	  		
					if(currentCatcher->catchWhat==HIGH_LOW_HIGH_DURATION|currentCatcher->catchWhat==LOW_HIGH_LOW_DURATION)
					{
						*TIMxCHy_CAPTURE_STA&=~0x40;
						*TIMxCHy_CAPTURE_STA|=0x100;
						*TIMxCHy_CAPTURE_VAL2=TIM_getCRRx(TIM_Route)+(*TIMxCHy_CAPTURE_STA&0X3F)*65536;
						//*TIMxCHy_CAPTURE_STA&=~0X3F;
					//	currentCatcher->duration=*TIMxCHy_CAPTURE_VAL3-*TIMxCHy_CAPTURE_VAL1;
					}else
					{
						*TIMxCHy_CAPTURE_STA|=0x80;

						*TIMxCHy_CAPTURE_VAL2=	TIM_getCRRx(TIM_Route)+(*TIMxCHy_CAPTURE_STA&0X3F)*65536;
						currentCatcher->duration=(*TIMxCHy_CAPTURE_VAL2-*TIMxCHy_CAPTURE_VAL1)*currentCatcher->resolution;
						if(currentCatcher->doWhenDone)
						{
							currentCatcher->doWhenDone(currentCatcher);
						}
						*TIMxCHy_CAPTURE_STA=0;
					}
					TIM_OCxPolarityConfig(TIM_Route,TIM_ICPolarity[0]);	
						
			}else  if(*TIMxCHy_CAPTURE_STA&0X100)//����3
			{
			 	uint32_t CCRx=TIM_getCRRx(TIM_Route);
				*TIMxCHy_CAPTURE_VAL3=(u32)(CCRx+(*TIMxCHy_CAPTURE_STA&0X3F)*65536);
				TIM_OCxPolarityConfig(TIM_Route,TIM_ICPolarity[1]);	
//				currentCatcher->lastDuration=currentCatcher->duration;
//				currentCatcher->lastDuty=currentCatcher->duty;
//				currentCatcher->lastRatio=currentCatcher->ratio;
				currentCatcher->duration=(*TIMxCHy_CAPTURE_VAL3-*TIMxCHy_CAPTURE_VAL1)*currentCatcher->resolution;
				currentCatcher->duty=(*TIMxCHy_CAPTURE_VAL2-*TIMxCHy_CAPTURE_VAL1)*currentCatcher->resolution;
				currentCatcher->ratio=currentCatcher->duty/currentCatcher->duration;
				
				if(currentCatcher->doWhenDone)
				{
					currentCatcher->doWhenDone(currentCatcher);
				}
				*TIMxCHy_CAPTURE_VAL1=CCRx;
				*TIMxCHy_CAPTURE_STA=0x40;
			}else
			{
				*TIMxCHy_CAPTURE_STA=0;
				*TIMxCHy_CAPTURE_VAL1=TIM_getCRRx(TIM_Route);
				*TIMxCHy_CAPTURE_STA|=0X40;		//��ǲ���1
		   	TIM_OCxPolarityConfig(TIM_Route,TIM_ICPolarity[1]);		//CC1P=1 ����Ϊ�½��ز���
			}	
		}			     	    					   
 	}
	if(currentCatcher->duration==-1000)
	{
		if(currentCatcher->doWhenTimeout)
		{
			currentCatcher->doWhenTimeout(currentCatcher);
		}
		*TIMxCHy_CAPTURE_STA=0;
		TIM_OCxPolarityConfig(TIM_Route,TIM_ICPolarity[0]);
		currentCatcher->duration=-1;
	}
  TIM_ClearITPendingBit(TIMCurrent, filterTIM_Input_IT_CC(TIM_Route)); //����жϱ�־λ
}
/****************************************************//****************************************************/
/****************************************************//****************************************************/
/****************************************************//****************************************************/
/****************************************************//****************************************************/
/****************************************************//****************************************************/
/****************************************************//****************************************************/
/****************************************************//****************************************************/

////PWM variables
//u16 TIM_CAPTURE_STA_VAL1_VAL2_VAL3_PWM[16][3];
//u32 catchWhat_Flag_PWM;
//u32 ISROUTEENABLED_Flag_PWM;
//PWMInputCatcherManagerTypeDef *defaultPWMInputCatcherManager;

////PWM Filter
//PWMInputCatcherTypeDef* filterInputCatcher_PWM(TIM_RouteTypeDef TIM_Route)
//{
//	u8 i;
//	for(i=0;i<defaultInputCatchManager->_num;i++)
//	{
//		if(defaultPWMInputCatcherManager->PWMInputCatcher[i].route==TIM_Route)
//			return &defaultPWMInputCatcherManager->PWMInputCatcher[i];
//	}
//}

//TIM_RouteTypeDef filterTIM_Route_Partner(TIM_RouteTypeDef TIM_Route)
//{
//	if(TIM_Route%4==0)
//		return TIM_Route+1;
//	else
//		return TIM_Route-1;
//}
//uint16_t filterTIxFPy(TIM_RouteTypeDef TIM_Route)
//{
//	if(filterTIM_InputChannel(TIM_Route)==TIM_Channel_1)
//		return TIM_TS_TI1FP1;
//	else
//		return TIM_TS_TI2FP2;
//}
///****************************************************/
////PWM Func
//void PWMInputCatcherManager_setDoWhenDone(TIM_RouteTypeDef Route ,	void (*func)(struct PWMInputCatcherType*))
//{
//	filterInputCatcher_PWM(Route)->doWhenDone=func; 
//}
//void PWMInputCatcherManager_setDoWhenTimeout(TIM_RouteTypeDef Route ,	void (*func)(struct PWMInputCatcherType*))
//{
//	filterInputCatcher_PWM(Route)->doWhenTimeout=func;
//}
//void PWMInputCatcherManagerStructure(PWMInputCatcherManagerTypeDef *InputCatchManager)
//{
//	defaultPWMInputCatcherManager=InputCatchManager;
//	defaultPWMInputCatcherManager->startCatching=PWMInputCatcherManager_startCatching;
//	defaultPWMInputCatcherManager->_num=0;
//	defaultPWMInputCatcherManager->setDoWhenDone=PWMInputCatcherManager_setDoWhenDone;
//	defaultPWMInputCatcherManager->setDoWhenTimeout=PWMInputCatcherManager_setDoWhenTimeout;
//}


//void PWMInputCatcherManager_startCatching(TIM_RouteTypeDef route,double resolution)
//{
//		TIM_ICInitTypeDef  TIM_ICInitStructure;
//	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//    NVIC_InitTypeDef         NVIC_InitStructure;
//    GPIO_InitTypeDef         GPIO_InitStructure;
//	  PWMInputCatcherTypeDef *currentCatcher=&defaultPWMInputCatcherManager->PWMInputCatcher[defaultPWMInputCatcherManager->_num];
//  	uint32_t TIMx_RCC=filterTIM_Input_RCC_Input(route);
//	
//	defaultInputCatchManager->_num++;
//	currentCatcher->route=route;
//	currentCatcher->period=-1;
//	currentCatcher->resolution=resolution;
//	currentCatcher->duty=-1;
//	currentCatcher->ratio=-1;
//	currentCatcher->doWhenDone=0;
//	currentCatcher->doWhenTimeout=0;
//	SETFLAG_PWM(route);
//	ENABLEROUTE_PWM(route);
//	
//		if(TIMx_RCC==RCC_APB2Periph_TIM1)
//			RCC_APB2PeriphClockCmd(TIMx_RCC, ENABLE);//###
//		else
//			RCC_APB1PeriphClockCmd(TIMx_RCC, ENABLE);//###	
//			
//		GPIO_InitStructure.GPIO_Pin = filterPin_Input(currentCatcher->route);
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_Init(filterGPIO_Input(currentCatcher->route), &GPIO_InitStructure);//###
//		 
//	TIM_TimeBaseStructure.TIM_Period = 0xffff; //�趨�������Զ���װֵ 
//	TIM_TimeBaseStructure.TIM_Prescaler =(u32)(72*resolution-1); 	//Ԥ��Ƶ��   
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(filterTIM_Input(route), &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
//		
//  TIM_ICInitStructure.TIM_Channel = filterTIM_InputChannel(route);; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
//	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
//	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
//	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
//	TIM_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
//	TIM_ICInit(filterTIM_Input(route), &TIM_ICInitStructure);
//		
//	TIM_SelectInputTrigger(filterTIM_Input(route), filterTIxFPy(route));     //ѡ����Ч�����        
//	TIM_SelectSlaveMode(filterTIM_Input(route), TIM_SlaveMode_Reset);  //����Ϊ���Ӹ�λģʽ
//	TIM_SelectMasterSlaveMode(filterTIM_Input(route), TIM_MasterSlaveMode_Enable);                                       

//	TIM_ITConfig(filterTIM_Input(route),TIM_IT_Update|filterTIM_Input_IT_CC(route),ENABLE);//��������ж� ,����CC1IE�����ж�	
//	TIM_ClearITPendingBit(filterTIM_Input(route), filterTIM_Input_IT_CC(route)|TIM_IT_Update); //����жϱ�־λ
//	
//		//�жϷ����ʼ��
//	NVIC_InitStructure.NVIC_IRQChannel = filterTIM_Input_IRQn(route);  //TIM3�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�2��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 

//	TIM_Cmd(filterTIM_Input(route), ENABLE);    
//}
//void IRQ_HIGH_LOW_PWM(TIM_RouteTypeDef TIM_Route)
//{
//	
//	u16  *TIMxCHy_CAPTURE_STA=&TIM_CAPTURE_STA_VAL1_VAL2_VAL3_PWM[TIM_Route][0];    				
//  u16	 *TIMxCHy_CAPTURE_VAL1=&TIM_CAPTURE_STA_VAL1_VAL2_VAL3_PWM[TIM_Route][1];
//	u16 *TIMxCHy_CAPTURE_VAL2=&TIM_CAPTURE_STA_VAL1_VAL2_VAL3_PWM[TIM_Route][2];
//	TIM_TypeDef *TIMCurrent=filterTIM_Input(TIM_Route);
//	PWMInputCatcherTypeDef *currentCatcher=filterInputCatcher_PWM(TIM_Route);
//	uint16_t TIM_ICPolarity[2];

//	if((*TIMxCHy_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
//	{	  
//		if (TIM_GetITStatus(TIMCurrent, TIM_IT_Update) != RESET)
//		{	    
//			if(*TIMxCHy_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
//			{
//				if((*TIMxCHy_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
//				{
//					*TIMxCHy_CAPTURE_STA|=0X80;//�����˴β���
//					//*TIMxCHy_CAPTURE_VAL=0XFFFF;
//					currentCatcher->period=-1000;
//				}else 
//				(*TIMxCHy_CAPTURE_STA)++;
//			}	 
//		}
//	if (TIM_GetITStatus(TIMCurrent, filterTIM_Input_IT_CC(TIM_Route)) != RESET)//����1���������¼�
//		{	
//			if(*TIMxCHy_CAPTURE_STA&0X40)		//����һ���½��� 		
//			{	  			
//				*TIMxCHy_CAPTURE_STA|=0X80;		//��ǳɹ�����һ��
//				*TIMxCHy_CAPTURE_VAL1=TIM_getCRRx(TIM_Route);
//	 			*TIMxCHy_CAPTURE_VAL2=TIM_getCRRx(filterTIM_Route_Partner(TIM_Route));
//			}else  								//��δ��ʼ,��һ�β���������
//			{
//				*TIMxCHy_CAPTURE_VAL1=0;
//				*TIMxCHy_CAPTURE_VAL2=0;
//				*TIMxCHy_CAPTURE_STA|=0X40;		//��ǲ�����������
//			}		    
//		}			     	    					   
// 	}
//	if((*TIMxCHy_CAPTURE_STA)&0X80)
//	{
//		if(currentCatcher->period!=-1000)
//	  {
//			currentCatcher->period=(((*TIMxCHy_CAPTURE_STA&0X3F)*65536)+(*TIMxCHy_CAPTURE_VAL1))*currentCatcher->resolution;
//			if(currentCatcher->doWhenDone)
//			{
//				currentCatcher->doWhenDone(currentCatcher);
//			}
//		}
//		else if(currentCatcher->doWhenTimeout)
//		{
//			currentCatcher->doWhenTimeout(currentCatcher);
//		}
//		*TIMxCHy_CAPTURE_STA=0;
//		*TIMxCHy_CAPTURE_VAL1=0;
//		*TIMxCHy_CAPTURE_VAL2=0;
//		currentCatcher->period=-1;
//	}
//  TIM_ClearITPendingBit(TIMCurrent, filterTIM_Input_IT_CC(TIM_Route)); //����жϱ�־λ
//}

void TIM1_CC_IRQHandler(void)
{ 
	IRQHANDLE(TIM1CHANNEL1);
//	IRQHANDLE_PWM(TIM1CHANNEL1);
}
//void TIM1_UP_IRQHandler(void)
//{
//	TIM1_CC_IRQHandler();
//	TIM_ClearITPendingBit(filterTIM_Input(TIM1CHANNEL1),TIM_IT_Update); //����жϱ�־λ
//}
//void TIM2_IRQHandler(void)
//{ 
//	IRQHANDLE(TIM2CHANNEL1); 
//	TIM_ClearITPendingBit(filterTIM_Input(TIM2CHANNEL1),TIM_IT_Update); //����жϱ�־λ
//}
//void TIM3_IRQHandler(void)
//{ 
//	IRQHANDLE(TIM3CHANNEL1); 
//	TIM_ClearITPendingBit(filterTIM_Input(TIM3CHANNEL1),TIM_IT_Update); //����жϱ�־λ
//}
//void TIM4_IRQHandler(void)
//{ 
//	IRQHANDLE(TIM4CHANNEL1); 
//	TIM_ClearITPendingBit(filterTIM_Input(TIM4CHANNEL1),TIM_IT_Update); //����жϱ�־λ
//}