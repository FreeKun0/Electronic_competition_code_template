#ifndef __STEPPER_H
#define __STEPPER_H

#include "stm32f10x.h"

// 硬件引脚定义
#define STEP_PORT_X		GPIOA
#define STEP_PIN_X		GPIO_Pin_0    // TIM5_CH1
#define STEP_PORT_Y		GPIOA
#define STEP_PIN_Y		GPIO_Pin_1    // TIM5_CH2

#define DIR_PORT_X		GPIOC
#define DIR_PIN_X		GPIO_Pin_0    // X轴方向控制
#define DIR_PORT_Y		GPIOC
#define DIR_PIN_Y		GPIO_Pin_2    // Y轴方向控制

#define EN_PORT_X		GPIOC
#define EN_PIN_X		GPIO_Pin_1    // X轴使能控制（高电平使能）
#define EN_PORT_Y		GPIOC
#define EN_PIN_Y		GPIO_Pin_4    // Y轴使能控制（高电平使能）

// 定时器定义
#define STEPPER_TIM		TIM5
#define TIM_CLK			72000000UL    // 定时器时钟频率(72MHz)


// 电机状态结构体
typedef struct 
{
	uint8_t		enabled;        // 使能状态
	uint8_t		direction;      // 方向
	uint32_t	speed;			// 速度（脉冲/秒）
	int32_t		current_step;   // 当前步数
	uint8_t		edge_flag;      // 边沿计数标志
} MotorState;

// 步进电机编号定义
typedef enum {
    STEP_X = 0,  // X轴步进电机（TIM5_CH1 - PA0）
    STEP_Y = 1   // Y轴步进电机（TIM5_CH2 - PA1）
} StepID;

// 方向定义
#define DIR_FORWARD		0
#define DIR_BACKWARD	1

extern MotorState stepper[2];

// 初始化函数
void Stepper_Init(void);

// 电机控制函数
void Stepper_Enable(StepID id, uint8_t enable);       // 使能/禁用电机
void Stepper_SetDir(StepID id, uint8_t dir);          // 设置方向
void Stepper_SetSpeed(StepID id, uint32_t speed);     // 设置速度（脉冲/秒）
void Stepper_Start(StepID id);                        // 启动电机
void Stepper_Stop(StepID id);                         // 停止电机
void Stepper_DualControl(int32_t x_param, int32_t y_param); // 双电机综合控制

// 获取当前步数
int32_t Stepper_GetCurrentStep(StepID id);

#endif /* __STEPPER_H */
