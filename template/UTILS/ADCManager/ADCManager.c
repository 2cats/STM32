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
	±ØÌîÊôĞÔ£º
		ADC_Channels
		ADC_Channels_num
		isContinus
....
*/

/**********WARNING************/
/*
	isTransferDoneÊôĞÔ
		ÔÚDMA´«ÊäÍê³ÉÖĞ¶ÏÖĞ±»ÖÃÎ»£¬ĞèÊÖ¶¯ÇåÁã£¬²»¹ıÃ¿´Îµ÷ÓÃstartTransferÇ°£¬»áÏÈÇåÁã
	ÄÚ²¿µçÑ¹²Î¿¼µçÑ¹CH17 & ÄÚ²¿ÎÂ¶È´«¸ĞÆ÷CH16  Ê¹ÓÃÈçExampleÖĞËùÊ¾£¬ÓëÆÕÍ¨Í¨µÀÊ¹ÓÃ·½·¨ÏàÍ¬
		ÄÚ²¿µçÑ¹²Î¿¼µçÑ¹µäĞÍÖµ:1.2V  Vref=RAWDATA*(3.3/4096)
		ÄÚ²¿ÎÂ¶È´«¸ĞÆ÷ÎÂ¶È×ª»»¹«Ê½£º temperature=(1.43-RAWDATA*(3.3/4096))/0.0043+25
	¬
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
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//Ê¹ÄÜÖ¸¶¨µÄADC1µÄÈí¼ş×ª»»Æô¶¯¹¦ÄÜ	
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
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;  //TIM3ÖĞ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //ÏÈÕ¼ÓÅÏÈ¼¶2¼¶
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //´ÓÓÅÏÈ¼¶0¼¶
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQÍ¨µÀ±»Ê¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);  //¸ù¾İNVIC_InitStructÖĞÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷ 

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
	RCC_APB2PeriphClockCmd(filterGPIO_RCC_ADC() | RCC_APB2Periph_ADC1	, ENABLE );	  //Ê¹ÄÜADC1Í¨µÀÊ±ÖÓ
 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //ÉèÖÃADC·ÖÆµÒò×Ó6 72M/6=12,ADC×î´óÊ±¼ä²»ÄÜ³¬¹ı14M

	for(i=0;i<defaultADCManager->ADC_Channels_num;i++)
	{
		if(defaultADCManager->ADC_Channels[i]==ADC_Channel_16|defaultADCManager->ADC_Channels[i]==ADC_Channel_17)
			continue;
		GPIO_InitStructure.GPIO_Pin = filterPin_ADC(defaultADCManager->ADC_Channels[i]);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//Ä£ÄâÊäÈëÒı½Å
		GPIO_Init(filterGPIO_ADC(defaultADCManager->ADC_Channels[i]), &GPIO_InitStructure);	
	}                    

	ADC_DeInit(ADC1);  //¸´Î»ADC1,½«ÍâÉè ADC1 µÄÈ«²¿¼Ä´æÆ÷ÖØÉèÎªÈ±Ê¡Öµ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC¹¤×÷Ä£Ê½:ADC1ºÍADC2¹¤×÷ÔÚ¶ÀÁ¢Ä£Ê½
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//Ä£Êı×ª»»¹¤×÷ÔÚµ¥Í¨µÀÄ£Ê½
	ADC_InitStructure.ADC_ContinuousConvMode = (FunctionalState)defaultADCManager->isContinus;	//Ä£Êı×ª»»¹¤×÷ÔÚµ¥´Î×ª»»Ä£Ê½
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//×ª»»ÓÉÈí¼ş¶ø²»ÊÇÍâ²¿´¥·¢Æô¶¯
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADCÊı¾İÓÒ¶ÔÆë
	ADC_InitStructure.ADC_NbrOfChannel = defaultADCManager->ADC_Channels_num;	//Ë³Ğò½øĞĞ¹æÔò×ª»»µÄADCÍ¨µÀµÄÊıÄ¿
	ADC_Init(ADC1, &ADC_InitStructure);	//¸ù¾İADC_InitStructÖĞÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèADCxµÄ¼Ä´æÆ÷   

	for(i=0;i<defaultADCManager->ADC_Channels_num;i++)
	{
		if(defaultADCManager->ADC_Channels[i]==ADC_Channel_16|defaultADCManager->ADC_Channels[i]==ADC_Channel_17)
			ADC_TempSensorVrefintCmd(ENABLE);
		ADC_RegularChannelConfig(ADC1, defaultADCManager->ADC_Channels[i], i+1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCÍ¨µÀ,²ÉÑùÊ±¼äÎª239.5ÖÜÆÚ	
	}
	
	ADC_DMA_Config();
	ADC_DMACmd(ADC1, ENABLE); 
	
	ADC_Cmd(ADC1, ENABLE);	//Ê¹ÄÜÖ¸¶¨µÄADC1
	
	ADC_ResetCalibration(ADC1);	//Ê¹ÄÜ¸´Î»Ğ£×¼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//µÈ´ı¸´Î»Ğ£×¼½áÊø
	
	ADC_StartCalibration(ADC1);	 //¿ªÆôADĞ£×¼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //µÈ´ıĞ£×¼½áÊø
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
