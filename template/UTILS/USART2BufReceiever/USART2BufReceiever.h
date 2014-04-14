#ifndef _USART2BUFRECEIVER_H_
#define _USART2BUFRECEIVER_H_
#include "stm32f10x.h"
#include "usart2.h"
#define USART2_BufSIZE 50

#define _ALL2_Free (_ALL2_Circles_Count+USART2_BufSIZE-DMA1_Channel6->CNDTR)
#define _ALL2_Pointer (_ALL2_Pointer_Circles+_buf2_Oldest_Data_Pointer)
#define _CHECK2_RESET_POINTER  if(_buf2_Oldest_Data_Pointer>=USART2_BufSIZE) {_buf2_Oldest_Data_Pointer=0;_ALL2_Pointer_Circles+=USART2_BufSIZE;}
typedef struct USART2BufReceieverType{
	void (*enableReceieve)(void);
	int (*getAvaliableNum)(void);
	u8 (*getByte)(void);
	void (*getBytes)(u8 *dat,int num);
	u8 (*checkAllRight)(void);
	u32 (*getRxNum)(void);
}USART2BufReceieverTypeDef;

u8 USART2BufReceiever_getByte(void);
void USART2BufReceiever_getBytes(u8 *dat,int num);
u8 USART2BufReceiever_checkAllRight(void);
void USART2BufReceiever_enableReceieve(void);
void USART2BufReceiever_Structure(USART2BufReceieverTypeDef *USART2BufReceiever);
int USART2BufReceiever_getAvaliableNum(void);
void DMA1_Channel6_IRQHandler(void);
u32 USART2BufReceiever_getRxNum(void);
void uart2_init_BR(u32 bound,USART2BufReceieverTypeDef *USART2BufReceiever);
#endif
