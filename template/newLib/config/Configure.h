#ifndef CONFIGURE_H
#define CONFIGURE_H
#include <stm32f10x.h>

struct USART_ConfBase
{
protected:
		USART_ConfBase(){}
public:
	uint32_t  RCC_USART;//=RCC_APB2Periph_USART1;
	uint32_t	RCC_GPIO;//=RCC_APB2Periph_GPIOA;
	GPIO_TypeDef *	GPIOx;
	USART_TypeDef * USARTx;
	uint16_t	PIN_TX;//=GPIO_Pin_9;
	uint16_t	PIN_RX;//=GPIO_Pin_10;
	GPIOSpeed_TypeDef	SPEED;//=GPIO_Speed_50MHz;
	uint16_t	USART_WordLength; //= USART_WordLength_8b;
	uint16_t	USART_StopBits;// = USART_StopBits_1;
	uint16_t	USART_Parity;// = USART_Parity_No;
	uint16_t	USART_HardwareFlowControl ;//= USART_HardwareFlowControl_None;
	uint16_t	USART_Mode;// = USART_Mode_Rx | USART_Mode_Tx;

	IRQn_Type USARTx_IRQChannel;

	void (*RCC_APBxPeriphClockCmd)(uint32_t RCC_APB1Periph, FunctionalState NewState);
	
	DMA_Channel_TypeDef *DMA_Channel;
	IRQn_Type DMA_IRQChannel;
	uint32_t	DMA_ITx;

	u8 id;
};
struct _USART1_Conf: public USART_ConfBase
{
	_USART1_Conf()
	{
		RCC_USART=RCC_APB2Periph_USART1;
		RCC_GPIO=RCC_APB2Periph_GPIOA;
		GPIOx=GPIOA;
		USARTx=USART1;
		PIN_TX=GPIO_Pin_9;
		PIN_RX=GPIO_Pin_10;
		SPEED=GPIO_Speed_50MHz;
		USART_WordLength = USART_WordLength_8b;
		USART_StopBits = USART_StopBits_1;
		USART_Parity = USART_Parity_No;
		USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		
		USARTx_IRQChannel=USART1_IRQn;
		
		RCC_APBxPeriphClockCmd=RCC_APB2PeriphClockCmd;
		
		DMA_Channel=DMA1_Channel5;
		DMA_IRQChannel=DMA1_Channel5_IRQn;
		DMA_ITx=DMA1_IT_TC5;
		
		id=1;
	}
};
struct _USART2_Conf:public USART_ConfBase
{
	_USART2_Conf()
	{
		RCC_USART=RCC_APB1Periph_USART2;
		RCC_GPIO=RCC_APB2Periph_GPIOA,
		GPIOx=GPIOA;
		USARTx=USART2;
		PIN_TX=GPIO_Pin_2;
		PIN_RX=GPIO_Pin_3;
		SPEED=GPIO_Speed_50MHz;
		USART_WordLength = USART_WordLength_8b;
		USART_StopBits = USART_StopBits_1;
		USART_Parity = USART_Parity_No;
		USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
				
		USARTx_IRQChannel=USART2_IRQn;
		
		RCC_APBxPeriphClockCmd=RCC_APB1PeriphClockCmd;
		
		DMA_Channel=DMA1_Channel6;
		DMA_IRQChannel=DMA1_Channel6_IRQn;
		DMA_ITx=DMA1_IT_TC6;
		
		id=2;
	}
};
struct TIMER_ChannelBase
{
	GPIO_TypeDef *	GPIOx;
	uint16_t 	GPIO_Pin;
	uint32_t	RCC_GPIO;
	void (*TIM_OCxInit)(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
	void (*TIM_SetComparex)(TIM_TypeDef* TIMx, uint16_t Compare);

	uint16_t TIM_IT_CCx;
	uint16_t TIM_Channelx;

};
struct TIMER_ConfBase
{
protected:
		TIMER_ConfBase(){}
public:
	uint32_t RCC_TIM;
	void (*RCC_APBxPeriphClockCmd)(uint32_t RCC_APB1Periph, FunctionalState NewState);
  uint16_t TIM_Prescaler; 
  uint16_t TIM_CounterMode;      
  uint16_t TIM_Period;           
  uint16_t TIM_ClockDivision;    
  uint8_t  TIM_RepetitionCounter; 
	TIM_TypeDef * TIMx;
	IRQn_Type TIMx_IRQn;
	TIMER_ChannelBase TIMER_Channels[4];
	u8 id;

};
struct _TIM4_Conf:public TIMER_ConfBase
{
	_TIM4_Conf()
	{
		RCC_TIM=RCC_APB1Periph_TIM4;
		RCC_APBxPeriphClockCmd=RCC_APB1PeriphClockCmd;
		TIM_Prescaler=71; 
		TIM_CounterMode=TIM_CounterMode_Up;      
		TIM_Period=999;
		TIM_ClockDivision=TIM_CKD_DIV1;    
		TIMx=TIM4;
		TIMx_IRQn=TIM4_IRQn;
		TIMER_ChannelBase _TIMER_Channels[]={
				{GPIOB,GPIO_Pin_6,RCC_APB2Periph_GPIOB,TIM_OC1Init,TIM_SetCompare1},
				{GPIOB,GPIO_Pin_7,RCC_APB2Periph_GPIOB,TIM_OC2Init,TIM_SetCompare2},
				{GPIOB,GPIO_Pin_8,RCC_APB2Periph_GPIOB,TIM_OC3Init,TIM_SetCompare3},
				{GPIOB,GPIO_Pin_9,RCC_APB2Periph_GPIOB,TIM_OC4Init,TIM_SetCompare4},
		};
		TIMER_Channels[0]=_TIMER_Channels[0];
		TIMER_Channels[1]=_TIMER_Channels[1];
		TIMER_Channels[2]=_TIMER_Channels[2];
		TIMER_Channels[3]=_TIMER_Channels[3];
		id=4;
	}
};
struct _TIM3_Conf:public TIMER_ConfBase
{
	_TIM3_Conf()
	{
		RCC_TIM=RCC_APB1Periph_TIM3;
		RCC_APBxPeriphClockCmd=RCC_APB1PeriphClockCmd;
		TIM_Prescaler=71;
		TIM_CounterMode=TIM_CounterMode_Up;
		TIM_Period=999;
		TIM_ClockDivision=TIM_CKD_DIV1;
		TIMx=TIM3;
		TIMx_IRQn=TIM3_IRQn;
		TIMER_ChannelBase _TIMER_Channels[]={
				{GPIOA,GPIO_Pin_6,RCC_APB2Periph_GPIOA,TIM_OC1Init,TIM_SetCompare1},
				{GPIOA,GPIO_Pin_7,RCC_APB2Periph_GPIOA,TIM_OC2Init,TIM_SetCompare2},
				{GPIOB,GPIO_Pin_0,RCC_APB2Periph_GPIOB,TIM_OC3Init,TIM_SetCompare3},
				{GPIOB,GPIO_Pin_1,RCC_APB2Periph_GPIOB,TIM_OC4Init,TIM_SetCompare4},
		};
		TIMER_Channels[0]=_TIMER_Channels[0];
		TIMER_Channels[1]=_TIMER_Channels[1];
		TIMER_Channels[2]=_TIMER_Channels[2];
		TIMER_Channels[3]=_TIMER_Channels[3];
		id=3;
	}
};
struct _TIM2_Conf:public TIMER_ConfBase
{
	_TIM2_Conf()
	{
		RCC_TIM=RCC_APB1Periph_TIM2;
		RCC_APBxPeriphClockCmd=RCC_APB1PeriphClockCmd;
		TIM_Prescaler=71;
		TIM_CounterMode=TIM_CounterMode_Up;
		TIM_Period=999;
		TIM_ClockDivision=TIM_CKD_DIV1;
		TIMx=TIM2;
		TIMx_IRQn=TIM2_IRQn;
		TIMER_ChannelBase _TIMER_Channels[]={
				{GPIOA,GPIO_Pin_0,RCC_APB2Periph_GPIOA,TIM_OC1Init,TIM_SetCompare1},
				{GPIOA,GPIO_Pin_1,RCC_APB2Periph_GPIOA,TIM_OC2Init,TIM_SetCompare2},
				{GPIOA,GPIO_Pin_2,RCC_APB2Periph_GPIOA,TIM_OC3Init,TIM_SetCompare3},
				{GPIOA,GPIO_Pin_3,RCC_APB2Periph_GPIOA,TIM_OC4Init,TIM_SetCompare4},
		};
		TIMER_Channels[0]=_TIMER_Channels[0];
		TIMER_Channels[1]=_TIMER_Channels[1];
		TIMER_Channels[2]=_TIMER_Channels[2];
		TIMER_Channels[3]=_TIMER_Channels[3];
		id=2;
	}
};
struct _TIM1_Conf:public TIMER_ConfBase
{
	_TIM1_Conf()
	{
		RCC_TIM=RCC_APB2Periph_TIM1;
		RCC_APBxPeriphClockCmd=RCC_APB2PeriphClockCmd;
		TIM_Prescaler=71;
		TIM_CounterMode=TIM_CounterMode_Up;
		TIM_Period=999;
		TIM_ClockDivision=TIM_CKD_DIV1;
		TIMx=TIM1;
		TIMx_IRQn=TIM1_UP_IRQn;
		TIMER_ChannelBase _TIMER_Channels[]={
				{GPIOA,GPIO_Pin_8,RCC_APB2Periph_GPIOA,TIM_OC1Init,TIM_SetCompare1},
				{GPIOA,GPIO_Pin_9,RCC_APB2Periph_GPIOA,TIM_OC2Init,TIM_SetCompare2},
				{GPIOA,GPIO_Pin_10,RCC_APB2Periph_GPIOA,TIM_OC3Init,TIM_SetCompare3},
				{GPIOA,GPIO_Pin_11,RCC_APB2Periph_GPIOA,TIM_OC4Init,TIM_SetCompare4},
		};
		TIMER_Channels[0]=_TIMER_Channels[0];
		TIMER_Channels[1]=_TIMER_Channels[1];
		TIMER_Channels[2]=_TIMER_Channels[2];
		TIMER_Channels[3]=_TIMER_Channels[3];
		id=1;
	}
};

extern _USART1_Conf USART1_Conf;
extern _USART2_Conf USART2_Conf;
extern _TIM4_Conf 	TIM4_Conf;
extern _TIM3_Conf 	TIM3_Conf;
extern _TIM2_Conf 	TIM2_Conf;
extern _TIM1_Conf 	TIM1_Conf;
#endif
