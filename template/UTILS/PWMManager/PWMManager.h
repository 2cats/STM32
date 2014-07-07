#ifndef _MOTOR_H_
#define _MOTOR_H_
#define PWMManagerNum 16
#include "stm32f10x.h"
#ifdef __cplusplus
 extern "C" {
#endif


/************精度调节**************/
#define TIM_FREQ_DIV1	72
#define TIM_FREQ_DIV2	72
#define TIM_FREQ_DIV3	72
#define TIM_FREQ_DIV4	72
//1,2,3,4,5.....
//典型值为1或72,越小精度越高但范围缩小
//原理：
//TIM_TimeBaseStructure.TIM_Prescaler =TIM_FREQ_DIV-1;
//TIM_TimeBaseStructure.TIM_Period =(u16)(TIM_REAL_FREQ/PWMProducer->frequency);//ARR寄存器
//TIM_OCInitStructure.TIM_Pulse = TIM_REAL_FREQ/PWMProducer->frequency*PWMProducer->ratio;





#define TIM_REAL_FREQ1 (72000000.0/TIM_FREQ_DIV1)
#define TIM_REAL_FREQ2 (72000000.0/TIM_FREQ_DIV2)
#define TIM_REAL_FREQ3 (72000000.0/TIM_FREQ_DIV3)
#define TIM_REAL_FREQ4 (72000000.0/TIM_FREQ_DIV4)
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
typedef struct PWMProducerType{
	float frequency;
	float ratio;
	TIM_RouteTypeDef route;
}PWMProducerTypeDef;

typedef struct PWMManagerType{
	PWMProducerTypeDef PWMProducer[16];
	u8 num;
	void(*ProduceNewPWM)(TIM_RouteTypeDef route,float frequency,float ratio);
	void(*changeRatio)(TIM_RouteTypeDef,float ratio);
	void(*changeFrequency)(TIM_RouteTypeDef,float frequency);
}PWMManagerTypeDef;

void PWMStart(PWMProducerTypeDef*PWMProducer);
void PWM_chageRatio(PWMProducerTypeDef* PWMProducer, float ratio);

void PWMManagerStructure(PWMManagerTypeDef *PWMManager);
void PWMManager_ProduceNewPWM(TIM_RouteTypeDef route,float frequency,float ratio);
void PWMManager_changeRatio(TIM_RouteTypeDef route,float ratio);
void PWMManager_changeFrequency(TIM_RouteTypeDef route,float frequency);
#ifdef __cplusplus
}
#endif

#endif
