#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "gui.h"
#include "math.h"

extern char *hdu;
int  GUI_JPEG_Draw        (const void * pFileData, int DataSize, int x0, int y0);
 int main(void)
 {	 
int i=0;
	delay_init();	    	 //延时函数初始化	  
 	LED_Init();			     //LED端口初始化
	 		
	GUI_Init();
GUI_SetFont(&GUI_Font8x16);
GUI_SetBkColor(GUI_BLUE);
GUI_Clear();
GUI_JPEG_Draw(hdu, sizeof(hdu), 0, 0);
}
