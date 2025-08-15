#include "Encoder.h"

_MotorPulse motor1, motor2;

short Motor1_Pulse_Unit = 0, Motor2_Pulse_Unit = 0;		//���10ms����
long Motor1_Pulse_Sigma = 0, Motor2_Pulse_Sigma = 0;	//ÿ��ֱ�л�ת����ۼ�����

//TIM3_CH1(PA6)��TIM3_CH2(PA7)	���ֱ�����
//TIM4_CH1(PB6)��TIM4_CH2(PB7)	���ֱ�����
void Encoder_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		//���ֱ������ӿ�TIM3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		//���ֱ������ӿ�TIM4
	
	E1A_GPIO_CLK_ENABLE();
	E1B_GPIO_CLK_ENABLE();
	E2A_GPIO_CLK_ENABLE();
	E2B_GPIO_CLK_ENABLE();
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = E1A_GPIO_Pin;
	GPIO_Init(E1A_GPIO_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = E1B_GPIO_Pin;
	GPIO_Init(E1B_GPIO_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = E2A_GPIO_Pin;
	GPIO_Init(E2A_GPIO_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = E2B_GPIO_Pin;
	GPIO_Init(E2B_GPIO_Port, &GPIO_InitStructure);
		
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);
	
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

//int16_t Encoder_1Get(void)
//{
//	int16_t Temp1;
//	Temp1 = TIM_GetCounter(TIM3);
//	TIM_SetCounter(TIM3, 0);
//	return Temp1;
//}
//int16_t Encoder_2Get(void)
//{
//	int16_t Temp2;
//	Temp2 = TIM_GetCounter(TIM4);
//	TIM_SetCounter(TIM4, 0);
//	return Temp2;
//}

/*******************ʵ������ʱ��ȡ��������ֵ************************/
void GetMotorPulse(void)//��ȡ�������
{
	Motor1_Pulse_Unit = (short)(TIM_GetCounter(TIM3));	//��ȡ������ֵ
	Motor1_Pulse_Unit = Motor1_Pulse_Unit;   			//�����¿�������������
	TIM_SetCounter(TIM3, 0);							//TIM4����������
	
	Motor2_Pulse_Unit = (short)(TIM_GetCounter(TIM4));	//��ȡ������ֵ
	Motor2_Pulse_Unit = -Motor2_Pulse_Unit;  			//�����¿������������� 
	TIM_SetCounter(TIM4, 0);							//TIM2����������
	
	//Ϊʲôֻ����������ȡ��֮�󣬲�������������PID���ã���Ȼ�ͻ���λ�û�ʧЧ
	//��ʱ��Ҫ�˷�ʱ��ȥ��ԭ��,ֱ�����ڻ����ٶȣ������ֵ�� ��ֵȡ�� �ͺá�

	
	Motor1_Pulse_Sigma += Motor1_Pulse_Unit;	//λ���⻷ʹ�õ������ۻ�	//���һ��λ�ÿ���֮��������
	Motor2_Pulse_Sigma += Motor2_Pulse_Unit;	//λ���⻷ʹ�õ������ۻ�	//��¼��֮��ҲҪ��ʱ����ѽ������

}
