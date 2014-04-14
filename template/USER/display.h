#ifndef _DISPLAY_H
#define _DISPLAY_H
#define DATA_POSITION 75
extern unsigned char KeyOperateFlag;
extern unsigned char GUI_NEEDING_UPDATE_FLAG;
#define IS_TMP_CONFIGUREING (KeyOperateFlag&1)
#define SET_TMP_CONFIGUREING (KeyOperateFlag|=1)
#define UNSET_TMP_CONFIGUREING (KeyOperateFlag&=~1)

#define IS_MOTOR_CONFIGUREING (KeyOperateFlag&2)
#define SET_MOTOR_CONFIGUREING (KeyOperateFlag|=2)
#define UNSET_MOTOR_CONFIGUREING (KeyOperateFlag&=~2)

#define IS_PID_Target_NU (KeyOperateFlag&4)
#define SET_PID_Target_NU (KeyOperateFlag|=4)
#define UNSET_PID_Target_NU (KeyOperateFlag&=~4)

#define IS_TMP_KEY_NU_S (KeyOperateFlag&8)
#define SET_TMP_KEY_NU_S (KeyOperateFlag|=8)
#define UNSET_TMP_KEY_NU_S (KeyOperateFlag&=~8)

#define IS_TMP_KEY_NU_R (KeyOperateFlag&16)
#define SET_TMP_KEY_NU_R (KeyOperateFlag|=16)
#define UNSET_TMP_KEY_NU_R (KeyOperateFlag&=~16)

#define IS_MOTOR_KEY_NU_S (KeyOperateFlag&32)
#define SET_MOTOR_KEY_NU_S (KeyOperateFlag|=32)
#define UNSET_MOTOR_KEY_NU_S (KeyOperateFlag&=~32)

#define IS_MOTOR_KEY_NU_R (KeyOperateFlag&64)
#define SET_MOTOR_KEY_NU_R (KeyOperateFlag|=64)
#define UNSET_MOTOR_KEY_NU_R (KeyOperateFlag&=~64)

void initData_Display(void);
void update_Temprature_Display(double tempt);
void initKey_Display(void);
void setTarget_Display(int target);
void configTmpKey_Display(void);
void configMotorKey_Display(void);
#endif
