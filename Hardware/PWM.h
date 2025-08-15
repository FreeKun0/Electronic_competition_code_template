#ifndef __PWM_H
#define __PWM_H
#include "stm32f10x.h"                  // Device header

/* 舵机1x、1y PWM*/
void PWM1xy_Init(void);
void PWM1x_SetCompare1(uint16_t Compare);
void PWM1y_SetCompare2(uint16_t Compare);

/* 舵机2x、2y PWM*/
//void PWM2xy_Init(void);
//void PWM2x_SetCompare1(uint16_t Compare);
//void PWM2y_SetCompare2(uint16_t Compare);

/* 电机1、2 PWM*/
void PWMAB_Init(void);
void PWMA_SetCompare1(uint16_t Compare);
void PWMB_SetCompare2(uint16_t Compare);

/* 电机3、4 PWM*/
//void PWMCD_Init(void);
//void PWMC_SetCompare3(uint16_t Compare);
//void PWMD_SetCompare4(uint16_t Compare);
#endif
