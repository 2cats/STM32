#ifndef H_NEWKEYSCAN_H
#define H_NEWKEYSCAN_H
#include <stm32f10x.h>
#ifdef __cplusplus
 extern "C" {
#endif

struct KeyIOPort 
{
	GPIO_TypeDef *GPIO_x;
	unsigned short GPIO_pin;
};
extern void (*NewKeyTriggered)(unsigned char);

void NewKeyScan_Init(void);

void NewKeyScan_Proc(void);

#ifdef __cplusplus
 }
#endif

#endif
