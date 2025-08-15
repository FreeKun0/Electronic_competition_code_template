#ifndef __YT_H
#define __YT_H

#include "stm32f10x.h"
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


#include "control.h"
#include "Servo.h"
#include "Motor.h"
#include "Encoder.h"
#include "Infrared.h"
#include "mpu6050.h"
#include "parameter.h"
#include "bsp_pid.h"
#include "HWT101.h"
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

#define WATING_PENCIL_LINE_10MS		(1000 * 1)		//1000ms移动一条边
#define CHANGE_PWM	100

#define PWM_STEP_SMALL	3
#define PWM_STEP_LARGE	20

#include <math.h>
#include <stdarg.h>
//#define MY_PI 3.1415926535f
#define MAX_POINTS 1000
#define MAX_VERTICES 20  // 最大顶点数

// 图形类型枚举
typedef enum {
	SHAPE_LINE,             // 直线
	SHAPE_TRIANGLE,         // 三角形
	SHAPE_ROT_RECT,         // 倾斜矩形
	SHAPE_CIRCLE,           // 圆形
	SHAPE_POLYGON,          // 任意多边形
	SHAPE_REGULAR_POLYGON   // 正多边形（支持倾斜角度）
} ShapeType;

// 直线参数结构体
typedef struct {
	float x0, y0;           // 起点坐标
	float x1, y1;           // 终点坐标
	uint16_t segments;      // 分段数
} LineParams;

// 三角形参数结构体
typedef struct {
	float p1x, p1y;         // 顶点1坐标
	float p2x, p2y;         // 顶点2坐标
	float p3x, p3y;         // 顶点3坐标
	uint16_t segments_per_side;  // 每边分段数
} TriangleParams;

// 倾斜矩形参数结构体
typedef struct {
	float x0, y0;           // 左上角坐标
	float width;            // 宽度
	float height;           // 高度
	float angle;            // 倾斜角度（度）
	uint16_t segments_per_side;  // 每边分段数
} RotRectParams;

// 圆形参数结构体
typedef struct {
	float center_x;         // 圆心X坐标
	float center_y;         // 圆心Y坐标
	float radius;           // 半径
	uint16_t segments;      // 分段数（越大越圆）
} CircleParams;

// 任意多边形参数结构体
typedef struct {
	float vertices[MAX_VERTICES][2];  // 顶点数组 [i][0]=x, [i][1]=y
	uint8_t vertex_count;     // 顶点数量（≥3）
	uint16_t segments_per_side;  // 每边分段数
} PolygonParams;

// 正多边形参数结构体（支持倾斜角度）
typedef struct {
	float center_x;          // 中心X坐标
	float center_y;          // 中心Y坐标
	float radius;            // 外接圆半径
	uint8_t sides;           // 边数（≥3）
	float angle_deg;         // 倾斜角度（度）
	uint16_t segments_per_side;  // 每边分段数
} RegularPolygonParams;

// 通用图形状态结构体
typedef struct {
	uint8_t run_flag;        // 运行标志（1=运行，0=停止）
	uint8_t init_flag;       // 初始化标志（1=已初始化）
	uint16_t point_idx;      // 当前点索引
	uint16_t total_points;   // 总点数
	uint8_t period;          // 每个点的停留周期
	ShapeType curr_shape;    // 当前图形类型

	union {
		LineParams line;             // 直线参数
		TriangleParams tri;          // 三角形参数
		RotRectParams rect;          // 矩形参数
		CircleParams circle;         // 圆形参数
		PolygonParams polygon;       // 任意多边形参数
		RegularPolygonParams regular_poly;  // 正多边形参数
	} params;

} ShapeState;

// 全局共用点数组
extern float shared_x_points[MAX_POINTS];
extern float shared_y_points[MAX_POINTS];

// 全局状态变量
extern ShapeState shape_state;

// 函数声明
void GenerateLinePoints(float x1, float y1, float x2, float y2, uint16_t segments, 
					   float x_array[], float y_array[], uint16_t* start_idx);
void GeneratePolygonPoints(PolygonParams* poly, float x_array[], float y_array[], uint16_t* total_points);
void GenerateRegularPolygonVertices(RegularPolygonParams* regular_poly, float vertices[][2], uint8_t* vertex_count);
void GenerateShapePoints(void);

// 直线初始化函数
void Line_Init(float x0, float y0, float x1, float y1, uint16_t segments, uint16_t period);
// 三角形初始化函数
void Triangle_Init(float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, 
                  uint16_t segments_per_side, uint16_t period);
// 倾斜矩形初始化函数
void RotRect_Init(float x0, float y0, float width, float height, float angle_deg, 
                 uint16_t segments_per_side, uint16_t period);
// 圆形初始化函数
void Circle_Init(float center_x, float center_y, float radius, uint16_t segments, uint16_t period);
// 任意多边形初始化函数
void Polygon_Init(float(*vertices)[2], uint8_t vertex_count, uint16_t segments_per_side, uint16_t period);
// 正多边形初始化函数
void RegularPolygon_Init(float center_x, float center_y, float radius, uint8_t sides, 
                        float angle_deg, uint16_t segments_per_side, uint16_t period);
void Shape_Control(void);









	
void pencil_piont_get_1(void);
void pencil_piont_get_2(void);
void pencil_piont_get_3(void);
void red_piont_reset_1(void);
void red_piont_reset_2(void);
void pencil_line_1(void);		//巡铅笔线 开环 5PWM 直达
void pencil_line_2(void);		//巡铅笔线 闭环 pid 
void pencil_line_3(void);		//巡铅笔线 闭环 步进 终点反馈 5坐标
void pencil_line_4(void);		//巡铅笔线 闭环 步进 终点反馈 5PWM
void pencil_line_5(void);		//巡铅笔线 开环 5PWM 步进
void black_piont_get_guding_1(void);	//题目三 获取固定黑框PWM
void black_line_guding_1(void);	//题目三 走固定黑框
void black_piont_get_suiji_1(void);
void black_line_1(void);
void reset_zhuizong_1(void);
void yundong_zhuizong_1(void);
void tar_cur_pionts(void);

extern uint16_t pencil_points_PWM[5][2];
extern uint16_t black_points_PWM_guding[4][2];
extern uint16_t PWM1_for_TASK1;
extern uint16_t PWM2_for_TASK1;
extern uint16_t PWM_step_for_TASK1;	//步进10PWM，KEY3切换为1PWM

#endif
