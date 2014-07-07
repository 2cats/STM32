#include "usart.h"
#include "delay.h"
#include "stdlib.h"
#include "postman/Postman.h"
#include "Timer/timer.h"
#include "nrf/NRF.h"
struct AA
{
	char x[512];
};
AA aa;
int count;
int interg;
void receive_proc(Package *package)
{

		switch(package->type)
		{
			case 0x99:
				interg=parse(int,package->data);
				break;
			case 0x76:
				aa=parse(AA,package->data);
				break;
		}
		count++;

}

int main()
{
	delay_init();
	EXTIManagerTypeDef EXTIManager;
	EXTIManager_Structure(&EXTIManager);
	EXTIManager.startListenling(GPIOG,GPIO_Pin_8,EXTI_Falling,NRF_RecvHandler);
	NRF nrf;	
	Postman _postman(&nrf);
	nrf.setCallback(&_postman,receive_proc);

	while(1)
	{
		if(count>0x38)
		{
			nrf.setMode(NRF_TX_MODE);
			count=0;
			delay_ms(3000);
			while(1)
			{
				_postman.send(detail(aa),0x76);
			}
		}
		
	}
}