//#include "MKeyScan/MKeyScan.h"
#include "delay.h"
#include "timer/Timer.h"
#include "InputCatcher.h"
#include "millis.h"
#include "util/util.h"
#include "gui.h"
#include "touch.h" 
#include "WM.h"
#include "MENU.h"
#include "FRAMEWIN.h"

void MainTask(void);
int main()
{

	delay_init();
	Timer timer4(TIM4_Conf);
	TP_Init();
	timer4.doEvery(10,GUI_TOUCH_Exec);
	GUI_Init();
	GUI_SetBkColor(GUI_RED);
	GUI_Clear();
	MainTask();
	while(1)
	{
		GUI_Exec();
	}
}