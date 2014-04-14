//#define USING_PROBE

/*******使用方法*******/
/*
	添加对应串口的lib，使能上面的USING_PROBE，调用如下三个函数：
//  #include "probe_header.h"
//	ProbeCom_Init();  Now J-Link is ready2use 
//	ProbeRS232_Init(115200);  Initialize the RS-232 interface  
//	ProbeRS232_RxIntEn();

*/

#ifdef USING_PROBE
#include "probe_com_cfg.h"



void  ProbeRS232_RxTxISRHandler (void);
#ifdef PROBE_USING_USART1
void USART1_IRQHandler(void)
{	
	ProbeRS232_RxTxISRHandler();
}
#endif
#ifdef PROBE_USING_USART2
void USART2_IRQHandler(void)
{	
	UltraProbe_IRq_Handler();
}
#endif

#endif


