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
USART2BufReceieverTypeDef USARTBufReceiever;
extern u8 FB_ENABLED;
double PID_input,PID_output,PID_target=0.001;
PIDTypeDef PID;
MPUDataTypeDef_T sensorData;
double PWMcontol_L=0,PWMcontol_R=0;
void balanceProc(void)
{
  if(PID.Compute())//用角速度作为微分项进行PID运算
	{
		//WriteAllMotors(PID_output+PWMcontol);
//		uart2_printf("hello\n");
//		if(FB_ENABLED)
//		{
//			PWMcontol_L+=CONTROL_SPEED_FB;
//			PWMcontol_R=PWMcontol_L;
//		}
		
		WriteLRMotors(PID_output+PWMcontol_L,PID_output+PWMcontol_R);
		MPU6050_GetTransformedData(&sensorData);
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
//uart_init(115200);
	uart2_init_BR(115200,&USARTBufReceiever);
	MPU6050_ALL_Initialize();
	Motor_Init();
	PIDConfigure();
	MILLIS_Init();
	//Kalman_Init(&KalmanFilter);
//	delay_ms(300);
	MPU6050_GetTransformedData(&sensorData);
	//KalmanFilter.setStartAngle(sensorData.accy);
	ComplementaryFilter_Init(&ComplementaryFilter,2,0.9);
	//usmart_init(0,115200);
}
u32 time=0;
int main()
{

	All_Init();
	while(1)
	{
		balanceProc();
		 BT_Talk();
		if(millis()-time>1000)
		{
			time=millis();
			uart2_printf("%4g,%4g\n",PWMcontol_L,PWMcontol_R);
		}
		//usmart_scan();
	}
}