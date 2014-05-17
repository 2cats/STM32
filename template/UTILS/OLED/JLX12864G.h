#ifndef _JLX12864G_H
#define _JLX12864G_H

#include "sys.h"

/**********CONFIGURE***************/

#define USING_UCGUI
#define OLED_CMD_BUFFER_LEN  			200//printf��������������

/*********************************/


#ifdef USING_UCGUI
	#include  "GUI.h"
	#define OLED_BLACK GUI_WHITE
	#define OLED_WHITE GUI_BLACK
#endif

extern const u8 font_s[][6];
extern const u8 font_l[][16];
extern const u8 tsktsk[];
extern const u8 font_hz_tongxgc[][16];

void OLED_WriteString(u8 page, u8 column, u8 *s, u8 str_long, u8 size);
void OLED_WriteChar(u8 page,u8 column,u8 c, u8 size);
void OLED_WriteNumber(u8 page,u8 column, unsigned long number, u8 num_bit, u8 dot_bit, u8 front_no_zero, u8 size);
void OLED_WriteHz(u8 page, u8 column, u8 *s, u8 str_long);
void OLED_Draw_Pic(u8 * p);
void OLED_Init(void);
void OLED_Clear(void);

//��/����
void OLED_DrawPoint(int x, int y, int PixelIndex);
unsigned int OLED_GetPixelIndex(int x, int y);	

//1- ��֧�����ģ������÷���printf��ͬ�����Զ����У��Լ����\n��
//2- ʹ��OLED_printf�蹴ѡmicroLib
#ifdef USING_UCGUI
void OLED_printf (char *fmt, ...);//�������ǰUCGUIϵͳ��ǰλ��
void OLED_printfAt (int x,int y,char *fmt, ...);//�������ǰUCGUIϵͳָ��λ��
#else
void OLED_printfAt(int x,int y,char *fmt, ...);//�������x��[1-8]����y��[1-128](С�֣�
#endif

#endif
