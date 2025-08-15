#include "gray.h"

uint8_t L2_Val, L1_Val, M_Val, R1_Val, R2_Val;
float Gray_Num;




/**********************************�Ҷȴ�����Ѳ��**************************/

void gray_Init(void)
{
	GARY_L2_GPIO_CLK_ENABLE();
	GARY_L1_GPIO_CLK_ENABLE();
	GARY_M_GPIO_CLK_ENABLE();
	GARY_R1_GPIO_CLK_ENABLE();
	GARY_R2_GPIO_CLK_ENABLE();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);			//����AFIO��ʱ�ӣ���ӳ������ȿ���AFIO��ʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);		//��JTAG����ʧ�ܣ�PA15 PB3 PB4 ��Ϊ��ͨGPIO����ʹ��
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GARY_L2_GPIO_Pin;
	GPIO_Init(GARY_L2_GPIO_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GARY_L1_GPIO_Pin;
	GPIO_Init(GARY_L1_GPIO_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GARY_M_GPIO_Pin;
	GPIO_Init(GARY_M_GPIO_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GARY_R1_GPIO_Pin;
	GPIO_Init(GARY_R1_GPIO_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GARY_R2_GPIO_Pin;
	GPIO_Init(GARY_R2_GPIO_Port, &GPIO_InitStructure);
}


/**
  * ��    �����Ҷ�Ѳ�߿���
  * ��    ������
  * �� �� ֵ����
  */
//ֻҪ���ٶȻ����ƺã�����ֵ����ȷ����  ��ͬ�ٶ�ֵ��Ҫ��ͬ�Ĳ���ֵ��  ���Ժ�һ����õġ�  ��ͬ��ת��Ҫ��Ӧ��ͬ�Ĳ���ϵ������ֵ
void Gray_GoStraight_control(void)
{
//	static uint8_t stop_count;
	Get_Gray_TTL();  
	
	/* ���ﲹ��ֵ���ٶȻ���������ֵ�й�  */ //���pwm�޷�Ϊ200ʱ��300��350��
	Gray_Num =  0;
	
//	if	   (L1_Val == 1 &&  M_Val == 0 && R1_Val == 0)	Gray_Num = -GRAY_PWM_DA;
//	else if(L1_Val == 1 &&  M_Val == 1 && R1_Val == 0)	Gray_Num = -GRAY_PWM_XIAO;
//	else if(L1_Val == 0 &&  M_Val == 1 && R1_Val == 0)	Gray_Num = 0;	   
//	else if(L1_Val == 0 &&  M_Val == 1 && R1_Val == 1)	Gray_Num = GRAY_PWM_XIAO;
//	else if(L1_Val == 0 &&  M_Val == 0 && R1_Val == 1)	Gray_Num = GRAY_PWM_DA;
//	else if(L1_Val == 1 &&  M_Val == 1 && R1_Val == 1)	Gray_Num = 0;
//	else if(L1_Val == 1 &&  M_Val == 0 && R1_Val == 1)	Gray_Num = 0;
////	else if(L1_Val == 0 &&  M_Val == 0 && R1_Val == 0)	Gray_Num = 0;
	
	/* 1��ʶ�� */
	if	   (L1_Val == 1 &&  M_Val == 0 && R1_Val == 0)	Gray_Num = -GRAY_PWM_DA;
	else if(L1_Val == 1 &&  M_Val == 1 && R1_Val == 0)	Gray_Num = -GRAY_PWM_XIAO;
	else if(L1_Val == 0 &&  M_Val == 1 && R1_Val == 0)	Gray_Num = 0;	   
	else if(L1_Val == 0 &&  M_Val == 1 && R1_Val == 1)	Gray_Num = GRAY_PWM_XIAO;
	else if(L1_Val == 0 &&  M_Val == 0 && R1_Val == 1)	Gray_Num = GRAY_PWM_DA;
	else if(L1_Val == 1 &&  M_Val == 1 && R1_Val == 1)	Gray_Num = 0;
	else if(L1_Val == 1 &&  M_Val == 0 && R1_Val == 1)	Gray_Num = 0;
//	else if(L1_Val == 0 &&  M_Val == 0 && R1_Val == 0)	Gray_Num = 0;

	if(L2_Val == 1 && R2_Val == 0)
	{
		Gray_Num = -GRAY_PWM_DA_DA;
	}
	else if(L2_Val == 0 && R2_Val == 1)
	{
		Gray_Num = GRAY_PWM_DA_DA;
	}
	if(L2_Val == 1 && R2_Val == 1)	//L2 R2 ��ʶ��
	{
//		if(++stop_count >= 5)	//�������
//		{
//			stop_count = 0;
////			Beep_ms_TIM(500);
////			motor_disable();
//		}

	}

}
/**
  * ��    ������ȡ�Ҷȵ�ƽ���ߵ�ƽ��ʶ�𵽺�����,M_Val��1����ɫָʾ��Ϩ��
  * ��    ������
  * �� �� ֵ����
  */
void Get_Gray_TTL(void)
{
	if(GPIO_ReadInputDataBit(GARY_L2_GPIO_Port, GARY_L2_GPIO_Pin) == Bit_SET){L2_Val = 1;} else {L2_Val = 0;}
	if(GPIO_ReadInputDataBit(GARY_L1_GPIO_Port, GARY_L1_GPIO_Pin) == Bit_SET){L1_Val = 1;} else {L1_Val = 0;}
	if(GPIO_ReadInputDataBit(GARY_M_GPIO_Port,  GARY_M_GPIO_Pin ) == Bit_SET){M_Val  = 1;} else {M_Val  = 0;}
	if(GPIO_ReadInputDataBit(GARY_R1_GPIO_Port, GARY_R1_GPIO_Pin) == Bit_SET){R1_Val = 1;} else {R1_Val = 0;}
	if(GPIO_ReadInputDataBit(GARY_R2_GPIO_Port, GARY_R2_GPIO_Pin) == Bit_SET){R2_Val = 1;} else {R2_Val = 0;}
//	L2_Val = GPIO_ReadInputDataBit(GARY_L2_GPIO_Port, GARY_L2_GPIO_Pin) == Bit_SET;
//	L1_Val = GPIO_ReadInputDataBit(GARY_L1_GPIO_Port, GARY_L1_GPIO_Pin) == Bit_SET;
//	M_Val  = GPIO_ReadInputDataBit(GARY_M_GPIO_Port,  GARY_M_GPIO_Pin ) == Bit_SET;
//	R1_Val = GPIO_ReadInputDataBit(GARY_R1_GPIO_Port, GARY_R1_GPIO_Pin) == Bit_SET;
//	R2_Val = GPIO_ReadInputDataBit(GARY_R2_GPIO_Port, GARY_R2_GPIO_Pin) == Bit_SET;
	
}
/**
  * ��    �������ػҶȲ���ֵ
  * ��    ������
  * �� �� ֵ������ֵ
  */
float Get_Gray_Num()
{
	return Gray_Num;
}

/**
  * ��    �������ûҶȲ���ֵ
  * ��    ����Ŀ��ҶȲ���ֵ
  * �� �� ֵ����
  */
void Set_Gray_Num(float set_num)
{
	Gray_Num = set_num;
}



uint8_t gw_val[8];
uint16_t weights[8] = {2, 4, 8, 16, 32, 64, 128, 256};
uint16_t weight_sum;
uint16_t gw_out_pwm[8] = {0, 100, 200, 300, 400, 500, 600, 700};

void GW_Gray_GoStraight_control(void)
{
	GWL4 = GPIO_ReadInputDataBit(GARY_L2_GPIO_Port, GARY_L2_GPIO_Pin) == Bit_SET;
	GWL3 = GPIO_ReadInputDataBit(GARY_L1_GPIO_Port, GARY_L1_GPIO_Pin) == Bit_SET;
	GWL2 = GPIO_ReadInputDataBit(GARY_M_GPIO_Port,  GARY_M_GPIO_Pin ) == Bit_SET;
	GWL1 = GPIO_ReadInputDataBit(GARY_R1_GPIO_Port, GARY_R1_GPIO_Pin) == Bit_SET;
	GWR1 = GPIO_ReadInputDataBit(GARY_R2_GPIO_Port, GARY_R2_GPIO_Pin) == Bit_SET;
	
//	static uint8_t stop_count;
//	Get_Gray_TTL();  
	Gray_Num =  0;
	
	/* ���ﲹ��ֵ���ٶȻ���������ֵ�й�  */ //���pwm�޷�Ϊ200ʱ��300��350��
	weight_sum =  0;
	for(int i = 0; i <= 8; i++)
	{
		/* gw_val[i]��������1 */
		weight_sum += gw_val[i] * weights[i];
	}
	switch(weight_sum)
	{
		case 2:				//L4
			Gray_Num =-gw_out_pwm[7];
		break;
		case 6:
			Gray_Num =-gw_out_pwm[6];
		break;
		case 4:case 14:		//L3
			Gray_Num =-gw_out_pwm[5];
		break;
		case 12:
			Gray_Num =-gw_out_pwm[4];
		break;
		case 8:case 28:		//L2
			Gray_Num =-gw_out_pwm[3];
		break;
		case 24:
			Gray_Num =-gw_out_pwm[2];
		break;
		case 16:case 56:	//L1
			Gray_Num =-gw_out_pwm[1];
		break;
		case 48:case 0:		//M
			Gray_Num = gw_out_pwm[0];
		break;
		case 32:case 112:	//R1
			Gray_Num = gw_out_pwm[1];
		break;
		case 96:
			Gray_Num = gw_out_pwm[2];
		break;
		case 64:case 224:	//R2
			Gray_Num = gw_out_pwm[3];
		break;
		case 192:
			Gray_Num = gw_out_pwm[4];
		break;
		case 128:case 448:	//R3
			Gray_Num = gw_out_pwm[5];
		break;
		case 384:
			Gray_Num = gw_out_pwm[6];
		break;
		case 256:			//R4
			Gray_Num = gw_out_pwm[7];
		break;
		
		default:
			break;
	}

}




