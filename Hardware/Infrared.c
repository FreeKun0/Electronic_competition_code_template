#include "Infrared.h"


// 全局变量
uint8_t  IR_Data[4] = {0};     // 地址码+地址反码+数据码+数据反码（共32位）
uint8_t  IR_Addr = 0;          // 地址码
uint8_t  IR_AddrRev = 0;       // 地址反码
uint8_t  IR_Cmd = 0;           // 数据码
uint8_t  IR_CmdRev = 0;        // 数据反码
uint8_t  IR_DataReady = 0;     // 接收成功标志位
uint8_t  IR_Repeat = 0;        // 重置状态
uint32_t IR_PreTime = 0;       // 上次捕获时间
uint8_t  IR_State = 0;         // 状态标志位
uint8_t  IR_BitCount = 0;      // 位计数器

void Infrared_Init(void)
{
	TIM1_Init(72-1,65536-1);
}


//void TIM3_Init(uint32_t PSC,uint32_t ARR)
//{
//	
//	// 1. 使能时钟
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);		/* RCC_APB1Periph_TIM3 */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	/* RCC_APB2Periph_GPIOB */
//	
//	// 2. 配置GPIO
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	/* GPIO_Pin_1 */
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);		/* GPIOB */
//	
//	// 3. 配置TIM3时基
//	TIM_InternalClockConfig(TIM3);	/* TIM3 */
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInitStructure.TIM_Period = ARR;
//	TIM_TimeBaseInitStructure.TIM_Prescaler = PSC;
//	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);	/* TIM3 */
//	
//	TIM_ClearFlag(TIM3,TIM_FLAG_Update);	/* TIM3 */  // 清除更新标志位（避免初始状态触发中断）
//	
//	// 4. 配置输入捕获
//	TIM_ICInitTypeDef TIM_ICInitStructure;
//	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;	/* TIM_Channel_4 */
//	TIM_ICInitStructure.TIM_ICFilter = 0x3;
//	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
//	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//	TIM_ICInit(TIM3,&TIM_ICInitStructure);	/* TIM3 */
//	
//	// 5. 配置中断（捕获事件触发中断）
//	TIM_ITConfig(TIM3,TIM_IT_CC4,ENABLE);	/* TIM3 */	/* TIM_IT_CC4 */
//	
//	// 6. 配置NVIC
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	/* TIM3_IRQn */
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	// 7. 启动TIM3
//	TIM_Cmd(TIM3,ENABLE);	/* TIM3 */
//}


//void TIM3_IRQHandler(void)	/* TIM3_IRQHandler */
//{
//    if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)		/* TIM3 */	/* TIM_IT_CC4 */
//    {

//        TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);		/* TIM3 */	/* TIM_IT_CC4 */
//        uint32_t current_time = TIM_GetCapture4(TIM3);	/* TIM_GetCapture4 */	/* TIM3 */
//        uint32_t elapsed_time;
//        
//        // 计算时间差  elapsed_time
//        if (current_time >= IR_PreTime)	// 未溢出：当前值 - 上次值
//            elapsed_time = current_time - IR_PreTime;
//        else							// 溢出：(最大值-上次值) + 当前值 + 1
//            elapsed_time = 0xFFFF - IR_PreTime + current_time + 1;
//        IR_PreTime = current_time;
//        
//        // 处理捕获到的时间
////        IR_Decode(elapsed_time);
//		IR_Falling_Decode(elapsed_time);
//    }
//}

//TIM1_CH1 PA8
void TIM1_Init(uint32_t PSC,uint32_t ARR)
{
	
	// 1. 使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);		/* RCC_APB1Periph_TIM3 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	/* RCC_APB2Periph_GPIOB */
	
	// 2. 配置GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	/* GPIO_Pin_1 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);		/* GPIOB */
	
	// 3. 配置TIM3时基
	TIM_InternalClockConfig(TIM1);	/* TIM3 */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = ARR;
	TIM_TimeBaseInitStructure.TIM_Prescaler = PSC;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);	/* TIM3 */
	
	
	
	// 4. 配置输入捕获
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;	/* TIM_Channel_4 */
	TIM_ICInitStructure.TIM_ICFilter = 0x3;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM1,&TIM_ICInitStructure);	/* TIM3 */
	

//	TIM_CtrlPWMOutputs(TIM1, ENABLE);		//TIM1 TIM8作为高级定时器，必须使能主输出才能输出PWM

	// 5. 配置中断（捕获事件触发中断）
	TIM_ITConfig(TIM1,TIM_IT_CC1,ENABLE);	/* TIM3 */	/* TIM_IT_CC4 */
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);	/* TIM3 */  // 清除更新标志位（避免初始状态触发中断）
	
	// 6. 配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;	/* TIM3_IRQn */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	// 7. 启动TIM3
	TIM_Cmd(TIM1,ENABLE);	/* TIM3 */
}


void TIM1_CC_IRQHandler(void)	/* TIM3_IRQHandler */
{
    if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)		/* TIM3 */	/* TIM_IT_CC4 */
    {

        TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);		/* TIM3 */	/* TIM_IT_CC4 */
        uint32_t current_time = TIM_GetCapture1(TIM1);	/* TIM_GetCapture4 */	/* TIM3 */
        uint32_t elapsed_time;
        
        // 计算时间差  elapsed_time
        if (current_time >= IR_PreTime)	// 未溢出：当前值 - 上次值
            elapsed_time = current_time - IR_PreTime;
        else							// 溢出：(最大值-上次值) + 当前值 + 1
            elapsed_time = 0xFFFF - IR_PreTime + current_time + 1;
        IR_PreTime = current_time;
        
        // 处理捕获到的时间
//        IR_Decode(elapsed_time);
		IR_Falling_Decode(elapsed_time);
    }
}

// 红外解码
void IR_Decode(uint32_t time)
{
    switch(IR_State)
    {
        case 0:  // 等待引导码
            if (time > (NEC_HDR_MARK - 1000) && time < (NEC_HDR_MARK + 1000))
			{
//				Buzzer=1;
                IR_State = 1;  // 检测到引导码高电平
			}
            break;
            
        case 1:  // 等待引导码低电平
            if (time > (NEC_HDR_SPACE - 1000) && time < (NEC_HDR_SPACE + 1000))
                IR_State = 2;  // 检测到引导码低电平，进入数据接收模式
            else if (time > (NEC_RPT_SPACE - 500) && time < (NEC_RPT_SPACE + 500))
            {
                IR_Repeat = 1;  // 检测到重复码
                IR_State = 0;   // 回到初始状态
            }
            else
                IR_State = 0;   // 回到初始状态
            break;
            
        case 2:  // 接收数据
            // 
            if (time > (NEC_ONE_SPACE - 500) && time < (NEC_ONE_SPACE + 500))
            {
                // 逻辑1
                IR_Data[IR_BitCount/8] |= (1 << (IR_BitCount%8));
            }
            else if (time > (NEC_ZERO_SPACE - 200) && time < (NEC_ZERO_SPACE + 200))
            {
                // 逻辑0
                IR_Data[IR_BitCount/8] &= ~(1 << (IR_BitCount%8));
            }
            else
            {
                // 无效电平时间，重置解码状态
                IR_State = 0;
                IR_BitCount = 0;
                return;
            }
            
            IR_BitCount++;
            
            if (IR_BitCount >= 32)  // 32位数据接收完成
            {
                IR_Addr = IR_Data[0];
                IR_AddrRev = IR_Data[1];
                IR_Cmd = IR_Data[2];
                IR_CmdRev = IR_Data[3];
                if ((IR_Addr + IR_AddrRev == 0xFF) && (IR_Cmd + IR_CmdRev == 0xFF))
				{
					IR_DataReady = 1;  // 标记数据已准备好
				}
				IR_State = 0;      // 重置状态
				IR_BitCount = 0;   // 重置位计数器
            }
			else
            {
                // 无效电平，重置状态
                IR_State = 0;
                IR_BitCount = 0;
            }
            break;
    }
}

// 红外解码（下降沿捕获模式）
// 参数time：两次下降沿的时间间隔（us）
void IR_Falling_Decode(uint32_t time)
{
    switch(IR_State)
    {
        case 0:  // 状态0：等待引导码（第一次下降沿）
            // 引导码总时长为13500us（9000高+4500低），检测到则进入数据接收状态
            if (time > (NEC_HDR_PULSE - 2000) && time < (NEC_HDR_PULSE + 2000))
            {
                IR_State = 1;  // 进入状态1，接收32位数据
            }
            break;
            
        case 1:  // 状态1：接收32位数据（每次下降沿对应1位数据）
            // 判断时间间隔对应的逻辑值（下降沿间隔=高电平+低电平总时长）
            if (time > (NEC_ONE_PULSE - 500) && time < (NEC_ONE_PULSE + 500))
            {
                // 间隔2250us左右，判定为逻辑1
                IR_Data[IR_BitCount/8] |= (1 << (IR_BitCount%8));
            }
            else if (time > (NEC_ZERO_PULSE - 300) && time < (NEC_ZERO_PULSE + 300))
            {
                // 间隔1120us左右，判定为逻辑0
                IR_Data[IR_BitCount/8] &= ~(1 << (IR_BitCount%8));
            }
            else if (time > (NEC_RPT_PULSE - 2000) && time < (NEC_RPT_PULSE + 2000))
            {
                // 间隔11750us左右，判定为重复码
                IR_Repeat = 1;
                IR_State = 0;  // 重置状态
                return;
            }
            else
            {
                // 无效时间，重置状态
                IR_State = 0;
                IR_BitCount = 0;
                return;
            }
            
            IR_BitCount++;  // 接收位数+1
            
            if (IR_BitCount >= 32)  // 32位数据接收完成
            {
                // 提取并校验数据（同双边沿模式）
                IR_Addr = IR_Data[0];
                IR_AddrRev = IR_Data[1];
                IR_Cmd = IR_Data[2];
                IR_CmdRev = IR_Data[3];
                
                if ((IR_Addr + IR_AddrRev == 0xFF) && (IR_Cmd + IR_CmdRev == 0xFF))
                {
                    IR_DataReady = 1;  // 数据有效，置位就绪标志
                }
                
                IR_State = 0;      // 重置状态
                IR_BitCount = 0;   // 重置位计数器
            }
            break;
    }
}

uint8_t IR_GetNum(void)
{
	uint8_t Temp;
	if (IR_DataReady)
	{
		IR_DataReady = 0;
		if(IR_Cmd)
		{
			Temp = IR_Cmd;
			IR_Cmd = 0;
			return Temp;
		}
	}
	return 0;
}



