/**
 * @file physics.h
 * @brief 物理引擎模块 - 处理斜抛运动的物理计算
 *
 * 核心算法说明：
 *   核心公式：X = X0 + Vx * t + 1/2 * g * t^2
 *             Y = Y0 + Vy * t
 *
 *   核心思想：以连续打印为前提，反推每移动一个单位所需的时间，
 *            从而实现连续的抛物线轨迹渲染。
 *
 *   解决方法：分别计算X增加一个单位时Y的位移s1，和Y增加一个单位时X的位移s2，
 *            取较短者作为下一步移动的基准，保证轨迹在屏幕上连续不断裂。
 */
#ifndef PHYSICS_H
#define PHYSICS_H

/* 圆周率近似值 */
#define PI 3.14

/* 重力加速度（非标准正坐标系，向下为正） */
#define GRAVITY 10.0

/**
 * @brief 物理状态结构体 - 保存球体的运动状态
 */
typedef struct {
    double x;    /* 精确X坐标（垂直方向） */
    double y;    /* 精确Y坐标（水平方向） */
    double vx;   /* X方向速度分量（垂直） */
    double vy;   /* Y方向速度分量（水平） */
    int old_x;   /* 整型X坐标，用于屏幕打印 */
    int old_y;   /* 整型Y坐标，用于屏幕打印 */
} PhysicsState;

/**
 * @brief 初始化物理状态
 * @param state  指向物理状态的指针
 * @param start_x 起始X坐标
 * @param start_y 起始Y坐标
 * @param velocity 初速度大小
 * @param angle   发射角度（度），自右向上转角，0为水平，90为竖直上抛
 */
void physics_init(PhysicsState *state, double start_x, double start_y,
                  double velocity, double angle);

/**
 * @brief 执行一步物理计算，更新位置和速度
 * @param state 指向物理状态的指针
 *
 * 算法说明：
 *   分别计算X移动一个单位和Y移动一个单位所需的时间t1和t2，
 *   取位���较短的方向优先移动，确保轨迹连续。
 */
void physics_step(PhysicsState *state);

/**
 * @brief 处理边界碰撞反弹
 * @param state  指向物理状态的指针
 * @param width  地图高度（行数）
 * @param length 地图宽度（列数）
 * @return 1表示发生了碰撞，0表示未碰撞
 */
int physics_check_boundary(PhysicsState *state, int width, int length);

#endif /* PHYSICS_H */
