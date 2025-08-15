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

uint8_t KeyNum;			//���հ�������
uint8_t KeyBoardNum;	//���վ��󰴼�����
uint8_t IR_Num;			//���պ���ң�ؼ���
uint8_t Usart2Num = 0x00;
/* ��̨ */
uint16_t C_red[2];				//��ɫ����λ��
uint16_t C_red_last[2];
uint16_t C_red_last_last[2];
//uint16_t rotation_red[2] = {1500, 1500};		//��ɫ������λ��(PWM)
float rotation_red[2] = {1500, 1500};		//��ɫ������λ��(PWM)

//uint16_t C_green[2];			//��ɫ����λ��
//uint16_t C_green_last[2];
//uint16_t C_green_last_last[2];
//uint16_t rotation_green[2] = {1500, 1500};		//�̼�����λ��(PWM)

uint16_t C_tar[2];				//��ɫ����λ��
uint16_t C_tar_last[2];
uint16_t C_tar_last_last[2];

//Ǧ��
uint16_t pencil_points[5][2] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};	//Ǧ�ʶ��� ԭ��
float path_points[PATTH_POINTS_NUM][2];		// ·������������
uint8_t total_path_points = 0;				// ��·������
uint8_t current_path_point = 0;             // ��ǰ·��������

//�ڿ�
//uint16_t black_points[4][2] = {
//1, 1, 
//144, 20, 
//156, 166, 
//22, 156
//};		//�ڽ�������
uint16_t black_points[4][2] = {0};		//�ڽ�������
float black_path_points[BLACK_PATTH_POINTS_NUM][2];		// ·������������
uint8_t black_total_path_points = 0;				// ��·������
uint8_t black_current_path_point = 0;             // ��ǰ·��������

/* С�� */
uint16_t rpm_limit;
float Line_Outval;
float Line_Outval_last;

/* ��ʱ���ж� 72MHz / 1000 / 72 = 1000��1ms��һ���ж� */
void TIM6_IRQHandler(void)
{
	static uint16_t Count1;
	static uint16_t Count2;
	static uint16_t Count3;
//	static uint16_t Count4;
	static uint16_t Count5;
	
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
	{
		/* ����ɨ�� */
		Key_Tick();
//		Keyboard_Tick();
		
		/* LE BEEP ��ʱ */
		LED_BEEP_Tick();
		
		/* 500ms��һ�δ�������������� */
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
		
		/* ��ʱ */
		//			OLED_Printf(0 ,  40, OLED_6X8, "time:%6.2fs        ", count.counting_time / 100.0);
		if(flag.counting_flag == 1)
		{
			if (++Count3 >= 10)	//��һ��0.01s
			{
				Count3 = 0;
				count.counting_time++;
			}
		}
		else
		{
			Count3 = 0;
		}

		
		/* YAWУ׼ */
//		if (++Count4 >= 10)
//		{
//			Count4 = 0;
//			MPU_Get_Gyroscope(&gyrox, &gyroy, &gyroz);	//������
//			if(flag.YAW_succeed_flag == 0)
//			{
//				YAW_count++;
//			}
//			if(YAW_count >= 11 && YAW_count <= 40)
//			{
//				gyro_x_sum += (float)gyrox / 32768 * 2000;
//				gyro_y_sum += (float)gyroy / 32768 * 2000;
//				gyro_z_sum += (float)gyroz / 32768 * 2000;
////				Motor_PWM(0,0);	//���־�ֹ
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
		/* �ȴ�ѲǦ�����ƶ��������� */
		if(flag.wating_pencil_line_flag)
		{
			count.wating_pencil_line_count++;
//			Yellow_ON();
		}
		else
		{
//			Yellow_OFF();
		}
		
		
		/* ��ʱ���жϺ������񣺶������ */
		if(++Count1 >= PID_PERIOD)	//10ms��һ��
//		if(++Count1 >= 40)	//xms��һ��
		{
			Count1 = 0;
/*-------------------------------------------�������---------------------------------------------*/
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
			
			
/*---------------------------------------------���-----------------------------------------------*/
			if(TASK == 2)	//��λ
			{
				if(flag.reset_flag == 1)
				{
					if(fabs((float)C_red[0] - pencil_points[4][0]) <= 2 && 
					   fabs((float)C_red[1] - pencil_points[4][1]) <= 2)
					{
						flag.reset_flag = 0;
					}
					
					/* ��Ŀ���/׷��->������ʾ */
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
					
					/* PID���� */
					PID_x_control(rotation_red);	//rotation_red[2]�Ǻ켤������ǰλ�ã�PWM��
					PID_y_control(rotation_red);
					
					/* ���PWM */
					Servo1_SetPWM(rotation_red[0]);	//������µ�rotation_red[2]��������
					Servo2_SetPWM(rotation_red[1]);
				}
			}
			else if(TASK == 3)	//ѲǦ����
			{
				if(flag.pencil_line_flag == 1)
				{
					if(fabs((float)C_red[0] - path_points[current_path_point][0]) <= 2 && 
					   fabs((float)C_red[1] - path_points[current_path_point][1]) <= 2)
					{
						// ����Ŀ�����ƶ�����һ��·����
						current_path_point++;
						
						// ����������·���㣬���Ϊ���״̬
						if (current_path_point >= total_path_points)
						{
							flag.pencil_line_succeed_flag = 1;
							pid_0(&pid_x);
						}
					}
					set_pid_target(&pid_x, path_points[current_path_point][0]);
					set_pid_target(&pid_y, path_points[current_path_point][1]);
					
					/* PID���� */
					PID_x_control(rotation_red);	//rotation_red[2]�Ǻ켤������ǰλ�ã�PWM��
					PID_y_control(rotation_red);
					
					/* ���PWM */
					Servo1_SetPWM(rotation_red[0]);	//������µ�rotation_red[2]��������
					Servo2_SetPWM(rotation_red[1]);
				}

			}
			else if(TASK == 7)	//�ߺڿ�
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
						// ����Ŀ�����ƶ�����һ��·����
						black_current_path_point++;
						Beep_ms_TIM(125);	//������125ms
						
						// ����������·���㣬���Ϊ���״̬
						if(black_current_path_point >= black_total_path_points)
						{
							flag.black_line_succeed_flag = 1;
							Beep_ms_TIM(500);	//������125ms
						}
					}
					set_pid_target(&pid_x, black_path_points[black_current_path_point][0]);
					set_pid_target(&pid_y, black_path_points[black_current_path_point][1]);
					
					/* PID���� */
					PID_x_control(rotation_red);	//rotation_red[2]�Ǻ켤������ǰλ�ã�PWM��
					PID_y_control(rotation_red);
					
					/* ���PWM */
					Servo1_SetPWM(rotation_red[0]);	//������µ�rotation_red[2]��������
					Servo2_SetPWM(rotation_red[1]);
				}
			}
//			else if(TASK == 8)	//��λ׷��
//			{
//				if(flag.green_reset_flag == 1)
//				{
//					//׷�ٵ�
////					if(fabs((float)C_red[0] - (float)C_green[0]) <= 6 &&
////					   fabs((float)C_red[1] - (float)C_green[1]) <= 6)
////					{
////						Beep_ms_TIM(125);	//������125ms
////						flag.green_reset_succeed_flag = 1;
////					}
////					else
////					{
//						set_pid_target(&pid2_x, (float)C_red[0]);
//						set_pid_target(&pid2_y, (float)C_red[1]);

//						//��λ׷��
//						pid2_x.Ki = -0.05;
//						pid2_y.Ki =  0.05;
//						pid2_x.integral_limit = -15;
//						pid2_y.integral_limit =  15;
//						pid2_x.out_limit = 5;
//						pid2_y.out_limit = 5;
//						
//						/* PID���� */
//						PID2_x_control(rotation_green);	//rotation_green���̼�������ǰλ�ã�PWM��
//						PID2_y_control(rotation_green);
//						
//						/* ���PWM */
//						Servo21_SetPWM(rotation_green[0]);	//������µ�rotation_red[2]��������
//						Servo22_SetPWM(rotation_green[1]);
////					}
//				}
//			}
//			else if(TASK == 9)	//�˶�׷��
//			{
//				//����˶�
//				if(flag.black_line_flag == 1)
//				{
//					if(fabs((float)C_red[0] - black_path_points[black_current_path_point][0]) <= 3 && 
//					   fabs((float)C_red[1] - black_path_points[black_current_path_point][1]) <= 3)
//					{
//						// ����Ŀ�����ƶ�����һ��·����
//						black_current_path_point++;
//						Beep_ms_TIM(125);	//������125ms
//						
//						// ����������·���㣬���Ϊ���״̬
//						if (black_current_path_point >= black_total_path_points)
//						{
//							flag.black_line_succeed_flag = 1;
//							Beep_ms_TIM(500);	//������125ms
////							White_ON();
//						}
//					}
//					set_pid_target(&pid_x, black_path_points[black_current_path_point][0]);
//					set_pid_target(&pid_y, black_path_points[black_current_path_point][1]);
//					
//					/* PID���� */
//					PID_x_control(rotation_red);	//rotation_red[2]�Ǻ켤������ǰλ�ã�PWM��
//					PID_y_control(rotation_red);
//					
//					/* ���PWM */
//					Servo1_SetPWM(rotation_red[0]);	//������µ�rotation_red[2]��������
//					Servo2_SetPWM(rotation_red[1]);
//				}
//				//�̹�׷��
//				if(flag.zhuizong_flag == 1)
//				{
//					//׷�ٵ�
//					if(fabs((float)C_red[0] - (float)C_green[0]) <= 6 &&
//					   fabs((float)C_red[1] - (float)C_green[1]) <= 6)
//					{
//						Beep_ms_TIM(125);	//������125ms
//						if(flag.black_line_succeed_flag == 1)
//						{
//							flag.zhuizong_succeed_flag = 1;
//						}
//					}
//					//�˶�׷��
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
//					/* PID���� */
//					PID2_x_control(rotation_green);	//rotation_green���̼�������ǰλ�ã�PWM��
//					PID2_y_control(rotation_green);
//					
//					/* ���PWM */
//					Servo21_SetPWM(rotation_green[0]);	//������µ�rotation_red[2]��������
//					Servo22_SetPWM(rotation_green[1]);
//				}

//			}
			else if(TASK == 10)	//����Ŀ�����ʵ�ʵ�
			{
				if(++Count5 >= 1)
				{
					Count5 = 0;
					
					if(flag.tar_cur_flag == 1)
					{

	//					set_pid_target(&pid1_x, (float)C_tar[0]);
	//					set_pid_target(&pid1_y, (float)C_tar[1]);

	//					//��λ׷��
	//					pid2_x.Ki = -0.05;
	//					pid2_y.Ki =  0.05;
	//					pid2_x.integral_limit = -15;
	//					pid2_y.integral_limit =  15;
	//					pid2_x.out_limit = 5;
	//					pid2_y.out_limit = 5;
	//					
	//					/* PID���� */
	//					PID2_x_control(rotation_green);	//rotation_green���̼�������ǰλ�ã�PWM��
	//					PID2_y_control(rotation_green);
	//					
	//					/* ���PWM */
	//					Servo21_SetPWM(rotation_green[0]);	//������µ�rotation_red[2]��������
	//					Servo22_SetPWM(rotation_green[1]);
							
						//�˶�׷��
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
						
						/* PID���� */
						PID_x_control(rotation_red);	//rotation_red[2]�Ǻ켤������ǰλ�ã�PWM��
						PID_y_control(rotation_red);
						
	//					Beep_ms_TIM(100);
						/* ���PWM */
						Servo1_SetPWM(rotation_red[0]);	//������µ�rotation_red[2]��������
						Servo2_SetPWM(rotation_red[1]);
					}
					
				}

			}
			
/*---------------------------------------------С��-----------------------------------------------*/
			GetMotorPulse();//��ȡ���������Ŀ
//			yaw = HWT101_ReadYaw() - yaw_diff;
//			if(pid_YAW.target_val == 180 && yaw < -10)
//				yaw += 360;
//			else if(pid_YAW.target_val == - 180 && yaw > 10)
//				yaw -= 360;
			
		#if defined(PID_DEBUG_EN)	//��λ������
			
		/************��λ������*******************/
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
		#if !defined(PID_DEBUG_EN)	//�ѻ�����

			//С��λ��ֱ����һ��������������ۻ��ͺã�ת��ʱ����������ʼһ��λ��ǰֱ�����㡣
			param.Motor1_journey_cm = pulse2cm(Motor1_Pulse_Sigma);
			param.Motor2_journey_cm = pulse2cm(Motor2_Pulse_Sigma);
			#if defined(GRAY_EN)
				Gray_GoStraight_control();    //һֱ��ȡ�Ҷȴ�����״̬����ֻ����ִ��car_go��ʱ�򣬲Ž��в���
//				GW_Gray_GoStraight_control();
			#endif
			/* ֱ�� */
			if(flag.line_flag == 1) 
			{
				/* �����õ��ͣ��*/
				//�����жϣ�ֱ�е���λ�ú�ֹͣPID���ƣ���ֹʶ��ͼƬʱ���ߵȴ�װжҩʱ������ܣ�ֱ��Ѳ�߽����Ľ׶β������
				if(fabs(param.Motor1_journey_cm - param.Target_Journey_cm) <= 1.0)
				{
					if(++count.stop_count >=  P_Waiting_for_GO_10MS)  //100 * 10ms = 1s	 //����ʱ���жϷų��㣬�Ա�ɲ��ͣ��
					{
						count.stop_count = 0;
						
						flag.line_flag = 0;
						flag.stop_flag = 1;
						
						flag.move_at_any_speed_flag = 0;

						param.Motor1_PWM  = 0;
						param.Motor2_PWM = 0;
						pid_0_motor();
						
						motor_disable();	//����ٶȿ죬����ֱ��ʧ�ܺ����ֱ�����ȥ�����߰����������С��
											//�������ȥ ʧ��ǰ�Ȱ������������
//						Green_Turn();	//ʵ�ʲ�Ҫ
					}
				}
				else
				{
					flag.stop_flag = 0;  
					count.stop_count = 0; 					
				}

				/* ������� */
				if (is_motor_en == 1)
				{
					Location_Speed_control();
					
					if(Get_Gray_Num() == 0)
					{
						long Pulse = (Motor1_Pulse_Sigma + Motor2_Pulse_Sigma) / 2;

						Motor1_Pulse_Sigma = Pulse;  //������ʱ��������ϸ����������
						Motor2_Pulse_Sigma = Pulse;
					}

					param.Motor1_PWM = param.speed1_Outval;  
					param.Motor2_PWM = param.speed2_Outval;
				
					/* �Ҷȴ�������Ѳ�߲��� */
				#if defined(GRAY_EN)
					Line_Outval_last = Line_Outval;
					Line_Outval =  Get_Gray_Num();     //lineNum����PWM����װ��ֵһ�����ҲŻ������Ե�Ч���� ����ͨ���ı�Line_Outval��ֵ�����Ʋ���ֵ�ļ���
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
			/* ת��4���Ƕȴ��ٶ� */
			if(flag.spin_start_flag == 1) 
			{
				if (is_motor_en == 1)
				{
					if(fabs(yaw - pid_YAW.target_val) <= 3.0)
					{
						if(++count.spin_count >=  P_Waiting_for_TURN_10MS)  //30 * 10ms = 300ms	 //����ʱ���жϷų��㣬�Ա�ɲ��ͣ��
						{
							count.spin_count = 0;
							
							flag.spin_start_flag = 0;
							flag.spin_succeed_flag = 1;
							
							flag.move_at_any_speed_flag = 0;
							
							param.Motor1_PWM  = 0;
							param.Motor2_PWM = 0;
							pid_0_motor();
							motor_disable();
							
							Red_Turn();	//ʵ�ʲ�Ҫ
						}
					}
					//���������ƫ Ӧ����������� ���ָ�ֵ
					//�����ǽǶ����������� ʵ��ֵƫ�� err=tar-act<0Ϊ��ֵ
					//��������pwm=-yaw_out
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
		
		
		
		/* ����жϱ�־λ */
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
}
/*-------------�ö�ʱ����BEEP��ʱ����----------------------*/
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
	/* ��������ʱ���� */
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
/*----------------------��ʱ----------------------*/
void jishi_start(void)
{
	count.counting_time = 0;	//����ϴεļ�ʱ
	flag.counting_flag = 1;		//��ʼ��ʱ
}
void jishi_stop(void)
{
	flag.counting_flag = 0;
}
/*----------------------pidĳЩ��������----------------------*/
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
/*------------------------------------------------------------��̨------------------------------------------------------------*/
/*----------------------x----------------------*/
void PID_x_control(float *rotation)
{
	float cont_val = 0.0; 
	float actual_coordinate;	//ʵ������
	float max_rotate[2] = {1300, 1700};	//���λ��

	actual_coordinate = (float)C_red[0];
	
	cont_val = pid_x_realize(&pid_x, actual_coordinate);

	/* ����޷� */
	cont_val = (cont_val > pid_x.out_limit ? pid_x.out_limit : cont_val);
	cont_val = (cont_val < -pid_x.out_limit ? -pid_x.out_limit : cont_val);
	
	param.pid_x_Outval = cont_val;
	
	/* ��������Ƕ����������ٶȣ�������������Ƕȣ��ٶȻ��֣� */

	if (rotation[0] + param.pid_x_Outval > max_rotate[1])//��ת�Ƕ�����
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
	float max_rotate[2] = {1300, 1700};	//���λ��

	actual_coordinate = (float)C_red[1];
	
	cont_val = pid_y_realize(&pid_y, actual_coordinate);   

	/* Ŀ�����޴��� */
	cont_val = (cont_val > pid_y.out_limit ? pid_y.out_limit : cont_val);
	cont_val = (cont_val < -pid_y.out_limit ? -pid_y.out_limit : cont_val);
	
	param.pid_y_Outval = cont_val;
	
	/* ��������Ƕ����������ٶȣ�������������Ƕȣ��ٶȻ��֣� */

//	param.pid_y_Outval = Limit_float(param.pid_y_Outval, max_rotate[0], max_rotate[1]);
//	rotation[1] += param.pid_y_Outval;
	
	if (rotation[1] + param.pid_y_Outval > max_rotate[1])//��ת�Ƕ�����
		rotation[1] = max_rotate[1];
	else if (rotation[1] + param.pid_y_Outval < max_rotate[0])
		rotation[1] = max_rotate[0];
	else
		rotation[1] += param.pid_y_Outval;
}

// ����·����ֵ�㺯��
// ���ܣ������ڶ���֮�����ɾ��ȷֲ��Ĳ�ֵ�㣬ȷ�������ϸ���ֱ���ƶ�
void Path_Points_Get(void)
{
    uint8_t pointIndex = 0;
    
    // Ϊ�����ε�ÿ�������ɲ�ֵ��
    for (uint8_t edge = 0; edge < 4; edge++)
	{
        // ��ȡ��ǰ�ߵ������յ�����
        float x1 = pencil_points[edge][0];
        float y1 = pencil_points[edge][1];
        float x2 = pencil_points[(edge + 1) % 4][0];  // ��һ����������
        float y2 = pencil_points[(edge + 1) % 4][1];
        
        // ���߶������ɾ��ȷֲ��Ĳ�ֵ��
        for (uint8_t i = 0; i < POINTS_PER_EDGE; i++)
		{
            // ���Բ�ֵ��ʽ��x = x1 + (x2-x1) * t, y = y1 + (y2-y1) * t
            // ����tΪ��ֵ��������0��1
            float t = (float)i / (float)POINTS_PER_EDGE;
            path_points[pointIndex][0] = x1 + (x2 - x1) * t;
            path_points[pointIndex][1] = y1 + (y2 - y1) * t;
            pointIndex++;
        }
        
        // ��Ӷ��㣨��Ϊ��ǰ�ߵ��յ����һ���ߵ���㣩
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
    
    // Ϊ�����ε�ÿ�������ɲ�ֵ��
    for (uint8_t edge = 0; edge < 4; edge++)
	{
        // ��ȡ��ǰ�ߵ������յ�����
        float x1 = (float)black_points[edge][0];
        float y1 = (float)black_points[edge][1];
        float x2 = (float)black_points[(edge + 1) % 4][0];  // ��һ����������
        float y2 = (float)black_points[(edge + 1) % 4][1];
        
        // ���߶������ɾ��ȷֲ��Ĳ�ֵ��
        for (uint8_t i = 0; i < BLACK_POINTS_PER_EDGE; i++)
		{
            // ���Բ�ֵ��ʽ��x = x1 + (x2-x1) * t, y = y1 + (y2-y1) * t
            // ����tΪ��ֵ��������0��1
            float t = (float)i / (float)BLACK_POINTS_PER_EDGE;
            black_path_points[pointIndex][0] = x1 + (x2 - x1) * t;
            black_path_points[pointIndex][1] = y1 + (y2 - y1) * t;
            pointIndex++;
        }
        
        // ��Ӷ��㣨��Ϊ��ǰ�ߵ��յ����һ���ߵ���㣩
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
/*------------------------------------------------------------С��------------------------------------------------------------*/
/*----------------------ֱ��----------------------*/
//void Car_go(int32_t location_cm)
//{
//	//�ýǶȻ�ǰ����ǰ�Ƕ�����
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
//	param.Target_Journey_cm = location_cm;   //��ֹ��ʱ��PID������
//	
//	float Car_location = cm2pulse(location_cm);
//	
//	/* ���ز��� */
//	location_cm = K_CAR_GO * location_cm;
//	
//#if defined(BACK_CM_BUCHANG_EN)
//	/* ��ͷ���벹�� */
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
	//�ýǶȻ�ǰ����ǰ�Ƕ�����
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
	/* ��ͷ���벹�� */
	if (flag.back_180_flag_cm)
	{
		location_cm -= BACK_CM_BuChang_CM;
		flag.back_180_flag_cm = 0;
	}
#endif
	
	
	/* ���ز��� */
	location_cm = K_CAR_GO(location_cm) * location_cm;
	
	param.Target_Journey_cm = location_cm;   //��ֹ��ʱ��PID������
	
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

/*----------------------ת��----------------------*/
void spin_Turn(spin_dir_t zhuanxiang)	//��ʵת�򻷲�����Ҫ�ܾ�׼��ת���ֱ����ֱ��ʱ��Ѳ�ߺ�����������ͺ�
{
	flag.line_flag = 0;
	flag.stop_flag = 0;   //ִ��ת��ʱ����ֱ����ɵı�־λ����. �������һ����ֱ�У������ת�䣬������ҵ��������ֶ���
	flag.spin_start_flag = 1;   
	flag.spin_succeed_flag = 0;  

	param.Motor1_journey_cm = 0;
	param.Motor2_journey_cm = 0;
	Motor1_Pulse_Sigma = 0;
	Motor2_Pulse_Sigma = 0;
	
	//90���Ӧ��������
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
//		case right_90:	param.Target_Journey_cm = K_RIGHT_90 * Car_Turn_val;		break;	//Ŀ����벻�Ӹ���
//		case back_180:	param.Target_Journey_cm = 2 * K_BACK_180 * Car_Turn_val;	break;	//Ŀ����벻�Ӹ���
//		default:	break;
//	}
	param.Target_Journey_cm = fabs(Car_Turn_val);
	
/* ת��ǶȲ��� */
#if defined(TURN_BUCHANG_EN)
	switch(zhuanxiang)
	{
		case left_90:	flag.left_90_flag_angle  = 1;	break;	//��ת�ǶȲ���
		case right_90:	flag.right_90_flag_angle = 1;	break;	//��ת�ǶȲ���
		case back_180:	flag.back_180_flag_angle = 1;	break;	//��ͷ���벹��
		default:	break;
	}
#endif
	
/* ��ͷ���벹�� */
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
void spin_Turn_angle(float angle)	//����Ƕ�ת��//����
{
	flag.line_flag = 0;
	flag.stop_flag = 0;   //ִ��ת��ʱ����ֱ����ɵı�־λ����. �������һ����ֱ�У������ת�䣬������ҵ��������ֶ���
	flag.spin_start_flag = 1;
	flag.spin_succeed_flag = 0;

	param.Motor1_journey_cm = 0;
	param.Motor2_journey_cm = 0;
	Motor1_Pulse_Sigma = 0;
	Motor2_Pulse_Sigma = 0;
	
	float Car_Turn_val = angle2pulse(angle);

	//���ز���
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

void spin_Turn_YAW(float angle)	//����Ƕ�ת�� �����Ҹ�
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
void spin_Turn_speed(spin_dir_t zhuanxiang, uint16_t max_rpm)	//�����ٶ�ת��
{
	flag.move_at_any_speed_flag = 1;
	rpm_limit = max_rpm;
	spin_Turn(zhuanxiang);
}

/*---------------------��PID---------------------*/
void Location_Speed_control(void)
{
	if (is_motor_en == 1)
	{
		static uint8_t location_control_count = 0;
		location_control_count++;
		if(location_control_count >= 2)
		{
			location_control_count = 0; 
			
			/* λ�û��������Ϊ�ٶ� ���ٶȻ������� */
			param.location1_Outval = location1_pid_control();
			param.location2_Outval = location2_pid_control();
		}
		
		/* λ�û��������Ϊ�ٶȾ�set_pid_target�����ٶȻ� */
		set_pid_target(&pid_speed1, param.location1_Outval);		//ÿ�ζ�������λ�û���ֵ
		set_pid_target(&pid_speed2, param.location2_Outval);	//ÿ�ζ�������λ�û���ֵ
			
		/* param.speed1_Outval �������������ΪPWM */
		param.speed1_Outval = speed1_pid_control();				//Ҫ�ǵ��ת�򲻷���Ԥ�ڣ�������������ȡ����ֵ
		param.speed2_Outval = speed2_pid_control();
	}
}


float speed1_pid_control(void)  
{
    float cont_val = 0.0;	//��ǰ����ֵ
    float actual_speed;
	
	/* ��ǰ�ٶȣ�rpm�� = ��������ȡ������ * 60s / (44 * 20.41 * (10ms / 1000)) ����1000�ŵ�����ᱨ�� division by zero ���Էŵ�ǰ��*/
//	actual_speed = ((float)Motor1_Pulse_Unit * 60.0 * 1000.0)/(MAI_CHONG_PER_ROUND * PID_PERIOD);
	actual_speed = pulse2rpm(Motor1_Pulse_Unit);

	/* ��ǰ�ٶȴ��� speed_pid_realize �����ٶ�pid�㷨ʵ�� */
	cont_val = speed_pid_realize(&pid_speed1, actual_speed);	// ���� PID ����
    
	return cont_val;
}

float speed2_pid_control(void)  
{
   
    float cont_val = 0.0;	//��ǰ����ֵ
    float actual_speed;
	
	actual_speed = pulse2rpm(Motor2_Pulse_Unit);
	
    cont_val = speed_pid_realize(&pid_speed2, actual_speed);	// ���� PID ����
    
	return cont_val;
}


float location1_pid_control(void)  
{
	float cont_val = 0.0; 
	float actual_location;
	float target_speed_rpm_limit;

	actual_location = Motor1_Pulse_Sigma;
	
	/* ����ǰλ��actual_location(������)����location_pid_realize����λ��pid�㷨ʵ�� */
	/* Ȼ�����ֵ���أ�֮����Ϊ�ٶ�Ŀ��ֵ(RPM)�����ٶ�pid */			
	cont_val = location_pid_realize(&pid_location1, actual_location);   

	/* Ŀ���ٶ����޴��� */
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

	/* Ŀ���ٶ����޴��� */
	target_speed_rpm_limit = flag.move_at_any_speed_flag ? rpm_limit : pid_location2.out_limit;	//pid_location2.out_limit
	cont_val = Limit_float(cont_val, target_speed_rpm_limit, -target_speed_rpm_limit);
	
	return cont_val;
}
/* ת�� �Ƕȴ��ٶ� */
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
//			/* λ�û��������Ϊ�ٶ� ���ٶȻ������� */
//			param.location1_Outval = location1_pid_control();
//			param.location2_Outval = location2_pid_control();
//		}
		
		param.yaw_Outval = yaw_pid_control();
		
		/* λ�û��������Ϊ�ٶȾ�set_pid_target�����ٶȻ� */
		set_pid_target(&pid_speed1, -param.yaw_Outval);		//ÿ�ζ�������λ�û���ֵ
		set_pid_target(&pid_speed2, param.yaw_Outval);	//ÿ�ζ�������λ�û���ֵ
			
		/* param.speed1_Outval �������������ΪPWM */
		param.speed1_Outval = speed1_pid_control();				//Ҫ�ǵ��ת�򲻷���Ԥ�ڣ�������������ȡ����ֵ
		param.speed2_Outval = speed2_pid_control();
	}
}
/* ת�� �ǶȻ� */
float yaw_pid_control(void)
{
	float cont_val = 0.0; 
	float actual_yaw;
	
	actual_yaw = yaw;

    cont_val = YAW_pid_realize(&pid_YAW, actual_yaw);   

	/* ����޷� */
	cont_val = Limit_float(cont_val, pid_YAW.out_limit, -pid_YAW.out_limit);
	
	return cont_val;
}
