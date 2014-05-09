#include "filter.h"

/*
//Interface:
**********互补滤波**********
ComplementaryFilterTypeDef：滤波管理对象

type:
	1:一阶互补
	2:二阶互补
K:
	对加速度计取值的权重
lastfilted:
	上一次的滤波值
getAngle(double angle_acc, double angle_gyro_speed, double timedelta)
	angle_acc：由加速度计计算的角度值，单位rad
	angle_gyro_speed：陀螺仪输出值，单位rad/s,方向正负与angle_acc相同
	timedelta：读取sensor的间隔，单位s

Example：
	ComplementaryFilterTypeDef ComplementaryFilter
	ComplementaryFilter_Init(&ComplementaryFilter,1,0.9);
	//...readSenor...calc timedelta...
	ComplementaryFilter.getAngle( angle_acc,  angle_gyro_speed,  timedelta);

**********卡尔曼滤波**********
	KalmanFilterTypeDef
	void Kalman_Init(KalmanFilterTypeDef *filter);//滤波参数采用默认，如需修改，下有函数

Example：
	KalmanFilterTypeDef KalmanFilter；
	Kalman_Init(&KalmanFilter);
	//...readSenor...calc timedelta...
	KalmanFilter.setStartAngle(StartAngle);
	//...readSenor...calc timedelta...
	KalmanFilter.getAngle( newAngle,  newRate, dt);
*/
KalmanFilterTypeDef *DefaultKalmanFilter;
ComplementaryFilterTypeDef *DefaultComplementaryFilter;
void  ComplementaryFilter_Init(ComplementaryFilterTypeDef *filter,unsigned char _type,double _K)
{
	DefaultComplementaryFilter=filter;
	DefaultComplementaryFilter->type=_type;
	DefaultComplementaryFilter->K=_K;
	DefaultComplementaryFilter->lastfilted=0;
	if(_type==1)
		DefaultComplementaryFilter->getAngle=Complementary1_getAngle;
	else
		DefaultComplementaryFilter->getAngle=Complementary2_getAngle;
}
double Complementary1_getAngle(double angle_acc, double angle_gyro_speed, double timedelta)//angle_gyro_speed角速度,dt为间隔（秒）
{
    float A = DefaultComplementaryFilter->K / (DefaultComplementaryFilter->K  + timedelta);
    DefaultComplementaryFilter->lastfilted = A * (DefaultComplementaryFilter->lastfilted  + angle_gyro_speed * timedelta) + (1-A) * angle_acc;
	  return  DefaultComplementaryFilter->lastfilted ;
}
double Complementary2_getAngle(double angle_acc, double angle_gyro_speed, double timedelta)//angle_gyro_speed角速度
{
		double  y1=0,x2=0,x1=0;
    x1 = (angle_acc - DefaultComplementaryFilter->lastfilted ) * DefaultComplementaryFilter->K * DefaultComplementaryFilter->K;
    y1 = y1 + x1 * timedelta;
    x2 = y1 + 2 * DefaultComplementaryFilter->K *(angle_acc - DefaultComplementaryFilter->lastfilted ) + angle_gyro_speed;
    DefaultComplementaryFilter->lastfilted  = DefaultComplementaryFilter->lastfilted  + x2 * timedelta;
	  return DefaultComplementaryFilter->lastfilted ;
}


void Kalman_Init(KalmanFilterTypeDef *filter)  {
        /* We will set the variables like so, these can also be tuned by the user */
	      DefaultKalmanFilter=filter;
				filter->getAngle=Kalman_getAngle;
				filter->setStartAngle=Kalman_setStartAngle;
        DefaultKalmanFilter->Q_angle = 0.001;
        DefaultKalmanFilter->Q_bias = 0.003;
        DefaultKalmanFilter->R_measure = 0.03;

        DefaultKalmanFilter->angle = 0; // Reset the angle
        DefaultKalmanFilter->bias = 0; // Reset bias

        DefaultKalmanFilter->P[0][0] = 0; // Since we assume that the bias is 0 and we know the starting angle (use setAngle), the error covariance matrix is set like so - see: http://en.wikipedia.org/wiki/Kalman_filter#Example_application.2C_technical
        DefaultKalmanFilter->P[0][1] = 0;
        DefaultKalmanFilter->P[1][0] = 0;
        DefaultKalmanFilter->P[1][1] = 0;
    };
    // The angle should be in degrees and the rate should be in degrees per second and the delta time in seconds
double Kalman_getAngle(double newAngle, double newRate, double dt)
	{
        // KasBot V2  -  Kalman filter module - http://www.x-firm.com/?page_id=145
        // Modified by Kristian Lauszus
        // See my blog post for more information: http://blog.tkjelectronics.dk/2012/09/a-practical-approach-to-kalman-filter-and-how-to-implement-it

        // Discrete Kalman filter time update equations - Time Update ("Predict")
        // Update xhat - Project the state ahead
        /* Step 1 */
        DefaultKalmanFilter->rate = newRate - DefaultKalmanFilter->bias;
        DefaultKalmanFilter->angle += dt * DefaultKalmanFilter->rate;

        // Update estimation error covariance - Project the error covariance ahead
        /* Step 2 */
        DefaultKalmanFilter->P[0][0] += dt * (dt*DefaultKalmanFilter->P[1][1] - DefaultKalmanFilter->P[0][1] - DefaultKalmanFilter->P[1][0] + DefaultKalmanFilter->Q_angle);
        DefaultKalmanFilter->P[0][1] -= dt * DefaultKalmanFilter->P[1][1];
        DefaultKalmanFilter->P[1][0] -= dt * DefaultKalmanFilter->P[1][1];
        DefaultKalmanFilter->P[1][1] += DefaultKalmanFilter->Q_bias * dt;

        // Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
        // Calculate Kalman gain - Compute the Kalman gain
        /* Step 4 */
        DefaultKalmanFilter->S = DefaultKalmanFilter->P[0][0] + DefaultKalmanFilter->R_measure;
        /* Step 5 */
        DefaultKalmanFilter->K[0] = DefaultKalmanFilter->P[0][0] / DefaultKalmanFilter->S;
        DefaultKalmanFilter->K[1] = DefaultKalmanFilter->P[1][0] / DefaultKalmanFilter->S;

        // Calculate angle and bias - Update estimate with measurement zk (newAngle)
        /* Step 3 */
        DefaultKalmanFilter->y = newAngle - DefaultKalmanFilter->angle;
        /* Step 6 */
        DefaultKalmanFilter->angle += DefaultKalmanFilter->K[0] * DefaultKalmanFilter->y;
        DefaultKalmanFilter->bias += DefaultKalmanFilter->K[1] * DefaultKalmanFilter->y;

        // Calculate estimation error covariance - Update the error covariance
        /* Step 7 */
        DefaultKalmanFilter->P[0][0] -= DefaultKalmanFilter->K[0] * DefaultKalmanFilter->P[0][0];
        DefaultKalmanFilter->P[0][1] -= DefaultKalmanFilter->K[0] * DefaultKalmanFilter->P[0][1];
        DefaultKalmanFilter->P[1][0] -= DefaultKalmanFilter->K[1] * DefaultKalmanFilter->P[0][0];
        DefaultKalmanFilter->P[1][1] -= DefaultKalmanFilter->K[1] * DefaultKalmanFilter->P[0][1];

        return DefaultKalmanFilter->angle;
};
void Kalman_setStartAngle(double newAngle) { DefaultKalmanFilter->angle = newAngle; }; // Used to set angle, this should be set as the starting angle
double Kalman_getRate() { return DefaultKalmanFilter->rate; }; // Return the unbiased rate

/* These are used to tune the Kalman filter */
void Kalman_setQangle(double newQ_angle) { DefaultKalmanFilter->Q_angle = newQ_angle; };
void Kalman_setQbias(double newQ_bias) { DefaultKalmanFilter->Q_bias = newQ_bias; };
void Kalman_setRmeasure(double newR_measure) { DefaultKalmanFilter->R_measure = newR_measure; };

double Kalman_getQangle() { return DefaultKalmanFilter->Q_angle; };
double Kalman_getQbias() { return DefaultKalmanFilter->Q_bias; };
double Kalman_getRmeasure() { return DefaultKalmanFilter->R_measure; };
