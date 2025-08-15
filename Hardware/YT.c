#include "YT.h"
#include <math.h>


uint16_t pencil_points_PWM[5][2] =
{	1660, 1300,		//左上
	1355, 1300,		//右上
	1355, 1594,		//右下
	1655, 1599,		//左下
	1506, 1450		//原点
};
uint16_t black_points_PWM_guding[4][2] =
{	1595, 1385,		//左上
	1415, 1385,		//右上
	1415, 1512,		//右下
	1595, 1512,		//左下
};
uint16_t PWM1_for_TASK1 = 1500;
uint16_t PWM2_for_TASK1 = 1500;
uint16_t PWM_step_for_TASK1 = PWM_STEP_LARGE;	//步进10PWM，KEY3切换为1PWM

#include "YT.h"
#include "laser.h"  // 激光控制函数（Laser_P_ON/Laser_P_OFF）

// 全局共用点数组初始化
float shared_x_points[MAX_POINTS] = {0};
float shared_y_points[MAX_POINTS] = {0};

// 全局状态变量初始化
ShapeState shape_state = {0};

// 直线点生成函数：生成两点之间的插值点
void GenerateLinePoints(float x1, float y1, float x2, float y2, uint16_t segments, 
					   float x_array[], float y_array[], uint16_t* start_idx)
{
	uint16_t idx = *start_idx;
	float x_step = (x2 - x1) / segments;  // X方向步长
	float y_step = (y2 - y1) / segments;  // Y方向步长

	// 生成直线上的点（包含起点和终点）
	for (uint16_t i = 0; i <= segments; i++)
	{
		if (idx >= MAX_POINTS)
			break;  // 防止数组越界
		x_array[idx] = x1 + x_step * i;
		y_array[idx] = y1 + y_step * i;
		idx++;
	}

	*start_idx = idx;  // 更新起始索引（供下一条边使用）
}

// 任意多边形点生成函数：按顶点顺序连接成闭合图形
void GeneratePolygonPoints(PolygonParams* poly, float x_array[], float y_array[], uint16_t* total_points)
{
	uint16_t start_idx = 0;
	uint8_t v_count = poly->vertex_count;

	// 至少需要3个顶点
	if (v_count < 3)
	{
		*total_points = 0;
		return;
	}

	// 依次连接每个顶点（v0→v1, v1→v2, ..., vn-1→v0）
	for (uint8_t i = 0; i < v_count; i++)
	{
		// 当前顶点和下一个顶点（最后一个顶点连接回第一个）
		uint8_t next_i = (i + 1) % v_count;
		float x1 = poly->vertices[i][0];
		float y1 = poly->vertices[i][1];
		float x2 = poly->vertices[next_i][0];
		float y2 = poly->vertices[next_i][1];

		// 生成当前边的点（第一条边保留起点，后续边跳过重复点）
		uint16_t segments = poly->segments_per_side;
		if (i > 0)
			segments--;  // 跳过重复顶点

		GenerateLinePoints(x1, y1, x2, y2, segments, x_array, y_array, &start_idx);
	}

	*total_points = start_idx;
}

// 正多边形顶点计算函数：根据中心、半径、边数和倾斜角度生成顶点
void GenerateRegularPolygonVertices(RegularPolygonParams* regular_poly, float vertices[][2], uint8_t* vertex_count)
{
	*vertex_count = 0;
	uint8_t sides = regular_poly->sides;
	if (sides < 3)
		return;  // 边数至少为3

	float center_x = regular_poly->center_x;
	float center_y = regular_poly->center_y;
	float radius = regular_poly->radius;
	float angle_rad = regular_poly->angle_deg * MY_PI / 180.0f;  // 倾斜角度（弧度）
	float step_angle = 2.0f * MY_PI / sides;  // 每个顶点的角度间隔

	// 计算每个顶点的坐标
	for (uint8_t i = 0; i < sides; i++)
	{
		float curr_angle = angle_rad + step_angle * i;  // 当前顶点的角度（含倾斜）
		vertices[i][0] = center_x + radius * cos(curr_angle);
		vertices[i][1] = center_y + radius * sin(curr_angle);
		(*vertex_count)++;
	}
}

// 生成图形所有点的总函数
void GenerateShapePoints(void)
{
	uint16_t start_idx = 0;
	shape_state.total_points = 0;

	switch (shape_state.curr_shape)
	{
		case SHAPE_LINE:
		{
			// 生成直线点
			GenerateLinePoints(
				shape_state.params.line.x0, shape_state.params.line.y0,
				shape_state.params.line.x1, shape_state.params.line.y1,
				shape_state.params.line.segments,
				shared_x_points, shared_y_points, &start_idx
			);
			shape_state.total_points = start_idx;
			break;
		}

		case SHAPE_TRIANGLE:
		{
			uint16_t seg = shape_state.params.tri.segments_per_side;

			// 第一条边：p1→p2
			GenerateLinePoints(
				shape_state.params.tri.p1x, shape_state.params.tri.p1y,
				shape_state.params.tri.p2x, shape_state.params.tri.p2y,
				seg, shared_x_points, shared_y_points, &start_idx
			);

			// 第二条边：p2→p3（跳过重复点）
			GenerateLinePoints(
				shape_state.params.tri.p2x, shape_state.params.tri.p2y,
				shape_state.params.tri.p3x, shape_state.params.tri.p3y,
				seg - 1, shared_x_points, shared_y_points, &start_idx
			);

			// 第三条边：p3→p1（跳过重复点）
			GenerateLinePoints(
				shape_state.params.tri.p3x, shape_state.params.tri.p3y,
				shape_state.params.tri.p1x, shape_state.params.tri.p1y,
				seg - 1, shared_x_points, shared_y_points, &start_idx
			);

			shape_state.total_points = start_idx;
			break;
		}

		case SHAPE_ROT_RECT:
		{
			uint16_t seg = shape_state.params.rect.segments_per_side;
			float x0 = shape_state.params.rect.x0;
			float y0 = shape_state.params.rect.y0;
			float w = shape_state.params.rect.width;
			float h = shape_state.params.rect.height;
			float angle_rad = shape_state.params.rect.angle * MY_PI / 180.0f;
			float cos_ang = cos(angle_rad);
			float sin_ang = sin(angle_rad);

			// 计算矩形四个顶点
			float p0x = x0, p0y = y0;
			float p1x = x0 + w * cos_ang, p1y = y0 + w * sin_ang;
			float p2x = p1x - h * sin_ang, p2y = p1y + h * cos_ang;
			float p3x = x0 - h * sin_ang, p3y = y0 + h * cos_ang;

			// 四条边：p0→p1→p2→p3→p0
			GenerateLinePoints(p0x, p0y, p1x, p1y, seg, shared_x_points, shared_y_points, &start_idx);
			GenerateLinePoints(p1x, p1y, p2x, p2y, seg - 1, shared_x_points, shared_y_points, &start_idx);
			GenerateLinePoints(p2x, p2y, p3x, p3y, seg - 1, shared_x_points, shared_y_points, &start_idx);
			GenerateLinePoints(p3x, p3y, p0x, p0y, seg - 1, shared_x_points, shared_y_points, &start_idx);

			shape_state.total_points = start_idx;
			break;
		}

		case SHAPE_CIRCLE:
		{
			uint16_t segments = shape_state.params.circle.segments;
			float center_x = shape_state.params.circle.center_x;
			float center_y = shape_state.params.circle.center_y;
			float radius = shape_state.params.circle.radius;

			// 用参数方程生成圆周点
			for (uint16_t i = 0; i <= segments; i++)
			{
				if (start_idx >= MAX_POINTS)
					break;
				float angle = 2.0f * MY_PI * i / segments;  // 角度（弧度）
				shared_x_points[start_idx] = center_x + radius * cos(angle);
				shared_y_points[start_idx] = center_y + radius * sin(angle);
				start_idx++;
			}

			shape_state.total_points = start_idx;
			break;
		}

		case SHAPE_POLYGON:
		{
			// 生成任意多边形点
			GeneratePolygonPoints(
				&shape_state.params.polygon,
				shared_x_points, shared_y_points, &start_idx
			);
			shape_state.total_points = start_idx;
			break;
		}

		case SHAPE_REGULAR_POLYGON:
		{
			// 生成正多边形顶点
			float vertices[MAX_VERTICES][2];
			uint8_t vertex_count = 0;
			GenerateRegularPolygonVertices(
				&shape_state.params.regular_poly,
				vertices, &vertex_count
			);

			// 转换为任意多边形参数并生成点
			PolygonParams temp_poly;
			temp_poly.vertex_count = vertex_count;
			temp_poly.segments_per_side = shape_state.params.regular_poly.segments_per_side;
			for (uint8_t i = 0; i < vertex_count; i++)
			{
				temp_poly.vertices[i][0] = vertices[i][0];
				temp_poly.vertices[i][1] = vertices[i][1];
			}

			GeneratePolygonPoints(&temp_poly, shared_x_points, shared_y_points, &start_idx);
			shape_state.total_points = start_idx;
			break;
		}
	}
}

// 直线初始化函数
void Line_Init(float x0, float y0, float x1, float y1, uint16_t segments, uint16_t period)
{
    // 重置状态
    shape_state.run_flag = 1;
    shape_state.init_flag = 0;
    shape_state.point_idx = 0;
    shape_state.curr_shape = SHAPE_LINE;

    // 设置直线参数
    shape_state.params.line.x0 = x0;
    shape_state.params.line.y0 = y0;
    shape_state.params.line.x1 = x1;
    shape_state.params.line.y1 = y1;
    shape_state.params.line.segments = segments;
    shape_state.period = period;
}

// 三角形初始化函数
void Triangle_Init(float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, 
                  uint16_t segments_per_side, uint16_t period)
{
    // 重置状态
    shape_state.run_flag = 1;
    shape_state.init_flag = 0;
    shape_state.point_idx = 0;
    shape_state.curr_shape = SHAPE_TRIANGLE;

    // 设置三角形参数
    shape_state.params.tri.p1x = p1x;
    shape_state.params.tri.p1y = p1y;
    shape_state.params.tri.p2x = p2x;
    shape_state.params.tri.p2y = p2y;
    shape_state.params.tri.p3x = p3x;
    shape_state.params.tri.p3y = p3y;
    shape_state.params.tri.segments_per_side = segments_per_side;
    shape_state.period = period;
}

// 倾斜矩形初始化函数
void RotRect_Init(float x0, float y0, float width, float height, float angle_deg, 
                 uint16_t segments_per_side, uint16_t period)
{
    // 重置状态
    shape_state.run_flag = 1;
    shape_state.init_flag = 0;
    shape_state.point_idx = 0;
    shape_state.curr_shape = SHAPE_ROT_RECT;

    // 设置矩形参数
    shape_state.params.rect.x0 = x0;
    shape_state.params.rect.y0 = y0;
    shape_state.params.rect.width = width;
    shape_state.params.rect.height = height;
    shape_state.params.rect.angle = angle_deg;
    shape_state.params.rect.segments_per_side = segments_per_side;
    shape_state.period = period;
}

// 圆形初始化函数
void Circle_Init(float center_x, float center_y, float radius, uint16_t segments, uint16_t period)
{
    // 重置状态
    shape_state.run_flag = 1;
    shape_state.init_flag = 0;
    shape_state.point_idx = 0;
    shape_state.curr_shape = SHAPE_CIRCLE;

    // 设置圆形参数
    shape_state.params.circle.center_x = center_x;
    shape_state.params.circle.center_y = center_y;
    shape_state.params.circle.radius = radius;
    shape_state.params.circle.segments = segments;
    shape_state.period = period;
}

// 任意多边形初始化函数
void Polygon_Init(float(*vertices)[2], uint8_t vertex_count, uint16_t segments_per_side, uint16_t period)
{
    // 重置状态
    shape_state.run_flag = 1;
    shape_state.init_flag = 0;
    shape_state.point_idx = 0;
    shape_state.curr_shape = SHAPE_POLYGON;

    // 保存顶点（限制最大顶点数）
    shape_state.params.polygon.vertex_count = (vertex_count <= MAX_VERTICES) ? vertex_count : MAX_VERTICES;
    for (uint8_t i = 0; i < shape_state.params.polygon.vertex_count; i++)
    {
        shape_state.params.polygon.vertices[i][0] = vertices[i][0];
        shape_state.params.polygon.vertices[i][1] = vertices[i][1];
    }
    shape_state.params.polygon.segments_per_side = (segments_per_side > 4) ? segments_per_side : 10;
    shape_state.period = period;
}

// 正多边形初始化函数
void RegularPolygon_Init(float center_x, float center_y, float radius, uint8_t sides, 
                        float angle_deg, uint16_t segments_per_side, uint16_t period)
{
    // 重置状态
    shape_state.run_flag = 1;
    shape_state.init_flag = 0;
    shape_state.point_idx = 0;
    shape_state.curr_shape = SHAPE_REGULAR_POLYGON;

    // 设置正多边形参数
    shape_state.params.regular_poly.center_x = center_x;
    shape_state.params.regular_poly.center_y = center_y;
    shape_state.params.regular_poly.radius = radius;
    shape_state.params.regular_poly.sides = sides;
    shape_state.params.regular_poly.angle_deg = angle_deg;
    shape_state.params.regular_poly.segments_per_side = segments_per_side;
    shape_state.period = period;

    // 确保边数≥3
    if (shape_state.params.regular_poly.sides < 3)
        shape_state.params.regular_poly.sides = 3;
}

// 图形控制函数（定时中断中调用）
void Shape_Control(void)
{
	if (shape_state.run_flag != 1)
		return;

	// 首次运行：生成图形所有点
	if (shape_state.init_flag == 0)
	{
		GenerateShapePoints();
		shape_state.init_flag = 1;
		shape_state.point_idx = 0;
		Laser_P_OFF();  // 关闭激光，准备移动到起点
		return;
	}

	// 阶段1：移动到第一个点（激光关闭，等待50个周期）
	if (shape_state.point_idx == 0)
	{
		static uint16_t move_to_first_cnt = 0;

		// 持续发送第一个点的坐标
		extern float rotation_red[2];
		extern void Servo_All_SetPWM(float, float);
		rotation_red[0] = shared_x_points[0];
		rotation_red[1] = shared_y_points[0];
		Servo_All_SetPWM(rotation_red[0], rotation_red[1]);

		// 等待50个周期确保到位
		if (move_to_first_cnt < 50)
		{
			move_to_first_cnt++;
		}
		else
		{
			move_to_first_cnt = 0;
			Laser_P_ON();  // 到达起点，开启激光
			shape_state.point_idx++;
		}
		return;
	}

	// 阶段2：正常绘制（激光开启）
	if (shape_state.point_idx < shape_state.total_points && shape_state.point_idx < MAX_POINTS)
	{
		// 发送当前点坐标
		extern float rotation_red[2];
		extern void Servo_All_SetPWM(float, float);
		rotation_red[0] = shared_x_points[shape_state.point_idx];
		rotation_red[1] = shared_y_points[shape_state.point_idx];
		Servo_All_SetPWM(rotation_red[0], rotation_red[1]);

		// 按周期切换到下一个点
		static uint8_t delay_cnt = 0;
		delay_cnt++;
		if (delay_cnt >= shape_state.period)
		{
			delay_cnt = 0;
			shape_state.point_idx++;

			// 绘制完成：关闭激光
			if (shape_state.point_idx >= shape_state.total_points)
			{
				shape_state.run_flag = 0;
				shape_state.init_flag = 0;
				Laser_P_OFF();
			}
		}
	}
	else
	{
		// 异常处理：超出点范围
		shape_state.run_flag = 0;
		shape_state.init_flag = 0;
		Laser_P_OFF();
	}
}















/* TASK1 获取铅笔线顶点、原点 记录像素坐标 */
//void pencil_piont_get_1(void)
//{
//	switch(Do_count1)
//	{
//		case 0:
//			if(KeyNum == KEY5_PRES)
//			{
//				Do_count1++;
//				USART3_SendByte('1');
//			}
//		break;
//		case 1:
////			if(USART3_GetRxFlag() || KeyNum == KEY3_PRES)
//			if(flag.get_one_pencil_succeed_flag == 1 || KeyNum == KEY3_PRES)
//			{
//				flag.get_one_pencil_succeed_flag = 0;
//				
//				count.pencil_count++;
//				if(count.pencil_count < 5)
//				{
//					Do_count1 = 0;
//					Beep_ms(250);
//				}
//				else
//				{
//					Beep_ms(125);
//					Delay_ms(125);
//					Beep_ms(125);
//					Do_count1 = 0;
//					count.pencil_count = 0;
//				}
//			}
//		break;
//	}
//}

/* TASK1 获取铅笔线顶点、原点 记录PWM */
void pencil_piont_get_2(void)
{
	if(Usart2Num == 0xA0)	//KEY3 切换PWM步进大小
	{
		PWM_step_for_TASK1 = (PWM_step_for_TASK1 == PWM_STEP_LARGE ? PWM_STEP_SMALL : PWM_STEP_LARGE);
	}
	switch(Do_count1)
	{
		case 0:	//KEY5 开始移动
//			if(KeyNum == KEY5_PRES)
			if(1)
			{
				Do_count1++;
			}
		break;
		case 1:
//			if(KeyNum == KEY_UP_PRES)
//			{
////				PWM2_for_TASK1 -= PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
////				Servo2_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[1] -= PWM_step_for_TASK1;
//				Servo1_SetPWM(rotation_red[1]);
//				Servo21_SetPWM(rotation_red[1]);
//				
//			}
//			else if(KeyNum == KEY_DOWN_PRES)
//			{
////				PWM2_for_TASK1 += PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
////				Servo2_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[1] += PWM_step_for_TASK1;
//				Servo2_SetPWM(rotation_red[1]);
//				Servo22_SetPWM(rotation_red[1]);
//			}
//			else if(KeyNum == KEY_LEFT_PRES)
//			{
////				PWM1_for_TASK1 += PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
////				Servo2_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[0] += PWM_step_for_TASK1;
//				Servo1_SetPWM(rotation_red[0]);
//				Servo21_SetPWM(rotation_red[0]);
//			}
//			else if(KeyNum == KEY_RIGHT_PRES)
//			{
////				PWM1_for_TASK1 -= PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
////				Servo2_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[0] -= PWM_step_for_TASK1;
//				Servo1_SetPWM(rotation_red[0]);
//				Servo21_SetPWM(rotation_red[0]);
//			}
			
//			if(KeyNum == KEY5_PRES)
			if(Usart2Num == 0xA1)
			{
				Beep_ms(125);
				pencil_points_PWM[count.pencil_count][0] = rotation_red[0];
				pencil_points_PWM[count.pencil_count][1] = rotation_red[1];
				
				count.pencil_count++;
				if(count.pencil_count >= 5)
				{
					Delay_ms(125);
					Beep_ms(125);
					count.pencil_count = 0;
					Do_count1 = 0;
				}
			}
		break;
	}
}

/* TASK1 获取铅笔线顶点、原点 记录像素坐标和PWM */
void pencil_piont_get_3(void)
{
	if(KeyNum == KEY3_PRES)	//KEY3 切换PWM步进大小
	{
		PWM_step_for_TASK1 = (PWM_step_for_TASK1 == PWM_STEP_LARGE ? PWM_STEP_SMALL : PWM_STEP_LARGE);
	}
	switch(Do_count1)
	{
		case 0:	//KEY5 开启TASK1
			if(KeyNum == KEY5_PRES)
			{
				Do_count1++;
				
				count.pencil_count = 0;
			}
		break;
		case 1:
			/* 获取坐标 */
			if(KeyNum == KEY5_PRES)	//移动到位
			{
				//KEY5 记录PWM
				pencil_points_PWM[count.pencil_count][0] = rotation_red[0];	
				pencil_points_PWM[count.pencil_count][1] = rotation_red[1];
				
				//KEY5 给K210消息 准备接收坐标
				K210_SendByte('1');	
			}
			if(USART3_GetRxFlag())	//接收到坐标后 count.pencil_count++
			{
				count.pencil_count++;
				if(count.pencil_count < 5)
				{
					Beep_ms(250);
				}
				else
				{
					Beep_ms(125);
					Delay_ms(125);
					Beep_ms(125);
					Do_count1 = 0;	//回到Do_count1 = 0;可以重新校准
				}
			}
//			/* 移动云台舵机激光 */
//			if(KeyNum == KEY_UP_PRES)
//			{
////				PWM2_for_TASK1 -= PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[1] -= PWM_step_for_TASK1;
//				Servo2_SetPWM(rotation_red[1]);
//			}
//			else if(KeyNum == KEY_DOWN_PRES)
//			{
////				PWM2_for_TASK1 += PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[1] += PWM_step_for_TASK1;
//				Servo2_SetPWM(rotation_red[1]);
//			}
//			else if(KeyNum == KEY_LEFT_PRES)
//			{
////				PWM1_for_TASK1 += PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[0] += PWM_step_for_TASK1;
//				Servo1_SetPWM(rotation_red[0]);
//			}
//			else if(KeyNum == KEY_RIGHT_PRES)
//			{
////				PWM1_for_TASK1 -= PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[0] -= PWM_step_for_TASK1;
//				Servo1_SetPWM(rotation_red[0]);
//			}
		break;
	}
}

/* TASK2 复位 开环 */
void red_piont_reset_1(void)
{
	switch(Do_count2)
	{
		case 0:
			if(Usart2Num == 0xA2)
			{
				Do_count2++;
				
				Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[4][0], pencil_points_PWM[4][1]);
			}
		break;
		case 1:
			
		break;
	}
}
/* TASK2 复位 闭环 */
//void red_piont_reset_2(void)
//{
//	switch(Do_count2)
//	{
//		case 0:
//			if(KeyNum == KEY5_PRES)
//			{
//				Do_count2++;
//				
//				flag.reset_flag = 1;
//				flag.stop_flag = 0;
//				flag.pencil_line_flag = 0;
//				flag.pencil_line_succeed_flag = 0;
//				flag.black_line_flag = 0;
//				flag.black_line_succeed_flag = 0;
//				flag.reach_flag = 0;

//				K210_SendByte('2');
//				set_pid_target(&pid_x, pencil_points[4][0]);
//				set_pid_target(&pid_y, pencil_points[4][1]);
//			}

//		break;
//		case 1:
//			if(flag.reset_flag == 0)
//			{
//				Do_count2++;
//			}
//		break;
//		case 2:
//			
//		break;
//	}
//}

/* TASK3 巡铅笔线 开环 5PWM*/
//void pencil_line_1(void)
//{
//	switch(Do_count3)
//	{
//		case 0:	//按下KEY5，去左上角
//			if(KeyNum == KEY5_PRES)
//			{
//				Do_count3++;
//				
//				flag.wating_pencil_line_flag = 1;

//				Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[0][0], pencil_points_PWM[0][1]);
//			}
//		break;
//		case 1:	//去右上角
//			if(count.wating_pencil_line_count >= WATING_PENCIL_LINE_10MS)
//			{
//				Do_count3++;
//				
//				count.wating_pencil_line_count = 0;
//				
//				Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[1][0], pencil_points_PWM[1][1]);
//			}
//		break;
//		case 2:	//去右下角
//			if(count.wating_pencil_line_count >= WATING_PENCIL_LINE_10MS)
//			{
//				Do_count3++;
//				
//				count.wating_pencil_line_count = 0;
//				
//				Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[2][0], pencil_points_PWM[2][1]);
//			}
//		break;
//		case 3:	//去左下角
//			if(count.wating_pencil_line_count >= WATING_PENCIL_LINE_10MS)
//			{
//				Do_count3++;
//				
//				count.wating_pencil_line_count = 0;
//				
//				Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[3][0], pencil_points_PWM[3][1]);
//			}
//		break;
//		case 4:	//去左上角
//			if(count.wating_pencil_line_count >= WATING_PENCIL_LINE_10MS)
//			{
//				Do_count3++;
//				
//				flag.wating_pencil_line_flag = 0;
//				count.wating_pencil_line_count = 0;
//				
//				Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[0][0], pencil_points_PWM[0][1]);
//			}
//		break;
//		case 5:
//			
//		break;
//	}
//}

///* TASK3 巡铅笔线 闭环 */
//void pencil_line_2(void)
//{
//	switch(Do_count3)
//	{
//		case 0:	//计算插值点
//			if(KeyNum == KEY5_PRES)
//			{
//				Do_count3++;

//				flag.reset_flag = 0;
//				flag.stop_flag = 0;
//				flag.pencil_line_flag = 1;
//				flag.pencil_line_succeed_flag = 0;
//				flag.black_line_flag = 0;
//				flag.black_line_succeed_flag = 0;
//				flag.reach_flag = 0;
//				
//				Red_OFF();
//				Beep_OFF();
//				
//				USART3_SendByte('3');
//				
//				Path_Points_Get();
//				current_path_point = 0;  // 从第一个路径点开始
//			}
//		break;
//		case 1:
//			if(flag.pencil_line_succeed_flag == 1)
//			{
//				Do_count3++;
//			}
//		break;
//		case 2:
//			
//		break;
//	}
//}

///* TASK3 巡铅笔线 闭环 步进 终点反馈 5坐标*/
//void pencil_line_3(void)
//{
//	int16_t x_error;
//	int16_t y_error;
//	int16_t step_PWM = 5;
//	uint16_t piont_err_min = 2;
//	switch(Do_count3)
//	{
//		case 0:	//KEY5, 向K210发送'3', 开始TASK3
//			if(KeyNum == KEY5_PRES)
//			{
//				Do_count3++;
//				
//				flag.pencil_line_count_increase_flag = 1;
//				count.current_pencil_count = 0;
//				
//				USART3_SendByte('3');
//			}
//		break;
//		case 1:	//从原点pencil_points[4]到左上角pencil_points[0]
//			x_error = pencil_points[0][0] - pencil_points[4][0];
//			y_error = pencil_points[0][1] - pencil_points[4][1];
//			if(count.pencil_line_count % abs(x_error) == 0)	//x取余，变y
//			{

//				Servo2_SetPWM(rotation_red[1]);
//			}
//			if(count.pencil_line_count % abs(y_error) == 0)	//y取余，变x
//			{
//				if		(x_error > piont_err_min)	rotation_red[0] -= step_PWM;	//x方向，坐标与PWM负相关
//				else if	(x_error < -piont_err_min)	rotation_red[0] += step_PWM;
//				Servo1_SetPWM(rotation_red[0]);
//			}
//			if(USART3_GetRxFlag())	//红激光点进入K210设置的目标点的范围内
//			{
//				if(USART3_GetRxData() == 0xAA)
//				{
//					Do_count3++;
//					Beep_ms(250);
//				}
//			}
//		break;
//		case 2:	//从左上角顺时针绕一圈
//			x_error = pencil_points[(count.current_pencil_count + 1) % 4][0] - pencil_points[count.current_pencil_count][0];
//			y_error = pencil_points[(count.current_pencil_count + 1) % 4][1] - pencil_points[count.current_pencil_count][1];
//			if(count.pencil_line_count % abs(x_error) == 0)	//x取余，变y
//			{
//				if		(y_error > piont_err_min)	rotation_red[1] += step_PWM;	//y方向，坐标与PWM正相关
//				else if	(y_error < -piont_err_min)	rotation_red[1] -= step_PWM;
//				Servo2_SetPWM(rotation_red[1]);
//			}
//			if(count.pencil_line_count % abs(y_error) == 0)	//y取余，变x
//			{
//				if		(x_error > piont_err_min)	rotation_red[0] -= step_PWM;	//x方向，坐标与PWM负相关
//				else if	(x_error < -piont_err_min)	rotation_red[0] += step_PWM;
//				Servo1_SetPWM(rotation_red[0]);
//			}
//			if(USART3_GetRxFlag())
//			{
//				if(USART3_GetRxData() == 0xAA)
//				{
//					count.current_pencil_count++;
//					Beep_ms(250);
//					//0->1:0 1->2:1 2->3:2 3->0:3 到0:收到K210信息就变成4，代表完成TASK3
//					if(count.current_pencil_count == 4)
//					{
//						Do_count3++;
//						
//						Delay_ms(250);
//						Beep_ms(250);
//					}
//				}
//			}
//		break;
//		case 3:
//			flag.pencil_line_count_increase_flag = 0;
//			count.current_pencil_count = 0;

//		break;
//	}
//	
//}

///* TASK3 巡铅笔线 闭环 步进 终点反馈 5PWM*/
//void pencil_line_4(void)
//{
//	int16_t x_error;
//	int16_t y_error;
//	int16_t step_PWM = 5;
//	uint16_t PWM_err_min = 2;
//	switch(Do_count3)
//	{
//		case 0:	//KEY5, 向K210发送'3', 开始TASK3
//			if(KeyNum == KEY5_PRES)
//			{
//				Do_count3++;
//				
//				flag.pencil_line_count_increase_flag = 1;
//				count.current_pencil_count = 0;
//				
//				USART3_SendByte('3');
//			}
//		break;
//		case 1:	//从原点rotation_red到左上角pencil_points[0]
//			x_error = pencil_points_PWM[0][0] - rotation_red[0];
//			y_error = pencil_points_PWM[0][1] - rotation_red[1];
//			if(count.pencil_line_count % abs(x_error) == 0)	//x取余，变y
//			{
//				if		(y_error > PWM_err_min)		rotation_red[1] += step_PWM;	//以PWM为误差参考,都是正相关
//				else if	(y_error < -PWM_err_min)	rotation_red[1] -= step_PWM;
//				Servo2_SetPWM(rotation_red[1]);
//			}
//			if(count.pencil_line_count % abs(y_error) == 0)	//y取余，变x
//			{
//				if		(x_error > PWM_err_min)		rotation_red[0] += step_PWM;
//				else if	(x_error < -PWM_err_min)	rotation_red[0] -= step_PWM;
//				Servo1_SetPWM(rotation_red[0]);
//			}
//			if(USART3_GetRxFlag())	//红激光点进入K210设置的目标点的范围内
//			{
//				if(USART3_GetRxData() == 0xAA)
//				{
//					Do_count3++;
//					Beep_ms(250);
//				}
//			}
//		break;
//		case 2:	//从左上角顺时针绕一圈
//			x_error = pencil_points_PWM[(count.current_pencil_count + 1) % 4][0] - pencil_points_PWM[count.current_pencil_count][0];
//			y_error = pencil_points_PWM[(count.current_pencil_count + 1) % 4][1] - pencil_points_PWM[count.current_pencil_count][1];
//			if(count.pencil_line_count % abs(x_error) == 0)	//x取余，变y
//			{
//				if		(y_error > PWM_err_min)		rotation_red[1] += step_PWM;	//y方向，坐标与PWM正相关
//				else if	(y_error < -PWM_err_min)	rotation_red[1] -= step_PWM;
//				Servo2_SetPWM(rotation_red[1]);
//			}
//			if(count.pencil_line_count % abs(y_error) == 0)	//y取余，变x
//			{
//				if		(x_error > PWM_err_min)		rotation_red[0] -= step_PWM;	//x方向，坐标与PWM负相关
//				else if	(x_error < -PWM_err_min)	rotation_red[0] += step_PWM;
//				Servo1_SetPWM(rotation_red[0]);
//			}
//			if(USART3_GetRxFlag())
//			{
//				if(USART3_GetRxData() == 0xAA)
//				{
//					count.current_pencil_count++;
//					Beep_ms(250);
//					//0->1:0 1->2:1 2->3:2 3->0:3 到0:收到K210信息就变成4，代表完成TASK3
//					if(count.current_pencil_count == 4)
//					{
//						Do_count3++;
//						
//						Delay_ms(250);
//						Beep_ms(250);
//					}
//				}
//			}
//		break;
//		case 3:
//			flag.pencil_line_count_increase_flag = 1;
//			count.current_pencil_count = 0;

//		break;
//	}
//}

/* TASK3 巡铅笔线 开环 5PWM 累加步进*/
void pencil_line_5(void)
{
	uint16_t step_PWM = 2;
	uint16_t wating_time = 2;

	switch(Do_count3)
	{
		case 0:	//按下KEY5，去左上角，直达
//			if(KeyNum == KEY5_PRES)
			if(1)
			{
				flag.wating_pencil_line_flag = 1;

				Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[0][0], pencil_points_PWM[0][1]);
			}
			if(count.wating_pencil_line_count >= 500)	//等足够长时间
			{
				Do_count3++;
				
				count.wating_pencil_line_count = 0;
				Beep_ms(250);
			}
		break;
		case 1:	//去右上角	步进 只改xPWM
			if(count.wating_pencil_line_count >= wating_time)	//500ms步进step_PWM（30）
			{
				count.wating_pencil_line_count = 0;
				
				if(abs(abs(pencil_points_PWM[1][0] - pencil_points_PWM[0][0]) / 2 - abs(pencil_points_PWM[1][0] - rotation_red[0])) <=  step_PWM)
				{
					Servo_All_SetPWM_ChangeRotationRed(rotation_red[0], pencil_points_PWM[1][1]);
				}
				if(abs(rotation_red[0] - pencil_points_PWM[1][0]) > step_PWM)
				{
					rotation_red[0] -= step_PWM;	//右边PWM小，这里是-
					Servo_All_SetPWM(rotation_red[0], rotation_red[1]);
				}
				else
				{
					//如果接近目标点 直达 刚好弥补只改一个方向PWM时，另一个方向的误差
					Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[1][0], pencil_points_PWM[1][1]);
					Do_count3++;
//					Beep_ms(250);
				}
			}
		break;
		case 2:	//去右下角	步进
			if(count.wating_pencil_line_count >= wating_time)
			{
				count.wating_pencil_line_count = 0;
				
				if(abs(abs(pencil_points_PWM[2][1] - pencil_points_PWM[1][1]) / 2 - abs(pencil_points_PWM[2][1] - rotation_red[1])) <=  step_PWM)
				{
					Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[2][0], rotation_red[1]);
				}
				
				if(abs(rotation_red[1] - pencil_points_PWM[2][1]) > step_PWM)
				{
					rotation_red[1] += step_PWM;	//下边PWM大，这里是+
					Servo_All_SetPWM(rotation_red[0], rotation_red[1]);
				}
				else
				{
					Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[2][0], pencil_points_PWM[2][1]);
					Do_count3++;
//					Beep_ms(250);
				}
			}
		break;
		case 3:	//去左下角	步进
			if(count.wating_pencil_line_count >= wating_time)
			{
				count.wating_pencil_line_count = 0;
				
				if(abs(abs(pencil_points_PWM[3][0] - pencil_points_PWM[2][0]) / 2 - abs(pencil_points_PWM[3][0] - rotation_red[0])) <=  step_PWM)
				{
					Servo_All_SetPWM_ChangeRotationRed(rotation_red[0], pencil_points_PWM[3][1]);
				}
				
				if(abs(rotation_red[0] - pencil_points_PWM[3][1]) > step_PWM)
				{
					rotation_red[0] += step_PWM;	//左边PWM大，这里是+
					Servo_All_SetPWM(rotation_red[0], rotation_red[1]);
				}
				else
				{
					Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[3][0], pencil_points_PWM[3][1]);
					Do_count3++;
//					Beep_ms(250);
				}
			}
		break;
		case 4:	//去左上角	步进
			if(count.wating_pencil_line_count >= wating_time)
			{
				count.wating_pencil_line_count = 0;
				
				if(abs(abs(pencil_points_PWM[0][1] - pencil_points_PWM[3][1]) / 2 - abs(pencil_points_PWM[0][1] - rotation_red[1])) <=  step_PWM)
				{
					Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[0][0], rotation_red[1]);
				}
				
				if(abs(rotation_red[1] - pencil_points_PWM[0][1]) > step_PWM)
				{
					rotation_red[1] -= step_PWM;	//上边PWM小，这里是-
					Servo_All_SetPWM(rotation_red[0], rotation_red[1]);
				}
				else
				{
					Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[0][0], pencil_points_PWM[0][1]);
					
					Do_count3 = 0;
					
//					Beep_ms(250);
					flag.wating_pencil_line_flag = 0;
					count.wating_pencil_line_count = 0;
				}
			}
		break;
	}
}

/* TASK4 获取黑框角点 记录PWM */
void black_piont_get_guding_1(void)
{
	if(KeyNum == KEY3_PRES)	//KEY3 切换PWM步进大小
	{
		PWM_step_for_TASK1 = (PWM_step_for_TASK1 == PWM_STEP_LARGE ? PWM_STEP_SMALL : PWM_STEP_LARGE);
	}
	switch(Do_count4)
	{
		case 0:	//KEY5 开始移动
			if(KeyNum == KEY5_PRES)
			{
				Do_count4++;
			}
		break;
		case 1:
//			if(KeyNum == KEY_UP_PRES)
//			{
////				PWM2_for_TASK1 -= PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[1] -= PWM_step_for_TASK1;
//				Servo2_SetPWM(rotation_red[1]);
//			}
//			else if(KeyNum == KEY_DOWN_PRES)
//			{
////				PWM2_for_TASK1 += PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[1] += PWM_step_for_TASK1;
//				Servo2_SetPWM(rotation_red[1]);
//			}
//			else if(KeyNum == KEY_LEFT_PRES)
//			{
////				PWM1_for_TASK1 += PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[0] += PWM_step_for_TASK1;
//				Servo1_SetPWM(rotation_red[0]);
//			}
//			else if(KeyNum == KEY_RIGHT_PRES)
//			{
////				PWM1_for_TASK1 -= PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[0] -= PWM_step_for_TASK1;
//				Servo1_SetPWM(rotation_red[0]);
//			}
			
			if(KeyNum == KEY5_PRES)
			{
				Beep_ms(125);
				black_points_PWM_guding[count.black_count_3][0] = rotation_red[0];
				black_points_PWM_guding[count.black_count_3][1] = rotation_red[1];
				
				count.black_count_3++;
				if(count.black_count_3 >= 4)	//这里是四个点 所以时4 不是之前的5
				{
					Delay_ms(125);
					Beep_ms(125);
					count.black_count_3 = 0;
					Do_count4 = 0;
				}
			}
		break;
	}
}

/* TASK5 巡黑框 */
void black_line_guding_1(void)
{
	uint16_t step_PWM = 2;
	uint16_t wating_time = 5;

	switch(Do_count5)
	{
		case 0:	//按下KEY5，去左上角，直达
			if(KeyNum == KEY5_PRES)
			{
				flag.wating_pencil_line_flag = 1;

				Servo_All_SetPWM_ChangeRotationRed(black_points_PWM_guding[0][0], black_points_PWM_guding[0][1]);
			}
			if(count.wating_pencil_line_count >= 500)	//等足够长时间
			{
				Do_count5++;
				
				count.wating_pencil_line_count = 0;
//				Beep_ms(250);
			}
		break;
		case 1:	//去右上角	步进 只改xPWM
			if(count.wating_pencil_line_count >= wating_time)	//500ms步进step_PWM（30）
			{
				count.wating_pencil_line_count = 0;
				
				if(abs(abs(black_points_PWM_guding[1][0] - black_points_PWM_guding[0][0]) / 2 - abs(black_points_PWM_guding[1][0] - rotation_red[0])) <=  step_PWM)
				{
					Servo_All_SetPWM_ChangeRotationRed(rotation_red[0], black_points_PWM_guding[1][1]);
				}
				if(abs(rotation_red[0] - black_points_PWM_guding[1][0]) > step_PWM)
				{
					rotation_red[0] -= step_PWM;	//右边PWM小，这里是-
					Servo_All_SetPWM(rotation_red[0], rotation_red[1]);
				}
				else
				{
					//如果接近目标点 直达 刚好弥补只改一个方向PWM时，另一个方向的误差
					Servo_All_SetPWM_ChangeRotationRed(black_points_PWM_guding[1][0], black_points_PWM_guding[1][1]);
					Do_count5++;
//					Beep_ms(250);
				}
			}
		break;
		case 2:	//去右下角	步进
			if(count.wating_pencil_line_count >= wating_time)
			{
				count.wating_pencil_line_count = 0;
				
				if(abs(abs(black_points_PWM_guding[2][1] - black_points_PWM_guding[1][1]) / 2 - abs(black_points_PWM_guding[2][1] - rotation_red[1])) <=  step_PWM)
				{
					Servo_All_SetPWM_ChangeRotationRed(black_points_PWM_guding[2][0], rotation_red[1]);
				}
				
				if(abs(rotation_red[1] - black_points_PWM_guding[2][1]) > step_PWM)
				{
					rotation_red[1] += step_PWM;	//下边PWM大，这里是+
					Servo_All_SetPWM(rotation_red[0], rotation_red[1]);
				}
				else
				{
					Servo_All_SetPWM_ChangeRotationRed(black_points_PWM_guding[2][0], black_points_PWM_guding[2][1]);
					Do_count5++;
//					Beep_ms(250);
				}
			}
		break;
		case 3:	//去左下角	步进
			if(count.wating_pencil_line_count >= wating_time)
			{
				count.wating_pencil_line_count = 0;
				
				if(abs(abs(black_points_PWM_guding[3][0] - black_points_PWM_guding[2][0]) / 2 - abs(black_points_PWM_guding[3][0] - rotation_red[0])) <=  step_PWM)
				{
					Servo_All_SetPWM_ChangeRotationRed(rotation_red[0], black_points_PWM_guding[3][1]);
				}
				
				if(abs(rotation_red[0] - black_points_PWM_guding[3][1]) > step_PWM)
				{
					rotation_red[0] += step_PWM;	//左边PWM大，这里是+
					Servo_All_SetPWM(rotation_red[0], rotation_red[1]);
				}
				else
				{
					Servo_All_SetPWM_ChangeRotationRed(black_points_PWM_guding[3][0], black_points_PWM_guding[3][1]);
					Do_count5++;
//					Beep_ms(250);
				}
			}
		break;
		case 4:	//去左上角	步进
			if(count.wating_pencil_line_count >= wating_time)
			{
				count.wating_pencil_line_count = 0;
				
				if(abs(abs(black_points_PWM_guding[0][1] - black_points_PWM_guding[3][1]) / 2 - abs(black_points_PWM_guding[0][1] - rotation_red[1])) <=  step_PWM)
				{
					Servo_All_SetPWM_ChangeRotationRed(black_points_PWM_guding[0][0], rotation_red[1]);
				}
				
				if(abs(rotation_red[1] - black_points_PWM_guding[0][1]) > step_PWM)
				{
					rotation_red[1] -= step_PWM;	//上边PWM小，这里是-
					Servo_All_SetPWM(rotation_red[0], rotation_red[1]);
				}
				else
				{
					Servo_All_SetPWM_ChangeRotationRed(black_points_PWM_guding[0][0], black_points_PWM_guding[0][1]);
					
					Do_count5 = 0;
					
//					Beep_ms(250);
					flag.wating_pencil_line_flag = 0;
					count.wating_pencil_line_count = 0;
				}
			}
		break;
	}
}

void black_piont_get_suiji_1(void)
{
	switch(Do_count6)
	{
		case 0:	//发送'6'，开始接收
			if(KeyNum == KEY5_PRES)
			{
				Do_count6++;
				flag.black_pionts_get_flag = 1;	//等待接收
				K210_SendByte('6');
			}
		break;
		case 1:	//接收到，计算路径点
			if(flag.black_pionts_get_flag == 2)	//接收到
			{
				Do_count6++;
				
				black_path_points_get();
				Beep_ms(250);
			}
		break;
		case 2:	//计算完
//			if(flag.black_path_pionts_get_succeed_flag == 1 && KeyNum == KEY5_PRES)
			if(flag.black_path_pionts_get_succeed_flag == 1)
			{
				Delay_ms(500);
				Beep_ms(250);
				
				TASK = 7; 
				Do_count7 = 0;
				flag.black_path_pionts_get_succeed_flag = 0;
			}
		break;
	}
}

/* TASK7 巡黑框 闭环 */
void black_line_1(void)
{
	switch(Do_count7)
	{
		case 0:	//发送‘7’，开始巡线
//			if(1)
			if(KeyNum == KEY5_PRES)
			{
				Do_count7++;
				
				flag.counting_flag = 1;	//开始计时
				count.counting_time = 0;

				flag.reset_flag = 0;
				flag.stop_flag = 0;
				flag.pencil_line_flag = 0;
				flag.pencil_line_succeed_flag = 0;
				flag.black_line_flag = 1;	//走矩形框
				flag.black_line_succeed_flag = 0;
				flag.reach_flag = 0;
				
				Red_OFF();
				Beep_OFF();
				
				K210_SendByte('7');
				black_current_path_point = 0;  // 从第一个路径点开始
			}
		break;
		case 1:
			if(flag.black_line_succeed_flag == 1)
			{
				flag.counting_flag =0;	//计时结束
				flag.black_line_flag = 0;
				Do_count7 = 0;
			}
		break;
		case 2:
			
		break;
	}
}
/* TASK8 复位追踪  */
void reset_zhuizong_1(void)
{
	switch(Do_count8)
	{
		case 0:	//发送‘8’，开始追踪
//			if(1)
			if(KeyNum == KEY5_PRES)
			{
				Do_count8++;
				
				flag.counting_flag = 1;	//开始计时
				count.counting_time = 0;

				flag.reset_flag = 0;
				flag.stop_flag = 0;
				flag.pencil_line_flag = 0;
				flag.pencil_line_succeed_flag = 0;
				flag.black_line_flag = 0;	
				flag.black_line_succeed_flag = 0;
				flag.reach_flag = 0;
				flag.zhuizong_flag = 0;	
				flag.zhuizong_succeed_flag = 0;
				flag.green_reset_flag = 1;	//复位追踪
				flag.green_reset_succeed_flag = 0;
				
				Red_OFF();
				Beep_OFF();
				
				K210_SendByte('8');
			}
		break;
		case 1:
			if(flag.green_reset_succeed_flag == 1)
			{
				flag.counting_flag =0;	//计时结束
				flag.green_reset_flag = 0;
				Do_count8 = 0;
//				K210_SendByte('!');
			}
		break;
		case 2:
			
		break;
	}
}
/* TASK9 运动追踪  */
void yundong_zhuizong_1(void)
{
	switch(Do_count9)
	{
		case 0:	//发送‘9’，开始追踪
//			if(1)
			if(KeyNum == KEY5_PRES)
			{
				Do_count9++;
				
				flag.counting_flag = 1;	//开始计时
				count.counting_time = 0;

				flag.reset_flag = 0;
				flag.stop_flag = 0;
				flag.pencil_line_flag = 0;
				flag.pencil_line_succeed_flag = 0;
				flag.black_line_flag = 1;	//走矩形框
				flag.black_line_succeed_flag = 0;
				flag.reach_flag = 0;
				flag.zhuizong_flag = 1;	//追踪
				flag.zhuizong_succeed_flag = 0;
				flag.green_reset_flag = 0;
				flag.green_reset_flag = 0;
				flag.green_reset_succeed_flag = 0;
				
				Red_OFF();
				Beep_OFF();
				
				K210_SendByte('9');
			}
		break;
		case 1:
			//如果 红激光走完 且 绿激光追上
			if(flag.black_line_succeed_flag && flag.zhuizong_succeed_flag)
			{
				flag.counting_flag =0;	//计时结束
				
				flag.black_line_flag = 0;
				flag.zhuizong_flag = 0;
				Do_count9 = 0;
				K210_SendByte('!');
			}
		break;
		case 2:
			
		break;
	}
}

/* TASK10 目标点实际点  */
void tar_cur_pionts(void)
{
	switch(Do_count10)
	{
		case 0:	//发送‘A’，开始追踪
//			if(1)
			if(KeyNum == KEY3_PRES)
			{
				Do_count10++;
				
				flag.counting_flag = 1;	//开始计时
				count.counting_time = 0;

				flag.reset_flag = 0;
				flag.stop_flag = 0;
				flag.pencil_line_flag = 0;
				flag.pencil_line_succeed_flag = 0;
				flag.black_line_flag = 0;	//走矩形框
				flag.black_line_succeed_flag = 0;
				flag.reach_flag = 0;
				flag.zhuizong_flag = 0;	//追踪
				flag.zhuizong_succeed_flag = 0;
				flag.green_reset_flag = 0;
				flag.green_reset_flag = 0;
				flag.green_reset_succeed_flag = 0;
				
				flag.tar_cur_flag = 1;
				flag.tar_cur_succeed_flag = 0;
				
				Beep_ms_TIM(500);
//				Red_OFF();
//				Beep_OFF();
				
				K210_SendByte(0x0A);
			}
		break;
		case 1:
			//如果 红激光走完 且 绿激光追上
			if(flag.cur_catch_tar_flag == 1)
			{
				flag.cur_catch_tar_flag = 0;
				flag.counting_flag =0;	//计时结束
				
				flag.tar_cur_flag = 0;
				flag.tar_cur_succeed_flag = 1;
				Do_count10 = 0;
				
				Beep_ms_TIM(1000);
				K210_SendByte('Y');
			}
		break;
		case 2:
			
		break;
	}
}
void open_line_task(void)
{
	
}
