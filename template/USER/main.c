#include  "usart.h"
#include "delay.h"
#include "MPU6050.h"
#include "SuperTask.h"
MPUDataTypeDef_T mpudata;
int main(void)
{
	uart_init(115200);
	MPU6050_ALL_Initialize();

	while(1)
	{
			MPU6050_GetTransformedData(&mpudata);
	 		printf("%g,%g,%g\n",mpudata.accx,mpudata.accy,mpudata.accz);
	}
}
