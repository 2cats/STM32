#include "NewKeyScan.h"
#include "delay.h"
static struct KeyIOPort key_row[] = {
{GPIOB, GPIO_Pin_5}, {GPIOB, GPIO_Pin_6},
{GPIOB, GPIO_Pin_7}, {GPIOB, GPIO_Pin_9},

};

static struct KeyIOPort key_col[] = {
{GPIOB, GPIO_Pin_10}, {GPIOB, GPIO_Pin_13},
{GPIOB, GPIO_Pin_14}, {GPIOB, GPIO_Pin_11},
};

void (*NewKeyTriggered)(unsigned char)=0;
unsigned char NewKeyScan_Trg;
unsigned char NewKeyScan_Cont;
static u8 row_Count,col_Count;
#define KEY_GPIO_RCC	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB
#define KEY_BIT(j)	(GPIO_ReadInputDataBit(key_col[j].GPIO_x, key_col[j].GPIO_pin))

void NewKeyScan_Init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
    unsigned char i;
		RCC_APB2PeriphClockCmd(KEY_GPIO_RCC,ENABLE);
		for(i = 0 ;i < 4; i++)
		{
			GPIO_InitStructure.GPIO_Pin = key_row[i].GPIO_pin;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
			GPIO_Init(key_row[i].GPIO_x, &GPIO_InitStructure);	
		}
		for(i = 0 ; i < 4; i++)
		{
			GPIO_InitStructure.GPIO_Pin = key_col[i].GPIO_pin;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
			GPIO_Init(key_col[i].GPIO_x, &GPIO_InitStructure);	
		}
		
		for(i = 0; i < 4; i++)
		{
			GPIO_SetBits(key_row[i].GPIO_x, key_row[i].GPIO_pin); //high
		}
		row_Count=sizeof(key_row)/sizeof(struct KeyIOPort);
		col_Count=sizeof(key_col)/sizeof(struct KeyIOPort);
}
/*
----------------------------------------------------------------------------
1 						2					 3				 4			 ...			in_Count
in_Count+1
.
.
....																			 ...			[out_Count*in_Count]
____________________________________________________________________________
*/
void NewKeyScan_Proc(void)
{
  volatile char i, j;
  volatile	unsigned char ReadData;
	for(i = 0; i < row_Count; i++)
	{
		GPIO_ResetBits(key_row[i].GPIO_x, key_row[i].GPIO_pin);//拉低一根out
		for(j=0;j<col_Count;j++)
		{
			if(!KEY_BIT(j))
			{
				ReadData= 4*i + j + 1;//使NewKeyScan_Trg不为0，表示有按键情况
				GPIO_SetBits(key_row[i].GPIO_x, key_row[i].GPIO_pin);
				goto NewKey_Proc;
			}
		}	
		GPIO_SetBits(key_row[i].GPIO_x, key_row[i].GPIO_pin);
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
				NewKeyTriggered(NewKeyScan_Trg);
//			if(NewKeyScan_Trg==10+'0')
//			{
//				NewKeyTriggered('Y');
//			}else if(NewKeyScan_Trg==11+'0')
//			{
//				NewKeyTriggered('0');
//			}else if(NewKeyScan_Trg==12+'0')
//			{
//				NewKeyTriggered('B');
//			}else if(NewKeyScan_Trg<='9'&&NewKeyScan_Trg>='0'){
//				NewKeyTriggered(NewKeyScan_Trg);
//			}
		}
}