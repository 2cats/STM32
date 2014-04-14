#include "sys.h"
 

#include "usart2.h"	 
#define CMD_BUFFER_LEN  			200
//USART2_TX PA2 
//USART2_RX PA3 
//uart2_printf有问题

//串口2中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误  
//初始化IO 串口2
//bound:波特率

void uart2_init(u32 bound){
    //GPIO端口设置
   GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
#if EN_USART2_RX		  //如果使能了接收  
	NVIC_InitTypeDef NVIC_InitStructure;
#endif
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART2，GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
 	USART_DeInit(USART2);  //复位串口1
	 //USART1_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
    //USART1_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10


   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART2, &USART_InitStructure); //初始化串口
#if EN_USART2_RX		  //如果使能了接收中断
   //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
#endif
    USART_Cmd(USART2, ENABLE);                    //使能串口 

}
void uart2_sendChar(u8 ch)
{
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
		USART_SendData(USART2,(uint8_t)ch);   
}
////u8 data[6];
//void USART2_IRQHandler(void)                	//串口1中断服务程序
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
	char buffer[CMD_BUFFER_LEN+1];      // CMD_BUFFER_LEN长度自己定义吧
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
//输入结束标识： 0x0d,0x0a 
//确保输入的一行内包含所需信息
int uart2_scanf(const char *fmt,...)
{
	char buffer[CMD_BUFFER_LEN+1];      // CMD_BUFFER_LEN长度自己定义吧
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
