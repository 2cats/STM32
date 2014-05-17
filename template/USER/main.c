/*2014 温控Master*/
#include "include.h"
//结构体变量
USARTBufReceieverTypeDef USARTBufReceiever;
PWMManagerTypeDef PWMManager;
EXTIManagerTypeDef EXTIManager;
PIDTypeDef PID;

//PID变量
double PID_target=70;
double PID_input=0;
double PID_output=0;
//PID参数
double KP=6.6834,KI=0.08 ,KD=140,PIDSampleTime=50;
double heater_PWM=1;int OUTPUT_REAL_MAX=100;
double PID_alarm_MIN;
double PID_alarm_MAX;

//速度变量
double speed;

//辅助变量
double PID_lastinput=0;//温度缓存，改善DS18B20错误
unsigned long time2speed;
extern volatile char DISP_Update;
extern volatile u32 FSM_VAL;
unsigned int timecount;
u8 spoken;
//控制变量
u8 STOP_HEATER=0,STOP_MOTOR=0;


//部分函数声明
void writeMotors(void);
unsigned char PID_proc(void);



//霍尔中断测速
void doWhenTrigger( struct EXTILineType *triggeredLine)
{
	speed=(1000.0/(millis()-time2speed));
	time2speed=millis();
	
}
void Thread(void)
{
/********1Tick*********/
	NewKeyScan_Proc();
/********2Tick*********/
	if(!(timecount%2))//2,4,6,8...
	{
		
	}
/********3Tick*********/
	if(!(timecount%3))//3,6,9,12...
	{
		
	}

/********4Tick*********/

/********20Tick*********/
	
	if(!(timecount%10))
	{
		UART1_Send_Double(PID_input);//发送温度数据给实时波形显示控制端
	}

/********6Tick*********/

/********7Tick*********/

/*******CountFlow******/
	if(timecount == 0Xffffffff)
	{
		timecount=0;
	}else
	{
		timecount++;
	}
	
	

}
void KeyTriggered(unsigned char c)
{
	FSM_Proc(c);
}
//初始化
void All_Init()
{
//延时
	delay_init();
//millis
	MILLIS_Init();
//串口
	uart_init(9600);//,&USARTBufReceiever);//串口1
	uart2_init(9600);
	PT_EnableIRQ();//开蓝牙中断
//12864
	GUI_Init();
//LED
	LED_Init();
//按键
	NewKeyScan_Init();
	NewKeyTriggered=KeyTriggered;
//红外键盘接收
	IRrecv_Init();
	IRrecv_DoWhenDone=IRrecv_Handler;
//实时波形通信
	doEvery(DoEvery_Ms,100,Thread);
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
//24C02
	AT24CXX_Init();
	while(AT24CXX_Check())
	{
			printf("24C02 ERROR!\n");
	}
	AT24CXX_Read(TARGET_STORGE_ADDR,(u8*)&PID_target,sizeof(PID_target));
	AT24CXX_Read(TARGET_MIN_STORGE_ADDR,(u8*)&PID_alarm_MIN,sizeof(PID_alarm_MIN));
	AT24CXX_Read(TARGET_MAX_STORGE_ADDR,(u8*)&PID_alarm_MAX,sizeof(PID_alarm_MAX));
	RESTRAINTARGET(PID_target)
	RESTRAINALARM_MAX(PID_alarm_MAX)
	RESTRAINALARM_MIN(PID_alarm_MIN)
//DS18B20
	while(DS18B20_Init())//：返回0表示存在
	{
			printf("DS18B20 ERROR!\n");
	}
//初始菜单显示
	dispIDLE();
}

//输出函数
void writeMotors()
{
	double realOutput=PID_output/OUTPUT_REAL_MAX;//归一化
	if(realOutput>0&&!STOP_HEATER)
	{
		realOutput=realOutput*0.8;
		PWMManager.changeRatio(HEATER_CHANNEL,realOutput);
		PWMManager.changeRatio(MOTOR_CHANNEL,0);
	}
	else if(!STOP_MOTOR)
	{
		//风扇输出*HEATER_FACTOR
		//realOutput=PID_output*HEATER_FACTOR<1?PID_output*HEATER_FACTOR:1;
		realOutput=-realOutput;
		realOutput=realOutput*0.8+0.2;
		PWMManager.changeRatio(HEATER_CHANNEL,0);
		PWMManager.changeRatio(MOTOR_CHANNEL,realOutput);
	}
		
}
unsigned char PID_proc()
{
	//用于运行时响应参数的改变
	PID.SetPID(KP,KI,KD);
	if(PID_input-PID_target>10||PID_input-PID_target<-10)
	{
		PID.SetMode(MANUAL);
		PID_output=PID_target-PID_input>0?100:-100;
		writeMotors();
	}else
	{
		PID.SetMode(AUTOMATIC);
	}
	//PID操作  返回1表示有操作，返回0表示采样时间未到未操作
	return PID.Compute();
	//...
}

void update_Temprature()
{
	PID_input=DS18B20_Get_Temp();//读温度
	if((PID_input-PID_lastinput>10||PID_input-PID_lastinput<-10)&&(PID_lastinput<85&&PID_lastinput>5))//改善DS12B20读数错误
		PID_input=PID_lastinput;
	PID_lastinput=PID_input;
	
	if(PID_input>70)//温度大于70度时，停止加热，题目要求
		STOP_HEATER=1;
	else
		STOP_HEATER=0;
	if(PID_input<30)//温度小于30度时，停止风扇
		STOP_MOTOR=1;
	else
		STOP_MOTOR=0;
	
	if(PID_input>PID_alarm_MAX)
	{
		Alarm_LED_MAX();
		if(!spoken)
		{
			spoken=1;
			//...
		}
	}else if(PID_input<PID_alarm_MIN)
	{
		Alarm_LED_MIN();
		if(!spoken)
		{
			spoken=1;
			//...
		}
	}else
	{
		Alarm_Release();
	}

}
//蓝牙通信
void BT_Talk(void)
{
		BT_SendTMP(PID_input);//发送温度
		BT_SendSPD(speed);//发送速度
	  BT_SendPOW(PID_output>0?PWM2POWER(PID_output):0);//发送功率
}
unsigned long timeKeyScan;
unsigned long regularIDLE;
//运行一些需要定时/延时的操作
void delayFuncs(void)
{
	if(millis()-time2speed>SPEED_STOP_WAIT_TIME)//风扇中断超时，把风扇的速度置为0
	{
		speed=0;
	}
	if(millis()-timeKeyScan>1000)
	{
		timeKeyScan=millis();
		if(FSM_VAL==DISP_IDLE)
		{
			OLED_printfAt(INPUT_POSI,1,"Input:%.2f           ",PID_input);
			disp_update_Heater(); 
			disp_update_Speed(speed);
			GUI_FillRect(0,41,PID_input/100*128,43);
			GUI_SetDrawMode(GUI_DRAWMODE_REV);
			GUI_FillRect(PID_input/100*128,41,127,43);
			GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
		}
		return ;
	}
	if(millis()-regularIDLE>4000)
	{
		regularIDLE=millis();
		OLED_printfAt(INPUT_POSI,1,"Input:%.2f           ",PID_input);
		disp_update_Heater(); 
		disp_update_Speed(speed);
		GUI_FillRect(0,41,PID_input/100*128,43);
		GUI_SetDrawMode(GUI_DRAWMODE_REV);
		GUI_FillRect(PID_input/100*128,41,127,43);
		GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
	}
}

int main()
{
	All_Init();
	while(1)
	{
		update_Temprature();//更新温度0
		if(PID_proc())//到达采样时间
		{
			writeMotors();//PID输出
			BT_Talk();//蓝牙通信
			delayFuncs();//测速置零
		}
		
		Display_Proc();//菜单显示
	}
}
