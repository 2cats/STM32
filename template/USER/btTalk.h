#ifndef __ULTRA_PROBE_h
#define __ULTRA_PROBE_h

#include "usart2.h"

extern double PID_target;
extern u8 NEED_TARGET_UPDATE;
#define COMMEND_CHANGE_T	'_'
#define DATA_TMP		1
#define DATA_SPD		2
#define DATA_POW		3
//#define DATA_TMP	  't'
//#define DATA_SPD		's'
//#define DATA_POW		'p'

#define  UP_SM_IDLE		0xff
#define  UP_SM0		0
#define  UP_SM1		1
#define  UP_SM2		2
#define  UP_SM3		3
#define  UP_SM4		4
#define  UP_SM5		5
#define  UP_SM6		6
#define  UP_SM7		7
#define  UP_SM8		8
#define  UP_SM9		9

#define UP_DATA_START_FLAG	'<'
#define UP_DATA_SPLIT_FLAG	'/'
#define UP_DATA_END_FLAG	'>'
#define UP_PERDATA_BUF_SIZE	 11

#define UP_CASE_HANDLER(x)			\
			case x :\
			if(data==UP_DATA_SPLIT_FLAG)\
			{\
					UP_FSM_STATE=x+1;\
					DATA_POINT=0;\
			}\
			else if(data==UP_DATA_END_FLAG)\
			{\
				int i;\
				UP_FSM_STATE=UP_SM_IDLE;\
				BT_ParsePacket();\
				for(i=0;i<9;i++)\
					UP_FSM_PER_LEN[i]=0;\
			}else\
			{\
			 	UP_FSM_BUF[UP_FSM_STATE][DATA_POINT++]=data;\
			 	UP_FSM_PER_LEN[UP_FSM_STATE]=DATA_POINT;\
			}\
			if(DATA_POINT>=UP_PERDATA_BUF_SIZE)\
				UP_FSM_STATE=UP_SM_IDLE;\
		break;

void BT_IT_Init(void);
void BT_ParsePacket(void);
void BT_IRq_Handler(void);
void BT_SendTMP(double tmp);
void BT_SendPOW(double power);
void BT_SendSPD(double speed);

#endif

