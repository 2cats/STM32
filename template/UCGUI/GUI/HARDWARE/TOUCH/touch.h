#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "sys.h"
#ifdef __cplusplus
 extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//触摸屏驱动（支持ADS7843/7846/UH7843/7846/XPT2046/TSC2046等） 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/11
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

#define TP_PRES_DOWN 0x80  //触屏被按下	  
#define TP_CATH_PRES 0x40  //有按键按下了 	  
										    
//与触摸屏芯片连接引脚	   
#define PEN  	PFin(10)  	//PF10 INT
#define DOUT 	PFin(8)   	//PF8  MISO
#define TDIN 	PFout(9)  	//PF9  MOSI
#define TCLK 	PBout(1)  	//PB1  SCLK
#define TCS  	PBout(2)  	//PB2  CS  
     
#define  CMD_RDX	0XD0
#define  CMD_RDY  0X90

void TP_Write_Byte(u8 num);						//向控制芯片写入一个数据
u16 TP_Read_AD(u8 CMD);							//读取AD转换值
u16 TP_Read_XOY(u8 xy);							//带滤波的坐标读取(X/Y)
u8 TP_Read_XY(u16 *x,u16 *y);					//双方向读取(X+Y)
void TP_Init(void);
#ifdef __cplusplus
}
#endif
#endif




