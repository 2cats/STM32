#include "newPID.h" 

/*********INTERFACE*********/
/*
//Example��
	MILLIS_Init();

	double PID_input,PID_output,PID_target;
	PIDTypeDef PID;
	PID_Structure_Init(1,1,1,10,&PID);//10msPID��������
	PID.SetIOT(&PID_input,&PID_output,&PID_target);//IOT��ֵ�����ں����������޸�
	PID.Compute();//PID����
*/
//variables
unsigned long _PID_lastTime;
double _PID_ITerm, _PID_lastInput;
double _PID_kp;                  // * (P)roportional Tuning Parameter
double _PID_ki;                  // * (I)ntegral Tuning Parameter
double _PID_kd;                  // * (D)erivative Tuning Parameter
PIDTypeDef *defaultPID;

//funcs
void PID_SetPID(double Kp, double Ki, double Kd)
{
	double SampleTimeInSec;
	if (Kp<0 || Ki<0 || Kd<0) return;
	defaultPID->kp=Kp;
	defaultPID->ki=Ki;
	defaultPID->kd=Kd;
	SampleTimeInSec= ((double)defaultPID->sampleTime)/1000;  
	_PID_kp = Kp;
	_PID_ki = Ki * SampleTimeInSec;
	_PID_kd = Kd / SampleTimeInSec;
 
  if(defaultPID->direction ==REVERSE)
   {
      _PID_kp = (0 - _PID_kp);
      _PID_ki = (0 - _PID_ki);
      _PID_kd = (0 - _PID_kd);
   }
}
void PID_Structure_Init(double Kp, double Ki, double Kd,unsigned long sampleTime,PIDTypeDef*PID)
{
	defaultPID=PID;
	defaultPID->sampleTime=sampleTime;
	PID_SetPID(Kp,  Ki,  Kd);

	defaultPID->Compute=PID_Compute;
	defaultPID->SetPID=PID_SetPID;
	defaultPID->SetDirection=PID_SetDirection;
	defaultPID->SetIOT=PID_setIOT;
	defaultPID->SetMode=PID_SetMode;
	defaultPID->SetOutputLimits=PID_SetOutputLimits;
	defaultPID->SetSampleTime=PID_SetSampleTime;

//Ĭ������
	defaultPID->direction=DIRECT;
	defaultPID->mode=AUTOMATIC;
	defaultPID->outMax=255;
	defaultPID->outMin=-255;
	
	_PID_lastTime = millis()-sampleTime;
}
void PID_setIOT(double* Input, double* Output, double* target)
{
	defaultPID->input=Input;
	defaultPID->output=Output;
	defaultPID->target=target;
}
unsigned char PID_Compute()
{
	unsigned long now,timeChange;
	if(!defaultPID->mode) return 0;//��ʱΪ�˹�ģʽ��ʲô��������ֱ�ӷ���0
	now = millis();
	timeChange = (now - _PID_lastTime);
	if(timeChange>=defaultPID->sampleTime)
	{
		double input,error,dInput,output;
		/*Compute all the working error variables*/
		 input = *defaultPID->input;
		 error = *defaultPID->target - input;
		_PID_ITerm+= (_PID_ki * error);
		if(_PID_ITerm > defaultPID->outMax) _PID_ITerm= defaultPID->outMax;
		else if(_PID_ITerm < defaultPID->outMin) _PID_ITerm= defaultPID->outMin;
	  dInput = (input - _PID_lastInput);

		/*Compute PID Output*/
	  output = _PID_kp * error + _PID_ITerm- _PID_kd * dInput;

		if(output > defaultPID->outMax) output = defaultPID->outMax;
		else if(output < defaultPID->outMin) output = defaultPID->outMin;
		*defaultPID->output = output;

		/*Remember some variables for next time*/
		_PID_lastInput = input;
		_PID_lastTime = now;
		return 1;
	}
	else return 0;//δ������ʱ�䣬ʲô��������ֱ�ӷ���0
}
void PID_SetSampleTime(int NewSampleTime)
{
   if (NewSampleTime > 0)
   {
      double ratio  = (double)NewSampleTime
                      / (double)defaultPID->sampleTime;
      _PID_ki *= ratio;
      _PID_kd /= ratio;
      defaultPID->sampleTime = (unsigned long)NewSampleTime;
   }
}
void PID_SetOutputLimits(double Min, double Max)
{
   if(Min >= Max) return;
   defaultPID->outMin = Min;
   defaultPID->outMax = Max;
 
   if(defaultPID->mode)
   {
	   if(*defaultPID->output > defaultPID->outMax) *defaultPID->output = defaultPID->outMax;
	   else if(*defaultPID->output < defaultPID->outMin) *defaultPID->output = defaultPID->outMin;
	 
	   if(_PID_ITerm > defaultPID->outMax) _PID_ITerm= defaultPID->outMax;
	   else if(_PID_ITerm < defaultPID->outMin) _PID_ITerm= defaultPID->outMin;
   }
}
void PID_prepareM2A()
{
   _PID_ITerm = *defaultPID->output;//�˴����������ڣ���̬ʱ�����  ����̬���������ֱ��_PID_ITerm=0
   _PID_lastInput = *defaultPID->input;
   if(_PID_ITerm > defaultPID->outMax) _PID_ITerm = defaultPID->outMax;
   else if(_PID_ITerm < defaultPID->outMin) _PID_ITerm = defaultPID->outMin;
}
void PID_SetMode(int Mode)
{
    unsigned char newAuto = (Mode == AUTOMATIC);
    if(newAuto == !defaultPID->mode)
    {  /*we just went from manual to auto*/
        PID_prepareM2A();
    }
    defaultPID->mode = newAuto;
}
void PID_SetDirection(int Direction)
{
   if( defaultPID->mode  && Direction != defaultPID->direction)
   {
			_PID_kp = (0 - _PID_kp);
      _PID_ki = (0 - _PID_ki);
      _PID_kd = (0 - _PID_kd);
   }   
    defaultPID->direction= Direction;
}

