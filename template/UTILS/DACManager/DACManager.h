#ifndef _DAC_MANAGER_H
#define _DAC_MANAGER_H
#include "stm32f10x.h"
//PA4
//12λ�Ҷ���

typedef struct DACManager
{
	void(*enable)(void);
	void(*disable)(void);
 	void(*setValue)(uint16_t registerValue);
}DACManagerTypeDef;
void DACManager_Init_Enable(DACManagerTypeDef *DACManager);
#endif 
