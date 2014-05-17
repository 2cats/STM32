#ifndef __KEYSCAN_H
#define __KEYSCAN_H
#include "stm32f10x.h"

struct io_port 
{
	GPIO_TypeDef *GPIO_x;
	unsigned short GPIO_pin;
};

#define SCAN_KEY_IDLE 0x01
#define SCAN_KEY_M1 	0x02

void keyboard_init(void);

unsigned char update_key(void);
extern void (*DoWhenMKTriggered)(char value);
void Scan_MatrixKey_Proc(void);

#endif
