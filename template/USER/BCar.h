#ifndef H_BCAR_H
#define H_BCAR_H
#include "PWMManager.h" 
#define PWM_FREQ 20000
#define MOTORA1	TIM3CHANNEL1
#define MOTORA2	TIM3CHANNEL2
#define MOTORB1	TIM3CHANNEL3
#define MOTORB2	TIM3CHANNEL4
#define MOTOR_DEAD_LINE	0.15
//0.18
#define MOTOR_BACK_SCALE	1
#define MOTORB_OFFSET	0.02
#define CONTROL_SPEED 0.3
#define CONTROL_SPEED_FB 0.1
#define VIRTICAL_ANGLE 	0.05
void Motor_Init(void);
void WriteAllMotors(double ratio);
void StopAllMotors(void);
void WriteLRMotors(double L,double R);
u8 setPID(int p,int i,int d);
void BT_Talk(void);
#endif
