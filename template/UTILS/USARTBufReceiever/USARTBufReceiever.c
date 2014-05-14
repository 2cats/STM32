#include "USARTBufReceiever.h" 
//����1���ͻ��������ṩ��ȡ���ڽ����ж����ݵļ��׽ӿ�

/*************!WARNING!******************/
/*
	��һ���Խ���̫������ʱ���൫�����û���⣩��buf���ݺ��п�����ʹ��ǰ�����ǣ��������ݴ������󣬽������������buf����
*/

/************INTERFACE*****************/
/*

Old Example:
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
u8 USARTBufReceiever_checkAllRight(void)//�ж��Ƿ񻺳����
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
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 

	
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
 	USARTBufReceiever->enableReceieve(); //������uart_init�����
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
