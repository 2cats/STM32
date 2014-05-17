#include "keyscan.h"
#include "delay.h"
#include "stm32f10x_gpio.h"
 struct io_port key_output[4] = {
{GPIOB, GPIO_Pin_5}, {GPIOB, GPIO_Pin_6},
{GPIOB, GPIO_Pin_7}, {GPIOB, GPIO_Pin_9},

};

struct io_port key_input[3] = {
{GPIOB, GPIO_Pin_12}, {GPIOB, GPIO_Pin_13},
{GPIOB, GPIO_Pin_14}, 
};


void keyboard_init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
    unsigned char i;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);
//按键扫描输出线 输出低电平
/* PB9 PB8 PB7 PB6 PB5 PG15 输出*/
		for(i = 0 ;i < 4; i++)
		{
			GPIO_InitStructure.GPIO_Pin = key_output[i].GPIO_pin;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(key_output[i].GPIO_x, &GPIO_InitStructure);	
		}
		for(i = 0 ; i < 3; i++)
		{
			GPIO_InitStructure.GPIO_Pin = key_input[i].GPIO_pin;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(key_input[i].GPIO_x, &GPIO_InitStructure);	
		}
		
		for(i = 0; i < 4; i++)
		{
			GPIO_SetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin); //high
		}
}
unsigned char update_key(void)
{
  //delay_init();
	unsigned char keyreturn = 0;
  char i, j;
	for(i = 0; i < 4; i++)
	{
		GPIO_ResetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);
			for(j = 0; j <3; j++)
			{
				 if(GPIO_ReadInputDataBit(key_input[j].GPIO_x, key_input[j].GPIO_pin) == 0)
				 {
						delay_ms(20);
					  if(GPIO_ReadInputDataBit(key_input[j].GPIO_x, key_input[j].GPIO_pin) == 0)
					  {
							keyreturn = 3*i + j + 1;
							if(keyreturn < 10)
							{
								return keyreturn + '0';
							}
							else if(keyreturn == 10)
							{
								return 'Y';
							}
							else if(keyreturn == 11)
							{
								return '0';
							}
							else if(keyreturn == 12)
							{
								return 'B';
							}
						};
					}
				
			}
			GPIO_SetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);
		
	}
	return 0;
}

void KeyFSM_Init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
    unsigned char i;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);
//按键扫描输出线 输出低电平
/* PB9 PB8 PB7 PB6 PB5 PG15 输出*/
		for(i = 0 ;i < 4; i++)
		{
			GPIO_InitStructure.GPIO_Pin = key_output[i].GPIO_pin;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(key_output[i].GPIO_x, &GPIO_InitStructure);	
		}
		for(i = 0 ; i < 3; i++)
		{
			GPIO_InitStructure.GPIO_Pin = key_input[i].GPIO_pin;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(key_input[i].GPIO_x, &GPIO_InitStructure);	
		}
		
		for(i = 0; i < 4; i++)
		{
			GPIO_SetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin); //high
		}
}

unsigned char _Scan_Key_Value;
unsigned char _Scan_Key_FSM;
char ScanKeyValue(void)
{
  char i, j;
	for(i = 0; i < 4; i++)
	{
		GPIO_ResetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);
			for(j = 0; j <3; j++)
			{
				 if(GPIO_ReadInputDataBit(key_input[j].GPIO_x, key_input[j].GPIO_pin) == 0)
				 {
						return (3*i + j + 1);
				 }
				
			}
			GPIO_SetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);
	}
	return 0xff;
}
void (*DoWhenMKTriggered)(char value)=0;
void Scan_MatrixKey_Proc(void)
{
	switch(_Scan_Key_FSM)
	{
		case SCAN_KEY_IDLE:
		 	if((_Scan_Key_Value=ScanKeyValue())!=0xff)
			{
				_Scan_Key_FSM=SCAN_KEY_M1;
			}
			break;
	 	case SCAN_KEY_M1:
			if(_Scan_Key_Value==ScanKeyValue())
			{
				_Scan_Key_FSM=SCAN_KEY_IDLE;
				_Scan_Key_Value=0xff;
				if(DoWhenMKTriggered!=0)
				{
					DoWhenMKTriggered(_Scan_Key_Value);
				}
			}
			break;	
	}
}


		