#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "sys.h"
#ifdef __cplusplus
 extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//������������֧��ADS7843/7846/UH7843/7846/XPT2046/TSC2046�ȣ� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/11
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

#define TP_PRES_DOWN 0x80  //����������	  
#define TP_CATH_PRES 0x40  //�а��������� 	  
										    
//�봥����оƬ��������	   
#define PEN  	PFin(10)  	//PF10 INT
#define DOUT 	PFin(8)   	//PF8  MISO
#define TDIN 	PFout(9)  	//PF9  MOSI
#define TCLK 	PBout(1)  	//PB1  SCLK
#define TCS  	PBout(2)  	//PB2  CS  
     
#define  CMD_RDX	0XD0
#define  CMD_RDY  0X90

void TP_Write_Byte(u8 num);						//�����оƬд��һ������
u16 TP_Read_AD(u8 CMD);							//��ȡADת��ֵ
u16 TP_Read_XOY(u8 xy);							//���˲��������ȡ(X/Y)
u8 TP_Read_XY(u16 *x,u16 *y);					//˫�����ȡ(X+Y)
void TP_Init(void);
#ifdef __cplusplus
}
#endif
#endif




