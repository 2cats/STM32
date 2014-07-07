#include "MKeyScan/MKeyScan.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

//2014/6/30	WEC
/*
						�������<״̬��ɨ��>
������
��ʼ���󣬶�ʱ����ɨ�躯����ͨ���ص�������ȡ�����¼�������

Example:
void KeyScan_Handler(struct MKey_Info* info)
{
	switch(info->type)
	{
//	case MKey_PRESS:
//		*usart<<"press\n";
//		break;
//	case MKey_HOLD:
//		(*usart).printf("%d\n",info->holdTime);
//		break;
	case MKey_RELEASE:
		usart->printf("%d\n",info->value);
	 	//info->value;
		break;
//	case MKey_CLICK:
//		*usart<<"click\n";
//		break;
//	case MKey_DOUBLE:
//		*usart<<"double\n";
//		break;
	}
}
//main
	Timer timer4(TIM4_Conf);
	MKeyScan_Init(10,250);
	MKeyScan_cb=KeyScan_Handler;
	timer4.doEvery(10,MKeyScan_Poc);
	
	
ע�����
	�޸�����ʱ����עһ��ͷ�ļ����KEY_GPIO_RCC
	����hold�¼���holdʱһֱ������#define MKey_ENABLE_HOLD ���Թر�/��HOLD����
	MKeyScan_scanInterval���ڼ���holdʱ�䣬û�õ�hold���ܵĻ���ֵ�ǳ��ǳ�����
	MKeyScan_doubleInterval��˫������������˵�����Ӧ̫����С��˫��Ҫ��̫�ߡ�����Ϊ0�������Ǹ�����ʱ������������Ӧ���������˫���¼�
	��Ҳ����˵�����������ͨ�ļ�⣬������ô��ʼ����MKeyScan_Init(0,0),Ȼ��switch��MKey_RELEASE�¼���
	
˵����
	MKey_PRESS �¼�Ҳ�Ǿ���һ����ʱ��֤�ģ�������������ʹ���
	��MKey_CLICK��һ����MKey_RELEASE������MKey_RELEASE��һ����MKey_CLICK����������MKey_DOUBLE
*/



//Pin Config
static struct KeyIOPort key_row[] = { { GPIOB, GPIO_Pin_5 },
		{ GPIOB, GPIO_Pin_6 }, { GPIOB, GPIO_Pin_7 }, { GPIOB, GPIO_Pin_9 },

};
static struct KeyIOPort key_col[] = { { GPIOB, GPIO_Pin_10 }, { GPIOB,
GPIO_Pin_13 }, { GPIOB, GPIO_Pin_14 }, { GPIOB, GPIO_Pin_11 }, };

void (*MKeyScan_cb)(struct MKey_Info*);

//private field
static u8 MKeyScan_row_Count, MKeyScan_col_Count;
static u8 MKeyScan_STA = MKeyScan_STA_IDEL;
static struct MKey_Info MKey_info;
static int MKeyScan_doubleInterval = 250;
static u8 MKeyScan_lastVlaue = MKeyScan_NULL;
static u8 MKeyScan_needWaitting = 0;
static int MKeyScan_scanInterval = 10;
static int MKeyScan_releaseInterVal;
#define PIN_READ(j)		(GPIO_ReadInputDataBit(key_col[j].GPIO_x, key_col[j].GPIO_pin))

void MKeyScan_Init(int scanInterval, int doubleInterval) {
	GPIO_InitTypeDef GPIO_InitStructure;
	unsigned char i;
	RCC_APB2PeriphClockCmd(KEY_GPIO_RCC, ENABLE);
	MKeyScan_row_Count = sizeof(key_row) / sizeof(struct KeyIOPort);
	MKeyScan_col_Count = sizeof(key_col) / sizeof(struct KeyIOPort);
	for (i = 0; i < MKeyScan_row_Count; i++) {
		GPIO_InitStructure.GPIO_Pin = key_row[i].GPIO_pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_Init(key_row[i].GPIO_x, &GPIO_InitStructure);
	}
	for (i = 0; i < MKeyScan_col_Count; i++) {
		GPIO_InitStructure.GPIO_Pin = key_col[i].GPIO_pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_Init(key_col[i].GPIO_x, &GPIO_InitStructure);
	}

	for (i = 0; i < MKeyScan_row_Count; i++) { //����ɨ��
		GPIO_SetBits(key_row[i].GPIO_x, key_row[i].GPIO_pin); //high
	}
	MKeyScan_scanInterval = scanInterval;
	MKeyScan_doubleInterval = doubleInterval;
}
u8 MKeyScan_GETKEY() {
	u8 i, j;
	for (i = 0; i < MKeyScan_row_Count; i++) {
		GPIO_ResetBits(key_row[i].GPIO_x, key_row[i].GPIO_pin); //ѡ����i
		for (j = 0; j < MKeyScan_col_Count; j++) {
			if (!PIN_READ(j)) //[i,j]�����
					{
				GPIO_SetBits(key_row[i].GPIO_x, key_row[i].GPIO_pin); //ȡ��ѡ����i,׼����һ��ɨ��
				return i * MKeyScan_row_Count + j + 1;
			}
		}
		GPIO_SetBits(key_row[i].GPIO_x, key_row[i].GPIO_pin); //ȡ��ѡ����i��׼��ѡ����һ��
		delay_us(30);
	}
	return MKeyScan_NULL;
}
void MKeyScan_CallBack(u8 type) {
	if (MKeyScan_cb != 0) {
		MKey_info.type = type;
		MKeyScan_cb(&MKey_info);
	}
}

void MKeyScan_Poc() {
	u8 key_code = MKeyScan_GETKEY();
	if (MKeyScan_needWaitting)
		MKeyScan_releaseInterVal += MKeyScan_scanInterval;
	if (MKeyScan_releaseInterVal > MKeyScan_doubleInterval
			&& MKeyScan_needWaitting) {
		MKeyScan_CallBack(MKey_CLICK);
		MKeyScan_needWaitting = 0;
	}
	switch (MKeyScan_STA) {
	case MKeyScan_STA_IDEL:
		if (key_code != MKeyScan_NULL) {
			MKey_info.value = key_code;
			MKeyScan_STA = MKeyScan_STA_WAIT4CON; //Trigger
		}
		break;
	case MKeyScan_STA_WAIT4CON:
		if (key_code == MKey_info.value) {
			MKeyScan_CallBack(MKey_PRESS);
#ifdef  MKey_ENABLE_HOLD
			MKey_info.holdTime = 0;
#endif
			MKeyScan_STA = MKeyScan_STA_HOLDING; //press
		} else {
			MKeyScan_STA = MKeyScan_STA_IDEL;
		}
		break;
	case MKeyScan_STA_HOLDING:
		if (key_code == MKey_info.value) {
			//hold
#ifdef  MKey_ENABLE_HOLD
			MKey_info.holdTime += MKeyScan_scanInterval;
			MKeyScan_CallBack(MKey_HOLD);
#endif
		} else {
			//release
			MKeyScan_CallBack(MKey_RELEASE);
			MKeyScan_needWaitting = 1;
			if (MKey_info.value == MKeyScan_lastVlaue
					&& MKeyScan_releaseInterVal < MKeyScan_doubleInterval) {
				MKeyScan_CallBack(MKey_DOUBLE);
				MKeyScan_needWaitting = 0;
				MKeyScan_lastVlaue = MKeyScan_NULL;
			} else {
				MKeyScan_lastVlaue = MKey_info.value;
			}

			MKeyScan_releaseInterVal = 0;
			MKeyScan_STA = MKeyScan_STA_IDEL;
		}

		break;
	}
}

