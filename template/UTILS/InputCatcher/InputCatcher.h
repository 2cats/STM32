#ifndef _INPUTCATCHER_H_
#define _INPUTCATCHER_H_

#include "stm32f10x.h"

/***********************************/
//defines
//#define CATCH_CONTINUOUS 1
//#define CATCH_ONCE 0

#define GETFLAG(x)   ((catchWhat_Flag>>x)&1)
#define SETFLAG(x)   (catchWhat_Flag|=(1<<x))
#define RESETFLAG(x) (catchWhat_Flag&=~(1<<x))

#define ISROUTEENABLED(x)  ((catchWhat_Flag>>x)&1)
#define ENABLEROUTE(x)   (catchWhat_Flag|=(1<<x))
#define DISENABLEROUTE(x) (catchWhat_Flag&=~(1<<x))

#define IRQHANDLE(x) 	if(ISROUTEENABLED(x)){IRQ_HIGH_LOW(x);}\
if(ISROUTEENABLED((x+1))){IRQ_HIGH_LOW((TIM_RouteTypeDef)(x+1));}\
if(ISROUTEENABLED((x+2))){IRQ_HIGH_LOW((TIM_RouteTypeDef)(x+2));}\
if(ISROUTEENABLED((x+3))){IRQ_HIGH_LOW((TIM_RouteTypeDef)(x+3));}
/***********************************/
//structDefs
#ifndef _TIM_ROUTE_TYPE_
#define _TIM_ROUTE_TYPE_
typedef enum{
TIM1CHANNEL1,//PA8
TIM1CHANNEL2,//PA9 
TIM1CHANNEL3,//PA10
TIM1CHANNEL4,//PA11
	
TIM2CHANNEL1,//PA0
TIM2CHANNEL2,//PA1
TIM2CHANNEL3,//PA2
TIM2CHANNEL4,//PA3
	
TIM3CHANNEL1,//PA6
TIM3CHANNEL2,//PA7
TIM3CHANNEL3,//PB0
TIM3CHANNEL4,//PB1
	
TIM4CHANNEL1,//PB6
TIM4CHANNEL2,//PB7
TIM4CHANNEL3,//PB8
TIM4CHANNEL4,//PB9
}TIM_RouteTypeDef;
#endif
typedef enum {
HIGH_DURATION,LOW_DURATION,HIGH_LOW_HIGH_DURATION,LOW_HIGH_LOW_DURATION,
}CatchWhatTypeDef;
typedef struct InputCatcherType
{
	TIM_RouteTypeDef route;
	double resolution;
	void (*doWhenDone)(struct InputCatcherType*);
//	void (*doWhenRatioChanged)(struct InputCatcherType*);
	void (*doWhenTimeout)(struct InputCatcherType*);
	CatchWhatTypeDef catchWhat;
	double duration;
	double ratio;
	double duty; 
//	double lastDuration;
//	double lastRatio;
//	double lastDuty; 
//	double ratio_offet;
}InputCatcherTypeDef;
typedef struct InputCatchManagerType
{
	InputCatcherTypeDef InputCatcher[16];
	void (*startCatching)(TIM_RouteTypeDef Route,CatchWhatTypeDef catchWhat,double resolution);
	void (*setDoWhenDone)(TIM_RouteTypeDef Route,	void (*func)(struct InputCatcherType*));
//void (*setDoWhenRatioChanged)(TIM_RouteTypeDef Route,	double offset,void (*func)(struct InputCatcherType*));
	void (*setDoWhenTimeout)(TIM_RouteTypeDef Route,	void (*func)(struct InputCatcherType*));
	//u8  (*isCatchingDone)(TIM_RouteTypeDef Route);
	//double (*getduration)(TIM_RouteTypeDef Route);
	u8 _num;
}InputCatchManagerTypeDef; 

/**********************************/
#ifndef _TIM_FILTER_TYPE_
#define _TIM_FILTER_TYPE_
//filters
uint32_t  filterGPIO_Input_RCC_Input( TIM_RouteTypeDef TIM_Route);
GPIO_TypeDef * filterGPIO_Input( TIM_RouteTypeDef TIM_Route);
uint16_t  filterPin_Input( TIM_RouteTypeDef TIM_Route);

TIM_TypeDef*  filterTIM_Input( TIM_RouteTypeDef TIM_Route);
uint32_t  filterTIM_Input_RCC_Input( TIM_RouteTypeDef TIM_Route);
uint32_t  filterTIM_Input_IRQn( TIM_RouteTypeDef TIM_Route);
uint32_t  filterTIM_Input_IT_CC( TIM_RouteTypeDef TIM_Route);

uint16_t filterTIM_InputChannel( TIM_RouteTypeDef TIM_Route);

uint16_t filterPolarity( CatchWhatTypeDef CatchWhat);
InputCatcherTypeDef* filterInputCatcher(TIM_RouteTypeDef TIM_Route);
void TIM_OCxPolarityConfig( TIM_RouteTypeDef Route, uint16_t TIM_OCPolarity);
uint16_t TIM_getCRRx( TIM_RouteTypeDef Route);
#endif
/**********************************/
//InputCatchManager Funcs
void InputCatchManager_startCatching(TIM_RouteTypeDef route,CatchWhatTypeDef catchWhat,double resolution);
u8 InputCatchManager_isCatchingDone(TIM_RouteTypeDef Route);
void InputCatchManager_setDoWhenDone(TIM_RouteTypeDef Route ,	void (*func)(struct InputCatcherType*));
void InputCatchManager_setDoWhenTimeout(TIM_RouteTypeDef Route ,	void (*func)(struct InputCatcherType*));
void InputCatchManagerStructure(InputCatchManagerTypeDef *InputCatchManager);
double InputCatchManager_getduration(TIM_RouteTypeDef Route);
/**********************************/
//IRQs
void IRQ_HIGH_LOW(TIM_RouteTypeDef TIM_Route);
void TIM1_CC_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
#endif
