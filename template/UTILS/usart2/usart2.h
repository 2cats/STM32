#ifndef __USART2_H
#define __USART2_H
#include "sys.h" 
#include <stdarg.h>
#define EN_USART2_RX 			0		//ʹ�ܣ�1��/��ֹ��0������1����
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart2_init(u32 bound);
void uart2_sendChar(u8 ch); 
void uart2_print(char *str);
void uart2_println(u8 *str);
void uart2_printf (char *fmt, ...);
int uart2_scanf(const char *fmt,...);
#endif
