#include "Protocol.h"
static unsigned char PT_FSM_STATE=PT_SM_IDLE;
static unsigned char PT_FSM_BUF[10][PT_PERDATA_BUF_SIZE];
static unsigned char PT_PACKAGE_LENGTH=0;
static int PT_FSM_PER_LEN[10];
static unsigned char PT_i;

/********Package Define************/
/*
һ������ų�������
<PT_FSM_BUF[0][.]/PT_FSM_BUF[1][.]>
������ų�������
<PT_FSM_BUF[0][.]/PT_FSM_BUF[1][.]/PT_FSM_BUF[2][.]/PT_FSM_BUF[3][.]/......../PT_FSM_BUF[8][.]>
Ĭ�ϵĿ�ʼ�����ָ������������ֱ�Ϊ < / >,�������ݳ�ͻʱ��ʱ����

ÿһС������ݣ�PT_FSM_BUF[i][0] -> PT_FSM_BUF[i][PT_PERDATA_BUF_SIZE-1]
ÿһС�����󳤶ȣ�PT_PERDATA_BUF_SIZE   (��ʵ�����ݳ���ÿС��������������ֱ����״̬��ΪPT_SM_IDLE)
ÿһС���ʵ�ʳ��ȣ�PT_FSM_PER_LEN[i] (1 -> PT_PERDATA_BUF_SIZE  ��Ϊ0��ʾ��С��Ϊ�գ�ȫ����ֵ�����壬��������ÿ�����������ʱȫ������)
һ�������10С�񣬳������ֱ����������ָ���Ҳ�����һС��Ľ����жϱ�ʶ��

PT_PACKAGE_LENGTH ��ʾ�յ��ǿ�С��ĸ���(0 -> 10,���鲻Ҫ����С��������ݰ��м�)

*/

//��ʵ�ֺ���<1>
//�����յ���һ����
void PT_ParsePacket(void)
{
	//...
	
}
//��ʵ�ֺ���<2>
//�ɹ�����1�ֽڷ���1�����򷵻�0
unsigned char PT_GetOneByte(unsigned char *data)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
	{
		*data=USART_ReceiveData(USART2);
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		return 1;
	}
	else
	{
		return 0;
	}
}
void PT_EnableIRQ(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
}
//unsigned char PT_SendOneByte(unsigned char data)
//{
//	;
//}

//�ú���һ�����ڽ����ж�
void PT_IRq_Handler(void)
{
	unsigned char data;
	if(!PT_GetOneByte(&data))//��ʵ�ֺ���<2>
		return;
	switch(PT_FSM_STATE)
	{
		case PT_SM_IDLE :
			if(data==PT_DATA_START_FLAG)
			{
				PT_FSM_STATE=PT_SM0;
			}
			break;
			PT_CASE_HANDLER(0)
			PT_CASE_HANDLER(1)
			PT_CASE_HANDLER(2)
			PT_CASE_HANDLER(3)
			PT_CASE_HANDLER(4)
			PT_CASE_HANDLER(5)
			PT_CASE_HANDLER(6)
			PT_CASE_HANDLER(7)
			PT_CASE_HANDLER(8)
		case PT_SM9 :
			if(data==PT_DATA_END_FLAG||data==PT_DATA_SPLIT_FLAG)
			{
				PT_ParsePacket();
				PT_FSM_STATE=PT_SM_IDLE;
				PT_PACKAGE_LENGTH=0;
				for(PT_i=0;PT_i<10;PT_i++)
					PT_FSM_PER_LEN[PT_i]=0;
			}else
			{
				PT_FSM_PER_LEN[PT_FSM_STATE]==0?PT_PACKAGE_LENGTH++:PT_PACKAGE_LENGTH;
			 	PT_FSM_BUF[PT_FSM_STATE][PT_FSM_PER_LEN[PT_FSM_STATE]++]=data;
			}
			if(PT_FSM_PER_LEN[PT_FSM_STATE]>=PT_PERDATA_BUF_SIZE)
				PT_FSM_STATE=PT_SM_IDLE;
		break;			
	}
}

