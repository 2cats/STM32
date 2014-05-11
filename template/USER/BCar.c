#include "BCar.h"
#include "usart.h"
#include "newPID.h"
#include "USART2BufReceiever.h" 
PWMManagerTypeDef PWMManager;

u8 FB_ENABLED=0;
extern PIDTypeDef PID;
extern USART2BufReceieverTypeDef USARTBufReceiever;
extern double PWMcontol_L,PWMcontol_R;
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
	if(ratio>0)
	{
		ratio+=(MOTOR_DEAD_LINE);
		PWMManager.changeRatio(MOTORA1,ratio);
		PWMManager.changeRatio(MOTORB1,ratio-MOTORB_OFFSET);
		PWMManager.changeRatio(MOTORA2,0);
		PWMManager.changeRatio(MOTORB2,0);
	}
	else
	{
		ratio-=MOTOR_DEAD_LINE;
		ratio=-ratio;
		PWMManager.changeRatio(MOTORA2,ratio);
		PWMManager.changeRatio(MOTORB2,ratio-MOTORB_OFFSET);
		PWMManager.changeRatio(MOTORA1,0);
		PWMManager.changeRatio(MOTORB1,0);
	}
}
void WriteLRMotors(double L,double R)
{
	if(L>0)
	{
		L+=(MOTOR_DEAD_LINE);
		PWMManager.changeRatio(MOTORA1,L);
		PWMManager.changeRatio(MOTORA2,0);
	}
	else
	{
		L-=MOTOR_DEAD_LINE;
		L=-L;
		PWMManager.changeRatio(MOTORA2,L);
		PWMManager.changeRatio(MOTORA1,0);
	}
	if(R>0)
	{
		R+=(MOTOR_DEAD_LINE);
		PWMManager.changeRatio(MOTORB1,R-MOTORB_OFFSET);
		PWMManager.changeRatio(MOTORB2,0);
	}
	else
	{
		R-=MOTOR_DEAD_LINE;
		R=-R;
		PWMManager.changeRatio(MOTORB2,R-MOTORB_OFFSET);
		PWMManager.changeRatio(MOTORB1,0);
	}
}
void StopAllMotors(void)
{
		PWMManager.changeRatio(MOTORB1,0);
		PWMManager.changeRatio(MOTORB1,0);
		PWMManager.changeRatio(MOTORB2,0);
		PWMManager.changeRatio(MOTORB2,0);
}
u8 setPID(int p,int i,int d)
{
	PID.SetPID(p/1000.0,i/1000.0,d/1000.0);
	return 'k';
}
#define CONTROL_STOP	'9'
#define CONTROL_FORW	'0'
#define CONTROL_BACK	'1'
#define CONTROL_LEFT	'3'
#define CONTROL_RIGHT	'2'
void BT_Talk()
{
	if(USARTBufReceiever.getAvaliableNum()>0)
	{
		switch(USARTBufReceiever.getByte())
		{
			case CONTROL_STOP:
				PWMcontol_L=PWMcontol_R=0;
				FB_ENABLED=0;
				break;
			case CONTROL_FORW:
				PWMcontol_L=PWMcontol_R=CONTROL_SPEED_FB;
				FB_ENABLED=1;
				break;
			case CONTROL_BACK:
				PWMcontol_L=PWMcontol_R=-CONTROL_SPEED_FB;
				FB_ENABLED=1;
				break;
			case CONTROL_LEFT:
					PWMcontol_L=CONTROL_SPEED;
					PWMcontol_R=0;
					FB_ENABLED=0;
				break;
			case CONTROL_RIGHT:
					PWMcontol_L=0;
					PWMcontol_R=CONTROL_SPEED;
					FB_ENABLED=0;
				break;
		}
	}
}