#include "Encoder.h"

_MotorPulse motor1, motor2;

short Motor1_Pulse_Unit = 0, Motor2_Pulse_Unit = 0;		//电机10ms脉冲
long Motor1_Pulse_Sigma = 0, Motor2_Pulse_Sigma = 0;	//每次直行或转弯的累计脉冲

//TIM3_CH1(PA6)、TIM3_CH2(PA7)	左轮编码器
//TIM4_CH1(PB6)、TIM4_CH2(PB7)	右轮编码器
void Encoder_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		//左轮编码器接口TIM3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		//右轮编码器接口TIM4
	
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

/*******************实际运行时读取编码器数值************************/
void GetMotorPulse(void)//读取电机脉冲
{
	Motor1_Pulse_Unit = (short)(TIM_GetCounter(TIM3));	//获取计数器值
	Motor1_Pulse_Unit = Motor1_Pulse_Unit;   			//测试下看看符不符合先
	TIM_SetCounter(TIM3, 0);							//TIM4计数器清零
	
	Motor2_Pulse_Unit = (short)(TIM_GetCounter(TIM4));	//获取计数器值
	Motor2_Pulse_Unit = -Motor2_Pulse_Unit;  			//测试下看看符不符合先 
	TIM_SetCounter(TIM4, 0);							//TIM2计数器清零
	
	//为什么只有在两个都取反之后，才能让两个串级PID有用？不然就会让位置环失效
	//暂时不要浪费时间去找原因,直接在内环（速度）的输出值那 数值取反 就好。

	
	Motor1_Pulse_Sigma += Motor1_Pulse_Unit;	//位置外环使用的脉冲累积	//完成一次位置控制之后才清除。
	Motor2_Pulse_Sigma += Motor2_Pulse_Unit;	//位置外环使用的脉冲累积	//记录了之后也要及时清零呀！！！

}
