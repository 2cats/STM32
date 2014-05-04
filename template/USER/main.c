#include  "usart2.h"
#include "delay.h"
#include "Protocol.h"
int main()
{
	uart2_init(115200);
	PT_EnableIRQ();
	delay_init();
	while(1)
	{
		delay_ms(1000);
	}
}
