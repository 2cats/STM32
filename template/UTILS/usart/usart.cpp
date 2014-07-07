#include "usart.h"
template <Configures Configure>
inline Usart<Configure>::Usart(int baud)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(ConfigLoader<Configure>::RCC_USART, ENABLE);	
	RCC_APB2PeriphClockCmd(ConfigLoader<Configure>::RCC_GPIO, ENABLE);
	
	USART_DeInit((USART_TypeDef *)ConfigLoader<Configure>::GPIOx);
	//USART1_TX  
	GPIO_InitStructure.GPIO_Pin = ConfigLoader<Configure>::PIN_TX; 
	GPIO_InitStructure.GPIO_Speed =(GPIOSpeed_TypeDef) ConfigLoader<Configure>::SPEED;
	GPIO_InitStructure.GPIO_Mode =(GPIOMode_TypeDef) GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init((GPIO_TypeDef *)ConfigLoader<Configure>::GPIOx, &GPIO_InitStructure);
	//USART1_RX	  
	GPIO_InitStructure.GPIO_Pin = ConfigLoader<Configure>::PIN_RX;
	GPIO_InitStructure.GPIO_Mode = (GPIOMode_TypeDef)GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init((GPIO_TypeDef *)ConfigLoader<Configure>::GPIOx, &GPIO_InitStructure);  


	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = baud;//一般设置为9600;
	USART_InitStructure.USART_WordLength = ConfigLoader<Configure>::USART_WordLength;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = ConfigLoader<Configure>::USART_StopBits;//一个停止位
	USART_InitStructure.USART_Parity = ConfigLoader<Configure>::USART_Parity;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = ConfigLoader<Configure>::USART_HardwareFlowControl;//无硬件数据流控制
	USART_InitStructure.USART_Mode = ConfigLoader<Configure>::USART_Mode;	//收发模式

	USART_Init((USART_TypeDef *)ConfigLoader<Configure>::GPIOx, &USART_InitStructure); //初始化串口

	USART_Cmd((USART_TypeDef *)ConfigLoader<Configure>::GPIOx, ENABLE);                    //使能串口 
};
template <Configures Configure>
Usart<Configure>::Usart(int baud)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(ConfigLoader<Configure>::RCC_USART, ENABLE);	
	RCC_APB2PeriphClockCmd(ConfigLoader<Configure>::RCC_GPIO, ENABLE);
	
	USART_DeInit((USART_TypeDef *)ConfigLoader<Configure>::GPIOx);
	//USART1_TX  
	GPIO_InitStructure.GPIO_Pin = ConfigLoader<Configure>::PIN_TX; 
	GPIO_InitStructure.GPIO_Speed =(GPIOSpeed_TypeDef) ConfigLoader<Configure>::SPEED;
	GPIO_InitStructure.GPIO_Mode =(GPIOMode_TypeDef) GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init((GPIO_TypeDef *)ConfigLoader<Configure>::GPIOx, &GPIO_InitStructure);
	//USART1_RX	  
	GPIO_InitStructure.GPIO_Pin = ConfigLoader<Configure>::PIN_RX;
	GPIO_InitStructure.GPIO_Mode = (GPIOMode_TypeDef)GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init((GPIO_TypeDef *)ConfigLoader<Configure>::GPIOx, &GPIO_InitStructure);  


	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = baud;//一般设置为9600;
	USART_InitStructure.USART_WordLength = ConfigLoader<Configure>::USART_WordLength;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = ConfigLoader<Configure>::USART_StopBits;//一个停止位
	USART_InitStructure.USART_Parity = ConfigLoader<Configure>::USART_Parity;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = ConfigLoader<Configure>::USART_HardwareFlowControl;//无硬件数据流控制
	USART_InitStructure.USART_Mode = ConfigLoader<Configure>::USART_Mode;	//收发模式

	USART_Init((USART_TypeDef *)ConfigLoader<Configure>::GPIOx, &USART_InitStructure); //初始化串口

	USART_Cmd((USART_TypeDef *)ConfigLoader<Configure>::GPIOx, ENABLE);                    //使能串口 
};