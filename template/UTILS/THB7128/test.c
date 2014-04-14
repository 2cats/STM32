
#include "THB7128.h"

u8 count;
void doWhenDone()
{
	if(count==5)
	{
		THB7128_Disable();
		THB7128_SetDoWhenDone(0);
		return ;
	}

	if(count++%2)
		THB7128_ClockWise();
	else
	{
		THB7128_AntiClockWise();

	}	
	THB7128_ChangeFreq(count*1000);
	THB7128_PWM_Out(THB7128_STEPS_PER_CIRCLE);
}
int main()
{
	THB7128_Init(THB7128_CLOCKWISE,3000);
	THB7128_SetDoWhenDone(doWhenDone);
	THB7128_PWM_Out(THB7128_STEPS_PER_CIRCLE);
	while(1);
}
