#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "parameter.h"
#include "control.h"

/* 电机引脚及时钟使能 */
#define M_STBY_GPIO_Pin 		GPIO_Pin_5
#define M_STBY_GPIO_Port 		GPIOA
#define M_STBY_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); }while(0)




#define M1_IN1_GPIO_Pin 		GPIO_Pin_12
#define M1_IN1_GPIO_Port 		GPIOB
#define M1_IN1_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); }while(0)
	
#define M1_IN2_GPIO_Pin 		GPIO_Pin_13
#define M1_IN2_GPIO_Port 		GPIOB
#define M1_IN2_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); }while(0)
	
#define M2_IN1_GPIO_Pin 		GPIO_Pin_14
#define M2_IN1_GPIO_Port 		GPIOB
#define M2_IN1_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); }while(0)
	
#define M2_IN2_GPIO_Pin 		GPIO_Pin_15
#define M2_IN2_GPIO_Port 		GPIOB
#define M2_IN2_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); }while(0)

/* 电机1方向引脚1极性 */
#define M1_IN1(x)   do{ x ? \
                      GPIO_SetBits(M1_IN1_GPIO_Port, M1_IN1_GPIO_Pin) : \
                      GPIO_ResetBits(M1_IN1_GPIO_Port, M1_IN1_GPIO_Pin); \
					}while(0)

/* 电机2方向引脚2极性 */
#define M1_IN2(x)   do{ x ? \
                      GPIO_SetBits(M1_IN2_GPIO_Port, M1_IN2_GPIO_Pin) : \
                      GPIO_ResetBits(M1_IN2_GPIO_Port, M1_IN2_GPIO_Pin); \
					}while(0)

/* 电机2方向引脚1极性 */
#define M2_IN1(x)   do{ x ? \
                      GPIO_SetBits(M2_IN1_GPIO_Port, M2_IN1_GPIO_Pin) : \
                      GPIO_ResetBits(M2_IN1_GPIO_Port, M2_IN1_GPIO_Pin); \
					}while(0)
/* 电机2方向引脚2极性 */
#define M2_IN2(x)   do{ x ? \
                      GPIO_SetBits(M2_IN2_GPIO_Port, M2_IN2_GPIO_Pin) : \
                      GPIO_ResetBits(M2_IN2_GPIO_Port, M2_IN2_GPIO_Pin); \
					}while(0)		  
				  
  		  
#define PWM_PERIOD_COUNT		(900)						//定时器PWM自动重装载值ARR
#define PWM_MAX_PERIOD_COUNT	(PWM_PERIOD_COUNT - 50)		//如果PWM弄成了满的，一些驱动板就会出现问题（硬件上的原因）


extern uint8_t is_motor_en;

void Motor_Init(void);
void MotorOutput(int16_t PWM1,int16_t PWM2);
void Motor1_SetPWM(int16_t PWM);
void Motor2_SetPWM(int16_t PWM);

void motor_enable(void);
void motor_disable(void);
					
					
//单位时间脉冲数	pulse_unit	int16_t
//转速				rpm			float
//速度				cmps		float

//总脉冲数			pulse		long
//距离				cm			float
//角度				angle		float

//单位时间脉冲数 to rpm
float pulse2rpm(int16_t pulse_unit);
//rpm to cmps
float rpm2cmps(float rpm);
//单位时间脉冲数 to cm/s
float pulse2cmps(int16_t pulse_unit);
//总脉冲数 to 圈数
float pulse2round(long pulse);
//总脉冲数 to cm
float pulse2cm(long pulse);
//cm to 总脉冲数
long cm2pulse(float cm);
//角度 to 距离cm
float angle2cm(float angle);
//角度 to 总脉冲数
long angle2pulse(float angle);
#endif
