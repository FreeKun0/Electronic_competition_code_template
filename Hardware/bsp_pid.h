#ifndef __BSP_PID_H
#define	__BSP_PID_H
#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "parameter.h"

#define SPEED_INTEGRAL_MAX	1700	//速度环积分限幅
#define X_INTEGRAL_MAX		5000 
#define Y_INTEGRAL_MAX		5000
#define X2_INTEGRAL_MAX		5000 
#define Y2_INTEGRAL_MAX		5000

/* PID结构体 */
typedef struct
{
	float Kp, Ki, Kd;          	//比例、积分、微分系数
    float target_val;           //目标值
    float actual_val;        	//实际值
	float out_val;				//输出值
	float out_limit;			//输出限幅
    float err;             		//偏差值
	float err_dead;				//误差死区
    float err_last;          	//上一个偏差值
	float integral;				//积分值
	float integral_limit;		//积分上限
}_pid;

extern _pid pid_YAW;

extern _pid pid_speed1, pid_speed2;    
extern _pid pid_location1, pid_location2;


void yaw_Turn_pid_init(void);
void yaw_CarGo_pid_init(void);
void pid_param_init(void);
void set_p_i_d(_pid *pid, float Kp, float Ki, float Kd);

void set_pid_target(_pid *pid, float temp_val);

float get_pid_target(_pid *pid);
float get_pid_actual(_pid *pid);


float location_pid_realize(_pid *pid, float actual_val);
float speed_pid_realize(_pid *pid, float actual_val);
float YAW_pid_realize(_pid *pid, float actual_val);




extern _pid pid_x, pid_y;

float pid_x_realize(_pid *pid, float actual_val);
float pid_y_realize(_pid *pid, float actual_val);

extern _pid pid2_x, pid2_y;

float pid2_x_realize(_pid *pid, float actual_val);
float pid2_y_realize(_pid *pid, float actual_val);

#endif
