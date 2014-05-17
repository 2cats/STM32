#include "IRremote.h"
#include <string.h>
static PWMManagerTypeDef PWMManager;
static InputCatchManagerTypeDef InputCatchManager;
static IR_RawDataTypeDef IR_RawData;

void (*IRrecv_DoWhenDone)(IR_RawDataTypeDef *);
void (*IRrecv_DoWhenFlow)(void);

void IRsend_DoWhenCathed1(struct InputCatcherType*t)
{
	if(IR_RawData.length>=IR_RECV_BUF_LEN)
	{
		if(IRrecv_DoWhenFlow!=0)
		{
			IRrecv_DoWhenFlow();
		}
		IR_RawData.length=0;//溢出后仍然继续decode，故绝对不允许溢出，溢出回调主要用于调试
	}
	IR_RawData.data[IR_RawData.length]=t->duty;//忽略小数点
	IR_RawData.data[IR_RawData.length+1]=t->duration-t->duty;//忽略小数点
	IR_RawData.length+=2;
}
void IRsend_DoWhenTimeout(struct InputCatcherType*t)
{
	if(IRrecv_DoWhenDone!=0)
	{
		IRrecv_DoWhenDone(&IR_RawData);//收到一个完整红外包
	}
	IR_RawData.length=0;
}
void IRsend_mark(IR_DATA_TYPE time) {
  PWMManager.changeRatio(IR_SEND_CHANNEL,IR_RATIO); 
  delay_us(time);
}
void IRsend_space(IR_DATA_TYPE time) {
  PWMManager.changeRatio(IR_SEND_CHANNEL,0); 
  delay_us(time);
}

void IRrecv_Init(void)
{
	InputCatchManagerStructure(&InputCatchManager);
	InputCatchManager.startCatching(IR_RECV_CHANNEL,LOW_HIGH_LOW_DURATION,1);//分辨率1us,且CATCHING_FLOW_SIZE为1时，溢出时间为65ms
	InputCatchManager.setDoWhenDone(IR_RECV_CHANNEL,IRsend_DoWhenCathed1);//optional
	InputCatchManager.setDoWhenTimeout(IR_RECV_CHANNEL,IRsend_DoWhenTimeout);//optional
	IR_RawData.length=0;
	IRrecv_DoWhenDone=0;
	IRrecv_DoWhenFlow=0;
} 
void IRsend_Init(void)
{
	PWMManagerStructure(&PWMManager);
	PWMManager.ProduceNewPWM(IR_SEND_CHANNEL,IR_FREQ,0);
}
void IR_SendRaw(IR_RawDataTypeDef *rawData)
{
	int i;
	for(i=0;i<rawData->length;i+=2)
	{
	 	IRsend_mark(rawData->data[i]);
		IRsend_space(rawData->data[i+1]);
	}
}
void IR_SendDefault(void)
{
	IR_SendRaw(&IR_RawData);
}
void IRsend_RawDataCpy(IR_RawDataTypeDef*dest,IR_RawDataTypeDef*src)
{
	unsigned int length=sizeof(*src);
	memcpy(dest,src,length);
}


/********** NEC ***********/

char IR_2Bit(IR_DATA_TYPE space,IR_DATA_TYPE mark)
{
	if(space>_1_SPACE_LEN-BIT_TOLERATION\
		&&space<_1_SPACE_LEN+BIT_TOLERATION\
		&&mark>_1_MARK_LEN-BIT_TOLERATION\
		&&mark<_1_MARK_LEN+BIT_TOLERATION)\
	{
		return  1;
	}
	else 	if(space>_0_SPACE_LEN-BIT_TOLERATION\
		&&space<_0_SPACE_LEN+BIT_TOLERATION\
		&&mark>_0_MARK_LEN-BIT_TOLERATION\
		&&mark<_0_MARK_LEN+BIT_TOLERATION)\
	{
		return  0;
	}
	return -1;
}
char NEC_IsLeader(IR_DATA_TYPE space,IR_DATA_TYPE mark)
{
	if(space>NEC_LEADER_SPACE_LEN-BIT_TOLERATION\
		&&space<NEC_LEADER_SPACE_LEN+BIT_TOLERATION\
		&&mark>NEC_LEADER_MARK_LEN-BIT_TOLERATION\
		&&mark<NEC_LEADER_MARK_LEN+BIT_TOLERATION)\
	{
		return  1;
	}
	return 0;
}
/*
[0: 	 1] 	引导码
[2: 	17]		地址码
[18:	33]		地址反码
[34: 	49]		指令码
[50:	65]		指令反码
*/
char NEC_Parse(IR_RawDataTypeDef* rawData,NEC_DataTypeDef *NEC_Data)
{
	int i;
	volatile unsigned char addr=0,naddr=0,cmd=0,ncmd=0;
	if(rawData->length<66)
		//return 0;
	if(!NEC_IsLeader(rawData->data[0],rawData->data[1]))
	{
		return 0;
	}
	for(i=2;i<18;i+=2)
	{
		addr<<=1;	
		addr|=IR_2Bit(rawData->data[i],rawData->data[i+1]);
	}
	for(i=18;i<34;i+=2)
	{
		naddr<<=1;
		naddr|=IR_2Bit(rawData->data[i],rawData->data[i+1]);
	}
	if(naddr!=((unsigned char)(~addr)))
		return 0;
	for(i=34;i<50;i+=2)
	{
		cmd<<=1;
		cmd|=IR_2Bit(rawData->data[i],rawData->data[i+1]);
	}
	for(i=50;i<66;i+=2)
	{
		ncmd<<=1;
		ncmd|=IR_2Bit(rawData->data[i],rawData->data[i+1]);
	}
	if(ncmd!=((unsigned char)(~cmd)))
		return 0;
	NEC_Data->address=addr;
	NEC_Data->command=cmd;
	return 1;
}
void IR_SendByte(u8 data)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(data&0X80)
		{
			IRsend_mark(_1_SPACE_LEN);
			IRsend_space(_1_MARK_LEN);
		}
		else
		{
			IRsend_mark(_0_SPACE_LEN);
			IRsend_space(_0_MARK_LEN);
		}
		data<<=1;
	}
}
void IR_SendNEC(NEC_DataTypeDef *NEC_Data)
{
	volatile unsigned char addr=NEC_Data->address,naddr=(u8)(~NEC_Data->address),cmd=NEC_Data->command,ncmd=(u8)(~NEC_Data->command);
	IRsend_mark(NEC_LEADER_SPACE_LEN);
	IRsend_space(NEC_LEADER_MARK_LEN);
	IR_SendByte(addr);
	IR_SendByte(naddr);
	IR_SendByte(cmd);
	IR_SendByte(ncmd);
	IRsend_mark(_1_SPACE_LEN);
	IRsend_space(_1_MARK_LEN);	
}

