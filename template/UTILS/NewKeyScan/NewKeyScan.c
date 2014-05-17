#include "NewKeyScan.h"
#include "delay.h"
struct KeyIOPort key_out[4] = {
{GPIOB, GPIO_Pin_5}, {GPIOB, GPIO_Pin_6},
{GPIOB, GPIO_Pin_7}, {GPIOB, GPIO_Pin_9},

};

struct KeyIOPort key_in[3] = {
{GPIOB, GPIO_Pin_12}, {GPIOB, GPIO_Pin_13},
{GPIOB, GPIO_Pin_14}, 
};


unsigned char NewKeyScan_Trg;
unsigned char NewKeyScan_Cont;
void (*NewKeyTriggered)(unsigned char)=0;
#define KEY_GPIO_RCC	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB
#define KEY_BIT(j)	(GPIO_ReadInputDataBit(key_in[j].GPIO_x, key_in[j].GPIO_pin))

void NewKeyScan_Init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
    unsigned char i;
		RCC_APB2PeriphClockCmd(KEY_GPIO_RCC,ENABLE);
		for(i = 0 ;i < 4; i++)
		{
			GPIO_InitStructure.GPIO_Pin = key_out[i].GPIO_pin;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
			GPIO_Init(key_out[i].GPIO_x, &GPIO_InitStructure);	
		}
		for(i = 0 ; i < 3; i++)
		{
			GPIO_InitStructure.GPIO_Pin = key_in[i].GPIO_pin;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
			GPIO_Init(key_in[i].GPIO_x, &GPIO_InitStructure);	
		}
		
		for(i = 0; i < 4; i++)
		{
			GPIO_SetBits(key_out[i].GPIO_x, key_out[i].GPIO_pin); //high
		}
}

void NewKeyScan_Proc(void)
{
  volatile char i, j;
  volatile	unsigned char ReadData;
	for(i = 0; i < 4; i++)
	{
		GPIO_ResetBits(key_out[i].GPIO_x, key_out[i].GPIO_pin);
		for(j=0;j<3;j++)
		{
			if(!KEY_BIT(j))
			{
				ReadData= 3*i + j + 1+'0';//使NewKeyScan_Trg不为0，表示有按键情况
				GPIO_SetBits(key_out[i].GPIO_x, key_out[i].GPIO_pin);
				goto NewKey_Proc;
			}
		}	
		GPIO_SetBits(key_out[i].GPIO_x, key_out[i].GPIO_pin);
		delay_us(30);
	}
	ReadData=0;//使NewKeyScan_Trg为0，表示无按键情况(使得NewKeyScan_Trg,NewKeyScan_Cont必为0，即初始状态)
NewKey_Proc:
		//1、2两句产生的效果:某一按键连续被检测到时，NewKeyScan_Trg只有在第一次检测到时才被赋值为该按键值
		NewKeyScan_Trg=ReadData & (ReadData ^ NewKeyScan_Cont);//1
		NewKeyScan_Cont=ReadData;//2

		//翻译某些按键
		if(NewKeyScan_Trg&&(NewKeyTriggered))
		{
			if(NewKeyScan_Trg==10+'0')
			{
				NewKeyTriggered('Y');
			}else if(NewKeyScan_Trg==11+'0')
			{
				NewKeyTriggered('0');
			}else if(NewKeyScan_Trg==12+'0')
			{
				NewKeyTriggered('B');
			}else if(NewKeyScan_Trg<='9'&&NewKeyScan_Trg>='0'){
				NewKeyTriggered(NewKeyScan_Trg);
			}
			
		}
}