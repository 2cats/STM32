#include "MPU6050.h"
#include "usart.h"
#include "delay.h"
#include "newPID.h"
#include "millis.h"
#include "BCar.h"
#include "filter.h"
#include "usmart.h"
#include "USART2BufReceiever.h" 
#include "InputCatcher.h"
#include "HX711.h"
#include "gui.h"
#include "24cxx.h" 
#include "HX711.h"
#include "IRremote.h"
#include "24l01.h"

int main()
{
	u8 tmp_buf[33]={"aaabbbcccddd"};		 //���һ���ֽ���������ַ���������
	NRF24L01_Init(); 	
	while(NRF24L01_Check())	//��������
	{
	}
////////////////////////////////////////////////
	NRF24L01_RX_Mode();	//����ģʽ
	while(1)
	{
			if(NRF24L01_RxPacket(tmp_buf)==0)
			{
					//..
			}
	}

}



