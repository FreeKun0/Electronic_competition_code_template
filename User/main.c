#include "parameter.h"
#include "Delay.h"
#include "bsp_pid.h"
#include <math.h>

#include "Serial.h"
#include "USART2.h"
#include "k210.h"
#include "UART4.h"
#include "UART5.h"

#include "Timer.h"

#include "OLED.h"
#include "LED.h"
#include "Beep.h"
#include "Key.h"
#include "Keyboard.h"
#include "gray.h"
#include "huidu8.h"
#include "AD.h"
#include "YT.h"
#include "laser.h"

#include "control.h"
#include "Servo.h"
#include "Motor.h"
#include "Encoder.h"
#include "Infrared.h"
#include "mpu6050.h"
#include "HWT101.h"

#include "stepper.h"
/*-------------------------------------宏定义-------------------------------------------*/


/*-------------------------------------函数声明-------------------------------------------*/

void Remap_Init(void);
void Key_Handling(void);
void KeyBoard_Handling(void);
void Infrared_Handling(void);
void USART2_Handling(void);
void OLED_Show(void);

	
/*-------------------------------------变量定义-------------------------------------------*/

uint8_t a;
uint8_t a1;
uint8_t a2;
uint8_t a3;
uint8_t a4;
uint8_t a5;

int16_t PWM_test;
float tar_cm;
	
/*-------------------------------------主函数-------------------------------------------*/
int main(void)
{
	/*参数初始化*/
	tar_cm = 9999;
	TASK = 0;
	pid_param_init();

	/* 重映射 */
	Remap_Init();		//将JTAG引脚失能,PA15 PB3 PB4 作为普通GPIO引脚使用
	
	/*模块初始化*/
//	Laser_P_Init();		//PB3
	OLED_Init();		//SCL PB8	SDA PB9
//	MPU_Init();			//SCL PB0	SDA PB1
//	HWT101_Init();		//SCL PB0	SDA PB1
	LED_Init();			//PA4 PC8 PC9 PC13 PC3			只开启了前三个		LED4、5与L2、R2冲突
	Key_Init();			//PA11 PA12 PB4 PB5
//	Keyboard_Init();	//PA11 PA12 PB4 PB5 PA0 PA1 PA15 PB3				KEY5、6与舵机PWM
	Beep_Init();		//PC5	(PA15)										CH15/PC5	KEY7/PA15
	
//	gray_Init();		//PC13 PC0 PC1 PC2 PC3				都用了			L2、R2与LED4、LED5冲突
//	huidu8_Init();		//SCL PC0	SDA PC1
	
//	AD_Init();			//ADC1_CH14 PC4	ADC1_CH15 PC5	只用了CH14			CH15/BEEP

//	Motor_Init();		//TIM8_CH1(PC6) TIM8_CH2(PC7)	A轮PWM、B轮PWM		
						//PA5 PB12 PB13 PB14 PB15		STBY 电机A B方向
//	Encoder_Init();		//TIM3_CH1(PA6) TIM3_CH2(PA7)	A轮编码器
						//TIM4_CH1(PB6) TIM4_CH2(PB7)	B轮编码器
//	Servo_Init();		//TIM5_CH1(PA0) TIM5_CH2(PA1)	云台舵机1xyPWM		KEY5、6

//	Infrared_Init();	//TIM1_CH1(PA8)

	Stepper_Init();
    Stepper_Enable(STEP_X, 1);
    Delay_ms(100);  // 等待驱动芯片稳定

	Delay_ms(100);
	//这里必须给rotation_red,rotation_green赋值
//	Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[4][0], pencil_points_PWM[4][1]);
//	Servo_All_SetPWM_ChangeRotationRed(1500 , 1500);
	Delay_ms(300);

	/* 串口 */
	Serial_Init(115200);//USART1(PA9  PA10)	//电脑
	USART2_Init(115200);//USART2(PA2  PA3 ) //手机
	USART3_Init(115200);//USART3(PB10 PB11) //视觉
	UART4_Init(115200);	//UART4 (PC10 PC11) //双车
	UART5_Init(115200);	//UART5 (PC12 PD2 ) //串口屏
	
	HMI_Printf("rest");	//串口屏重启
//	K210_SendByte('0');	//K210复位
	Delay_ms(10);
	
	//定时器初始化一定放在最后，防止定时器中断打断其他初始化
	Timer_Init();		//TIM6 定时更新中断

	param.stepx_run_ms = 100;
	param.stepx_stop_ms = 10;
	param.stepx_speed = 10;
	param.stepx_dir = 0;
	
//	Laser_P_OFF();
	while (1)
	{
//        // 1. 正向转动
//        Stepper_SetDir(STEP_X, param.stepx_dir);
//        Stepper_SetSpeed(STEP_X, param.stepx_speed);
//        Stepper_Start(STEP_X);
//		Blue_ON();
//        Delay_ms(param.stepx_run_ms);
//		
//        
//        // 2. 停止
//        Stepper_Stop(STEP_X);
//		Blue_OFF();
//        Delay_ms(param.stepx_stop_ms);
		
		
//		stepper[STEP_X].current_step;
		OLED_Printf(0 ,  0 , OLED_6X8, ":%d   ", stepper[STEP_X].current_step);
		OLED_Update();
		/* OLED显示 */
//		OLED_Show();
//		OLED_Update();

	}
}



void Remap_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);			//开启AFIO的时钟，重映射必须先开启AFIO的时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);		//将JTAG引脚失能，PA15 PB3 PB4 作为普通GPIO引脚使用
}


void Key_Handling(void)
{
	KeyNum = Key_GetNum();
	
	if(KeyNum == KEY1_PRES)
	{
//		if(flag.stop_flag == 0)
//		{
//			flag.stop_flag = 1;
//			
//			last_TASK = TASK;
//			TASK = 0;
//			
//			Beep_ms(250);
//		}
//		else if(flag.stop_flag == 1)
//		{
//			flag.stop_flag = 0;
//			
//			Beep_ms(500);
//			
//			TASK = last_TASK;
//		}
		Laser_P_Turn();
//		Red_Turn();
	}
	else if(KeyNum == KEY2_PRES)
	{
//		ServoLine_Init(rotation_red[0], rotation_red[1], 1355, 1594, 50, 10);
//		ServoLine_Init(1400,1390, 1280, 1550, 50, 10);
		
//		TASK++;
//		if(TASK > 9)	TASK = 1;
//		
//		if(TASK == 1)
//		{
//			Do_count1 = 0;
//		}
//		else if(TASK == 2)
//		{
//			Do_count2 = 0;
//		}
//		else if(TASK == 3)
//		{
//			Do_count3 = 0;
//		}
//		else if(TASK == 4)
//		{
//			Do_count4 = 0;
//		}
//		else if(TASK == 5)
//		{
//			Do_count5 = 0;
//		}
//		else if(TASK == 6)
//		{
//			Do_count6 = 0;
//		}
//		else if(TASK == 7)
//		{
//			Do_count7 = 0;
//		}
//		else if(TASK == 8)
//		{
//			Do_count8 = 0;
//		}
//		else if(TASK == 9)
//		{
//			Do_count9 = 0;
//		}
	}
	else if(KeyNum == KEY3_PRES)
	{
		
//		K210_SendByte(0x55);
		Beep_ms_TIM(1000);
		Red_ms_TIM(1000);
		Yellow_ms_TIM(1000);
		Green_ms_TIM(1000);
		Blue_ms_TIM(1000);
		
	}
	else if(KeyNum == KEY4_PRES)	//软件复位
	{
		__set_FAULTMASK(1); //关闭所有中断
		NVIC_SystemReset(); //复位
	}
	
//	/* 移动云台舵机激光 */
//	if(KeyNum == KEY_UP_PRES)
//	{
//	}
//	else if(KeyNum == KEY_DOWN_PRES)
//	{
//	}
//	else if(KeyNum == KEY_LEFT_PRES)
//	{
//	}
//	else if(KeyNum == KEY_RIGHT_PRES)
//	{
//	}
}


void Infrared_Handling(void)
{
/*	69	70	71
	68	64	67
	7	21	9
	22	25	13
	12	24	94
	8	28	90
	66	82	74	*/
	IR_Num = IR_GetNum();
	switch(IR_Num)
	{
		case 69:
			spin_Turn_YAW(90);
			break;
		case 70:
			spin_Turn_YAW(-90);
			break;
		case 71:
			spin_Turn_YAW(180);
			break;
		case 68:
			spin_Turn_YAW(-180);
			break;
		case 64:
			reset();
			break;
		case 67:
			HWT101_CaliYaw();
			break;
		case 7:
			tar_cm += 10;
			break;
		case 21:
			tar_cm -= 10;
			break;
		case 9:
			Car_go(tar_cm);
			break;
		case 22:
			tar_cm += 60;
			break;
		case 25:
			tar_cm -= 60;
			break;
		case 13:
			break;
		case 12:
			break;
		case 24:
			TASK = 99;
			Do_count1 = 0;
			break;
		case 94:
			break;
		case 8:
			break;
		case 28:
			break;
		case 90:
			break;
		case 66:
			break;
		case 82:
			break;
		case 74:
			break;
		default:
			break;
	}

}
void USART2_Handling(void)
{
	if(USART2_GetRxFlag())
	{
		Usart2Num = USART2_GetRxData();
		switch(Usart2Num)
		{
			/* 默认值是0x00 即使没收到 */
			/* 复位系统 */
			case 0xFF:	
			{
				__set_FAULTMASK(1); //关闭所有中断
				NVIC_SystemReset(); //复位
			}
			break;
			
			case 0x01:	TASK = 1;	Do_count1 = 0;		break;
			case 0x02:	TASK = 2;	Do_count2 = 0;		break;
			case 0x03:	TASK = 3;	Do_count3 = 0;		break;
			case 0x04:	TASK = 4;	Do_count4 = 0;		break;
			case 0x05:	TASK = 5;	Do_count5 = 0;		break;
			case 0x06:	TASK = 6;	Do_count6 = 0;		break;
			case 0x07:	TASK = 7;	Do_count7 = 0;		break;
			case 0x08:	TASK = 8;	Do_count8 = 0;		break;
			case 0x09:	TASK = 9;	Do_count9 = 0;		break;
			case 0x0A:	TASK = 10;	Do_count10 = 0;		break;
			case 0x0B:	TASK = 11;	Do_count11 = 0;		break;
			case 0x0C:	TASK = 12;	Do_count12 = 0;		break;
			
			case 0x10:	//上
			{
				rotation_red[1] -= PWM_step_for_TASK1;
				Servo2_SetPWM(rotation_red[1]);
			}break;
			case 0x11:	//下
			{
				rotation_red[1] += PWM_step_for_TASK1;
				Servo2_SetPWM(rotation_red[1]);
			}break;
			case 0x12:	//左
			{
				rotation_red[0] += PWM_step_for_TASK1;
				Servo1_SetPWM(rotation_red[0]);
			}break;
			case 0x13:	//右
			{
				rotation_red[0] -= PWM_step_for_TASK1;
				Servo1_SetPWM(rotation_red[0]);
			}break;
			case 0x14:
			{
				PWM_step_for_TASK1 = (PWM_step_for_TASK1 == PWM_STEP_LARGE ? PWM_STEP_SMALL : PWM_STEP_LARGE);
			}break;
			case 0x15:
			{
				Laser_P_ON();
			}break;
			case 0x16:
			{
				Laser_P_OFF();
			}break;
			
			case 0x20:
			{
				Line_Init(1500,1450, 1280, 1550, 200, 5);
			}break;
			case 0x21:
			{
				Triangle_Init(1340, 1390, 1280, 1530, 1440, 1470, 100, 5);
			}break;
			case 0x22:
			{
				RotRect_Init(1460, 1550, -70, -70, 45, 50, 5);
			}break;
			case 0x23:
			{
				Circle_Init(1480, 1490, 60, 300, 5);
			}break;
			case 0x24:
			{
				// 定义六边形顶点（按顺序排列）
				float hex_vertices[6][2] = {
					{1595, 1450},   // 顶点0
					{1495, 1451},  // 顶点1
					{1595, 1531},  // 顶点2
					{1535, 1431},  // 顶点3
					{1495, 1531},   // 顶点4
					{1595, 1450}    // 顶点5
				};

				// 初始化多边形：顶点数组、6个顶点、每条边20段、周期5
				Polygon_Init(hex_vertices, 6, 50, 5);
			}break;
			case 0x25:
			{
//				绘制正六边形：中心(1500,1500)，半径500，6条边，倾斜30度，每条边20段，周期3
				RegularPolygon_Init(1500, 1490, 70, 3, 30, 50, 5);

//				绘制正方形（4条边）：中心(1500,1500)，半径400，倾斜45度
//				Shape_Init(SHAPE_REGULAR_POLYGON, 1500.0f, 1500.0f, 400.0f, 4, 45.0f, 15, 3);

//				绘制等边三角形（3条边）：中心(1500,1500)，半径600，无倾斜（0度）
//				Shape_Init(SHAPE_REGULAR_POLYGON, 1500.0f, 1500.0f, 600.0f, 3, 0.0f, 10, 3);
			}break;
			
			
			case 0x30:
			{
				Car_go(90);
			}break;
			case 0x31:
			{
				spin_Turn_YAW(90);
			}break;
			case 0x32:
			{
				spin_Turn_YAW(-90);
			}break;
			case 0x33:
			{
				spin_Turn_YAW(180);
			}break;
			case 0x34:
			{
				spin_Turn_YAW(-180);
			}break;
			
			case 0x99:	TASK = 99;	Do_count1 = 0;		break;
			
//			case 0x0A:	TASK = 1;	Do_count1 = 0;		break;
//			case 0x0B:	TASK = 1;	Do_count1 = 0;		break;
//			case 0x0C:	TASK = 1;	Do_count1 = 0;		break;

			default:	break;
		}
	}
	else
	{
		Usart2Num = 0x00;
	}
}
void OLED_Show(void)
{
/*
			%d十进制		%X十六进制	%c字符
	'1':	49			31			1
	'A': 	65			41			A

	横坐标大于120 或 纵坐标大于56 就显示不完全了
	OLED_6X8 宽6高8
*/
	/*--------------------------显示任务进度--------------------------------*/
	switch(TASK)
	{
		case 1:
			OLED_Printf(0 ,  0 , OLED_6X8, "Do_count1:%d   ", Do_count1);
		break;
		case 2:
			OLED_Printf(0 ,  0 , OLED_6X8, "Do_count2:%d   ", Do_count2);
		break;
		case 3:
			OLED_Printf(0 ,  0 , OLED_6X8, "Do_count3:%d   ", Do_count3);
		break;
		case 4:
			OLED_Printf(0 ,  0 , OLED_6X8, "Do_count4:%d   ", Do_count4);
		break;
		case 5:
			OLED_Printf(0 ,  0 , OLED_6X8, "Do_count5:%d   ", Do_count5);
		break;
		case 6:
			OLED_Printf(0 ,  0 , OLED_6X8, "Do_count6:%d   ", Do_count6);
		break;
		case 7:
			OLED_Printf(0 ,  0 , OLED_6X8, "Do_count7:%d   ", Do_count7);
		break;
		case 8:
			OLED_Printf(0 ,  0 , OLED_6X8, "Do_count8:%d   ", Do_count8);
		break;
		case 9:
			OLED_Printf(0 ,  0 , OLED_6X8, "Do_count9:%d   ", Do_count9);
		break;
		case 10:
			OLED_Printf(0 ,  0 , OLED_6X8, "Do_count10:%d   ", Do_count10);
		break;
		case 11:
			OLED_Printf(0 ,  0 , OLED_6X8, "Do_count11:%d   ", Do_count11);
		break;
		case 12:
			OLED_Printf(0 ,  0 , OLED_6X8, "Do_count912:%d   ", Do_count12);
		break;
	}
	
	
	/*----------------------------一直显示----------------------------------*/
//	OLED_Printf(0 ,  8 , OLED_6X8, ":%d                     ", );

	/*---------------------------对应任务显示--------------------------------*/
	switch(TASK)
	{
		case 1:
//			OLED_Printf(0 ,  16, OLED_6X8, ":%d                     ", );
//			OLED_Printf(0 ,  24, OLED_6X8, ":%d                     ", );
//			OLED_Printf(0 ,  32, OLED_6X8, ":%d                     ", );
//			OLED_Printf(0 ,  40, OLED_6X8, ":%d                     ", );
//			OLED_Printf(0 ,  48, OLED_6X8, ":%d                     ", );
//			OLED_Printf(0 ,  56, OLED_6X8, ":%d                     ", );
			break;
		case 2:
			
			break;
		case 3:
			
			break;
		case 4:
			
			break;
		case 5:
			
			break;
		case 6:
			
			break;
		case 7:
			
			break;
		case 8:
			
			break;
		case 9:
			
			break;
		case 10:
			OLED_Printf(0 ,  16, OLED_6X8, "C_tar  :%3d,%3d      ", C_tar[0], C_tar[1]  );
			OLED_Printf(0 ,  24, OLED_6X8, "C_red  :%3d,%3d      ", C_red[0], C_red[1]);
			OLED_Printf(0 ,  32, OLED_6X8, "error  :%3d,%3d      ", C_tar[0] - C_red[0], C_tar[1] - C_red[1]);
//			OLED_Printf(0 ,  40, OLED_6X8, "time   :%6.2fs       ", count.counting_time / 100.0);
			OLED_Printf(0 ,  40, OLED_6X8, "catch  :%d           ", flag.cur_catch_tar_flag);
			OLED_Printf(0 ,  48, OLED_6X8, "pwm(%04.0f,%04.0f)     ", rotation_red[0], rotation_red[1]);

			
			break;
		case 11:
			
			break;
		case 12:
			
			break;
	}

	
//	OLED_Printf(0 ,  0 , OLED_6X8, ":%d   ", );
//	OLED_Printf(0 ,  8 , OLED_6X8, ":%d   ", );
//	OLED_Printf(0 ,  16, OLED_6X8, ":%d   ", );
//	OLED_Printf(0 ,  24, OLED_6X8, ":%d   ", );
//	OLED_Printf(0 ,  32, OLED_6X8, ":%d   ", );
//	OLED_Printf(0 ,  40, OLED_6X8, ":%d   ", );
//	OLED_Printf(0 ,  48, OLED_6X8, ":%d   ", );
//	OLED_Printf(0 ,  56, OLED_6X8, ":%d   ", );
	                
	
//	OLED_Printf(0 ,  0 , OLED_6X8, "0:%X   ", K210_RxBuffer[0]);
//	OLED_Printf(0 ,  8 , OLED_6X8, "1:%X   ", K210_RxBuffer[1]);
//	OLED_Printf(0 ,  16, OLED_6X8, "2:%X   ", K210_RxBuffer[2]);
//	OLED_Printf(0 ,  24, OLED_6X8, "3:%X   ", K210_RxBuffer[3]);
//	OLED_Printf(0 ,  32, OLED_6X8, "4:%X   ", K210_RxBuffer[4]);
//	OLED_Printf(54,  0 , OLED_6X8, "5:%X   ", K210_RxBuffer[5]);
//	OLED_Printf(54,  8 , OLED_6X8, "6:%X   ", K210_RxBuffer[6]);
//	OLED_Printf(54,  16, OLED_6X8, "7:%X   ", K210_RxBuffer[7]);
//	OLED_Printf(54,  24, OLED_6X8, "8:%X   ", K210_RxBuffer[8]);
//	OLED_Printf(54,  32, OLED_6X8, "9:%X   ", K210_RxBuffer[9]);


	
//	OLED_Printf(0 ,  0 , OLED_6X8, ":%d   ", );
//	OLED_Printf(0 ,  8 , OLED_6X8, ":%d   ", );
//	OLED_Printf(0 ,  16, OLED_6X8, ":%d   ", );
//	OLED_Printf(0 ,  24, OLED_6X8, "L2_Val:%d   ", L2_Val);
//	OLED_Printf(0 ,  32, OLED_6X8, "L1_Val:%d   ", L1_Val);
//	OLED_Printf(0 ,  40, OLED_6X8, "M_Val :%d   ", M_Val);
//	OLED_Printf(0 ,  48, OLED_6X8, "R1_Val:%d   ", R1_Val);
//	OLED_Printf(0 ,  56, OLED_6X8, "R2_Val:%d   ", R2_Val);
	/* 最后刷新屏幕 */
	OLED_Update();
}
void KeyBoard_Handling(void)
{
	KeyBoardNum = Keyboard_GetNum();
	switch (KeyBoardNum)
	{
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			break;
		case 14:
			break;
		case 15:
			break;
		case 16:
			break;
		default:
			break;
	}
}
