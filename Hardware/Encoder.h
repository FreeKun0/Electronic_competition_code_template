#ifndef __ENCODER_H
#define __ENCODER_H
#include "stm32f10x.h"                  // Device header
//#include "parameter.h"


#define E1A_GPIO_Pin 	GPIO_Pin_6
#define E1A_GPIO_Port 	GPIOA
#define E1A_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); }while(0)

#define E1B_GPIO_Pin 	GPIO_Pin_7
#define E1B_GPIO_Port 	GPIOA
#define E1B_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); }while(0)


#define E2A_GPIO_Pin 	GPIO_Pin_6
#define E2A_GPIO_Port 	GPIOB
#define E2A_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); }while(0)

#define E2B_GPIO_Pin 	GPIO_Pin_7
#define E2B_GPIO_Port 	GPIOB
#define E2B_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); }while(0)

typedef struct _MotorPulse
{
	short one;	//电机10ms脉冲 对标速度
    long sum;	//电机累计脉冲 对标距离
}_MotorPulse;

extern _MotorPulse motor1, motor2;

extern short Motor1_Pulse_Unit, Motor2_Pulse_Unit;
extern long Motor1_Pulse_Sigma, Motor2_Pulse_Sigma;

//TIM3_CH1(PA6)、TIM3_CH2(PA7)	左轮编码器
//TIM4_CH1(PB6)、TIM4_CH2(PB7)	右轮编码器
void Encoder_Init(void);
//int16_t Encoder_1Get(void);
//int16_t Encoder_2Get(void);
void GetMotorPulse(void);

#endif

