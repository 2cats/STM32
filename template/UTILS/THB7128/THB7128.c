#include "THB7128.h"

/********INTERFACE*********/
/*
频率理想极限：接近1M
Example:
//以3KHz的PWM频率驱动电机转1圈
	THB7128_Init(THB7128_CLOCKWISE,3000);
	THB7128_PWM_Out(THB7128_STEPS_PER_CIRCLE);
//THB7128_SetDoWhenDone(doWhenDone);//本次操作完成的回调函数，在定时器中断中执行

实用函数：
	THB7128_ChangeFreq：											更改频率
	THB7128_IsLastDone：											判断上一次操作是否完成
	THB7128_ClockWise/THB7128_AntiClockWise：	设置旋转方向
	THB7128_Enable/THB7128_Disable：					使能/失能
*/

/*
本次硬件说明：
 -----------------------
|CP- CM-  EN-  全部接地 |
 -----------------------
CP+    一个高电平脉冲转动一个单位

CM+ H				顺时针
CM+ L/悬空	逆时针

EN+ H				失能
EN+ L/悬空	使能

不同器件/连接需更改头文件相关定义
*/



//中断和InputCatcher冲突
u32 thb7128_pwm_count;
void(*thb7128_defaultDoWhenDone)(void)=0;
void THB7128_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /*CLK*/
    GPIO_InitStructure.GPIO_Pin = THB7128_CLK_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(THB7128_PORT, &GPIO_InitStructure);

    /*EN CWW*/
    GPIO_InitStructure.GPIO_Pin = THB7128_CWW_Pin | THB7128_EN_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(THB7128_PORT, &GPIO_InitStructure);
}

void THB7128_TIM_Init(unsigned short int Prescaler,unsigned short int Period)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    TIM_TimeBaseStructure.TIM_Period = Period;
    TIM_TimeBaseStructure.TIM_Prescaler = Prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIMX, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIMX, TIM_FLAG_Update);
    TIM_ITConfig(TIMX, TIM_IT_Update, ENABLE);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = TIM_TimeBaseStructure.TIM_Period / 2;//0.5占空比
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC2Init(TIMX, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIMX, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIMX, ENABLE);
}
void THB7128_RCC_Init(void)
{
	RCC_APB2PeriphClockCmd(
            RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC
                    | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(
            RCC_APB1Periph_TIM4 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM2,
            ENABLE);
}
void THB7128_Init(u8 direction,u32 freq)
{
		THB7128_RCC_Init();
    THB7128_GPIO_Init();
		if(direction)
			THB7128_ClockWise();
		else
			THB7128_AntiClockWise();
		THB7128_Enable();
    THB7128_TIM_Init(71,1e6/freq);
		THB7128_NVIC_Configuration();	
}
void THB7128_PWM_Out(u32 count)
{
		thb7128_pwm_count=count;
    TIM_Cmd(TIMX, ENABLE);
}

void THB7128_ClockWise(void)
{
#if THB7128_CLOCKWISE
   GPIO_SetBits(THB7128_PORT, THB7128_CWW_Pin);       
#else
	 GPIO_ResetBits(THB7128_PORT, THB7128_CWW_Pin);
#endif

}

void THB7128_AntiClockWise(void)
{
#if THB7128_CLOCKWISE
   GPIO_ResetBits(THB7128_PORT, THB7128_CWW_Pin);       
#else
	 GPIO_SetBits(THB7128_PORT, THB7128_CWW_Pin);
#endif

}

void THB7128_Enable(void)
{
#if THB7128_ENABLE_VALUE
   GPIO_SetBits(THB7128_PORT, THB7128_EN_Pin);       
#else
	 GPIO_ResetBits(THB7128_PORT, THB7128_EN_Pin);
#endif

}
void THB7128_Disable(void)
{
#if THB7128_ENABLE_VALUE
	GPIO_ResetBits(THB7128_PORT, THB7128_EN_Pin);    
#else
	GPIO_SetBits(THB7128_PORT, THB7128_EN_Pin);   
#endif

}
void THB7128_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
u8 THB7128_IsLastDone(void)
{
	return thb7128_pwm_count==0;
}
void THB7128_SetDoWhenDone(void(*doWhenDone)(void))
{
	thb7128_defaultDoWhenDone=doWhenDone;
}
void THB7128_ChangeFreq(u32 freq)
{
	TIMX->ARR=1e6/freq;
}

//void TIM2_IRQHandler(void)
//{

//    if (TIM_GetITStatus(TIMX, TIM_IT_Update) != RESET)
//    {
//        TIM_ClearITPendingBit(TIMX, TIM_IT_Update);
//        thb7128_pwm_count--;
//        if (thb7128_pwm_count == 0)
//        {
//            TIM_Cmd(TIMX, DISABLE);
//						if(thb7128_defaultDoWhenDone!=0)
//							thb7128_defaultDoWhenDone();
//        }
//    }
//}

