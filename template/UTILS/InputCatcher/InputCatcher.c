#include "InputCatcher.h"
#include "stm32f10x.h"

//
/*********！WARNING！*********/
/*

【使用注意】：
	
	不能和PWMManager共用同一个定时器
	
	同一个定时器各个通道的分辨率必须相同

	回调函数的耗时尽量短，串口发送等耗时长的操作可能会产生极其严重的错误
	
	同时使用一个TIM的多个通道时，各个通道的"分辨率"必须相同

	!定时器中断默认已给出（在本文件末尾），在工程中避免中断重复或未定义

【极限值】：

	建议频率范围：20Hz--100KHz	
	
	溢出时间=63*65536*分辨率（分辨率为1us时，溢出时间约为4s）
	注：分辨率就是捕获时间的最小区分单位，设置分辨率时，尽量能被72整除，极限分辨率为 1/72.0 us

【单位】：

	全部时间单位：us

	
*/
/*********！INTERFACE！*********/
/*
//Example：
		InputCatchManagerTypeDef InputCatchManager;
		InputCatchManagerStructure(&InputCatchManager);
		InputCatchManager.startCatching(TIM3CHANNEL2,HIGH_DURATION,1);//TIM3CHANNEL2捕获高电平时间，分辨率1us，持续捕获
		InputCatchManager.setDoWhenDone(TIM3CHANNEL2,doSomeThing);//optional
		InputCatchManager.setDoWhenTimeout(TIM3CHANNEL2,doSomeThingWhenTimeout);//optional
	
	CatchType(功能):
		16路输入捕获
		捕获类型：
		HIGH_DURATION:测高电平时间
		LOW_DURATION:测低电平时间
		HIGH_LOW_HIGH_DURATION:测PWM的信息,周期duration，高电平时间duty,占空比duty/duration,首次触发为高电平
		LOW_HIGH_LOW_DURATION：测PWM的信息,周期duration，低电平时间duty,占空比duty/duration（=1-HIGH_LOW_HIGH_DURATION）首次触发为低电平
	Q&A:
	1.何时可能发生溢出事件?
		当“满足触发条件”但“未完成本次捕获”时，信号长度过长将产生溢出事件	（例如HIGH_DURATION的触发条件就是上升沿）
		
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
	
	RCC_APB2PeriphClockCmd(filterGPIO_Input_RCC_Input(route), ENABLE);//###  //使能GPIO外设和AFIO复用功能模块时钟使能
	GPIO_InitStructure.GPIO_Pin = filterPin_Input(currentCatcher->route);
	if(currentCatcher->catchWhat==HIGH_DURATION|currentCatcher->catchWhat==HIGH_LOW_HIGH_DURATION)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	else
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(filterGPIO_Input(currentCatcher->route), &GPIO_InitStructure);//###
	
	//初始化定时器5 TIM2	 
	TIM_TimeBaseStructure.TIM_Period = 0xffff; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =(u32)(72*resolution-1); 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(filterTIM_Input(route), &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	//初始化TIM2输入捕获参数
 
	TIM_ICInitStructure.TIM_Channel = filterTIM_InputChannel(route);
	TIM_ICInitStructure.TIM_ICPolarity = filterPolarity(currentCatcher->catchWhat);;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
	TIM_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(filterTIM_Input(route), &TIM_ICInitStructure);

	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = filterTIM_Input_IRQn(route);  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	if(filterTIM_Input(route)==TIM1)
	{
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	}

	TIM_ITConfig(filterTIM_Input(route),TIM_IT_Update|filterTIM_Input_IT_CC(route),ENABLE);

	TIM_Cmd(filterTIM_Input(route),ENABLE ); 
	TIM_ClearITPendingBit(filterTIM_Input(route),TIM_IT_Update); //清除中断标志位

}

void InputCatchManager_setDoWhenDone(TIM_RouteTypeDef Route ,	void (*func)(struct InputCatcherType*))
{
	filterInputCatcher(Route)->doWhenDone=func;
}

void InputCatchManager_setDoWhenTimeout(TIM_RouteTypeDef Route ,	void (*func)(struct InputCatcherType*))
{
	filterInputCatcher(Route)->doWhenTimeout=func;
}
void InputCatchManagerStructure(InputCatchManagerTypeDef *InputCatchManager)
{
	defaultInputCatchManager=InputCatchManager;
	defaultInputCatchManager->startCatching=InputCatchManager_startCatching;
	defaultInputCatchManager->_num=0;
	defaultInputCatchManager->setDoWhenDone=InputCatchManager_setDoWhenDone;
	defaultInputCatchManager->setDoWhenTimeout=InputCatchManager_setDoWhenTimeout;
}
u8 InputCatchManager_isCatchingDone(TIM_RouteTypeDef Route)
{
	return TIM_CAPTURE_STA_VAL1_VAL2_VAL3[Route][0]&0x80 ;
}
double InputCatchManager_getduration(TIM_RouteTypeDef Route)
{
	return filterInputCatcher(Route)->duration;
}

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
	if((*TIMxCHy_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{	  
		if (TIM_GetITStatus(TIMCurrent, TIM_IT_Update) != RESET)
		{	    
			if((*TIMxCHy_CAPTURE_STA&0X40) | (*TIMxCHy_CAPTURE_STA&0X100))//已经捕获2or3
			{
				if((*TIMxCHy_CAPTURE_STA&CATCHING_FLOW_SIZE)==CATCHING_FLOW_SIZE)//高电平太长了
				{
					*TIMxCHy_CAPTURE_STA|=0X80;//结束此次捕获
					//*TIMxCHy_CAPTURE_VAL=0XFFFF;
					currentCatcher->duration=-1000;
					
				}else 
				(*TIMxCHy_CAPTURE_STA)++;
			}	 
		}
	if (TIM_GetITStatus(TIMCurrent, filterTIM_Input_IT_CC(TIM_Route)) != RESET)//捕获1发生捕获事件
		{	
			if(*TIMxCHy_CAPTURE_STA&0X40)		//捕获到2	
			{	  		
					if(currentCatcher->catchWhat==HIGH_LOW_HIGH_DURATION|currentCatcher->catchWhat==LOW_HIGH_LOW_DURATION)
					{
						*TIMxCHy_CAPTURE_STA&=~0x40;
						*TIMxCHy_CAPTURE_STA|=0x100;
						*TIMxCHy_CAPTURE_VAL2=TIM_getCRRx(TIM_Route)+(*TIMxCHy_CAPTURE_STA&CATCHING_FLOW_SIZE)*65536;
					}else
					{
						*TIMxCHy_CAPTURE_STA|=0x80;

						*TIMxCHy_CAPTURE_VAL2=	TIM_getCRRx(TIM_Route)+(*TIMxCHy_CAPTURE_STA&CATCHING_FLOW_SIZE)*65536;
						currentCatcher->duration=(*TIMxCHy_CAPTURE_VAL2-*TIMxCHy_CAPTURE_VAL1)*currentCatcher->resolution;
						if(currentCatcher->doWhenDone)
						{
							currentCatcher->doWhenDone(currentCatcher);
						}
						*TIMxCHy_CAPTURE_STA=0;
					}
					TIM_OCxPolarityConfig(TIM_Route,TIM_ICPolarity[0]);	
						
			}else  if(*TIMxCHy_CAPTURE_STA&0X100)//捕获到3
			{
			 	uint32_t CCRx=TIM_getCRRx(TIM_Route);
				*TIMxCHy_CAPTURE_VAL3=(u32)(CCRx+(*TIMxCHy_CAPTURE_STA&CATCHING_FLOW_SIZE)*65536);
				TIM_OCxPolarityConfig(TIM_Route,TIM_ICPolarity[1]);	
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
				*TIMxCHy_CAPTURE_STA|=0X40;		//标记捕获到1
		   	TIM_OCxPolarityConfig(TIM_Route,TIM_ICPolarity[1]);		//CC1P=1 设置为下降沿捕获
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
  TIM_ClearITPendingBit(TIMCurrent, filterTIM_Input_IT_CC(TIM_Route)); //清除中断标志位
}


void TIM1_CC_IRQHandler(void)
{ 
	IRQHANDLE(TIM1CHANNEL1);
//	IRQHANDLE_PWM(TIM1CHANNEL1);
}
void TIM1_UP_IRQHandler(void)
{
	TIM1_CC_IRQHandler();
	TIM_ClearITPendingBit(filterTIM_Input(TIM1CHANNEL1),TIM_IT_Update); //清除中断标志位
}
//void TIM2_IRQHandler(void)
//{ 
//	IRQHANDLE(TIM2CHANNEL1); 
//	TIM_ClearITPendingBit(filterTIM_Input(TIM2CHANNEL1),TIM_IT_Update); //清除中断标志位
//}
void TIM3_IRQHandler(void)
{ 
	IRQHANDLE(TIM3CHANNEL1); 
	TIM_ClearITPendingBit(filterTIM_Input(TIM3CHANNEL1),TIM_IT_Update); //清除中断标志位
}
void TIM4_IRQHandler(void)
{ 
	IRQHANDLE(TIM4CHANNEL1); 
	TIM_ClearITPendingBit(filterTIM_Input(TIM4CHANNEL1),TIM_IT_Update); //清除中断标志位
}