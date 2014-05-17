#ifndef H_NEWKEYSCAN_H
#define H_NEWKEYSCAN_H
#include <stm32f10x.h>
struct KeyIOPort 
{
	GPIO_TypeDef *GPIO_x;
	unsigned short GPIO_pin;
};
extern void (*NewKeyTriggered)(unsigned char);

void NewKeyScan_Init(void);

void NewKeyScan_Proc(void);
#endif
