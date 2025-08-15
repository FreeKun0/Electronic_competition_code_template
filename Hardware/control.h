#ifndef __CONTROL_H
#define __CONTROL_H
#include "Delay.h"
#include "bsp_pid.h"
#include <math.h>

#include "Serial.h"
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
#include "AD.h"
#include "laser.h"


#include "control.h"
#include "Servo.h"
#include "Motor.h"
#include "Encoder.h"
#include "Infrared.h"
#include "mpu6050.h"
#include "parameter.h"
#include "bsp_pid.h"
#include "HWT101.h"
#include "stepper.h"

/*-------------------------------------�궨��-------------------------------------------*/

/* ��λ������ */
//#define PID_DEBUG_EN
#define PID_DEBUG_MODE	6	//1�����1�ٶȻ���2�����2�ٶȻ���3�����1λ���ٶȻ���4�����2λ���ٶȻ���5���ǶȻ�
//mode1:debug����pid_speed1.target_val��RPM������λ����ʾĿ��RPM��ʵ��RPM�����PWM
//mode3:debug����pid_location1.target_val(��������һȦ20.41*11*4=898.04��)����λ����ʾĿ�����壬ʵ�����壬���Ŀ��RPM�������������ǲ���ת��1Ȧ��

/*��sysTick�����PID��������ڣ��Ժ���Ϊ��λ*/
#define PID_PERIOD  			cycle_period_ms    //��ʱ���ж�ִ�в��ټ�pid�ȵ�����

/*------------------------С��----------------------------*/
#define LUN_JU			15.6	//cm
#define WheelR			3.25	//cm
/* 310 */
#define ENCODER_RESOLUTION			11		//����������ֱ���
#define ENCODER_TOTAL_RESOLUTION	(ENCODER_RESOLUTION * 4)	//4��Ƶ����ֱܷ���
#define JIAN_SU_BI					20.41	//������ٱ�
#define MAI_CHONG_PER_ROUND			( JIAN_SU_BI * ENCODER_TOTAL_RESOLUTION )	//ÿȦ��������

#define TARGET_SPEED_RPM_LIMIT		150	// 60rpm����3s����60cm	////����ÿ�һ���ܵ� ������2s ʣ��14s 360cm/14s = 25.7cm/s   25.7/20*120=154

/* �Ҷ� */
//#define GRAY_EN
/* ֱ�߽ǶȻ� */
#define LINE_YAW_EN

/* ��ͷ���벹�� */
//#define BACK_CM_BUCHANG_EN
#define BACK_CM_BuChang_CM			1.5		//cm, ��ת֮�����һ��ֱ�м�ȥ�������

/* ����ϵ�� */
#define K_A			0.3			//���Ӹ��غ��С���ϵ��
#define K_CAR_GO(x)	( 1 + 1 / (1 + K_A * x) )
//#define K_CAR_GO	1.05	//�� �������� �� ��ͷ�̶� �����
#define K_LEFT_90	1.05	//�� �������� ����أ���С����������ת��Ƕ�PWM����Ч�����ƣ�
#define K_RIGHT_90	1.05
#define K_BACK_180	1.05
#define K_ANGLE		1.05	//����ǶȵĲ��� ���������ǵ�

/* ֱ�ߺ�ת��ȴ�ʱ�� */
#define P_Waiting_for_GO_10MS			30		//10ms, 30 * 10ms = 300ms��ͣ��
#define P_Waiting_for_TURN_TOTAL_10MS	150		//10ms, 150 * 10ms = 1500ms��ת��
#define P_Waiting_for_TURN_10MS			70		//10ma, 30 * 10ms = 300msת��


/*------------------------���----------------------------*/

// ·�������� - �洢�������������ϵ����в�ֵ��
// ÿ��������10����ֵ�㣬����4�����㣬��44����
#define POINTS_PER_EDGE		10				// ÿ���ߵĲ�ֵ����
#define PATTH_POINTS_NUM	(POINTS_PER_EDGE * 4 + 4)

//�ڿ� ·����
#define BLACK_POINTS_PER_EDGE		10				// ÿ���ߵĲ�ֵ����
#define BLACK_PATTH_POINTS_NUM	(BLACK_POINTS_PER_EDGE * 4 + 4)

typedef enum
{
	left_90,
	right_90,
	back_180
}spin_dir_t;
/*-------------------------------------ȫ�ֱ���-------------------------------------------*/
extern uint8_t TASK;
extern uint8_t last_TASK;
extern uint8_t next_TASK;

extern uint8_t Do_count1;
extern uint8_t Do_count2;
extern uint8_t Do_count3;
extern uint8_t Do_count4;
extern uint8_t Do_count5;
extern uint8_t Do_count6;
extern uint8_t Do_count7;
extern uint8_t Do_count8;
extern uint8_t Do_count9;
extern uint8_t Do_count10;
extern uint8_t Do_count11;
extern uint8_t Do_count12;
extern uint8_t KeyNum;			//���հ�������
extern uint8_t KeyBoardNum;	//���վ��󰴼�����
extern uint8_t IR_Num;			//���պ���ң�ؼ���
extern uint8_t Usart2Num;

/* ��̨ */
extern uint16_t C_red[2];
extern uint16_t C_red_last[2];
extern uint16_t C_red_last_last[2];
//extern uint16_t rotation_red[2];
extern float rotation_red[2];

//��̨2
//extern uint16_t C_green[2];
//extern uint16_t C_green_last[2];
//extern uint16_t C_green_last_last[2];
//extern uint16_t rotation_green[2];

extern uint16_t C_tar[2];
extern uint16_t C_tar_last[2];
extern uint16_t C_tar_last_last[2];

extern uint16_t pencil_points[5][2];
extern uint16_t black_points[4][2];

extern float path_points[PATTH_POINTS_NUM][2];	// ·������������
extern uint8_t total_path_points;				// ��·������
extern uint8_t current_path_point;				// ��ǰ·��������

extern float black_path_points[BLACK_PATTH_POINTS_NUM][2];		// ·������������
extern uint8_t black_total_path_points;				// ��·������
extern uint8_t black_current_path_point;             // ��ǰ·��������


/* С�� */
extern uint16_t rpm_limit;
extern float Line_Outval;
extern float Line_Outval_last;

/*-------------------------------------��������-------------------------------------------*/
void LED_BEEP_Tick(void);
void Beep_ms_TIM(uint16_t xms);
void Red_ms_TIM(uint16_t xms);
void Green_ms_TIM(uint16_t xms);
void Yellow_ms_TIM(uint16_t xms);
#if defined(LED_BLUE)
void Blue_ms_TIM(uint16_t xms);
#endif
#if defined(LED_WHITE)
void White_ms_TIM(uint16_t xms);
#endif
void jishi_stop(void);
void jishi_start(void);

void pid_0(_pid *pid);
void pid_0_motor(void);

/* ��̨ */
void PID_x_control(float *rotation);
void PID_y_control(float *rotation);

void Path_Points_Get(void);
void black_path_points_get(void);

/* С�� */
float speed1_pid_control(void);
float location1_pid_control(void);
float speed2_pid_control(void);
float location2_pid_control(void);
float yaw_pid_control(void);

void Location_Speed_control(void);
void yaw_speed_control(void);

void Car_go(int32_t location_cm);
void Car_go_speed(int32_t location_cm, uint16_t max_rpm);
void spin_Turn(spin_dir_t zhuanxiang);
void spin_Turn_angle(float angle);	//����Ƕ�ת��
void spin_Turn_speed(spin_dir_t zhuanxiang, uint16_t max_rpm);
void spin_Turn_YAW(float angle);


void get_yaw(void);
void set_yaw_tar(float tar_yaw);
#endif 

