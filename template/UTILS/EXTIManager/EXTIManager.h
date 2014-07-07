#ifndef _EXTIMANAGER_H_
#define _EXTIMANAGER_H_
#include "stm32f10x.h"

#ifdef __cplusplus
 extern "C" {
#endif

//u32 KEYBINDING_FLAG; 
//#define ISKEYBINDING(x) ((KEYBINDING_FLAG>>x)&1)
//#define SETKEYBINDING(x) (KEYBINDING_FLAG|=(1<<x))
#define EXTI_IQRNS(x)  void EXTI##x##_IRQHandler(void)\
{\
	_EXTILine[x]->doWhenTrigger(_EXTILine[x]);\
	EXTI_ClearITPendingBit(EXTI_Line##x); \
}
#define	EXTI_IQRNS_SPEC(n) if(status>>n&1)\
{		_EXTILine[n]->doWhenTrigger(_EXTILine[n]);\
		EXTI_ClearITPendingBit(EXTI_Line##n);\
}
typedef enum
{
  EXTI_Rising,
  EXTI_Falling,  
  EXTI_Both_DEFAULT_UP,
  EXTI_Both_DEFAULT_DOWN,
}MyEXTITrigger_TypeDef;


typedef struct EXTILineType{
	GPIO_TypeDef *GPIO;
	uint16_t GPIO_Pin;
 	MyEXTITrigger_TypeDef triger;
	void (*doWhenTrigger)( struct EXTILineType *triggeredLine);
//	KeyFSMTypeDef KeyFSM; 
}EXTILineTypeDef;

typedef struct EXTIManagerType{
	EXTILineTypeDef 	EXTILine[16];
	u8 _num;
	void (*startListenling)(GPIO_TypeDef* GPIO,uint16_t GPIO_Pin,MyEXTITrigger_TypeDef triger,void (*doWhenTrigger)( struct EXTILineType *triggeredLine));
	//void (*setDoWhenTrigger)( struct EXTILineType triggeredLine); 
}EXTIManagerTypeDef;

void setExactLine(EXTILineTypeDef * Line);
uint8_t filterGPIO_PortSourceGPIOC(GPIO_TypeDef *GPIOx);
GPIOMode_TypeDef filterGPIO_Mode(MyEXTITrigger_TypeDef triger);
EXTITrigger_TypeDef filterTrigger_Type(MyEXTITrigger_TypeDef triger);

uint8_t filterPinSource(	uint16_t GPIO_Pin);
IRQn_Type filterEXTI_IRQn(uint16_t GPIO_Pin);
void EXTIManager_StartListenling(GPIO_TypeDef *GPIO,uint16_t GPIO_Pin,MyEXTITrigger_TypeDef triger,void (*doWhenTrigger)( struct EXTILineType *triggeredLine));
void EXTIManager_Structure(EXTIManagerTypeDef *EXTIManager);

//	delay_ms(10);//Ïû¶¶
//	if(KEY0==0)	 //°´¼üKEY0
//	{
//		LED0=!LED0;
//		LED1=!LED1; 
//	}		 
#ifdef __cplusplus
}
#endif
#endif
