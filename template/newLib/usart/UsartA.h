#include "usart.h"
#include "delay.h"
#include "stdlib.h"
#include "postman/Postman.h"
#include "Timer/timer.h"
#include "nrf/NRF.h"
struct AA
{
	char x[1024];
};
AA aa;
int count;
int interg;
Package package;

int main()
{
	delay_init();

	Usart usart1(USART1_Conf,9600);	
	Postman postman(&usart1);

	while(1)
	{
		postman.send(detail(aa),0x96);
		if(postman.receive(&package))
		{
			switch(package.type)
			{
				case 0x99:
				
					break;
				case 0x76:
				
					break;
			}
		}
		count++;
		
	}
}