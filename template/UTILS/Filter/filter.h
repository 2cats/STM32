#ifndef _FILTER_H_
#define _FILTER_H_


//一阶互补
typedef struct {
	unsigned char type;
	double K;// // 对加速度计取值的权重
	double lastfilted;
	double (*getAngle)(double angle_acc, double angle_gyro_speed, double timedelta);//angle_gyro为角速度
}ComplementaryFilterTypeDef;

void  ComplementaryFilter_Init(ComplementaryFilterTypeDef *filter,unsigned char _type,double _K);

double Complementary1_getAngle(double angle_acc, double angle_gyro_speed, double timedelta);//angle_gyro为角速度
//二阶互补
double Complementary2_getAngle( double angle_acc, double angle_gyro_speed, double timedelta);//angle_gyro为角速度

typedef struct {
    /* Kalman filter variables */
    double Q_angle; // Process noise variance for the accelerometer
    double Q_bias; // Process noise variance for the gyro bias
    double R_measure; // Measurement noise variance - this is actually the variance of the measurement noise

    double angle; // The angle calculated by the Kalman filter - part of the 2x1 state vector
    double bias; // The gyro bias calculated by the Kalman filter - part of the 2x1 state vector
    double rate; // Unbiased rate calculated from the rate and the calculated bias - you have to call getAngle to update the rate

    double P[2][2]; // Error covariance matrix - This is a 2x2 matrix
    double K[2]; // Kalman gain - This is a 2x1 vector
    double y; // Angle difference
    double S; // Estimate error
//Functions:
double (*getAngle)(double newAngle, double newRate, double dt);//newRate角速度
void (*setStartAngle)(double newAngle);// Used to set angle, this should be set as the starting angle

}KalmanFilterTypeDef;


void Kalman_Init(KalmanFilterTypeDef *filter);
    // The angle should be in degrees and the rate should be in degrees per second and the delta time in seconds
double Kalman_getAngle(double newAngle, double newRate, double dt);
void Kalman_setStartAngle(double newAngle) ;// set angle, this should be set as the starting angle
double Kalman_getRate(void);// Return the unbiased rate

/* These are used to tune the Kalman filter */
void Kalman_setQangle(double newQ_angle) ;
void Kalman_setQbias(double newQ_bias);
void Kalman_setRmeasure(double newR_measure) ;

double Kalman_getQangle(void);
double Kalman_getQbias(void) ;
double Kalman_getRmeasure(void) ;

#endif
