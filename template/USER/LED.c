#include "LED.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(LED_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = RED_LED_PIN|GREEN_LED_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_GPIO, &GPIO_InitStructure);	
}
void Alarm_LED_MAX(void)
{
	GPIO_SetBits(LED_GPIO,RED_LED_PIN);
}
void Alarm_LED_MIN(void)
{
	GPIO_SetBits(LED_GPIO,GREEN_LED_PIN);
}
void Alarm_Release(void)
{
	GPIO_ResetBits(LED_GPIO,GREEN_LED_PIN);
	GPIO_ResetBits(LED_GPIO,RED_LED_PIN);
}
void Speak_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}