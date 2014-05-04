#include  "usart2.h"
#include "delay.h"
#include "Protocol.h"
#include "ServoManager.h"
ServoManagerTypeTypeDef  ServoManager;
int main()
{
	uart2_init(115200);
	PT_EnableIRQ();
	delay_init();
	ServoManager_Struc_Init(&ServoManager,TIM1CHANNEL1);
	ServoManager.changeAngle(&ServoManager,90);
	while(1)
	{
		delay_ms(1000);
	}
}
