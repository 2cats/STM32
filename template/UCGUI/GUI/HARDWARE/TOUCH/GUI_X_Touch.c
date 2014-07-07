#include "GUI.h"
#include "GUI_X.h"

#include "touch.h" 

void GUI_TOUCH_X_ActivateX(void) {    //不用配置
}

void GUI_TOUCH_X_ActivateY(void) {  //不用配置
}

int  GUI_TOUCH_X_MeasureX(void) {

	u16 var = TP_Read_XOY(CMD_RDX);  //读取X轴的AD转换值  不是坐标值

	//printf("\r\n MeasureX is %d \r\n",var);

  	return var;		  
}

int  GUI_TOUCH_X_MeasureY(void) {

	u16 var = TP_Read_XOY(CMD_RDY);   //读取Y轴的AD转换值

	//printf("\r\n MeasureY is %d \r\n",var);

  	return var;		
}
