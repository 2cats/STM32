#include "usart.h"
#include "delay.h"
#include "postman/Postman.h"
#include "string.h"
#include "nrf/NRF.h"
Package package;
struct AA
{
	char x[512];
}aa;
int count;
char dst[5];
int interg=8889;
char src[]="hello";
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
	EXTIManagerTypeDef EXTIManager;
	delay_init();
	
	aa.x[0]='a';
	aa.x[29]='z';

	EXTIManager_Structure(&EXTIManager);
	EXTIManager.startListenling(GPIOG,GPIO_Pin_8,EXTI_Falling,NRF_RecvHandler);
	Usart usart1(USART1_Conf,9600);
	NRF nrf(NRF_TX_MODE);
	Postman postman(&nrf);
	nrf.setCallback(&postman,receive_proc);
	while(1)
	{
	//	postman.send(detail(interg),0x99);
		postman.send(detail(aa),0x76);
		count++;
		if(count>0x3f)
		{
			nrf.setMode(NRF_RX_MODE);
			count=0;
			while(1)
			{
			}
		}
	}
}