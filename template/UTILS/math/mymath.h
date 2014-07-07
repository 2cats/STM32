#ifndef __math_h
#define __math_h
#include <stm32f10x.h>
#ifdef __cplusplus
 extern "C" {
#endif

double char2double(u8 *str);
void double2char(double d,char *str);
	 #ifdef __cplusplus
 }
#endif

#endif
