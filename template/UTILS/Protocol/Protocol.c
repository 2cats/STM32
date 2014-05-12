#include "Protocol.h"
static unsigned char PT_FSM_STATE=PT_SM_IDLE;
static unsigned char PT_FSM_BUF[10][PT_PERDATA_BUF_SIZE];
static unsigned char PT_PACKAGE_LENGTH=0;
static int PT_FSM_PER_LEN[10];
static unsigned char PT_i;

/********Package Define************/
/*
一个包大概长这样：
<PT_FSM_BUF[0][.]/PT_FSM_BUF[1][.]>
最大包大概长这样：
<PT_FSM_BUF[0][.]/PT_FSM_BUF[1][.]/PT_FSM_BUF[2][.]/PT_FSM_BUF[3][.]/......../PT_FSM_BUF[8][.]>
默认的开始符，分隔符，结束符分别为 < / >,当与数据冲突时及时更换

每一小格的数据：PT_FSM_BUF[i][0] -> PT_FSM_BUF[i][PT_PERDATA_BUF_SIZE-1]
每一小格的最大长度：PT_PERDATA_BUF_SIZE   (若实际数据超出每小格的最大容量，则直接置状态机为PT_SM_IDLE)
每一小格的实际长度：PT_FSM_PER_LEN[i] (1 -> PT_PERDATA_BUF_SIZE  ，为0表示该小格为空，全部数值有意义，即它会在每个包传输完成时全部清零)
一个包最多10小格，超出部分被抛弃（即分隔符也是最后一小格的结束判断标识）

PT_PACKAGE_LENGTH 表示收到非空小格的个数(0 -> 10,建议不要将空小格放在数据包中间)

*/

//自实现函数<1>
//解析收到的一个包
void PT_ParsePacket(void)
{
	//...
	
}
//自实现函数<2>
//成功接收1字节返回1，否则返回0
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
}
//unsigned char PT_SendOneByte(unsigned char data)
//{
//	;
//}

//该函数一般置于接收中断
void PT_IRq_Handler(void)
{
	unsigned char data;
	if(!PT_GetOneByte(&data))//自实现函数<2>
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

