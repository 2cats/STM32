#include "usart.h"
int main()
{
	Usart<USART1_Conf> usart1(9600);
	int x=sizeof(int);
}
