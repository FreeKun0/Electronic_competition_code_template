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

/*-------------------------------------宏定义-------------------------------------------*/

/* 上位机调参 */
//#define PID_DEBUG_EN
#define PID_DEBUG_MODE	6	//1：电机1速度环，2：电机2速度环，3：电机1位置速度环，4：电机2位置速度环，5：角度环
//mode1:debug设置pid_speed1.target_val（RPM），上位机显示目标RPM，实际RPM，输出PWM
//mode3:debug设置pid_location1.target_val(脉冲数（一圈20.41*11*4=898.04）)，上位机显示目标脉冲，实际脉冲，输出目标RPM（贴个胶带看是不是转了1圈）

/*在sysTick里调用PID计算的周期，以毫秒为单位*/
#define PID_PERIOD  			cycle_period_ms    //定时器中断执行测速及pid等的周期

/*------------------------小车----------------------------*/
#define LUN_JU			15.6	//cm
#define WheelR			3.25	//cm
/* 310 */
#define ENCODER_RESOLUTION			11		//编码器物理分辨率
#define ENCODER_TOTAL_RESOLUTION	(ENCODER_RESOLUTION * 4)	//4倍频后的总分辨率
#define JIAN_SU_BI					20.41	//电机减速比
#define MAI_CHONG_PER_ROUND			( JIAN_SU_BI * ENCODER_TOTAL_RESOLUTION )	//每圈的脉冲数

#define TARGET_SPEED_RPM_LIMIT		150	// 60rpm可以3s走完60cm	////这里得看一下跑道 等三次2s 剩余14s 360cm/14s = 25.7cm/s   25.7/20*120=154

/* 灰度 */
//#define GRAY_EN
/* 直线角度环 */
#define LINE_YAW_EN

/* 倒头距离补偿 */
//#define BACK_CM_BUCHANG_EN
#define BACK_CM_BuChang_CM			1.5		//cm, 后转之后的下一次直行减去这个距离

/* 比例系数 */
#define K_A			0.3			//增加负重后减小这个系数
#define K_CAR_GO(x)	( 1 + 1 / (1 + K_A * x) )
//#define K_CAR_GO	1.05	//与 车身重量 和 摆头程度 正相关
#define K_LEFT_90	1.05	//与 车身重量 正相关，减小此数和增大转向角度PWM补偿效果相似？
#define K_RIGHT_90	1.05
#define K_BACK_180	1.05
#define K_ANGLE		1.05	//任意角度的补偿 不是陀螺仪的

/* 直走和转弯等待时间 */
#define P_Waiting_for_GO_10MS			30		//10ms, 30 * 10ms = 300ms内停稳
#define P_Waiting_for_TURN_TOTAL_10MS	150		//10ms, 150 * 10ms = 1500ms内转完
#define P_Waiting_for_TURN_10MS			70		//10ma, 30 * 10ms = 300ms转稳


/*------------------------舵机----------------------------*/

// 路径点数组 - 存储正方形四条边上的所有插值点
// 每条边生成10个插值点，加上4个顶点，共44个点
#define POINTS_PER_EDGE		10				// 每条边的插值点数
#define PATTH_POINTS_NUM	(POINTS_PER_EDGE * 4 + 4)

//黑框 路径点
#define BLACK_POINTS_PER_EDGE		10				// 每条边的插值点数
#define BLACK_PATTH_POINTS_NUM	(BLACK_POINTS_PER_EDGE * 4 + 4)

typedef enum
{
	left_90,
	right_90,
	back_180
}spin_dir_t;
/*-------------------------------------全局变量-------------------------------------------*/
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
extern uint8_t KeyNum;			//接收按键键码
extern uint8_t KeyBoardNum;	//接收矩阵按键键码
extern uint8_t IR_Num;			//接收红外遥控键码
extern uint8_t Usart2Num;

/* 云台 */
extern uint16_t C_red[2];
extern uint16_t C_red_last[2];
extern uint16_t C_red_last_last[2];
//extern uint16_t rotation_red[2];
extern float rotation_red[2];

//云台2
//extern uint16_t C_green[2];
//extern uint16_t C_green_last[2];
//extern uint16_t C_green_last_last[2];
//extern uint16_t rotation_green[2];

extern uint16_t C_tar[2];
extern uint16_t C_tar_last[2];
extern uint16_t C_tar_last_last[2];

extern uint16_t pencil_points[5][2];
extern uint16_t black_points[4][2];

extern float path_points[PATTH_POINTS_NUM][2];	// 路径点坐标数组
extern uint8_t total_path_points;				// 总路径点数
extern uint8_t current_path_point;				// 当前路径点索引

extern float black_path_points[BLACK_PATTH_POINTS_NUM][2];		// 路径点坐标数组
extern uint8_t black_total_path_points;				// 总路径点数
extern uint8_t black_current_path_point;             // 当前路径点索引


/* 小车 */
extern uint16_t rpm_limit;
extern float Line_Outval;
extern float Line_Outval_last;

/*-------------------------------------函数声明-------------------------------------------*/
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

/* 云台 */
void PID_x_control(float *rotation);
void PID_y_control(float *rotation);

void Path_Points_Get(void);
void black_path_points_get(void);

/* 小车 */
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
void spin_Turn_angle(float angle);	//任意角度转向
void spin_Turn_speed(spin_dir_t zhuanxiang, uint16_t max_rpm);
void spin_Turn_YAW(float angle);


void get_yaw(void);
void set_yaw_tar(float tar_yaw);
#endif 

