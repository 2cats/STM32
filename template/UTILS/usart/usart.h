#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			0		//ʹ�ܣ�1��/��ֹ��0������1�����ж�
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
#endif


