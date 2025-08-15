#include "gray.h"

uint8_t L2_Val, L1_Val, M_Val, R1_Val, R2_Val;
float Gray_Num;




/**********************************灰度传感器巡线**************************/

void gray_Init(void)
{
	GARY_L2_GPIO_CLK_ENABLE();
	GARY_L1_GPIO_CLK_ENABLE();
	GARY_M_GPIO_CLK_ENABLE();
	GARY_R1_GPIO_CLK_ENABLE();
	GARY_R2_GPIO_CLK_ENABLE();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);			//开启AFIO的时钟，重映射必须先开启AFIO的时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);		//将JTAG引脚失能，PA15 PB3 PB4 作为普通GPIO引脚使用
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//浮空输入
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
  * 函    数：灰度巡线控制
  * 参    数：无
  * 返 回 值：无
  */
//只要把速度环控制好，补偿值即可确定。  不同速度值需要不同的补偿值。  测试好一个最好的。  不同的转速要对应不同的补偿系数或数值
void Gray_GoStraight_control(void)
{
//	static uint8_t stop_count;
	Get_Gray_TTL();  
	
	/* 这里补偿值跟速度环输出的最大值有关  */ //输出pwm限幅为200时，300和350好
	Gray_Num =  0;
	
//	if	   (L1_Val == 1 &&  M_Val == 0 && R1_Val == 0)	Gray_Num = -GRAY_PWM_DA;
//	else if(L1_Val == 1 &&  M_Val == 1 && R1_Val == 0)	Gray_Num = -GRAY_PWM_XIAO;
//	else if(L1_Val == 0 &&  M_Val == 1 && R1_Val == 0)	Gray_Num = 0;	   
//	else if(L1_Val == 0 &&  M_Val == 1 && R1_Val == 1)	Gray_Num = GRAY_PWM_XIAO;
//	else if(L1_Val == 0 &&  M_Val == 0 && R1_Val == 1)	Gray_Num = GRAY_PWM_DA;
//	else if(L1_Val == 1 &&  M_Val == 1 && R1_Val == 1)	Gray_Num = 0;
//	else if(L1_Val == 1 &&  M_Val == 0 && R1_Val == 1)	Gray_Num = 0;
////	else if(L1_Val == 0 &&  M_Val == 0 && R1_Val == 0)	Gray_Num = 0;
	
	/* 1是识别到 */
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
	if(L2_Val == 1 && R2_Val == 1)	//L2 R2 都识别到
	{
//		if(++stop_count >= 5)	//连续五次
//		{
//			stop_count = 0;
////			Beep_ms_TIM(500);
////			motor_disable();
//		}

	}

}
/**
  * 函    数：读取灰度电平，高电平是识别到红线了,M_Val置1，红色指示灯熄灭
  * 参    数：无
  * 返 回 值：无
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
  * 函    数：返回灰度补偿值
  * 参    数：无
  * 返 回 值：补偿值
  */
float Get_Gray_Num()
{
	return Gray_Num;
}

/**
  * 函    数：设置灰度补偿值
  * 参    数：目标灰度补偿值
  * 返 回 值：无
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
	
	/* 这里补偿值跟速度环输出的最大值有关  */ //输出pwm限幅为200时，300和350好
	weight_sum =  0;
	for(int i = 0; i <= 8; i++)
	{
		/* gw_val[i]在线上是1 */
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




