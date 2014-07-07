#ifndef USARTT_H
#define USARTT_H
#include <stm32f10x.h>
#include "util/util.h"
#include "config/Configure.h"
#define CMD_BUFFER_LEN 512
#define DMA_BUFSIZE 512

class  Usart
{
public :
	Usart(USART_ConfBase&,int,void(*callback)(u8 data)=0);

	bool write(uint8_t byte);
	bool write(uint8_t *bytes,int len);
	bool write(char const *bytes,int len);
	void printf(char const *fmt, ...);

	uint8_t dma_read();
	bool dma_read(uint8_t *bytes,int len);
	int dma_availableNum();

	Usart& operator<<(char const*str);
	Usart& operator<<(char c);
	Usart& operator<<(unsigned char c);
	Usart& operator<<(int32_t val);
	Usart& operator<<(uint32_t val);
	Usart& operator<<(int16_t val);
	Usart& operator<<(uint16_t val);
	Usart& operator<<(double val);
	
	void resume();
	void pause();
	void resumeDmaRead();
	void pauseDmaRead();

private:
	void enableDmaRead();
	void enableIRq();
	USART_TypeDef *USARTx;
	USART_ConfBase theConfig;
	u8 USART_defaultBuf[DMA_BUFSIZE];

//deprecated sync funcs
	uint8_t read();
	bool isDataAvailable();
	bool isReadyToSend();
	//u8 mode;
};


#endif