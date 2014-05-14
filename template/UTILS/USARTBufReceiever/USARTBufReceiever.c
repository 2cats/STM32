#include "USARTBufReceiever.h" 
//串口1环型缓存器，提供获取串口接收中断数据的简易接口

/*************!WARNING!******************/
/*
	当一次性接收太多数据时（多但间隔长没问题），buf数据很有可能在使用前被覆盖，导致数据错误现象，解决方法：增大buf容量
*/

/************INTERFACE*****************/
/*

Old Example:
	USARTBufReceieverTypeDef USARTBufReceiever;
	USARTBufReceiever_Structure(&USARTBufReceiever);
	uart_init(115200);
	USARTBufReceiever.enableReceieve(); //必须在uart_init后调用
	if(USARTBufReceiever.getAvaliableNum()>0)
	{
		tmp=USARTBufReceiever.getByte();
		//printf("%c",tmp);
	}
	
*/
/************UPDATE*****************/
/*
2014/4/3
	new Function : uart_init_BR
	Example:
		USARTBufReceieverTypeDef USARTBufReceiever;
		uart_init_BR(115200,&USARTBufReceiever);
		if(USARTBufReceiever.getAvaliableNum()>0)
		{
			tmp=USARTBufReceiever.getByte();
			//printf("%c",tmp);
		}
*/

//variables
/*********************************/
u8 USART_defaultBuf[USART_BufSIZE];
int _buf_Oldest_Data_Pointer=0;
int _ALL_Pointer_Circles=0,_ALL_Circles_Count=0;
USARTBufReceieverTypeDef *defaultUSARTBufReceiever;

//Functions
/*********************************/
u8 USARTBufReceiever_getByte(void)
{
	 u8 tmp=USART_defaultBuf[_buf_Oldest_Data_Pointer++];
	_CHECK_RESET_POINTER
	return tmp;
}
void USARTBufReceiever_getBytes(u8 *dat,int num)
{
	int i;
	if(defaultUSARTBufReceiever->getAvaliableNum()<num)return;
	for(i=0;i<num;i++)
	{
		dat[i]=USART_defaultBuf[_buf_Oldest_Data_Pointer++];
		_CHECK_RESET_POINTER
	}
}
u8 USARTBufReceiever_checkAllRight(void)//判断是否缓冲溢出
{
	if(_ALL_Free-_ALL_Pointer>=USART_BufSIZE)
		return 0; 
	else
		return 1;//OK
}
int USARTBufReceiever_getAvaliableNum(void)
{
	return (_ALL_Free-_ALL_Pointer);
}
void USARTBufReceiever_enableReceieve(void)
{
	DMA_InitTypeDef DMA_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA??

	DMA_DeInit(DMA1_Channel5);

	DMA_InitStructure.DMA_PeripheralBaseAddr =  (uint32_t) & (USART1->DR); //????????
	DMA_InitStructure.DMA_MemoryBaseAddr =(u32)USART_defaultBuf; //RAM???
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //????
	DMA_InitStructure.DMA_BufferSize = USART_BufSIZE;//Size
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //???????????
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //RAM?????????

	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize =DMA_MemoryDataSize_Byte; //????!!!!!!!!

	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA_Mode_Normal; //??????
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //???
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //???RAM2RAM
	DMA_Init(DMA1_Channel5, &DMA_InitStructure); 

	DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE ); //DMA??(???NVIC)
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);//?????DMA??
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 

	
	DMA_Cmd(DMA1_Channel5,ENABLE);//??DMA
}
u32 USARTBufReceiever_getRxNum(void)
{
	return _ALL_Pointer;
}
void USARTBufReceiever_Structure(USARTBufReceieverTypeDef *USARTBufReceiever)
{
	defaultUSARTBufReceiever=USARTBufReceiever;
	defaultUSARTBufReceiever->getAvaliableNum=USARTBufReceiever_getAvaliableNum;
	defaultUSARTBufReceiever->checkAllRight=USARTBufReceiever_checkAllRight;
	defaultUSARTBufReceiever->getByte=USARTBufReceiever_getByte;
	defaultUSARTBufReceiever->getBytes=USARTBufReceiever_getBytes;
	defaultUSARTBufReceiever->enableReceieve=USARTBufReceiever_enableReceieve;
	defaultUSARTBufReceiever->getRxNum=USARTBufReceiever_getRxNum; 
}
void uart_init_BR(u32 bound,USARTBufReceieverTypeDef *USARTBufReceiever)
{
	USARTBufReceiever_Structure(USARTBufReceiever); 
	uart_init(bound);
 	USARTBufReceiever->enableReceieve(); //必须在uart_init后调用
}
//IQR
/*********************************/
void DMA1_Channel5_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC5))
	{
		_ALL_Circles_Count+=USART_BufSIZE;
		DMA_ClearITPendingBit(DMA1_IT_TC5);
	}
}
