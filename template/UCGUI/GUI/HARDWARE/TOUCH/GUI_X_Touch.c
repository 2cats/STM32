#include "GUI.h"
#include "GUI_X.h"

#include "touch.h" 

void GUI_TOUCH_X_ActivateX(void) {    //��������
}

void GUI_TOUCH_X_ActivateY(void) {  //��������
}

int  GUI_TOUCH_X_MeasureX(void) {

	u16 var = TP_Read_XOY(CMD_RDX);  //��ȡX���ADת��ֵ  ��������ֵ

	//printf("\r\n MeasureX is %d \r\n",var);

  	return var;		  
}

int  GUI_TOUCH_X_MeasureY(void) {

	u16 var = TP_Read_XOY(CMD_RDY);   //��ȡY���ADת��ֵ

	//printf("\r\n MeasureY is %d \r\n",var);

  	return var;		
}
