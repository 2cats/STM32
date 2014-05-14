#include "stm32f10x.h"
#include "HX711.h"
#include "delay.h"
#include "sys.h"
#define OFFSET_AVERNUM	5
static float _HX711_Offset=0;
void Init_HX711(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(HX711_GPIO_RCC, ENABLE);	 //使能PA端口时钟
	
	GPIO_InitStructure.GPIO_Pin = ADDO_PIN	;		  //ADDO的引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		//浮空输入
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(PORT, &GPIO_InitStructure);					 //根据设定参数初始化ADDO
	
  GPIO_InitStructure.GPIO_Pin = ADSK_PIN	;		  //ADSK的引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//浮空输入
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(PORT, &GPIO_InitStructure);					 //根据设定参数初始化ADSK

	GPIO_ResetBits(PORT,ADSK_PIN);
	ReSetOffset();
}

float HX711_ReadCount(void)
{
	unsigned long Count;
	unsigned char i;
	float weight;
	
	ADSK=0;   //使能AD（PD_SCK 置低）
	Count=0;
	while(ADDO);  //AD转换未结束则等待，否则开始读取
	for(i=0;i<24;i++)
	{
	ADSK=1;   //PD_SCK 置高（发送脉冲）
	Count=Count<<1; //下降沿来时变量Count左移一位，右侧补零
	ADSK=0;   //PD_SCK 置低
	if(ADDO) Count++;
	}
	ADSK=1;
	Count=Count^0x800000;//第25个脉冲下降沿来时，转换数据
	ADSK=0;
	weight=HX711_Value(Count);
	return(weight);
}

float HX711_Value(unsigned long num)
{
	float num_f;
	
  num_f=num/8388607.0*3.30;//(2^13-1)*Power Voltage    
	return ((num_f*1000-64.044)*1/0.763-_HX711_Offset);
}
float ReSetOffset(void)
{ 
	int i;
	double offsetsum=0;
		for(i=0;i<OFFSET_AVERNUM;i++)
	{
		offsetsum+=HX711_ReadCount();
	}
	_HX711_Offset=offsetsum/OFFSET_AVERNUM;
	return _HX711_Offset;
}



