#include "millis.h"
#include "delay.h"
/************/
//功能：返回运行时间ms

void PWR_BackupAccessCmd(FunctionalState NewState);
void(*defaultDoSomeThing)(void);
//LSE : 32.768KHZ
//占用TIM4（1ms中断）
//!millis 50天后失效
//millis()返回的值每1ms+1，由于使用的是RTC实际上是每0.992ms，无中断，故不会打断模拟I2C
void MILLIS_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);
	
	RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
	RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟  
	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
	RTC_WaitForSynchro();		//等待RTC寄存器同步  
	RTC_EnterConfigMode();/// 允许配置	
	RTC_SetPrescaler(62); //设置RTC预分频的值8MHz->992.06349206349206349206349206349us#
	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
	RTC_SetCounter(0);	
	RTC_WaitForLastTask();
	RTC->ALRH =0XFFFF;
	RTC_WaitForLastTask();
	RTC->ALRL = 0XFFFF;
	RTC_WaitForLastTask();
	RTC_ITConfig(RTC_IT_ALR, ENABLE);		//使能RTC中断
	RTC->CRL &=2;
	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
	RTC_ExitConfigMode(); //退出配置模式
// PWR_BackupAccessCmd(DISABLE);
//		RCC_LSEConfig(RCC_LSE_ON);	//设置外部低速晶振(LSE),使用外设低速晶振
//		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//检查指定的RCC标志位设置与否,等待低速晶振就绪
//			{
//			temp++;
//			delay_ms(10);
//			}
//		if(temp>=250)return 1; 

	NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;   //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;   //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

}
void doAfter_ms(u32 xm,void(*doSomeThing)(void))
{
	u32 AlarmValue=xm+RTC_CURRENT_VAL;
	u32 curr;
	RTC_WaitForLastTask();	
	RTC_EnterConfigMode();
/*******如果不对CNT赋值，则它会清零。原因未明**********/
/********UPDATE:真机上可去除*******************/
	curr=RTC_CURRENT_VAL;
	RTC->CNTL =curr&0xffff;
	RTC_WaitForLastTask();
	RTC->CNTH =curr>>16;
	RTC_WaitForLastTask();
/******上面几行仿真运行时间为0.61us********************/
	RTC->ALRH =AlarmValue >> 16;
	RTC_WaitForLastTask();	
	RTC->ALRL = (AlarmValue & 0xffff);
	RTC_WaitForLastTask();

	RTC_ExitConfigMode(); 
	defaultDoSomeThing=doSomeThing;
}
	
unsigned long millis()//1MS
{
	return RTC_CURRENT_VAL;
}
void RTCAlarm_IRQHandler(void)
{
	RTC_ClearITPendingBit(RTC_IT_ALR);	
}
void RTC_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_ALR)==SET)
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);	
		if(defaultDoSomeThing!=0)
			defaultDoSomeThing();
	}
}
