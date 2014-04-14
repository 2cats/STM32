#ifndef _USARTBUFRECEIVER_H_
#define _USARTBUFRECEIVER_H_
#include "stm32f10x.h"
#include "usart.h"
#define USART_BufSIZE 50

#define _ALL_Free (_ALL_Circles_Count+USART_BufSIZE-DMA1_Channel5->CNDTR)
#define _ALL_Pointer (_ALL_Pointer_Circles+_buf_Oldest_Data_Pointer)
#define _CHECK_RESET_POINTER  if(_buf_Oldest_Data_Pointer>=USART_BufSIZE) {_buf_Oldest_Data_Pointer=0;_ALL_Pointer_Circles+=USART_BufSIZE;}
typedef struct USARTBufReceieverType{
	void (*enableReceieve)(void);
	int (*getAvaliableNum)(void);
	u8 (*getByte)(void);
	void (*getBytes)(u8 *dat,int num);
	u8 (*checkAllRight)(void);
	u32 (*getRxNum)(void);
}USARTBufReceieverTypeDef;

u8 USARTBufReceiever_getByte(void);
void USARTBufReceiever_getBytes(u8 *dat,int num);
u8 USARTBufReceiever_checkAllRight(void);
void USARTBufReceiever_enableReceieve(void);
void USARTBufReceiever_Structure(USARTBufReceieverTypeDef *USARTBufReceiever);
int USARTBufReceiever_getAvaliableNum(void);
void DMA1_Channel5_IRQHandler(void);
u32 USARTBufReceiever_getRxNum(void);
void uart_init_BR(u32 bound,USARTBufReceieverTypeDef *USARTBufReceiever);
#endif
