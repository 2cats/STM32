#include "ds18b20.h"
#include "delay.h"	
/***************INTERFACE****************/ 
/*
Example:

	while(DS18B20_Init())//������0��ʾ����
		printf("error\n");
	while(1)
	{
		delay_ms(800);
		printf("temprature:%lf\n",DS18B20_Get_Temp());
	}


��ʼ����
	Ĭ��PG11�������޸���ı����²�����
	#define DS18B20_GPIO GPIOG
	#define DS18B20_Pin	 11   	
	#define	DS18B20_DQ_OUT PGout(DS18B20_Pin) //���ݶ˿�	PA0 
	#define	DS18B20_DQ_IN  PGin(DS18B20_Pin)  //���ݶ˿�	PA0 
	//#define DS18B20_GPIO_0_7 
	u8 DS18B20_Init(void)
	{
 	...
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PORTG��ʱ�� 
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				//PORTG.11 �������
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOG, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOG,GPIO_Pin_11);    //���1
	....
	}
��ȡ�¶ȣ� 
	short DS18B20_Get_Temp(void)[����ֵ��-55~125,���ȣ�0.0625C] 
�ٶȣ�
	һ��ת������750ms
*/						  


//��λDS18B20
void DS18B20_Rst(void)	   
{                 
	DS18B20_IO_OUT(); //SET PA0 OUTPUT
    DS18B20_DQ_OUT=0; //����DQ
    delay_us(750);    //����750us
    DS18B20_DQ_OUT=1; //DQ=1 
	delay_us(15);     //15US
}
//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽DS18B20�Ĵ���
//����0:����
u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;
	DS18B20_IO_IN();//SET PA0 INPUT	 
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}
//��DS18B20��ȡһ��λ
//����ֵ��1/0
u8 DS18B20_Read_Bit(void) 			 // read one bit
{
    u8 data;
	DS18B20_IO_OUT();//SET PA0 OUTPUT
    DS18B20_DQ_OUT=0; 
	delay_us(2);
    DS18B20_DQ_OUT=1; 
	DS18B20_IO_IN();//SET PA0 INPUT
	delay_us(12);
	if(DS18B20_DQ_IN)data=1;
    else data=0;	 
    delay_us(50);           
    return data;
}
//��DS18B20��ȡһ���ֽ�
//����ֵ������������
u8 DS18B20_Read_Byte(void)    // read one byte
{        
    u8 i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}
//дһ���ֽڵ�DS18B20
//dat��Ҫд����ֽ�
void DS18B20_Write_Byte(u8 dat)     
 {             
    u8 j;
    u8 testb;
	DS18B20_IO_OUT();//SET PA0 OUTPUT;
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            DS18B20_DQ_OUT=0;// Write 1
            delay_us(2);                            
            DS18B20_DQ_OUT=1;
            delay_us(60);             
        }
        else 
        {
            DS18B20_DQ_OUT=0;// Write 0
            delay_us(60);             
            DS18B20_DQ_OUT=1;
            delay_us(2);                          
        }
    }
}
//��ʼ�¶�ת��
void DS18B20_Start(void)// ds1820 start convert
{   						               
    DS18B20_Rst();	   
	DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
} 
//��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���
//����1:������
//����0:����    	 
u8 DS18B20_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PORTG��ʱ�� 
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				//PORTG.11 �������
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOA,GPIO_Pin_11);    //���1

	DS18B20_Rst();

	return DS18B20_Check();
}  
//UPDATE:
//�������ֵ�ֱ��ʣ��޸ķ�������Ϊdouble
//��ds18b20�õ��¶�ֵ
//���ȣ�0.0625C
//����ֵ���¶�ֵ ��-55~125�� 
double DS18B20_Get_Temp(void)
{
    u8 zhengfu;
    u8 TL,TH;
		short temRaw;
		double finalTemp;
    DS18B20_Start ();                    // ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0xbe);// convert	    
    TL=DS18B20_Read_Byte(); // LSB   
    TH=DS18B20_Read_Byte(); // MSB  

    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        zhengfu=0;//�¶�Ϊ��  
    }else zhengfu=1;//�¶�Ϊ��	  	  
    temRaw=TH; //��ø߰�λ
    temRaw<<=8;    
    temRaw+=TL;//��õװ�λ
    finalTemp=(double)temRaw*0.0625;//ת��    
		return zhengfu?finalTemp:-finalTemp;
} 
 
