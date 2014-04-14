#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_
#include "usart2.h"
#define  PT_SM_IDLE		0xff
#define  PT_SM0		0
#define  PT_SM1		1
#define  PT_SM2		2
#define  PT_SM3		3
#define  PT_SM4		4
#define  PT_SM5		5
#define  PT_SM6		6
#define  PT_SM7		7
#define  PT_SM8		8
#define  PT_SM9		9

#define PT_DATA_START_FLAG	'<'
#define PT_DATA_SPLIT_FLAG	'/'
#define PT_DATA_END_FLAG	'>'
#define PT_PERDATA_BUF_SIZE	 11

#define PT_CASE_HANDLER(x)			\
			case x :\
			if(data==PT_DATA_SPLIT_FLAG)\
			{\
					PT_FSM_STATE=x+1;\
			}\
			else if(data==PT_DATA_END_FLAG)\
			{\
				PT_ParsePacket();\
				PT_FSM_STATE=PT_SM_IDLE;\
				PT_PACKAGE_LENGTH=0;\
				for(PT_i=0;PT_i<10;PT_i++)\
					PT_FSM_PER_LEN[PT_i]=0;\
			}else\
			{\
				PT_FSM_PER_LEN[PT_FSM_STATE]==0?PT_PACKAGE_LENGTH++:PT_PACKAGE_LENGTH;\
			 	PT_FSM_BUF[PT_FSM_STATE][PT_FSM_PER_LEN[PT_FSM_STATE]++]=data;\
			}\
			if(PT_FSM_PER_LEN[PT_FSM_STATE]>=PT_PERDATA_BUF_SIZE)\
				PT_FSM_STATE=PT_SM_IDLE;\
		break;
void PT_ParsePacket(void);
unsigned char PT_GetOneByte(unsigned char *data);
void PT_IRq_Handler(void);

#endif
