#include "display.h"
#include "JLX12864G.h"
u8 KeyOperateFlag=3;
unsigned char GUI_NEEDING_UPDATE_FLAG=0;

void initData_Display(void)
{
	OLED_printfAt(1,1,"Temprature:");
	OLED_printfAt(2,1,"Power:");
	OLED_printfAt(3,1,"Speed:");
	OLED_printfAt(4,1,"Target:");
	OLED_printfAt(5,1,"Output:");

}
void update_Temprature_Display(double tempt)
{
	OLED_printfAt(1,DATA_POSITION,"%f",tempt);
}
void initKey_Display(void)
{
	OLED_printfAt(8,1,"Tmp ");
	OLED_printfAt(8,110,"Fan");
}
void configTmpKey_Display()
{	
	if(!IS_TMP_CONFIGUREING)
		return;
	OLED_printfAt(8,1,"T... ");
}
void configMotorKey_Display()
{	
	if(!IS_MOTOR_CONFIGUREING)
		return;
	SET_MOTOR_CONFIGUREING;
	OLED_printfAt(8,1,"F... ");
}
void setTarget_Display(int target)
{
	OLED_printfAt(4,DATA_POSITION,"%f",(double)target);
}
void setMotor_Display(int target)
{
	//OLED_printfAt(4,DATA_POSITION,"%d    ",target);
}
