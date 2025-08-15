#include "Infrared.h"


// ȫ�ֱ���
uint8_t  IR_Data[4] = {0};     // ��ַ��+��ַ����+������+���ݷ��루��32λ��
uint8_t  IR_Addr = 0;          // ��ַ��
uint8_t  IR_AddrRev = 0;       // ��ַ����
uint8_t  IR_Cmd = 0;           // ������
uint8_t  IR_CmdRev = 0;        // ���ݷ���
uint8_t  IR_DataReady = 0;     // ���ճɹ���־λ
uint8_t  IR_Repeat = 0;        // ����״̬
uint32_t IR_PreTime = 0;       // �ϴβ���ʱ��
uint8_t  IR_State = 0;         // ״̬��־λ
uint8_t  IR_BitCount = 0;      // λ������

void Infrared_Init(void)
{
	TIM1_Init(72-1,65536-1);
}


//void TIM3_Init(uint32_t PSC,uint32_t ARR)
//{
//	
//	// 1. ʹ��ʱ��
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);		/* RCC_APB1Periph_TIM3 */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	/* RCC_APB2Periph_GPIOB */
//	
//	// 2. ����GPIO
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	/* GPIO_Pin_1 */
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);		/* GPIOB */
//	
//	// 3. ����TIM3ʱ��
//	TIM_InternalClockConfig(TIM3);	/* TIM3 */
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInitStructure.TIM_Period = ARR;
//	TIM_TimeBaseInitStructure.TIM_Prescaler = PSC;
//	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);	/* TIM3 */
//	
//	TIM_ClearFlag(TIM3,TIM_FLAG_Update);	/* TIM3 */  // ������±�־λ�������ʼ״̬�����жϣ�
//	
//	// 4. �������벶��
//	TIM_ICInitTypeDef TIM_ICInitStructure;
//	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;	/* TIM_Channel_4 */
//	TIM_ICInitStructure.TIM_ICFilter = 0x3;
//	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
//	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//	TIM_ICInit(TIM3,&TIM_ICInitStructure);	/* TIM3 */
//	
//	// 5. �����жϣ������¼������жϣ�
//	TIM_ITConfig(TIM3,TIM_IT_CC4,ENABLE);	/* TIM3 */	/* TIM_IT_CC4 */
//	
//	// 6. ����NVIC
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	/* TIM3_IRQn */
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	// 7. ����TIM3
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
//        // ����ʱ���  elapsed_time
//        if (current_time >= IR_PreTime)	// δ�������ǰֵ - �ϴ�ֵ
//            elapsed_time = current_time - IR_PreTime;
//        else							// �����(���ֵ-�ϴ�ֵ) + ��ǰֵ + 1
//            elapsed_time = 0xFFFF - IR_PreTime + current_time + 1;
//        IR_PreTime = current_time;
//        
//        // �����񵽵�ʱ��
////        IR_Decode(elapsed_time);
//		IR_Falling_Decode(elapsed_time);
//    }
//}

//TIM1_CH1 PA8
void TIM1_Init(uint32_t PSC,uint32_t ARR)
{
	
	// 1. ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);		/* RCC_APB1Periph_TIM3 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	/* RCC_APB2Periph_GPIOB */
	
	// 2. ����GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	/* GPIO_Pin_1 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);		/* GPIOB */
	
	// 3. ����TIM3ʱ��
	TIM_InternalClockConfig(TIM1);	/* TIM3 */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = ARR;
	TIM_TimeBaseInitStructure.TIM_Prescaler = PSC;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);	/* TIM3 */
	
	
	
	// 4. �������벶��
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;	/* TIM_Channel_4 */
	TIM_ICInitStructure.TIM_ICFilter = 0x3;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM1,&TIM_ICInitStructure);	/* TIM3 */
	

//	TIM_CtrlPWMOutputs(TIM1, ENABLE);		//TIM1 TIM8��Ϊ�߼���ʱ��������ʹ��������������PWM

	// 5. �����жϣ������¼������жϣ�
	TIM_ITConfig(TIM1,TIM_IT_CC1,ENABLE);	/* TIM3 */	/* TIM_IT_CC4 */
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);	/* TIM3 */  // ������±�־λ�������ʼ״̬�����жϣ�
	
	// 6. ����NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;	/* TIM3_IRQn */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	// 7. ����TIM3
	TIM_Cmd(TIM1,ENABLE);	/* TIM3 */
}


void TIM1_CC_IRQHandler(void)	/* TIM3_IRQHandler */
{
    if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)		/* TIM3 */	/* TIM_IT_CC4 */
    {

        TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);		/* TIM3 */	/* TIM_IT_CC4 */
        uint32_t current_time = TIM_GetCapture1(TIM1);	/* TIM_GetCapture4 */	/* TIM3 */
        uint32_t elapsed_time;
        
        // ����ʱ���  elapsed_time
        if (current_time >= IR_PreTime)	// δ�������ǰֵ - �ϴ�ֵ
            elapsed_time = current_time - IR_PreTime;
        else							// �����(���ֵ-�ϴ�ֵ) + ��ǰֵ + 1
            elapsed_time = 0xFFFF - IR_PreTime + current_time + 1;
        IR_PreTime = current_time;
        
        // �����񵽵�ʱ��
//        IR_Decode(elapsed_time);
		IR_Falling_Decode(elapsed_time);
    }
}

// �������
void IR_Decode(uint32_t time)
{
    switch(IR_State)
    {
        case 0:  // �ȴ�������
            if (time > (NEC_HDR_MARK - 1000) && time < (NEC_HDR_MARK + 1000))
			{
//				Buzzer=1;
                IR_State = 1;  // ��⵽������ߵ�ƽ
			}
            break;
            
        case 1:  // �ȴ�������͵�ƽ
            if (time > (NEC_HDR_SPACE - 1000) && time < (NEC_HDR_SPACE + 1000))
                IR_State = 2;  // ��⵽������͵�ƽ���������ݽ���ģʽ
            else if (time > (NEC_RPT_SPACE - 500) && time < (NEC_RPT_SPACE + 500))
            {
                IR_Repeat = 1;  // ��⵽�ظ���
                IR_State = 0;   // �ص���ʼ״̬
            }
            else
                IR_State = 0;   // �ص���ʼ״̬
            break;
            
        case 2:  // ��������
            // 
            if (time > (NEC_ONE_SPACE - 500) && time < (NEC_ONE_SPACE + 500))
            {
                // �߼�1
                IR_Data[IR_BitCount/8] |= (1 << (IR_BitCount%8));
            }
            else if (time > (NEC_ZERO_SPACE - 200) && time < (NEC_ZERO_SPACE + 200))
            {
                // �߼�0
                IR_Data[IR_BitCount/8] &= ~(1 << (IR_BitCount%8));
            }
            else
            {
                // ��Ч��ƽʱ�䣬���ý���״̬��
                IR_State = 0;
                IR_BitCount = 0;
                return;
            }
            
            IR_BitCount++;
            
            if (IR_BitCount >= 32)  // 32λ���ݽ������
            {
                IR_Addr = IR_Data[0];
                IR_AddrRev = IR_Data[1];
                IR_Cmd = IR_Data[2];
                IR_CmdRev = IR_Data[3];
                if ((IR_Addr + IR_AddrRev == 0xFF) && (IR_Cmd + IR_CmdRev == 0xFF))
				{
					IR_DataReady = 1;  // ���������׼����
				}
				IR_State = 0;      // ����״̬
				IR_BitCount = 0;   // ����λ������
            }
			else
            {
                // ��Ч��ƽ������״̬
                IR_State = 0;
                IR_BitCount = 0;
            }
            break;
    }
}

// ������루�½��ز���ģʽ��
// ����time�������½��ص�ʱ������us��
void IR_Falling_Decode(uint32_t time)
{
    switch(IR_State)
    {
        case 0:  // ״̬0���ȴ������루��һ���½��أ�
            // ��������ʱ��Ϊ13500us��9000��+4500�ͣ�����⵽��������ݽ���״̬
            if (time > (NEC_HDR_PULSE - 2000) && time < (NEC_HDR_PULSE + 2000))
            {
                IR_State = 1;  // ����״̬1������32λ����
            }
            break;
            
        case 1:  // ״̬1������32λ���ݣ�ÿ���½��ض�Ӧ1λ���ݣ�
            // �ж�ʱ������Ӧ���߼�ֵ���½��ؼ��=�ߵ�ƽ+�͵�ƽ��ʱ����
            if (time > (NEC_ONE_PULSE - 500) && time < (NEC_ONE_PULSE + 500))
            {
                // ���2250us���ң��ж�Ϊ�߼�1
                IR_Data[IR_BitCount/8] |= (1 << (IR_BitCount%8));
            }
            else if (time > (NEC_ZERO_PULSE - 300) && time < (NEC_ZERO_PULSE + 300))
            {
                // ���1120us���ң��ж�Ϊ�߼�0
                IR_Data[IR_BitCount/8] &= ~(1 << (IR_BitCount%8));
            }
            else if (time > (NEC_RPT_PULSE - 2000) && time < (NEC_RPT_PULSE + 2000))
            {
                // ���11750us���ң��ж�Ϊ�ظ���
                IR_Repeat = 1;
                IR_State = 0;  // ����״̬
                return;
            }
            else
            {
                // ��Чʱ�䣬����״̬
                IR_State = 0;
                IR_BitCount = 0;
                return;
            }
            
            IR_BitCount++;  // ����λ��+1
            
            if (IR_BitCount >= 32)  // 32λ���ݽ������
            {
                // ��ȡ��У�����ݣ�ͬ˫����ģʽ��
                IR_Addr = IR_Data[0];
                IR_AddrRev = IR_Data[1];
                IR_Cmd = IR_Data[2];
                IR_CmdRev = IR_Data[3];
                
                if ((IR_Addr + IR_AddrRev == 0xFF) && (IR_Cmd + IR_CmdRev == 0xFF))
                {
                    IR_DataReady = 1;  // ������Ч����λ������־
                }
                
                IR_State = 0;      // ����״̬
                IR_BitCount = 0;   // ����λ������
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



