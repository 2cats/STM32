#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//DS18B20驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/12
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
 extern "C" {
#endif

////////////////////////////////////////////////////////////////////
//DS18B20_Init->config GPIO
#define DS18B20_GPIO GPIOA
#define DS18B20_Pin	 11   	
#define	DS18B20_DQ_OUT PAout(DS18B20_Pin) //数据端口	PA0 
#define	DS18B20_DQ_IN  PAin(DS18B20_Pin)  //数据端口	PA0 
//#define DS18B20_GPIO_0_7 

////////////////////////////////////////////////////////////////////


//IO方向设置
#ifdef DS18B20_GPIO_0_7
#define DS18B20_IO_IN()  {DS18B20_GPIO->CRL&=~(0XF<<4*(DS18B20_Pin));DS18B20_GPIO->CRL|=8<<(4*DS18B20_Pin);}
#define DS18B20_IO_OUT() {DS18B20_GPIO->CRL&=~(0XF<<4*(DS18B20_Pin));DS18B20_GPIO->CRL|=3<<(4*DS18B20_Pin);}

#else 

#define DS18B20_IO_IN()  {GPIOG->CRH&=~(0XF<<4*(DS18B20_Pin-8));GPIOG->CRH|=8<<(4*(DS18B20_Pin-8));}
#define DS18B20_IO_OUT() {GPIOG->CRH&=~(0XF<<4*(DS18B20_Pin-8));GPIOG->CRH|=3<<(4*(DS18B20_Pin-8));}
#endif
									   


//IO方向设置
//#define DS18B20_IO_IN()  {GPIOG->CRH&=0XFFFF0FFF;GPIOG->CRH|=8<<12;}
//#define DS18B20_IO_OUT() {GPIOG->CRH&=0XFFFF0FFF;GPIOG->CRH|=3<<12;}
//////IO操作函数											   
//#define	DS18B20_DQ_OUT PGout(11) //数据端口	PA0 
//#define	DS18B20_DQ_IN  PGin(11)  //数据端口	PA0 

u8 DS18B20_Init(void);//初始化DS18B20
double DS18B20_Get_Temp(void);//获取温度
void DS18B20_Start(void);//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);//读出一个字节
u8 DS18B20_Read_Bit(void);//读出一个位
u8 DS18B20_Check(void);//检测是否存在DS18B20
void DS18B20_Rst(void);//复位DS18B20    
#ifdef __cplusplus
}
#endif
#endif





