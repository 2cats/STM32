#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <stm32f10x.h>

#define GETMENU1 ((unsigned char)(FSM_VAL&0XFF))
#define GETMENU2 ((unsigned char)((FSM_VAL&0XFF00)>>8))
#define GETMENU3 ((unsigned char)((FSM_VAL&0XFF0000)>>16))
#define GETMENU4 ((unsigned char)((FSM_VAL&0XFF000000)>>24))

#define MENU_NULL	0

#define MENU1_SET_ALARM	0X01
#define MENU1_SET_TEMP	0X02
#define MENU1_SET_HEATER	0X03
#define MENU1_SET_CURVE	0X04
 
#define SET_MENU(n,val)	FSM_VAL&=(~(0xff<<(8*((n)-1))));FSM_VAL|=((val)<<(8*((n)-1)));		 

#define CLEAR_MENU(n)		FSM_VAL&=(~(0xff<<(8*((n)-1))));		


#define MENU_SUB1	0X01
#define MENU_SUB2	0X02
#define MENU_SUB3	0X03
#define MENU_SUB4	0X04
#define MENU_SUB5	0X05
#define MENU_SUB6	0X06
#define MENU_SUB7	0X07
#define MENU_SUB8	0X08

#define DISP_IDLE		0
#define DISP_MENU1	1
#define DISP_MENU2	2
#define DISP_MENU3	3
#define DISP_MENU4	4

#define DISP_MENU11	0x0101
#define DISP_MENU12	0x0201
#define DISP_MENU31 0x0103
#define DISP_MENU41	0x0104
#define DISP_MENU42	0x0204
#define DISP_MENU43	0x0304

#define DISP_MENU411	0x010104
#define DISP_MENU412	0x020104

#define DISP_MENU4111	0x01010104
#define CURVE_MAX_LEN	50
#define MENU_KEY_1	'1'
#define MENU_KEY_2	'2'
#define MENU_KEY_3	'3'
#define MENU_KEY_4	'4'
#define MENU_KEY_5	'5'
#define MENU_KEY_8	'8'
#define MENU_KEY_B	'B'
#define MENU_KEY_Y	'Y'
#define MENU_KEY_P	'P'
#define MENU_KEY_N	'N'

#define IS_KEY_NUM(keyVal)	(keyVal!=MENU_KEY_B&&keyVal!=MENU_KEY_Y&&keyVal!=0&&keyVal!=MENU_KEY_P&&keyVal!=MENU_KEY_N)
#define IS_KEY_PN(keyVal)		(keyVal==MENU_KEY_P||keyVal==MENU_KEY_N)
#define GET_KEY_MATH_VALUE	

#define TARGET_POSI 1
#define INPUT_POSI 2
#define HEATER_POSI 3
#define FANSPEED_POSI 4
#define ALARM_POSI 5
#define STATUS_BAR_POSI 7
#define MENU_BAR_POSI 8

#define POWER_FACTOR	16.5
#define PWM2POWER(x)	((x)*0.165)

void Display_Proc(void);
void FSM_Proc(u8 key);
void dispIDLE(void);
void disp_update_Speed(double speed);
void disp_update_Heater(void);
#endif
