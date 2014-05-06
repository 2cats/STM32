#ifndef _iic_analog_h_
#define _iic_analog_h_
#include "stm32f10x.h"
//2014_3_18

#define IIC_SUPER_TASK
#ifdef IIC_SUPER_TASK
#include "SuperTask.h"
#endif
/*********************************************************************************/
/*�޸�ģ��IIC�Ķ�ȡ�����Լ����ŵĶ˿ں�											 */
/*��Щ�궨�嶨������Ժ����ų�ʼ�����������г�ʼ��ʱ��ʹ�ܵȱ�Ҫ�Ĳ���			 */
/*********************************************************************************/
#define IIC_GPIO (GPIOB)
#define IIC_GOIO_SDA (GPIOB)
#define IIC_GPIO_SCL (GPIOB)
#define IIC_SDA (GPIO_Pin_11)
#define IIC_SCL (GPIO_Pin_10)
/*********************************************************************************/




/************************************************************************************/
/*ʹ�ô˺��� ��ʼ�� ģ��IIC  ���в��� �����Ϻ궨���� �ж��� ʹ��ʱֻ��Ҫ�޸ĺ궨�弴�� */
/* �˺�������ʱ �븴�����´��� 
	IIC_GPIO_Configuration( IIC_GOIO_SDA , IIC_SDA , IIC_GPIO_SCL , IIC_SCL );  */
/************************************************************************************/
//IIC ��������
extern void IIC_GPIO_Configuration( GPIO_TypeDef * GPIOx_SDA , uint16_t SDA_Pin , GPIO_TypeDef * GPIOx_SCL , uint16_t SCL_Pin );






/*********************************************************************************/
/*ʹ�����´���ʱ�����޸�														 */
/*																				 */
/*********************************************************************************/
//ʹ�����ģ��I2C
#define SET_SDA		{ GPIO_SetBits( IIC_GPIO , IIC_SDA ); }
#define RESET_SDA	{ GPIO_ResetBits( IIC_GPIO , IIC_SDA );}
#define SET_SCL		{ GPIO_SetBits( IIC_GPIO , IIC_SCL ); }
#define RESET_SCL 	{ GPIO_ResetBits( IIC_GPIO , IIC_SCL); }
#define IIC_SDA_STATE (IIC_GPIO->IDR&IIC_SDA)
#define IIC_SCL_STATE (IIC_GPIO->IDR&IIC_SDA)

#define IIC_DELAY { IIC_Delay(); }

enum IIC_REPLAY_ENUM
{
	IIC_NACK = 0,
	IIC_ACK = 1
};

enum IIC_BUS_STATE_ENUM
{
	IIC_BUS_READY = 0,
	IIC_BUS_BUSY=1,
	IIC_BUS_ERROR=2
};

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
extern u8 IIC_SendByte(u8 Data);
//IIC ���յ��ֽ�
extern u8 IIC_RecvByte(void);
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

