#ifndef _SERVOMANAGER_H_
#define _SERVOMANAGER_H_
#include "PWMManager.h"
typedef struct ServoManagerType{
	void(*changeAngle)(struct ServoManagerType *self,double angle);
	TIM_RouteTypeDef route;
}ServoManagerTypeTypeDef;
void ServoManager_ChangeAngle(ServoManagerTypeTypeDef *self,double angle);
void ServoManager_Struc_Init(ServoManagerTypeTypeDef *ServoManager,TIM_RouteTypeDef route);

#endif
