#include "btTalk.h"
#include "stm32f10x_usart.h"

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

NEC_DataTypeDef NEC_Data;

u8 IR_CODE[]={ 0x42,//'0'
 0x68,//'1'
 0x98,
 0xb0,
 0x30,
 0x18,
 0x7a,
 0x10,
 0x38,
 0x5a,//'9'
 0x52,//'B'
 0xA8,//'Y'
 0xFF,//'B<<'
 0x90,//'P'
 0xE0,//'N'
};
u8 getIRCode(u8 rawCode)
{
	u8 i;
	if(rawCode==IR_CODE[10]||rawCode==IR_CODE[12])
	{
		return 'B';
	}else if(rawCode==IR_CODE[11])
	{
		return 'Y';
	}else if(rawCode==IR_CODE[13])
	{
		return 'P';
	}else if(rawCode==IR_CODE[14])
	{
		return 'N';
	}
	
	
	for(i=0;i<10;i++)
	{
		if(IR_CODE[i]==rawCode)
		{
			return i+'0';
		}
	}
	return 0xff;
	
}
void IRrecv_Handler(IR_RawDataTypeDef *raw)
{
	if(NEC_Parse(raw,&NEC_Data))
	{	
		FSM_Proc(getIRCode(NEC_Data.command));
	}
}

void UART1_Send_Double(double data)
{
	char str[9];
	str[8]='\0';
	double2char(data,str);
	printf("<%s>>",str);
}

