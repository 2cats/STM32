#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//DS18B20��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
 extern "C" {
#endif

////////////////////////////////////////////////////////////////////
//DS18B20_Init->config GPIO
#define DS18B20_GPIO GPIOA
#define DS18B20_Pin	 11   	
#define	DS18B20_DQ_OUT PAout(DS18B20_Pin) //���ݶ˿�	PA0 
#define	DS18B20_DQ_IN  PAin(DS18B20_Pin)  //���ݶ˿�	PA0 
//#define DS18B20_GPIO_0_7 

////////////////////////////////////////////////////////////////////


//IO��������
#ifdef DS18B20_GPIO_0_7
#define DS18B20_IO_IN()  {DS18B20_GPIO->CRL&=~(0XF<<4*(DS18B20_Pin));DS18B20_GPIO->CRL|=8<<(4*DS18B20_Pin);}
#define DS18B20_IO_OUT() {DS18B20_GPIO->CRL&=~(0XF<<4*(DS18B20_Pin));DS18B20_GPIO->CRL|=3<<(4*DS18B20_Pin);}

#else 

#define DS18B20_IO_IN()  {GPIOG->CRH&=~(0XF<<4*(DS18B20_Pin-8));GPIOG->CRH|=8<<(4*(DS18B20_Pin-8));}
#define DS18B20_IO_OUT() {GPIOG->CRH&=~(0XF<<4*(DS18B20_Pin-8));GPIOG->CRH|=3<<(4*(DS18B20_Pin-8));}
#endif
									   


//IO��������
//#define DS18B20_IO_IN()  {GPIOG->CRH&=0XFFFF0FFF;GPIOG->CRH|=8<<12;}
//#define DS18B20_IO_OUT() {GPIOG->CRH&=0XFFFF0FFF;GPIOG->CRH|=3<<12;}
//////IO��������											   
//#define	DS18B20_DQ_OUT PGout(11) //���ݶ˿�	PA0 
//#define	DS18B20_DQ_IN  PGin(11)  //���ݶ˿�	PA0 

u8 DS18B20_Init(void);//��ʼ��DS18B20
double DS18B20_Get_Temp(void);//��ȡ�¶�
void DS18B20_Start(void);//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);//����һ���ֽ�
u8 DS18B20_Read_Bit(void);//����һ��λ
u8 DS18B20_Check(void);//����Ƿ����DS18B20
void DS18B20_Rst(void);//��λDS18B20    
#ifdef __cplusplus
}
#endif
#endif





