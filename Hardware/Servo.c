#include "Servo.h"

/*-----------------------------------云台1xy PWM-----------------------------------------*/
//舵机初始化
void Servo_Init(void)	//TIM5_CH3(PA2)、TIM5_CH4(PA3)	与USART2冲突
{
	PWM1xy_Init();									//初始化舵机的底层PWM
}

//舵机x设置角度
void Servo1_SetAngle(float Angle)
{
	PWM1x_SetCompare1(Angle / 180 * 2000 + 500);	//设置占空比
}

//舵机y设置角度
void Servo2_SetAngle(float Angle)
{
	if(Angle < 5 )	Angle = 5;
	if(Angle > 170)	Angle = 170;
	PWM1y_SetCompare2(Angle / 180 * 2000 + 500);	//设置占空比
}

//舵机x、y设置角度
void Servo_All_SetAngle(float Angle1, float Angle2)
{
	Servo1_SetAngle(Angle1);
	Servo2_SetAngle(Angle2);
}

//舵机x设置PWM
void Servo1_SetPWM(float PWM)
{
//	if(PWM < 556 )	PWM = 556;
//	if(PWM > 2389)	PWM = 2389;
	PWM1x_SetCompare1(PWM);	//设置占空比
}

//舵机y设置PWM
void Servo2_SetPWM(float PWM)
{
	if(PWM < 556 )	PWM = 556;
	if(PWM > 2389)	PWM = 2389;
	PWM1y_SetCompare2(PWM);	//设置占空比
}

//舵机x、y设置PWM
void Servo_All_SetPWM(float PWM1, float PWM2)
{
	Servo1_SetPWM(PWM1);
	Servo2_SetPWM(PWM2);
}
//舵机x、y设置PWM,同时记录当前PWM(刷新rotation_red[2])
void Servo_All_SetPWM_ChangeRotationRed(float PWM1, float PWM2)
{
	rotation_red[0] = PWM1;
	rotation_red[1] = PWM2;
	Servo_All_SetPWM(rotation_red[0], rotation_red[1]);
}

/*-----------------------------------云台2xy PWM-----------------------------------------*/
////舵机初始化
//void Servo2_Init(void)	//TIM4_CH1(PB6)、TIM4_CH2(PB7)
//{
//	PWM2xy_Init();									//初始化舵机的底层PWM
//}

////舵机x设置角度
//void Servo21_SetAngle(float Angle)
//{
//	PWM2x_SetCompare1(Angle / 180 * 2000 + 500);	//设置占空比
//}

////舵机y设置角度
//void Servo22_SetAngle(float Angle)
//{
//	if(Angle < 5 )	Angle = 5;
//	if(Angle > 170)	Angle = 170;
//	PWM2y_SetCompare2(Angle / 180 * 2000 + 500);	//设置占空比
//}

////舵机x、y设置角度
//void Servo2_All_SetAngle(float Angle1, float Angle2)
//{
//	Servo21_SetAngle(Angle1);
//	Servo22_SetAngle(Angle2);
//}

////舵机x设置PWM
//void Servo21_SetPWM(float PWM)
//{
////	if(PWM < 556 )	PWM = 556;
////	if(PWM > 2389)	PWM = 2389;
//	PWM2x_SetCompare1(PWM);	//设置占空比
//}

////舵机y设置PWM
//void Servo22_SetPWM(float PWM)
//{
//	if(PWM < 556 )	PWM = 556;
//	if(PWM > 2389)	PWM = 2389;
//	PWM2y_SetCompare2(PWM);	//设置占空比
//}

////舵机x、y设置PWM
//void Servo2_All_SetPWM(float PWM1, float PWM2)
//{
//	Servo21_SetPWM(PWM1);
//	Servo22_SetPWM(PWM2);
//}

