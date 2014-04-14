#ifndef NEWPID_v1_h
#define NEWPID_v1_h
#include "millis.h"

#define LIBRARY_VERSION	1.0.1
#define AUTOMATIC	1
#define MANUAL	0
#define DIRECT  0
#define REVERSE  1

typedef struct PIDType{

	double kp;                  // * (P)roportional Tuning Parameter
	double ki;                  // * (I)ntegral Tuning Parameter
	double kd;                  // * (D)erivative Tuning Parameter\
	

	int direction;

	double *input;              // * Pointers to the Input, Output, and target variables
	double *output;             //   This creates a hard link between the variables and the 
	double *target;           //   PID, freeing the user from having to constantly tell us
															//   what these values are.  with pointers we'll just know.
	unsigned long sampleTime;
	double outMin, outMax;
	unsigned char mode;
	
	
	unsigned char (*Compute)(void);
	void (*SetIOT)(double* Input, double* Output, double* target);
	void (*SetPID)(double Kp, double Ki, double Kd);
	
	void (*SetSampleTime)(int NewSampleTime);
	void (*SetOutputLimits)(double Min, double Max);
	void (*SetMode)(int Mode);
	void (*SetDirection)(int Direction);
}PIDTypeDef;

void PID_SetPID(double Kp, double Ki, double Kd);
void PID_Structure_Init(double Kp, double Ki, double Kd,unsigned long sampleTime,PIDTypeDef*PID);
void PID_setIOT(double* Input, double* Output, double* target);
unsigned char PID_Compute(void);
void PID_SetSampleTime(int NewSampleTime);
void PID_SetOutputLimits(double Min, double Max);
void PID_SetMode(int Mode);
void PID_SetDirection(int Direction);
#endif

