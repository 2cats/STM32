#ifndef _H_LED_
#define _H_LED_
#include <stm32f10x.h>

#define RED_LED_PIN 		GPIO_Pin_10
#define GREEN_LED_PIN	  GPIO_Pin_9
#define LED_GPIO				GPIOC
#define LED_RCC					RCC_APB2Periph_GPIOC
void LED_Init(void);
void Alarm_LED_MAX(void);
void Alarm_LED_MIN(void);
void Alarm_Release(void);
#endif