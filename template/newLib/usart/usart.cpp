#include "usart.h"
#include "util/util.h"
#include "config/Configure.h"
#include "stdio.h"
#include <stdarg.h>
#include <string.h>
/*
串口1&2 C++封装
构造函数分析：
	若不提供中断回调函数，则默认使用DMA环形缓存
	
配合Postman：(DMA)
Example:

#include "usart/UsartA.h"
#include "usart/UsartB.h"

Postman *postman;
void uart1_receive_proc()
{
	Package package;
	while(postman->receive(&package))
	{
		switch(package.type)
		{
			case 0x99:
			
				break;
			case 0x76:
			
				break;
		}
	}
}


	Usart usart1(USART1_Conf,9600);//创建 stream
	Postman _postman(&usart1);//创建 postman
	postman=&_postman;
	Timer timer(TIM1_Conf);
	timer.doEvery(10,uart1_receive_proc);//不断调用 postman.receive  ，判断并处理 package
	
速度测试：
115200:	10	kb/s
9600:		0.8	kb/s
*/
void(*USART_callback[3])(u8 data);
int freeCir[3],currP[3];
Usart::Usart(USART_ConfBase& conf,int baud,void(*callback)(u8 data)):USARTx(conf.USARTx),theConfig(conf)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	conf.RCC_APBxPeriphClockCmd(conf.RCC_USART, ENABLE);	
	RCC_APB2PeriphClockCmd(conf.RCC_GPIO, ENABLE);
	
	USART_DeInit(conf.USARTx);
	//USART1_TX  
	GPIO_InitStructure.GPIO_Pin = conf.PIN_TX; 
	GPIO_InitStructure.GPIO_Speed =conf.SPEED;
	GPIO_InitStructure.GPIO_Mode =(GPIOMode_TypeDef) GPIO_Mode_AF_PP;
	GPIO_Init(conf.GPIOx, &GPIO_InitStructure);
	//USART1_RX	  
	GPIO_InitStructure.GPIO_Pin =conf.PIN_RX;
	GPIO_InitStructure.GPIO_Mode = (GPIOMode_TypeDef)GPIO_Mode_IN_FLOATING;
	GPIO_Init(conf.GPIOx, &GPIO_InitStructure);  


	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = conf.USART_WordLength;
	USART_InitStructure.USART_StopBits = conf.USART_StopBits;
	USART_InitStructure.USART_Parity =conf.USART_Parity;
	USART_InitStructure.USART_HardwareFlowControl =conf.USART_HardwareFlowControl;
	USART_InitStructure.USART_Mode =conf.USART_Mode;

	USART_Init(conf.USARTx, &USART_InitStructure);
	USART_Cmd(conf.USARTx, ENABLE);
	if(callback!=0)
	{
		enableIRq();
		USART_callback[conf.id]=callback;
	}
	else
		enableDmaRead();
	
}
void Usart::enableDmaRead()
{
	DMA_InitTypeDef DMA_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_DeInit(theConfig.DMA_Channel);

	DMA_InitStructure.DMA_PeripheralBaseAddr =  (uint32_t) & (USARTx->DR); 
	DMA_InitStructure.DMA_MemoryBaseAddr =(u32)USART_defaultBuf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 
	DMA_InitStructure.DMA_BufferSize = DMA_BUFSIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize =DMA_MemoryDataSize_Byte;

	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
 	DMA_Init(theConfig.DMA_Channel, &DMA_InitStructure); 

	DMA_ITConfig(theConfig.DMA_Channel,DMA_IT_TC,ENABLE ); 
	USART_DMACmd(USARTx,USART_DMAReq_Rx,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = theConfig.DMA_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);  
	
	DMA_Cmd(theConfig.DMA_Channel,ENABLE);

}
void Usart::enableIRq()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = theConfig.USARTx_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	USART_ITConfig(theConfig.USARTx, USART_IT_RXNE, ENABLE);//开启中断
}
bool Usart::write(uint8_t byte)
{
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET){}
		USART_SendData(USARTx,(uint8_t)byte); 
	return true;
}
bool Usart::write(uint8_t *bytes,int len)
{
	int i;
	for(i=0;i<len;i++)
	{
		write(bytes[i]);
	}
	return true;
}
bool Usart::write(char const *bytes,int len)
{
	int i;
	for(i=0;i<len;i++)
	{
		write(bytes[i]);
	}
	return true;
}
void Usart::printf(char const *fmt, ...)
{
	char buffer[CMD_BUFFER_LEN+1];      // CMD_BUFFER_LEN长度自己定义吧
	int i = 0;
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, CMD_BUFFER_LEN+1, fmt, arg_ptr);
	while ((i < CMD_BUFFER_LEN) && buffer[i])
	{
		write(buffer[i++]);	
	}
	va_end(arg_ptr);
}

uint8_t Usart::block_read()
{
	while(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE)==RESET);
	return  USARTx->DR;
}
bool Usart::isDataAvailable()
{
	return USART_GetFlagStatus(USARTx,USART_FLAG_RXNE)==SET;
}
bool Usart::isReadyToSend()  
{
    return USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==SET;
}

void Usart::resume()
{
	USART_Cmd(USARTx,ENABLE);
}

inline void Usart::pause() 
{
	USART_Cmd(USARTx,DISABLE);
}


Usart& Usart::operator<<(unsigned char c)
{
	write(c);
	return *this;
	
}
Usart& Usart::operator<<(char c)
{
	write(c);
	return *this;
}
Usart& Usart::operator<<(char const *str)
{
	write(str,(int)strlen(str));
	return *this;
}
Usart& Usart::operator<<(int32_t val)
{
	printf("%d",val);
	return *this;
}
Usart& Usart::operator<<(uint32_t val)
{
	printf("%d",val);
	return *this;
}
Usart& Usart::operator<<(int16_t val)
{
	printf("%d",val);
	return *this;
}
Usart& Usart::operator<<(uint16_t val)
{
	printf("%d",val);
	return *this;
}
Usart& Usart::operator<<(double val)
{
	printf("%lf",val);
	return *this;
}


u8 Usart::read(void)
{
	if(!available())
		return 0xff;
	u8 tmp=USART_defaultBuf[currP[theConfig.id]%DMA_BUFSIZE];
	currP[theConfig.id]++;
	return tmp;
}
int Usart::available()
{
	return freeCir[theConfig.id]+DMA_BUFSIZE-theConfig.DMA_Channel->CNDTR-currP[theConfig.id];
}
bool Usart::read(uint8_t *bytes,int len)
{
	int i;
	if(available()<len)
		return false;
	for(i=0;i<len;i++)
	{
		bytes[i]=read();
	}
	return true;
}
void Usart::resumeDmaRead()
{
	DMA_Cmd(theConfig.DMA_Channel,ENABLE);
}
void Usart::pauseDmaRead()
{
	DMA_Cmd(theConfig.DMA_Channel,DISABLE);
}

extern "C"
{

	void DMA1_Channel5_IRQHandler(void)
	{
		if(DMA_GetITStatus(DMA1_IT_TC5))
		{
			freeCir[1]+=DMA_BUFSIZE;
			DMA_ClearITPendingBit(DMA1_IT_TC5);
		}
	}	
	void DMA1_Channel6_IRQHandler(void)
	{
		if(DMA_GetITStatus(DMA1_IT_TC6))
		{
			freeCir[2]+=DMA_BUFSIZE;
			DMA_ClearITPendingBit(DMA1_IT_TC6);
		}
	}
	void USART1_IRQHandler(void)
	{
		if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
		 {
			 if(USART_callback[1])
				USART_callback[1](USART1->DR);
			 USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		 }
	}
	void USART2_IRQHandler(void)                	
	{
		if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
		 {
			 if(USART_callback[2])
				USART_callback[2](USART2->DR);
			 USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		 }
	}
	
}
