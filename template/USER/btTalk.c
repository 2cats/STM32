#include "btTalk.h"
#include "stm32f10x_usart.h"
#include "display.h"
unsigned char UP_FSM_STATE=UP_SM_IDLE;
unsigned char UP_FSM_BUF[9][UP_PERDATA_BUF_SIZE];
int UP_FSM_PER_LEN[9];
int DATA_POINT;

unsigned char _i;
void UltraProbe_Init(void)
{
	UP_FSM_STATE=UP_SM_IDLE;
}
/*
	上位机修改温度格式：
< COMMEND_CHANGE_T / [8byte] >
double	<->	char
All	13byte
1-5	整数
6-8	小数

			MCU返回数据：
温度：< DATA_TMP / [8byte] >
速度：< DATA_SPD / [8byte] >
功率：< DATA_POW / [8byte] >
*/
//发送时先发高位
//0		1		2		3		4			5		6				7	
//万	千	百	十	个 .	0.1	0.01	0.001
double char2double(u8 *str)
{
	short i;
	int H=0,T=1,L=0;
	for(i=4;i>=0;i--)
	{
		H+=((str[i]-'0')*T);
		T*=10;
	}
	T=1;
	for(i=7;i>=5;i--)
	{
		L+=((str[i]-'0')*T);
		T*=10;
	}
	return H+L/1000.0;
	
	
}
void BT_ParsePacket()
{
	switch(UP_FSM_BUF[0][0])
	{
		case COMMEND_CHANGE_T:
				PID_target=char2double(UP_FSM_BUF[1]);
			 	SET_PID_Target_NU;
			break;		
	}
}
void BT_IRq_Handler(void)
{
	unsigned char data;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
	{
		data=USART_ReceiveData(USART2);
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
	else
		return;
	switch(UP_FSM_STATE)
	{
		case UP_SM_IDLE :
			if(data==UP_DATA_START_FLAG)
			{
				UP_FSM_STATE=UP_SM0;
				DATA_POINT=0;
			}
			break;
			UP_CASE_HANDLER(0)
			UP_CASE_HANDLER(1)
			UP_CASE_HANDLER(2)
			UP_CASE_HANDLER(3)
			UP_CASE_HANDLER(4)
			UP_CASE_HANDLER(5)
			UP_CASE_HANDLER(6)
			UP_CASE_HANDLER(7)
			UP_CASE_HANDLER(8)
		case UP_SM9 :
			if(data==UP_DATA_END_FLAG||data==UP_DATA_SPLIT_FLAG)
			{
				int i;
				UP_FSM_STATE=UP_SM_IDLE;
				BT_ParsePacket();
				for(i=0;i<9;i++)
					UP_FSM_PER_LEN[i]=0;
			}else
			{
			 	UP_FSM_BUF[UP_FSM_STATE][DATA_POINT++]=data;
			 	UP_FSM_PER_LEN[UP_FSM_STATE]=DATA_POINT;
			}
			if(DATA_POINT>=UP_PERDATA_BUF_SIZE)
				UP_FSM_STATE=UP_SM_IDLE;
		break;
			
			
	}
}
void BT_IT_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
}
//发送时先发高位
//0		1		2		3		4			5		6				7	
//万	千	百	十	个 .	0.1	0.01	0.001
void double2char(double d,char *str)
{
	int H=d,T=10000,L=(d-H)*10000;
	short i;
	for(i=0;i<=4;i++)
	{	
		str[i]=H/T+'0';
		H=H%T;
		T=T/10;
	}
	T=1000;;
	for(i=5;i<=7;i++)
	{
		str[i]=L/T+'0';
		L=L%T;
		T=T/10;
	}
}
void BT_SendTMP(double tmp)
{
	char str[9];
	str[8]='\0';
	uart2_sendChar('<');
	uart2_sendChar(DATA_TMP);
	uart2_sendChar('/');
	double2char(tmp,str);
	uart2_print(str);
	uart2_sendChar('>');
}
void BT_SendPOW(double power)
{
	char str[9];
	str[8]='\0';
	uart2_sendChar('<');
	uart2_sendChar(DATA_POW);
	uart2_sendChar('/');
	double2char(power,str);
	uart2_print(str);
	uart2_sendChar('>');
}
void BT_SendSPD(double speed)
{
	char str[9];
	str[8]='\0';
	uart2_sendChar('<');
	uart2_sendChar(DATA_SPD);
	uart2_sendChar('/');
	double2char(speed,str);
	uart2_print(str);
	uart2_sendChar('>');
}
