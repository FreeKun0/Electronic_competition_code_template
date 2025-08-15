#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "parameter.h"
#include "control.h"

/* ������ż�ʱ��ʹ�� */
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

/* ���1��������1���� */
#define M1_IN1(x)   do{ x ? \
                      GPIO_SetBits(M1_IN1_GPIO_Port, M1_IN1_GPIO_Pin) : \
                      GPIO_ResetBits(M1_IN1_GPIO_Port, M1_IN1_GPIO_Pin); \
					}while(0)

/* ���2��������2���� */
#define M1_IN2(x)   do{ x ? \
                      GPIO_SetBits(M1_IN2_GPIO_Port, M1_IN2_GPIO_Pin) : \
                      GPIO_ResetBits(M1_IN2_GPIO_Port, M1_IN2_GPIO_Pin); \
					}while(0)

/* ���2��������1���� */
#define M2_IN1(x)   do{ x ? \
                      GPIO_SetBits(M2_IN1_GPIO_Port, M2_IN1_GPIO_Pin) : \
                      GPIO_ResetBits(M2_IN1_GPIO_Port, M2_IN1_GPIO_Pin); \
					}while(0)
/* ���2��������2���� */
#define M2_IN2(x)   do{ x ? \
                      GPIO_SetBits(M2_IN2_GPIO_Port, M2_IN2_GPIO_Pin) : \
                      GPIO_ResetBits(M2_IN2_GPIO_Port, M2_IN2_GPIO_Pin); \
					}while(0)		  
				  
  		  
#define PWM_PERIOD_COUNT		(900)						//��ʱ��PWM�Զ���װ��ֵARR
#define PWM_MAX_PERIOD_COUNT	(PWM_PERIOD_COUNT - 50)		//���PWMŪ�������ģ�һЩ������ͻ�������⣨Ӳ���ϵ�ԭ��


extern uint8_t is_motor_en;

void Motor_Init(void);
void MotorOutput(int16_t PWM1,int16_t PWM2);
void Motor1_SetPWM(int16_t PWM);
void Motor2_SetPWM(int16_t PWM);

void motor_enable(void);
void motor_disable(void);
					
					
//��λʱ��������	pulse_unit	int16_t
//ת��				rpm			float
//�ٶ�				cmps		float

//��������			pulse		long
//����				cm			float
//�Ƕ�				angle		float

//��λʱ�������� to rpm
float pulse2rpm(int16_t pulse_unit);
//rpm to cmps
float rpm2cmps(float rpm);
//��λʱ�������� to cm/s
float pulse2cmps(int16_t pulse_unit);
//�������� to Ȧ��
float pulse2round(long pulse);
//�������� to cm
float pulse2cm(long pulse);
//cm to ��������
long cm2pulse(float cm);
//�Ƕ� to ����cm
float angle2cm(float angle);
//�Ƕ� to ��������
long angle2pulse(float angle);
#endif
