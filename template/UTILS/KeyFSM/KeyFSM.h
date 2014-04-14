#ifndef _KEYfsm_H_
#define _KEYfsm_H_	 
#include "stm32f10x.h"
#define  FSM_STATE1 1
#define  FSM_STATE2 2
#define  FSM_STATE3 3

#define  KEY_FREE_VALUE 1
#define  KEY_TRIG_VALUE	(!KEY_FREE_VALUE)

#define ANTI_SHAKING_TIME_MS 10
#define SCAN_INTERVAL_MS ANTI_SHAKING_TIME_MS

//#define PREVIOUS_INIT	0  //刚刚初始化状态
//#define PREVIOUS_CLICK	1  //上一次操作是单击
#define PREVIOUS_DOUBLE 2 //上一次操作是双击
//#define PREVIOUS_TRIBLE 3 //上一次操作是三击
#define PREVIOUS_NONEDOUBLE 4 //上一次操作不是双击
typedef struct KeyFSMType
{
//public:
	GPIO_TypeDef *GPIO;
	uint16_t GPIO_Pin;

	void (*doWhenRelease)(struct KeyFSMType *self);
	void (*doWhenPress)(struct KeyFSMType *self);
	void (*doWhenClick)(struct KeyFSMType *self);
	void (*doWhenHold)(struct KeyFSMType *self);
	void (*doWhenDoubleClick)(struct KeyFSMType *self);
	u32  doubleClickInterval;
	u32 duration;
//private:	
	u8 (*getKeyValue)(struct KeyFSMType *self);
	u8 currentState;
	u32 _DURATION_MASK_;
	u32 _LAST_TRIGGERED_TIME;
	u8 _CLICK_NEENDING_CONFIRM;
	u8  _PREVIOUS_OPERATION;
}KeyFSMTypeDef;
void Key_FSM_Structure(KeyFSMTypeDef* aKeyFSM,GPIO_TypeDef *GPIO,uint16_t GPIO_Pin,u32  doubleClickInterval);
void Key_FSM_Handler(KeyFSMTypeDef* aKeyFSM);



#endif

