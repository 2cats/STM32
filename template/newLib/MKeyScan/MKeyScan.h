#ifndef __MKEYSCAN_H
#define __MKEYSCAN_H 	
#ifdef __cplusplus
 extern "C" {
#endif
#include <stm32f10x.h>
#include "delay.h"
/*	Interface:*/
void MKeyScan_Init(int scanInterval,int doubleInterval);
void MKeyScan_Poc();
//#define MKey_ENABLE_HOLD
//回调函数参数定义
struct MKey_Info
{
	u8 value;
	int holdTime;
	u8 type;
};
extern void (*MKeyScan_cb)(struct MKey_Info*);

#define MKey_PRESS		0

#ifdef  MKey_ENABLE_HOLD
#define MKey_HOLD		1
#endif
#define MKey_RELEASE	2
#define MKey_CLICK		3
#define MKey_DOUBLE		4
 /*	Config:*/
#define KEY_GPIO_RCC				RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB
u8 MKeyScan_GETKEY();//返回有效状态的按键值（没有情况时返回MKeyScan_NULL）

//Example：
/*

void KeyScan_Handler(struct MKey_Info* info)
{
	switch(info->type)
	{
	case MKey_PRESS:
		break;
	case MKey_HOLD:
		//info->holdTime;
		break;
	case MKey_RELEASE:
		//info->value;
		break;
	case MKey_PRESS:
		break;
	case MKey_CLICK:
		break;
	case MKey_DOUBLE:
		break;
	}
}

*/

/*	Private Field:*/
 struct KeyIOPort
 {
 	GPIO_TypeDef *GPIO_x;
 	unsigned short GPIO_pin;
 };

#define MKeyScan_NULL				0
#define MKeyScan_STA_IDEL			0
#define MKeyScan_STA_WAIT4CON		1
#define MKeyScan_STA_HOLDING		2

#ifdef __cplusplus
}
#endif

#endif
