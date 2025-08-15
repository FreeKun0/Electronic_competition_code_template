#include "stepper.h"




// ȫ��״̬����
MotorState stepper[2] = {0};

/*-----------------------------------���������ʼ��-----------------------------------------*/
// TIM5_CH1(PA0) TIM5_CH2(PA1)
void Stepper_Init(void)
{
	/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// ����X/Y��PWM������ţ��������������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = STEP_PIN_X | STEP_PIN_Y;
	GPIO_Init(STEP_PORT_X, &GPIO_InitStructure);
	
	// ���÷���������ţ����������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	// X�᷽��
	GPIO_InitStructure.GPIO_Pin = DIR_PIN_X;
	GPIO_Init(DIR_PORT_X, &GPIO_InitStructure);
	
	// Y�᷽��
	GPIO_InitStructure.GPIO_Pin = DIR_PIN_Y;
	GPIO_Init(DIR_PORT_Y, &GPIO_InitStructure);
	
	// ����ʹ�ܿ������ţ����������
	// X��ʹ��
	GPIO_InitStructure.GPIO_Pin = EN_PIN_X;
	GPIO_Init(EN_PORT_X, &GPIO_InitStructure);
	
	// Y��ʹ��
	GPIO_InitStructure.GPIO_Pin = EN_PIN_Y;
	GPIO_Init(EN_PORT_Y, &GPIO_InitStructure);
	
	// ��ʼ״̬�����õ�����͵�ƽ���ã�
	GPIO_ResetBits(EN_PORT_X, EN_PIN_X);
	GPIO_ResetBits(EN_PORT_Y, EN_PIN_Y);
	
	/*����ʱ��Դ*/
	TIM_InternalClockConfig(STEPPER_TIM);
	
	/*ʱ����Ԫ��ʼ��*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 7200 - 1;				// �������ڣ���ARR��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;					// Ԥ��Ƶ������PSC��ֵ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(STEPPER_TIM, &TIM_TimeBaseInitStructure);
	
	/*����Ƚϳ�ʼ��*/
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 3600;							// CCRֵ��50%ռ�ձ�
	
	// ����ͨ��1 (X��)
	TIM_OC1Init(STEPPER_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(STEPPER_TIM, TIM_OCPreload_Enable);
	
	// ����ͨ��2 (Y��)
	TIM_OC2Init(STEPPER_TIM, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(STEPPER_TIM, TIM_OCPreload_Enable);
	
	/*Ԥװ������*/
	TIM_ARRPreloadConfig(STEPPER_TIM, ENABLE);
	
	/*�ж�����*/
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// ʹ�ܱȽ��ж�
	TIM_ITConfig(STEPPER_TIM, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);
	
	/*��ʱ��ʹ��*/
	TIM_Cmd(STEPPER_TIM, DISABLE);	// ��ʼ���ö�ʱ��
	
	/*��ʼ�����״̬*/
	stepper[STEP_X].enabled = 0;
	stepper[STEP_X].direction = DIR_FORWARD;
	stepper[STEP_X].speed = 0;
	stepper[STEP_X].current_step = 0;
	stepper[STEP_X].edge_flag = 0;
	
	stepper[STEP_Y].enabled = 0;
	stepper[STEP_Y].direction = DIR_FORWARD;
	stepper[STEP_Y].speed = 0;
	stepper[STEP_Y].current_step = 0;
	stepper[STEP_Y].edge_flag = 0;
}

// ʹ��/���õ�����ߵ�ƽʹ�ܣ�
void Stepper_Enable(StepID id, uint8_t enable) 
{
	stepper[id].enabled = enable;
	
	if (id == STEP_X)
	{
		if (enable)
		{
			GPIO_SetBits(EN_PORT_X, EN_PIN_X);  // �ߵ�ƽʹ��
		}
		else
		{
			GPIO_ResetBits(EN_PORT_X, EN_PIN_X);  // �͵�ƽ����
			Stepper_Stop(STEP_X);
		}
	} 
	else 
	{
		if (enable) 
		{
			GPIO_SetBits(EN_PORT_Y, EN_PIN_Y);  // �ߵ�ƽʹ��
		} 
		else 
		{
			GPIO_ResetBits(EN_PORT_Y, EN_PIN_Y);  // �͵�ƽ����
			Stepper_Stop(STEP_Y);
		}
	}
}

// ���÷���
void Stepper_SetDir(StepID id, uint8_t dir)
{
	stepper[id].direction = dir;
	
	if (id == STEP_X)
	{
		if (dir == DIR_FORWARD)
		{
				GPIO_ResetBits(DIR_PORT_X, DIR_PIN_X);
		}
		else
		{
				GPIO_SetBits(DIR_PORT_X, DIR_PIN_X);
		}
	}
	else
	{
		if (dir == DIR_FORWARD)
		{
				GPIO_ResetBits(DIR_PORT_Y, DIR_PIN_Y);
		}
		else
		{
				GPIO_SetBits(DIR_PORT_Y, DIR_PIN_Y);
		}
	}
}

// �����ٶ�(����/��) - ʹ�ÿ⺯������ARR�ͱȽ�ֵ
void Stepper_SetSpeed(StepID id, uint32_t speed)
{
	if (speed == 0)
	{
		Stepper_Stop(id);
		stepper[id].speed = 0;
		return;
	}
	
	stepper[id].speed = speed;
	uint16_t period = (TIM_CLK / speed) - 1;
	
	// ʹ�ÿ⺯�������Զ�����ֵ(ARR)
	TIM_SetAutoreload(STEPPER_TIM, period);
	
	// ʹ�ÿ⺯�����ñȽ�ֵ(CCR)��ռ�ձ�50%
	if (id == STEP_X)
	{
		TIM_SetCompare1(STEPPER_TIM, period / 2);
	} 
	else 
	{
		TIM_SetCompare2(STEPPER_TIM, period / 2);
	}
}

// ������� - ʹ�ÿ⺯��ʹ�ܶ�ʱ��
void Stepper_Start(StepID id)
{
	TIM_Cmd(STEPPER_TIM, ENABLE);  // ʹ�ÿ⺯��������ʱ��
}

// ֹͣ��� - ʹ�ÿ⺯�����ö�ʱ��
void Stepper_Stop(StepID id)
{
	TIM_Cmd(STEPPER_TIM, DISABLE);  // ʹ�ÿ⺯��ֹͣ��ʱ��
}

// ˫����ۺϿ���
void Stepper_DualControl(int32_t x_param, int32_t y_param)
{
	// ����X��
	if (x_param > 0)
	{
		Stepper_SetDir(STEP_X, DIR_FORWARD);
		Stepper_SetSpeed(STEP_X, x_param);
		Stepper_Start(STEP_X);
	}
	else if (x_param < 0)
	{
		Stepper_SetDir(STEP_X, DIR_BACKWARD);
		Stepper_SetSpeed(STEP_X, -x_param);
		Stepper_Start(STEP_X);
	}
	else
	{
		Stepper_Stop(STEP_X);
		Stepper_SetSpeed(STEP_X, 0);
	}
	
	// ����Y��
	if (y_param > 0)
	{
		Stepper_SetDir(STEP_Y, DIR_FORWARD);
		Stepper_SetSpeed(STEP_Y, y_param);
		Stepper_Start(STEP_Y);
	}
	else if (y_param < 0)
	{
		Stepper_SetDir(STEP_Y, DIR_BACKWARD);
		Stepper_SetSpeed(STEP_Y, -y_param);
		Stepper_Start(STEP_Y);
	}
	else
	{
		Stepper_Stop(STEP_Y);
		Stepper_SetSpeed(STEP_Y, 0);
	}
}

// ��ȡ��ǰ����
int32_t Stepper_GetCurrentStep(StepID id)
{
	return stepper[id].current_step;
}

// TIM5�жϷ�����
void TIM5_IRQHandler(void)
{
	// ����ͨ��1�жϣ�X�ᣩ
	if (TIM_GetITStatus(STEPPER_TIM, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(STEPPER_TIM, TIM_IT_CC1);

		if (stepper[STEP_X].enabled)
		{
			if (stepper[STEP_X].edge_flag++ % 2 == 0)
			{
				if (stepper[STEP_X].direction == DIR_FORWARD)
				{
					stepper[STEP_X].current_step++;
				}
				else 
				{
					stepper[STEP_X].current_step--;
				}
			}
		}
	}

	// ����ͨ��2�жϣ�Y�ᣩ
	if (TIM_GetITStatus(STEPPER_TIM, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(STEPPER_TIM, TIM_IT_CC2);

		if (stepper[STEP_Y].enabled)
		{
			if (stepper[STEP_Y].edge_flag++ % 2 == 0)
			{
				if (stepper[STEP_Y].direction == DIR_FORWARD)
				{
					stepper[STEP_Y].current_step++;
				}
				else 
				{
					stepper[STEP_Y].current_step--;
				}
			}
		}
	}
}
