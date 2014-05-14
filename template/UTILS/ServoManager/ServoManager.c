#include "ServoManager.h"
//舵机驱动

//TIM_FREQ_DIV>=22
static ServoManagerTypeTypeDef *defaultServoManager[16];
static PWMManagerTypeDef defaultServoPWMManager;
static u8 _defaultServoPWMManagerInitialedFlag=0;
void ServoManager_ChangeAngle(ServoManagerTypeTypeDef *self,double angle)
{
	defaultServoPWMManager.changeRatio(self->route,angle*4.166667e-4+0.025);//频率不变
}
//占空比
//0.025--->0.1
void ServoManager_Struc_Init(ServoManagerTypeTypeDef *ServoManager,TIM_RouteTypeDef route)
{	
	defaultServoManager[route]=ServoManager;
	defaultServoManager[route]->route=route;
	defaultServoManager[route]->changeAngle=ServoManager_ChangeAngle;
	if(!_defaultServoPWMManagerInitialedFlag)
	{
		PWMManagerStructure(&defaultServoPWMManager);
		_defaultServoPWMManagerInitialedFlag=1;
	}
	defaultServoPWMManager.ProduceNewPWM(route,50,0);
}

