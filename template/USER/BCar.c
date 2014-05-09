#include "BCar.h"
#include "usart.h"
#include "newPID.h"
PWMManagerTypeDef PWMManager;
extern PIDTypeDef PID;
void Motor_Init(void)
{
		PWMManagerStructure(&PWMManager);
		PWMManager.ProduceNewPWM(MOTORA1,PWM_FREQ,0);
		PWMManager.ProduceNewPWM(MOTORA2,PWM_FREQ,0);
		PWMManager.ProduceNewPWM(MOTORB1,PWM_FREQ,0);
		PWMManager.ProduceNewPWM(MOTORB2,PWM_FREQ,0);
}
void WriteAllMotors(double ratio)
{
//	if(ratio<0.23&&ratio>-0.23)
//		StopAllMotors();
	if(ratio>0)
	{
		ratio+=(MOTOR_DEAD_LINE);
		ratio=ratio*MOTOR_BACK_SCALE;
	//	printf("+:%g\n",ratio);
		PWMManager.changeRatio(MOTORA1,ratio);
		PWMManager.changeRatio(MOTORB1,ratio-MOTORB_OFFSET);
		PWMManager.changeRatio(MOTORA2,0);
		PWMManager.changeRatio(MOTORB2,0);
	}
	else
	{
		ratio-=MOTOR_DEAD_LINE;
		ratio=-ratio;
	//	printf("-:%g\n",ratio);
		PWMManager.changeRatio(MOTORA2,ratio);
		PWMManager.changeRatio(MOTORB2,ratio-MOTORB_OFFSET);
		PWMManager.changeRatio(MOTORA1,0);
		PWMManager.changeRatio(MOTORB1,0);
	}
}
void StopAllMotors(void)
{
		PWMManager.changeRatio(MOTORA1,0);
		PWMManager.changeRatio(MOTORB1,0);
		PWMManager.changeRatio(MOTORA2,0);
		PWMManager.changeRatio(MOTORB2,0);
}
u8 setPID(int p,int i,int d)
{
	PID.SetPID(p/1000.0,i/1000.0,d/1000.0);
	return 'k';
}
