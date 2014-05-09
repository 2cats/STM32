#include "MPU6050.h"
#include "usart.h"
#include "delay.h"
#include "newPID.h"
#include "millis.h"
#include "BCar.h"
#include "filter.h"
#include "usmart.h"
#define PID_MAX	(1-MOTOR_DEAD_LINE)
#define PID_INTERVAL 10
ComplementaryFilterTypeDef ComplementaryFilter;

double PID_input,PID_output,PID_target=0.1;
PIDTypeDef PID;
MPUDataTypeDef_T sensorData;
void balanceProc(void)
{
  if(PID.Compute())//用角速度作为微分项进行PID运算
	{
//		if(PID_input>0.9||PID_input<-0.9)
//		StopAllMotors();
//		else
		WriteAllMotors(PID_output);
		//printf("%g,%g\n",sensorData.accx,PID_input);
		//	printf("out:%g,int:%g/n",PID_output,PID_input);
		MPU6050_GetTransformedData(&sensorData);
		//printf("%g,%g,%g  %g,%g,%g\n",sensorData.accx,sensorData.accy,sensorData.accz,sensorData.gyrox,sensorData.gyroy,sensorData.gyroz);
		//PID_input=KalmanFilter.getAngle(sensorData.accx,sensorData.gyroy,PID_INTERVAL);
		PID_input=ComplementaryFilter.getAngle( sensorData.accx,  sensorData.gyroy,  PID_INTERVAL/1000.0);

	}
	else
	{
		PID_target+1;
	}
}
void PIDConfigure()
{
	PID_Structure_Init(2,25,0.04,PID_INTERVAL,&PID);//10msPID计算周期2 40.02 //dead:0.15 12.11v     2,25,0.03
	PID.SetIOT(&PID_input,&PID_output,&PID_target);//IOT的值均可在函数外任意修改
	PID.SetOutputLimits(-PID_MAX,PID_MAX);
}
void All_Init()
{
	delay_init();
  uart_init(115200);
	MPU6050_ALL_Initialize();
	Motor_Init();
	PIDConfigure();
	MILLIS_Init();
	//Kalman_Init(&KalmanFilter);
//	delay_ms(300);
	MPU6050_GetTransformedData(&sensorData);
	//KalmanFilter.setStartAngle(sensorData.accy);
	ComplementaryFilter_Init(&ComplementaryFilter,2,0.9);
	usmart_init(0,115200);
}

int main()
{

	All_Init();
	while(1)
	{
		//WriteAllMotors(-0.07);
		balanceProc();
		usmart_scan();
		//MPU6050_GetTransformedData(&sensorData);
		//printf("%g,%g,%g  %g,%g,%g\n",sensorData.accx,sensorData.accy,sensorData.accz,sensorData.gyrox,sensorData.gyroy,sensorData.gyroz);
	}
}



