#ifndef __ULTRA_PROBE_h
#define __ULTRA_PROBE_h
#include "mymath.h"
#include "IRremote.h"
#include "display.h"
#include "usart2.h"
#include "usart.h"
extern double PID_target;
extern u8 NEED_TARGET_UPDATE;
#define COMMEND_CHANGE_T	'_'
#define DATA_TMP		1
#define DATA_SPD		2
#define DATA_POW		3

#define UP_DATA_START_FLAG	'<'
#define UP_DATA_SPLIT_FLAG	'/'
#define UP_DATA_END_FLAG	'>'


void BT_SendTMP(double tmp);
void BT_SendPOW(double power);
void BT_SendSPD(double speed);
void IRrecv_Handler(IR_RawDataTypeDef *raw);
void UART1_Send_Double(double data);
#endif

