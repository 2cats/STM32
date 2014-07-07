#include "mygui.h"
void GUI_AllInit(Timer *timer,GUI_COLOR bkColor)//timer 1tick=1ms
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	TP_Init();
	timer->doEvery(10,GUI_TOUCH_Exec);
	GUI_Init();
	GUI_SetBkColor(bkColor);
	GUI_Clear();
}