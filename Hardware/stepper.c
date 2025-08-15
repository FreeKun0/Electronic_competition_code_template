#include "stepper.h"




// 全局状态变量
MotorState stepper[2] = {0};

/*-----------------------------------步进电机初始化-----------------------------------------*/
// TIM5_CH1(PA0) TIM5_CH2(PA1)
void Stepper_Init(void)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 配置X/Y轴PWM输出引脚（复用推挽输出）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = STEP_PIN_X | STEP_PIN_Y;
	GPIO_Init(STEP_PORT_X, &GPIO_InitStructure);
	
	// 配置方向控制引脚（推挽输出）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	// X轴方向
	GPIO_InitStructure.GPIO_Pin = DIR_PIN_X;
	GPIO_Init(DIR_PORT_X, &GPIO_InitStructure);
	
	// Y轴方向
	GPIO_InitStructure.GPIO_Pin = DIR_PIN_Y;
	GPIO_Init(DIR_PORT_Y, &GPIO_InitStructure);
	
	// 配置使能控制引脚（推挽输出）
	// X轴使能
	GPIO_InitStructure.GPIO_Pin = EN_PIN_X;
	GPIO_Init(EN_PORT_X, &GPIO_InitStructure);
	
	// Y轴使能
	GPIO_InitStructure.GPIO_Pin = EN_PIN_Y;
	GPIO_Init(EN_PORT_Y, &GPIO_InitStructure);
	
	// 初始状态：禁用电机（低电平禁用）
	GPIO_ResetBits(EN_PORT_X, EN_PIN_X);
	GPIO_ResetBits(EN_PORT_Y, EN_PIN_Y);
	
	/*配置时钟源*/
	TIM_InternalClockConfig(STEPPER_TIM);
	
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 7200 - 1;				// 计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;					// 预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(STEPPER_TIM, &TIM_TimeBaseInitStructure);
	
	/*输出比较初始化*/
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 3600;							// CCR值，50%占空比
	
	// 配置通道1 (X轴)
	TIM_OC1Init(STEPPER_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(STEPPER_TIM, TIM_OCPreload_Enable);
	
	// 配置通道2 (Y轴)
	TIM_OC2Init(STEPPER_TIM, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(STEPPER_TIM, TIM_OCPreload_Enable);
	
	/*预装载配置*/
	TIM_ARRPreloadConfig(STEPPER_TIM, ENABLE);
	
	/*中断配置*/
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// 使能比较中断
	TIM_ITConfig(STEPPER_TIM, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);
	
	/*定时器使能*/
	TIM_Cmd(STEPPER_TIM, DISABLE);	// 初始禁用定时器
	
	/*初始化电机状态*/
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

// 使能/禁用电机（高电平使能）
void Stepper_Enable(StepID id, uint8_t enable) 
{
	stepper[id].enabled = enable;
	
	if (id == STEP_X)
	{
		if (enable)
		{
			GPIO_SetBits(EN_PORT_X, EN_PIN_X);  // 高电平使能
		}
		else
		{
			GPIO_ResetBits(EN_PORT_X, EN_PIN_X);  // 低电平禁用
			Stepper_Stop(STEP_X);
		}
	} 
	else 
	{
		if (enable) 
		{
			GPIO_SetBits(EN_PORT_Y, EN_PIN_Y);  // 高电平使能
		} 
		else 
		{
			GPIO_ResetBits(EN_PORT_Y, EN_PIN_Y);  // 低电平禁用
			Stepper_Stop(STEP_Y);
		}
	}
}

// 设置方向
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

// 设置速度(脉冲/秒) - 使用库函数配置ARR和比较值
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
	
	// 使用库函数更新自动重载值(ARR)
	TIM_SetAutoreload(STEPPER_TIM, period);
	
	// 使用库函数设置比较值(CCR)，占空比50%
	if (id == STEP_X)
	{
		TIM_SetCompare1(STEPPER_TIM, period / 2);
	} 
	else 
	{
		TIM_SetCompare2(STEPPER_TIM, period / 2);
	}
}

// 启动电机 - 使用库函数使能定时器
void Stepper_Start(StepID id)
{
	TIM_Cmd(STEPPER_TIM, ENABLE);  // 使用库函数启动定时器
}

// 停止电机 - 使用库函数禁用定时器
void Stepper_Stop(StepID id)
{
	TIM_Cmd(STEPPER_TIM, DISABLE);  // 使用库函数停止定时器
}

// 双电机综合控制
void Stepper_DualControl(int32_t x_param, int32_t y_param)
{
	// 处理X轴
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
	
	// 处理Y轴
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

// 获取当前步数
int32_t Stepper_GetCurrentStep(StepID id)
{
	return stepper[id].current_step;
}

// TIM5中断服务函数
void TIM5_IRQHandler(void)
{
	// 处理通道1中断（X轴）
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

	// 处理通道2中断（Y轴）
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
