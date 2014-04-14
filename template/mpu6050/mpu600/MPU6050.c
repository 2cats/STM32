#include "stm32f10x.h"
#include "MPU6050.h"
#include "iic_analog.h"
//2014_3_18
//2CATS
void MPU6050_ALL_Initialize(void)
{
	IIC_GPIO_Configuration( IIC_GOIO_SDA , IIC_SDA , IIC_GPIO_SCL , IIC_SCL );//初始化IIC硬件
	MPU6050_Initialize();//初始化MPU6050
}
char getWho(void)
{
	char AccXH = 0 ;

	AccXH = Single_Read_IIC( SLAVEADRESS , WHO_AM_I );

	return AccXH;
}
short getAccX(void)
{
	short AccX = 0;
	char AccXH = 0 , AccXL = 0;

	AccXH = Single_Read_IIC( SLAVEADRESS , ACCEL_XOUT_H );
	AccXL = Single_Read_IIC( SLAVEADRESS , ACCEL_XOUT_L );

	AccX = (AccXH<<8)|AccXL;

	return AccX;
}

short getAccY(void)
{
	short AccY = 0;
	char AccYH = 0 , AccYL = 0;

	AccYH = Single_Read_IIC( SLAVEADRESS , ACCEL_YOUT_H );
	AccYL = Single_Read_IIC( SLAVEADRESS , ACCEL_YOUT_L );

	AccY = (AccYH<<8)|AccYL;

	return AccY;
}

short getAccZ(void)
{
	short AccZ = 0;
	char AccZH = 0 , AccZL = 0;

	AccZH = Single_Read_IIC( SLAVEADRESS , ACCEL_ZOUT_H );
	AccZL = Single_Read_IIC( SLAVEADRESS , ACCEL_ZOUT_L );

	AccZ = (AccZH<<8)|AccZL;

	return AccZ;
}

short getGyroX(void)
{
	short GyroX = 0;
	char GyroXH = 0 , GyroXL = 0; 
	
	GyroXH = Single_Read_IIC( SLAVEADRESS , GYRO_XOUT_H );
	GyroXL = Single_Read_IIC( SLAVEADRESS , GYRO_XOUT_L );
	
	GyroX = (GyroXH<<8)|GyroXL;
	
	return GyroX;	
}

short getGyroY(void)
{
   	short GyroY = 0;
	char GyroYH = 0 , GyroYL = 0; 
	
	GyroYH = Single_Read_IIC( SLAVEADRESS , GYRO_YOUT_H );
	GyroYL = Single_Read_IIC( SLAVEADRESS , GYRO_YOUT_L );
	
	GyroY = (GyroYH<<8)|GyroYL;
	
	return GyroY;	
}

short getGyroZ(void)
{
   	short GyroZ = 0;
	char GyroZH = 0 , GyroZL = 0; 
	
	GyroZH = Single_Read_IIC( SLAVEADRESS , GYRO_ZOUT_H );
	GyroZL = Single_Read_IIC( SLAVEADRESS , GYRO_ZOUT_L );
	
	GyroZ = (GyroZH<<8)|GyroZL;
	
	return GyroZ;	
}

short getTemperature(void)
{
 	short temperature = 0;
	char temperatureH = 0 , temperatureL = 0;

	temperatureH = Single_Read_IIC( SLAVEADRESS , TEMP_OUT_H );
	temperatureL = Single_Read_IIC( SLAVEADRESS , TEMP_OUT_L );

	temperature = (temperatureH<<8)|temperatureL;

	return temperature;
}

void MPU6050_SetClockSource(uint8_t source) 
{
		IIC_WriteBits(SLAVEADRESS, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
}
void MPU6050_SetFullScaleGyroRange(uint8_t range) 
{
    IIC_WriteBits(SLAVEADRESS, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}
void MPU6050_SetFullScaleAccelRange(uint8_t range) 
{
    IIC_WriteBits(SLAVEADRESS, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}
void MPU6050_SetSleepModeStatus(FunctionalState NewState) 
{
		IIC_WriteBit(SLAVEADRESS, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, NewState);
}
void MPU6050_Initialize() 
{
	  MPU6050_SetSleepModeStatus(DISABLE); 
    MPU6050_SetClockSource(MPU6050_CLOCK_PLL_XGYRO);
    MPU6050_SetFullScaleGyroRange(MPU6050_GYRO_FS_250);
    MPU6050_SetFullScaleAccelRange(MPU6050_ACCEL_FS_2);
//	Single_Write_IIC(SLAVEADRESS,SMPLRT_DIV,0x07);

}
void MPU6050_GetCarRawData(MPUDataTypeDef_R* mpudata)
{
	mpudata->accx=getAccX();
	mpudata->accy=getAccZ();
	mpudata->accz=getAccY();
	mpudata->gyrox=getGyroX();
	mpudata->gyroy=getGyroZ();
	mpudata->gyroz=getGyroY();
}
void MPU6050_GetTransformedData(MPUDataTypeDef_T* mpudata)//ACC->g   GYRO->rad/s
{
	mpudata->accx=getAccX()/16384.0;
	mpudata->accy=getAccZ()/16384.0;
	mpudata->accz=getAccY()/16384.0;
	mpudata->gyrox=getGyroX()*0.0001332312;
	mpudata->gyroy=getGyroZ()*0.0001332312;
	mpudata->gyroz=getGyroY()*0.0001332312;
}
