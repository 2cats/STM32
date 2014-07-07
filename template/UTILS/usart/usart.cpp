#include "usart.h"
template <Configures Configure>
inline Usart<Configure>::Usart(int baud)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(ConfigLoader<Configure>::RCC_USART, ENABLE);	
	RCC_APB2PeriphClockCmd(ConfigLoader<Configure>::RCC_GPIO, ENABLE);
	
	USART_DeInit((USART_TypeDef *)ConfigLoader<Configure>::GPIOx);
	//USART1_TX  
	GPIO_InitStructure.GPIO_Pin = ConfigLoader<Configure>::PIN_TX; 
	GPIO_InitStructure.GPIO_Speed =(GPIOSpeed_TypeDef) ConfigLoader<Configure>::SPEED;
	GPIO_InitStructure.GPIO_Mode =(GPIOMode_TypeDef) GPIO_Mode_AF_PP;	//�����������
	GPIO_Init((GPIO_TypeDef *)ConfigLoader<Configure>::GPIOx, &GPIO_InitStructure);
	//USART1_RX	  
	GPIO_InitStructure.GPIO_Pin = ConfigLoader<Configure>::PIN_RX;
	GPIO_InitStructure.GPIO_Mode = (GPIOMode_TypeDef)GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init((GPIO_TypeDef *)ConfigLoader<Configure>::GPIOx, &GPIO_InitStructure);  


	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = baud;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = ConfigLoader<Configure>::USART_WordLength;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = ConfigLoader<Configure>::USART_StopBits;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = ConfigLoader<Configure>::USART_Parity;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = ConfigLoader<Configure>::USART_HardwareFlowControl;//��Ӳ������������
	USART_InitStructure.USART_Mode = ConfigLoader<Configure>::USART_Mode;	//�շ�ģʽ

	USART_Init((USART_TypeDef *)ConfigLoader<Configure>::GPIOx, &USART_InitStructure); //��ʼ������

	USART_Cmd((USART_TypeDef *)ConfigLoader<Configure>::GPIOx, ENABLE);                    //ʹ�ܴ��� 
};
template <Configures Configure>
Usart<Configure>::Usart(int baud)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(ConfigLoader<Configure>::RCC_USART, ENABLE);	
	RCC_APB2PeriphClockCmd(ConfigLoader<Configure>::RCC_GPIO, ENABLE);
	
	USART_DeInit((USART_TypeDef *)ConfigLoader<Configure>::GPIOx);
	//USART1_TX  
	GPIO_InitStructure.GPIO_Pin = ConfigLoader<Configure>::PIN_TX; 
	GPIO_InitStructure.GPIO_Speed =(GPIOSpeed_TypeDef) ConfigLoader<Configure>::SPEED;
	GPIO_InitStructure.GPIO_Mode =(GPIOMode_TypeDef) GPIO_Mode_AF_PP;	//�����������
	GPIO_Init((GPIO_TypeDef *)ConfigLoader<Configure>::GPIOx, &GPIO_InitStructure);
	//USART1_RX	  
	GPIO_InitStructure.GPIO_Pin = ConfigLoader<Configure>::PIN_RX;
	GPIO_InitStructure.GPIO_Mode = (GPIOMode_TypeDef)GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init((GPIO_TypeDef *)ConfigLoader<Configure>::GPIOx, &GPIO_InitStructure);  


	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = baud;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = ConfigLoader<Configure>::USART_WordLength;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = ConfigLoader<Configure>::USART_StopBits;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = ConfigLoader<Configure>::USART_Parity;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = ConfigLoader<Configure>::USART_HardwareFlowControl;//��Ӳ������������
	USART_InitStructure.USART_Mode = ConfigLoader<Configure>::USART_Mode;	//�շ�ģʽ

	USART_Init((USART_TypeDef *)ConfigLoader<Configure>::GPIOx, &USART_InitStructure); //��ʼ������

	USART_Cmd((USART_TypeDef *)ConfigLoader<Configure>::GPIOx, ENABLE);                    //ʹ�ܴ��� 
};