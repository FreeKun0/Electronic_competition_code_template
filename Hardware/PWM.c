#include "stm32f10x.h"                  // Device header

/*-----------------------------------��̨1xy PWM-----------------------------------------*/
//TTIM5_CH1(PA0) TIM5_CH2(PA1)
void PWM1xy_Init(void)
{
	/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	/* RCC_APB1Periph_TIM3 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	/* RCC_APB2Periph_GPIOA */
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;	/* GPIO_Pin_6 */	/* GPIO_Pin_7 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	/* GPIOA */
	
	/*����ʱ��Դ*/
	TIM_InternalClockConfig(TIM5);	/* TIM3 */
	
	/*ʱ����Ԫ��ʼ��*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;				//�������ڣ���ARR��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;				//Ԥ��Ƶ������PSC��ֵ

	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);	/* TIM3 */
	
	/*����Ƚϳ�ʼ��*/ 
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);	/* TIM_OC1Init */	/* TIM3 */
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);	/* TIM_OC2Init */	/* TIM3 */
	
	/*TIMʹ��*/
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

/*-----------------------------------��̨2xy PWM-----------------------------------------*/
//void PWM2xy_Init(void)
//{
//	/*����ʱ��*/
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	/* RCC_APB1Periph_TIM4 */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	/* RCC_APB2Periph_GPIOB */
//	
//	/*GPIO��ʼ��*/
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;	/* GPIO_Pin_6 */	/* GPIO_Pin_7 */
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	/* GPIOB */
//	
//	/*����ʱ��Դ*/
//	TIM_InternalClockConfig(TIM4);	 /* TIM4 */
//	
//	/*ʱ����Ԫ��ʼ��*/
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;				//�������ڣ���ARR��ֵ
//	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;				//Ԥ��Ƶ������PSC��ֵ

//	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);	 /* TIM4 */
//	
//	/*����Ƚϳ�ʼ��*/ 
//	TIM_OCInitTypeDef TIM_OCInitStructure;
//	TIM_OCStructInit(&TIM_OCInitStructure);
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
//	TIM_OC1Init(TIM4, &TIM_OCInitStructure);	/* TIM_OC1Init */	/* TIM4 */
//	TIM_OC2Init(TIM4, &TIM_OCInitStructure);	/* TIM_OC2Init */	/* TIM4 */
//	
//	/*TIMʹ��*/
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

/*-----------------------------------���AB PWM-----------------------------------------*/
//TIM8_CH1(PC6)��TIM8_CH2(PC7)
void PWMAB_Init(void)
{
	/*����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);		//����PWM TIM8_CH1(PC6) ����PWM TIM8_CH2(PC7)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	/*GPIO��ӳ�䣨������Ҫѡ���Ƿ����ã��˴��Բ�����ӳ��Ϊ����*/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);			//����AFIO��ʱ�ӣ���ӳ����뿪��
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);			//TIM1������ӳ��GPIO_PartialRemap_TIM1,��ȫ��ӳ��GPIO_FullRemap_TIM1
//	// ����ӳ�䵽JTAG������ţ������JTAG����
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/*����ʱ��Դ*/
	TIM_InternalClockConfig(TIM8);
	
	/*ʱ����Ԫ��ʼ��*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 900 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;	//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStructure);
	
	/*����Ƚϳ�ʼ��������TIM1_CH1��*/
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);
	
	/*�߼���ʱ���������ã�ʹ�������*/
	TIM_CtrlPWMOutputs(TIM8, ENABLE);	//TIM1 TIM8��Ϊ�߼���ʱ��������ʹ��������������PWM
	
	/*TIMʹ��*/
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

/*-----------------------------------���CD PWM-----------------------------------------*/
//TIM8_CH3(PC8)��TIM8_CH4(PC9)
void PWMCD_Init(void)
{
	/*����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);		//����PWM TIM8_CH1(PC6) ����PWM TIM8_CH2(PC7)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	/*GPIO��ӳ�䣨������Ҫѡ���Ƿ����ã��˴��Բ�����ӳ��Ϊ����*/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);			//����AFIO��ʱ�ӣ���ӳ����뿪��
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);			//TIM1������ӳ��GPIO_PartialRemap_TIM1,��ȫ��ӳ��GPIO_FullRemap_TIM1
//	// ����ӳ�䵽JTAG������ţ������JTAG����
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/*����ʱ��Դ*/
	TIM_InternalClockConfig(TIM8);
	
	/*ʱ����Ԫ��ʼ��*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 900 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;	//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStructure);
	
	/*����Ƚϳ�ʼ��������TIM1_CH1��*/
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);
	
	/*�߼���ʱ���������ã�ʹ�������*/
	TIM_CtrlPWMOutputs(TIM8, ENABLE);	//TIM1 TIM8��Ϊ�߼���ʱ��������ʹ��������������PWM
	
	/*TIMʹ��*/
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

