#include "bsp_pid.h"
#include "control.h"

//定义全局变量
_pid pid_YAW;

_pid pid_speed1, pid_speed2;
_pid pid_location1, pid_location2;

_pid pid_x, pid_y;
_pid pid2_x, pid2_y;

/**
  * @brief  YAW算法实现
  * @param  actual_val:实际值
  *	@note 	无
  * @retval 通过PID计算后的输出
  */
float YAW_pid_realize(_pid *pid, float actual_val)
{
	/* 传入实际值 */
	pid->actual_val = actual_val;
	
	/*计算误差*/	//此处target_val 由 set_pid_target 设置
    pid->err = pid->target_val - actual_val;
  
    /* 设定闭环死区 */
    if( (pid->err >= - pid->err_dead) && (pid->err <= pid->err_dead) ) 
    {
		pid->err = 0;
		pid->integral = 0;
    }
	
	/*误差积累*/
	if(fabs(pid->Ki) >= 1e-6)
	{
		pid->integral += pid->err;
	}
	
	/*积分限幅*/
	if 		(pid->integral >= pid->integral_limit)	{pid->integral =  pid->integral_limit;}
	else if (pid->integral < -pid->integral_limit)	{pid->integral = -pid->integral_limit;}

	/*PID算法实现*/
    pid->out_val = 	pid->Kp * pid->err + 
					pid->Ki * pid->integral + 
					pid->Kd * (pid->err - pid->err_last);
  
	/*误差传递*/
    pid->err_last = pid->err;
    
	/*返回当前实际值*/
    return pid->out_val;
}

void yaw_Turn_pid_init(void)
{
	//单角度环
//	pid_YAW.Kp = 1.0;
//	pid_YAW.Ki = 0.8;
//	pid_YAW.Kd = 0.0;
//	pid_YAW.err_dead = 1.5;
//	pid_YAW.integral_limit = 300;
//	pid_YAW.out_limit = 400;
	
	//角度串速度
	//慢
//	pid_YAW.Kp = 1.5;
//	pid_YAW.Ki = 0.05;
//	pid_YAW.Kd = 10.0;
//	pid_YAW.err_dead = 1.0;
//	pid_YAW.integral_limit = 100;
//	pid_YAW.out_limit = 60;
	//快
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
  * @brief  PID参数初始化
  *	@note 	无
  * @retval 无
  */
void pid_param_init()
{
	/*--------------------------------YAW--------------------------------*/
	/*****************PID参数*********************/
//	yaw_Turn_pid_init();
	yaw_CarGo_pid_init();
	
	//直走角度环

	/*****************初始参数*********************/
	pid_YAW.target_val = 0.0;				
	pid_YAW.actual_val = 0.0;
	pid_YAW.out_val = 0.0;
	pid_YAW.err = 0.0;
	pid_YAW.err_last = 0.0;
	pid_YAW.integral = 0.0;

	/*--------------------------------小车--------------------------------*/
	/*****************PID参数*********************/
	/* 电机1位置 */
	pid_location1.Kp = 0.25;
	pid_location1.Ki = 0.0;	//位置环没积分
	pid_location1.Kd = 0.0;
	pid_location1.err_dead = 0.5;
	pid_location1.integral_limit = 0;	//位置环没积分限幅
	pid_location1.out_limit = TARGET_SPEED_RPM_LIMIT;	//位置环输出目标RPM给速度环，这个150是RPM_max

	/* 电机1速度 */
	pid_speed1.Kp = 5.0;
	pid_speed1.Ki = 0.4;
	pid_speed1.Kd = 0.0;
	pid_speed1.err_dead = 1.0;
	pid_speed1.integral_limit = 1700;
	pid_speed1.out_limit = 0;	//速度环输出PWM，这里不限幅 直接在电机MotorOutput限幅，最大PWM=900,限幅成900-50

	/* 电机2位置 */
	pid_location2.Kp = 0.25;
	pid_location2.Ki = 0.0;
	pid_location2.Kd = 0.0;
	pid_location2.err_dead = 0.5;
	pid_location2.integral_limit = 0;
	pid_location2.out_limit = TARGET_SPEED_RPM_LIMIT;

	/* 电机2速度 */
	pid_speed2.Kp = 5.0;
	pid_speed2.Ki = 0.4;
	pid_speed2.Kd = 0.0;
	pid_speed2.err_dead = 1.0;
	pid_speed2.integral_limit = 1700;
	pid_speed2.out_limit = 0;
	
	/*****************初始参数*********************/
	/* 电机1位置 */
	pid_location1.target_val = 0.0;				
	pid_location1.actual_val = 0.0;
	pid_location1.out_val = 0.0;
	pid_location1.err = 0.0;
	pid_location1.err_last = 0.0;
	pid_location1.integral = 0.0;
	/* 电机1速度 */
	pid_speed1.target_val = 0.0;				
	pid_speed1.actual_val = 0.0;
	pid_speed1.out_val = 0.0;
	pid_speed1.err = 0.0;
	pid_speed1.err_last = 0.0;
	pid_speed1.integral = 0.0;
	/* 电机2位置 */
	pid_location2.target_val = 0.0;				
	pid_location2.actual_val = 0.0;
	pid_location2.out_val = 0.0;
	pid_location2.err = 0.0;
	pid_location2.err_last = 0.0;
	pid_location2.integral = 0.0;
	/* 电机2速度 */
	pid_speed2.target_val = 0.0;				
	pid_speed2.actual_val = 0.0;
	pid_speed2.out_val = 0.0;
	pid_speed2.err = 0.0;
	pid_speed2.err_last = 0.0;
	pid_speed2.integral = 0.0;
	
	
	/*--------------------------------舵机--------------------------------*/
	/*****************PID参数*********************/
	
	//红激光
	/* pid_x */
	pid_x.Kp = -0.5;
	pid_x.Ki = -0.0;
	pid_x.Kd = -0.1;
	pid_x.err_dead = 1;	//像素坐标偏差
	pid_x.integral_limit = 0;
	pid_x.out_limit = 1;
	/* pid_y */
	pid_y.Kp = 0.5;
	pid_y.Ki = 0.0;
	pid_y.Kd = 0.1;
	pid_y.err_dead = 1;
	pid_y.integral_limit = 0;
	pid_y.out_limit = 1;
	
	//绿激光
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
	
	/*****************初始参数*********************/
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
  	pid->Kp = Kp;    // 设置比例系数 P
	pid->Ki = Ki;    // 设置积分系数 I
	pid->Kd = Kd;    // 设置微分系数 D
}

void set_pid_target(_pid *pid, float temp_val)
{
  pid->target_val = temp_val;    // 设置当前的目标值
}

float get_pid_target(_pid *pid)
{
  return pid->target_val;    // 获取当前的目标值
}

float get_pid_actual(_pid *pid)
{
  return pid->actual_val;    // 获取当前的实际值
}

/**
  * @brief  pid_x算法实现
  * @param  actual_val:实际值
  *	@note 	无
  * @retval 通过PID计算后的输出
  */
float pid_x_realize(_pid *pid, float actual_val)
{
	/* 传入实际值 */
	pid->actual_val = actual_val;
	
	/*计算误差*/	//此处target_val 由 set_pid_target 设置
    pid->err = pid->target_val - actual_val;
  
    /* 设定闭环死区 */
    if( (pid->err >= - pid->err_dead) && (pid->err <= pid->err_dead) ) 
    {
		pid->err = 0;
    }
	
	/*误差积累*/
	if(fabs(pid->Ki) >= 1e-6)
	{
		pid->integral += pid->err;
	}

	/*积分限幅*/
	if 		(pid->integral >= pid->integral_limit)	{pid->integral =  pid->integral_limit;}
	else if (pid->integral < -pid->integral_limit)	{pid->integral = -pid->integral_limit;}

	/*PID算法实现*/
    pid->out_val = 	pid->Kp * pid->err + 
					pid->Ki * pid->integral + 
					pid->Kd * (pid->err - pid->err_last);
  
	/*误差传递*/
    pid->err_last = pid->err;
    
	/*返回当前实际值*/
    return pid->out_val;
}

/**
  * @brief  pid_y算法实现
  * @param  actual_val:实际值
  *	@note 	无
  * @retval 通过PID计算后的输出
  */
float pid_y_realize(_pid *pid, float actual_val)
{
	/* 传入实际值 */
	pid->actual_val = actual_val;
	
	/*计算误差*/	//此处target_val 由 set_pid_target 设置
    pid->err = pid->target_val - actual_val;
  
    /* 设定闭环死区 */
    if( (pid->err >= - pid->err_dead) && (pid->err <= pid->err_dead) ) 
    {
		pid->err = 0;
    }
	
	/*误差积累*/
	if(fabs(pid->Ki) >= 1e-6)
	{
		pid->integral += pid->err;
	}
	
	/*积分限幅*/
	if 		(pid->integral >= pid->integral_limit)	{pid->integral =  pid->integral_limit;}
	else if (pid->integral < -pid->integral_limit)	{pid->integral = -pid->integral_limit;}

	/*PID算法实现*/
    pid->out_val = 	pid->Kp * pid->err + 
					pid->Ki * pid->integral + 
					pid->Kd * (pid->err - pid->err_last);
  
	/*误差传递*/
    pid->err_last = pid->err;
    
	/*返回当前实际值*/
    return pid->out_val;
}

/**
  * @brief  pid_x算法实现
  * @param  actual_val:实际值
  *	@note 	无
  * @retval 通过PID计算后的输出
  */
float pid2_x_realize(_pid *pid, float actual_val)
{
	/* 传入实际值 */
	pid->actual_val = actual_val;
	
	/*计算误差*/	//此处target_val 由 set_pid_target 设置
    pid->err = pid->target_val - actual_val;
  
    /* 设定闭环死区 */
    if( (pid->err >= - pid->err_dead) && (pid->err <= pid->err_dead) ) 
    {
		pid->err = 0;
    }
	
	/*误差积累*/
	if(fabs(pid->Ki) >= 1e-6)
	{
		pid->integral += pid->err;
	}

	/*积分限幅*/
	if 		(pid->integral >= pid->integral_limit)	{pid->integral =  pid->integral_limit;}
	else if (pid->integral < -pid->integral_limit)	{pid->integral = -pid->integral_limit;}

	/*PID算法实现*/
    pid->out_val = 	pid->Kp * pid->err + 
					pid->Ki * pid->integral + 
					pid->Kd * (pid->err - pid->err_last);
  
	/*误差传递*/
    pid->err_last = pid->err;
    
	/*返回当前实际值*/
    return pid->out_val;
}

/**
  * @brief  pid_y算法实现
  * @param  actual_val:实际值
  *	@note 	无
  * @retval 通过PID计算后的输出
  */
float pid2_y_realize(_pid *pid, float actual_val)
{
	/* 传入实际值 */
	pid->actual_val = actual_val;
	
	/*计算误差*/	//此处target_val 由 set_pid_target 设置
    pid->err = pid->target_val - actual_val;
  
    /* 设定闭环死区 */
    if( (pid->err >= - pid->err_dead) && (pid->err <= pid->err_dead) ) 
    {
		pid->err = 0;
    }
	
	/*误差积累*/
	if(fabs(pid->Ki) >= 1e-6)
	{
		pid->integral += pid->err;
	}
	
	/*积分限幅*/
	if 		(pid->integral >= pid->integral_limit)	{pid->integral =  pid->integral_limit;}
	else if (pid->integral < -pid->integral_limit)	{pid->integral = -pid->integral_limit;}

	/*PID算法实现*/
    pid->out_val = 	pid->Kp * pid->err + 
					pid->Ki * pid->integral + 
					pid->Kd * (pid->err - pid->err_last);
  
	/*误差传递*/
    pid->err_last = pid->err;
    
	/*返回当前实际值*/
    return pid->out_val;
}

/**
  * @brief  位置PID算法实现
  * @param  actual_val:实际值
  *	@note 	无
  * @retval 通过PID计算后的输出
  */
float location_pid_realize(_pid *pid, float actual_val)  //位置环光个Kp好像也可以
{
	/* 传入实际值 */
	pid->actual_val = actual_val;
	
	/*计算误差*/	//此处target_val 由 set_pid_target 设置
    pid->err = pid->target_val - actual_val;
  
    /* 设定闭环死区 */
    if( (pid->err >= - pid->err_dead) && (pid->err <= pid->err_dead) ) 
    {
		pid->err = 0;
		pid->integral = 0;	//位置到了就清空积分？
    }
	
	/*误差积累*/
	if(fabs(pid->Ki) >= 1e-6)
	{
		pid->integral += pid->err;
	}
	
	/*积分限幅*/	//位置环不需要积分限幅
//	if 		(pid->integral >= pid->integral_limit)	{pid->integral =  pid->integral_limit;}
//	else if (pid->integral < -pid->integral_limit)	{pid->integral = -pid->integral_limit;}

	/*PID算法实现*/
    pid->out_val = 	pid->Kp * pid->err + 
					pid->Ki * pid->integral + 
					pid->Kd * (pid->err - pid->err_last);
  
	/*误差传递*/
    pid->err_last = pid->err;
    
	/*返回当前实际值，此值作为速度pid的输入 即速度pid的目标速度rpm*/
    return pid->out_val;
}

/**
  * @brief  速度PID算法实现
  * @param  actual_val:实际值
  *	@note 	无
  * @retval 通过PID计算后的输出
  */
float speed_pid_realize(_pid *pid, float actual_val)
{
	/* 传入实际值 */
	pid->actual_val = actual_val;
	
	/* 计算误差 */
	pid->err = pid->target_val - actual_val;
	
    /* 设定闭环死区 */
    if( (pid->err >= - pid->err_dead) && (pid->err <= pid->err_dead) ) 
    {
		pid->err = 0;
    }

	/*误差积累*/
	if(fabs(pid->Ki) >= 1e-6)
	{
		pid->integral += pid->err;
	}
	
	/*积分限幅*/
	if 		(pid->integral >= pid->integral_limit)	{pid->integral =  pid->integral_limit;}
	else if (pid->integral < -pid->integral_limit)	{pid->integral = -pid->integral_limit;}

	/*PID算法实现*/
	pid->out_val = 	pid->Kp * pid->err +
					pid->Ki * pid->integral +
					pid->Kd * (pid->err - pid->err_last);
	
	/*输出偏移*/	
	//这个为了解决直线后期驱动力小相当于没驱动力的问题，但是会影响转弯的实现，因为转弯pwm本来就小，这里会将pwm放大，导致一直转
//	if(pid->out_val > 0)		pid->out_val += 10;

//	else if(pid->out_val < 0)	pid->out_val -= 10;

//	else						pid->out_val = 0;

	/*误差传递*/
	pid->err_last = pid->err;

	/*返回当前实际值*/
	return pid->out_val;
}
