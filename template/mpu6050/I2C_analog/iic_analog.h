#ifndef _iic_analog_h_
#define _iic_analog_h_
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
//2014_3_18
////#define IIC_SUPER_TASK
//#ifdef IIC_SUPER_TASK
//#include "SuperTask.h"
//#endif 



//IO��������
#define SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
#define SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}

//IO��������	 
#define IIC_SCL    PBout(10) //SCL
#define IIC_SDA    PBout(11) //SDA	 
#define READ_SDA   PBin(11)  //����SDA 

void IIC_GPIO_Configuration( void);
//IIC ��ʱ
extern void IIC_Delay(void);
//IIC ��������
extern void IIC_Start(void);
//IIC ֹͣ����
extern void IIC_Stop(void);
//IIC ���Ͷ���
extern void IIC_SendACK(void);
//IIC ֹͣ����
extern void IIC_SendNACK(void);
//IIC ���͵��ֽ�
void IIC_Send_Byte(u8 Data);
//IIC ���յ��ֽ�
u8 IIC_Read_Byte(unsigned char ack);
//IIC д�뵥�ֽ�
extern void Single_Write_IIC(u8 SlaveAddress,u8 REG_Address,u8 REG_data);
//IIC ��ȡ���ֽ�
extern u8 Single_Read_IIC(u8 SlaveAddress, u8 REG_Address);


//GPIO ������
extern uint16_t GPIO_Filter( GPIO_TypeDef * GPIOx );
//////////////////////////
void IIC_WriteBit(uint8_t slaveAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data);
void IIC_WriteBits(uint8_t slaveAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
#endif

