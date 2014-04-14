#include  "usart.h"
#include "delay.h"
#include "doEvery.h"
int count;
void dodo()
{
	count++;
}
int main()
{
	doEvery(DoEvery_Ms,10,dodo);
	while(1);
	
}