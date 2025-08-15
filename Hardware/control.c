#include "control.h"


uint8_t TASK = 0;
uint8_t last_TASK = 0;
uint8_t next_TASK = 0;

uint8_t Do_count1 = 0;
uint8_t Do_count2 = 0;
uint8_t Do_count3 = 0;
uint8_t Do_count4 = 0;
uint8_t Do_count5 = 0;
uint8_t Do_count6 = 0;
uint8_t Do_count7 = 0;
uint8_t Do_count8 = 0;
uint8_t Do_count9 = 0;
uint8_t Do_count10 = 0;
uint8_t Do_count11 = 0;
uint8_t Do_count12 = 0;

uint8_t KeyNum;			//接收按键键码
uint8_t KeyBoardNum;	//接收矩阵按键键码
uint8_t IR_Num;			//接收红外遥控键码
uint8_t Usart2Num = 0x00;
/* 云台 */
uint16_t C_red[2];				//红色激光位置
uint16_t C_red_last[2];
uint16_t C_red_last_last[2];
//uint16_t rotation_red[2] = {1500, 1500};		//红色激光舵机位置(PWM)
float rotation_red[2] = {1500, 1500};		//红色激光舵机位置(PWM)

//uint16_t C_green[2];			//绿色激光位置
//uint16_t C_green_last[2];
//uint16_t C_green_last_last[2];
//uint16_t rotation_green[2] = {1500, 1500};		//绿激光舵机位置(PWM)

uint16_t C_tar[2];				//红色激光位置
uint16_t C_tar_last[2];
uint16_t C_tar_last_last[2];

//铅笔
uint16_t pencil_points[5][2] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};	//铅笔顶点 原点
float path_points[PATTH_POINTS_NUM][2];		// 路径点坐标数组
uint8_t total_path_points = 0;				// 总路径点数
uint8_t current_path_point = 0;             // 当前路径点索引

//黑框
//uint16_t black_points[4][2] = {
//1, 1, 
//144, 20, 
//156, 166, 
//22, 156
//};		//黑胶带顶点
uint16_t black_points[4][2] = {0};		//黑胶带顶点
float black_path_points[BLACK_PATTH_POINTS_NUM][2];		// 路径点坐标数组
uint8_t black_total_path_points = 0;				// 总路径点数
uint8_t black_current_path_point = 0;             // 当前路径点索引

/* 小车 */
uint16_t rpm_limit;
float Line_Outval;
float Line_Outval_last;

/* 定时器中断 72MHz / 1000 / 72 = 1000，1ms进一次中断 */
void TIM6_IRQHandler(void)
{
	static uint16_t Count1;
	static uint16_t Count2;
	static uint16_t Count3;
//	static uint16_t Count4;
	static uint16_t Count5;
	
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
	{
		/* 按键扫描 */
		Key_Tick();
//		Keyboard_Tick();
		
		/* LE BEEP 定时 */
		LED_BEEP_Tick();
		
		/* 500ms闪一次代表程序正常运行 */
		if (++Count2 >= 500)
		{
			Count2 = 0;
//			Red_Turn(); 
////			Green_Turn();
////			Yellow_Turn();
//			Blue_Turn();
//			White_Turn();
//			Beep_Turn();
		}
		
		/* 计时 */
		//			OLED_Printf(0 ,  40, OLED_6X8, "time:%6.2fs        ", count.counting_time / 100.0);
		if(flag.counting_flag == 1)
		{
			if (++Count3 >= 10)	//记一次0.01s
			{
				Count3 = 0;
				count.counting_time++;
			}
		}
		else
		{
			Count3 = 0;
		}

		
		/* YAW校准 */
//		if (++Count4 >= 10)
//		{
//			Count4 = 0;
//			MPU_Get_Gyroscope(&gyrox, &gyroy, &gyroz);	//陀螺仪
//			if(flag.YAW_succeed_flag == 0)
//			{
//				YAW_count++;
//			}
//			if(YAW_count >= 11 && YAW_count <= 40)
//			{
//				gyro_x_sum += (float)gyrox / 32768 * 2000;
//				gyro_y_sum += (float)gyroy / 32768 * 2000;
//				gyro_z_sum += (float)gyroz / 32768 * 2000;
////				Motor_PWM(0,0);	//保持静止
//			}
//			else if(YAW_count > 40 && YAW_count < 50)
//			{
//				x_median_angle = gyro_x_sum / 30;
//				y_median_angle = gyro_y_sum / 30;
//				z_median_angle = gyro_z_sum / 30;
//				gyrox_correct = 0;
//				gyroy_correct = 0;
//				gyroz_correct = 0;
//			}
//			else if(YAW_count >= 50)
//			{
//				if(flag.YAW_succeed_flag == 0)
//				{
//					flag.YAW_succeed_flag = 1;
//				}
//				gyrox_correct = ((float)gyrox / 32768 * 2000 - x_median_angle) * 0.01; 
//				gyroy_correct = ((float)gyroy / 32768 * 2000 - y_median_angle) * 0.01; 
//				gyroz_correct = ((float)gyroz / 32768 * 2000 - z_median_angle) * 0.01; 
//				if(gyrox_correct < 0.005 && gyrox_correct > -0.005)
//				{
//					gyrox_correct = 0;
//				}
//				if(gyroy_correct < 0.005 && gyroy_correct > -0.005)
//				{
//					gyroy_correct = 0;
//				}
//				if(gyroz_correct < 0.005 && gyroz_correct > -0.005)
//				{
//					gyroz_correct = 0;
//				}
//				Pitch += gyrox_correct;
//				Roll += gyroy_correct;
//				yaw += gyroz_correct;
//			}
//		}
//		if(flag.YAW_succeed_flag == 1)
//		{
//			if(fabs(yaw) <= 1e-5 && fabs(gyroz_correct) <= 1e-5)
//			{
//				flag.YAW_succeed_flag = 2;
//				Beep_ms_TIM(500);
//			}
//		}
		/* 等待巡铅笔线移动（开环） */
		if(flag.wating_pencil_line_flag)
		{
			count.wating_pencil_line_count++;
//			Yellow_ON();
		}
		else
		{
//			Yellow_OFF();
		}
		
		
		/* 定时器中断核心任务：舵机驱动 */
		if(++Count1 >= PID_PERIOD)	//10ms进一次
//		if(++Count1 >= 40)	//xms进一次
		{
			Count1 = 0;
/*-------------------------------------------步进电机---------------------------------------------*/
			count.stepx_count++;
			if(count.stepx_count == 1)
			{
				Stepper_SetDir(STEP_X, param.stepx_dir);
				Stepper_SetSpeed(STEP_X, param.stepx_speed);
				Stepper_Start(STEP_X);
				Red_ON();
				Blue_OFF();
			}
			if(count.stepx_count == param.stepx_run_ms / 10)
			{
//				Stepper_Stop(STEP_X);
				Red_OFF();
				Blue_ON();
				
			}
			if(count.stepx_count == ( param.stepx_run_ms + param.stepx_stop_ms ) / 10 )
			{
				count.stepx_count = 0;
				
			}
			
			
/*---------------------------------------------舵机-----------------------------------------------*/
			if(TASK == 2)	//复位
			{
				if(flag.reset_flag == 1)
				{
					if(fabs((float)C_red[0] - pencil_points[4][0]) <= 2 && 
					   fabs((float)C_red[1] - pencil_points[4][1]) <= 2)
					{
						flag.reset_flag = 0;
					}
					
					/* 到目标点/追上->声光提示 */
					if(fabs(pid_x.err) < 2 && fabs(pid_y.err) < 2)
					{
						pid_0(&pid_x);
						flag.reach_flag = 1;
						Red_ON();
						Beep_ON();
					}
					else
					{
						flag.reach_flag = 0;
						Red_OFF();
						Beep_OFF();
					}
					
					/* PID控制 */
					PID_x_control(rotation_red);	//rotation_red[2]是红激光舵机当前位置（PWM）
					PID_y_control(rotation_red);
					
					/* 输出PWM */
					Servo1_SetPWM(rotation_red[0]);	//计算出新的rotation_red[2]输出给舵机
					Servo2_SetPWM(rotation_red[1]);
				}
			}
			else if(TASK == 3)	//巡铅笔线
			{
				if(flag.pencil_line_flag == 1)
				{
					if(fabs((float)C_red[0] - path_points[current_path_point][0]) <= 2 && 
					   fabs((float)C_red[1] - path_points[current_path_point][1]) <= 2)
					{
						// 到达目标点后，移动到下一个路径点
						current_path_point++;
						
						// 如果完成所有路径点，标记为完成状态
						if (current_path_point >= total_path_points)
						{
							flag.pencil_line_succeed_flag = 1;
							pid_0(&pid_x);
						}
					}
					set_pid_target(&pid_x, path_points[current_path_point][0]);
					set_pid_target(&pid_y, path_points[current_path_point][1]);
					
					/* PID控制 */
					PID_x_control(rotation_red);	//rotation_red[2]是红激光舵机当前位置（PWM）
					PID_y_control(rotation_red);
					
					/* 输出PWM */
					Servo1_SetPWM(rotation_red[0]);	//计算出新的rotation_red[2]输出给舵机
					Servo2_SetPWM(rotation_red[1]);
				}

			}
			else if(TASK == 7)	//走黑框
			{
				if(flag.black_line_flag == 1)
				{
					float err = 3;
					switch(black_current_path_point)
					{
						case BLACK_PATTH_POINTS_NUM + 1:
						case 42:
						case 41:
						case 43:
							err = 1;
						break;
						default:
							err = 3;
						break;
					}
					if(fabs((float)C_red[0] - black_path_points[black_current_path_point][0]) <= err && 
					   fabs((float)C_red[1] - black_path_points[black_current_path_point][1]) <= err)
					{
						// 到达目标点后，移动到下一个路径点
						black_current_path_point++;
						Beep_ms_TIM(125);	//蜂鸣器125ms
						
						// 如果完成所有路径点，标记为完成状态
						if(black_current_path_point >= black_total_path_points)
						{
							flag.black_line_succeed_flag = 1;
							Beep_ms_TIM(500);	//蜂鸣器125ms
						}
					}
					set_pid_target(&pid_x, black_path_points[black_current_path_point][0]);
					set_pid_target(&pid_y, black_path_points[black_current_path_point][1]);
					
					/* PID控制 */
					PID_x_control(rotation_red);	//rotation_red[2]是红激光舵机当前位置（PWM）
					PID_y_control(rotation_red);
					
					/* 输出PWM */
					Servo1_SetPWM(rotation_red[0]);	//计算出新的rotation_red[2]输出给舵机
					Servo2_SetPWM(rotation_red[1]);
				}
			}
//			else if(TASK == 8)	//复位追踪
//			{
//				if(flag.green_reset_flag == 1)
//				{
//					//追踪到
////					if(fabs((float)C_red[0] - (float)C_green[0]) <= 6 &&
////					   fabs((float)C_red[1] - (float)C_green[1]) <= 6)
////					{
////						Beep_ms_TIM(125);	//蜂鸣器125ms
////						flag.green_reset_succeed_flag = 1;
////					}
////					else
////					{
//						set_pid_target(&pid2_x, (float)C_red[0]);
//						set_pid_target(&pid2_y, (float)C_red[1]);

//						//复位追踪
//						pid2_x.Ki = -0.05;
//						pid2_y.Ki =  0.05;
//						pid2_x.integral_limit = -15;
//						pid2_y.integral_limit =  15;
//						pid2_x.out_limit = 5;
//						pid2_y.out_limit = 5;
//						
//						/* PID控制 */
//						PID2_x_control(rotation_green);	//rotation_green是绿激光舵机当前位置（PWM）
//						PID2_y_control(rotation_green);
//						
//						/* 输出PWM */
//						Servo21_SetPWM(rotation_green[0]);	//计算出新的rotation_red[2]输出给舵机
//						Servo22_SetPWM(rotation_green[1]);
////					}
//				}
//			}
//			else if(TASK == 9)	//运动追踪
//			{
//				//红光运动
//				if(flag.black_line_flag == 1)
//				{
//					if(fabs((float)C_red[0] - black_path_points[black_current_path_point][0]) <= 3 && 
//					   fabs((float)C_red[1] - black_path_points[black_current_path_point][1]) <= 3)
//					{
//						// 到达目标点后，移动到下一个路径点
//						black_current_path_point++;
//						Beep_ms_TIM(125);	//蜂鸣器125ms
//						
//						// 如果完成所有路径点，标记为完成状态
//						if (black_current_path_point >= black_total_path_points)
//						{
//							flag.black_line_succeed_flag = 1;
//							Beep_ms_TIM(500);	//蜂鸣器125ms
////							White_ON();
//						}
//					}
//					set_pid_target(&pid_x, black_path_points[black_current_path_point][0]);
//					set_pid_target(&pid_y, black_path_points[black_current_path_point][1]);
//					
//					/* PID控制 */
//					PID_x_control(rotation_red);	//rotation_red[2]是红激光舵机当前位置（PWM）
//					PID_y_control(rotation_red);
//					
//					/* 输出PWM */
//					Servo1_SetPWM(rotation_red[0]);	//计算出新的rotation_red[2]输出给舵机
//					Servo2_SetPWM(rotation_red[1]);
//				}
//				//绿光追踪
//				if(flag.zhuizong_flag == 1)
//				{
//					//追踪到
//					if(fabs((float)C_red[0] - (float)C_green[0]) <= 6 &&
//					   fabs((float)C_red[1] - (float)C_green[1]) <= 6)
//					{
//						Beep_ms_TIM(125);	//蜂鸣器125ms
//						if(flag.black_line_succeed_flag == 1)
//						{
//							flag.zhuizong_succeed_flag = 1;
//						}
//					}
//					//运动追踪
//					pid2_x.Ki = -0.02;
//					pid2_y.Ki =  0.02;
//					pid2_x.integral_limit = -10;
//					pid2_y.integral_limit =  10;
//					pid2_x.out_limit = 1;
//					pid2_y.out_limit = 1;
//					
//					
//					set_pid_target(&pid2_x, (float)C_red[0]);
//					set_pid_target(&pid2_y, (float)C_red[1]);
//					
//					/* PID控制 */
//					PID2_x_control(rotation_green);	//rotation_green是绿激光舵机当前位置（PWM）
//					PID2_y_control(rotation_green);
//					
//					/* 输出PWM */
//					Servo21_SetPWM(rotation_green[0]);	//计算出新的rotation_red[2]输出给舵机
//					Servo22_SetPWM(rotation_green[1]);
//				}

//			}
			else if(TASK == 10)	//发送目标点与实际点
			{
				if(++Count5 >= 1)
				{
					Count5 = 0;
					
					if(flag.tar_cur_flag == 1)
					{

	//					set_pid_target(&pid1_x, (float)C_tar[0]);
	//					set_pid_target(&pid1_y, (float)C_tar[1]);

	//					//复位追踪
	//					pid2_x.Ki = -0.05;
	//					pid2_y.Ki =  0.05;
	//					pid2_x.integral_limit = -15;
	//					pid2_y.integral_limit =  15;
	//					pid2_x.out_limit = 5;
	//					pid2_y.out_limit = 5;
	//					
	//					/* PID控制 */
	//					PID2_x_control(rotation_green);	//rotation_green是绿激光舵机当前位置（PWM）
	//					PID2_y_control(rotation_green);
	//					
	//					/* 输出PWM */
	//					Servo21_SetPWM(rotation_green[0]);	//计算出新的rotation_red[2]输出给舵机
	//					Servo22_SetPWM(rotation_green[1]);
							
						//运动追踪
//						pid_x.Kp = -0.5;
//						pid_x.Ki = -0.02;
//						pid_x.Kd = -0.1;
//						pid_x.integral_limit = -10;
//						pid_x.out_limit = 1;
//						
//						
//						pid_y.Kp =  0.5;
//						pid_y.Ki =  0.02;
//						pid_y.Kd =  0.1;
//						pid_y.integral_limit =  10;
//						pid_y.out_limit = 1;
						pid_x.Kp = -0.06;
						pid_x.Ki = -0;
						pid_x.Kd = -1;
						pid_x.integral_limit = -10;
						pid_x.out_limit = 1;
						
						
						pid_y.Kp =  0.06;
						pid_y.Ki =  0;
						pid_y.Kd =  1;
						pid_y.integral_limit =  10;
						pid_y.out_limit = 1;
						
						
						set_pid_target(&pid_x, C_tar[0]);
						set_pid_target(&pid_y, C_tar[1]);
						
						/* PID控制 */
						PID_x_control(rotation_red);	//rotation_red[2]是红激光舵机当前位置（PWM）
						PID_y_control(rotation_red);
						
	//					Beep_ms_TIM(100);
						/* 输出PWM */
						Servo1_SetPWM(rotation_red[0]);	//计算出新的rotation_red[2]输出给舵机
						Servo2_SetPWM(rotation_red[1]);
					}
					
				}

			}
			
/*---------------------------------------------小车-----------------------------------------------*/
			GetMotorPulse();//读取电机脉冲数目
//			yaw = HWT101_ReadYaw() - yaw_diff;
//			if(pid_YAW.target_val == 180 && yaw < -10)
//				yaw += 360;
//			else if(pid_YAW.target_val == - 180 && yaw > 10)
//				yaw -= 360;
			
		#if defined(PID_DEBUG_EN)	//上位机调参
			
		/************上位机调试*******************/
			#if(PID_DEBUG_MODE == 1)
				param.Motor1_PWM = speed1_pid_control();
				MotorOutput(param.Motor1_PWM, 0);
			
			#elif(PID_DEBUG_MODE == 2)
				param.Motor2_PWM = speed2_pid_control();
				MotorOutput(0, param.Motor2_PWM);
			
			#elif(PID_DEBUG_MODE == 3)
				Location_Speed_control();
				param.Motor1_PWM = param.speed1_Outval;
				MotorOutput(param.Motor1_PWM, 0);
				
			#elif(PID_DEBUG_MODE == 4)
				Location_Speed_control();
				param.Motor2_PWM = param.speed2_Outval;
				MotorOutput(0, param.Motor2_PWM);
				
			#elif(PID_DEBUG_MODE == 5)
//				set_pid_target(&pid_YAW, 0);
				param.yaw_Outval = yaw_pid_control();
				
				param.Motor1_PWM = -param.yaw_Outval;  
				param.Motor2_PWM = param.yaw_Outval;
				MotorOutput(param.Motor1_PWM, param.Motor2_PWM);
				
			#elif(PID_DEBUG_MODE == 6)
				yaw_speed_control();
				param.Motor1_PWM = param.speed1_Outval;
				param.Motor2_PWM = param.speed2_Outval;
				MotorOutput(param.Motor1_PWM, param.Motor2_PWM);
				
			#endif
		/*****************************************/
		#endif
		#if !defined(PID_DEBUG_EN)	//脱机运行

			//小车位置直接用一个电机的脉冲数累积就好，转向时不计数，开始一个位置前直接清零。
			param.Motor1_journey_cm = pulse2cm(Motor1_Pulse_Sigma);
			param.Motor2_journey_cm = pulse2cm(Motor2_Pulse_Sigma);
			#if defined(GRAY_EN)
				Gray_GoStraight_control();    //一直获取灰度传感器状态，但只有在执行car_go的时候，才进行补偿
//				GW_Gray_GoStraight_control();
			#endif
			/* 直行 */
			if(flag.line_flag == 1) 
			{
				/* 用来让电机停稳*/
				//来个判断，直行到达位置后停止PID控制，防止识别图片时或者等待装卸药时电机耗能，直走巡线结束的阶段才用这个
				if(fabs(param.Motor1_journey_cm - param.Target_Journey_cm) <= 1.0)
				{
					if(++count.stop_count >=  P_Waiting_for_GO_10MS)  //100 * 10ms = 1s	 //可以时间判断放长点，以便刹车停稳
					{
						count.stop_count = 0;
						
						flag.line_flag = 0;
						flag.stop_flag = 1;
						
						flag.move_at_any_speed_flag = 0;

						param.Motor1_PWM  = 0;
						param.Motor2_PWM = 0;
						pid_0_motor();
						
						motor_disable();	//如果速度快，这里直接失能好像会直接溜出去，或者把上面的误差改小点
											//不会溜出去 失能前先把所有输出清零
//						Green_Turn();	//实际不要
					}
				}
				else
				{
					flag.stop_flag = 0;  
					count.stop_count = 0; 					
				}

				/* 电机驱动 */
				if (is_motor_en == 1)
				{
					Location_Speed_control();
					
					if(Get_Gray_Num() == 0)
					{
						long Pulse = (Motor1_Pulse_Sigma + Motor2_Pulse_Sigma) / 2;

						Motor1_Pulse_Sigma = Pulse;  //可能有时候这里加上个补偿会更好
						Motor2_Pulse_Sigma = Pulse;
					}

					param.Motor1_PWM = param.speed1_Outval;  
					param.Motor2_PWM = param.speed2_Outval;
				
					/* 灰度传感器的巡线补偿 */
				#if defined(GRAY_EN)
					Line_Outval_last = Line_Outval;
					Line_Outval =  Get_Gray_Num();     //lineNum得在PWM的重装载值一半左右才会有明显的效果， 可以通过改变Line_Outval的值来控制补偿值的极性
					if(Line_Outval_last == GRAY_PWM_DA_DA || Line_Outval_last == -GRAY_PWM_DA_DA)
					{
						if(L2_Val + L1_Val + M_Val + R1_Val + R2_Val == 0)
						{
							Line_Outval = Line_Outval_last;
						}
					}
					param.Motor1_PWM += Line_Outval;  
					param.Motor2_PWM -= Line_Outval;
				#endif
				#if defined(LINE_YAW_EN)
					
					param.yaw_Outval = yaw_pid_control();
					param.Motor1_PWM -= param.yaw_Outval;
					param.Motor2_PWM += param.yaw_Outval;
				#endif
					MotorOutput(param.Motor1_PWM, param.Motor2_PWM);   
				}
			}
			/* 转向4：角度串速度 */
			if(flag.spin_start_flag == 1) 
			{
				if (is_motor_en == 1)
				{
					if(fabs(yaw - pid_YAW.target_val) <= 3.0)
					{
						if(++count.spin_count >=  P_Waiting_for_TURN_10MS)  //30 * 10ms = 300ms	 //可以时间判断放长点，以便刹车停稳
						{
							count.spin_count = 0;
							
							flag.spin_start_flag = 0;
							flag.spin_succeed_flag = 1;
							
							flag.move_at_any_speed_flag = 0;
							
							param.Motor1_PWM  = 0;
							param.Motor2_PWM = 0;
							pid_0_motor();
							motor_disable();
							
							Red_Turn();	//实际不要
						}
					}
					//如果车向左偏 应该左轮输出正 右轮负值
					//陀螺仪角度向左是增加 实际值偏大 err=tar-act<0为负值
					//所以左轮pwm=-yaw_out
					yaw_speed_control();
					param.Motor1_PWM = param.speed1_Outval;
					param.Motor2_PWM = param.speed2_Outval;
					MotorOutput(param.Motor1_PWM, param.Motor2_PWM); 
				}
			}
//			ServoLine_Control();
//			ServoCircle_Control();
//			ServoTriangle_Control();
//			ServoRotRect_Control();
			Shape_Control();
			
		#endif
		}
		
		
		
		/* 清除中断标志位 */
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
}
/*-------------用定时器让BEEP定时鸣叫----------------------*/
void Beep_ms_TIM(uint16_t xms)
{
	param.beep_ms = xms;
	flag.beep_flag = 1;
}
void Red_ms_TIM(uint16_t xms)
{
	param.red_ms = xms;
	flag.red_flag = 1;
}
void Green_ms_TIM(uint16_t xms)
{
	param.green_ms = xms;
	flag.green_flag = 1;
}
void Yellow_ms_TIM(uint16_t xms)
{
	param.yellow_ms = xms;
	flag.yellow_flag = 1;
}
#if defined(LED_BLUE)
void Blue_ms_TIM(uint16_t xms)
{
	param.blue_ms = xms;
	flag.blue_flag = 1;
}
#endif
#if defined(WHITE_BLUE)
void White_ms_TIM(uint16_t xms)
{
	param.white_ms = xms;
	flag.white_flag = 1;
}
#endif
void LED_BEEP_Tick(void)
{
	/* 蜂鸣器定时鸣叫 */
	if(flag.beep_flag == 1)
	{
		if(++count.beep_count <= param.beep_ms)
		{
			Beep_ON();
		}
		else
		{
			Beep_OFF();
			count.beep_count = 0;
			flag.beep_flag = 0;
		}
	}
	#if defined(LED_RED)
	if(flag.red_flag == 1)
	{
		if(++count.red_count <= param.red_ms)
		{
			Red_ON();
		}
		else
		{
			Red_OFF();
			count.red_count = 0;
			flag.red_flag = 0;
		}
	}
	#endif
	#if defined(LED_GREEN)
	if(flag.green_flag == 1)
	{
		if(++count.green_count <= param.green_ms)
		{
			Green_ON();
		}
		else
		{
			Green_OFF();
			count.green_count = 0;
			flag.green_flag = 0;
		}
	}
	#endif
	#if defined(LED_YELLOW)
	if(flag.yellow_flag == 1)
	{
		if(++count.yellow_count <= param.yellow_ms)
		{
			Yellow_ON();
		}
		else
		{
			Yellow_OFF();
			count.yellow_count = 0;
			flag.yellow_flag = 0;
		}
	}
	#endif
	#if defined(LED_BLUE)
	if(flag.blue_flag == 1)
	{
		if(++count.blue_count <= param.blue_ms)
		{
			Blue_ON();
		}
		else
		{
			Blue_OFF();
			count.blue_count = 0;
			flag.blue_flag = 0;
		}
	}
	#endif
	#if defined(LED_WHITE)
	if(flag.white_flag == 1)
	{
		if(++count.white_count <= param.white_ms)
		{
			White_ON();
		}
		else
		{
			White_OFF();
			count.white_count = 0;
			flag.white_flag = 0;
		}
	}
	#endif
}
/*----------------------计时----------------------*/
void jishi_start(void)
{
	count.counting_time = 0;	//清除上次的计时
	flag.counting_flag = 1;		//开始计时
}
void jishi_stop(void)
{
	flag.counting_flag = 0;
}
/*----------------------pid某些参数清零----------------------*/
void pid_0(_pid *pid)
{
	pid->integral = 0;
	pid->err = 0;
	pid->err_last = 0;
	pid->out_val = 0;
}
void pid_0_motor(void)
{
	pid_0(&pid_location1);
	pid_0(&pid_location2);
	pid_0(&pid_speed1);
	pid_0(&pid_speed2);
	pid_0(&pid_YAW);
	param.location1_Outval = 0;
	param.location2_Outval = 0;
	param.speed1_Outval = 0;
	param.speed2_Outval = 0;
	param.yaw_Outval = 0;
}
/*------------------------------------------------------------云台------------------------------------------------------------*/
/*----------------------x----------------------*/
void PID_x_control(float *rotation)
{
	float cont_val = 0.0; 
	float actual_coordinate;	//实际坐标
	float max_rotate[2] = {1300, 1700};	//输出位置

	actual_coordinate = (float)C_red[0];
	
	cont_val = pid_x_realize(&pid_x, actual_coordinate);

	/* 输出限幅 */
	cont_val = (cont_val > pid_x.out_limit ? pid_x.out_limit : cont_val);
	cont_val = (cont_val < -pid_x.out_limit ? -pid_x.out_limit : cont_val);
	
	param.pid_x_Outval = cont_val;
	
	/* 以上算出角度做增量（速度），下面算输出角度（速度积分） */

	if (rotation[0] + param.pid_x_Outval > max_rotate[1])//旋转角度限制
		rotation[0] = max_rotate[1];
	else if (rotation[0] + param.pid_x_Outval < max_rotate[0])
		rotation[0] = max_rotate[0];
	else
		rotation[0] += param.pid_x_Outval;
}

/*----------------------y----------------------*/
void PID_y_control(float *rotation)
{
	float cont_val = 0.0; 
	float actual_coordinate;
	float max_rotate[2] = {1300, 1700};	//输出位置

	actual_coordinate = (float)C_red[1];
	
	cont_val = pid_y_realize(&pid_y, actual_coordinate);   

	/* 目标上限处理 */
	cont_val = (cont_val > pid_y.out_limit ? pid_y.out_limit : cont_val);
	cont_val = (cont_val < -pid_y.out_limit ? -pid_y.out_limit : cont_val);
	
	param.pid_y_Outval = cont_val;
	
	/* 以上算出角度做增量（速度），下面算输出角度（速度积分） */

//	param.pid_y_Outval = Limit_float(param.pid_y_Outval, max_rotate[0], max_rotate[1]);
//	rotation[1] += param.pid_y_Outval;
	
	if (rotation[1] + param.pid_y_Outval > max_rotate[1])//旋转角度限制
		rotation[1] = max_rotate[1];
	else if (rotation[1] + param.pid_y_Outval < max_rotate[0])
		rotation[1] = max_rotate[0];
	else
		rotation[1] += param.pid_y_Outval;
}

// 生成路径插值点函数
// 功能：在相邻顶点之间生成均匀分布的插值点，确保激光严格沿直线移动
void Path_Points_Get(void)
{
    uint8_t pointIndex = 0;
    
    // 为正方形的每条边生成插值点
    for (uint8_t edge = 0; edge < 4; edge++)
	{
        // 获取当前边的起点和终点坐标
        float x1 = pencil_points[edge][0];
        float y1 = pencil_points[edge][1];
        float x2 = pencil_points[(edge + 1) % 4][0];  // 下一个顶点坐标
        float y2 = pencil_points[(edge + 1) % 4][1];
        
        // 在线段上生成均匀分布的插值点
        for (uint8_t i = 0; i < POINTS_PER_EDGE; i++)
		{
            // 线性插值公式：x = x1 + (x2-x1) * t, y = y1 + (y2-y1) * t
            // 其中t为插值比例，从0到1
            float t = (float)i / (float)POINTS_PER_EDGE;
            path_points[pointIndex][0] = x1 + (x2 - x1) * t;
            path_points[pointIndex][1] = y1 + (y2 - y1) * t;
            pointIndex++;
        }
        
        // 添加顶点（作为当前边的终点和下一条边的起点）
        if (edge < 3)
		{
            path_points[pointIndex][0] = x2;
            path_points[pointIndex][1] = y2;
            pointIndex++;
        }
    }
    total_path_points = pointIndex;
}


void black_path_points_get(void)
{
    uint8_t pointIndex = 0;
    
    // 为正方形的每条边生成插值点
    for (uint8_t edge = 0; edge < 4; edge++)
	{
        // 获取当前边的起点和终点坐标
        float x1 = (float)black_points[edge][0];
        float y1 = (float)black_points[edge][1];
        float x2 = (float)black_points[(edge + 1) % 4][0];  // 下一个顶点坐标
        float y2 = (float)black_points[(edge + 1) % 4][1];
        
        // 在线段上生成均匀分布的插值点
        for (uint8_t i = 0; i < BLACK_POINTS_PER_EDGE; i++)
		{
            // 线性插值公式：x = x1 + (x2-x1) * t, y = y1 + (y2-y1) * t
            // 其中t为插值比例，从0到1
            float t = (float)i / (float)BLACK_POINTS_PER_EDGE;
            black_path_points[pointIndex][0] = x1 + (x2 - x1) * t;
            black_path_points[pointIndex][1] = y1 + (y2 - y1) * t;
            pointIndex++;
        }
        
        // 添加顶点（作为当前边的终点和下一条边的起点）
        if (edge < 3)
		{
            black_path_points[pointIndex][0] = x2;
            black_path_points[pointIndex][1] = y2;
            pointIndex++;
        }
    }
    
    black_total_path_points = pointIndex;
	
	flag.black_path_pionts_get_succeed_flag = 1;
}
/*------------------------------------------------------------小车------------------------------------------------------------*/
/*----------------------直走----------------------*/
//void Car_go(int32_t location_cm)
//{
//	//用角度环前将当前角度清零
//#if defined(LINE_YAW_EN)
////	yaw = 0;
////	HWT101_CaliYaw();
//	set_yaw_tar(0);
//	yaw_CarGo_pid_init();
//#endif
//	
//	flag.line_flag = 1;
//	flag.stop_flag = 0;
//	flag.spin_start_flag = 0;
//	flag.spin_succeed_flag = 0;

//	param.Motor1_journey_cm = 0;
//	param.Motor2_journey_cm = 0;
//	Motor1_Pulse_Sigma = 0;
//	Motor2_Pulse_Sigma = 0;
//	
//	
//	param.Target_Journey_cm = location_cm;   //防止长时间PID控制用
//	
//	float Car_location = cm2pulse(location_cm);
//	
//	/* 车重补偿 */
//	location_cm = K_CAR_GO * location_cm;
//	
//#if defined(BACK_CM_BUCHANG_EN)
//	/* 倒头距离补偿 */
//	if (flag.back_180_flag_cm)
//	{
//		location_cm -= BACK_CM_BuChang_CM;
//		flag.back_180_flag_cm = 0;
//	}
//#endif
//	
//	set_pid_target(&pid_location1, Car_location);   
//	set_pid_target(&pid_location2, Car_location);
//	motor_enable();
//}

void Car_go(int32_t location_cm)
{
	//用角度环前将当前角度清零
#if defined(LINE_YAW_EN)
//	yaw = 0;
	HWT101_CaliYaw();
	set_yaw_tar(0);
	yaw_CarGo_pid_init();
#endif
	
	flag.line_flag = 1;
	flag.stop_flag = 0;
	flag.spin_start_flag = 0;
	flag.spin_succeed_flag = 0;

	param.Motor1_journey_cm = 0;
	param.Motor2_journey_cm = 0;
	Motor1_Pulse_Sigma = 0;
	Motor2_Pulse_Sigma = 0;

#if defined(BACK_CM_BUCHANG_EN)
	/* 倒头距离补偿 */
	if (flag.back_180_flag_cm)
	{
		location_cm -= BACK_CM_BuChang_CM;
		flag.back_180_flag_cm = 0;
	}
#endif
	
	
	/* 车重补偿 */
	location_cm = K_CAR_GO(location_cm) * location_cm;
	
	param.Target_Journey_cm = location_cm;   //防止长时间PID控制用
	
	float location_pluse = cm2pulse(location_cm);
	
	set_pid_target(&pid_location1, location_pluse);   
	set_pid_target(&pid_location2, location_pluse);
	motor_enable();
}

void Car_go_speed(int32_t location_cm, uint16_t max_rpm)
{
	flag.move_at_any_speed_flag = 1;
	rpm_limit = max_rpm;
	Car_go(location_cm);
}

/*----------------------转向----------------------*/
void spin_Turn(spin_dir_t zhuanxiang)	//其实转向环并不需要很精准，转弯后直接用直走时的巡线函数回正车身就好
{
	flag.line_flag = 0;
	flag.stop_flag = 0;   //执行转弯时，将直走完成的标志位清零. 即如果上一次是直行，这次是转弯，则不用在业务代码里手动置
	flag.spin_start_flag = 1;   
	flag.spin_succeed_flag = 0;  

	param.Motor1_journey_cm = 0;
	param.Motor2_journey_cm = 0;
	Motor1_Pulse_Sigma = 0;
	Motor2_Pulse_Sigma = 0;
	
	//90°对应的脉冲数
	float Car_Turn_val = angle2pulse(90);

	
	switch(zhuanxiang)
	{
		case left_90:	Car_Turn_val = K_LEFT_90 * Car_Turn_val;		break;
		case right_90:	Car_Turn_val = - K_RIGHT_90 * Car_Turn_val;		break;
		case back_180:	Car_Turn_val = - 2 * K_BACK_180 * Car_Turn_val;	break;
		default:	break;
	}
	
//	switch(zhuanxiang)
//	{
//		case left_90:	param.Target_Journey_cm = K_LEFT_90 * Car_Turn_val;			break;
//		case right_90:	param.Target_Journey_cm = K_RIGHT_90 * Car_Turn_val;		break;	//目标距离不加负号
//		case back_180:	param.Target_Journey_cm = 2 * K_BACK_180 * Car_Turn_val;	break;	//目标距离不加负号
//		default:	break;
//	}
	param.Target_Journey_cm = fabs(Car_Turn_val);
	
/* 转弯角度补偿 */
#if defined(TURN_BUCHANG_EN)
	switch(zhuanxiang)
	{
		case left_90:	flag.left_90_flag_angle  = 1;	break;	//左转角度补偿
		case right_90:	flag.right_90_flag_angle = 1;	break;	//右转角度补偿
		case back_180:	flag.back_180_flag_angle = 1;	break;	//倒头距离补偿
		default:	break;
	}
#endif
	
/* 倒头距离补偿 */
#if defined(BACK_CM_BUCHANG_EN)
	if(zhuanxiang == back_180)
	{
		flag.back_180_flag_cm = 1;
	}
#endif

	set_pid_target(&pid_location1,-Car_Turn_val);   
	set_pid_target(&pid_location2, Car_Turn_val);
	motor_enable();
}
void spin_Turn_angle(float angle)	//任意角度转向//左正
{
	flag.line_flag = 0;
	flag.stop_flag = 0;   //执行转弯时，将直走完成的标志位清零. 即如果上一次是直行，这次是转弯，则不用在业务代码里手动置
	flag.spin_start_flag = 1;
	flag.spin_succeed_flag = 0;

	param.Motor1_journey_cm = 0;
	param.Motor2_journey_cm = 0;
	Motor1_Pulse_Sigma = 0;
	Motor2_Pulse_Sigma = 0;
	
	float Car_Turn_val = angle2pulse(angle);

	//车重补偿
	Car_Turn_val = K_ANGLE * Car_Turn_val;

	param.Target_Journey_cm = fabs(Car_Turn_val);
	
	set_pid_target(&pid_location1,-Car_Turn_val);   
	set_pid_target(&pid_location2, Car_Turn_val);
	motor_enable();
}
void set_yaw_tar(float tar_yaw)
{
	HWT101_CaliYaw();
	set_pid_target(&pid_YAW, tar_yaw);
}
void get_yaw(void)
{
//	yaw = HWT101_ReadYaw() - yaw_diff;
	yaw = HWT101_ReadYaw();
//	if(pid_YAW.target_val == 180 && yaw < -10)
//		yaw += 360;
//	else if(pid_YAW.target_val == - 180 && yaw > 10)
//		yaw -= 360;
	if(pid_YAW.target_val > 180 - 15 && yaw < -10)
		yaw += 360;
	else if(pid_YAW.target_val < - 180 + 15 && yaw > 10)
		yaw -= 360;
}

void spin_Turn_YAW(float angle)	//任意角度转向 左正右负
{
//	yaw = 0;			//MPU
	HWT101_CaliYaw();	//HWT
	
	yaw_Turn_pid_init();
	
	set_yaw_tar(angle);
	flag.line_flag = 0;
	flag.stop_flag = 0;
	flag.spin_start_flag = 1;
	flag.spin_succeed_flag = 0;

	param.Motor1_journey_cm = 0;
	param.Motor2_journey_cm = 0;
	Motor1_Pulse_Sigma = 0;
	Motor2_Pulse_Sigma = 0;
	
//	set_pid_target(&pid_YAW, angle);
	motor_enable();
}
void spin_Turn_speed(spin_dir_t zhuanxiang, uint16_t max_rpm)	//任意速度转向
{
	flag.move_at_any_speed_flag = 1;
	rpm_limit = max_rpm;
	spin_Turn(zhuanxiang);
}

/*---------------------各PID---------------------*/
void Location_Speed_control(void)
{
	if (is_motor_en == 1)
	{
		static uint8_t location_control_count = 0;
		location_control_count++;
		if(location_control_count >= 2)
		{
			location_control_count = 0; 
			
			/* 位置环的输出作为速度 是速度环的输入 */
			param.location1_Outval = location1_pid_control();
			param.location2_Outval = location2_pid_control();
		}
		
		/* 位置环的输出作为速度经set_pid_target传入速度环 */
		set_pid_target(&pid_speed1, param.location1_Outval);		//每次都必须有位置环的值
		set_pid_target(&pid_speed2, param.location2_Outval);	//每次都必须有位置环的值
			
		/* param.speed1_Outval 经过补偿最后作为PWM */
		param.speed1_Outval = speed1_pid_control();				//要是电机转向不符合预期，就在这两句里取反数值
		param.speed2_Outval = speed2_pid_control();
	}
}


float speed1_pid_control(void)  
{
    float cont_val = 0.0;	//当前控制值
    float actual_speed;
	
	/* 当前速度（rpm） = 编码器获取的脉冲 * 60s / (44 * 20.41 * (10ms / 1000)) 这里1000放到后面会报错 division by zero 所以放到前面*/
//	actual_speed = ((float)Motor1_Pulse_Unit * 60.0 * 1000.0)/(MAI_CHONG_PER_ROUND * PID_PERIOD);
	actual_speed = pulse2rpm(Motor1_Pulse_Unit);

	/* 当前速度传入 speed_pid_realize 进行速度pid算法实现 */
	cont_val = speed_pid_realize(&pid_speed1, actual_speed);	// 进行 PID 计算
    
	return cont_val;
}

float speed2_pid_control(void)  
{
   
    float cont_val = 0.0;	//当前控制值
    float actual_speed;
	
	actual_speed = pulse2rpm(Motor2_Pulse_Unit);
	
    cont_val = speed_pid_realize(&pid_speed2, actual_speed);	// 进行 PID 计算
    
	return cont_val;
}


float location1_pid_control(void)  
{
	float cont_val = 0.0; 
	float actual_location;
	float target_speed_rpm_limit;

	actual_location = Motor1_Pulse_Sigma;
	
	/* 将当前位置actual_location(脉冲数)传入location_pid_realize进行位置pid算法实现 */
	/* 然后将输出值返回，之后组为速度目标值(RPM)传入速度pid */			
	cont_val = location_pid_realize(&pid_location1, actual_location);   

	/* 目标速度上限处理 */
	target_speed_rpm_limit = flag.move_at_any_speed_flag ? rpm_limit : pid_location1.out_limit;	//}pid_location1.out_limit
	cont_val = Limit_float(cont_val, target_speed_rpm_limit, -target_speed_rpm_limit);

	return cont_val;
}



float location2_pid_control(void)  
{
	float cont_val = 0.0; 
	float actual_location;
	float target_speed_rpm_limit;
	
	actual_location = Motor2_Pulse_Sigma;

    cont_val = location_pid_realize(&pid_location2, actual_location);   

	/* 目标速度上限处理 */
	target_speed_rpm_limit = flag.move_at_any_speed_flag ? rpm_limit : pid_location2.out_limit;	//pid_location2.out_limit
	cont_val = Limit_float(cont_val, target_speed_rpm_limit, -target_speed_rpm_limit);
	
	return cont_val;
}
/* 转向 角度串速度 */
void yaw_speed_control(void)
{
	if (is_motor_en == 1)
	{
//		static uint8_t location_control_count = 0;
//		location_control_count++;
//		if(location_control_count >= 2)
//		{
//			location_control_count = 0; 
//			
//			/* 位置环的输出作为速度 是速度环的输入 */
//			param.location1_Outval = location1_pid_control();
//			param.location2_Outval = location2_pid_control();
//		}
		
		param.yaw_Outval = yaw_pid_control();
		
		/* 位置环的输出作为速度经set_pid_target传入速度环 */
		set_pid_target(&pid_speed1, -param.yaw_Outval);		//每次都必须有位置环的值
		set_pid_target(&pid_speed2, param.yaw_Outval);	//每次都必须有位置环的值
			
		/* param.speed1_Outval 经过补偿最后作为PWM */
		param.speed1_Outval = speed1_pid_control();				//要是电机转向不符合预期，就在这两句里取反数值
		param.speed2_Outval = speed2_pid_control();
	}
}
/* 转向 角度环 */
float yaw_pid_control(void)
{
	float cont_val = 0.0; 
	float actual_yaw;
	
	actual_yaw = yaw;

    cont_val = YAW_pid_realize(&pid_YAW, actual_yaw);   

	/* 输出限幅 */
	cont_val = Limit_float(cont_val, pid_YAW.out_limit, -pid_YAW.out_limit);
	
	return cont_val;
}
