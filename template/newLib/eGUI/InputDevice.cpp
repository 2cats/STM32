#include "InputDevice.h"
void OnKey(MenuWidget *menuWidget,unsigned char c)
{
	int type=KEY_NULL;
	switch(c)
	{
		case 1:type=KEY_1;break;
		case 2:type=KEY_2;break;
		case 3:type=KEY_3;break;
		case 4:type=KEY_UP;break;
		case 5:type=KEY_4;break;
		case 6:type=KEY_5;break;
		case 7:type=KEY_6;break;
		case 8:type=KEY_DOWN;break;
		case 9:type=KEY_7;break;
		case 10:type=KEY_8;break;
		case 11:type=KEY_9;break;
		case 12:type=KEY_LEFT;break;
		case 13:type=KEY_OK;break;
		case 14:type=KEY_0;break;
		case 15:type=KEY_ESC;break;
		case 16:type=KEY_RIGHT;break;
	}
	GObject::sendMessage(menuWidget,type);
}