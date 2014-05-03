#include "millis.h"
#include "delay.h"
void PWR_BackupAccessCmd(FunctionalState NewState);
void(*defaultDoSomeThing)(void);
//LSE : 32.768KHZ
//ռ��TIM4��1ms�жϣ�
//!millis 50���ʧЧ
//millis()���ص�ֵÿ1ms+1������ʹ�õ���RTCʵ������ÿ0.992ms�����жϣ��ʲ�����ģ��I2C
void MILLIS_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	PWR_BackupAccessCmd(ENABLE);
	
	RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);		//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
	RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ��  
	RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	RTC_WaitForSynchro();		//�ȴ�RTC�Ĵ���ͬ��  
	RTC_EnterConfigMode();/// ��������	
	RTC_SetPrescaler(62); //����RTCԤ��Ƶ��ֵ8MHz->992.06349206349206349206349206349us#
	RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	RTC_SetCounter(0);	
	RTC_WaitForLastTask();
	RTC->ALRH =0XFFFF;
	RTC_WaitForLastTask();
	RTC->ALRL = 0XFFFF;
	RTC_WaitForLastTask();
	RTC_ITConfig(RTC_IT_ALR, ENABLE);		//ʹ��RTC�ж�
	RTC->CRL &=2;
	RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	RTC_ExitConfigMode(); //�˳�����ģʽ
// PWR_BackupAccessCmd(DISABLE);
//		RCC_LSEConfig(RCC_LSE_ON);	//�����ⲿ���پ���(LSE),ʹ��������پ���
//		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//���ָ����RCC��־λ�������,�ȴ����پ������
//			{
//			temp++;
//			delay_ms(10);
//			}
//		if(temp>=250)return 1; 

	NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;   //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;   //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

}
void doAfter_ms(u32 xm,void(*doSomeThing)(void))
{
	u32 AlarmValue=xm+RTC_CURRENT_VAL;
	u32 curr;
	RTC_WaitForLastTask();	
	RTC_EnterConfigMode();
/*******�������CNT��ֵ�����������㡣ԭ��δ��**********/
/********UPDATE:����Ͽ�ȥ��*******************/
	curr=RTC_CURRENT_VAL;
	RTC->CNTL =curr&0xffff;
	RTC_WaitForLastTask();
	RTC->CNTH =curr>>16;
	RTC_WaitForLastTask();
/******���漸�з�������ʱ��Ϊ0.61us********************/
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
