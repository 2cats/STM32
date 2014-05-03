#include "sys.h"
 

#include "usart2.h"	 
#define CMD_BUFFER_LEN  			200
//USART2_TX PA2 
//USART2_RX PA3 
//uart2_printf������

//����2�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���  
//��ʼ��IO ����2
//bound:������

void uart2_init(u32 bound){
    //GPIO�˿�����
   GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
#if EN_USART2_RX		  //���ʹ���˽���  
	NVIC_InitTypeDef NVIC_InitStructure;
#endif
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//ʹ��USART2��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
 	USART_DeInit(USART2);  //��λ����1
	 //USART1_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART1_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10


   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART2, &USART_InitStructure); //��ʼ������
#if EN_USART2_RX		  //���ʹ���˽����ж�
   //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
   
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
#endif
    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 

}
void uart2_sendChar(u8 ch)
{
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
		USART_SendData(USART2,(uint8_t)ch);   
}
////u8 data[6];
//void USART2_IRQHandler(void)                	//����1�жϷ������
//{
//	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
// {
//	USART_ClearITPendingBit(USART2,USART_IT_RXNE);
// }
//}

void uart2_print(char *str)
{	
	int i=0;
	while(str[i]!='\0')
	{
		uart2_sendChar(str[i++]);
	}
}
void uart2_println(u8 *str)
{	
	int i=0;
	while(str[i]!='\0')
	{
		uart2_sendChar(str[i++]);
	}
	uart2_sendChar('\n');
}
void uart2_printf (char *fmt, ...)
{
	char buffer[CMD_BUFFER_LEN+1];      // CMD_BUFFER_LEN�����Լ������
	u8 i = 0;
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, CMD_BUFFER_LEN+1, fmt, arg_ptr);
	while ((i < CMD_BUFFER_LEN) && buffer[i])
	{
		uart2_sendChar(buffer[i++]);	
	}
	va_end(arg_ptr);
}
unsigned char recv_char(void)
{
	while(USART_GetFlagStatus(USART2,USART_FLAG_RXNE)==RESET);
	return  USART2->DR;
}
//���������ʶ�� 0x0d,0x0a 
//ȷ�������һ���ڰ���������Ϣ
int uart2_scanf(const char *fmt,...)
{
	char buffer[CMD_BUFFER_LEN+1];      // CMD_BUFFER_LEN�����Լ������
	va_list arg_ptr;
  int i = 0;
	unsigned char c;
	 while(1)
		{
			c = recv_char();
			if((c == 0x0d) || (c == 0x0a))
			{
					buffer[i] = '\0';
					break;
			}else
			{
					buffer[i++] = c;
			}
		}
	va_start(arg_ptr, fmt);   
	i = vsscanf(buffer,fmt,arg_ptr);
	va_end(arg_ptr);
	return i;
}
