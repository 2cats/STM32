#include "display.h"
#include "keyscan.h"
#include "newPID.h" 
#include "24cxx.h" 
#include "JLX12864G.h"
#include "include.h"
u8 Key_Value;
volatile u32 FSM_VAL;
u8 CfgPN;
u8 Key_Cmd_Value[8];
u8 Key_Cmd_Length;
int disp__i;
#define CURVE_MAX_LEN	50
GUI_POINT aPoints[CURVE_MAX_LEN];
u8 curve_current_len;

extern PIDTypeDef PID;
volatile char DISP_Update;//是否需要更新显示
extern double PID_target;
extern double PID_input;
extern double PID_output;
extern double speed;
extern double PID_alarm_MIN;
extern double PID_alarm_MAX;

double  Curve_Per_Len;
double _tmp;
GUI_POINT curvePoints[CURVE_MAX_LEN];
u8 Curve_Top=1;

u8 getKeyMathValue()
{
	if(Key_Cmd_Length==0)
		return ' ';//!
	else
		return (Key_Cmd_Length==2?Key_Cmd_Value[0]*10+Key_Cmd_Value[1]:Key_Cmd_Value[0]);
}

int getValue()
{
	switch(Key_Cmd_Length)
	{
		case 0:
			return -1;
		case 1:
			return Key_Cmd_Value[0];
		case 2:
			return Key_Cmd_Value[0]*10+Key_Cmd_Value[1];
		case 3:
			return Key_Cmd_Value[0]*100+Key_Cmd_Value[1]*10+Key_Cmd_Value[2];
		case 4:
			return Key_Cmd_Value[0]*1000+Key_Cmd_Value[1]*100+Key_Cmd_Value[2]*10+Key_Cmd_Value[3];
//		case 5:
//			return Key_Cmd_Value[0]*10000+Key_Cmd_Value[1]*1000+Key_Cmd_Value[2]*100+Key_Cmd_Value[3]*10+Key_Cmd_Value[4];
	}
	return -1;
}
void Value2Plus()
{
	if(Key_Cmd_Length==1)
	{
		if(Key_Cmd_Value[0]<9)
			Key_Cmd_Value[0]++;
		else
		{
			Key_Cmd_Value[0]=0;
			Key_Cmd_Value[1]=1;
			Key_Cmd_Length=2;
		}
	}else
	{
		if(Key_Cmd_Value[1]<9)
			Key_Cmd_Value[1]++;
		else
		{
			Key_Cmd_Value[1]=0;
			Key_Cmd_Value[0]++;
			if(Key_Cmd_Value[0]==10)
			{
				Key_Cmd_Length=1;
				Key_Cmd_Value[0]=0;
			}
		}
	}
}
void Value2Minu(void)
{
	if(Key_Cmd_Length==1)
	{
		if(Key_Cmd_Value[0]>0)
			Key_Cmd_Value[0]--;
	}else
	{
		if(Key_Cmd_Value[1]>0)
			Key_Cmd_Value[1]--;
		else
		{
			Key_Cmd_Value[1]=9;
			Key_Cmd_Value[0]--;
			if(Key_Cmd_Value[0]==0)
			{
				Key_Cmd_Length=1;
			}
		}
	}
}
u8 __tmp;
u8 set_var(u8 Key_Value,double *x)
{
	if(Key_Value==MENU_KEY_Y&&Key_Cmd_Length>0)
	{
		*x=Key_Cmd_Length==2?Key_Cmd_Value[0]*10+Key_Cmd_Value[1]:Key_Cmd_Value[0];
		Key_Cmd_Length=0;
		FSM_VAL=0;
		return 1;
	}else if(IS_KEY_PN(Key_Value))
	{
		if((!CfgPN)&&Key_Cmd_Length==0)
		{
			CfgPN=1;
			__tmp=*x;
			if(__tmp<9)
			{
				Key_Cmd_Value[0]=__tmp;
				Key_Cmd_Length=1;
			}else
			{
				Key_Cmd_Value[0]=__tmp/10;
				Key_Cmd_Value[1]=__tmp%10;
				Key_Cmd_Length=2;
			}
			return 0;
		}
		if(Key_Value==MENU_KEY_P)
		{
			Value2Plus();
		}else
		{
			Value2Minu();
		}
		//(*x)++;
	}
	else if(IS_KEY_NUM(Key_Value))
	{
		if(Key_Cmd_Length==2)
		{
			return 0;
		}
		Key_Cmd_Value[Key_Cmd_Length++]=Key_Value-'0';
	}
	return 0;
}

void FSM_Proc(u8 key)
{
	if(key!=0)
	{
		Key_Value=key;
		DISP_Update=1;
		goto Parse;
	}		
	Key_Value=update_key();
	if(Key_Value==0)
	{
		return ;
	}
	DISP_Update=1;
/****GoBack******/
	Parse:
	if(Key_Value==MENU_KEY_B)
	{
		Key_Cmd_Length=0;
		if(FSM_VAL==DISP_MENU41||FSM_VAL==DISP_MENU42)
		{
			FSM_VAL=0;
			return ;
		}
		if(GETMENU4!=MENU_NULL)
		{
			CLEAR_MENU(4);
		}else if(GETMENU3!=MENU_NULL)
		{
			CLEAR_MENU(3);
		}else if(GETMENU2!=MENU_NULL)
		{
			CLEAR_MENU(2);
		}
		else if(GETMENU1!=MENU_NULL)
		{
			CLEAR_MENU(1);
		}
		return ;
	}

	if(GETMENU4!=MENU_NULL)
	{
		if(Key_Value==MENU_KEY_Y&&Key_Cmd_Length>0)
		{
			aPoints[curve_current_len++].y=Key_Cmd_Length==2?Key_Cmd_Value[0]*10+Key_Cmd_Value[1]:Key_Cmd_Value[0];
			Key_Cmd_Length=0;
			FSM_VAL=DISP_MENU41;
		}
		else if(IS_KEY_NUM(Key_Value))
		{
			if(Key_Cmd_Length==2)
			{
				return ;
			}
			Key_Cmd_Value[Key_Cmd_Length++]=Key_Value-'0';
		}
	}else if(GETMENU3!=MENU_NULL)
	{
		if(Key_Value==MENU_KEY_Y&&Key_Cmd_Length>0)
		{
			if(getValue()!=-1)
			{
					if(curve_current_len>0)
					{
						if(getValue()<=aPoints[curve_current_len-1].x)
						{
							goto BACK2MENU41;
						}
					}
					aPoints[curve_current_len].x=getValue();
				  SET_MENU(4,1);
			}else
			{
		BACK2MENU41:
				FSM_VAL=DISP_MENU41;
			}
			Key_Cmd_Length=0;
			
		}
		else if(IS_KEY_NUM(Key_Value))
		{
			if(Key_Cmd_Length==4)
			{
				return ;
			}
			Key_Cmd_Value[Key_Cmd_Length++]=Key_Value-'0';
		}
	}else if(GETMENU2!=MENU_NULL)//已点二级菜单
	{
		switch(GETMENU1)//
		{
			case MENU1_SET_ALARM:
				switch(GETMENU2)
				{
					case MENU_SUB1:
						if(set_var(Key_Value,&_tmp))
						{
								PID_alarm_MIN=_tmp;
								RESTRAINALARM_MIN(PID_alarm_MIN)
								AT24CXX_Write(TARGET_MIN_STORGE_ADDR,(u8*)&PID_alarm_MIN,sizeof(PID_alarm_MIN));
						}
						break;
					case MENU_SUB2:
						if(set_var(Key_Value,&_tmp))
						{
								PID_alarm_MAX=_tmp;	
  							RESTRAINALARM_MAX(PID_alarm_MAX)
							 AT24CXX_Write(TARGET_MAX_STORGE_ADDR,(u8*)&PID_alarm_MAX,sizeof(PID_alarm_MAX));
						}
						break;
				}
				break;
			case MENU1_SET_HEATER:
				switch(GETMENU2)
				{
					case MENU_SUB1:
						set_var(Key_Value,&PID_output);
						break;
				}
			case MENU1_SET_CURVE:
				switch(GETMENU2)
				{
					case MENU_SUB1:
						if(Key_Value==MENU_KEY_Y)
						{
							SET_MENU(3,1);
						}else if(Key_Value==MENU_KEY_2)
						{
							if(Curve_Top>1)
							{
								Curve_Top--;
							}
						}else if(Key_Value==MENU_KEY_8||Key_Value==MENU_KEY_5)
						{
							if(Curve_Top<curve_current_len)
							{
								Curve_Top++;
							}
						}
						break;
					case MENU_SUB2:
						
						break;
					case MENU_SUB3:
						break;
				}
				break;			
		}
		

	}else if(GETMENU1!=MENU_NULL)//已点一级菜单
	{
		switch(GETMENU1)
		{
			case MENU1_SET_ALARM:
				if(Key_Value==MENU_KEY_1||Key_Value==MENU_KEY_2)
				{
					SET_MENU(2,Key_Value-'0');
				}
				break;
			case MENU1_SET_TEMP:
				//输入的是设定温度
				
				if(set_var(Key_Value,&PID_target))
				{
						AT24CXX_Write(TARGET_STORGE_ADDR,(u8*)&PID_target,sizeof(PID_target));
				}
				break;
			case MENU1_SET_HEATER:
				//输入的是加热功率
					if(Key_Value==MENU_KEY_1)
					{
						PID.SetMode(MANUAL);
						SET_MENU(2,Key_Value-'0');
					}else if(Key_Value==MENU_KEY_2)
					{
						PID.SetMode(AUTOMATIC);
						FSM_VAL=0;
					}
				//set_var(Key_Value,&PID_output);
				break;
			case MENU1_SET_CURVE:
				if(Key_Value==MENU_KEY_1||Key_Value==MENU_KEY_2||Key_Value==MENU_KEY_3)
				{
					SET_MENU(2,Key_Value-'0');
				}
				break;
		}
	}else//IDLE状态
	{
		if(Key_Value==MENU_KEY_1||Key_Value==MENU_KEY_2||Key_Value==MENU_KEY_3||Key_Value==MENU_KEY_4)
		{
			SET_MENU(1,Key_Value-'0');
		}
	}
}
void disp_update_Speed(double speed)
{
	OLED_printfAt(FANSPEED_POSI,1,"Fan(RPM):%.2g           ",speed);
}
void disp_update_Heater(void)
{
	OLED_printfAt(HEATER_POSI,1,"Heater(W):%.2g           ", PWM2POWER(PID_output>0?PID_output:0));
}
void dispIDLE(void)
{
	GUI_Clear();
	OLED_printfAt(TARGET_POSI,1,"Target:%.2f         ",PID_target);
	OLED_printfAt(INPUT_POSI,1,"Input:%.2f           ",PID_input);
	disp_update_Heater();
	disp_update_Speed(speed);
	OLED_printfAt(ALARM_POSI,1,"Alarm:%.2f,%.2f",PID_alarm_MIN,PID_alarm_MAX);
	OLED_printfAt(STATUS_BAR_POSI,1,"                           ");
//	GUI_SetTextMode(GUI_TM_REV);
	OLED_printfAt(MENU_BAR_POSI,1,"1.ALM2.TMP3.POW4.PLOT");
}




void Display_Proc(void)
{
	if(DISP_Update==0)
	{
		return ;
	}
	switch(FSM_VAL)
	{
		case DISP_IDLE:
			dispIDLE();
			break;
		case DISP_MENU1:
			OLED_printfAt(STATUS_BAR_POSI,1,"Choose: 1.Min 2.Max        ");
			break;
		case DISP_MENU2:
			OLED_printfAt(STATUS_BAR_POSI,1,"Input Tmp:                 ");
			if(Key_Cmd_Length>0)
			{
				OLED_printfAt(STATUS_BAR_POSI,64,"%d       ",getKeyMathValue());
			}
			break;
		case DISP_MENU3:
			OLED_printfAt(STATUS_BAR_POSI,1,"1.Input 2.Auto                 ");
			break;
		case DISP_MENU31:
	 		OLED_printfAt(STATUS_BAR_POSI,1,"Input Pow:                 ");
			if(Key_Cmd_Length>0)
			{
				OLED_printfAt(STATUS_BAR_POSI,64,"%d            ",getKeyMathValue());
			}	
			break;
		case DISP_MENU4:
			OLED_printfAt(STATUS_BAR_POSI,1,"1.Cfg 2.Start");
			break;
		case DISP_MENU11:
			OLED_printfAt(STATUS_BAR_POSI,1,"Input Min:                 ");
			if(Key_Cmd_Length>0)
			{
				OLED_printfAt(STATUS_BAR_POSI,64,"%d        ",getKeyMathValue());
			}	
			break;
		case DISP_MENU12:
			OLED_printfAt(STATUS_BAR_POSI,1,"Input Max:                 ");
			if(Key_Cmd_Length>0)
			{
				OLED_printfAt(STATUS_BAR_POSI,64,"%d          ",getKeyMathValue());
			}	
			break;
		case DISP_MENU41:
			GUI_Clear();
			OLED_printfAt(1,1,"No.%8s%8s","Time","TMP");
		for(disp__i=1;disp__i<=(curve_current_len-Curve_Top+1<6?curve_current_len-Curve_Top+1:6);disp__i++)
			{
		 		OLED_printfAt(disp__i+1,1,"%d. %8d%8d",disp__i+Curve_Top-1,aPoints[disp__i+Curve_Top-2].x,aPoints[disp__i+Curve_Top-2].y);
			}
			OLED_printfAt(8,1,"Press Y to Input");
			break;
		case DISP_MENU42:
			GUI_Clear();
			if(curve_current_len<1)
			{
				OLED_printfAt(7,1,"None Value Set    ");
				break ;
			}
			OLED_printfAt(8,90,"Target");
			if(curve_current_len==1)
			{	
				GUI_DrawHLine(64-aPoints[0].y*0.7,0,127);
				break ;
			}
			Curve_Per_Len=127.0/(aPoints[curve_current_len-1].x-aPoints[0].x);
			
			for(disp__i=1;disp__i<=curve_current_len;disp__i++)
			{
		 		curvePoints[disp__i-1].x=(aPoints[disp__i-1].x-aPoints[0].x)*Curve_Per_Len;
				curvePoints[disp__i-1].y=63-aPoints[disp__i-1].y*0.63;
			}
			GUI_DrawPolyLine(curvePoints,curve_current_len,0,0);
			break;
		case DISP_MENU43:
			break;
		case DISP_MENU411:
			OLED_printfAt(8,1,"Input Time:                 ");
			if(Key_Cmd_Length>0)
			{
				OLED_printfAt(8,64,"%d    ",getValue());
			}	
			break;
		case DISP_MENU4111:
			OLED_printfAt(8,1,"Input TMP:                 ");
			if(Key_Cmd_Length>0)
			{
				OLED_printfAt(8,64,"%d    ",getKeyMathValue());
			}		
			break;
		default:
			DISP_Update=0;
	}
	DISP_Update=0;
}