#ifndef __BSP_PID_H
#define	__BSP_PID_H
#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "parameter.h"

#define SPEED_INTEGRAL_MAX	1700	//�ٶȻ������޷�
#define X_INTEGRAL_MAX		5000 
#define Y_INTEGRAL_MAX		5000
#define X2_INTEGRAL_MAX		5000 
#define Y2_INTEGRAL_MAX		5000

/* PID�ṹ�� */
typedef struct
{
	float Kp, Ki, Kd;          	//���������֡�΢��ϵ��
    float target_val;           //Ŀ��ֵ
    float actual_val;        	//ʵ��ֵ
	float out_val;				//���ֵ
	float out_limit;			//����޷�
    float err;             		//ƫ��ֵ
	float err_dead;				//�������
    float err_last;          	//��һ��ƫ��ֵ
	float integral;				//����ֵ
	float integral_limit;		//��������
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
