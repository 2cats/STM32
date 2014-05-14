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
	必填属性：
		ADC_Channels
		ADC_Channels_num
		isContinus
....
*/

/**********WARNING************/
/*
	isTransferDone属性
		在DMA传输完成中断中被置位，需手动清零，不过每次调用startTransfer前，会先清零，若初始化时指定连续转换，该属性意义不大
	内部电压参考电压CH17 & 内部温度传感器CH16  使用如Example中所示，与普通通道使用方法相同
		内部电压参考电压典型值:1.2V  Vref=RAWDATA*(3.3/4096)
		内部温度传感器温度转换公式： temperature=(1.43-RAWDATA*(3.3/4096))/0.0043+25

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
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
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
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 

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
	RCC_APB2PeriphClockCmd(filterGPIO_RCC_ADC() | RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC1通道时钟
 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	for(i=0;i<defaultADCManager->ADC_Channels_num;i++)
	{
		if(defaultADCManager->ADC_Channels[i]==ADC_Channel_16|defaultADCManager->ADC_Channels[i]==ADC_Channel_17)
			continue;
		GPIO_InitStructure.GPIO_Pin = filterPin_ADC(defaultADCManager->ADC_Channels[i]);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
		GPIO_Init(filterGPIO_ADC(defaultADCManager->ADC_Channels[i]), &GPIO_InitStructure);	
	}                    

	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = (FunctionalState)defaultADCManager->isContinus;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = defaultADCManager->ADC_Channels_num;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

	for(i=0;i<defaultADCManager->ADC_Channels_num;i++)
	{
		if(defaultADCManager->ADC_Channels[i]==ADC_Channel_16|defaultADCManager->ADC_Channels[i]==ADC_Channel_17)
			ADC_TempSensorVrefintCmd(ENABLE);
		ADC_RegularChannelConfig(ADC1, defaultADCManager->ADC_Channels[i], i+1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	
	}
	
	ADC_DMA_Config();
	ADC_DMACmd(ADC1, ENABLE); 
	
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
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
