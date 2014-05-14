#include "USART2BufReceiever.h" 
//����1���ͻ��������ṩ��ȡ���ڽ����ж����ݵļ��׽ӿ�

/*************!WARNING!******************/
/*
	��һ���Խ���̫������ʱ���൫�����û���⣩��buf���ݺ��п�����ʹ��ǰ�����ǣ��������ݴ������󣬽������������buf����
*/

/************INTERFACE*****************/
/*

Example:
	USARTBufReceieverTypeDef USARTBufReceiever;
	USARTBufReceiever_Structure(&USARTBufReceiever);
	uart_init(115200);
	USARTBufReceiever.enableReceieve(); //������uart_init�����
	if(USARTBufReceiever.getAvaliableNum()>0)
	{
		tmp=USARTBufReceiever.getByte();
		//printf("%c",tmp);
	}
	
*/
/************UPDATE*****************/
/*
2013/4/3
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
u8 USART2_defaultBuf[USART2_BufSIZE];
int _buf2_Oldest_Data_Pointer=0;
int _ALL2_Pointer_Circles=0,_ALL2_Circles_Count=0;
USART2BufReceieverTypeDef *defaultUSART2BufReceiever;

//Functions
/*********************************/
u8 USART2BufReceiever_getByte(void)
{
	 u8 tmp=USART2_defaultBuf[_buf2_Oldest_Data_Pointer++];
	_CHECK2_RESET_POINTER
	return tmp;
}
void USART2BufReceiever_getBytes(u8 *dat,int num)
{
	int i;
	if(defaultUSART2BufReceiever->getAvaliableNum()<num)return;
	for(i=0;i<num;i++)
	{
		dat[i]=USART2_defaultBuf[_buf2_Oldest_Data_Pointer++];
		_CHECK2_RESET_POINTER
	}
}
u8 USART2BufReceiever_checkAllRight(void)//�ж��Ƿ񻺳����
{
	if(_ALL2_Free-_ALL2_Pointer>=USART2_BufSIZE)
		return 0; 
	else
		return 1;//OK
}
int USART2BufReceiever_getAvaliableNum(void)
{
	return (_ALL2_Free-_ALL2_Pointer);
}
void USART2BufReceiever_enableReceieve(void)
{
	DMA_InitTypeDef DMA_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA??

	DMA_DeInit(DMA1_Channel6);

	DMA_InitStructure.DMA_PeripheralBaseAddr =  (uint32_t) & (USART2->DR); //????????
	DMA_InitStructure.DMA_MemoryBaseAddr =(u32)USART2_defaultBuf; //RAM???
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //????
	DMA_InitStructure.DMA_BufferSize = USART2_BufSIZE;//Size
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //???????????
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //RAM?????????

	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize =DMA_MemoryDataSize_Byte; //????!!!!!!!!

	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA_Mode_Normal; //??????
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //???
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //???RAM2RAM
	DMA_Init(DMA1_Channel6, &DMA_InitStructure); 

	DMA_ITConfig(DMA1_Channel6,DMA_IT_TC,ENABLE ); //DMA??(???NVIC)
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);//?????DMA??
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 

	
	DMA_Cmd(DMA1_Channel6,ENABLE);//??DMA
}
u32 USART2BufReceiever_getRxNum(void)
{
	return _ALL2_Pointer;
}
void USART2BufReceiever_Structure(USART2BufReceieverTypeDef *USART2BufReceiever)
{
	defaultUSART2BufReceiever=USART2BufReceiever;
	defaultUSART2BufReceiever->getAvaliableNum=USART2BufReceiever_getAvaliableNum;
	defaultUSART2BufReceiever->checkAllRight=USART2BufReceiever_checkAllRight;
	defaultUSART2BufReceiever->getByte=USART2BufReceiever_getByte;
	defaultUSART2BufReceiever->getBytes=USART2BufReceiever_getBytes;
	defaultUSART2BufReceiever->enableReceieve=USART2BufReceiever_enableReceieve;
	defaultUSART2BufReceiever->getRxNum=USART2BufReceiever_getRxNum; 
}
void uart2_init_BR(u32 bound,USART2BufReceieverTypeDef *USART2BufReceiever)
{
	USART2BufReceiever_Structure(USART2BufReceiever); 
	uart2_init(bound);
 	USART2BufReceiever->enableReceieve(); //������uart_init�����
}
//IQR
/*********************************/
void DMA1_Channel6_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC6))
	{
		_ALL2_Circles_Count+=USART2_BufSIZE;
		DMA_ClearITPendingBit(DMA1_IT_TC6);
	}
}
