#include "MPU6050.h"
#include "usart.h"
#include "delay.h"
#include "newPID.h"
#include "millis.h"
#include "BCar.h"
#include "filter.h"
#include "usmart.h"
#include "USART2BufReceiever.h" 
#define PID_MAX	(1-MOTOR_DEAD_LINE)
#define PID_INTERVAL 10
ComplementaryFilterTypeDef ComplementaryFilter;
double PWMcontol_L,PWMcontol_R;
double PID_input,PID_output,PID_target=VIRTICAL_ANGLE;
USART2BufReceieverTypeDef USART2BufReceiever;
PIDTypeDef PID;
MPUDataTypeDef_T sensorData;
void balanceProc(void)
{
  if(PID.Compute())//用角速度作为微分项进行PID运算
	{
		WriteLRMotors(PID_output+PWMcontol_L,PID_output+PWMcontol_R);
		MPU6050_GetTransformedData(&sensorData);
		PID_input=ComplementaryFilter.getAngle( sensorData.accx,  sensorData.gyroy,  PID_INTERVAL/1000.0);
	}
}
void PIDConfigure()
{
	PID_Structure_Init(2,15,0.04,PID_INTERVAL,&PID);//10msPID计算周期2 40.02 //dead:0.15 12.11v     2,25,0.03
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
	MPU6050_GetTransformedData(&sensorData);
	ComplementaryFilter_Init(&ComplementaryFilter,2,0.9);
	uart2_init_BR(115200,&USART2BufReceiever);
}
 
int main()
{
	All_Init();
	while(1)
	{
		balanceProc();
		BT_Talk(); 
	}
}



