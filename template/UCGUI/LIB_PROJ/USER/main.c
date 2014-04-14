
//#include "delay.h"
//#include "sys.h"
//#include "usart.h"
#include "gui.h"
#include "JLX12864G.h"
#include "FRAMEWIN.h"

 int main(void)
 {
//	SystemInit();
//	delay_init(72);	  //延时初始化
//	LCD_Init();
	GUI_Init();
//	 GUI_SetFont(&GUI_FontHZ12x12);
//  GUI_DispStringAt("通信工程", 3, 3);
////GUI_DrawCircle(30,30,5);
//	  FRAMEWIN_Create("Frame window",NULL,WM_CF_SHOW,10,10,50,20);
//	 
//	 GUI_SetBkColor(GUI_WHITE);
//GUI_Clear();
WM_CreateWindow(15, 2, 100, 50,WM_CF_SHOW, 0,0);
 GUI_Delay(1000);
	while(1)
	{
	//	delay_ms(400);
			
	}			 	
   

 }


