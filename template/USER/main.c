/*2014 �¿�Master*/
#include "include.h"
//�ṹ�����
USARTBufReceieverTypeDef USARTBufReceiever;
PWMManagerTypeDef PWMManager;
EXTIManagerTypeDef EXTIManager;
PIDTypeDef PID;

//PID����
double PID_target=70;
double PID_input=0;
double PID_output=0;
//PID����
double KP=6.6834,KI=0.08 ,KD=140,PIDSampleTime=50;
double heater_PWM=1;int OUTPUT_REAL_MAX=100;
double PID_alarm_MIN;
double PID_alarm_MAX;

//�ٶȱ���
double speed;

//��������
double PID_lastinput=0;//�¶Ȼ��棬����DS18B20����
unsigned long time2speed;
extern volatile char DISP_Update;
extern volatile u32 FSM_VAL;
unsigned int timecount;
u8 spoken;
//���Ʊ���
u8 STOP_HEATER=0,STOP_MOTOR=0;


//���ֺ�������
void writeMotors(void);
unsigned char PID_proc(void);



//�����жϲ���
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
		UART1_Send_Double(PID_input);//�����¶����ݸ�ʵʱ������ʾ���ƶ�
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
//��ʼ��
void All_Init()
{
//��ʱ
	delay_init();
//millis
	MILLIS_Init();
//����
	uart_init(9600);//,&USARTBufReceiever);//����1
	uart2_init(9600);
	PT_EnableIRQ();//�������ж�
//12864
	GUI_Init();
//LED
	LED_Init();
//����
	NewKeyScan_Init();
	NewKeyTriggered=KeyTriggered;
//������̽���
	IRrecv_Init();
	IRrecv_DoWhenDone=IRrecv_Handler;
//ʵʱ����ͨ��
	doEvery(DoEvery_Ms,100,Thread);
//�ⲿ�ж�_����
	EXTIManager_Structure(&EXTIManager);
	EXTIManager.startListenling(GPIOB,GPIO_Pin_0,EXTI_Falling,doWhenTrigger);
//PWM
	PWMManagerStructure(&PWMManager);
	PWMManager.ProduceNewPWM(MOTOR_CHANNEL,25000,0);
	PWMManager.ProduceNewPWM(HEATER_CHANNEL,25000,0);
//PID
	PID_Structure_Init(KP,KI,KD,PIDSampleTime,&PID);//10msPID��������
	PID.SetIOT(&PID_input,&PID_output,&PID_target);//IOT��ֵ�����ں����������޸�
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
	while(DS18B20_Init())//������0��ʾ����
	{
			printf("DS18B20 ERROR!\n");
	}
//��ʼ�˵���ʾ
	dispIDLE();
}

//�������
void writeMotors()
{
	double realOutput=PID_output/OUTPUT_REAL_MAX;//��һ��
	if(realOutput>0&&!STOP_HEATER)
	{
		realOutput=realOutput*0.8;
		PWMManager.changeRatio(HEATER_CHANNEL,realOutput);
		PWMManager.changeRatio(MOTOR_CHANNEL,0);
	}
	else if(!STOP_MOTOR)
	{
		//�������*HEATER_FACTOR
		//realOutput=PID_output*HEATER_FACTOR<1?PID_output*HEATER_FACTOR:1;
		realOutput=-realOutput;
		realOutput=realOutput*0.8+0.2;
		PWMManager.changeRatio(HEATER_CHANNEL,0);
		PWMManager.changeRatio(MOTOR_CHANNEL,realOutput);
	}
		
}
unsigned char PID_proc()
{
	//��������ʱ��Ӧ�����ĸı�
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
	//PID����  ����1��ʾ�в���������0��ʾ����ʱ��δ��δ����
	return PID.Compute();
	//...
}

void update_Temprature()
{
	PID_input=DS18B20_Get_Temp();//���¶�
	if((PID_input-PID_lastinput>10||PID_input-PID_lastinput<-10)&&(PID_lastinput<85&&PID_lastinput>5))//����DS12B20��������
		PID_input=PID_lastinput;
	PID_lastinput=PID_input;
	
	if(PID_input>70)//�¶ȴ���70��ʱ��ֹͣ���ȣ���ĿҪ��
		STOP_HEATER=1;
	else
		STOP_HEATER=0;
	if(PID_input<30)//�¶�С��30��ʱ��ֹͣ����
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
//����ͨ��
void BT_Talk(void)
{
		BT_SendTMP(PID_input);//�����¶�
		BT_SendSPD(speed);//�����ٶ�
	  BT_SendPOW(PID_output>0?PWM2POWER(PID_output):0);//���͹���
}
unsigned long timeKeyScan;
unsigned long regularIDLE;
//����һЩ��Ҫ��ʱ/��ʱ�Ĳ���
void delayFuncs(void)
{
	if(millis()-time2speed>SPEED_STOP_WAIT_TIME)//�����жϳ�ʱ���ѷ��ȵ��ٶ���Ϊ0
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
		update_Temprature();//�����¶�0
		if(PID_proc())//�������ʱ��
		{
			writeMotors();//PID���
			BT_Talk();//����ͨ��
			delayFuncs();//��������
		}
		
		Display_Proc();//�˵���ʾ
	}
}
