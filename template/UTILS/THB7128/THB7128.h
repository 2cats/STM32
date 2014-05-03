#ifndef _THB7128_H_
#define _THB7128_H_
#include "stm32f10x.h"

//Pin & Timer Using

#define THB7128_PORT      GPIOA
#define THB7128_CLK_Pin   GPIO_Pin_1
#define THB7128_CWW_Pin   GPIO_Pin_2
#define THB7128_EN_Pin    GPIO_Pin_3
#define TIMX              TIM2
//Ĭ��ʹ��ͨ��2
//Config

//�˴α�ʾTHB7128_EN_PinΪ�͵�ƽʱʹ��
#define THB7128_ENABLE_VALUE					0
//�˴α�ʾTHB7128_CWW_PinΪ�ߵ�ƽʱ˳ʱ��ת
#define THB7128_CLOCKWISE							1
#define THB7128_ANTICLOCKWISE					(!THB7128_CLOCKWISE)

//ϸ������ϸ����Ϊ1ʱ��һ������ת1.8��
#define THB7128_DIVITION		128
//һ������ת���ٶ�
#define THB7128_DEGREE_PER_PULSE			(1.8/THB7128_DIVITION)
//ת1Ȧ��Ҫ���ٸ�����
#define THB7128_STEPS_PER_CIRCLE			(360/THB7128_DEGREE_PER_PULSE)		


void THB7128_Init(u8 direction,u32 freq);
void THB7128_ChangeFreq(u32 freq);
void THB7128_TIM_Init(unsigned short int Prescaler, unsigned short int Period);
void THB7128_ClockWise(void);
void THB7128_AntiClockWise(void);
void THB7128_Enable(void);
void THB7128_Disable(void);
void THB7128_PWM_Out(u32 count);
u8   THB7128_IsLastDone(void);
void THB7128_SetDoWhenDone(void(*)(void));
void THB7128_NVIC_Configuration(void);

#endif
