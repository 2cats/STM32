#ifndef _JLX12864G_H
#define _JLX12864G_H

#include "sys.h"

/**********CONFIGURE***************/

#define USING_UCGUI
#define OLED_CMD_BUFFER_LEN  			200//printf单次最大输出长度

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

//画/读点
void OLED_DrawPoint(int x, int y, int PixelIndex);
unsigned int OLED_GetPixelIndex(int x, int y);	

//1- 不支持中文，其它用法和printf相同（不自动换行，自己添加\n）
//2- 使用OLED_printf需勾选microLib
#ifdef USING_UCGUI
void OLED_printf (char *fmt, ...);//输出到当前UCGUI系统当前位置
void OLED_printfAt (int x,int y,char *fmt, ...);//输出到当前UCGUI系统指定位置
#else
void OLED_printfAt(int x,int y,char *fmt, ...);//输出到第x行[1-8]，第y列[1-128](小字）
#endif

#endif
