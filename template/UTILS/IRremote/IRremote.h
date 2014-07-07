#ifndef IRREMOTE_H
#define IRREMOTE_H
#include "PWMManager.h"
#include "InputCatcher.h"
#include "delay.h"


#ifdef __cplusplus
 extern "C" {
#endif

	 
/********* Config **********/
//IR_FREQ ������ֵ��38000  ����Χ��ʹ 72000000/IR_FREQ ��0-65536֮�䣬����̫�ӽ���0����IR_FREQ���ɹ���
#define IR_FREQ	38000

//�ز�ռ�ձȣ�����ֵ��0.5
#define IR_RATIO	0.5

//����������ţ��͵�ƽ��ʾ���ز��ź�
#define IR_RECV_CHANNEL	TIM3CHANNEL4

//���ⷢ�����ţ����ܺͽ�������ʹ��ͬһ����ʱ��
#define IR_SEND_CHANNEL	TIM4CHANNEL1
#define IR_RECV_BUF_LEN	100

/*--------------------------*/

typedef unsigned short IR_DATA_TYPE;
typedef struct 
{
	IR_DATA_TYPE data[IR_RECV_BUF_LEN];
	unsigned int length;
}IR_RawDataTypeDef;
extern void (*IRrecv_DoWhenDone)(IR_RawDataTypeDef *);
extern void (*IRrecv_DoWhenFlow)(void);
void IRsend_DoWhenCathed1(struct InputCatcherType*t);
void IRsend_DoWhenTimeout(struct InputCatcherType*t);
void IRsend_mark(IR_DATA_TYPE time);
void IRsend_space(IR_DATA_TYPE time);
void IRrecv_Init(void);
void IRsend_Init(void);
void IR_SendRaw(IR_RawDataTypeDef *rawData);
void IR_SendDefault(void);
void IRsend_RawDataCpy(IR_RawDataTypeDef*dest,IR_RawDataTypeDef*src);


/********** NEC ***********/

#define _1_SPACE_LEN	560
#define _1_MARK_LEN	1680

#define _0_SPACE_LEN	560
#define _0_MARK_LEN	560

#define NEC_LEADER_SPACE_LEN 	9000
#define NEC_LEADER_MARK_LEN 	4500

#define BIT_TOLERATION 	250

typedef struct 
{
	unsigned char address;
	unsigned char command;
}NEC_DataTypeDef;
char NEC_Parse(IR_RawDataTypeDef* rawData,NEC_DataTypeDef *NEC_Data);

void IR_SendNEC(NEC_DataTypeDef *NEC_Data);
/*--------------------------*/
#ifdef __cplusplus
}
#endif

#endif 
