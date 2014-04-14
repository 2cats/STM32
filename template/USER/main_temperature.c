/*2014 温控*/
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
#define DEBUG_OUTPUT printf 
#define HEATER_CHANNEL TIM2CHANNEL1
#define MOTOR_CHANNEL TIM2CHANNEL2
#define SPEED_STOP_WAIT_TIME	1000
//结构体变量
USARTBufReceieverTypeDef USARTBufReceiever;
PWMManagerTypeDef PWMManager;
KeyFSMTypeDef aKeyFSM1;
KeyFSMTypeDef aKeyFSM2;
KeyFSMTypeDef aKeyFSM3;
KeyFSMTypeDef aKeyFSM4;
EXTIManagerTypeDef EXTIManager;
PIDTypeDef PID;
//PID变量
double PID_input,PID_output,PID_target=60;//PID_input为温度值，PID_output反映占空比
double PID_lastinput;//温度缓存，改善DS18B20错误
double KP=33.5,KI=1.17 ,KD=50,PIDSampleTime=50;
int OUTPUT_REAL_MAX=100;


//速度变量
unsigned long time2speed;
double speed;

//控制变量
u8 STOP_HEATER=0,STOP_MOTOR=0;
u8 IS_KEY_WORKING=0;

//部分函数声明
void writeMotors(void);
unsigned char PID_proc(void);
void update_Temprature(void);

//回调函数
//实际在中断中执行，故不宜操作12864，只在其中设置标志位，由regularDisplay函数进行显示操作
void doWhenClickKey1(struct KeyFSMType *self)
{
	if(!IS_TMP_CONFIGUREING)
	{
		SET_TMP_CONFIGUREING;
		SET_TMP_KEY_NU_S;
	}else
	{
		SET_TMP_KEY_NU_R;
		UNSET_TMP_CONFIGUREING;
	}
}
void doWhenClickKey2(struct KeyFSMType *self)
{
	if(IS_MOTOR_CONFIGUREING)
	{
		PID_output+=0.1;
		if(PID_output>=1)
			PID_output=1;
		writeMotors();
	}
	else if(IS_TMP_CONFIGUREING)
	{
		++PID_target;
		SET_PID_Target_NU;
	}
}
void doWhenClickKey3(struct KeyFSMType *self)
{
	if(IS_MOTOR_CONFIGUREING)
	{
		PID_output-=0.1;
		if(PID_output<=0)
			PID_output=0;
		writeMotors();
	}
	else if(IS_TMP_CONFIGUREING)
	{
		--PID_target;
		SET_PID_Target_NU;
	}
}
void doWhenClickKey4(struct KeyFSMType *self)
{
	if(!IS_MOTOR_CONFIGUREING)
	{
		SET_MOTOR_CONFIGUREING;
		SET_MOTOR_KEY_NU_S;
	}else
	{
		SET_MOTOR_KEY_NU_R;
	 	UNSET_MOTOR_CONFIGUREING;
	}
}
//霍尔中断测速
void doWhenTrigger( struct EXTILineType *triggeredLine)
{
	speed=(1000.0/(millis()-time2speed))/2.0;
	time2speed=millis();
}

//初始化
void All_Init()
{
//延时
	delay_init();
//millis
	MILLIS_Init();
//串口
//	ProbeCom_Init(); 
//	ProbeRS232_Init(115200);
//	ProbeRS232_RxIntEn();	
	uart_init_BR(115200,&USARTBufReceiever);//串口1开缓存
	uart2_init(115200);
 	BT_IT_Init();//开蓝牙中断
//12864
	OLED_Init();
	initData_Display();
	initKey_Display();
	setTarget_Display(PID_target);
//按键状态机
	Key_FSM_Structure(&aKeyFSM1,GPIOB,GPIO_Pin_1,300);//300表示双击的间隔等待时长（ms）
	Key_FSM_Structure(&aKeyFSM2,GPIOB,GPIO_Pin_2,300);
	Key_FSM_Structure(&aKeyFSM3,GPIOB,GPIO_Pin_3,300);
	Key_FSM_Structure(&aKeyFSM4,GPIOB,GPIO_Pin_4,300);
	aKeyFSM1.doWhenClick=doWhenClickKey1;
	aKeyFSM2.doWhenClick=doWhenClickKey2;
	aKeyFSM3.doWhenClick=doWhenClickKey3;
	aKeyFSM4.doWhenClick=doWhenClickKey4;
	
//外部中断_测速
	EXTIManager_Structure(&EXTIManager);
	EXTIManager.startListenling(GPIOB,GPIO_Pin_0,EXTI_Falling,doWhenTrigger);
//PWM
	PWMManagerStructure(&PWMManager);
	PWMManager.ProduceNewPWM(MOTOR_CHANNEL,25000,0);
	PWMManager.ProduceNewPWM(HEATER_CHANNEL,25000,0);
//PID
	PID_Structure_Init(KP,KI,KD,PIDSampleTime,&PID);//10msPID计算周期
	PID.SetIOT(&PID_input,&PID_output,&PID_target);//IOT的值均可在函数外任意修改
	PID.SetOutputLimits(-OUTPUT_REAL_MAX,OUTPUT_REAL_MAX);
//DS18B20
	while(DS18B20_Init())//：返回0表示存在
		DEBUG_OUTPUT("DS18B20 ERROR!\n");
}

//输出函数
void writeMotors()
{
	double realOutput=PID_output/OUTPUT_REAL_MAX;//归一化
	if(realOutput>0&&!STOP_HEATER)
	{
		//电阻在占空比0.7内为线性
		realOutput=PID_output*0.7;
		PWMManager.changeRatio(HEATER_CHANNEL,realOutput);
		PWMManager.changeRatio(MOTOR_CHANNEL,0);
	}
	else if(!STOP_MOTOR)
	{
		//风扇输出*3
		realOutput=PID_output*3<1?PID_output*3:1;
		PWMManager.changeRatio(HEATER_CHANNEL,0);
		PWMManager.changeRatio(MOTOR_CHANNEL,-realOutput);
	}
		
}
unsigned char PID_proc()
{
	//用于运行时响应参数的改变
	PID.SetPID(KP,KI,KD);
	//PID操作  返回1表示有操作，返回0表示采样时间未到未操作
	return PID.Compute();
	//...
}

void update_Temprature()
{
	PID_input=DS18B20_Get_Temp();//读温度
	if(PID_input>90||PID_input<10)//改善DS12B20读数错误
		PID_input=PID_lastinput;
	PID_lastinput=PID_input;
	
	if(PID_input>60)//温度大于60度时，停止加热，题目要求
		STOP_HEATER=1;
	else
		STOP_HEATER=0;
	if(PID_input<40)//温度小于40度时，停止风扇
		STOP_MOTOR=1;
	else
		STOP_MOTOR=0;
	update_Temprature_Display(PID_input);
}
//蓝牙通信
void BT_Talk(void)
{
		BT_SendTMP(PID_input);//发送温度
		BT_SendSPD(speed);//发送速度
		BT_SendPOW(PID_output);//发送功率
}
void regularDisplay(void)//根据标志位进行12864显示
{
		OLED_printfAt(5,DATA_POSITION,"%f",PID_output);
		OLED_printfAt(3,DATA_POSITION,"%f",speed);
		if(IS_PID_Target_NU)
		{
			OLED_printfAt(4,DATA_POSITION,"%f",PID_target);
			UNSET_PID_Target_NU;
		}
		if(IS_TMP_KEY_NU_S)
		{
			OLED_printfAt(8,1,"T... ");
			OLED_printfAt(8,40,"Up");
			OLED_printfAt(8,66,"Dowm");
			UNSET_TMP_KEY_NU_S;
			
		}else if(IS_TMP_KEY_NU_R)
		{
			OLED_printfAt(8,1,"Tmp ");
			OLED_printfAt(8,40,"   ");
			OLED_printfAt(8,66,"    ");
			UNSET_TMP_KEY_NU_R;
		}
		if(IS_MOTOR_KEY_NU_S)
		{
			OLED_printfAt(8,110,"F... ");
			OLED_printfAt(8,40,"Up");
			OLED_printfAt(8,66,"Dowm");
			UNSET_MOTOR_KEY_NU_S;
		}else if(IS_MOTOR_KEY_NU_R)
		{
			OLED_printfAt(8,110,"Fan  ");
			OLED_printfAt(8,40,"   ");
			OLED_printfAt(8,66,"    ");
			UNSET_MOTOR_KEY_NU_R;
		}
		
}
//运行一些需要定时/延时的操作
void delayFuncs(void)
{
	if(millis()-time2speed>SPEED_STOP_WAIT_TIME)//风扇中断超时，把风扇的速度置为0
	{
		speed=0;
		OLED_printfAt(3,DATA_POSITION,"%f  ",speed);
	}
}
int main()
{
	All_Init();
	while(1)
	{
		update_Temprature();
	
		if(PID_proc())
		{
			writeMotors();
			BT_Talk();
			regularDisplay();
			delayFuncs();
		}
		if(millis()>4000&&!IS_KEY_WORKING)//4s后开启键盘功能，因为键盘3和SWJ有冲突，避免无法下载程序
		{	
			IS_KEY_WORKING=1;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
		}
	}
}