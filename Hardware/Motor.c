#include "Motor.h"

uint8_t is_motor_en = 0;            // 电机使能

/*****************电机初始化***************/
//TIM8_CH1(PC6)、TIM8_CH2(PC7)	A轮PWM、B轮PWM
void Motor_Init(void)
{
	M_STBY_GPIO_CLK_ENABLE();
	M1_IN1_GPIO_CLK_ENABLE();
	M1_IN2_GPIO_CLK_ENABLE();
	M2_IN1_GPIO_CLK_ENABLE();
	M2_IN2_GPIO_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = M1_IN1_GPIO_Pin;
	GPIO_Init(M1_IN1_GPIO_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = M1_IN2_GPIO_Pin;
	GPIO_Init(M1_IN2_GPIO_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = M2_IN1_GPIO_Pin;
	GPIO_Init(M2_IN1_GPIO_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = M2_IN2_GPIO_Pin;
	GPIO_Init(M2_IN2_GPIO_Port, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = M_STBY_GPIO_Pin;			//电机使能引脚
	GPIO_Init(M_STBY_GPIO_Port, &GPIO_InitStructure);
	
	motor_enable();
	PWMAB_Init();
}

/*****************电机的控制函数***************/
/* 电机PWM输出 */
void MotorOutput(int16_t PWM1,int16_t PWM2)
{
	/*  速度上限处理 */
	if (PWM1 >= 0)		PWM1 = (PWM1 > PWM_MAX_PERIOD_COUNT) ? PWM_MAX_PERIOD_COUNT : PWM1;    
	else 				PWM1 = (PWM1 <-PWM_MAX_PERIOD_COUNT) ? -PWM_MAX_PERIOD_COUNT : PWM1;

	if(PWM2 >= 0)		PWM2 = (PWM2 > PWM_MAX_PERIOD_COUNT) ? PWM_MAX_PERIOD_COUNT : PWM2;
	else 				PWM2 = (PWM2 <-PWM_MAX_PERIOD_COUNT) ? -PWM_MAX_PERIOD_COUNT : PWM2; 

	Motor1_SetPWM(PWM1);        // 设置 PWM1 占空比
	Motor2_SetPWM(PWM2);      	// 设置 PWM2 占空比
 }

/* 电机速度及方向控制 */	//方向不对只需要改这里的 0 1
void Motor1_SetPWM(int16_t PWM1)
{
	if (PWM1 >= 0)
	{
		M1_IN1(0);
		M1_IN2(1);
		PWMA_SetCompare1(PWM1);
	}
	else
	{
		M1_IN1(1);
		M1_IN2(0);
		PWMA_SetCompare1(-PWM1);
	}
}

void Motor2_SetPWM(int16_t PWM2)
{
	if (PWM2 >= 0)
	{
		M2_IN1(1);
		M2_IN2(0);
		PWMB_SetCompare2(PWM2);
	}
	else
	{
		M2_IN1(0);
		M2_IN2(1);
		PWMB_SetCompare2(-PWM2);
	}
}


/* 电机驱动使能 */
void motor_enable()
{
	is_motor_en  = 1;
	GPIO_SetBits(M_STBY_GPIO_Port, M_STBY_GPIO_Pin);
}

/* 电机驱动失能 */
void motor_disable()
{
	is_motor_en  = 0;
	GPIO_ResetBits(M_STBY_GPIO_Port, M_STBY_GPIO_Pin);
}


//单位时间脉冲数	pulse_unit	int16_t
//转速				rpm			float
//速度				cmps		float

//总脉冲数			pulse		long
//距离				cm			float
//角度				angle		float

//单位时间脉冲数 to rpm
float pulse2rpm(int16_t pulse_unit)
{
	return ((float)pulse_unit * 60.0 * 1000.0)/(MAI_CHONG_PER_ROUND * PID_PERIOD);
}
//rpm to cmps
float rpm2cmps(float rpm)
{
	return (rpm / 60.0) * (2.0 * WheelR * MY_PI);
}
//单位时间脉冲数 to cm/s
float pulse2cmps(int16_t pulse_unit)
{
	float rpm;
	rpm = pulse2rpm(pulse_unit);
	return rpm2cmps(rpm);
}

//总脉冲数 to 圈数
float pulse2round(long pulse)
{
	return ((float)pulse /(MAI_CHONG_PER_ROUND));
}
//总脉冲数 to cm
float pulse2cm(long pulse)
{
	return ((float)pulse /(MAI_CHONG_PER_ROUND)) * (2 * WheelR * MY_PI);
}
//cm to 总脉冲数
long cm2pulse(float cm)
{
	return (long)((cm / (2.0 * WheelR * MY_PI)) * (MAI_CHONG_PER_ROUND));
}
//角度 to 距离cm
float angle2cm(float angle)
{
	return (angle / 360.0) * (MY_PI * LUN_JU);	//x°对应的距离
}
//角度 to 总脉冲数
long angle2pulse(float angle)
{
	float cm;
	cm = angle2cm(angle);
	return cm2pulse(cm);
}



