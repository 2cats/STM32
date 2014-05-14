#include "stm32f10x.h"
#include "HX711.h"
#include "delay.h"
#include "sys.h"
#define OFFSET_AVERNUM	5
static float _HX711_Offset=0;
void Init_HX711(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(HX711_GPIO_RCC, ENABLE);	 //ʹ��PA�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = ADDO_PIN	;		  //ADDO������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		//��������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(PORT, &GPIO_InitStructure);					 //�����趨������ʼ��ADDO
	
  GPIO_InitStructure.GPIO_Pin = ADSK_PIN	;		  //ADSK������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//��������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(PORT, &GPIO_InitStructure);					 //�����趨������ʼ��ADSK

	GPIO_ResetBits(PORT,ADSK_PIN);
	ReSetOffset();
}

float HX711_ReadCount(void)
{
	unsigned long Count;
	unsigned char i;
	float weight;
	
	ADSK=0;   //ʹ��AD��PD_SCK �õͣ�
	Count=0;
	while(ADDO);  //ADת��δ������ȴ�������ʼ��ȡ
	for(i=0;i<24;i++)
	{
	ADSK=1;   //PD_SCK �øߣ��������壩
	Count=Count<<1; //�½�����ʱ����Count����һλ���Ҳಹ��
	ADSK=0;   //PD_SCK �õ�
	if(ADDO) Count++;
	}
	ADSK=1;
	Count=Count^0x800000;//��25�������½�����ʱ��ת������
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



