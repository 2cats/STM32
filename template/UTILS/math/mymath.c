#include "mymath.h"
//发送时先发高位
//0		1		2		3		4			5		6				7	
//万	千	百	十	个 .	0.1	0.01	0.001
double char2double(u8 *str)
{
	volatile int i;
	volatile int H=0,T=1,L=0;
	for(i=4;i>=0;)
	{
		H=H+((str[i]-'0')*T);
		T=T*10;
		i=i-1;
	}
	T=1;
	for(i=7;i>=5;i=i-1)
	{
		L=L+((str[i]-'0')*T);
		T=T*10;
	}
	return H+L/1000.0;
	
	
}
//发送时先发高位
//0		1		2		3		4			5		6				7	
//万	千	百	十	个 .	0.1	0.01	0.001
void double2char(double d,char *str)
{
	int H=d,T=10000,L=(d-H)*10000;
	short i;
	for(i=0;i<=4;i++)
	{	
		str[i]=H/T+'0';
		H=H%T;
		T=T/10;
	}
	T=1000;;
	for(i=5;i<=7;i++)
	{
		str[i]=L/T+'0';
		L=L%T;
		T=T/10;
	}
}