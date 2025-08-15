#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "control.h"
#include "Delay.h"

/*-----------------------------------‘∆Ã®1xy PWM-----------------------------------------*/
void Servo_Init(void);		//TIM5_CH3(PA2)°¢TIM5_CH4(PA3)	”ÎUSART2≥ÂÕª
void Servo1_SetAngle(float Angle);
void Servo2_SetAngle(float Angle);
void Servo_All_SetAngle(float Angle1, float Angle2);

void Servo1_SetPWM(float PWM);
void Servo2_SetPWM(float PWM);
void Servo_All_SetPWM(float PWM1, float PWM2);
void Servo_All_SetPWM_ChangeRotationRed(float PWM1, float PWM2);


/*-----------------------------------‘∆Ã®2xy PWM-----------------------------------------*/
//void Servo2_Init(void);		//TIM4_CH1(PB6)°¢TIM4_CH2(PB7)
//void Servo21_SetAngle(float Angle);
//void Servo22_SetAngle(float Angle);
//void Servo2_All_SetAngle(float Angle1, float Angle2);

//void Servo21_SetPWM(float PWM);
//void Servo22_SetPWM(float PWM);
//void Servo2_All_SetPWM(float PWM1, float PWM2);

#endif
