#include "UltraProbe.h"
#include "stm32f10x_usart.h"
unsigned char UP_FSM_STATE=UP_SM_IDLE;
unsigned char UP_FSM_BUF[9][UP_PERDATA_BUF_SIZE];
int UP_FSM_PER_LEN[9];
int DATA_POINT;
			u8 sbuf[9];
unsigned  int UP_REGISTER_ADDR_LIST[UP_MAX_REGISTER_VARIABLE_NUM];
unsigned char UP_REGISTER_TYPES[UP_MAX_REGISTER_VARIABLE_NUM];
unsigned char UP_REGISTER_ADDR_LIST_NUM; 
unsigned char _i;
void UltraProbe_Init(void)
{
	UP_FSM_STATE=UP_SM_IDLE;
}
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
void UltraProbe_ParsePacket()
{
	switch(UP_FSM_BUF[0][0])
	{
		case COMMEND_WRITE://PC端先发低位
			switch(UP_FSM_BUF[2][0])
			{
				case TYPE_CHAR:
					*((char*)((*((unsigned  int*)UP_FSM_BUF[1]))))=(char)char2double(UP_FSM_BUF[3]);
			  case TYPE_INT:
					*((int*)((*((unsigned  int*)UP_FSM_BUF[1]))))=(int)char2double(UP_FSM_BUF[3])
			  case TYPE_UNSIGNED_CHAR:
					*((unsigned char*)((*((unsigned  int*)UP_FSM_BUF[1]))))=(unsigned char)char2double(UP_FSM_BUF[3])
			  case TYPE_UNSIGNED_SHORT:
					*((unsigned short*)((*((unsigned  int*)UP_FSM_BUF[1]))))=(unsigned short)char2double(UP_FSM_BUF[3])
			  case TYPE_SHORT:
					*((short*)((*((unsigned  int*)UP_FSM_BUF[1]))))=(short)char2double(UP_FSM_BUF[3])
			  case TYPE_UNSIGNED_INT:
					*((unsigned int*)((*((unsigned  int*)UP_FSM_BUF[1]))))=(unsigned int)char2double(UP_FSM_BUF[3])
			  case TYPE_LONG:
					*((long*)((*((unsigned  int*)UP_FSM_BUF[1]))))=(long)char2double(UP_FSM_BUF[3])
			  case TYPE_UNSIGNED_LONG:
					*((unsigned long*)((*((unsigned  int*)UP_FSM_BUF[1]))))=(unsigned long)char2double(UP_FSM_BUF[3])
			  case TYPE_FLOAT:
					*((float*)((*((unsigned  int*)UP_FSM_BUF[1]))))=(float)char2double(UP_FSM_BUF[3])
				case TYPE_DOUBLE:
					*((double*)((*((unsigned  int*)UP_FSM_BUF[1]))))=char2double(UP_FSM_BUF[3])
				//	*((double*)((*((unsigned  int*)UP_FSM_BUF[1]))))=(double)((unsigned int)UP_FSM_BUF[3][0]+((unsigned int)UP_FSM_BUF[3][1]<<8)+((unsigned int)UP_FSM_BUF[3][2]<<16)+((unsigned int)UP_FSM_BUF[3][3]<<24)+((unsigned int)UP_FSM_BUF[3][4]<<32)+);
			}
			break;
		case COMMEND_RIGISTER:
			UP_REGISTER_ADDR_LIST[UP_REGISTER_ADDR_LIST_NUM]=*((unsigned  int*)UP_FSM_BUF[1]);
			UP_REGISTER_TYPES[UP_REGISTER_ADDR_LIST_NUM++]=UP_FSM_BUF[2][0];
			break;
		case COMMEND_RETURN_ALL:
			for(_i=0;_i<UP_REGISTER_ADDR_LIST_NUM;_i++)
			{
		 		Tx1Byte('<');
				Tx1Byte(UP_REGISTER_ADDR_LIST[_i]&0xff);
				Tx1Byte((UP_REGISTER_ADDR_LIST[_i]&0xff00)>>8);
				Tx1Byte((UP_REGISTER_ADDR_LIST[_i]&0xff0000)>>16);
				Tx1Byte((UP_REGISTER_ADDR_LIST[_i]&0xff000000)>>24);
				Tx1Byte('/');
	
			switch(UP_REGISTER_TYPES[_i])
			{
				case TYPE_CHAR:
					double2char((double)(*(char *)UP_REGISTER_ADDR_LIST[_i]),str);break;
			  case TYPE_INT:
					double2char((double)(*(int *)UP_REGISTER_ADDR_LIST[_i]),str);
					break;
			  case TYPE_UNSIGNED_CHAR:
					double2char((double)(*(unsigned char *)UP_REGISTER_ADDR_LIST[_i]),str);break;
			  case TYPE_UNSIGNED_SHORT:
					double2char((double)(*(unsigned short *)UP_REGISTER_ADDR_LIST[_i]),str);break;
			  case TYPE_SHORT:
					double2char((double)(*(short *)UP_REGISTER_ADDR_LIST[_i]),str);break;
			  case TYPE_UNSIGNED_INT:
					double2char((double)(*(unsigned int *)UP_REGISTER_ADDR_LIST[_i]),str);break;
					break;
			  case TYPE_FLOAT:
					double2char((double)(*(double  *)UP_REGISTER_ADDR_LIST[_i]),str);break;
				case TYPE_DOUBLE:
					double2char((double)(*(unsigned short *)UP_REGISTER_ADDR_LIST[_i]),str);break;
			}
			str[8]='\0';
			uart2_printf(str);
			Tx1Byte('>');
			}
			break;		
	}
}
void UltraProbe_IRq_Handler(void)
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
				UltraProbe_ParsePacket();
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