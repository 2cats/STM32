#include "delay.h"
#include "USART2BufReceiever.h" 
#include "USARTBufReceiever.h"
#include "JLX12864G.h"
#include "ds18b20.h"
#include "PWMManager.h" 
#include "KeyFSM.h"
#include "EXTIManager.h" 
#include "millis.h"
#include "newPID.h" 
#include "probe_header.h"
#include "display.h"
#include "btTalk.h"
#include "doEvery.h"
#include "keyscan.h"
#include "gui.h"
#include "Protocol.h"
#include "IRremote.h"
#include "24cxx.h" 
#include <math.h>
#include "LED.h"
#include "NewKeyScan.h"
#define HEATER_CHANNEL TIM2CHANNEL2
#define MOTOR_CHANNEL TIM2CHANNEL1
#define SPEED_STOP_WAIT_TIME	1000   //测速超时置0时间
#define HEATER_FACTOR	3

#define RESTRAINALARM_MAX(m)	(m=m>75?75:m);
#define RESTRAINALARM_MIN(m)	(m=m<1?1:m);
#define RESTRAINTARGET(m)			(m=m>90?30:m);(m=m<5?30:m);
