#ifndef __STEPPER_H
#define __STEPPER_H

#include "stm32f10x.h"

// Ӳ�����Ŷ���
#define STEP_PORT_X		GPIOA
#define STEP_PIN_X		GPIO_Pin_0    // TIM5_CH1
#define STEP_PORT_Y		GPIOA
#define STEP_PIN_Y		GPIO_Pin_1    // TIM5_CH2

#define DIR_PORT_X		GPIOC
#define DIR_PIN_X		GPIO_Pin_0    // X�᷽�����
#define DIR_PORT_Y		GPIOC
#define DIR_PIN_Y		GPIO_Pin_2    // Y�᷽�����

#define EN_PORT_X		GPIOC
#define EN_PIN_X		GPIO_Pin_1    // X��ʹ�ܿ��ƣ��ߵ�ƽʹ�ܣ�
#define EN_PORT_Y		GPIOC
#define EN_PIN_Y		GPIO_Pin_4    // Y��ʹ�ܿ��ƣ��ߵ�ƽʹ�ܣ�

// ��ʱ������
#define STEPPER_TIM		TIM5
#define TIM_CLK			72000000UL    // ��ʱ��ʱ��Ƶ��(72MHz)


// ���״̬�ṹ��
typedef struct 
{
	uint8_t		enabled;        // ʹ��״̬
	uint8_t		direction;      // ����
	uint32_t	speed;			// �ٶȣ�����/�룩
	int32_t		current_step;   // ��ǰ����
	uint8_t		edge_flag;      // ���ؼ�����־
} MotorState;

// ���������Ŷ���
typedef enum {
    STEP_X = 0,  // X�Ჽ�������TIM5_CH1 - PA0��
    STEP_Y = 1   // Y�Ჽ�������TIM5_CH2 - PA1��
} StepID;

// ������
#define DIR_FORWARD		0
#define DIR_BACKWARD	1

extern MotorState stepper[2];

// ��ʼ������
void Stepper_Init(void);

// ������ƺ���
void Stepper_Enable(StepID id, uint8_t enable);       // ʹ��/���õ��
void Stepper_SetDir(StepID id, uint8_t dir);          // ���÷���
void Stepper_SetSpeed(StepID id, uint32_t speed);     // �����ٶȣ�����/�룩
void Stepper_Start(StepID id);                        // �������
void Stepper_Stop(StepID id);                         // ֹͣ���
void Stepper_DualControl(int32_t x_param, int32_t y_param); // ˫����ۺϿ���

// ��ȡ��ǰ����
int32_t Stepper_GetCurrentStep(StepID id);

#endif /* __STEPPER_H */
