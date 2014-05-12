#include "KeyFSM.h"
#include "millis.h"
#include "stm32f10x.h"
/*************键盘状态机****************/
/*
   占用资源：
   		RTC	& 闹钟中断   （注意：doAfter_ms函数不可再作它用，millis()函数没影响）
	注意：
		  使用前调用 MILLIS_Init();  
		  回调函数在闹钟中断中执行，故时间不宜长

事件
	Press				按键刚按下时触发：
	Release				按键放开时触发	
	Hold				按键按下保持时持续触发，可获得持续时长	
	Click				Release后且double超时时触发
	DoubleClick			Release后在doubleClickInterval内再次Release时触发
Example:
	
	void doWhenRelease(struct KeyFSMType *self)
	{
		printf("Release\n");
	}
	void doWhenClick(struct KeyFSMType *self)
	{
		printf("Click\n");
	}
	void doWhenHold(struct KeyFSMType *self)
	{
		printf("Hold,%d\n",self->duration);
	}
	void doWhenDouble(struct KeyFSMType *self)
	{
		printf("Double\n");
	}
	void doWhenPress(struct KeyFSMType *self)
	{
		printf("Press\n");
	}  
	KeyFSMTypeDef aKeyFSM;      
	Key_FSM_Structure(&aKeyFSM,GPIOB,GPIO_Pin_0,250);//300表示双击的间隔等待时长（ms）
	aKeyFSM.doWhenRelease=doWhenRelease;
	aKeyFSM.doWhenClick=doWhenClick;
	//aKeyFSM.doWhenPress=doWhenPress;
	aKeyFSM.doWhenHold=doWhenHold;
	aKeyFSM.doWhenDoubleClick=doWhenDouble;
	//事件回调函数不必全指定
*/

u8 IS_MILLIS_RUNNING;
KeyFSMTypeDef *	defaultKeyFSM[16];
u8 defaultKeyFSM_NUM;
u8 Key_FSM_getKeyValue(struct KeyFSMType *self)
{
	return GPIO_ReadInputDataBit(self->GPIO, self->GPIO_Pin);
}

uint32_t filterGPIO_RCC_by_GPIO_FSM(GPIO_TypeDef *GPIOx)
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
 void MILLIS_KEY_SCAN_THREAD(void)
{
    u8 i;
	for(i=0;i<defaultKeyFSM_NUM;i++)
		Key_FSM_Handler(defaultKeyFSM[i]);
	doAfter_ms(ANTI_SHAKING_TIME_MS,MILLIS_KEY_SCAN_THREAD);
}      
void Key_FSM_Structure(KeyFSMTypeDef* aKeyFSM,	GPIO_TypeDef *GPIO,uint16_t GPIO_Pin,u32  doubleClickInterval)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	aKeyFSM->GPIO=GPIO;
	aKeyFSM->GPIO_Pin=GPIO_Pin;
	aKeyFSM->doubleClickInterval=doubleClickInterval;
	aKeyFSM->getKeyValue=Key_FSM_getKeyValue;
	aKeyFSM->currentState=FSM_STATE1;

	aKeyFSM->doWhenRelease=0;
	aKeyFSM->doWhenHold=0;
	aKeyFSM->doWhenDoubleClick=0;
	aKeyFSM->doWhenClick=0;
	aKeyFSM->doWhenPress=0;
		
	aKeyFSM->duration=0;
	aKeyFSM->_DURATION_MASK_=0;
	aKeyFSM->_LAST_TRIGGERED_TIME=0;
	aKeyFSM->_CLICK_NEENDING_CONFIRM=0;
	aKeyFSM->_PREVIOUS_OPERATION=0;
	RCC_APB2PeriphClockCmd(filterGPIO_RCC_by_GPIO_FSM(GPIO), ENABLE);//###
	GPIO_InitStructure.GPIO_Mode=KEY_FREE_VALUE?GPIO_Mode_IPU:GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin;
	GPIO_Init(GPIO,&GPIO_InitStructure);
	defaultKeyFSM[defaultKeyFSM_NUM++]=aKeyFSM;
	if(!IS_MILLIS_RUNNING)
	{
	  doAfter_ms(SCAN_INTERVAL_MS,MILLIS_KEY_SCAN_THREAD);
	  IS_MILLIS_RUNNING=1;
	}
}

void Key_FSM_Handler(KeyFSMTypeDef* aKeyFSM)
{
	switch(aKeyFSM->currentState)
	{
		case FSM_STATE1:
			if(millis()-aKeyFSM->_LAST_TRIGGERED_TIME>aKeyFSM->doubleClickInterval&&aKeyFSM->_CLICK_NEENDING_CONFIRM)
			{
				if(aKeyFSM->doWhenClick!=0)
				   aKeyFSM->doWhenClick(aKeyFSM);
				aKeyFSM->_CLICK_NEENDING_CONFIRM=0;
			}
			if(aKeyFSM->getKeyValue(aKeyFSM)==KEY_FREE_VALUE)
			  aKeyFSM->currentState=FSM_STATE1;
			else
			  {
			  	aKeyFSM->currentState=FSM_STATE2;
			  }
			break;
		case FSM_STATE2:
			if(aKeyFSM->getKeyValue(aKeyFSM)==KEY_FREE_VALUE)
			  aKeyFSM->currentState=FSM_STATE1;
			else
			  {
				if(aKeyFSM->doWhenPress!=0)
					aKeyFSM->doWhenPress(aKeyFSM); 
			  	aKeyFSM->currentState=FSM_STATE3;
			  }
			break;
		case FSM_STATE3:
			if(aKeyFSM->getKeyValue(aKeyFSM)==KEY_FREE_VALUE)
			{
				aKeyFSM->currentState=FSM_STATE1;
				aKeyFSM->duration=aKeyFSM->_DURATION_MASK_*SCAN_INTERVAL_MS;
				if(millis()-aKeyFSM->_LAST_TRIGGERED_TIME<aKeyFSM->doubleClickInterval&&aKeyFSM->_PREVIOUS_OPERATION==PREVIOUS_NONEDOUBLE)	  
				{
					if(aKeyFSM->doWhenDoubleClick!=0)
						aKeyFSM->doWhenDoubleClick(aKeyFSM);
					aKeyFSM->_CLICK_NEENDING_CONFIRM=0;
					aKeyFSM->_PREVIOUS_OPERATION=PREVIOUS_DOUBLE;
				}
				else	 
				{
					aKeyFSM->_PREVIOUS_OPERATION=PREVIOUS_NONEDOUBLE;
					aKeyFSM->_CLICK_NEENDING_CONFIRM=1;
				}
				aKeyFSM->_LAST_TRIGGERED_TIME=millis();	
				if(aKeyFSM->doWhenRelease!=0)
					aKeyFSM->doWhenRelease(aKeyFSM);
				aKeyFSM->_DURATION_MASK_=0; 
				aKeyFSM->duration=0;
			}
			else
			{
				aKeyFSM->_DURATION_MASK_++;
				aKeyFSM->duration=aKeyFSM->_DURATION_MASK_*SCAN_INTERVAL_MS;
				if(aKeyFSM->doWhenHold!=0)
					aKeyFSM->doWhenHold(aKeyFSM);
			}
			break;
	}
  
}
