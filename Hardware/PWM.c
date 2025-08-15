#include "stm32f10x.h"                  // Device header

/*-----------------------------------云台1xy PWM-----------------------------------------*/
//TTIM5_CH1(PA0) TIM5_CH2(PA1)
void PWM1xy_Init(void)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	/* RCC_APB1Periph_TIM3 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	/* RCC_APB2Periph_GPIOA */
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;	/* GPIO_Pin_6 */	/* GPIO_Pin_7 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	/* GPIOA */
	
	/*配置时钟源*/
	TIM_InternalClockConfig(TIM5);	/* TIM3 */
	
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;				//计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;				//预分频器，即PSC的值

	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);	/* TIM3 */
	
	/*输出比较初始化*/ 
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);	/* TIM_OC1Init */	/* TIM3 */
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);	/* TIM_OC2Init */	/* TIM3 */
	
	/*TIM使能*/
	TIM_Cmd(TIM5, ENABLE);	/* TIM3 */
}

void PWM1x_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM5, Compare);	/* TIM_SetCompare1 */	/* TIM3 */
}

void PWM1y_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM5, Compare);	/* TIM_SetCompare2 */ /* TIM3 */
}

/*-----------------------------------云台2xy PWM-----------------------------------------*/
//void PWM2xy_Init(void)
//{
//	/*开启时钟*/
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	/* RCC_APB1Periph_TIM4 */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	/* RCC_APB2Periph_GPIOB */
//	
//	/*GPIO初始化*/
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;	/* GPIO_Pin_6 */	/* GPIO_Pin_7 */
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	/* GPIOB */
//	
//	/*配置时钟源*/
//	TIM_InternalClockConfig(TIM4);	 /* TIM4 */
//	
//	/*时基单元初始化*/
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;				//计数周期，即ARR的值
//	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;				//预分频器，即PSC的值

//	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);	 /* TIM4 */
//	
//	/*输出比较初始化*/ 
//	TIM_OCInitTypeDef TIM_OCInitStructure;
//	TIM_OCStructInit(&TIM_OCInitStructure);
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
//	TIM_OC1Init(TIM4, &TIM_OCInitStructure);	/* TIM_OC1Init */	/* TIM4 */
//	TIM_OC2Init(TIM4, &TIM_OCInitStructure);	/* TIM_OC2Init */	/* TIM4 */
//	
//	/*TIM使能*/
//	TIM_Cmd(TIM4, ENABLE);	 /* TIM4 */
//}

//void PWM2x_SetCompare1(uint16_t Compare)
//{
//	TIM_SetCompare1(TIM4, Compare);	/* TIM_SetCompare1 */ /* TIM4 */
//}

//void PWM2y_SetCompare2(uint16_t Compare)
//{
//	TIM_SetCompare2(TIM4, Compare);	/* TIM_SetCompare2 */	/* TIM4 */
//}

/*-----------------------------------电机AB PWM-----------------------------------------*/
//TIM8_CH1(PC6)、TIM8_CH2(PC7)
void PWMAB_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);		//左轮PWM TIM8_CH1(PC6) 右轮PWM TIM8_CH2(PC7)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	/*GPIO重映射（根据需要选择是否启用，此处以部分重映射为例）*/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);			//开启AFIO的时钟，重映射必须开启
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);			//TIM1部分重映射GPIO_PartialRemap_TIM1,完全重映射GPIO_FullRemap_TIM1
//	// 若重映射到JTAG相关引脚，需禁用JTAG功能
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/*配置时钟源*/
	TIM_InternalClockConfig(TIM8);
	
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 900 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;	//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStructure);
	
	/*输出比较初始化（配置TIM1_CH1）*/
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);
	
	/*高级定时器特有配置：使能主输出*/
	TIM_CtrlPWMOutputs(TIM8, ENABLE);	//TIM1 TIM8作为高级定时器，必须使能主输出才能输出PWM
	
	/*TIM使能*/
	TIM_Cmd(TIM8, ENABLE);
}

void PWMA_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM8, Compare);
}

void PWMB_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM8, Compare);
}

/*-----------------------------------电机CD PWM-----------------------------------------*/
//TIM8_CH3(PC8)、TIM8_CH4(PC9)
void PWMCD_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);		//左轮PWM TIM8_CH1(PC6) 右轮PWM TIM8_CH2(PC7)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	/*GPIO重映射（根据需要选择是否启用，此处以部分重映射为例）*/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);			//开启AFIO的时钟，重映射必须开启
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);			//TIM1部分重映射GPIO_PartialRemap_TIM1,完全重映射GPIO_FullRemap_TIM1
//	// 若重映射到JTAG相关引脚，需禁用JTAG功能
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/*配置时钟源*/
	TIM_InternalClockConfig(TIM8);
	
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 900 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;	//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStructure);
	
	/*输出比较初始化（配置TIM1_CH1）*/
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);
	
	/*高级定时器特有配置：使能主输出*/
	TIM_CtrlPWMOutputs(TIM8, ENABLE);	//TIM1 TIM8作为高级定时器，必须使能主输出才能输出PWM
	
	/*TIM使能*/
	TIM_Cmd(TIM8, ENABLE);
}
void PWMC_SetCompare3(uint16_t Compare)
{
	TIM_SetCompare3(TIM8, Compare);
}

void PWMD_SetCompare4(uint16_t Compare)
{
	TIM_SetCompare4(TIM8, Compare);
}

