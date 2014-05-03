#include "ADCManager.h"
/******INTERFACE********/
/*
Example:
//config by ADCMANAGER_ALL_INIT
	ADCManagerTypeDef ADCManager;
	uint8_t Channels[]={ADC_Channel_2,ADC_Channel_16,ADC_Channel_17};
	ADCMANAGER_ALL_INIT(ADCManager,Channels,1);
	
	ADCManager.startTransfer();
	
	if(ADCManager.isTransferDone)
	{
		printf("data:%d\tdata:%d\t%d\n",ADCManager.getData(ADC_Channel_2),ADCManager.getData(ADC_Channel_16),ADCManager.getData(ADC_Channel_17));
		ADCManager.isTransferDone=0;
	}
//config manaly
....
	�������ԣ�
		ADC_Channels
		ADC_Channels_num
		isContinus
....
*/

/**********WARNING************/
/*
	isTransferDone����
		��DMA��������ж��б���λ�����ֶ����㣬����ÿ�ε���startTransferǰ����������
	�ڲ���ѹ�ο���ѹCH17 & �ڲ��¶ȴ�����CH16  ʹ����Example����ʾ������ͨͨ��ʹ�÷�����ͬ
		�ڲ���ѹ�ο���ѹ����ֵ:1.2V  Vref=RAWDATA*(3.3/4096)
		�ڲ��¶ȴ������¶�ת����ʽ�� temperature=(1.43-RAWDATA*(3.3/4096))/0.0043+25
	�
*/
//variables
/***********************/
uint16_t ADC_DEFAULT_DATA[17]; 

ADCManagerTypeDef *defaultADCManager;

//Filter
/***********************/
GPIO_TypeDef * filterGPIO_ADC( uint8_t ADC_Channel)
{
	switch(ADC_Channel)
	{
		case ADC_Channel_0:return GPIOA;
		case ADC_Channel_1:return GPIOA;
		case ADC_Channel_2:return GPIOA;
		case ADC_Channel_3:return GPIOA;
		case ADC_Channel_4:return GPIOA;
		case ADC_Channel_5:return GPIOA;
		case ADC_Channel_6:return GPIOA;
		case ADC_Channel_7:return GPIOA;
		case ADC_Channel_8:return GPIOB;
		case ADC_Channel_9:return GPIOB;
		case ADC_Channel_10:return GPIOC;
		case ADC_Channel_11:return GPIOC;
		case ADC_Channel_12:return GPIOC;
		case ADC_Channel_13:return GPIOC;
		case ADC_Channel_14:return GPIOC;
		case ADC_Channel_15:return GPIOC;
		default :return 0;
	}
}
uint16_t  filterPin_ADC( uint8_t ADC_Channel)
{
	switch(ADC_Channel)
	{
		case ADC_Channel_0:return GPIO_Pin_0;
		case ADC_Channel_1:return GPIO_Pin_1;
		case ADC_Channel_2:return GPIO_Pin_2;
		case ADC_Channel_3:return GPIO_Pin_3;
		case ADC_Channel_4:return GPIO_Pin_4;
		case ADC_Channel_5:return GPIO_Pin_5;
		case ADC_Channel_6:return GPIO_Pin_6;
		case ADC_Channel_7:return GPIO_Pin_7;
		case ADC_Channel_8:return GPIO_Pin_0;
		case ADC_Channel_9:return GPIO_Pin_1;
		case ADC_Channel_10:return GPIO_Pin_0;
		case ADC_Channel_11:return GPIO_Pin_1;
		case ADC_Channel_12:return GPIO_Pin_2;
		case ADC_Channel_13:return GPIO_Pin_3;
		case ADC_Channel_14:return GPIO_Pin_4;
		case ADC_Channel_15:return GPIO_Pin_5;
	}
	return 0;
}
uint32_t  filterGPIO_RCC_ADC( void)
{ 
	uint32_t MYRCC=0;
	u8 i;
	GPIO_TypeDef * tmp_GPIO;
	u8 tmp=0; 
	for(i=0;i<defaultADCManager->ADC_Channels_num;i++)
	{
		tmp_GPIO=filterGPIO_ADC(defaultADCManager->ADC_Channels[i]);
		if(tmp_GPIO==GPIOA)
			tmp&=1;
		else if(tmp_GPIO==GPIOB)
			tmp&=2;
		else if(tmp_GPIO==GPIOC)
			tmp&=4;
	}	
	if(tmp&1)
	{
		MYRCC|=RCC_APB2Periph_GPIOA;
	}
	if(tmp&2)
	{
		MYRCC|=RCC_APB2Periph_GPIOB;
	}
	if(tmp&4)
	{
		MYRCC|=RCC_APB2Periph_GPIOC;
	}
	return MYRCC;
}
//Funcs
/***********************/
void ADCManager_startTransfer(void)
{
	defaultADCManager->isTransferDone=0;
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
}
uint16_t ADCManager_getData(uint8_t ADC_Channel)
{
	u8 i;
	for(i=0;i<defaultADCManager->ADC_Channels_num;i++)
	{
		if(defaultADCManager->ADC_Channels[i]==ADC_Channel)
			return ADC_DEFAULT_DATA[i];
	}
	return 0;
}
void ADC_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA??

	DMA_DeInit(DMA1_Channel1);

	DMA_InitStructure.DMA_PeripheralBaseAddr =  (uint32_t) & (ADC1->DR); //????????
	DMA_InitStructure.DMA_MemoryBaseAddr =(u32)ADC_DEFAULT_DATA; //RAM???
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //????
	DMA_InitStructure.DMA_BufferSize = defaultADCManager->ADC_Channels_num;//Size
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //???????????
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //RAM?????????
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize =DMA_MemoryDataSize_HalfWord; //????!!!!!!!!


	DMA_InitStructure.DMA_Mode =defaultADCManager->isContinus?DMA_Mode_Circular:DMA_Mode_Normal;//DMA_Mode_Normal; //??????
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //???
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //???RAM2RAM
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); 

	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE ); //DMA??(???NVIC)
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 

	DMA_Cmd(DMA1_Channel1,ENABLE);//??DMA
}
void ADCManager_Structure(ADCManagerTypeDef *ADCManager)
{

	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	u8 i;
	defaultADCManager=ADCManager;
	defaultADCManager->getData=ADCManager_getData;
	defaultADCManager->startTransfer=ADCManager_startTransfer;
	//defaultADCManager->ADC_Channels_num=sizeof(defaultADCManager->ADC_Channels);
	RCC_APB2PeriphClockCmd(filterGPIO_RCC_ADC() | RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	for(i=0;i<defaultADCManager->ADC_Channels_num;i++)
	{
		if(defaultADCManager->ADC_Channels[i]==ADC_Channel_16|defaultADCManager->ADC_Channels[i]==ADC_Channel_17)
			continue;
		GPIO_InitStructure.GPIO_Pin = filterPin_ADC(defaultADCManager->ADC_Channels[i]);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
		GPIO_Init(filterGPIO_ADC(defaultADCManager->ADC_Channels[i]), &GPIO_InitStructure);	
	}                    

	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = (FunctionalState)defaultADCManager->isContinus;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = defaultADCManager->ADC_Channels_num;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

	for(i=0;i<defaultADCManager->ADC_Channels_num;i++)
	{
		if(defaultADCManager->ADC_Channels[i]==ADC_Channel_16|defaultADCManager->ADC_Channels[i]==ADC_Channel_17)
			ADC_TempSensorVrefintCmd(ENABLE);
		ADC_RegularChannelConfig(ADC1, defaultADCManager->ADC_Channels[i], i+1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	
	}
	
	ADC_DMA_Config();
	ADC_DMACmd(ADC1, ENABLE); 
	
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}


//IRQs
/***********************/
void DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1))
	{
		defaultADCManager->isTransferDone=1;
		DMA_ClearITPendingBit(DMA1_IT_TC1);
	}
}
