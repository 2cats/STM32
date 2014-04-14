#ifndef _ADCMANAGER_H_
#define _ADCMANAGER_H_
#include "stm32f10x.h"
#define ADCMANAGER_ALL_INIT(ADCManager,ChannelsArray,isC) ADCManager.ADC_Channels=ChannelsArray;\
	ADCManager.isContinus=isC;\
	ADCManager.ADC_Channels_num=sizeof(ChannelsArray);\
	ADCManager_Structure(&ADCManager);
	
typedef struct ADCManagerType{
	uint8_t *ADC_Channels;
	uint8_t ADC_Channels_num;
	u8 isTransferDone;
	u8 isContinus;//1£ºÁ¬Ðø
	void (*startTransfer)(void);
	uint16_t (*getData)(uint8_t ADC_Channel);
}ADCManagerTypeDef;

GPIO_TypeDef * filterGPIO_ADC( uint8_t ADC_Channel);
uint16_t  filterPin_ADC( uint8_t ADC_Channel);
uint32_t  filterGPIO_RCC_ADC( void);
void ADCManager_startTransfer(void);
uint16_t ADCManager_getData(uint8_t ADC_Channel);
void ADC_DMA_Config(void);
void ADCManager_Structure(ADCManagerTypeDef *ADCManager);
void DMA1_Channel1_IRQHandler(void);
#endif
