#ifndef __PARAMETER_H
#define __PARAMETER_H
#include "stm32f10x.h"                  // Device header

/* 圆周率 */
#define MY_PI	3.14159265359

void reset(void);
float Limit_uint32_t(uint32_t num, uint32_t upper, uint32_t lower);
float Limit_int32_t(int32_t num, int32_t upper, int32_t lower);
float Limit_uint16_t(uint16_t num, uint16_t upper, uint16_t lower);
float Limit_int16_t(int16_t num, int16_t upper, int16_t lower);
float Limit_float(float num, float upper, float lower);

/* 参数结构体初始化 */
typedef struct
{
	uint8_t uart_rx[50];
	uint8_t HMI_rx[50];
	
	uint16_t beep_ms;
	uint16_t red_ms;
	uint16_t green_ms;
	uint16_t yellow_ms;
	uint16_t blue_ms;
	uint16_t white_ms;
	
	/* 步进电机 */
	
	uint16_t stepx_run_ms;
	uint16_t stepx_stop_ms;
	uint32_t stepx_speed;
	uint8_t stepx_dir;
	
	
	/* 云台 */
	float pid_x_Outval;
	float pid_y_Outval;
	
	float pid2_x_Outval;
	float pid2_y_Outval;
	
	
	/* 小车 */
	float Target_Journey_cm;					//Car_go中设置的目标距离，单位cm，用于判断小车快到目标距离处需要在下一阶段任务中设置
	float Motor1_journey_cm, Motor2_journey_cm;	//电机行驶距离，单位cm
	float Motor1_PWM, Motor2_PWM;				//最终输出的PWM，单位PWM
	float speed1_Outval, speed2_Outval;			//速度环输出的PWM，单位PWM
	float location1_Outval, location2_Outval;	//位置环输出的目标速度，单位RPM，输入速度环	
	float yaw_Outval;
	
}Param_InitTypeDef;

/* 标志位结构体初始化 */
typedef struct
{
	uint8_t _flag;
	uint8_t beep_flag;		//蜂鸣器计时
	uint8_t red_flag;
	uint8_t green_flag;
	uint8_t yellow_flag;
	uint8_t blue_flag;
	uint8_t white_flag;
	
	uint8_t counting_flag;	//计时
	
	/* 云台 */
	
	uint8_t reset_flag;
	uint8_t stop_flag;
	uint8_t reach_flag;
	
	uint8_t wating_pencil_line_flag;
	
	uint8_t pencil_line_count_increase_flag;
	
	//获取铅笔角点
	uint8_t get_one_pencil_succeed_flag;
	//走铅笔线
	uint8_t pencil_line_flag;
	uint8_t pencil_line_succeed_flag;
	//获取矩形角点
	uint8_t black_pionts_get_flag;
	uint8_t black_path_pionts_get_succeed_flag;
	//走矩形框
	uint8_t black_line_flag;
	uint8_t black_line_succeed_flag;
	//复位追踪
	uint8_t green_reset_flag;
	uint8_t green_reset_succeed_flag;
	//运动追踪
	uint8_t zhuizong_flag;
	uint8_t zhuizong_succeed_flag;
	//目标值实际值
	uint8_t cur_catch_tar_flag;
	uint8_t tar_cur_flag;
	uint8_t tar_cur_succeed_flag;
	
	/* 小车 */
	uint8_t YAW_succeed_flag;			//YAW校准成功
	uint8_t find_satrt_flag;			//K210识别
	uint8_t spin_start_flag;			//开车始转弯
	uint8_t spin_succeed_flag;			//成功转弯
//	uint8_t stop_flag;					//停
	uint8_t TWO_wait_flag;				//二车等待
	uint8_t line_flag;					//直行（巡线）
	uint8_t back_180_flag_cm;			//倒转距离补偿
	uint8_t left_90_flag_angle;			//左转角度补偿
	uint8_t right_90_flag_angle;		//右转角度补偿
	uint8_t back_180_flag_angle;		//倒转角度补偿
	uint8_t blue_load_flag;				//蓝牙模拟装药物
	uint8_t move_at_any_speed_flag;		//以任意速度移动

	
}Flag_InitTypeDef;

/* 计数结构体初始化 */
typedef struct
{
	uint8_t count;
	uint16_t _count;
	
	
	uint16_t beep_count;	//蜂鸣器计时
	uint16_t red_count;		//蜂鸣器计时
	uint16_t green_count;
	uint16_t yellow_count;
	uint16_t blue_count;
	uint16_t white_count;
	
	uint32_t counting_time;	//计时
	
	/* 步进电机 */
	uint16_t stepx_count;
	
	/* 云台 */
	uint8_t pencil_count;	//记录五点坐标用
	uint8_t black_count_3;	//记录黑框PWM用 第三题

	uint16_t wating_pencil_line_count;	//铅笔线定点移动 等待舵机旋转到位
	uint32_t pencil_line_count;		//铅笔线（终点反馈） 用于整除取余
	uint8_t current_pencil_count;
	
	uint16_t black_spend_time_count;
	
	/* 小车 */
	uint8_t load_cont;					//检测药物节拍
	uint8_t BlueTooth_send_time_count;	//发送节拍：K210/双车蓝牙
	uint8_t OLED_Show_cont;				//OLED显示节拍
	uint16_t find_time_count;			//K210识别计时
	uint16_t stop_count, spin_count;	//刹车末尾计时，转弯计时
	uint16_t turn_bucahng_count;		//倒转补偿计数
	uint16_t TWO_wait_time_count;		//二车等待计时
	
	
}Count_InitTypeDef;

extern Param_InitTypeDef param;
extern Flag_InitTypeDef  flag;
extern Count_InitTypeDef count;

#endif


