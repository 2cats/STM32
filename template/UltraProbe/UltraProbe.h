#ifndef __ULTRA_PROBE_h
#define __ULTRA_PROBE_h

#include "usart2.h"
#define TYPE_CHAR		0
#define TYPE_UNSIGNED_CHAR 	1
#define TYPE_SHORT		2
#define TYPE_UNSIGNED_SHORT 	3
#define TYPE_INT		4
#define TYPE_UNSIGNED_INT 	5
#define TYPE_LONG		6
#define TYPE_UNSIGNED_LONG	7
#define TYPE_FLOAT 		8
#define TYPE_DOUBLE 		9

#define COMMEND_WRITE		1		
#define COMMEND_RIGISTER	0	
#define COMMEND_RETURN_ALL	2

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
#define UP_PERDATA_BUF_SIZE	 8
#define UP_MAX_REGISTER_VARIABLE_NUM 20


#define Tx1Byte	 uart2_sendChar
#define TxPrintf	 uart2_printf

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
				UltraProbe_ParsePacket();\
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


void UltraProbe_Init(void);
void UltraProbe_ParsePacket(void);
void UltraProbe_IRq_Handler(void);
#endif