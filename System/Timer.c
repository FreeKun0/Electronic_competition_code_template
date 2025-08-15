#include "stm32f10x.h"                  // Device header


void Timer_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); // 修改为TIM6的时钟使能，TIM6在APB1总线上
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    /* 这里定时器中断 72MHz / 2 / 36000 = 1000，1ms进一次中断 */
    TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1; // 周期值不变
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1; // 预分频值不变
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStructure); // 修改为TIM6
    
    TIM_ClearFlag(TIM6, TIM_FLAG_Update); // 修改为TIM6
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE); // 修改为TIM6
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn; // 修改为TIM6的中断通道
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_Cmd(TIM6, ENABLE); // 修改为TIM6
}

/*
void TIM6_IRQHandler(void) // 修改为TIM4的中断处理函数
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
    {
        
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    }
}
*/
//void Timer_Init(void)
//{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
//	
//	TIM_InternalClockConfig(TIM1);
//	
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	/* 这里定时器中断 72MHz / 1000 / 72 = 1000，1ms进一次中断 *///10ms最大脉冲72，折算为速度467.69rpm = 位置环输出最大值 = 速度环输入最大值
//	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;
//	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
//	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
//	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
//	
//	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
//	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	TIM_Cmd(TIM1, ENABLE);
//}

///*
//void TIM1_UP_IRQHandler(void)
//{
//	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
//	{
//		
//		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
//	}
//}
//*/
