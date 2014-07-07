#ifndef _HX711_H
#define _HX711__H
#ifdef __cplusplus
 extern "C" {
#endif

#define ADDO_PIN	  GPIO_Pin_0
#define ADSK_PIN	  GPIO_Pin_1
#define PORT  GPIOA
#define HX711_GPIO_RCC		RCC_APB2Periph_GPIOA


#define ADSK  PAout(1)
#define ADDO  PAin(0)
void Init_HX711(void);
float HX711_ReadCount(void);
float HX711_Value(unsigned long num);
float ReSetOffset(void);
#ifdef __cplusplus
 }
#endif

#endif

