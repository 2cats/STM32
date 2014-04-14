#include "stm32f10x.h"
#include "EXTIManager.h" 
/******INTERFACE********/
/*
//ALL GPIO
Example:
void doWhenTrigger( struct EXTILineType *triggeredLine)
{
	printf("GPIO:%x\tLine:0x%04x triggered\n",(u32)triggeredLine->GPIO, triggeredLine->GPIO_Pin);
}
	EXTIManagerTypeDef EXTIManager;
	EXTIManager_Structure(&EXTIManager);
	EXTIManager.startListenling(GPIOA,GPIO_Pin_5,EXTI_Both_DEFAULT_UP,doWhenTrigger);
	EXTIManager.startListenling(GPIOA,GPIO_Pin_1,EXTI_Both_DEFAULT_DOWN,doWhenTrigger);

*/
/*********！WARNING！*********/
/*

	GPIO_Pin_x即为通道号，不可重复startListenling（比如，可以同时监听GPIOA的0-15引脚 ，但不可同时监听GPIOA和GPIOB的引脚x）
	
*/


//variables
/***********************/
EXTIManagerTypeDef *defaultEXTIManager; 
EXTILineTypeDef *_EXTILine[16];

//filters
/***********************/
void setExactLine(EXTILineTypeDef * Line)
{
	u8 i=0;
	uint16_t GPIO_Pin=Line->GPIO_Pin;
	while(!(GPIO_Pin&1))
	{
		GPIO_Pin>>=1;
		i++;
	}
	_EXTILine[i]=Line;
}

uint8_t filterGPIO_PortSourceGPIOC(GPIO_TypeDef *GPIOx)
{
	switch((uint32_t)GPIOx)
	{
		case (uint32_t)GPIOA:return GPIO_PortSourceGPIOA;
		case (uint32_t)GPIOB:return GPIO_PortSourceGPIOB;
		case (uint32_t)GPIOC:return GPIO_PortSourceGPIOC;
		case (uint32_t)GPIOD:return GPIO_PortSourceGPIOD;
		case (uint32_t)GPIOE:return GPIO_PortSourceGPIOE;
		case (uint32_t)GPIOF:return GPIO_PortSourceGPIOF;
		case (uint32_t)GPIOG:return GPIO_PortSourceGPIOG;
	}
	return 0;
}
GPIOMode_TypeDef filterGPIO_Mode(MyEXTITrigger_TypeDef triger)
{
	switch((u8)triger)
	{
		case (u8)EXTI_Rising:return GPIO_Mode_IPD;
		case (u8)EXTI_Falling:return GPIO_Mode_IPU;
		case (u8)EXTI_Both_DEFAULT_UP:return GPIO_Mode_IPU;
		case (u8)EXTI_Both_DEFAULT_DOWN:return GPIO_Mode_IPD;
	}
	return (GPIOMode_TypeDef)0;
}
uint32_t filterGPIO_RCC_by_GPIO(GPIO_TypeDef *GPIOx)
{
	switch((uint32_t)GPIOx)
	{
		case (uint32_t)GPIOA:return RCC_APB2Periph_GPIOA;
		case (uint32_t)GPIOB:return RCC_APB2Periph_GPIOB;
		case (uint32_t)GPIOC:return RCC_APB2Periph_GPIOC;
		case (uint32_t)GPIOD:return RCC_APB2Periph_GPIOD;
		case (uint32_t)GPIOE:return RCC_APB2Periph_GPIOE;
		case (uint32_t)GPIOF:return RCC_APB2Periph_GPIOF;
		case (uint32_t)GPIOG:return RCC_APB2Periph_GPIOG;
	}
	return 0;
}

EXTITrigger_TypeDef filterTrigger_Type(MyEXTITrigger_TypeDef triger)
{
	switch((u8)triger)
	{
		case (u8)EXTI_Rising:return EXTI_Trigger_Rising;
		case (u8)EXTI_Falling:return EXTI_Trigger_Falling;
		case (u8)EXTI_Both_DEFAULT_UP:return EXTI_Trigger_Rising_Falling;
		case (u8)EXTI_Both_DEFAULT_DOWN:return EXTI_Trigger_Rising_Falling;
	}
	return (EXTITrigger_TypeDef)0;
}

uint8_t filterPinSource(	uint16_t GPIO_Pin)
{
	u8 i=0;
	while(!(GPIO_Pin&1))
	{
		GPIO_Pin>>=1;
		i++;
	}
	return i;
}
IRQn_Type filterEXTI_IRQn(uint16_t GPIO_Pin)
{
	if(GPIO_Pin<=0x0010)
		switch(GPIO_Pin)
		{
			case GPIO_Pin_0:return EXTI0_IRQn;
			case GPIO_Pin_1:return EXTI1_IRQn;
			case GPIO_Pin_2:return EXTI2_IRQn;
			case GPIO_Pin_3:return EXTI3_IRQn;
			case GPIO_Pin_4:return EXTI4_IRQn;
		}
	else if(GPIO_Pin<=0x0200)
		return EXTI9_5_IRQn;
	else 
		return EXTI15_10_IRQn;
	return (IRQn_Type)0;
}

//Funcs
/***********************/
void EXTIManager_StartListenling(GPIO_TypeDef *GPIO,uint16_t GPIO_Pin,MyEXTITrigger_TypeDef triger,void (*doWhenTrigger)( struct EXTILineType *triggeredLine))
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	defaultEXTIManager->EXTILine[defaultEXTIManager->_num].GPIO=GPIO;
	defaultEXTIManager->EXTILine[defaultEXTIManager->_num].GPIO_Pin=GPIO_Pin;
	defaultEXTIManager->EXTILine[defaultEXTIManager->_num].triger=triger;
	defaultEXTIManager->EXTILine[defaultEXTIManager->_num].doWhenTrigger=doWhenTrigger;

	setExactLine(&defaultEXTIManager->EXTILine[defaultEXTIManager->_num]);
	defaultEXTIManager->_num++;

	RCC_APB2PeriphClockCmd(filterGPIO_RCC_by_GPIO(GPIO), ENABLE);//###
	GPIO_InitStructure.GPIO_Mode=filterGPIO_Mode(triger);
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin;
	GPIO_Init(GPIO,&GPIO_InitStructure);
	
  GPIO_EXTILineConfig(filterGPIO_PortSourceGPIOC(GPIO),filterPinSource(GPIO_Pin));

	EXTI_InitStructure.EXTI_Line=GPIO_Pin;	//KEY2
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = filterTrigger_Type(triger);
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


	NVIC_InitStructure.NVIC_IRQChannel = filterEXTI_IRQn(GPIO_Pin);			//使能按键WK_UP所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 
}

void EXTIManager_Structure(EXTIManagerTypeDef *EXTIManager)
{
	defaultEXTIManager=EXTIManager;
	EXTIManager->startListenling=EXTIManager_StartListenling;
	EXTIManager->_num=0;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
}
EXTI_IQRNS(0)
EXTI_IQRNS(1)
EXTI_IQRNS(2)
EXTI_IQRNS(3)
EXTI_IQRNS(4)
void EXTI9_5_IRQHandler(void)
{
	u32 status=EXTI->PR&0x3E0;
	EXTI_IQRNS_SPEC(5)
	EXTI_IQRNS_SPEC(6)
	EXTI_IQRNS_SPEC(7)
	EXTI_IQRNS_SPEC(8)
	EXTI_IQRNS_SPEC(9)	
}
void EXTI15_10_IRQHandler(void)
{
	u32 status=EXTI->PR&0xFC00;
	EXTI_IQRNS_SPEC(10)
	EXTI_IQRNS_SPEC(11)
	EXTI_IQRNS_SPEC(12)
	EXTI_IQRNS_SPEC(13)
	EXTI_IQRNS_SPEC(14)		
	EXTI_IQRNS_SPEC(15)		
}
//	delay_ms(10);//消抖
//	if(KEY0==0)	 //按键KEY0
//	{
//		LED0=!LED0;
//		LED1=!LED1; 
//	}		 
