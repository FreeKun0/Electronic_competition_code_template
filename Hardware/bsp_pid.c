#include "bsp_pid.h"
#include "control.h"

//����ȫ�ֱ���
_pid pid_YAW;

_pid pid_speed1, pid_speed2;
_pid pid_location1, pid_location2;

_pid pid_x, pid_y;
_pid pid2_x, pid2_y;

/**
  * @brief  YAW�㷨ʵ��
  * @param  actual_val:ʵ��ֵ
  *	@note 	��
  * @retval ͨ��PID���������
  */
float YAW_pid_realize(_pid *pid, float actual_val)
{
	/* ����ʵ��ֵ */
	pid->actual_val = actual_val;
	
	/*�������*/	//�˴�target_val �� set_pid_target ����
    pid->err = pid->target_val - actual_val;
  
    /* �趨�ջ����� */
    if( (pid->err >= - pid->err_dead) && (pid->err <= pid->err_dead) ) 
    {
		pid->err = 0;
		pid->integral = 0;
    }
	
	/*������*/
	if(fabs(pid->Ki) >= 1e-6)
	{
		pid->integral += pid->err;
	}
	
	/*�����޷�*/
	if 		(pid->integral >= pid->integral_limit)	{pid->integral =  pid->integral_limit;}
	else if (pid->integral < -pid->integral_limit)	{pid->integral = -pid->integral_limit;}

	/*PID�㷨ʵ��*/
    pid->out_val = 	pid->Kp * pid->err + 
					pid->Ki * pid->integral + 
					pid->Kd * (pid->err - pid->err_last);
  
	/*����*/
    pid->err_last = pid->err;
    
	/*���ص�ǰʵ��ֵ*/
    return pid->out_val;
}

void yaw_Turn_pid_init(void)
{
	//���ǶȻ�
//	pid_YAW.Kp = 1.0;
//	pid_YAW.Ki = 0.8;
//	pid_YAW.Kd = 0.0;
//	pid_YAW.err_dead = 1.5;
//	pid_YAW.integral_limit = 300;
//	pid_YAW.out_limit = 400;
	
	//�Ƕȴ��ٶ�
	//��
//	pid_YAW.Kp = 1.5;
//	pid_YAW.Ki = 0.05;
//	pid_YAW.Kd = 10.0;
//	pid_YAW.err_dead = 1.0;
//	pid_YAW.integral_limit = 100;
//	pid_YAW.out_limit = 60;
	//��
	pid_YAW.Kp = 1.5;
	pid_YAW.Ki = 0.05;
	pid_YAW.Kd = 15.0;
	pid_YAW.err_dead = 1.0;
	pid_YAW.integral_limit = 300;
	pid_YAW.out_limit = 80;
}
void yaw_CarGo_pid_init(void)
{

	pid_YAW.Kp = 20.0;
	pid_YAW.Ki = 1.0;
	pid_YAW.Kd = 0.0;
	pid_YAW.err_dead = 0.0;
	pid_YAW.integral_limit = 300;
	pid_YAW.out_limit = 500;
}

/**
  * @brief  PID������ʼ��
  *	@note 	��
  * @retval ��
  */
void pid_param_init()
{
	/*--------------------------------YAW--------------------------------*/
	/*****************PID����*********************/
//	yaw_Turn_pid_init();
	yaw_CarGo_pid_init();
	
	//ֱ�߽ǶȻ�

	/*****************��ʼ����*********************/
	pid_YAW.target_val = 0.0;				
	pid_YAW.actual_val = 0.0;
	pid_YAW.out_val = 0.0;
	pid_YAW.err = 0.0;
	pid_YAW.err_last = 0.0;
	pid_YAW.integral = 0.0;

	/*--------------------------------С��--------------------------------*/
	/*****************PID����*********************/
	/* ���1λ�� */
	pid_location1.Kp = 0.25;
	pid_location1.Ki = 0.0;	//λ�û�û����
	pid_location1.Kd = 0.0;
	pid_location1.err_dead = 0.5;
	pid_location1.integral_limit = 0;	//λ�û�û�����޷�
	pid_location1.out_limit = TARGET_SPEED_RPM_LIMIT;	//λ�û����Ŀ��RPM���ٶȻ������150��RPM_max

	/* ���1�ٶ� */
	pid_speed1.Kp = 5.0;
	pid_speed1.Ki = 0.4;
	pid_speed1.Kd = 0.0;
	pid_speed1.err_dead = 1.0;
	pid_speed1.integral_limit = 1700;
	pid_speed1.out_limit = 0;	//�ٶȻ����PWM�����ﲻ�޷� ֱ���ڵ��MotorOutput�޷������PWM=900,�޷���900-50

	/* ���2λ�� */
	pid_location2.Kp = 0.25;
	pid_location2.Ki = 0.0;
	pid_location2.Kd = 0.0;
	pid_location2.err_dead = 0.5;
	pid_location2.integral_limit = 0;
	pid_location2.out_limit = TARGET_SPEED_RPM_LIMIT;

	/* ���2�ٶ� */
	pid_speed2.Kp = 5.0;
	pid_speed2.Ki = 0.4;
	pid_speed2.Kd = 0.0;
	pid_speed2.err_dead = 1.0;
	pid_speed2.integral_limit = 1700;
	pid_speed2.out_limit = 0;
	
	/*****************��ʼ����*********************/
	/* ���1λ�� */
	pid_location1.target_val = 0.0;				
	pid_location1.actual_val = 0.0;
	pid_location1.out_val = 0.0;
	pid_location1.err = 0.0;
	pid_location1.err_last = 0.0;
	pid_location1.integral = 0.0;
	/* ���1�ٶ� */
	pid_speed1.target_val = 0.0;				
	pid_speed1.actual_val = 0.0;
	pid_speed1.out_val = 0.0;
	pid_speed1.err = 0.0;
	pid_speed1.err_last = 0.0;
	pid_speed1.integral = 0.0;
	/* ���2λ�� */
	pid_location2.target_val = 0.0;				
	pid_location2.actual_val = 0.0;
	pid_location2.out_val = 0.0;
	pid_location2.err = 0.0;
	pid_location2.err_last = 0.0;
	pid_location2.integral = 0.0;
	/* ���2�ٶ� */
	pid_speed2.target_val = 0.0;				
	pid_speed2.actual_val = 0.0;
	pid_speed2.out_val = 0.0;
	pid_speed2.err = 0.0;
	pid_speed2.err_last = 0.0;
	pid_speed2.integral = 0.0;
	
	
	/*--------------------------------���--------------------------------*/
	/*****************PID����*********************/
	
	//�켤��
	/* pid_x */
	pid_x.Kp = -0.5;
	pid_x.Ki = -0.0;
	pid_x.Kd = -0.1;
	pid_x.err_dead = 1;	//��������ƫ��
	pid_x.integral_limit = 0;
	pid_x.out_limit = 1;
	/* pid_y */
	pid_y.Kp = 0.5;
	pid_y.Ki = 0.0;
	pid_y.Kd = 0.1;
	pid_y.err_dead = 1;
	pid_y.integral_limit = 0;
	pid_y.out_limit = 1;
	
	//�̼���
	/* pid2_x */
	pid2_x.Kp = -0.5;
	pid2_x.Ki = -0.01;
	pid2_x.Kd = -0.1;
	pid2_x.err_dead = 1;
	pid2_x.integral_limit = 10;
	pid2_x.out_limit = 1;
	/* pid2_y */
	pid2_y.Kp = 0.5;
	pid2_y.Ki = 0.01;
	pid2_y.Kd = 0.1;
	pid2_y.err_dead = 1;
	pid2_y.integral_limit = 10;
	pid2_y.out_limit = 1;
	
	/*****************��ʼ����*********************/
	/* pid_x */
	pid_x.target_val = 0.0;				
	pid_x.actual_val = 0.0;
	pid_x.out_val = 0.0;
	pid_x.err = 0.0;
	pid_x.err_last = 0.0;
	pid_x.integral = 0.0;
	/* pid_y */
	pid_y.target_val = 0.0;				
	pid_y.actual_val = 0.0;
	pid_y.out_val = 0.0;
	pid_y.err = 0.0;
	pid_y.err_last = 0.0;
	pid_y.integral = 0.0;
	
	/* pid2_x */
	pid2_x.target_val = 0.0;				
	pid2_x.actual_val = 0.0;
	pid2_x.out_val = 0.0;
	pid2_x.err = 0.0;
	pid2_x.err_last = 0.0;
	pid2_x.integral = 0.0;
	/* pid2_y */
	pid2_y.target_val = 0.0;				
	pid2_y.actual_val = 0.0;
	pid2_y.out_val = 0.0;
	pid2_y.err = 0.0;
	pid2_y.err_last = 0.0;
	pid2_y.integral = 0.0;
}

void set_p_i_d(_pid *pid, float Kp, float Ki, float Kd)
{
  	pid->Kp = Kp;    // ���ñ���ϵ�� P
	pid->Ki = Ki;    // ���û���ϵ�� I
	pid->Kd = Kd;    // ����΢��ϵ�� D
}

void set_pid_target(_pid *pid, float temp_val)
{
  pid->target_val = temp_val;    // ���õ�ǰ��Ŀ��ֵ
}

float get_pid_target(_pid *pid)
{
  return pid->target_val;    // ��ȡ��ǰ��Ŀ��ֵ
}

float get_pid_actual(_pid *pid)
{
  return pid->actual_val;    // ��ȡ��ǰ��ʵ��ֵ
}

/**
  * @brief  pid_x�㷨ʵ��
  * @param  actual_val:ʵ��ֵ
  *	@note 	��
  * @retval ͨ��PID���������
  */
float pid_x_realize(_pid *pid, float actual_val)
{
	/* ����ʵ��ֵ */
	pid->actual_val = actual_val;
	
	/*�������*/	//�˴�target_val �� set_pid_target ����
    pid->err = pid->target_val - actual_val;
  
    /* �趨�ջ����� */
    if( (pid->err >= - pid->err_dead) && (pid->err <= pid->err_dead) ) 
    {
		pid->err = 0;
    }
	
	/*������*/
	if(fabs(pid->Ki) >= 1e-6)
	{
		pid->integral += pid->err;
	}

	/*�����޷�*/
	if 		(pid->integral >= pid->integral_limit)	{pid->integral =  pid->integral_limit;}
	else if (pid->integral < -pid->integral_limit)	{pid->integral = -pid->integral_limit;}

	/*PID�㷨ʵ��*/
    pid->out_val = 	pid->Kp * pid->err + 
					pid->Ki * pid->integral + 
					pid->Kd * (pid->err - pid->err_last);
  
	/*����*/
    pid->err_last = pid->err;
    
	/*���ص�ǰʵ��ֵ*/
    return pid->out_val;
}

/**
  * @brief  pid_y�㷨ʵ��
  * @param  actual_val:ʵ��ֵ
  *	@note 	��
  * @retval ͨ��PID���������
  */
float pid_y_realize(_pid *pid, float actual_val)
{
	/* ����ʵ��ֵ */
	pid->actual_val = actual_val;
	
	/*�������*/	//�˴�target_val �� set_pid_target ����
    pid->err = pid->target_val - actual_val;
  
    /* �趨�ջ����� */
    if( (pid->err >= - pid->err_dead) && (pid->err <= pid->err_dead) ) 
    {
		pid->err = 0;
    }
	
	/*������*/
	if(fabs(pid->Ki) >= 1e-6)
	{
		pid->integral += pid->err;
	}
	
	/*�����޷�*/
	if 		(pid->integral >= pid->integral_limit)	{pid->integral =  pid->integral_limit;}
	else if (pid->integral < -pid->integral_limit)	{pid->integral = -pid->integral_limit;}

	/*PID�㷨ʵ��*/
    pid->out_val = 	pid->Kp * pid->err + 
					pid->Ki * pid->integral + 
					pid->Kd * (pid->err - pid->err_last);
  
	/*����*/
    pid->err_last = pid->err;
    
	/*���ص�ǰʵ��ֵ*/
    return pid->out_val;
}

/**
  * @brief  pid_x�㷨ʵ��
  * @param  actual_val:ʵ��ֵ
  *	@note 	��
  * @retval ͨ��PID���������
  */
float pid2_x_realize(_pid *pid, float actual_val)
{
	/* ����ʵ��ֵ */
	pid->actual_val = actual_val;
	
	/*�������*/	//�˴�target_val �� set_pid_target ����
    pid->err = pid->target_val - actual_val;
  
    /* �趨�ջ����� */
    if( (pid->err >= - pid->err_dead) && (pid->err <= pid->err_dead) ) 
    {
		pid->err = 0;
    }
	
	/*������*/
	if(fabs(pid->Ki) >= 1e-6)
	{
		pid->integral += pid->err;
	}

	/*�����޷�*/
	if 		(pid->integral >= pid->integral_limit)	{pid->integral =  pid->integral_limit;}
	else if (pid->integral < -pid->integral_limit)	{pid->integral = -pid->integral_limit;}

	/*PID�㷨ʵ��*/
    pid->out_val = 	pid->Kp * pid->err + 
					pid->Ki * pid->integral + 
					pid->Kd * (pid->err - pid->err_last);
  
	/*����*/
    pid->err_last = pid->err;
    
	/*���ص�ǰʵ��ֵ*/
    return pid->out_val;
}

/**
  * @brief  pid_y�㷨ʵ��
  * @param  actual_val:ʵ��ֵ
  *	@note 	��
  * @retval ͨ��PID���������
  */
float pid2_y_realize(_pid *pid, float actual_val)
{
	/* ����ʵ��ֵ */
	pid->actual_val = actual_val;
	
	/*�������*/	//�˴�target_val �� set_pid_target ����
    pid->err = pid->target_val - actual_val;
  
    /* �趨�ջ����� */
    if( (pid->err >= - pid->err_dead) && (pid->err <= pid->err_dead) ) 
    {
		pid->err = 0;
    }
	
	/*������*/
	if(fabs(pid->Ki) >= 1e-6)
	{
		pid->integral += pid->err;
	}
	
	/*�����޷�*/
	if 		(pid->integral >= pid->integral_limit)	{pid->integral =  pid->integral_limit;}
	else if (pid->integral < -pid->integral_limit)	{pid->integral = -pid->integral_limit;}

	/*PID�㷨ʵ��*/
    pid->out_val = 	pid->Kp * pid->err + 
					pid->Ki * pid->integral + 
					pid->Kd * (pid->err - pid->err_last);
  
	/*����*/
    pid->err_last = pid->err;
    
	/*���ص�ǰʵ��ֵ*/
    return pid->out_val;
}

/**
  * @brief  λ��PID�㷨ʵ��
  * @param  actual_val:ʵ��ֵ
  *	@note 	��
  * @retval ͨ��PID���������
  */
float location_pid_realize(_pid *pid, float actual_val)  //λ�û����Kp����Ҳ����
{
	/* ����ʵ��ֵ */
	pid->actual_val = actual_val;
	
	/*�������*/	//�˴�target_val �� set_pid_target ����
    pid->err = pid->target_val - actual_val;
  
    /* �趨�ջ����� */
    if( (pid->err >= - pid->err_dead) && (pid->err <= pid->err_dead) ) 
    {
		pid->err = 0;
		pid->integral = 0;	//λ�õ��˾���ջ��֣�
    }
	
	/*������*/
	if(fabs(pid->Ki) >= 1e-6)
	{
		pid->integral += pid->err;
	}
	
	/*�����޷�*/	//λ�û�����Ҫ�����޷�
//	if 		(pid->integral >= pid->integral_limit)	{pid->integral =  pid->integral_limit;}
//	else if (pid->integral < -pid->integral_limit)	{pid->integral = -pid->integral_limit;}

	/*PID�㷨ʵ��*/
    pid->out_val = 	pid->Kp * pid->err + 
					pid->Ki * pid->integral + 
					pid->Kd * (pid->err - pid->err_last);
  
	/*����*/
    pid->err_last = pid->err;
    
	/*���ص�ǰʵ��ֵ����ֵ��Ϊ�ٶ�pid������ ���ٶ�pid��Ŀ���ٶ�rpm*/
    return pid->out_val;
}

/**
  * @brief  �ٶ�PID�㷨ʵ��
  * @param  actual_val:ʵ��ֵ
  *	@note 	��
  * @retval ͨ��PID���������
  */
float speed_pid_realize(_pid *pid, float actual_val)
{
	/* ����ʵ��ֵ */
	pid->actual_val = actual_val;
	
	/* ������� */
	pid->err = pid->target_val - actual_val;
	
    /* �趨�ջ����� */
    if( (pid->err >= - pid->err_dead) && (pid->err <= pid->err_dead) ) 
    {
		pid->err = 0;
    }

	/*������*/
	if(fabs(pid->Ki) >= 1e-6)
	{
		pid->integral += pid->err;
	}
	
	/*�����޷�*/
	if 		(pid->integral >= pid->integral_limit)	{pid->integral =  pid->integral_limit;}
	else if (pid->integral < -pid->integral_limit)	{pid->integral = -pid->integral_limit;}

	/*PID�㷨ʵ��*/
	pid->out_val = 	pid->Kp * pid->err +
					pid->Ki * pid->integral +
					pid->Kd * (pid->err - pid->err_last);
	
	/*���ƫ��*/	
	//���Ϊ�˽��ֱ�ߺ���������С�൱��û�����������⣬���ǻ�Ӱ��ת���ʵ�֣���Ϊת��pwm������С������Ὣpwm�Ŵ󣬵���һֱת
//	if(pid->out_val > 0)		pid->out_val += 10;

//	else if(pid->out_val < 0)	pid->out_val -= 10;

//	else						pid->out_val = 0;

	/*����*/
	pid->err_last = pid->err;

	/*���ص�ǰʵ��ֵ*/
	return pid->out_val;
}
